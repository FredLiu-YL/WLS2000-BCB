//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RecipeForm.h"
#include "frmMain.h"
#include "CassetteIDForm.h"
#include "InspectionForm.h"
#include "RecipeSetForm.h"

#include "LogoFrm.h"

extern int MousePosX;
extern int MousePosY;
void __fastcall Beep(int mstime);
void __fastcall SetRedLamp(bool mode);
void __fastcall Beep(int mstime);

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDControl"
#pragma link "LMDCustomBevelPanel"
#pragma link "LMDCustomControl"
#pragma link "LMDCustomPanel"
#pragma link "LMDCustomPanelFill"
#pragma link "LMDCustomParentPanel"
#pragma link "LMDCustomSheetControl"
#pragma link "LMDPageControl"
#pragma link "LMDCustomTrackBar"
#pragma link "LMDTrackBar"
#pragma link "LMDButton"
#pragma link "LMDCustomButton"
#pragma resource "*.dfm"
TfrmRecipe *frmRecipe;
//---------------------------------------------------------------------------
__fastcall TfrmRecipe::TfrmRecipe(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipe::FormShow(TObject *Sender)
{
static boolfirst = true;

   if(boolfirst == true) {
      sgRecipeSelection->ColWidths[0] = 280;
      sgRecipeSelection->ColWidths[1] = 150;

      sgRecipeSelection->Cells[0][0] = " Recipe ID";
      sgRecipeSelection->Cells[1][0] = "Modified";

      sgRecipeSelection->RowCount = 2;

      // Recipe ID List
      btnBatchSelectClick(this);
   }
   boolfirst = false;

   frmRecipe->Left = 3;
   frmRecipe->Top = 3;
   frmRecipe->Width = 1911;
   frmRecipe->Height = 1040;
   frnMain->pnlCurrentOperation->Caption = "Recipe";

   btnCassetteLoad->Enabled = frnMain->btnCassetteLoad->Enabled;
   btnCassette2Load->Enabled = frnMain->btnCassette2Load->Enabled;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::sgRecipeSelectionDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   /*
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ACol == 1) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }
   */

 /*
 class TMyGridBase : public TCustomGrid
   {
   public:
       __property Row;
   };
   class TMyGrid : public TCustomDBGrid
   {
   public:
       __property DataLink;
   };

   TMyGrid *MyGrid = (TMyGrid*)Sender;
   TMyGridBase *MyGridBase = (TMyGridBase*)Sender;
   TDBGrid *Grid = (TDBGrid*)Sender;

   if(MyGrid->DataLink->ActiveRecord == MyGridBase->Row-1) {
      Grid->Canvas->Font->Color = clWhite;
      Grid->Canvas->Brush->Color = clHighlight;
   }
   else {
      if(Column->Field->DataSet->RecNo%2) {
        if(Grid->Canvas->Brush->Color == clWindow) {
           Grid->Canvas->Brush->Color=clInfoBk;
           Grid->Canvas->FillRect(Rect);
        }
      }
      DrawText(Grid->Canvas->Handle, Column->Field->Text.c_str(),-1,(RECT*)&Rect,DT_SINGLELINE | DT_VCENTER |DT_CENTER);
   }

   Grid->DefaultDrawColumnCell(Rect,DataCol,Column,State);}
  */

// 區分大小來源
TStringGrid *stringgrid;
stringgrid = (TStringGrid*)Sender;

   if(ARow != 0) {
      // 奇偶數不同顏色
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;       // 米色
      else
          ((TStringGrid*)Sender)->Canvas->Brush->Color = clSilver;
   }

   ((TStringGrid*)Sender)->Canvas->Font->Color = clBlack;
   ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
   if(ACol == 0)
      DrawText(((TStringGrid*)Sender)->Canvas->Handle,((TStringGrid*)Sender)->Cells[ACol][ARow].c_str(),-1,(RECT*)&Rect,DT_SINGLELINE|DT_VCENTER|DT_LEFT);
   else
      DrawText(((TStringGrid*)Sender)->Canvas->Handle,((TStringGrid*)Sender)->Cells[ACol][ARow].c_str(),-1,(RECT*)&Rect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);

}
//---------------------------------------------------------------------------
//caNone  The form is not allowed to close, so nothing happens.
//caHide  The form is not closed, but just hidden. Your application can still access a hidden form.
//caFree  The form is closed and all allocated memory for the form is freed.
//caMinimize  The form is minimized, rather than closed. This is the default action for MDI child forms.
void __fastcall TfrmRecipe::FormClose(TObject *Sender,
      TCloseAction &Action)
{

   // Main
   frnMain->SetST(ST_RECIPE,false);
   frnMain->pnlCurrentOperation->Caption = "Main";
   frnMain->WriteSystemLog("frmRecipe Close.");

   Action = caHide;

   // 2023 2 28 - chc frmLogo Visible
   if(frmRecipeSet->Visible == false && frmInspection->Visible == false && frnMain->pnlMaintenance->Visible == false)
      frmLogo->Visible = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnCassetteLoadClick(TObject *Sender)
{
bool boolunload = false;
bool blp2;
bool boolmap = true;

   // 2023 2 16 - chc Check System Error
   if(frnMain->CheckSystemError() == true) {
      pnlMessage->Caption = "Error! Cannot do.";
      return;
   }

   // 2023 2 3 - chc LP1
   if(btnCassette2Load->Caption == "UnLoad") {
      pnlMessage->Caption = "CST2 Working!";
      return;
   }
   blp2 = btnCassette2Load->Enabled;
   pnlCassette1ID->Caption = "";
   pnlCassetteID->Caption = "";

   // 2023 3 20 - chc Clear CST Status
   // 2023 4 17 - chc UnLoad才要清
   //pnlLoadCancelClick(this);

   if(btnCassetteLoad->Caption == "UnLoad") {
      pnlMessage->Caption = "LP1 Unloading...";
      boolunload = true;
   }
   else {
      pnlMessage->Caption = "LP1 Loading...";

      // 2023 4 17 - chc Load才要清
      frnMain->WriteSystemLog("Clear CST Status");
      pnlLoadCancelClick(this);

   }
   pnlRecipeSelect->Enabled = false;
   btnCassette2Load->Enabled = false;
   pnlPortNo->Caption = "1";
   pnlCassetteLoad->Color = clSilver;
   btnCassetteLoad->Enabled = false;

   // 2023 3 29 - chc Set CST
   frnMain->rgCST->ItemIndex = LOADPORT_1;
   frnMain->rgSECSPortNo->ItemIndex = LOADPORT_1;

   // Read RFID
   // 2023 3 29 - chc Load才要讀碼
   if(btnCassetteLoad->Caption == "Load") {

      frnMain->rgRFIDLoadPort->ItemIndex = LOADPORT_1;
      frnMain->pnlCSTCode->Caption = "UNKNOWN-";
      frnMain->pnlSECSCSTID->Caption = frnMain->pnlCSTCode->Caption;
      frnMain->btnRFIDReadClick(this);
   }

   // 2023 3 26 - chc SECS: RFID Read
   // 2023 3 29 - chc 若有SECS要等MAPP CMD
   //frnMain->rgPortStatus->ItemIndex = PORT_STATUS_CSTREAD;
   //frnMain->WriteSystemLog("SECS: RFID1 Read " + frnMain->pnlCSTCode->Caption);
   //frnMain->SetToSECS(SECS_CMD_PTST_1 - SECS_CMD_STAT_1);

   //---------------------------------------------------------------------
   // 2023 3 29 - chc 若有SECS要等MAPP CMD
   // 有連線時由SECS MAP CMD(3)觸發LOAD
   frnMain->WaitTime(1000);
   int timeout,cnt;
   // 是否連線?
   // 2023 3 29 - chc Load才要讀碼
   //if(frnMain->pnlSECSConnect->Color == clLime && frnMain->cbOffLine->Checked == false) {
   if(frnMain->pnlSECSConnect->Color == clLime && frnMain->cbOffLine->Checked == false && btnCassetteLoad->Caption == "Load") {

      // 連線且Recmote才要做
      frnMain->rgPortStatus->ItemIndex = PORT_STATUS_CSTREAD;
      frnMain->WriteSystemLog("SECS: PTST-CSTREAD " + frnMain->pnlCSTCode->Caption);
      frnMain->SetToSECS(SECS_CMD_PTST_1 - SECS_CMD_STAT_1);

      // 2023 3 31 - chc Cancel
      boolLP1Cancel = false;

      // 45秒
      timeout = 45;
      // Wait
      cnt = 0;
      while(1) {

         // 2023 3 31 - chc Cancel
         if(boolLP1Cancel == true) {
            pnlMessage->Caption = "LP1 Cancel";
            frnMain->WriteSystemLog(pnlMessage->Caption);
            break;
         }

         //frnMain->WriteSystemLog("SECS: LP1 Load Wait...");
         frnMain->WaitTime(1000);
         cnt++;
         if(cnt > timeout) {
            pnlCassetteLoad->Color = clRed;
            pnlMessage->Caption = "LP1 Load Timeout失敗!";
            frnMain->WriteSystemLog(pnlMessage->Caption);
            break;
         }
         if(frnMain->pnlCassetteLoad->Color == clLime) {
            pnlMessage->Caption = "LP1 Load完成.";
            frnMain->WriteSystemLog(pnlMessage->Caption);
            // 由Idle/Run/Down改成Init/Idle/Setup/Ready/Execution/Pause
            // Load後為為EXECUTING
            frnMain->SendSECSStatus(EQ_STATUS_EXECUTING);
            // 禁止變更LoadPort
            frnMain->rgLoadPortSelect->Enabled = false;
            // CSTID
            CassetteIDConfirm = -1;
            frmCassetteID->pnlPortNo->Caption = "1";
            //frmCassetteID->edCassetteID->Text = "OLYMPUS-Carrier";
            pnlCassette1ID->Caption = frnMain->pnlCSTCode->Caption;
            frmCassetteID->edCassetteID->Text = pnlCassette1ID->Caption;
            // Set to SECS
            frnMain->pnlSECSCSTID->Caption = pnlCassette1ID->Caption;
            frmCassetteID->ShowModal();

            // 2023 3 31 - chc 不做
            /*
            // ok
            if(frmRecipe->CassetteIDConfirm == 1) {
                 pnlCassette1ID->Caption = frmCassetteID->edCassetteID->Text;
                 pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;
            }
            else {
               pnlCassette1ID->Caption = "";
               pnlPort->Caption = "";
            }
            */
            
            break;
         }
         else if(frnMain->pnlCassetteLoad->Color == clRed) {
            pnlCassetteLoad->Color = clRed;
            pnlMessage->Caption = "LP1 Load失敗!";
            frnMain->WriteSystemLog(pnlMessage->Caption);
            break;
         }
      }
   }
   else {
      if(frnMain->pnlSECSConnect->Color != clLime)
         frnMain->WriteSystemLog("SECS未連線: 直接做Load");
      if(frnMain->cbOffLine->Checked == true)
         frnMain->WriteSystemLog("在Local狀態下: 直接做Load");
   //---------------------------------------------------------------------

      frnMain->btnCassetteLoadClick(this);
      if(pnlCassetteLoad->Color == clLime && boolunload == false) {
         pnlMessage->Caption = "LP1 Loaded";
         CassetteIDConfirm = -1;
         frmCassetteID->pnlPortNo->Caption = "1";

         //frmCassetteID->edCassetteID->Text = "OLYMPUS-Carrier";
         pnlCassette1ID->Caption = frnMain->pnlCSTCode->Caption;
         frmCassetteID->edCassetteID->Text = pnlCassette1ID->Caption;

         // 2023 3 28 - chc Set to SECS
         frnMain->pnlSECSCSTID->Caption = pnlCassette1ID->Caption;

         // 2023 4 12 - chc Remote不用問
         if(frnMain->cbOffLine->Checked == false) {
            frmRecipe->pnlCassetteID->Caption = frmCassetteID->edCassetteID->Text;
            frmRecipe->pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;
         }
         else

            frmCassetteID->ShowModal();

         // ok
         // 2023 3 31 - chc 不做
         /*
         if(frmRecipe->CassetteIDConfirm == 1) {
            pnlCassetteID->Caption = frmCassetteID->edCassetteID->Text;
            pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;
         }
         else {
            pnlCassetteID->Caption = "";
            pnlPort->Caption = "";
         }
         */

         // 2023 2 3 - chc Loaded
         //blp2 = false;

      }

      else if(pnlCassetteLoad->Color == clLime && boolunload == true) {
         pnlMessage->Caption = "LP1 Unloaded";
      }

      else if(pnlCassetteLoad->Color != clLime) {
         if(boolunload == true)
            pnlMessage->Caption = "LP1 Unload Fail!";
         else
            pnlMessage->Caption = "LP1 Load Fail!";
         boolmap = false;
      }
   }

   // 2023 2 12 - chc 確認是否MappingOk?
   TPanel *panel;
   for(int i=0 ; i<25 ; i++) {
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      if(panel->Color != clLime && panel->Color != clSilver) {
         boolmap = false;
         break;
      }
   }
   if(boolmap == false) {
      pnlCassetteLoad->Color = clRed;
      pnlLoadOK->Enabled = false;
      pnlLoadOK->Font->Color = clGray;
   }
   else {
      pnlLoadOK->Enabled = true;
      pnlLoadOK->Font->Color = clBlack;

      pnlCassetteLoad->Color = clLime;
   }
   btnCassetteLoad->Enabled = true;
   pnlStartInspection->Enabled = false;
   pnlEndInspection->Enabled = false;
   pnlStartInspection->Color = clGreen;
   pnlEndInspection->Color = clMaroon;
   pnlStartInspection->Font->Color = clGray;
   pnlEndInspection->Font->Color = clGray;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;

   // 2023 2 3 - chc LP1
   //btnCassette2Load->Enabled = true;
   btnCassette2Load->Enabled = blp2;

   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;
   pnlRecipeSelect->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnCassette2LoadClick(TObject *Sender)
{
bool boolunload = false;
bool blp1;
bool boolmap = true;

   // 2023 2 16 - chc Check System Error
   if(frnMain->CheckSystemError() == true) {
      pnlMessage->Caption = "Error! Cannot do.";
      return;
   }

   // 2023 2 3 - chc LP1
   if(btnCassetteLoad->Caption == "UnLoad") {
      pnlMessage->Caption = "CST1 Working!";
      return;
   }
   blp1 = btnCassetteLoad->Enabled;
   pnlCassette2ID->Caption = "";
   pnlCassetteID->Caption = "";

   // 2023 3 20 - chc Clear CST Status
   // 2023 4 17 - chc UnLoad才要清
   //pnlLoadCancelClick(this);

   if(btnCassette2Load->Caption == "UnLoad") {
      pnlMessage->Caption = "LP2 Unloading...";
      boolunload = true;
   }
   else {
      pnlMessage->Caption = "LP2 Loading...";

      // 2023 4 17 - chc Load才要清
      frnMain->WriteSystemLog("Clear CST Status");
      pnlLoadCancelClick(this);

   }
   pnlRecipeSelect->Enabled = false;
   btnCassetteLoad->Enabled = false;
   pnlPortNo->Caption = "2";
   pnlCassette2Load->Color = clSilver;
   btnCassette2Load->Enabled = false;

   // 2023 3 29 - chc Set CST
   frnMain->rgCST->ItemIndex = LOADPORT_2;
   frnMain->rgSECSPortNo->ItemIndex = LOADPORT_2;

   // Read RFID
   // 2023 3 29 - chc Load才要讀碼
   if(btnCassetteLoad->Caption == "Load") {

      frnMain->rgRFIDLoadPort->ItemIndex = LOADPORT_2;
      frnMain->pnlCSTCode->Caption = "UNKNOWN-";
      frnMain->pnlSECSCSTID->Caption = frnMain->pnlCSTCode->Caption;
      frnMain->btnRFIDReadClick(this);
   }   

   // 2023 3 26 - chc SECS: RFID Read
   // 2023 3 29 - chc 若有SECS要等MAPP CMD
   //frnMain->rgPortStatus->ItemIndex = PORT_STATUS_CSTREAD;
   //frnMain->WriteSystemLog("SECS: RFID2 Read " + frnMain->pnlCSTCode->Caption);
   //frnMain->SetToSECS(SECS_CMD_PTST_1 - SECS_CMD_STAT_1);

   //---------------------------------------------------------------------
   // 2023 3 29 - chc 若有SECS要等MAPP CMD
   // 有連線時由SECS MAP CMD(3)觸發LOAD
   frnMain->WaitTime(1000);
   int timeout,cnt;
   // 是否連線?
   // 2023 3 29 - chc Load才要讀碼
   //if(frnMain->pnlSECSConnect->Color == clLime && frnMain->cbOffLine->Checked == false) {
   if(frnMain->pnlSECSConnect->Color == clLime && frnMain->cbOffLine->Checked == false && btnCassette2Load->Caption == "Load") {

      // 連線且Recmote才要做
      frnMain->rgPortStatus->ItemIndex = PORT_STATUS_CSTREAD;
      frnMain->WriteSystemLog("SECS: PTST-CSTREAD " + frnMain->pnlCSTCode->Caption);

      // 2023 3 31 - chc Check CSTID: Trim() - 去除Space
      if(frnMain->pnlCSTCode->Caption.Trim() == "") {
         frnMain->WriteSystemLog("SECS: Set UNKNOWN-(1)");
         frnMain->pnlCSTCode->Caption = "UNKNOWN-";
         frnMain->pnlSECSCSTID->Caption = frnMain->pnlCSTCode->Caption;
      }
      if(frnMain->pnlSECSCSTID->Caption.Trim() == "") {
         frnMain->WriteSystemLog("SECS: Set UNKNOWN-(2)");
         frnMain->pnlSECSCSTID->Caption = "UNKNOWN-";
      }

      frnMain->SetToSECS(SECS_CMD_PTST_1 - SECS_CMD_STAT_1);
      // 45秒
      timeout = 45;
      // Wait
      cnt = 0;

      // 2023 3 31 - chc Cancel
      boolLP2Cancel = false;

      while(1) {

         // 2023 3 31 - chc Cancel
         if(boolLP2Cancel == true) {
            pnlMessage->Caption = "LP2 Cancel";
            frnMain->WriteSystemLog(pnlMessage->Caption);
            break;
         }

         //frnMain->WriteSystemLog("SECS: LP2 Load Wait...");
         frnMain->WaitTime(1000);
         cnt++;
         if(cnt > timeout) {
            pnlCassette2Load->Color = clRed;
            pnlMessage->Caption = "LP2 Load Timeout失敗!";
            break;
         }
         if(frnMain->pnlCassette2Load->Color == clLime) {
            pnlMessage->Caption = "LP2 Load完成.";
            // 由Idle/Run/Down改成Init/Idle/Setup/Ready/Execution/Pause
            // Load後為為EXECUTING
            frnMain->SendSECSStatus(EQ_STATUS_EXECUTING);
            // 禁止變更LoadPort
            frnMain->rgLoadPortSelect->Enabled = false;
            // CSTID
            CassetteIDConfirm = -1;
            pnlMessage->Caption = "LP2 Loaded";
            frmCassetteID->pnlPortNo->Caption = "2";
            //frmCassetteID->edCassetteID->Text = "OLYMPUS-Carrier";
            pnlCassette2ID->Caption = frnMain->pnlCSTCode->Caption;
            frmCassetteID->edCassetteID->Text = pnlCassette2ID->Caption;
            // Set to SECS
            frnMain->pnlSECSCSTID->Caption = pnlCassette2ID->Caption;
            frmCassetteID->ShowModal();

            // 2023 3 31 - chc 不做
            /*
            // ok
            if(frmRecipe->CassetteIDConfirm == 1) {
                 pnlCassette2ID->Caption = frmCassetteID->edCassetteID->Text;
                 pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;
            }
            else {
               pnlCassette2ID->Caption = "";
               pnlPort->Caption = "";
            }
            */

            break;
         }
         else if(frnMain->pnlCassette2Load->Color == clRed) {
            pnlMessage->Caption = "LP2 Load失敗!";
            break;
         }
      }
   }
   else {
      if(frnMain->pnlSECSConnect->Color != clLime)
         frnMain->WriteSystemLog("SECS未連線: 直接做Load");
      if(frnMain->cbOffLine->Checked == true)
         frnMain->WriteSystemLog("在Local狀態下: 直接做Load");
   //---------------------------------------------------------------------

      frnMain->btnCassette2LoadClick(this);
      if(pnlCassette2Load->Color == clLime && boolunload == false) {
         CassetteIDConfirm = -1;
         pnlMessage->Caption = "LP2 Loaded";
         frmCassetteID->pnlPortNo->Caption = "2";

         //frmCassetteID->edCassetteID->Text = "OLYMPUS-Carrier";
         pnlCassette2ID->Caption = frnMain->pnlCSTCode->Caption;
         frmCassetteID->edCassetteID->Text = pnlCassette2ID->Caption;

         // 2023 3 28 - chc Set to SECS
         frnMain->pnlSECSCSTID->Caption = pnlCassette1ID->Caption;

         frmCassetteID->ShowModal();

         // 2023 3 31 - chc 不做
         /*
         // ok
         if(frmRecipe->CassetteIDConfirm == 1) {
              pnlCassetteID->Caption = frmCassetteID->edCassetteID->Text;
              pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;
         }
         else {
            pnlCassetteID->Caption = "";
            pnlPort->Caption = "";
         }
         */

         // 2023 2 3 - chc Loaded
         //blp1 = false;

      }

      else if(pnlCassette2Load->Color == clLime && boolunload == true) {
         pnlMessage->Caption = "LP2 Unloaded";
      }

      else if(pnlCassette2Load->Color != clLime) {
         if(boolunload == true)
            pnlMessage->Caption = "LP2 Unload Fail!";
         else
            pnlMessage->Caption = "LP2 Load Fail!";
         boolmap = false;
      }
   }

   // 2023 2 12 - chc 確認是否MappingOk?
   TPanel *panel;
   for(int i=0 ; i<25 ; i++) {
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      if(panel->Color != clLime && panel->Color != clSilver) {
         boolmap = false;
         break;
      }
   }
   if(boolmap == false) {
      pnlCassetteLoad->Color = clRed;
      pnlLoadOK->Enabled = false;
      pnlLoadOK->Font->Color = clGray;
   }
   else {
      pnlLoadOK->Enabled = true;
      pnlLoadOK->Font->Color = clBlack;

      pnlCassette2Load->Color = clLime;
   }
   btnCassette2Load->Enabled = true;
   pnlStartInspection->Enabled = false;
   pnlEndInspection->Enabled = false;
   pnlStartInspection->Color = clGreen;
   pnlEndInspection->Color = clMaroon;
   pnlStartInspection->Font->Color = clGray;
   pnlEndInspection->Font->Color = clGray;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;

   // 2023 2 3 - chc LP1
   //btnCassetteLoad->Enabled = true;
   btnCassetteLoad->Enabled = blp1;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;
   pnlRecipeSelect->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlLoadOKClick(TObject *Sender)
{
TPanel *panel,*panelT,*panelBE,*panelBC,*panelM,*panelC;
AnsiString str;

   // 2023 4 25 - chc CST Result
   InitInspectResult();

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      if(panel->Color == clLime) {
         panelT->Color = clNavy;
         panelBE->Color = clNavy;
         panelBC->Color = clNavy;
         panelM->Color = clNavy;

         // 2023 4 19 - chc WID是全選
         //panelC->Color = clNavy;
         panelC->Color = clLime;

         // 2023 4 25 - chc CST Result
         // Status: 0-None, 1-NonSelect, 2-Selected
         InspectResult[i].Status = 1;

      }
      // Error
      else if(panel->Color != clLime && panel->Color != clSilver) {

      }
      else {
         panelT->Color = clSilver;
         panelBE->Color = clSilver;
         panelBC->Color = clSilver;
         panelM->Color = clSilver;
         panelC->Color = clSilver;
      }
   }

   // 2023 3 22 - chc Clear RecipeName
   edRecipeID->Text = "";

   // 2023 3 31a - chc Clear RecipeName
   //TPanel *panel;
   for(int i=0 ; i<25 ; i++) {
      panel = (TPanel*)(FindComponent("pnlRecipeID" + IntToStr(i+1)));
      panel->Caption = "";
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlLoadCancelClick(TObject *Sender)
{
TPanel *panel,*panelT,*panelBE,*panelBC,*panelM,*panelC,*panelID;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      panelT->Color = clSilver;
      panelBE->Color = clSilver;
      panelBC->Color = clSilver;
      panelM->Color = clSilver;
      panelC->Color = clSilver;

      // 2023 4 12 - chc pnlRecipeID25
      panelID = (TPanel*)(FindComponent("pnlRecipeID" + IntToStr(i+1)));
      panelID->Color = clSilver;
      panelID->Caption = "";

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlAllTopClick(TObject *Sender)
{
TPanel *panel,*panelT;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      if(panel->Color == clLime)
         panelT->Color = clLime;
      else
         panelT->Color = clSilver;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlAllWaferIDClick(TObject *Sender)
{
TPanel *panel,*panelWI;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelWI = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      if(panel->Color == clLime)
         panelWI->Color = clLime;
      else
         panelWI->Color = clSilver;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlAllBackCenterClick(TObject *Sender)
{
TPanel *panel,*panelBC;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      if(panel->Color == clLime)
         panelBC->Color = clLime;
      else
         panelBC->Color = clSilver;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlAllMicroClick(TObject *Sender)
{
TPanel *panel,*panelM;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      if(panel->Color == clLime)
         panelM->Color = clLime;
      else
         panelM->Color = clSilver;
   }

}
//---------------------------------------------------------------------------
// Restore
void __fastcall TfrmRecipe::pnlAllRestoreClick(TObject *Sender)
{

   pnlLoadOKClick(this);
}
//---------------------------------------------------------------------------
// pnlWafer25Top
void __fastcall TfrmRecipe::pnlWaferTop25Click(TObject *Sender)
{
AnsiString name;

   TPanel* pnlSender = (TPanel*)Sender;
   name = pnlSender->Name;
   if(pnlSender->Color == clNavy)
      pnlSender->Color = clLime;
   else if(pnlSender->Color == clLime)
      pnlSender->Color = clNavy;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlWaferBackEdge25Click(TObject *Sender)
{
AnsiString name;

   TPanel* pnlSender = (TPanel*)Sender;
   name = pnlSender->Name;
   if(pnlSender->Color == clNavy)
      pnlSender->Color = clLime;
   else if(pnlSender->Color == clLime)
      pnlSender->Color = clNavy;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlWaferBackCenter25Click(TObject *Sender)
{
AnsiString name;

   TPanel* pnlSender = (TPanel*)Sender;
   name = pnlSender->Name;
   if(pnlSender->Color == clNavy)
      pnlSender->Color = clLime;
   else if(pnlSender->Color == clLime)
      pnlSender->Color = clNavy;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlWaferMicro25Click(TObject *Sender)
{
AnsiString name;

   TPanel* pnlSender = (TPanel*)Sender;
   name = pnlSender->Name;
   if(pnlSender->Color == clNavy)
      pnlSender->Color = clLime;
   else if(pnlSender->Color == clLime)
      pnlSender->Color = clNavy;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlWaferWaferID25Click(TObject *Sender)
{
AnsiString name;

   TPanel* pnlSender = (TPanel*)Sender;
   name = pnlSender->Name;
   if(pnlSender->Color == clNavy)
      pnlSender->Color = clLime;
   else if(pnlSender->Color == clLime)
      pnlSender->Color = clNavy;
}
//---------------------------------------------------------------------------
// RecipeID.txt
void __fastcall TfrmRecipe::btnBatchSelectClick(TObject *Sender)
{
FILE *pFile;
AnsiString fname,msg;
char id[50],sdate[50],stime[50];
int no;

   // TestRecipeForBE 2022/12/05 11:20
   fname = frnMain->SystemDirectory + "\\RecipeID.txt";

   // 2023 2 20 - chc Create RecipeID.txt
   /*
   pFile = fopen(fname.c_str(),"r");
   if(pFile) {
      sgRecipeSelection->RowCount = 2;
      sgRecipeSelection->Cells[0][sgRecipeSelection->RowCount-1] = "";
      sgRecipeSelection->Cells[1][sgRecipeSelection->RowCount-1] = "";
      no = 1;

      rewind(pFile);
      while(!feof(pFile)) {
         id[0] = 0;
         sdate[0] = 0;
         stime[0] = 0;
         fscanf(pFile,"%s %s %s\n",id,sdate,stime);
         msg.sprintf("%s %s",sdate,stime);
         sgRecipeSelection->Cells[0][sgRecipeSelection->RowCount-1] = id;
         sgRecipeSelection->Cells[1][sgRecipeSelection->RowCount-1] = msg;
         sgRecipeSelection->RowCount++;
      }
      fclose(pFile);
   }
   */
   CreateRecipeID();

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::sgRecipeSelectionClick(TObject *Sender)
{
int recno;

   recno = sgRecipeSelection->Row;
   if(sgRecipeSelection->RowCount > 2) {
      if(recno > 0 && recno < sgRecipeSelection->RowCount-1) {
         edRecipeID->Text = sgRecipeSelection->Cells[0][recno];

         // 2023 2 20 - chc 更新Recipe值to frmInspection: Macro/Micro/WaferID
         UpdateRecipe(edRecipeID->Text);
         // 是否已切換Recipe?
         frnMain->boolRecipeSelect = true;

         // 2023 3 26 - chc SECS: Select Recipe, oRCPM
         frnMain->edSECSRecipeID->Text = edRecipeID->Text;
         frnMain->rgSECSRecipe->ItemIndex = RECIPE_CHANGE;
         frnMain->WriteSystemLog("SECS: Recipe Selected " + edRecipeID->Text);
         frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);

      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnStartMicroClick(TObject *Sender)
{

   frmInspection->ShowModal();
}
//---------------------------------------------------------------------------
AnsiString StartTime,EndTime;
int InspectedNo;
//2023/3/28 10:29
// 單片作業
// 1. 取片
// 2. Macro: Judge
// 3. Aligner
// 4. Chuck: Judge
// 5. 退片
// Loop Next
// Update Select: pnlTray25Select, pnlTray2_25Select
void __fastcall TfrmRecipe::pnlStartInspectionClick(TObject *Sender)
{
TPanel *panel1,*panel2;
TPanel *panel,*panelT,*panelBE,*panelBC,*panelM;
TPanel *ipanelT,*ipanelBE,*ipanelBC,*ipanelM;
TPanel *panelwid;
AnsiString str,klafname;
int no,itop,ibe,ibc,imicro;
bool bresult = true;
bool boolload1,boolload2;

// 2023 4 17 - chc 切換Recipe
AnsiString oldrecipe = "";
AnsiString currecipe = "";

// 2023 4 25 - chc 建立WID Status: pnlWaferWaferID07
TPanel *panelW,*ipanelW;

   // 2023 4 27 - chc Light Tower
   frnMain->TowerType = TOWER_RUN;
   frnMain->WriteSystemLog("pnlStartInspectionClick: TOWER_RUN");

   // 2023 5 26 - chc SECS: ProcessEnd: 量測完成
   // 2023 3 28 - chc 退出Chuck時送
   //frnMain->rgSECSStatus->ItemIndex = STATUS_PROCESSEND;
   //frnMain->WriteSystemLog("SECS: Start Processend " + edRecipeID->Text);
   //frnMain->SetToSECS(SECS_CMD_PREV - SECS_CMD_STAT_1);

   // 2023 4 12 - chc 不可以confirm
   pnlConfirmRecipe->Enabled = false;
   pnlConfirmRecipe->Font->Color = clGray;

   // 2023 3 30 - chc 不可以操作gbTransferStatus
   gbTransferStatus->Enabled = false;
   gbTransferStatus->Color = clGray;

   // 2023 4 12 - chc 禁止操作RecipeSelect
   gbRecipeSelect->Enabled = false;
   gbRecipeSelect->Color = clGray;

   // 2023 2 12 - chc Serial No: top/Left/Right
   frmInspection->TopSeialNo = 0;
   frmInspection->TopLeftSeialNo = 0;
   frmInspection->TopRightSeialNo = 0;
   frmInspection->BackLeftSeialNo = 0;
   frmInspection->BackRightSeialNo = 0;

   boolload1 = btnCassetteLoad->Enabled;
   boolload2 = btnCassette2Load->Enabled;

   pnlStartInspection->Color = clGreen;
   pnlEndInspection->Color = clRed;
   pnlMessage->Font->Color = clBlack;
   frnMain->cbPause->Checked = cbPause->Checked;
   frnMain->cbDemoStep->Checked = cbPause->Checked;
   pnlStartInspection->Font->Color = clGray;
   pnlStartInspection->Enabled = false;
   pnlEndInspection->Font->Color = clBlack;
   pnlEndInspection->Enabled = true;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;

   // Update Status
   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      if(pnlPortNo->Caption.ToInt() == 1)
         panel = (TPanel*)(frnMain->FindComponent("pnlTray" + IntToStr(i+1) + "Select"));
      else
         panel = (TPanel*)(frnMain->FindComponent("pnlTray2_" + IntToStr(i+1) + "Select"));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));

      // 2023 4 26 - chc WID
      panelW = (TPanel*)(FindComponent("pnlWaferWaferID" + str));

      panel->Color = clGray;
      if(panelT->Color == clLime || /*panelBE->Color == clLime ||*/ panelBC->Color == clLime || panelM->Color == clLime) {
         panel->Color = clLime;
      }

      // Copy to frmInspection
      ipanelT = (TPanel*)(frmInspection->FindComponent("pnlWaferTop" + str));
      ipanelBE = (TPanel*)(frmInspection->FindComponent("pnlWaferBackEdge" + str));
      ipanelBC = (TPanel*)(frmInspection->FindComponent("pnlWaferBackCenter" + str));
      ipanelM = (TPanel*)(frmInspection->FindComponent("pnlWaferMicro" + str));

      // 2023 4 26 - chc WID
      ipanelW = (TPanel*)(frmInspection->FindComponent("pnlWaferWaferID" + str));

      ipanelT->Color = panelT->Color;
      ipanelBE->Color = panelBE->Color;
      ipanelBC->Color = panelBC->Color;
      ipanelM->Color = panelM->Color;

      // 2023 4 26 - chc WID
      ipanelW->Color = panelW->Color;

   }
   frmInspection->pnlPort->Caption = pnlPort->Caption;
   frmInspection->pnlCassetteID->Caption = pnlCassetteID->Caption;

   // 2023 2 3 - chc Wafer01\Photo, Reslts, MeasureData
   AnsiString sdate,path,wname;
   DateTimeToString(sdate,"yyyymmddhhmm",Now());
   frnMain->InspectLotPath = pnlCassetteID->Caption;
   frnMain->InspectLotID = sdate + "_" + pnlCassetteID->Caption;
   frnMain->InspectDataPath = frmRecipeSet->edResultPath->Text + "\\" + sdate + "_" + pnlCassetteID->Caption;
   if(!DirectoryExists(frnMain->InspectDataPath))
      mkdir(frnMain->InspectDataPath.c_str());

   // 2023 4 25 - chc StartTime
   DateTimeToString(StartTime,"yyyy'/'mm'/'dd hh':'mm",Now());
   InspectedNo = 0;

   // Set Flag
   frnMain->boolInInspecting = true;
   frnMain->btnMacroInverse->Enabled = false;
   frmInspection->btnMacroInverse->Enabled = false;
   boolInInspection = true;
   pnlMessage->Caption = "Running...";
   for(int i=0 ; i<25 ; i++) {
      if(pnlPortNo->Caption.ToInt() == 1)
         panel = (TPanel*)(frnMain->FindComponent("pnlTray" + IntToStr(i+1) + "Select"));
      else
         panel = (TPanel*)(frnMain->FindComponent("pnlTray2_" + IntToStr(i+1) + "Select"));

      // 2023 2 10 - chc 雙片: 最後一片已被取走
      //if(panel->Color != clLime)
      if(panel->Color != clLime && panel->Color != (TColor)0x0068C6AA && panel->Color != clOlive) {
         frnMain->WriteSystemLog("**無Wafer: " + IntToStr(i+1));
         continue;
      }
      if(panel->Color == clLime)
         frnMain->WriteSystemLog("**有Wafer: " + IntToStr(i+1));
      else if(panel->Color == (TColor)0x0068C6AA)
         frnMain->WriteSystemLog("**有Wafer(作業中): " + IntToStr(i+1));
      else if(panel->Color == clOlive)
         frnMain->WriteSystemLog("**有Wafer(已處理): " + IntToStr(i+1));

      pnlSlotNo->Caption = IntToStr(i+1);
      frnMain->InspectWaferNo = i+1;
      pnlMessage->Caption = "Slot " + IntToStr(i+1) + "Inspecting...";
      frnMain->rgWaferNo->ItemIndex = 24 - i;

      // 2023 2 3 - chc Wafer01\Photo, Results, MeasureData: ZP1612010_8_PSS_1_1.JPG
      wname.sprintf("Wafer%02d",i+1);
      frnMain->InspectWaferPath = frnMain->InspectDataPath + "\\" + wname;
      if(!DirectoryExists(frnMain->InspectWaferPath))
         mkdir(frnMain->InspectWaferPath.c_str());
      frnMain->InspectMicroPhotoPath = frnMain->InspectDataPath + "\\" + wname + "\\MicroPhoto";
      if(!DirectoryExists(frnMain->InspectMicroPhotoPath))
         mkdir(frnMain->InspectMicroPhotoPath.c_str());
      frnMain->InspectResultsPath = frnMain->InspectDataPath + "\\" + wname + "\\Results";
      if(!DirectoryExists(frnMain->InspectResultsPath))
         mkdir(frnMain->InspectResultsPath.c_str());
      frnMain->InspectMeasureDataPath = frnMain->InspectDataPath + "\\" + wname + "\\MeasureData";
      if(!DirectoryExists(frnMain->InspectMeasureDataPath))
         mkdir(frnMain->InspectMeasureDataPath.c_str());
      frnMain->InspectMacroPhotoPath = frnMain->InspectDataPath + "\\" + wname + "\\MacroPhoto";
      if(!DirectoryExists(frnMain->InspectMacroPhotoPath))
         mkdir(frnMain->InspectMacroPhotoPath.c_str());

      // Set Status: boolDoMacroTop,boolDoMacroBackEdge,boolDoMacroBackCenter,boolDoMicro;
      str.sprintf("%02d",i+1);
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));

      // 2023 4 25 - chc 建立WID Status: pnlWaferWaferID07, boolDoWaferID
      panelW = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      if(panelW->Color == clLime)
         frnMain->boolDoWaferID = true;
      else
         frnMain->boolDoWaferID = false;

      itop = ibe = ibc = imicro = 0;
      if(panelT->Color == clLime) {
         frnMain->boolDoMacroTop = true;
         itop = 1;
      }
      else
         frnMain->boolDoMacroTop = false;
      if(panelBE->Color == clLime) {
         frnMain->boolDoMacroBackEdge = true;
         ibe = 1;
      }
      else
         frnMain->boolDoMacroBackEdge = false;
      if(panelBC->Color == clLime) {
         frnMain->boolDoMacroBackCenter = true;
         ibc = 1;
      }
      else
         frnMain->boolDoMacroBackCenter = false;
      if(panelM->Color == clLime) {
         frnMain->boolDoMicro = true;
         imicro = 1;
      }
      else
         frnMain->boolDoMicro = false;

      str.sprintf("Wafer(no,T,E,C,M): %d,%d,%d,%d,%d",i+1,itop,ibe,ibc,imicro);
      frnMain->WriteSystemLog(str);

      // 2023 2 10 - chc 下一片
      frnMain->boolDoMacroTopNext = false;
      frnMain->boolDoMacroBackCenterNext = false;
      frnMain->boolDoMicroNext = false;
      frnMain->iMacroJudgeWaferNo = -1;
      for(int j=i+1 ; j<25 ; j++) {
         if(pnlPortNo->Caption.ToInt() == 1)
            panel = (TPanel*)(frnMain->FindComponent("pnlTray" + IntToStr(j+1) + "Select"));
         else
            panel = (TPanel*)(frnMain->FindComponent("pnlTray2_" + IntToStr(j+1) + "Select"));
         if(panel->Color != clLime)
            continue;
         str.sprintf("%02d",j+1);
         panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
         panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
         panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
         panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
         itop = ibe = ibc = imicro = 0;
         if(panelT->Color == clLime) {
            frnMain->boolDoMacroTopNext = true;
            itop = 1;
         }
         else
            frnMain->boolDoMacroTopNext = false;
         if(panelBE->Color == clLime) {
            frnMain->boolDoMacroBackEdgeNext = true;
            ibe = 1;
         }
         else
            frnMain->boolDoMacroBackEdgeNext = false;
         if(panelBC->Color == clLime) {
            frnMain->boolDoMacroBackCenterNext = true;
            ibc = 1;
         }
         else
            frnMain->boolDoMacroBackCenterNext = false;
         if(panelM->Color == clLime) {
            frnMain->boolDoMicroNext = true;
            imicro = 1;
         }
         else
            frnMain->boolDoMicroNext = false;
         str.sprintf("NextWafer(no,T,E,C,M): %d,%d,%d,%d,%d",j+1,itop,ibe,ibc,imicro);
         frnMain->WriteSystemLog(str);
         frnMain->iMacroJudgeWaferNo = j+1;
         break;
      }

      // 2023 2 28 - chc Read .kla file : 0213-NEGINCOME-T2-04.txt
      klafname = frmRecipeSet->edWaferTxtPath->Text + "\\" + edLotID->Text + "-" + IntToStr(i+1) + ".kla";
      if(FileExists(klafname)) {
         frmInspection->LoadTXTFile(klafname);
      }

      // 2023 3 30 - chc Serial No: top/Left/Right
      frmInspection->TopSeialNo = 0;
      frmInspection->TopLeftSeialNo = 0;
      frmInspection->TopRightSeialNo = 0;
      frmInspection->BackLeftSeialNo = 0;
      frmInspection->BackRightSeialNo = 0;

      // 2023 4 17 - chc 切換Recipe
      //if(currecipe == "") {
      //   currecipe = edRecipeID->Text;
      //}
      //else {
         currecipe = frnMain->WaferBuffer[i].RecipeName;
      //}
      frnMain->WriteSystemLog("Recipe: " + currecipe);
      if(oldrecipe != currecipe) {
         frnMain->WriteSystemLog("Recipe Switch: " + oldrecipe + " To " + currecipe);
         edRecipeID->Text = currecipe;
         // 是否已切換Recipe?
         frnMain->boolRecipeSelect = true;
         // SECS: Select Recipe, oRCPM
         frnMain->edSECSRecipeID->Text = edRecipeID->Text;
         frnMain->rgSECSRecipe->ItemIndex = RECIPE_CHANGE;
         frnMain->WriteSystemLog("SECS: Recipe Selected " + edRecipeID->Text);
         frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);
      }
      // 更新Recipe值to frmInspection: Macro/Micro/WaferID
      UpdateRecipe(edRecipeID->Text);
      oldrecipe = currecipe;

      // 2023 4 27 - chc frmInspection Title
      frmInspection->Caption = "WLS3200-Inspection : [" + edRecipeID->Text + "," + IntToStr(i+1) + "]";

      pnlMessage->Caption = "Slot " + IntToStr(i+1) + " Loading...";
      frnMain->WriteSystemLog(pnlMessage->Caption);
      frnMain->WriteSystemLog("**LoadWafer: " + IntToStr(i+1));
      frnMain->btnLoadWaferClick(this);
      if(frnMain->pnlLoadWafer->Color != clLime) {
         frnMain->AddMainLog("Wafer Get Error!", WAFER_GET_ERR,true);
         pnlMessage->Caption = "Wafer Load Error!";
         frnMain->WriteSystemLog(pnlMessage->Caption);
         pnlMessage->Font->Color = clRed;
         bresult = false;
         break;
      }
      // Update Result: pnlMacroTopResult->Caption, pnlMicroTopResult->Caption
      else {
         UpdateResult();
      }
      // Stop?
      if(boolInInspection == false) {
         break;
      }

      // 2023 5 9 - chc 關BXFM Lamp
      frmInspection->SetIntensity(0);

      pnlMessage->Caption = "Slot " + IntToStr(i+1) + " Unloading...";
      frnMain->WriteSystemLog(pnlMessage->Caption);
      frnMain->WriteSystemLog("**UnLoadWafer: " + IntToStr(i+1));
      frnMain->btnUnloadWaferClick(this);
      if(frnMain->pnlUnloadWafer->Color != clLime) {
         frnMain->AddMainLog("Wafer Put Error!", WAFER_PUT_ERR,true);
         pnlMessage->Caption = "Wafer Unload Error!";
         frnMain->WriteSystemLog(pnlMessage->Caption);
         pnlMessage->Font->Color = clRed;
         bresult = false;
         break;
      }
      // Stop
      if(boolInInspection == false) {
         frnMain->WriteSystemLog("Inspection: Stop");
         break;
      }

      // 2023 4 25 - chc CST Report
      //struct RESULT_STRU {
      //   AnsiString WaferID;
      //   AnsiString TopMacro;
      //   AnsiString BackMacro;
      //   AnsiString Micro;
      //   int DefectNo;
      //   double Yield;
      //   int Status;
      //};
      //struct RESULT_STRU InspectResult[25];
      panelwid = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      InspectedNo++;
      InspectResult[i].WaferID = panelwid->Caption;
      // Top
      if(frmInspection->MacroTopResult == JUDGE_PASS)
         InspectResult[i].TopMacro = "Pass";
      else if(frmInspection->MacroTopResult == JUDGE_NONE)
         InspectResult[i].TopMacro = "None";
      else
         InspectResult[i].TopMacro = "Reject";
      // Back
      if(frmInspection->MacroBackResult == JUDGE_PASS)
         InspectResult[i].BackMacro = "Pass";
      else if(frmInspection->MacroBackResult == JUDGE_NONE)
         InspectResult[i].BackMacro = "None";
      else
         InspectResult[i].BackMacro = "Reject";
      // Micro
      if(frmInspection->MicroResult == JUDGE_PASS)
         InspectResult[i].Micro = "Pass";
      else if(frmInspection->MicroResult == JUDGE_NONE)
         InspectResult[i].Micro = "None";
      else
         InspectResult[i].Micro = "Reject";
      // Status: 0-None, 1-NonSelect, 2-Selected
      InspectResult[i].Status = 2;

   }
   frnMain->WriteSystemLog("**End");
   pnlStartInspection->Color = clLime;
   pnlEndInspection->Color = clMaroon;
   pnlMessage->Caption = "End of Inspection";
   pnlEndInspection->Font->Color = clGray;
   pnlEndInspection->Enabled = false;

   // 2023 4 25 - chc CST Report
   DateTimeToString(EndTime,"yyyy'/'mm'/'dd hh':'mm",Now());
   GenerateCSTReport();

   // 2023 4 12 - chc 要重Confirm才可以Start
   //pnlStartInspection->Font->Color = clBlack;
   //pnlStartInspection->Enabled = true;

   // Set Flag
   frnMain->boolInInspecting = false;
   frmInspection->btnMacroInverse->Enabled = true;
   frnMain->btnMacroInverse->Enabled = true;

   // Error
   if(bresult == false) {
      pnlMessage->Caption = "Inspection Error!";
      frnMain->WriteSystemLog("Inspection: Stop");
      if(frmInspection->Visible == true) {
         frmInspection->Close();

         // 2023 4 27a - chc 不做
         //Beep(1000);
         //SetRedLamp(true);

      }

      // 2023 4 27 - chc Light Tower
      if(frnMain->TowerType != TOWER_STOP) {
         frnMain->TowerType = TOWER_ERROR;
         frnMain->WriteSystemLog("pnlStartInspectionClick: TOWER_ERROR");
      }
   }
   else {
      pnlDisplayResult->Enabled = true;
      pnlDisplayResult->Font->Color = clBlack;
   }

   btnCassetteLoad->Enabled = boolload1;
   btnCassette2Load->Enabled = boolload2;

   // 正常結束?
   if(bresult == true) {
      // 自動Unload?
      // 2023 4 12 - chc 要離線才可以AutoUnload
      //if(frnMain->cbAutoUnload->Checked == true) {
      // 2023 4 25 - chc 一律要unload
      //if(frnMain->cbAutoUnload->Checked == true && frnMain->cbOffLine->Checked == true) {
      if(frnMain->cbAutoUnload->Checked == true) {

         int lpno;
         // 暫停?
         if(cbPause->Checked == true) {
            if(Application->MessageBox("Unload CST?      ", "Recipe Run", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
               goto end;
            }
            pnlRecipeSelect->Enabled = false;
            if(pnlPortNo->Caption == "1") {
               lpno = 1;
               pnlMessage->Caption = "LP1 Unloading...";
               btnCassetteLoadClick(this);
            }
            else {
               lpno = 2;
               pnlMessage->Caption = "LP2 Unloading...";
               btnCassette2LoadClick(this);
            }
            pnlMessage->Caption = "Done: LP" + IntToStr(lpno) + " Unloaded.";
         }
         else {
            pnlRecipeSelect->Enabled = false;
            if(pnlPortNo->Caption == "1") {
               lpno = 1;
               pnlMessage->Caption = "LP1 Unloading...";
               btnCassetteLoadClick(this);
            }
            else {
               lpno = 2;
               pnlMessage->Caption = "LP2 Unloading...";
               btnCassette2LoadClick(this);
            }
            pnlMessage->Caption = "Done: LP" + IntToStr(lpno) + " Unloaded.";
         }
      }

      // 2023 4 27 - chc Beep
      Beep(1000);

   }
end:
   pnlRecipeSelect->Enabled = true;

   // 2023 3 30 - chc 不可以操作gbTransferStatus
   gbTransferStatus->Enabled = true;
   gbTransferStatus->Color = clSilver;

   // 2023 4 12 - chc 禁止操作RecipeSelect
   gbRecipeSelect->Enabled = true;
   gbRecipeSelect->Color = 0x00C6C6C6;

   // 2023 4 12 - chc 不可以confirm
   pnlConfirmRecipe->Enabled = true;
   pnlConfirmRecipe->Font->Color = clBlack;

   // 2023 4 27 - chc Light Tower
   if(frnMain->TowerType != TOWER_ERROR && frnMain->TowerType != TOWER_STOP) {
      frnMain->WriteSystemLog("pnlStartInspectionClick: TOWER_ENDRUN");
      frnMain->TowerType = TOWER_ENDRUN;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlEndInspectionClick(TObject *Sender)
{

   boolInInspection = false;
   pnlEndInspection->Color = clMaroon;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlConfirmRecipeClick(TObject *Sender)
{

   // 2023 3 22 - chc Clear RecipeName
   if(edRecipeID->Text == "") {
      pnlStartInspection->Enabled = false;
      pnlEndInspection->Enabled = false;
      pnlStartInspection->Color = clGreen;
      pnlEndInspection->Color = clMaroon;
      pnlMessage->Caption = "No recipe selected!";
      pnlStartInspection->Font->Color = clGray;
      pnlEndInspection->Font->Color = clGray;
      pnlDisplayResult->Enabled = false;
      pnlDisplayResult->Font->Color = clGray;
      return;
   }

   // 2023 3 30a - chc Set RecipeID: pnlRecipeID25
   if(frnMain->cbOffLine->Checked == true) {
      TPanel *panel,*panelmap;
      for(int i=0 ; i<25 ; i++) {
         panel = (TPanel*)(FindComponent("pnlRecipeID" + IntToStr(i+1)));

         // 2023 4 11 - chc 有片才要填入RecipeID: pnlTray25
         panelmap = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
         if(panelmap->Color == clSilver)
            panel->Caption = "";
         else {

            panel->Caption = edRecipeID->Text;

            // 2023 4 17 - chc 填入WaferID
            frnMain->WaferBuffer[i].RecipeName = edRecipeID->Text;

         }
      }
   }

   pnlStartInspection->Enabled = true;
   pnlEndInspection->Enabled = false;
   pnlStartInspection->Color = clLime;
   pnlEndInspection->Color = clMaroon;
   pnlStartInspection->Font->Color = clBlack;
   pnlEndInspection->Font->Color = clGray;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   MousePosX = X;
   MousePosY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::imCloseClick(TObject *Sender)
{

   frnMain->SetST(ST_RECIPE,false);
   frnMain->pnlCurrentOperation->Caption = "Main";
   frnMain->WriteSystemLog("frmRecipe X-Close.");
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipe::UpdateResult()
{
TPanel *panelT,*panelBE,*panelBC,*panelM,*panelC;
TPanel *ipanelT,*ipanelBE,*ipanelBC,*ipanelM,*ipanelC;
AnsiString str;
int no;

   no = frnMain->InspectWaferNo;
   str.sprintf("%02d",no);
   panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
   panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
   panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
   panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
   panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));

   if(frmInspection->MicroResult == JUDGE_PASS)
      panelM->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(frmInspection->MicroResult == JUDGE_NONE)
      panelM->Color = clNavy;

   else
      panelM->Color = clMaroon;

   if(frmInspection->MacroTopResult == JUDGE_PASS)
      panelT->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(frmInspection->MacroTopResult == JUDGE_NONE)
      panelT->Color = clNavy;

   else
      panelT->Color = clMaroon;

   if(frmInspection->MacroBackResult == JUDGE_PASS)
      panelBC->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(frmInspection->MacroBackResult == JUDGE_NONE)
      panelBC->Color = clNavy;

   else
      panelBC->Color = clMaroon;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::cbPauseClick(TObject *Sender)
{

   frnMain->cbDemoStep->Checked = cbPause->Checked;
   frnMain->cbPause->Checked = cbPause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnCloseInspectionClick(TObject *Sender)
{

   if(frmInspection->Visible == true) {
      frnMain->WriteSystemLog("btnCloseInspectionClick: frmInspection.");
      frmInspection->Close();
   }
}
//---------------------------------------------------------------------------
// 2023 2 20 - chc Create RecipeID.txt
// TestRecipeForBE 2022/12/05 11:20
void __fastcall TfrmRecipe::CreateRecipeID()
{
FILE *pFile;
AnsiString fname,mfname,name,sdate,stime;
TIniFile *pSystemFile;
AnsiString recipefname,msg;

   sgRecipeSelection->RowCount = 2;
   sgRecipeSelection->Cells[0][sgRecipeSelection->RowCount-1] = "";
   sgRecipeSelection->Cells[1][sgRecipeSelection->RowCount-1] = "";

   // TestRecipeForBE 2022/12/05 11:20
   fname = frnMain->SystemDirectory + "\\RecipeID.txt";
   pFile = fopen(fname.c_str(),"w+t");

   TSearchRec sr;
   int iAttributes = 0;
   int rno = 0;
   //faReadOnly	  $00000001	  Read-only files
   //faHidden	  $00000002	  Hidden files
   //faSysFile	  $00000004	  System files
   //faVolumeID	  $00000008	  Volume ID files
   //faDirectory  $00000010	  Directory files
   //faArchive	  $00000020	  Archive files
   //faAnyFile	  $0000003F	  Any file
   mfname = frmRecipeSet->edRecipePath->Text + "\\*.ini";
   iAttributes |= faArchive;
   if(FindFirst(mfname, iAttributes, sr) == 0) {
      do {
         if((sr.Attr & iAttributes) == sr.Attr) {                               // 檔名在sr.Name
            rno++;

            // TestRecipeForBE 2022/12/05 11:20
            recipefname = frmRecipeSet->edRecipePath->Text + "\\" + sr.Name;
            pSystemFile = new TIniFile(recipefname);
            name = pSystemFile->ReadString("Recipe","Name"     ,"Default");
            sdate = pSystemFile->ReadString("Recipe","Date"    ,"2023\/01\/01");
            stime = pSystemFile->ReadString("Recipe","Time"    ,"00:00");
            delete pSystemFile;
            fprintf(pFile,"%s %s %s\n",name.c_str(),sdate.c_str(),stime.c_str());

            msg.sprintf("%s %s",sdate,stime);
            sgRecipeSelection->Cells[0][sgRecipeSelection->RowCount-1] = name;
            sgRecipeSelection->Cells[1][sgRecipeSelection->RowCount-1] = msg;
            sgRecipeSelection->RowCount++;

         }
      } while (FindNext(sr) == 0);
   }
   sgRecipeSelection->Cells[0][sgRecipeSelection->RowCount-1] = "";
   sgRecipeSelection->Cells[1][sgRecipeSelection->RowCount-1] = "";
   FindClose(sr);
   fclose(pFile);

}
//---------------------------------------------------------------------------
// 2023 2 20 - chc 更新Recipe值to frmInspection: Macro/Micro/WaferID
// RecipeBufferRun: 讀取ini檔, 放到frmRecipeSet->RecipeBufferRun & frmInspection的Recipe Parameter區
void __fastcall TfrmRecipe::UpdateRecipe(AnsiString recipename)
{
TIniFile *pSystemFile;
AnsiString recipefname,ptnfname;

   recipefname = frmRecipeSet->edRecipePath->Text + "\\" + recipename + ".ini";
   pSystemFile = new TIniFile(recipefname);

   // 2023 4 12 - chc Pattern display: imMatchPattern, Recipe 1-B1.bmp
   ptnfname = frmRecipeSet->edRecipePath->Text + "\\" + recipename + "\\" + recipename + "-B1.bmp";
   if(FileExists(ptnfname)) {
      frmInspection->imMatchPattern->Picture->LoadFromFile(ptnfname);
      frmInspection->imMatchPattern->Refresh();
   }

   // Recipe
   frmRecipeSet->RecipeBufferRun.Name = pSystemFile->ReadString("Recipe","Name"                 ,"Default");
   frmRecipeSet->RecipeBufferRun.Date = pSystemFile->ReadString("Recipe","Date"                 ,"2023\/01\/01");
   frmRecipeSet->RecipeBufferRun.Time = pSystemFile->ReadString("Recipe","Time"                 ,"00:00");
   frmRecipeSet->RecipeBufferRun.WaferSize = pSystemFile->ReadInteger("Recipe","WaferSize"      ,0);
   frmInspection->rgWaferType->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferSize;
   // frmRecipeSet
   frmRecipeSet->rgWaferType->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferSize;

   // WaferID
   frmRecipeSet->RecipeBufferRun.WaferID.Degree1 = pSystemFile->ReadInteger("WaferID","Degree1"           ,3);
   frmRecipeSet->RecipeBufferRun.WaferID.boolActive1 = pSystemFile->ReadInteger("WaferID","boolActive1"   ,false);
   frmInspection->rgWaferIDDegree1->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.Degree1;
   frmInspection->cbReadWaferID1->Checked = frmRecipeSet->RecipeBufferRun.WaferID.boolActive1;
   // frmRecipeSet
   frmRecipeSet->rgWaferIDDegree1->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.Degree1;
   frmRecipeSet->cbReadWaferID1->Checked = frmRecipeSet->RecipeBufferRun.WaferID.boolActive1;
   frmRecipeSet->RecipeBufferRun.WaferID.Degree2 = pSystemFile->ReadInteger("WaferID","Degree2"           ,3);
   frmRecipeSet->RecipeBufferRun.WaferID.boolActive2 = pSystemFile->ReadInteger("WaferID","boolActive2"   ,false);
   frmInspection->rgWaferIDDegree2->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.Degree2;
   frmInspection->cbReadWaferID2->Checked = frmRecipeSet->RecipeBufferRun.WaferID.boolActive2;
   // frmRecipeSet
   frmRecipeSet->rgWaferIDDegree2->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.Degree2;
   frmRecipeSet->cbReadWaferID2->Checked = frmRecipeSet->RecipeBufferRun.WaferID.boolActive2;

   // 2023 4 5a - chc WaferDirection
   frmRecipeSet->RecipeBufferRun.WaferID.InLP1Degree = pSystemFile->ReadInteger("WaferID","InLP1Degree"           ,2);
   frmRecipeSet->RecipeBufferRun.WaferID.OutLP1Degree = pSystemFile->ReadInteger("WaferID","OutLP1Degree"         ,6);
   frmRecipeSet->RecipeBufferRun.WaferID.InLP2Degree = pSystemFile->ReadInteger("WaferID","InLP2Degree"           ,2);
   frmRecipeSet->RecipeBufferRun.WaferID.OutLP2Degree = pSystemFile->ReadInteger("WaferID","OutLP2Degree"         ,6);
   frmInspection->rgWaferInLP1Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.InLP1Degree;
   frmInspection->rgWaferOutLP1Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.OutLP1Degree;
   frmInspection->rgWaferInLP2Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.InLP2Degree;
   frmInspection->rgWaferOutLP2Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.OutLP2Degree;
   // frmRecipeSet
   frmRecipeSet->rgWaferInLP1Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.InLP1Degree;
   frmRecipeSet->rgWaferOutLP1Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.OutLP1Degree;
   frmRecipeSet->rgWaferInLP2Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.InLP2Degree;
   frmRecipeSet->rgWaferOutLP2Degree->ItemIndex = frmRecipeSet->RecipeBufferRun.WaferID.OutLP2Degree;
   // 0/45/90/135/180/225/270/315
   frnMain->edAlignerDegree->Text = frmRecipeSet->RecipeBufferRun.WaferID.InLP1Degree * 450;
   frnMain->edAlignerDegreeOut->Text = frmRecipeSet->RecipeBufferRun.WaferID.OutLP1Degree * 450;
   frnMain->edAligner2Degree->Text = frmRecipeSet->RecipeBufferRun.WaferID.InLP2Degree * 450;
   frnMain->edAligner2DegreeOut->Text = frmRecipeSet->RecipeBufferRun.WaferID.OutLP2Degree * 450;

   // Macro
   // TopRead
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LoadPosition = pSystemFile->ReadInteger("Macro-Top","LoadPosition"           ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.HomeDuration = pSystemFile->ReadInteger("Macro-Top","HomeDuration"           ,0);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftDuration = pSystemFile->ReadInteger("Macro-Top","LeftDuration"           ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightDuration = pSystemFile->ReadInteger("Macro-Top","RightDuration"         ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.ViewDuration = pSystemFile->ReadInteger("Macro-Top","ViewDuration"           ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftCCDShutter = pSystemFile->ReadInteger("Macro-Top","LeftCCDShutter"       ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightCCDShutter = pSystemFile->ReadInteger("Macro-Top","RightCCDShutter"     ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftLightValue = pSystemFile->ReadInteger("Macro-Top","LeftLightValue"       ,50);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightLightValue = pSystemFile->ReadInteger("Macro-Top","RightLightValue"     ,50);
   // Top
   frmInspection->rgMacroTopLoadPosition->ItemIndex    = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LoadPosition;
   frmInspection->edMacroTopHomeDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.HomeDuration);
   frmInspection->edMacroTopLeftDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftDuration);
   frmInspection->edMacroTopRightDuration->Text        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightDuration);
   frmInspection->edMacroTopViewDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.ViewDuration);
   frmInspection->rgLeftCCDShutter->ItemIndex          = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftCCDShutter;
   frmInspection->rgRightCCDShutter->ItemIndex         = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightCCDShutter;
   frmInspection->pnlTopMacroLeftLamp->Caption         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftLightValue);
   frmInspection->pnlTopMacroRightLamp->Caption        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightLightValue);
   // frmRecipeSet
   frmRecipeSet->rgMacroTopLoadPosition->ItemIndex    = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LoadPosition;
   frmRecipeSet->edMacroTopHomeDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.HomeDuration);
   frmRecipeSet->edMacroTopLeftDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftDuration);
   frmRecipeSet->edMacroTopRightDuration->Text        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightDuration);
   frmRecipeSet->edMacroTopViewDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.ViewDuration);
   frmRecipeSet->rgLeftCCDShutter->ItemIndex          = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftCCDShutter;
   frmRecipeSet->rgRightCCDShutter->ItemIndex         = frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightCCDShutter;
   frmRecipeSet->pnlTopMacroLeftLamp->Caption         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.LeftLightValue);
   frmRecipeSet->pnlTopMacroRightLamp->Caption        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Top.RightLightValue);

   // BackRead
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LoadPosition = pSystemFile->ReadInteger("Macro-Back","LoadPosition"           ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.HomeDuration = pSystemFile->ReadInteger("Macro-Back","HomeDuration"           ,0);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftDuration = pSystemFile->ReadInteger("Macro-Back","LeftDuration"           ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightDuration = pSystemFile->ReadInteger("Macro-Back","RightDuration"         ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.ViewDuration = pSystemFile->ReadInteger("Macro-Back","ViewDuration"           ,1000);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftCCDShutter = pSystemFile->ReadInteger("Macro-Back","LeftCCDShutter"       ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightCCDShutter = pSystemFile->ReadInteger("Macro-Back","RightCCDShutter"     ,3);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftLightValue = pSystemFile->ReadInteger("Macro-Back","LeftLightValue"       ,50);
   frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightLightValue = pSystemFile->ReadInteger("Macro-Back","RightLightValue"     ,50);
   // Back
   frmInspection->rgMacroBackLoadPosition->ItemIndex    = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LoadPosition;
   frmInspection->edMacroBackHomeDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.HomeDuration);
   frmInspection->edMacroBackLeftDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftDuration);
   frmInspection->edMacroBackRightDuration->Text        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightDuration);
   frmInspection->edMacroBackViewDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.ViewDuration);
   frmInspection->rgLeftCCDShutter->ItemIndex           = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftCCDShutter;
   frmInspection->rgRightCCDShutter->ItemIndex          = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightCCDShutter;
   frmInspection->pnlBackMacroLeftLamp->Caption         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftLightValue);
   frmInspection->pnlBackMacroRightLamp->Caption        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightLightValue);
   // frmRecipeSet
   frmRecipeSet->rgMacroBackLoadPosition->ItemIndex    = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LoadPosition;
   frmRecipeSet->edMacroBackHomeDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.HomeDuration);
   frmRecipeSet->edMacroBackLeftDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftDuration);
   frmRecipeSet->edMacroBackRightDuration->Text        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightDuration);
   frmRecipeSet->edMacroBackViewDuration->Text         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.ViewDuration);
   frmRecipeSet->rgLeftCCDShutter->ItemIndex           = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftCCDShutter;
   frmRecipeSet->rgRightCCDShutter->ItemIndex          = frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightCCDShutter;
   frmRecipeSet->pnlBackMacroLeftLamp->Caption         = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.LeftLightValue);
   frmRecipeSet->pnlBackMacroRightLamp->Caption        = IntToStr(frmRecipeSet->RecipeBufferRun.MacroRecipe.Back.RightLightValue);

   // Auto Save
   frmRecipeSet->RecipeBufferRun.MacroRecipe.boolAutoSave = pSystemFile->ReadBool("Macro","boolAutoSave"     ,false);
   frmInspection->cbAutoSaveMacro->Checked              = frmRecipeSet->RecipeBufferRun.MacroRecipe.boolAutoSave;
   // frmRecipeSet
   frmRecipeSet->cbAutoSaveMacro->Checked               = frmRecipeSet->RecipeBufferRun.MacroRecipe.boolAutoSave;

   // Micro
   // Auto Save
   frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave = pSystemFile->ReadBool("Micro","boolAutoSave"     ,false);
   frmInspection->cbAutoSaveMicro->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave;
   // frmRecipeSet
   frmRecipeSet->cbAutoSaveMicro->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave;

   // 2023 5 5 - chc all Die: boolDieInsideAll
   frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave = pSystemFile->ReadBool("Micro","boolDieInsideAll"     ,true);
   frmInspection->cbDieInsideAll->Checked              = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolDieInsideAll;
   frmRecipeSet->cbDieInsideAll->Checked               = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolDieInsideAll;

   // 2023 4 11 - chc ImageSaveMode
   frmRecipeSet->RecipeBufferRun.MicroRecipe.ImageSaveMode = pSystemFile->ReadInteger("Micro","ImageSaveMode"     ,IMAGE_SAVE_BINCODE);
   frmRecipeSet->rgImageSaveMode->ItemIndex             = frmRecipeSet->RecipeBufferRun.MicroRecipe.ImageSaveMode;
   frmInspection->rgImageSaveMode->ItemIndex            = frmRecipeSet->RecipeBufferRun.MicroRecipe.ImageSaveMode;

   // 2023 4 12 - chc MinYield
   frmRecipeSet->RecipeBufferRun.MicroRecipe.MinYield = pSystemFile->ReadFloat("Micro","MinYield"     ,80.5);
   AnsiString tmp;
   tmp.sprintf("%.1f",frmRecipeSet->RecipeBufferRun.MicroRecipe.MinYield);
   frmRecipeSet->edMinYield->Text             = tmp;
   frmInspection->edMinYield->Text            = tmp;

   // Parameter
   // CCD: Shutter, boolAutoWB
   frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.Shutter = pSystemFile->ReadInteger("Micro-CCD","Shutter"     ,3);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.boolAutoWB = pSystemFile->ReadBool("Micro-CCD","boolAutoWB"     ,false);
   frmInspection->rgCCDShutter->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.Shutter;
   frmInspection->cbWBAuto->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.boolAutoWB;
   // frmRecipeSet
   frmRecipeSet->rgCCDShutter->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.Shutter;
   frmRecipeSet->cbWBAuto->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.CCD.boolAutoWB;

   // DIC
   frmRecipeSet->RecipeBufferRun.MicroRecipe.DIC.FCValue = pSystemFile->ReadInteger("Micro-DIC","FCValue"     ,368);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.DIC.boolActive = pSystemFile->ReadBool("Micro-DIC","boolActive"     ,true);
   frmInspection->tbDICFC->Position = frmRecipeSet->RecipeBufferRun.MicroRecipe.DIC.FCValue;
   //frnMain->boolDIC = frmRecipeSet->RecipeBufferEdit.MicroRecipe.DIC.boolActive;
   // frmRecipeSet
   frmRecipeSet->tbDICFC->Position = frmRecipeSet->RecipeBufferRun.MicroRecipe.DIC.FCValue;

   // BXFM
   frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Lens = pSystemFile->ReadInteger("Micro-BXFM","Lens"        ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Intensity = pSystemFile->ReadInteger("Micro-BXFM","Intensity"   ,120);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.AS = pSystemFile->ReadInteger("Micro-BXFM","AS"          ,3113);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Observation = pSystemFile->ReadInteger("Micro-BXFM","Observation" ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.boolAF = pSystemFile->ReadBool("Micro-BXFM","boolAF"         ,false);
   frmInspection->rgBXFMLens->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Lens;
   frmInspection->tbBXFMLMP->Position = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Intensity;
   frmInspection->pnlASValue->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.AS);
   // 切換時會觸發OnClick
   frmInspection->rgObservation->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Observation;

   // 2023 4 3 - chc Assign cbAF會觸發Click
   //frmInspection->cbAF->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.boolAF;

   // frmRecipeSet
   frmRecipeSet->rgBXFMLens->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Lens;
   frmRecipeSet->tbBXFMLMP->Position = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Intensity;
   frmRecipeSet->pnlASValue->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.AS);

   // 切換時會觸發OnClick
   frmRecipeSet->rgObservation->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.Observation;

   // 2023 4 3 - chc Assign cbAF會觸發Click
   //frmRecipeSet->cbAF->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.BXFM.boolAF;

   // Match
   AnsiString msg;
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Match.MinScore = pSystemFile->ReadFloat("Micro-Match","MinScore"     ,0.5);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Match.No = pSystemFile->ReadInteger("Micro-Match","No"         ,2);
   msg.sprintf("%.2f",frmRecipeSet->RecipeBufferRun.MicroRecipe.Match.MinScore);
   frmInspection->edMinScore->Text = msg;
   frmInspection->cbMatchNo->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Match.No);
   // frmRecipeSet
   frmRecipeSet->edMinScore->Text = msg;
   frmRecipeSet->cbMatchNo->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Match.No);

   // Pattern
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.X = pSystemFile->ReadInteger("Micro-Pattern","Base1.X"     ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.Y = pSystemFile->ReadInteger("Micro-Pattern","Base1.Y"     ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.X = pSystemFile->ReadInteger("Micro-Pattern","Base2.X"     ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.Y = pSystemFile->ReadInteger("Micro-Pattern","Base2.Y"     ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.X = pSystemFile->ReadInteger("Micro-Pattern","Position.X"  ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.Y = pSystemFile->ReadInteger("Micro-Pattern","Position.Y"  ,0);
   frmInspection->pnlBase1X->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.X);
   frmInspection->pnlBase1Y->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.Y);
   frmInspection->pnlBase2X->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.X);
   frmInspection->pnlBase2Y->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.Y);
   frmInspection->pnlPositionX->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.X);
   frmInspection->pnlPositionY->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.Y);
   // frmRecipeSet
   frmRecipeSet->pnlBase1X->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.X);
   frmRecipeSet->pnlBase1Y->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base1.Y);
   frmRecipeSet->pnlBase2X->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.X);
   frmRecipeSet->pnlBase2Y->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Base2.Y);
   frmRecipeSet->pnlPositionX->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.X);
   frmRecipeSet->pnlPositionY->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Position.Y);

   // 2023 5 9 - chc 漏Z/T
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdZ = pSystemFile->ReadInteger("Micro-Pattern","StdZ"     ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdT = pSystemFile->ReadInteger("Micro-Pattern","StdT"     ,0);
   frmInspection->pnlStdZ->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdZ);
   frmInspection->pnlStdT->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdT);
   frmRecipeSet->pnlStdZ->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdZ);
   frmRecipeSet->pnlStdT->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.StdT);

   // 2023 5 3 - chc 存中心點: pnlCenterX
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.X = pSystemFile->ReadInteger("Micro-Pattern","Center.X"  ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.Y = pSystemFile->ReadInteger("Micro-Pattern","Center.Y"  ,0);
   frmInspection->pnlCenterDefineX->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.X);
   frmInspection->pnlCenterDefineY->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.Y);
   frmRecipeSet->pnlCenterX->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.X);
   frmRecipeSet->pnlCenterY->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Center.Y);

   // 2023 4 24 - chc 存Match: Intensity & Lens
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Intensity = pSystemFile->ReadInteger("Micro-Pattern","Intensity"    ,80);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Lens = pSystemFile->ReadInteger("Micro-Pattern","Lens"              ,0);
   frmInspection->edPatternIntensity->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Intensity);
   frmInspection->rgPatternLens->ItemIndex = frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.Lens;

   // 2023 5 4 - chc Alignment Position
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.PositionIntensity = pSystemFile->ReadInteger("Micro-Pattern","PositionIntensity"    ,50);
   frmInspection->edPositionIntensity->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Pattern.PositionIntensity);

   // Chip
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Width = pSystemFile->ReadInteger("Micro-Chip","Width"     ,4140);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Height = pSystemFile->ReadInteger("Micro-Chip","Height"    ,4140);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dx = pSystemFile->ReadInteger("Micro-Chip","Dx"        ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dy = pSystemFile->ReadInteger("Micro-Chip","Dy"        ,0);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Ignore = pSystemFile->ReadInteger("Micro-Chip","Ignore"    ,300);
   frmInspection->edChipWidth->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Width);
   frmInspection->edChipHeight->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Height);
   frmInspection->edChipDx->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dx);
   frmInspection->edChipDy->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dy);
   frmInspection->edIgnore->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Ignore);
   // frmRecipeSet
   frmRecipeSet->edChipWidth->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Width);
   frmRecipeSet->edChipHeight->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Height);
   frmRecipeSet->edChipDx->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dx);
   frmRecipeSet->edChipDy->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Dy);
   frmRecipeSet->edIgnore->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.Ignore);
   // SubDieXNo, SubDieYNo
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieXNo = pSystemFile->ReadInteger("Micro-Chip","SubDieXNo"    ,1);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieYNo = pSystemFile->ReadInteger("Micro-Chip","SubDieYNo"    ,1);

   // 2023 4 5 - chc 未更新edSubDieXno,edSubDieYno & edSubDieWidth,edSubDieHeight
   frmInspection->edSubDieXno->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieXNo);
   frmInspection->edSubDieYno->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieYNo);
   // frmRecipeSet
   frmRecipeSet->edSubDieXno->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieXNo);
   frmRecipeSet->edSubDieYno->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieYNo);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieWidth = pSystemFile->ReadInteger("Micro-Chip","SubDieWidth"     ,2730);
   frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieHeight = pSystemFile->ReadInteger("Micro-Chip","SubDieHeight"     ,2730);
   // frmRecipeSet
   frmInspection->edSubDieWidth->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieWidth);
   frmInspection->edSubDieHeight->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieHeight);
   frmRecipeSet->edSubDieWidth->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieWidth);
   frmRecipeSet->edSubDieHeight->Text = IntToStr(frmRecipeSet->RecipeBufferRun.MicroRecipe.Chip.SubDieHeight);

   // Auto Save
   // 2023 4 11 - chc 前面已做過
   //frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave = pSystemFile->ReadBool("Micro","boolAutoSave"         ,false);
   //frmInspection->cbAutoSaveMicro->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave;
   //// frmRecipeSet
   //frmRecipeSet->cbAutoSaveMicro->Checked = frmRecipeSet->RecipeBufferRun.MicroRecipe.boolAutoSave;

   // 2023 3 30a - chc BinCode List ##### 非Write是Read
   // RecipeBufferRun: 讀取ini檔, 放到frmRecipeSet->RecipeBufferRun & frmInspection的Recipe Parameter區

   // 2023 3 30 - chc Save BinCode Select & Item
