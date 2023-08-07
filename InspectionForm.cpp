//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "InspectionForm.h"
#include "frmMain.h"
#include "RecipeSetForm.h"

#include "LogoFrm.h"
#include "RecipeForm.h"

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
#pragma link "LMDButtonControl"
#pragma link "LMDCheckBox"
#pragma link "LMDCustomCheckBox"
#pragma link "LMDPNGImage"
#pragma link "LMDCustomButtonGroup"
#pragma link "LMDCustomGroupBox"
#pragma link "LMDCustomRadioGroup"
#pragma link "LMDRadioGroup"
#pragma link "LMDBaseEdit"
#pragma link "LMDCustomEdit"
#pragma link "LMDEdit"
#pragma link "LMDBaseControl"
#pragma link "LMDBaseGraphicControl"
#pragma link "LMDBaseMeter"
#pragma link "LMDCustomProgress"
#pragma link "LMDGraphicControl"
#pragma link "LMDProgress"
#pragma resource "*.dfm"
TfrmInspection *frmInspection;

int MeasurePointNo = -1;
extern int WaferGetStep;
extern bool boolInLoad;
extern bool boolInUnLoad;
extern bool boolInLoopTest;
extern unsigned long GigaWidth,GigaHeight;
extern bool boolCCDImageLoadedAry[3];
extern bool boolBaslerImageLoadedAry[3];
extern int MousePosX;
extern int MousePosY;
bool __fastcall MoveToXY(int xpos,int ypos);
void __fastcall Beep(int mstime);
extern int CCDShutterAry[3],CCDGainAry[3],CCDGrayAry[3],CCDContrastAry[3];
extern bool bool_CCD_StatusAry[3];
extern bool boolGetNextWafer;
extern bool boolGetNextWaferDone;

void __fastcall GetPosition(int axisno, I32 *pos);
bool __fastcall MoveToY(int position);
bool __fastcall MoveToX(int position);
bool __fastcall MoveToT(int position);
void __fastcall Beep(int mstime);
bool __fastcall MoveToXY(int xpos,int ypos);
void __fastcall Beep(int mstime);
bool __fastcall MoveToZ(int position);
bool __fastcall WaitMotionXYZTDone(int ms);

// 2023 1 28 - chc 點出圓心位置
int CircleCenterX1=-1,CircleCenterY1=-1;
int CirclePointX1=-1, CirclePointY1=-1;
int CircleCenterX2=-1,CircleCenterY2=-1;
int CirclePointX2=-1, CirclePointY2=-1;
// 記錄線位置: 相對於原點
int Line1X=-1,Line1Y=-1;
int Line2X=-1,Line2Y=-1;

// 2023 2 13 - chc 設定已執行的點數量
int PointSetNo = 0;

//---------------------------------------------------------------------------
__fastcall TfrmInspection::TfrmInspection(TComponent* Owner)
        : TForm(Owner)
{

   ImageFactor = 1;
   ImageLeftFactor = 1;
   ImageRightFactor = 1;
   MeasureState = 0;

   imDraw->Visible = true;
   imDraw->Top = imMeasure->Top;
   imDraw->Left = imMeasure->Left;
   imDraw->Width = imMeasure->Width;
   imDraw->Height = imMeasure->Height;
   GridTitle();

   // 2023 2 12 - chc Serial No: top/Left/Right
   TopSeialNo = 0;
   TopLeftSeialNo = 0;
   TopRightSeialNo = 0;
   BackLeftSeialNo = 0;
   BackRightSeialNo = 0;

   // 2023 4 1 - chc DefectList & BinCode Title
   SetupBINCodeTitle();
   SetupDefectListTitle();

   // 2023 4 8 - chc 位置
   pnlAutoSaveMicro->Top = 8;
   pnlAutoSaveMicro->Left = 616;
   pnlStageMoving->Top = 376;
   pnlStageMoving->Left = 712;
   pnlLoadingNextWafer->Left = 160;
   pnlLoadingNextWafer->Top = 200;
   pnlMicroResult->Left = 16;
   pnlMicroResult->Top = 8;

   // 2023 4 12 - chc From Save: boolFromSave
   boolFromSave = false;

   // 2023 4 26 - chc 顯示翻面訊息
   pnlMacroJudageDone->Left = 560;
   pnlMacroJudageDone->Top = 648;
   pnlMicroJudageDone->Left = 560;
   pnlMicroJudageDone->Top = 712;

   // 2023 4 28 - chc Capture cnt: CaptureCount
   CaptureCount = 0;

}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::FormShow(TObject *Sender)
{
static bool boolfirst = true;

   // CCD FOV vs CCDImage Area
   CCDDx = frnMain->CCDInfoAry[0].Width - imInspection->Width;
   CCDDy = frnMain->CCDInfoAry[0].Height - imInspection->Height;
   CCDRangeX = CCDDx / 2;
   CCDRangeY = CCDDy / 2;

   rgSpeed->ItemIndex = frnMain->rgSpeed->ItemIndex;

   frnMain->pnlCurrentOperation->Caption = "Inspection";

   // 2023 2 20b - chc 由Recipe決定
   if(frnMain->boolRecipeSelect == true) {
      rgBXFMLensChange(this,rgBXFMLens->ItemIndex);
      rgObservationClick(this);
   }
   else {

      rgBXFMLens->ItemIndex = frnMain->rgBXFMLens->ItemIndex;
   }
   sgDefectCount->ColWidths[0] = 30;
   sgDefectCount->ColWidths[1] = 40;
   sgDefectCount->ColWidths[2] = 150;
   sgDefectCount->ColWidths[3] = 60;

   sgDefectCount->Cells[0][0] = "No";
   sgDefectCount->Cells[1][0] = "ID";
   sgDefectCount->Cells[2][0] = "Defect Name";
   sgDefectCount->Cells[3][0] = "Count";
   sgDefectCount->RowCount = 2;

   // DefectDefine
   sgDefectDefine->ColWidths[0] = 30;
   sgDefectDefine->ColWidths[1] = 40;
   sgDefectDefine->ColWidths[2] = 150;

   sgDefectDefine->Cells[0][0] = "No";
   sgDefectDefine->Cells[1][0] = "ID";
   sgDefectDefine->Cells[2][0] = "Defect Name";
   sgDefectDefine->RowCount = 2;

   InitWaferMap();
   tm250ms->Enabled = true;

   // 繪Wafer Map
   if(boolInLoopTest == true || frnMain->boolInInspecting == true) {
      // Macro
      //if(WaferGetStep == 1) {
      //   pcInspection->ActivePage = tsMacroInspection;
      //}
      //// Micro
      //else {
         pcInspection->ActivePage = tsMicroInspection;
         btnWaferMapDrawClick(this);
         Sleep(100);
         btnWaferMapDrawClick(this);
      //}
   }

   frmInspection->Left = 3;
   frmInspection->Top = 3;
   frmInspection->Width = 1911;
   frmInspection->Height = 1040;

   // 取Cassette Mapping
   TPanel *panelT,*panelBE,*panelBC,*panelM,*panelC;
   TPanel *ipanelT,*ipanelBE,*ipanelBC,*ipanelM,*ipanelC;
   AnsiString str;
   for(int i=0 ; i<25 ; i++) {
      str.sprintf("%02d",i+1);
      panelT = (TPanel*)(FindComponent("pnlWaferTop" + str));
      panelBE = (TPanel*)(FindComponent("pnlWaferBackEdge" + str));
      panelBC = (TPanel*)(FindComponent("pnlWaferBackCenter" + str));
      panelM = (TPanel*)(FindComponent("pnlWaferMicro" + str));
      panelC = (TPanel*)(FindComponent("pnlWaferWaferID" + str));
      ipanelT = (TPanel*)(frmInspection->FindComponent("pnlWaferTop" + str));
      ipanelBE = (TPanel*)(frmInspection->FindComponent("pnlWaferBackEdge" + str));
      ipanelBC = (TPanel*)(frmInspection->FindComponent("pnlWaferBackCenter" + str));
      ipanelM = (TPanel*)(frmInspection->FindComponent("pnlWaferMicro" + str));
      ipanelC = (TPanel*)(frmInspection->FindComponent("pnlWaferWaferID" + str));

      ipanelT->Color = panelT->Color;
      ipanelBE->Color = panelBE->Color;
      ipanelBC->Color = panelBC->Color;
      ipanelM->Color = panelM->Color;
      ipanelC->Color = panelC->Color;
   }

   // 2022 12 22 - chc Chuck Wafer? Tazmo
   if(boolfirst == true && frnMain->boolInInspecting == false) {
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         btnMacroInverse->Visible = true;
         pnlMacroInverse->Visible = true;
      }
      else {
         btnMacroInverse->Visible = false;
         pnlMacroInverse->Visible = false;
         btnMacroRobotPosition->Enabled = true;
         btnMacroReadyPosition->Enabled = true;
         SetBackFun(false);
         SetTopFun(false);
         btnMacroInnerPosition->Enabled = true;
         btnMacroInnerStop->Enabled = false;
         btnMacroOuterPosition->Enabled = true;
      }
   }
   boolfirst = false;

   // 2023 2 16a - chc DIC - WLS3200 : boolDIC
   if(frnMain->boolDIC == false) {
      gbDIC->Enabled = false;
      rgObservation->Items->Clear();
      rgObservation->Items->Append("BF");
      rgObservation->Items->Append("DF");
      rgObservation->Columns = 2;
   }

   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      btnMacroInnerPosition->Enabled = false;
      btnMacroOuterPosition->Enabled = false;
   }
   else {

      // 2023 2 16a - chc DIC - WLS3200 : boolDIC
      if(frnMain->boolDIC == false)
         gbDIC->Enabled = false;

      pnlLeftCCD->Visible = false;
      pnlRightCCD->Visible = false;
      cbCCDFullImageLR->Visible = false;
      cbFreezeRL->Visible = false;
      rgFileFormat->Visible = false;
      laRightCCD->Visible = false;
      laLeftCCD->Visible = false;
      pnlRightFactor->Visible = false;
      pnlLeftFactor->Visible = false;
      rgRightCCDShutter->Visible = false;
      rgLeftCCDShutter->Visible = false;
      btnSaveRightCCD->Visible = false;
      cbMacroBackCrossLine->Visible = false;
      btnSaveLeftCCD->Visible = false;
      cbMacroTopCrossLine->Visible = false;

      // 2023 2 16a - chc DIC - WLS3200 : boolDIC
      if(frnMain->boolDIC == false) {
         rgObservation->Items->Clear();
         rgObservation->Items->Append("BF");
         rgObservation->Items->Append("DF");
         rgObservation->Columns = 2;
      }

      rgWaferType->Items->Clear();
      rgWaferType->Items->Append("12\"");
      rgWaferType->Items->Append("12\"");

      // 2023 4 5 - chc 沒有Macro AutoSave
      cbAutoSaveMacro->Visible = false;

   }

   GetStatus();
   //frmInspection->Top = 100;

   // Setup BIN Code
   // 2023 2 28 - chc 不用載入Demo檔
   //SetupBINCode();
   //SetupDefectList();

   // 2023 2 11 - chc 確認XYZT是否可動
   // => 2023 2 12 確認需要
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE) {
      if(WaitMotionXYZTDone(500) == false) {
         frnMain->WriteSystemLog("frmInspection-Show: XYZT尚未Ready.");
      }
      else {
         // Reset Move State
         frnMain->WriteSystemLog("frmInspection-Show: XYZT已Ready.");
         frnMain->iAxisMove[X_AXIS] = NO_MOVE;
         frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
         frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
         frnMain->iAxisMove[T_AXIS] = NO_MOVE;
      }
   }

   // 2023 2 12 - chc 重設status
   pnlLoadingNextWafer->Visible = false;
   tsMicroInspection->Enabled = true;

   // CCD Parameter
   // Shutter
   // 2023 2 20b - chc 由Recipe決定: 是否已切換Recipe?
   if(frnMain->boolRecipeSelect == true) {
      rgCCDShutterClick(this);
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
         rgLeftCCDShutterClick(this);
         rgRightCCDShutterClick(this);
      }
   }
   else {

      rgCCDShutter->ItemIndex = CCDShutterAry[CCD_TOP];
      rgLeftCCDShutter->ItemIndex = CCDShutterAry[CCD_LEFT];
      rgRightCCDShutter->ItemIndex = CCDShutterAry[CCD_RIGHT];
   }

   // Intensity: 120
   //tbBXFMLMP->Position = 120;
   // 2023 2 20b - chc 由Recipe決定
   if(frnMain->boolRecipeSelect == true) {
      int no;
      no = tbBXFMLMP->Position;
      frnMain->WriteSystemLog("frmInspection-Show: Recipe Selected, Intensity= " + IntToStr(no));
      SetIntensity(no);
      // AS=3113
      if(pnlASValue->Caption.ToInt() == 3113)
         btnAS6Click(this);
   }
   else {

      int no;
      no = tbBXFMLMP->Position;
      SetIntensity(no);
      // AS=3113
      btnAS6Click(this);
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::GetStatus()
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

   // 2023 4 3 - chc Assign cbAF會觸發Click
   //cbAF->Checked =                      frnMain->boolAFStatus;

   // Macro
   pnlLeftLight->Caption =              frnMain->pnlLED2CH1->Caption;
   pnlRightLight->Caption =             frnMain->pnlLED2CH2->Caption;
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      rgLeftCCDShutter->ItemIndex =     CCDShutterAry[CCD_LEFT];
      rgRightCCDShutter->ItemIndex =    CCDShutterAry[CCD_RIGHT];
   }   
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgDefectCountDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow == 0) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else if(ACol == 0 || ACol == 1) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }
   else {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgDefectDefineDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow == 0) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else if(ACol == 0 || ACol == 1) {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }
   else {
      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
}
//---------------------------------------------------------------------------
bool boolDrawImage = false;
void __fastcall TfrmInspection::btnWaferMapDrawClick(
      TObject *Sender)
{

   DrawBigImage(1);

   // 2023 5 6 - chc 是呼叫DrawBigImage()就設定
   //boolDrawImage = true;

   // 2023 3 22 - chc Draw Sub die
   DrawSubDie();

}
//---------------------------------------------------------------------------
// 2023 4 5 - chc Gross Number
int GrossNo = 0;

TColor IDColor[16] = {clBlack,clMaroon,clGreen,clOlive,clNavy,clPurple,clTeal,clGray,
                      clSilver,clRed,clLime,clYellow,clBlue,clFuchsia,clAqua,clWhite};
// 2022 8 20 - chc 測試Load Big Image
// 12": 304.8mm
// Wafer: 708*708
// 300mm(12") => 423um/Pixel
// 5000*5000 => 11*11
//
// .存dx,sx,dy,sy
int MapOutDx;
int MapOutSx;
int MapOutDy;
int MapOutSy;
int MapCode[1000][1000];
#define MAX_ROW_NO      1000
#define MAX_COL_NO      1000
// 原點位置
int GridNoX;
int GridNoY;
// 中心Die座標
int CenterDieRow = -1;
int CenterDieCol = -1;
// 最小Row,Col
int iMinRow = -1;
int iMinCol = -1;
int iMaxRow = -1;
int iMaxCol = -1;
//
int MapX,MapY;
// 偏移量
int COffsetX,COffsetY;

// 5 - 指定位置,紫
// 4 - 量測點(紅)
// 3 - Testable(綠)
// 2 - Type(2-half),黑
// 1 - Type(1-Out),黃
// 0 - Initial
// type: 0-Inner, 1-Out, 2-half
//
// mode: 1-imWaferMap, 2-imWaferMapResult
void __fastcall TfrmInspection::DrawBigImage(int mode)
{
int no,width,height,sx,sy,x,y,w,h,cx,cy,dx,dy,bw,bh,iw,ih,bx,by,ncx,ncy;
int maxno,row,col,id;
AnsiString fname,msg,idname;

//DoubleBuffered = true;
//sbWaferMap->DoubleBuffered = true;

TRect Rect;
int factor = 2;

// Die的起點: 依Die方向決定
int diex,diey;
static bool boolfirst = true;

// 第一次呼叫InMeasure
bool boolinmeasure = true;

// 2023 5 4 - chc test 繪格線
double fdx,fdy;

// 最小Row,Col
int minrow,mincol,maxrow,maxcol,minsx,minsy,minex,miney;
minrow = -1;
mincol = -1;
maxrow = -1;
maxcol = -1;

// 2023 4 5 - chc Result
TImage *imWaferMap;
TPanel *pnlWaferMap;
if(mode == 1) {
   imWaferMap = imWaferMapSrc;
   pnlWaferMap = pnlWaferMapSrc;

   // 2023 5 4 - chc 要設原值
   imWaferMap->Width = 643;
   imWaferMap->Height = 643;

}
else {
   imWaferMap = imWaferMapResult;
   pnlWaferMap = pnlWaferMapResult;

   // 2023 5 4 - chc 要設原值
   imWaferMap->Width = 725;
   imWaferMap->Height = 725;

}

// 2023 1 2 - chc 錯
int maxsy,maxey;

   if(boolfirst == true) {
      //boolfirst = false;
      for(int i=0 ; i<MAX_ROW_NO ; i++) {
         for(int j=0 ; j<MAX_COL_NO ; j++) {

            // 2023 5 2 - chc 初始為-2
            //MapCode[i][j] = 0;
            MapCode[i][j] = -2;

         }
      }
   }

   // 2023 5 6 - chc 是呼叫DrawBigImage()就設定
   boolDrawImage = true;

   factor = 1;
   imWaferMap->Picture->Bitmap->PixelFormat = pf4bit;

   // bw,bh: imWaferMap的大小
   //bw = sbWaferMap->Width;
   //bh = sbWaferMap->Height;
   bw = pnlWaferMap->Width;
   bh = pnlWaferMap->Height;

   // 304800 / 5000 = 60.96 => 61格, 正負2格共65格 => 畫在1207*921內
   // 限在850以內 => 850/65=13.07, 取整數13點
   // 原始影像: 65*13=845*845
   int gridnox,gridnoy,diew,dieh,ix,iy;
   double fx,fy;

   // 2023 3 23a - chc 修正
   double fgridnox,fgridnoy;

   // 改成小數及加入切割道: edDieSizeGapX
   diew = edChipWidth->Text.ToDouble() + edChipDx->Text.ToDouble();
   dieh = edChipHeight->Text.ToDouble() + edChipDy->Text.ToDouble();

   // 是300, 內縮6(左右各3mm)
   double diameter = 300000;

   // ignore改成30000
   double ignore = edIgnore->Text.ToInt();
   double realr = diameter - (ignore * 2);

   // Wafer位置固定為置中
   if(true) {
      gridnox = (realr - diew) / diew + 1;
      gridnoy = (realr - dieh) / dieh + 1;
      msg.sprintf("realr,diew,dieh,nox,noy=%.1f,%d,%d,%d,%d",realr,diew,dieh,gridnox,gridnoy);
      frnMain->WriteSystemLog(msg);

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

      // 2023 3 23a - chc 修正
      tmp = diameter / diew;
      gridnox = tmp;
      tmp = diameter / dieh;
      gridnoy = tmp;
      fgridnox = (double)diameter / diew + 2;
      fgridnoy = (double)diameter / dieh + 2;

      // 外擴
      gridnox += 2;
      gridnoy += 2;
   }
   // 左上
   else if(rgWaferDieDirection->ItemIndex == 1) {
      gridnox = realr / diew;
      gridnoy = realr / dieh;
   }
   // 左下
   else if(rgWaferDieDirection->ItemIndex == 2) {
      gridnox = realr / diew;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }
   // 右上
   else if(rgWaferDieDirection->ItemIndex == 3) {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = realr / dieh;
   }
   else {
      gridnox = ((int(realr / 2)) / diew) * 2 ;
      gridnoy = ((int(realr / 2)) / dieh) * 2 ;
   }

   // 2023 4 27a - chc 繪格線
   fdx = (double)((imWaferMap->Width-10)*factor) / gridnox;
   fdy = (double)((imWaferMap->Height-10)*factor) / gridnoy;

   // dx,dy: Die的實際大小(有倍率)
   dx = (double)((imWaferMap->Width-10)*factor) / (double)gridnox;
   dy = (double)((imWaferMap->Height-10)*factor) / (double)gridnoy;

   // width,height: Mapper圖的實際大小(有倍率)
   width = gridnox * dx;
   height = gridnoy * dy;

// 2023 5 4 - chc test
width = gridnox * fdx;
height = gridnoy * fdy;

   // 2021 9 27 - chc 原點位置: GridNoY
   GridNoX = gridnox;
   GridNoY = gridnoy;

   // 2023 3 23a - chc 修正
   //pnlChipWNo->Caption = IntToStr(gridnox);
   //pnlChipHNo->Caption = IntToStr(gridnoy);
   pnlChipWNo->Caption = IntToStr(gridnox-2);
   pnlChipHNo->Caption = IntToStr(gridnoy-2);

   // 100% - Full Wafer(12")
   // 影像框
   // Factor
   if(factor > 1) {
      // bx,by影像框起點: 有倍率則是(0,0)就可以開始畫
      bx = by = 0;
      // 整個imWaferMap區域均可以畫
      imWaferMap->Width = bw;
      imWaferMap->Height = bh;
      imWaferMap->Picture->Bitmap->Width = width;
      imWaferMap->Picture->Bitmap->Height = height;
      // iw,ih: 外擴: imWaferMap的繪圖區域
      iw = width;
      ih = height;
   }
   else {
      // iw,ih: 外擴: imWaferMap的繪圖區域
      iw = width + (bw - width) / 2;
      ih = height + (bh - height) /2;
      // imWaferMap內某區域內可以畫
      imWaferMap->Width = iw;
      imWaferMap->Height = ih;
      // bx,by影像框起點
      bx = (bw - iw) / 2;
      by = (bh - ih) / 2;
      imWaferMap->Picture->Bitmap->Width = iw * factor;
      imWaferMap->Picture->Bitmap->Height = ih * factor;

      // Die的起點: 依Die方向決定
      // 置中
      if(rgWaferDieDirection->ItemIndex == 0) {
         diex = 0;
         diey = 0;
      }
      // 正中
// 2023 3 22 - chc 不做, 置中
//      bx = 0;
//      by = 0;
   }

   imWaferMap->Top = by;
   imWaferMap->Left = bx;
   // sx,sy: die起點
   // iw,ih: 外擴: imWaferMap的繪圖區域
   // width,height: Mapper圖的實際大小(有倍率)
   // => sx,sy會改變Notch位置
   sx = (iw - width) / 2;
   sy = (ih - height) / 2;

   // 計算Die中心/Wafer中心的偏移Pixel數
   // 1. 放入Wafer
   // 2. 找Wafer中心(cx,cy)
   // 3. 定中心Die的中心(dx,dy)
   // 4. 以Wafer中心畫出Wafer Mapping圖
   // 5. 標示Wafer中心與Die中心
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
   // dx,dy: 每Die的Pixel數量
   // 偏移量(pulse)轉成um
   coffsetx *= X_RESOLUTION;
   coffsety *= frnMain->Y_RESOLUTION;
   while(coffsetx > diew) {
      coffsetx -= diew;
   }
   while(coffsety > dieh) {
      coffsety -= dieh;
   }

   // 若偏移量太大就不做偏移量就好
   if(fabs(coffsetx) > diew)
      coffsetx = 0;
   if(fabs(coffsety) > dieh)
      coffsety = 0;

   // 偏移量(um)轉成Pixel
   coffsetx = coffsetx * dx / (double)diew;
   coffsety = coffsety * dy / (double)dieh;
   coffsetx = 0 - coffsetx;
   coffsety = 0 - coffsety;
   // 由Die中心點往外畫格線
   // sx,xy: 是整個Grid在TImage畫的開始位置與Die所在位置無關

   // 正常不可有
   //coffsetx = coffsety = 5;                                                     // ==> For Test

   coffsetx *= factor;
   coffsety *= factor;
   COffsetX = coffsetx;
   COffsetY = coffsety;
*/
coffsetx = 0;
coffsety = 0;

   // 2023 3 2 - chc 正中
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

   // 2023 3 23a - chc 修正
   Wafer.Width = ((double)(diameter)/diew) * dx;
   Wafer.Height = ((double)(diameter)/dieh) * dy;

   // 計算中心Die的Col,Row: CenterDieCol,CenterDieRow
   // 直徑: realr(um)
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

   // 只要有一者為0就無偏移
   if(real_cx == 0 || real_cy == 0 || centerdie_x == 0 || centerdie_y == 0) {
      diffx = 0;
      diffy = 0;
   }

   // 2023 4 5a - chc 固定為0
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

   // 中心點在Die的中心點上
   // 2023 2 19 - chc 移到後面
   /*
   CenterDieCol = (double)(fx + diew/2) / (double)diew;
   CenterDieRow = (double)(fy + dieh/2) / (double)dieh;

   // 最小Row,Col: 切換Recipe後MinCol/MinRow第一次的DrawBigImage()不會有值 => 要呼叫兩次(否則無法畫出中心Die位置-藍色)
   if(iMinCol != -1)
      CenterDieCol -= iMinCol;
   if(iMinRow != -1)
      CenterDieRow -= (GridNoY - iMinRow - 1);

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
   // 中心
   cx = iw / 2;
   cy = ih / 2 + 1;

   // 清空
   imWaferMap->Canvas->Brush->Color = clSilver;
   Rect.left = 0;
   Rect.top = 0;
   Rect.Right = iw - 1;
   Rect.Bottom = ih - 1;
   imWaferMap->Canvas->FillRect(Rect);
   // 畫外框
   imWaferMap->Canvas->Pen->Color = clNavy;
   imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->MoveTo(0,0);
   imWaferMap->Canvas->LineTo(0,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,ih-1);
   imWaferMap->Canvas->LineTo(iw-1,0);
   imWaferMap->Canvas->LineTo(0,0);

   // 格線
   imWaferMap->Canvas->Pen->Width = 1;
   imWaferMap->Canvas->Pen->Color = clBlack;
   // 由Die中心點往外畫格線
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

   // realr
   diameter = realr;

   // 畫圓: 300
   // X
   // 65*5000=325000
   // (325000 - 300000) / 2 = 12500
   // 325000 / width(845) = 384.65um/Pixel
   // 起點: 12500 / 384.65 = 32.49 => 取整數32
   // 總點數=300000 / 384.65 = 779.929 => 取整數779
   int esx,esy,ew,eh,maxw,maxh;
   double px,py;
   maxw = gridnox * diew;
   px = (double)maxw / width;

   // 是300, 內縮6(左右各3mm): diameter
   esx = ((double)(maxw - diameter) / 2) / px;
   ew = diameter / px;
   ew++;
   maxh = gridnoy * dieh;
   py = (double)maxh / height;

   // 是300, 內縮6(左右各3mm): diameter
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

   // 各格狀態
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
   // 半徑
   // 是300, 內縮6(左右各3mm): diameter
   r = (diameter / 2) / px;
   rx = (diameter / 2) / px;
   ry = (diameter / 2) / py;

   squarex = rx * rx;
   squarey = ry * ry;
   squarexy = squarex * squarey;
   // 每格四角距離是否均在r以內?
   imWaferMap->Canvas->Brush->Style = bsSolid;
   testno = inkno = skipno = 0;

   // 存dx,sx,dy,sy
   MapOutDx = dx;
   MapOutSx = sx;
   MapOutDy = dy;
   MapOutSy = sy;

   for(int col=0 ; col<gridnox ; col++) {
      // 由Die中心點往外畫格線
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;

      // 2023 5 4 - chc test
      gsx = col * fdx + sx + coffsetx;
      gex = (col+1) * fdx + 1 + sx + coffsetx;

      // 縮小
      gsx++;
      gex--;

      // 2023 3 2 - chc 縮小
      //for(int row=0 ; row<gridnoy ; row++) {
      // 2023 5 2 - chc 不縮小
      //for(int row=0 ; row<gridnoy-3 ; row++) {
      for(int row=0 ; row<gridnoy ; row++) {

         // 由Die中心點往外畫格線
         gsy = row * dy + sy + coffsety;
         gey = gsy + dy + 1;

         // 2023 5 4 - chc test
         gsy = row * fdy + sy + coffsety;
         gey = (row+1) * fdy + 1 + sy + coffsety;

         // 縮小
         gsy++;
         gey--;

         // 2023 3 22a - chc Check gey: sy+esy1+eh1-6*factor
         if(gey >= (sy+esy1+eh1-6*factor-COffsetY))
            continue;

         // 重定
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

         // 重定
         sy += (dy/2);
         cy += (dy/2);

         if(fsx < 1 && fex < 1 && fsy < 1 && fey < 1) {
            // 綠色
            imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
            // 最小Row,Col: minrow,mincol,minsx,minex,minsy,miney
            if(col < mincol || mincol == -1) {
               mincol = col;
               minsx = gsx;
               minex = gex;
            }

            // 2023 1 2 - chc 錯
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

               // 2023 1 2 - chc 錯
               maxsy = gsy;
               maxey = gey;

            }

            // 2023 1 2 - chc BIN Code
            /*
            // 量測點
            if(InMeasure(row,col,boolinmeasure) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // 最小Row,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 4;
               }
               imWaferMap->Canvas->Brush->Color = clRed;
            }

            // 指定點
            else if(InSelect(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // 最小Row,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 5;
               }
               imWaferMap->Canvas->Brush->Color = clFuchsia;
            }

            // Center Die, 最小Row,Col
            else if(InCenterDie1(row,col) == true) {
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // 最小Row,Col
                  // 下: 180
                  int r,c;
                  r = row;
                  c = col;
//
                  // 2023 3 1 - chc 依座標決定
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
               // 綠色
               imWaferMap->Canvas->Brush->Color = (TColor)0x0002FF50;
               // 填入code
               if(row < MAX_ROW_NO && col < MAX_COL_NO) {
                  // 最小Row,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }
                  MapCode[row][col] = 3;
               }
            }
            */

            // 2023 5 2 - chc 綠色
            MapCode[row][col] = 0;

            // 第一次呼叫InMeasure
            boolinmeasure = false;

            // 外擴
            //imWaferMap->Canvas->Rectangle(gsx,gsy,gex,gey);
            imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);

            // 2023 3 22 - chc Sub-Die
            DrawSubDieMap(gsx-1,gsy-1,gex+1,gey+1,mode);

            testno++;
            if(factor > 3) {
               DrawRowCol(gsx,gsy,col,row,0,factor);
            }
         }
         else if(fsx > 1 && fex > 1 && fsy > 1 && fey > 1) {

            // 2023 5 2 - chc 外
            MapCode[row][col] = -2;

            continue;
         }
         // 部份在邊界外, 檢查所有Pixel
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

            // 2023 5 2 - chc 半外
            MapCode[row][col] = -1;

         }
      }
   }
/*
   pnlTestables->Caption = IntToStr(testno);
   pnlInkdies->Caption = IntToStr(inkno);
   pnlSkipdies->Caption = IntToStr(skipno);
   pnlTotaldies->Caption = IntToStr(testno+skipno+inkno);
*/

   // 2023 4 5 - chc Gross Number
   GrossNo = testno;

   // 2023 3 22 - chc 再繪格線
   imWaferMap->Canvas->Pen->Width = 1;
   // 大Chip(有Sub-die)用粗線
   if(edSubDieXno->Text.ToInt() > 1 || edSubDieYno->Text.ToInt() > 1)
      imWaferMap->Canvas->Pen->Width = 2;
   imWaferMap->Canvas->Pen->Color = clBlack;
   // 由Die中心點往外畫格線
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
   
   // 最小Row,Col
   iMinRow = minrow;
   iMinCol = mincol;
   iMaxRow = maxrow;
   iMaxCol = maxcol;

   // 中心點在Die的中心點上
   // 2023 2 19 - chc 移到後面
   // 2023 4 5a - chc  由Wafer中心往外推算
   //CenterDieCol = (double)(ffx + diew/2) / (double)diew;
   //CenterDieRow = (double)(ffy + dieh/2) / (double)dieh;
   //// 最小Row,Col: 切換Recipe後MinCol/MinRow第一次的DrawBigImage()不會有值 => 要呼叫兩次(否則無法畫出中心Die位置-藍色)
   //if(iMinCol != -1)
   //   CenterDieCol -= iMinCol;
   //if(iMinRow != -1)
   //   // 2023 2 19 - chc 與col一樣
   //   //CenterDieRow -= (GridNoY - iMinRow - 1);
   //   CenterDieRow -= iMinRow;
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

      // 2023 4 25 - chc 加入Name
      //col = sgDefectList->Cells[4][i+1].ToInt();
      //row = sgDefectList->Cells[5][i+1].ToInt();
      col = sgDefectList->Cells[4+1][i+1].ToInt();
      row = sgDefectList->Cells[5+1][i+1].ToInt();

      // 下: 180
      // 2023 3 1 - chc 依座標決定
      //if(rgWaferDegree->ItemIndex == 2) {
      if(rgWaferAxisOrientation->ItemIndex == 1) {

         //row = GridNoY - row - 1;
         //// 最小Row,Col
         //col -= MinCol;
         //row -= (GridNoY - iMinRow - 1);                       => row = row + MinRow;
         col = col + iMinCol;
         row = GridNoY - row + iMinRow - 2;
      }
      // 由Die中心點往外畫格線
      gsx = col * dx + sx + coffsetx;
      gex = gsx + dx + 1;
      // 縮小
      gsx++;
      gex--;
      gsy = row * dy + sy + coffsety;
      gey = gsy + dy + 1;
      // 縮小
      gsy++;
      gey--;
      MapCode[row][col] = id;
      if(id > 0) {
         imWaferMap->Canvas->Brush->Color = IDColor[id - 1];

         // 2023 4 5 - chc Color來自: frmRecipeSet->RecipeBufferRun.BinCode.Color[i-1]
         for(int i=0 ; i<frmRecipeSet->RecipeBufferRun.BinCode.ItemNo ; i++) {
            if(id == frmRecipeSet->RecipeBufferRun.BinCode.ID[i]) {
               imWaferMap->Canvas->Brush->Color = frmRecipeSet->RecipeBufferRun.BinCode.Color[i];
               break;
            }
         }

         imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
      }

      // 2023 4 12 - chc Default Color
      else {
         imWaferMap->Canvas->Brush->Color = clWhite;
         imWaferMap->Canvas->Brush->Style = bsClear;
         imWaferMap->Canvas->Pen->Width = 2;
         imWaferMap->Canvas->Pen->Color = clWhite;
         imWaferMap->Canvas->Rectangle(gsx-1,gsy-1,gex+1,gey+1);
         imWaferMap->Canvas->Brush->Style = bsSolid;
      }

   }

   // 實際畫弧
   // The angles Angle and AngleLength are 1/16th of a degree. For example, a full circle equals 5760 (16*360).
   // Positive values of Angle and AngleLength mean counterclockwise while negative values mean clockwise direction.
   // Zero degrees is at the 3'o clock position.
   // Arc(int X, int Y, int W, int H, int Angle, int AngleLength);
   // -44 to 269
   imWaferMap->Canvas->Pen->Width = 3*factor;
   imWaferMap->Canvas->Brush->Style = bsClear;
   imWaferMap->Canvas->Pen->Color = (TColor)0x00457D7D;

   // 重定
   sy -= (dy/2);
   cy -= (dy/2);

   //imWaferMap->Canvas->Ellipse(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh);
   // 2021 7 21 - chc Notch上或下
   // 下: 180
//
   if(rgWaferDegree->ItemIndex == 2) {

      // 2023 3 1 - chc 固定左上
      //rgWaferAxisOrientation->ItemIndex = 1;
      rgWaferAxisOrientation->ItemIndex = 0;

      // 2023 1 2 - chc 錯
      int tmp;
      minsy = tmp;
      minsy = maxsy;
      maxsy = tmp;

      // 畫Notch: 2.3030(W), 1.50(H), 0度是左上角, 180度是左下角
      imWaferMap->Canvas->Arc(sx+esx,sy+esy,sx+esx+ew,sy+esy+eh,cx,sy+esy+eh-6*factor,cx,sy+esy+eh-6*factor);
      imWaferMap->Canvas->Pen->Color = clBlack;
      imWaferMap->Canvas->Arc(sx+esx1,sy+esy1,sx+esx1+ew1,sy+esy1+eh1,cx+7*factor,sy+esy1+eh1-6*factor,cx-7*factor,sy+esy1+eh1-6*factor);
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy1+eh1-6*factor,cx+7*factor,sy+esy1+eh1+8*factor,cx+7*factor,sy+esy1+eh1-0*factor,cx-7*factor,sy+esy1+eh1-0*factor);

      // 最小Row,Col: minrow,mincol,minsx,minex,minsy,miney
      // 2023 3 2 - chc 左上角
      //imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor+dy/2, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor+dy/2);

   }
   // 上: 0
   else {
//
      imWaferMap->Canvas->Arc(sx+esx, sy+esy, sx+esx+ew, sy+esy+eh,cx-7*factor,sy+esy+6*factor,cx+7*factor,sy+esy+6*factor);
      // 畫Notch: 2.3030(W), 1.50(H)
      imWaferMap->Canvas->Arc(cx-7*factor,sy+esy-6*factor,cx+7*factor,sy+esy+8*factor,cx-7*factor,sy+esy-0*factor,cx+7*factor,sy+esy-0*factor);
      // 0度是左上角, 180度是左下角
      // 2023 3 1 - chc 原點在左上角
      //imWaferMap->Canvas->Ellipse(sx+coffsetx+3*factor, sy+coffsety+3*factor, sx+coffsetx+dx-2*factor, sy+coffsety+dy-2*factor);
      imWaferMap->Canvas->Ellipse(minsx+coffsetx+3*factor, minsy+coffsety+3*factor, minsx+coffsetx+dx-2*factor, minsy+coffsety+dy-2*factor);

//
      rgWaferAxisOrientation->ItemIndex = 0;
   }
