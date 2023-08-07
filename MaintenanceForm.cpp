//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MaintenanceForm.h"
#include "frmMain.h"
#include "InspectionForm.h"
#include "RecipeForm.h"

void __fastcall TestSignalTower();
void __fastcall WriteDO(int idno);
extern I32 IO3_ID;
extern I32 IO1_ID;
extern AnsiString RobotCommandCode;
extern bool boolRobotAck;
extern bool boolRobotComplete;
extern bool boolRobotNak;
extern int RobotCmd;
#define ROBOT_CMD_NONE          0
#define ROBOT_CMD_LR            1
#define ROBOT_CMD_GP            2
#define ROBOT_CMD_LS            3
#define ROBOT_CMD_VACUUM        4
// Tazmo
#define ROBOT_CMD_RST           5
#define ROBOT_CMD_GET           6
#define ROBOT_CMD_PUT           7
#define ROBOT_CMD_MTP           8
// 記錄取退片中
extern bool boolInLoad;
extern bool boolInUnLoad;
extern int WafeFromFOUP;
void __fastcall WriteDO(int idno);
#define WAFER_TOP                0
#define WAFER_BACK               1
extern int RobotCmdState;
#define ROBOT_CMD_STATE_NONE            0
#define ROBOT_CMD_STATE_SENT            1                                       // 4401
#define ROBOT_CMD_STATE_DONE            2                                       // 0601
#define ROBOT_CMD_STATE_TIMEOUT         3
// 已收到4401
#define ROBOT_CMD_STATE_4401_LS         4                                       // 4401
// 已收到0601
#define ROBOT_CMD_STATE_0601_LS         5                                       // 0601

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "LMDButton"
#pragma link "LMDCustomButton"
#pragma link "LMDControl"
#pragma link "LMDCustomBevelPanel"
#pragma link "LMDCustomButtonGroup"
#pragma link "LMDCustomControl"
#pragma link "LMDCustomGroupBox"
#pragma link "LMDCustomPanel"
#pragma link "LMDCustomParentPanel"
#pragma link "LMDCustomRadioGroup"
#pragma link "LMDCustomTrackBar"
#pragma link "LMDRadioGroup"
#pragma link "LMDTrackBar"
#pragma link "LMDBaseControl"
#pragma link "LMDBaseGraphicControl"
#pragma link "LMDBaseMeter"
#pragma link "LMDCustomProgress"
#pragma link "LMDGraphicControl"
#pragma link "LMDGroupBox"
#pragma link "LMDProgress"
#pragma link "LMDBaseEdit"
#pragma link "LMDCustomEdit"
#pragma link "LMDEdit"
#pragma resource "*.dfm"
TfrmMaintenance *frmMaintenance;
//---------------------------------------------------------------------------
__fastcall TfrmMaintenance::TfrmMaintenance(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnLightTestClick(TObject *Sender)
{

   TestSignalTower();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::cbID5Bit11Click(TObject *Sender)
{

   //frnMain->cbID5Bit11->Checked = cbID5Bit11->Checked;
   //WriteDO(IO3_ID);
   frnMain->cbBit11->Checked = cbID5Bit11->Checked;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::cbID5Bit12Click(TObject *Sender)
{

   //frnMain->cbID5Bit12->Checked = cbID5Bit12->Checked;
   //WriteDO(IO3_ID);
   frnMain->cbBit12->Checked = cbID5Bit12->Checked;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::cbID5Bit13Click(TObject *Sender)
{

   //frnMain->cbID5Bit13->Checked = cbID5Bit13->Checked;
   //WriteDO(IO3_ID);
   frnMain->cbBit13->Checked = cbID5Bit13->Checked;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::cbID5Bit14Click(TObject *Sender)
{

   //frnMain->cbID5Bit14->Checked = cbID5Bit14->Checked;
   //WriteDO(IO3_ID);
   frnMain->cbBit14->Checked = cbID5Bit14->Checked;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::cbID5Bit15Click(TObject *Sender)
{

   //frnMain->cbID5Bit15->Checked = cbID5Bit15->Checked;
   //WriteDO(IO3_ID);
   frnMain->cbBit15->Checked = cbID5Bit15->Checked;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
bool boolStop = false;
void __fastcall TfrmMaintenance::btnInnerLoopClick(TObject *Sender)
{
int no;

   btnStop->Enabled = true;
   btnInnerLoop->Enabled = false;
   boolStop = false;
   no = edLoopNo->Text.ToInt();
   pnlLoopNo->Caption = "0";
   for(int i=0 ; i<no ; i++){
      btnMacroInnerPositionClick(this);
      if(pnlMacroInnerPosition->Color != clLime)
         break;
      Sleep(500);
      btnMacroInnerStopClick(this);
      if(pnlMacroInnerStop->Color != clLime)
         break;
      pnlLoopNo->Caption = IntToStr(i+1);;
      if(boolStop == true)
         break;
   }
   btnInnerLoop->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerPositionClick(
      TObject *Sender)
{

   pnlMacroInnerPosition->Color = clSilver;
   frnMain->btnMacroInnerPositionClick(this);
   pnlMacroInnerPosition->Color = frnMain->pnlMacroInnerPosition->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerStopClick(TObject *Sender)
{

   pnlMacroInnerStop->Color = clSilver;
   frnMain->btnMacroInnerStopClick(this);
   pnlMacroInnerStop->Color = frnMain->pnlMacroInnerStop->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnOuterLoopClick(TObject *Sender)
{
int no;

   btnStop->Enabled = true;
   btnOuterLoop->Enabled = false;
   boolStop = false;
   no = edLoopNo->Text.ToInt();
   pnlLoopNo->Caption = "0";
   for(int i=0 ; i<no ; i++){
      btnMacroOuterPositionClick(this);
      if(pnlMacroOuterPosition->Color != clLime)
         break;
      Sleep(500);
      btnMacroOuterStopClick(this);
      if(pnlMacroOuterStop->Color != clLime)
         break;
      pnlLoopNo->Caption = IntToStr(i+1);;
      if(boolStop == true)
         break;
   }
   btnOuterLoop->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterPositionClick(
      TObject *Sender)
{

   pnlMacroOuterPosition->Color = clSilver;
   frnMain->btnMacroOuterPositionClick(this);
   pnlMacroOuterPosition->Color = frnMain->pnlMacroOuterPosition->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterStopClick(TObject *Sender)
{

   pnlMacroOuterStop->Color = clSilver;
   frnMain->btnMacroOuterStopClick(this);
   pnlMacroOuterStop->Color = frnMain->pnlMacroOuterStop->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::imCloseClick(TObject *Sender)
{

   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroHomeClick(TObject *Sender)
{

   pnlMacroHome->Color = clSilver;
   frnMain->btnMacroHomeClick(this);
   pnlMacroHome->Color = frnMain->pnlMacroHome->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroRobotPositionClick(
      TObject *Sender)
{

   pnlMacroRobotPosition->Color = clSilver;
   frnMain->btnMacroRobotPositionClick(this);
   pnlMacroRobotPosition->Color = frnMain->pnlMacroRobotPosition->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYMoveClick(TObject *Sender)
{

   //pnlMacroInnerYMove->Color = clSilver;
   //frnMain->btnMacroInnerYMoveClick(this);
   //pnlMacroInnerYMove->Color = frnMain->pnlMacroInnerYMove->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXBackClick(TObject *Sender)
{

   //pnlMacroInnerXBack->Color = clSilver;
   //frnMain->btnMacroInnerXBackClick(this);
   //pnlMacroInnerXBack->Color = frnMain->pnlMacroInnerXBack->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTMoveClick(TObject *Sender)
{

   //pnlMacroInnerTMove->Color = clSilver;
   //frnMain->btnMacroInnerTMoveClick(this);
   //pnlMacroInnerTMove->Color = frnMain->pnlMacroInnerTMove->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCCWClick(TObject *Sender)
{

   //pnlMacroOuterTCCW->Color = clSilver;
   //frnMain->btnMacroOuterTCCWClick(this);
   //pnlMacroOuterTCCW->Color = frnMain->pnlMacroOuterTCCW->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroReadyPositionClick(
      TObject *Sender)
{

   pnlMacroReadyPosition->Color = clSilver;
   frnMain->btnMacroReadyPositionClick(this);
   pnlMacroReadyPosition->Color = frnMain->pnlMacroReadyPosition->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYBackClick(TObject *Sender)
{

   //pnlMacroInnerYBack->Color = clSilver;
   //frnMain->btnMacroInnerYBackClick(this);
   //pnlMacroInnerYBack->Color = frnMain->pnlMacroInnerYBack->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXMoveClick(TObject *Sender)
{

   //pnlMacroInnerXMove->Color = clSilver;
   //frnMain->btnMacroInnerXMoveClick(this);
   //pnlMacroInnerXMove->Color = frnMain->pnlMacroInnerXMove->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTBackClick(TObject *Sender)
{

   //pnlMacroInnerTBack->Color = clSilver;
   //frnMain->btnMacroInnerTBackClick(this);
   //pnlMacroInnerTBack->Color = frnMain->pnlMacroInnerTBack->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCWClick(TObject *Sender)
{

   //pnlMacroOuterTCW->Color = clSilver;
   //frnMain->btnMacroOuterTCWClick(this);
   //pnlMacroOuterTCW->Color = frnMain->pnlMacroOuterTCW->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroXYHomeClick(TObject *Sender)
{

   pnlMacroXYHome->Color = clSilver;
   frnMain->btnMacroXYHomeClick(this);
   pnlMacroXYHome->Color = frnMain->pnlMacroXYHome->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroZ1DownClick(TObject *Sender)
{

   pnlMacroZ1Down->Color = clSilver;
   frnMain->btnMacroZ1DownClick(this);
   pnlMacroZ1Down->Color = frnMain->pnlMacroZ1Down->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroT1ResetClick(TObject *Sender)
{

   pnlMacroT1Reset->Color = clSilver;
   frnMain->btnMacroT1ResetClick(this);
   pnlMacroT1Reset->Color = frnMain->pnlMacroT1Reset->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroZ2UpClick(TObject *Sender)
{

   pnlMacroZ2Up->Color = clSilver;
   frnMain->btnMacroZ2UpClick(this);
   pnlMacroZ2Up->Color = frnMain->pnlMacroZ2Up->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroZ2HomeClick(TObject *Sender)
{

   pnlMacroZ2Home->Color = clSilver;
   frnMain->btnMacroZ2HomeClick(this);
   pnlMacroZ2Home->Color = frnMain->pnlMacroZ2Home->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroZ2DownClick(TObject *Sender)
{

   pnlMacroZ2Down->Color = clSilver;
   frnMain->btnMacroZ2DownClick(this);
   pnlMacroZ2Down->Color = frnMain->pnlMacroZ2Down->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroZ1UpClick(TObject *Sender)
{

   pnlMacroZ1Up->Color = clSilver;
   frnMain->btnMacroZ1UpClick(this);
   pnlMacroZ1Up->Color = frnMain->pnlMacroZ1Up->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroDemoClick(TObject *Sender)
{
bool boolmove;

   boolmove = frnMain->cbMacroDemoMove->Checked;

   frnMain->cbMacroDemoMove->Checked = true;
   frnMain->btnMacroDemoClick(this);
   frnMain->cbMacroDemoMove->Checked = boolmove;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnStopClick(TObject *Sender)
{

   boolStop = true;
   btnStop->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerYMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerYMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerXBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerXBackMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerTMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerTMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCCWMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroOuterTCCWMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCCWMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroOuterTCCWMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCCWMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCCWMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerYBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerYBackMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerYBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerXMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerXMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerXMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerTBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerTBackMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroInnerTBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCWMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCWMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCWMouseExit(TObject *Sender)
{

   frnMain->btnMacroOuterTCWMouseExit(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMacroOuterTCWMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCWMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::rgBXFMLensChange(TObject *Sender,
      int ButtonIndex)
{

   frnMain->rgBXFMLens->ItemIndex = rgBXFMLens->ItemIndex;
   frmInspection->rgBXFMLens->ItemIndex = rgBXFMLens->ItemIndex;
   frnMain->rgBXFMLensChange(Sender, ButtonIndex);
   pnlBXFMLens->Color = frnMain->pnlBXFMLens->Color;
   frmInspection->pnlBXFMLens->Color = frnMain->pnlBXFMLens->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::tbBXFMLMPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->tbBXFMLMP->Position = tbBXFMLMP->Position;
   frmInspection->tbBXFMLMP->Position = tbBXFMLMP->Position;
   frnMain->tbBXFMLMPMouseUp(Sender,Button,Shift,X,Y);
   pnlBXFMLMPValue->Caption =  frnMain->pnlBXFMLMPValue->Caption;
   pnlBXFMLampSet->Color =  frnMain->pnlBXFMLampSet->Color;
   frmInspection->pnlBXFMLMPValue->Caption =  frnMain->pnlBXFMLMPValue->Caption;
   frmInspection->pnlBXFMLampSet->Color =  frnMain->pnlBXFMLampSet->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::rgBXFMLMPSwitchChange(TObject *Sender,
      int ButtonIndex)
{

   frnMain->rgBXFMLMPSwitch->ItemIndex = rgBXFMLMPSwitch->ItemIndex;
   frnMain->rgBXFMLMPSwitchChange(Sender,ButtonIndex);
   pnlBXFMLMPSwitch->Color = frnMain->pnlBXFMLMPSwitch->Color;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::FormShow(TObject *Sender)
{

   rgBXFMLens->ItemIndex = frnMain->rgBXFMLens->ItemIndex;
   tbBXFMLMP->Position = frnMain->tbBXFMLMP->Position;
   rgBXFMLMPSwitch->ItemIndex = frnMain->rgBXFMLMPSwitch->ItemIndex;
   rgMoveStep->ItemIndex = frnMain->rgMoveStep->ItemIndex;
   frnMain->cbContinueMove1->Checked = false;

   // alarm
   if(frnMain->AlarmStatus.boolLPLight == true) {
      tsMacro->TabVisible = false;
      tsEFEM->TabVisible = false;
   }
   else {
      tsMacro->TabVisible = true;
      tsEFEM->TabVisible = true;
   }

}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardX1Click(TObject *Sender)
{

   frnMain->btnBackwardXClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardX1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardXMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardX1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardXMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardY1Click(TObject *Sender)
{

   frnMain->btnBackwardYClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardY1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardYMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardY1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardYMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveY1Click(TObject *Sender)
{

   frnMain->btnMoveYClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveY1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveYMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveY1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveYMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveX1Click(TObject *Sender)
{

   frnMain->btnMoveXClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveX1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveXMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveX1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveXMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardZ1Click(TObject *Sender)
{

   frnMain->btnBackwardZClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardZ1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardZMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardZ1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardZMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveZ1Click(TObject *Sender)
{

   frnMain->btnMoveZClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveZ1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveZMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveZ1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveZMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCW1xClick(TObject *Sender)
{

   frnMain->btnBackwardTClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardTMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardTMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveR1Click(TObject *Sender)
{

   frnMain->btnMoveRClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveR1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveRMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnMoveR1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveRMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardR1Click(TObject *Sender)
{

   frnMain->btnBackwardRClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardR1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardRMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnBackwardR1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnBackwardRMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCCW1xClick(TObject *Sender)
{

   frnMain->btnMoveTClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveTMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::btnCCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMoveTMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMaintenance::rgMoveStepClick(TObject *Sender)
{

   frnMain->rgMoveStep->ItemIndex = rgMoveStep->ItemIndex;
}
//---------------------------------------------------------------------------
// Loop Test
void __fastcall TfrmMaintenance::btnLoopTestClick(TObject *Sender)
{

   btnLoopTestStop->Visible = true;
   btnLoopTest->Enabled = false;

   frnMain->edRobotABScanSpeed->Text = edRobotABScanSpeed->Text;
   frnMain->cbChuckScan->Checked = cbChuckScan->Checked;
   frnMain->cbDoubleWafer->Checked = cbDoubleWafer->Checked;
   frnMain->cbAlignerAlignment->Checked = cbAlignerAlignment->Checked;
   frnMain->cbChuckDemo->Checked = cbChuckDemo->Checked;
   frnMain->cbUnloadNotch->Checked = cbUnloadNotch->Checked;
   frnMain->edLoopTestCount->Text = edLoopTestCount->Text;
   frnMain->cbLoopPause->Checked = cbLoopPause->Checked;
   frnMain->cbPause->Checked = cbPause->Checked;
   frnMain->rgLoopTest->ItemIndex = rgLoopTest->ItemIndex;

   frnMain->btnLoopTestClick(this);
   frnMain->pnlLoopTest->Color = pnlLoopTest->Color;

   btnLoopTestStop->Visible = false;
   btnLoopTest->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::cbPauseClick(TObject *Sender)
{

   frnMain->cbPause->Checked = cbPause->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnLoopTestStopClick(TObject *Sender)
{

   frnMain->btnLoopTestStopClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::combRobotComportChange(TObject *Sender)
{

   frnMain->InitRobotCom();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::combRobotBaudrateChange(TObject *Sender)
{

   frnMain->InitRobotCom();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotConnectClick(TObject *Sender)
{

   try {
      frnMain->ybRobot->SettingsDialog(this,true);
      if(frnMain->boolRobotStatus == false)
         frnMain->ybRobot->Active = true;
      frnMain->boolRobotStatus = true;
      RobotButton();
   }
   catch(Exception &e) {
      ShowMessage("ybRobot: " + e.Message);
      frnMain->boolRobotStatus = false;
      RobotButton();
      pnlMaintenanceMessage->Caption = "COM Port(Robot) 啟動失敗!";
      frnMain->WriteSystemLog(pnlMaintenanceMessage->Caption);
   }
   UpdateRobotStatus();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotCMDClick(TObject *Sender)
{
unsigned char command[50],buf[50];
AnsiString msg;
int len;

   if(frnMain->boolRobotStatus == false) {
      pnlRobotMessage->Caption = "未連線!";
      frnMain->WriteSystemLog("btnRobotCMDClick: Robot尚未連線!");
      return;
   }

   boolRobotAck = false;
   boolRobotComplete = false;
   boolRobotNak = false;

   sprintf(command,"%s",edRobotCmd->Text);
   sprintf(buf,"%s%c",command,0x0d);
   len = strlen(buf);

   RobotCommandCode = "";
   if(edRobotCmd->Text.Length() >= 3)
      RobotCommandCode = edRobotCmd->Text.SubString(1,3);
   frnMain->WriteSystemLog("btnRobotCMDClick: run...");
   frnMain->ybRobot->Write(buf,len);
   pnlRobotOperation->Caption = "Busy";
   pnlRobot->Enabled = false;
   btnRobotEnable->Visible = true;

   msg.sprintf("CMD: %s",command);
   pnlMaintenanceMessage->Caption = msg;
   frnMain->WriteSystemLog(msg);

   AddRobotSendMessage(edRobotCmd->Text);
   pnlRobotMessage->Caption = msg;
   frnMain->WriteSystemLog("btnRobotCMDClick: edRobotCmd= " + edRobotCmd->Text);
}
//---------------------------------------------------------------------------
// boolRobotStatus = false;
void __fastcall TfrmMaintenance::RobotButton()
{

   btnRobotCMD->Enabled = frnMain->boolRobotStatus;
   btnRobotOriginal->Enabled = frnMain->boolRobotStatus;
   btnRobotPos->Enabled = frnMain->boolRobotStatus;
   cbRobot->Enabled = frnMain->boolRobotStatus;
   rgWaferLocation->Enabled = frnMain->boolRobotStatus;
   btnRobotGet->Enabled = frnMain->boolRobotStatus;
   btnRobotPut->Enabled = frnMain->boolRobotStatus;
   btnRobotVacuumOn->Enabled = frnMain->boolRobotStatus;
   btnRobotVacuumOff->Enabled = frnMain->boolRobotStatus;
   btnRobotStas->Enabled = frnMain->boolRobotStatus;
   btnRobotWithWafer->Enabled = frnMain->boolRobotStatus;
   btnRobotGetPosition->Enabled = frnMain->boolRobotStatus;
   btnRobotClearAlarm->Enabled = frnMain->boolRobotStatus;
   btnRobotEanble->Enabled = frnMain->boolRobotStatus;
   btnRobotDisable->Enabled = frnMain->boolRobotStatus;
}
//---------------------------------------------------------------------------
// Robot Active
void __fastcall TfrmMaintenance::UpdateRobotStatus()
{

   if(frnMain->boolRobotStatus == true) {
      pnlRobotStatus->Color = clLime;
      frnMain->pnlRobotReady->Color = clLime;
   }
   else {
      pnlRobotStatus->Color = clRed;
      frnMain->pnlRobotReady->Color = clRed;
      if(pnlRobotOriginal->Color != clLime)
         frnMain->WriteSystemLog("#UpdateRobotStatus(): boolRobotStatus為false!");
   }
}
//---------------------------------------------------------------------------
// Robot Send
void __fastcall TfrmMaintenance::AddRobotSendMessage(AnsiString msg)
{
AnsiString putmsg;

   if(mmRobotMsg->Lines->Count > 400) {
      for(int i=320 ; i<mmRobotMsg->Lines->Count ; i++)
         mmRobotMsg->Lines->Delete(i);
   }

   putmsg = Now().FormatString("nn':'ss':'zzz' '> ") + msg;
   mmRobotMsg->Lines->Insert(0,putmsg);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotOriginalClick(TObject *Sender)
{
bool ret;
bool first = true;

   pnlRobotOriginal->Color = clSilver;
again:
   RobotCmd = ROBOT_CMD_RST;
   frnMain->WriteSystemLog("RobotCmd-16 = ROBOT_CMD_RST");
   edRobotCmd->Text = "RST";
   // 要Refresh
   edRobotCmd->Refresh();
   btnRobotCMDClick(this);
   ret = frnMain->WaitTMRobotDone(30000);
   if(ret == true)
      pnlRobotOriginal->Color = clLime;
   else
      pnlRobotOriginal->Color = clRed;

   // Again
   if(boolRobotNak == true && first == true) {
      first = false;
      frnMain->WriteSystemLog("RobotCmd-16 = ROBOT_CMD_RST Again");
      frnMain->WaitTime(1000);
      goto again;
   }
   return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotPosClick(TObject *Sender)
{
int pos;
bool ret;
int no,slot,arm;
AnsiString str;

   pnlRobotOriginal->Color = clSilver;
   no = edRobotPos->Text.ToInt();
   if(no == 1 || no == 2 || no == 11 || no == 12 || no == 21 || no == 22 || no == 23 || no == 31 || no == 32 || no == 33) {
      if(no == 1 || no == 11 || no == 21 || no == 22 || no == 23)
         arm = 1;
      else
         arm = 2;
      slot = 1;
      if(no == 1 || no == 2)
         slot = combRobotTrayNo->Text.ToInt();
      str.sprintf("MTP,%d,%d,%d",no,slot,arm);
      edRobotCmd->Text = str;
      // 要Refresh
      edRobotCmd->Refresh();
      btnRobotCMDClick(this);
      ret = frnMain->WaitTMRobotDone(30000);
      if(ret == true)
         pnlRobotOriginal->Color = clLime;
      else
         pnlRobotOriginal->Color = clRed;
   }
   else
      pnlRobotOriginal->Color = clRed;
   return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotGetClick(TObject *Sender)
{
bool ret;
int no,zvalue,waferno;
AnsiString msg,offsetstr,name,waferid;
TPanel *panel,*panela;
int slotno,foupno,stageno,cnt,timeout;

   pnlRobotGet->Color = clSilver;

   // 確認Robot無片
   if(boolInLoad != true && boolInUnLoad != true) {
      // Robot是否有片?
      btnRobotWithWaferClick(this);
      if(pnlRobotWithWafer->Color == clLime || pnlRobotWithWafer->Color == clRed) {
         frnMain->pnlLoadWafer->Color = clRed;
         pnlRobotMessage->Caption = "Get失敗: Robot上有Wafer!";
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         pnlRobotGet->Color = clRed;
         return;
      }
      frnMain->WriteSystemLog("Get: Robot無片 Ok");
   }

   // 2022 9 6 - chc 若是在取退片呼叫, 則不用檢查
   if(boolInLoad == true || boolInUnLoad == true)
      goto ready;

   // 確認LP1是否已Load?
   if(rgWaferLocation->ItemIndex == LOCATION_LP1) {
      frnMain->WriteSystemLog("確認LP1是否Ready?");
      btnLoadPortStasClick(this);
      ret = frnMain->WaitLoadPortDone(5000);
      if(ret == false) {
         pnlRobotMessage->Caption = "LoadPort1: Timeout!";
         pnlRobotGet->Color = clRed;
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         return;
      }
      if(frnMain->pnlLoadPortS3->Caption != "2") {
         pnlRobotMessage->Caption = "LoadPort1: 動作中!";
         pnlRobotGet->Color = clRed;
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         return;
      }
      frnMain->WriteSystemLog("確認LP1已Ready.");
   }
   // 確認LP2是否已Load?
   else if(rgWaferLocation->ItemIndex == LOCATION_LP2) {
      frnMain->WriteSystemLog("確認LP2是否Ready?");
      btnLoadPort2StasClick(this);
      ret = WaitLoadPort2Done(5000);
      if(ret == false) {
         pnlRobotMessage->Caption = "LoadPort2: Timeout!";
         pnlRobotGet->Color = clRed;
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         return;
      }
      if(frnMain->pnlLoadPort2S3->Caption != "2") {
         pnlRobotMessage->Caption = "LoadPort2: 動作中!";
         pnlRobotGet->Color = clRed;
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         return;
      }
      frnMain->WriteSystemLog("確認LP2已Ready.");
   }

ready:
   // 2022 7 27 - chc 檢查W
   if(frnMain->cbPause->Checked == true) {
      if(Application->MessageBox("移動Robot軸(取片)     ", "取片作業", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         pnlRobotGet->Color = clRed;
         return;
      }
   }

   if(frnMain->GoRobotWAxis(0) == false) {
      pnlRobotGet->Color = clRed;
      frnMain->pnlAlarmMessage->Caption = "W軸(取片)移動失敗!";
      pnlMaintenanceMessage->Caption = frnMain->pnlAlarmMessage->Caption;
      frnMain->AddMainLog("WAxis(Get)Move Error!", RAXIS_GET_ERR,true);
      return;
   }
   frnMain->WriteSystemLog("W軸到位.");

   frnMain->ChangeWaferPosition(rgWaferLocation->ItemIndex,WAFER_GET,1);
   waferno = combRobotTrayNo->Text.ToInt();

   if(rgWaferLocation->ItemIndex == LOCATION_LP1) {
      no = 1;
      name = "Cassette";
      WafeFromFOUP = 1;
      rgCST->ItemIndex = WafeFromFOUP - 1;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_LP2) {
      no = 2;
      name = "Cassette";
      WafeFromFOUP = 2;
      rgCST->ItemIndex = WafeFromFOUP - 1;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_ALIGNER1) {
      no = 11;
      name = "Aligner";
      waferno = 1;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_ALIGNER2) {
      no = 12;
      name = "Aligner";
      waferno = 1;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_CHUCK) {
      if(rgCST->ItemIndex == LOADPORT_1)
         no = 23;
      else
         no = 33;
      name = "Chuck";
      waferno = 1;
      // 到入料點
      frnMain->btnToLoadPositionClick(this);
      if(frnMain->pnlToLoadPosition->Color != clLime) {
         pnlRobotGet->Color = clRed;
         frnMain->pnlAlarmMessage->Caption = "Stage(入料點)移動失敗!";
         pnlMaintenanceMessage->Caption = frnMain->pnlAlarmMessage->Caption;
         frnMain->AddMainLog("Stage Move Error!", STAGE_MOVE_ERR,true);
         return;
      }
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_MACRO) {
      if(WafeFromFOUP == 1 && rgSide->ItemIndex == WAFER_TOP)
         no = 21;
      else if(WafeFromFOUP == 1 && rgSide->ItemIndex == WAFER_BACK)
         no = 22;
      else if(WafeFromFOUP == 2 && rgSide->ItemIndex == WAFER_TOP)
         no = 31;
      else if(WafeFromFOUP == 2 && rgSide->ItemIndex == WAFER_BACK)
         no = 32;
      name = "Macro";
      waferno = 1;
   }
   // 點位,Slot,Arm
   msg.sprintf("MTP,%d,%d,%d",no,waferno,WafeFromFOUP);

   edRobotCmd->Text = msg;
   // ==> 1
   // 2020 4 23 - chc Log
   frnMain->WriteSystemLog("Robot取片1: " + msg);

   if(frnMain->cbPause->Checked == true) {
      if(Application->MessageBox(("Robot-1準備由" + name + "取第" + IntToStr(waferno) + "片(MTP)? " + msg).c_str(), "取片作業", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         pnlRobotGet->Color = clRed;
         return;
      }
   }

   // 2020 4 27 - chc 要先設定狀態
   RobotCmdState = ROBOT_CMD_STATE_NONE;

   // 2020 5 6 - chc Log
   frnMain->WriteSystemLog("RobotCmdState-10 = ROBOT_CMD_STATE_NONE");

   // 2022 12 20 - chc Tazmo
   //RobotCmd = ROBOT_CMD_GP;
   RobotCmd = ROBOT_CMD_MTP;

   // 2020 5 6 - chc Log
   frnMain->WriteSystemLog("RobotCmd-10 = ROBOT_CMD_GP");

   btnRobotCMDClick(this);

   // Tazmo
   ret = WaitTMRobotDone(10000);
   if(ret == false) {
      pnlRobotMessage->Caption = IntToStr(no) + ": Timeout!";
      pnlRobotGet->Color = clRed;
      return;
   }

   frnMain->ChangeWaferPosition(rgWaferLocation->ItemIndex,WAFER_GET,2);
   // Vacuum off
   if(frnMain->rgWaferLocation->ItemIndex == LOCATION_MACRO) {
      frnMain->WriteSystemLog("RobotCmd-12 = 內外環真空Off...");
      frnMain->MacroVacuum(false);
      frnMain->WriteSystemLog("RobotCmd-12 = 內外環真空Off.");
   }
   // 放真空
   else if(rgWaferLocation->ItemIndex == LOCATION_CHUCK) {
      frnMain->WriteSystemLog("RobotCmd-12 = Chuck真空關...");
      // Chuck放真空
      frnMain->ChuckVacuum(false);
      frnMain->WriteSystemLog("RobotCmd-12 = Chuck真空關.");
   }
   // 點位,Slot,Arm
   msg.sprintf("GET,%d,%d,%d",no,waferno,WafeFromFOUP);
   edRobotCmd->Text = msg;
   frnMain->WriteSystemLog("Robot取片1a: " + msg);
   if(cbPause->Checked == true) {
      if(Application->MessageBox(("Robot-1準備由" + name + "取第" + IntToStr(waferno) + "片(GET)? " + msg).c_str(), "取片作業", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
         pnlRobotGet->Color = clRed;
         return;
      }
   }
   // 要先設定狀態
   RobotCmdState = ROBOT_CMD_STATE_NONE;
   frnMain->WriteSystemLog("RobotCmdState-10 = ROBOT_CMD_STATE_NONE");
   RobotCmd = ROBOT_CMD_GET;
   frnMain->WriteSystemLog("RobotCmd-10 = ROBOT_CMD_GET");
   btnRobotCMDClick(this);
   ret = WaitTMRobotDone(10000);
   if(ret == false) {
      pnlRobotMessage->Caption = IntToStr(no) + ": Timeout!";
      pnlRobotGet->Color = clRed;
      return;
   }

   if(rgWaferLocation->ItemIndex == LOCATION_ALIGNER1) {
      frnMain->pnlWaferAlignerInformation->Color = (TColor)0x0068C6AA;
      frmRecipe->pnlWaferAlignerInformation->Color = frnMain->pnlWaferAlignerInformation->Color;
      frnMain->pnlWaferAlignerInformation->Caption = "";
      stageno = STAGE_ALIGNER;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_LP1) {
      panel->Color = (TColor)0x0068C6AA;
      panela->Color = (TColor)0x0068C6AA;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_LP2) {
      panel->Color = (TColor)0x0068C6AA;
      panela->Color = (TColor)0x0068C6AA;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_CHUCK) {
      frnMain->pnlWaferChuckInformation->Color = (TColor)0x0068C6AA;
      frmRecipe->pnlWaferChuckInformation->Color = frnMain->pnlWaferChuckInformation->Color;
      stageno = STAGE_CHUCK;
      frnMain->pnlWaferChuckInformation->Caption = "";
      frmRecipe->pnlWaferChuckInformation->Caption = frnMain->pnlWaferChuckInformation->Caption;
   }
   else if(rgWaferLocation->ItemIndex == LOCATION_MACRO) {
      frnMain->pnlWaferMacroInformation->Color = (TColor)0x0068C6AA;
      frmRecipe->pnlWaferMacroInformation->Color = frnMain->pnlWaferMacroInformation->Color;
      stageno = STAGE_MACRO;
      frnMain->pnlWaferMacroInformation->Caption = "";
      frmRecipe->pnlWaferMacroInformation->Caption = frnMain->pnlWaferMacroInformation->Caption;
   }

   // 2022 12 20 - chc Tazmo
   else if(rgWaferLocation->ItemIndex == LOCATION_ALIGNER2) {
      frnMain->pnlWaferAlignerInformation->Color = (TColor)0x0068C6AA;
      frmRecipe->pnlWaferAlignerInformation->Color = frnMain->pnlWaferAlignerInformation->Color;
      frnMain->pnlWaferAlignerInformation->Caption = "";
      stageno = STAGE_ALIGNER;
   }

   // 2022 8 23 - chc LP1/LP2不用轉換
   if(rgWaferLocation->ItemIndex != LOCATION_LP1 && rgWaferLocation->ItemIndex != LOCATION_LP2) {
      frnMain->StageInformation[stageno].boolWithWafer = false;
      frnMain->StageInformation[STAGE_ROBOT].boolWithWafer = true;
      frnMain->StageInformation[STAGE_ROBOT].WaferID = frnMain->StageInformation[stageno].WaferID;
      frnMain->StageInformation[STAGE_ROBOT].SlotNo = frnMain->StageInformation[stageno].SlotNo;
      frnMain->StageInformation[STAGE_ROBOT].FoupNo = frnMain->StageInformation[stageno].FoupNo;
      frnMain->pnlWaferRobotInformation->Caption = IntToStr(frnMain->StageInformation[STAGE_ROBOT].FoupNo) + "," + IntToStr(frnMain->StageInformation[STAGE_ROBOT].SlotNo) + " " + frnMain->StageInformation[STAGE_ROBOT].WaferID;
      frmRecipe->pnlWaferRobotInformation->Caption = frnMain->pnlWaferRobotInformation->Caption;
   }
   else {
      frnMain->StageInformation[STAGE_ROBOT].boolWithWafer = true;
      frnMain->StageInformation[STAGE_ROBOT].WaferID = "---";
      frnMain->StageInformation[STAGE_ROBOT].SlotNo = 25 - frnMain->rgWaferNo->ItemIndex;
      frnMain->StageInformation[STAGE_ROBOT].FoupNo = frnMain->rgFoupSource->ItemIndex + 1;
      frnMain->pnlWaferRobotInformation->Caption = IntToStr(frnMain->StageInformation[STAGE_ROBOT].FoupNo) + "," + IntToStr(frnMain->StageInformation[STAGE_ROBOT].SlotNo) + " " + frnMain->StageInformation[STAGE_ROBOT].WaferID;
      frmRecipe->pnlWaferRobotInformation->Caption = frnMain->pnlWaferRobotInformation->Caption;
   }

   // 2020 5 21 - chc 確認是否有真空(有片)?
   frnMain->WriteSystemLog("Robot上有Wafer?");
   btnRobotWithWaferClick(this);

   if(frnMain->RobotWithWafer == ROBOT_WAFER_NO || pnlRobotWithWafer->Color == clRed) {
      pnlRobotMessage->Caption = IntToStr(no) + ": 無片!";
      frnMain->pnlOperationMessage->Caption = "異常: Robot取片後Fork上無片!";
      pnlMaintenanceMessage->Caption = frnMain->pnlOperationMessage->Caption;
      pnlRobotGet->Color = clRed;
      return;
   }
   frnMain->WriteSystemLog("Robot上有Wafer.");

   pnlRobotMessage->Caption = "GET: Succ.";
   pnlRobotGet->Color = clLime;

   frnMain->ChangeWaferPosition(rgWaferLocation->ItemIndex,WAFER_GET,4);

   // 2020 4 27 - chc 若為Aligner, 取片後要做ORGN
   if(rgWaferLocation->ItemIndex == LOCATION_ALIGNER1 || rgWaferLocation->ItemIndex == LOCATION_ALIGNER2) {
      // TSMC修改: 改用timer
      frnMain->tmAlignerORG->Enabled = true;
   }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMaintenance::WaitLoadPortDone(timems)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Sleep(10);
      Application->ProcessMessages();
      frnMain->GetTimeTic(&stoptime,&stoptick);
      elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
      if(elapsedms > timems)
         return false;
      if(pnlLoadPortOperation->Caption != "Busy")
         return true;
   }
}
//---------------------------------------------------------------------------
bool __fastcall TfrmMaintenance::WaitLoadPort2Done(timems)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Sleep(10);
      Application->ProcessMessages();
      frnMain->GetTimeTic(&stoptime,&stoptick);
      elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
      if(elapsedms > timems)
         return false;
      if(pnlLoadPort2Operation->Caption != "Busy")
         return true;
   }
}
//---------------------------------------------------------------------------
//14:59:49.826 -> RST[Cr]
//14:59:49.857 <- [Ack]
//14:59:59.966 <- RST[Cr]
//15:00:00.961 <- RST[Cr]
//15:00:01.958 <- RST[Cr]
//
bool __fastcall TfrmMaintenance::WaitTMRobotDone(timems)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
// 記錄State : 下完Command, 等0.1sec才進來 WaitRobotDone()
int state = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      if((/*boolRobotAck == true &&*/ boolRobotComplete == true) || boolRobotNak == true) {
         if(/*boolRobotAck == true &&*/ boolRobotComplete == true)
            frnMain->WriteSystemLog("WaitRobotDone: Ok");
         else
            frnMain->WriteSystemLog("WaitRobotDone: Nak");
         break;
      }
      Sleep(50);
      Application->ProcessMessages();
      Sleep(50);

      frnMain->GetTimeTic(&stoptime,&stoptick);
      elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
      if(elapsedms > timems) {
         frnMain->WriteSystemLog("WaitRobotDone: Timeout! " + IntToStr(timems));
         return false;
      }
   }
   return true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnRobotWithWaferClick(TObject *Sender)
{

   frnMain->RobotWithWafer = ROBOT_WAFER_UNKOWN;
   pnlRobotWithWafer->Color = clWhite;

   if(rgCST->ItemIndex == LOADPORT_1)
      edRobotCmd->Text = "WCH,1";
   else
      edRobotCmd->Text = "WCH,2";
   // 要Refresh
   edRobotCmd->Refresh();
   btnRobotCMDClick(this);
   // 等待完成 2000ms: pnlRobotWithWafer->Color
   int timeout = 20;
   int cnt = 0;
   while(1) {
      if(frnMain->RobotWithWafer != ROBOT_WAFER_UNKOWN)
         break;
      Sleep(100);
      Application->ProcessMessages();
      cnt++;
      if(cnt > timeout) {
         pnlRobotWithWafer->Color = clRed;
         pnlRobotMessage->Caption = "btnRobotWithWaferClick: Timeout!";
         frnMain->WriteSystemLog(pnlRobotMessage->Caption);
         return;
      }
   }
   return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnLoadPortStasClick(TObject *Sender)
{
int no;
        
}
//---------------------------------------------------------------------------

void __fastcall TfrmMaintenance::btnLoadPort2StasClick(TObject *Sender)
{
int no;
        
}
//---------------------------------------------------------------------------

