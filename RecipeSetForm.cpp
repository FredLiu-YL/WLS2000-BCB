//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RecipeSetForm.h"
#include "frmMain.h"

#include "LogoFrm.h"
#include "InspectionForm.h"
#include "RecipeForm.h"
//---------------------------------------------------------------------------
// NCC
#include "iMatchDLL\iVisionErrs.h"
#include "iMatchDLL\iVision_Types.h"
#include "iMatchDLL\iVision.h"
#include "iMatchDLL\iImage.h"
#include "iMatchDLL\iMatch.h"
extern LONG_PTR ColorImg,ColorLearnImg,GrayImg,GrayLearnImg;
extern LONG_PTR iMatchModel;
// iImgPocess
#include "iMatchDLL\iImgProcess.h"

void __fastcall GetPosition(int axisno, I32 *pos);
bool __fastcall MoveToY(int position);
bool __fastcall MoveToX(int position);
bool __fastcall MoveToT(int position);
void __fastcall Beep(int mstime);
bool __fastcall MoveToXY(int xpos,int ypos);
void __fastcall Beep(int mstime);
bool __fastcall MoveToZ(int position);

extern bool boolCCDImageLoadedAry[3];
extern unsigned long GigaWidth,GigaHeight;
extern int MousePosX;
extern int MousePosY;
extern int CCDShutterAry[3],CCDGainAry[3],CCDGrayAry[3],CCDContrastAry[3];

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
#pragma link "LMDButton"
#pragma link "LMDCustomButton"
#pragma link "LMDButtonControl"
#pragma link "LMDCheckBox"
#pragma link "LMDCustomCheckBox"
#pragma link "LMDCustomButtonGroup"
#pragma link "LMDCustomGroupBox"
#pragma link "LMDCustomRadioGroup"
#pragma link "LMDCustomTrackBar"
#pragma link "LMDRadioGroup"
#pragma link "LMDTrackBar"
#pragma link "LMDBaseEdit"
#pragma link "LMDCustomEdit"
#pragma link "LMDEdit"
#pragma link "LMDGroupBox"
#pragma resource "*.dfm"
TfrmRecipeSet *frmRecipeSet;
//---------------------------------------------------------------------------
__fastcall TfrmRecipeSet::TfrmRecipeSet(TComponent* Owner)
        : TForm(Owner)
{

   ImageFactor = 1;
   ImageLeftFactor = 1;
   ImageRightFactor = 1;

   // 2023 3 20 - chc Load System Parameters
   // 2023 3 23 - chc �bFormshowŪ
   //LoadSystemParameters();

   // 2023 3 29a - chc DefectList Title
   DefectListTitle();
   BinCodeItemTitle();
   BinCodeTitle();
   CreateRecipeID();

   // 2023 4 28 - chc Capture cnt: CaptureCount
   CaptureCount = 0;

}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroHomeClick(TObject *Sender)
{

   gbMacroParameters->Enabled = false;
   pnlMacroHome->Color = clSilver;
   frnMain->btnMacroHomeClick(this);
   pnlMacroHome->Color = frnMain->pnlMacroHome->Color;
   gbMacroParameters->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYMoveMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerYMove->Color = clLime;
   frnMain->btnMacroInnerYMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYMoveMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerYMoveMouseExit(Sender);
   pnlMacroInnerYMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYMoveMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerYMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXBackMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerXBack->Color = clLime;
   frnMain->btnMacroInnerXBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXBackMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerXBackMouseExit(Sender);
   pnlMacroInnerXBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXBackMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerXBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXMoveMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerXMove->Color = clLime;
   frnMain->btnMacroInnerXMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXMoveMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerXMoveMouseExit(Sender);
   pnlMacroInnerXMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerXMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXMoveMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerXMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYBackMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerYBack->Color = clLime;
   frnMain->btnMacroInnerYBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYBackMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerYBackMouseExit(Sender);
   pnlMacroInnerYBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerYBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYBackMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerYBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTMoveMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerTMove->Color = clLime;
   frnMain->btnMacroInnerTMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTMoveMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerTMoveMouseExit(Sender);
   pnlMacroInnerTMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTMoveMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerTMove->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTBackMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   pnlMacroInnerTBack->Color = clLime;
   frnMain->btnMacroInnerTBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTBackMouseExit(TObject *Sender)
{

   frnMain->btnMacroInnerTBackMouseExit(Sender);
   pnlMacroInnerTBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInnerTBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTBackMouseUp(Sender,Button,Shift,X,Y);
   pnlMacroInnerTBack->Color = clSilver;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroInverseClick(TObject *Sender)
{

   gbMacroParameters->Enabled = false;
   pnlMacroInverse->Color = clSilver;
   frnMain->btnMacroInverseClick(this);
   pnlMacroInverse->Color = frnMain->pnlMacroInverse->Color;
   if(pnlMacroInverse->Color == clLime) {
      if(frnMain->rgSide->ItemIndex == WAFER_TOP)
         rgWaferStatus->ItemIndex = 1;
      else
         rgWaferStatus->ItemIndex = 2;
   }
   gbMacroParameters->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroTopToClick(TObject *Sender)
{
TMouseButton Button;
TShiftState Shift;
int X,Y,delaytime;

   pnlMacroTopTo->Color = clSilver;
   frmInspection->pnlMacroTopTo->Color = clSilver;
   btnMacroHomeClick(this);
   if(pnlMacroHome->Color != clLime) {
      pnlMacroTopTo->Color = clRed;
      frmInspection->pnlMacroTopTo->Color = clRed;
      return;
   }
   // Home
   if(rgMacroTopLoadPosition->ItemIndex == MACRO_HOME)
      return;

   X = Y = 0;
   // Left
   if(rgMacroTopLoadPosition->ItemIndex == MACRO_LEFT || rgMacroTopLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroTopLeftDuration->Text.ToInt();
      if(delaytime > 0) {
         frnMain->WriteSystemLog("Left...");
         btnMacroInnerXBackMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Left Down.");
         Sleep(delaytime);
         btnMacroInnerXBackMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Left.");
      }
   }
   // Right
   if(rgMacroTopLoadPosition->ItemIndex == MACRO_RIGHT || rgMacroTopLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroTopRightDuration->Text.ToInt();

      // 2023 4 17 - chc Left�n��0
      //if(delaytime > 0) {
      if(delaytime > 0 && edMacroTopLeftDuration->Text.ToInt() == 0) {

         frnMain->WriteSystemLog("Right...");
         btnMacroInnerXMoveMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Right Down.");
         Sleep(delaytime);
         btnMacroInnerXMoveMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Right.");
      }
   }
   // View
   if(rgMacroTopLoadPosition->ItemIndex == MACRO_VIEW || rgMacroTopLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroTopViewDuration->Text.ToInt();
      if(delaytime > 0) {
         frnMain->WriteSystemLog("View...");
         btnMacroInnerYMoveMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("View Down.");
         Sleep(delaytime);
         btnMacroInnerYMoveMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("View.");
      }
   }

   pnlMacroTopTo->Color = clLime;
   frmInspection->pnlMacroTopTo->Color = clLime;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::btnMacroBackToClick(TObject *Sender)
{
TMouseButton Button;
TShiftState Shift;
int X,Y,delaytime;

   pnlMacroBackTo->Color = clSilver;
   frmInspection->pnlMacroBackTo->Color = clSilver;
   btnMacroHomeClick(this);
   if(pnlMacroHome->Color != clLime) {
      pnlMacroBackTo->Color = clRed;
      frmInspection->pnlMacroBackTo->Color = clRed;
      return;
   }
   // Home
   if(rgMacroBackLoadPosition->ItemIndex == MACRO_HOME)
      return;

   X = Y = 0;
   // Left
   if(rgMacroBackLoadPosition->ItemIndex == MACRO_LEFT || rgMacroBackLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroBackLeftDuration->Text.ToInt();
      if(delaytime > 0) {
         frnMain->WriteSystemLog("Left...");
         btnMacroInnerXBackMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Left Down.");
         Sleep(delaytime);
         btnMacroInnerXBackMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Left.");
      }
   }
   // Right
   if(rgMacroBackLoadPosition->ItemIndex == MACRO_RIGHT || rgMacroBackLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroBackRightDuration->Text.ToInt();

      // 2023 4 17 - chc Left�n��0
      //if(delaytime > 0) {
      if(delaytime > 0 && edMacroBackLeftDuration->Text.ToInt() == 0) {

         frnMain->WriteSystemLog("Right...");
         btnMacroInnerXMoveMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Right Down.");
         Sleep(delaytime);
         btnMacroInnerXMoveMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("Right.");
      }
   }
   // View
   if(rgMacroBackLoadPosition->ItemIndex == MACRO_VIEW || rgMacroBackLoadPosition->ItemIndex == MACRO_ALL) {
      delaytime = edMacroBackViewDuration->Text.ToInt();
      if(delaytime > 0) {
         frnMain->WriteSystemLog("View...");
         btnMacroInnerYMoveMouseDown(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("View Down.");
         Sleep(delaytime);
         btnMacroInnerYMoveMouseUp(this, Button, Shift, X, Y);
         frnMain->WriteSystemLog("View.");
      }
   }

   pnlMacroBackTo->Color = clLime;
   frmInspection->pnlMacroBackTo->Color = clLime;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::imCloseClick(TObject *Sender)
{

   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::FormShow(TObject *Sender)
{
// 2023 3 2 - chc Init
static bool boolfirst = true;

   // CCD FOV vs CCDImage Area
   CCDDx = frnMain->CCDInfoAry[0].Width - imInspection->Width;
   CCDDy = frnMain->CCDInfoAry[0].Height - imInspection->Height;
   CCDRangeX = CCDDx / 2;
   CCDRangeY = CCDDy / 2;

   //frmRecipeSet->Left = (frnMain->MASTER_SCREEN_WIDTH - frmRecipeSet->Width) / 2;
   //frmRecipeSet->Top = (frnMain->MASTER_SCREEN_HEIGHT - frmRecipeSet->Height) / 2;
   frmRecipeSet->Width = 1890;
   frmRecipeSet->Height = 1024;
   frnMain->pnlCurrentOperation->Caption = "RecipeSet";

   cbCCDFullImageLR->Checked = true;
   if(pcWaferInformation->ActivePage == tsMacro) {
      frnMain->rgJoystickMode->ItemIndex = 1;
   }
   else {
      frnMain->rgJoystickMode->ItemIndex = 0;
   }

   frnMain->pnlMainOperation->Caption = "Macro With Wafer?";
   frnMain->btnMacroWithWaferClick(this);
   if(frnMain->btnMacroWithWafer->Color == clLime) {
      rgWaferStatus->ItemIndex = frnMain->rgSide->ItemIndex + 1;
   }
   else
      rgWaferStatus->ItemIndex = 0;
   frnMain->pnlMainOperation->Caption = "";

   // 2023 2 16a - chc DIC - WLS3200 : boolDIC
   if(frnMain->boolDIC == false) {
      gbDIC->Enabled = false;
      rgObservation->Items->Clear();
      rgObservation->Items->Append("BF");
      rgObservation->Items->Append("DF");
      rgObservation->Columns = 2;
   }

   if(frnMain->RobotMode != ROBOT_MODE_TAZMO) {
      pnlLeftCCD->Visible = false;
      pnlRightCCD->Visible = false;
      cbCCDFullImageLR->Visible = false;
      rgWaferType->Items->Clear();
      rgWaferType->Items->Append("12\"");
      rgWaferType->Items->Append("12\"");

      // 2023 4 5 - chc �S��Macro AutoSave
      cbAutoSaveMacro->Visible = false;

   }
   GetStatus();

   // 2023 5 6 - chc ��s��m
   tm250ms->Enabled = true;

   // 2023 3 2 - chc Init
   if(boolfirst == true) {
      boolfirst = false;

      InitWaferMap();

      int no;
      no = tbBXFMLMP->Position;
      SetIntensity(no);
      // AS=3113
      btnAS6Click(this);
   }

   // 2023 3 20 - chc Single LoadPort
   if(frnMain->LoadPortMode == LOADPORT_SINGLE || frnMain->RobotMode != ROBOT_MODE_TAZMO)
      gbLP2Edge->Visible = false;
   else
      gbLP2Edge->Visible = true;

}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::GetStatus()
{

   // Micro
   rgBXFMLens->ItemIndex =              frnMain->rgBXFMLens->ItemIndex;
   tbBXFMLMP->Position =                frnMain->tbBXFMLMP->Position;
   pnlBXFMLMPValue->Caption =           frnMain->pnlBXFMLMPValue->Caption;
   pnlASValue->Caption =                frnMain->tbBXFMAS->Position;
   rgCCDShutter->ItemIndex =            frnMain->rgCCDShutter->ItemIndex;
   rgMoveStep->ItemIndex =              frnMain->rgMoveStep->ItemIndex;
   rgMoveStep->ItemIndex =              frnMain->rgMoveStep->ItemIndex;
   rgWaferType->ItemIndex =             frnMain->rgCST->ItemIndex;

   // 2023 4 3 - chc Assign cbAF�|Ĳ�oClick
   cbAF->Checked =                      frnMain->boolAFStatus;

   // Macro
   pnlTopMacroLeftLamp->Caption =       frnMain->pnlLED2CH1->Caption;
   pnlTopMacroRightLamp->Caption =      frnMain->pnlLED2CH2->Caption;
   pnlBackMacroLeftLamp->Caption =      frnMain->pnlLED2CH1->Caption;
   pnlBackMacroRightLamp->Caption =     frnMain->pnlLED2CH2->Caption;
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      rgLeftCCDShutter->ItemIndex =     CCDShutterAry[CCD_LEFT];
      rgRightCCDShutter->ItemIndex =    CCDShutterAry[CCD_RIGHT];
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::pcWaferInformationChange(TObject *Sender)
{

   if(pcWaferInformation->ActivePage == tsMacro) {
      frnMain->rgJoystickMode->ItemIndex = 1;
   }
   else {
      frnMain->rgJoystickMode->ItemIndex = 0;
      rgMoveStep->ItemIndex = frnMain->rgMoveStep->ItemIndex;
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::FormClose(TObject *Sender,
      TCloseAction &Action)
{

   frnMain->rgJoystickMode->ItemIndex = 0;
   //Action = caFree;

   // 2023 2 28 - chc frmLogo Visible
   if(frmInspection->Visible == false && frmRecipe->Visible == false && frnMain->pnlMaintenance->Visible == false)
      frmLogo->Visible = true;

}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
int x,y,w,h,leftx,lefty,rightx,righty;

   x = MousePos.x;
   y = MousePos.y;

   // ������Form�W
   int fx = frmRecipeSet->Left;
   int fy = frmRecipeSet->Top;
   x -= fx;
   y -= fy;
   x -= 12;
   y -= 60;

   // Left/Right Light
   w = pnlTopMacroLeftLamp->Width;
   h = pnlTopMacroLeftLamp->Height;
   leftx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlTopMacroLeftLamp->Left;
   lefty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlTopMacroLeftLamp->Top;
   rightx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlTopMacroRightLamp->Left;;
   righty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlTopMacroRightLamp->Top;

   // Down
   if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
      if(frnMain->tbLED2CH1->Position > 1)
         frnMain->tbLED2CH1->Position -= 1;
      else
         frnMain->tbLED2CH1->Position = 0;
      frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
      frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
      pnlTopMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      //pnlBackMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
   }
   else if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
      if(frnMain->tbLED2CH2->Position > 1)
         frnMain->tbLED2CH2->Position -= 1;
      else
         frnMain->tbLED2CH2->Position = 0;
      frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
      frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
      pnlTopMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      //pnlBackMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
   }

   // Back
   // Left/Right Light
   w = pnlBackMacroLeftLamp->Width;
   h = pnlBackMacroLeftLamp->Height;
   leftx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlBackMacroLeftLamp->Left;
   lefty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlBackMacroLeftLamp->Top;
   rightx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlBackMacroRightLamp->Left;;
   righty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlBackMacroRightLamp->Top;

   // Down
   if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
      if(frnMain->tbLED2CH1->Position > 1)
         frnMain->tbLED2CH1->Position -= 1;
      else
         frnMain->tbLED2CH1->Position = 0;
      frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
      frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
      pnlBackMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      //pnlTopMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
   }
   else if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
      if(frnMain->tbLED2CH2->Position > 1)
         frnMain->tbLED2CH2->Position -= 1;
      else
         frnMain->tbLED2CH2->Position = 0;
      frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
      frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
      pnlBackMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      //pnlTopMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
   }


}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
int x,y,w,h,leftx,lefty,rightx,righty;

   x = MousePos.x;
   y = MousePos.y;

   // ������Form�W
   int fx = frmRecipeSet->Left;
   int fy = frmRecipeSet->Top;
   x -= fx;
   y -= fy;
   x -= 12;
   y -= 60;

   // Left/Right Light
   w = pnlTopMacroLeftLamp->Width;
   h = pnlTopMacroLeftLamp->Height;
   leftx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlTopMacroLeftLamp->Left;
   lefty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlTopMacroLeftLamp->Top;
   rightx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlTopMacroRightLamp->Left;;
   righty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlTopMacroRightLamp->Top;

   // Up
   if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
      if(frnMain->tbLED2CH1->Position < 254)
         frnMain->tbLED2CH1->Position += 1;
      else
         frnMain->tbLED2CH1->Position = 255;
      frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
      frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
      pnlTopMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      //pnlBackMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
   }
   else if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
      if(frnMain->tbLED2CH2->Position < 254)
         frnMain->tbLED2CH2->Position += 1;
      else
         frnMain->tbLED2CH2->Position = 255;
      frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
      frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
      pnlTopMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      //pnlBackMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
   }

   // Back
   // Left/Right Light
   w = pnlBackMacroLeftLamp->Width;
   h = pnlBackMacroLeftLamp->Height;
   leftx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlBackMacroLeftLamp->Left;
   lefty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlBackMacroLeftLamp->Top;
   rightx = pcRecipeSet->Left + Panel1->Left + 4 + pcWaferInformation->Left + pnlBackMacroRightLamp->Left;;
   righty = pcRecipeSet->Top + Panel1->Top + 30 + pcWaferInformation->Top + pnlBackMacroRightLamp->Top;

   // Up
   if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
      if(frnMain->tbLED2CH1->Position < 254)
         frnMain->tbLED2CH1->Position += 1;
      else
         frnMain->tbLED2CH1->Position = 255;
      frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
      frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
      pnlBackMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      //pnlTopMacroLeftLamp->Caption = frnMain->tbLED2CH1->Position;
      frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
   }
   else if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
      if(frnMain->tbLED2CH2->Position < 254)
         frnMain->tbLED2CH2->Position += 1;
      else
         frnMain->tbLED2CH2->Position = 255;
      frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
      frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
      pnlBackMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      //pnlTopMacroRightLamp->Caption = frnMain->tbLED2CH2->Position;
      frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::rgLeftCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 1;
   frnMain->rgCCDShutter->ItemIndex = rgLeftCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::rgRightCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 2;
   frnMain->rgCCDShutter->ItemIndex = rgRightCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::pnlRecipeLoadClick(TObject *Sender)
{
AnsiString fname;

   // 2023 5 9 - chc �令Recipe��Load
   CreateRecipeID();

   // 2023 4 1 - chc �����Ѹ��Jtxt��
   return;

   opTxtFile->InitialDir = edWaferTxtPath->Text;
   if(opTxtFile->Execute()) {
      fname = opTxtFile->FileName;
      if(FileExists(fname)) {
         LoadTXTFile(fname);
      }
   }
}
//---------------------------------------------------------------------------
// Macro Save
// Macro
/*
struct SIDE_STRU {
   int LoadPosition;
   int HomeDuration;
   int LeftDuration;
   int RightDuration;
   int ViewDuration;
   int LeftLightValue;
   int RightLightValue;
   int LeftCCDShutter;
   int RightCCDShutter;
};
struct MACRO_RECIPE_STRU {
   struct SIDE_STRU Top;
   struct SIDE_STRU Back;
};
// Recipe
struct RECIPE_STRU {
   AnsiString Name;
   int WaferSize;                                                               // 8/12
   struct MICRO_RECIPE_STRU MicroRecipe;
   struct MACRO_RECIPE_STRU MacroRecipe;
};
*/
// RecipeBufferEdit
void __fastcall TfrmRecipeSet::btnSaveMacroParametersClick(TObject *Sender)
{
TIniFile *pSystemFile;
AnsiString recipefname,recipename;

   recipename = combRecipe->Text;
   recipefname = edRecipePath->Text + "\\" + recipename + ".ini";
   pSystemFile = new TIniFile(recipefname);

   // 2023 3 26 - chc SECS: Modify(Save) Recipe
   frnMain->edSECSRecipeID->Text = combRecipe->Text;
   if(FileExists(recipefname)) {
      frnMain->rgSECSRecipe->ItemIndex = RECIPE_MODIFY;
      frnMain->WriteSystemLog("SECS: Macro Recipe Modified " + combRecipe->Text);
   }
   else {
      frnMain->rgSECSRecipe->ItemIndex = RECIPE_CREATE;
      frnMain->WriteSystemLog("SECS: Macro Recipe Saved " + combRecipe->Text);
   }
   frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);

   // Top
   RecipeBufferEdit.MacroRecipe.Top.LoadPosition = rgMacroTopLoadPosition->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Top.HomeDuration = edMacroTopHomeDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Top.LeftDuration = edMacroTopLeftDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Top.RightDuration = edMacroTopRightDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Top.ViewDuration = edMacroTopViewDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Top.LeftCCDShutter = rgLeftCCDShutter->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Top.RightCCDShutter = rgRightCCDShutter->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Top.LeftLightValue = pnlTopMacroLeftLamp->Caption.ToInt();
   RecipeBufferEdit.MacroRecipe.Top.RightLightValue = pnlTopMacroRightLamp->Caption.ToInt();
   // Write
   pSystemFile->WriteInteger("Macro-Top","LoadPosition"     ,RecipeBufferEdit.MacroRecipe.Top.LoadPosition);
   pSystemFile->WriteInteger("Macro-Top","HomeDuration"     ,RecipeBufferEdit.MacroRecipe.Top.HomeDuration);
   pSystemFile->WriteInteger("Macro-Top","LeftDuration"     ,RecipeBufferEdit.MacroRecipe.Top.LeftDuration);
   pSystemFile->WriteInteger("Macro-Top","RightDuration"    ,RecipeBufferEdit.MacroRecipe.Top.RightDuration);
   pSystemFile->WriteInteger("Macro-Top","ViewDuration"     ,RecipeBufferEdit.MacroRecipe.Top.ViewDuration);
   pSystemFile->WriteInteger("Macro-Top","LeftCCDShutter"   ,RecipeBufferEdit.MacroRecipe.Top.LeftCCDShutter);
   pSystemFile->WriteInteger("Macro-Top","RightCCDShutter"  ,RecipeBufferEdit.MacroRecipe.Top.RightCCDShutter);
   pSystemFile->WriteInteger("Macro-Top","LeftLightValue"   ,RecipeBufferEdit.MacroRecipe.Top.LeftLightValue);
   pSystemFile->WriteInteger("Macro-Top","RightLightValue"  ,RecipeBufferEdit.MacroRecipe.Top.RightLightValue);

   // Back
   RecipeBufferEdit.MacroRecipe.Back.LoadPosition = rgMacroBackLoadPosition->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Back.HomeDuration = edMacroBackHomeDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Back.LeftDuration = edMacroBackLeftDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Back.RightDuration = edMacroBackRightDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Back.ViewDuration = edMacroBackViewDuration->Text.ToInt();
   RecipeBufferEdit.MacroRecipe.Back.LeftCCDShutter = rgLeftCCDShutter->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Back.RightCCDShutter = rgRightCCDShutter->ItemIndex;
   RecipeBufferEdit.MacroRecipe.Back.LeftLightValue = pnlBackMacroLeftLamp->Caption.ToInt();
   RecipeBufferEdit.MacroRecipe.Back.RightLightValue = pnlBackMacroRightLamp->Caption.ToInt();
   // Write
   pSystemFile->WriteInteger("Macro-Back","LoadPosition"     ,RecipeBufferEdit.MacroRecipe.Back.LoadPosition);
   pSystemFile->WriteInteger("Macro-Back","HomeDuration"     ,RecipeBufferEdit.MacroRecipe.Back.HomeDuration);
   pSystemFile->WriteInteger("Macro-Back","LeftDuration"     ,RecipeBufferEdit.MacroRecipe.Back.LeftDuration);
   pSystemFile->WriteInteger("Macro-Back","RightDuration"    ,RecipeBufferEdit.MacroRecipe.Back.RightDuration);
   pSystemFile->WriteInteger("Macro-Back","ViewDuration"     ,RecipeBufferEdit.MacroRecipe.Back.ViewDuration);
   pSystemFile->WriteInteger("Macro-Back","LeftCCDShutter"   ,RecipeBufferEdit.MacroRecipe.Back.LeftCCDShutter);
   pSystemFile->WriteInteger("Macro-Back","RightCCDShutter"  ,RecipeBufferEdit.MacroRecipe.Back.RightCCDShutter);
   pSystemFile->WriteInteger("Macro-Back","LeftLightValue"   ,RecipeBufferEdit.MacroRecipe.Back.LeftLightValue);
   pSystemFile->WriteInteger("Macro-Back","RightLightValue"  ,RecipeBufferEdit.MacroRecipe.Back.RightLightValue);

   // Auto Save
   RecipeBufferEdit.MacroRecipe.boolAutoSave = cbAutoSaveMacro->Checked;
   // Write
   pSystemFile->WriteBool("Macro","boolAutoSave"     ,RecipeBufferEdit.MacroRecipe.boolAutoSave);

   // Name/Date/Time: TestRecipeForBE 2022/12/05 11:20
   DateTimeToString(RecipeBufferEdit.Date,"yyyy\/mm\/dd",Now());
   DateTimeToString(RecipeBufferEdit.Time,"hh\:mm",Now());
   RecipeBufferEdit.Name = recipename;
   pSystemFile->WriteString("Recipe","Date"     ,RecipeBufferEdit.Date);
   pSystemFile->WriteString("Recipe","Time"     ,RecipeBufferEdit.Time);
   pSystemFile->WriteString("Recipe","Name"     ,RecipeBufferEdit.Name);

   pSystemFile->UpdateFile();

   delete pSystemFile;

}
//---------------------------------------------------------------------------
//ClassLookup 253		=> 253 ���O, This is a table of index numbers. Each index number is associated with a description of a defect
//0 "No Revision"
//1 "1"
//2 "2"
AnsiString IDName[256];
int IDNo[256];
int IDCount = 0;
int MapCode[1000][1000];
// fname: include path
void __fastcall TfrmRecipeSet::LoadTXTFile(AnsiString fname)
{
FILE *pFile;
AnsiString str,lotid,slotno,diepitch,diewidth,dieheight,dieorgx,dieorgy,dieorg,defectlist,tmp,sizeno,sizetype;
char buf[500],sx[20],sy[20],sw[20],sh[20],sa[20],sd[20];
int no,len,pos,wafetype;
int rno,rid,rc,rw;
double rfx,rfy,rfw,rfh,rfa,rfd;
int defectno = 0;
AnsiString sid,sname;

   IDCount = 0;
   pFile = fopen(fname.c_str(),"r");
   if(pFile) {
      rewind(pFile);
      while(!feof(pFile)) {
         if(fgets(buf,200,pFile) > 0) {
            str.sprintf("%s",buf);
            len = str.Length();
            len--;
            // LotID
            pos = str.Pos("LotID ");
            // LotID "ZP1612021";
            // 12345678901234567890
            if(pos > 0) {
               lotid = str.SubString(pos+7,len-7-2);
               edLotID->Text = lotid;
               continue;
            }
            // SlotNo
            pos = str.Pos("Slot ");
            // Slot 1;
            // 12345678901234567890
            if(pos > 0) {
               slotno = str.SubString(pos+5,len-5-1);
               edSlotNo->Text = slotno;
               continue;
            }
            // DiePitch : W H
            pos = str.Pos("DiePitch ");
            // DiePitch 3429 3164;
            // 3429 3164
            // 12345678901234567890
            if(pos > 0) {
               diepitch = str.SubString(pos+9,len-9-1);
               len = diepitch.Length();
               pos = diepitch.Pos(" ");
               if(pos > 0) {
                  diewidth = diepitch.SubString(1,pos-1);
                  dieheight = diepitch.SubString(pos+1,len-pos);
                  edChipWidth->Text = diewidth;
                  edChipHeight->Text = dieheight;
               }
               continue;
            }
            // DieOrigin 0.000000 0.000000;
            pos = str.Pos("DieOrigin ");
            // DieOrigin 0.000000 0.000000;
            // 0.000000 0.000000
            // 12345678901234567890
            if(pos > 0) {
               dieorg = str.SubString(pos+10,len-10-1);
               len = dieorg.Length();
               pos = dieorg.Pos(" ");
               if(pos > 0) {
                  dieorgx = dieorg.SubString(1,pos-1);
                  dieorgy = dieorg.SubString(pos+1,len-pos);
                  edChipDx->Text = dieorgx;
                  edChipDy->Text = dieorgy;
               }
               continue;
            }
            // Size
            pos = str.Pos("SampleSize ");
            // SampleSize 1 200;
            // 0.000000 0.000000
            // 12345678901234567890
            if(pos > 0) {
               dieorg = str.SubString(pos+11,len-11-1);
               len = diepitch.Length();
               pos = diepitch.Pos(" ");
               if(pos > 0) {
                  sizeno = dieorg.SubString(1,pos-1);
                  sizetype = dieorg.SubString(pos+1,len-pos);
                  wafetype = 12;
                  if(sizetype == "200") {
                     wafetype = 8;
                     rgWaferType->ItemIndex = 1;
                  }
                  else if(sizetype == "300") {
                     wafetype = 12;
                     rgWaferType->ItemIndex = 0;
                  }
               }
               continue;
            }

            //DefectList
            //1 1332.5 1332.5 1 9 19;
            //TiffFileName ZP1612010_8_PSS_2_1.JPG;
            //123456789012345678901234567890
            // 2023 2 24 - chc �s���o���: 13��
            // DefectRecordSpec 13 DEFECTID XREL YREL XINDEX YINDEX XSIZE YSIZE DEFECTAREA DSIZE CLASSNUMBER TEST IMAGECOUNT IMAGELIST;
            //                        1       2   3      4     5      6     7        8       9       10       11       12       13
            //TiffFileName 0213-NEGINCOME-T2_04_EagleTP_107236_1.jpg;
            //DefectList									*
            // 1 659.55611 3090.16102 13 -7 25.44 45.792 899.599104 52.384 237 1 1 1 1 0;
            pos = str.Pos("DefectList");
            if(pos > 0) {
               //int rno,rid,rc,rw;
               //double rfx,rfy;
               if(fgets(buf,200,pFile) > 0) {
                  str.sprintf("%s",buf);
                  len = str.Length();
                  len--;
                  len = strlen(buf);
                  buf[len-1] = '\0';

                  // 2023 2 28 - chc �[�JDefect W/H/A
                  //if(sscanf(buf,"%d %s %s %d %d %d\n",&rno,sx,sy,&rid,&rc,&rw) != NULL) {
                  if(sscanf(buf,"%d %s %s %d %d %s %s %s %s %d\n",&rno,sx,sy,&rc,&rw,sw,sh,sa,sd,&rid) != NULL) {

                     if(defectno == 0) {
                        // BINCode Define
                        sgDefectList->ColWidths[0] = 30;
                        sgDefectList->ColWidths[1] = 30;
                        sgDefectList->ColWidths[2] = 50;
                        sgDefectList->ColWidths[3] = 50;
                        sgDefectList->ColWidths[4] = 35;
                        sgDefectList->ColWidths[5] = 35;
                        sgDefectList->ColWidths[6] = 40;

                        // 2023 2 28 - chc �[�JDefect W/H/A
                        sgDefectList->ColWidths[7] = 60;
                        sgDefectList->ColWidths[8] = 60;
                        sgDefectList->ColWidths[9] = 65;

                        // 2023 4 7 - chc �[�JLens/Intensity
                        sgDefectList->ColWidths[10] = 40;
                        sgDefectList->ColWidths[11] = 40;

                        // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
                        sgDefectList->ColWidths[12] = 40;
                        sgDefectList->ColWidths[13] = 40;

                        sgDefectList->Cells[0][0] = "No";
                        sgDefectList->Cells[1][0] = "ID";
                        sgDefectList->Cells[2][0] = "X";
                        sgDefectList->Cells[3][0] = "Y";
                        sgDefectList->Cells[4][0] = "Col";
                        sgDefectList->Cells[5][0] = "Row";
                        sgDefectList->Cells[6][0] = "Judge";

                        // 2023 2 28 - chc �[�JDefect W/H/A
                        sgDefectList->Cells[7][0] = "XSize";
                        sgDefectList->Cells[8][0] = "YSize";
                        sgDefectList->Cells[9][0] = "Area";

                        // 2023 4 7 - chc �[�JLens/Intensity
                        sgDefectList->Cells[10][0] = "Lens";
                        sgDefectList->Cells[11][0] = "Inten.";

                        // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
                        sgDefectList->Cells[12][0] = "Obser.";
                        sgDefectList->Cells[13][0] = "DICVa.";

                        sgDefectList->RowCount = 2;

                        // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
                        /*
                        sgDefectList->Cells[0][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[1][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[2][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[3][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[4][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[5][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[6][sgDefectList->RowCount-1] = "";

                        // 2023 2 28 - chc �[�JDefect W/H/A
                        sgDefectList->Cells[7][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[8][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[9][sgDefectList->RowCount-1] = "";

                        // 2023 4 7 - chc �[�JLens/Intensity
                        sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
                        */
                        for(int i=0 ; i<sgDefectList->ColCount ; i++)
                           sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

                     }
                     sgDefectList->Cells[0][sgDefectList->RowCount-1] = IntToStr(rno);
                     sgDefectList->Cells[1][sgDefectList->RowCount-1] = rid;
                     rfx = atof(sx);
                     tmp.sprintf("%.1f",rfx);
                     sgDefectList->Cells[2][sgDefectList->RowCount-1] = tmp;
                     rfy = atof(sy);
                     tmp.sprintf("%.1f",rfy);
                     sgDefectList->Cells[3][sgDefectList->RowCount-1] = tmp;
                     sgDefectList->Cells[4][sgDefectList->RowCount-1] = rc;
                     sgDefectList->Cells[5][sgDefectList->RowCount-1] = rw;

                     // 2023 2 28 - chc �[�JDefect W/H/A
                     rfw = atof(sw);
                     tmp.sprintf("%.3f",rfw);
                     sgDefectList->Cells[7][sgDefectList->RowCount-1] = tmp;
                     rfh = atof(sh);
                     tmp.sprintf("%.3f",rfh);
                     sgDefectList->Cells[8][sgDefectList->RowCount-1] = tmp;
                     rfa = atof(sa);
                     tmp.sprintf("%.3f",rfa);
                     sgDefectList->Cells[9][sgDefectList->RowCount-1] = tmp;

                     // 2023 4 7 - chc �[�JLens/Intensity
                     sgDefectList->Cells[10][sgDefectList->RowCount-1] = rgBXFMLens->ItemIndex;
                     sgDefectList->Cells[11][sgDefectList->RowCount-1] = tbBXFMLMP->Position;

                     // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
                     sgDefectList->Cells[12][sgDefectList->RowCount-1] = rgObservation->ItemIndex;
                     sgDefectList->Cells[13][sgDefectList->RowCount-1] = tbDICFC->Position;

                     MapCode[rw][rc] = rid;
                     sgDefectList->RowCount++;
                     defectno++;
                  }
               }
               continue;
            }

            // BincodeList
            //ClassLookup 253		=> 253 ���O, This is a table of index numbers. Each index number is associated with a description of a defect
            //12345678901234567890
            //0 "No Revision"
            //   123456789012
            //1 "1"
            //2 "2"
            //...
            //252 "Width-500";
            pos = str.Pos("ClassLookup");
            if(pos > 0) {
               str.Delete(1,pos+11);
               len = str.Length();
               str = str.SubString(1,len-1);
               no = str.ToInt();
               IDCount = 0;
               for(int i=0 ; i<no ; i++) {
                  if(fgets(buf,200,pFile) > 0) {
                     str.sprintf("%s",buf);
                     len = str.Length();
                     pos = str.Pos(" ");
                     if(pos > 0) {
                        sid = str.SubString(1,pos-1);
                        str.Delete(1,pos+1);
                        pos = str.Pos("\"");
                        if(pos > 0) {
                           IDCount++;
                           sname = str.SubString(1,pos-1);
                           IDNo[i] = sid.ToInt();
                           IDName[i] = sname;
                        }
                     }
                  }
               }
               continue;
            }

         }
      }
      fclose(pFile);
      SetupBINCode();

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbStdPatternClick(TObject *Sender)
{

   shCenter->Visible = cbStdPattern->Checked;
   shLeft->Visible = cbStdPattern->Checked;
   shRight->Visible = cbStdPattern->Checked;
   shUp->Visible = cbStdPattern->Checked;
   shDown->Visible = cbStdPattern->Checked;
   shLeftUp->Visible = cbStdPattern->Checked;
   shLeftDown->Visible = cbStdPattern->Checked;
   shRightUp->Visible = cbStdPattern->Checked;
   shRightDown->Visible = cbStdPattern->Checked;
   if(cbStdPattern->Checked == true) {
      UpdateCenterDotPosition();
      // �j�Partial Image
      if(cbCCDFullImage->Checked == true) {
         cbCCDFullImage->Checked = false;
      }
   }
}
//---------------------------------------------------------------------------
// 2023 1 5 - chc ����Center Pattern��
void __fastcall TfrmRecipeSet::UpdateCenterDotPosition()
{
int x1,y1,x2,y2,w,h,cx,cy;

   x1 = shCenter->Left;
   y1 = shCenter->Top;
   x2 = shCenter->Left + shCenter->Width;
   y2 = shCenter->Top + shCenter->Height;
   // �]�w�U�վ��I��m
   cx = (x1+x2) / 2;
   cy = (y1+y2) / 2;
   w = shLeft->Width;
   h = shLeft->Height;
   // ��
   shLeft->Left = x1 - w;
   shLeft->Top = cy - h/2;
   // ���W
   shLeftUp->Left = x1 - w;
   shLeftUp->Top = y1 - h;
   // ���U
   shLeftDown->Left = x1 - w;
   shLeftDown->Top = y2;
   // �k
   shRight->Left = x2;
   shRight->Top = cy - h/2;
   // �k�W
   shRightUp->Left = x2;
   shRightUp->Top = y1 - h;
   // �k�U
   shRightDown->Left = x2;
   shRightDown->Top = y2;
   // �W
   shUp->Left = cx - w/2;
   shUp->Top = y1 - h;
   // �U
   shDown->Left = cx - w/2;
   shDown->Top = y2;

   shLeft->Refresh();
   shLeftUp->Refresh();
   shLeftDown->Refresh();
   shRight->Refresh();
   shRightUp->Refresh();
   shRightDown->Refresh();
   shUp->Refresh();
   shDown->Refresh();

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMatchToCenterClick(TObject *Sender)
{

   if(shCenter->Visible == false)
      return;

   shCenter->Left = shVertical->Left - shCenter->Width/2;
   shCenter->Top = shHorizontal->Top - shCenter->Height/2;
   UpdateCenterDotPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnEraseMatchBoxClick(TObject *Sender)
{

   if(shMarkMatch->Visible == true)
      shMarkMatch->Visible = false;
   else
      shMarkMatch->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSavePatternClick(TObject *Sender)
{
AnsiString recipename,path,filename,name,msg;
int left,top,width,height,ccdno,no;

   ccdno = 0;
   // �q���I: �Y�����w��, �����]������1/5 - 4/5
   bool boolset = false;
   if(true) {
      if(shCenter->Visible == false) {
         boolset = true;
         cbStdPattern->Checked = true;
         int x1,y1,w,h;
         w = imInspection->Width;
         h = imInspection->Height;
         x1 = w / 5;
         y1 = h / 5;
         x1 += imInspection->Left;
         y1 += imInspection->Top;
         shCenter->Left = x1;
         shCenter->Top = y1;
         shCenter->Width = (w*3) / 5;
         shCenter->Height = (h*3) / 5;
         cbStdPatternClick(this);
      }
   }

   if(shCenter->Visible == true) {
      // Save XYZ Position
      UpdateMotionPosition();
      if(rgMatchSource->ItemIndex == 0) {
         pnlBase1X->Caption = pnlX->Caption;
         pnlBase1Y->Caption = pnlY->Caption;
      }
      else if(rgMatchSource->ItemIndex == 1) {
         pnlBase2X->Caption = pnlX->Caption;
         pnlBase2Y->Caption = pnlY->Caption;
      }
      else if(rgMatchSource->ItemIndex == 2) {
         pnlPositionX->Caption = pnlX->Caption;
         pnlPositionY->Caption = pnlY->Caption;
      }
      pnlStdZ->Caption = pnlZ->Caption;
      pnlStdT->Caption = pnlT->Caption;

      left = shCenter->Left;
      top = shCenter->Top;
      width = shCenter->Width;
      height = shCenter->Height;
      msg.sprintf("Save: X,Y,W,H-CX,CY-L,T=%d,%d,%d,%d-%d,%d-%d,%d",left,top,width,height,LaserCenterX,LaserCenterY,imInspection->Left,imInspection->Top);
      frnMain->WriteSystemLog(msg);
      left -= imInspection->Left;
      top -= imInspection->Top;
      left += LaserCenterX;
      top += LaserCenterY;
      if(frnMain->CCDInfoAry[ccdno].Type == CCD_BW)
         frnMain->PatternROI.Detach();
      else
         frnMain->CPatternROI.Detach();

      // �Ƕ�: for test
      if(frnMain->boolForTest == false || boolCCDImageLoadedAry[ccdno] == true) {
         if(frnMain->CCDInfoAry[ccdno].Type == CCD_BW) {
            frnMain->PatternROI.Attach(&frnMain->CCDImageAry[ccdno]);
            frnMain->PatternROI.SetPlacement(left,top,width,height);
         }
         else {
            frnMain->CPatternROI.Attach(&frnMain->usb_ImageAry[ccdno]);
            frnMain->CPatternROI.SetPlacement(left,top,width,height);
         }
      }

      // By Recipe: combRecipe->Text
      no = combRecipe->ItemIndex;
      if(no < 0) {
         no = 0;
         combRecipe->ItemIndex = 0;
      }
      recipename = combRecipe->Text;

      // �YPath�|���إ�, �n�۰ʲ���
      path = edRecipePath->Text + "\\" + recipename;
      if(!DirectoryExists(path))
         mkdir(path.c_str());
      path = edRecipePath->Text + "\\" + recipename + "\\";

      if(rgMatchSource->ItemIndex == 0) {
         filename = recipename + "-B1.bmp";
         name = "Base1";
      }
      else if(rgMatchSource->ItemIndex == 1) {
         filename = recipename + "-B2.bmp";
         name = "Base2";
      }
      else if(rgMatchSource->ItemIndex == 2) {
         filename = recipename + "-P.bmp";
         name = "Position";
      }

      // 2019 12 3 - chc �߰ݬO�_�s��?
      if(Application->MessageBox(("�T�{�x�s[" + name + "]���зǼv��["+ filename + "]? ").c_str(), "�зǼv�����x�s�@�~", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         return;
      }

      // for test
      if(frnMain->boolForTest == false || boolCCDImageLoadedAry[ccdno] == true) {
         try {
            if(frnMain->CCDInfoAry[ccdno].Type == CCD_BW)
               frnMain->PatternROI.Save((path+filename).c_str(),E_FILE_FORMAT_GRAY_LEVEL_BMP);
            else
               frnMain->CPatternROI.Save((path+filename).c_str(),E_FILE_FORMAT_COLOR_BMP);
         }
         catch(Exception &e) {
            frnMain->WriteSystemLog(">>Error(btnSavePatternClick): " + e.Message);
            Application->MessageBox(("�v���ɦs�ɥ���: " + path + filename).c_str(),"�q���t��: �v���ɲ��ͧ@�~",MB_ICONINFORMATION|MB_SYSTEMMODAL|MB_OK);
            return;
         }
      }
      pnlSystemMessage->Caption = filename + " �s�ɧ���.";
      frnMain->WriteSystemLog("Save Pattern: " + path + filename);

      // 2023 4 24 - chc �sMatch: Intensity & Lens
      if(rgMatchSource->ItemIndex == 0) {
         RecipeBufferEdit.MicroRecipe.Pattern.Intensity = tbBXFMLMP->Position;
         RecipeBufferEdit.MicroRecipe.Pattern.Lens = rgBXFMLens->ItemIndex;
      }

      // 2023 5 4 - chc Alignment Position
      if(rgMatchSource->ItemIndex == 2) {
         RecipeBufferEdit.MicroRecipe.Pattern.PositionIntensity = tbBXFMLMP->Position;
      }

      // �������
      if(boolset == true)
         cbStdPattern->Checked = false;
   }

}
//---------------------------------------------------------------------------
int DoubleClickX,DoubleClickY;

void __fastcall TfrmRecipeSet::imInspectionDblClick(TObject *Sender)
{
int level;
double fdy,ccdprecision,fdx;
int axisno,position;
I32 cmd;
AnsiString msg;
int cx,cy,X,Y;
double ratiox,ratioy;

   frnMain->WriteSystemLog("imInspectionDblClick() Enter...");

   // �n����, �קK����
   imInspection->Enabled = false;

   // ���v�����
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImage->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imInspection->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imInspection->Height;
   }

   // X/Y position(set by MouseDown Event)
   X = DoubleClickX;
   Y = DoubleClickY;

   cx = imInspection->Width / 2;
   cy = imInspection->Height / 2;

   cx = shVertical->Left - imInspection->Left;
   cy = shHorizontal->Top - imInspection->Top;

   pnlSystemMessage->Caption = "Bouble Click: XYMoving...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   // Y�b
   frnMain->pnlYMove->Color = clSilver;
   level = shHorizontal->Top;                                                   // 488
   ccdprecision = GetLensPrecision();

   if(cbCCDFullImage->Checked == true)
      ccdprecision *= ((double)frnMain->CCDInfoAry[0].Width / imInspection->Width);

   fdy = (Y - (level - imInspection->Top)) * ccdprecision;

   // ���v�����
   // 2023 3 20 - chc ��פw���L
   //fdy *= ratioy;

   msg.sprintf("Y,level,top,fy,dy= %d,%d,%d,%.4f,%.4f",Y,level,imInspection->Top,fdy,ccdprecision);
   frnMain->WriteSystemLog(msg);

   if(frnMain->cbCCDYDirection->Checked == true)
      fdy = 0 - fdy;
   msg.sprintf("%.3f",fdy);
   frnMain->pnlYAltOffset->Caption = msg;
   // Y - absolute
   axisno = Y_AXIS;

   GetPosition(axisno, &cmd);
   position = cmd + (fdy / frnMain->Y_RESOLUTION);
   if(MoveToY(position) == false) {
      pnlSystemMessage->Caption = "MouseDoubleClick: YMove Fail!";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      frnMain->pnlYMove->Color = clRed;
      // Beep�n
      Beep(500);
      frnMain->pnlAlarmMessage->Caption = " Y-Axis Move Error!";
      frnMain->AddPerformanceMessage(frnMain->pnlAlarmMessage->Caption);
   }
   else {
      frnMain->pnlYMove->Color = clLime;
      pnlSystemMessage->Caption = "MouseDoubleClick: YMove Success";
   }

   // X�b
   frnMain->pnlXMove->Color = clSilver;
   level = shVertical->Left;                                                    // 603
   fdx = (X - (level - imInspection->Left)) * ccdprecision;

   // ���v�����
   // 2023 3 20 - chc ��פw���L
   //fdx *= ratiox;

   msg.sprintf("X,level,left,fx,dx= %d,%d,%d,%.4f,%.4f",X,level,imInspection->Left,fdx,ccdprecision);
   frnMain->WriteSystemLog(msg);
   if(frnMain->cbCCDXDirection->Checked == true)
      fdx = 0 - fdx;
   msg.sprintf("%.3f",fdx);
   frnMain->pnlXAltOffset->Caption = msg;
   // X - absolute
   axisno = X_AXIS;

   GetPosition(axisno, &cmd);
   position = cmd + (fdx / frnMain->X_RESOLUTION);
   if(MoveToX(position) == false) {
      pnlSystemMessage->Caption = "MouseDoubleClick: XMove Fail!";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      frnMain->pnlXMove->Color = clRed;
      // Beep�n
      Beep(500);
      frnMain->pnlAlarmMessage->Caption = " X-Axis Move Error!";
      frnMain->AddPerformanceMessage(frnMain->pnlAlarmMessage->Caption);
   }
   else {
      frnMain->pnlXMove->Color = clLime;
      pnlSystemMessage->Caption = "MouseDoubleClick: XMove Success";
   }

   // �j�����X�b
   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;

   // �n����, �קK����
   imInspection->Enabled = true;
}
//---------------------------------------------------------------------------
// GetLensPrecision
double __fastcall TfrmRecipeSet::GetLensPrecision()
{
double ccdprecision;
int ccdno;

   ccdno = frnMain->rgBXFMLens->ItemIndex + 1;
   if(ccdno == 1)
      ccdprecision = frnMain->edCCDPrecision1->Text.ToDouble();
   else if(ccdno == 2)
      ccdprecision = frnMain->edCCDPrecision2->Text.ToDouble();
   else if(ccdno == 3)
      ccdprecision = frnMain->edCCDPrecision3->Text.ToDouble();
   else if(ccdno == 4)
      ccdprecision = frnMain->edCCDPrecision4->Text.ToDouble();
   else if(ccdno == 5)
      ccdprecision = frnMain->edCCDPrecision5->Text.ToDouble();

   // ��׭����
   AnsiString msg;
   msg.sprintf("%.4f",ccdprecision);
   pnlLensPrecision->Caption = msg;

   return(ccdprecision);
}
//---------------------------------------------------------------------------
int ImageSX = 0;
int ImageSY = 0;
int ImageXPos,ImageYPos;
bool ImageLeftMouse = false;
bool boolFormWheel = false;

void __fastcall TfrmRecipeSet::imInspectionMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   DoubleClickX = X;
   DoubleClickY = Y;

   // 2023 3 17 - chc ImageLeftMouse
   ImageXPos = X;
   ImageYPos = Y;
   if(Button == mbLeft) {
      ImageLeftMouse = true;
   }

}
//---------------------------------------------------------------------------
bool boolDrawImage = false;
void __fastcall TfrmRecipeSet::btnWaferMapDrawClick(TObject *Sender)
{

   DrawBigImage();

   // 2023 5 6 - chc �bDrawBigImage���]�w
   //boolDrawImage = true;

   // 2023 3 20 - chc Draw Sub die
   DrawSubDie();

}
//---------------------------------------------------------------------------
TColor IDColor[16] = {clBlack,clMaroon,clGreen,clOlive,clNavy,clPurple,clTeal,clGray,
                      clSilver,clRed,clLime,clYellow,clBlue,clFuchsia,clAqua,clWhite};
// 2022 8 20 - chc ����Load Big Image
// 12": 304.8mm
// Wafer: 708*708
// 300mm(12") => 423um/Pixel
// 5000*5000 => 11*11
//
// .�sdx,sx,dy,sy
int MapOutDx;
int MapOutSx;
int MapOutDy;
int MapOutSy;
//int MapCode[1000][1000];
#define MAX_ROW_NO      1000
#define MAX_COL_NO      1000
// ���I��m
int GridNoX;
int GridNoY;
// ����Die�y��
int CenterDieRow = -1;
int CenterDieCol = -1;
// �̤pRow,Col
int MinRow = -1;
int MinCol = -1;
int MaxRow = -1;
int MaxCol = -1;
//
int MapX,MapY;
// �����q
int COffsetX,COffsetY;

// 5 - ���w��m,��
// 4 - �q���I(��)
// 3 - Testable(��)
// 2 - Type(2-half),��
// 1 - Type(1-Out),��
// 0 - Initial
// type: 0-Inner, 1-Out, 2-half
//
void __fastcall TfrmRecipeSet::DrawBigImage_new()
{
int no,width,height,sx,sy,x,y,w,h,cx,cy,dx,dy,bw,bh,iw,ih,bx,by,ncx,ncy;
int maxno,row,col,id;
AnsiString fname,msg;

//DoubleBuffered = true;
//sbWaferMap->DoubleBuffered = true;

TRect Rect;
int factor = 2;

// Die���_�I: ��Die��V�M�w
int diex,diey;
static bool boolfirst = true;

// �Ĥ@���I�sInMeasure
bool boolinmeasure = true;

// �̤pRow,Col
int minrow,mincol,maxrow,maxcol,minsx,minsy,minex,miney;
minrow = -1;
mincol = -1;
maxrow = -1;
maxcol = -1;

// 2023 1 2 - chc ��
int maxsy,maxey;

   if(boolfirst == true) {
      //boolfirst = false;
      for(int i=0 ; i<MAX_ROW_NO ; i++) {
         for(int j=0 ; j<MAX_COL_NO ; j++) {
            MapCode[i][j] = 0;
         }
      }
   }

   factor = 1;
   imWaferMap->Picture->Bitmap->PixelFormat = pf4bit;

   // 2023 5 6 - chc �bDrawBigImage���]�w
   boolDrawImage = true;

   // bw,bh: pnlWaferMap���j�p
   //bw = sbWaferMap->Width;
   //bh = sbWaferMap->Height;
   bw = pnlWaferMap->Width;
   bh = pnlWaferMap->Height;

   // 304800 / 5000 = 60.96 => 61��, ���t2��@65�� => �e�b1207*921��
   // ���b850�H�� => 850/65=13.07, �����13�I
   // ��l�v��: 65*13=845*845
   int gridnox,gridnoy,diew,dieh,ix,iy;
   double fx,fy;

   // �令�p�ƤΥ[�J���ιD: edDieSizeGapX
   diew = edChipWidth->Text.ToDouble() + edChipDx->Text.ToDouble();
   dieh = edChipHeight->Text.ToDouble() + edChipDy->Text.ToDouble();

   // �O300, ���Y6(���k�U3mm)
   double diameter = 300000;

   // ignore�令30000
   double ignore = edIgnore->Text.ToInt();
   double realr = diameter - (ignore * 2);

   // Wafer��m�T�w���m��
   if(true) {
      gridnox = (realr - diew) / diew + 1;
      gridnoy = (realr - dieh) / dieh + 1;
      msg.sprintf("realr,diew,dieh,nox,noy=%.1f,%d,%d,%d,%d",realr,diew,dieh,gridnox,gridnoy);
      frnMain->WriteSystemLog(msg);

      // 2023 3 20 - chc ���B�z
      /*
      int tmp;
      tmp = realr - diew;
      tmp = tmp / diew;
      tmp /= 2;
      tmp *= 2;
      tmp++;
      gridnox = tmp;
      tmp = realr - dieh;
      tmp = tmp / dieh;
      tmp /= 2;
      tmp *= 2;
      tmp++;
      gridnoy = tmp;
      // �~�X
      gridnox += 2;
      gridnoy += 2;
      */
      
   }
   // ���W
   else if(rgWaferDieDirection->ItemIndex == 1) {
      gridnox = realr / diew;
      gridnoy = realr / dieh;
   }
   // ���U
   else if(rgWaferDieDirection->ItemIndex == 2) {
      gridnox = realr / diew;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }
   // �k�W
   else if(rgWaferDieDirection->ItemIndex == 3) {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = realr / dieh;
   }
   else {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }

   // 2023 3 20 - chc �ץ�
   gridnox += 2;
   gridnoy += 2;

   // dx,dy: Die����ڤj�p(�����v)
   dx = ((imWaferMap->Width-10)*factor) / gridnox;
   dy = ((imWaferMap->Height-10)*factor) / gridnoy;
   // width,height: Mapper�Ϫ���ڤj�p(�����v)
   width = gridnox * dx;
   height = gridnoy * dy;

   // 2023 3 2 - chc �ץ�
   msg.sprintf("(1)realr,diew,dieh,nox,noy=%.1f,%d,%d,%d,%d",realr,diew,dieh,gridnox,gridnoy);
   frnMain->WriteSystemLog(msg);

   // 2023 3 2a - chc �Y�p
   //gridnoy--;

   // 2021 9 27 - chc ���I��m: GridNoY
   GridNoX = gridnox;
   GridNoY = gridnoy;

   pnlChipWNo->Caption = IntToStr(gridnox);
   pnlChipHNo->Caption = IntToStr(gridnoy);

   // 100% - Full Wafer(12")
   // �v����
   // Factor
   if(factor > 1) {
      // bx,by�v���ذ_�I: �����v�h�O(0,0)�N�i�H�}�l�e
      bx = by = 0;
      // ���imWaferMap�ϰ짡�i�H�e
      imWaferMap->Width = bw;
      imWaferMap->Height = bh;
      imWaferMap->Picture->Bitmap->Width = width;
      imWaferMap->Picture->Bitmap->Height = height;
      // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
      iw = width;
      ih = height;
   }
   else {
      // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
      iw = width + (bw - width) / 2;
      ih = height + (bh - height) /2;
      // imWaferMap���Y�ϰ줺�i�H�e
      imWaferMap->Width = iw;
      imWaferMap->Height = ih;
      // bx,by�v���ذ_�I
      bx = (bw - iw) / 2;
      by = (bh - ih) / 2;
      imWaferMap->Picture->Bitmap->Width = iw * factor;
      imWaferMap->Picture->Bitmap->Height = ih * factor;

      // Die���_�I: ��Die��V�M�w
      // �m��
      if(rgWaferDieDirection->ItemIndex == 0) {
         diex = 0;
         diey = 0;
      }
      // ����
// 2023 3 20a - chc ����, �m��
//      bx = 0;
//      by = 0;
   }

   imWaferMap->Top = by;
   imWaferMap->Left = bx;
   // sx,sy: die�_�I
   // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
   // width,height: Mapper�Ϫ���ڤj�p(�����v)
   // => sx,sy�|����Notch��m
   sx = (iw - width) / 2;
   sy = (ih - height) / 2;

   // �p��Die����/Wafer���ߪ�����Pixel��
   // 1. ��JWafer
   // 2. ��Wafer����(cx,cy)
   // 3. �w����Die������(dx,dy)
   // 4. �HWafer���ߵe�XWafer Mapping��
   // 5. �Х�Wafer���߻PDie����
   // 6.
   double coffsetx,coffsety;
/*
   if(pnlCenterXFind->Caption == "" || pnlCenterXFind->Caption == "0")
      pnlCenterXFind->Caption = pnlWaferDieCenterX->Caption;
   if(pnlCenterYFind->Caption == "" || pnlCenterYFind->Caption == "0")
      pnlCenterYFind->Caption = pnlWaferDieCenterY->Caption;
   coffsetx = pnlWaferDieCenterX->Caption.ToInt() - pnlCenterXFind->Caption.ToInt();
   coffsety = pnlWaferDieCenterY->Caption.ToInt() - pnlCenterYFind->Caption.ToInt();

   // diew,diew: um
   // dx,dy: �CDie��Pixel�ƶq
   // �����q(pulse)�নum
   coffsetx *= frnMain->X_RESOLUTION;
   coffsety *= frnMain->Y_RESOLUTION;
   while(coffsetx > diew) {
      coffsetx -= diew;
   }
   while(coffsety > dieh) {
      coffsety -= dieh;
   }

   // �Y�����q�Ӥj�N���������q�N�n
   if(fabs(coffsetx) > diew)
      coffsetx = 0;
   if(fabs(coffsety) > dieh)
      coffsety = 0;

   // �����q(um)�নPixel
   coffsetx = coffsetx * dx / (double)diew;
   coffsety = coffsety * dy / (double)dieh;
   coffsetx = 0 - coffsetx;
   coffsety = 0 - coffsety;
   // ��Die�����I���~�e��u
   // sx,xy: �O���Grid�bTImage�e���}�l��m�PDie�Ҧb��m�L��

   // ���`���i��
   //coffsetx = coffsety = 5;                                                     // ==> For Test

   coffsetx *= factor;
   coffsety *= factor;
   COffsetX = coffsetx;
   COffsetY = coffsety;
*/
coffsetx = 0;
coffsety = 0;

   // 2023 3 2 - chc ����
   // 2023 3 20 - chc �ץ�
   //coffsetx = dx;
   //coffsety = dy/2;
   int hx,hy;
   hx = dx / 2;
   while(1) {
      hx += dx;
      if(hx > width/2) {
         hx -= dx;
         coffsetx = width / 2 -  hx;
         break;
      }
   }
   hy = dy / 2;
   while(1) {
      hy += dy;
      if(hy > height/2) {
         hy -= dy;
         coffsety = height / 2 -  hy;
         break;
      }
   }
   //coffsetx = (width/2 - (((width - dx) / 2) / dx) * dx) - dx/2;
   //coffsety = (height/2 - (((height - dy) / 2) / dy) * dy) - dy/2;

COffsetX = coffsetx;
COffsetY = coffsety;

   Wafer.X = sx;
   Wafer.Y = sy;
   Wafer.Dx = dx;
   Wafer.Dy = dy;
   Wafer.boolSet = true;
   Wafer.Width = width;
   Wafer.Height = height;

   // �p�⤤��Die��Col,Row: CenterDieCol,CenterDieRow
   // ���|: realr(um)
   // diew,diew: um
   int real_cx,real_cy,centerdie_x,centerdie_y;
   double diffx,diffy;

   // pulse
/*
   real_cx = pnlCenterXFind->Caption.ToInt();
   real_cy = pnlCenterYFind->Caption.ToInt();
   centerdie_x = pnlWaferDieCenterX->Caption.ToInt();
   centerdie_y = pnlWaferDieCenterY->Caption.ToInt();
   diffx = centerdie_x - real_cx;
   diffy = centerdie_y - real_cy;
*/
real_cx = 0;
real_cy = 0;

   // �u�n���@�̬�0�N�L����
   if(real_cx == 0 || real_cy == 0 || centerdie_x == 0 || centerdie_y == 0) {
      diffx = 0;
      diffy = 0;
   }

   diffx = 0 - diffx;
   diffy = 0 - diffy;
   // pulse to um
   diffx *= frnMain->X_RESOLUTION;
   diffy *= frnMain->Y_RESOLUTION;
   //
   fx = realr / 2 + diffx;
   fy = realr / 2 + diffy;

   // �����I�bDie�������I�W
   // 2023 2 19 - chc ����᭱
   /*
   CenterDieCol = (fx + diew/2) / diew;
   CenterDieRow = (fy + dieh/2) / dieh;

   // �̤pRow,Col: ����Recipe��MinCol/MinRow�Ĥ@����DrawBigImage()���|���� => �n�I�s�⦸(�_�h�L�k�e�X����Die��m-�Ŧ�)
   if(MinCol != -1)
      CenterDieCol -= MinCol;
   if(MinRow != -1)
      CenterDieRow -= (GridNoY - MinRow - 1);

   pnlCenterCol->Caption = IntToStr(CenterDieCol);
   pnlCenterRow->Caption = IntToStr(CenterDieRow);
   */
   double ffx,ffy;
   ffx = fx;
   ffy = fy;

/*
   if(tbZoom->Visible == false)
      tbZoom->Visible = true;
*/
   // ����
   cx = iw / 2;
   cy = ih / 2 + 1;

   // �M��
   imWaferMap->Canvas->Brush->Color = clSilver;
   Rect.left = 0;
   Rect.top = 0;
   Rect.Right = iw - 1;
   Rect.Bottom = ih - 1;
   imWaferMap->Canvas->FillRect(Rect);
   // �e�~��
   imWaferMap->Canvas->Pen->Color = clNavy;
   imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->MoveTo(0,0);
   imWaferMap->Canvas->LineTo(0,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,0);
   imWaferMap->Canvas->LineTo(0,0);

   // ��u
   imWaferMap->Canvas->Pen->Width = 1;
   imWaferMap->Canvas->Pen->Color = clBlack;
   // ��Die�����I���~�e��u
   for(int col=coffsetx ; col<width ; col+=dx) {
     imWaferMap->Canvas->MoveTo(sx+col,sy+0);
     imWaferMap->Canvas->LineTo(sx+col,sy+height-1);
   }
   for(int row=coffsety ; row<height ; row+=dy) {
     imWaferMap->Canvas->MoveTo(sx+0,sy+row);
     imWaferMap->Canvas->LineTo(sx+width-1,sy+row);
   }

   // realr
   diameter = realr;

   // �e��: 300
   // X
   // 65*5000=325000
   // (325000 - 300000) / 2 = 12500
   // 325000 / width(845) = 384.65um/Pixel
   // �_�I: 12500 / 384.65 = 32.49 => �����32
   // �`�I��=300000 / 384.65 = 779.929 => �����779
   int esx,esy,ew,eh,maxw,maxh;
   double px,py;
   maxw = gridnox * diew;
   px = (double)maxw / width;

   // �O300, ���Y6(���k�U3mm): diameter
   esx = ((double)(maxw - diameter) / 2) / px;
   ew = diameter / px;
   ew++;
   maxh = gridnoy * dieh;
   py = (double)maxh / height;

   // �O300, ���Y6(���k�U3mm): diameter
   esy = ((double)(maxh - diameter) / 2) / py;
   eh = diameter / py;
   eh++;

   // for test: esx1,ew1,esy1,eh1
   int esx1,esy1,ew1,eh1,maxw1,maxh1;
   double diameter1 = 300000;
   double realr1 = 300000;
   diameter1 = realr1;
   double px1,py1;
   maxw1 = gridnox * diew;
   px1 = (double)maxw1 / width;
   esx1 = ((double)(maxw1 - diameter1) / 2) / px1;
   ew1 = diameter1 / px1;
   ew1++;
   maxh1 = gridnoy * dieh;
   py1 = (double)maxh1 / height;
   esy1 = ((double)(maxh1 - diameter1) / 2) / py1;
   eh1 = diameter1 / py1;
   eh1++;

   // �U�檬�A
   imWaferMap->Canvas->Pen->Color = clBlack;
   int gsx,gex,gsy,gey;
   double r,dsx,dex,dsy,dey,nr,rx,ry;
   double dsx1,dex1,dsy1,dey1;
   double squarex,squarey,squarexy;
   double fsx,fex,fsy,fey;
   int testno,inkno,skipno;
   ncx = cx;
   ncy = (sy+esy-6 + sy+esy+8) / 2;
   nr = (double)((sy+esy+8) - (sy+esy-6)) / 2;
   // �b�|
   // �O300, ���Y6(���k�U3mm): diameter
   r = (diameter / 2) / px;
   rx = (diameter / 2) / px;
   ry = (diameter / 2) / py;

   squarex = rx * rx;
   squarey = ry * ry;
   squarexy = squarex * squarey;
   // �C��|���Z���O�_���br�H��?
   imWaferMap->Canvas->Brush->Style = bsSolid;
   testno = inkno = skipno = 0;

   // �sdx,sx,dy,sy
   MapOutDx = dx;
   MapOutSx = sx;
   MapOutDy = dy;
   MapOutSy = sy;

   for(int col=0 ; col<gridnox ; col++) {
      // ��Die�����I���~�e��u
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;
      // �Y�p
      gsx++;
      gex--;

      // 2023 3 2 - chc �Y�p
      // 2023 3 20 - chc �٭�
      for(int row=0 ; row<gridnoy ; row++) {
      //for(int row=0 ; row<gridnoy-3 ; row++) {

         // ��Die�����I���~�e��u
         gsy = row * dy + sy + coffsety;
         gey = gsy + dy + 1;
         // �Y�p
         gsy++;
         gey--;

         // ���w
         sy -= (dy/2);
         cy -= (dy/2);

         // dsx(gsx,gsy)
         fx = abs(gsx - cx);
         fy = abs(gsy - cy);
         dsx = sqrt((fx*fx)+(fy*fy));
         dsx1 = squarey * (fx*fx) + squarex * (fy*fy);
         fsx = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
         // dex(gex,gsy)
         fx = abs(gex - cx);
         fy = abs(gsy - cy);
         dex = sqrt((fx*fx)+(fy*fy));
         dex1 = squarey * (fx*fx) + squarex * (fy*fy);
         fex = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
         // dsy(gsx,gey)
         fx = abs(gsx - cx);
         fy = abs(gey - cy);
         dsy = sqrt((fx*fx)+(fy*fy));
         dsy1 = squarey * (fx*fx) + squarex * (fy*fy);
         fsy = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
         // dey(gex,gey)
         fx = abs(gex - cx);
         fy = abs(gey - cy);
         dey = sqrt((fx*fx)+(fy*fy));
         dey1 = squarey * (fx*fx) + squarex * (fy*fy);
         fey = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);

         // ���w
         sy += (dy/2);
         cy += (dy/2);

         if(fsx < 1 && fex < 1 && fsy < 1 && fey < 1) {
            // ���
            imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
            // �̤pRow,Col: minrow,mincol,minsx,minex,minsy,miney
            if(col < mincol || mincol == -1) {
               mincol = col;
               minsx = gsx;
               minex = gex;
            }

            // 2023 1 2 - chc ��
            //if(row > minrow || minrow == -1) {
            if(row < minrow || minrow == -1) {

               minrow = row;
               minsy = gsy;
               miney = gey;
            }
            if(col > maxcol || maxcol == -1) {
               maxcol = col;
            }
            if(row > maxrow || maxrow == -1) {
               maxrow = row;

               // 2023 1 2 - chc ��
               maxsy = gsy;
               maxey = gey;

            }

            // 2023 1 2 - chc BIN Code
            /*
            // �q���I
            if(InMeasure(row,col,boolinmeasure) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 4;
               }
               imWaferMap->Canvas->Brush->Color = clRed;
            }

            // ���w�I
            else if(InSelect(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 5;
               }
               imWaferMap->Canvas->Brush->Color = clFuchsia;
            }

            // Center Die, �̤pRow,Col
            else if(InCenterDie1(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  // �U: 180
                  int r,c;
                  r = row;
                  c = col;
//
                  // 2023 3 1 - chc �̮y�ШM�w
                  //if(rgWaferDegree->ItemIndex == 2) {
                  if(rgWaferAxisOrientation->ItemIndex == 1) {

                     r = GridNoY - row - 1;
                     c -= MinCol;
                     r -= (GridNoY - MinRow - 1);
                  }
//
                  if(CenterDieRow != r || CenterDieCol != c)
                     goto notcenter;
                  c = 0;

                  MapCode[row][col] = 6;
               }
               imWaferMap->Canvas->Brush->Color = clBlue;
            }

            else {
notcenter:
               // ���
               imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
               // ��Jcode
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 3;
               }
            }
            */

            // �Ĥ@���I�sInMeasure
            boolinmeasure = false;

            // �~�X
            //imWaferMap->Canvas->Rectangle(gsx,gsy,gex,gey);
            imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
            testno++;
            if(factor > 3) {
               DrawRowCol(gsx,gsy,col,row,0,factor);
            }
         }
         else if(fsx > 1 && fex > 1 && fsy > 1 && fey > 1)
            continue;
         // �����b��ɥ~, �ˬd�Ҧ�Pixel
         else {
            int outno,inno;
            double dis,dis1,fdis;
            outno = inno = 0;
            for(int col1=gsx ; col1<=gex ; col1++) {
               for(int row1=gsy ; row1<=gey ; row1++) {
                  fx = abs(col1 - cx);
                  fy = abs(row1 - cy);
                  dis = sqrt((fx*fx)+(fy*fy));
                  dis1 = squarey * (fx*fx) + squarex * (fy*fy);
                  fdis = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
                  if(fdis > 1) {
                     outno++;
                  }
                  else {
                     // Notch
                     if((row >= 2 && row <= 4) && (col >= gridnox/2-2 && col <= gridnox/2+2)) {
                        fx = abs(col1 - ncx);
                        fy = abs(row1 - ncy);
                        dis = sqrt((fx*fx)+(fy*fy));
                        if(dis <= nr) {
                           outno++;
                           continue;
                        }
                     }
                     inno++;
                  }
               }
            }
         }
      }
   }
/*
   pnlTestables->Caption = IntToStr(testno);
   pnlInkdies->Caption = IntToStr(inkno);
   pnlSkipdies->Caption = IntToStr(skipno);
   pnlTotaldies->Caption = IntToStr(testno+skipno+inkno);
*/

   // �̤pRow,Col
   MinRow = minrow;
   MinCol = mincol;
   MaxRow = maxrow;
   MaxCol = maxcol;

   // �����I�bDie�������I�W
   // 2023 2 19 - chc ����᭱
   // 2023 4 5a - chc  ��Wafer���ߩ��~����
   //CenterDieCol = (double)(ffx + diew/2) / (double)diew;
   //CenterDieRow = (double)(ffy + dieh/2) / (double)dieh;
   //// �̤pRow,Col: ����Recipe��MinCol/MinRow�Ĥ@����DrawBigImage()���|���� => �n�I�s�⦸(�_�h�L�k�e�X����Die��m-�Ŧ�)
   //if(MinCol != -1)
   //   CenterDieCol -= MinCol;
   //if(MinRow != -1)
   //   // 2023 2 19 - chc �Pcol�@��
   //   //CenterDieRow -= (GridNoY - MinRow - 1);
   //   CenterDieRow -= MinRow;
   CenterDieCol = (ffx - (diew/2)) / diew;
   CenterDieRow = (ffy - (dieh/2)) / dieh;
   pnlChipCol->Caption = IntToStr(CenterDieCol);
   pnlChipRow->Caption = IntToStr(CenterDieRow);

   pnlCenterCol->Caption = IntToStr(CenterDieCol);
   pnlCenterRow->Caption = IntToStr(CenterDieRow);

   // 2023 3 22 - chc update to Work Col/Row
   pnlWorkCol->Caption =  pnlCenterCol->Caption;
   pnlWorkRow->Caption =  pnlCenterRow->Caption;

   // Put BIN Code
   maxno = sgDefectList->RowCount - 2;
   for(int i=0 ; i<maxno ; i++) {
      id = sgDefectList->Cells[1][i+1].ToInt();

      // 2023 4 25 - chc �[�Jname
      //col = sgDefectList->Cells[4][i+1].ToInt();
      //row = sgDefectList->Cells[5][i+1].ToInt();
      col = sgDefectList->Cells[4+1][i+1].ToInt();
      row = sgDefectList->Cells[5+1][i+1].ToInt();

      // �U: 180
      // 2023 3 1 - chc �̮y�ШM�w
      //if(rgWaferDegree->ItemIndex == 2) {
      if(rgWaferAxisOrientation->ItemIndex == 1) {

         //row = GridNoY - row - 1;
         //// �̤pRow,Col
         //col -= MinCol;
         //row -= (GridNoY - MinRow - 1);                       => row = row + MinRow;
         col = col + MinCol;
         row = GridNoY - row + MinRow - 2;
      }
      // ��Die�����I���~�e��u
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;
      // �Y�p
      gsx++;
      gex--;
      gsy = row * dy + sy + coffsety;
      gey = gsy + dy + 1;
      // �Y�p
      gsy++;
      gey--;
      MapCode[row][col] = id;
      if(id > 0) {
         imWaferMap->Canvas->Brush->Color = IDColor[id - 1];
         imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
      }
   }

   // ��ڵe��
   // The angles Angle and AngleLength are 1/16th of a degree. For example, a full circle equals 5760 (16*360).
   // Positive values of Angle and AngleLength mean counterclockwise while negative values mean clockwise direction.
   // Zero degrees is at the 3'o clock position.
   // Arc(int X, int Y, int W, int H, int Angle, int AngleLength);
   // -44 to 269
   imWaferMap->Canvas->Pen->Width = 3*factor;
   imWaferMap->Canvas->Brush->Style = bsClear;
   imWaferMap->Canvas->Pen->Color = (TColor)0x00457D7D;

   // ���w
   sy -= (dy/2);
   cy -= (dy/2);

   //imWaferMap->Canvas->Ellipse(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh);
   // 2021 7 21 - chc Notch�W�ΤU
   // �U: 180
//
   if(rgWaferDegree->ItemIndex == 2) {

      // 2023 3 1 - chc �T�w���W
      //rgWaferAxisOrientation->ItemIndex = 1;
      rgWaferAxisOrientation->ItemIndex = 0;

      // 2023 1 2 - chc ��
      int tmp;
      minsy = tmp;
      minsy = maxsy;
      maxsy = tmp;

      // �eNotch: 2.3030(W), 1.50(H), 0�׬O���W��, 180�׬O���U��
      // ����
      imWaferMap->Canvas->Arc(sx+esx,sy+esy,sx+esx+ew,sy+esy+eh,cx,sy+esy+eh-6*factor,cx,sy+esy+eh-6*factor);
      imWaferMap->Canvas->Pen->Color = clBlack;
      // �~��
      imWaferMap->Canvas->Arc(sx+esx1,sy+esy1,sx+esx1+ew1,sy+esy1+eh1,cx+7*factor,sy+esy1+eh1-6*factor,cx-7*factor,sy+esy1+eh1-6*factor);
      // Notch
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy1+eh1-6*factor,cx+7*factor,sy+esy1+eh1+8*factor,cx+7*factor,sy+esy1+eh1-0*factor,cx-7*factor,sy+esy1+eh1-0*factor);

      // �̤pRow,Col: minrow,mincol,minsx,minex,minsy,miney
      // 2023 3 2 - chc ���W��
      //imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor+dy/2, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor+dy/2);

   }
   // �W: 0
   else {
//
      imWaferMap->Canvas->Arc(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh,cx-7*factor,sy+esy+6*factor,cx+7*factor,sy+esy+6*factor);
      // �eNotch: 2.3030(W), 1.50(H)
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy-6*factor,cx+7*factor,sy+esy+8*factor,cx-7*factor,sy+esy-0*factor,cx+7*factor,sy+esy-0*factor);

      // 0�׬O���W��, 180�׬O���U��
      // 2023 3 1 - chc ���I�b���W��
      //imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);

//
      rgWaferAxisOrientation->ItemIndex = 0;
   }
//

   Wafer.Cx = cx;
   Wafer.Cy = cy;

   // ���I
   // �����q, 0�׬O���W��, 180�׬O���U��
   // ���ߥ���
   imWaferMap->Canvas->Pen->Width = 1*factor;
   imWaferMap->Canvas->Pen->Color = clRed;
   imWaferMap->Canvas->MoveTo(cx-5*factor,cy);
   imWaferMap->Canvas->LineTo(cx+5*factor,cy);
   imWaferMap->Canvas->MoveTo(cx,cy-5*factor);
   imWaferMap->Canvas->LineTo(cx,cy+5*factor);
   imWaferMap->Canvas->Pen->Width = 1;
   // �����I��col,row
   col = (cx - Wafer.X) / Wafer.Dx;
   row = (cy - Wafer.Y) / Wafer.Dy;

   // 2021 9 27 - chc ���I��m: GridNoY
   // �U: 180
//
   // 2023 3 1 - chc �̮y�ШM�w
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
   }
//

   // ���w
   sy += (dy/2);
   cy += (dy/2);

   // �̤pRow,Col
   if(MinCol != -1)
      col -= MinCol;
   if(MinRow != -1)
      row -= (GridNoY - MinRow - 1);

   // �e����
   imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->Pen->Color = clNavy;
   imWaferMap->Canvas->MoveTo(sx,sy);
   imWaferMap->Canvas->LineTo(sx,sy+height-1);
   imWaferMap->Canvas->LineTo(sx+width-1,sy+height-1);
   imWaferMap->Canvas->LineTo(sx+width-1,sy);
   imWaferMap->Canvas->LineTo(sx,sy);
/*
   tsWaferMap->Enabled = true;
*/
   return;
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::DrawBigImage()
{
int no,width,height,sx,sy,x,y,w,h,cx,cy,dx,dy,bw,bh,iw,ih,bx,by,ncx,ncy;
int maxno,row,col,id;
AnsiString fname,msg;

//DoubleBuffered = true;
//sbWaferMap->DoubleBuffered = true;

TRect Rect;
int factor = 2;

// Die���_�I: ��Die��V�M�w
int diex,diey;
static bool boolfirst = true;

// �Ĥ@���I�sInMeasure
bool boolinmeasure = true;

// 2023 4 27a - chc ø��u
double fdx,fdy;

// �̤pRow,Col
int minrow,mincol,maxrow,maxcol,minsx,minsy,minex,miney;
minrow = -1;
mincol = -1;
maxrow = -1;
maxcol = -1;

// 2023 1 2 - chc ��
int maxsy,maxey;

   // 2023 5 4 - chc �n�]���
   imWaferMap->Width = 643;
   imWaferMap->Height = 643;

   if(boolfirst == true) {
      //boolfirst = false;
      for(int i=0 ; i<MAX_ROW_NO ; i++) {
         for(int j=0 ; j<MAX_COL_NO ; j++) {
            MapCode[i][j] = 0;
         }
      }
   }

   factor = 1;
   imWaferMap->Picture->Bitmap->PixelFormat = pf4bit;

   // bw,bh: imWaferMap���j�p
   //bw = sbWaferMap->Width;
   //bh = sbWaferMap->Height;
   bw = pnlWaferMap->Width;
   bh = pnlWaferMap->Height;

   // 304800 / 5000 = 60.96 => 61��, ���t2��@65�� => �e�b1207*921��
   // ���b850�H�� => 850/65=13.07, �����13�I
   // ��l�v��: 65*13=845*845
   int gridnox,gridnoy,diew,dieh,ix,iy;
   double fx,fy;

   // 2023 3 23a - chc �ץ�
   double fgridnox,fgridnoy;

   // �令�p�ƤΥ[�J���ιD: edDieSizeGapX
   diew = edChipWidth->Text.ToDouble() + edChipDx->Text.ToDouble();
   dieh = edChipHeight->Text.ToDouble() + edChipDy->Text.ToDouble();

   // �O300, ���Y6(���k�U3mm)
   double diameter = 300000;

   // ignore�令30000
   double ignore = edIgnore->Text.ToInt();
   double realr = diameter - (ignore * 2);

   // Wafer��m�T�w���m��
   if(true) {
      gridnox = (realr - diew) / diew + 1;
      gridnoy = (realr - dieh) / dieh + 1;
      msg.sprintf("realr,diew,dieh,nox,noy=%.1f,%d,%d,%d,%d",realr,diew,dieh,gridnox,gridnoy);
      frnMain->WriteSystemLog(msg);

      // 2023 3 20 - chc ���B�z
      // 2023 3 21 - chc ��_
      int tmp;
      tmp = realr - diew;
      tmp = tmp / diew;
      tmp /= 2;
      tmp *= 2;
      tmp++;
      gridnox = tmp;
      tmp = realr - dieh;
      tmp = tmp / dieh;
      tmp /= 2;
      tmp *= 2;
      tmp++;
      gridnoy = tmp;

      // 2023 3 23a - chc �ץ�
      tmp = diameter / diew;
      gridnox = tmp;
      tmp = diameter / dieh;
      gridnoy = tmp;
      fgridnox = (double)diameter / diew + 2;
      fgridnoy = (double)diameter / dieh + 2;

      // �~�X
      gridnox += 2;
      gridnoy += 2;

   }
   // ���W
   else if(rgWaferDieDirection->ItemIndex == 1) {
      gridnox = realr / diew;
      gridnoy = realr / dieh;
   }
   // ���U
   else if(rgWaferDieDirection->ItemIndex == 2) {
      gridnox = realr / diew;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }
   // �k�W
   else if(rgWaferDieDirection->ItemIndex == 3) {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = realr / dieh;
   }
   else {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }

   // 2023 4 27a - chc ø��u
   fdx = (double)((imWaferMap->Width-10)*factor) / gridnox;
   fdy = (double)((imWaferMap->Height-10)*factor) / gridnoy;

   // dx,dy: Die����ڤj�p(�����v)
   // 2023 5 4a - chc �令double
   //dx = ((imWaferMap->Width-10)*factor) / gridnox;
   //dy = ((imWaferMap->Height-10)*factor) / gridnoy;
   dx = (double)((imWaferMap->Width-10)*factor) / (double)gridnox;
   dy = (double)((imWaferMap->Height-10)*factor) / (double)gridnoy;

   // width,height: Mapper�Ϫ���ڤj�p(�����v)
   width = gridnox * dx;
   height = gridnoy * dy;

// 2023 5 4 - chc test
width = gridnox * fdx;
height = gridnoy * fdy;

   // 2023 3 2 - chc �ץ�
   msg.sprintf("(1)realr,diew,dieh,nox,noy=%.1f,%d,%d,%d,%d",realr,diew,dieh,gridnox,gridnoy);
   frnMain->WriteSystemLog(msg);

   // 2023 3 2a - chc �Y�p
   // 2023 3 20 - chc �n��
   // 2023 3 21 - chc ��_
   //gridnoy--;

   // 2021 9 27 - chc ���I��m: GridNoY
   GridNoX = gridnox;
   GridNoY = gridnoy;

   // 2023 3 23a - chc �ץ�
   //pnlChipWNo->Caption = IntToStr(gridnox);
   //pnlChipHNo->Caption = IntToStr(gridnoy);
   pnlChipWNo->Caption = IntToStr(gridnox-2);
   pnlChipHNo->Caption = IntToStr(gridnoy-2);

   // 100% - Full Wafer(12")
   // �v����
   // Factor
   if(factor > 1) {
      // bx,by�v���ذ_�I: �����v�h�O(0,0)�N�i�H�}�l�e
      bx = by = 0;
      // ���imWaferMap�ϰ짡�i�H�e
      imWaferMap->Width = bw;
      imWaferMap->Height = bh;
      imWaferMap->Picture->Bitmap->Width = width;
      imWaferMap->Picture->Bitmap->Height = height;
      // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
      iw = width;
      ih = height;
   }
   else {
      // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
      iw = width + (bw - width) / 2;
      ih = height + (bh - height) /2;
      // imWaferMap���Y�ϰ줺�i�H�e
      imWaferMap->Width = iw;
      imWaferMap->Height = ih;
      // bx,by�v���ذ_�I
      bx = (bw - iw) / 2;
      by = (bh - ih) / 2;
      imWaferMap->Picture->Bitmap->Width = iw * factor;
      imWaferMap->Picture->Bitmap->Height = ih * factor;

      // Die���_�I: ��Die��V�M�w
      // �m��
      if(rgWaferDieDirection->ItemIndex == 0) {
         diex = 0;
         diey = 0;
      }
      // ����
// 2023 3 20a - chc ����, �m��
//      bx = 0;
//      by = 0;
   }

   imWaferMap->Top = by;
   imWaferMap->Left = bx;
   // sx,sy: die�_�I
   // iw,ih: �~�X: imWaferMap��ø�ϰϰ�
   // width,height: Mapper�Ϫ���ڤj�p(�����v)
   // => sx,sy�|����Notch��m
   sx = (iw - width) / 2;
   sy = (ih - height) / 2;

   // �p��Die����/Wafer���ߪ�����Pixel��
   // 1. ��JWafer
   // 2. ��Wafer����(cx,cy)
   // 3. �w����Die������(dx,dy)
   // 4. �HWafer���ߵe�XWafer Mapping��
   // 5. �Х�Wafer���߻PDie����
   // 6.
   double coffsetx,coffsety;
/*
   if(pnlCenterXFind->Caption == "" || pnlCenterXFind->Caption == "0")
      pnlCenterXFind->Caption = pnlWaferDieCenterX->Caption;
   if(pnlCenterYFind->Caption == "" || pnlCenterYFind->Caption == "0")
      pnlCenterYFind->Caption = pnlWaferDieCenterY->Caption;
   coffsetx = pnlWaferDieCenterX->Caption.ToInt() - pnlCenterXFind->Caption.ToInt();
   coffsety = pnlWaferDieCenterY->Caption.ToInt() - pnlCenterYFind->Caption.ToInt();

   // diew,diew: um
   // dx,dy: �CDie��Pixel�ƶq
   // �����q(pulse)�নum
   coffsetx *= frnMain->X_RESOLUTION;
   coffsety *= frnMain->Y_RESOLUTION;
   while(coffsetx > diew) {
      coffsetx -= diew;
   }
   while(coffsety > dieh) {
      coffsety -= dieh;
   }

   // �Y�����q�Ӥj�N���������q�N�n
   if(fabs(coffsetx) > diew)
      coffsetx = 0;
   if(fabs(coffsety) > dieh)
      coffsety = 0;

   // �����q(um)�নPixel
   coffsetx = coffsetx * dx / (double)diew;
   coffsety = coffsety * dy / (double)dieh;
   coffsetx = 0 - coffsetx;
   coffsety = 0 - coffsety;
   // ��Die�����I���~�e��u
   // sx,xy: �O���Grid�bTImage�e���}�l��m�PDie�Ҧb��m�L��

   // ���`���i��
   //coffsetx = coffsety = 5;                                                     // ==> For Test

   coffsetx *= factor;
   coffsety *= factor;
   COffsetX = coffsetx;
   COffsetY = coffsety;
*/
coffsetx = 0;
coffsety = 0;

   // 2023 3 2 - chc ����
   coffsetx = dx;
   coffsety = dy/2;

COffsetX = coffsetx;
COffsetY = coffsety;

   Wafer.X = sx;
   Wafer.Y = sy;
   Wafer.Dx = dx;
   Wafer.Dy = dy;
   Wafer.boolSet = true;
   Wafer.Width = width;
   Wafer.Height = height;

   // 2023 3 23a - chc �ץ�
   Wafer.Width = ((double)(diameter)/diew) * dx;
   Wafer.Height = ((double)(diameter)/dieh) * dy;

   // �p�⤤��Die��Col,Row: CenterDieCol,CenterDieRow
   // ���|: realr(um)
   // diew,diew: um
   int real_cx,real_cy,centerdie_x,centerdie_y;
   double diffx,diffy;

   // pulse
/*
   real_cx = pnlCenterXFind->Caption.ToInt();
   real_cy = pnlCenterYFind->Caption.ToInt();
   centerdie_x = pnlWaferDieCenterX->Caption.ToInt();
   centerdie_y = pnlWaferDieCenterY->Caption.ToInt();
   diffx = centerdie_x - real_cx;
   diffy = centerdie_y - real_cy;
*/
real_cx = 0;
real_cy = 0;

   // �u�n���@�̬�0�N�L����
   if(real_cx == 0 || real_cy == 0 || centerdie_x == 0 || centerdie_y == 0) {
      diffx = 0;
      diffy = 0;
   }

   // 2023 5 4a - chc �T�w��0
   diffx = 0;
   diffy = 0;

   diffx = 0 - diffx;
   diffy = 0 - diffy;
   // pulse to um
   diffx *= frnMain->X_RESOLUTION;
   diffy *= frnMain->Y_RESOLUTION;
   //
   fx = realr / 2 + diffx;
   fy = realr / 2 + diffy;

   // �����I�bDie�������I�W
   // 2023 2 19 - chc ����᭱
   /*
   CenterDieCol = (fx + diew/2) / diew;
   CenterDieRow = (fy + dieh/2) / dieh;

   // �̤pRow,Col: ����Recipe��MinCol/MinRow�Ĥ@����DrawBigImage()���|���� => �n�I�s�⦸(�_�h�L�k�e�X����Die��m-�Ŧ�)
   if(MinCol != -1)
      CenterDieCol -= MinCol;
   if(MinRow != -1)
      CenterDieRow -= (GridNoY - MinRow - 1);

   pnlCenterCol->Caption = IntToStr(CenterDieCol);
   pnlCenterRow->Caption = IntToStr(CenterDieRow);
   */
   double ffx,ffy;
   ffx = fx;
   ffy = fy;

/*
   if(tbZoom->Visible == false)
      tbZoom->Visible = true;
*/
   // ����
   cx = iw / 2;
   cy = ih / 2 + 1;

   // �M��
   imWaferMap->Canvas->Brush->Color = clSilver;
   Rect.left = 0;
   Rect.top = 0;
   Rect.Right = iw - 1;
   Rect.Bottom = ih - 1;
   imWaferMap->Canvas->FillRect(Rect);
   // �e�~��
   imWaferMap->Canvas->Pen->Color = clNavy;
   imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->MoveTo(0,0);
   imWaferMap->Canvas->LineTo(0,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,0);
   imWaferMap->Canvas->LineTo(0,0);

   // ��u
   imWaferMap->Canvas->Pen->Width = 1;

   // 2023 3 21 - chc �jChip(��Sub-die)�βʽu
   //if(edSubDieXno->Text.ToInt() > 1 || edSubDieYno->Text.ToInt() > 1)
   //   imWaferMap->Canvas->Pen->Width = 2;

   imWaferMap->Canvas->Pen->Color = clBlack;
   // ��Die�����I���~�e��u
   // 2023 4 27a - chc ø��u
// 2023 5 4 - chc test
/*
   for(int col=coffsetx ; col<width ; col+=dx) {
     imWaferMap->Canvas->MoveTo(sx+col,sy+0);
     imWaferMap->Canvas->LineTo(sx+col,sy+height-1);
   }
   for(int row=coffsety ; row<height ; row+=dy) {
     imWaferMap->Canvas->MoveTo(sx+0,sy+row);
     imWaferMap->Canvas->LineTo(sx+width-1,sy+row);
   }
*/
   //
   double findex = 0;
   int pos;
   for(int col=coffsetx ; col<width ; col+=dx) {
     pos = coffsetx + findex * fdx;
     imWaferMap->Canvas->MoveTo(sx+pos,sy+0);
     imWaferMap->Canvas->LineTo(sx+pos,sy+height-1);
     findex += 1.0;
   }
   findex = 0;
   for(int row=coffsety ; row<height ; row+=dy) {
     pos = coffsety + findex * fdy;
     imWaferMap->Canvas->MoveTo(sx+0,sy+pos);
     imWaferMap->Canvas->LineTo(sx+width-1,sy+pos);
     findex += 1.0;
   }
   //

   // realr
   diameter = realr;

   // �e��: 300
   // X
   // 65*5000=325000
   // (325000 - 300000) / 2 = 12500
   // 325000 / width(845) = 384.65um/Pixel
   // �_�I: 12500 / 384.65 = 32.49 => �����32
   // �`�I��=300000 / 384.65 = 779.929 => �����779
   int esx,esy,ew,eh,maxw,maxh;
   double px,py;
   maxw = gridnox * diew;
   px = (double)maxw / width;

   // �O300, ���Y6(���k�U3mm): diameter
   esx = ((double)(maxw - diameter) / 2) / px;
   ew = diameter / px;
   ew++;
   maxh = gridnoy * dieh;
   py = (double)maxh / height;

   // �O300, ���Y6(���k�U3mm): diameter
   esy = ((double)(maxh - diameter) / 2) / py;
   eh = diameter / py;
   eh++;

   // for test: esx1,ew1,esy1,eh1
   int esx1,esy1,ew1,eh1,maxw1,maxh1;
   double diameter1 = 300000;
   double realr1 = 300000;
   diameter1 = realr1;
   double px1,py1;
   maxw1 = gridnox * diew;
   px1 = (double)maxw1 / width;
   esx1 = ((double)(maxw1 - diameter1) / 2) / px1;
   ew1 = diameter1 / px1;
   ew1++;
   maxh1 = gridnoy * dieh;
   py1 = (double)maxh1 / height;
   esy1 = ((double)(maxh1 - diameter1) / 2) / py1;
   eh1 = diameter1 / py1;
   eh1++;

   // �U�檬�A
   imWaferMap->Canvas->Pen->Color = clBlack;
   int gsx,gex,gsy,gey;
   double r,dsx,dex,dsy,dey,nr,rx,ry;
   double dsx1,dex1,dsy1,dey1;
   double squarex,squarey,squarexy;
   double fsx,fex,fsy,fey;
   int testno,inkno,skipno;
   ncx = cx;
   ncy = (sy+esy-6 + sy+esy+8) / 2;
   nr = (double)((sy+esy+8) - (sy+esy-6)) / 2;
   // �b�|
   // �O300, ���Y6(���k�U3mm): diameter
   r = (diameter / 2) / px;
   rx = (diameter / 2) / px;
   ry = (diameter / 2) / py;

   squarex = rx * rx;
   squarey = ry * ry;
   squarexy = squarex * squarey;
   // �C��|���Z���O�_���br�H��?
   imWaferMap->Canvas->Brush->Style = bsSolid;
   testno = inkno = skipno = 0;

   // �sdx,sx,dy,sy
   MapOutDx = dx;
   MapOutSx = sx;
   MapOutDy = dy;
   MapOutSy = sy;

   for(int col=0 ; col<gridnox ; col++) {
      // ��Die�����I���~�e��u
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;

      // 2023 5 4 - chc test
      gsx = col * fdx + sx + coffsetx;
      gex = (col+1) * fdx + 1 + sx + coffsetx;

      // �Y�p
      gsx++;
      gex--;

      // 2023 3 2 - chc �Y�p
      // 2023 3 20 - chc �٭�
      // 2023 3 21 - chc ��_
      //for(int row=0 ; row<gridnoy ; row++) {
      // 2023 3 22a - chc -3 to -2
      //for(int row=0 ; row<gridnoy-3 ; row++) {
      // 2023 5 2 - chc ���Y�p
      //for(int row=0 ; row<gridnoy-2 ; row++) {
      for(int row=0 ; row<gridnoy ; row++) {

         // ��Die�����I���~�e��u
         gsy = row * dy + sy + coffsety;
         gey = gsy + dy + 1;

         // 2023 5 4 - chc test
         gsy = row * fdy + sy + coffsety;
         gey = (row+1) * fdy + 1 + sy + coffsety;

         // �Y�p
         gsy++;
         gey--;

         // 2023 3 22a - chc Check gey: sy+esy1+eh1-6*factor
         if(gey >= (sy+esy1+eh1-6*factor-COffsetY))
            continue;

         // ���w
// 2023 5 4 - chc test
         sy -= (dy/2);
         cy -= (dy/2);
//sy -= (fdy/2);
//cy -= (fdy/2);

         // dsx(gsx,gsy)
         fx = abs(gsx - cx);
         fy = abs(gsy - cy);
//         dsx = sqrt((fx*fx)+(fy*fy));
//         dsx1 = squarey * (fx*fx) + squarex * (fy*fy);
         fsx = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
//fsx = ((fx*fx)+(fy*fy)) / ((rx*rx)+(ry*ry));
         // dex(gex,gsy)
         fx = abs(gex - cx);
         fy = abs(gsy - cy);
//         dex = sqrt((fx*fx)+(fy*fy));
//         dex1 = squarey * (fx*fx) + squarex * (fy*fy);
         fex = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
//fex = ((fx*fx)+(fy*fy)) / ((rx*rx)+(ry*ry));
         // dsy(gsx,gey)
         fx = abs(gsx - cx);
         fy = abs(gey - cy);
//         dsy = sqrt((fx*fx)+(fy*fy));
//         dsy1 = squarey * (fx*fx) + squarex * (fy*fy);
         fsy = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
//fsy = ((fx*fx)+(fy*fy)) / ((rx*rx)+(ry*ry));
         // dey(gex,gey)
         fx = abs(gex - cx);
         fy = abs(gey - cy);
//         dey = sqrt((fx*fx)+(fy*fy));
//         dey1 = squarey * (fx*fx) + squarex * (fy*fy);
         fey = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
//fey = ((fx*fx)+(fy*fy)) / ((rx*rx)+(ry*ry));

// 2023 5 4 - chc Test
// ���
//if(fsx < 1 && fex < 1 && fsy < 1 && fey < 1) {
//   imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
//   imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
//}
//e/lse {
//   int i = 0;
//}
//continue;
//goto endloop;

         // ���w
// 2023 5 4 - chc test
         sy += (dy/2);
         cy += (dy/2);
//sy += (fdy/2);
//cy += (fdy/2);

         if(fsx < 1 && fex < 1 && fsy < 1 && fey < 1) {
            // ���
            imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
            // �̤pRow,Col: minrow,mincol,minsx,minex,minsy,miney
            if(col < mincol || mincol == -1) {
               mincol = col;
               minsx = gsx;
               minex = gex;
            }

            // 2023 1 2 - chc ��
            //if(row > minrow || minrow == -1) {
            if(row < minrow || minrow == -1) {

               minrow = row;
               minsy = gsy;
               miney = gey;
            }
            if(col > maxcol || maxcol == -1) {
               maxcol = col;
            }
            if(row > maxrow || maxrow == -1) {
               maxrow = row;

               // 2023 1 2 - chc ��
               maxsy = gsy;
               maxey = gey;

            }

            // 2023 1 2 - chc BIN Code
            /*
            // �q���I
            if(InMeasure(row,col,boolinmeasure) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 4;
               }
               imWaferMap->Canvas->Brush->Color = clRed;
            }

            // ���w�I
            else if(InSelect(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 5;
               }
               imWaferMap->Canvas->Brush->Color = clFuchsia;
            }

            // Center Die, �̤pRow,Col
            else if(InCenterDie1(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  // �U: 180
                  int r,c;
                  r = row;
                  c = col;
//
                  // 2023 3 1 - chc �̮y�ШM�w
                  //if(rgWaferDegree->ItemIndex == 2) {
                  if(rgWaferAxisOrientation->ItemIndex == 1) {

                     r = GridNoY - row - 1;
                     c -= MinCol;
                     r -= (GridNoY - MinRow - 1);
                  }
//
                  if(CenterDieRow != r || CenterDieCol != c)
                     goto notcenter;
                  c = 0;

                  MapCode[row][col] = 6;
               }
               imWaferMap->Canvas->Brush->Color = clBlue;
            }

            else {
notcenter:
               // ���
               imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
               // ��Jcode
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 3;
               }
            }
            */

            // �Ĥ@���I�sInMeasure
            boolinmeasure = false;

            // �~�X
            //imWaferMap->Canvas->Rectangle(gsx,gsy,gex,gey);
// 2023 5 4 - chc Test
            imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);

            // 2023 3 21 - chc Sub-Die
// 2023 5 4 - chc Test
            DrawSubDieMap(gsx-1,gsy-1,gex+1,gey+1);

            testno++;
// 2023 5 4 - chc Test
            if(factor > 3) {
               DrawRowCol(gsx,gsy,col,row,0,factor);
            }
         }
         else if(fsx > 1 && fex > 1 && fsy > 1 && fey > 1)
            continue;
         // �����b��ɥ~, �ˬd�Ҧ�Pixel
         else {
            int outno,inno;
            double dis,dis1,fdis;
            outno = inno = 0;
            for(int col1=gsx ; col1<=gex ; col1++) {
               for(int row1=gsy ; row1<=gey ; row1++) {
                  fx = abs(col1 - cx);
                  fy = abs(row1 - cy);
                  dis = sqrt((fx*fx)+(fy*fy));
                  dis1 = squarey * (fx*fx) + squarex * (fy*fy);
                  fdis = (fx*fx) / (rx*rx) + (fy*fy) / (ry*ry);
                  if(fdis > 1) {
                     outno++;
                  }
                  else {
                     // Notch
                     if((row >= 2 && row <= 4) && (col >= gridnox/2-2 && col <= gridnox/2+2)) {
                        fx = abs(col1 - ncx);
                        fy = abs(row1 - ncy);
                        dis = sqrt((fx*fx)+(fy*fy));
                        if(dis <= nr) {
                           outno++;
                           continue;
                        }
                     }
                     inno++;
                  }
               }
            }
         }

// 2023 5 4 - chc Test
//endloop:
         
      }
   }
/*
   pnlTestables->Caption = IntToStr(testno);
   pnlInkdies->Caption = IntToStr(inkno);
   pnlSkipdies->Caption = IntToStr(skipno);
   pnlTotaldies->Caption = IntToStr(testno+skipno+inkno);
*/

// 2023 5 4 - chc Test
//return;

   // 2023 3 21 - chc �Aø��u
   imWaferMap->Canvas->Pen->Width = 1;
   // �jChip(��Sub-die)�βʽu
   if(edSubDieXno->Text.ToInt() > 1 || edSubDieYno->Text.ToInt() > 1)
      imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->Pen->Color = clBlack;

   // ��Die�����I���~�e��u
   // 2023 5 4 - chc test
   /*
   for(int col=coffsetx ; col<width ; col+=dx) {
     imWaferMap->Canvas->MoveTo(sx+col,sy+0);
     imWaferMap->Canvas->LineTo(sx+col,sy+height-1);
   }
   for(int row=coffsety ; row<height ; row+=dy) {
     imWaferMap->Canvas->MoveTo(sx+0,sy+row);
     imWaferMap->Canvas->LineTo(sx+width-1,sy+row);
   }
   */
   findex = 0;
   //int pos;
   for(int col=coffsetx ; col<width ; col+=dx) {
     pos = coffsetx + findex * fdx;
     imWaferMap->Canvas->MoveTo(sx+pos,sy+0);
     imWaferMap->Canvas->LineTo(sx+pos,sy+height-1);
     findex += 1.0;
   }
   findex = 0;
   for(int row=coffsety ; row<height ; row+=dy) {
     pos = coffsety + findex * fdy;
     imWaferMap->Canvas->MoveTo(sx+0,sy+pos);
     imWaferMap->Canvas->LineTo(sx+width-1,sy+pos);
     findex += 1.0;
   }

   // �̤pRow,Col
   MinRow = minrow;
   MinCol = mincol;
   MaxRow = maxrow;
   MaxCol = maxcol;

   // �����I�bDie�������I�W
   // 2023 2 19 - chc ����᭱
   // 2023 4 5a - chc  ��Wafer���ߩ��~����
   //CenterDieCol = (double)(ffx + diew/2) / (double)diew;
   //CenterDieRow = (double)(ffy + dieh/2) / (double)dieh;
   //// �̤pRow,Col: ����Recipe��MinCol/MinRow�Ĥ@����DrawBigImage()���|���� => �n�I�s�⦸(�_�h�L�k�e�X����Die��m-�Ŧ�)
   //if(MinCol != -1)
   //   CenterDieCol -= MinCol;
   //if(MinRow != -1)
   //   // 2023 2 19 - chc �Pcol�@��
   //   //CenterDieRow -= (GridNoY - MinRow - 1);
   //   CenterDieRow -= MinRow;
   //
   //// 2023 3 21 - chc ���n��1
   //CenterDieCol--;
   //CenterDieRow--;
   CenterDieCol = (ffx - (diew/2)) / diew;
   CenterDieRow = (ffy - (dieh/2)) / dieh;
   pnlChipCol->Caption = IntToStr(CenterDieCol);
   pnlChipRow->Caption = IntToStr(CenterDieRow);

   pnlCenterCol->Caption = IntToStr(CenterDieCol);
   pnlCenterRow->Caption = IntToStr(CenterDieRow);

   // 2023 3 22 - chc update to Work Col/Row
   //pnlWorkCol->Caption = "";
   //pnlWorkRow->Caption = "";
   pnlWorkCol->Caption =  pnlCenterCol->Caption;
   pnlWorkRow->Caption =  pnlCenterRow->Caption;

   // Put BIN Code
   maxno = sgDefectList->RowCount - 2;
   for(int i=0 ; i<maxno ; i++) {
      id = sgDefectList->Cells[1][i+1].ToInt();

      // 2023 4 25 - chc �[�Jname
      //col = sgDefectList->Cells[4][i+1].ToInt();
      //row = sgDefectList->Cells[5][i+1].ToInt();
      col = sgDefectList->Cells[4+1][i+1].ToInt();
      row = sgDefectList->Cells[5+1][i+1].ToInt();

      // �U: 180
      // 2023 3 1 - chc �̮y�ШM�w
      //if(rgWaferDegree->ItemIndex == 2) {
      if(rgWaferAxisOrientation->ItemIndex == 1) {

         //row = GridNoY - row - 1;
         //// �̤pRow,Col
         //col -= MinCol;
         //row -= (GridNoY - MinRow - 1);                       => row = row + MinRow;
         col = col + MinCol;
         row = GridNoY - row + MinRow - 2;
      }
      // ��Die�����I���~�e��u
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;
      // �Y�p
      gsx++;
      gex--;
      gsy = row * dy + sy + coffsety;
      gey = gsy + dy + 1;
      // �Y�p
      gsy++;
      gey--;
      MapCode[row][col] = id;
      if(id > 0) {
         imWaferMap->Canvas->Brush->Color = IDColor[id - 1];
         imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
      }
   }

   // ��ڵe��
   // The angles Angle and AngleLength are 1/16th of a degree. For example, a full circle equals 5760 (16*360).
   // Positive values of Angle and AngleLength mean counterclockwise while negative values mean clockwise direction.
   // Zero degrees is at the 3'o clock position.
   // Arc(int X, int Y, int W, int H, int Angle, int AngleLength);
   // Arc Arc(x1,y1,x2,y2,x3,y3,x4,y4 : Integer);
   // Arc��k�b���W�e�@�q���A����(x1,y1),(x2,y2) ���I�ҽT�w�����ҨM
   // �w�C�����_�I�O����P�M��ꤤ�߻P(x3,y3)�s�u�����I�C���x�β��I�O��
   // ���P�M��ꤤ�߻P(x4,y4)�s�u�����I�A�H�f�ɰw��V�e��
   // -44 to 269
   imWaferMap->Canvas->Pen->Width = 3*factor;
   imWaferMap->Canvas->Brush->Style = bsClear;
   imWaferMap->Canvas->Pen->Color = (TColor)0x00457D7D;

   // ���w
   sy -= (dy/2);
   cy -= (dy/2);

   //imWaferMap->Canvas->Ellipse(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh);
   // 2021 7 21 - chc Notch�W�ΤU
   // �U: 180
//
   if(rgWaferDegree->ItemIndex == 2) {

      // 2023 3 1 - chc �T�w���W
      //rgWaferAxisOrientation->ItemIndex = 1;
      rgWaferAxisOrientation->ItemIndex = 0;

      // 2023 1 2 - chc ��
      int tmp;
      minsy = tmp;
      minsy = maxsy;
      maxsy = tmp;

      // �eNotch: 2.3030(W), 1.50(H), 0�׬O���W��, 180�׬O���U��
      imWaferMap->Canvas->Arc(sx+esx,sy+esy,sx+esx+ew,sy+esy+eh,cx,sy+esy+eh-6*factor,cx,sy+esy+eh-6*factor);
      imWaferMap->Canvas->Pen->Color = clBlack;
      imWaferMap->Canvas->Arc(sx+esx1,sy+esy1,sx+esx1+ew1,sy+esy1+eh1,cx+7*factor,sy+esy1+eh1-6*factor,cx-7*factor,sy+esy1+eh1-6*factor);
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy1+eh1-6*factor,cx+7*factor,sy+esy1+eh1+8*factor,cx+7*factor,sy+esy1+eh1-0*factor,cx-7*factor,sy+esy1+eh1-0*factor);

      // �̤pRow,Col: minrow,mincol,minsx,minex,minsy,miney
      // 2023 3 2 - chc ���W��
      //imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor+dy/2, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor+dy/2);

   }
   // �W: 0
   else {
//
      imWaferMap->Canvas->Arc(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh,cx-7*factor,sy+esy+6*factor,cx+7*factor,sy+esy+6*factor);
      // �eNotch: 2.3030(W), 1.50(H)
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy-6*factor,cx+7*factor,sy+esy+8*factor,cx-7*factor,sy+esy-0*factor,cx+7*factor,sy+esy-0*factor);

      // 0�׬O���W��, 180�׬O���U��
      // 2023 3 1 - chc ���I�b���W��
      //imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);

//
      rgWaferAxisOrientation->ItemIndex = 0;
   }