//

   Wafer.Cx = cx;
   Wafer.Cy = cy;

   // 原點
   // 偏移量, 0度是左上角, 180度是左下角
   // 中心打標
   imWaferMap->Canvas->Pen->Width = 1*factor;
   imWaferMap->Canvas->Pen->Color = clRed;
   imWaferMap->Canvas->MoveTo(cx-5*factor,cy);
   imWaferMap->Canvas->LineTo(cx+5*factor,cy);
   imWaferMap->Canvas->MoveTo(cx,cy-5*factor);
   imWaferMap->Canvas->LineTo(cx,cy+5*factor);
   imWaferMap->Canvas->Pen->Width = 1;
   // 中心點的col,row
   col = (cx - Wafer.X) / Wafer.Dx;
   row = (cy - Wafer.Y) / Wafer.Dy;

   // 2021 9 27 - chc 原點位置: GridNoY
   // 下: 180
//
   // 2023 3 1 - chc 依座標決定
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
   }
//

   // 重定
   sy += (dy/2);
   cy += (dy/2);

   // 最小Row,Col
   if(iMinCol != -1)
      col -= iMinCol;
   if(iMinRow != -1)
      row -= (GridNoY - iMinRow - 1);

   // 畫內框
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
void __fastcall TfrmInspection::InitWaferMap()
{

   Wafer.boolSet = false;
}
//---------------------------------------------------------------------------
// 2022 8 30 - chc Demo
int MeasurePointRow[9];
int MeasurePointCol[9];

// 第一次呼叫InMeasure
bool __fastcall TfrmInspection::InMeasure(int row,int col, bool first)
{
int no,r,c;
// 由XY重產生
int x,y,cx,cy,w,h,srow,scol,dx,dy;
double fw,fh;

// Demo
first = false;

   // 原點位置: GridNoY
   // 下: 180
   // 2023 3 1 - chc 依座標決定
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
      // 最小Row,Col
      col -= iMinCol;
      row -= (GridNoY - iMinRow - 1);
   }

   // 由XY重產生
   // 2022 9 2 - chc 暫不用
   //cx = sgArea->Cells[1][1].ToInt();
   //cy = sgArea->Cells[2][1].ToInt();
   cx = 0;
   cy = 0;
   
   fw = edChipWidth->Text.ToDouble() + edChipDx->Text.ToDouble();
   fh = edChipHeight->Text.ToDouble() + edChipDy->Text.ToDouble();
   // um to pulse
   fw = fw / frnMain->X_RESOLUTION;
   fh = fh / frnMain->Y_RESOLUTION;
   w = fw;
   h = fh;
   // 中心Die座標
   scol = pnlCenterCol->Caption.ToInt();
   srow = pnlCenterRow->Caption.ToInt();

   // 2022 9 2 - chc 暫不用
   //no = sgDot->RowCount - 2;

// Demo : 設定5 點Row,Col位置(9000um*9000um => 33*33)
no = 5;
int wcx,wcy;
wcx = (GridNoX - 2) / 2;
wcy = (GridNoY - 2) / 2;

   for(int i=0 ; i<no ; i++) {
      // 由XY重產生, (0,0)時才要變更
      if(first == false) {

// Demo : 設定5 點Row,Col位置(9000um*9000um => 33*33)
//         r = sgDot->Cells[6][i+1].ToInt();
//         c = sgDot->Cells[7][i+1].ToInt();
if(i == 0) {
   //r = 30;
   //c = 15;
   r = wcy * 2 - 2;
   c = wcx;
}
else if(i == 1) {
   //r = 15;
   //c = 3;
   r = wcy;
   c = 3;
}
else if(i == 2) {
   //r = 15;
   //c = 16;
   r = wcy;
   c = wcx;
}
else if(i == 3) {
   //r = 15;
   //c = 27;
   r = wcy;
   c = (wcx * 2) - 3;
}
else if(i == 4) {
   //r = 2;
   //c = 15;
   r = 2;
   c = wcx;
}
MeasurePointRow[i] = r+1;
MeasurePointCol[i] = c+1;

      }
      else {

         // 2022 9 2 - chc 暫不用
         //x = sgDot->Cells[1][i+1].ToInt();
         //y = sgDot->Cells[2][i+1].ToInt();
         x = cx;
         y = cy;
         
         dx = x - cx - w/2;
         dy = y - cy - h/2;
         c = scol - (dx / w);
         r = srow - (dy / h);
         // 下: 180
         // 2023 3 1 - chc 依座標決定
         //if(rgWaferDegree->ItemIndex == 2) {
         if(rgWaferAxisOrientation->ItemIndex == 1) {

            r = GridNoY - r - 1;
         }
      }

      if(c < MAX_COL_NO && r < MAX_ROW_NO && c >= 0 && r >= 0) {
         // (0,0)時才要變更
         if(first == true) {

            // 2022 9 2 - chc 暫不用
            //sgDot->Cells[6][i+1] = IntToStr(r);
            //sgDot->Cells[7][i+1] = IntToStr(c);
            
         }
      }

      if(r == row && c == col) {
         return true;
      }
   }
//
   return false;
}
//---------------------------------------------------------------------------
// 指定點
bool __fastcall TfrmInspection::InSelect(int row,int col)
{

/*
   // 原點位置: GridNoY
   // 下: 180
   // 2023 3 1 - chc 依座標決定
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
      // 最小Row,Col
      col -= iMinCol;
      row -= (GridNoY - iMinRow - 1);
   }

   if(SelectRow == -1 || SelectCol == -1)
      return false;
   if(SelectRow == row && SelectCol == col)
      return true;
*/
return false;
}
//---------------------------------------------------------------------------
// 最小Row,Col
bool __fastcall TfrmInspection::InCenterDie1(int row,int col)
{
int no;

/*
   // 原點位置: GridNoY
   // 下: 180
   // 2023 3 1 - chc 依座標決定
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      row = GridNoY - row - 1;
      col -= iMinCol;
      row -= (GridNoY - iMinRow - 1);
   }

   if(CenterDieRow == -1 || CenterDieCol == -1)
      return false;

   if(CenterDieRow == row && CenterDieCol == col) {
      no = 1;
      return true;
   }
*/
return false;
}
//---------------------------------------------------------------------------
// type : 0-Inner, 1-Out, 2-half
void __fastcall TfrmInspection::DrawRowCol(int sx,int sy,int col,int row,int type, int factor)
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

                  // 2021 12 15 - chc 最小Row,Col
                  if(row < 0 || col < 0) {
                     int no;
                     no = 1;
                  }

      // 2021 6 16 - chc 看MapCode[][]
      if(MapCode[row][col] == 4)
         imWaferMap->Canvas->Font->Color = clWhite;

      // 2021 8 11 - chc 指定點
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

      // 2021 6 16 - chc 看MapCode[][]
      if(MapCode[row][col] == 4)
         sdie = "Measure";

      // 2021 8 11 - chc 指定點
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

   // 2021 9 30 - chc 原點位置: GridNoY
   // 下: 180
   // 2023 3 1 - chc 依座標決定
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

