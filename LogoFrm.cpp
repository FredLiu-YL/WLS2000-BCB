//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LogoFrm.h"
#include "frmMain.h"

// 2022 9 4 - chc 加入Recipe/Inspection form
#include "InspectionForm.h"
#include "RecipeForm.h"
#include "SystemLogForm.h"
#include "RecipeSetForm.h"

extern int MousePosX;
extern int MousePosY;
void __fastcall Beep(int mstime);
void __fastcall SetRedLamp(bool mode);

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDButton"
#pragma link "LMDCustomButton"
#pragma resource "*.dfm"
TfrmLogo *frmLogo;

// 2022 8 15 - chc 分段啟動
extern bool boolFormShowDone;

//---------------------------------------------------------------------------
__fastcall TfrmLogo::TfrmLogo(TComponent* Owner)
  : TForm(Owner)
{

}
//---------------------------------------------------------------------------
bool boolHomeDone = false;
bool boolHomeStart = false;
void __fastcall TfrmLogo::tmLogoTimer(TObject *Sender)
{
static int cnt = 0;
static int fcnt = 0;

   cnt++;
   if(cnt == 1) {
      frnMain->Visible = false;
   }
   if(cnt == 2) {
      frnMain->Visible = true;
   }
   if(boolFormShowDone == true && pnlStart->Visible == true) {
      pnlStart->Visible = false;
      fcnt = cnt;
   }

   // 3秒
   // 2022 12 5 - chc New Home
   //if((cnt - fcnt) > 5) {
   //   Visible = false;
   //   tmLogo->Enabled = false;
   //}
   //else {
   //   Application->ProcessMessages();
   //   pnlLogoMain->Refresh();
   //}
   if(pnlHome3->Enabled == false && boolHomeStart == false && frnMain->pnlID3Bit15->Color == clSilver) {
      if(frnMain->btnHome2->Enabled == true) {
         frnMain->WriteSystemLog(">Logo : Home...");
         pnlMessage->Caption = "Execute[System Initialize]...";
         pnlHome3->Visible = true;
         if(pnlHome3->Left != 28) {
            pnlHome3->Enabled = true;
            pnlHome3->Left = 28;
            pnlHome3->Top = 96;
         }
      }
      else {
         if(frnMain->boolForTest == false) {
            frnMain->WriteSystemLog(">Logo : Home Starting...");
         }
         pnlMessage->Caption = "System Starting...";
         Application->ProcessMessages();
         pnlLogoMain->Refresh();
      }
   }
   else {
      // Home結束?
      if(boolHomeDone == true) {
         frnMain->WriteSystemLog(">Logo : Home Done.");
         pnlMessage->Caption = "System Initialized.";
         //frnMain->pnlEFEM2->Visible = true;
         frnMain->pnlMain->Color = clBlack;
         //Visible = false;
         tmLogo->Enabled = false;
         pnlHome3->Visible = false;
         pnlHome3Status->Visible = false;
         pnlLogo1->Visible = true;
         btnMainInspection->Enabled = true;
         //btnMainRecipeSetup->Enabled = true;
         btnMainRecipeData->Enabled = true;
         btnMainProcessResult->Enabled = true;
         btnMainUserSet->Enabled = true;
         //btnMainHardwareSetup->Enabled = true;
      }
      Application->ProcessMessages();
      pnlLogoMain->Refresh();
      //frnMain->WriteSystemLog(">tmLogoTimer : Leave.");
   }

}
//---------------------------------------------------------------------------
bool boolShowed = false;

