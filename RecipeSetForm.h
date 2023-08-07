//---------------------------------------------------------------------------

#ifndef RecipeSetFormH
#define RecipeSetFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "LMDControl.hpp"
#include "LMDCustomBevelPanel.hpp"
#include "LMDCustomControl.hpp"
#include "LMDCustomPanel.hpp"
#include "LMDCustomPanelFill.hpp"
#include "LMDCustomParentPanel.hpp"
#include "LMDCustomSheetControl.hpp"
#include "LMDPageControl.hpp"
#include <ExtCtrls.hpp>
#include "LMDButton.hpp"
#include "LMDCustomButton.hpp"
#include <ComCtrls.hpp>
#include <Graphics.hpp>
#include "LMDButtonControl.hpp"
#include "LMDCheckBox.hpp"
#include "LMDCustomCheckBox.hpp"
#include "LMDCustomButtonGroup.hpp"
#include "LMDCustomGroupBox.hpp"
#include "LMDCustomRadioGroup.hpp"
#include "LMDCustomTrackBar.hpp"
#include "LMDRadioGroup.hpp"
#include "LMDTrackBar.hpp"
#include <Buttons.hpp>
#include <Grids.hpp>
#include "LMDBaseEdit.hpp"
#include "LMDCustomEdit.hpp"
#include "LMDEdit.hpp"
#include <Dialogs.hpp>
#include "LMDGroupBox.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmRecipeSet : public TForm
{
__published:	// IDE-managed Components
        TLMDPageControl *pcRecipeSet;
        TLMDTabSheet *tsSystemConfiguration;
        TLMDTabSheet *tsWaferInformation;
        TPanel *Panel1;
        TPanel *Panel2;
        TGroupBox *GroupBox1;
        TEdit *edMacroPath;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *edMicroPath;
        TGroupBox *GroupBox2;
        TLabel *Label3;
        TEdit *edResultPath;
        TGroupBox *GroupBox3;
        TLabel *Label4;
        TEdit *edWaferTxtPath;
        TPanel *pnlRecipeLoad;
        TComboBox *combRecipe;
        TPageControl *pcWaferInformation;
        TTabSheet *tsMacro;
        TTabSheet *tsMicro;
        TGroupBox *gbMacroParameters;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TLabel *Label8;
        TLabel *Label11;
        TLabel *Label12;
        TLabel *Label13;
        TLabel *Label14;
        TRadioGroup *rgMacroTopLoadPosition;
        TEdit *edMacroTopHomeDuration;
        TEdit *edMacroTopLeftDuration;
        TEdit *edMacroTopRightDuration;
        TEdit *edMacroTopViewDuration;
        TLMDButton *btnMacroTopTo;
        TPanel *pnlMacroTopTo;
        TEdit *edLotID;
        TTabSheet *tsWaferID;
        TGroupBox *gbWaferIDParameters;
        TRadioGroup *rgWaferIDDegree1;
        TCheckBox *cbReadWaferID1;
        TLMDButton *btnSaveMacroParameters;
        TLabel *Label16;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label19;
        TLabel *Label20;
        TLabel *Label21;
        TLabel *Label22;
        TLabel *Label23;
        TRadioGroup *rgMacroBackLoadPosition;
        TEdit *edMacroBackHomeDuration;
        TEdit *edMacroBackLeftDuration;
        TEdit *edMacroBackRightDuration;
        TEdit *edMacroBackViewDuration;
        TLMDButton *btnMacroBackTo;
        TPanel *pnlMacroBackTo;
        TGroupBox *gbMacroOperation;
        TLMDButton *btnMacroInnerYMove;
        TPanel *pnlMacroInnerYMove;
        TLMDButton *btnMacroInnerXBack;
        TPanel *pnlMacroInnerXBack;
        TLMDButton *btnMacroInnerTMove;
        TPanel *pnlMacroInnerTMove;
        TLMDButton *btnMacroInnerTBack;
        TPanel *pnlMacroInnerTBack;
        TLMDButton *btnMacroInnerXMove;
        TPanel *pnlMacroInnerXMove;
        TLMDButton *btnMacroInnerYBack;
        TPanel *pnlMacroInnerYBack;
        TLMDButton *btnMacroHome;
        TPanel *pnlMacroHome;
        TGroupBox *gbWaferOperation;
        TLMDButton *LMDButton1;
        TPanel *Panel3;
        TLMDButton *LMDButton2;
        TPanel *Panel4;
        TLMDButton *LMDButton3;
        TPanel *Panel5;
        TLMDButton *btnMacroInverse;
        TPanel *pnlMacroInverse;
        TGroupBox *GroupBox4;
        TLabel *Label24;
        TEdit *edRecipePath;
        TRadioGroup *rgWaferStatus;
        TPanel *Panel6;
        TImage *imClose;
        TCheckBox *cbAutoSaveMacro;
        TPanel *pnlLeftCCD;
        TImage *imLeftCCD;
        TShape *shLeftHorizontal;
        TShape *shLeftVertical;
        TLMDCheckBox *cbCCDFullImageLR;
        TPanel *pnlRightCCD;
        TImage *imRightCCD;
        TShape *shRightHorizontal;
        TShape *shRightVertical;
        TPanel *pnlTopMacroLeftLamp;
        TPanel *pnlTopMacroRightLamp;
        TLabel *Label25;
        TLabel *Label26;
        TLabel *Label27;
        TPanel *pnlBackMacroLeftLamp;
        TPanel *pnlBackMacroRightLamp;
        TLabel *Label28;
        TLabel *Label29;
        TLabel *Label30;
        TPanel *Panel11;
        TLMDButton *btnSaveWafeID;
        TLMDButton *btnDeleteRecipe;
        TOpenDialog *opTxtFile;
        TPanel *pnlSlotTitle;
        TEdit *edSlotNo;
        TPanel *Panel17;
        TImage *imInspection;
        TShape *shHorizontal;
        TShape *shVertical;
        TShape *shCenter;
        TShape *shLeftUp;
        TShape *shLeft;
        TShape *shLeftDown;
        TShape *shDown;
        TShape *shRightDown;
        TShape *shRight;
        TShape *shRightUp;
        TShape *shUp;
        TShape *shMarkMatch;
        TPanel *pnlMainFactor;
        TRadioGroup *rgWaferDegree;
        TGroupBox *GroupBox10;
        TGroupBox *GroupBox14;
        TLMDRadioGroup *rgBXFMLens;
        TPanel *pnlBXFMLens;
        TGroupBox *GroupBox15;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label15;
        TButton *btnGetAFPos;
        TPanel *pnlAFUnit01;
        TPanel *pnlAFUnit001;
        TEdit *edAFMovUnit;
        TCheckBox *cbAF;
        TGroupBox *GroupBox16;
        TLabel *Label31;
        TLabel *Label32;
        TImage *Image2;
        TImage *Image3;
        TLabel *Label76;
        TButton *btnDefaultIntensity;
        TButton *btnDec;
        TButton *btnAdd;
        TLMDTrackBar *tbBXFMLMP;
        TPanel *pnlBXFMLMPValue;
        TPanel *pnlBXFMLampSet;
        TGroupBox *rgAS;
        TLabel *Label33;
        TSpeedButton *btnAS1;
        TSpeedButton *btnAS2;
        TSpeedButton *btnAS3;
        TSpeedButton *btnAS4;
        TSpeedButton *btnAS5;
        TSpeedButton *btnAS6;
        TLabel *Label93;
        TButton *btnDefaultAS;
        TPanel *pnlASValue;
        TGroupBox *gbDIC;
        TButton *btnDICNear;
        TButton *btnDICFar;
        TRadioGroup *rgObservation;
        TGroupBox *GroupBox36;
        TCheckBox *cbWBAuto;
        TLMDButton *btnWBOneTouch;
        TRadioGroup *rgCCDShutter;
        TPanel *pnlLensPrecision;
        TRadioGroup *rgWaferDieDirection;
        TRadioGroup *rgWaferAxisOrientation;
        TPanel *pnlWaferMap;
        TImage *imWaferMap;
        TShape *shMeasurePoint;
        TGroupBox *GroupBox7;
        TLabel *Label443;
        TLabel *Label444;
        TLabel *Label445;
        TLabel *Label446;
        TLabel *Label447;
        TLabel *Label452;
        TLabel *Label453;
        TLabel *Label454;
        TLabel *Label455;
        TLabel *Label448;
        TLabel *Label449;
        TLabel *Label37;
        TLabel *Label605;
        TEdit *edChipWidth;
        TEdit *edChipHeight;
        TEdit *edChipDx;
        TEdit *edChipDy;
        TPanel *pnlChipHNo;
        TPanel *pnlChipWNo;
        TEdit *Edit4;
        TEdit *edIgnore;
        TPanel *pnlWafer;
        TLabel *Label34;
        TLabel *Label35;
        TLabel *Label36;
        TLabel *Label450;
        TPanel *Panel9;
        TPanel *Panel10;
        TPanel *Panel14;
        TPanel *Panel15;
        TButton *Button34;
        TCheckBox *cbDieInsideAll;
        TComboBox *ComboBox1;
        TEdit *edSelectCol;
        TEdit *edSelectRow;
        TButton *btnColRowTo;
        TPanel *Panel51;
        TPanel *pnlWorkCol;
        TPanel *pnlWorkRow;
        TLMDButton *btnWaferMapDraw;
        TPanel *pnlChipCol;
        TPanel *pnlChipRow;
        TPanel *pnlCenterCol;
        TPanel *pnlCenterRow;
        TCheckBox *cbFreeze;
        TLMDCheckBox *cbCCDFullImage;
        TLMDButton *btnSaveMicroParameters;
        TPanel *Panel16;
        TShape *Shape1;
        TShape *Shape2;
        TShape *Shape3;
        TShape *Shape4;
        TGroupBox *GroupBox20;
        TSpeedButton *btnMoveLeftUp;
        TSpeedButton *btnMoveLeft;
        TSpeedButton *btnMoveLeftDown;
        TSpeedButton *btnMoveUp;
        TSpeedButton *btnMoveSpeed;
        TSpeedButton *btnMoveDown;
        TSpeedButton *btnMoveRightUp;
        TSpeedButton *btnMoveRight;
        TSpeedButton *btnMoveRightDown;
        TPanel *pnlX;
        TPanel *pnlY;
        TGroupBox *GroupBox21;
        TLabel *laImgTest;
        TLMDButton *btnMatchToCenter;
        TLMDButton *btnEraseMatchBox;
        TCheckBox *cbStdPattern;
        TCheckBox *cbMoveToCenterMatched;
        TLMDButton *btnAutoMatch;
        TLMDEdit *edMinScore;
        TPanel *pnlScore;
        TLMDButton *btnSavePattern;
        TLMDButton *btnAlignment;
        TLMDButton *btnCW1x;
        TLMDButton *btnCCW1x;
        TGroupBox *GroupBox19;
        TButton *btnNextPoint;
        TButton *btnPreviousPoint;
        TGroupBox *GroupBox6;
        TLMDButton *LMDButton4;
        TPanel *Panel18;
        TLMDButton *LMDButton5;
        TPanel *Panel19;
        TPanel *pnlSystemMessage;
        TLMDRadioGroup *rgSpeed;
        TRadioGroup *rgWaferType;
        TGroupBox *GroupBox8;
        TLabel *Label38;
        TEdit *edDeviceID;
        TGroupBox *GroupBox9;
        TLabel *Label39;
        TEdit *edInspectionStationID1;
        TEdit *edInspectionStationID2;
        TEdit *edInspectionStationID3;
        TLMDButton *btnSaveSystemParameters;
        TPanel *pnlAutoMatch;
        TRadioGroup *rgMatchMode;
        TLabel *Label434;
        TLMDEdit *edMinReduceArea;
        TPanel *pnlPattern;
        TImage *imPattern;
        TLabel *Label206;
        TComboBox *cbMatchNo;
        TPanel *pnlMatchedNo;
        TLabel *Label74;
        TPanel *pnlCCDX;
        TPanel *pnlCCDY;
        TLabel *Label75;
        TPanel *pnlCCDR;
        TPanel *pnlCCDB;
        TPanel *pnlCCDG;
        TLMDRadioGroup *rgBXFMFilter1;
        TLMDRadioGroup *rgBXFMFilter2;
        TLMDButton *btnMacroInnerPosition;
        TPanel *pnlMacroInnerPosition;
        TLMDButton *btnMacroInnerStop;
        TPanel *pnlMacroInnerStop;
        TLMDButton *btnMacroOuterPosition;
        TPanel *pnlMacroOuterPosition;
        TLMDButton *btnMacroOuterStop;
        TPanel *pnlMacroOuterStop;
        TLMDButton *btnMacroOuterTCCW;
        TPanel *pnlMacroOuterTCCW;
        TLMDButton *btnMacroOuterTCW;
        TPanel *pnlMacroOuterTCW;
        TRadioGroup *rgMoveStep;
        TLMDRadioGroup *rgMatchSource;
        TLMDButton *btnCheckAlignment;
        TPanel *pnlAlignment;
        TPanel *pnlCheckAlignment;
        TPanel *pnlBase1X;
        TPanel *pnlBase1Y;
        TLabel *Label40;
        TLabel *Label41;
        TPanel *pnlBase2X;
        TPanel *pnlBase2Y;
        TPanel *pnlPositionX;
        TPanel *pnlPositionY;
        TPanel *pnlStdZ;
        TLMDCheckBox *cbRotateDirection;
        TPanel *pnlRemoteRotate;
        TLMDCheckBox *cbDirectMove;
        TPanel *pnlTRemoteDegree;
        TPanel *pnlTRemoteOffset;
        TPanel *pnlDegree;
        TLMDCheckBox *cbDirectMoveDirection;
        TSpeedButton *SpeedButton21;
        TPanel *pnlZ;
        TPanel *pnlStdT;
        TPanel *pnlT;
        TShape *shCurretnPosition;
        TRadioGroup *rgLeftCCDShutter;
        TRadioGroup *rgRightCCDShutter;
        TButton *btnAFUp;
        TButton *btnAFDown;
        TLabel *Label42;
        TLabel *Label43;
        TPanel *pnlStageMoving;
        TTimer *tmUpdatePosition;
        TCheckBox *cbAutoExposure;
        TPanel *pnlDICNear;
        TPanel *pnlDICFar;
        TLMDTrackBar *tbDICFC;
        TPanel *pnlDICBar;
        TPanel *pnlObservation;
        TCheckBox *cbMicroCrossLine;
        TLMDCheckBox *cbTableDirection;
        TLMDCheckBox *cbJoystickMoveContinue;
        TCheckBox *cbAutoSaveMicro;
        TPanel *pnlAFPos;
        TLMDButton *btnTOrg;
        TPageControl *pcDefectList;
        TTabSheet *tsDefectList;
        TTabSheet *tsBinCodeList;
        TStringGrid *sgDefectList;
        TStringGrid *sgBINCode;
        TStringGrid *sgDie;
        TSpeedButton *btnAS0;
        TLabel *Label44;
        TLMDGroupBox *gbLP1Edge;
        TLMDGroupBox *gbAOICenter;
        TLabel *Label153;
        TLabel *Label154;
        TLabel *Label90;
        TLMDEdit *ed12EdgeX1;
        TLMDEdit *ed12EdgeY1;
        TLMDButton *btn12Edge1;
        TLMDButton *btnTo12Edge1;
        TLMDEdit *ed12EdgeZ1;
        TPanel *pnlTo12Edge1;
        TLMDGroupBox *LMDGroupBox3;
        TLabel *Label108;
        TLabel *Label109;
        TLabel *Label110;
        TLMDEdit *ed12EdgeX2;
        TLMDEdit *ed12EdgeY2;
        TLMDButton *btn12Edge2;
        TLMDButton *btnTo12Edge2;
        TLMDEdit *ed12EdgeZ2;
        TPanel *pnlTo12Edge2;
        TLMDGroupBox *LMDGroupBox4;
        TLabel *Label117;
        TLabel *Label118;
        TLabel *Label119;
        TLMDEdit *ed12EdgeX3;
        TLMDEdit *ed12EdgeY3;
        TLMDButton *btn12Edge3;
        TLMDButton *btnTo12Edge3;
        TLMDEdit *ed12EdgeZ3;
        TPanel *pnlTo12Edge3;
        TLMDGroupBox *gbLP2Edge;
        TLMDGroupBox *LMDGroupBox5;
        TLabel *Label45;
        TLabel *Label46;
        TLabel *Label47;
        TLMDEdit *ed8EdgeX1;
        TLMDEdit *ed8EdgeY1;
        TLMDButton *btn8Edge1;
        TLMDButton *btnTo8Edge1;
        TLMDEdit *ed8EdgeZ1;
        TPanel *pnlTo8Edge1;
        TLMDGroupBox *LMDGroupBox6;
        TLabel *Label48;
        TLabel *Label49;
        TLabel *Label50;
        TLMDEdit *ed8EdgeX2;
        TLMDEdit *ed8EdgeY2;
        TLMDButton *btn8Edge2;
        TLMDButton *btnTo8Edge2;
        TLMDEdit *ed8EdgeZ2;
        TPanel *pnlTo8Edge2;
        TLMDGroupBox *LMDGroupBox7;
        TLabel *Label51;
        TLabel *Label52;
        TLabel *Label53;
        TLMDEdit *ed8EdgeX3;
        TLMDEdit *ed8EdgeY3;
        TLMDButton *btn8Edge3;
        TLMDButton *btnTo8Edge3;
        TLMDEdit *ed8EdgeZ3;
        TPanel *pnlTo8Edge3;
        TLabel *Label54;
        TRadioGroup *rgEdgeNo;
        TButton *btnToEdge;
        TButton *btnFindCenter;
        TPanel *pnlCenterX;
        TPanel *pnlCenterY;
        TLMDButton *btnLoadTestImage;
        TLabel *Label55;
        TEdit *edSubDieXno;
        TLabel *Label57;
        TEdit *edSubDieYno;
        TRadioGroup *rgWaferIDDegree2;
        TCheckBox *cbReadWaferID2;
        TLMDButton *btnBackwardZ1;
        TLMDButton *btnMoveZ1;
        TTimer *tmFindCenter;
        TPopupMenu *pmPoint;
        TMenuItem *mnDeletePoint;
        TPopupMenu *pmMap;
        TMenuItem *mnAddPoint;
        TMenuItem *mnDeleteAllPoint;
        TEdit *edSubDieWidth;
        TEdit *edSubDieHeight;
        TGroupBox *GroupBox5;
        TLabel *Label56;
        TEdit *edAlignmentDefault;
        TTabSheet *tsBinCodeSelect;
        TCheckBox *cbGlassIQCF;
        TCheckBox *cbGlassIQCB;
        TCheckBox *cbGlassOQCF;
        TCheckBox *cbGlassOQCB;
        TCheckBox *cbSiIQCSS;
        TCheckBox *cbSiIQCLS;
        TCheckBox *cbSiIQCPad;
        TCheckBox *cbSiIQCSA;
        TCheckBox *cbSiOQCSS;
        TCheckBox *cbSiOQCLS;
        TCheckBox *cbSiOQCPad;
        TCheckBox *cbSiOQCW;
        TStringGrid *sgBinCodeItem;
        TButton *btnGlassIQCF;
        TButton *btnGlassIQCB;
        TButton *btnGlassOQCF;
        TButton *btnGlassOQCB;
        TButton *btnSiIQCPad;
        TButton *btnSiIQCSA;
        TButton *btnSiIQCLS;
        TButton *btnSiIQCSS;
        TButton *btnSiOQCPad;
        TButton *btnSiOQCW;
        TButton *btnSiOQCLS;
        TButton *btnSiOQCSS;
        TButton *btnBinCodeUpdate;
        TCheckBox *cbGood;
        TButton *btnGood;
        TCheckBox *cbNA;
        TButton *btnNA;
        TGroupBox *GroupBox11;
        TRadioGroup *rgWaferInLP1Degree;
        TLMDButton *btnSaveWafeDegree;
        TRadioGroup *rgWaferOutLP1Degree;
        TRadioGroup *rgWaferInLP2Degree;
        TRadioGroup *rgWaferOutLP2Degree;
        TCheckBox *cbMacroTopCrossLine;
        TCheckBox *cbMacroBackCrossLine;
        TRadioGroup *rgImageSaveMode;
        TLabel *Label58;
        TEdit *edMinYield;
        TPanel *pnlAF;
        TGroupBox *GroupBox12;
        TLabel *Label59;
        TEdit *edCognexPath;
        TLMDButton *btnCognexRun;
        TLabel *Label60;
        TEdit *edCognexTitle;
        TLMDButton *btnSaveWaferInformation;
        TLMDRadioGroup *rg12EdgeLens;
        TLMDRadioGroup *rg8EdgeLens;
        TLabel *Label61;
        TEdit *ed12EdgeIntensity1;
        TLabel *Label63;
        TEdit *ed12EdgeIntensity2;
        TLabel *Label64;
        TEdit *ed12EdgeIntensity3;
        TLabel *Label65;
        TEdit *ed8EdgeIntensity1;
        TLabel *Label66;
        TEdit *ed8EdgeIntensity2;
        TLabel *Label67;
        TEdit *ed8EdgeIntensity3;
        TPanel *Panel7;
        TImage *Image1;
        TPanel *Panel8;
        TPanel *Panel12;
        TPanel *Panel13;
        TPanel *Panel21;
        TPanel *Panel22;
        TPanel *Panel23;
        TImage *Image4;
        TPanel *Panel24;
        TPanel *Panel25;
        TPanel *Panel26;
        TPanel *Panel27;
        TPanel *Panel28;
        TPanel *Panel29;
        TImage *Image5;
        TPanel *Panel30;
        TPanel *Panel31;
        TPanel *Panel32;
        TPanel *Panel33;
        TPanel *Panel34;
        TPanel *Panel35;
        TPanel *Panel36;
        TPanel *Panel37;
        TPanel *Panel38;
        TGroupBox *GroupBox13;
        TLabel *Label62;
        TEdit *edLampLife;
        TGroupBox *GroupBox17;
        TLabel *Label68;
        TEdit *edDieSaveOverlap;
        TLabel *Label69;
        TTimer *tm250ms;
        void __fastcall btnMacroHomeClick(TObject *Sender);
        void __fastcall btnMacroInnerYMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerYMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerXBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerXMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerYBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInverseClick(TObject *Sender);
        void __fastcall btnMacroTopToClick(TObject *Sender);
        void __fastcall btnMacroBackToClick(TObject *Sender);
        void __fastcall imCloseClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall pcWaferInformationChange(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall FormMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall rgLeftCCDShutterClick(TObject *Sender);
        void __fastcall rgRightCCDShutterClick(TObject *Sender);
        void __fastcall pnlRecipeLoadClick(TObject *Sender);
        void __fastcall btnSaveMacroParametersClick(TObject *Sender);
        void __fastcall cbStdPatternClick(TObject *Sender);
        void __fastcall btnMatchToCenterClick(TObject *Sender);
        void __fastcall btnEraseMatchBoxClick(TObject *Sender);
        void __fastcall btnSavePatternClick(TObject *Sender);
        void __fastcall imInspectionDblClick(TObject *Sender);
        void __fastcall imInspectionMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnWaferMapDrawClick(TObject *Sender);
        void __fastcall shCenterMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shCenterMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shCenterMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shLeftUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shLeftUpMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shLeftUpMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shLeftMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shLeftMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall shLeftMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall shLeftDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shLeftDownMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shLeftDownMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shUpMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall shUpMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall shUpMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall shDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shDownMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall shDownMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall shRightUpMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shRightUpMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shRightUpMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shRightMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shRightMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shRightMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shRightDownMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shRightDownMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shRightDownMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sgDefectListDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall btnAlignmentClick(TObject *Sender);
        void __fastcall btnAutoMatchClick(TObject *Sender);
        void __fastcall cbCCDFullImageChange(TObject *Sender);
        void __fastcall imPatternClick(TObject *Sender);
        void __fastcall imInspectionMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall btnMoveLeftUpClick(TObject *Sender);
        void __fastcall btnMoveLeftClick(TObject *Sender);
        void __fastcall btnMoveLeftDownClick(TObject *Sender);
        void __fastcall btnMoveUpClick(TObject *Sender);
        void __fastcall btnMoveSpeedClick(TObject *Sender);
        void __fastcall btnMoveDownClick(TObject *Sender);
        void __fastcall btnMoveRightUpClick(TObject *Sender);
        void __fastcall btnMoveRightClick(TObject *Sender);
        void __fastcall btnMoveRightDownClick(TObject *Sender);
        void __fastcall btnCW1xClick(TObject *Sender);
        void __fastcall btnCCW1xClick(TObject *Sender);
        void __fastcall rgCCDShutterClick(TObject *Sender);
        void __fastcall rgBXFMLensChange(TObject *Sender, int ButtonIndex);
        void __fastcall tbBXFMLMPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnDefaultIntensityClick(TObject *Sender);
        void __fastcall btnDecClick(TObject *Sender);
        void __fastcall btnAddClick(TObject *Sender);
        void __fastcall btnDefaultASClick(TObject *Sender);
        void __fastcall btnAS2Click(TObject *Sender);
        void __fastcall btnAS1Click(TObject *Sender);
        void __fastcall btnAS3Click(TObject *Sender);
        void __fastcall btnAS4Click(TObject *Sender);
        void __fastcall btnAS5Click(TObject *Sender);
        void __fastcall btnAS6Click(TObject *Sender);
        void __fastcall rgMoveStepClick(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall pnlBase1XDblClick(TObject *Sender);
        void __fastcall pnlBase2XDblClick(TObject *Sender);
        void __fastcall pnlPositionXDblClick(TObject *Sender);
        void __fastcall imWaferMapMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall imWaferMapMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imWaferMapDblClick(TObject *Sender);
        void __fastcall btnColRowToClick(TObject *Sender);
        void __fastcall cbAFClick(TObject *Sender);
        void __fastcall pnlAFUnit01Click(TObject *Sender);
        void __fastcall pnlAFUnit001Click(TObject *Sender);
        void __fastcall btnGetAFPosClick(TObject *Sender);
        void __fastcall btnAFUpClick(TObject *Sender);
        void __fastcall btnAFDownClick(TObject *Sender);
        void __fastcall rgObservationClick(TObject *Sender);
        void __fastcall tmUpdatePositionTimer(TObject *Sender);
        void __fastcall combRecipeChange(TObject *Sender);
        void __fastcall cbAutoExposureClick(TObject *Sender);
        void __fastcall laImgTestClick(TObject *Sender);
        void __fastcall btnDICNearClick(TObject *Sender);
        void __fastcall btnDICFarClick(TObject *Sender);
        void __fastcall tbDICFCMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbDICFCMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall tbDICFCMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall cbMicroCrossLineClick(TObject *Sender);
        void __fastcall cbMacroTopCrossLineClick(TObject *Sender);
        void __fastcall cbMacroBackCrossLineClick(TObject *Sender);
        void __fastcall cbTableDirectionClick(TObject *Sender);
        void __fastcall cbJoystickMoveContinueClick(TObject *Sender);
        void __fastcall btnSaveMicroParametersClick(TObject *Sender);
        void __fastcall btnTOrgClick(TObject *Sender);
        void __fastcall btnSaveWafeIDClick(TObject *Sender);
        void __fastcall sgBINCodeDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall sgDieDblClick(TObject *Sender);
        void __fastcall btnAS0Click(TObject *Sender);
        void __fastcall btnSaveSystemParametersClick(TObject *Sender);
        void __fastcall btn12Edge1Click(TObject *Sender);
        void __fastcall btn12Edge2Click(TObject *Sender);
        void __fastcall btn12Edge3Click(TObject *Sender);
        void __fastcall btn8Edge1Click(TObject *Sender);
        void __fastcall btn8Edge2Click(TObject *Sender);
        void __fastcall btn8Edge3Click(TObject *Sender);
        void __fastcall btnTo12Edge1Click(TObject *Sender);
        void __fastcall btnTo12Edge2Click(TObject *Sender);
        void __fastcall btnTo12Edge3Click(TObject *Sender);
        void __fastcall btnTo8Edge1Click(TObject *Sender);
        void __fastcall btnTo8Edge2Click(TObject *Sender);
        void __fastcall btnTo8Edge3Click(TObject *Sender);
        void __fastcall btnToEdgeClick(TObject *Sender);
        void __fastcall btnFindCenterClick(TObject *Sender);
        void __fastcall pnlCenterXDblClick(TObject *Sender);
        void __fastcall cbFreezeClick(TObject *Sender);
        void __fastcall btnLoadTestImageClick(TObject *Sender);
        void __fastcall imInspectionMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall sgDieMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall btnBackwardZ1Click(TObject *Sender);
        void __fastcall btnMoveZ1Click(TObject *Sender);
        void __fastcall tmFindCenterTimer(TObject *Sender);
        void __fastcall btnCW1xMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCW1xMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCCW1xMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCCW1xMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall mnDeletePointClick(TObject *Sender);
        void __fastcall mnDeleteAllPointClick(TObject *Sender);
        void __fastcall btnDeleteRecipeClick(TObject *Sender);
        void __fastcall mnAddPointClick(TObject *Sender);
        void __fastcall btnGlassIQCFClick(TObject *Sender);
        void __fastcall btnGlassIQCBClick(TObject *Sender);
        void __fastcall btnGlassOQCFClick(TObject *Sender);
        void __fastcall btnGlassOQCBClick(TObject *Sender);
        void __fastcall btnSiIQCSSClick(TObject *Sender);
        void __fastcall btnSiIQCLSClick(TObject *Sender);
        void __fastcall btnSiIQCPadClick(TObject *Sender);
        void __fastcall btnSiIQCSAClick(TObject *Sender);
        void __fastcall btnSiOQCSSClick(TObject *Sender);
        void __fastcall btnSiOQCLSClick(TObject *Sender);
        void __fastcall btnSiOQCPadClick(TObject *Sender);
        void __fastcall btnSiOQCWClick(TObject *Sender);
        void __fastcall btnBinCodeUpdateClick(TObject *Sender);
        void __fastcall sgBinCodeItemDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
        void __fastcall btnGoodClick(TObject *Sender);
        void __fastcall btnNAClick(TObject *Sender);
        void __fastcall sgDefectListMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnSaveWafeDegreeClick(TObject *Sender);
        void __fastcall btnCognexRunClick(TObject *Sender);
        void __fastcall btnSaveWaferInformationClick(TObject *Sender);
        void __fastcall sgDefectListClick(TObject *Sender);
        void __fastcall sgDefectListDblClick(TObject *Sender);
        void __fastcall tm250msTimer(TObject *Sender);
        void __fastcall shCurretnPositionMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall combRecipeKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall combRecipeExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmRecipeSet(TComponent* Owner);

        #define MACRO_HOME              0
        #define MACRO_LEFT              1
        #define MACRO_RIGHT             2
        #define MACRO_VIEW              3
        #define MACRO_ALL               4

   void __fastcall TfrmRecipeSet::LoadTXTFile(AnsiString fname);
   void __fastcall TfrmRecipeSet::UpdateCenterDotPosition();
   double __fastcall TfrmRecipeSet::GetLensPrecision();
   void __fastcall TfrmRecipeSet::DrawBigImage();
   struct WAFER_STRU    {
      int X,Y,Dx,Dy;
      bool boolSet;
      int Width,Height;
      int Cx,Cy;
   };
   struct WAFER_STRU Wafer;
   void __fastcall TfrmRecipeSet::DrawRowCol(int sx,int sy,int col,int row,int type, int factor);
   // 記錄作業時間長度
   long NCCStartTime,NCCStopTime,NCCElapsedms;
   short NCCStartTick,NCCStopTick;
   int MarkCenterX,MarkCenterY;
   bool __fastcall TfrmRecipeSet::DoNCCMatch(int stdwidth,int stdheight, AnsiString patternfname);
   bool __fastcall TfrmRecipeSet::CheckNCCMatch();

   // CCD FOV vs CCDImage Area
   int CCDRangeX,CCDRangeY,CCDDx,CCDDy;
   int LaserCenterX,LaserCenterY;
   int ImageFactor,ImageLeftFactor,ImageRightFactor;
   void __fastcall TfrmRecipeSet::SetIntensity(int no);
   void __fastcall TfrmRecipeSet::UpdateMotionPosition();
   void __fastcall TfrmRecipeSet::RemoteRotate(int x1,int y1, int x2, int y2);
   int RotateLeftXPos,RotateRightXPos;
   int RotateLeftYPos,RotateRightYPos;
   // 加入Position的記錄, 否則長距離的轉正與單畫面的轉正方向相反!!
   int RotateLeftXCmd,RotateRightXCmd;
   int RotateLeftYCmd,RotateRightYCmd;
   void __fastcall TfrmRecipeSet::MoveToXYZT(int x,int y,int z,int t);
   void __fastcall TfrmRecipeSet::UpdateCurrentPosition();
   void __fastcall TfrmRecipeSet::InitWaferMap();
   void __fastcall TfrmRecipeSet::GetStatus();

// Save Micro Recipe
// RecipeName
// WaferSize: 12"/8"
// Pattern: Base1/Base2/Position XYZT
// Match: MinScore, MatchMo
// BXFM: ObjLens,Intensity,AS,Observation, AF Active
// Chip: Width,Height,GapX,GapY,Ignore
// CCD: Shutter, WhiteBalance
// DIC: Active, FCNear, FCFar
//
struct EDGE_STRU {
   int X,Y,Z;

   // 2023 4 24 - chc 加入Edge Intensit/Lens
   int Intensity;

};
struct XYZT_STRU {
   int X,Y,Z,T;
};
struct PATTERN_STRU {
   struct XYZT_STRU Base1;
   struct XYZT_STRU Base2;
   struct XYZT_STRU Position;

   // 2023 4 24 - chc 存Match: Intensity & Lens
   int Intensity;
   int Lens;

   // 2023 5 3 - chc 中心點
   struct XYZT_STRU Center;

   // 2023 5 4 - chc Position
   int PositionIntensity;

   // 2023 5 9 - chc 漏Z/T
   int StdZ,StdT;

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
   int SubDieXNo,SubDieYNo;

   // 2023 4 5 - chc SubDieWidth/Height
   int SubDieWidth,SubDieHeight;

};
struct CCD_STRU {
   int Shutter;
   bool boolAutoWB;
};
struct DIC_STRU {
   int FCValue;
   bool boolActive;
};
#define IMAGE_SAVE_BINCODE      0
#define IMAGE_SAVE_NONE         1
struct MICRO_RECIPE_STRU {
   struct PATTERN_STRU Pattern;
   struct MATCH_STRU Match;
   struct BXFM_STRU BXFM;
   struct DIC_STRU DIC;
   struct CCD_STRU CCD;
   struct CHIP_STRU Chip;
   bool boolAutoSave;

   // 2023 4 11 - chc ImageSaveMode
   int ImageSaveMode;

   // 2023 4 12 - chc MinYield
   double MinYield;

   // 2023 5 5 - chc all Die: boolDieInsideAll
   bool boolDieInsideAll;

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
   bool boolAutoSave;
};
struct WAFERID_STRU {
   int Degree1,Degree2;
   bool boolActive1,boolActive2;

   // 2023 4 5a - chc Wafer Direction
   int InLP1Degree,OutLP1Degree;
   int InLP2Degree,OutLP2Degree;

};

// 2023 3 30 - chc BinCode List & Item
#define MAX_BINCODE_TXTNO          20
#define MAX_BINCODE_ITEMNO         200
struct BINCODE_STRU {
   bool bSelect[MAX_BINCODE_TXTNO];
   AnsiString TxtName[MAX_BINCODE_TXTNO];
   int ID[MAX_BINCODE_ITEMNO];
   AnsiString IDName[MAX_BINCODE_ITEMNO];
   TColor Color[MAX_BINCODE_ITEMNO];
   int TxtNo,ItemNo;
   int No[MAX_BINCODE_ITEMNO];
   double Percent[MAX_BINCODE_ITEMNO];
};

// 2023 4 1 - chc DefectList
// No, ID, X, Y, Col, Row Judge, XSize, YSize, Area
#define MAX_DEFECT_NO          200
struct DEFECTDATA_STRU {
   int ID,X,Y,Col,Row,Judge;
   double XSize,YSize,Area;

   // 2023 4 7 - chc 加入Lens/Intensity
   int Lens,Intensity;

   // 2023 4 14 - chc 加入BF/DF/DIC選項 & DIC強度 : Observation,DICValue
   int Observation,DICValue;

   // 2023 4 25 - chc 加入name
   AnsiString IDName;

   // 2023 5 6 - chc 是否為SubDie?
   bool boolSubDie;

};
struct DEFECTLIST_STRU {
   struct DEFECTDATA_STRU Data[MAX_DEFECT_NO];
   int No;
};

// Recipe
struct RECIPE_STRU {
   AnsiString Name,Date,Time;
   int WaferSize;                                                               // 8/12
   struct MICRO_RECIPE_STRU MicroRecipe;
   struct MACRO_RECIPE_STRU MacroRecipe;
   struct WAFERID_STRU WaferID;
   struct BINCODE_STRU BinCode;
   struct DEFECTLIST_STRU DefectList;
};
//
   struct RECIPE_STRU RecipeBufferEdit;
   struct RECIPE_STRU RecipeBufferRun;
   bool __fastcall TfrmRecipeSet::ReadRecipe(AnsiString rfname,int mode);
   void __fastcall TfrmRecipeSet::SetupBINCode();

   // 2023 3 13 - chc System Parameters
struct SYSTEM_STRU {
   AnsiString ResultPath,TxtPath,RecipePath,DeviceID;
   AnsiString InspectionStationID[3];
   int FilterFW1,FilterFW2;
   bool TableDirection;
   bool JoystickContinueMove;
   struct EDGE_STRU W12Edge[3];
   struct EDGE_STRU W8Edge[3];

   // 2023 4 19 - chc 加入Cognex
   AnsiString CognexPath,CognexTitle;
   int AlignmentDefault;

   // 2023 4 24 - chc 加入Edge Intensit/Lens
   int W12EdgeLens;
   int W8EdgeLens;

   // 2023 5 5 - chc Lamp Life
   int LampLife;

   // 2023 5 6 - chc DieSaveOverlap
   int DieSaveOverlap;

};
   struct SYSTEM_STRU SystemConfig;
   AnsiString SystemConfigName;

   // 2023 3 16 - chc Move to XY
   void __fastcall TfrmRecipeSet::FindCenter(int x1,int y1,int x2,int y2,int x3,int y3,int *cx,int *cy);
   void __fastcall TfrmRecipeSet::FindEdge(int no,int *x,int *y);
   void __fastcall TfrmRecipeSet::MoveToXYFun(int x,int y);

   // 2023 3 20 - chc Draw Sub die
   void __fastcall TfrmRecipeSet::DrawSubDie();
   // 2023 3 20 - chc Load System Parameters
   void __fastcall TfrmRecipeSet::LoadSystemParameters();

   void __fastcall TfrmRecipeSet::DrawBigImage_new();
   // 2023 3 21 - chc Sub-Die
   void __fastcall TfrmRecipeSet::DrawSubDieMap(int sx,int sy,int ex,int ey);
   void __fastcall TfrmRecipeSet::AddPointData();
   // 2023 3 29a - chc DefectList Title
   void __fastcall TfrmRecipeSet::DefectListTitle();
   void __fastcall TfrmRecipeSet::BinCodeItemTitle();
   void __fastcall TfrmRecipeSet::BinCodeRead(AnsiString fname);
   // 2023 3 30 - chc BinCode Color
   TColor __fastcall TfrmRecipeSet::GetBinCodeColor(i);

   // 2023 4 1 - chc Create RecipeID List: combRecipe
   void __fastcall TfrmRecipeSet::CreateRecipeID();
   void __fastcall TfrmRecipeSet::BinCodeTitle();

   // 2023 4 28 - chc Capture cnt: CaptureCount
   int CaptureCount;
   
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRecipeSet *frmRecipeSet;
//---------------------------------------------------------------------------
#endif