// 2023 3 30 - chc BinCode List & Item
//#define MAX_BINCODE_TXTNO          20
//#define MAX_BINCODE_ITEMNO         200
//struct BINCODE_STRU {
//   bool bSelect[MAX_BINCODE_TXTNO];
//   AnsiString TxtName[MAX_BINCODE_TXTNO];
//   int ID[MAX_BINCODE_ITEMNO];
//   AnsiString IDName[MAX_BINCODE_ITEMNO];
//   TColor Color[MAX_BINCODE_ITEMNO];
//   int TxtNo,ItemNo;
//};
   frmRecipeSet->RecipeBufferRun.BinCode.TxtNo = pSystemFile->ReadInteger("BINCode","TxtNo"         ,14);
   frmInspection->pnlTxtNumber->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.TxtNo);
   frmRecipeSet->RecipeBufferRun.BinCode.ItemNo = pSystemFile->ReadInteger("BINCode","ItemNo"       ,0);
   frmInspection->pnlItemNumber->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.ItemNo);
   for(int i=0 ; i<frmRecipeSet->RecipeBufferRun.BinCode.TxtNo ; i++) {
      // 目前用指定, 未來由txt檔代入
      frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i] = pSystemFile->ReadBool("BINCode","TxtSelect"+IntToStr(i)      ,false);
      if(i == 0) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"GlassIQCF");
         frmInspection->cbGlassIQCF->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbGlassIQCF->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbGlassIQCF->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbGlassIQCF->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 1) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"GlassIQCB");
         frmInspection->cbGlassIQCB->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbGlassIQCB->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbGlassIQCB->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbGlassIQCB->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 2) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"GlassOQCF");
         frmInspection->cbGlassOQCF->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbGlassOQCF->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbGlassOQCF->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbGlassOQCF->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 3) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"GlassOQCB");
         frmInspection->cbGlassOQCB->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbGlassOQCB->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbGlassOQCB->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbGlassOQCB->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 4) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiIQCSS");
         frmInspection->cbSiIQCSS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiIQCSS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiIQCSS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiIQCSS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 5) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiIQCLS");
         frmInspection->cbSiIQCLS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiIQCLS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiIQCLS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiIQCLS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 6) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiIQCPad");
         frmInspection->cbSiIQCPad->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiIQCPad->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiIQCPad->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiIQCPad->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 7) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiIQCSA");
         frmInspection->cbSiIQCSA->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiIQCSA->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiIQCSA->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiIQCSA->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 8) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiOQCSS");
         frmInspection->cbSiOQCSS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiOQCSS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiOQCSS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiOQCSS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 9) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiOQCLS");
         frmInspection->cbSiOQCLS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiOQCLS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiOQCLS->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiOQCLS->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 10) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiOQCPad");
         frmInspection->cbSiOQCPad->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiOQCPad->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiOQCPad->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiOQCPad->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 11) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"SiOQCW");
         frmInspection->cbSiOQCW->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbSiOQCW->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbSiOQCW->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbSiOQCW->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 12) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"Good");
         frmInspection->cbGood->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbGood->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbGood->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbGood->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
      else if(i == 13) {
         frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i] = pSystemFile->ReadString("BINCode","TxtName"+IntToStr(i)   ,"NA");
         frmInspection->cbNA->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmInspection->cbNA->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
         frmRecipeSet->cbNA->Caption = frmRecipeSet->RecipeBufferRun.BinCode.TxtName[i];
         frmRecipeSet->cbNA->Checked = frmRecipeSet->RecipeBufferRun.BinCode.bSelect[i];
      }
   }
   //frmRecipeSet->RecipeBufferRun.BinCode.ItemNo = pSystemFile->ReadInteger("BINCode","ItemNo"                        ,0);
   //frmInspection->pnlItemNumber->Caption = IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.ItemNo);

   frmInspection->sgBINCode->Visible = false;
   frmInspection->sgBINCode->RowCount = 2;
   frmRecipeSet->sgBINCode->Visible = false;
   frmRecipeSet->sgBINCode->RowCount = 2;
   for(int i=0 ; i<4 ; i++) {
      frmInspection->sgBINCode->Cells[i][frmInspection->sgBINCode->RowCount-1] = "";
      frmRecipeSet->sgBINCode->Cells[i][frmInspection->sgBINCode->RowCount-1] = "";
   }
   frmInspection->sgBINCode->RowCount = frmRecipeSet->RecipeBufferRun.BinCode.ItemNo + 2;
   frmRecipeSet->sgBINCode->RowCount = frmRecipeSet->RecipeBufferRun.BinCode.ItemNo + 2;
   for(int i=1 ; i<=frmRecipeSet->RecipeBufferRun.BinCode.ItemNo ; i++) {
      frmRecipeSet->RecipeBufferRun.BinCode.ID[i-1] = pSystemFile->ReadInteger("BINCode","ItemID"+IntToStr(i)             ,1);
      frmRecipeSet->RecipeBufferRun.BinCode.Color[i-1] = pSystemFile->ReadInteger("BINCode","ItemColor"+IntToStr(i)       ,clSilver);
      frmRecipeSet->RecipeBufferRun.BinCode.IDName[i-1] = pSystemFile->ReadString("BINCode","ItemName"+IntToStr(i)        ,"None");
      frmInspection->sgBINCode->Cells[0][i] = IntToStr(i);
      frmInspection->sgBINCode->Cells[1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.ID[i-1]);
      frmInspection->sgBINCode->Cells[3][i] = frmRecipeSet->RecipeBufferRun.BinCode.IDName[i-1];
      frmRecipeSet->sgBINCode->Cells[0][i] = IntToStr(i);
      frmRecipeSet->sgBINCode->Cells[1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.ID[i-1]);
      frmRecipeSet->sgBINCode->Cells[3][i] = frmRecipeSet->RecipeBufferRun.BinCode.IDName[i-1];
   }
   for(int i=0 ; i<4 ; i++) {
      frmInspection->sgBINCode->Cells[i][frmInspection->sgBINCode->RowCount-1] = "";
      frmRecipeSet->sgBINCode->Cells[i][frmInspection->sgBINCode->RowCount-1] = "";
   }
   frmInspection->sgBINCode->Visible = true;
   frmInspection->sgBINCode->Refresh();
   frmRecipeSet->sgBINCode->Visible = true;
   frmRecipeSet->sgBINCode->Refresh();

   // 2023 4 1 - chc DefectList
   frmRecipeSet->RecipeBufferRun.DefectList.No = pSystemFile->ReadInteger("DefectList","No"                                  ,0);
   frmInspection->sgDefectList->Visible = false;
   frmInspection->sgDefectList->RowCount = 2;
   frmRecipeSet->sgDefectList->Visible = false;
   frmRecipeSet->sgDefectList->RowCount = 2;
   for(int i=0 ; i<10 ; i++) {
      frmInspection->sgDefectList->Cells[i][frmInspection->sgDefectList->RowCount-1] = "";
      frmRecipeSet->sgDefectList->Cells[i][frmInspection->sgDefectList->RowCount-1] = "";
   }
   frmInspection->sgDefectList->RowCount = frmRecipeSet->RecipeBufferRun.DefectList.No + 2;
   frmRecipeSet->sgDefectList->RowCount = frmRecipeSet->RecipeBufferRun.DefectList.No + 2;
   for(int i=1 ; i<=frmRecipeSet->RecipeBufferRun.DefectList.No ; i++) {
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].ID = pSystemFile->ReadInteger("DefectList","ID"+IntToStr(i)         ,0);

      // 2023 4 25 - chc 加入Name
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].IDName = pSystemFile->ReadString("DefectList","IDName"+IntToStr(i)         ,"");

      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].X = pSystemFile->ReadInteger("DefectList","X"+IntToStr(i)           ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Y = pSystemFile->ReadInteger("DefectList","Y"+IntToStr(i)           ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Col = pSystemFile->ReadInteger("DefectList","Col"+IntToStr(i)       ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Row = pSystemFile->ReadInteger("DefectList","Row"+IntToStr(i)       ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Judge = pSystemFile->ReadInteger("DefectList","Judge"+IntToStr(i)   ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].XSize = pSystemFile->ReadFloat("DefectList","XSize"+IntToStr(i)     ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].YSize = pSystemFile->ReadFloat("DefectList","YSize"+IntToStr(i)     ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Area = pSystemFile->ReadFloat("DefectList","Area"+IntToStr(i)       ,0);

      // 2023 4 7 - chc 加入Lens/Intensity
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Lens = pSystemFile->ReadFloat("DefectList","Lens"+IntToStr(i)       ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Intensity = pSystemFile->ReadFloat("DefectList","Intensity"+IntToStr(i)       ,0);

      // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Observation = pSystemFile->ReadFloat("DefectList","Observation"+IntToStr(i)   ,0);
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].DICValue = pSystemFile->ReadFloat("DefectList","DICValue"+IntToStr(i)         ,368);

      // 2023 5 9 - chc 加入boolSubDie
      frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].boolSubDie = pSystemFile->ReadBool("DefectList","boolSubDie"+IntToStr(i)     ,false);

      frmInspection->sgDefectList->Cells[0][i] = IntToStr(i);
      frmInspection->sgDefectList->Cells[1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].ID);

      // 2023 4 25 - chc 加入Name
      frmInspection->sgDefectList->Cells[2][i] = frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].IDName;

      frmInspection->sgDefectList->Cells[2+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].X);
      frmInspection->sgDefectList->Cells[3+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Y);
      frmInspection->sgDefectList->Cells[4+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Col);
      frmInspection->sgDefectList->Cells[5+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Row);
      frmInspection->sgDefectList->Cells[6+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Judge);
      frmInspection->sgDefectList->Cells[7+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].XSize);
      frmInspection->sgDefectList->Cells[8+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].YSize);
      frmInspection->sgDefectList->Cells[9+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Area);

      // 2023 4 7 - chc 加入Lens/Intensity
      frmInspection->sgDefectList->Cells[10+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Lens);
      frmInspection->sgDefectList->Cells[11+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Intensity);

      // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
      frmInspection->sgDefectList->Cells[12+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Observation);
      frmInspection->sgDefectList->Cells[13+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].DICValue);

      frmRecipeSet->sgDefectList->Cells[0][i] = IntToStr(i);
      frmRecipeSet->sgDefectList->Cells[1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].ID);

      // 2023 4 25 - chc 加入Name
      frmRecipeSet->sgDefectList->Cells[2][i] = frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].IDName;

      frmRecipeSet->sgDefectList->Cells[2+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].X);
      frmRecipeSet->sgDefectList->Cells[3+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Y);
      frmRecipeSet->sgDefectList->Cells[4+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Col);
      frmRecipeSet->sgDefectList->Cells[5+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Row);
      frmRecipeSet->sgDefectList->Cells[6+1][i] = IntToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Judge);
      frmRecipeSet->sgDefectList->Cells[7+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].XSize);
      frmRecipeSet->sgDefectList->Cells[8+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].YSize);
      frmRecipeSet->sgDefectList->Cells[9+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Area);

      // 2023 4 7 - chc 加入Lens/Intensity
      frmRecipeSet->sgDefectList->Cells[10+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Lens);
      frmRecipeSet->sgDefectList->Cells[11+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Intensity);

      // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
      frmRecipeSet->sgDefectList->Cells[12+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].Observation);
      frmRecipeSet->sgDefectList->Cells[13+1][i] = FloatToStr(frmRecipeSet->RecipeBufferRun.DefectList.Data[i-1].DICValue);

   }

   // 2023 4 7 - chc 加入Lens/Intensity
   //for(int i=0 ; i<10 ; i++) {
   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   //for(int i=0 ; i<12 ; i++) {
   for(int i=0 ; i<frmInspection->sgDefectList->ColCount ; i++) {

      frmInspection->sgDefectList->Cells[i][frmInspection->sgDefectList->RowCount-1] = "";
      frmRecipeSet->sgDefectList->Cells[i][frmInspection->sgDefectList->RowCount-1] = "";
   }
   frmInspection->sgDefectList->Visible = true;
   frmInspection->sgDefectList->Refresh();
   frmRecipeSet->sgDefectList->Visible = true;
   frmRecipeSet->sgDefectList->Refresh();

   // 2023 5 6 - chc DieSaveOverlap
   frmInspection->edDieSaveOverlap->Text = IntToStr(frmRecipeSet->SystemConfig.DieSaveOverlap);

   // 2023 4 12 - chc Re-Draw
   frmInspection->btnWaferMapDrawClick(frmInspection);

   delete pSystemFile;

}
//---------------------------------------------------------------------------
// 3023 3 22 - chc Reset to Mapping Data
void __fastcall TfrmRecipe::pnlLoadResetClick(TObject *Sender)
{
TPanel *panel,*panelT,*panelBE,*panelBC,*panelM,*panelC;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      if(panel->Color == clOlive)
         panel->Color = clLime;
      panelT->Color = clSilver;
      panelBE->Color = clSilver;
      panelBC->Color = clSilver;
      panelM->Color = clSilver;
      panelC->Color = clSilver;
   }

   // 2023 3 22 - chc
   pnlStartInspection->Enabled = false;
   pnlEndInspection->Enabled = false;
   pnlStartInspection->Color = clGreen;
   pnlEndInspection->Color = clMaroon;
   pnlStartInspection->Font->Color = clGray;
   pnlEndInspection->Font->Color = clGray;
   pnlDisplayResult->Enabled = false;
   pnlDisplayResult->Font->Color = clGray;

}
//---------------------------------------------------------------------------
// 3023 3 22 - chc Clear to Mapping Data
// 目前未使用
void __fastcall TfrmRecipe::ClearMapData()
{
TPanel *panel,*panelT,*panelBE,*panelBC,*panelM,*panelC;
AnsiString str;

   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      panel->Color = clSilver;
      panelT->Color = clSilver;
      panelBE->Color = clSilver;
      panelBC->Color = clSilver;
      panelM->Color = clSilver;
      panelC->Color = clSilver;

      // 2023 3 28a - chc Clear WaferID
      panel->Caption = "";

      // 2023 3 31a - chc 區分LP1/LP2
      if(frnMain->rgCST->ItemIndex == LOADPORT_1) {
         panel = (TPanel*)(frnMain->FindComponent("pnlTray" + IntToStr(i+1)));
      }
      else {
         panel = (TPanel*)(frnMain->FindComponent("pnlTray2_" + IntToStr(i+1)));
      }
      panel->Caption = "";

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlRemoteClick(TObject *Sender)
{

   frnMain->btnRemoteClick(this);
   if(frnMain->cbOffLine->Checked == false) {
      pnlLineStatus->Caption = "Online";
      pnlRemote->Font->Color = clNavy;
      pnlLocal->Font->Color = clBlack;
   }
   else {
      pnlRemote->Font->Color = clBlack;
      pnlLocal->Font->Color = clNavy;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::pnlLocalClick(TObject *Sender)
{

   frnMain->btnLocalClick(this);
   if(frnMain->cbOffLine->Checked == true) {
      pnlLineStatus->Caption = "Offline";
      pnlRemote->Font->Color = clBlack;
      pnlLocal->Font->Color = clNavy;
   }
   else {
      pnlRemote->Font->Color = clNavy;
      pnlLocal->Font->Color = clBlack;
   }
}
//---------------------------------------------------------------------------
// 2023 4 25 - chc CST Report
void __fastcall TfrmRecipe::GenerateCSTReport()
{
int wno,ino;
FILE *pFile;
AnsiString fname,slotid,srfid;
TPanel *panel;

   // frnMain->InspectDataPath
   fname.sprintf("%s\\%s.csv",frnMain->InspectDataPath.c_str(),frnMain->InspectLotID.c_str());
   pFile = fopen(fname.c_str(),"w+t");

   if(edLotIDFromWaferID->Text == "")
      slotid = edLotID->Text;
   else
      slotid = edLotIDFromWaferID->Text;
   if(pnlPortNo->Caption.ToInt() == 1)
      srfid = pnlCassette1ID->Caption;
   else
      srfid = pnlCassette2ID->Caption;
   wno = 0;
   for(int i=0 ; i<25 ; i++) {
      panel = (TPanel*)(FindComponent("pnlTray" + IntToStr(i+1)));
      if(panel->Color != clSilver)
         wno++;
   }
   // Title: StartTime, EndTime, LotID, RFID, DeviceName, Number of Wafer
   fprintf(pFile,"StartTime,EndTime,LotID,RFID,DeviceName,TotalPCs,InspectedPCs\n");
   fprintf(pFile,"%s,%s,%s,%s,%s,%d,%d\n",StartTime.c_str(),EndTime.c_str(),slotid.c_str(),srfid.c_str(),
                                       edDeviceName->Text.c_str(),wno,InspectedNo);

   // Wafer Title
   // 0-None, 1-Pass, 2-Reject
   //struct RESULT_STRU {
   //   AnsiString WaferID;
   //   AnsiString TopMacro;
   //   AnsiString BackMacro;
   //   AnsiString Micro;
   //   int DefectNo;
   //   double Yield;
   //   int Status;
   //};
   fprintf(pFile,"\n");
   fprintf(pFile,"No,WID,Recipe,Top-Macro,BackMacro,Micro,DefectNo,Yield\n");
   for(int i=0 ; i<25 ; i++) {
      if(InspectResult[i].Status != 0) {
         fprintf(pFile,"%d,%s,%s,%s,%s,%s,%d,%.2f\n",i+1,InspectResult[i].WaferID,
                                                  InspectResult[i].RecipeName,
                                                  InspectResult[i].TopMacro,
                                                  InspectResult[i].BackMacro,
                                                  InspectResult[i].Micro,
                                                  InspectResult[i].DefectNo,
                                                  InspectResult[i].Yield);
      }
   }

   fclose(pFile);
}
//---------------------------------------------------------------------------
//   struct RESULT_STRU {
//      AnsiString WaferID;
//      AnsiString TopMacro;
//      AnsiString BackMacro;
//      AnsiString Micro;
//      int DefectNo;
//      double Yield;
//      int Status;
//   };
//   struct RESULT_STRU InspectResult[25];
void __fastcall TfrmRecipe::InitInspectResult()
{

   for(int i=0 ; i<25 ; i++) {
      InspectResult[i].Status = 0;
      InspectResult[i].TopMacro = "None";
      InspectResult[i].BackMacro = "None";
      InspectResult[i].Micro = "None";
      InspectResult[i].WaferID = "";
      InspectResult[i].DefectNo = 0;
      InspectResult[i].Yield = 0;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnResetAlarmClick(TObject *Sender)
{

   // 2023 4 27 - chc Light Tower
   frnMain->TowerType = TOWER_IDLE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipe::btnResetBuzzerClick(TObject *Sender)
{

   frnMain->boolTowerBuzzer = false;
}
//---------------------------------------------------------------------------