//

   Wafer.Cx = cx;
   Wafer.Cy = cy;

   // ���I
   // �����q, 0�׬O���W��, 180�׬O���U��
   // ���ߥ���
   imWaferMap->Canvas->Pen->Width = 1*factor;
   imWaferMap->Canvas->Pen->Color = clRed;
   imWaferMap->Canvas->MoveTo(cx-5*factor,cy);
   imWaferMap->Canvas->LineTo(cx+5*factor,cy);
   imWaferMap->Canvas->MoveTo(cx,cy-5*factor);
   imWaferMap->Canvas->LineTo(cx,cy+5*factor);
   imWaferMap->Canvas->Pen->Width = 1;
   // �����I��col,row
   col = (cx - Wafer.X) / Wafer.Dx;
   row = (cy - Wafer.Y) / Wafer.Dy;

   // 2021 9 27 - chc ���I��m: GridNoY
   // �U: 180
//
   // 2023 3 1 - chc �̮y�ШM�w
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
   }
//

   // ���w
   sy += (dy/2);
   cy += (dy/2);

   // �̤pRow,Col
   if(MinCol != -1)
      col -= MinCol;
   if(MinRow != -1)
      row -= (GridNoY - MinRow - 1);

   // �e����
   imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->Pen->Color = clNavy;
   imWaferMap->Canvas->MoveTo(sx,sy);
   imWaferMap->Canvas->LineTo(sx,sy+height-1);
   imWaferMap->Canvas->LineTo(sx+width-1,sy+height-1);
   imWaferMap->Canvas->LineTo(sx+width-1,sy);
   imWaferMap->Canvas->LineTo(sx,sy);