void __fastcall TfrmInspection::imWaferMapSrcMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   MapX = X;
   MapY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imWaferMapSrcMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   if(Wafer.boolSet == true) {
      int col,row;
      // 偏移量
      col = (X - COffsetX - Wafer.X) / Wafer.Dx;
      row = (Y - COffsetY - Wafer.Y) / Wafer.Dy;
      if(X < (COffsetX + Wafer.X))
         col = -1;
      if(Y < (COffsetY + Wafer.Y))
         row = -1;

      // 原點位置: GridNoY
      // 下: 180
      // 2023 3 1 - chc 依座標決定
      //if(rgWaferDegree->ItemIndex == 2) {
      if(rgWaferAxisOrientation->ItemIndex == 1) {

         row = GridNoY - row - 1;
         // 最小Row,Col
         col -= iMinCol;

         // 2023 1 2 - chc 錯
         //row -= (GridNoY - iMinRow - 1);
         row -= (iMinRow + 1);

      }

      pnlChipCol->Caption = IntToStr(col);
      pnlChipRow->Caption = IntToStr(row);

      // 2023 3 22 - chc 加入Hint
      AnsiString msg;
      msg.sprintf("(%d,%d)",col,row);
      imWaferMapSrc->ShowHint = false;
      imWaferMapSrc->Hint = msg;
      imWaferMapSrc->ShowHint = true;

   }
}
//---------------------------------------------------------------------------
// 2022 9 2 - chc 標示量測點位置
void __fastcall TfrmInspection::tm250msTimer(TObject *Sender)
{

   tm250ms->Enabled = false;

   // Demo: 劃出外框: gsx-1,gsy-1,gex+1,gey+1, shMeasurePoint->Visible = true
   // 2023 4 11 - chc 不顯示
   /*
   if(MeasurePointNo != -1) {

      // 2022 9 15 - chc MeasurePoint
      rgMeasurePoint->ItemIndex = MeasurePointNo;
      shMeasurePoint->Visible = true;

      int px,py;
      RowColToXY(&px, &py);
      if(shMeasurePoint->Width <= 70) {
         shMeasurePoint->Width += 5;
         shMeasurePoint->Height += 5;
      }
      else {
         shMeasurePoint->Width = 30;
         shMeasurePoint->Height = 30;
      }
      shMeasurePoint->Left = px - shMeasurePoint->Width / 2;
      shMeasurePoint->Top = py - shMeasurePoint->Height / 2;
   }

   // 2022 9 15 - chc MeasurePoint
   else {
      rgMeasurePoint->ItemIndex = 0;
      shMeasurePoint->Visible = false;
   }
   */

   // 2023 5 6 - chc 更新目前位置
   if(frmInspection->Visible == true) {
      UpdateMotionPosition();
   }

   tm250ms->Enabled = true;
}
//---------------------------------------------------------------------------
// Row,Col to X,Y
// MeasurePointRow[i] = r;
// MeasurePointCol[i] = c;
// MeasurePointNo
void __fastcall TfrmInspection::RowColToXY(int *x, int *y)
{
int row,col,no,px,py;

   no = MeasurePointNo - 1;
   row = MeasurePointRow[no];
   col = MeasurePointCol[no];

   // From Location(Defect List)
   // 2023 2 27 - chc Looptest不變
   if(boolInLoopTest != true) {

      col = pnlWorkCol->Caption.ToInt();
      row = pnlWorkRow->Caption.ToInt();
   }
   // 下: 180
   // 2023 3 1 - chc 依座標決定
   //if(rgWaferDegree->ItemIndex == 2) {
   if(rgWaferAxisOrientation->ItemIndex == 1) {

      col = col + iMinCol;
      row = GridNoY - row + iMinRow - 2;
   }

   // 原點位置: GridNoY
   // 下: 180
   // 2023 3 1 - chc 依座標決定
   ////if(rgWaferDegree->ItemIndex == 2) {
   //if(rgWaferAxisOrientation->ItemIndex == 1) {
   //   row = GridNoY - row - 1;
   //   // 最小Row,Col
   //   col -= iMinCol;
   //   row -= (GridNoY - iMinRow - 1);
   //}

   px = col * Wafer.Dx + COffsetX + Wafer.X;
   py = row * Wafer.Dy + COffsetY + Wafer.Y;
   px += (Wafer.Dx / 2);
   py += (Wafer.Dy / 2);
   *x = px;
   *y = py;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgMeasurePointClick(TObject *Sender)
{

   // 2023 4 11 - chc 不顯示
   /*
   if(rgMeasurePoint->ItemIndex == 0) {
      MeasurePointNo = -1;
      shMeasurePoint->Visible = false;
   }
   else {
      MeasurePointNo = rgMeasurePoint->ItemIndex;
      shMeasurePoint->Visible = true;
   }
   */
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::FormClose(TObject *Sender,
      TCloseAction &Action)
{

   // Main
   frnMain->SetST(ST_INSPECTION,false);
   frnMain->pnlMainMode->Caption = "Manual";
   frnMain->pnlCurrentOperation->Caption = "Main";
   //Action = caFree;

   // 2023 2 28 - chc frmLogo Visible
   if(frmRecipeSet->Visible == false && frmRecipe->Visible == false && frnMain->pnlMaintenance->Visible == false)
      frmLogo->Visible = true;

}
//---------------------------------------------------------------------------
//int ImageFactor = 1;
int ImageSX = 0;
int ImageSY = 0;
int ImageXPos,ImageYPos;
int DoubleClickX,DoubleClickY;
bool ImageLeftMouse = false;
bool boolFormWheel = false;
// Left
int ImageLeftSX = 0;
int ImageLeftSY = 0;
int ImageLeftXPos,ImageLeftYPos;
bool ImageLeftLeftMouse = false;
// Right
int ImageRightSX = 0;
int ImageRightSY = 0;
int ImageRightXPos,ImageRightYPos;
bool ImageRightLeftMouse = false;

void __fastcall TfrmInspection::imInspectionMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 5 8 - chc Test
   frnMain->WriteSystemLog("imInspectionMouseDown");

   DoubleClickX = X;
   DoubleClickY = Y;

   ImageXPos = X;
   ImageYPos = Y;
   if(Button == mbLeft) {
      ImageLeftMouse = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imInspectionMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int gray,sx,sy;

   // 主畫面
   unsigned char r,g,b;
   TColor color;
   sx = 0;
   sy = 0;

   // 2023 3 17 - chc Ratio
   // 全影像比例
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
   // 顯示游標值
   pnlCCDR->Caption = r;
   pnlCCDG->Caption = g;
   pnlCCDB->Caption = b;
   pnlCCDX->Caption = X + sx;
   pnlCCDY->Caption = Y + sy;

   // Move
   if(ImageLeftMouse == true && frmInspection->cbFreeze->Checked == true && cbCCDFullImage->Checked == false) {
      int dx,dy,sx,sy;
      int width,height,w,h,no,w1,h1;
      w1 = imInspection->Width;
      h1 = imInspection->Height;
      dx = X - ImageXPos;
      dy = Y - ImageYPos;
      width = GigaWidth;
      height = GigaHeight;

      // 2023 3 17 - chc 是指可視範圍
      //w = width / ImageFactor;
      //h = height / ImageFactor;
      w = w1 / ImageFactor;
      h = h1 / ImageFactor;

      // 2023 3 17 - chc 只校ImageFactor就可以
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

void __fastcall TfrmInspection::imInspectionMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 5 8 - chc Test
   frnMain->WriteSystemLog("imInspectionMouseUp");

   ImageLeftMouse = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
int x,y,w,h,dh;
int leftx,lefty,rightx,righty,value;
bool booldo = false;
AnsiString msg;

   x = MousePos.x;
   y = MousePos.y;

   // 對應到Form上
   int fx = frmInspection->Left;
   int fy = frmInspection->Top;
   x -= fx;
   y -= fy;
   x -= 4;
   y -= 25;

   pnlWheelMsg->Caption = "Wheel Down";
   Handled = true;
   if(pcInspection->ActivePage == tsMicroInspection) {
      frnMain->WriteSystemLog("Micro: Wheel Down");
      if((boolCCDImageLoadedAry[0] == false && boolBaslerImageLoadedAry[0] == false) || frmInspection->cbFreeze->Checked == false)
         return;

      frnMain->WriteSystemLog("Micro: Wheel Down Active");
      // Area
      leftx = Panel3->Left + imInspection->Left;
      rightx = leftx + imInspection->Width;
      lefty = Panel3->Top + imInspection->Top;
      righty = lefty + imInspection->Height;
      msg.sprintf("sx,ex,sy,ey: x,y=%d,%d,%d,%d: %d,%d",leftx,rightx,lefty,righty,x,y);
      frnMain->WriteSystemLog(msg);
      if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
         frnMain->WriteSystemLog("Micro: Wheel Down Zoom");
         boolFormWheel = true;
         WheelCCD = 0;
         imDecClick(this);
         boolFormWheel = false;
      }
   }
   else if(pcInspection->ActivePage == tsMacroInspection) {
      if(frmInspection->cbFreezeRL->Checked == true) {
         if(boolCCDImageLoadedAry[1] == true || boolBaslerImageLoadedAry[1] == true) {
            // Area
            leftx = pnlLeftCCD->Left + imLeftCCD->Left;
            rightx = leftx + imLeftCCD->Width;
            lefty = pnlLeftCCD->Top + imLeftCCD->Top;
            righty = lefty + imLeftCCD->Height;
            // Left CCD
            if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
               boolFormWheel = true;
               WheelCCD = 1;
               imDecClick(this);
               boolFormWheel = false;
            }
         }
         // Right CCD
         if(boolCCDImageLoadedAry[2] == true || boolBaslerImageLoadedAry[2] == true) {
            leftx = pnlRightCCD->Left + imRightCCD->Left;
            rightx = leftx + imRightCCD->Width;
            lefty = pnlRightCCD->Top + imRightCCD->Top;
            righty = lefty + imRightCCD->Height;
            // Right CCD
            if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
               boolFormWheel = true;
               WheelCCD = 2;
               imDecClick(this);
               boolFormWheel = false;
            }
         }
      }

      // Left/Right Light
      w = pnlLeftMacroLamp->Width;
      h = pnlLeftMacroLamp->Height;
      leftx = pcInspection->Left + pnlMacroInspection->Left + 4 + pnlLeftMacroLamp->Left;
      lefty = pcInspection->Top + pnlMacroInspection->Top + 30 + pnlLeftMacroLamp->Top;
      rightx = pcInspection->Left + pnlMacroInspection->Left + 4 + pnlRightMacroLamp->Left;
      righty = pcInspection->Top + pnlMacroInspection->Top + 30 + pnlRightMacroLamp->Top;
      // Down
      if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
         pnlLeftMacroLamp->Color = clGray;
         if(frnMain->tbLED2CH1->Position > 2)
            frnMain->tbLED2CH1->Position -= 2;
         else
            frnMain->tbLED2CH1->Position = 0;
         frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
         frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
         pnlLeftLight->Caption = frnMain->tbLED2CH1->Position;
         frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
      }
      else {
         pnlLeftMacroLamp->Color = clSilver;
      }
      if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
         pnlRightMacroLamp->Color = clGray;
         if(frnMain->tbLED2CH2->Position > 2)
            frnMain->tbLED2CH2->Position -= 2;
         else
            frnMain->tbLED2CH2->Position = 0;
         frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
         frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
         pnlRightLight->Caption = frnMain->tbLED2CH2->Position;
         frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
      }
      else {
         pnlRightMacroLamp->Color = clSilver;
      }

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imAddClick(TObject *Sender)
{
int width,height,w,h,no;

   pnlWheelMsg->Caption = "Wheel Add";
   no = WheelCCD;
   if(WheelCCD == 0) {
      if(frmInspection->cbFreeze->Checked == false)
         return;

      frnMain->WriteSystemLog("Micro: Add");
      //no = 0;
      width = GigaWidth;
      height = GigaHeight;
      ImageFactor++;
      if(ImageFactor > 16) {
         ImageFactor = 16;
         return;
      }

      //pnlMainFactor->Caption = IntToStr(ImageFactor) + "x";
      AnsiString str;
      double stdratio = 0.387;
      double fratio = stdratio * ImageFactor;
      int iratio = fratio * 100;
      str.sprintf("%d\%",iratio);
      pnlMainFactor->Caption = str;

      pnlMainFactor->Refresh();
      w = width / ImageFactor;
      h = height / ImageFactor;
      imInspection->Picture->Bitmap->Width = w;
      imInspection->Picture->Bitmap->Height = h;

      double fdw,fdh;
      // Button: 中心點
      if(boolFormWheel == false) {
         fdw = double(width)/(ImageFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageFactor-1) - h;
         fdh /= 2;
         ImageSX += fdw;
         ImageSY += fdh;
      }
      // Wheel: 指定點 : ImageXPos, ImageYPos
      else {
         fdw = double(width)/(ImageFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageFactor-1) - h;
         fdh /= 2;
         fdw *= ((double)ImageXPos / ((double)imInspection->Width/2));
         fdh *= ((double)ImageYPos / ((double)imInspection->Height/2));
         ImageSX += fdw;
         ImageSY += fdh;
      }
      if((ImageSX + w) > width)
         ImageSX = width - w;
      if((ImageSY + h) > height)
         ImageSY = height - h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageSX,ImageSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imInspection->Canvas->Handle);

      int X,Y,ox,oy,nx,ny,ow,dw;
      if(boolFormWheel == true) {
         X = ImageXPos;
         Y = ImageYPos;
      }
      else {
         X = imInspection->Width / 2;
         Y = imInspection->Height / 2;
      }
   }
   else if(WheelCCD == 1) {
      if(frmInspection->cbFreezeRL->Checked == false)
         return;
      //no = 1;
      width = GigaWidth;
      height = GigaHeight;
      ImageLeftFactor++;
      if(ImageLeftFactor > 16) {
         ImageLeftFactor = 16;
         return;
      }

      //pnlLeftFactor->Caption = IntToStr(ImageLeftFactor) + "x";
      AnsiString str;
      double stdratio = 0.334;
      double fratio = stdratio * ImageLeftFactor;
      int iratio = fratio * 100;
      str.sprintf("%d\%",iratio);
      pnlLeftFactor->Caption = str;

      pnlLeftFactor->Refresh();
      w = width / ImageLeftFactor;
      h = height / ImageLeftFactor;
      imLeftCCD->Picture->Bitmap->Width = w;
      imLeftCCD->Picture->Bitmap->Height = h;

      double fdw,fdh;
      // Button: 中心點
      if(boolFormWheel == false) {
         fdw = double(width)/(ImageLeftFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageLeftFactor-1) - h;
         fdh /= 2;
         ImageLeftSX += fdw;
         ImageLeftSY += fdh;
      }
      // Wheel: 指定點 : ImageXPos, ImageYPos
      else {
         fdw = double(width)/(ImageLeftFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageLeftFactor-1) - h;
         fdh /= 2;
         fdw *= ((double)ImageLeftXPos / ((double)imLeftCCD->Width/2));
         fdh *= ((double)ImageLeftYPos / ((double)imLeftCCD->Height/2));
         ImageLeftSX += fdw;
         ImageLeftSY += fdh;
      }
      if((ImageLeftSX + w) > width)
         ImageLeftSX = width - w;
      if((ImageLeftSY + h) > height)
         ImageLeftSY = height - h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageLeftSX,ImageLeftSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imLeftCCD->Canvas->Handle);

      int X,Y,ox,oy,nx,ny,ow,dw;
      if(boolFormWheel == true) {
         X = ImageLeftXPos;
         Y = ImageLeftYPos;
      }
      else {
         X = imLeftCCD->Width / 2;
         Y = imLeftCCD->Height / 2;
      }
   }
   else if(WheelCCD == 2) {
      if(frmInspection->cbFreezeRL->Checked == false)
         return;
      //no = 2;
      width = GigaWidth;
      height = GigaHeight;
      ImageRightFactor++;
      if(ImageRightFactor > 16) {
         ImageRightFactor = 16;
         return;
      }

      //pnlRightFactor->Caption = IntToStr(ImageRightFactor) + "x";
      AnsiString str;
      double stdratio = 0.334;
      double fratio = stdratio * ImageRightFactor;
      int iratio = fratio * 100;
      str.sprintf("%d\%",iratio);
      pnlRightFactor->Caption = str;

      pnlRightFactor->Refresh();
      w = width / ImageRightFactor;
      h = height / ImageRightFactor;
      imRightCCD->Picture->Bitmap->Width = w;
      imRightCCD->Picture->Bitmap->Height = h;

      double fdw,fdh;
      // Button: 中心點
      if(boolFormWheel == false) {
         fdw = double(width)/(ImageRightFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageRightFactor-1) - h;
         fdh /= 2;
         ImageRightSX += fdw;
         ImageRightSY += fdh;
      }
      // Wheel: 指定點 : ImageXPos, ImageYPos
      else {
         fdw = double(width)/(ImageRightFactor-1) - w;
         fdw /= 2;
         fdh = double(height)/(ImageRightFactor-1) - h;
         fdh /= 2;
         fdw *= ((double)ImageRightXPos / ((double)imRightCCD->Width/2));
         fdh *= ((double)ImageRightYPos / ((double)imRightCCD->Height/2));
         ImageRightSX += fdw;
         ImageRightSY += fdh;
      }
      if((ImageRightSX + w) > width)
         ImageRightSX = width - w;
      if((ImageRightSY + h) > height)
         ImageRightSY = height - h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageRightSX,ImageRightSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imRightCCD->Canvas->Handle);

      int X,Y,ox,oy,nx,ny,ow,dw;
      if(boolFormWheel == true) {
         X = ImageRightXPos;
         Y = ImageRightYPos;
      }
      else {
         X = imRightCCD->Width / 2;
         Y = imRightCCD->Height / 2;
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imDecClick(TObject *Sender)
{
int width,height,w,h,no;

   pnlWheelMsg->Caption = "Wheel Dec";
   no = WheelCCD;
   if(WheelCCD == 0) {
      if(frmInspection->cbFreeze->Checked == false)
         return;
      frnMain->WriteSystemLog("Micro: Dec");
      //no = 0;
      width = GigaWidth;
      height = GigaHeight;
      if(ImageFactor >= 2) {
         ImageFactor--;

         //pnlMainFactor->Caption = IntToStr(ImageFactor) + "x";
         AnsiString str;
         double stdratio = 0.387;
         double fratio = stdratio * ImageFactor;
         int iratio = fratio * 100;
         str.sprintf("%d\%",iratio);
         pnlMainFactor->Caption = str;

         pnlMainFactor->Refresh();
         w = width / ImageFactor;
         h = height / ImageFactor;
         imInspection->Picture->Bitmap->Width = w;
         imInspection->Picture->Bitmap->Height = h;

         double fdw,fdh;
         fdw = w - double(width)/(ImageFactor+1);
         fdw /= 2;
         fdh = h - double(height)/(ImageFactor+1);
         fdh /= 2;
         fdw *= ((double)ImageXPos / ((double)imInspection->Width/2));
         fdh *= ((double)ImageYPos / ((double)imInspection->Height/2));
         ImageSX -= fdw;
         ImageSY -= fdh;
         if(ImageSX < 0)
            ImageSX = 0;
         if(ImageSY < 0)
            ImageSY = 0;

         if((ImageSX + w) > width)
            ImageSX = width - w;
         if((ImageSY + h) > height)
            ImageSY = height - h;

         frnMain->usb_ImageROIAry[no].SetPlacement(ImageSX,ImageSY,w,h);
         frnMain->usb_ImageROIAry[no].Draw(imInspection->Canvas->Handle);
      }
   }
   else if(WheelCCD == 1) {
      if(frmInspection->cbFreezeRL->Checked == false)
         return;
      //no = 1;
      width = GigaWidth;
      height = GigaHeight;
      if(ImageLeftFactor >= 2) {
         ImageLeftFactor--;

         //pnlLeftFactor->Caption = IntToStr(ImageLeftFactor) + "x";
         AnsiString str;
         double stdratio = 0.334;
         double fratio = stdratio * ImageLeftFactor;
         int iratio = fratio * 100;
         str.sprintf("%d\%",iratio);
         pnlLeftFactor->Caption = str;

         pnlLeftFactor->Refresh();
         w = width / ImageLeftFactor;
         h = height / ImageLeftFactor;
         imLeftCCD->Picture->Bitmap->Width = w;
         imLeftCCD->Picture->Bitmap->Height = h;

         double fdw,fdh;
         fdw = w - double(width)/(ImageLeftFactor+1);
         fdw /= 2;
         fdh = h - double(height)/(ImageLeftFactor+1);
         fdh /= 2;
         fdw *= ((double)ImageLeftXPos / ((double)imLeftCCD->Width/2));
         fdh *= ((double)ImageLeftYPos / ((double)imLeftCCD->Height/2));
         ImageLeftSX -= fdw;
         ImageLeftSY -= fdh;
         if(ImageLeftSX < 0)
            ImageLeftSX = 0;
         if(ImageLeftSY < 0)
            ImageLeftSY = 0;

         if((ImageLeftSX + w) > width)
            ImageLeftSX = width - w;
         if((ImageLeftSY + h) > height)
            ImageLeftSY = height - h;

         frnMain->usb_ImageROIAry[no].SetPlacement(ImageLeftSX,ImageLeftSY,w,h);
         frnMain->usb_ImageROIAry[no].Draw(imLeftCCD->Canvas->Handle);
      }
   }
   else if(WheelCCD == 2) {
      if(frmInspection->cbFreezeRL->Checked == false)
         return;
      //no = 2;
      width = GigaWidth;
      height = GigaHeight;
      if(ImageRightFactor >= 2) {
         ImageRightFactor--;

         //pnlRightFactor->Caption = IntToStr(ImageRightFactor) + "x";
         AnsiString str;
         double stdratio = 0.334;
         double fratio = stdratio * ImageRightFactor;
         int iratio = fratio * 100;
         str.sprintf("%d\%",iratio);
         pnlRightFactor->Caption = str;

         pnlRightFactor->Refresh();
         w = width / ImageRightFactor;
         h = height / ImageRightFactor;
         imRightCCD->Picture->Bitmap->Width = w;
         imRightCCD->Picture->Bitmap->Height = h;

         double fdw,fdh;
         fdw = w - double(width)/(ImageRightFactor+1);
         fdw /= 2;
         fdh = h - double(height)/(ImageRightFactor+1);
         fdh /= 2;
         fdw *= ((double)ImageRightXPos / ((double)imRightCCD->Width/2));
         fdh *= ((double)ImageRightYPos / ((double)imRightCCD->Height/2));
         ImageRightSX -= fdw;
         ImageRightSY -= fdh;
         if(ImageRightSX < 0)
            ImageRightSX = 0;
         if(ImageRightSY < 0)
            ImageRightSY = 0;

         if((ImageRightSX + w) > width)
            ImageRightSX = width - w;
         if((ImageRightSY + h) > height)
            ImageRightSY = height - h;

         frnMain->usb_ImageROIAry[no].SetPlacement(ImageRightSX,ImageRightSY,w,h);
         frnMain->usb_ImageROIAry[no].Draw(imRightCCD->Canvas->Handle);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
int x,y,w,h,dh;
int leftx,lefty,rightx,righty,value;
bool booldo = false;
AnsiString msg;

   x = MousePos.x;
   y = MousePos.y;

   // 對應到Form上
   int fx = frmInspection->Left;
   int fy = frmInspection->Top;
   x -= fx;
   y -= fy;
   x -= 4;
   y -= 25;

   pnlWheelMsg->Caption = "Wheel Up";
   Handled = true;
   if(pcInspection->ActivePage == tsMicroInspection) {
      frnMain->WriteSystemLog("Micro: Wheel Up");
      if((boolCCDImageLoadedAry[0] == false && boolBaslerImageLoadedAry[0] == false) || frmInspection->cbFreeze->Checked == false)
         return;

      frnMain->WriteSystemLog("Micro: Wheel Up Active");

      // 放大
      leftx = Panel3->Left + imInspection->Left;
      rightx = leftx + imInspection->Width;
      lefty = Panel3->Top + imInspection->Top;
      righty = lefty + imInspection->Height;
      msg.sprintf("sx,ex,sy,ey: x,y=%d,%d,%d,%d: %d,%d",leftx,rightx,lefty,righty,x,y);
      frnMain->WriteSystemLog(msg);
      if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
         frnMain->WriteSystemLog("Micro: Wheel Zoom");
         boolFormWheel = true;
         WheelCCD = 0;
         imAddClick(this);
         boolFormWheel = false;
      }
   }
   else if(pcInspection->ActivePage == tsMacroInspection) {
      if(frmInspection->cbFreezeRL->Checked == true) {
         if(boolCCDImageLoadedAry[1] == true || boolBaslerImageLoadedAry[1] == true) {
            // Area
            leftx = pnlLeftCCD->Left + imLeftCCD->Left;
            rightx = leftx + imLeftCCD->Width;
            lefty = pnlLeftCCD->Top + imLeftCCD->Top;
            righty = lefty + imLeftCCD->Height;
            // Left CCD
            if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
               boolFormWheel = true;
               WheelCCD = 1;
               imAddClick(this);
               boolFormWheel = false;
            }
         }
         // Right CCD
         if(boolCCDImageLoadedAry[2] == true || boolBaslerImageLoadedAry[2] == true) {
            leftx = pnlRightCCD->Left + imRightCCD->Left;
            rightx = leftx + imRightCCD->Width;
            lefty = pnlRightCCD->Top + imRightCCD->Top;
            righty = lefty + imRightCCD->Height;
            // Right CCD
            if(x >= leftx && x <= rightx && y >= lefty && y <= righty) {
               boolFormWheel = true;
               WheelCCD = 2;
               imAddClick(this);
               boolFormWheel = false;
            }
         }
      }

      // Left/Right Light
      w = pnlLeftMacroLamp->Width;
      h = pnlLeftMacroLamp->Height;
      leftx = pcInspection->Left + pnlMacroInspection->Left + 4 + pnlLeftMacroLamp->Left;
      lefty = pcInspection->Top + pnlMacroInspection->Top + 30 + pnlLeftMacroLamp->Top;
      rightx = pcInspection->Left + pnlMacroInspection->Left + 4 + pnlRightMacroLamp->Left;
      righty = pcInspection->Top + pnlMacroInspection->Top + 30 + pnlRightMacroLamp->Top;
      // Up
      if((x >= leftx && x <= (leftx+w)) && (y >= lefty && y <= (lefty+h))) {
         pnlLeftMacroLamp->Color = clGray;
         if(frnMain->tbLED2CH1->Position < 253)
            frnMain->tbLED2CH1->Position += 2;
         else
            frnMain->tbLED2CH1->Position = 255;
         frnMain->pnlLED2CH1->Caption = IntToStr(frnMain->tbLED2CH1->Position);
         frnMain->LED2CH1Value = frnMain->tbLED2CH1->Position;
         pnlLeftLight->Caption = frnMain->tbLED2CH1->Position;
         frnMain->SetLED2Value(1,frnMain->LED2CH1Value);
      }
      else {
         pnlLeftMacroLamp->Color = clSilver;
      }
      if((x >= rightx && x <= (rightx+w)) && (y >= righty && y <= (righty+h))) {
         pnlRightMacroLamp->Color = clGray;
         if(frnMain->tbLED2CH2->Position < 253)
            frnMain->tbLED2CH2->Position += 2;
         else
            frnMain->tbLED2CH2->Position = 255;
         frnMain->pnlLED2CH2->Caption = IntToStr(frnMain->tbLED2CH2->Position);
         frnMain->LED2CH2Value = frnMain->tbLED2CH2->Position;
         pnlRightLight->Caption = frnMain->tbLED2CH2->Position;
         frnMain->SetLED2Value(2,frnMain->LED2CH2Value);
      }
      else {
         pnlRightMacroLamp->Color = clSilver;
      }

   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroJudgPasseClick(TObject *Sender)
{

   // 2023 2 23p - chc 分離Macro/Micro
   //frnMain->iWaitJudge = 1;
   frnMain->iWaitMacroJudge = 1;

   // Hirata
   if(frnMain->RobotMode != ROBOT_MODE_TAZMO) {

      // 2023 2 17 - chc 禁止op
      tsMacroInspection->Enabled = false;

      if(btnMacroInnerStop->Enabled == true) {
         pnlMacroTopResult->Caption = "Pass";
         MacroTopResult = JUDGE_PASS;

         // 2023 2 17 - chc Log
         frnMain->WriteSystemLog("Macro-Pass: Inner Judged. do Stop");

         // 2023 2 17 - chc 要在外面做
         btnMacroInnerStopClick(this);
         // Error
         if(pnlMacroInnerStop->Color != clLime) {

         }

         // 2023 2 16 - chc auto Back
         if(pnlMacroBackResult->Caption == "NA") {
            pnlWheelMsg->Caption = "To Back...";

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Pass: Do ToBack...");

            // Stop
            // 2023 2 17 - chc 要在外面做
            //btnMacroInnerStopClick(this);
            //// Error
            //if(pnlMacroInnerStop->Color != clLime) {
            //
            //}

            // To Back
            btnMacroOuterPositionClick(this);
            // Error
            if(pnlMacroOuterPosition->Color != clLime) {

            }
            pnlWheelMsg->Caption = "To Back done.";

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Pass: Do ToBack done.");

            // 2023 4 5 - chc 設定Top/Back
            frnMain->rgSide->ItemIndex = WAFER_BACK;
            frnMain->SetMacroParameter();

            // 2023 2 17 - chc Wait Back Judge
            tsMacroInspection->Enabled = true;
            return;

         }

         // 2023 2 17 - chc Log
         else
            frnMain->WriteSystemLog("Macro-Pass: Back Judged.");

      }
      if(btnMacroOuterStop->Enabled == true) {
         pnlMacroBackResult->Caption = "Pass";
         MacroBackResult = JUDGE_PASS;

         // 2023 2 17 - chc Log
         frnMain->WriteSystemLog("Macro-Pass: Outer Judged. do Stop");

         // 2023 2 17 - chc 要做Stop
         btnMacroOuterStopClick(this);
         // Error
         if(pnlMacroOuterStop->Color != clLime) {

         }

      }

      if(frnMain->boolInInspecting == true) {
         if(pnlMacroTopResult->Caption != "NA" && pnlMacroBackResult->Caption != "NA") {

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Pass: All Judged.");

            if(btnMacroInnerStop->Enabled == true) {

               // 2023 2 17 - chc Log
               frnMain->WriteSystemLog("Macro-Pass: Inner Stop.");

               btnMacroInnerStopClick(this);
            }
            else if(btnMacroOuterStop->Enabled == true) {

               // 2023 2 17 - chc Log
               frnMain->WriteSystemLog("Macro-Pass: Outer Stop.");

               btnMacroOuterStopClick(this);
            }
            // 不做Home
            //btnMacroHomeClick(this);
            // 2023 2 23p - chc 分離Macro/Micro
            //frnMain->iWaitJudge = 3;
            frnMain->iWaitMacroJudge = 3;

            // 2023 4 5 - chc 設定Top/Back
            frnMain->rgSide->ItemIndex = WAFER_TOP;

            Close();
         }
      }

      // 2023 2 17 - chc 禁止op
      tsMacroInspection->Enabled = true;

      return;
   }

   if(frnMain->rgSide->ItemIndex == WAFER_TOP) {
      pnlMacroTopResult->Caption = "Pass";
      MacroTopResult = JUDGE_PASS;
      frnMain->WriteSystemLog("Macro: Top judge to pass");

      // 2023 1 11 - chc 有Back=>自動翻面
      if(frnMain->boolInInspecting == true) {
         frnMain->WriteSystemLog("Macro: in inspecting, to Inverse?");
         if(frnMain->boolDoMacroBackCenter == true) {
            frnMain->WriteSystemLog("Macro: Back selected, do Inverse");
            //btnMacroInverseClick(this);
            tsMacroInspection->Enabled = false;

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = true;
            pnlMacroJudageDone->Caption = "To Back-Macro...";

            DoInverse();

            // 2023 4 5 - chc Switch Macro Parameter
            frnMain->SetMacroParameter();

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = false;

            // 2023 3 30 - chc AutoSave
            if(frmInspection->cbAutoSaveMacro->Checked == true) {
               // Inspection: 若為AutoSave則要自動Save及離開: btnMacroTopToClick
               if(frnMain->DoMacroAuto() == true) {
                  frnMain->WriteSystemLog("MacroBack: AutoSave done.");
               }
               else {
                  frnMain->WriteSystemLog("MacroBack: AutoSave done.");
               }

               // 2023 4 26 - chc 顯示翻面訊息
               pnlMacroJudageDone->Visible = true;
               pnlMacroJudageDone->Caption = "Macro Homing...";

               btnMacroHomeClick(this);

               // 2023 4 26 - chc 顯示翻面訊息
               pnlMacroJudageDone->Caption = "Wafer Transfering...";

               frnMain->iWaitMacroJudge = 3;
            }

            tsMacroInspection->Enabled = true;
         }
         // 2023 1 11 - chc 無Back=>自動結束
         else {

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = true;
            pnlMacroJudageDone->Caption = "Macro Homing...";

            frnMain->WriteSystemLog("Macro: Judge done.");
            btnMacroHomeClick(this);

            // 2023 2 23p - chc 分離Macro/Micro
            //frnMain->iWaitJudge = 3;
            frnMain->iWaitMacroJudge = 3;

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Caption = "Wafer Transfering...";

         }
      }

   }
   else {
      pnlMacroBackResult->Caption = "Pass";
      MacroBackResult = JUDGE_PASS;
      frnMain->WriteSystemLog("Macro: Back judge to pass");
   }
   if(pnlMacroTopResult->Caption != "NA" && pnlMacroBackResult->Caption != "NA") {

      // 2023 1 11 - chc 自動結束
      //btnMacroDone->Enabled = true;
      if(frnMain->boolInInspecting == true) {

         // 2023 4 26 - chc 顯示翻面訊息
         pnlMacroJudageDone->Visible = true;
         pnlMacroJudageDone->Caption = "Macro Homing...";

         btnMacroHomeClick(this);

         // 2023 2 23p - chc 分離Macro/Micro
         //frnMain->iWaitJudge = 3;
         frnMain->iWaitMacroJudge = 3;

         // 2023 4 26 - chc 顯示翻面訊息
         pnlMacroJudageDone->Caption = "Wafer Transfering...";

      }

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroJudgRejectClick(TObject *Sender)
{

   // 2023 2 23p - chc 分離Macro/Micro
   //frnMain->iWaitJudge = 2;
   frnMain->iWaitMacroJudge = 2;

   // Hirata
   if(frnMain->RobotMode != ROBOT_MODE_TAZMO) {

      // 2023 2 17 - chc 禁止op
      tsMacroInspection->Enabled = false;

      if(btnMacroInnerStop->Enabled == true) {
         pnlMacroTopResult->Caption = "Reject";
         MacroTopResult = JUDGE_REJECT;

         // 2023 2 17 - chc Log
         frnMain->WriteSystemLog("Macro-Reject: Inner Judged. do Stop");

         // 2023 2 17 - chc 要在外面做
         btnMacroInnerStopClick(this);
         // Error
         if(pnlMacroInnerStop->Color != clLime) {

         }

         // 2023 2 16 - chc auto Back
         // 2023 2 17 - chc 是NA才要做
         //if(pnlMacroBackResult->Caption != "NA") {
         if(pnlMacroBackResult->Caption == "NA") {

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Reject: Do ToBack...");

            // Stop
            // 2023 2 17 - chc 要在外面做
            //btnMacroInnerStopClick(this);
            //// Error
            //if(pnlMacroInnerStop->Color != clLime) {
            //
            //}

            // To Back
            btnMacroOuterPositionClick(this);

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Reject: Do ToBack done.");
            pnlWheelMsg->Caption = "To Back done.";

            // Error
            if(pnlMacroOuterPosition->Color != clLime) {

            }
            tsMacroInspection->Enabled = true;

            // 2023 4 5 - chc 設定Top/Back
            frnMain->rgSide->ItemIndex = WAFER_BACK;
            frnMain->SetMacroParameter();

            // 2023 2 17 - chc Wait Back Judge
            return;

         }

         // 2023 2 17 - chc Log
         else
            frnMain->WriteSystemLog("Macro-Reject: Back Judged.");

      }
      if(btnMacroOuterStop->Enabled == true) {
         pnlMacroBackResult->Caption = "Reject";
         MacroBackResult = JUDGE_REJECT;

         // 2023 2 17 - chc Log
         frnMain->WriteSystemLog("Macro-Reject: Outer Judged. do Stop");

         // 2023 2 17 - chc 要做Stop
         btnMacroOuterStopClick(this);
         // Error
         if(pnlMacroOuterStop->Color != clLime) {

         }

      }
      if(frnMain->boolInInspecting == true) {
         if(pnlMacroTopResult->Caption != "NA" && pnlMacroBackResult->Caption != "NA") {

            // 2023 2 17 - chc Log
            frnMain->WriteSystemLog("Macro-Reject: All Judged.");

            if(btnMacroInnerStop->Enabled == true) {

               // 2023 2 17 - chc Log
               frnMain->WriteSystemLog("Macro-Reject: Inner Stop.");

               btnMacroInnerStopClick(this);
            }
            else if(btnMacroOuterStop->Enabled == true) {

               // 2023 2 17 - chc Log
               frnMain->WriteSystemLog("Macro-Reject: Outer Stop.");

               btnMacroOuterStopClick(this);
            }
            // 不做Home
            //btnMacroHomeClick(this);

            // 2023 2 23p - chc 分離Macro/Micro
            //frnMain->iWaitJudge = 3;
            frnMain->iWaitMacroJudge = 3;

            // 2023 4 5 - chc 設定Top/Back
            frnMain->rgSide->ItemIndex = WAFER_TOP;

            Close();
         }
      }

      // 2023 2 17 - chc 禁止op
      tsMacroInspection->Enabled = true;

      return;
   }

   if(frnMain->rgSide->ItemIndex == WAFER_TOP) {
      pnlMacroTopResult->Caption = "Reject";
      MacroTopResult = JUDGE_REJECT;

      // 2023 2 10 - chc 有Back=>自動翻面
      frnMain->WriteSystemLog("Macro: Top judge to Reject");
      if(frnMain->boolInInspecting == true) {
         frnMain->WriteSystemLog("Macro: in inspecting, to Inverse?");
         if(frnMain->boolDoMacroBackCenter == true) {
            frnMain->WriteSystemLog("Macro: Back selected, do Inverse.");
            tsMacroInspection->Enabled = false;

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = true;
            pnlMacroJudageDone->Caption = "To Back-Macro...";

            DoInverse();

            // 2023 4 5 - chc Switch Macro Parameter
            frnMain->SetMacroParameter();

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = false;

            // 2023 3 30 - chc AutoSave
            // => 先保留: Reject不會做Save
            /*
            if(frmInspection->cbAutoSaveMacro->Checked == true) {
               // Inspection: 若為AutoSave則要自動Save及離開: btnMacroTopToClick
               if(DoMacroAuto() == true) {
                  frnMain->WriteSystemLog("MacroBack: AutoSave done.");
               }
               else {
                  frnMain->WriteSystemLog("MacroBack: AutoSave done.");
               }
               frnMain->iWaitMacroJudge = 3;
               btnMacroHomeClick(this);
            }
            */
            
            tsMacroInspection->Enabled = true;
         }
         // 無Back=>自動結束
         else {
            frnMain->WriteSystemLog("Macro: Judge done.");

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Visible = true;
            pnlMacroJudageDone->Caption = "Macro Homing...";

            btnMacroHomeClick(this);

            // 2023 2 23p - chc 分離Macro/Micro
            //frnMain->iWaitJudge = 3;
            frnMain->iWaitMacroJudge = 3;

            // 2023 4 26 - chc 顯示翻面訊息
            pnlMacroJudageDone->Caption = "Wafer Transfering...";

         }
      }

   }
   else {
      pnlMacroBackResult->Caption = "Reject";
      MacroBackResult = JUDGE_REJECT;
      frnMain->WriteSystemLog("Macro: Back judge to Reject");
   }
   if(pnlMacroTopResult->Caption != "NA" && pnlMacroBackResult->Caption != "NA") {

      // 2023 1 11 - chc 自動結束
      //btnMacroDone->Enabled = true;

      // 2023 2 10 - chc 自動結束
      if(frnMain->boolInInspecting == true) {

         // 2023 4 26 - chc 顯示翻面訊息
         pnlMacroJudageDone->Visible = true;
         pnlMacroJudageDone->Caption = "Macro Homing...";

         btnMacroHomeClick(this);

         // 2023 2 23p - chc 分離Macro/Micro
         //frnMain->iWaitJudge = 3;
         frnMain->iWaitMacroJudge = 3;

         // 2023 4 26 - chc 顯示翻面訊息
         pnlMacroJudageDone->Caption = "Wafer Transfering...";

      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::btnMicroJudgPassClick(TObject *Sender)
{

   // 2023 2 23p - chc 分離Macro/Micro
   //frnMain->iWaitJudge = 1;
   frnMain->iWaitMicroJudge = 1;

   if(frnMain->boolInInspecting == true) {
      MicroResult = JUDGE_PASS;
      UpdateResult();
   }

   // 2023 4 5 - chc 存圖檔
   SaveResultMap(true);

   // 2023 2 12 - chc 此時不關閉
   //Close();

   // 2023 4 26 - chc 顯示翻面訊息
   pnlMicroJudageDone->Visible = true;
   pnlMicroJudageDone->Caption = "Wafer Transfering...";

   // 2023 2 12 - chc 雙片: 確認已完成NextWafer
   if(frnMain->cbDoubleWafer->Checked == true && frnMain->boolInInspecting == true) {
      // 有同步取片?
      if(boolGetNextWafer == true && boolGetNextWaferDone == false) {
         frmInspection->pnlLoadingNextWafer->Visible = true;
         frmInspection->tsMicroInspection->Enabled = false;
      }
   }

   // 2023 4 12 - chc 在Inspecion中且Recipe AF有啟動就啟動AF
   if(frnMain->boolInInspecting == true) {
      frnMain->WriteSystemLog("frmInspection(btnMicroJudgPassClick): Set AF to false");

      // 2023 4 3 - chc Assign cbAF會觸發Click
      //frmInspection->cbAF->Checked = false;
      //frnMain->SwitchAF(false);
      if(frmInspection->cbAF->Checked == true)
         frmInspection->cbAF->Checked = false;
      else {
         frmInspection->pnlAF->Color = clSilver;
         frnMain->SwitchAF(false);
      }

   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMicroJudgRejectClick(TObject *Sender)
{

   // 2023 2 23p - chc 分離Macro/Micro
   //frnMain->iWaitJudge = 2;
   frnMain->iWaitMicroJudge = 2;

   if(frnMain->boolInInspecting == true) {
      MicroResult = JUDGE_REJECT;
      UpdateResult();
   }

   // 2023 4 5 - chc 存圖檔
   SaveResultMap(false);

   // 2023 2 12 - chc 此時不關閉
   //Close();

   // 2023 4 26 - chc 顯示翻面訊息
   pnlMicroJudageDone->Visible = true;
   pnlMicroJudageDone->Caption = "Wafer Transfering...";

   // 2023 2 12 - chc 雙片: 確認已完成NextWafer
   if(frnMain->cbDoubleWafer->Checked == true && frnMain->boolInInspecting == true) {
      // 有同步取片?
      if(boolGetNextWafer == true && boolGetNextWaferDone == false) {
         frmInspection->pnlLoadingNextWafer->Visible = true;
         frmInspection->tsMicroInspection->Enabled = false;
      }
   }

   // 2023 4 12 - chc 在Inspecion中且Recipe AF有啟動就啟動AF
   if(frnMain->boolInInspecting == true) {
      frnMain->WriteSystemLog("frmInspection(btnMicroJudgRejectClick): Set AF to false");

      // 2023 4 3 - chc Assign cbAF會觸發Click
      //frmInspection->cbAF->Checked = false;
      //frnMain->SwitchAF(false);
      if(frmInspection->cbAF->Checked == true)
         frmInspection->cbAF->Checked = false;
      else {
         frmInspection->pnlAF->Color = clSilver;
         frnMain->SwitchAF(false);
      }

   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroHomeClick(TObject *Sender)
{

   gbMacroOperation->Enabled = false;
   pnlMacroHome->Color = clSilver;
   frnMain->btnMacroHomeClick(this);
   pnlMacroHome->Color = frnMain->pnlMacroHome->Color;
   gbMacroOperation->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerPositionClick(TObject *Sender)
{

   frmInspection->tsMacroInspection->Enabled = false;
   pnlMacroInnerPosition->Color = clSilver;
   frnMain->btnMacroInnerPositionClick(this);
   pnlMacroInnerPosition->Color = frnMain->pnlMacroInnerPosition->Color;
   if(pnlMacroInnerPosition->Color == clLime) {
      btnMacroInnerPosition->Enabled = false;
      SetMainFun(false);
      SetTopFun(true);
   }
   frnMain->rgJoystickMode->ItemIndex = 1;
   frnMain->WriteSystemLog("Joystick: to Macro-Top");
   btnMacroOuterPosition->Enabled = false;
   frmInspection->tsMacroInspection->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerStopClick(TObject *Sender)
{

   if(pnlMacroTopResult->Caption == "NA") {
      pnlWheelMsg->Caption = "Top Needs Judge!";
      return;
   }

   frmInspection->tsMacroInspection->Enabled = false;
   pnlMacroInnerStop->Color = clSilver;
   frnMain->btnMacroInnerStopClick(this);
   pnlMacroInnerStop->Color = frnMain->pnlMacroInnerStop->Color;

   // 2023 4 5 - chc 要設成Disable
   btnMacroInnerStop->Enabled = false;

   if(pnlMacroInnerStop->Color == clLime) {
      btnMacroInnerPosition->Enabled = true;
      btnMacroOuterPosition->Enabled = true;
      SetMainFun(true);
      SetTopFun(false);
   }
   frnMain->rgJoystickMode->ItemIndex = 0;
   frnMain->WriteSystemLog("Joystick: to Table");
   frmInspection->tsMacroInspection->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterPositionClick(TObject *Sender)
{

   frmInspection->tsMacroInspection->Enabled = false;
   pnlMacroOuterPosition->Color = clSilver;
   frnMain->btnMacroOuterPositionClick(this);
   pnlMacroOuterPosition->Color = frnMain->pnlMacroOuterPosition->Color;
   if(pnlMacroOuterPosition->Color == clLime) {
      btnMacroOuterPosition->Enabled = false;
      SetMainFun(false);
      SetBackFun(true);
   }
   frnMain->rgJoystickMode->ItemIndex = 2;
   frnMain->WriteSystemLog("Joystick: to Macro-Back");
   btnMacroInnerPosition->Enabled = false;
   frmInspection->tsMacroInspection->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterStopClick(TObject *Sender)
{

   if(pnlMacroBackResult->Caption == "NA") {
      pnlWheelMsg->Caption = "Back Needs Judge!";
      return;
   }

   frmInspection->tsMacroInspection->Enabled = false;
   pnlMacroOuterStop->Color = clSilver;
   frnMain->btnMacroOuterStopClick(this);
   pnlMacroOuterStop->Color = frnMain->pnlMacroOuterStop->Color;

   if(pnlMacroOuterStop->Color == clLime) {
      btnMacroOuterPosition->Enabled = true;
      btnMacroInnerPosition->Enabled = true;
      SetMainFun(true);
      SetBackFun(false);
   }
   frnMain->rgJoystickMode->ItemIndex = 0;
   frnMain->WriteSystemLog("Joystick: to Table");
   frmInspection->tsMacroInspection->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroRobotPositionClick(TObject *Sender)
{

   pnlMacroRobotPosition->Color = clSilver;
   frnMain->btnMacroRobotPositionClick(this);
   pnlMacroRobotPosition->Color = frnMain->pnlMacroRobotPosition->Color;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroReadyPositionClick(TObject *Sender)
{

   pnlMacroReadyPosition->Color = clSilver;
   frnMain->btnMacroReadyPositionClick(this);
   pnlMacroReadyPosition->Color = frnMain->pnlMacroReadyPosition->Color;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerYMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerXBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerXBackMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerTMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerTMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCCWMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCCWMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCCWMouseExit(TObject *Sender)
{

   frnMain->btnMacroOuterTCCWMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCCWMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCCWMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerYBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerYBackMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerYBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerXMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXMoveMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerXMoveMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerXMoveMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerXMoveMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTBackMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerTBackMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTBackMouseExit(
      TObject *Sender)
{

   frnMain->btnMacroInnerTBackMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerTBackMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroInnerTBackMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCWMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCWMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCWMouseExit(TObject *Sender)
{

   frnMain->btnMacroOuterTCWMouseExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroOuterTCWMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   frnMain->btnMacroOuterTCWMouseUp(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroInnerYMoveMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{

   frnMain->btnMacroInnerYMoveMouseDown(Sender,Button,Shift,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::ResetTopOpeation()
{

   SetMainFun(true);
   btnMacroInnerPosition->Enabled = true;
   SetTopFun(false);
   btnMacroOuterPosition->Enabled = true;
   SetBackFun(false);
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::SetTopFun(bool mode)
{

   btnMacroInnerYMove->Enabled = mode;
   btnMacroInnerYBack->Enabled = mode;
   if(frnMain->RobotMode != ROBOT_MODE_TAZMO)
      btnMacroInnerStop->Enabled = mode;
   btnMacroInnerXBack->Enabled = mode;
   btnMacroInnerXMove->Enabled = mode;
   btnMacroInnerTMove->Enabled = mode;
   btnMacroInnerTBack->Enabled = mode;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::SetBackFun(bool mode)
{

   // 2022 12 23 - chc Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO)
      return;

   btnMacroOuterTCCW->Enabled = mode;
   btnMacroOuterTCW->Enabled = mode;
   btnMacroOuterStop->Enabled = mode;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::SetMainFun(bool mode)
{

   btnMacroHome->Enabled = mode;

   // 2022 12 23 - chc Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      btnMacroRobotPosition->Enabled = false;
      btnMacroReadyPosition->Enabled = false;
      btnMacroOuterPosition->Enabled = false;
      btnMacroOuterTCCW->Enabled = false;
      btnMacroOuterTCW->Enabled = false;
      btnMacroOuterStop->Enabled = false;
   }
   else {

      btnMacroRobotPosition->Enabled = mode;
      btnMacroReadyPosition->Enabled = mode;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgBXFMLensChange(TObject *Sender,
      int ButtonIndex)
{

   // 2023 4 4 - chc 自動調Shutter
   // 2023 4 17 - chc 不做
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

   // 2023 5 3 - chc Log
   frnMain->WriteSystemLog("rgBXFMLensChange: " + IntToStr(rgBXFMLens->ItemIndex));

   frnMain->rgBXFMLens->ItemIndex = rgBXFMLens->ItemIndex;
   frnMain->rgBXFMLensChange(Sender, ButtonIndex);

   pnlBXFMLens->Color = frnMain->pnlBXFMLens->Color;

   // 2023 5 3 - chc Wait
   // 2023 5 5 - chc 改用WaitTime
   //Sleep(500);
   //Application->ProcessMessages();
   frnMain->WaitTime(500);

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::tbBXFMLMPMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   no = tbBXFMLMP->Position;
   SetIntensity(no);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnDecClick(TObject *Sender)
{
int no;

   no = tbBXFMLMP->Position;
   if(no > 0) {
      no--;
      SetIntensity(no);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAddClick(TObject *Sender)
{
int no;

   no = tbBXFMLMP->Position;
   if(no < 120) {
      no++;
      SetIntensity(no);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnDefaultIntensityClick(TObject *Sender)
{
int no;

   no = 90;
   SetIntensity(no);
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::SetIntensity(int no)
{
TMouseButton Button;
TShiftState Shift;
int X,Y;

   X = 0;
   Y = 0;
   tbBXFMLMP->Position = no;

   // 2023 2 20 - chc Log
   frnMain->WriteSystemLog("frmInspection-SetIntensity: " + IntToStr(no));

   frnMain->tbBXFMLMP->Position = tbBXFMLMP->Position;
   frnMain->tbBXFMLMPMouseUp(this,Button,Shift,X,Y);
   pnlBXFMLMPValue->Caption =  frnMain->pnlBXFMLMPValue->Caption;
   pnlBXFMLampSet->Color =  frnMain->pnlBXFMLampSet->Color;

   // 2023 2 20 - chc Log
   frnMain->WriteSystemLog("frmInspection-SetIntensity: " + pnlBXFMLMPValue->Caption);

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnDefaultASClick(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS1Click(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS2Click(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS3Click(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS4Click(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS5Click(TObject *Sender)
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

void __fastcall TfrmInspection::btnAS6Click(TObject *Sender)
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
// 翻面: Macro有片才可以
void __fastcall TfrmInspection::btnMacroInverseClick(TObject *Sender)
{


   if(frnMain->boolInInspecting == true)
      return;

   // 獨立Function
   //gbMacroOperation->Enabled = false;
   //pnlMacroInverse->Color = clSilver;
   //frnMain->btnMacroInverseClick(this);
   //pnlMacroInverse->Color = frnMain->pnlMacroInverse->Color;
   //if(pnlMacroInverse->Color == clLime) {
   //   if(frnMain->rgSide->ItemIndex == WAFER_TOP)
   //      frmInspection->gbMacroResult->Caption = "Macro Result(Top)";
   //   else
   //      frmInspection->gbMacroResult->Caption = "Macro Result(Back)";
   //}
   //gbMacroOperation->Enabled = true;
   DoInverse();
}
//---------------------------------------------------------------------------
// 獨立Function
void __fastcall TfrmInspection::DoInverse()
{

   // 2023 2 22p - chc Log
   frnMain->WriteSystemLog("DoInverse() Enter...");

   gbMacroOperation->Enabled = false;
   pnlMacroInverse->Color = clSilver;
   frnMain->btnMacroInverseClick(this);
   pnlMacroInverse->Color = frnMain->pnlMacroInverse->Color;
   if(pnlMacroInverse->Color == clLime) {
      if(frnMain->rgSide->ItemIndex == WAFER_TOP)
         frmInspection->gbMacroResult->Caption = "Macro Result(Top)";
      else
         frmInspection->gbMacroResult->Caption = "Macro Result(Back)";
   }
   gbMacroOperation->Enabled = true;

   // 2023 2 22p - chc Log
   frnMain->WriteSystemLog("DoInverse() End.");

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroDoneClick(TObject *Sender)
{

   if(frnMain->boolInInspecting == true)
      return;

   if(pnlMacroTopResult->Caption != "NA" && pnlMacroBackResult->Caption != "NA") {
      btnMacroHomeClick(this);

      // 2023 2 23p - chc 分離Macro/Micro
      //frnMain->iWaitJudge = 3;
      frnMain->iWaitMacroJudge = 3;

      //Close();
   }
   else {
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::imLeftCCDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageLeftXPos = X;
   ImageLeftYPos = Y;
   if(Button == mbLeft) {
      ImageLeftLeftMouse = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imLeftCCDMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int gray,sx,sy;

   // 主畫面
   unsigned char r,g,b;
   TColor color;
   sx = 0;
   sy = 0;

   // 2023 3 17 - chc Ratio
   // 全影像比例
   double ratiox,ratioy;
   int x,y;
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImageLR->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imLeftCCD->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imLeftCCD->Height;
      x = X * ratiox;
      y = Y * ratioy;
      color = imLeftCCD->Canvas->Pixels[x][y];
   }
   else

      color = imLeftCCD->Canvas->Pixels[X][Y];
   b = (color & 0xff0000) >> 16;
   g = (color & 0x00ff00) >> 8;
   r = color & 0x0000ff;
   // 顯示游標值
   pnlLeftCCDR->Caption = r;
   pnlLeftCCDG->Caption = g;
   pnlLeftCCDB->Caption = b;
   pnlLeftCCDX->Caption = X + sx;
   pnlLeftCCDY->Caption = Y + sy;

   // Move
   if(ImageLeftLeftMouse == true && frmInspection->cbFreezeRL->Checked == true && cbCCDFullImageLR->Checked == false) {
      int dx,dy,sx,sy;
      int width,height,w,h,no,w1,h1;
      w1 = imLeftCCD->Width;
      h1 = imLeftCCD->Height;
      dx = X - ImageLeftXPos;
      dy = Y - ImageLeftYPos;
      width = GigaWidth;
      height = GigaHeight;

      // 2023 3 17 - chc 是指可視範圍
      //w = width / ImageLeftFactor;
      //h = height / ImageLeftFactor;
      w = w1 / ImageLeftFactor;
      h = h1 / ImageLeftFactor;

      // 2023 3 17 - chc 只校ImageFactor就可以
      //dx = (dx * width) / (w1 * ImageLeftFactor);
      //dy = (dy * height) / (h1 * ImageLeftFactor);
      dx = dx / ImageLeftFactor;
      dy = dy / ImageLeftFactor;

      sx = ImageLeftSX - dx;
      sy = ImageLeftSY - dy;
      if(sx > 0) {
         ImageLeftSX = sx;
      }
      else
         ImageLeftSX = 0;
      if(sy > 0) {
         ImageLeftSY = sy;
      }
      else
         ImageLeftSY = 0;
      if((ImageLeftSX + w) > width)
         ImageLeftSX = width - w;
      if((ImageLeftSY + h) > height)
         ImageLeftSY = height - h;

      no = 1;
      imLeftCCD->Picture->Bitmap->Width = w;
      imLeftCCD->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageLeftSX,ImageLeftSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imLeftCCD->Canvas->Handle);
      imLeftCCD->Refresh();
   }
   ImageLeftXPos = X;
   ImageLeftYPos = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imLeftCCDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageLeftLeftMouse = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::imRightCCDMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageRightXPos = X;
   ImageRightYPos = Y;
   if(Button == mbLeft) {
      ImageRightLeftMouse = true;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imRightCCDMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int gray,sx,sy;

   // 主畫面
   unsigned char r,g,b;
   TColor color;
   sx = 0;
   sy = 0;

   // 2023 3 17 - chc Ratio
   // 全影像比例
   double ratiox,ratioy;
   int x,y;
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImageLR->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imRightCCD->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imRightCCD->Height;
      x = X * ratiox;
      y = Y * ratioy;
      color = imRightCCD->Canvas->Pixels[x][y];
   }
   else

      color = imRightCCD->Canvas->Pixels[X][Y];
   b = (color & 0xff0000) >> 16;
   g = (color & 0x00ff00) >> 8;
   r = color & 0x0000ff;
   // 顯示游標值
   pnlRightCCDR->Caption = r;
   pnlRightCCDG->Caption = g;
   pnlRightCCDB->Caption = b;
   pnlRightCCDX->Caption = X + sx;
   pnlRightCCDY->Caption = Y + sy;

   // Move
   if(ImageRightLeftMouse == true && frmInspection->cbFreezeRL->Checked == true && cbCCDFullImageLR->Checked == false) {
      int dx,dy,sx,sy;
      int width,height,w,h,no,w1,h1;
      w1 = imRightCCD->Width;
      h1 = imRightCCD->Height;
      dx = X - ImageRightXPos;
      dy = Y - ImageRightYPos;
      width = GigaWidth;
      height = GigaHeight;

      // 2023 3 17 - chc 是指可視範圍
      //w = width / ImageRightFactor;
      //h = height / ImageRightFactor;
      w = w1 / ImageRightFactor;
      h = h1 / ImageRightFactor;

      // 2023 3 17 - chc 只校ImageFactor就可以
      //dx = (dx * width) / (w1 * ImageRightFactor);
      //dy = (dy * height) / (h1 * ImageRightFactor);
      dx = dx / ImageRightFactor;
      dy = dy / ImageRightFactor;

      sx = ImageRightSX - dx;
      sy = ImageRightSY - dy;
      if(sx > 0) {
         ImageRightSX = sx;
      }
      else
         ImageRightSX = 0;
      if(sy > 0) {
         ImageRightSY = sy;
      }
      else
         ImageRightSY = 0;
      if((ImageRightSX + w) > width)
         ImageRightSX = width - w;
      if((ImageRightSY + h) > height)
         ImageRightSY = height - h;

      no = 2;
      imRightCCD->Picture->Bitmap->Width = w;
      imRightCCD->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageRightSX,ImageRightSY,w,h);
      frnMain->usb_ImageROIAry[no].Draw(imRightCCD->Canvas->Handle);
      imRightCCD->Refresh();
   }
   ImageRightXPos = X;
   ImageRightYPos = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imRightCCDMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageRightLeftMouse = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnSaveLeftCCDClick(TObject *Sender)
{
AnsiString filename;
int row,col;

   row = 0;
   col = 0;
   pnlWheelMsg->Caption = "";
   if(frnMain->rgSide->ItemIndex == WAFER_TOP) {

      // 2023 4 25 - chc 存jpeg
      if(rgFileFormat->ItemIndex == 0)
         filename.sprintf("%s\\%s_%d_PSS_%d_%d_TL%d.jpeg",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                          frnMain->InspectWaferNo,col,row,TopLeftSeialNo+1);
      else

         filename.sprintf("%s\\%s_%d_PSS_%d_%d_TL%d.bmp",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                          frnMain->InspectWaferNo,col,row,TopLeftSeialNo+1);
      TopLeftSeialNo++;
   }
   else {

      // 2023 4 25 - chc 存jpeg
      if(rgFileFormat->ItemIndex == 0)
         filename.sprintf("%s\\%s_%d_PSS_%d_%d_BL%d.jpeg",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                          frnMain->InspectWaferNo,col,row,BackLeftSeialNo+1);
      else

         filename.sprintf("%s\\%s_%d_PSS_%d_%d_BL%d.bmp",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                          frnMain->InspectWaferNo,col,row,BackLeftSeialNo+1);
      BackLeftSeialNo++;
   }

   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0) {
      TJPEGImage *jpeg;
      jpeg = new TJPEGImage;
      jpeg->Assign(imLeftCCD->Picture->Graphic);
      jpeg->CompressionQuality = 95;
      jpeg->SaveToFile(filename);
      delete jpeg;
   }
   else

      imLeftCCD->Picture->Bitmap->SaveToFile(filename);

   pnlWheelMsg->Caption = "Saved: " + filename;
   return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnSaveRightCCDClick(TObject *Sender)
{
AnsiString filename;
int row,col;

   row = 0;
   col = 0;

   pnlWheelMsg->Caption = "";
   if(frnMain->rgSide->ItemIndex == WAFER_TOP) {

      // 2023 4 25 - chc 存jpeg
      if(rgFileFormat->ItemIndex == 0)
         filename.sprintf("%s\\%s_%d_PSS_%d_%d_TR%d.jpeg",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,TopRightSeialNo+1);
      else

         filename.sprintf("%s\\%s_%d_PSS_%d_%d_TR%d.bmp",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,TopRightSeialNo+1);
      TopRightSeialNo++;
   }
   else {

      // 2023 4 25 - chc 存jpeg
      if(rgFileFormat->ItemIndex == 0)
         filename.sprintf("%s\\%s_%d_PSS_%d_%d_BR%d.jpeg",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,BackRightSeialNo+1);
      else

         filename.sprintf("%s\\%s_%d_PSS_%d_%d_BR%d.bmp",frnMain->InspectMacroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,BackRightSeialNo+1);
      BackRightSeialNo++;
   }

   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0) {
      TJPEGImage *jpeg;
      jpeg = new TJPEGImage;
      jpeg->Assign(imRightCCD->Picture->Graphic);
      jpeg->CompressionQuality = 95;
      jpeg->SaveToFile(filename);
      delete jpeg;
   }
   else

      imRightCCD->Picture->Bitmap->SaveToFile(filename);
   pnlWheelMsg->Caption = "Saved: " + filename;
   return;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveLeftClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveRightClick(TObject *Sender)
{

   // 2023 3 22 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();

      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
//   #define JOG_LOW_SPEED        0
//   #define JOG_MIDDLE_SPEED     1
//   #define JOG_HIGH_SPEED       2
void __fastcall TfrmInspection::btnMoveSpeedClick(TObject *Sender)
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

void __fastcall TfrmInspection::btnSaveMainCCDClick(TObject *Sender)
{
AnsiString filename;
int no;

   pnlSystemMessage->Caption = "";
   // 2023 4 7 - chc col,row
   //filename.sprintf("%s\\%s_%d_PSS_%d_%d_%d.bmp",frnMain->InspectMicroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
   //                                    frnMain->InspectWaferNo,frnMain->InspectDieRow,frnMain->InspectDieCol,TopSeialNo+1);
   int col,row;

   // 2023 4 12 - chc From Save: boolFromSave
   if(boolFromSave == true) {
      int rno;
      rno = sgDefectList->Row;
      if(rno <= 0 || rno >= sgDefectList->RowCount-1)
         return;

      // 2023 4 25 - chc 加入Name
      //col = sgDefectList->Cells[4][rno].ToInt();
      //row = sgDefectList->Cells[5][rno].ToInt();
      col = sgDefectList->Cells[4+1][rno].ToInt();
      row = sgDefectList->Cells[5+1][rno].ToInt();

   }
   else {
      col = pnlChipCol->Caption.ToInt();
      row = pnlChipRow->Caption.ToInt();
   }
   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0)
      filename.sprintf("%s\\%s_%d_PSS_%d_%d_%d.jpeg",frnMain->InspectMicroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,TopSeialNo+1);
   else

      filename.sprintf("%s\\%s_%d_PSS_%d_%d_%d.bmp",frnMain->InspectMicroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,TopSeialNo+1);

   TopSeialNo++;

   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0) {
      TJPEGImage *jpeg;
      jpeg = new TJPEGImage;
      jpeg->Assign(imInspection->Picture->Graphic);
      jpeg->CompressionQuality = 95;
      jpeg->SaveToFile(filename);
      delete jpeg;
   }
   else

      imInspection->Picture->Bitmap->SaveToFile(filename);
   pnlSystemMessage->Caption = "Saved: " + filename;

   // 2023 4 12 - chc From Save: boolFromSave
   boolFromSave = false;

   return;

   // 固定為1
   no = 0;

   // 強制為True
   if(boolCCDImageLoadedAry[no] == true || boolBaslerImageLoadedAry[no] == true) {
      spCCDImage->InitialDir = frnMain->ImageDirectory;
      // 設定filter
      if(rgFileFormat->ItemIndex == 0)
         spCCDImage->Filter = "JPG(*.jpg)|*.jpg";
      else
         spCCDImage->Filter = "BMP(*.bmp)|*.bmp";
      if(spCCDImage->Execute()) {
         // 改變副檔名
         if(rgFileFormat->ItemIndex == 0)
            filename = ChangeFileExt(spCCDImage->FileName,".jpg");
         else
            filename = ChangeFileExt(spCCDImage->FileName,".bmp");
         // 是Gray才對
         if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
            if(rgFileFormat->ItemIndex == 0)
               frnMain->CCDImageAry[no].Save(filename.c_str(),E_FILE_FORMAT_GRAY_LEVEL_JPEG);
            else
               frnMain->CCDImageAry[no].Save(filename.c_str(),E_FILE_FORMAT_GRAY_LEVEL_BMP);
            // => Full & Part都存
            //partfilename = ChangeFileExt(spCCDImage->FileName,"-p.bmp");
            //frnMain->CCDImageROIAry[no].Save(partfilename.c_str(),E_FILE_FORMAT_GRAY_LEVEL_BMP);
         }
         else {
            if(rgFileFormat->ItemIndex == 0)
               frnMain->usb_ImageROIAry[no].Save(filename.c_str(),E_FILE_FORMAT_COLOR_BMP);
            else
               frnMain->usb_ImageROIAry[no].Save(filename.c_str(),E_FILE_FORMAT_COLOR_JPEG);
         }
         //pnlSystemMessage->Caption = "Image Saved.";
      }
   }
}
//---------------------------------------------------------------------------
// Left Up
void __fastcall TfrmInspection::btnMoveLeftUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
// Left Down
void __fastcall TfrmInspection::btnMoveLeftDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardXClick(this);
   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
// Right Up
void __fastcall TfrmInspection::btnMoveRightUpClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   frnMain->btnMoveYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
// Right Down
void __fastcall TfrmInspection::btnMoveRightDownClick(TObject *Sender)
{

   // 2023 3 2 - chc Die move:
   // 2023 3 23 - chc 改成目前位置移動
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

      // 2023 5 6 - chc 不需要
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

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      return;
   }

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveXClick(this);
   frnMain->btnBackwardYClick(this);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::UpdateResult()
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

   // Micro
   if(MicroResult == JUDGE_PASS)
      panelM->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(MicroResult == JUDGE_NONE)
      panelM->Color = clNavy;

   else
      panelM->Color = clMaroon;

   // Macro-Top
   if(pnlMacroTopResult->Caption == "Pass")
      MacroTopResult = JUDGE_PASS;

   // 2023 4 8 - chc Assign None
   else if(pnlMacroTopResult->Caption == "None")
      MacroTopResult = JUDGE_NONE;

   else
      MacroTopResult = JUDGE_REJECT;
   if(MacroTopResult == JUDGE_PASS)
      panelT->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(MacroTopResult == JUDGE_NONE)
      panelT->Color = clNavy;

   else
      panelT->Color = clMaroon;

   // Macro-Back
   if(pnlMacroBackResult->Caption == "Pass")
      MacroBackResult = JUDGE_PASS;

   // 2023 4 8 - chc Assign None
   else if(pnlMacroBackResult->Caption == "None")
      MacroBackResult = JUDGE_NONE;

   else
      MacroBackResult = JUDGE_REJECT;
   if(MacroBackResult == JUDGE_PASS)
      panelBC->Color = clGreen;

   // 2023 4 8 - chc Assign None
   else if(MacroBackResult == JUDGE_NONE)
      panelBC->Color = clNavy;

   else
      panelBC->Color = clMaroon;

}
//---------------------------------------------------------------------------
AnsiString IDName[256];
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
void __fastcall TfrmInspection::SetupBINCode()
{
FILE *pFile;
AnsiString fname,msg,sid,sname;
char buf[250];
int no,pos,len,id;

   // BINCode Define
   sgBINCode->ColWidths[0] = 30;
   sgBINCode->ColWidths[1] = 30;
   sgBINCode->ColWidths[2] = 20;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";
   sgBINCode->RowCount = 2;

   fname = frnMain->SystemDirectory + "\\BINCode.txt";
   pFile = fopen(fname.c_str(),"r");
   if(pFile) {
      sgBINCode->RowCount = 2;
      sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
      sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";
      sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
      sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";
      no = 1;

      rewind(pFile);
      while(!feof(pFile)) {
         //name[0] = 0;
         //fscanf(pFile,"%d %s\n",&id,name);
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
               id = sid.ToInt();
               if(id >= 1 && id <= 16) {
                  IDName[id-1] = sname;
               }
               sgBINCode->RowCount++;
               no++;
            }
         }
      }
      fclose(pFile);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgBINCodeDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   // 2023 4 1 - chc RowColor: 在DrawText前先做, 否則Text會被蓋住
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // 米色
      else
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00D1D1D1;
   }
   // 0x00B3B3B3
   if(ARow == 0 || ACol == 0) {
      ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00B3B3B3;             // 米色
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
      // 差量加上顏色
      if(ACol == 2) {
         // 2023 4 1 - chc 使用指定Color
         /*
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
         */
         ((TStringGrid*)Sender)->Canvas->Brush->Color = frmRecipeSet->RecipeBufferRun.BinCode.Color[ARow-1];

         Rect.Left += 3;
         Rect.Top += 5;
         Rect.Right -= 3;
         Rect.Bottom -= 5;
         ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
      }
   }
}
//---------------------------------------------------------------------------
// DefectList.txt
// No,ID,X,Y,Col,Row,
// 7 1332.5 1332.5 8 23 16;
//TiffFileName ZP1612010_8_PSS_8_1.JPG;
void __fastcall TfrmInspection::SetupDefectList()
{
FILE *pFile;
AnsiString fname,msg,tmp;
char sid[50],sx[50],sy[50],scol[50],srow[50],sno[50],buf[200],sw[20],sh[20],sa[20],sd[20],slens[20],sintensity[20];
int no,cnt,row,col,id,len;
double rfx,rfy,rfw,rfh,rfa,rfd;

// 2023 4 25 - chc 加入name
char sidname[50];

// 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
char sobservation[20],sdicvalue[20];

   // BINCode Define
   sgDefectList->ColWidths[0] = 30;
   sgDefectList->ColWidths[1] = 30;

   // 2023 4 25 - chc 加入name
   sgDefectList->ColWidths[2] = 70;

   sgDefectList->ColWidths[2+1] = 50;
   sgDefectList->ColWidths[3+1] = 50;
   sgDefectList->ColWidths[4+1] = 35;
   sgDefectList->ColWidths[5+1] = 35;
   sgDefectList->ColWidths[6+1] = 40;

   // 2023 2 28 - chc 加入Defect W/H/A
   sgDefectList->ColWidths[7+1] = 60;
   sgDefectList->ColWidths[8+1] = 60;
   sgDefectList->ColWidths[9+1] = 65;

   // 2023 4 7 - chc 加入Lens/Intensity
   sgDefectList->ColWidths[10+1] = 40;
   sgDefectList->ColWidths[11+1] = 40;

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   sgDefectList->ColWidths[12+1] = 40;
   sgDefectList->ColWidths[13+1] = 40;

   sgDefectList->Cells[0][0] = "No";
   sgDefectList->Cells[1][0] = "ID";

   // 2023 4 25 - chc 加入name
   sgDefectList->Cells[2][0] = "Name";

   sgDefectList->Cells[2+1][0] = "X";
   sgDefectList->Cells[3+1][0] = "Y";
   sgDefectList->Cells[4+1][0] = "Col";
   sgDefectList->Cells[5+1][0] = "Row";
   sgDefectList->Cells[6+1][0] = "Judge";

   // 2023 2 28 - chc 加入Defect W/H/A
   sgDefectList->Cells[7+1][0] = "XSize";
   sgDefectList->Cells[8+1][0] = "YSize";
   sgDefectList->Cells[9+1][0] = "Area";

   // 2023 4 7 - chc 加入Lens/Intensity
   sgDefectList->Cells[10+1][0] = "Lens";
   sgDefectList->Cells[11+1][0] = "Inten.";

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   sgDefectList->Cells[12+1][0] = "Obser.";
   sgDefectList->Cells[13+1][0] = "DICVa.";

   sgDefectList->RowCount = 2;

   fname = frnMain->SystemDirectory + "\\DefectList.txt";
   pFile = fopen(fname.c_str(),"r");
   if(pFile) {
      sgDefectList->RowCount = 2;

      // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
      /*
      sgDefectList->Cells[0][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[1][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[2][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[3][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[4][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[5][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[6][sgDefectList->RowCount-1] = "";

      // 2023 2 28 - chc 加入Defect W/H/A
      sgDefectList->Cells[7][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[8][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[9][sgDefectList->RowCount-1] = "";

      // 2023 4 7 - chc 加入Lens/Intensity
      sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
      sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
      */
      for(int i=0 ; i<sgDefectList->ColCount ; i++)
         sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

      no = 1;
      cnt = 0;

      rewind(pFile);
      while(!feof(pFile)) {
         cnt++;
         if(fgets(buf,200,pFile) > 0) {
            if(cnt == 3) {
               len = strlen(buf);
               if(len > 0)
                  buf[len-2] = '\n';

               // 2023 2 28 - chc 加入Defect W/H/A
               //if(sscanf(buf,"%s %s %s %s %s %s\n",sno,sx,sy,sid,scol,srow) != NULL) {
               // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
               //if(sscanf(buf,"%s %s %s %s %s %s %s %s %s %s %s %s\n",sno,sx,sy,scol,srow,sw,sh,sa,sd,sid,slens,sintensity) != NULL) {
               // 2023 4 25 - chc 加入name
               //if(sscanf(buf,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",sno,sx,sy,scol,srow,sw,sh,sa,sd,sid,slens,sintensity,sobservation,sdicvalue) != NULL) {
               if(sscanf(buf,"%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\n",sno,sx,sy,scol,srow,sw,sh,sa,sd,sid,slens,sintensity,sobservation,sdicvalue,sidname) != NULL) {

                  //sgDefectList->Cells[0][sgDefectList->RowCount-1] = IntToStr(no);
                  sgDefectList->Cells[0][sgDefectList->RowCount-1] = sno;

                  sgDefectList->Cells[1][sgDefectList->RowCount-1] = sid;

                  // 2023 4 25 - chc 加入name
                  sgDefectList->Cells[2][sgDefectList->RowCount-1] = sidname;

                  //sgDefectList->Cells[2][sgDefectList->RowCount-1] = sx;
                  rfx = atof(sx);
                  tmp.sprintf("%.1f",rfx);
                  sgDefectList->Cells[2+1][sgDefectList->RowCount-1] = tmp;

                  //sgDefectList->Cells[3][sgDefectList->RowCount-1] = sy;
                  rfy = atof(sy);
                  tmp.sprintf("%.1f",rfy);
                  sgDefectList->Cells[3+1][sgDefectList->RowCount-1] = tmp;

                  sgDefectList->Cells[4+1][sgDefectList->RowCount-1] = scol;
                  sgDefectList->Cells[5+1][sgDefectList->RowCount-1] = srow;

                  // 2023 2 28 - chc 加入Defect W/H/A
                  rfw = atof(sw);
                  tmp.sprintf("%.3f",rfw);
                  sgDefectList->Cells[7+1][sgDefectList->RowCount-1] = tmp;
                  rfh = atof(sh);
                  tmp.sprintf("%.3f",rfh);
                  sgDefectList->Cells[8+1][sgDefectList->RowCount-1] = tmp;
                  rfa = atof(sa);
                  tmp.sprintf("%.3f",rfa);
                  sgDefectList->Cells[9+1][sgDefectList->RowCount-1] = tmp;

                  // 2023 4 7 - chc 加入Lens/Intensity
                  sgDefectList->Cells[10+1][sgDefectList->RowCount-1] = slens;
                  sgDefectList->Cells[11+1][sgDefectList->RowCount-1] = sintensity;

                  // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
                  sgDefectList->Cells[12+1][sgDefectList->RowCount-1] = sobservation;
                  sgDefectList->Cells[13+1][sgDefectList->RowCount-1] = sdicvalue;

                  row = atoi(srow);
                  col = atoi(scol);
                  id = atoi(sid);
                  MapCode[row][col] = id;
                  sgDefectList->RowCount++;
                  no++;
               }
            }
         }
         if(cnt == 4)
            cnt = 0;
      }
      fclose(pFile);

      int tno;
      tno = sgDefectList->RowCount - 2;
      if(tno > 0) {
         MeasurePointNo = 1;

         // 2023 4 11 - chc 不顯示
         //shMeasurePoint->Visible = true;

         // 2023 4 25 - chc 加入name
         //pnlWorkCol->Caption = sgDefectList->Cells[4][MeasurePointNo].ToInt();
         //pnlWorkRow->Caption = sgDefectList->Cells[5][MeasurePointNo].ToInt();
         pnlWorkCol->Caption = sgDefectList->Cells[4+1][MeasurePointNo].ToInt();
         pnlWorkRow->Caption = sgDefectList->Cells[5+1][MeasurePointNo].ToInt();
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgDefectListDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // 米色
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

void __fastcall TfrmInspection::cbWBAutoClick(TObject *Sender)
{
bool boolwbauto;

   boolwbauto = cbWBAuto->Checked;
   if(frnMain->pnlISCCDStaus->Color == clLime)
      frnMain->ICImagingControl1->WhiteBalanceAuto = boolwbauto;
   if(boolwbauto == false)
      btnWBOneTouch->Enabled = true;
   else
      btnWBOneTouch->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnWBOneTouchClick(TObject *Sender)
{

   //frnMain->ICImagingControl1->VCDPropertyItems->FindItem(VCDID_WhiteBalance);
   if(frnMain->pnlISCCDStaus->Color == clLime) {
      cbWBAuto->Checked = true;
      frnMain->ICImagingControl1->WhiteBalanceAuto = true;
      Sleep(1500);
      frnMain->ICImagingControl1->WhiteBalanceAuto = false;
      cbWBAuto->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgObservationClick(TObject *Sender)
{

   // 2023 2 12 - chc 在同步時不做
   pnlObservation->Color = clSilver;
   if(boolGetNextWaferDone == false && boolGetNextWafer == true) {
      pnlSystemMessage->Caption = "DIC Not Available";
      pnlObservation->Color = clRed;
      return;
   }

   tsMicroInspection->Enabled = false;
   frnMain->WriteSystemLog("frmInspection: DIC切換");
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
   tsMicroInspection->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnNextPointClick(TObject *Sender)
{
int tno;

   // 2023 4 1 - chc 直接Assign : no = sgDefectList->Row
   int no;
   no = sgDefectList->Row;
   MeasurePointNo = no;

   tno = sgDefectList->RowCount - 2;
   if(tno > 0 && MeasurePointNo < tno) {
      MeasurePointNo++;

      // 2023 4 11 - chc 不顯示
      //shMeasurePoint->Visible = true;

      // 2023 4 25 - chc 加入Name
      //pnlWorkCol->Caption = sgDefectList->Cells[4][MeasurePointNo].ToInt();
      //pnlWorkRow->Caption = sgDefectList->Cells[5][MeasurePointNo].ToInt();
      pnlWorkCol->Caption = sgDefectList->Cells[4+1][MeasurePointNo].ToInt();
      pnlWorkRow->Caption = sgDefectList->Cells[5+1][MeasurePointNo].ToInt();

      // 2023 4 7 - chc XY Move
      int x,y;

      // 2023 4 25 - chc 加入Name
      //x = sgDefectList->Cells[2][MeasurePointNo].ToInt();
      //y = sgDefectList->Cells[3][MeasurePointNo].ToInt();
      x = sgDefectList->Cells[2+1][MeasurePointNo].ToInt();
      y = sgDefectList->Cells[3+1][MeasurePointNo].ToInt();

      frnMain->WriteSystemLog("Move: X,Y= " + IntToStr(x) + "," + IntToStr(y));
      pnlStageMoving->Visible = true;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();
      
      pnlStageMoving->Visible = false;
      sgDefectList->Row = MeasurePointNo;

      // 2023 4 12 - chc Switch Lens/Intensity
      // 2023 4 14 - chc Switch Lens/Intensity/Observation/DIC
      /*
      int lens,intensity;
      lens = sgDefectList->Cells[10][MeasurePointNo].ToInt();
      intensity = sgDefectList->Cells[11][MeasurePointNo].ToInt();
      if(lens != frmInspection->rgBXFMLens->ItemIndex) {
         frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Lens switch to " + IntToStr(lens));
         frmInspection->rgBXFMLens->ItemIndex = lens;
         rgBXFMLensChange(this,lens);
         Sleep(1000);
         Application->ProcessMessages();
      }
      if(intensity != frmInspection->tbBXFMLMP->Position) {
         frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Intensity switch to " + IntToStr(intensity));
         frmInspection->tbBXFMLMP->Position = intensity;
         frmInspection->SetIntensity(intensity);
         Sleep(1000);
         Application->ProcessMessages();
      }
      */
      SwitchInspectCondition(MeasurePointNo);

   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnPreviousPointClick(TObject *Sender)
{
int tno;

   // 2023 4 1 - chc 直接Assign : no = sgDefectList->Row
   int no;
   no = sgDefectList->Row;
   MeasurePointNo = no;

   tno = sgDefectList->RowCount - 2;
   if(tno > 0 && MeasurePointNo > 1) {
      MeasurePointNo--;

      // 2023 4 11 - chc 不顯示
      //shMeasurePoint->Visible = true;

      // 2023 4 25 - chc 加入Name
      //pnlWorkCol->Caption = sgDefectList->Cells[4][MeasurePointNo].ToInt();
      //pnlWorkRow->Caption = sgDefectList->Cells[5][MeasurePointNo].ToInt();
      pnlWorkCol->Caption = sgDefectList->Cells[4+1][MeasurePointNo].ToInt();
      pnlWorkRow->Caption = sgDefectList->Cells[5+1][MeasurePointNo].ToInt();

      // 2023 4 7 - chc XY Move
      int x,y;

      // 2023 4 25 - chc 加入Name
      //x = sgDefectList->Cells[2][MeasurePointNo].ToInt();
      //y = sgDefectList->Cells[3][MeasurePointNo].ToInt();
      x = sgDefectList->Cells[2+1][MeasurePointNo].ToInt();
      y = sgDefectList->Cells[3+1][MeasurePointNo].ToInt();

      frnMain->WriteSystemLog("Move: X,Y= " + IntToStr(x) + "," + IntToStr(y));
      pnlStageMoving->Visible = true;
      MoveToXY(x,y);
      UpdateMotionPosition();

      // 2023 5 6 - chc 不需要
      //UpdateCurrentPosition();

      pnlStageMoving->Visible = false;
      sgDefectList->Row = MeasurePointNo;

      // 2023 4 12 - chc Switch Lens/Intensity
      // 2023 4 14 - chc Switch Lens/Intensity/Observation/DIC
      /*
      int lens,intensity;
      lens = sgDefectList->Cells[10][MeasurePointNo].ToInt();
      intensity = sgDefectList->Cells[11][MeasurePointNo].ToInt();
      if(lens != frmInspection->rgBXFMLens->ItemIndex) {
         frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Lens switch to " + IntToStr(lens));
         frmInspection->rgBXFMLens->ItemIndex = lens;
         rgBXFMLensChange(this,lens);
         Sleep(1000);
         Application->ProcessMessages();
      }
      if(intensity != frmInspection->tbBXFMLMP->Position) {
         frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Intensity switch to " + IntToStr(intensity));
         frmInspection->tbBXFMLMP->Position = intensity;
         frmInspection->SetIntensity(intensity);
         Sleep(1000);
         Application->ProcessMessages();
      }
      */
      SwitchInspectCondition(MeasurePointNo);

   }
}
//---------------------------------------------------------------------------
int AdjustCenterX = 0;
int AdjustCenterY = 0;
int AdjustCenterDX = 0;
int AdjustCenterDY = 0;

void __fastcall TfrmInspection::sgDefectListDblClick(TObject *Sender)
{
int tno,no;

   tno = sgDefectList->RowCount - 2;
   no = sgDefectList->Row;
   if(tno > 0) {
      if(no >= 1 && no <= tno) {
         MeasurePointNo = no;

         // 2023 4 7 - chc XY Move: not need
         //shMeasurePoint->Visible = true;

         // 2023 4 1 - chc 直接Assign : no = sgDefectList->Row
         //pnlWorkCol->Caption = sgDefectList->Cells[4][MeasurePointNo].ToInt();
         //pnlWorkRow->Caption = sgDefectList->Cells[5][MeasurePointNo].ToInt();
         // 2023 4 25 - chc 加入Name
         //pnlWorkCol->Caption = sgDefectList->Cells[4][MeasurePointNo];
         //pnlWorkRow->Caption = sgDefectList->Cells[5][MeasurePointNo];
         pnlWorkCol->Caption = sgDefectList->Cells[4+1][MeasurePointNo];
         pnlWorkRow->Caption = sgDefectList->Cells[5+1][MeasurePointNo];

         // 2023 4 12 - chc Switch Lens/Intensity
         // 2023 4 14 - chc Switch Lens/Intensity/Observation/DIC
         /*
         int lens,intensity;
         lens = sgDefectList->Cells[10][MeasurePointNo].ToInt();
         intensity = sgDefectList->Cells[11][MeasurePointNo].ToInt();
         if(lens != frmInspection->rgBXFMLens->ItemIndex) {
            frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Lens switch to " + IntToStr(lens));
            frmInspection->rgBXFMLens->ItemIndex = lens;
            rgBXFMLensChange(this,lens);
            Sleep(1000);
            Application->ProcessMessages();
         }
         if(intensity != frmInspection->tbBXFMLMP->Position) {
            frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Intensity switch to " + IntToStr(intensity));
            frmInspection->tbBXFMLMP->Position = intensity;
            frmInspection->SetIntensity(intensity);
            Sleep(1000);
            Application->ProcessMessages();
         }
         */
         SwitchInspectCondition(MeasurePointNo);

         // 2023 4 7 - chc XY Move
         int x,y;

         // 2023 4 25 - chc 加入Name
         //x = sgDefectList->Cells[2][MeasurePointNo].ToInt();
         //y = sgDefectList->Cells[3][MeasurePointNo].ToInt();
         x = sgDefectList->Cells[2+1][MeasurePointNo].ToInt();
         y = sgDefectList->Cells[3+1][MeasurePointNo].ToInt();

         // 2023 5 3a - chc 要做中心點位移補償: AdjustCenterDX
         x += AdjustCenterDX;
         y += AdjustCenterDY;

         frnMain->WriteSystemLog("Move: X,Y= " + IntToStr(x) + "," + IntToStr(y));
         pnlStageMoving->Visible = true;
         MoveToXY(x,y);
         UpdateMotionPosition();

         // 2023 5 6 - chc 不需要
         //UpdateCurrentPosition();
         
         pnlStageMoving->Visible = false;

      }
   }


}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 0;
   frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgLeftCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 1;
   frnMain->rgCCDShutter->ItemIndex = rgLeftCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgRightCCDShutterClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 2;
   frnMain->rgCCDShutter->ItemIndex = rgRightCCDShutter->ItemIndex;
   frnMain->rgCCDShutterClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgDefectListClick(TObject *Sender)
{
int tno,no;

   tno = sgDefectList->RowCount - 2;
   no = sgDefectList->Row;
   if(tno > 0) {
      if(no >= 1 && no <= tno) {

         // 2023 4 25 - chc 加入Name
         //edSelectCol->Text = sgDefectList->Cells[4][no];
         //edSelectRow->Text = sgDefectList->Cells[5][no];
         edSelectCol->Text = sgDefectList->Cells[4+1][no];
         edSelectRow->Text = sgDefectList->Cells[5+1][no];

      }
   }
   AnsiString name = edChipHeight->Parent->Name;
   name = Panel18->Parent->Name;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCW1xClick(TObject *Sender)
{

   // 2023 4 19 - chc 改成吋動
   /*
   return;

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = true;
   */

   frnMain->btnBackwardTClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCCW1xClick(TObject *Sender)
{

   // 2023 4 19 - chc 改成吋動
   /*
   return;

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = true;
   */
   
   frnMain->btnMoveTClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   MousePosX = X;
   MousePosY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imCloseClick(TObject *Sender)
{

   frnMain->SetST(ST_INSPECTION,false);
   frnMain->pnlMainMode->Caption = "Manual";
   frnMain->pnlCurrentOperation->Caption = "Main";
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgMoveStepClick(TObject *Sender)
{

   // 2023 4 27 - chc MDie/SDie
   if(rgMoveStep->ItemIndex <= 3)

      frnMain->rgMoveStep->ItemIndex = rgMoveStep->ItemIndex;
   rgMoveStepMeasure->ItemIndex = rgMoveStep->ItemIndex;
}
//---------------------------------------------------------------------------
// Save XYZ Position
void __fastcall TfrmInspection::UpdateCurrentPosition()
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

   // 2023 3 21 - chc 修正: imWaferMap
   //shCurretnPosition->Left = cx - px - sw/2;
   //shCurretnPosition->Top = cy + py - sh/2;
   shCurretnPosition->Left = cx - px - sw/2 + imWaferMapSrc->Left;
   shCurretnPosition->Top = cy + py - sh/2 + imWaferMapSrc->Top;
   shCurretnPosition->Refresh();

}
//---------------------------------------------------------------------------
// move
void __fastcall TfrmInspection::imWaferMapSrcDblClick(TObject *Sender)
{
int x,y,sw,sh,cx,cy,sx,sy,tcx,tcy,w,h,px,py;
double ratio,fx,fy,sizew;
AnsiString msg;

   tsMicroInspection->Enabled = false;
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

   // Wafer中心點
   tcx = frnMain->edCenterX->Text.ToInt();
   tcy = frnMain->edCenterY->Text.ToInt();

   // 2023 5 3 - chc 校正
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

   // 2023 3 21 - chc 固定到此Chip中心點
   //x = tcx - ((MapX - cx) * ratio) / frnMain->X_RESOLUTION;
   //y = tcy + ((MapY - cy) * ratio) / frnMain->Y_RESOLUTION;
   int col,row,chipx,chipy;
   // 偏移量
   col = (MapX - COffsetX - Wafer.X) / Wafer.Dx;
   row = (MapY - COffsetY - Wafer.Y) / Wafer.Dy;
   if(MapX < (COffsetX + Wafer.X))
      col = -1;
   if(MapY < (COffsetY + Wafer.Y))
      row = -1;
   // 原點位置: GridNoY

   // 2023 5 3a - chc 用col,row來移動
   /*
   // 下: 180
   if(rgWaferAxisOrientation->ItemIndex == 1) {
      row = GridNoY - row - 1;
      // 最小Row,Col
      col -= iMinCol;
      row -= (iMinRow + 1);
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
      // Beep聲
      Beep(500);
   }
   */
   edSelectCol->Text = IntToStr(col);
   edSelectRow->Text = IntToStr(row);
   btnColRowToClick(this);

   UpdateCurrentPosition();
   tsMicroInspection->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;
}
//---------------------------------------------------------------------------
// Save XYZ Position
void __fastcall TfrmInspection::UpdateMotionPosition()
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
   pnlZMeasure->Caption = frnMain->pnlZ->Caption;
   if(boolDrawImage == true) {
      UpdateCurrentPosition();
   }

   // 2023 5 6 - chc Re-Entrant
   active = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnColRowToClick(TObject *Sender)
{
int col,row,w,h,dx,dy,cx,cy,ccol,crow,dcol,drow,x,y;
double fx,fy;
AnsiString msg;

   col = edSelectCol->Text.ToInt();
   row = edSelectRow->Text.ToInt();

   // 2023 5 4 - chc 檢查範圍
   if(col > pnlChipWNo->Caption.ToInt() || col < 0) {
      return;
   }
   if(row > pnlChipHNo->Caption.ToInt() || row < 0) {
      return;
   }

   // 2023 5 3a - chc 修正col/row
   // 2023 5 4 - chc 是0, 不是1
   if(rgWaferAxisOrientation->ItemIndex == 0) {
      row = GridNoY - row - iMinRow - 2;
   }

   tsMicroInspection->Enabled = false;
   tmUpdatePosition->Enabled = true;
   pnlStageMoving->Visible = true;

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

   // 2023 5 3 - chc 校正
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
      // Beep聲
      Beep(500);
   }
   UpdateCurrentPosition();
   tsMicroInspection->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;

   // 2023 5 5 - chc Update working col/row
   pnlWorkCol->Caption = IntToStr(col);
   pnlWorkRow->Caption = IntToStr(row);

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbAFClick(TObject *Sender)
{

   // 2023 4 13 - chc status
   pnlAF->Color = clSilver;

   tsMicroInspection->Enabled = false;
   if(cbAF->Checked == false)
      frnMain->WriteSystemLog("frmInspection(cbAFClick): Set AF to false");
   else
      frnMain->WriteSystemLog("frmInspection(cbAFClick): Set AF to true");
   frnMain->SwitchAF(cbAF->Checked);
   tsMicroInspection->Enabled = true;
}
//---------------------------------------------------------------------------
// AF unit
double AFUnit = 0.1;
void __fastcall TfrmInspection::pnlAFUnit01Click(TObject *Sender)
{

   pnlAFUnit01->Color = clGray;
   pnlAFUnit001->Color = clSilver;
   AFUnit = 0.1;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::pnlAFUnit001Click(TObject *Sender)
{

   pnlAFUnit01->Color = clSilver;
   pnlAFUnit001->Color = clGray;
   AFUnit = 0.01;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnGetAFPosClick(TObject *Sender)
{

   frnMain->btnA2MAPosClick(this);
   //Sleep(500);
   //pnlAFPos->Caption = frnMain->pnlA2MPos->Caption;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAFUpClick(TObject *Sender)
{
double fv;

   fv = edAFMovUnit->Text.ToDouble() * AFUnit;
   frnMain->AFMoveUp(fv);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAFDownClick(TObject *Sender)
{
double fv;

   fv = edAFMovUnit->Text.ToDouble() * AFUnit;
   frnMain->AFMoveDown(fv);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMacroQuitClick(TObject *Sender)
{

   frnMain->cbPause->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMicroQuitClick(TObject *Sender)
{

   frnMain->cbPause->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::tmUpdatePositionTimer(TObject *Sender)
{

   UpdateCurrentPosition();
}
//---------------------------------------------------------------------------
int OP1x = -1;
int OP1y = -1;
int OP2x = -1;
int OP2y = -1;
int OP3x = -1;
int OP3y = -1;
int OP4x = -1;
int OP4y = -1;
// 2023 1 22 - chc 3points - circle
// (a,b), (c,d), (e,f)
// (cx,cy)
// a1=a-c
// b1=b-d
// c1=(a*a+b*b-c*c-d*d)/2
// a2=a-e
// b2=b-f
// c2=(a*a+b*b-e*e-f*f)/2
// cx=(c1*b2-c2*b1) / (c1*a2-c2*a1)
// cy=(c1*a2-c2*a1) / (b1*a2-b2*a1)
bool boolCircle = false;
#define MEASURE_CIRCLE         0
#define MEASURE_ANGLE          1
#define MEASURE_LINE           2
#define MEASURE_VLINE          3
#define MEASURE_HORLINE        4
#define MEASURE_VERLINE        5
#define MEASURE_HEIGHT         6
#define MEASURE_NULL           -1
int MeasureMode = MEASURE_NULL;
int MeasureHeightLow = 0;
int MeasureHeightHigh = 1;

// 點圓心長度
double CircleCX1=0,CircleCY1=0;
double CirclePX1=0,CirclePY1=0;
double CircleCX2=0,CircleCY2=0;
double CirclePX2=0,CirclePY2=0;
void __fastcall TfrmInspection::mnPoint1Click(TObject *Sender)
{

   pnlCircleP1X->Caption = IntToStr(ImageXPos);
   pnlCircleP1Y->Caption = IntToStr(ImageYPos);
   pnlCircleP1Status->Color = clLime;
   shPoint1->Left = ImageXPos + imMeasure->Left - shPoint1->Width/2;
   shPoint1->Top = ImageYPos + imMeasure->Top - shPoint1->Height/2;
   if(cbPointSetDisplay->Checked == true)
      shPoint1->Visible = true;

   // Reset
   shPoint2->Visible = false;
   pnlPoint2->Visible = false;
   shPoint3->Visible = false;
   pnlPoint3->Visible = false;
   shCircle->Visible = false;

   // 加入說明
   pnlPoint1->Top = shPoint1->Top - pnlPoint1->Height - 2;
   pnlPoint1->Left = shPoint1->Left + shPoint1->Width + 2;
   if(cbPointSetDisplay->Checked == true)
      pnlPoint1->Visible = true;

   if(MeasureMode == MEASURE_CIRCLE)
      AddMeasureMessage(AnsiString("圓第1點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_ANGLE)
      AddMeasureMessage(AnsiString("角第1點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_LINE)
      AddMeasureMessage(AnsiString("線第1點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_VLINE)
      AddMeasureMessage(AnsiString("垂線第1點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_HEIGHT) {
      AddMeasureMessage(AnsiString("高度第1點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
      MeasureHeightLow = frnMain->edCounterZ->Text.ToInt();
      pnlZValue1->Caption = frnMain->edCounterZ->Text;
      pnlZMeasure->Caption = frnMain->edCounterZ->Text;
   }

   OP1x = ImageXPos;
   OP1y = ImageYPos;
   OP2x = -1;
   OP2y = -1;
   OP3x = -1;
   OP3y = -1;
   OP4x = -1;
   OP4y = -1;
   pnlData->Visible = false;

}
//---------------------------------------------------------------------------
// 2023 1 22 - chc DoubleClick
void __fastcall TfrmInspection::imMeasureDblClick(TObject *Sender)
{
int X,Y;

   X = ImageXPos;
   Y = ImageYPos;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbPointSetDisplayChange(TObject *Sender)
{
bool mode;

   if(cbPointSetDisplay->Checked == false)
      mode = false;
   else
      mode = true;

   // true不作動
   if(mode == true)
      return;

   // 設定點位: 5點
   pnlPointSet1->Visible = mode;
   pnlPointSet2->Visible = mode;
   pnlPointSet3->Visible = mode;
   pnlPointSet4->Visible = mode;
   pnlPointSet5->Visible = mode;
   shPointSet1->Visible = mode;
   shPointSet2->Visible = mode;
   shPointSet3->Visible = mode;
   shPointSet4->Visible = mode;
   shPointSet5->Visible = mode;
   // 點位: 3點
   // true不可以做
   if(mode == false) {
      shPoint1->Visible = mode;
      shPoint2->Visible = mode;
      shPoint3->Visible = mode;
      pnlPoint1->Visible = mode;
      pnlPoint2->Visible = mode;
      pnlPoint3->Visible = mode;
      // 圓
      shCircle->Visible = mode;
   }

   // 圓心: 2點
   shCircleCenter1->Visible = mode;
   shCircleCenter2->Visible = mode;
   pnlCircleCenter1->Visible = mode;
   pnlCircleCenter2->Visible = mode;
   // 線: 2點
   shLineSet1->Visible = mode;
   shLineSet2->Visible = mode;
   pnlLineSet1->Visible = mode;
   pnlLineSet2->Visible = mode;
   // 點圓心: 2點
   shCirclePoint1->Visible = mode;
   shCirclePoint2->Visible = mode;
   pnlCirclePoint1->Visible = mode;
   pnlCirclePoint2->Visible = mode;

   // 2016 12 18 - chc 圓虛線
   shCircleMark1->Visible = mode;
   shCircleMark2->Visible = mode;
}
//---------------------------------------------------------------------------
// 2023 1 22- chc 加入訊息 : mmMeasure
void __fastcall TfrmInspection::AddMeasureMessage(AnsiString msg)
{
AnsiString putmsg;

// 2023 2 13a - chc Test
return;

   putmsg = Now().FormatString("hh':'nn':'ss'-> '") + msg;
   mmMeasure->Lines->Insert(0,putmsg);
   if(mmMeasure->Lines->Count > 400) {
      for(int i=320 ; i<mmMeasure->Lines->Count ; i++)
         mmMeasure->Lines->Delete(i);
   }
   //mmMeasure->Lines->Insert(0,msg);
}
//---------------------------------------------------------------------------
// 2023 1 22 - chc 3points - circle
// (a,b), (c,d), (e,f)
// (cx,cy)
// a1=a-c
// b1=b-d
// c1=(a*a+b*b-c*c-d*d)/2
// a2=a-e
// b2=b-f
// c2=(a*a+b*b-e*e-f*f)/2
// cx=(c1*b2-c2*b1) / (c1*a2-c2*a1)
// cy=(c1*a2-c2*a1) / (b1*a2-b2*a1)
void __fastcall TfrmInspection::btnCircleClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_CIRCLE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnCircle->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = true;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "三點量測圓心與半徑/面積";
   //pnlCircle->Visible = true;
   //shCircle->Visible = false;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;

   // Visible
   laCircleCenter->Visible = true;
   pnlCircleCenterX->Visible = true;
   pnlCircleCenterY->Visible = true;
   laCircleRadius->Caption = "半徑";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   laCircleArea->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleAreaUnit->Caption = "um*um";
   else
      laCircleAreaUnit->Caption = "mm*mm";
   pnlCircleArea->Visible = true;
   laCircleAreaUnit->Visible = true;

   // 圓心編號
   rgCircleNo->Visible = true;

   // InVisible
   laAngle->Visible = false;
   pnlAngle->Visible = false;
   laAngleUnit->Visible = false;
   pnlAngle12->Visible = false;

   MeasureMode = MEASURE_CIRCLE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAngleClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_ANGLE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnAngle->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = true;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "三點量測夾角";
   //pnlCircle->Visible = true;
   //shCircle->Visible = false;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   MeasureMode = MEASURE_ANGLE;

   // InVisible
   laCircleCenter->Visible = false;
   pnlCircleCenterX->Visible = false;
   pnlCircleCenterY->Visible = false;
   laCircleRadius->Visible = false;
   pnlCircleRadius->Visible = false;
   laCircleRadiusUnit->Visible = false;
   laCircleArea->Visible = false;
   pnlCircleArea->Visible = false;
   laCircleAreaUnit->Visible = false;

   // 圓心編號
   rgCircleNo->Visible = false;

   // Visible
   laAngle->Visible = true;
   pnlAngle->Visible = true;
   laAngleUnit->Visible = true;
   pnlAngle12->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnPoint2Click(TObject *Sender)
{

   // 2023 1 29 - chc Draw
   if(MeasureMode == MEASURE_HORLINE) {
      int y;
      y = pnlCircleP1Y->Caption.ToInt();
      ImageYPos = y;
   }
   else if(MeasureMode == MEASURE_VERLINE) {
      int x;
      x = pnlCircleP1X->Caption.ToInt();
      ImageXPos = x;
   }

   pnlCircleP2X->Caption = IntToStr(ImageXPos);
   pnlCircleP2Y->Caption = IntToStr(ImageYPos);
   pnlCircleP2Status->Color = clLime;
   shPoint2->Left = ImageXPos + imMeasure->Left - shPoint2->Width/2;
   shPoint2->Top = ImageYPos + imMeasure->Top - shPoint2->Height/2;
   if(cbPointSetDisplay->Checked == true)
      shPoint2->Visible = true;

   // 說明
   pnlPoint2->Top = shPoint2->Top - pnlPoint2->Height - 2;
   pnlPoint2->Left = shPoint2->Left + shPoint2->Width + 2;
   if(cbPointSetDisplay->Checked == true)
      pnlPoint2->Visible = true;

   if(MeasureMode == MEASURE_CIRCLE)
      AddMeasureMessage(AnsiString("圓第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_ANGLE)
      AddMeasureMessage(AnsiString("角第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_LINE)
      AddMeasureMessage(AnsiString("線第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_VLINE)
      AddMeasureMessage(AnsiString("垂線第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_HORLINE)
      AddMeasureMessage(AnsiString("水平線第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_VERLINE)
      AddMeasureMessage(AnsiString("垂直線第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_HEIGHT)
      AddMeasureMessage(AnsiString("高度第2點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));

   // 2023 1 29 - chc Draw
   if(MeasureMode == MEASURE_ANGLE || MeasureMode == MEASURE_VLINE) {
      TRect Rect;
      int p1x,p1y,p2x,p2y;
      //imDraw->Visible = true;
      //imDraw->Top = imMeasure->Top;
      //imDraw->Left = imMeasure->Left;
      //imDraw->Width = imMeasure->Width;
      //imDraw->Height = imMeasure->Height;
      Rect.left = 0;
      Rect.top = 0;
      Rect.Right = imDraw->Width - 1;
      Rect.Bottom = imDraw->Height - 1;
      imDraw->Canvas->Brush->Color = clWhite;
      imDraw->Canvas->FillRect(Rect);
      imDraw->Canvas->Pen->Color = pnlLineColor->Color;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      imDraw->Canvas->Pen->Width = 2;
      imDraw->Canvas->MoveTo(p1x,p1y);
      imDraw->Canvas->LineTo(p2x,p2y);

      OP2x = ImageXPos;
      OP2y = ImageYPos;
      OP3x = -1;
      OP3y = -1;
   }
   if(MeasureMode == MEASURE_LINE || MeasureMode == MEASURE_HORLINE || MeasureMode == MEASURE_VERLINE || MeasureMode == MEASURE_HEIGHT) {
      pnlData->Visible = false;
      AddMeasureData();
   }
   else {
      pnlPoint2->Caption = " 點2";
      pnlPoint2->Width = 32;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnPoint3Click(TObject *Sender)
{

   pnlCircleP3X->Caption = IntToStr(ImageXPos);
   pnlCircleP3Y->Caption = IntToStr(ImageYPos);
   pnlCircleP3Status->Color = clLime;
   shPoint3->Left = ImageXPos + imMeasure->Left - shPoint3->Width/2;
   shPoint3->Top = ImageYPos + imMeasure->Top - shPoint3->Height/2;
   if(cbPointSetDisplay->Checked == true)
      shPoint3->Visible = true;

   // 說明
   pnlPoint3->Top = shPoint3->Top - pnlPoint3->Height - 2;
   pnlPoint3->Left = shPoint3->Left + shPoint3->Width + 2;
   if(cbPointSetDisplay->Checked == true)
      pnlPoint3->Visible = true;

   if(MeasureMode == MEASURE_CIRCLE)
      AddMeasureMessage(AnsiString("圓第3點(X,Y)Pixle: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_ANGLE)
      AddMeasureMessage(AnsiString("角第3點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));
   else if(MeasureMode == MEASURE_VLINE)
      AddMeasureMessage(AnsiString("垂線第3點(X,Y)Pixel: ") + IntToStr(ImageXPos) + "," + IntToStr(ImageYPos));

   if(MeasureMode == MEASURE_CIRCLE || MeasureMode == MEASURE_ANGLE || MeasureMode == MEASURE_VLINE) {
      pnlData->Visible = false;
      AddMeasureData();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCircleComputeClick(TObject *Sender)
{
double a,b,c,d,e,f,a1,b1,c1,a2,b2,c2,r,area,cx,cy,resolution;
AnsiString msg;
double fx,fy;

   if(MeasureMode == MEASURE_CIRCLE || MeasureMode == MEASURE_ANGLE || MeasureMode == MEASURE_VLINE) {
      if(pnlCircleP1Status->Color == clSilver || pnlCircleP2Status->Color == clSilver || pnlCircleP3Status->Color == clSilver) {
         return;
      }
   }
   else if(MeasureMode == MEASURE_LINE) {
      if(pnlCircleP1Status->Color == clSilver || pnlCircleP2Status->Color == clSilver) {
         return;
      }
   }

   // 校正值
   fx = pnlXFactor->Caption.ToDouble();
   fy = pnlYFactor->Caption.ToDouble();

   resolution = frmRecipeSet->GetLensPrecision();
   if(cbCCDFullImageMeasure->Checked == true)
      resolution *= (frnMain->CCDInfoAry[0].Width / imMeasure->Width);
   msg.sprintf("%.4f",resolution);
   pnlPrecision->Caption = msg;
   a = pnlCircleP1X->Caption.ToDouble();
   b = pnlCircleP1Y->Caption.ToDouble();
   c = pnlCircleP2X->Caption.ToDouble();
   d = pnlCircleP2Y->Caption.ToDouble();
   e = pnlCircleP3X->Caption.ToDouble();
   f = pnlCircleP3Y->Caption.ToDouble();
   if(MeasureMode == MEASURE_CIRCLE) {
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
      msg.sprintf("%.1f",cx);
      pnlCircleCenterX->Caption = msg;
      msg.sprintf("%.1f",cy);
      pnlCircleCenterY->Caption = msg;
      r = sqrt((a-cx)*(a-cx) + (b-cy)*(b-cy));
      shCircle->Left = int(cx - r) + imMeasure->Left;
      shCircle->Top = int(cy - r) + imMeasure->Top;
      shCircle->Width = int(r*2);
      shCircle->Height = int(r*2);
      shCircle->Pen->Color = pnlLineColor->Color;
      shCircle->Visible = true;
      r *= resolution;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",r);
      else
         msg.sprintf("%.3f",r/1000.0);
      pnlCircleRadius->Caption = msg;
      area = M_PI * r * r;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",area);
      else
         msg.sprintf("%.4f",area/(1000.0*1000.0));
      pnlCircleArea->Caption = msg;

      // Add Measure Item
      //if(MeasureState == 1) {
         if(pnlUnit->Caption == "um")
            msg.sprintf("面積(半徑): %.3fum*um(%.2fum)",area,r);
         else
            msg.sprintf("面積(半徑): %.4fmm*mm(%.3fmm)",area/(1000.0*1000.0),r/1000.0);
         AddMeasureMessage(msg);
      //}
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();

      // 點圓心長度
      /*
      int x,y;
      double tx,ty,resolution,len,dx,dy;
      int sx,sy;
      AnsiString msg;
      x = cx;
      y = cy;
      resolution = frmRecipeSet->GetLensPrecision();
      if(cbCCDFullImageMeasure->Checked == true)
         resolution *= (frnMain->CCDInfoAry[0].Width / imMeasure->Width);
      tx = edCounterX->Text.ToDouble();
      ty = edCounterY->Text.ToDouble();
      sx = shVertical->Left - imMeasure->Left;
      sy = shHorizontal->Top - imMeasure->Top;
      tx += ((x-sx)*resolution/ frnMain->X_RESOLUTION);
      ty += ((y-sy)*resolution/frnMain->Y_RESOLUTION);

      if(rgCircleNo->ItemIndex == 0) {
         CircleCX1 = tx;
         CircleCY1 = ty;
      }
      else {
         CircleCX2 = tx;
         CircleCY2 = ty;
      }

      if(pnlUnit->Caption == "um") {
         if(rgCircleNo->ItemIndex == 0)
            AddMeasureMessage(AnsiString("圓心點1(X,Y)um: ") + FloatToStr(tx) + "," + FloatToStr(ty));
         else
            AddMeasureMessage(AnsiString("圓心點2(X,Y)um: ") + FloatToStr(tx) + "," + FloatToStr(ty));
      }
      else {
         if(rgCircleNo->ItemIndex == 0)
            AddMeasureMessage(AnsiString("圓心點1(X,Y)mm: ") + FloatToStr(tx/1000.0) + "," + FloatToStr(ty/1000.0));
         else
            AddMeasureMessage(AnsiString("圓心點2(X,Y)mm: ") + FloatToStr(tx/1000.0) + "," + FloatToStr(ty/1000.0));
      }

      if(rgCircleNo->ItemIndex == 0) {
         dx = fabs(CirclePX1 - CircleCX1);
         dy = fabs(CirclePY1 - CircleCY1);
         // 取得Factor
         GetFactor(&fx,&fy,CirclePX1,CirclePY1,CircleCX1,CircleCY1);
      }
      else {
         dx = fabs(CirclePX2 - CircleCX2);
         dy = fabs(CirclePY2 - CircleCY2);
         // 取得Factor
         GetFactor(&fx,&fy,CirclePX2,CirclePY2,CircleCX2,CircleCY2);
      }

      dx *= frnMain->X_RESOLUTION;
      dy *= frnMain->Y_RESOLUTION;

      // 校正
      dx *= fx;
      dy *= fy;

      // 使用fx,fy
      dx = fx * frnMain->X_RESOLUTION;
      dy = fy * frnMain->Y_RESOLUTION;

      len = sqrt(dx*dx + dy*dy);
      if(pnlUnit->Caption == "um") {
         if(rgCircleNo->ItemIndex == 0)
            msg.sprintf("點圓心1長(圓心1-點)um: %.1f(dx,dy)%.1f,%.1f",len,dx,dy);
         else
            msg.sprintf("點圓心2長(圓心2-點)um: %.1f(dx,dy)%.1f,%.1f",len,dx,dy);
      }
      else {
         if(rgCircleNo->ItemIndex == 0)
            msg.sprintf("點圓心1長(圓心1-點)mm: %.3f(dx,dy)%.3f,%.3f",len/1000.0,dx/1000.0,dy/1000.0);
         else
            msg.sprintf("點圓心2長(圓心2-點)mm: %.3f(dx,dy)%.3f,%.3f",len/1000.0,dx/1000.0,dy/1000.0);
      }

      AddMeasureMessage(msg);
      pnlLineLength->Caption = msg;

      // 取得Factor
      GetFactor(&fx,&fy,CircleCX1,CircleCY1,CircleCX2,CircleCY2);

      // 計算兩個圓距離
      dx = fabs(CircleCX2 - CircleCX1);
      dy = fabs(CircleCY2 - CircleCY1);
      dx *= frnMain->X_RESOLUTION;
      dy *= frnMain->Y_RESOLUTION;
      // 校正
      dx *= fx;
      dy *= fy;

      // 使用fx,fy
      dx = fx * frnMain->X_RESOLUTION;
      dy = fy * frnMain->Y_RESOLUTION;

      len = sqrt(dx*dx + dy*dy);
      if(pnlUnit->Caption == "um")
         msg.sprintf("兩圓心距離um: %.1f(dx,dy)%.1f,%.1f",len,dx,dy);
      else
         msg.sprintf("兩圓心距離mm: %.3f(dx,dy)%.3f,%.3f",len/1000.0,dx/1000.0,dy/1000.0);
      AddMeasureMessage(msg);

      // 點出圓心位置
      // (cx,cy)為Pixel座標, 要轉成Table座標
      double fcmdx,fcmdy;
      int cmdx,cmdy,ox,oy;

      if(rgCircleNo->ItemIndex == 0) {
         shCircleCenter1->Left = cx + imMeasure->Left - shCircleCenter1->Width/2;
         shCircleCenter1->Top = cy + imMeasure->Top - shCircleCenter1->Height/2;
         shCircleCenter1->Visible = true;
         pnlCircleCenter1->Top = shCircleCenter1->Top - pnlCircleCenter1->Height - 2;
         pnlCircleCenter1->Left = shCircleCenter1->Left + shCircleCenter1->Width + 2;
         pnlCircleCenter1->Visible = true;
      }
      else {
         shCircleCenter2->Left = cx + imMeasure->Left - shCircleCenter2->Width/2;
         shCircleCenter2->Top = cy + imMeasure->Top - shCircleCenter2->Height/2;
         shCircleCenter2->Visible = true;
         pnlCircleCenter2->Top = shCircleCenter2->Top - pnlCircleCenter2->Height - 2;
         pnlCircleCenter2->Left = shCircleCenter2->Left + shCircleCenter2->Width + 2;
         pnlCircleCenter2->Visible = true;
      }

      // Command
      _8154_get_position(X_AXIS, &fcmdx);
      _8154_get_position(Y_AXIS, &fcmdy);
      // Center
      if(rgRelativePosition->ItemIndex == 0) {
         ox = edAOIX->Text.ToInt();
         oy = edAOIY->Text.ToInt();
      }
      // Load/UnLoad
      else if(rgRelativePosition->ItemIndex == 0) {
         ox = edLoadX->Text.ToInt();
         oy = edLoadY->Text.ToInt();
      }
      // Origin
      else {
         ox = edOriginalX->Text.ToInt();
         oy = edOriginalY->Text.ToInt();
      }
      fcmdx -= ox;
      fcmdy -= oy;
      // 要計算Pixel位置
      resolution = frmRecipeSet->GetLensPrecision();
      if(cbCCDFullImageMeasure->Checked == true)
         resolution *= (frnMain->CCDInfoAry[0].Width / imMeasure->Width);
      dx = cx - sx;
      dy = cy - sy;
      fcmdx += ((dx * resolution) / frnMain->X_RESOLUTION);
      fcmdy += ((dy * resolution) / frnMain->Y_RESOLUTION);
      cmdx = fcmdx;
      cmdy = fcmdy;

      if(rgCircleNo->ItemIndex == 0) {
         CircleCenterX1 = cmdx;
         CircleCenterY1 = cmdy;
      }
      else {
         CircleCenterX2 = cmdx;
         CircleCenterY2 = cmdy;
      }
      */
   }
   // 角度
   else if(MeasureMode == MEASURE_ANGLE) {
      double dx1,dy1,dx2,dy2,value1,degree1,value2,degree2,degree;
      dx1 = a - c;
      dy1 = b - d;
      dx2 = e - c;
      dy2 = f - d;

      // Y要反向
      dy1 = 0 - dy1;
      dy2 = 0 - dy2;

      if(dy1 != 0)
         value1 = (double)(dy1 / dx1);
      else
         value1 = 0;
      // radian
      degree1 = atan(value1);
      // degree
      degree1 = (360 * degree1) / (2*M_PI);
      if(dx2 != 0)
         value2 = (double)(dy2 / dx2);
      else
         value2 = 0;
      // radian
      degree2 = atan(value2);
      // degree
      degree2 = (360 * degree2) / (2*M_PI);
      degree = degree1 - degree2;
      msg.sprintf("%.4f",degree);
      pnlAngle->Caption = msg;
      msg.sprintf("%.4f, %.4f",degree1,degree2);
      pnlAngle12->Caption = msg;

      //if(MeasureState == 1) {
         msg.sprintf("角度: %.4f度",degree);
         AddMeasureMessage(msg);
      //}

      // 2023 1 29 - chc Draw
      int p3x,p3y,p2x,p2y,p1x,p1y;
      //imDraw->Visible = true;
      //imDraw->Top = imMeasure->Top;
      //imDraw->Left = imMeasure->Left;
      //imDraw->Width = imMeasure->Width;
      //imDraw->Height = imMeasure->Height;
//      TRect Rect;
//      Rect.left = 0;
//      Rect.top = 0;
//      Rect.Right = imDraw->Width - 1;
//      Rect.Bottom = imDraw->Height - 1;
//      imDraw->Canvas->Brush->Color = clWhite;
//      imDraw->Canvas->FillRect(Rect);
//      imDraw->Canvas->Pen->Color = pnlLineColor->Color;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      p3x = pnlCircleP3X->Caption.ToInt();
      p3y = pnlCircleP3Y->Caption.ToInt();
      imDraw->Canvas->Pen->Width = 2;
//      imDraw->Canvas->MoveTo(p1x,p1y);
//      imDraw->Canvas->LineTo(p2x,p2y);
//      imDraw->Canvas->LineTo(p3x,p3y);
      if(OP3x != -1 && OP3y != -1) {
         imDraw->Canvas->Pen->Color = clWhite;
         imDraw->Canvas->MoveTo(p2x,p2y);
         imDraw->Canvas->LineTo(OP3x,OP3y);
         imDraw->Canvas->Pen->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p2x,p2y);
         imDraw->Canvas->LineTo(p3x,p3y);
      }
      else {
         imDraw->Canvas->Brush->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p2x,p2y);
         imDraw->Canvas->LineTo(p3x,p3y);
      }
      OP3x = p3x;
      OP3y = p3y;
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }

   // Line: a,b,c,d
   else if(MeasureMode == MEASURE_LINE) {
      double dx,dy,value;
      dx = a - c;
      dy = b - d;
      value =  sqrt(dx*dx + dy*dy);
      value *= resolution;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",value);
      else
         msg.sprintf("%.3f",value/1000.0);
      pnlCircleRadius->Caption = msg;
      // Add Measure Item
      //if(MeasureState == 1) {
         if(pnlUnit->Caption == "um")
            msg.sprintf("線長: %.2fum",value);
         else
            msg.sprintf("線長: %.3fmm",value/1000.0);
         AddMeasureMessage(msg);
      //}
      // 2023 1 29 - chc Draw
      int p2x,p2y,p1x,p1y;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      imDraw->Canvas->Pen->Width = 2;
      if(OP2x != -1 && OP2y != -1) {
         imDraw->Canvas->Pen->Color = clWhite;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(OP2x,OP2y);
         imDraw->Canvas->Pen->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      else {
         imDraw->Canvas->Brush->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      OP2x = p2x;
      OP2y = p2y;
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }
   // VLine: a,b,c,d,e,f
   else if(MeasureMode == MEASURE_VLINE) {
      double dx,dy,m1,m2,c1,c2,value,x,y;
      dx = c - a;
      dy = d - b;
      if(dx == 0)
         m1 = 1;
      else
         m1 = dy / dx;
      if(dy == 0)
         m2 = 1;
      else
         m2 = (0-dx) / dy;
      c2 = f - m2*e;
      c1 = d - m1*c;
      if((m2-m1) == 0)
         x = (c1-c2);
      else
         x = (c1-c2) / (m2-m1);
      if(dy == 0)
         x = e;
      y = m1*x + c1;
      if(dx == 0)
         y = f;
      dx = e - x;
      dy = f - y;
      value =  sqrt(dx*dx + dy*dy);
      value *= resolution;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.1f",value);
      else
         msg.sprintf("%.3f",value/1000.0);
      pnlCircleRadius->Caption = msg;
      // Add Measure Item
      //if(MeasureState == 1) {
         if(pnlUnit->Caption == "um")
            msg.sprintf("線長: %.2fum",value);
         else
            msg.sprintf("線長: %.3fmm",value/1000.0);
         AddMeasureMessage(msg);
      //}
      // Draw
      int p2x,p2y,p1x,p1y,p3x,p3y,p4x,p4y;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      p3x = pnlCircleP3X->Caption.ToInt();
      p3y = pnlCircleP3Y->Caption.ToInt();
      p4x = x;
      p4y = y;
      pnlCircleP4X->Caption = IntToStr(p4x);
      pnlCircleP4Y->Caption = IntToStr(p4y);
      imDraw->Canvas->Pen->Width = 2;
      if(OP3x != -1 && OP3y != -1 && OP4x != -1 && OP4y != -1) {
         imDraw->Canvas->Pen->Color = clWhite;
         imDraw->Canvas->MoveTo(OP3x,OP3y);
         imDraw->Canvas->LineTo(OP4x,OP4y);
         imDraw->Canvas->Pen->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
         imDraw->Canvas->MoveTo(p3x,p3y);
         imDraw->Canvas->LineTo(p4x,p4y);
      }
      else {
         imDraw->Canvas->Brush->Color = clBlue;
         imDraw->Canvas->MoveTo(p3x,p3y);
         imDraw->Canvas->LineTo(p4x,p4y);
      }
      OP3x = p3x;
      OP3y = p3y;
      OP4x = p4x;
      OP4y = p4y;
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }
   // Line: a,b,c,d
   else if(MeasureMode == MEASURE_HORLINE) {
      double dx,dy,value;
      dx = a - c;
      dy = b - d;
      value =  sqrt(dx*dx + dy*dy);
      value *= resolution;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",value);
      else
         msg.sprintf("%.3f",value/1000.0);
      pnlCircleRadius->Caption = msg;
      // Add Measure Item
      //if(MeasureState == 1) {
         if(pnlUnit->Caption == "um")
            msg.sprintf("線長: %.2fum",value);
         else
            msg.sprintf("線長: %.3fmm",value/1000.0);
         AddMeasureMessage(msg);
      //}
      // 2023 1 29 - chc Draw
      int p2x,p2y,p1x,p1y;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      imDraw->Canvas->Pen->Width = 2;
      if(OP2x != -1 && OP2y != -1) {
         imDraw->Canvas->Pen->Color = clWhite;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(OP2x,OP2y);
         imDraw->Canvas->Pen->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      else {
         imDraw->Canvas->Brush->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      OP2x = p2x;
      OP2y = p2y;
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }
   // Line: a,b,c,d
   else if(MeasureMode == MEASURE_VERLINE) {
      double dx,dy,value;
      dx = a - c;
      dy = b - d;
      value =  sqrt(dx*dx + dy*dy);
      value *= resolution;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",value);
      else
         msg.sprintf("%.3f",value/1000.0);
      pnlCircleRadius->Caption = msg;
      // Add Measure Item
      //if(MeasureState == 1) {
         if(pnlUnit->Caption == "um")
            msg.sprintf("線長: %.2fum",value);
         else
            msg.sprintf("線長: %.3fmm",value/1000.0);
         AddMeasureMessage(msg);
      //}
      // 2023 1 29 - chc Draw
      int p2x,p2y,p1x,p1y;
      p1x = pnlCircleP1X->Caption.ToInt();
      p1y = pnlCircleP1Y->Caption.ToInt();
      p2x = pnlCircleP2X->Caption.ToInt();
      p2y = pnlCircleP2Y->Caption.ToInt();
      imDraw->Canvas->Pen->Width = 2;
      if(OP2x != -1 && OP2y != -1) {
         imDraw->Canvas->Pen->Color = clWhite;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(OP2x,OP2y);
         imDraw->Canvas->Pen->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      else {
         imDraw->Canvas->Brush->Color = pnlLineColor->Color;
         imDraw->Canvas->MoveTo(p1x,p1y);
         imDraw->Canvas->LineTo(p2x,p2y);
      }
      OP2x = p2x;
      OP2y = p2y;
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }
   // Height
   else if(MeasureMode == MEASURE_HEIGHT) {
      double fheight;
      fheight = abs(MeasureHeightLow - MeasureHeightHigh);
      fheight *= Z_RESOLUTION;
      if(pnlUnit->Caption == "um")
         msg.sprintf("%.2f",fheight);
      else
         msg.sprintf("%.3f",fheight/1000.0);
      pnlCircleRadius->Caption = msg;
      if(pnlUnit->Caption == "um")
         msg.sprintf("高度: %.2fum",fheight);
      else
         msg.sprintf("高度: %.3fmm",fheight/1000.0);
      AddMeasureMessage(msg);
      pnlData->Visible = true;
      pnlData->Caption = msg;
      pnlData->Refresh();
   }

}
//---------------------------------------------------------------------------
// 2023 1 22 - chc 取得Factor
void __fastcall TfrmInspection::GetFactor(double *fx,double *fy,double x1, double y1, double x2, double y2)
{

   *fx = 1.0;
   *fy = 1.0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::shCircleMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

   imInspectionMouseMove(Sender,Shift,imMeasure->Left+shCircle->Left+X,imMeasure->Top+shCircle->Top+Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCircleExitClick(TObject *Sender)
{

   shCircle->Visible = false;
   pnlCircle->Visible = false;
   shPoint1->Visible = false;
   shPoint2->Visible = false;
   shPoint3->Visible = false;

   pnlPoint1->Visible = false;
   pnlPoint2->Visible = false;
   pnlPoint3->Visible = false;

   TRect Rect;
   //imDraw->Visible = true;
   //imDraw->Top = imMeasure->Top;
   //imDraw->Left = imMeasure->Left;
   //imDraw->Width = imMeasure->Width;
   //imDraw->Height = imMeasure->Height;
   Rect.left = 0;
   Rect.top = 0;
   Rect.Right = imDraw->Width - 1;
   Rect.Bottom = imDraw->Height - 1;
   imDraw->Canvas->Brush->Color = clWhite;
   imDraw->Canvas->FillRect(Rect);

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imMeasureMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageXPos = X;
   ImageYPos = Y;
   if(Button == mbLeft) {
      ImageLeftMouse = true;

      // 2023 2 13 - chc 設定已執行的點數量
      if(MeasureMode != MEASURE_NULL) {
         PointSetNo++;
         if(PointSetNo == 1) {
            btnClearDrawClick(this);
            mnPoint1Click(this);
         }
         else if(PointSetNo == 2) {
            mnPoint2Click(this);
            if(MeasureMode == MEASURE_LINE || MeasureMode == MEASURE_HORLINE || MeasureMode == MEASURE_VERLINE || MeasureMode == MEASURE_HEIGHT) {
               PointSetNo = 0;
            }
         }
         else if(PointSetNo == 3) {
            mnPoint3Click(this);
            if(MeasureMode == MEASURE_CIRCLE || MeasureMode == MEASURE_ANGLE || MeasureMode == MEASURE_VLINE) {
               PointSetNo = 0;
            }   
         }
      }

   }
}
//---------------------------------------------------------------------------
// 2023 3 7 - chc Zoom Move
int ImageMeasureSX = 0;
int ImageMeasureSY = 0;

void __fastcall TfrmInspection::imMeasureMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int gray,sx,sy;

   // 主畫面
   unsigned char r,g,b;
   TColor color;
   sx = 0;
   sy = 0;

   // 2023 3 17 - chc Ratio
   // 全影像比例
   double ratiox,ratioy;
   int x,y;
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImageMeasure->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imMeasure->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imMeasure->Height;
      x = X * ratiox;
      y = Y * ratioy;
      // imMeasure->Picture->Bitmap->Canvas->, imMeasure->Canvas->: 結果是一樣
      //color = imMeasure->Picture->Bitmap->Canvas->Pixels[x][y];
      color = imMeasure->Canvas->Pixels[x][y];
   }
   else {

      color = imMeasure->Canvas->Pixels[X][Y];
   }

   b = (color & 0xff0000) >> 16;
   g = (color & 0x00ff00) >> 8;
   r = color & 0x0000ff;
   // 顯示游標值
   pnlMeasureR->Caption = r;
   pnlMeasureG->Caption = g;
   pnlMeasureB->Caption = b;
   pnlMeasureX->Caption = X + sx;
   pnlMeasureY->Caption = Y + sy;

   // Move
   if(ImageLeftMouse == true && cbFreezeMeasure->Checked == true && cbCCDFullImageMeasure->Checked == false) {
      int dx,dy,sx,sy;
      int width,height,w,h,no,w1,h1;
      w1 = imMeasure->Width;
      h1 = imMeasure->Height;
      dx = X - ImageXPos;
      dy = Y - ImageYPos;
      width = GigaWidth;
      height = GigaHeight;

      // 2023 3 17 - chc 是指可視範圍
      //w = width / ImageFactor;
      //h = height / ImageFactor;
      w = w1 / ImageFactor;
      h = h1 / ImageFactor;

      // 2023 3 17 - chc 只校ImageFactor就可以
      //dx = (dx * width) / (w1 * ImageFactor);
      //dy = (dy * height) / (h1 * ImageFactor);
      dx = dx / ImageFactor;
      dy = dy / ImageFactor;

      // 2023 3 17 - chc 設定起點: ImageMeasureSX,ImageMeasureSY
      //sx = ImageSX - dx;
      //sy = ImageSY - dy;
      //if(sx > 0) {
      //   ImageSX = sx;
      //}
      //else
      //   ImageSX = 0;
      //if(sy > 0) {
      //   ImageSY = sy;
      //}
      //else
      //   ImageSY = 0;
      //if((ImageSX + w) > width)
      //   ImageSX = width - w;
      //if((ImageSY + h) > height)
      //   ImageSY = height - h;
      sx = ImageMeasureSX - dx;
      sy = ImageMeasureSY - dy;
      if(sx > 0) {
         ImageMeasureSX = sx;
      }
      else
         ImageMeasureSX = 0;
      if(sy > 0) {
         ImageMeasureSY = sy;
      }
      else
         ImageMeasureSY = 0;
      if((ImageMeasureSX + w) > width)
         ImageMeasureSX = width - w;
      if((ImageMeasureSY + h) > height)
         ImageMeasureSY = height - h;

      no = 0;
      imMeasure->Picture->Bitmap->Width = w;
      imMeasure->Picture->Bitmap->Height = h;

      // 2023 3 17 - chc 設定起點: ImageMeasureSX,ImageMeasureSY
      //frnMain->usb_ImageROIAry[no].SetPlacement(ImageSX,ImageSY,w,h);
      frnMain->usb_ImageROIAry[no].SetPlacement(ImageMeasureSX,ImageMeasureSY,w,h);

      frnMain->usb_ImageROIAry[no].Draw(imMeasure->Canvas->Handle);
      imMeasure->Refresh();

   }

   ImageXPos = X;
   ImageYPos = Y;

   // 2023 1 28 - chc Real time display
   // 2023 2 1 - chc Real time display => pnlToolSelect
   //if(pnlCircle->Visible == true) {
   if(pnlToolSelect->Visible == true) {

      if(MeasureMode == MEASURE_CIRCLE || MeasureMode == MEASURE_ANGLE) {
         if(shPoint1->Visible == true && shPoint2->Visible == true && shPoint3->Visible == false) {
            pnlCircleP3X->Caption = IntToStr(ImageXPos);
            pnlCircleP3Y->Caption = IntToStr(ImageYPos);
            pnlCircleP3Status->Color = clLime;
            btnCircleComputeClick(this);
         }

         // 2023 2 4 - chc Draw
         else if(MeasureMode == MEASURE_ANGLE && shPoint1->Visible == true && shPoint2->Visible == false && shPoint3->Visible == false) {
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            int p2x,p2y,p1x,p1y;
            p1x = pnlCircleP1X->Caption.ToInt();
            p1y = pnlCircleP1Y->Caption.ToInt();
            p2x = pnlCircleP2X->Caption.ToInt();
            p2y = pnlCircleP2Y->Caption.ToInt();
            imDraw->Canvas->Pen->Width = 2;
            if(OP2x != -1 && OP2y != -1) {
               imDraw->Canvas->Pen->Color = clWhite;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(OP2x,OP2y);
               imDraw->Canvas->Pen->Color = pnlLineColor->Color;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(p2x,p2y);
            }
            else {
               imDraw->Canvas->Brush->Color = pnlLineColor->Color;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(p2x,p2y);
            }
            OP2x = p2x;
            OP2y = p2y;
         }

      }
      else if(MeasureMode == MEASURE_LINE) {
         if(shPoint1->Visible == true && shPoint2->Visible == false) {
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            pnlCircleP2Status->Color = clLime;
            btnCircleComputeClick(this);
         }
      }
      else if(MeasureMode == MEASURE_VLINE) {
         if(shPoint1->Visible == true && shPoint2->Visible == true && shPoint3->Visible == false) {
            pnlCircleP3X->Caption = IntToStr(ImageXPos);
            pnlCircleP3Y->Caption = IntToStr(ImageYPos);
            pnlCircleP3Status->Color = clLime;
            btnCircleComputeClick(this);
         }

         // 2023 2 4 - chc Draw
         else if(shPoint1->Visible == true && shPoint2->Visible == false && shPoint3->Visible == false) {
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            int p2x,p2y,p1x,p1y;
            p1x = pnlCircleP1X->Caption.ToInt();
            p1y = pnlCircleP1Y->Caption.ToInt();
            p2x = pnlCircleP2X->Caption.ToInt();
            p2y = pnlCircleP2Y->Caption.ToInt();
            imDraw->Canvas->Pen->Width = 2;
            if(OP2x != -1 && OP2y != -1) {
               imDraw->Canvas->Pen->Color = clWhite;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(OP2x,OP2y);
               imDraw->Canvas->Pen->Color = pnlLineColor->Color;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(p2x,p2y);
            }
            else {
               imDraw->Canvas->Brush->Color = pnlLineColor->Color;
               imDraw->Canvas->MoveTo(p1x,p1y);
               imDraw->Canvas->LineTo(p2x,p2y);
            }
            OP2x = p2x;
            OP2y = p2y;
         }
      }
      else if(MeasureMode == MEASURE_HORLINE) {
         if(shPoint1->Visible == true && shPoint2->Visible == false) {
            int y;
            y = pnlCircleP1Y->Caption.ToInt();
            ImageYPos = y;
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            pnlCircleP2Status->Color = clLime;
            btnCircleComputeClick(this);
         }
      }
      else if(MeasureMode == MEASURE_VERLINE) {
         if(shPoint1->Visible == true && shPoint2->Visible == false) {
            int x;
            x = pnlCircleP1X->Caption.ToInt();
            ImageXPos = x;
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            pnlCircleP2Status->Color = clLime;
            btnCircleComputeClick(this);
         }
      }
      else if(MeasureMode == MEASURE_HEIGHT) {
         if(shPoint1->Visible == true && shPoint2->Visible == false) {
            int x;
            x = pnlCircleP1X->Caption.ToInt();
            ImageXPos = x;
            pnlCircleP2X->Caption = IntToStr(ImageXPos);
            pnlCircleP2Y->Caption = IntToStr(ImageYPos);
            pnlCircleP2Status->Color = clLime;
            MeasureHeightHigh = frnMain->edCounterZ->Text.ToInt();
            pnlZValue2->Caption = frnMain->edCounterZ->Text;
            pnlZMeasure->Caption = frnMain->edCounterZ->Text;
            btnCircleComputeClick(this);
         }
      }
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imMeasureMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   ImageLeftMouse = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnLineClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_LINE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnLine->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = false;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "線長量測";
   //pnlCircle->Visible = true;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   laCircleRadius->Caption = "長度";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   MeasureMode = MEASURE_LINE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnVLineClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_VLINE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnVLine->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = true;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "垂線長量測";
   //pnlCircle->Visible = true;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   laCircleRadius->Caption = "長度";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   MeasureMode = MEASURE_VLINE;
}
//---------------------------------------------------------------------------
// 2023 1 29 - chc Clear Image
void __fastcall TfrmInspection::btnClearDrawClick(TObject *Sender)
{

   shCircle->Visible = false;
   pnlCircle->Visible = false;
   shPoint1->Visible = false;
   shPoint2->Visible = false;
   shPoint3->Visible = false;

   pnlPoint1->Visible = false;
   pnlPoint2->Visible = false;
   pnlPoint3->Visible = false;

   TRect Rect;
   Rect.left = 0;
   Rect.top = 0;
   Rect.Right = imDraw->Width - 1;
   Rect.Bottom = imDraw->Height - 1;
   imDraw->Canvas->Brush->Color = clWhite;
   imDraw->Canvas->FillRect(Rect);
}
//---------------------------------------------------------------------------
// 2023 1 29 - chc Load Image
void __fastcall TfrmInspection::btnLoadTestImageClick(TObject *Sender)
{
AnsiString fname;
int ccdno,w,h;

   fname = frnMain->SystemDirectory + "\\Test2.bmp";
   //imMeasure->Picture->LoadFromFile(fname);
   ccdno = 0;
   frnMain->ImageAry.Load(fname.c_str());

   w = frnMain->ImageAry.GetWidth();
   h = frnMain->ImageAry.GetHeight();
   //frnMain->CCDInfoAry[ccdno].Width = w;
   //frnMain->CCDInfoAry[ccdno].Width = h;
   imMeasure->Picture->Bitmap->Width = w;
   imMeasure->Picture->Bitmap->Height = h;
   frnMain->ImageAry.Draw(imMeasure->Canvas->Handle);

   // 2023 3 17 - chc 設定Size
   frnMain->CCDInfoAry[0].Width = w;
   frnMain->CCDInfoAry[0].Height = h;
   frnMain->usb_ImageAry[0].Load(fname.c_str());
   GigaWidth = w;
   GigaHeight = h;

   double f;
   f = (double)imMeasure->Width / w * 100.0;
   fname.sprintf("%.2f\%",f);
   pnlRatio->Caption = fname;
   cbCCDFullImageMeasure->Checked = true;
}
//---------------------------------------------------------------------------
// 2023 1 29 - chc Load Image
void __fastcall TfrmInspection::GridTitle()
{

   sgMeasure->RowCount = 2;
   sgMeasure->ColCount = 12;

   // Title
   sgMeasure->ColWidths[0] = 40;
   sgMeasure->ColWidths[1] = 60;
   sgMeasure->ColWidths[2] = 120;
   sgMeasure->ColWidths[3] = 120;
   sgMeasure->ColWidths[4] = 40;
   sgMeasure->ColWidths[5] = 40;
   sgMeasure->ColWidths[6] = 40;
   sgMeasure->ColWidths[7] = 40;
   sgMeasure->ColWidths[8] = 40;
   sgMeasure->ColWidths[9] = 40;
   sgMeasure->ColWidths[10] = 40;
   sgMeasure->ColWidths[11] = 40;

   sgMeasure->Cells[0][0] = "No";
   sgMeasure->Cells[1][0] = "ID";
   sgMeasure->Cells[2][0] = "半徑/長/角/高";
   sgMeasure->Cells[3][0] = "面積";
   sgMeasure->Cells[4][0] = "X1";
   sgMeasure->Cells[5][0] = "Y1";
   sgMeasure->Cells[6][0] = "X2";
   sgMeasure->Cells[7][0] = "Y2";
   sgMeasure->Cells[8][0] = "X3";
   sgMeasure->Cells[9][0] = "Y3";
   sgMeasure->Cells[10][0] = "X4";
   sgMeasure->Cells[11][0] = "Y4";

}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::AddMeasureData()
{
int no,x1,y1,x2,y2,x3,y3,x4,y4;
AnsiString name,msg;
double f1,f2;

   no = sgMeasure->RowCount - 1;
   x1 = pnlCircleP1X->Caption.ToInt();
   y1 = pnlCircleP1Y->Caption.ToInt();
   x2 = pnlCircleP2X->Caption.ToInt();
   y2 = pnlCircleP2Y->Caption.ToInt();
   x3 = pnlCircleP3X->Caption.ToInt();
   y3 = pnlCircleP3Y->Caption.ToInt();
   x4 = pnlCircleP4X->Caption.ToInt();
   y4 = pnlCircleP4Y->Caption.ToInt();

   if(MeasureMode == MEASURE_CIRCLE) {
      name = "圓徑";
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = pnlCircleArea->Caption.ToDouble();
      msg.sprintf(" 點3:%.2fum,%.3fum*um",f1,f2);
      pnlPoint3->Caption = msg;
      pnlPoint3->Width = msg.Length() * 8 + 2;
   }
   else if(MeasureMode == MEASURE_ANGLE) {
      name = "角度";
      f1 = pnlAngle->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點3:%.4f度",f1);
      pnlPoint3->Caption = msg;
      pnlPoint3->Width = msg.Length() * 8 + 2;
   }
   else if(MeasureMode == MEASURE_LINE) {
      name = "線長";
      x3 = y3 = 0;
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點2:%.2fum",f1);
      pnlPoint2->Caption = msg;
      pnlPoint2->Width = msg.Length() * 9 + 2;
   }
   else if(MeasureMode == MEASURE_VLINE) {
      name = "垂線長";
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點3:%.2fum",f1);
      pnlPoint3->Caption = msg;
      pnlPoint3->Width = msg.Length() * 8 + 2;
   }
   else if(MeasureMode == MEASURE_HORLINE) {
      name = "水平線";
      x3 = y3 = 0;
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點2:%.2fum",f1);
      pnlPoint2->Caption = msg;
      pnlPoint2->Width = msg.Length() * 9 + 2;
   }
   else if(MeasureMode == MEASURE_VERLINE) {
      name = "垂直線";
      x3 = y3 = 0;
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點2:%.2fum",f1);
      pnlPoint2->Caption = msg;
      pnlPoint2->Width = msg.Length() * 9 + 2;
   }
   else if(MeasureMode == MEASURE_HEIGHT) {
      name = "高度";
      x3 = y3 = 0;
      f1 = pnlCircleRadius->Caption.ToDouble();
      f2 = 0;
      msg.sprintf(" 點2:%.2fum",f1);
      pnlPoint2->Caption = msg;
      pnlPoint2->Width = msg.Length() * 9 + 2;
   }

   sgMeasure->Cells[0][no] = IntToStr(no);
   sgMeasure->Cells[1][no] = name;

   msg.sprintf("%.3f",f1);
   sgMeasure->Cells[2][no] = msg;
   msg.sprintf("%.3f",f2);
   sgMeasure->Cells[3][no] = msg;

   sgMeasure->Cells[4][no] = IntToStr(x1);
   sgMeasure->Cells[5][no] = IntToStr(y1);
   sgMeasure->Cells[6][no] = IntToStr(x2);
   sgMeasure->Cells[7][no] = IntToStr(y2);
   sgMeasure->Cells[8][no] = IntToStr(x3);
   sgMeasure->Cells[9][no] = IntToStr(y3);
   sgMeasure->Cells[10][no] = IntToStr(x4);
   sgMeasure->Cells[11][no] = IntToStr(y4);

   sgMeasure->RowCount++;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgMeasureDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093E4E2;             // 米色
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

void __fastcall TfrmInspection::cbCCDFullImageMeasureClick(TObject *Sender)
{
int w,h,width,height,sx,sy,x,y;
int ccdno;

   ccdno = 0;
   width = frnMain->CCDInfoAry[ccdno].Width;
   height = frnMain->CCDInfoAry[ccdno].Height;

   w = imMeasure->Width;
   h = imMeasure->Height;
   if(cbCCDFullImageMeasure->Checked == true) {
      w = width;
      h = height;
   }

   double f;
   AnsiString msg;
   f = (double)w / width * 100.0;
   msg.sprintf("%.2f\%",f);
   pnlRatio->Caption = msg;

   sx = (width - frmInspection->imMeasure->Width) / 2;
   sy = (height - frmInspection->imMeasure->Height) / 2;
   //frmInspection->LaserCenterX = sx;
   //frmInspection->LaserCenterY = sy;
   //frmInspection->CCDRangeX = sx;
   //frmInspection->CCDRangeY = sy;
   frmInspection->imMeasure->Picture->Bitmap->Width = w;
   frmInspection->imMeasure->Picture->Bitmap->Height = h;
   frnMain->usb_ImageROIAry[ccdno].Detach();
   frnMain->usb_ImageROIAry[ccdno].Attach(&frnMain->usb_ImageAry[ccdno]);
   frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
   frnMain->usb_ImageROIAry[ccdno].SetPlacement(x,y,w,h);
   frnMain->usb_ImageROIAry[ccdno].Draw(frmInspection->imMeasure->Canvas->Handle);

   // 2023 3 17 - chc 設定起點: ImageMeasureSX,ImageMeasureSY
   if(cbCCDFullImageMeasure->Checked == true) {
      sx = 0;
      sy = 0;
   }
   ImageMeasureSX = sx;
   ImageMeasureSY = sy;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnDeleteItemClick(TObject *Sender)
{
int no,tno,colcnt;

   no = sgMeasure->Row;
   if(no < 1 || sgMeasure->RowCount <= 2 || no == sgMeasure->RowCount-1) {
      return;
   }

   colcnt = sgMeasure->ColCount;
   tno = sgMeasure->RowCount-1;
   for(int i=no ; i<tno ; i++) {
      for(int j=0 ; j<colcnt ; j++) {
         sgMeasure->Cells[j][i] = sgMeasure->Cells[j][i+1];
      }
   }
   sgMeasure->RowCount--;

   //重編各批編號
   tno = sgMeasure->RowCount-1;
   for(int i=1 ; i<tno ; i++) {
      sgMeasure->Cells[0][i] = i;
   }

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnDisplayItemClick(TObject *Sender)
{
int no,x1,y1,x2,y2,x3,y3,x4,y4,id;
double a,b,c,d,e,f,a1,b1,c1,a2,b2,c2,r,area,cx,cy,resolution;
AnsiString msg,name;

   no = sgMeasure->Row;
   if(no < 1 || sgMeasure->RowCount <= 2 || no == sgMeasure->RowCount-1) {
      return;
   }

   name = sgMeasure->Cells[1][no];
   id = MEASURE_LINE;
   if(name == "圓徑")
      id = MEASURE_CIRCLE;
   else if(name == "角度")
      id = MEASURE_ANGLE;
   else if(name == "線長")
      id = MEASURE_LINE;
   else if(name == "垂線長")
      id = MEASURE_VLINE;
   x1 = sgMeasure->Cells[4][no].ToInt();
   y1 = sgMeasure->Cells[5][no].ToInt();
   x2 = sgMeasure->Cells[6][no].ToInt();
   y2 = sgMeasure->Cells[7][no].ToInt();
   x3 = sgMeasure->Cells[8][no].ToInt();
   y3 = sgMeasure->Cells[9][no].ToInt();
   x4 = sgMeasure->Cells[10][no].ToInt();
   y4 = sgMeasure->Cells[11][no].ToInt();

   btnClearDrawClick(this);
   imDraw->Canvas->Pen->Color = pnlLineColor->Color;
   imDraw->Canvas->Pen->Width = 2;
   if(id == MEASURE_CIRCLE) {
      a = x1;
      b = y1;
      c = x2;
      d = y2;
      e = x3;
      f = y3;
      a1 = a - c;
      b1 = b - d;
      c1 = (a*a + b*b - c*c - d*d) / 2;
      a2 = a - e;
      b2 = b - f;
      c2 = (a*a + b*b - e*e - f*f) / 2;
      cx = (c1*b2-c2*b1) / (a1*b2-a2*b1);
      cy = (c1*a2-c2*a1) / (a2*b1-a1*b2);
      msg.sprintf("%.1f",cx);
      pnlCircleCenterX->Caption = msg;
      msg.sprintf("%.1f",cy);
      pnlCircleCenterY->Caption = msg;
      r = sqrt((a-cx)*(a-cx) + (b-cy)*(b-cy));
      shCircle->Left = int(cx - r) + imMeasure->Left;
      shCircle->Top = int(cy - r) + imMeasure->Top;
      shCircle->Width = int(r*2);
      shCircle->Height = int(r*2);
      //shCircle->Visible = true;

      // 2023 2 3 - chc Draw Circle
      int px1,py1,px2,py2;
      px1 = int(cx - r);
      py1 = int(cy - r);
      px2 = px1 + int(r*2);
      py2 = py1 + int(r*2);
      imDraw->Canvas->Ellipse(px1,py1,px2,py2);

   }
   else if(id == MEASURE_ANGLE) {
      imDraw->Canvas->MoveTo(x1,y1);
      imDraw->Canvas->LineTo(x2,y2);
      imDraw->Canvas->LineTo(x3,y3);
   }
   else if(id == MEASURE_LINE) {
      imDraw->Canvas->MoveTo(x1,y1);
      imDraw->Canvas->LineTo(x2,y2);
   }
   else if(id == MEASURE_VLINE) {
      imDraw->Canvas->MoveTo(x1,y1);
      imDraw->Canvas->LineTo(x2,y2);
      imDraw->Canvas->MoveTo(x3,y3);
      imDraw->Canvas->LineTo(x4,y4);
   }
   else if(id == MEASURE_HORLINE) {
      imDraw->Canvas->MoveTo(x1,y1);
      imDraw->Canvas->LineTo(x2,y2);
   }
   else if(id == MEASURE_VERLINE) {
      imDraw->Canvas->MoveTo(x1,y1);
      imDraw->Canvas->LineTo(x2,y2);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgMeasureMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   if(Button == mbRight) {
      no = sgMeasure->Row;
      X += gbMeasure->Left;
      X += sgMeasure->Left;
      X += frmInspection->Left;
      X += 10;
      Y += gbMeasure->Top;
      Y += sgMeasure->Top;
      Y += frmInspection->Top;
      Y += 58;
      SetCursorPos(X, Y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      Sleep(100);
      mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
      Sleep(100);
      no = sgMeasure->Row;
   }
}
//---------------------------------------------------------------------------
// Save Data : Lot ID folder format YYYYMMDD_lot ID_solt No. Wafer 01 Die Position,
void __fastcall TfrmInspection::mnSaveClick(TObject *Sender)
{
int no;
AnsiString msg,filename,sid,sv1,sv2,sx1,sy1,sx2,sy2,sx3,sy3,name;
FILE *pFile;

   no = sgMeasure->Row;
   if(no < 1 || sgMeasure->RowCount <= 2) {
      return;
   }

   /*
   sdMeasureData->InitialDir = frmRecipeSet->edResultPath->Text;
   // 設定filter
   sdMeasureData->Filter = "CSV(*.csv)|*.csv";
   if(sdMeasureData->Execute()) {
      // 改變副檔名
      filename = ChangeFileExt(sdMeasureData->FileName,".csv");
      pFile = fopen(filename.c_str(),"w+t");
      if(pFile) {
         for(int i=0 ; i<sgMeasure->RowCount-1 ; i++) {
            sid = sgMeasure->Cells[1][i];
            sv1 = sgMeasure->Cells[2][i];
            sv2 = sgMeasure->Cells[3][i];
            sx1 = sgMeasure->Cells[4][i];
            sy1 = sgMeasure->Cells[5][i];
            sx2 = sgMeasure->Cells[6][i];
            sy2 = sgMeasure->Cells[7][i];
            sx3 = sgMeasure->Cells[8][i];
            sy3 = sgMeasure->Cells[9][i];
            fprintf(pFile,"%d,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",i,sid,sv1,sv2,sx1,sy1,sx2,sy2,sx3,sy3);
         }
         fclose(pFile);
      }
   }
   */
   // ZP1612010_8_PSS_1_1.JPG
   name.sprintf("%s\\%s_%d_PSS_%d_%d",frnMain->InspectMeasureDataPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,frnMain->InspectDieRow,frnMain->InspectDieCol);
   filename = name + ".csv";
   pFile = fopen(filename.c_str(),"w+t");
   if(pFile) {
      for(int i=0 ; i<sgMeasure->RowCount-1 ; i++) {
         sid = sgMeasure->Cells[1][i];
         sv1 = sgMeasure->Cells[2][i];
         sv2 = sgMeasure->Cells[3][i];
         sx1 = sgMeasure->Cells[4][i];
         sy1 = sgMeasure->Cells[5][i];
         sx2 = sgMeasure->Cells[6][i];
         sy2 = sgMeasure->Cells[7][i];
         sx3 = sgMeasure->Cells[8][i];
         sy3 = sgMeasure->Cells[9][i];
         fprintf(pFile,"%d,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",i,sid,sv1,sv2,sx1,sy1,sx2,sy2,sx3,sy3);
      }
      fclose(pFile);
   }

   imDraw->Visible = false;
   btnLoadTestImageClick(this);
   DrawToMeasure();
   Sleep(500);

   //filename = ChangeFileExt(sdMeasureData->FileName,".bmp");
   filename = name + ".bmp";
   //imMeasure->Picture->SaveToFile(filename);
   // 2023 4 25 - chc 不存.bmp, 存jpeg
   //imMeasure->Picture->Bitmap->SaveToFile(filename);
   TJPEGImage *jpeg;
   jpeg = new TJPEGImage;
   // vBmp.Assign(ImageShow.Picture.Graphic)
   jpeg->Assign(imMeasure->Picture->Graphic);
   jpeg->CompressionQuality = 95;
   filename = name + ".jpeg";
   jpeg->SaveToFile(filename);
   delete jpeg;

   imDraw->Visible = true;
   btnLoadTestImageClick(this);

}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::DrawToMeasure()
{
int no,x1,y1,x2,y2,x3,y3,x4,y4,id,len;
double a,b,c,d,e,f,a1,b1,c1,a2,b2,c2,r,area,cx,cy,resolution,ratiox,ratioy,fv1,fv2;
AnsiString msg,name;

   if(sgMeasure->RowCount <= 2) {
      return;
   }

   int ccdno = 0;
   //ratiox = double(frnMain->CCDInfoAry[ccdno].Width) / imDraw->Width;
   //ratioy = double(frnMain->CCDInfoAry[ccdno].Height) / imDraw->Height;
   ratiox = double(frnMain->ImageAry.GetWidth()) / imDraw->Width;
   ratioy = double(frnMain->ImageAry.GetHeight()) / imDraw->Height;

   for(no=1 ; no<sgMeasure->RowCount-1 ; no++) {
      name = sgMeasure->Cells[1][no];
      id = MEASURE_LINE;
      if(name == "圓徑")
         id = MEASURE_CIRCLE;
      else if(name == "角度")
         id = MEASURE_ANGLE;
      else if(name == "線長")
         id = MEASURE_LINE;
      else if(name == "垂線長")
         id = MEASURE_VLINE;
      fv1 = sgMeasure->Cells[2][no].ToDouble();
      fv2 = sgMeasure->Cells[3][no].ToDouble();
      x1 = sgMeasure->Cells[4][no].ToInt();
      y1 = sgMeasure->Cells[5][no].ToInt();
      x2 = sgMeasure->Cells[6][no].ToInt();
      y2 = sgMeasure->Cells[7][no].ToInt();
      x3 = sgMeasure->Cells[8][no].ToInt();
      y3 = sgMeasure->Cells[9][no].ToInt();
      x4 = sgMeasure->Cells[10][no].ToInt();
      y4 = sgMeasure->Cells[11][no].ToInt();

      x1 *= ratiox;
      y1 *= ratioy;
      x2 *= ratiox;
      y2 *= ratioy;
      x3 *= ratiox;
      y3 *= ratioy;
      x4 *= ratiox;
      y4 *= ratioy;

      imMeasure->Canvas->Pen->Color = pnlLineColor->Color;
      imMeasure->Canvas->Pen->Width = 2;
      imMeasure->Canvas->Font->Name = "Arial";
      imMeasure->Canvas->Font->Size = 12;
      imMeasure->Canvas->Font->Color = pnlFontColor->Color;
      imMeasure->Canvas->Brush->Color = pnlTextColor->Color;
      if(id == MEASURE_CIRCLE) {
         a = x1;
         b = y1;
         c = x2;
         d = y2;
         e = x3;
         f = y3;
         a1 = a - c;
         b1 = b - d;
         c1 = (a*a + b*b - c*c - d*d) / 2;
         a2 = a - e;
         b2 = b - f;
         c2 = (a*a + b*b - e*e - f*f) / 2;
         cx = (c1*b2-c2*b1) / (a1*b2-a2*b1);
         cy = (c1*a2-c2*a1) / (a2*b1-a1*b2);
         msg.sprintf("%.1f",cx);
         pnlCircleCenterX->Caption = msg;
         msg.sprintf("%.1f",cy);
         pnlCircleCenterY->Caption = msg;
         r = sqrt((a-cx)*(a-cx) + (b-cy)*(b-cy));

         int px1,py1,px2,py2;
         px1 = int(cx - r);
         py1 = int(cy - r);
         px2 = px1 + int(r*2);
         py2 = py1 + int(r*2);
         imMeasure->Canvas->Brush->Style = bsClear;
         imMeasure->Canvas->Ellipse(px1,py1,px2,py2);
         msg.sprintf("%d-%s: %.2fum,%.4fum*um",no,name.c_str(),fv1,fv2);
         len = msg.Length() * 13;
         imMeasure->Canvas->Brush->Color = pnlTextColor->Color;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(cx,cy,msg);
      }
      else if(id == MEASURE_ANGLE) {
         imMeasure->Canvas->MoveTo(x1,y1);
         imMeasure->Canvas->LineTo(x2,y2);
         imMeasure->Canvas->LineTo(x3,y3);
         msg.sprintf("%d-%s: %.2fum",no,name.c_str(),fv1);
         len = msg.Length() * 13;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(x2+10,y2+10,msg);
      }
      else if(id == MEASURE_LINE) {
         imMeasure->Canvas->MoveTo(x1,y1);
         imMeasure->Canvas->LineTo(x2,y2);
         msg.sprintf("%d-%s: %.2fum",no,name.c_str(),fv1);
         len = msg.Length() * 13;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(x2+10,y2+10,msg);
      }
      else if(id == MEASURE_VLINE) {
         imMeasure->Canvas->MoveTo(x1,y1);
         imMeasure->Canvas->LineTo(x2,y2);
         imMeasure->Canvas->MoveTo(x3,y3);
         imMeasure->Canvas->LineTo(x4,y4);
         msg.sprintf("%d-%s: %.2fum",no,name.c_str(),fv1);
         len = msg.Length() * 13;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(x4+10,y4+10,msg);
      }
      else if(id == MEASURE_HORLINE) {
         imMeasure->Canvas->MoveTo(x1,y1);
         imMeasure->Canvas->LineTo(x2,y2);
         msg.sprintf("%d-%s: %.2fum",no,name.c_str(),fv1);
         len = msg.Length() * 13;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(x2+10,y2+10,msg);
      }
      else if(id == MEASURE_VERLINE) {
         imMeasure->Canvas->MoveTo(x1,y1);
         imMeasure->Canvas->LineTo(x2,y2);
         msg.sprintf("%d-%s: %.2fum",no,name.c_str(),fv1);
         len = msg.Length() * 13;
         imMeasure->Canvas->Rectangle(cx-2,cy-2,cx+len+5,cy+16);
         imMeasure->Canvas->TextOutA(x2+10,y2+10,msg);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnHorLineClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_HORLINE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnHorLine->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = false;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "線長量測";
   //pnlCircle->Visible = true;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   laCircleRadius->Caption = "長度";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   MeasureMode = MEASURE_HORLINE;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnVerLineClick(TObject *Sender)
{

   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_VERLINE) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnVerLine->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = false;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "線長量測";
   //pnlCircle->Visible = true;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   laCircleRadius->Caption = "長度";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   MeasureMode = MEASURE_VERLINE;
}
//---------------------------------------------------------------------------
// Test2.bmp
void __fastcall TfrmInspection::pcInspectionChange(TObject *Sender)
{
AnsiString fname;

   if(pcInspection->ActivePage == tsMeasurement) {
      cbFreezeMeasure->Checked = true;
      Sleep(500);
      fname = frnMain->SystemDirectory + "\\Test2.bmp";
      int ccdno;
      ccdno = 0;

      if(bool_CCD_StatusAry[ccdno] == false) {
      }
      else {
         //imMeasure->Picture->SaveToFile(fname);
         frnMain->usb_ImageAry[ccdno].Save(fname.c_str(),E_FILE_FORMAT_COLOR_BMP);
         int w,h;
         frnMain->ImageAry.Load(fname.c_str());
         w = frnMain->ImageAry.GetWidth();
         h = frnMain->ImageAry.GetHeight();
         imMeasure->Picture->Bitmap->Width = w;
         imMeasure->Picture->Bitmap->Height = h;
         frnMain->ImageAry.Draw(imMeasure->Canvas->Handle);
      }   
//
   }
   else {
      cbFreezeMeasure->Checked = false;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnLineColorClick(TObject *Sender)
{

   if(cdMeasureData->Execute()) {
      pnlLineColor->Color = cdMeasureData->Color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnFontColorClick(TObject *Sender)
{

   if(cdMeasureData->Execute()) {
      pnlFontColor->Color = cdMeasureData->Color;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnTextColorClick(TObject *Sender)
{

   if(cdMeasureData->Execute()) {
      pnlTextColor->Color = cdMeasureData->Color;
   }
}
//---------------------------------------------------------------------------
// Move to center
void __fastcall TfrmInspection::imInspectionDblClick(TObject *Sender)
{
int level;
double fdy,ccdprecision,fdx;
int axisno,position;
I32 cmd;
AnsiString msg;
int cx,cy,X,Y;
double ratiox,ratioy;

   // 2023 5 8 - chc Test
   frnMain->WriteSystemLog("imInspectionDblClick");

   // 要關閉, 避免重複
   imInspection->Enabled = false;

   // 2023 2 11 - chc 確任XYZT是否可動
   // => 2023 2 12 確認需要
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE) {
      if(WaitMotionXYZTDone(500) == false) {
         frnMain->WriteSystemLog("frmInspection-Micro: DobleClick: XYZT尚未Ready.");
      }
      else {
         // Reset Move State
         frnMain->WriteSystemLog("frmInspection-Micro: DobleClick: XYZT已Ready.");
         frnMain->iAxisMove[X_AXIS] = NO_MOVE;
         frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
         frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
         frnMain->iAxisMove[T_AXIS] = NO_MOVE;
      }
   }

   // 全影像比例
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
   // Y軸
   frnMain->pnlYMove->Color = clSilver;
   level = shHorizontal->Top;                                                   // 488
   ccdprecision = frmRecipeSet->GetLensPrecision();
   if(cbCCDFullImage->Checked == true)
      ccdprecision *= ((double)frnMain->CCDInfoAry[0].Width / imInspection->Width);
   msg.sprintf("%.4f",ccdprecision);
   pnlLensPrecision->Caption = msg;
   fdy = (Y - (level - imInspection->Top)) * ccdprecision;

   // 全影像比例
   // 2023 2 11 - chc 精度已除過
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
      // Beep聲
      Beep(500);
      frnMain->pnlAlarmMessage->Caption = " Y-Axis Move Error!";
      frnMain->AddPerformanceMessage(frnMain->pnlAlarmMessage->Caption);
   }
   else {
      frnMain->pnlYMove->Color = clLime;
      pnlSystemMessage->Caption = "MouseDoubleClick: YMove Success";
   }

   // X軸
   frnMain->pnlXMove->Color = clSilver;
   level = shVertical->Left;                                                    // 603
   fdx = (X - (level - imInspection->Left)) * ccdprecision;

   // 全影像比例
   // 2023 2 11 - chc 精度已除過
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
      // Beep聲
      Beep(500);
      frnMain->pnlAlarmMessage->Caption = " X-Axis Move Error!";
      frnMain->AddPerformanceMessage(frnMain->pnlAlarmMessage->Caption);
   }
   else {
      frnMain->pnlXMove->Color = clLime;
      pnlSystemMessage->Caption = "MouseDoubleClick: XMove Success";
   }

   // 強制切到X軸
   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;

   // 要關閉, 避免重複
   imInspection->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbAutoExposureClick(TObject *Sender)
{
int ccdno;

   ccdno = frnMain->combCCDSelect->ItemIndex;
   frnMain->combCCDSelect->ItemIndex = 0;
   frnMain->cbAutoExposure->Checked = cbAutoExposure->Checked;
   frnMain->cbAutoExposureClick(this);
   frnMain->combCCDSelect->ItemIndex = ccdno;
}
//---------------------------------------------------------------------------




void __fastcall TfrmInspection::cbMeasureCrossLineClick(TObject *Sender)
{

   shMeasureHLine->Visible = cbMeasureCrossLine->Checked;
   shMeasureVLine->Visible = cbMeasureCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbMicroCrossLineClick(TObject *Sender)
{

   shVertical->Visible = cbMicroCrossLine->Checked;
   shHorizontal->Visible = cbMicroCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbMacroBackCrossLineClick(TObject *Sender)
{

   shRightVertical->Visible = cbMacroBackCrossLine->Checked;
   shRightHorizontal->Visible = cbMacroBackCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::cbMacroTopCrossLineClick(TObject *Sender)
{

   shLeftVertical->Visible = cbMacroTopCrossLine->Checked;
   shLeftHorizontal->Visible = cbMacroTopCrossLine->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnDICNearClick(TObject *Sender)
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

void __fastcall TfrmInspection::btnDICFarClick(TObject *Sender)
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

void __fastcall TfrmInspection::mnDeleteAllClick(TObject *Sender)
{
int no,tno;

   tno = sgMeasure->RowCount;
   sgMeasure->RowCount = 2;
   for(int j=1 ; j<tno ; j++) {
      for(int i=0 ; i<12 ; i++) {
         sgMeasure->Cells[i][j] = "";
      }
   }
}
//---------------------------------------------------------------------------
// 高低差
void __fastcall TfrmInspection::btnHeightClick(TObject *Sender)
{

   pnlZValue2->Caption = frnMain->edCounterZ->Text;
   pnlZValue1->Caption = frnMain->edCounterZ->Text;
   btnClearDrawClick(this);
   if(MeasureMode != MEASURE_HEIGHT) {
      pnlToolSelect->Visible = false;
   }

   // Tool
   if(pnlToolSelect->Visible == false) {
      pnlToolSelect->Left = btnHeight->Left - 4;
      pnlToolSelect->Top = 0;
      pnlToolSelect->Visible = true;
   }
   else {
      pnlToolSelect->Visible = false;
      imMeasure->PopupMenu = NULL;
      mnPoint1->Visible = false;
      mnPoint2->Visible = false;
      mnPoint3->Visible = false;
      MeasureMode = MEASURE_NULL;
      return;
   }

   // 2023 2 13 - chc 設定已執行的點數量
   PointSetNo = 0;

   mnPoint1->Visible = true;
   mnPoint2->Visible = true;
   mnPoint3->Visible = false;
   pnlData->Visible = false;
   imMeasure->PopupMenu = pmPointSet;
   //pnlCircle->Left = 1175;
   //pnlCircle->Top = 456;
   //laMessage->Caption = "線長量測";
   //pnlCircle->Visible = true;
   //shPoint1->Visible = false;
   //shPoint2->Visible = false;
   //shPoint3->Visible = false;
   pnlCircleP1Status->Color = clSilver;
   pnlCircleP2Status->Color = clSilver;
   pnlCircleP3Status->Color = clSilver;
   laCircleRadius->Caption = "長度";
   laCircleRadius->Visible = true;
   pnlCircleRadius->Visible = true;
   laCircleRadiusUnit->Visible = true;
   if(pnlUnit->Caption == "um")
      laCircleRadiusUnit->Caption = "um";
   else
      laCircleRadiusUnit->Caption = "mm";
   MeasureMode = MEASURE_HEIGHT;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::rgMoveStepMeasureClick(TObject *Sender)
{

   frnMain->rgMoveStep->ItemIndex = rgMoveStepMeasure->ItemIndex;
   rgMoveStep->ItemIndex = rgMoveStepMeasure->ItemIndex;
}
//---------------------------------------------------------------------------
bool boolDICLeftDownMove = false;

void __fastcall TfrmInspection::tbDICFCMouseUp(TObject *Sender,
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

void __fastcall TfrmInspection::tbDICFCMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int fc;

   if(boolDICLeftDownMove == true) {
      fc = tbDICFC->Position;
      gbDIC->Caption = "DIC:" + IntToStr(fc);
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::tbDICFCMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   boolDICLeftDownMove = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveUpMeasureClick(TObject *Sender)
{

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnBackwardZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveDownMeasureClick(TObject *Sender)
{

   // 2023 2 17 - chc 寸動
   frnMain->cbContinueMove->Checked = false;

   frnMain->btnMoveZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------
// 2023 2 18 - chc 回到入料角度
void __fastcall TfrmInspection::btnTOrgClick(TObject *Sender)
{

   tsMicroInspection->Enabled = false;
   frnMain->ToLoadT();
   tsMicroInspection->Enabled = true;
}
//---------------------------------------------------------------------------
//ClassLookup 253		=> 253 類別, This is a table of index numbers. Each index number is associated with a description of a defect
//0 "No Revision"
//1 "1"
//2 "2"
//AnsiString IDName[256];
int IDNo[256];
int IDCount = 0;
//int MapCode[1000][1000];
// fname: include path
void __fastcall TfrmInspection::LoadTXTFile(AnsiString fname)
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
               len = diepitch.Length();
               pos = diepitch.Pos(" ");
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
            // 2023 2 24 - chc 新取得資料: 13個
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

                  // 2023 2 28 - chc 加入Defect W/H/A
                  //if(sscanf(buf,"%d %s %s %d %d %d\n",&rno,sx,sy,&rid,&rc,&rw) != NULL) {
                  if(sscanf(buf,"%d %s %s %d %d %s %s %s %s %d %d %d\n",&rno,sx,sy,&rc,&rw,sw,sh,sa,sd,&rid) != NULL) {

                     if(defectno == 0) {
                        // BINCode Define
                        sgDefectList->ColWidths[0] = 30;
                        sgDefectList->ColWidths[1] = 30;

                        // 2023 4 25 - chc 加入name
                        sgDefectList->ColWidths[2] = 70;

                        sgDefectList->ColWidths[2+1] = 50;
                        sgDefectList->ColWidths[3+1] = 50;
                        sgDefectList->ColWidths[4+1] = 35;
                        sgDefectList->ColWidths[5+1] = 35;
                        sgDefectList->ColWidths[6+1] = 40;

                        // 2023 2 28 - chc 加入Defect W/H/A
                        sgDefectList->ColWidths[7+1] = 60;
                        sgDefectList->ColWidths[8+1] = 60;
                        sgDefectList->ColWidths[9+1] = 65;

                        // 2023 4 7 - chc 加入Lens/Intensity
                        sgDefectList->ColWidths[10+1] = 40;
                        sgDefectList->ColWidths[11+1] = 40;

                        // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
                        sgDefectList->ColWidths[12+1] = 40;
                        sgDefectList->ColWidths[13+1] = 40;

                        sgDefectList->Cells[0][0] = "No";
                        sgDefectList->Cells[1][0] = "ID";

                        // 2023 4 25 - chc 加入name
                        sgDefectList->Cells[2][0] = "Name";

                        sgDefectList->Cells[2+1][0] = "X";
                        sgDefectList->Cells[3+1][0] = "Y";
                        sgDefectList->Cells[4+1][0] = "Col";
                        sgDefectList->Cells[5+1][0] = "Row";
                        sgDefectList->Cells[6+1][0] = "Judge";

                        // 2023 2 28 - chc 加入Defect W/H/A
                        sgDefectList->Cells[7+1][0] = "XSize";
                        sgDefectList->Cells[8+1][0] = "YSize";
                        sgDefectList->Cells[9+1][0] = "Area";

                        // 2023 4 7 - chc 加入Lens/Intensity
                        sgDefectList->Cells[10+1][0] = "Lens";
                        sgDefectList->Cells[11+1][0] = "Inten.";

                        // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
                        sgDefectList->Cells[12+1][0] = "Obser.";
                        sgDefectList->Cells[13+1][0] = "DICVa.";

                        sgDefectList->RowCount = 2;

                        // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
                        /*
                        sgDefectList->Cells[0][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[1][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[2][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[3][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[4][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[5][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[6][sgDefectList->RowCount-1] = "";

                        // 2023 2 28 - chc 加入Defect W/H/A
                        sgDefectList->Cells[7][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[8][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[9][sgDefectList->RowCount-1] = "";

                        // 2023 4 7 - chc 加入Lens/Intensity
                        sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
                        sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
                        */
                        for(int i=0 ; i<sgDefectList->ColCount ; i++)
                           sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

                     }
                     sgDefectList->Cells[0][sgDefectList->RowCount-1] = IntToStr(rno);
                     sgDefectList->Cells[1][sgDefectList->RowCount-1] = rid;

                     // 2023 4 25 - chc 加入name

                     rfx = atof(sx);
                     tmp.sprintf("%.1f",rfx);
                     sgDefectList->Cells[2][sgDefectList->RowCount-1] = tmp;
                     rfy = atof(sy);
                     tmp.sprintf("%.1f",rfy);
                     sgDefectList->Cells[3][sgDefectList->RowCount-1] = tmp;
                     sgDefectList->Cells[4][sgDefectList->RowCount-1] = rc;
                     sgDefectList->Cells[5][sgDefectList->RowCount-1] = rw;

                     // 2023 2 28 - chc 加入Defect W/H/A
                     rfw = atof(sw);
                     tmp.sprintf("%.3f",rfw);
                     sgDefectList->Cells[7][sgDefectList->RowCount-1] = tmp;
                     rfh = atof(sh);
                     tmp.sprintf("%.3f",rfh);
                     sgDefectList->Cells[8][sgDefectList->RowCount-1] = tmp;
                     rfa = atof(sa);
                     tmp.sprintf("%.3f",rfa);
                     sgDefectList->Cells[9][sgDefectList->RowCount-1] = tmp;

                     // 2023 4 7 - chc 加入Lens/Intensity
                     sgDefectList->Cells[10][sgDefectList->RowCount-1] = "0";
                     sgDefectList->Cells[11][sgDefectList->RowCount-1] = "120";

                     // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
                     sgDefectList->Cells[12][sgDefectList->RowCount-1] = "0";
                     sgDefectList->Cells[13][sgDefectList->RowCount-1] = "368";

                     MapCode[rw][rc] = rid;
                     sgDefectList->RowCount++;
                     defectno++;
                  }
               }
               continue;
            }

            // BincodeList
            //ClassLookup 253		=> 253 類別, This is a table of index numbers. Each index number is associated with a description of a defect
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
      SetupBINCodeNew();

   }
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
void __fastcall TfrmInspection::SetupBINCodeNew()
{

   // BINCode Define
   sgBINCode->ColWidths[0] = 40;
   sgBINCode->ColWidths[1] = 35;
   sgBINCode->ColWidths[2] = 30;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";
   sgBINCode->RowCount = 2;

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
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnToEdgeClick(TObject *Sender)
{
int no;

   tmUpdatePosition->Enabled = true;
   btnToEdge->Enabled = false;
   no = rgEdgeNo->ItemIndex + 1;
   if(rgWaferType->ItemIndex == LOADPORT_1) {
      frnMain->To12EdgePosition(no);

      // 2023 4 24 - chc 加入Edge Intensit/Lens
      // 2023 5 4 - chc Inspection中no=1才要做
      if(frnMain->boolInInspecting == true && no == 1) {

         if(tbBXFMLMP->Position != frmRecipeSet->SystemConfig.W12Edge[no-1].Intensity) {
            tbBXFMLMP->Position = frmRecipeSet->SystemConfig.W12Edge[no-1].Intensity;
            SetIntensity(frmRecipeSet->SystemConfig.W12Edge[no-1].Intensity);
         }
         if(frmRecipeSet->SystemConfig.W12EdgeLens != rgBXFMLens->ItemIndex) {
            rgBXFMLens->ItemIndex = frmRecipeSet->SystemConfig.W12EdgeLens;

            // 2023 5 3 - chc Log
            frnMain->WriteSystemLog("btnToEdgeClick: " + IntToStr(rgBXFMLens->ItemIndex));
            // 2023 5 3 - chc 切換
            rgBXFMLensChange(this,rgBXFMLens->ItemIndex);
         }

      }

   }
   else {
      frnMain->To8EdgePosition(no);

      // 2023 4 24 - chc 加入Edge Intensit/Lens
      // 2023 5 4 - chc Inspection中no=1才要做
      if(frnMain->boolInInspecting == true && no == 1) {

         if(frmRecipeSet->SystemConfig.W8EdgeLens != rgBXFMLens->ItemIndex) {
            rgBXFMLens->ItemIndex = frmRecipeSet->SystemConfig.W8EdgeLens;

            // 2023 5 4 - chc 切換
            rgBXFMLensChange(this,rgBXFMLens->ItemIndex);

         }
         if(tbBXFMLMP->Position != frmRecipeSet->SystemConfig.W8Edge[no-1].Intensity) {
            tbBXFMLMP->Position = frmRecipeSet->SystemConfig.W8Edge[no-1].Intensity;
            SetIntensity(frmRecipeSet->SystemConfig.W8Edge[no-1].Intensity);
         }
      }
   }
   btnToEdge->Enabled = true;
   tmUpdatePosition->Enabled = false;

   // 2023 4 19 - chc 更新位置
   UpdateMotionPosition();

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAS0Click(TObject *Sender)
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
// 2023 3 17 - chc Freeze決定
void __fastcall TfrmInspection::cbFreezeClick(TObject *Sender)
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

void __fastcall TfrmInspection::cbFreezeRLClick(TObject *Sender)
{
int w,h,width,height,sx,sy,x,y;
int ccdno;

   ccdno = 1;
   width = frnMain->CCDInfoAry[ccdno].Width;
   height = frnMain->CCDInfoAry[ccdno].Height;
   w = imLeftCCD->Width;
   h = imLeftCCD->Height;
   if(cbCCDFullImageLR->Checked == true) {
      sx = 0;
      sy = 0;
   }
   else {
      sx = (width - w) / 2;
      sy = (height - h) / 2;
      imLeftCCD->Picture->Bitmap->Width = w;
      imLeftCCD->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[ccdno].Detach();
      frnMain->usb_ImageROIAry[ccdno].Attach(&frnMain->usb_ImageAry[ccdno]);
      frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
      frnMain->usb_ImageROIAry[ccdno].SetPlacement(x,y,w,h);
      frnMain->usb_ImageROIAry[ccdno].Draw(imLeftCCD->Canvas->Handle);
   }
   ImageLeftSX = sx;
   ImageLeftSY = sy;

   ccdno = 2;
   width = frnMain->CCDInfoAry[ccdno].Width;
   height = frnMain->CCDInfoAry[ccdno].Height;
   w = imRightCCD->Width;
   h = imRightCCD->Height;
   if(cbCCDFullImageLR->Checked == true) {
      sx = 0;
      sy = 0;
   }
   else {
      sx = (width - w) / 2;
      sy = (height - h) / 2;
      imLeftCCD->Picture->Bitmap->Width = w;
      imLeftCCD->Picture->Bitmap->Height = h;
      frnMain->usb_ImageROIAry[ccdno].Detach();
      frnMain->usb_ImageROIAry[ccdno].Attach(&frnMain->usb_ImageAry[ccdno]);
      frnMain->GetImageXY(width,height,sx,sy,w,h,&x,&y);
      frnMain->usb_ImageROIAry[ccdno].SetPlacement(x,y,w,h);
      frnMain->usb_ImageROIAry[ccdno].Draw(imRightCCD->Canvas->Handle);
   }
   ImageRightSX = sx;
   ImageRightSY = sy;

}
//---------------------------------------------------------------------------
int DieWorkCol = 0;
int DieWorkRow = 0;

void __fastcall TfrmInspection::sgDieDblClick(TObject *Sender)
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

   // Wafer中心點
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
      // Beep聲
      Beep(500);
   }
   UpdateCurrentPosition();
   tsMicroInspection->Enabled = true;
   pnlStageMoving->Visible = false;
   tmUpdatePosition->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgDieMouseMove(TObject *Sender,
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
   // 加入Hint
   sgDie->ShowHint = false;
   msg.sprintf("(%d,%d)",col,row);
   sgDie->Hint = msg;
   sgDie->ShowHint = true;
}
//---------------------------------------------------------------------------
// 2023 3 22 - chc Sub-Die
void __fastcall TfrmInspection::DrawSubDieMap(int sx,int sy,int ex,int ey,int mode)
{
int dx,dy,xno,yno,width,height,x,y;
double fdx,fdy;

// 2023 4 5 - chc Result
TImage *imWaferMap;
TPanel *pnlWaferMap;
if(mode == 1) {
   imWaferMap = imWaferMapSrc;
   pnlWaferMap = pnlWaferMapSrc;
}
else {
   imWaferMap = imWaferMapResult;
   pnlWaferMap = pnlWaferMapResult;
}

   width = ex - sx + 1;
   height = ey - sy + 1;
   xno = edSubDieXno->Text.ToInt();
   yno = edSubDieYno->Text.ToInt();
   fdx = (double)width / xno;
   fdy = (double)height / yno;

   imWaferMapSrc->Canvas->Pen->Color = clGray;
   imWaferMapSrc->Canvas->Pen->Width = 1;
   // 垂直線
   for(int i=1 ; i<xno ; i++) {
      x = sx + fdx * i;
      imWaferMap->Canvas->MoveTo(x,sy);
      imWaferMap->Canvas->LineTo(x,ey);
   }
   // 水平線
   for(int i=1 ; i<yno ; i++) {
      y = sy + fdy * i;
      imWaferMap->Canvas->MoveTo(sx,y);
      imWaferMap->Canvas->LineTo(ex,y);
   }
}
//---------------------------------------------------------------------------
// 2023 3 22 - chc Draw Sub die
void __fastcall TfrmInspection::DrawSubDie()
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

void __fastcall TfrmInspection::btnBackwardZ1Click(TObject *Sender)
{

   frnMain->cbContinueMove->Checked = false;
   frnMain->btnBackwardZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnMoveZ1Click(TObject *Sender)
{

   frnMain->cbContinueMove->Checked = false;
   frnMain->btnMoveZClick(this);
   UpdateMotionPosition();
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc 改成吋動
   return;

   frnMain->cbContinueMove->Checked = true;
   frnMain->btnBackwardTMouseDown(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc 改成吋動
   return;

   frnMain->btnBackwardTMouseUp(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCCW1xMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc 改成吋動
   return;
   
   frnMain->btnMoveTMouseDown(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnCCW1xMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{

   // 2023 4 19 - chc 改成吋動
   return;

   frnMain->btnMoveTMouseUp(this,Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
// Find Center: 3Pt
int FindStep = 0;
void __fastcall TfrmInspection::btnFindCenterClick(TObject *Sender)
{

   // 2023 5 3a - chc 中心誤差量
   AdjustCenterDX = 0;
   AdjustCenterDY = 0;

   // 2023 3 22 - chc 改用timer來做
   pnlCenterX->Caption = "";
   pnlCenterY->Caption = "";
   FindStep = 0;
   tmFindCenter->Enabled = true;
   return;
}
//---------------------------------------------------------------------------
// Alignment
void __fastcall TfrmInspection::btnAlignmentClick(TObject *Sender)
{
int cx,cy,x1,y1,x2,y2,dx,dy,ex,diff,w,r,x,y;

   UpdateMotionPosition();
   pnlStdZ->Caption = pnlZ->Caption;
   pnlStdT->Caption = pnlT->Caption;
   if(pnlBase1X->Caption == "" || pnlBase1Y->Caption == "" || pnlStdZ->Caption == "" || pnlStdT->Caption == "") {
      pnlSystemMessage->Caption = "Undefine Base1!";
      return;
   }

   // 2023 4 24 - chc 存Match: Intensity & Lens
   if(edPatternIntensity->Text.ToInt() != tbBXFMLMP->Position) {
      frnMain->WriteSystemLog("Alignment切換亮度: " + IntToStr(tbBXFMLMP->Position) + " To " + IntToStr(edPatternIntensity->Text.ToInt()));
      tbBXFMLMP->Position = edPatternIntensity->Text.ToInt();
      SetIntensity(edPatternIntensity->Text.ToInt());
   }
   if(rgPatternLens->ItemIndex != rgBXFMLens->ItemIndex) {
      frnMain->WriteSystemLog("Alignment切換Lens: " + IntToStr(rgBXFMLens->ItemIndex) + " To " + IntToStr(rgPatternLens->ItemIndex));
      rgBXFMLens->ItemIndex = rgPatternLens->ItemIndex;

      // 2023 5 3 - chc 切換
      rgBXFMLensChange(this,rgBXFMLens->ItemIndex);

   }

   rgMatchSource->ItemIndex = 0;

   // 2023 3 22 - chc 已找到的中心點來做
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

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Left...";
   pnlMicroJudageDone->Visible = true;

   // Base1 - Left
   MoveToXY(x1,y1);
   //Sleep(500);
   // 2023 5 3 - chc 延長
   //for(int i=0 ; i<6 ; i++) {
   // 2023 5 5 - chc 改用WaitTime
   //for(int i=0 ; i<12 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(400);

   // 2023 4 19 - chc 更新位置
   UpdateMotionPosition();

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Matching...";

   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Base1-1 Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }

   // 2023 4 15 - chc Pause
   if(frmRecipe->cbPause->Checked == true) {

      if(Application->MessageBox("Alignment: Point1 done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
         return;
   }
   // Base1 - Right
   //Sleep(500);
   // 2023 5 5 - chc 改用WaitTime
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

   // 2023 4 27 - chc 精度校正
   w = (double)w / frnMain->X_RESOLUTION;

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Right...";

   r = diff / w;
   ex = x1 + w * r;
   //MoveToXY(cx+dx,y1);
   MoveToXY(ex,y1);
   //Sleep(500);
   // 2023 5 5 - chc 改用WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //}
   frnMain->WaitTime(400);

   // 2023 4 19 - chc 更新位置
   UpdateMotionPosition();

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Matching...";

   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Base1-2 Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Rotating...";

   pnlAlignment->Color = clLime;
   UpdateMotionPosition();
   x2 = pnlX->Caption.ToInt();
   y2 = pnlY->Caption.ToInt();
   RemoteRotate(x1,y1,x2,y2);

   // 2023 3 22 - chc 不做Base2/Position
   // 2023 5 3 - chc 續做Position
   //return;

   // 2023 4 15 - chc Pause
   /*
   if(frmRecipe->cbPause->Checked == true) {

      if(Application->MessageBox("Alignment: Point2 done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
         return;
   }
   //Sleep(500);
   for(int i=0 ; i<5 ; i++) {
      Sleep(100);
      Application->ProcessMessages();
   }
   UpdateMotionPosition();

   // Base1/Base2/Position
   if(rgMatchSource->ItemIndex == 0) {

      // 2023 4 15 - chc Pause
      if(frmRecipe->cbPause->Checked == true) {

         if(Application->MessageBox("To do Base2?      ", "Pattern Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
            return;
      }
      rgMatchSource->ItemIndex = 1;
      int x,y,cx,cy,x1,y1;

      // 2023 3 22 - chc 已找到的中心點來做
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
         // Beep聲
         Beep(500);
      }
   }
   */

   // 2023 5 5 - chc 改用WaitTime
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
   // 已找到的中心點來做
   if(pnlCenterX->Caption != "" && pnlCenterY->Caption != "") {
      cx = pnlCenterX->Caption.ToInt();
      cy = pnlCenterY->Caption.ToInt();
   }
   else {
      cx = frnMain->edCenterX->Text.ToInt();
      cy = frnMain->edCenterY->Text.ToInt();
   }
   dx = cx - pnlCenterDefineX->Caption.ToInt();
   dy = cy - pnlCenterDefineY->Caption.ToInt();
   x = pnlPositionX->Caption.ToInt();
   y = pnlPositionY->Caption.ToInt();
   x += dx;
   y += dy;

   // 2023 5 5 - chc 要提前
   if(edPositionIntensity->Text.ToInt() != tbBXFMLMP->Position) {
      frnMain->WriteSystemLog("Alignment切換亮度: " + IntToStr(tbBXFMLMP->Position) + " To " + IntToStr(edPositionIntensity->Text.ToInt()));
      tbBXFMLMP->Position = edPositionIntensity->Text.ToInt();
      SetIntensity(edPositionIntensity->Text.ToInt());
   }

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Position...";

   if(MoveToXY(x,y) == false) {
      pnlSystemMessage->Caption = "XY Move Faile!";
      // Beep聲
      Beep(500);
   }

   // Delay
   // 2023 5 5 - chc 改用WaitTime
   //for(int i=0 ; i<5 ; i++) {
   //   Sleep(100);
   //   Application->ProcessMessages();
   //   frnMain->tmISCCDTimer(frnMain);
   //}
   for(int i=0 ; i<2 ; i++) {
      frnMain->WaitTime(200);
      frnMain->tmISCCDTimer(frnMain);
   }

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Alignment: Matching...";

   // 更新位置
   UpdateMotionPosition();
   btnAutoMatchClick(this);
   if(pnlAutoMatch->Color != clLime) {
      pnlSystemMessage->Caption = "Pattern Match Fail!";
      pnlAlignment->Color = clRed;
      return;
   }

   // 2023 5 3 - chc Position Intensity
   // 2023 5 5 - chc 要提前
   //if(edPositionIntensity->Text.ToInt() != tbBXFMLMP->Position) {
   //   frnMain->WriteSystemLog("Alignment切換亮度: " + IntToStr(tbBXFMLMP->Position) + " To " + IntToStr(edPositionIntensity->Text.ToInt()));
   //   tbBXFMLMP->Position = edPositionIntensity->Text.ToInt();
   //   SetIntensity(edPositionIntensity->Text.ToInt());
   //}

   UpdateMotionPosition();
   // Die左上角
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

   // 2023 5 5 - chc 取消顯示
   pnlPattern->Visible = false;
   shMarkMatch->Visible = false;

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Visible = false;

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnAutoMatchClick(TObject *Sender)
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

   // 固定CCD0
   no = 0;
   pnlAutoMatch->Color = clSilver;
   pnlAutoMatch->Refresh();
   if(boolCCDImageLoadedAry[no] == false) {
      pnlAutoMatch->Color = clRed;
      pnlSystemMessage->Caption = "No Image! Load/Capture image first.";
      frnMain->WriteSystemLog("btnAutoMatchClick: " + pnlSystemMessage->Caption);
      return;
   }

   // 強制為Full Image
   if(cbCCDFullImage->Checked == false) {
      cbCCDFullImage->Checked = true;
   }

   AnsiString patternfname,recipename;
   int stdheight,stdwidth,areaheight,areawidth;

   // Get Recipe Name
   //
   //recipename = combRecipe->Text;
   recipename = frmRecipe->edRecipeID->Text;

   path = frmRecipeSet->edRecipePath->Text + "\\" + recipename + "\\";
   // 基1
   if(rgMatchSource->ItemIndex == 0) {
      patternfname = path + recipename + "-B1.bmp";
   }
   // 基2
   else if(rgMatchSource->ItemIndex == 1) {
      patternfname = path + recipename + "-B2.bmp";
   }
   // 量測
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

   // 找不到!!
   if(ret == false) {
      pnlSystemMessage->Caption = "Pattern Not found!";
      pnlAutoMatch->Color = clRed;
      return;
   }
   pnlAutoMatch->Color = clLime;

   // 是否移到中心位置?
   if(cbMoveToCenterMatched->Checked == true) {
      // MarkCenterX, MarkCenterY: Match的中心點(對CCD來定義: 2448*2048)
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
         // mcx,mcy: 轉成對imCCD來定義: 2448*2048
         mcx -= 0;
         mcy -= 0;
         // DoubleClickX,DoubleClickY: 以imCCD為(0,0)點
         DoubleClickX = mcx;
         DoubleClickY = mcy;
         // Double Click
         imInspectionDblClick(this);
         msg.sprintf("btnAutoMatchClick(MoveToCenter): MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY);
         frnMain->WriteSystemLog(msg);
         MarkCenterX = cx + 0;
         MarkCenterY = cy + 0;
         // 指向
         shMarkMatch->Top = cy + imInspection->Top - shMarkMatch->Height/2;
         shMarkMatch->Left = cx + imInspection->Left - shMarkMatch->Width/2;
      }
      else {
         cx = shVertical->Left - imInspection->Left;
         cy = shHorizontal->Top - imInspection->Top;
         mcx = MarkCenterX;
         mcy = MarkCenterY;
         // mcx,mcy: 轉成對imCCD來定義: 2448*2048
         mcx -= LaserCenterX;
         mcy -= LaserCenterY;
         // DoubleClickX,DoubleClickY: 以imCCD為(0,0)點
         DoubleClickX = mcx;
         DoubleClickY = mcy;
         // Double Click
         imInspectionDblClick(this);
         msg.sprintf("btnAutoMatchClick(MoveToCenter): MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY);
         frnMain->WriteSystemLog(msg);
         MarkCenterX = cx + LaserCenterX;
         MarkCenterY = cy + LaserCenterY;
         // 指向
         shMarkMatch->Top = cy + imInspection->Top - shMarkMatch->Height/2;
         shMarkMatch->Left = cx + imInspection->Left - shMarkMatch->Width/2;
      }
   }
}
//---------------------------------------------------------------------------
// 200ms
void __fastcall TfrmInspection::tmFindCenterTimer(TObject *Sender)
{
static int no,x1,y1,x2,y2,x3,y3,cx,cy;
AnsiString msg;

// 2023 4 27a - chc Shutter value
static int shutter;
   // 0 - 15
   if(FindStep == 0) {
      shutter = rgCCDShutter->ItemIndex;
      frnMain->WriteSystemLog("Shutter= " + IntToStr(shutter));
   }
   FindStep++;
   tmFindCenter->Enabled = false;

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Caption = "Point1 Moving...";
   pnlMicroJudageDone->Visible = true;

   // 1/2/3/4/5
   if(FindStep == 1) {
      no = 1;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point1 Moving...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point1 Moved.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point1 Moved.";

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 2 || FindStep == 3 || FindStep == 4) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 5) {

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point1 Finding...";

      no = 1;
      pnlSystemMessage->Caption = "Point1 Finding...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x1,&y1);
      msg.sprintf("Point1: %d,%d",x1,y1);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point1 Found.";

      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point1","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }

   // 2023 4 25 - chc Delay
   if(FindStep == 6 || FindStep == 7 || FindStep == 8) {
      tmFindCenter->Enabled = true;
      goto end;
   }

   // 9/10/11/12/13
   if(FindStep == 9) {

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point2 Moving...";

      no = 2;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point2 Moving...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point2 Moved.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point2 Moved.";

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 10 || FindStep == 11 || FindStep == 12) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 13) {

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point2 Finding...";

      no = 2;
      pnlSystemMessage->Caption = "Point2 Finding...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x2,&y2);
      msg.sprintf("Point2: %d,%d",x2,y2);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point2 Found.";

      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point2","FindEdge    ",MB_ICONINFORMATION|MB_OK);
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }

   // 2023 4 25 - chc Delay
   if(FindStep == 14 || FindStep == 15 || FindStep == 16) {
      tmFindCenter->Enabled = true;
      goto end;
   }

   // 17/18/19/20/21
   if(FindStep == 17) {

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point3 Moving...";

      no = 3;
      rgEdgeNo->ItemIndex = no - 1;
      pnlSystemMessage->Caption = "Point3 Moveing...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      btnToEdgeClick(this);
      pnlSystemMessage->Caption = "Point3 Moved.";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      tmFindCenter->Enabled = true;

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point3 Moved.";

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 18 || FindStep == 19 || FindStep == 20) {
      tmFindCenter->Enabled = true;

      // 2023 4 14 - chc 統一
      //return;
      goto end;

   }
   if(FindStep == 21) {

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point3 Finding...";

      no = 3;
      pnlSystemMessage->Caption = "Point3 Finding...";
      frnMain->WriteSystemLog(pnlSystemMessage->Caption);
      FindEdge(no,&x3,&y3);
      msg.sprintf("Point3: %d,%d",x3,y3);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "Point3 Found.";

      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Point3","FindEdge    ",MB_ICONINFORMATION|MB_OK);

      // 2023 5 3a - chc 中心誤差量
      AdjustCenterDX = 0;
      AdjustCenterDY = 0;

      cx = frnMain->edCenterX->Text.ToInt();
      cy = frnMain->edCenterY->Text.ToInt();
      FindCenter(x1,y1,x2,y2,x3,y3,&cx,&cy);
      pnlCenterX->Caption = IntToStr(cx);
      pnlCenterY->Caption = IntToStr(cy);

      // 2023 5 3a - chc 中心點補償值: AdjustCenterDX
      AdjustCenterDX = cx - pnlCenterDefineX->Caption.ToInt();
      AdjustCenterDY = cy - pnlCenterDefineY->Caption.ToInt();
      frnMain->WriteSystemLog("Center位移量(dx,dy): " + IntToStr(AdjustCenterDX) + "," + IntToStr(AdjustCenterDY));

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "To Center...";

      MoveToXY(cx,cy);
      msg.sprintf("Center: %d,%d",cx,cy);
      pnlSystemMessage->Caption = msg;
      frnMain->WriteSystemLog(msg);

      // 2023 4 25 - chc Center Finish?  boolCenterFinish
      frnMain->boolCenterFinish = true;

      // 2023 5 6 - chc 顯示
      pnlMicroJudageDone->Caption = "To Center done.";

      // 2023 4 27a - chc Shutter value
      // 0 - 15
      frnMain->WriteSystemLog("Shutter(old,new)= " + IntToStr(shutter) + "," + IntToStr(rgCCDShutter->ItemIndex));
      if(rgCCDShutter->ItemIndex != shutter) {
         rgCCDShutter->ItemIndex = shutter;
         // => 不用做, 否則會做三次
         //frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
         //frnMain->rgCCDShutterClick(this);
      }

      // 2023 5 3 - chc delay
      // 2023 5 6 - chc 改用WaitTime
      //Sleep(500);
      //Application->ProcessMessages();
      frnMain->WaitTime(300);

   }

// 2023 4 14 - chc 更新Motion Position
end:
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();

   // 2023 5 6 - chc 顯示
   pnlMicroJudageDone->Visible = false;

}
//---------------------------------------------------------------------------
// Edge - MoveCenter - Record
void __fastcall TfrmInspection::FindEdge(int no,int *retx,int *rety)
{
int gray,sx,sy,ex,ey,x,y,gap,rx,ry,cx,cy;
unsigned char r1,g1,b1,dgray,r2,g2,b2;
TColor color1,color2;
double ratiox,ratioy;

// 2023 4 14 - chc 改成找3次: 1/4, 1/2, 3/4
int loop = 1;
bool boolless = false;
bool boolmore = false;

   // 全影像比例
   ratiox = 1.0;
   ratioy = 1.0;
   if(cbCCDFullImage->Checked == true) {
      ratiox = (double)frnMain->CCDInfoAry[0].Width / imInspection->Width;
      ratioy = (double)frnMain->CCDInfoAry[0].Height / imInspection->Height;
   }

   // 2023 5 9 - chc 20改成40
   dgray = 40;

   sx = 120;
   sy = 120;
   ex = imInspection->Width - 120;
   ey = imInspection->Height - 120;

   // 2023 4 4 - chc 改成1/4處
   // 2023 5 3a - chc 改成1/5
   //cx = imInspection->Width / 2;
   //cy = imInspection->Height / 2;
   cx = imInspection->Width*loop / 5;
   cy = imInspection->Height*loop / 5;

   sx *= ratiox;
   sy *= ratioy;
   ex *= ratiox;
   ey *= ratioy;

   // 2023 4 27a - chc 調整亮度: Shutter
   // (0.21 × R) + (0.72 × G) + (0.07 × B)
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
      boolless = false;
      boolmore = false;
againshutter:
      sum = 0;
      cnt = 0;
      for(int row=y1 ; row<y2 ; row+=5) {
         for(int col=x1 ; col<x2 ; col+=5) {
            color1 = imInspection->Canvas->Pixels[col][row];
            b1 = (color1 & 0xff0000) >> 16;
            g1 = (color1 & 0x00ff00) >> 8;
            r1 = color1 & 0x0000ff;
            // 用g
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
            // => 不用做, 否則會做三次
            //frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
            //frnMain->rgCCDShutterClick(this);
            // 2023 4 28 - chc Capture cnt: CaptureCount
            CaptureCount = 0;
            for(int i=0 ; i<200 ; i++) {
               Sleep(10);
               Application->ProcessMessages();

               // 2023 5 5 - chc 改成30 or 30次
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
            // => 不用做, 否則會做三次
            //frnMain->rgCCDShutter->ItemIndex = rgCCDShutter->ItemIndex;
            //frnMain->rgCCDShutterClick(this);
            // 2023 4 28 - chc Capture cnt: CaptureCount
            CaptureCount = 0;
            for(int i=0 ; i<200 ; i++) {
               Sleep(10);
               Application->ProcessMessages();

               // 2023 5 5 - chc 改成30 or 30次
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

// 2023 4 14 - chc 改成找3次: 1/4, 1/2, 3/4
again:
   cx *= ratiox;
   cy *= ratioy;
   rx = ry = -1;
   // 左
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
   // 右
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
   // 下
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

      // 2023 5 5 - chc 改用WaitTime
      //Sleep(200);
      //Application->ProcessMessages();
      frnMain->WaitTime(300);

      frnMain->WriteSystemLog("FindEdge: Move to Center.");

      if(frmRecipe->cbPause->Checked == true)
         Application->MessageBox("Do DoubleClick.","FindEdge    ",MB_ICONINFORMATION|MB_OK);
         
   }

   // 2023 4 14 - chc 改成找3次: 1/4, 1/2, 3/4
   else {
      // 1/2/3
      loop++;
      // 2023 5 3a - chc 1/4改成1/5
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
// Remote Rotate
// 雷射機
//#define T_RESOLUTION    0.001
//#define X_RESOLUTION    0.1
void __fastcall TfrmInspection::RemoteRotate(int x1,int y1, int x2, int y2)
{
double dx,ccdprecision,dy,degree,meany,value,fdy,fcmd,pdegree;
I32 cmd,ypos;
AnsiString msg;
int axisno,pos,level;
bool ret;

   frnMain->WriteSystemLog(">[水平轉正]");
   // 取得精度
   ccdprecision = GetLensPrecision();

   // 用Pulse
   RotateLeftXPos = x1;
   RotateLeftYPos = y1;
   RotateRightXPos = x2;
   RotateRightYPos = y2;
   RotateLeftXCmd = x1;
   RotateRightXCmd = x2;

   // 記錄目前Y位置
   GetPosition(Y_AXIS, &ypos);
   // X軸距離, pulse
   dx = abs(RotateLeftXPos-RotateRightXPos);
   // XY精度不同
   dx *= (frnMain->X_RESOLUTION / frnMain->Y_RESOLUTION);

   // any Position, Left: Larger: 加入Position的記錄, 否則長距離的轉正與單畫面的轉正方向相反!!
   if(RotateLeftXCmd != RotateRightXCmd) {                                   // 有動到X-Axis位置
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
      // 用Pulse
      value = (double)(dy) / (dx);
      // 弳度量
      degree = atan(value);
      pdegree = degree;
      // Log
      msg.sprintf("LeftX,RightX,LeftY,RightY,dx,dy,value,rd=%d,%d,%d,%d,%.1f,%.1f,%.4f,%.4f",RotateLeftXPos,RotateRightXPos,RotateLeftYPos,RotateRightYPos,dx,dy,value,degree);
      frnMain->WriteSystemLog(msg);

      // 度度量
      degree = (360 * degree) / (2*M_PI);
      // 定義T Resolution: TResolution;

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
         pnlSystemMessage->Caption = "T遠程轉正失敗!";
         pnlRemoteRotate->Color = clRed;
         // Beep聲
         Beep(500);
      }
      else {
         pnlRemoteRotate->Color = clLime;
         // 再連續5次轉正
//         CheckTPosition(T_AXIS,pos);
      }
   }

   // 轉正角
   double dg;
   dg = degree;
   if(cbRotateDirection->Checked == false)
      dg = 0 - dg;
   msg.sprintf("%.4f",dg);
   pnlDegree->Caption = msg;

   // 若為0度, 不需要移動
   if(degree == 0)
      return;

   for(int i=0 ; i<5 ; i++) {
      Sleep(100);
      Application->ProcessMessages();
   }
   if(Application->MessageBox("Alignment: Done, Continue?      ", "Alignment Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   double fx,fy;
   // 要用左右的中心點, 不是目前位置(因目前位置可能停在左或右)
   // => RotateRightXPos, RotateLeftXPos 已轉成pulse
   //axisno = X_AXIS;
   //GetPosition(axisno, &fx);
   fx = (RotateRightXPos + RotateLeftXPos) / 2;

   // XY精度不同
   fx *= (frnMain->X_RESOLUTION / frnMain->Y_RESOLUTION);

   // ypos與fy是相同值
   axisno = Y_AXIS;
   GetPosition(axisno, &cmd);
   fy = cmd;

   // 直接平移
   if(cbDirectMove->Checked == true) {
      /*
      meany = (RotateLeftYPos + RotateRightYPos) / 2;
      pos = meany;
      // 再校正
      if(cbDirectMoveDirection->Checked == true)
         pos -= (meany - ypos);
      else
         pos += (meany - ypos);
      msg.sprintf("OffsetMove: Pos1y,Pos2y,Curry,mean,pos=%d,%d,%d,%.5f",RotateLeftYPos,RotateRightYPos,cmd,(int)meany,pos,);
      frnMain->WriteSystemLog(msg);
      if(MoveToY(pos) == false) {
         pnlSystemMessage->Caption = "直接Y移動失敗!";
         // Beep聲
         Beep(500);
      }
      */
      // Match again
      btnAutoMatchClick(this);
      if(pnlAutoMatch->Color != clLime) {
         pnlSystemMessage->Caption = "Base1-Offset Match Fail!";
         pnlAlignment->Color = clRed;
         // Beep聲
         Beep(500);
      }
   }

   // 切到X軸
   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;
   msg.sprintf("%d",(int)dy);
   pnlTRemoteOffset->Caption = msg;
   msg.sprintf("%.3f",degree);
   pnlTRemoteDegree->Caption = msg;

}
//---------------------------------------------------------------------------
// 2023 1 8 - chc 原點(Match的左上角)
int OX,OY;
// 要記錄當時Table位置, 否則切換Pattern時會錯: 先回原位置再作動
I32 OTableX,OTableY;
// 北科大
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
bool __fastcall TfrmInspection::DoNCCMatch(int stdwidth,int stdheight, AnsiString patternfname)

{
// 記錄是否做過Match的前作業 - 改成Global
static boolfirst = false;
static int w,h,iw,ih;
int row,col,no,x,y,x1,y1,x2,y2;
AnsiString learnfname,tmp;
int matchno, selectno;
double score;
unsigned char *ptr1,*ptr2;

// 若分數低於0.7 則再做一次
bool boolfirstdo = true;
// Match五個位置調整
int offsetcnt = 0;

   frnMain->WriteSystemLog("DoNCCMatch: Enter...");

   // NCC 設定初值, 可指定數量
   DetectNums = cbMatchNo->Text.ToInt();
   DontcareThreshold = 0;

   // 大角度做Rotate可能會失敗!
   // 2022 11 29 - chc 不Rotate
   //Rotated = true;
   Rotated = false;

   Scaled = false;
   UsingDoncare = false;
   MinScore = edMinScore->Text.ToDouble();

   // 大角度做Rotate可能會失敗! 縮小為2度
   //MaxAngle = 10;
   //MinAngle = -10;
   MaxAngle = 2;
   MinAngle = -2;

   MaxScale = 1.1;
   MinScale = 0.9;
   UsingColor = true;
   LoadColor = false;
   MinReduceArea = 64;

// 若分數低於0.7 則再做一次
again:

   shMarkMatch->Visible = false;
   frnMain->WriteSystemLog("DoNCCMatch: Enter...1");

   // ----------------------------------------------------------------------------
   // 以原始影像處理
   // 來源: usb_ImageAry[no], usb_ImageROIAry[no]
   // 固定為0
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

// Match Fail: 五個位置調整: offsetcnt
matchagain:

   if(cbCCDFullImage->Checked == true) {
      // 搜尋區域: (x1,y1) - (x2,y2)
      x1 = 0;
      y1 = 0;
      // 目前Laser Center所在位置
      // 對NCC來說, 目前畫面的影像區域是由(LaserCenterX,LaserCenterY)起算
      cx = w / 2;
      cy = h / 2;
      // 要Match的影像區域(相對CCD視野)
      // 記錄起點: 只看十字中心位置
      OX = 10;
      OY = 10;
      x1 = 10;
      y1 = 10;
      x2 = w - 10;
      y2 = h - 10;

      // 2022 11 23 - chc Match Fail: 五個位置調整
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

      // 搜尋區域: (x1,y1) - (x2,y2)
      // LaserCenter X/Y
      // => Lasr Cneter: 固定為80,35
      x1 = LaserCenterX;
      y1 = LaserCenterY;

      // 目前Laser Center所在位置
      // 對NCC來說, 目前畫面的影像區域是由(LaserCenterX,LaserCenterY)起算
      cx = (shVertical->Left-imInspection->Left + x1);
      cy = (shHorizontal->Top-imInspection->Top + y1);

      // 要Match的影像區域(相對CCD視野)
      // 記錄起點: 只看十字中心位置
      x1 = cx - imInspection->Width/2 + 10;
      y1 = cy - imInspection->Height/2 + 10;
      x2 = cx + imInspection->Width/2 - 10;
      y2 = cy + imInspection->Height/2 - 10;
      OX = LaserCenterX + 10;
      OY = LaserCenterY + 10;
   }
   tmp.sprintf("DoNCCMatch: (x1,y1,x2,y2,OX,OY) = (%d,%d,%d,%d,%d,%d)",x1,y1,x2,y2,OX,OY);
   frnMain->WriteSystemLog(tmp);

   // 也要記錄當時Table位置, 否則切換Pattern時會錯: 先回原位置再作動
   // => ?
   GetPosition(X_AXIS, &OTableX);
   GetPosition(Y_AXIS, &OTableY);

   // NCC, 設定GrayImg & 取得影像Data: x1,y1,W:(x2-x1),H:(y2-y1)
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

   // Match的時間
   frnMain->GetTimeTic(&NCCStartTime,&NCCStartTick);
   if(boolfirst == false) {
      if(CheckNCCMatch() == false) {
         pnlSystemMessage->Caption = "NCC Dongle Not Exist!";
         frnMain->WriteSystemLog("DoNCCMatch: NCC dongle NotExist!");
         return false;
      }
      boolfirst = true;
      // 設定參數
      iSetAngle(iMatchModel,MaxAngle,MinAngle);
      iSetScale(iMatchModel,MaxScale,MinScale);

      // 2022 11 25 - chc 加設Robustnes
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

   // Learn Std: 已在呼叫前載入: patternfname
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

   // 失敗
   if(matchno < 1) {
      pnlSystemMessage->Caption = "Match Fail!";
      frnMain->WriteSystemLog("DoNCCMatch: Match Fail!");

      // Match Fail: 五個位置調整: offsetcnt
      offsetcnt++;
      if(offsetcnt < 5) {
         frnMain->WriteSystemLog("DoNCCMatch: Match Fail! 重試: " + IntToStr(offsetcnt));
         goto matchagain;
      }

      return false;
   }

   // 2022 11 25 - chc for test
   //else {
   //   if(boolForTest == true) {
   //      // 2022 11 23 - chc Match Fail: 五個位置調整: offsetcnt
   //      offsetcnt++;
   //      if(offsetcnt < 5) {
   //         frnMain->WriteSystemLog("DoNCCMatch: Match Ok! 重試: " + IntToStr(offsetcnt));
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

   // 取第一個Match者
   for(selectno=0 ; selectno<1 ; selectno++) {
      iGetNCCMatchResults(iMatchModel,selectno,&FindObj);
      if(selectno > 0)
         continue;
      x = FindObj.CX;
      y = FindObj.CY;

      // 校正: OX = LaserCenterX + 10, OY = LaserCenterY + 10
      x += OX;
      y += OY;

      // 存到Global: 原始2448*2048影像區
      MarkCenterX = x;
      MarkCenterY = y;

      score = FindObj.Score;
      tmp.sprintf("%.3f",score);
      pnlScore->Caption = tmp;
      // 指向
      // 對NCC來說, 目前畫面的影像區域是由(LaserCenterX,LaserCenterY)起算: 要減LaserCenter
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

      // 加入角度值
      //tmp.sprintf("%.3f",FindObj.Angle);
      //pnlRotateAngle->Caption = tmp;

      tmp.sprintf("DoNCCMatch: MarkCenterX,MarkCenterY,cx,cy,LaserCenterX,LaserCenterY=%d,%d,%d,%d,%d,%d",MarkCenterX,MarkCenterY,(int)FindObj.CX,(int)FindObj.CY,LaserCenterX,LaserCenterY);
      frnMain->WriteSystemLog(tmp);

   }

   // 2019 12 4 - chc 若分數低於0.7 則再做一次
   // 2022 11 24 - chc 不做
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
// 2023 3 16 - chc Center XY
void __fastcall TfrmInspection::FindCenter(int x1,int y1,int x2,int y2,int x3,int y3,int *rcx,int *rcy)
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
// GetLensPrecision
double __fastcall TfrmInspection::GetLensPrecision()
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

   // 精度值顯示
   AnsiString msg;
   msg.sprintf("%.4f",ccdprecision);
   pnlLensPrecision->Caption = msg;

   return(ccdprecision);
}
//---------------------------------------------------------------------------
// 2023 1 8 - chc NCC
// iVisitingKeyPro or iGetKeyProState
bool __fastcall TfrmInspection::CheckNCCMatch()
{
char* VersionStr;
char* VersionDateStr;

   VersionStr = iGetiImageVersion();
   VersionDateStr = iGetiMatchVersionDate();

   // iMatchModel Keypro
   E_iVision_ERRORS Err = E_NULL;

   // E_TRUE or E_FALSE
   // 改用iVisitingKey()
   Err = iVisitingKey();

   // iGetKeyProState(E_TRUE/E_FALSE)不對! 改用iVisitingKeyPro(E_OK1)
   if(Err != E__OK)
      return false;
   return true;
}
//---------------------------------------------------------------------------
void __fastcall TfrmInspection::SetupBINCodeTitle()
{

   // BINCode Define
   sgBINCode->ColWidths[0] = 30;
   sgBINCode->ColWidths[1] = 30;
   sgBINCode->ColWidths[2] = 20;
   sgBINCode->ColWidths[3] = 120;

   sgBINCode->Cells[0][0] = "No";
   sgBINCode->Cells[1][0] = "ID";
   sgBINCode->Cells[2][0] = "Cr";
   sgBINCode->Cells[3][0] = "Defect Name";

   sgBINCode->RowCount = 2;
   sgBINCode->Cells[0][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[1][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[2][sgBINCode->RowCount-1] = "";
   sgBINCode->Cells[3][sgBINCode->RowCount-1] = "";
}
//---------------------------------------------------------------------------
// DefectList.txt
// No,ID,X,Y,Col,Row,
// 7 1332.5 1332.5 8 23 16;
//TiffFileName ZP1612010_8_PSS_8_1.JPG;
void __fastcall TfrmInspection::SetupDefectListTitle()
{

   // BINCode Define
   sgDefectList->ColWidths[0] = 30;
   sgDefectList->ColWidths[1] = 30;

   // 2023 4 25 - chc 加入name
   sgDefectList->ColWidths[2] = 70;

   sgDefectList->ColWidths[2+1] = 50;
   sgDefectList->ColWidths[3+1] = 50;
   sgDefectList->ColWidths[4+1] = 35;
   sgDefectList->ColWidths[5+1] = 35;
   sgDefectList->ColWidths[6+1] = 40;
   sgDefectList->ColWidths[7+1] = 60;
   sgDefectList->ColWidths[8+1] = 60;
   sgDefectList->ColWidths[9+1] = 65;

   // 2023 4 7 - chc 加入Lens/Intensity
   sgDefectList->ColWidths[10+1] = 40;
   sgDefectList->ColWidths[11+1] = 40;

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   sgDefectList->ColWidths[12+1] = 40;
   sgDefectList->ColWidths[13+1] = 40;

   sgDefectList->Cells[0][0] = "No";
   sgDefectList->Cells[1][0] = "ID";

   // 2023 4 25 - chc 加入name
   sgDefectList->Cells[2][0] = "Name";

   sgDefectList->Cells[2+1][0] = "X";
   sgDefectList->Cells[3+1][0] = "Y";
   sgDefectList->Cells[4+1][0] = "Col";
   sgDefectList->Cells[5+1][0] = "Row";
   sgDefectList->Cells[6+1][0] = "Judge";
   sgDefectList->Cells[7+1][0] = "XSize";
   sgDefectList->Cells[8+1][0] = "YSize";
   sgDefectList->Cells[9+1][0] = "Area";

   // 2023 4 7 - chc 加入Lens/Intensity
   sgDefectList->Cells[10+1][0] = "Lens";
   sgDefectList->Cells[11+1][0] = "Inten.";

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   sgDefectList->Cells[12+1][0] = "Obser.";
   sgDefectList->Cells[13+1][0] = "DICVa.";

   sgDefectList->RowCount = 2;

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
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

   // 2023 4 7 - chc 加入Lens/Intensity
   sgDefectList->Cells[10][sgDefectList->RowCount-1] = "";
   sgDefectList->Cells[11][sgDefectList->RowCount-1] = "";
   */
   for(int i=0 ; i<sgDefectList->ColCount ; i++)
      sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnDefectPassClick(TObject *Sender)
{
int tno,dno,bno;
AnsiString msg;

   tno = sgDefectList->RowCount;
   dno = sgDefectList->Row;
   if(dno == 0 || sgDefectList->RowCount <= 2 || dno >= tno-1)
      return;
   bno = sgBINCode->Row;

   // 2023 4 17 - chc 不詢問
   //msg.sprintf("Set Pass(BinCode=%s) to DefectN0.%d ?",sgBINCode->Cells[1][bno].c_str(),dno);
   //if(Application->MessageBox(msg.c_str(), "Defect Judge", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
   //   return;
   
   // 2023 4 25 - chc 加入name
   //sgDefectList->Cells[6][dno] = "Pass";
   sgDefectList->Cells[6+1][dno] = "Pass";
   sgDefectList->Cells[2][dno] = sgBINCode->Cells[3][bno];

   sgDefectList->Cells[1][dno] = sgBINCode->Cells[1][bno];

   // 2023 4 7 - chc Draw WaferMap
   btnWaferMapDrawClick(this);

   // 2023 4 12 - chc auto Next Defect
   if(dno < tno-1)
      btnNextPointClick(this);
   
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::sgBINCodeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   if(Button == mbRight) {
      no = sgBINCode->Row;
      X += gbBincode->Left;
      X += sgBINCode->Left;
      X += frmInspection->Left;
      X += 12;
      Y += gbBincode->Top;
      Y += sgBINCode->Top;
      Y += frmInspection->Top;
      Y += 58;
      Y += 2;
      SetCursorPos(X, Y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      Sleep(100);
      mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
      Sleep(100);
      no = sgBINCode->Row;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::mnDefectRejectClick(TObject *Sender)
{
int tno,dno,bno;
AnsiString msg;

   tno = sgDefectList->RowCount;
   dno = sgDefectList->Row;
   if(dno == 0 || sgDefectList->RowCount <= 2 || dno >= tno-1)
      return;
   bno = sgBINCode->Row;

   // 2023 4 17 - chc 不詢問
   //msg.sprintf("Set Reject(BinCode=%s) to DefectN0.%d ?",sgBINCode->Cells[1][bno].c_str(),dno);
   //if(Application->MessageBox(msg.c_str(), "Defect Judge", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
   //   return;

   // 2023 4 25 - chc 加入name
   //sgDefectList->Cells[6][dno] = "Reject";
   sgDefectList->Cells[6+1][dno] = "Reject";
   sgDefectList->Cells[2][dno] = sgBINCode->Cells[3][bno];

   sgDefectList->Cells[1][dno] = sgBINCode->Cells[1][bno];

   // 2023 4 7 - chc Draw WaferMap
   btnWaferMapDrawClick(this);

   // 2023 4 11 - chc ImageSaveMode
   if(rgImageSaveMode->ItemIndex == IMAGE_SAVE_BINCODE) {
      frnMain->WriteSystemLog("Micro Die Judge: Reject and save Image.");

      // 2023 4 12 - chc From Save: boolFromSave
      boolFromSave = true;

      btnSaveMainCCDClick(this);
   }

   // 2023 4 12 - chc auto Next Defect
   if(dno < tno-1)
      btnNextPointClick(this);

}
//---------------------------------------------------------------------------
// 2023 4 5 - chc Result
void __fastcall TfrmInspection::sgBINCodeResultDrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{

   // 得到焦點和選中單元格，不需要處理，只有 (State.Contains(gdFixed)) 才開始進行處理
   if(State.Contains(gdFocused) || State.Contains(gdSelected))
      return;

   // 2023 4 1 - chc RowColor: 在DrawText前先做, 否則Text會被蓋住
   if(ARow != 0 && ACol != 0) {
      if((ARow % 2) != 0)
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x0093F4F2;             // 米色
      else
         ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00D1D1D1;
   }
   // 0x00B3B3B3
   if(ARow == 0) {
      ((TStringGrid*)Sender)->Canvas->Brush->Color = (TColor)0x00B3B3B3;             // 米色
   }
   ((TStringGrid*)Sender)->Canvas->FillRect(Rect);

   TStringGrid *Grid = (TStringGrid*)Sender;
   if(ARow == 0) {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_CENTER);
   }
   else {
//      Grid->Canvas->FillRect(Rect);
      DrawText(Grid->Canvas->Handle,Grid->Cells[ACol][ARow].c_str(),Grid->Cells[ACol][ARow].Length(),&Rect,DT_SINGLELINE + DT_VCENTER + DT_LEFT);
   }

   if(ARow != 0 && ARow < sgBINCode->RowCount-1) {
      // 差量加上顏色
      if(ACol == 1) {
         ((TStringGrid*)Sender)->Canvas->Brush->Color = frmRecipeSet->RecipeBufferRun.BinCode.Color[ARow-1];
         Rect.Left += 3;
         Rect.Top += 5;
         Rect.Right -= 3;
         Rect.Bottom -= 5;
         ((TStringGrid*)Sender)->Canvas->FillRect(Rect);
      }
   }
}
//---------------------------------------------------------------------------
// 2023 4 5 - chc 存圖檔
void __fastcall TfrmInspection::SaveResultMap(bool bjudge)
{

   // 在Inspection且不是AutoSave才要存
   //if(frnMain->boolInInspecting == false || cbAutoSaveMicro->Checked == true)
   if(cbAutoSaveMicro->Checked == true)
      return;

   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01\Results    : .csv, *.txt
   int idno,defectno;
   pnlMicroResult->Visible = true;
   DrawBigImage(2);
   // 要有Refresh()才Save畫得出來
   imWaferMapResult->Refresh();
   pnlMicroResult->Refresh();
   AnsiString fname;

   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0)
      fname.sprintf("%s\\%s_%d_PSS_Map.jpeg",frnMain->InspectResultsPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo);
   else

      fname.sprintf("%s\\%s_%d_PSS_Map.bmp",frnMain->InspectResultsPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo);
   TCanvas *csBuf = new TCanvas;
   Graphics::TBitmap *Source = new Graphics::TBitmap();
   HDC hDC = GetDC(pnlMicroResult->Handle);
   csBuf->Handle = hDC;
   Source->Width = pnlMicroResult->Width;
   Source->Height = pnlMicroResult->Height;
   // BINCode Define
   sgBINCodeResult->ColWidths[0] = 75;
   sgBINCodeResult->ColWidths[1] = 40;
   sgBINCodeResult->ColWidths[2] = 55;
   sgBINCodeResult->ColWidths[3] = 75;
   sgBINCodeResult->Cells[0][0] = "Name";
   sgBINCodeResult->Cells[1][0] = "Color";
   sgBINCodeResult->Cells[2][0] = "Quantity";
   sgBINCodeResult->Cells[3][0] = "%";
   sgBINCodeResult->RowCount = 2;
   sgBINCodeResult->Cells[0][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[1][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[2][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[3][sgBINCodeResult->RowCount-1] = "";
   idno = sgBINCode->RowCount - 2;
   defectno = sgDefectList->RowCount - 2;
   for(int i=0; i<idno ; i++) {
      //sgBINCodeResult->Cells[0][sgBINCodeResult->RowCount-1] = " " + IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.ID[i]);
      sgBINCodeResult->Cells[0][sgBINCodeResult->RowCount-1] = " " + frmRecipeSet->RecipeBufferRun.BinCode.IDName[i];
      sgBINCodeResult->Cells[1][sgBINCodeResult->RowCount-1] = "";
      sgBINCodeResult->Cells[2][sgBINCodeResult->RowCount-1] = "";
      sgBINCodeResult->Cells[3][sgBINCodeResult->RowCount-1] = "";
      sgBINCodeResult->RowCount++;
      // 各ID的數量與百分比
      frmRecipeSet->RecipeBufferRun.BinCode.No[i] = 0;
      frmRecipeSet->RecipeBufferRun.BinCode.Percent[i] = 0;
   }
   sgBINCodeResult->Cells[0][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[1][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[2][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Cells[3][sgBINCodeResult->RowCount-1] = "";
   sgBINCodeResult->Refresh();
   for(int i=0 ; i<2 ; i++) {
      Application->ProcessMessages();
      Sleep(100);
   }
   AnsiString temp;
   DateTimeToString(temp,"yyyy-mm-dd hh:nn:ss",Now());
   laTestedDateTime->Caption = temp;
   if(rgWaferType->ItemIndex == 0)
      laDiameter->Caption = "300.00mm";
   else
      laDiameter->Caption = "200.00mm";
   laGross->Caption = IntToStr(GrossNo) + "(" + IntToStr(defectno) + ")";
   // 所有Defect
   for(int i=0 ; i<defectno ; i++) {
      for(int j=0 ; j<idno ; j++) {
         if(sgDefectList->Cells[1][i+1] == frmRecipeSet->RecipeBufferRun.BinCode.ID[j]) {
            frmRecipeSet->RecipeBufferRun.BinCode.No[j]++;
            break;
         }
      }
   }
   for(int i=0 ; i<idno ; i++) {
      if(defectno > 0)
         frmRecipeSet->RecipeBufferRun.BinCode.Percent[i] = (double)frmRecipeSet->RecipeBufferRun.BinCode.No[i] * 100 / defectno;
      else
         frmRecipeSet->RecipeBufferRun.BinCode.Percent[i] = 0.0;
      sgBINCodeResult->Cells[2][i+1] = " " + IntToStr(frmRecipeSet->RecipeBufferRun.BinCode.No[i]);
      temp.sprintf(" %.02f \%",frmRecipeSet->RecipeBufferRun.BinCode.Percent[i]);
      sgBINCodeResult->Cells[3][i+1] = temp;
   }
   if(frnMain->rgCST->ItemIndex == LOADPORT_1)
      laDegree->Caption = frnMain->edAlignerDegree->Text / 10;
   else
      laDegree->Caption = frnMain->edAligner2Degree->Text / 10;

   // 2023 4 25 - chc 個別的Recipe
   frmRecipe->InspectResult[frnMain->InspectWaferNo-1].RecipeName = frmRecipe->edRecipeID->Text;

   laRecipeID->Caption = frmRecipe->edRecipeID->Text;
   laWidth->Caption = edChipWidth->Text + "um";
   laHeight->Caption = edChipHeight->Text + "um";
   laFilename->Caption = fname;
   laTestedNo->Caption = IntToStr(defectno);

   // 5/10/20/50/100
   if(rgBXFMLens->ItemIndex == 0)
      laLens->Caption = "5X";
   else if(rgBXFMLens->ItemIndex == 1)
      laLens->Caption = "10X";
   else if(rgBXFMLens->ItemIndex == 2)
      laLens->Caption = "20X";
   else if(rgBXFMLens->ItemIndex == 3)
      laLens->Caption = "50X";
   else if(rgBXFMLens->ItemIndex == 4)
      laLens->Caption = "100X";
   // Yield
   double fyield = 0.0;
   double ffail = 0.0;
   int ngno = 0;
   for(int i=0 ; i<defectno ; i++) {

      // 2023 4 25 - chc 加入name
      //if(sgDefectList->Cells[6][i+1] == "Reject") {
      if(sgDefectList->Cells[6+1][i+1] == "Reject") {

         ngno++;
      }
   }
   if(defectno > 0) {
      fyield = 100.0 * (defectno-ngno) / defectno;
      ffail = 100.0 * ngno / defectno;
   }
   if(bjudge == true)
      temp.sprintf(" [Pass] Lot: %s  Product: %s  Yield: %.2f%c  Gross: %d  ID: %s",frmRecipe->edLotID->Text.c_str(),frmRecipe->edDeviceName->Text.c_str(),fyield,
                    '%',GrossNo,frnMain->StageInformation[STAGE_CHUCK].WaferID.c_str());
   else
      temp.sprintf(" [Reject] Lot: %s  Product: %s  Yield: %.2f%c  Gross: %d  ID: %s",frmRecipe->edLotID->Text.c_str(),frmRecipe->edDeviceName->Text.c_str(),fyield,
                    '%',GrossNo,frnMain->StageInformation[STAGE_CHUCK].WaferID.c_str());

   // 2023 4 12 - chc MinYield
   AnsiString tmp;
   tmp.sprintf("%.1f%c(Fail Rate=%.1f%c)",frmRecipeSet->RecipeBufferRun.MicroRecipe.MinYield,'%',ffail,'%');
   laMinYield->Caption = tmp;
   if(fyield < frmRecipeSet->RecipeBufferRun.MicroRecipe.MinYield) {
      laMinYield->Color = clRed;
   }
   else {
      laMinYield->Color = clLime;
   }

   // 2023 4 25 - chc CST Report
   frmRecipe->InspectResult[frnMain->StageInformation[STAGE_CHUCK].SlotNo-1].DefectNo = defectno;
   frmRecipe->InspectResult[frnMain->StageInformation[STAGE_CHUCK].SlotNo-1].Yield = fyield;

   pnlResultTitle->Caption = temp;
   sgBINCodeResult->Row = sgBINCodeResult->RowCount-1;
   pnlMicroResult->Refresh();
   Source->Canvas->CopyRect( Rect(0,0,Source->Width,Source->Height),csBuf,Rect(0,0,pnlMicroResult->Width,pnlMicroResult->Height));

   // 2023 4 12 - chc MinYield
   //Application->MessageBox("Result Data   ","Micro Inspection",MB_ICONINFORMATION|MB_OK);
   if(fyield < frmRecipeSet->RecipeBufferRun.MicroRecipe.MinYield) {
      Application->MessageBox("Result Data(Yield Fail!)   ","Micro Inspection",MB_ICONINFORMATION|MB_OK);
   }
   else {
      Application->MessageBox("Result Data(Yield Ok)   ","Micro Inspection",MB_ICONINFORMATION|MB_OK);
   }

   // 2023 4 25 - chc 存jpeg
   if(rgFileFormat->ItemIndex == 0) {
      TJPEGImage *jpeg;
      jpeg = new TJPEGImage;
      //jpeg->Assign(imInspection->Picture->Graphic);
      jpeg->Assign(Source);
      jpeg->CompressionQuality = 95;
      jpeg->SaveToFile(fname);
      delete jpeg;
   }
   else

      Source->SaveToFile(fname);
   delete csBuf;
   delete Source;
   pnlMicroResult->Visible = false;

   int xno,yno;
   FILE *pFile;
   fname.sprintf("%s\\%s_%d_PSS_Map.csv",frnMain->InspectResultsPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo);
   pFile = fopen(fname.c_str(),"w+t");

   // 2023 4 11 - chc 加入Title
   fprintf(pFile,"%s\n",pnlResultTitle->Caption.c_str());

   xno = pnlChipWNo->Caption.ToInt();
   yno = pnlChipHNo->Caption.ToInt();

   // 2023 5 2 - chc 修正數量

   for(int i=0 ; i<yno ; i++) {
      for(int j=0 ; j<xno ; j++) {
         if(j != 0)
            fprintf(pFile,",");

         //fprintf(pFile,"%d",MapCode[i][j]);
         if(MapCode[i][j] == -2)
            fprintf(pFile,".");
         else if(MapCode[i][j] == -1)
            fprintf(pFile,"z");
         else
            fprintf(pFile,"%d",MapCode[i][j]);

         if(j == xno-1)
            fprintf(pFile,"\n");
      }
   }
   fclose(pFile);

   // 2023 4 11 - chc 再產生DefectList.csv
   fname.sprintf("%s\\%s_%d_PSS_Defect.csv",frnMain->InspectResultsPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo);
   pFile = fopen(fname.c_str(),"w+t");
   // Title
   fprintf(pFile,"%s\n",pnlResultTitle->Caption.c_str());
   int no;
   no = sgDefectList->RowCount - 2;
   for(int i=0 ; i<=no ; i++) {
      // 12個欄位
      for(int j=0 ; j<sgDefectList->ColCount ; j++) {
         fprintf(pFile,"%s",sgDefectList->Cells[j][i].c_str());
         if(j == sgDefectList->ColCount-1)
            fprintf(pFile,"\n");
         else
            fprintf(pFile,",");
      }
   }
   fclose(pFile);
}
//---------------------------------------------------------------------------
// 2023 4 8 - chc Assign None
void __fastcall TfrmInspection::mnDefectNoneClick(TObject *Sender)
{
int tno,dno,bno;
AnsiString msg;

   tno = sgDefectList->RowCount;
   dno = sgDefectList->Row;
   if(dno == 0 || sgDefectList->RowCount <= 2 || dno >= tno-1)
      return;
   bno = sgBINCode->Row;

   // 2023 4 17 - chc 不詢問
   //msg.sprintf("Set Reject(BinCode=%s) to DefectN0.%d ?",sgBINCode->Cells[1][bno].c_str(),dno);
   //if(Application->MessageBox(msg.c_str(), "Defect Judge", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
   //   return;

   // 2023 4 25 - chc 加入name
   //sgDefectList->Cells[6][dno] = "None";
   sgDefectList->Cells[6+1][dno] = "None";
   sgDefectList->Cells[2][dno] = "";

   // None應為0
   //sgDefectList->Cells[1][dno] = sgBINCode->Cells[1][bno];
   sgDefectList->Cells[1][dno] = "0";

   // 2023 4 7 - chc Draw WaferMap
   btnWaferMapDrawClick(this);

   // 2023 4 11 - chc ImageSaveMode
   if(rgImageSaveMode->ItemIndex == IMAGE_SAVE_BINCODE) {
      frnMain->WriteSystemLog("Micro Die Judge: None and save Image.");

      // 2023 4 12 - chc From Save: boolFromSave
      boolFromSave = true;

      btnSaveMainCCDClick(this);
   }

   // 2023 4 12 - chc auto Next Defect
   if(dno < tno-1)
      btnNextPointClick(this);

}
//---------------------------------------------------------------------------
// 2023 4 12 - chc 加入Deefect
void __fastcall TfrmInspection::mnAddPointClick(TObject *Sender)
{
int row,col,dno,r,c;
bool bexist = false;

   // 取得目前XY
   col = pnlWorkCol->Caption.ToInt();
   row = pnlWorkRow->Caption.ToInt();
   // Exist?
   dno = sgDefectList->RowCount - 2;
   for(int i=1 ; i<=dno ; i++) {

      // 2023 4 25 - chc 加入name
      //c = sgDefectList->Cells[4][i].ToInt();
      //r = sgDefectList->Cells[5][i].ToInt();
      c = sgDefectList->Cells[4+1][i].ToInt();
      r = sgDefectList->Cells[5+1][i].ToInt();

      if(c == col && r == row) {
         pnlSystemMessage->Caption = "Defect Existed!";
         bexist = true;
         break;
      }
   }
   if(bexist == false) {
      AddPointData();
      btnWaferMapDrawClick(this);
   }
}
//---------------------------------------------------------------------------
// 2023 4 12 - chc Add Defect
// sgDefectList->Cells[0][0] = "No";
// sgDefectList->Cells[1][0] = "ID";
// sgDefectList->Cells[2][0] = "X";
// sgDefectList->Cells[3][0] = "Y";
// sgDefectList->Cells[4][0] = "Col";
// sgDefectList->Cells[5][0] = "Row";
// sgDefectList->Cells[6][0] = "Judge";
// 加入Defect W/H/A
// sgDefectList->Cells[7][0] = "XSize";
// sgDefectList->Cells[8][0] = "YSize";
// sgDefectList->Cells[9][0] = "Area";
void __fastcall TfrmInspection::AddPointData()
{
int no,col,row,x,y,cx,cy;
AnsiString name,msg;
double f1,f2;

   no = sgDefectList->RowCount - 1;
   col = pnlWorkCol->Caption.ToInt();
   row = pnlWorkRow->Caption.ToInt();

   UpdateMotionPosition();
   x = pnlX->Caption.ToInt();
   y = pnlY->Caption.ToInt();
   frnMain->WriteSystemLog("Add: X,Y= " + IntToStr(x) + "," + IntToStr(y));

   sgDefectList->Cells[0][no] = IntToStr(no);
   sgDefectList->Cells[1][no] = "0";

   // 2023 4 25 - chc 加入name
   sgDefectList->Cells[2][no] = "";

   sgDefectList->Cells[2+1][no] = IntToStr(x);
   sgDefectList->Cells[3+1][no] = IntToStr(y);
   sgDefectList->Cells[4+1][no] = IntToStr(col);
   sgDefectList->Cells[5+1][no] = IntToStr(row);
   sgDefectList->Cells[6+1][no] = "0";
   sgDefectList->Cells[7+1][no] = "0";
   sgDefectList->Cells[8+1][no] = "0";
   sgDefectList->Cells[9+1][no] = "0";

   // 加入Lens/Intensity
   sgDefectList->Cells[10+1][no] = rgBXFMLens->ItemIndex;
   sgDefectList->Cells[11+1][no] = tbBXFMLMP->Position;

   // 2023 4 25 - chc 加入Observation, DICValue
   sgDefectList->Cells[12+1][no] = rgObservation->ItemIndex;
   sgDefectList->Cells[13+1][no] = tbDICFC->Position;

   sgDefectList->RowCount++;

   // Clear
   for(int i=0 ; i<sgDefectList->ColCount ; i++)
      sgDefectList->Cells[i][sgDefectList->RowCount-1] = "";

   // 2023 4 17 - chc 指到最後一筆
   sgDefectList->Row = sgDefectList->RowCount - 2;

}
//---------------------------------------------------------------------------
// 2023 4 12 - chc Add Defect
void __fastcall TfrmInspection::mnDeletePointClick(TObject *Sender)
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

   //重編各批編號
   tno = sgDefectList->RowCount-1;
   for(int i=1 ; i<tno ; i++) {
      sgDefectList->Cells[0][i] = i;
   }
   btnWaferMapDrawClick(this);

}
//---------------------------------------------------------------------------
// 2023 4 12 - chc Restore BinCode
void __fastcall TfrmInspection::mnRestorePointClick(TObject *Sender)
{
int tno,dno,bno;
AnsiString msg;

   tno = sgDefectList->RowCount;
   dno = sgDefectList->Row;
   if(dno == 0 || sgDefectList->RowCount <= 2 || dno >= tno-1)
      return;

   msg.sprintf("Set None(BinCode=0) to DefectN0.%d ?",dno);
   if(Application->MessageBox(msg.c_str(), "Defect Judge", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   // Restore to 0
   // 2023 4 25 - chc 加入name
   //sgDefectList->Cells[6][dno] = "0";
   sgDefectList->Cells[6+1][dno] = "0";
   sgDefectList->Cells[2][dno] = "";

   sgDefectList->Cells[1][dno] = "0";

   // Draw WaferMap
   btnWaferMapDrawClick(this);
}
//---------------------------------------------------------------------------
// 2023 4 3 - chc Mouse down
void __fastcall TfrmInspection::sgDefectListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   if(Button == mbRight) {
      no = sgDefectList->Row;
      X += gbDefectList1->Left;
      X += sgDefectList->Left;
      X += frmInspection->Left;
      X += 12;
      Y += gbDefectList1->Top;
      Y += sgDefectList->Top;
      Y += frmInspection->Top;
      Y += 58;
      Y += 2;
      SetCursorPos(X, Y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      Sleep(100);
      mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
      Sleep(100);
      no = sgDefectList->Row;
   }
}
//---------------------------------------------------------------------------
// 2023 4 14 - chc Switch Lens/Intensity/Observation/DIC
void __fastcall TfrmInspection::SwitchInspectCondition(int no)
{
int lens,intensity;

   // 2023 4 25 - chc 加入name
   //lens = sgDefectList->Cells[10][MeasurePointNo].ToInt();
   //intensity = sgDefectList->Cells[11][MeasurePointNo].ToInt();
   lens = sgDefectList->Cells[10+1][MeasurePointNo].ToInt();
   intensity = sgDefectList->Cells[11+1][MeasurePointNo].ToInt();

   if(lens != frmInspection->rgBXFMLens->ItemIndex) {
      frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Lens switch to " + IntToStr(lens));
      frmInspection->rgBXFMLens->ItemIndex = lens;
      rgBXFMLensChange(this,lens);
      Sleep(1000);
      Application->ProcessMessages();
   }
   if(intensity != frmInspection->tbBXFMLMP->Position) {
      frnMain->WriteSystemLog("Micro " + IntToStr(MeasurePointNo) + ": Intensity switch to " + IntToStr(intensity));
      frmInspection->tbBXFMLMP->Position = intensity;
      frmInspection->SetIntensity(intensity);
      Sleep(1000);
      Application->ProcessMessages();
   }

   // Observation
   int observation,dicvalue;

   // 2023 4 25 - chc 加入name
   //observation = sgDefectList->Cells[12][MeasurePointNo].ToInt();
   //dicvalue = sgDefectList->Cells[13][MeasurePointNo].ToInt();
   observation = sgDefectList->Cells[12+1][MeasurePointNo].ToInt();
   dicvalue = sgDefectList->Cells[13+1][MeasurePointNo].ToInt();

   if(observation != frmInspection->rgObservation->ItemIndex) {
      // 會觸發Click做切換
      frmInspection->rgObservation->ItemIndex = observation;
      // DIC
      if(observation == 2) {
         Sleep(1000);
         Application->ProcessMessages();
         pnlDICBar->Color = clSilver;
         tbDICFC->Position = dicvalue;
         frnMain->DICFCMove(3,dicvalue);
         if(frnMain->pnlDICFCValue->Caption == "OUT" || frnMain->pnlDICFCValue->Caption == "")
            pnlDICBar->Color = clRed;
         else {
            pnlDICBar->Color = clLime;
         }
         gbDIC->Caption = "DIC:" + IntToStr(dicvalue);
      }
   }
}
//---------------------------------------------------------------------------
// 2023 4 26 - chc 顯示翻面訊息
void __fastcall TfrmInspection::pnlMacroJudageDoneClick(TObject *Sender)
{

   pnlMacroJudageDone->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::pnlMicroJudageDoneClick(TObject *Sender)
{

   pnlMicroJudageDone->Visible = false;
}
//---------------------------------------------------------------------------
// 2023 4 27 - chc DrawMap
void __fastcall TfrmInspection::btnDrawMapClick(TObject *Sender)
{

   btnWaferMapDrawClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::imPatternClick(TObject *Sender)
{

   pnlPattern->Visible = false;
}
//---------------------------------------------------------------------------
// 2023 5 5 - chc Save all die images
// boolFromSave=true: AutoSave & Judaged
bool boolStopSave = false;
void __fastcall TfrmInspection::btnSaveDieClick(TObject *Sender)
{
AnsiString filename,msg;
int no,col,row,xno,yno,x,y,dx,dy,lensno,ccdwidth,ccdheight,diew,dieh,sx,sy,r,c,count,tcount;
double resolution,fovx,fovy,fratio;

   // Update working col/row
   col = pnlWorkCol->Caption.ToInt();
   row = pnlWorkRow->Caption.ToInt();
   //btnColRowToClick(this);
   //frnMain->WaitTime(100);
   //UpdateMotionPosition();
   //UpdateCurrentPosition();

   // Lens
   lensno = rgBXFMLens->ItemIndex;
   resolution = frmRecipeSet->GetLensPrecision();
   // Pixel: CCDInfoAry[no].Width
   ccdwidth = frnMain->CCDInfoAry[0].Width;
   ccdheight = frnMain->CCDInfoAry[0].Height;
   // um
   fovx = ccdwidth * resolution;
   fovy = ccdheight * resolution;
   // um
   diew = edChipWidth->Text.ToInt();
   dieh = edChipHeight->Text.ToInt();
   // 次數
   xno = diew / fovx;
   yno = dieh / fovy;
   xno++;
   yno++;
   // 目前Table Position
   x = pnlX->Caption.ToInt();
   y = pnlY->Caption.ToInt();
   dx = (fovx/2) / frnMain->X_RESOLUTION;
   dy = (fovy/2) / frnMain->Y_RESOLUTION;
   msg.sprintf("目前XY(fovx,fovy,dx,dy)= %d,%d(%.2f,%.2f,%d,%d)",x,y,fovx,fovy,dx,dy);
   frnMain->WriteSystemLog(msg);
   // 移到左上角
   x -= ((fovx/2) / frnMain->X_RESOLUTION);
   y += ((fovy/2) / frnMain->Y_RESOLUTION);
   msg.sprintf("移動後XY= %d,%d",x,y);
   frnMain->WriteSystemLog(msg);
   MoveToXY(x,y);
   frnMain->WaitTime(300);
   UpdateMotionPosition();

   // 2023 5 6 - chc 不需要
   //UpdateCurrentPosition();

   Application->MessageBox("Left Upper","Die Save    ",MB_ICONINFORMATION|MB_OK);

   // 2023 5 6 - chc DieSaveOverlap
   int overlap;
   overlap = edDieSaveOverlap->Text.ToInt();
   if(overlap > 0) {
      fovx -= overlap;
      fovy -= overlap;
      // 次數
      xno = diew / fovx;
      yno = dieh / fovy;
      xno++;
      yno++;
   }

   // 移動量: pulse
   dx = fovx / frnMain->X_RESOLUTION;
   dy = fovy / frnMain->X_RESOLUTION;
   sx = x;
   sy = y;
   pnlMicroJudageDone->Visible = true;
   pnlMicroJudageDone->Caption = "Save: ---";
   btnStopSave->Visible = true;
   boolStopSave = false;
   pbDieSave->Position = 0;
   pbDieSave->Visible = true;
   count = 0;
   tcount = xno * yno;
   for(int r=0 ; r<yno ; r++) {
      if(boolStopSave == true)
         break;
      x = sx;
      for(int c=0 ; c<xno ; c++) {
         if(boolStopSave == true)
            break;
         msg.sprintf("MoveTo[%d,%d]: %d,%d",yno,xno,r+1,c+1);
         pnlMicroJudageDone->Caption = msg;
         pnlMicroJudageDone->Refresh();
         MoveToXY(x,y);
         frnMain->WaitTime(300);
         msg.sprintf("Save[%d,%d]: %d,%d",yno,xno,r+1,c+1);
         pnlMicroJudageDone->Caption = msg;
         pnlMicroJudageDone->Refresh();
         frnMain->WaitTime(100);
         // 存jpeg
         if(rgFileFormat->ItemIndex == 0)
            filename.sprintf("%s\\%s_%d_PSS_%d_%d_%d_%d.jpeg",frnMain->InspectMicroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,r+1,c+1);
         else
            filename.sprintf("%s\\%s_%d_PSS_%d_%d_%d_%d.bmp",frnMain->InspectMicroPhotoPath.c_str(),frnMain->InspectLotID.c_str(),
                                       frnMain->InspectWaferNo,col,row,r+1,c+1);
         x -= dx;
         // 存jpeg
         if(rgFileFormat->ItemIndex == 0) {
            TJPEGImage *jpeg;
            jpeg = new TJPEGImage;
            jpeg->Assign(imInspection->Picture->Graphic);
            jpeg->CompressionQuality = 95;
            jpeg->SaveToFile(filename);
            delete jpeg;
         }
         else
            imInspection->Picture->Bitmap->SaveToFile(filename);
         pnlSystemMessage->Caption = "Saved: " + filename;
         //msg.sprintf("(row,col)=(%d,%d)",r+1,c+1);
         //Application->MessageBox(msg.c_str(),"Die Save    ",MB_ICONINFORMATION|MB_OK);

         count++;
         fratio = (count * 100) / tcount;
         pbDieSave->Position = (int)fratio;
         pbDieSave->Refresh();
      }
      y += dy;
   }
   pnlMicroJudageDone->Visible = false;
   btnStopSave->Visible = false;
   pbDieSave->Visible = false;
}
//---------------------------------------------------------------------------
// 2023 5 5 - chc Sub die Judge
void __fastcall TfrmInspection::mnAddSubDieClick(TObject *Sender)
{
int row,col,dno,r,c,srow,scol;
bool bexist = false;

   // 取得目前XY
   col = pnlWorkCol->Caption.ToInt();
   row = pnlWorkRow->Caption.ToInt();
   srow = sgDie->Row;
   scol = sgDie->Col;
   // Exist?
   dno = sgDie->RowCount - 2;
   for(int i=1 ; i<=dno ; i++) {
      c = sgDefectList->Cells[4+1][i].ToInt();
      r = sgDefectList->Cells[5+1][i].ToInt();

      if(c == col && r == row) {
         pnlSystemMessage->Caption = "Defect Existed!";
         bexist = true;
         break;
      }
   }
   if(bexist == false) {
      AddPointData();
      btnWaferMapDrawClick(this);
   }
}
//---------------------------------------------------------------------------
// 2023 5 5 - chc SubDie judge
void __fastcall TfrmInspection::sgDieMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
int no;

   if(Button == mbRight) {
      no = sgDie->Row;
      X += pnlDie->Left;
      X += sgDie->Left;
      X += frmInspection->Left;
      X += 12;
      Y += pnlDie->Top;
      Y += sgDie->Top;
      Y += frmInspection->Top;
      Y += 0;
      Y += 0;
      SetCursorPos(X, Y);
      mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
      Sleep(100);
      mouse_event(MOUSEEVENTF_LEFTUP,0, 0, 0, 0);
      Sleep(100);
      no = sgDie->Row;
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::btnStopSaveClick(TObject *Sender)
{

   boolStopSave = true;
   frnMain->WriteSystemLog("DieSave: Stop");
}
//---------------------------------------------------------------------------

void __fastcall TfrmInspection::shCurretnPositionMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
int no,left,top,x,y;

   left = shCurretnPosition->Left;
   top = shCurretnPosition->Top;
   x = shCurretnPosition->Left - imWaferMapSrc->Left + X;
   y = shCurretnPosition->Top - imWaferMapSrc->Top + Y;
   imWaferMapSrcMouseMove(this,Shift,x,y);
}
//---------------------------------------------------------------------------