void __fastcall TfrmLogo::FormShow(TObject *Sender)
{

   if(boolShowed == false) {
      pnlStart->Left = 28;
      pnlStart->Top = 64;
      pnlStart->Width = 461;
      pnlStart->Height = 193;
      pnlHome3Status->Left = 28;
      pnlHome3Status->Top = 96;
      pnlHome3Status->Left = 28;
      pnlHome3Status->Top = 176;

      tmLogo->Enabled = true;
      pnlStart->Caption = "System Starting...";
      pnlStart->Font->Color = clBlack;
      pnlLogo1->Visible = false;
   }
   else {
      Visible = true;
   }

   // 設定視窗位置
   frmLogo->Left = (frnMain->MASTER_SCREEN_WIDTH - frmLogo->Width) / 2;
   frmLogo->Top  = (frnMain->MASTER_SCREEN_HEIGHT - frmLogo->Height) / 2;
   boolShowed = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

   if(Key == 27) {                                      // ESC - Exit
      Close();
   }

   // Alt-P Print Screen(Current Windows)
   if(Key == 80 && Shift.Contains(ssAlt))
      frnMain->SaveWindowScreen(frmLogo);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::imCloseClick(TObject *Sender)
{

   Close();        
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::btnMainInspectionClick(TObject *Sender)
{

   // 2023 2 10 - chc 改為Show
   frmRecipe->ShowModal();
   //frmRecipe->Show();

   // 2023 2 28 - chc frmLogo Visible
   frmLogo->Visible = false;
    
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::btnMainRecipeSetupClick(TObject *Sender)
{

   // 2023 2 28 - chc Show
   //frmRecipeSet->ShowModal();
   frmRecipeSet->Show();

   // 2023 2 28 - chc frmLogo Visible
   frmLogo->Visible = false;
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::FormClose(TObject *Sender, TCloseAction &Action)
{

   frnMain->Visible = true;
   //Action = caFree;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::pnlHome3Click(TObject *Sender)
{
int ret;
static bool bactive = false;

   if(bactive == true) {
      frnMain->WriteSystemLog(">pnlHome3Click : ReEntrant!");
      return;
   }
   bactive = true;

   // 2023 4 27a - chc Light Tower
   frnMain->TowerType = TOWER_INIT;
   frnMain->WriteSystemLog("pnlHome3Click: TOWER_INIT");

   frnMain->WriteSystemLog(">pnlHome3Click : Enter..");
   pnlHome3->Enabled = false;
   pnlHome3->Font->Color = clSilver;

   // 2023 3 20 - chc 不執行, goto next
   // do
   //goto next;

   // 2023 1 15 - chc Robot ORGN
   pnlMessage->Caption = "Robot ORGN...";
   pnlMessage->Refresh();
   frnMain->pnlMainOperation->Caption = pnlMessage->Caption;
   frnMain->pnlMainOperation->Refresh();
   frnMain->WriteSystemLog(pnlMessage->Caption);
   if(frnMain->RobotORGN() == false) {
      frnMain->pnlAlarm->Caption = "Robot ORGN Error!";
      frnMain->pnlAlarm->Font->Color = clRed;
      frnMain->AddMainLog("Robot ORGN Error!", ROBOT_ORGN_ERR,true);
      pnlMessage->Caption = frnMain->pnlAlarm->Caption;
      frnMain->WriteSystemLog(pnlMessage->Caption);
      Beep(500);
      SetRedLamp(true);
      pnlHome3->Enabled = true;
      pnlHome3->Font->Color = clWhite;
      bactive = false;
      return;
   }

next:

   // 2023 1 15 - chc Wafer Exist?
   //  0. ok
   //  1. Robot COM Error
   //  2. Aligner COM Error
   //  3. Robot with wafer
   //  4. Aligner with wafer
   //  5. Chuck with wafer
   //  6. Macro with wafer
   pnlMessage->Caption = "Wafer In System?";
   frnMain->pnlMainOperation->Caption = pnlMessage->Caption;
   frnMain->pnlMainOperation->Refresh();
   frnMain->WriteSystemLog(pnlMessage->Caption);
   ret = frnMain->WaferInSystem();
   if(ret != 0) {
      if(ret == 1) {
         frnMain->pnlAlarm->Caption = "Robot COM Error!";
         frnMain->pnlAlarm->Font->Color = clRed;
         frnMain->AddMainLog(frnMain->pnlAlarm->Caption, ROBOT_COM_ERR,true);
      }
      else if(ret == 2) {
         frnMain->pnlAlarm->Caption = "Aligner COM Error!";
         frnMain->pnlAlarm->Font->Color = clRed;
         frnMain->AddMainLog(frnMain->pnlAlarm->Caption, ALIGNER_COM_ERR,true);
      }
      else {
         frnMain->pnlAlarm->Caption = "Wafer In System!";
         frnMain->pnlAlarm->Font->Color = clRed;
         frnMain->AddMainLog("Wafer In System!", WAFER_EXIST_ERR,true);
      }
      pnlMessage->Caption = frnMain->pnlAlarm->Caption;
      frnMain->WriteSystemLog(pnlMessage->Caption);
      Beep(500);

      // 2023 4 29 - chc 記錄各處Wafer狀態
      if(ret > 2) {
         if(frnMain->boolWaferInChuck == true || frnMain->boolWaferInMacroInner == true || frnMain->boolWaferInAligner1 == true ||
            frnMain->boolWaferInAligner2 == true || frnMain->boolWaferInRobot == true || frnMain->boolWaferInMacroOuter == true) {
            AnsiString msg;
            msg = "Wafer in system: ";
            if(frnMain->boolWaferInChuck == true) {
               msg += "Chuck,";
            }
            if(frnMain->boolWaferInMacroInner == true) {
               msg += "MacroIn,";
            }
            if(frnMain->boolWaferInMacroOuter == true) {
               msg += "MacroOut,";
            }
            if(frnMain->boolWaferInAligner1 == true) {
               msg += "Aligner1,";
            }
            if(frnMain->boolWaferInAligner2 == true) {
               msg += "Aligner2,";
            }
            if(frnMain->boolWaferInRobot == true) {
               msg += "Robot,";
            }
            msg += " To continue?";
            if(Application->MessageBox(msg.c_str(), "System Initialize", MB_YESNO|MB_SYSTEMMODAL) == IDYES) {
               goto togo;
            }
         }
      }

      SetRedLamp(true);
      pnlHome3->Enabled = true;
      pnlHome3->Font->Color = clWhite;
      bactive = false;
      return;
   }
togo:
   boolHomeStart = true;
   pnlHome3Status->Visible = true;

   if(pnlHome3->Caption == "RESET") {
      pnlLogo1->Visible = false;
      boolHomeDone = false;
      frnMain->WriteSystemLog(">pnlHome3Click(RESET) : Do AxisSVON()");
      frnMain->AxisSVON();
      if(frnMain->btnHome->Enabled == true) {
         frnMain->WriteSystemLog(">pnlHome3Click(RESET) : Do Home");
         frnMain->pnlSystemMessage->Caption = "Motion SVON OK.";
         pnlMessage->Caption = "System Initializing...";
         frnMain->btnHomeClick(this);
         boolHomeDone = true;
         pnlHome3->Visible = false;
         pnlHome3Status->Visible = false;
         pnlLogo1->Visible = true;
      }
      else {
         frnMain->pnlSystemMessage->Caption = "Motion SVON Fail!";
         pnlHome3->Enabled = true;
         pnlHome3->Font->Color = clWhite;
         frnMain->WriteSystemLog(">pnlHome3Click(RESET) : Do AxisSVON() Fail!");
      }
   }
   else {
      frnMain->WriteSystemLog(">pnlHome3Click(Start) : Do Home");
      pnlMessage->Caption = "System Initializing...";
      frnMain->btnHomeClick(this);
      boolHomeDone = true;
   }
   frnMain->WriteSystemLog(">pnlHome3Click : Leave.");

   // 2023 4 27 - chc LightTower:
   if(frnMain->TowerType != TOWER_ERROR && frnMain->TowerType != TOWER_STOP) {
      frnMain->WriteSystemLog("pnlHome3Click: TOWER_IDLE");
      frnMain->TowerType = TOWER_IDLE;
   }

   bactive = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::pnlLogoUserClick(TObject *Sender)
{

   frnMain->btnPasswordClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::btnMainHardwareSetupClick(TObject *Sender)
{
static bool boolfirst = true;

   frmLogo->Visible = false;
   if(boolfirst == true) {
      boolfirst = false;
      frnMain->pnlMaintenance->Top = 72;
      frnMain->pnlMaintenance->Left = 424;
   }
   frnMain->pnlMaintenance->Visible = true;
   frnMain->pcMaintenance->ActivePage = frnMain->tsHirata;
   frnMain->pcRobot->ActivePage = frnMain->tsCommand;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::btnMainRecipeDataClick(TObject *Sender)
{

   //frmInspection->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall TfrmLogo::btnMainUserSetClick(TObject *Sender)
{

   frnMain->btnPasswordClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::btnMainProcessResultClick(TObject *Sender)
{

   frmSystemLog->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::imSystemLogClick(TObject *Sender)
{

   frmSystemLog->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::imHelpClick(TObject *Sender)
{

   //frnMain->pnlEFEM->Visible = false;
   //frnMain->cbEFEM->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::imMainClick(TObject *Sender)
{

   pnlLogo1->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{

   MousePosX = X;
   MousePosY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::Panel1Click(TObject *Sender)
{

   pnlLogo1->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmLogo::mnTimeResetClick(TObject *Sender)
{

   frnMain->LampLifeTime = 0;
   frnMain->LampLifeCount = 0;
   pnlLampLifeTime->Caption = "";
}
//---------------------------------------------------------------------------