/*
   tsWaferMap->Enabled = true;
*/
   return;
}
//---------------------------------------------------------------------------
// type : 0-Inner, 1-Out, 2-half
void __fastcall TfrmRecipeSet::DrawRowCol(int sx,int sy,int col,int row,int type, int factor)
{
AnsiString msg,sdie;
int dy,gap;

/*
   if(cbFontDisplay->Checked == false)
      return;

   if(factor < 6) {
      imWaferMap->Canvas->Font->Size = 8;
      dy = 12;
      gap = 2;
   }
   else if(factor < 10) {
      imWaferMap->Canvas->Font->Size = 11;
      dy = 14;
      gap = 4;
   }
   else if(factor < 15) {
      imWaferMap->Canvas->Font->Size = 13;
      dy = 18;
      gap = 4;
   }
   else {
      imWaferMap->Canvas->Font->Size = 15;
      dy = 20;
      gap = 4;
   }
   imWaferMap->Canvas->Font->Name = "Arial";                                 // Arial
   if(type == 0) {

                  // 2021 12 15 - chc �̤pRow,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }

      // 2021 6 16 - chc ��MapCode[][]
      if(MapCode[row][col] == 4)
         imWaferMap->Canvas->Font->Color = clWhite;

      // 2021 8 11 - chc ���w�I
      else if(MapCode[row][col] == 5)
         imWaferMap->Canvas->Font->Color = clWhite;

      // 2021 8 24 - chc Center Die
      else if(MapCode[row][col] == 6)
         imWaferMap->Canvas->Font->Color = clWhite;

      else

         imWaferMap->Canvas->Font->Color = clBlack;
   }
   else if(type == 1)
      imWaferMap->Canvas->Font->Color = clBlack;
   else
      imWaferMap->Canvas->Font->Color = clWhite;
   msg.sprintf("X:%d / Y:%d",col,row);
   imWaferMap->Canvas->TextOut(sx+5,sy+5,msg);
   if(type == 0) {

      // 2021 6 16 - chc ��MapCode[][]
      if(MapCode[row][col] == 4)
         sdie = "Measure";

      // 2021 8 11 - chc ���w�I
      else if(MapCode[row][col] == 5)
         sdie = "Selected";

      // 2021 8 11 - chc Center Die
      else if(MapCode[row][col] == 6)
         sdie = "Center Die";

      else

         sdie = "testable";
   }
   else if(type == 1)
      sdie = "skipdie";
   else if(type == 2)
      sdie = "inkdie";

   // 2021 9 30 - chc ���I��m: GridNoY
   // �U: 180
   // 2023 3 1 - chc �̮y�ШM�w
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
   }

   msg.sprintf("X:%03d/Y:%03d",col,row);
   imWaferMap->Canvas->TextOut(sx+gap,sy+gap,msg);
   msg.sprintf("%-10s",sdie);
   imWaferMap->Canvas->TextOut(sx+gap,sy+gap+dy*1,msg);
   return;
*/   
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc ����Center Pattern��
bool boolCenterMove = false;
int CenterX,CenterY;
void __fastcall TfrmRecipeSet::shCenterMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterMove = true;
   CenterX = X;
   CenterY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shCenterMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height;

   // ���ʤ�
   if(boolCenterMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      dx = X - CenterX;
      dy = Y - CenterY;
      x1 = shCenter->Left + dx;
      y1 = shCenter->Top + dy;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      if(x1 < 0) {
         x1 = 0;
         x2 = x1 + shCenter->Width - 1;
      }
      if(y1 < 0) {
         y1 = 0;
         y2 = y1 + shCenter->Height - 1;
      }
      if(x2 >= width) {
         x2 = width - 1;
         x1 = x2 - shCenter->Width + 1;
      }
      if(y2 >= height) {
         y2 = height - 1;
         y1 = y2 - shCenter->Height + 1;
      }
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Refresh();
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shCenterMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 7 -  chc LeftUp����
bool boolCenterLeftUpMove = false;
int CenterLeftUpX,CenterLeftUpY;
void __fastcall TfrmRecipeSet::shLeftUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        
   boolCenterLeftUpMove = true;
   CenterLeftUpX = X;
   CenterLeftUpY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftUpMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterLeftUpMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterLeftUpX;
      dy = Y - CenterLeftUpY;
      if((x2 - (x1+dx)) >= minx) {
         x1 += dx;
      }
      if((y2 - (y1+dy)) >= miny) {
         y1 += dy;
      }
      if(x1 < 0)
         x1 = 0;
      if(y1 < 0)
         y1 = 0;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterLeftUpMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc Left����
bool boolCenterLeftMove = false;
int CenterLeftX,CenterLeftY;
void __fastcall TfrmRecipeSet::shLeftMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterLeftMove = true;
   CenterLeftX = X;
   CenterLeftY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterLeftMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterLeftX;
      if((x2 - (x1+dx)) >= minx) {
         x1 += dx;
      }
      if(x1 < 0)
         x1 = 0;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterLeftMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc LeftDown����
bool boolCenterLeftDownMove = false;
int CenterLeftDownX,CenterLeftDownY;
void __fastcall TfrmRecipeSet::shLeftDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterLeftDownMove = true;
   CenterLeftDownX = X;
   CenterLeftDownY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftDownMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterLeftDownMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterLeftUpX;
      dy = Y - CenterLeftUpY;
      if((x2 - (x1+dx)) >= minx) {
         x1 += dx;
      }
      if(((y2+dy) - y1) >= miny) {
         y2 += dy;
      }
      if(x1 < 0)
         x1 = 0;
      if(y2 >= height)
         y2 = height - 1;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shLeftDownMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterLeftDownMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc Up����
bool boolCenterUpMove = false;
int CenterUpX,CenterUpY;
void __fastcall TfrmRecipeSet::shUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterUpMove = true;
   CenterUpX = X;
   CenterUpY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shUpMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterUpMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dy = Y - CenterUpY;
      if((y2 - (y1+dy)) >= miny) {
         y1 += dy;
      }
      if(y1 < 0)
         y1 = 0;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterUpMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc Down����
bool boolCenterDownMove = false;
int CenterDownX,CenterDownY;
void __fastcall TfrmRecipeSet::shDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterDownMove = true;
   CenterDownX = X;
   CenterDownY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shDownMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterDownMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dy = Y - CenterDownY;
      if(((y2+dy) - y1) >= miny) {
         y2 += dy;
      }
      if(y2 >= height)
         y2 = height - 1;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shDownMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterDownMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc RightUp����
bool boolCenterRightUpMove = false;
int CenterRightUpX,CenterRightUpY;
void __fastcall TfrmRecipeSet::shRightUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterRightUpMove = true;
   CenterRightUpX = X;
   CenterRightUpY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightUpMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterRightUpMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterRightUpX;
      dy = Y - CenterRightUpY;
      if(((x2+dx) - x1) >= minx) {
         x2 += dx;
      }
      if((y2 - (y1+dy)) >= miny) {
         y1 += dy;
      }
      if(x2 >= width)
         x2 = width - 1;
      if(y1 < 0)
         y1 = 0;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterRightUpMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc Right����
bool boolCenterRightMove = false;
int CenterRightX,CenterRightY;
void __fastcall TfrmRecipeSet::shRightMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterRightMove = true;
   CenterRightX = X;
   CenterRightY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterRightMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterRightX;
      if(((x2+dx) - x1) >= minx) {
         x2 += dx;
      }
      if(x2 >= width)
         x2 = width - 1;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterRightMove = false;
}
//---------------------------------------------------------------------------
// 2023 1 6 - chc RightDown����
bool boolCenterRightDownMove = false;
int CenterRightDownX,CenterRightDownY;
void __fastcall TfrmRecipeSet::shRightDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
        
   boolCenterRightDownMove = true;
   CenterRightDownX = X;
   CenterRightDownY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightDownMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int dx,dy,x1,y1,x2,y2,w,h,width,height,minx,miny;

   // ���ʤ�
   minx = 20;
   miny = 20;
   if(boolCenterRightDownMove == true) {
      // �ե����߮ئ�m
      width = imInspection->Width;
      height = imInspection->Height;
      w = shCenter->Width;
      h = shCenter->Height;
      x1 = shCenter->Left;
      y1 = shCenter->Top;
      x2 = x1 + shCenter->Width - 1;
      y2 = y1 + shCenter->Height - 1;
      // �ܤ�
      dx = X - CenterRightDownX;
      dy = Y - CenterRightDownY;
      if(((x2+dx) - x1) >= minx) {
         x2 += dx;
      }
      if(((y2+dy) - y1) >= miny) {
         y2 += dy;
      }
      if(x2 >= width)
         x2 = width - 1;
      if(y2 >= height)
         y2 = height - 1;
      shCenter->Left = x1;
      shCenter->Top = y1;
      shCenter->Width = x2 - x1 + 1;
      shCenter->Height = y2 - y1 + 1;
      UpdateCenterDotPosition();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::shRightDownMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolCenterRightDownMove = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgDefectListDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // �o��J�I�M�襤�椸��A���ݭn�B�z�A�u�� (State.Contains(gdFixed)) �~�}�l�i��B�z
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // �̦�
      else
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00D1D1D1;
      ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
   }
   if(ARow == 0 || ACol == 0) {
      ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00B3B3B3;
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else {
      //Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
}
//---------------------------------------------------------------------------
int AdjustCenterX = 0;
int AdjustCenterY = 0;
int AdjustCenterDX = 0;
int AdjustCenterDY = 0;

// Base1-Left, Base1-Right, Base2
void __fastcall TfrmRecipeSet::btnAlignmentClick(TObject *Sender)
{
int cx,cy,x1,y1,x2,y2,dx,dy,ex,diff,w,r,x,y;

   if(pnlBase1X->Caption == "" || pnlBase1Y->Caption == "" || pnlStdZ->Caption == "" || pnlStdT->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Base1!";
      return;
   }

   rgMatchSource->ItemIndex = 0;

   // 2023 3 22 - chc �w��쪺�����I�Ӱ�
   if(pnlCenterX->Caption != "" && pnlCenterY->Caption != "") {
      cx = pnlCenterX->Caption.ToInt();
      cy = pnlCenterY->Caption.ToInt();
   }
   else {

      cx = frnMain->edCenterX->Text.ToInt();
      cy = frnMain->edCenterY->Text.ToInt();
   }
   x1 = pnlBase1X->Caption.ToInt();
   y1 = pnlBase1Y->Caption.ToInt();

   pnlAlignment->Color = clSilver;
   pnlSystemMessage->Caption = "";
   cbMoveToCenterMatched->Checked = true;

   // Base1 - Left
   MoveToXY(x1,y1);
   //Sleep(500);
   // 2023 5 5 - chc ���WaitTime
   //for(int i=0 ; i<6 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(400);

   // 2023 4 19 - chc ��s��m
   UpdateMotionPosition();

   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Base1-1 Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }

   if(Application->MessageBox("Alignment: Point1 done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   // Base1 - Right
   //Sleep(500);
   // 2023 5 5 - chc ���WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(300);

   UpdateMotionPosition();
   x1 = pnlX->Caption.ToInt();
   y1 = pnlY->Caption.ToInt();
   dx = cx - x1;
   dy = cy - y1;
   ex = cx + dx;
   diff = ex - x1;
   w = edChipWidth->Text.ToInt();

   // 2023 4 27 - chc ��׮ե�
   w = (double)w / frnMain->X_RESOLUTION;

   r = diff / w;
   ex = x1 + w * r;
   //MoveToXY(cx+dx,y1);
   MoveToXY(ex,y1);
   //Sleep(500);
   // 2023 5 5 - chc ���WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(400);

   // 2023 4 19 - chc ��s��m
   UpdateMotionPosition();

   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Base1-2 Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }

   pnlAlignment->Color = clLime;
   UpdateMotionPosition();
   x2 = pnlX->Caption.ToInt();
   y2 = pnlY->Caption.ToInt();
   RemoteRotate(x1,y1,x2,y2);

   // 2023 3 22 - chc ����Base2/Position
   //return;

   /*
   if(Application->MessageBox("Alignment: Point2 done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   //Sleep(500);
   for(int i=0 ; i<5 ; i++) {
      Sleep(100);
      Application->ProcessMessages();
   }
   UpdateMotionPosition();

   // Base1/Base2/Position
   if(rgMatchSource->ItemIndex == 0) {
      if(Application->MessageBox("To do Base2?      ", "Pattern Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
         return;
      rgMatchSource->ItemIndex = 1;
      int x,y,cx,cy,x1,y1;

      // 2023 3 22 - chc �w��쪺�����I�Ӱ�
      if(pnlCenterX->Caption != "" && pnlCenterY->Caption != "") {
         cx = pnlCenterX->Caption.ToInt();
         cy = pnlCenterY->Caption.ToInt();
      }
      else {

         cx = frnMain->edCenterX->Text.ToInt();
         cy = frnMain->edCenterY->Text.ToInt();
      }
      x1 = pnlBase1X->Caption.ToInt();
      y1 = pnlBase1Y->Caption.ToInt();
      x = cx;
      y = y1 - abs(cx - x1);
      if(MoveToXY(x,y) == false) {
         pnlSystemMessage->Caption = "XY Move Faile!";
         // Beep�n
         Beep(500);
      }
   }
   */

   // 2023 5 5 - chc ���WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(400);

   UpdateMotionPosition();
   // Position
   if(frmRecipe->cbPause->Checked == true) {
      if(Application->MessageBox("To do Position?      ", "Pattern Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
         return;
   }
   rgMatchSource->ItemIndex = 2;
   // �w��쪺�����I�Ӱ�
   if(pnlCenterX->Caption != "" && pnlCenterY->Caption != "") {
      cx = pnlCenterX->Caption.ToInt();
      cy = pnlCenterY->Caption.ToInt();
   }
   else {
      cx = frnMain->edCenterX->Text.ToInt();
      cy = frnMain->edCenterY->Text.ToInt();
   }
   dx = cx - pnlCenterX->Caption.ToInt();
   dy = cy - pnlCenterY->Caption.ToInt();
   x = pnlPositionX->Caption.ToInt();
   y = pnlPositionY->Caption.ToInt();
   x += dx;
   y += dy;

   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }

   // Delay
   // 2023 5 5 - chc ���WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //   frnMain->tmISCCDTimer(frnMain);
   //}
   for(int i=0 ; i<2 ; i++) {
      frnMain->WaitTime(200);
      frnMain->tmISCCDTimer(frnMain);
   }

   // ��s��m
   UpdateMotionPosition();
   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Pattern Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }
   UpdateMotionPosition();
   // Die���W��
   x1 = pnlX->Caption.ToInt();
   y1 = pnlY->Caption.ToInt();
   // DieSize
   double fdiewno,fdiehno;
   double fdiew,fdieh;
   int idiewno,idiehno,px,py;
   AnsiString msg;
   fdiew = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
   fdieh = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
   dx = cx - x1;
   dy = cy - y1;
   fdiewno = (double)dx / fdiew;
   fdiehno = (double)dy / fdieh;
   idiewno = fdiewno;
   idiehno = fdiehno;
   //px = cx - (fdiewno - idiewno) * fdiew;
   //py = cy - (fdiehno - idiehno) * fdieh;
   px = x1 - idiewno * fdiew;
   py = y1 + idiehno * fdieh;
   msg.sprintf("Alignment cx,cy,x1,y1,fdiew,fdieh,fdiewno,fdiehno,idiewno,idiehno,px,py=%d,%d,%d,%d,%.2f,%.2f,%.2f,%.2f,%d,%d,%d,%d",
                cx,cy,x1,y1,fdiew,fdieh,fdiewno,fdiehno,idiewno,idiehno,px,py);
   frnMain->WriteSystemLog(msg);
   AdjustCenterX = px;
   AdjustCenterY = py;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAutoMatchClick(TObject *Sender)
{
int no,cx,cy,mcx,mcy,dx,dy;
bool ret;
double fdy,ccdprecision,fdx;
int axisno,position;
I32 cmd;
AnsiString msg,path;

   // Disable Std Frame
   if(shCenter->Visible == true) {
      cbStdPattern->Checked = false;
   }
   // Log
   frnMain->WriteSystemLog("btnAutoMatchClick Enter...");

   // �T�wCCD0
   no = 0;
   pnlAutoMatch->Color = clSilver;
   pnlAutoMatch->Refresh();
   if(boolCCDImageLoadedAry[no] == false) {
      pnlAutoMatch->Color = clRed;
      pnlSystemMessage->Caption = "No Image! Load/Capture image first.";
      frnMain->WriteSystemLog("btnAutoMatchClick: " + pnlSystemMessage->Caption);
      return;
   }

   // �j�Full Image
   if(cbCCDFullImage->Checked == false) {
      cbCCDFullImage->Checked = true;
   }

   AnsiString patternfname,recipename;
   int stdheight,stdwidth,areaheight,areawidth;

   // Get Recipe Name
   recipename = combRecipe->Text;
   path = edRecipePath->Text + "\\" + recipename + "\\";
   // ��1
   if(rgMatchSource->ItemIndex == 0) {
      patternfname = path + recipename + "-B1.bmp";
   }
   // ��2
   else if(rgMatchSource->ItemIndex == 1) {
      patternfname = path + recipename + "-B2.bmp";
   }
   // �q��
   else if(rgMatchSource->ItemIndex == 2) {
      patternfname = path + recipename + "-P.bmp";
   }
   // Check Recipe file
   if(!FileExists(patternfname)) {
      pnlSystemMessage->Caption = "Pattern File Not Exist! " + patternfname;
      pnlAutoMatch->Color = clRed;
      frnMain->WriteSystemLog("btnAutoMatchClick: " + pnlSystemMessage->Caption);
      return;
   }
   // Load Pattern
   if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
      frnMain->StdImage.Load(patternfname.c_str());
      stdheight = frnMain->StdImage.GetHeight();
      stdwidth = frnMain->StdImage.GetWidth();
   }
   else {
      frnMain->CStdImage.Load(patternfname.c_str());
      stdheight = frnMain->CStdImage.GetHeight();
      stdwidth = frnMain->CStdImage.GetWidth();
   }
   // Display Pattern
   imPattern->Picture->Bitmap->Width = stdwidth;
   imPattern->Picture->Bitmap->Height = stdheight;

   // Full CCD: 2448 - 1100
   if(cbCCDFullImage->Checked == true) {
      imPattern->Width = stdwidth / 2;
      imPattern->Height = stdheight / 2;
   }
   else {
      imPattern->Width = stdwidth;
      imPattern->Height = stdheight;
   }
   pnlPattern->Width = imPattern->Width + 10;
   pnlPattern->Height = imPattern->Height + 10;

   imPattern->Top = 5;
   imPattern->Left = 5;
   imPattern->Picture->LoadFromFile(patternfname);
   pnlPattern->Visible = true;

   // Log
   frnMain->WriteSystemLog("btnAutoMatchClick: Load StdPattern(W*H)= " + IntToStr(stdwidth) + "," + IntToStr(stdheight));

   areaheight = imInspection->Height;
   areawidth = imInspection->Width;
   ret = false;
   ret = DoNCCMatch(stdwidth,stdheight,patternfname);

   // �䤣��!!
   if(ret == false) {
      pnlSystemMessage->Caption = "Pattern Not found!";
      pnlAutoMatch->Color = clRed;
      return;
   }
   pnlAutoMatch->Color = clLime;

   // �O�_���줤�ߦ�m?
   if(cbMoveToCenterMatched->Checked == true) {
      // MarkCenterX, MarkCenterY: Match�������I(��CCD�өw�q: 2448*2048)
      int cx,cy,mcx,mcy;
      AnsiString msg;
      if(cbCCDFullImage->Checked == true) {
         double ratiox,ratioy;
         int w,h,iw,ih;
         // BW : CCDImageAry
         if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
            w = frnMain->CCDImageAry[no].GetWidth();
            h = frnMain->CCDImageAry[no].GetHeight();
         }
         else {
            w = frnMain->usb_ImageAry[no].GetWidth();
            h = frnMain->usb_ImageAry[no].GetHeight();
         }
         iw = imInspection->Width;
         ih = imInspection->Height;
         ratiox = (double)w / iw;
         ratioy = (double)h / ih;
         cx = shVertical->Left - imInspection->Left;
         cy = shHorizontal->Top - imInspection->Top;
         mcx = MarkCenterX / ratiox;
         mcy = MarkCenterY / ratioy;
         // mcx,mcy: �ন��imCCD�өw�q: 2448*2048
         mcx -= 0;
         mcy -= 0;
         // DoubleClickX,DoubleClickY: �HimCCD��(0,0)�I
         DoubleClickX = mcx;
         DoubleClickY = mcy;
         // Double Click
         imInspectionDblClick(this);
         msg.sprintf("btnAutoMatchClick(MoveToCenter): MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY);
         frnMain->WriteSystemLog(msg);
         MarkCenterX = cx + 0;
         MarkCenterY = cy + 0;
         // ���V
         shMarkMatch->Top = cy + imInspection->Top - shMarkMatch->Height/2;
         shMarkMatch->Left = cx + imInspection->Left - shMarkMatch->Width/2;
      }
      else {
         cx = shVertical->Left - imInspection->Left;
         cy = shHorizontal->Top - imInspection->Top;
         mcx = MarkCenterX;
         mcy = MarkCenterY;
         // mcx,mcy: �ন��imCCD�өw�q: 2448*2048
         mcx -= LaserCenterX;
         mcy -= LaserCenterY;
         // DoubleClickX,DoubleClickY: �HimCCD��(0,0)�I
         DoubleClickX = mcx;
         DoubleClickY = mcy;
         // Double Click
         imInspectionDblClick(this);
         msg.sprintf("btnAutoMatchClick(MoveToCenter): MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY);
         frnMain->WriteSystemLog(msg);
         MarkCenterX = cx + LaserCenterX;
         MarkCenterY = cy + LaserCenterY;
         // ���V
         shMarkMatch->Top = cy + imInspection->Top - shMarkMatch->Height/2;
         shMarkMatch->Left = cx + imInspection->Left - shMarkMatch->Width/2;
      }
   }
}
//---------------------------------------------------------------------------
// 2023 1 8 - chc ���I(Match�����W��)
int OX,OY;
// �n�O�����Table��m, �_�h����Pattern�ɷ|��: ���^���m�A�@��
I32 OTableX,OTableY;
// �_��j
int DetectNums;
int DontcareThreshold;
int MinReduceArea;
bool Rotated,Scaled,UsingDoncare;
double MinScore,MaxAngle,MinAngle,MaxScale,MinScale;
bool UsingColor;
bool LoadColor;
E_iVision_ERRORS NCCError;
char *NCCStr;
NCCFind FindObj;
//typedef struct
//{
//	int    Width;
//	int    Height;
//	double CX;
//	double CY;
//	double Angle;
//	double Scale;
//	double Score;
//}NCCFind;
bool __fastcall TfrmRecipeSet::DoNCCMatch(int stdwidth,int stdheight, AnsiString patternfname)

{
// �O���O�_���LMatch���e�@�~ - �令Global
static boolfirst = false;
static int w,h,iw,ih;
int row,col,no,x,y,x1,y1,x2,y2;
AnsiString learnfname,tmp;
int matchno, selectno;
double score;
unsigned char *ptr1,*ptr2;

// �Y���ƧC��0.7 �h�A���@��
bool boolfirstdo = true;
// Match���Ӧ�m�վ�
int offsetcnt = 0;

   frnMain->WriteSystemLog("DoNCCMatch: Enter...");

   // NCC �]�w���, �i���w�ƶq
   DetectNums = cbMatchNo->Text.ToInt();
   DontcareThreshold = 0;

   // �j���װ�Rotate�i��|����!
   // 2022 11 29 - chc ��Rotate
   //Rotated = true;
   Rotated = false;

   Scaled = false;
   UsingDoncare = false;
   MinScore = edMinScore->Text.ToDouble();

   // �j���װ�Rotate�i��|����! �Y�p��2��
   //MaxAngle = 10;
   //MinAngle = -10;
   MaxAngle = 2;
   MinAngle = -2;

   MaxScale = 1.1;
   MinScale = 0.9;
   UsingColor = true;
   LoadColor = false;
   MinReduceArea = 64;

// �Y���ƧC��0.7 �h�A���@��
again:

   shMarkMatch->Visible = false;
   frnMain->WriteSystemLog("DoNCCMatch: Enter...1");

   // ----------------------------------------------------------------------------
   // �H��l�v���B�z
   // �ӷ�: usb_ImageAry[no], usb_ImageROIAry[no]
   // �T�w��0
   no = 0;
   if(boolfirst == false) {
      // BW : CCDImageAry
      if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
         frnMain->ProcessImage.SetSize(&frnMain->CCDImageAry[no]);
         w = frnMain->CCDImageAry[no].GetWidth();
         h = frnMain->CCDImageAry[no].GetHeight();
      }
      else {
         frnMain->CProcessImage.SetSize(&frnMain->usb_ImageAry[no]);
         w = frnMain->usb_ImageAry[no].GetWidth();
         h = frnMain->usb_ImageAry[no].GetHeight();
      }
      iw = imInspection->Width;
      ih = imInspection->Height;
   }

   frnMain->WriteSystemLog("DoNCCMatch: SetSize.");
   // Image => ProcessImage
   for(row=0 ; row<h ; row++) {
      // BW : CCDImageAry
      if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
         ptr1 = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(0,row);
         ptr2 = (unsigned char *)frnMain->ProcessImage.GetImagePtr(0,row);
         memcpy(ptr2,ptr1,w);
      }
      else {
         ptr1 = (unsigned char *)frnMain->usb_ImageAry[no].GetImagePtr(0,row);
         ptr2 = (unsigned char *)frnMain->CProcessImage.GetImagePtr(0,row);
         memcpy(ptr2,ptr1,w*3);
      }
   }

   frnMain->WriteSystemLog("DoNCCMatch: Captured.");
   int cx,cy;

// Match Fail: ���Ӧ�m�վ�: offsetcnt
matchagain:

   if(cbCCDFullImage->Checked == true) {
      // �j�M�ϰ�: (x1,y1) - (x2,y2)
      x1 = 0;
      y1 = 0;
      // �ثeLaser Center�Ҧb��m
      // ��NCC�ӻ�, �ثe�e�����v���ϰ�O��(LaserCenterX,LaserCenterY)�_��
      cx = w / 2;
      cy = h / 2;
      // �nMatch���v���ϰ�(�۹�CCD����)
      // �O���_�I: �u�ݤQ�r���ߦ�m
      OX = 10;
      OY = 10;
      x1 = 10;
      y1 = 10;
      x2 = w - 10;
      y2 = h - 10;

      // 2022 11 23 - chc Match Fail: ���Ӧ�m�վ�
      if(offsetcnt == 1) {
         OX = 60;
         OY = 60;
      }
      else if(offsetcnt == 2) {
         OX = 80;
         OY = 80;
      }
      else if(offsetcnt == 3) {
         OX = 100;
         OY = 100;
      }
      else if(offsetcnt == 4) {
         OX = 120;
         OY = 120;
      }
      x1 = OX;
      y1 = OY;
      x2 = w - OX;
      y2 = h - OY;

   }
   else {

      // �j�M�ϰ�: (x1,y1) - (x2,y2)
      // LaserCenter X/Y
      // => Lasr Cneter: �T�w��80,35
      x1 = LaserCenterX;
      y1 = LaserCenterY;

      // �ثeLaser Center�Ҧb��m
      // ��NCC�ӻ�, �ثe�e�����v���ϰ�O��(LaserCenterX,LaserCenterY)�_��
      cx = (shVertical->Left-imInspection->Left + x1);
      cy = (shHorizontal->Top-imInspection->Top + y1);

      // �nMatch���v���ϰ�(�۹�CCD����)
      // �O���_�I: �u�ݤQ�r���ߦ�m
      x1 = cx - imInspection->Width/2 + 10;
      y1 = cy - imInspection->Height/2 + 10;
      x2 = cx + imInspection->Width/2 - 10;
      y2 = cy + imInspection->Height/2 - 10;
      OX = LaserCenterX + 10;
      OY = LaserCenterY + 10;
   }
   tmp.sprintf("DoNCCMatch: (x1,y1,x2,y2,OX,OY) = (%d,%d,%d,%d,%d,%d)",x1,y1,x2,y2,OX,OY);
   frnMain->WriteSystemLog(tmp);

   // �]�n�O�����Table��m, �_�h����Pattern�ɷ|��: ���^���m�A�@��
   // => ?
   GetPosition(X_AXIS, &OTableX);
   GetPosition(Y_AXIS, &OTableY);

   // NCC, �]�wGrayImg & ���o�v��Data: x1,y1,W:(x2-x1),H:(y2-y1)
   if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
      frnMain->MatchImageROI.Detach();
      frnMain->MatchImageROI.Attach(&frnMain->ProcessImage);
      frnMain->MatchImageROI.SetPlacement(x1,y1,(x2-x1+1),(y2-y1+1));                        // X,Y,Width,Height
   }
   else {
      frnMain->CMatchImageROI.Detach();
      frnMain->CMatchImageROI.Attach(&frnMain->CProcessImage);
      frnMain->CMatchImageROI.SetPlacement(x1,y1,(x2-x1+1),(y2-y1+1));                        // X,Y,Width,Height
   }
   AnsiString imgfilename;

   // BW : CCDImageAry
   if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
      imgfilename = frnMain->SystemDirectory + "\\GrayImg.bmp";
      frnMain->MatchImageROI.Save(imgfilename.c_str(),E_FILE_FORMAT_GRAY_LEVEL_BMP);
      frnMain->WriteSystemLog("DoNCCMatch: Save GrayImg.bmp.");

      // 2023 3 2 - chc Delay
      Sleep(500);

      NCCError = iReadImage(GrayImg,imgfilename.c_str());
      frnMain->WriteSystemLog("DoNCCMatch: Read GrayImg.bmp. " + IntToStr(NCCError));
   }
   else {
      imgfilename = frnMain->SystemDirectory + "\\ColorImg.bmp";
      frnMain->CMatchImageROI.Save(imgfilename.c_str(),E_FILE_FORMAT_COLOR_BMP);
      frnMain->WriteSystemLog("DoNCCMatch: Save ColorImg.bmp. " + imgfilename);

      // 2023 3 2 - chc Delay
      Sleep(500);

      if(FileExists(imgfilename)) {
         NCCError = iReadImage(ColorImg,imgfilename.c_str());
         frnMain->WriteSystemLog("DoNCCMatch: Read ColorImg.bmp. retcode=" + IntToStr(NCCError) + "," + imgfilename);
      }
      else {
         frnMain->WriteSystemLog("DoNCCMatch: Read ColorImg.bmp not exist: " + imgfilename);
         return false;
      }
   }

   NCCStr = iGetErrorText(NCCError);
   pnlSystemMessage->Caption = NCCStr;
   frnMain->WriteSystemLog(pnlSystemMessage->Caption + "," + imgfilename);
   // ###
   if(NCCError != E__OK) {
      frnMain->WriteSystemLog("DoNCCMatch: Read ColorImg.bmp Fail!");
      pnlSystemMessage->Caption = (AnsiString)NCCStr + "-ILoad";
      return false;
   }

   // Match���ɶ�
   frnMain->GetTimeTic(&NCCStartTime,&NCCStartTick);
   if(boolfirst == false) {
      if(CheckNCCMatch() == false) {
         pnlSystemMessage->Caption = "NCC Dongle Not Exist!";
         frnMain->WriteSystemLog("DoNCCMatch: NCC dongle NotExist!");
         return false;
      }
      boolfirst = true;
      // �]�w�Ѽ�
      iSetAngle(iMatchModel,MaxAngle,MinAngle);
      iSetScale(iMatchModel,MaxScale,MinScale);

      // 2022 11 25 - chc �[�]Robustnes
      iSetRobustness(iMatchModel,true);

      // Minscore & DefectNums Must set each time
      //iSetMinScore(iMatchModel,MinScore);
      //iSetOccurrence(iMatchModel,DetectNums);

      iSetMinReduceArea(iMatchModel,MinReduceArea);
      iSetIsRotated(iMatchModel,Rotated);
      iSetIsScaled(iMatchModel,Scaled);
      if(iIsColorModel(iMatchModel) == E_TRUE)
         UsingColor = true;
      else
         UsingColor = false;
      frnMain->WriteSystemLog("DoNCCMatch: Set Parameter.");
   }

   // Minscore & DefectNums Must set each time
   iSetMinScore(iMatchModel,MinScore);
   iSetOccurrence(iMatchModel,DetectNums);
   frnMain->WriteSystemLog("DoNCCMatch: Set Score, Occurrence.");

   // Learn Std: �w�b�I�s�e���J: patternfname
   //learnfname = RecipeDirectory + "\\" + RecipeName + "\\" + RecipeName + "-p.bmp";
   //if(!FileExists(learnfname)) {
   //   pnlSystemMessage->Caption = "Pattern File Not Exist! " + learnfname;
   //   return false;
   //}
   //StdImage.Load(learnfname.c_str());
   learnfname = patternfname;

   // BW : CCDImageAry
   if(frnMain->CCDInfoAry[no].Type == CCD_BW)
      NCCError = iReadImage(GrayLearnImg,learnfname.c_str());
   else
      NCCError = iReadImage(ColorLearnImg,learnfname.c_str());
   NCCStr = iGetErrorText(NCCError);
   pnlSystemMessage->Caption = NCCStr;
   // ###
   if(NCCError != E__OK) {
      pnlSystemMessage->Caption = (AnsiString)NCCStr + "-LLoad!";
      frnMain->WriteSystemLog("DoNCCMatch: iReadImage Error!");
      return false;
   }
   frnMain->WriteSystemLog("DoNCCMatch: Read Pattern.");

   LoadColor = true;
   iSetDontCareThreshold(iMatchModel, DontcareThreshold);

   if(frnMain->CCDInfoAry[no].Type == CCD_BW)
      NCCError = CreateNCCModel(GrayLearnImg,iMatchModel,UsingDoncare);
   else
      NCCError = CreateNCCModel(ColorLearnImg,iMatchModel,UsingDoncare);

   // ###
   if(NCCError != E__OK) {
      NCCStr = iGetErrorText(NCCError);
      pnlSystemMessage->Caption = (AnsiString)NCCStr + "-Learn!";
      frnMain->WriteSystemLog("DoNCCMatch: CreateNCCModel Error!");
      return false;
   }
   frnMain->WriteSystemLog("DoNCCMatch: CreateNCCModel." + IntToStr(OX));

   if(frnMain->CCDInfoAry[no].Type == CCD_BW)
      NCCError = MatchNCCModel(GrayImg,iMatchModel);
   else
      NCCError = MatchNCCModel(ColorImg,iMatchModel);
   frnMain->WriteSystemLog("DoNCCMatch: MatchNCCModel.");

   // ###
   if(NCCError != E__OK) {
      NCCStr = iGetErrorText(NCCError);
      pnlSystemMessage->Caption = (AnsiString)NCCStr + "Match!";
      matchno = 0;
      frnMain->WriteSystemLog("DoNCCMatch: MatchNCCModel Fail!");
      return false;
   }
   else {
      iGetNCCMatchNum(iMatchModel,&matchno);
      frnMain->WriteSystemLog("DoNCCMatch: MatchNCCModel ok= " + IntToStr(matchno));
   }
   // Update Matched No
   pnlMatchedNo->Caption = IntToStr(matchno);

   // ����
   if(matchno < 1) {
      pnlSystemMessage->Caption = "Match Fail!";
      frnMain->WriteSystemLog("DoNCCMatch: Match Fail!");

      // Match Fail: ���Ӧ�m�վ�: offsetcnt
      offsetcnt++;
      if(offsetcnt < 5) {
         frnMain->WriteSystemLog("DoNCCMatch: Match Fail! ����: " + IntToStr(offsetcnt));
         goto matchagain;
      }

      return false;
   }

   // 2022 11 25 - chc for test
   //else {
   //   if(boolForTest == true) {
   //      // 2022 11 23 - chc Match Fail: ���Ӧ�m�վ�: offsetcnt
   //      offsetcnt++;
   //      if(offsetcnt < 5) {
   //         frnMain->WriteSystemLog("DoNCCMatch: Match Ok! ����: " + IntToStr(offsetcnt));
   //         goto matchagain;
   //      }
   //   }
   //}

   frnMain->GetTimeTic(&NCCStopTime,&NCCStopTick);
   NCCElapsedms = ((NCCStopTime*1000+NCCStopTick) - (NCCStartTime*1000+NCCStartTick));
   pnlSystemMessage->Caption = "Match Success. " + IntToStr(NCCElapsedms) + "ms";

   // Sort Mode of Matched : maxno: 10
   // sort by near
   // AddsgResult(index,FindObj.Width,FindObj.Height,FindObj.CX,FindObj.CY,FindObj.Angle,FindObj.Scale,FindObj.Score);
   // void __fastcall TMainForm::AddsgResult(int index, int width, int height, int cx, int cy, double angle, double scale, double score)
   struct SORT_STRU     {
      double Score;
      double Distance;
      int Index;
      bool Selected;
      bool Done;
   };
   struct SORT_STRU     sortbuffer[15];
   int sx,sy,selno,sortedno,ino;
   double dx,dy,minscore,mindistance;

   // ���Ĥ@��Match��
   for(selectno=0 ; selectno<1 ; selectno++) {
      iGetNCCMatchResults(iMatchModel,selectno,&FindObj);
      if(selectno > 0)
         continue;
      x = FindObj.CX;
      y = FindObj.CY;

      // �ե�: OX = LaserCenterX + 10, OY = LaserCenterY + 10
      x += OX;
      y += OY;

      // �s��Global: ��l2448*2048�v����
      MarkCenterX = x;
      MarkCenterY = y;

      score = FindObj.Score;
      tmp.sprintf("%.3f",score);
      pnlScore->Caption = tmp;
      // ���V
      // ��NCC�ӻ�, �ثe�e�����v���ϰ�O��(LaserCenterX,LaserCenterY)�_��: �n��LaserCenter
      if(cbCCDFullImage->Checked == true) {
         double ratiox,ratioy;
         ratiox = (double)w / iw;
         ratioy = (double)h / ih;
         if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
            shMarkMatch->Width = frnMain->StdImage.GetWidth() / ratiox;
            shMarkMatch->Height = frnMain->StdImage.GetHeight() / ratioy;
         }
         else {
            shMarkMatch->Width = frnMain->CStdImage.GetWidth() / ratiox;
            shMarkMatch->Height = frnMain->CStdImage.GetHeight() / ratioy;
         }
         shMarkMatch->Top = y / ratioy + imInspection->Top - shMarkMatch->Height/2 - 1;
         shMarkMatch->Left = x / ratiox + imInspection->Left - shMarkMatch->Width/2 - 1;
      }
      else {
         if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
            shMarkMatch->Width = frnMain->StdImage.GetWidth();
            shMarkMatch->Height = frnMain->StdImage.GetHeight();
         }
         else {
            shMarkMatch->Width = frnMain->CStdImage.GetWidth();
            shMarkMatch->Height = frnMain->CStdImage.GetHeight();
         }
         shMarkMatch->Top = y + imInspection->Top - shMarkMatch->Height/2 - LaserCenterY;
         shMarkMatch->Left = x + imInspection->Left - shMarkMatch->Width/2 - LaserCenterX;
      }
      shMarkMatch->Visible = true;
      shMarkMatch->Refresh();
      pnlAutoMatch->Color = clLime;

      // �[�J���׭�
      //tmp.sprintf("%.3f",FindObj.Angle);
      //pnlRotateAngle->Caption = tmp;

      tmp.sprintf("DoNCCMatch: MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,(int)FindObj.CX,(int)FindObj.CY,LaserCenterX,LaserCenterY);
      frnMain->WriteSystemLog(tmp);

   }

   // 2019 12 4 - chc �Y���ƧC��0.7 �h�A���@��
   // 2022 11 24 - chc ����
   //if(score < 0.7) {
   //   if(boolfirstdo == true) {
   //      boolfirstdo = false;
   //      goto again;
   //   }
   //}

   frnMain->WriteSystemLog("DoNCCMatch: Done.");

   return true;
}
//---------------------------------------------------------------------------
// 2023 1 8 - chc NCC
// iVisitingKeyPro or iGetKeyProState
bool __fastcall TfrmRecipeSet::CheckNCCMatch()
{
char* VersionStr;
char* VersionDateStr;

   VersionStr = iGetiImageVersion();
   VersionDateStr = iGetiMatchVersionDate();

   // iMatchModel Keypro
   E_iVision_ERRORS Err = E_NULL;

   // E_TRUE or E_FALSE
   // ���iVisitingKey()
   Err = iVisitingKey();

   // iGetKeyProState(E_TRUE/E_FALSE)����! ���iVisitingKeyPro(E_OK1)
   if(Err != E__OK)
      return false;
   return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbCCDFullImageChange(TObject *Sender)
{

   // 2022 10 7 - chc �LCCD, ImageLoaded
   if(boolCCDImageLoadedAry[0] == true) {
      int w,h,width,height,sx,sy,x,y,no;
      no = 0;
      if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
         width = frnMain->CCDImageAry[no].GetWidth();
         height = frnMain->CCDImageAry[no].GetHeight();
         w = imInspection->Width;
         h = imInspection->Height;
         if(cbCCDFullImage->Checked == true) {
            w = width;
            h = height;
         }
         sx = (width - imInspection->Width) / 2;
         sy = (height - imInspection->Height) / 2;
         imInspection->Picture->Bitmap->Width = w;
         imInspection->Picture->Bitmap->Height = h;
         frnMain->CCDImageROIAry[no].Detach();
         frnMain->CCDImageROIAry[no].Attach(&frnMain->CCDImageAry[no]);
         frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
         frnMain->CCDImageROIAry[no].SetPlacement(x,y,w,h);
         frnMain->CCDImageROIAry[no].Draw(imInspection->Canvas->Handle);
      }
      else {
         width = frnMain->usb_ImageAry[no].GetWidth();
         height = frnMain->usb_ImageAry[no].GetHeight();
         w = imInspection->Width;
         h = imInspection->Height;
         if(cbCCDFullImage->Checked == true) {
            w = width;
            h = height;
         }
         sx = (width - imInspection->Width) / 2;
         sy = (height - imInspection->Height) / 2;
         imInspection->Picture->Bitmap->Width = w;
         imInspection->Picture->Bitmap->Height = h;
         frnMain->usb_ImageROIAry[no].Detach();
         frnMain->usb_ImageROIAry[no].Attach(&frnMain->usb_ImageAry[no]);
         frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
         frnMain->usb_ImageROIAry[no].SetPlacement(x,y,w,h);
         frnMain->usb_ImageROIAry[no].Draw(imInspection->Canvas->Handle);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::imPatternClick(TObject *Sender)
{

   pnlPattern->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::imInspectionMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int gray,sx,sy;

   // �D�e��
   unsigned char r,g,b;
   TColor color;
   sx = 0;
   sy = 0;

   // ���v�����
   double ratiox,ratioy;
   int x,y;
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImage->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imInspection->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imInspection->Height;
      x = X * ratiox;
      y = Y * ratioy;
      color = imInspection->Canvas->Pixels[x][y];
   }
   else

      color = imInspection->Canvas->Pixels[X][Y];
   b = (color & 0xff0000) >> 16;
   g = (color & 0x00ff00) >> 8;
   r = color & 0x0000ff;
   // ��ܴ�Э�
   pnlCCDR->Caption = r;
   pnlCCDG->Caption = g;
   pnlCCDB->Caption = b;
   pnlCCDX->Caption = X + sx;
   pnlCCDY->Caption = Y + sy;

   // Move
   if(ImageLeftMouse == true && cbFreeze->Checked == true && cbCCDFullImage->Checked == false) {
      int dx,dy,sx,sy;
      int width,height,w,h,no,w1,h1;
      w1 = imInspection->Width;
      h1 = imInspection->Height;
      dx = X - ImageXPos;
      dy = Y - ImageYPos;
      width = GigaWidth;
      height = GigaHeight;

      // 2023 3 17 - chc �O���i���d��
      //w = width / ImageFactor;
      //h = height / ImageFactor;
      w = w1 / ImageFactor;
      h = h1 / ImageFactor;

      // 2023 3 17 - chc �u��ImageFactor�N�i�H
      //dx = (dx * width) / (w1 * ImageFactor);
      //dy = (dy * height) / (h1 * ImageFactor);
      dx = dx / ImageFactor;
      dy = dy / ImageFactor;

      sx = ImageSX - dx;
      sy = ImageSY - dy;
      if(sx > 0) {
         ImageSX = sx;
      }
      else
         ImageSX = 0;
      if(sy > 0) {
         ImageSY = sy;
      }
      else
         ImageSY = 0;
      if((ImageSX + w) > width)
         ImageSX = width - w;
      if((ImageSY + h) > height)
         ImageSY = height - h;

      no = 0;
      imInspection->Picture->Bitmap->Width = w;
      imInspection->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageSX,ImageSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imInspection->Canvas->Handle);
      imInspection->Refresh();
   }
   ImageXPos = X;
   ImageYPos = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveLeftUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row--;
      col--;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row--;
      sgDie->Col--;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fdx;
      y -= fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();

      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fsx;
      y -= fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveLeftClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      col--;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Col--;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fdx;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fsx;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveLeftDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row++;
      col--;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row++;
      sgDie->Col--;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fdx;
      y += fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x += fsx;
      y += fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row--;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row--;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      y -= fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      y -= fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveSpeedClick(TObject *Sender)
{
int no;

   no = frnMain->rgSpeed->ItemIndex;
   no++;
   if(no > JOG_HIGH_SPEED)
      no = JOG_LOW_SPEED;

   frnMain->rgSpeed->ItemIndex = no;
   rgSpeed->ItemIndex = no;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row++;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row++;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      y += fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      y += fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveRightUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row--;
      col++;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row--;
      sgDie->Col++;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fdx;
      y -= fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fsx;
      y -= fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveRightClick(TObject *Sender)
{

   // 2023 3 22 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      col++;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Col++;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fdx;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fsx;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveRightDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc �令�ثe��m����
   /*
   int col,row;
   if(rgMoveStep->ItemIndex == 4) {
      if(pnlWorkCol->Caption == "" || pnlWorkRow->Caption == "")
         return;
      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
      row++;
      col++;
      edSelectCol->Text = IntToStr(col);
      edSelectRow->Text = IntToStr(row);
      btnColRowToClick(this);
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      sgDie->Row++;
      sgDie->Col++;
      sgDieDblClick(this);
      return;
   }
   */
   int x,y,fdx,fdy,fsx,fsy,sxno,syno;
   if(rgMoveStep->ItemIndex == 4) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      fdx = edChipWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fdy = edChipHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fdx;
      y += fdy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }
   else if(rgMoveStep->ItemIndex == 5) {
      UpdateMotionPosition();
      x = pnlX->Caption.ToInt();
      y = pnlY->Caption.ToInt();
      if(edSubDieXno->Text == "")
         edSubDieXno->Text = "1";
      sxno = edSubDieXno->Text.ToInt();
      if(sxno <= 0)
         sxno = 1;
      if(edSubDieYno->Text == "")
         edSubDieYno->Text = "1";
      syno = edSubDieYno->Text.ToInt();
      if(syno <= 0)
         syno = 1;
      fsx = edSubDieWidth->Text.ToDouble() / frnMain->X_RESOLUTION;
      fsy = edSubDieHeight->Text.ToDouble() / frnMain->Y_RESOLUTION;
      x -= fsx;
      y += fsy;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc ���ݭn
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCW1xClick(TObject *Sender)
{

   // 2023 4 19 - chc �令�T��
   /*
   return;

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = true;
   */
   
   frnMain->btnBackwardTClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCCW1xClick(TObject *Sender)
{

   // 2023 4 19 - chc �令�T��
   /*
   return;

   // 2023 2 17 - chc �o��
   frnMain->cbContinueMove->Checked = true;
   */
   
   frnMain->btnMoveTClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::rgCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 0;
   frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::rgBXFMLensChange(TObject *Sender,
      int ButtonIndex)
{

   // 2023 4 4 - chc �۰ʽ�Shutter
   // 2023 4 17 - chc ����
   //int ono,nno,sono,snno;
   //ono = frnMain->rgBXFMLens->ItemIndex;                                        // 0 - 5x
   //nno = rgBXFMLens->ItemIndex;                                                 // 3 - 50x
   //sono = rgCCDShutter->ItemIndex;                                              // 6 - 1/200
   //snno = sono + (ono - nno);                                                   // 6  + (0 - 3) = 3
   //if(snno < 0)
   //   snno = 0;
   //if(snno > 15)
   //   snno = 15;
   //rgCCDShutter->ItemIndex = snno;

   frnMain->rgBXFMLens->ItemIndex = rgBXFMLens->ItemIndex;
   frnMain->rgBXFMLensChange(Sender, ButtonIndex);

   // 2023 5 3 - chc Log
   frnMain->WriteSystemLog("rgBXFMLensChange: " + IntToStr(rgBXFMLens->ItemIndex));

   pnlBXFMLens->Color = frnMain->pnlBXFMLens->Color;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::tbBXFMLMPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

int no;

   no = tbBXFMLMP->Position;
   SetIntensity(no);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::SetIntensity(int no)
{
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;
   tbBXFMLMP->Position = no;

   frnMain->tbBXFMLMP->Position = tbBXFMLMP->Position;
   frnMain->tbBXFMLMPMouseUp(this,Button,Shift,X,Y);
   pnlBXFMLMPValue->Caption =  frnMain->pnlBXFMLMPValue->Caption;
   pnlBXFMLampSet->Color =  frnMain->pnlBXFMLampSet->Color;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnDefaultIntensityClick(TObject *Sender)
{
int no;

   no = 90;
   SetIntensity(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnDecClick(TObject *Sender)
{
int no;

   no = tbBXFMLMP->Position;
   if(no > 0) {
      no--;
      SetIntensity(no);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAddClick(TObject *Sender)
{
int no;

   no = tbBXFMLMP->Position;
   if(no < 120) {
      no++;
      SetIntensity(no);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnDefaultASClick(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 3113;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS2Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 1000;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS1Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 500;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS3Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 1500;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS4Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 2000;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS5Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 2500;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS6Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 3113;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::rgMoveStepClick(TObject *Sender)
{

   // 2023 3 2 - chc die Move
   if(rgMoveStep->ItemIndex < 4)

      frnMain->rgMoveStep->ItemIndex = rgMoveStep->ItemIndex;
}
//---------------------------------------------------------------------------
// Save XYZ Position
void __fastcall TfrmRecipeSet::UpdateMotionPosition()
{
// 2023 5 6 - chc Re-Entrant
bool active = false;
   if(active == true)
      return;
   active = true;

   pnlX->Caption = frnMain->pnlX->Caption;
   pnlY->Caption = frnMain->pnlY->Caption;
   pnlZ->Caption = frnMain->pnlZ->Caption;
   pnlT->Caption = frnMain->edCounterT->Text;
   if(boolDrawImage == true) {
      UpdateCurrentPosition();
   }

   // 2023 5 6 - chc Re-Entrant
   active = false;
}
//---------------------------------------------------------------------------
// Save XYZ Position
void __fastcall TfrmRecipeSet::UpdateCurrentPosition()
{
int x,y,sw,sh,cx,cy,sx,sy,tcx,tcy,w,h,px,py;
double ratio,fx,fy,sizew;

   x = pnlX->Caption.ToInt();
   y = pnlY->Caption.ToInt();
   sw = shCurretnPosition->Width;
   sh = shCurretnPosition->Height;

   // pixel
   sx = Wafer.X;
   sy = Wafer.Y;
   cx = Wafer.Cx;
   cy = Wafer.Cy;
   w = Wafer.Width;
   h = Wafer.Height;

   // 2023 3 17 - chc Check Divide b zero
   if(w == 0)
      return;

   tcx = frnMain->edCenterX->Text.ToInt();
   tcy = frnMain->edCenterY->Text.ToInt();
   // 12", um/Pixel
   if(rgWaferType->ItemIndex == 0) {
      sizew = 300000;
      ratio = sizew / (double)w;
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO)
         sizew = 200000;
      else
         sizew = 300000;
      ratio = sizew / (double)w;
   }
   // um
   fx = (double)(x - tcx) * frnMain->X_RESOLUTION;
   fy = (double)(y - tcy) * frnMain->Y_RESOLUTION;
   // pixel
   px = fx / ratio;
   py = fy / ratio;

   shCurretnPosition->Visible = true;

   // 2023 3 21 - chc �ץ�: imWaferMap
   //shCurretnPosition->Left = cx - px - sw/2;
   //shCurretnPosition->Top = cy + py - sh/2;
   shCurretnPosition->Left = cx - px - sw/2 + imWaferMap->Left;
   shCurretnPosition->Top = cy + py - sh/2 + imWaferMap->Top;

}
//---------------------------------------------------------------------------
// Remote Rotate
// �p�g��
//#define T_RESOLUTION    0.001
//#define X_RESOLUTION    0.1
void __fastcall TfrmRecipeSet::RemoteRotate(int x1,int y1, int x2, int y2)
{
double dx,ccdprecision,dy,degree,meany,value,fdy,fcmd,pdegree;
I32 cmd,ypos;
AnsiString msg;
int axisno,pos,level;
bool ret;

   frnMain->WriteSystemLog(">[�����ॿ]");
   // ���o���
   ccdprecision = GetLensPrecision();

   // ��Pulse
   RotateLeftXPos = x1;
   RotateLeftYPos = y1;
   RotateRightXPos = x2;
   RotateRightYPos = y2;
   RotateLeftXCmd = x1;
   RotateRightXCmd = x2;

   // �O���ثeY��m
   GetPosition(Y_AXIS, &ypos);
   // X�b�Z��, pulse
   dx = abs(RotateLeftXPos-RotateRightXPos);
   // XY��פ��P
   dx *= (frnMain->X_RESOLUTION / frnMain->Y_RESOLUTION);

   // any Position, Left: Larger: �[�JPosition���O��, �_�h���Z�����ॿ�P��e�����ॿ��V�ۤ�!!
   if(RotateLeftXCmd != RotateRightXCmd) {                                   // ���ʨ�X-Axis��m
      if(RotateLeftXPos > RotateRightXPos)
         dy = RotateRightYPos - RotateLeftYPos;
      else
         dy = RotateLeftYPos - RotateRightYPos;
   }
   else {
      if(RotateLeftXPos < RotateRightXPos)
         dy = RotateRightYPos - RotateLeftYPos;
      else
         dy = RotateLeftYPos - RotateRightYPos;
   }
   if(dx == 0 || dy == 0) {
      degree = 0;
      pdegree = 0;
   }
   else {
      // ��Pulse
      value = (double)(dy) / (dx);
      // �y�׶q
      degree = atan(value);
      pdegree = degree;
      // Log
      msg.sprintf("LeftX,RightX,LeftY,RightY,dx,dy,value,rd=%d,%d,%d,%d,%.1f,%.1f,%.4f,%.4f",RotateLeftXPos,RotateRightXPos,RotateLeftYPos,RotateRightYPos,dx,dy,value,degree);
      frnMain->WriteSystemLog(msg);

      // �׫׶q
      degree = (360 * degree) / (2*M_PI);
      // �w�qT Resolution: TResolution;

      // 2023 3 24 - chc Adjust
      //pos = degree / T_RESOLUTION;
      pos = (degree / T_RESOLUTION + 0.5);

      // Log
      msg.sprintf("degree,TPulse,TResolution,CCDPrecision=%.4f,%d,%.8f,%.5f",degree,pos,T_RESOLUTION,ccdprecision);
      frnMain->WriteSystemLog(msg);

      if(cbRotateDirection->Checked == false)
         pos = 0 - pos;
      axisno = T_AXIS;
      GetPosition(axisno, &cmd);
      pos = pos + cmd;
      if(MoveToT(pos) == false) {
         pnlSystemMessage->Caption = "T���{�ॿ����!";
         pnlRemoteRotate->Color = clRed;
         // Beep�n
         Beep(500);
      }
      else {
         pnlRemoteRotate->Color = clLime;
         // �A�s��5���ॿ
//         CheckTPosition(T_AXIS,pos);
      }
   }

   // �ॿ��
   double dg;
   dg = degree;
   if(cbRotateDirection->Checked == false)
      dg = 0 - dg;
   msg.sprintf("%.4f",dg);
   pnlDegree->Caption = msg;

   // �Y��0��, ���ݭn����
   if(degree == 0)
      return;

   for(int i=0 ; i<5 ; i++) {
      Sleep(100);
      Application->ProcessMessages();
   }
   if(Application->MessageBox("Alignment: Done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   double fx,fy;
   // �n�Υ��k�������I, ���O�ثe��m(�]�ثe��m�i�ఱ�b���Υk)
   // => RotateRightXPos, RotateLeftXPos �w�নpulse
   //axisno = X_AXIS;
   //GetPosition(axisno, &fx);
   fx = (RotateRightXPos + RotateLeftXPos) / 2;

   // XY��פ��P
   fx *= (frnMain->X_RESOLUTION / frnMain->Y_RESOLUTION);

   // ypos�Pfy�O�ۦP��
   axisno = Y_AXIS;
   GetPosition(axisno, &cmd);
   fy = cmd;

   // ��������
   if(cbDirectMove->Checked == true) {
      /*
      meany = (RotateLeftYPos + RotateRightYPos) / 2;
      pos = meany;
      // �A�ե�
      if(cbDirectMoveDirection->Checked == true)
         pos -= (meany - ypos);
      else
         pos += (meany - ypos);
      msg.sprintf("OffsetMove: Pos1y,Pos2y,Curry,mean,pos=%d,%d,%d,%.5f",RotateLeftYPos,RotateRightYPos,cmd,(int)meany,pos,);
      frnMain->WriteSystemLog(msg);
      if(MoveToY(pos) == false) {
         pnlSystemMessage->Caption = "����Y���ʥ���!";
         // Beep�n
         Beep(500);
      }
      */
      // Match again
      btnAutoMatchClick(this);
      if(pnlAutoMatch->Color != clLime) {
         pnlSystemMessage->Caption = "Base1-Offset Match Fail!";
         pnlAlignment->Color = clRed;
         // Beep�n
         Beep(500);
      }
   }

   // ����X�b
   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;
   msg.sprintf("%d",(int)dy);
   pnlTRemoteOffset->Caption = msg;
   msg.sprintf("%.3f",degree);
   pnlTRemoteDegree->Caption = msg;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   MousePosX = X;
   MousePosY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::MoveToXYZT(int x,int y,int z,int t)
{

   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }
   if(MoveToT(t) == false) {
      pnlSystemMessage->Caption = "T Move Fail!";
      // Beep�n
      Beep(500);
   }
   if(MoveToZ(z) == false) {
      pnlSystemMessage->Caption = "Z Move Fail!";
      // Beep�n
      Beep(500);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::pnlBase1XDblClick(TObject *Sender)
{
int x,y,z,t;

   if(pnlBase1X->Caption == "" || pnlBase1Y->Caption == "" || pnlStdZ->Caption == "" || pnlStdT->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Value!";
      return;
   }
   x = pnlBase1X->Caption.ToInt();
   y = pnlBase1Y->Caption.ToInt();
   z = pnlStdZ->Caption.ToInt();
   t = pnlStdT->Caption.ToInt();
   MoveToXYZT(x,y,z,t);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::pnlBase2XDblClick(TObject *Sender)
{
int x,y,z,t;

   if(pnlBase2X->Caption == "" || pnlBase2Y->Caption == "" || pnlStdZ->Caption == "" || pnlStdT->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Value!";
      return;
   }
   x = pnlBase2X->Caption.ToInt();
   y = pnlBase2Y->Caption.ToInt();
   z = pnlStdZ->Caption.ToInt();
   t = pnlStdT->Caption.ToInt();
   MoveToXYZT(x,y,z,t);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::pnlPositionXDblClick(TObject *Sender)
{
int x,y,z,t;

   if(pnlPositionX->Caption == "" || pnlPositionY->Caption == "" || pnlStdZ->Caption == "" || pnlStdT->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Value!";
      return;
   }
   x = pnlPositionX->Caption.ToInt();
   y = pnlPositionY->Caption.ToInt();
   z = pnlStdZ->Caption.ToInt();
   t = pnlStdT->Caption.ToInt();
   MoveToXYZT(x,y,z,t);
}
//---------------------------------------------------------------------------
// Save Micro Recipe
// RecipeName
// WaferSize: 12"/8"
// Pattern: Base1/Base2/Position XYZT
// Match: MinScore, MatchMo
// BXFM: ObjLens,Intensity,AS,Observation, AF Active
// Chip: Width,Height,GapX,GapY,Ignore
// CCD: Shutter, WhiteBalance
// DIC: Active, FCNear, FCFar
/*
struct XYZT_STRU {
   int X,Y,Z,T;
};
struct PATTERN_STRU {
   struct XYZT_STRU Base1;
   struct XYZT_STRU Base2;
   struct XYZT_STRU Position;
};
struct MATCH_STRU {
   double MinScore;
   int No;
};
struct BXFM_STRU {
   int Lens,Intensity,AS,Observation;
   bool boolAF;
};
struct CHIP_STRU {
   int Width,Height,Dx,Dy,Ignore;
};
struct CCD_STRU {
   int Shutter;
   bool boolAutoWB;
};
struct DIC_STRU {
   int FCValue;
   bool boolActive;
   int OBsevationMode;
};
struct MICRO_RECIPE_STRU {
   struct PATTERN_STRU Pattern;
   struct MATCH_STRU Match;
   struct BXFM_STRU BXFM;
   struct DIC_STRU DIC;
   struct CCD_STRU CCD;
   struct CHIP_STRU Chip;
};

// Macro
struct SIDE_STRU {
   int LoadPosition;
   int HomeDuration;
   int LeftDuration;
   int RightDuration;
   int ViewDuration;
   int LeftLightValue;
   int RightLightValue;
   int LeftCCDShutter;
   int RightCCDShutter;
};
struct MACRO_RECIPE_STRU {
   struct SIDE_STRU Top;
   struct SIDE_STRU Back;
};
// Recipe
struct RECIPE_STRU {
   AnsiString Name;
   int WaferSize;                                                               // 8/12
   struct MICRO_RECIPE_STRU MicroRecipe;
   struct MACRO_RECIPE_STRU MacroRecipe;
};
*/

void __fastcall TfrmRecipeSet::imWaferMapMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   if(Wafer.boolSet == true) {
      int col,row;
      // �����q
      col = (X - COffsetX - Wafer.X) / Wafer.Dx;
      row = (Y - COffsetY - Wafer.Y) / Wafer.Dy;
      if(X < (COffsetX + Wafer.X))
         col = -1;
      if(Y < (COffsetY + Wafer.Y))
         row = -1;

      // ���I��m: GridNoY
      // �U: 180
      // 2023 3 1 - chc �̮y�ШM�w
      //if(rgWaferDegree->ItemIndex == 2) {
      if(rgWaferAxisOrientation->ItemIndex == 1) {

         row = GridNoY - row - 1;
         // �̤pRow,Col
         col -= MinCol;

         // 2023 1 2 - chc ��
         //row -= (GridNoY - MinRow - 1);
         row -= (MinRow + 1);

      }

      // 2023 3 2 - chc �ե�
      // 2023 3 21 - chc ���ݭn�ե�, ����I��(0,0)
      //col++;
      //row++;

      pnlChipCol->Caption = IntToStr(col);
      pnlChipRow->Caption = IntToStr(row);

      // 2023 3 21 - chc �[�JHint
      AnsiString msg;
      msg.sprintf("(%d,%d)",col,row);
      imWaferMap->ShowHint = false;
      imWaferMap->Hint = msg;
      imWaferMap->ShowHint = true;

   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::InitWaferMap()
{

   Wafer.boolSet = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::imWaferMapMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   MapX = X;
   MapY = Y;
}
//---------------------------------------------------------------------------
int DieWorkCol = 0;
int DieWorkRow = 0;
void __fastcall TfrmRecipeSet::imWaferMapDblClick(TObject *Sender)
{
int x,y,sw,sh,cx,cy,sx,sy,tcx,tcy,w,h,px,py;
double ratio,fx,fy,sizew;
AnsiString msg;

   tsMicro->Enabled = false;
   tmUpdatePosition->Enabled = true;
   pnlStageMoving->Visible = true;
   x = pnlX->Caption.ToInt();
   y = pnlY->Caption.ToInt();
   sw = shCurretnPosition->Width;
   sh = shCurretnPosition->Height;

   // pixel
   sx = Wafer.X;
   sy = Wafer.Y;
   cx = Wafer.Cx;
   cy = Wafer.Cy;
   w = Wafer.Width;
   h = Wafer.Height;

   // Wafer�����I
   tcx = frnMain->edCenterX->Text.ToInt();
   tcy = frnMain->edCenterY->Text.ToInt();

   // 2023 5 3 - chc �ե�
   if(AdjustCenterX != 0 && AdjustCenterY != 0) {
      tcx = AdjustCenterX;
      tcy = AdjustCenterY;
   }

   // 12", um/Pixel
   if(rgWaferType->ItemIndex == 0) {
      sizew = 300000;
      ratio = sizew / (double)w;
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO)
         sizew = 200000;
      else
         sizew = 300000;
      ratio = sizew / (double)w;
   }

   // 2023 3 21 - chc �T�w�즹Chip�����I
   //x = tcx - ((MapX - cx) * ratio) / frnMain->X_RESOLUTION;
   //y = tcy + ((MapY - cy) * ratio) / frnMain->Y_RESOLUTION;
   int col,row,chipx,chipy;
   // �����q
   col = (MapX - COffsetX - Wafer.X) / Wafer.Dx;
   row = (MapY - COffsetY - Wafer.Y) / Wafer.Dy;
   if(MapX < (COffsetX + Wafer.X))
      col = -1;
   if(MapY < (COffsetY + Wafer.Y))
      row = -1;
   // ���I��m: GridNoY

   // 2023 5 3a - chc ��col,row�Ӳ���
   /*
   // �U: 180
   if(rgWaferAxisOrientation->ItemIndex == 1) {
      row = GridNoY - row - 1;
      // �̤pRow,Col
      col -= MinCol;
      row -= (MinRow + 1);
   }
   chipx = col * Wafer.Dx + COffsetX + Wafer.X + Wafer.Dx/2;
   chipy = row * Wafer.Dy + COffsetY + Wafer.Y + Wafer.Dy/2;
   x = tcx - ((chipx - cx) * ratio) / frnMain->X_RESOLUTION;
   y = tcy + ((chipy - cy) * ratio) / frnMain->Y_RESOLUTION;
   pnlWorkCol->Caption = IntToStr(col);
   pnlWorkRow->Caption = IntToStr(row);

   msg.sprintf("MoveToXY(x,y-px,py-cx,cy): %d,%d-%d,%d-%d,%d",x,y,MapX,MapY,cx,cy);
   frnMain->WriteSystemLog(msg);
   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }
   */
   edSelectCol->Text = IntToStr(col);
   edSelectRow->Text = IntToStr(row);
   btnColRowToClick(this);

   UpdateCurrentPosition();
   tsMicro->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnColRowToClick(TObject *Sender)
{
int col,row,w,h,dx,dy,cx,cy,ccol,crow,dcol,drow,x,y;
double fx,fy;
AnsiString msg;

   col = edSelectCol->Text.ToInt();
   row = edSelectRow->Text.ToInt();

   // 2023 5 4 - chc �ˬd�d��
   if(col > pnlChipWNo->Caption.ToInt() || col < 0) {
      return;
   }
   if(row > pnlChipHNo->Caption.ToInt() || row < 0) {
      return;
   }

   tsMicro->Enabled = false;
   tmUpdatePosition->Enabled = true;
   pnlStageMoving->Visible = true;

   // 2023 5 3a - chc �ץ�col/row
   // 2023 5 4 - chc �O0, ���O1
   if(rgWaferAxisOrientation->ItemIndex == 0) {
      row = GridNoY - row - MinRow - 2;
   }

   // um
   w = edChipWidth->Text.ToInt();
   h = edChipHeight->Text.ToInt();
   dx = edChipDx->Text.ToInt();
   dy = edChipDy->Text.ToInt();
   w += dx;
   h += dy;
   // pulse
   cx = frnMain->edCenterX->Text.ToInt();
   cy = frnMain->edCenterY->Text.ToInt();

   // 2023 5 3 - chc �ե�
   if(AdjustCenterX != 0 && AdjustCenterY != 0) {
      cx = AdjustCenterX;
      cy = AdjustCenterY;
   }

   ccol = pnlCenterCol->Caption.ToInt();
   crow = pnlCenterRow->Caption.ToInt();
   dcol = col - ccol;
   drow = row - crow;
   // um
   dcol *= w;
   drow *= h;
   // XY: um
   fx = (double)dcol / frnMain->X_RESOLUTION;
   fy = (double)drow / frnMain->Y_RESOLUTION;
   x = cx - fx;
   y = cy - fy;

   msg.sprintf("MoveToXY(x,y-c,r-cc,cr-dc,dr-fx,fy): %d,%d-%d,%d-%d,%d-%d,%d-%.2f,%.2f",x,y,col,row,ccol,crow,dcol,drow,fx,fy);
   frnMain->WriteSystemLog(msg);
   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }
   UpdateCurrentPosition();
   tsMicro->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;

   pnlWorkCol->Caption = IntToStr(col);
   pnlWorkRow->Caption = IntToStr(row);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbAFClick(TObject *Sender)
{

   // 2023 4 13 - chc status
   pnlAF->Color = clSilver;

   tsMicro->Enabled = false;
   if(cbAF->Checked == false)
      frnMain->WriteSystemLog("frmRecipeSet(cbAFClick): Set AF to false");
   else
      frnMain->WriteSystemLog("frmRecipeSet(cbAFClick): Set AF to true");
   frnMain->SwitchAF(cbAF->Checked);
   tsMicro->Enabled = true;
}
//---------------------------------------------------------------------------
// AF unit
double AFUnit = 0.1;
void __fastcall TfrmRecipeSet::pnlAFUnit01Click(TObject *Sender)
{

   pnlAFUnit01->Color = clGray;
   pnlAFUnit001->Color = clSilver;
   AFUnit = 0.1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::pnlAFUnit001Click(TObject *Sender)
{

   pnlAFUnit01->Color = clSilver;
   pnlAFUnit001->Color = clGray;
   AFUnit = 0.01;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnGetAFPosClick(TObject *Sender)
{

   frnMain->btnA2MAPosClick(this);
   //Sleep(500);
   //pnlAFPos->Caption = frnMain->pnlA2MPos->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAFUpClick(TObject *Sender)
{
double fv;

   fv = edAFMovUnit->Text.ToDouble() * AFUnit;
   frnMain->AFMoveUp(fv);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAFDownClick(TObject *Sender)
{
double fv;

   fv = edAFMovUnit->Text.ToDouble() * AFUnit;
   frnMain->AFMoveDown(fv);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::rgObservationClick(TObject *Sender)
{

   tsMicro->Enabled = false;
   frnMain->WriteSystemLog("frmInspection: DIC����");
   if(frnMain->DICSwitch(rgObservation->ItemIndex) == false)
      pnlObservation->Color = clRed;
   else {
      pnlObservation->Color = clLime;

      // 2023 2 18 - chc DIC Move
      if(rgObservation->ItemIndex == 2)
         gbDIC->Enabled = true;
      else
         gbDIC->Enabled = false;

   }
   tsMicro->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::tmUpdatePositionTimer(TObject *Sender)
{

   UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::combRecipeChange(TObject *Sender)
{
AnsiString recipename;

   recipename = combRecipe->Text;
   //ReadRecipe(recipename,MODE_EDIT);

   // 2023 4 1 - chc ��sRecipe��to frmInspection: Macro/Micro/WaferID
   frmRecipe->UpdateRecipe(recipename);
   // �O�_�w����Recipe?
   frnMain->boolRecipeSelect = true;
   // SECS: Select Recipe, oRCPM
   frnMain->edSECSRecipeID->Text = combRecipe->Text;
   frnMain->rgSECSRecipe->ItemIndex = RECIPE_CHANGE;
   frnMain->WriteSystemLog("SECS: Recipe Selected " + combRecipe->Text);
   frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);

   // 2023 4 7 - chc Draw Wafer Map
   btnWaferMapDrawClick(this);

}
//---------------------------------------------------------------------------
// mode : 1-Run, 2-Edit
bool __fastcall TfrmRecipeSet::ReadRecipe(AnsiString rfname,int mode)
{
TIniFile *pSystemFile;
AnsiString str,fname,recipename;
struct RECIPE_STRU *RecipeBuffer;

   // ini�ɦW�P�ؿ�
   fname = frnMain->RecipeDirectory + "\\" + rfname;
   if(!FileExists(fname)) {
      frnMain->sbSystem->Panels->Items[1]->Text = rfname + "Not Existed!";
      return false;
   }
   if(mode == MODE_RUN)
      RecipeBuffer = &RecipeBufferRun;
   else
      RecipeBuffer = &RecipeBufferEdit;

   // ���Jini��
   pSystemFile = new TIniFile(fname);
   RecipeBuffer->Name               = pSystemFile->ReadString(RECIPE_INFORMATION_SECTION,RECICPE_NAME                        ,"A1");
   // Size
   RecipeBuffer->WaferSize          = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Wafer Size"                       ,12);

   // Ū��Recipe: Base1/Base2
   /*
   for(int i=0 ; i<MAX_BASE_NO ; i++) {
      RecipeBuffer->BaseBuf[i].X    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Base"+IntToStr(i+1)+" X"       ,20000+i);
      RecipeBuffer->BaseBuf[i].Y    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Base"+IntToStr(i+1)+" Y"       ,20000+i);
      RecipeBuffer->BaseBuf[i].Z    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Base"+IntToStr(i+1)+" Z"       ,20000+i);
      RecipeBuffer->BaseBuf[i].T    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Base"+IntToStr(i+1)+" T"       ,20000+i);
   }

   RecipeBuffer->DotNo              = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Dot No"                        ,5);
   if(RecipeBuffer->DotNo > MAX_DOT_NO)
      RecipeBuffer->DotNo = MAX_DOT_NO;
   sgDot->RowCount = RecipeBuffer->DotNo + 2;

   for(int i=0 ; i<RecipeBuffer->DotNo ; i++) {
      RecipeBuffer->DotBuf[i].X    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Dot"+IntToStr(i+1)+" X"       ,20000+i);
      RecipeBuffer->DotBuf[i].Y    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Dot"+IntToStr(i+1)+" Y"       ,20000+i);
      RecipeBuffer->DotBuf[i].Z    = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"Dot"+IntToStr(i+1)+" Z"       ,20000+i);
      sgDot->Cells[DOT_FIELD_INDEX][i+1] = IntToStr(i+1);
      sgDot->Cells[DOT_FIELD_X][i+1] = IntToStr(RecipeBuffer->DotBuf[i].X);
      sgDot->Cells[DOT_FIELD_Y][i+1] = IntToStr(RecipeBuffer->DotBuf[i].Y);
      sgDot->Cells[DOT_FIELD_Z][i+1] = IntToStr(RecipeBuffer->DotBuf[i].Z);
      RecipeBuffer->DotBuf[i].Recipe    = pSystemFile->ReadString(RECIPE_INFORMATION_SECTION,"Dot"+IntToStr(i+1)+" Recipe"       ,"NA");
      sgDot->Cells[DOT_FIELD_RECIPE][i+1] = RecipeBuffer->DotBuf[i].Recipe;
   }

   // �M���̫�@Row
   for(int i=0 ; i<sgDot->ColCount ; i++)
      sgDot->Cells[i][sgDot->RowCount-1] = "";

   // �]�w��1 Area/Dot��
   // Dot
   edDotNo->Text = "1";
   edDotTNo->Text = IntToStr(RecipeBuffer->DotNo);
   pnlDotX->Caption = IntToStr(RecipeBuffer->DotBuf[0].X);
   pnlDotY->Caption = IntToStr(RecipeBuffer->DotBuf[0].Y);
   pnlDotZ->Caption = IntToStr(RecipeBuffer->DotBuf[0].Z);
   comboSubRecipe->Text = RecipeBuffer->DotBuf[0].Recipe;
   // Base
   pnlBase1X->Caption = IntToStr(RecipeBuffer->BaseBuf[0].X);
   pnlBase1Y->Caption = IntToStr(RecipeBuffer->BaseBuf[0].Y);
   pnlBase1Z->Caption = IntToStr(RecipeBuffer->BaseBuf[0].Z);
   pnlBase2X->Caption = IntToStr(RecipeBuffer->BaseBuf[1].X);
   pnlBase2Y->Caption = IntToStr(RecipeBuffer->BaseBuf[1].Y);
   pnlBase2Z->Caption = IntToStr(RecipeBuffer->BaseBuf[1].Z);

   // LED�G�׻PCCD: Shutter & Gain��
   RecipeBuffer->LEDValue[0] = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"LED Value1"     ,LED2CH1Value);
   RecipeBuffer->LEDValue[1] = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,"LED Value2"     ,LED2CH2Value);
   RecipeBuffer->Shutter = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,CCD_SHUTTER          ,CCDShutter);
   RecipeBuffer->Gain = pSystemFile->ReadInteger(RECIPE_INFORMATION_SECTION,CCD_GAIN                ,CCDGain);
   // �n�]�wLED/Gain/shutter @@

   // Match Parameter
   RecipeBuffer->MinScore = pSystemFile->ReadFloat("Match" ,"Score"                                 ,0.5);
   RecipeBuffer->MatchMode = pSystemFile->ReadInteger("Match" ,"Mode"                               ,0);
   RecipeBuffer->MinReduceArea = pSystemFile->ReadInteger("Match" ,"MinRedceArea"                   ,64);
   str.sprintf("%.2f",RecipeBuffer->MinScore);
   edMinScore->Text = str;
   rgMatchMode->ItemIndex = RecipeBuffer->MatchMode;
   edMinReduceArea->Text = IntToStr(RecipeBuffer->MinReduceArea);
   */

   delete pSystemFile;
   return true;
}
//---------------------------------------------------------------------------


void __fastcall TfrmRecipeSet::cbAutoExposureClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 0;
   frnMain->cbAutoExposure->Checked = cbAutoExposure->Checked;
   frnMain->cbAutoExposureClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------
// 2023 2 13 - chc Test MIMMatch
void __fastcall TfrmRecipeSet::laImgTestClick(TObject *Sender)
{

// 2023 2 14a - chc ������: �|��  => �L��
//
AnsiString imgfilename;

   imgfilename = frnMain->SystemDirectory + "\\ColorImg.bmp";
   NCCError = iReadImage(ColorImg,imgfilename.c_str());
   frnMain->WriteSystemLog("iReadImage: NCCError= " + IntToStr(NCCError));

/*
AnsiString fname;
int ccdno,w,h;

   fname = frnMain->SystemDirectory + "\\Test2.bmp";
   //imMeasure->Picture->LoadFromFile(fname);
   ccdno = 0;

   //frnMain->ImageAry.Load(fname.c_str());
   //w = frnMain->ImageAry.GetWidth();
   //h = frnMain->ImageAry.GetHeight();

   frnMain->usb_ImageAry[ccdno].Load(fname.c_str());
   w = frnMain->usb_ImageAry[ccdno].GetWidth();
   h = frnMain->usb_ImageAry[ccdno].GetHeight();
   frnMain->CCDInfoAry[ccdno].Width = w;
   frnMain->CCDInfoAry[ccdno].Width = h;
   frnMain->CCDInfoAry[ccdno].Type = CCD_COLOR;
   frnMain->usb_ImageROIAry[ccdno].Detach();
   frnMain->usb_ImageROIAry[ccdno].Attach(&frnMain->usb_ImageAry[ccdno]);

   imInspection->Picture->Bitmap->Width = w;
   imInspection->Picture->Bitmap->Height = h;

   //frnMain->ImageAry.Draw(imInspection->Canvas->Handle);
   frnMain->usb_ImageAry[ccdno].Draw(imInspection->Canvas->Handle);

   double f;
   f = (double)imInspection->Width / w * 100.0;
   fname.sprintf("%.2f\%",f);
   pnlMainFactor->Caption = fname;
   cbCCDFullImage->Checked = true;
   boolCCDImageLoadedAry[ccdno] = true;
*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnDICNearClick(TObject *Sender)
{
int fc = 10;

   // 2023 2 18 - chc Check in IN?
   pnlSystemMessage->Caption = "";
   if(rgObservation->ItemIndex != 2) {
      pnlDICNear->Color = clRed;
      pnlSystemMessage->Caption = "Not in DIC Mode!";
      return;
   }

   pnlDICNear->Color = clSilver;
   fc = tbDICFC->Position;
   frnMain->DICFCMove(1,fc);
   if(frnMain->pnlDICFCValue->Caption == "OUT" || frnMain->pnlDICFCValue->Caption == "")
      pnlDICNear->Color = clRed;
   else {
      pnlDICNear->Color = clLime;
      tbDICFC->Position = frnMain->pnlDICFCValue->Caption.ToInt();
   }
   gbDIC->Caption = "DIC:" + frnMain->pnlDICFCValue->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnDICFarClick(TObject *Sender)
{
int fc = 10;

   // 2023 2 18 - chc Check in IN?
   pnlSystemMessage->Caption = "";
   if(rgObservation->ItemIndex != 2) {
      pnlDICFar->Color = clRed;
      pnlSystemMessage->Caption = "Not in DIC Mode!";
      return;
   }

   pnlDICFar->Color = clSilver;
   fc = tbDICFC->Position;
   frnMain->DICFCMove(2,fc);
   if(frnMain->pnlDICFCValue->Caption == "OUT" || frnMain->pnlDICFCValue->Caption == "")
      pnlDICFar->Color = clRed;
   else {
      pnlDICFar->Color = clLime;
      tbDICFC->Position = frnMain->pnlDICFCValue->Caption.ToInt();
   }
   gbDIC->Caption = "DIC:" + frnMain->pnlDICFCValue->Caption;
}
//---------------------------------------------------------------------------
bool boolDICLeftDownMove = false;

void __fastcall TfrmRecipeSet::tbDICFCMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int fc;

   // 2023 2 18 - chc Check in IN?
   pnlSystemMessage->Caption = "";
   if(rgObservation->ItemIndex != 2) {
      pnlDICBar->Color = clRed;
      pnlSystemMessage->Caption = "Not in DIC Mode!";
      return;
   }

   boolDICLeftDownMove = false;
   pnlDICBar->Color = clSilver;
   fc = tbDICFC->Position;
   frnMain->DICFCMove(3,fc);
   if(frnMain->pnlDICFCValue->Caption == "OUT" || frnMain->pnlDICFCValue->Caption == "")
      pnlDICBar->Color = clRed;
   else {
      pnlDICBar->Color = clLime;
      //tbDICFC->Position = frnMain->pnlDICFCValue->Caption.ToInt();
   }
   //gbDIC->Caption = "DIC:" + frnMain->pnlDICFCValue->Caption;
   gbDIC->Caption = "DIC:" + IntToStr(fc);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::tbDICFCMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int fc;

   if(boolDICLeftDownMove == true) {
      fc = tbDICFC->Position;
      gbDIC->Caption = "DIC:" + IntToStr(fc);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::tbDICFCMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolDICLeftDownMove = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbMicroCrossLineClick(TObject *Sender)
{

   shVertical->Visible = cbMicroCrossLine->Checked;
   shHorizontal->Visible = cbMicroCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbMacroTopCrossLineClick(TObject *Sender)
{

   shLeftVertical->Visible = cbMacroTopCrossLine->Checked;
   shLeftHorizontal->Visible = cbMacroTopCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbMacroBackCrossLineClick(TObject *Sender)
{

   shRightVertical->Visible = cbMacroBackCrossLine->Checked;
   shRightHorizontal->Visible = cbMacroBackCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbTableDirectionClick(TObject *Sender)
{

   frnMain->cbTableDirection->Checked = cbTableDirection->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbJoystickMoveContinueClick(TObject *Sender)
{

   frnMain->cbJoystickMoveContinue->Checked = cbJoystickMoveContinue->Checked;
}
//---------------------------------------------------------------------------
//struct MICRO_RECIPE_STRU {
//   struct PATTERN_STRU Pattern;
//   struct MATCH_STRU Match;
//   struct BXFM_STRU BXFM;
//   struct DIC_STRU DIC;
//   struct CCD_STRU CCD;
//   struct CHIP_STRU Chip;
//   bool boolAutoSave;
//};
void __fastcall TfrmRecipeSet::btnSaveMicroParametersClick(TObject *Sender)
{
TIniFile *pSystemFile;
AnsiString recipefname,recipename;

   recipename = combRecipe->Text;
   recipefname = edRecipePath->Text + "\\" + recipename + ".ini";
   pSystemFile = new TIniFile(recipefname);

   // 2023 5 9 - chc �ˬdRecipe�O�_�s�b?
   if(combRecipe->Items->IndexOf(combRecipe->Text) == -1 && combRecipe->Text != "") {
      // �ƻs�@���s��Recipe
      if(Application->MessageBox(("Sure to Add new Recipe: [" + combRecipe->Text + "]?").c_str(), " Recipe Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         combRecipe->Text = "";
         combRecipe->ItemIndex = -1;
         return;
      }
      combRecipe->Items->Append(combRecipe->Text);
   }

   // 2023 3 26 - chc SECS: Modify(Save) Recipe
   frnMain->edSECSRecipeID->Text = combRecipe->Text;
   if(FileExists(recipefname)) {
      frnMain->rgSECSRecipe->ItemIndex = RECIPE_MODIFY;
      frnMain->WriteSystemLog("SECS: Micro Recipe Modified " + combRecipe->Text);
   }
   else {
      frnMain->rgSECSRecipe->ItemIndex = RECIPE_CREATE;
      frnMain->WriteSystemLog("SECS: Micro Recipe Saved " + combRecipe->Text);
   }
   frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);

   // CCD: Shutter, boolAutoWB
   RecipeBufferEdit.MicroRecipe.CCD.Shutter = rgCCDShutter->ItemIndex;
   RecipeBufferEdit.MicroRecipe.CCD.boolAutoWB = cbWBAuto->Checked;
   // Write
   pSystemFile->WriteInteger("Micro-CCD","Shutter"     ,RecipeBufferEdit.MicroRecipe.CCD.Shutter);
   pSystemFile->WriteBool("Micro-CCD","boolAutoWB"     ,RecipeBufferEdit.MicroRecipe.CCD.boolAutoWB);

   // DIC
   RecipeBufferEdit.MicroRecipe.DIC.FCValue = tbDICFC->Position;
   RecipeBufferEdit.MicroRecipe.DIC.boolActive = frnMain->boolDIC;
   // Write
   pSystemFile->WriteInteger("Micro-DIC","FCValue"     ,RecipeBufferEdit.MicroRecipe.DIC.FCValue);
   pSystemFile->WriteBool("Micro-DIC","boolActive"     ,RecipeBufferEdit.MicroRecipe.DIC.boolActive);

   // BXFM
   RecipeBufferEdit.MicroRecipe.BXFM.Lens = rgBXFMLens->ItemIndex;
   RecipeBufferEdit.MicroRecipe.BXFM.Intensity = tbBXFMLMP->Position;
   RecipeBufferEdit.MicroRecipe.BXFM.AS = pnlASValue->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.BXFM.Observation = rgObservation->ItemIndex;
   RecipeBufferEdit.MicroRecipe.BXFM.boolAF = cbAF->Checked;
   // Write
   pSystemFile->WriteInteger("Micro-BXFM","Lens"        ,RecipeBufferEdit.MicroRecipe.BXFM.Lens);
   pSystemFile->WriteInteger("Micro-BXFM","Intensity"   ,RecipeBufferEdit.MicroRecipe.BXFM.Intensity);
   pSystemFile->WriteInteger("Micro-BXFM","AS"          ,RecipeBufferEdit.MicroRecipe.BXFM.AS);
   pSystemFile->WriteInteger("Micro-BXFM","Observation" ,RecipeBufferEdit.MicroRecipe.BXFM.Observation);
   pSystemFile->WriteBool("Micro-BXFM","boolAF"         ,RecipeBufferEdit.MicroRecipe.BXFM.boolAF);

   // Match
   RecipeBufferEdit.MicroRecipe.Match.MinScore = edMinScore->Text.ToDouble();
   RecipeBufferEdit.MicroRecipe.Match.No = cbMatchNo->Text.ToInt();
   // Write
   pSystemFile->WriteFloat("Micro-Match","MinScore"     ,RecipeBufferEdit.MicroRecipe.Match.MinScore);
   pSystemFile->WriteInteger("Micro-Match","No"         ,RecipeBufferEdit.MicroRecipe.Match.No);

   // Pattern
   if(pnlBase1X->Caption == "")
      pnlBase1X->Caption = "0";
   if(pnlBase1Y->Caption == "")
      pnlBase1Y->Caption = "0";
   if(pnlBase2X->Caption == "")
      pnlBase2X->Caption = "0";
   if(pnlBase2Y->Caption == "")
      pnlBase2Y->Caption = "0";
   if(pnlPositionX->Caption == "")
      pnlPositionX->Caption = "0";
   if(pnlPositionY->Caption == "")
      pnlPositionY->Caption = "0";
   RecipeBufferEdit.MicroRecipe.Pattern.Base1.X = pnlBase1X->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Base1.Y = pnlBase1Y->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Base2.X = pnlBase2X->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Base2.Y = pnlBase2Y->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Position.X = pnlPositionX->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Position.Y = pnlPositionY->Caption.ToInt();
   // Write
   pSystemFile->WriteInteger("Micro-Pattern","Base1.X"     ,RecipeBufferEdit.MicroRecipe.Pattern.Base1.X);
   pSystemFile->WriteInteger("Micro-Pattern","Base1.Y"     ,RecipeBufferEdit.MicroRecipe.Pattern.Base1.Y);
   pSystemFile->WriteInteger("Micro-Pattern","Base2.X"     ,RecipeBufferEdit.MicroRecipe.Pattern.Base2.X);
   pSystemFile->WriteInteger("Micro-Pattern","Base2.Y"     ,RecipeBufferEdit.MicroRecipe.Pattern.Base2.Y);
   pSystemFile->WriteInteger("Micro-Pattern","Position.X"  ,RecipeBufferEdit.MicroRecipe.Pattern.Position.X);
   pSystemFile->WriteInteger("Micro-Pattern","Position.Y"  ,RecipeBufferEdit.MicroRecipe.Pattern.Position.Y);

   // 2023 5 9 - chc �|Z/T
   if(pnlStdZ->Caption == "")
      pnlStdZ->Caption = "0";
   if(pnlStdT->Caption == "")
      pnlStdT->Caption = "0";
   RecipeBufferEdit.MicroRecipe.Pattern.StdZ = pnlStdZ->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.StdT = pnlStdT->Caption.ToInt();
   pSystemFile->WriteInteger("Micro-Pattern","StdZ"     ,RecipeBufferEdit.MicroRecipe.Pattern.StdZ);
   pSystemFile->WriteInteger("Micro-Pattern","StdT"     ,RecipeBufferEdit.MicroRecipe.Pattern.StdT);

   // 2023 5 3 - chc �s�����I: pnlCenterX
   RecipeBufferEdit.MicroRecipe.Pattern.Center.X = pnlCenterX->Caption.ToInt();
   RecipeBufferEdit.MicroRecipe.Pattern.Center.Y = pnlCenterY->Caption.ToInt();
   pSystemFile->WriteInteger("Micro-Pattern","Center.X"  ,RecipeBufferEdit.MicroRecipe.Pattern.Center.X);
   pSystemFile->WriteInteger("Micro-Pattern","Center.Y"  ,RecipeBufferEdit.MicroRecipe.Pattern.Center.Y);

   // 2023 4 24 - chc �sMatch: Intensity & Lens
   pSystemFile->WriteInteger("Micro-Pattern","Intensity"   ,RecipeBufferEdit.MicroRecipe.Pattern.Intensity);
   pSystemFile->WriteInteger("Micro-Pattern","Lens"        ,RecipeBufferEdit.MicroRecipe.Pattern.Lens);

   // 2023 5 4 - chc Alignment Position
   pSystemFile->WriteInteger("Micro-Pattern","PositionIntensity"   ,RecipeBufferEdit.MicroRecipe.Pattern.PositionIntensity);

   // Chip
   int Width,Height,Dx,Dy,Ignore;
   RecipeBufferEdit.MicroRecipe.Chip.Width = edChipWidth->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.Height = edChipHeight->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.Dx = edChipDx->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.Dy = edChipDy->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.Ignore = edIgnore->Text.ToInt();
   // SubDieXNo, SubDieYNo
   RecipeBufferEdit.MicroRecipe.Chip.SubDieXNo = edSubDieXno->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.SubDieYNo = edSubDieYno->Text.ToInt();
   // Write
   pSystemFile->WriteInteger("Micro-Chip","Width"     ,RecipeBufferEdit.MicroRecipe.Chip.Width);
   pSystemFile->WriteInteger("Micro-Chip","Height"    ,RecipeBufferEdit.MicroRecipe.Chip.Height);
   pSystemFile->WriteInteger("Micro-Chip","Dx"        ,RecipeBufferEdit.MicroRecipe.Chip.Dx);
   pSystemFile->WriteInteger("Micro-Chip","Dy"        ,RecipeBufferEdit.MicroRecipe.Chip.Dy);
   pSystemFile->WriteInteger("Micro-Chip","Ignore"    ,RecipeBufferEdit.MicroRecipe.Chip.Ignore);
   // SubDieXNo, SubDieYNo
   pSystemFile->WriteInteger("Micro-Chip","SubDieXNo"    ,RecipeBufferEdit.MicroRecipe.Chip.SubDieXNo);
   pSystemFile->WriteInteger("Micro-Chip","SubDieYNo"    ,RecipeBufferEdit.MicroRecipe.Chip.SubDieYNo);

   // 2023 4 27 - chc SubDieWidth/Height
   RecipeBufferEdit.MicroRecipe.Chip.SubDieWidth = edSubDieWidth->Text.ToInt();
   RecipeBufferEdit.MicroRecipe.Chip.SubDieHeight = edSubDieHeight->Text.ToInt();

   // 2023 4 5 - chc SubDieWidth/Height
   pSystemFile->WriteInteger("Micro-Chip","SubDieWidth"    ,RecipeBufferEdit.MicroRecipe.Chip.SubDieWidth);
   pSystemFile->WriteInteger("Micro-Chip","SubDieHeight"   ,RecipeBufferEdit.MicroRecipe.Chip.SubDieHeight);

   // Auto Save
   RecipeBufferEdit.MicroRecipe.boolAutoSave = cbAutoSaveMicro->Checked;
   // Write
   pSystemFile->WriteBool("Micro","boolAutoSave"         ,RecipeBufferEdit.MicroRecipe.boolAutoSave);

   // 2023 5 5 - chc all Die: boolDieInsideAll
   RecipeBufferEdit.MicroRecipe.boolDieInsideAll = cbDieInsideAll->Checked;
   pSystemFile->WriteBool("Micro","boolDieInsideAll"     ,RecipeBufferEdit.MicroRecipe.boolDieInsideAll);

   // 2023 4 11 - chc ImageSaveMode
   RecipeBufferEdit.MicroRecipe.ImageSaveMode = rgImageSaveMode->ItemIndex;
   // Write
   pSystemFile->WriteInteger("Micro","ImageSaveMode"         ,RecipeBufferEdit.MicroRecipe.ImageSaveMode);

   // 2023 4 12 - chc MinYield
   RecipeBufferEdit.MicroRecipe.MinYield = edMinYield->Text.ToDouble();
   // Write
   pSystemFile->WriteFloat("Micro","MinYield"         ,RecipeBufferEdit.MicroRecipe.MinYield);

   // WaferSize
   RecipeBufferEdit.WaferSize = rgWaferType->ItemIndex;
   // Write
   pSystemFile->WriteInteger("Recipe","WaferSize"         ,RecipeBufferEdit.WaferSize);

   // Date/Time: TestRecipeForBE 2022/12/05 11:20
   DateTimeToString(RecipeBufferEdit.Date,"yyyy\/mm\/dd",Now());
   DateTimeToString(RecipeBufferEdit.Time,"hh\:mm",Now());
   RecipeBufferEdit.Name = recipename;
   pSystemFile->WriteString("Recipe","Name"     ,RecipeBufferEdit.Name);
   pSystemFile->WriteString("Recipe","Date"     ,RecipeBufferEdit.Date);
   pSystemFile->WriteString("Recipe","Time"     ,RecipeBufferEdit.Time);

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
   RecipeBufferEdit.BinCode.TxtNo = 14;
   pSystemFile->WriteInteger("BINCode","TxtNo"                                  ,RecipeBufferEdit.BinCode.TxtNo);
   for(int i=0 ; i<RecipeBufferEdit.BinCode.TxtNo ; i++) {
      if(i == 0) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbGlassIQCF->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbGlassIQCF->Checked;
      }
      else if(i == 1) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbGlassIQCB->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbGlassIQCB->Checked;
      }
      else if(i == 2) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbGlassOQCF->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbGlassOQCF->Checked;
      }
      else if(i == 3) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbGlassOQCB->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbGlassOQCB->Checked;
      }
      else if(i == 4) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiIQCSS->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiIQCSS->Checked;
      }
      else if(i == 5) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiIQCLS->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiIQCLS->Checked;
      }
      else if(i == 6) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiIQCPad->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiIQCPad->Checked;
      }
      else if(i == 7) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiIQCSA->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiIQCSA->Checked;
      }
      else if(i == 8) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiOQCSS->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiOQCSS->Checked;
      }
      else if(i == 9) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiOQCLS->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiOQCLS->Checked;
      }
      else if(i == 10) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiOQCPad->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiOQCPad->Checked;
      }
      else if(i == 11) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbSiOQCW->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbSiOQCW->Checked;
      }
      else if(i == 12) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbGood->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbGood->Checked;
      }
      else if(i == 13) {
         RecipeBufferEdit.BinCode.TxtName[i] = cbNA->Caption;
         RecipeBufferEdit.BinCode.bSelect[i] = cbNA->Checked;
      }
      pSystemFile->WriteString("BINCode","TxtName"+IntToStr(i)                  ,RecipeBufferEdit.BinCode.TxtName[i]);
      pSystemFile->WriteBool("BINCode","TxtSelect"+IntToStr(i)                  ,RecipeBufferEdit.BinCode.bSelect[i]);

      // 2023 4 19 - chc ��b�~��
      //RecipeBufferEdit.BinCode.ItemNo = sgBINCode->RowCount - 2;
      //pSystemFile->WriteInteger("BINCode","ItemNo"                              ,RecipeBufferEdit.BinCode.ItemNo);
      //for(int i=1 ; i<=RecipeBufferEdit.BinCode.ItemNo ; i++) {
      //   RecipeBufferEdit.BinCode.ID[i-1] = sgBINCode->Cells[1][i].ToInt();
      //   RecipeBufferEdit.BinCode.Color[i-1] = GetBinCodeColor(i);
      //   RecipeBufferEdit.BinCode.IDName[i-1] = sgBINCode->Cells[3][i];
      //   pSystemFile->WriteInteger("BINCode","ItemID"+IntToStr(i)               ,RecipeBufferEdit.BinCode.ID[i-1]);
      //   pSystemFile->WriteInteger("BINCode","ItemColor"+IntToStr(i)            ,RecipeBufferEdit.BinCode.Color[i-1]);
      //   pSystemFile->WriteString("BINCode","ItemName"+IntToStr(i)              ,RecipeBufferEdit.BinCode.IDName[i-1]);
      //}

   }

   // 2023 4 19 - chc ��b�~��
   RecipeBufferEdit.BinCode.ItemNo = sgBINCode->RowCount - 2;
   pSystemFile->WriteInteger("BINCode","ItemNo"                              ,RecipeBufferEdit.BinCode.ItemNo);
   for(int i=1 ; i<=RecipeBufferEdit.BinCode.ItemNo ; i++) {
      RecipeBufferEdit.BinCode.ID[i-1] = sgBINCode->Cells[1][i].ToInt();
      RecipeBufferEdit.BinCode.Color[i-1] = GetBinCodeColor(i);
      RecipeBufferEdit.BinCode.IDName[i-1] = sgBINCode->Cells[3][i];
      pSystemFile->WriteInteger("BINCode","ItemID"+IntToStr(i)               ,RecipeBufferEdit.BinCode.ID[i-1]);
      pSystemFile->WriteInteger("BINCode","ItemColor"+IntToStr(i)            ,RecipeBufferEdit.BinCode.Color[i-1]);
      pSystemFile->WriteString("BINCode","ItemName"+IntToStr(i)              ,RecipeBufferEdit.BinCode.IDName[i-1]);
   }

   // 2023 4 1 - chc Save DefectList: sgDefectList
   //                     sgDefectList->Cells[0][0] = "No";
   //                     sgDefectList->Cells[1][0] = "ID";
   //                     sgDefectList->Cells[2][0] = "X";
   //                     sgDefectList->Cells[3][0] = "Y";
   //                     sgDefectList->Cells[4][0] = "Col";
   //                     sgDefectList->Cells[5][0] = "Row";
   //                     sgDefectList->Cells[6][0] = "Judge";
   //                     sgDefectList->Cells[7][0] = "XSize";
   //                     sgDefectList->Cells[8][0] = "YSize";
   //                     sgDefectList->Cells[9][0] = "Area";
   RecipeBufferEdit.DefectList.No = sgDefectList->RowCount - 2;
   pSystemFile->WriteInteger("DefectList","No"                                  ,RecipeBufferEdit.DefectList.No);
   for(int i=1 ; i<=RecipeBufferEdit.DefectList.No ; i++) {
      frnMain->WriteSystemLog("Save DefectList: " + IntToStr(i));

      // 2023 5 6 - chc �O�_��SubDie?
      if(sgDefectList->Cells[0][i].Pos("*") > 0)
         RecipeBufferEdit.DefectList.Data[i-1].boolSubDie = true;
      else
         RecipeBufferEdit.DefectList.Data[i-1].boolSubDie = false;

      for(int j=1 ; j<=sgDefectList->ColCount-1 ; j++) {
         if(sgDefectList->Cells[j][i].Trim() == "")
            sgDefectList->Cells[j][i] = "0";
      }
      frnMain->WriteSystemLog("Save DefectList: Save...");
      RecipeBufferEdit.DefectList.Data[i-1].ID     = sgDefectList->Cells[1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-1...");

      // 2023 4 25 - chc �[�Jname
      RecipeBufferEdit.DefectList.Data[i-1].IDName = sgDefectList->Cells[2][i];

      RecipeBufferEdit.DefectList.Data[i-1].X      = sgDefectList->Cells[2+1][i].ToInt();
      RecipeBufferEdit.DefectList.Data[i-1].Y      = sgDefectList->Cells[3+1][i].ToInt();
      RecipeBufferEdit.DefectList.Data[i-1].Col    = sgDefectList->Cells[4+1][i].ToInt();
      RecipeBufferEdit.DefectList.Data[i-1].Row    = sgDefectList->Cells[5+1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-2...");
      RecipeBufferEdit.DefectList.Data[i-1].Judge  = sgDefectList->Cells[6+1][i].ToInt();
      RecipeBufferEdit.DefectList.Data[i-1].XSize  = sgDefectList->Cells[7+1][i].ToDouble();
      RecipeBufferEdit.DefectList.Data[i-1].YSize  = sgDefectList->Cells[8+1][i].ToDouble();
      RecipeBufferEdit.DefectList.Data[i-1].Area   = sgDefectList->Cells[9+1][i].ToDouble();
      //frnMain->WriteSystemLog("Save DefectList: Save-3...");

      // 2023 4 7 - chc �[�JLens/Intensity
      if(sgDefectList->Cells[10+1][i].Trim() == "")
         sgDefectList->Cells[10+1][i] = "0";
      RecipeBufferEdit.DefectList.Data[i-1].Lens   = sgDefectList->Cells[10+1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-4...");
      if(sgDefectList->Cells[11+1][i].Trim() == "")
         sgDefectList->Cells[11+1][i] = "0";
      RecipeBufferEdit.DefectList.Data[i-1].Intensity   = sgDefectList->Cells[11+1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-5...");

      // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
      if(sgDefectList->Cells[12+1][i].Trim() == "")
         sgDefectList->Cells[12+1][i] = "0";
      RecipeBufferEdit.DefectList.Data[i-1].Observation   = sgDefectList->Cells[12+1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-6...");
      if(sgDefectList->Cells[13+1][i].Trim() == "")
         sgDefectList->Cells[13+1][i] = "0";
      RecipeBufferEdit.DefectList.Data[i-1].DICValue   = sgDefectList->Cells[13+1][i].ToInt();
      //frnMain->WriteSystemLog("Save DefectList: Save-7...");

      pSystemFile->WriteInteger("DefectList","ID"+IntToStr(i)                   ,RecipeBufferEdit.DefectList.Data[i-1].ID);

      // 2023 4 25 - chc �[�Jname
      pSystemFile->WriteString("DefectList","IDName"+IntToStr(i)                ,RecipeBufferEdit.DefectList.Data[i-1].IDName);

      // 2023 5 9 - chc �[�JboolSubDie
      pSystemFile->WriteBool("DefectList","boolSubDie"+IntToStr(i)              ,RecipeBufferEdit.DefectList.Data[i-1].boolSubDie);

      pSystemFile->WriteInteger("DefectList","X"+IntToStr(i)                    ,RecipeBufferEdit.DefectList.Data[i-1].X);
      pSystemFile->WriteInteger("DefectList","Y"+IntToStr(i)                    ,RecipeBufferEdit.DefectList.Data[i-1].Y);
      pSystemFile->WriteInteger("DefectList","Col"+IntToStr(i)                  ,RecipeBufferEdit.DefectList.Data[i-1].Col);
      pSystemFile->WriteInteger("DefectList","Row"+IntToStr(i)                  ,RecipeBufferEdit.DefectList.Data[i-1].Row);
      pSystemFile->WriteInteger("DefectList","Judge"+IntToStr(i)                ,RecipeBufferEdit.DefectList.Data[i-1].Judge);
      pSystemFile->WriteFloat("DefectList","XSize"+IntToStr(i)                  ,RecipeBufferEdit.DefectList.Data[i-1].XSize);
      pSystemFile->WriteFloat("DefectList","YSize"+IntToStr(i)                  ,RecipeBufferEdit.DefectList.Data[i-1].YSize);
      pSystemFile->WriteFloat("DefectList","Area"+IntToStr(i)                   ,RecipeBufferEdit.DefectList.Data[i-1].Area);

      // 2023 4 7 - chc �[�JLens/Intensity
      pSystemFile->WriteInteger("DefectList","Lens"+IntToStr(i)                 ,RecipeBufferEdit.DefectList.Data[i-1].Lens);
      pSystemFile->WriteInteger("DefectList","Intensity"+IntToStr(i)            ,RecipeBufferEdit.DefectList.Data[i-1].Intensity);

      // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
      pSystemFile->WriteInteger("DefectList","Observation"+IntToStr(i)          ,RecipeBufferEdit.DefectList.Data[i-1].Observation);
      pSystemFile->WriteInteger("DefectList","DICValue"+IntToStr(i)             ,RecipeBufferEdit.DefectList.Data[i-1].DICValue);
      frnMain->WriteSystemLog("Save DefectList: Save.");

   }

   pSystemFile->UpdateFile();

   delete pSystemFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTOrgClick(TObject *Sender)
{

   tsMicro->Enabled = false;
   frnMain->ToLoadT();
   tsMicro->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSaveWafeIDClick(TObject *Sender)
{
TIniFile *pSystemFile;
AnsiString recipefname,recipename;

   recipename = combRecipe->Text;
   recipefname = edRecipePath->Text + "\\" + recipename + ".ini";
   pSystemFile = new TIniFile(recipefname);

   // WaferID
   RecipeBufferEdit.WaferID.Degree1 = rgWaferIDDegree1->ItemIndex;
   RecipeBufferEdit.WaferID.boolActive1 = cbReadWaferID1->Checked;
   RecipeBufferEdit.WaferID.Degree2 = rgWaferIDDegree2->ItemIndex;
   RecipeBufferEdit.WaferID.boolActive2 = cbReadWaferID2->Checked;
   // Write
   pSystemFile->WriteInteger("WaferID","Degree1"         ,RecipeBufferEdit.WaferID.Degree1);
   pSystemFile->WriteBool("WaferID","boolActive1"        ,RecipeBufferEdit.WaferID.boolActive1);
   pSystemFile->WriteInteger("WaferID","Degree2"         ,RecipeBufferEdit.WaferID.Degree2);
   pSystemFile->WriteBool("WaferID","boolActive2"        ,RecipeBufferEdit.WaferID.boolActive2);

   pSystemFile->UpdateFile();

   delete pSystemFile;
}
//---------------------------------------------------------------------------
//  1 "OXIDATION"
//  2 "RESIDUE"
//  3 "BUBBLE"
//  4 "CORROSION"
//  5 "UNEVEN CONDUCTOR"
//  6 "LINT"
//  7 "DUST"
//  8 "SURFACE GAP"
//  9 "PARTIAL ETCH"
//  10 "SURFACE HOLE"
//  11 "GOUGE"
//  12 "SCRATCH"
//  13 "WATER"
//  14 "BURN"
//  15 "UNKNOWN"
//  16 "GOOD";
// Setup BIN Code
// BINCode.txt
void __fastcall TfrmRecipeSet::SetupBINCode()
{
FILE *pFile;
AnsiString fname,msg,sid,sname;

   fname = frnMain->SystemDirectory + "\\BINCode.txt";
   pFile = fopen(fname.c_str(),"w+t");

   // BINCode Define
   sgBINCode->RowCount = 2;
   sgBINCode->ColWidths[0] = 40;
   sgBINCode->ColWidths[1] = 35;
   sgBINCode->ColWidths[2] = 30;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";

   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";

   for(int i=0; i<IDCount ; i++) {
      sgBINCode->Cells[0][sgBINCode->RowCount-1] = IntToStr(i+1);
      sgBINCode->Cells[1][sgBINCode->RowCount-1] = IDNo[i];
      sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
      sgBINCode->Cells[3][sgBINCode->RowCount-1] = IDName[i];
      sgBINCode->RowCount++;
      fprintf(pFile,"%d %s\n",IDNo[i],IDName[i].c_str());
   }
   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";

   fclose(pFile);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgBINCodeDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{

   // �o��J�I�M�襤�椸��A���ݭn�B�z�A�u�� (State.Contains(gdFixed)) �~�}�l�i��B�z
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   // 2023 3 30 - chc RowColor: �bDrawText�e����, �_�hText�|�Q�\��
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // �̦�
      else
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00D1D1D1;
   }
   // 0x00B3B3B3
   if(ARow == 0 || ACol == 0) {
      ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00B3B3B3;             // �̦�
   }
   ((TStringGrid*)Sender)->Canvas->FillRect(Rect);

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow == 0) {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else if(ACol == 0 || ACol == 1 || ACol == 2) {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }

   if(ARow != 0 && ARow < sgBINCode->RowCount-1) {
      // �t�q�[�W�C��
      if(ACol == 2) {
         if((ARow % 16) == 1)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clBlack;
         else if((ARow % 16) == 2)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clMaroon;
         else if((ARow % 16) == 3)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clGreen;
         else if((ARow % 16) == 4)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clOlive;
         else if((ARow % 16) == 5)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clNavy;
         else if((ARow % 16) == 6)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clPurple;
         else if((ARow % 16) == 7)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clTeal;
         else if((ARow % 16) == 8)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clGray;
         else if((ARow % 16) == 9)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clSilver;
         else if((ARow % 16) == 10)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clRed;
         else if((ARow % 16) == 11)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clLime;
         else if((ARow % 16) == 12)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clYellow;
         else if((ARow % 16) == 13)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clBlue;
         else if((ARow % 16) == 14)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clFuchsia;
         else if((ARow % 16) == 15)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clAqua;
         else if((ARow % 16) == 0)
            ((TStringGrid*)Sender)->Canvas->Brush->Color = clWhite;
         Rect.Left += 3;
         Rect.Top += 5;
         Rect.Right -= 3;
         Rect.Bottom -= 5;
         ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgDieDblClick(TObject *Sender)
{
int col,row,mcol,mrow;
int x,y,sw,sh,cx,cy,sx,sy,tcx,tcy,w,h,px,py,diexno,dieyno;
double ratio,fx,fy,sizew,fdiew,fdieh,fchipx,fchipy;
AnsiString msg;

   // Master Die
   DieWorkCol = pnlWorkCol->Caption.ToInt();
   DieWorkRow = pnlWorkRow->Caption.ToInt();
   mrow = DieWorkRow;
   mcol = DieWorkCol;
   // Sub Die
   row = sgDie->Row;
   col = sgDie->Col;
   diexno = edSubDieXno->Text.ToInt();
   dieyno = edSubDieYno->Text.ToInt();
   fdiew = (double)Wafer.Dx / diexno;
   fdieh = (double)Wafer.Dy / dieyno;
   cx = Wafer.Cx;
   cy = Wafer.Cy;

   // Wafer�����I
   w = Wafer.Width;
   h = Wafer.Height;
   tcx = frnMain->edCenterX->Text.ToInt();
   tcy = frnMain->edCenterY->Text.ToInt();
   // 12", um/Pixel
   if(rgWaferType->ItemIndex == 0) {
      sizew = 300000;
      ratio = sizew / (double)w;
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO)
         sizew = 200000;
      else
         sizew = 300000;
      ratio = sizew / (double)w;
   }

   // Master die Position
   fchipx = mcol * Wafer.Dx + COffsetX + Wafer.X;
   fchipy = mrow * Wafer.Dy + COffsetY + Wafer.Y;
   // Sub die Position
   fchipx += (col * (fdiew*1.5));
   fchipy += (row * (fdieh*1.5));
   // Pixel to pulse
   x = tcx - ((fchipx - cx) * ratio) / frnMain->X_RESOLUTION;
   y = tcy + ((fchipy - cy) * ratio) / frnMain->Y_RESOLUTION;

   frnMain->WriteSystemLog(msg);
   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }
   UpdateCurrentPosition();
   tsMicro->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnAS0Click(TObject *Sender)
{
int no;
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;

   no = 100;
   frnMain->tbBXFMAS->Position = no;
   frnMain->tbBXFMASMouseUp(this,Button,Shift,X,Y);
   pnlASValue->Caption = IntToStr(no);
}
//---------------------------------------------------------------------------
// 2023 3 13 - chc System Parameters
//struct SYSTEM_STRU {
//   AnsiString ResultPath,TxtPath,RecipePath,DeviceID;
//   AnsiStrnig InspectionStationID[3];
//   int FilterFW1,FilterFW2;
//   bool TableDirection;
//   bool JoystickContinueMove;
//   struct XYZ_STRU W12Edge[3];
//   struct XYZ_STRU W8Edge[3];
//};
//   struct SYSTEM_STRU SystemConfig;
void __fastcall TfrmRecipeSet::btnSaveSystemParametersClick(TObject *Sender)
{
TIniFile *pSystemFile;

   SystemConfigName = frnMain->SystemDirectory + "\\SystemConfig.ini";
   pSystemFile = new TIniFile(SystemConfigName);

   // Path
   SystemConfig.ResultPath = edResultPath->Text;
   SystemConfig.TxtPath = edWaferTxtPath->Text;
   SystemConfig.RecipePath = edRecipePath->Text;
   pSystemFile->WriteString("Path","Result"                                     ,SystemConfig.ResultPath);
   pSystemFile->WriteString("Path","Txt"                                        ,SystemConfig.TxtPath);
   pSystemFile->WriteString("Path","Recipe"                                     ,SystemConfig.RecipePath);

   // DeviceID
   SystemConfig.DeviceID = edDeviceID->Text;
   pSystemFile->WriteString("ID","Device"                                       ,SystemConfig.DeviceID);
   SystemConfig.InspectionStationID[0] = edInspectionStationID1->Text;
   SystemConfig.InspectionStationID[1] = edInspectionStationID2->Text;
   SystemConfig.InspectionStationID[2] = edInspectionStationID3->Text;
   pSystemFile->WriteString("ID","InspectionStation1"                           ,SystemConfig.InspectionStationID[0]);
   pSystemFile->WriteString("ID","InspectionStation2"                           ,SystemConfig.InspectionStationID[1]);
   pSystemFile->WriteString("ID","InspectionStation3"                           ,SystemConfig.InspectionStationID[2]);

   // 12" Edge
   // 1
   SystemConfig.W12Edge[0].X = ed12EdgeX1->Text.ToInt();
   SystemConfig.W12Edge[0].Y = ed12EdgeY1->Text.ToInt();
   SystemConfig.W12Edge[0].Z = ed12EdgeZ1->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge1","X"                                     ,SystemConfig.W12Edge[0].X);
   pSystemFile->WriteInteger("W12Edge1","Y"                                     ,SystemConfig.W12Edge[0].Y);
   pSystemFile->WriteInteger("W12Edge1","Z"                                     ,SystemConfig.W12Edge[0].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[0].Intensity = ed12EdgeIntensity1->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge1","Intensity"                             ,SystemConfig.W12Edge[0].Intensity);

   // 2
   SystemConfig.W12Edge[1].X = ed12EdgeX2->Text.ToInt();
   SystemConfig.W12Edge[1].Y = ed12EdgeY2->Text.ToInt();
   SystemConfig.W12Edge[1].Z = ed12EdgeZ2->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge2","X"                                     ,SystemConfig.W12Edge[1].X);
   pSystemFile->WriteInteger("W12Edge2","Y"                                     ,SystemConfig.W12Edge[1].Y);
   pSystemFile->WriteInteger("W12Edge2","Z"                                     ,SystemConfig.W12Edge[1].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[1].Intensity = ed12EdgeIntensity2->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge2","Intensity"                             ,SystemConfig.W12Edge[1].Intensity);

   // 3
   SystemConfig.W12Edge[2].X = ed12EdgeX3->Text.ToInt();
   SystemConfig.W12Edge[2].Y = ed12EdgeY3->Text.ToInt();
   SystemConfig.W12Edge[2].Z = ed12EdgeZ3->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge3","X"                                     ,SystemConfig.W12Edge[2].X);
   pSystemFile->WriteInteger("W12Edge3","Y"                                     ,SystemConfig.W12Edge[2].Y);
   pSystemFile->WriteInteger("W12Edge3","Z"                                     ,SystemConfig.W12Edge[2].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[2].Intensity = ed12EdgeIntensity3->Text.ToInt();
   pSystemFile->WriteInteger("W12Edge3","Intensity"                             ,SystemConfig.W12Edge[2].Intensity);

   // 2023 3 20 -  chc �L8" Port, 8"���P12"�@��
   if(frnMain->LoadPortMode == LOADPORT_SINGLE || frnMain->RobotMode != ROBOT_MODE_TAZMO) {
      ed8EdgeX1->Text = ed12EdgeX1->Text;
      ed8EdgeY1->Text = ed12EdgeY1->Text;
      ed8EdgeZ1->Text = ed12EdgeZ1->Text;
      ed8EdgeX2->Text = ed12EdgeX2->Text;
      ed8EdgeY2->Text = ed12EdgeY2->Text;
      ed8EdgeZ2->Text = ed12EdgeZ2->Text;
      ed8EdgeX3->Text = ed12EdgeX3->Text;
      ed8EdgeY3->Text = ed12EdgeY3->Text;
      ed8EdgeZ3->Text = ed12EdgeZ3->Text;
   }

   // 8" Edge
   // 1
   SystemConfig.W8Edge[0].X = ed8EdgeX1->Text.ToInt();
   SystemConfig.W8Edge[0].Y = ed8EdgeY1->Text.ToInt();
   SystemConfig.W8Edge[0].Z = ed8EdgeZ1->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge1","X"                                      ,SystemConfig.W8Edge[0].X);
   pSystemFile->WriteInteger("W8Edge1","Y"                                      ,SystemConfig.W8Edge[0].Y);
   pSystemFile->WriteInteger("W8Edge1","Z"                                      ,SystemConfig.W8Edge[0].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[0].Intensity = ed8EdgeIntensity1->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge1","Intensity"                             ,SystemConfig.W8Edge[0].Intensity);

   // 2
   SystemConfig.W8Edge[1].X = ed12EdgeX2->Text.ToInt();
   SystemConfig.W8Edge[1].Y = ed12EdgeY2->Text.ToInt();
   SystemConfig.W8Edge[1].Z = ed12EdgeZ2->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge2","X"                                      ,SystemConfig.W8Edge[1].X);
   pSystemFile->WriteInteger("W8Edge2","Y"                                      ,SystemConfig.W8Edge[1].Y);
   pSystemFile->WriteInteger("W8Edge2","Z"                                      ,SystemConfig.W8Edge[1].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[1].Intensity = ed8EdgeIntensity2->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge2","Intensity"                             ,SystemConfig.W8Edge[1].Intensity);

   // 3
   SystemConfig.W8Edge[2].X = ed8EdgeX3->Text.ToInt();
   SystemConfig.W8Edge[2].Y = ed8EdgeY3->Text.ToInt();
   SystemConfig.W8Edge[2].Z = ed8EdgeZ3->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge3","X"                                      ,SystemConfig.W8Edge[2].X);
   pSystemFile->WriteInteger("W8Edge3","Y"                                      ,SystemConfig.W8Edge[2].Y);
   pSystemFile->WriteInteger("W8Edge3","Z"                                      ,SystemConfig.W8Edge[2].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[2].Intensity = ed8EdgeIntensity3->Text.ToInt();
   pSystemFile->WriteInteger("W8Edge3","Intensity"                              ,SystemConfig.W8Edge[2].Intensity);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12EdgeLens = rg12EdgeLens->ItemIndex;
   pSystemFile->WriteInteger("W12Edge","Lens"                                   ,SystemConfig.W12EdgeLens);
   SystemConfig.W8EdgeLens = rg8EdgeLens->ItemIndex;
   pSystemFile->WriteInteger("W8Edge","Lens"                                    ,SystemConfig.W8EdgeLens);

   // 2023 4 19 - chc Cognex
   SystemConfig.CognexPath = edCognexPath->Text;
   SystemConfig.CognexTitle = edCognexTitle->Text;
   pSystemFile->WriteString("Path","Cognex"                                     ,SystemConfig.CognexPath);
   pSystemFile->WriteString("Title","Cognex"                                    ,SystemConfig.CognexTitle);

   // 2023 4 19 - chc edAlignmentDefault
   SystemConfig.AlignmentDefault = edAlignmentDefault->Text.ToInt();
   pSystemFile->WriteInteger("Alignment","Default"                              ,SystemConfig.AlignmentDefault);

   // 2023 5 5 - chc Lamp Life
   SystemConfig.LampLife = edLampLife->Text.ToInt();
   pSystemFile->WriteInteger("Alignment","Lamp Life"                            ,SystemConfig.LampLife);

   // 2023 5 6 - chc DieSaveOverlap
   SystemConfig.DieSaveOverlap = edDieSaveOverlap->Text.ToInt();
   pSystemFile->WriteInteger("Alignment","DieSaveOverlap"                       ,SystemConfig.DieSaveOverlap);

   pSystemFile->UpdateFile();

   delete pSystemFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn12Edge1Click(TObject *Sender)
{

   frnMain->Set12EdgePosition(1);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed12EdgeIntensity1->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn12Edge2Click(TObject *Sender)
{

   frnMain->Set12EdgePosition(2);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed12EdgeIntensity2->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn12Edge3Click(TObject *Sender)
{

   frnMain->Set12EdgePosition(3);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed12EdgeIntensity3->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn8Edge1Click(TObject *Sender)
{

   frnMain->Set8EdgePosition(1);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed8EdgeIntensity1->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn8Edge2Click(TObject *Sender)
{

   frnMain->Set8EdgePosition(2);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed8EdgeIntensity2->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btn8Edge3Click(TObject *Sender)
{

   frnMain->Set8EdgePosition(3);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   ed8EdgeIntensity3->Text = IntToStr(tbBXFMLMP->Position);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo12Edge1Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To12EdgePosition(1);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W12EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W12EdgeLens;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo12Edge2Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To12EdgePosition(2);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W12EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W12EdgeLens;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo12Edge3Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To12EdgePosition(3);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W12EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W12EdgeLens;
   }
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo8Edge1Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To8EdgePosition(1);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W8EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W8EdgeLens;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo8Edge2Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To8EdgePosition(2);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W8EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W8EdgeLens;
   }
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnTo8Edge3Click(TObject *Sender)
{

   pcRecipeSet->Enabled = false;
   frnMain->To8EdgePosition(3);
   pcRecipeSet->Enabled = true;

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   if(SystemConfig.W8EdgeLens != rgBXFMLens->ItemIndex) {
      rgBXFMLens->ItemIndex = SystemConfig.W8EdgeLens;
   }
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnToEdgeClick(TObject *Sender)
{
int no;

   tmUpdatePosition->Enabled = true;
   btnToEdge->Enabled = false;
   no = rgEdgeNo->ItemIndex + 1;
   if(rgWaferType->ItemIndex == LOADPORT_1) {
      frnMain->To12EdgePosition(no);

      // 2023 4 24 - chc �[�JEdge Intensit/Lens
      // 2023 5 4 - chc Inspection��no=1�~�n��
      if(frnMain->boolInInspecting == true && no == 1) {

         if(tbBXFMLMP->Position != SystemConfig.W12Edge[no-1].Intensity) {
            tbBXFMLMP->Position = SystemConfig.W12Edge[no-1].Intensity;
            SetIntensity(SystemConfig.W12Edge[no-1].Intensity);
         }
         if(SystemConfig.W12EdgeLens != rgBXFMLens->ItemIndex) {
            rgBXFMLens->ItemIndex = frmRecipeSet->SystemConfig.W12EdgeLens;

            // 2023 5 3 - chc Log
            frnMain->WriteSystemLog("btnToEdgeClick: " + IntToStr(rgBXFMLens->ItemIndex));

            // 2023 5 4 - chc ����
            rgBXFMLensChange(this,rgBXFMLens->ItemIndex);
         }

      }
   }
   else {
      frnMain->To8EdgePosition(no);

      // 2023 4 24 - chc �[�JEdge Intensit/Lens
      // 2023 5 4 - chc Inspection��no=1�~�n��
      if(frnMain->boolInInspecting == true && no == 1) {

         if(SystemConfig.W8EdgeLens != rgBXFMLens->ItemIndex) {
            rgBXFMLens->ItemIndex = SystemConfig.W8EdgeLens;

            // 2023 5 4 - chc ����
            rgBXFMLensChange(this,rgBXFMLens->ItemIndex);

         }
         if(tbBXFMLMP->Position != SystemConfig.W8Edge[no-1].Intensity) {
            tbBXFMLMP->Position = SystemConfig.W8Edge[no-1].Intensity;
            SetIntensity(SystemConfig.W8Edge[no-1].Intensity);
         }
      }

   }
   btnToEdge->Enabled = true;
   tmUpdatePosition->Enabled = false;

   // 2023 4 19 - chc ��s��m
   UpdateMotionPosition();

}
//---------------------------------------------------------------------------
// 2023 3 16 - chc Find Center
int FindStep = 0;
void __fastcall TfrmRecipeSet::btnFindCenterClick(TObject *Sender)
{
int no,x1,y1,x2,y2,x3,y3,cx,cy;
AnsiString msg;

   // 2023 5 3a - chc ���߻~�t�q
   AdjustCenterDX = 0;
   AdjustCenterDY = 0;

   // 2023 3 22 - chc ���timer�Ӱ�
   pnlCenterX->Caption = "";
   pnlCenterY->Caption = "";
   FindStep = 0;
   tmFindCenter->Enabled = true;
   return;

   no = 1;
   rgEdgeNo->ItemIndex = no - 1;
   pnlSystemMessage->Caption = "Point1 Move...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   btnToEdgeClick(this);
   pnlSystemMessage->Caption = "Point1 Move.";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   // 2023 4 12 - chc ������1000ms
   for(int i=0 ; i<20 ; i++) {
      Sleep(50);
      Application->ProcessMessages();
   }
   pnlSystemMessage->Caption = "Point1 Find...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   // FindEdge()������|��0.5sec
   FindEdge(no,&x1,&y1);
   msg.sprintf("Point1: %d,%d",x1,y1);
   pnlSystemMessage->Caption = msg;
   frnMain->WriteSystemLog(msg);
   Application->MessageBox("Point1","FindEdge    ",MB_ICONINFORMATION|MB_OK);

   no++;
   rgEdgeNo->ItemIndex = no - 1;
   pnlSystemMessage->Caption = "Point2 Move...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   btnToEdgeClick(this);
   pnlSystemMessage->Caption = "Point2 Move.";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   // 2023 4 12 - chc ������1000ms
   for(int i=0 ; i<20 ; i++) {
      Sleep(50);
      Application->ProcessMessages();
   }
   pnlSystemMessage->Caption = "Point2 Find...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   FindEdge(no,&x2,&y2);
   msg.sprintf("Point2: %d,%d",x2,y2);
   pnlSystemMessage->Caption = msg;
   frnMain->WriteSystemLog(msg);
   Application->MessageBox("Point2","FindEdge    ",MB_ICONINFORMATION|MB_OK);

   no++;
   rgEdgeNo->ItemIndex = no - 1;
   pnlSystemMessage->Caption = "Point3 Move...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   btnToEdgeClick(this);
   pnlSystemMessage->Caption = "Point3 Move.";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   // 2023 4 12 - chc ������1000ms
   for(int i=0 ; i<20 ; i++) {
      Sleep(50);
      Application->ProcessMessages();
   }
   pnlSystemMessage->Caption = "Point3 Find...";
   frnMain->WriteSystemLog(pnlSystemMessage->Caption);
   FindEdge(no,&x3,&y3);
   msg.sprintf("Point3: %d,%d",x3,y3);
   pnlSystemMessage->Caption = msg;
   frnMain->WriteSystemLog(msg);
   Application->MessageBox("Point3","FindEdge    ",MB_ICONINFORMATION|MB_OK);

   cx = frnMain->edCenterX->Text.ToInt();
   cy = frnMain->edCenterY->Text.ToInt();
   FindCenter(x1,y1,x2,y2,x3,y3,&cx,&cy);
   pnlCenterX->Caption = IntToStr(cx);
   pnlCenterY->Caption = IntToStr(cy);

   MoveToXY(cx,cy);
   msg.sprintf("Center: %d,%d",cx,cy);
   pnlSystemMessage->Caption = msg;
   frnMain->WriteSystemLog(msg);

   // 2023 3 22 - chc ��s�����I
   frnMain->edCenterX->Text = pnlCenterX->Caption;
   frnMain->edCenterY->Text = pnlCenterY->Caption;

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::pnlCenterXDblClick(TObject *Sender)
{
int x,y;

   if(pnlCenterX->Caption == "" || pnlCenterY->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Value!";
      return;
   }
   x = pnlCenterX->Caption.ToInt();
   y = pnlCenterY->Caption.ToInt();
   MoveToXYFun(x,y);
}
//---------------------------------------------------------------------------
// 2023 3 16 - chc Move to XY
void __fastcall TfrmRecipeSet::MoveToXYFun(int x,int y)
{

   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep�n
      Beep(500);
   }
}
//---------------------------------------------------------------------------
// 2023 3 16 - chc Center XY
void __fastcall TfrmRecipeSet::FindCenter(int x1,int y1,int x2,int y2,int x3,int y3,int *rcx,int *rcy)
{
double a,b,c,d,e,f,a1,b1,c1,a2,b2,c2,r,area,cx,cy,resolution;
AnsiString msg;
double fx,fy;

   a = x1;
   b = y1;
   c = x2;
   d = y2;
   e = x3;
   f = y3;

   if(c == e && d == f)
      return;
   a1 = a - c;
   b1 = b - d;
   c1 = (a*a + b*b - c*c - d*d) / 2;
   a2 = a - e;
   b2 = b - f;
   c2 = (a*a + b*b - e*e - f*f) / 2;
   if((a1*b2-a2*b1) == 0)
      return;
   cx = (c1*b2-c2*b1) / (a1*b2-a2*b1);
   if((a2*b1-a1*b2) == 0)
      return;
   cy = (c1*a2-c2*a1) / (a2*b1-a1*b2);

   *rcx = cx;
   *rcy = cy;
}
//---------------------------------------------------------------------------
// Edge - MoveCenter - Record
void __fastcall TfrmRecipeSet::FindEdge(int no,int *retx,int *rety)
{
int gray,sx,sy,ex,ey,x,y,gap,rx,ry,cx,cy;
unsigned char r1,g1,b1,dgray,r2,g2,b2;
TColor color1,color2;
double ratiox,ratioy;

// 2023 4 14 - chc �令��3��: 1/4, 1/2, 3/4
int loop = 1;
bool boolless = false;
bool boolmore = false;

   // ���v�����
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImage->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imInspection->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imInspection->Height;
   }

   // 2023 5 9 - chc 20�令40
   dgray = 40;

   sx = 120;
   sy = 120;
   ex = imInspection->Width - 120;
   ey = imInspection->Height - 120;

   // 2023 4 4 - chc �令1/4�B
   // 2023 5 3a - chc �令1/5
   //cx = imInspection->Width / 2;
   //cy = imInspection->Height / 2;
   cx = imInspection->Width*loop / 5;
   cy = imInspection->Height*loop / 5;

   sx *= ratiox;
   sy *= ratioy;
   ex *= ratiox;
   ey *= ratioy;

   // 2023 4 27a - chc �վ�G��: Shutter
   // (0.21 �� R) + (0.72 �� G) + (0.07 �� B)
   if(no == 1) {
      pnlSystemMessage->Caption = "Auto Shutter...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      int shutter,x1,y1,x2,y2,sum,r,g,b,gray,cnt;
      x1 = imInspection->Width - 120;
      y1 = imInspection->Height - 120;
      x2 = imInspection->Width - 10;
      y2 = imInspection->Height - 10;
      x1 *= ratiox;
      y1 *= ratioy;
      x2 *= ratiox;
      y2 *= ratioy;
      shutter = rgCCDShutter->ItemIndex;
againshutter:
      sum = 0;
      cnt = 0;
      for(int row=y1 ; row<y2 ; row+=5) {
         for(int col=x1 ; col<x2 ; col+=5) {
            color1 = imInspection->Canvas->Pixels[col][row];
            b1 = (color1 & 0xff0000) >> 16;
            g1 = (color1 & 0x00ff00) >> 8;
            r1 = color1 & 0x0000ff;
            // ��g
            gray = (0.21*r1) + (0.72*g1) + (0.07*b1);
            //gray = g1;
            sum += gray;
            cnt++;
         }
      }
      gray = sum / cnt;
      pnlSystemMessage->Caption = "Gray = " + IntToStr(gray);
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      if(gray < 120) {
         if(boolmore == true) {
            goto again;
         }
         boolless = true;
         if(shutter > 0) {
            pnlSystemMessage->Caption = "Too dark: " + IntToStr(shutter) + " To " + IntToStr(shutter-1);
            frnMain->WriteSystemLog(pnlSystemMessage->Caption);
            shutter--;
            rgCCDShutter->ItemIndex = shutter;
            // => ���ΰ�, �_�h�|���T��
            //frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
            //frnMain->rgCCDShutterClick(this);
            // 2023 4 28 - chc Capture cnt: CaptureCount
            CaptureCount = 0;
            for(int i=0 ; i<200 ; i++) {
               Sleep(10);
               Application->ProcessMessages();

               // 2023 5 5 - chc �令30 or 30��
               //if(CaptureCount > 10 && i > 50) {
               if(CaptureCount > 30 || i > 30) {

                  frnMain->WriteSystemLog("CaptureCount,i= " + IntToStr(CaptureCount) + "," + IntToStr(i));
                  break;
               }
            }
            goto againshutter;
         }
      }
      else if(gray > 180) {
         if(boolless == true) {
            goto again;
         }
         boolmore = true;
         if(shutter < 15) {
            pnlSystemMessage->Caption = "Too bright: " + IntToStr(shutter) + " To " + IntToStr(shutter+1);
            frnMain->WriteSystemLog(pnlSystemMessage->Caption);
            shutter++;
            rgCCDShutter->ItemIndex = shutter;
            // => ���ΰ�, �_�h�|���T��
            //frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
            //frnMain->rgCCDShutterClick(this);
            // 2023 4 28 - chc Capture cnt: CaptureCount
            CaptureCount = 0;
            for(int i=0 ; i<200 ; i++) {
               Sleep(10);
               Application->ProcessMessages();

               // 2023 5 5 - chc �令30 or 30��
               //if(CaptureCount > 10 && i > 50) {
               if(CaptureCount > 30 || i > 30) {

                  frnMain->WriteSystemLog("CaptureCount,i= " + IntToStr(CaptureCount) + "," + IntToStr(i));
                  break;
               }
            }
            goto againshutter;
         }
      }
   }

// 2023 4 4 - chc �令1/4�B
again:
   cx *= ratiox;
   cy *= ratioy;
   rx = ry = -1;
   // ��
   if(no == 1) {
      gap = 100;
      y = cy;
      for(x=sx ; x<ex ; x++) {
         color1 = imInspection->Canvas->Pixels[x][y];
         color2 = imInspection->Canvas->Pixels[x+gap][y];
         //b1 = (color1 & 0xff0000) >> 16;
         g1 = (color1 & 0x00ff00) >> 8;
         //r1 = color1 & 0x0000ff;
         //b2 = (color2 & 0xff0000) >> 16;
         g2 = (color2 & 0x00ff00) >> 8;
         //r2 = color2 & 0x0000ff;
         //frnMain->WriteSystemLog("Found1: " + IntToStr(x) + "," + IntToStr(x+gap) + "," + IntToStr(g1) + "," + IntToStr(g2));
         if(abs(g1 - g2) > dgray) {
            rx = x + gap;
            ry = y;
            frnMain->WriteSystemLog("Found1: " + IntToStr(rx) + "," + IntToStr(ry) + "," + IntToStr(g1) + "," + IntToStr(g2));
            break;
         }
      }
   }
   // �k
   else if(no == 2) {
      gap = -100;
      y = cy;
      for(x=ex ; x>sx ; x--) {
         color1 = imInspection->Canvas->Pixels[x][y];
         color2 = imInspection->Canvas->Pixels[x+gap][y];
         //b1 = (color1 & 0xff0000) >> 16;
         g1 = (color1 & 0x00ff00) >> 8;
         //r1 = color1 & 0x0000ff;
         //b2 = (color2 & 0xff0000) >> 16;
         g2 = (color2 & 0x00ff00) >> 8;
         //r2 = color2 & 0x0000ff;
         //frnMain->WriteSystemLog("Found2: " + IntToStr(x) + "," + IntToStr(x+gap) + "," + IntToStr(g1) + "," + IntToStr(g2));
         if(abs(g1 - g2) > dgray) {
            rx = x + gap;
            ry = y;
            frnMain->WriteSystemLog("Found2: " + IntToStr(rx) + "," + IntToStr(ry) + "," + IntToStr(g1) + "," + IntToStr(g2));
            break;
         }
      }
   }
   // �U
   else if(no == 3) {
      gap = -100;
      x = cx;
      for(y=ey ; y>sy ; y--) {
         color1 = imInspection->Canvas->Pixels[x][y];
         color2 = imInspection->Canvas->Pixels[x][y+gap];
         //b1 = (color1 & 0xff0000) >> 16;
         g1 = (color1 & 0x00ff00) >> 8;
         //r1 = color1 & 0x0000ff;
         //b2 = (color2 & 0xff0000) >> 16;
         g2 = (color2 & 0x00ff00) >> 8;
         //r2 = color2 & 0x0000ff;
         //frnMain->WriteSystemLog("Found3: " + IntToStr(y) + "," + IntToStr(y+gap) + "," + IntToStr(g1) + "," + IntToStr(g2));
         if(abs(g1 - g2) > dgray) {
            rx = x;
            ry = y + gap;
            frnMain->WriteSystemLog("Found3: " + IntToStr(rx) + "," + IntToStr(ry) + "," + IntToStr(g1) + "," + IntToStr(g2));
            break;
         }
      }
   }

   if(rx != -1 && ry != -1) {
      rx /= ratiox;
      ry /= ratioy;
      DoubleClickX = rx;
      DoubleClickY = ry;
      frnMain->WriteSystemLog("FindEdge: Move to Center... " + IntToStr(rx) + "," + IntToStr(ry));
      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Do DoubleClick...","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      // Double Click
      imInspectionDblClick(this);
      //Application->MessageBox("Do DoubleClick.","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      // 2023 5 5 - chc ���WaitTime
      //for(int i=0 ; i<5 ; i++) {
      //   Sleep(100);
      //   Application->ProcessMessages();
      //}
      frnMain->WaitTime(300);

      frnMain->WriteSystemLog("FindEdge: Move to Center.");
   }

   // 2023 4 14 - chc �令��3��: 1/4, 1/2, 3/4
   else {
      // 1/2/3
      loop++;
      // 2023 5 3a - chc 1/4�令1/5
      if(loop < 5) {
         cx = imInspection->Width*loop / 5;
         cy = imInspection->Height*loop / 5;
         goto again;
      }
   }

   *retx = pnlX->Caption.ToInt();
   *rety = pnlY->Caption.ToInt();
   frnMain->WriteSystemLog("FindEdge: Leave.");
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::cbFreezeClick(TObject *Sender)
{
int w,h,width,height,sx,sy,x,y;
int ccdno;

   ccdno = 0;
   width = frnMain->CCDInfoAry[ccdno].Width;
   height = frnMain->CCDInfoAry[ccdno].Height;
   w = imInspection->Width;
   h = imInspection->Height;
   if(cbCCDFullImage->Checked == true) {
      sx = 0;
      sy = 0;
   }
   else {
      sx = (width - w) / 2;
      sy = (height - h) / 2;
      imInspection->Picture->Bitmap->Width = w;
      imInspection->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[ccdno].Detach();
      frnMain->usb_ImageROIAry[ccdno].Attach(&frnMain->usb_ImageAry[ccdno]);
      frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
      frnMain->usb_ImageROIAry[ccdno].SetPlacement(x,y,w,h);
      frnMain->usb_ImageROIAry[ccdno].Draw(imInspection->Canvas->Handle);
   }
   ImageSX = sx;
   ImageSY = sy;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnLoadTestImageClick(TObject *Sender)
{
AnsiString fname;
int ccdno,w,h;

   fname = frnMain->SystemDirectory + "\\Test3.bmp";
   ccdno = 0;
   frnMain->ImageAry.Load(fname.c_str());

   w = frnMain->ImageAry.GetWidth();
   h = frnMain->ImageAry.GetHeight();
   //frnMain->CCDInfoAry[ccdno].Width = w;
   //frnMain->CCDInfoAry[ccdno].Width = h;
   imInspection->Picture->Bitmap->Width = w;
   imInspection->Picture->Bitmap->Height = h;
   frnMain->ImageAry.Draw(imInspection->Canvas->Handle);

   // 2023 3 17 - chc �]�wSize
   frnMain->CCDInfoAry[0].Width = w;
   frnMain->CCDInfoAry[0].Height = h;
   frnMain->usb_ImageAry[0].Load(fname.c_str());
   GigaWidth = w;
   GigaHeight = h;

   double f;
   f = (double)imInspection->Width / w * 100.0;
   fname.sprintf("%.2f\%",f);
   pnlMainFactor->Caption = fname;
   cbCCDFullImage->Checked = true;
   cbFreeze->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::imInspectionMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageLeftMouse = false;
}
//---------------------------------------------------------------------------
// 2023 3 20 - chc Draw Sub die
void __fastcall TfrmRecipeSet::DrawSubDie()
{
int w,h,xno,yno,dx,dy;

   xno = edSubDieXno->Text.ToInt();
   yno = edSubDieYno->Text.ToInt();
   if(xno == 1 && yno == 1) {
      sgDie->Visible = false;
      return;
   }

   sgDie->Visible = true;
   w = sgDie->Width;
   h = sgDie->Height;
   dx = w / xno;
   dy = h / yno;
   sgDie->DefaultColWidth = dx - 1;
   sgDie->DefaultRowHeight = dy - 1;
   sgDie->ColCount = xno;
   sgDie->RowCount = yno;

}
//---------------------------------------------------------------------------
// 2023 3 20 - chc Load System Parameters
//struct SYSTEM_STRU {
//   AnsiString ResultPath,TxtPath,RecipePath,DeviceID;
//   AnsiStrnig InspectionStationID[3];
//   int FilterFW1,FilterFW2;
//   bool TableDirection;
//   bool JoystickContinueMove;
//   struct XYZ_STRU W12Edge[3];
//   struct XYZ_STRU W8Edge[3];
//};
//   struct SYSTEM_STRU SystemConfig;
void __fastcall TfrmRecipeSet::LoadSystemParameters()
{
TIniFile *pSystemFile;

   SystemConfigName = frnMain->SystemDirectory + "\\SystemConfig.ini";
   pSystemFile = new TIniFile(SystemConfigName);

   // Path
   SystemConfig.ResultPath = pSystemFile->ReadString("Path","Result"            ,"C:\\WLS3200v\\Result");
   SystemConfig.TxtPath = pSystemFile->ReadString("Path","Txt"                  ,"C:\\WLS3200v\\WaferTxt");
   SystemConfig.RecipePath = pSystemFile->ReadString("Path","Recipe"            ,"C:\\WLS3200v\\Recipe");
   edResultPath->Text = SystemConfig.ResultPath;
   edWaferTxtPath->Text = SystemConfig.TxtPath;
   edRecipePath->Text = SystemConfig.RecipePath;

   // 2023 4 19 - chc Cognex
   SystemConfig.CognexPath = pSystemFile->ReadString("Path","Cognex"            ,"C:\\Program Files (x86)\\Cognex\\In-Sight\\In-Sight Explorer Wafer 4.5.0\\WaferID.exe");
   SystemConfig.CognexTitle = pSystemFile->ReadString("Title","Cognex"          ,"Cognex Wafer ID - 4.5.0");
   edCognexPath->Text = SystemConfig.CognexPath;
   edCognexTitle->Text = SystemConfig.CognexTitle;

   // 2023 4 19 - chc edAlignmentDefault
   SystemConfig.AlignmentDefault = pSystemFile->ReadInteger("Alignment","Default"                               ,-71);
   edAlignmentDefault->Text = IntToStr(SystemConfig.AlignmentDefault);

   // 2023 5 5 - chc Lamp Life
   SystemConfig.LampLife = pSystemFile->ReadInteger("Alignment","Lamp Life"                            ,2000);
   edLampLife->Text = IntToStr(SystemConfig.LampLife);

   // 2023 5 6 - chc DieSaveOverlap
   SystemConfig.DieSaveOverlap = pSystemFile->ReadInteger("Alignment","DieSaveOverlap"                            ,100);
   edDieSaveOverlap->Text = IntToStr(SystemConfig.DieSaveOverlap);

   // DeviceID
   SystemConfig.DeviceID = pSystemFile->ReadString("ID","Device"                ,"INSPECTION");
   edDeviceID->Text = SystemConfig.DeviceID;
   SystemConfig.InspectionStationID[0] = pSystemFile->ReadString("ID","InspectionStation1"     ,"Olympus");
   SystemConfig.InspectionStationID[1] = pSystemFile->ReadString("ID","InspectionStation2"     ,"OSF");
   SystemConfig.InspectionStationID[2] = pSystemFile->ReadString("ID","InspectionStation3"     ,"Oly101");
   edInspectionStationID1->Text = SystemConfig.InspectionStationID[0];
   edInspectionStationID2->Text = SystemConfig.InspectionStationID[1];
   edInspectionStationID3->Text = SystemConfig.InspectionStationID[2];

   // 12" Edge
   // 1
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100
      SystemConfig.W12Edge[0].X = pSystemFile->ReadInteger("W12Edge1","X"       ,3973563);
      SystemConfig.W12Edge[0].Y = pSystemFile->ReadInteger("W12Edge1","Y"       ,-214311);
      SystemConfig.W12Edge[0].Z = pSystemFile->ReadInteger("W12Edge1","Z"       ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W12Edge[0].X = pSystemFile->ReadInteger("W12Edge1","X"    ,2097981);
         SystemConfig.W12Edge[0].Y = pSystemFile->ReadInteger("W12Edge1","Y"    ,101107);
         SystemConfig.W12Edge[0].Z = pSystemFile->ReadInteger("W12Edge1","Z"    ,160000);
      }
      else {
         // 3200
         SystemConfig.W12Edge[0].X = pSystemFile->ReadInteger("W12Edge1","X"    ,3973563);
         SystemConfig.W12Edge[0].Y = pSystemFile->ReadInteger("W12Edge1","Y"    ,-214311);
         SystemConfig.W12Edge[0].Z = pSystemFile->ReadInteger("W12Edge1","Z"    ,250000);
      }
   }
   ed12EdgeX1->Text = IntToStr(SystemConfig.W12Edge[0].X);
   ed12EdgeY1->Text = IntToStr(SystemConfig.W12Edge[0].Y);
   ed12EdgeZ1->Text = IntToStr(SystemConfig.W12Edge[0].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[0].Intensity = pSystemFile->ReadInteger("W12Edge1","Intensity"    ,80);
   ed12EdgeIntensity1->Text = IntToStr(SystemConfig.W12Edge[0].Intensity);

   // 2
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100
      SystemConfig.W12Edge[1].X = pSystemFile->ReadInteger("W12Edge2","X"       ,2219828);
      SystemConfig.W12Edge[1].Y = pSystemFile->ReadInteger("W12Edge2","Y"       ,-215394);
      SystemConfig.W12Edge[1].Z = pSystemFile->ReadInteger("W12Edge2","Z"       ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W12Edge[1].X = pSystemFile->ReadInteger("W12Edge2","X"    ,761926);
         SystemConfig.W12Edge[1].Y = pSystemFile->ReadInteger("W12Edge2","Y"    ,281052);
         SystemConfig.W12Edge[1].Z = pSystemFile->ReadInteger("W12Edge2","Z"    ,160000);
      }
      else {
         // 3200
         SystemConfig.W12Edge[1].X = pSystemFile->ReadInteger("W12Edge2","X"    ,2219828);
         SystemConfig.W12Edge[1].Y = pSystemFile->ReadInteger("W12Edge2","Y"    ,-215394);
         SystemConfig.W12Edge[1].Z = pSystemFile->ReadInteger("W12Edge2","Z"    ,250000);
      }
   }
   ed12EdgeX2->Text = IntToStr(SystemConfig.W12Edge[1].X);
   ed12EdgeY2->Text = IntToStr(SystemConfig.W12Edge[1].Y);
   ed12EdgeZ2->Text = IntToStr(SystemConfig.W12Edge[1].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[1].Intensity = pSystemFile->ReadInteger("W12Edge2","Intensity"    ,80);
   ed12EdgeIntensity2->Text = IntToStr(SystemConfig.W12Edge[1].Intensity);

   // 3
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100
      SystemConfig.W12Edge[2].X = pSystemFile->ReadInteger("W12Edge3","X"       ,2645519);
      SystemConfig.W12Edge[2].Y = pSystemFile->ReadInteger("W12Edge3","Y"       ,2425166);
      SystemConfig.W12Edge[2].Z = pSystemFile->ReadInteger("W12Edge3","Z"       ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W12Edge[2].X = pSystemFile->ReadInteger("W12Edge3","X"    ,1075167);
         SystemConfig.W12Edge[2].Y = pSystemFile->ReadInteger("W12Edge3","Y"    ,149296);
         SystemConfig.W12Edge[2].Z = pSystemFile->ReadInteger("W12Edge3","Z"    ,160000);
      }
      else {
         // 3200
         SystemConfig.W12Edge[2].X = pSystemFile->ReadInteger("W12Edge3","X"    ,2645519);
         SystemConfig.W12Edge[2].Y = pSystemFile->ReadInteger("W12Edge3","Y"    ,2425166);
         SystemConfig.W12Edge[2].Z = pSystemFile->ReadInteger("W12Edge3","Z"    ,250000);
      }
   }
   ed12EdgeX3->Text = IntToStr(SystemConfig.W12Edge[2].X);
   ed12EdgeY3->Text = IntToStr(SystemConfig.W12Edge[2].Y);
   ed12EdgeZ3->Text = IntToStr(SystemConfig.W12Edge[2].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12Edge[2].Intensity = pSystemFile->ReadInteger("W12Edge3","Intensity"    ,80);
   ed12EdgeIntensity3->Text = IntToStr(SystemConfig.W12Edge[2].Intensity);

   // 8" Edge
   // 1
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100(X)
      SystemConfig.W8Edge[0].X = pSystemFile->ReadInteger("W8Edge1","X"         ,3973563);
      SystemConfig.W8Edge[0].Y = pSystemFile->ReadInteger("W8Edge1","Y"         ,-21431);
      SystemConfig.W8Edge[0].Z = pSystemFile->ReadInteger("W8Edge1","Z"         ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W8Edge[0].X = pSystemFile->ReadInteger("W8Edge1","X"      ,1895799);
         SystemConfig.W8Edge[0].Y = pSystemFile->ReadInteger("W8Edge1","Y"      ,242997);
         SystemConfig.W8Edge[0].Z = pSystemFile->ReadInteger("W8Edge1","Z"      ,260000);
      }
      else {
         // 3200
         SystemConfig.W8Edge[0].X = pSystemFile->ReadInteger("W8Edge1","X"      ,3973563);
         SystemConfig.W8Edge[0].Y = pSystemFile->ReadInteger("W8Edge1","Y"      ,-21431);
         SystemConfig.W8Edge[0].Z = pSystemFile->ReadInteger("W8Edge1","Z"      ,250000);
      }
   }
   ed8EdgeX1->Text = IntToStr(SystemConfig.W8Edge[0].X);
   ed8EdgeY1->Text = IntToStr(SystemConfig.W8Edge[0].Y);
   ed8EdgeZ1->Text = IntToStr(SystemConfig.W8Edge[0].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[0].Intensity = pSystemFile->ReadInteger("W8Edge1","Intensity"    ,80);
   ed8EdgeIntensity1->Text = IntToStr(SystemConfig.W8Edge[0].Intensity);

   // 2
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100(X)
      SystemConfig.W8Edge[1].X = pSystemFile->ReadInteger("W8Edge2","X"         ,2219828);
      SystemConfig.W8Edge[1].Y = pSystemFile->ReadInteger("W8Edge2","Y"         ,-215394);
      SystemConfig.W8Edge[1].Z = pSystemFile->ReadInteger("W8Edge2","Z"         ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W8Edge[1].X = pSystemFile->ReadInteger("W8Edge2","X"      ,1056949);
         SystemConfig.W8Edge[1].Y = pSystemFile->ReadInteger("W8Edge2","Y"      ,245323);
         SystemConfig.W8Edge[1].Z = pSystemFile->ReadInteger("W8Edge2","Z"      ,260000);
      }
      else {
         // 3200
         SystemConfig.W8Edge[1].X = pSystemFile->ReadInteger("W8Edge2","X"      ,2219828);
         SystemConfig.W8Edge[1].Y = pSystemFile->ReadInteger("W8Edge2","Y"      ,-215394);
         SystemConfig.W8Edge[1].Z = pSystemFile->ReadInteger("W8Edge2","Z"      ,250000);
      }
   }
   ed12EdgeX2->Text = IntToStr(SystemConfig.W8Edge[1].X);
   ed12EdgeY2->Text = IntToStr(SystemConfig.W8Edge[1].Y);
   ed12EdgeZ2->Text = IntToStr(SystemConfig.W8Edge[1].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[1].Intensity = pSystemFile->ReadInteger("W8Edge2","Intensity"    ,80);
   ed8EdgeIntensity2->Text = IntToStr(SystemConfig.W8Edge[1].Intensity);

   // 3
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      // 3100(X)
      SystemConfig.W8Edge[2].X = pSystemFile->ReadInteger("W8Edge3","X"         ,26455119);
      SystemConfig.W8Edge[2].Y = pSystemFile->ReadInteger("W8Edge3","Y"         ,2425166);
      SystemConfig.W8Edge[2].Z = pSystemFile->ReadInteger("W8Edge3","Z"         ,250000);
   }
   else {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         // VisEra
         SystemConfig.W8Edge[2].X = pSystemFile->ReadInteger("W8Edge3","X"      ,1294914);
         SystemConfig.W8Edge[2].Y = pSystemFile->ReadInteger("W8Edge3","Y"      ,980744);
         SystemConfig.W8Edge[2].Z = pSystemFile->ReadInteger("W8Edge3","Z"      ,260000);
      }
      else {
         // 3200
         SystemConfig.W8Edge[2].X = pSystemFile->ReadInteger("W8Edge3","X"      ,26455119);
         SystemConfig.W8Edge[2].Y = pSystemFile->ReadInteger("W8Edge3","Y"      ,2425166);
         SystemConfig.W8Edge[2].Z = pSystemFile->ReadInteger("W8Edge3","Z"      ,250000);
      }
   }
   ed8EdgeX3->Text = IntToStr(SystemConfig.W8Edge[2].X);
   ed8EdgeY3->Text = IntToStr(SystemConfig.W8Edge[2].Y);
   ed8EdgeZ3->Text = IntToStr(SystemConfig.W8Edge[2].Z);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W8Edge[2].Intensity = pSystemFile->ReadInteger("W8Edge3","Intensity"    ,80);
   ed8EdgeIntensity3->Text = IntToStr(SystemConfig.W8Edge[2].Intensity);

   // 2023 4 24 - chc �[�JEdge Intensit/Lens
   SystemConfig.W12EdgeLens = pSystemFile->ReadInteger("W12Edge","Lens"                                   ,0);
   rg12EdgeLens->ItemIndex = SystemConfig.W12EdgeLens;
   SystemConfig.W8EdgeLens = pSystemFile->ReadInteger("W8Edge","Lens"                                    ,0);
   rg8EdgeLens->ItemIndex = SystemConfig.W8EdgeLens;

   pSystemFile->UpdateFile();

   delete pSystemFile;
}
//---------------------------------------------------------------------------
// 2023 3 21 - chc Sub-Die
void __fastcall TfrmRecipeSet::DrawSubDieMap(int sx,int sy,int ex,int ey)
{
int dx,dy,xno,yno,width,height,x,y;
double fdx,fdy;

   width = ex - sx + 1;
   height = ey - sy + 1;
   xno = edSubDieXno->Text.ToInt();
   yno = edSubDieYno->Text.ToInt();
   fdx = (double)width / xno;
   fdy = (double)height / yno;

   imWaferMap->Canvas->Pen->Color = clGray;
   imWaferMap->Canvas->Pen->Width = 1;
   // �����u
   for(int i=1 ; i<xno ; i++) {
      x = sx + fdx * i;
      imWaferMap->Canvas->MoveTo(x,sy);
      imWaferMap->Canvas->LineTo(x,ey);
   }
   // �����u
   for(int i=1 ; i<yno ; i++) {
      y = sy + fdy * i;
      imWaferMap->Canvas->MoveTo(sx,y);
      imWaferMap->Canvas->LineTo(ex,y);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgDieMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int sx,sy,dx,dy,col,row;
AnsiString msg;

   sx = sgDie->Left;
   sy = sgDie->Top;
   dx = sgDie->DefaultColWidth;
   dy = sgDie->DefaultRowHeight;
   col = X / dx;
   row = Y / dy;

   //pnlChipCol->Caption = IntToStr(col);
   //pnlChipRow->Caption = IntToStr(row);
   // �[�JHint
   sgDie->ShowHint = false;
   msg.sprintf("(%d,%d)",col,row);
   sgDie->Hint = msg;
   sgDie->ShowHint = true;
}
//---------------------------------------------------------------------------


void __fastcall TfrmRecipeSet::btnBackwardZ1Click(TObject *Sender)
{

   frnMain->cbContinueMove->Checked = false;
   frnMain->btnBackwardZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnMoveZ1Click(TObject *Sender)
{

   frnMain->cbContinueMove->Checked = false;
   frnMain->btnMoveZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------
// 2023 3 22 - chc ���timer�Ӱ�
// 200ms
void __fastcall TfrmRecipeSet::tmFindCenterTimer(TObject *Sender)
{
static int no,x1,y1,x2,y2,x3,y3,cx,cy;
AnsiString msg;

// 2023 4 27a - chc Shutter value
int shutter;
   // 0 - 15
   shutter = rgCCDShutter->ItemIndex;

   FindStep++;
   tmFindCenter->Enabled = false;

   // 1/2/3/4/5
   if(FindStep == 1) {
      no = 1;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point1 Move...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point1 Move.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 2 || FindStep == 3 || FindStep == 4) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 5) {
      no = 1;
      pnlSystemMessage->Caption = "Point1 Find...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x1,&y1);
      msg.sprintf("Point1: %d,%d",x1,y1);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);
      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point1","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }

   // 2023 5 3a - chc Delay
   if(FindStep == 6 || FindStep == 7 || FindStep == 8) {
      tmFindCenter->Enabled = true;
      goto end;
   }

   // 9/10/11/12/13
   if(FindStep == 9) {
      no = 2;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point2 Move...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point2 Move.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 10 || FindStep == 11 || FindStep == 12) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 13) {
      no = 2;
      pnlSystemMessage->Caption = "Point2 Find...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x2,&y2);
      msg.sprintf("Point2: %d,%d",x2,y2);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);
      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point2","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }

   // 2023 5 3a - chc Delay
   if(FindStep == 14 || FindStep == 15 || FindStep == 16) {
      tmFindCenter->Enabled = true;
      goto end;
   }

   // 17/18/19/20/21
   if(FindStep == 17) {
      no = 3;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point3 Move...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point3 Move.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 18 || FindStep == 19 || FindStep == 20) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc �Τ@
      //return;
      goto end;

   }
   if(FindStep == 21) {
      no = 3;
      pnlSystemMessage->Caption = "Point3 Find...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x3,&y3);
      msg.sprintf("Point3: %d,%d",x3,y3);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);
      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point3","FindEdge    ",MB_ICONINFORMATION|MB_OK);

      // 2023 5 3a - chc ���߻~�t�q
      AdjustCenterDX = 0;
      AdjustCenterDY = 0;

      cx = frnMain->edCenterX->Text.ToInt();
      cy = frnMain->edCenterY->Text.ToInt();
      FindCenter(x1,y1,x2,y2,x3,y3,&cx,&cy);
      pnlCenterX->Caption = IntToStr(cx);
      pnlCenterY->Caption = IntToStr(cy);

      // 2023 5 3a - chc �����I���v��: AdjustCenterDX
      AdjustCenterDX = 0;
      AdjustCenterDY = 0;

      MoveToXY(cx,cy);
      msg.sprintf("Center: %d,%d",cx,cy);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);

      // 2023 5 9 - chc Delay
      frnMain->WaitTime(300);

   }

// 2023 4 14 - chc ��sMotion Position
end:
   UpdateMotionPosition();

   // 2023 5 6 - chc ���ݭn
   //UpdateCurrentPosition();

   // 2023 4 27a - chc Shutter value
   // 0 - 15
   if(rgCCDShutter->ItemIndex != shutter) {
      rgCCDShutter->ItemIndex = shutter;
      frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
      frnMain->rgCCDShutterClick(this);
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc �令�T��
   return;

   frnMain->cbContinueMove->Checked = true;
   frnMain->btnBackwardTMouseDown(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc �令�T��
   return;

   frnMain->btnBackwardTMouseUp(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc �令�T��
   return;

   frnMain->btnMoveTMouseDown(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc �令�T��
   return;

   frnMain->btnMoveTMouseUp(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::mnDeletePointClick(TObject *Sender)
{
int no,tno,colcnt;

   no = sgDefectList->Row;
   if(no < 1 || sgDefectList->RowCount <= 2 || no == sgDefectList->RowCount-1) {
      return;
   }

   colcnt = sgDefectList->ColCount;
   tno = sgDefectList->RowCount-1;
   for(int i=no ; i<tno ; i++) {
      for(int j=0 ; j<colcnt ; j++) {
         sgDefectList->Cells[j][i] = sgDefectList->Cells[j][i+1];
      }
   }
   sgDefectList->RowCount--;

   //���s�U��s��
   tno = sgDefectList->RowCount-1;
   for(int i=1 ; i<tno ; i++) {
      sgDefectList->Cells[0][i] = i;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::mnDeleteAllPointClick(TObject *Sender)
{
int no,tno,colcnt;

   tno = sgDefectList->RowCount;
   sgDefectList->RowCount = 2;
   colcnt = sgDefectList->ColCount;
   for(int j=1 ; j<tno ; j++) {
      for(int i=0 ; i<colcnt ; i++) {
         sgDefectList->Cells[i][j] = "";
      }
   }
}
//---------------------------------------------------------------------------
// sgDefectList->Cells[0][0] = "No";
// sgDefectList->Cells[1][0] = "ID";
// sgDefectList->Cells[2][0] = "X";
// sgDefectList->Cells[3][0] = "Y";
// sgDefectList->Cells[4][0] = "Col";
// sgDefectList->Cells[5][0] = "Row";
// sgDefectList->Cells[6][0] = "Judge";
// �[�JDefect W/H/A
// sgDefectList->Cells[7][0] = "XSize";
// sgDefectList->Cells[8][0] = "YSize";
// sgDefectList->Cells[9][0] = "Area";
void __fastcall TfrmRecipeSet::AddPointData()
{
int no,col,row,x,y,cx,cy;
AnsiString name,msg;
double f1,f2;

   no = sgDefectList->RowCount - 1;

   // 2023 4 11 - chc �OpnlWorkCol�~��
   //col = pnlChipCol->Caption.ToInt();
   //row = pnlChipRow->Caption.ToInt();
   col = pnlWorkCol->Caption.ToInt();
   row = pnlWorkRow->Caption.ToInt();

   UpdateMotionPosition();
   x = pnlX->Caption.ToInt();
   y = pnlY->Caption.ToInt();

   // 2023 4 7 - chc not judge
   //cx = frnMain->edCounterX->Text.ToInt();
   //cy = frnMain->edCounterY->Text.ToInt();
   //x = cx - x;
   //y = cy - y;
   frnMain->WriteSystemLog("Add: X,Y= " + IntToStr(x) + "," + IntToStr(y));

   sgDefectList->Cells[0][no] = IntToStr(no);
   sgDefectList->Cells[1][no] = "0";

   // 2023 4 25 - chc �[�Jname
   sgDefectList->Cells[2][no] = "";

   sgDefectList->Cells[2+1][no] = IntToStr(x);
   sgDefectList->Cells[3+1][no] = IntToStr(y);
   sgDefectList->Cells[4+1][no] = IntToStr(col);
   sgDefectList->Cells[5+1][no] = IntToStr(row);
   sgDefectList->Cells[6+1][no] = "0";
   sgDefectList->Cells[7+1][no] = "0";
   sgDefectList->Cells[8+1][no] = "0";
   sgDefectList->Cells[9+1][no] = "0";

   // 2023 4 7 - chc �[�JLens/Intensity
   sgDefectList->Cells[10+1][no] = rgBXFMLens->ItemIndex;
   sgDefectList->Cells[11+1][no] = tbBXFMLMP->Position;

   // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
   sgDefectList->Cells[12+1][no] = rgObservation->ItemIndex;
   sgDefectList->Cells[13+1][no] = tbDICFC->Position;

   sgDefectList->RowCount++;

   // 2023 4 7 - chc Clear
   // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
   /*
   sgDefectList->Cells[0][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[1][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[2][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[3][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[4][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[5][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[6][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[7][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[8][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[9][sgDefectList->RowCount-1] = "";

   // 2023 4 7 - chc �[�JLens/Intensity
   sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
   */
   for(int i=0 ; i<sgDefectList->ColCount ; i++)
      sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

   // 2023 4 17 - chc ����̫�@��
   sgDefectList->Row = sgDefectList->RowCount - 2;

}
//---------------------------------------------------------------------------
// 2023 3 26 - chc SECS, Delete Recipe
void __fastcall TfrmRecipeSet::btnDeleteRecipeClick(TObject *Sender)
{
AnsiString recipefname,recipename,mfname,delfname;
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

   recipename = combRecipe->Text;
   recipefname = edRecipePath->Text + "\\" + recipename + ".ini";

   // 2023 3 26 - chc SECS: Modify(Save) Recipe
   if(FileExists(recipefname)) {
      if(Application->MessageBox(("Sure to Delete Recipe[" + combRecipe->Text + "]? ").c_str(), "Recipe Delete", MB_YESNO|MB_SYSTEMMODAL) == IDYES) {
         DeleteFile(recipefname.c_str());
         frnMain->WriteSystemLog("SECS: Recipe Deleted " + recipefname);
         mfname = edRecipePath->Text + "\\" + recipename + "\\*.*";
         iAttributes |= faArchive;
         if(FindFirst(mfname, iAttributes, sr) == 0) {
            do {
               if((sr.Attr & iAttributes) == sr.Attr) {                               // �ɦW�bsr.Name
                  delfname = edRecipePath->Text + "\\" + recipename + "\\" + sr.Name;
                  DeleteFile(delfname.c_str());
                  frnMain->WriteSystemLog("SECS: Recipe Deleted " + delfname);
               }
            } while (FindNext(sr) == 0);
         }
         FindClose(sr);

         //combRecipe->Text = "";
         //combRecipe->ItemIndex = -1;
         // ??????
         combRecipe->DeleteSelected();
         frnMain->edSECSRecipeID->Text = combRecipe->Text;
         frnMain->rgSECSRecipe->ItemIndex = RECIPE_DELETE;
         frnMain->WriteSystemLog("SECS: Recipe Deleted " + combRecipe->Text);
         frnMain->SetToSECS(SECS_CMD_RCPM - SECS_CMD_STAT_1);
      }
   }
}
//---------------------------------------------------------------------------
// 2023 3 29a - chc �[�JPoint
void __fastcall TfrmRecipeSet::mnAddPointClick(TObject *Sender)
{

   // 2023 3 30a - chc ���o�ثeXY
   UpdateMotionPosition();

   AddPointData();
}
//---------------------------------------------------------------------------
// 2023 3 29a - chc DefectList Title
void __fastcall TfrmRecipeSet::DefectListTitle()
{

   // BINCode Define
   sgDefectList->ColWidths[0] = 30;
   sgDefectList->ColWidths[1] = 30;

   // 2023 4 25 - chc �[�Jname
   sgDefectList->ColWidths[2] = 70;

   sgDefectList->ColWidths[2+1] = 50;
   sgDefectList->ColWidths[3+1] = 50;
   sgDefectList->ColWidths[4+1] = 35;
   sgDefectList->ColWidths[5+1] = 35;
   sgDefectList->ColWidths[6+1] = 40;
   // Defect W/H/A
   sgDefectList->ColWidths[7+1] = 60;
   sgDefectList->ColWidths[8+1] = 60;
   sgDefectList->ColWidths[9+1] = 65;

   // 2023 4 25 - chc �[�JLens/Intensity
   sgDefectList->ColWidths[10+1] = 40;
   sgDefectList->ColWidths[11+1] = 40;
   // 2023 4 25 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
   sgDefectList->ColWidths[12+1] = 40;
   sgDefectList->ColWidths[13+1] = 40;

   sgDefectList->Cells[0][0] = "No";
   sgDefectList->Cells[1][0] = "ID";

   // 2023 4 25 - chc �[�Jname
   sgDefectList->Cells[2][0] = "Name";

   sgDefectList->Cells[2+1][0] = "X";
   sgDefectList->Cells[3+1][0] = "Y";
   sgDefectList->Cells[4+1][0] = "Col";
   sgDefectList->Cells[5+1][0] = "Row";
   sgDefectList->Cells[6+1][0] = "Judge";
   // Defect W/H/A
   sgDefectList->Cells[7+1][0] = "XSize";
   sgDefectList->Cells[8+1][0] = "YSize";
   sgDefectList->Cells[9+1][0] = "Area";

   // 2023 4 7 - chc �[�JLens/Intensity
   sgDefectList->Cells[10+1][0] = "Lens";
   sgDefectList->Cells[11+1][0] = "Inten.";

   // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
   sgDefectList->Cells[12+1][0] = "Obser.";
   sgDefectList->Cells[13+1][0] = "DICVa.";

   sgDefectList->RowCount = 2;

   // 2023 4 14 - chc �[�JBF/DF/DIC�ﶵ & DIC�j�� : Observation,DICValue
   /*
   sgDefectList->Cells[0][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[1][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[2][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[3][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[4][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[5][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[6][sgDefectList->RowCount-1] = "";
   // Defect W/H/A
   sgDefectList->Cells[7][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[8][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[9][sgDefectList->RowCount-1] = "";

   // 2023 4 7 - chc �[�JLens/Intensity
   sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
   */
   for(int i=0 ; i<sgDefectList->ColCount ; i++)
      sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

}
//---------------------------------------------------------------------------
// 2023 3 29a - chc BinCodeItem Title
void __fastcall TfrmRecipeSet::BinCodeItemTitle()
{

   sgBinCodeItem->ColWidths[0] = 20;
   sgBinCodeItem->ColWidths[1] = 25;
   sgBinCodeItem->ColWidths[2] = 75;
   sgBinCodeItem->Cells[0][0] = "No";
   sgBinCodeItem->Cells[1][0] = "ID";
   sgBinCodeItem->Cells[2][0] = "Name";
   sgBinCodeItem->RowCount = 2;
   sgBinCodeItem->Cells[0][sgBinCodeItem->RowCount-1] = "";

}
//---------------------------------------------------------------------------
// 2023 3 29a - chc BinCodeItem Title
void __fastcall TfrmRecipeSet::BinCodeTitle()
{

   sgBINCode->RowCount = 2;
   sgBINCode->ColWidths[0] = 40;
   sgBINCode->ColWidths[1] = 35;
   sgBINCode->ColWidths[2] = 30;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";
   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";

}
//---------------------------------------------------------------------------
// Read BinCode List
void __fastcall TfrmRecipeSet::btnGlassIQCFClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbGlassIQCF->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------
// Read BinCode List
void __fastcall TfrmRecipeSet::BinCodeRead(AnsiString fname)
{
AnsiString msg,sid,sname;
FILE *pFile;
char buf[250];
int no,pos,len,id;

   pFile = fopen(fname.c_str(),"r");
   if(pFile) {
      sgBinCodeItem->RowCount = 2;
      sgBinCodeItem->Cells[0][sgBinCodeItem->RowCount-1] = "";
      sgBinCodeItem->Cells[1][sgBinCodeItem->RowCount-1] = "";
      sgBinCodeItem->Cells[2][sgBinCodeItem->RowCount-1] = "";
      no = 1;

      rewind(pFile);
      while(!feof(pFile)) {
         if(fgets(buf,200,pFile) > 0) {
            msg = buf;
            len = msg.Length();
            pos = msg.Pos(" ");
            if(len > 0 && pos > 0) {
               sid = msg.SubString(1,pos-1);
               sname = msg.SubString(pos+1,len-pos);
               sgBinCodeItem->Cells[0][sgBinCodeItem->RowCount-1] = IntToStr(no);
               sgBinCodeItem->Cells[1][sgBinCodeItem->RowCount-1] = sid;
               sgBinCodeItem->Cells[2][sgBinCodeItem->RowCount-1] = sname;
               sgBinCodeItem->RowCount++;
               no++;
            }
         }
      }
      fclose(pFile);
      sgBinCodeItem->Cells[0][sgBinCodeItem->RowCount-1] = "";
      sgBinCodeItem->Cells[1][sgBinCodeItem->RowCount-1] = "";
      sgBinCodeItem->Cells[2][sgBinCodeItem->RowCount-1] = "";
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnGlassIQCBClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbGlassIQCB->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnGlassOQCFClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbGlassOQCF->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnGlassOQCBClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbGlassOQCB->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiIQCSSClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiIQCSS->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiIQCLSClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiIQCLS->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiIQCPadClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiIQCPad->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiIQCSAClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiIQCSA->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiOQCSSClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiOQCSS->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiOQCLSClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiOQCLS->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiOQCPadClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiOQCPad->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSiOQCWClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbSiOQCW->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------
// Collect BinCode
void __fastcall TfrmRecipeSet::btnBinCodeUpdateClick(TObject *Sender)
{
int maxno;
AnsiString fname,name;
AnsiString msg,sid,sname;
FILE *pFile;
char buf[250];
int no,pos,len,id;

   sgBINCode->RowCount = 2;
   sgBINCode->ColWidths[0] = 40;
   sgBINCode->ColWidths[1] = 35;
   sgBINCode->ColWidths[2] = 30;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";

   maxno = 14;
   no = 1;
   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";                             // ID
   sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";                             // Color
   sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";                             // Name
   for(int i=0 ; i<maxno ; i++) {
      if(i == 0) {
         if(cbGood->Checked == false)
            continue;
         name = cbGood->Caption;
      }
      else if(i == 1) {
         if(cbGlassIQCF->Checked == false)
            continue;
         name = cbGlassIQCF->Caption;
      }
      else if(i == 2) {
         if(cbGlassIQCB->Checked == false)
            continue;
         name = cbGlassIQCB->Caption;
      }
      else if(i == 3) {
         if(cbGlassOQCF->Checked == false)
            continue;
         name = cbGlassOQCF->Caption;
      }
      else if(i == 4) {
         if(cbGlassOQCB->Checked == false)
            continue;
         name = cbGlassOQCB->Caption;
      }
      else if(i == 5) {
         if(cbSiIQCSS->Checked == false)
            continue;
         name = cbSiIQCSS->Caption;
      }
      else if(i == 6) {
         if(cbSiIQCLS->Checked == false)
            continue;
         name = cbSiIQCLS->Caption;
      }
      else if(i == 7) {
         if(cbSiIQCPad->Checked == false)
            continue;
         name = cbSiIQCPad->Caption;
      }
      else if(i == 8) {
         if(cbSiIQCSA->Checked == false)
            continue;
         name = cbSiIQCSA->Caption;
      }
      else if(i == 9) {
         if(cbSiOQCSS->Checked == false)
            continue;
         name = cbSiOQCSS->Caption;
      }
      else if(i == 10) {
         if(cbSiOQCLS->Checked == false)
            continue;
         name = cbSiOQCLS->Caption;
      }
      else if(i == 11) {
         if(cbSiOQCPad->Checked == false)
            continue;
         name = cbSiOQCPad->Caption;
      }
      else if(i == 12) {
         if(cbSiOQCW->Checked == false)
            continue;
         name = cbSiOQCW->Caption;
      }
      else if(i == 13) {
         if(cbNA->Checked == false)
            continue;
         name = cbNA->Caption;
      }
      fname = frnMain->SystemDirectory + "\\" + name + ".txt";

      pFile = fopen(fname.c_str(),"r");
      if(pFile) {
         rewind(pFile);
         while(!feof(pFile)) {
            if(fgets(buf,200,pFile) > 0) {
               msg = buf;
               len = msg.Length();
               pos = msg.Pos(" ");
               if(len > 0 && pos > 0) {
                  sid = msg.SubString(1,pos-1);
                  sname = msg.SubString(pos+1,len-pos);
                  sgBINCode->Cells[0][sgBINCode->RowCount-1] = IntToStr(no);
                  sgBINCode->Cells[1][sgBINCode->RowCount-1] = sid;
                  sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
                  sgBINCode->Cells[3][sgBINCode->RowCount-1] = sname;
                  sgBINCode->RowCount++;
                  no++;
               }
            }
         }
         fclose(pFile);
      }
   }
   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgBinCodeItemDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // �o��J�I�M�襤�椸��A���ݭn�B�z�A�u�� (State.Contains(gdFixed)) �~�}�l�i��B�z
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   // 2023 3 30 - chc RowColor: �bDrawText�e����, �_�hText�|�Q�\��
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // �̦�
      else
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00D1D1D1;
      ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
   }

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow == 0 || ACol == 0) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else if(ACol == 0 || ACol == 1 || ACol == 2) {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }

}
//---------------------------------------------------------------------------
TColor __fastcall TfrmRecipeSet::GetBinCodeColor(int rno)
{

   if((rno % 16) == 1)
      return clBlack;
   else if((rno % 16) == 2)
      return clMaroon;
   else if((rno % 16) == 3)
      return clGreen;
   else if((rno % 16) == 4)
      return clOlive;
   else if((rno % 16) == 5)
      return clNavy;
   else if((rno % 16) == 6)
      return clPurple;
   else if((rno % 16) == 7)
      return clTeal;
   else if((rno % 16) == 8)
      return clGray;
   else if((rno % 16) == 9)
      return clSilver;
   else if((rno % 16) == 10)
      return clRed;
   else if((rno % 16) == 11)
      return clLime;
   else if((rno % 16) == 12)
      return clYellow;
   else if((rno % 16) == 13)
      return clBlue;
   else if((rno % 16) == 14)
      return clFuchsia;
   else if((rno % 16) == 15)
      return clAqua;
   else if((rno % 16) == 0)
      return clWhite;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnGoodClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbGood->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnNAClick(TObject *Sender)
{
AnsiString fname;

   fname = frnMain->SystemDirectory + "\\" + cbNA->Caption + ".txt";
   BinCodeRead(fname);
}
//---------------------------------------------------------------------------
// 2023 4 1 - chc Create RecipeID List: combRecipe
void __fastcall TfrmRecipeSet::CreateRecipeID()
{
AnsiString fname,name,filename,fext;
int pos,len;

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
   combRecipe->Items->Clear();
   fname = frmRecipeSet->edRecipePath->Text + "\\*.ini";
   iAttributes |= faArchive;
   if(FindFirst(fname, iAttributes, sr) == 0) {
      do {
         if((sr.Attr & iAttributes) == sr.Attr) {                               // �ɦW�bsr.Name
            rno++;
            fext = ExtractFileExt(sr.Name);
            pos = sr.Name.Pos(fext);
            if(pos > 0) {
               filename = sr.Name.SubString(1,pos-1);
            }
            combRecipe->Items->Append(filename);
         }
      } while (FindNext(sr) == 0);
   }
   FindClose(sr);
   combRecipe->Text = "";

}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgDefectListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   if(Button == mbRight) {
      no = sgDefectList->Row;
      X += pcDefectList->Left;
      X += sgDefectList->Left;
      X += frmRecipeSet->Left;
      X += 24;
      Y += pcDefectList->Top;
      Y += sgDefectList->Top;
      Y += frmRecipeSet->Top;
      Y += 58;
      Y += 65;
      SetCursorPos(X, Y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      Sleep(100);
      mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
      Sleep(100);
      no = sgDefectList->Row;
   }
}
//---------------------------------------------------------------------------
// WaferDirection
void __fastcall TfrmRecipeSet::btnSaveWafeDegreeClick(TObject *Sender)
{
TIniFile *pSystemFile;
AnsiString recipefname,recipename;

   recipename = combRecipe->Text;
   recipefname = edRecipePath->Text + "\\" + recipename + ".ini";
   pSystemFile = new TIniFile(recipefname);

   // WaferDirection
   RecipeBufferEdit.WaferID.InLP1Degree = rgWaferInLP1Degree->ItemIndex;
   RecipeBufferEdit.WaferID.OutLP1Degree = rgWaferOutLP1Degree->ItemIndex;
   RecipeBufferEdit.WaferID.InLP2Degree = rgWaferInLP2Degree->ItemIndex;
   RecipeBufferEdit.WaferID.OutLP2Degree = rgWaferOutLP2Degree->ItemIndex;
   // Write
   pSystemFile->WriteInteger("WaferID","InLP1Degree"          ,RecipeBufferEdit.WaferID.InLP1Degree);
   pSystemFile->WriteInteger("WaferID","OutLP1Degree"         ,RecipeBufferEdit.WaferID.OutLP1Degree);
   pSystemFile->WriteInteger("WaferID","InLP2Degree"          ,RecipeBufferEdit.WaferID.InLP2Degree);
   pSystemFile->WriteInteger("WaferID","OutLP2Degree"         ,RecipeBufferEdit.WaferID.OutLP2Degree);

   pSystemFile->UpdateFile();

   delete pSystemFile;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnCognexRunClick(TObject *Sender)
{
AnsiString ProgramName;
AnsiString ProgramTitle;

   //ProgramTitle = "Cognex Wafer ID - 4.5.0";
   ProgramTitle = edCognexTitle->Text;
   ProgramName = edCognexPath->Text;
   if(!frnMain->CheckRunning(ProgramTitle)) {
      frnMain->WriteSystemLog("Cognex����: " + ProgramTitle + "," + ProgramName);
      frnMain->ExecuteProgram(P_NOWAIT,ProgramName);
   }
   else
      frnMain->WriteSystemLog("Cognex�����椤!");
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::btnSaveWaferInformationClick(
      TObject *Sender)
{

   btnSaveWafeIDClick(this);
   btnSaveWafeDegreeClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TfrmRecipeSet::sgDefectListClick(TObject *Sender)
{
int tno,no;

   tno = sgDefectList->RowCount - 2;
   no = sgDefectList->Row;
   if(tno > 0) {
      if(no >= 1 && no <= tno) {
         edSelectCol->Text = sgDefectList->Cells[4][no];
         edSelectRow->Text = sgDefectList->Cells[5][no];
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmRecipeSet::sgDefectListDblClick(TObject *Sender)
{
int tno,no;

   tno = sgDefectList->RowCount - 2;
   no = sgDefectList->Row;
   if(tno > 0) {
      if(no >= 1 && no <= tno) {
         pnlWorkCol->Caption = sgDefectList->Cells[4][no];
         pnlWorkRow->Caption = sgDefectList->Cells[5][no];

         int x,y;
         x = sgDefectList->Cells[2][no].ToInt();
         y = sgDefectList->Cells[3][no].ToInt();
         // �n�������I�첾���v: AdjustCenterDX
         x += AdjustCenterDX;
         y += AdjustCenterDY;
         frnMain->WriteSystemLog("Move: X,Y= " + IntToStr(x) + "," + IntToStr(y));
         pnlStageMoving->Visible = true;
         MoveToXY(x,y);
         UpdateMotionPosition();

         // 2023 5 6 - chc ���ݭn
         //UpdateCurrentPosition();

         pnlStageMoving->Visible = false;
      }
   }
}
//---------------------------------------------------------------------------
// 2023 5 6 - chc ��s��m
void __fastcall TfrmRecipeSet::tm250msTimer(TObject *Sender)
{

   tm250ms->Enabled = false;

   if(frmRecipeSet->Visible == true) {
      UpdateMotionPosition();
   }

   tm250ms->Enabled = true;
}
//---------------------------------------------------------------------------
// 2023 5 6 - chc Position Move
void __fastcall TfrmRecipeSet::shCurretnPositionMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int no,left,top,x,y;

   left = shCurretnPosition->Left;
   top = shCurretnPosition->Top;
   x = shCurretnPosition->Left - imWaferMap->Left + X;
   y = shCurretnPosition->Top - imWaferMap->Top + Y;
   imWaferMapMouseMove(this,Shift,x,y);
}
//---------------------------------------------------------------------------
// 2023 5 9 - chc KeyDown
void __fastcall TfrmRecipeSet::combRecipeKeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{

   if(combRecipe->Items->IndexOf(combRecipe->Text) == -1 && Key == 13) {
      // �NFocus��O���a��: �|�I�sOnExit
      pnlSlotTitle->SetFocus();
   }
}
//---------------------------------------------------------------------------
// 2023 5 9 - chc KeyDown
void __fastcall TfrmRecipeSet::combRecipeExit(TObject *Sender)
{
AnsiString fname;

   if(combRecipe->Items->IndexOf(combRecipe->Text) == -1 && combRecipe->Text != "") {
      // �ƻs�@���s��Recipe
      if(Application->MessageBox(("Sure to Add new Recipe: [" + combRecipe->Text + "]?").c_str(), " Recipe Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         combRecipe->Text = "";
         combRecipe->ItemIndex = -1;
         return;
      }

      combRecipe->Items->Append(combRecipe->Text);
      /*
      if(WriteRecipe() == false) {
         goto notupdate;
      }
      // �ƻs��Ƨ�: pnlRecipeTypeRecipe to combRecipe->Text
      CopyRecipeFile();
      // �n�ܧ�pnlRecipeTypeRecipe
      pnlRecipeTypeRecipe->Caption = combRecipe->Text;

      // Create recipe
      edSECSRecipeID->Text = cbRecipe->Text;
      rgSECSRecipe->ItemIndex = RECIPE_CREATE;
      WriteSystemLog("boolSendSCESCommand: 3");
      rgSecsCommandSet->ItemIndex = -1;
      boolSendSCESCommand = true;
      rgSecsCommandSet->ItemIndex = SECS_CMD_RCPM - SECS_CMD_STAT_1;
      */
   }
}
//---------------------------------------------------------------------------

