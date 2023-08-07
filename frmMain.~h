//---------------------------------------------------------------------------

#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>

#include <string.h>
#include <stdio.h>
#include <math.h>
// Random
#include <stdlib.h>
// TimeTic
#include <time.h>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include "LMDButton.hpp"
#include "LMDControl.hpp"
#include "LMDCustomBevelPanel.hpp"
#include "LMDCustomButton.hpp"
#include "LMDCustomControl.hpp"
#include "LMDCustomGroupBox.hpp"
#include "LMDCustomPanel.hpp"
#include "LMDCustomPanelFill.hpp"
#include "LMDCustomParentPanel.hpp"
#include "LMDCustomSheetControl.hpp"
#include "LMDGroupBox.hpp"
#include "LMDPageControl.hpp"
#include <Graphics.hpp>
#include "LMDCustomButtonGroup.hpp"
#include "LMDCustomRadioGroup.hpp"
#include "LMDRadioGroup.hpp"
#include "ICImagingControl3_OCX.h"
#include "LMDBaseEdit.hpp"
#include "LMDButtonControl.hpp"
#include "LMDCheckBox.hpp"
#include "LMDCustomCheckBox.hpp"
#include "LMDCustomEdit.hpp"
#include "LMDCustomTrackBar.hpp"
#include "LMDEdit.hpp"
#include "LMDTrackBar.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <OleCtrls.hpp>
#include <Chart.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ExtDlgs.hpp>
#include "YbCommDevice.h"
#include "LMDComboBox.hpp"
#include "LMDCustomComboBox.hpp"
#include "LMDThemedComboBox.hpp"
#include <Mask.hpp>
#include <Grids.hpp>
#include "LMDBaseControl.hpp"
#include "LMDBaseGraphicControl.hpp"
#include "LMDBaseMeter.hpp"
#include "LMDCustomProgress.hpp"
#include "LMDGraphicControl.hpp"
#include "LMDProgress.hpp"
#include <jpeg.hpp>
#include "LMDPNGImage.hpp"
#include <sys\timeb.h>
#include <Filectrl.hpp>
#include <dir.h>

// Euresys eVision
#include "EmcLegacy.h"
#include "EImage.h"

// APS 7856
#include "type_def.h"
#include "APS168.h"
#include "APS_Define.h"
#include "ErrorCodeDef.h"

//---------------------------------------------------------------------------
class TfrnMain : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *pmSocketMessage;
        TMenuItem *mnSaveToFile;
        TMenuItem *mnClearMessage;
        TSaveDialog *sdSocketMessages;
        TClientSocket *csQRCode1;
        TTimer *tmActiveSocket;
        TPanel *pnlCCD;
        TImage *imCCD;
        TShape *shVertical;
        TShape *shHorizontal;
        TShape *shHorizontalUp;
        TShape *shHorizontalDown;
        TImage *imGrid;
        TPanel *pnlCurrentGray;
        TLMDPageControl *pcSystem;
        TLMDTabSheet *tsSystemInformation;
        TPanel *pnlMoveTop;
        TPanel *pnlAlarmMessage;
        TLMDButton *btnReset;
        TPanel *pnlRecipeSet;
        TPanel *pnlSystemMessage;
        TICImagingControl *ICImagingControl1;
        TTimer *tmISCCD;
        TPanel *pnlSystem;
        TLMDCheckBox *cbCCDFullImage;
        TStatusBar *sbSystem;
        TOpenPictureDialog *opCCDImage;
        TSavePictureDialog *spCCDImage;
        TYbCommDevice *ybLampControl2;
        TPanel *pnlAOILamp;
        TGroupBox *GroupBox4;
        TPanel *Panel65;
        TLMDButton *btnPassword;
        TPanel *Panel129;
        TLMDButton *btnUpdateMotionParameter;
        TTimer *tmMotionPolling;
        TTimer *tmSystem;
        TPanel *pnlPCI7856;
        TPanel *pnlX;
        TPanel *pnlZ;
        TPanel *pnlT;
        TPanel *pnlHome2;
        TLMDButton *btnHome2;
        TTimer *tmBeep;
        TTimer *tm400ms;
        TPanel *pnlQRCodeConnect;
        TPanel *pnlCCDStatus1;
        TLMDRadioGroup *rgStatusSocket;
        TPanel *pnlInDoor;
        TLMDPageControl *pcOperation;
        TLMDTabSheet *tsOperation;
        TLMDTabSheet *tsWaferInformation;
        TLMDTabSheet *tsStatus;
        TGroupBox *gbSubstrate;
        TLabel *Label414;
        TComboBox *cbThickness;
        TComboBox *cbSize;
        TComboBox *cbGrade;
        TEdit *edLotNo;
        TComboBox *cbVendor;
        TEdit *edGlassID;
        TPanel *Panel29;
        TPanel *Panel32;
        TPanel *pnlVendorTitle;
        TPanel *pnlGrade;
        TPanel *pnlLotNoTilte;
        TPanel *Panel36;
        TPanel *Panel39;
        TEdit *edBoxNo;
        TPanel *Panel71;
        TComboBox *cbMode;
        TGroupBox *gbSystem;
        TLabel *laCIM;
        TLabel *Label20;
        TLabel *Label21;
        TShape *shDeviceStatus;
        TLabel *Label27;
        TShape *shUpLamp;
        TLabel *laUpLamp;
        TShape *shDownLamp;
        TLabel *Label29;
        TShape *shCOMPort;
        TLabel *Label33;
        TShape *shSensor1;
        TLabel *Label37;
        TShape *shSensor2;
        TLabel *Label46;
        TShape *shNG;
        TLabel *Label6;
        TShape *shReady;
        TLabel *Label12;
        TShape *shIOStatus;
        TLabel *Label34;
        TLabel *Label384;
        TLabel *Label10;
        TShape *shMotionStatus;
        TLabel *Label26;
        TLabel *Label11;
        TPanel *pnlCIMStatus;
        TPanel *pnlSystemStatus;
        TComboBox *cbOperationMode;
        TPanel *pnlEMOAlarm;
        TPanel *pnlMotionReady;
        TPanel *pnlSystemPower;
        TPanel *Panel37;
        TComboBox *cbType;
        TPanel *pnlRun;
        TTimer *tmStatus;
        TPanel *pnlPassword;
        TLabel *Label111;
        TLabel *Label112;
        TPanel *Panel49;
        TLMDEdit *edLogName;
        TMaskEdit *medCurrentPassword;
        TLMDButton *btnLog;
        TLMDTabSheet *tsUserPosition1;
        TLabel *Label312;
        TPanel *pnlJX;
        TPanel *pnlJY;
        TPanel *pnlJZ;
        TPanel *pnlSpeed;
        TPanel *pnlY;
        TLMDButton *btnFindFocus;
        TPanel *pnlAutoFocus1;
        TTimer *tmJoystick;
        TLMDRadioGroup *rgBrukerZSpeed;
        TTimer *tmTrigger;
        TTimer *tmZ;
        TTimer *tmY;
        TTimer *tmX;
        TLMDTabSheet *tsRecipe;
        TLMDPageControl *pcRecipe;
        TLMDTabSheet *tsPointSet;
        TLMDGroupBox *LMDGroupBox38;
        TGroupBox *gbMatch;
        TLabel *Label88;
        TLabel *Label434;
        TPanel *pnlScore;
        TLMDEdit *edMinScore;
        TLMDButton *btnAutoMatch;
        TPanel *pnlNCC;
        TCheckBox *cbStdPattern;
        TLMDButton *btnEraseMatchBox;
        TLMDRadioGroup *rgMatchSource;
        TCheckBox *cbMoveToCenterMatched;
        TLMDButton *btnMatchToCenter;
        TLMDButton *btnSaveMatchParameter;
        TRadioGroup *rgMatchMode;
        TLMDEdit *edMinReduceArea;
        TLMDTabSheet *LMDTabSheet5;
        TLMDGroupBox *LMDGroupBox12;
        TLabel *Label89;
        TLabel *Label91;
        TLabel *Label92;
        TLabel *Label93;
        TLabel *Label97;
        TLabel *Label114;
        TLMDEdit *edType1Width;
        TLMDEdit *edType1Height;
        TLMDEdit *edType4Width;
        TLMDEdit *edType4Height;
        TLMDEdit *edType2Width;
        TLMDEdit *edType2Height;
        TLMDEdit *edType3Width;
        TLMDEdit *edType3Height;
        TPanel *Panel10;
        TComboBox *combSizeRecipe;
        TLMDTabSheet *tsArrayDieMove;
        TLMDGroupBox *gbArrayOperation;
        TLMDButton *btnType1Left;
        TLMDButton *btnType1Right;
        TLMDButton *btnType1Back;
        TLMDButton *btnType1Front;
        TLMDGroupBox *gbArray1Operation;
        TLMDButton *btnType2Left;
        TLMDButton *btnType2Right;
        TLMDButton *btnType2Back;
        TLMDButton *btnType2Front;
        TLMDGroupBox *gbArray2Operation;
        TLMDButton *btnType3Left;
        TLMDButton *btnType3Right;
        TLMDButton *btnType3Back;
        TLMDButton *btnType3Front;
        TLMDGroupBox *gbDieOperation;
        TLMDButton *btnType4Left;
        TLMDButton *btnType4Right;
        TLMDButton *btnType4Back;
        TLMDButton *btnType4Front;
        TPanel *pnlType1Width;
        TPanel *pnlType1Height;
        TPanel *pnlType2Width;
        TPanel *pnlType2Height;
        TPanel *pnlType3Width;
        TPanel *pnlType3Height;
        TPanel *pnlType4Width;
        TPanel *pnlType4Height;
        TLabel *Label115;
        TLabel *Label116;
        TLabel *Label122;
        TLabel *Label123;
        TLabel *Label124;
        TLabel *Label125;
        TLabel *Label126;
        TLabel *Label127;
        TShape *shMarkMatch;
        TShape *shDown;
        TShape *shLeftDown;
        TShape *shLeft;
        TShape *shLeftUp;
        TShape *shUp;
        TShape *shRightUp;
        TShape *shRight;
        TShape *shRightDown;
        TShape *shCenter;
        TShape *shDSX;
        TPanel *pnlDotX;
        TPanel *pnlDotY;
        TPanel *pnlDotZ;
        TPopupMenu *pmLensSwitch;
        TMenuItem *mnToLens;
        TMenuItem *mnToCCD;
        TLMDButton *btnReCalibration;
        TPanel *pnlReCalibration;
        TLMDButton *btnUpdateArrayDie;
        TPanel *pnlR;
        TPanel *pnlEFEM;
        TYbCommDevice *ybRobot;
        TYbCommDevice *ybLoadPort;
        TYbCommDevice *ybAligner;
        TTimer *tmSetupEFEM;
        TTimer *tmLoadUnload;
        TYbCommDevice *ybLoadPort3;
        TTimer *tmAlignerORG;
        TTimer *tmLoadUnload2;
        TLMDTabSheet *tsLog;
        TMemo *mmPerformance;
        TYbCommDevice *ybLampControl1;
        TPanel *pnlLeftMacroLamp;
        TPanel *pnlRightMacroLamp;
        TPanel *pnlEFEMReady;
        TPanel *pnlSystemCDAVacuum;
        TPanel *pnlEFEMCassette;
        TPanel *pnlEFEMCassette2;
        TPanel *pnlMeasureResult;
        TShape *shMeasure;
        TShape *shEllipse;
        TPopupMenu *pmMeasure;
        TMenuItem *mnClear;
        TMenuItem *mnSaveFile;
        TLMDTabSheet *tsMeasure;
        TPanel *pnlMeasureTool;
        TBitBtn *BitBtn32;
        TBitBtn *BitBtn33;
        TBitBtn *BitBtn34;
        TBitBtn *btnAngle;
        TBitBtn *BitBtn38;
        TBitBtn *BitBtn44;
        TBitBtn *btnMoveLeft;
        TBitBtn *btnMoveRear;
        TBitBtn *btnMoveFront;
        TBitBtn *btnMoveRight;
        TBitBtn *btnMoveUp;
        TBitBtn *btnMoveDown;
        TBitBtn *btnCircle;
        TMemo *mmMeasure;
        TPopupMenu *pmPointSet;
        TMenuItem *mnPoint1;
        TMenuItem *mnPoint2;
        TMenuItem *mnPoint3;
        TMenuItem *mnRecord;
        TMenuItem *mnLineLength1;
        TMenuItem *mnLineLength2;
        TMenuItem *mnCirclePoint1;
        TMenuItem *mnCirclePoint2;
        TMenuItem *mnSetPoint;
        TPanel *pnlPoint1;
        TShape *shPoint1;
        TPanel *pnlPoint2;
        TShape *shPoint2;
        TPanel *pnlPoint3;
        TShape *shPoint3;
        TPanel *pnlLineSet1;
        TShape *shLineSet1;
        TPanel *pnlLineSet2;
        TShape *shLineSet2;
        TPanel *pnlPointSet1;
        TShape *shPointSet1;
        TPanel *pnlPointSet2;
        TShape *shPointSet2;
        TPanel *pnlPointSet3;
        TShape *shPointSet3;
        TPanel *pnlPointSet4;
        TShape *shPointSet4;
        TPanel *pnlPointSet5;
        TShape *shPointSet5;
        TPanel *pnlCirclePoint1;
        TShape *shCirclePoint1;
        TPanel *pnlCircleCenter1;
        TShape *shCircleCenter1;
        TPanel *pnlCircleCenter2;
        TPanel *pnlCirclePoint2;
        TShape *shCirclePoint2;
        TShape *shCircleCenter2;
        TShape *shCircle;
        TLMDCheckBox *cbPointSetDisplay;
        TPanel *pnlCircle;
        TLabel *laMessage;
        TLabel *Label256;
        TLabel *Label305;
        TLabel *Label318;
        TLabel *Label320;
        TLabel *Label321;
        TLabel *laCircleCenter;
        TLabel *laCircleRadius;
        TLabel *laCircleArea;
        TLabel *laCircleRadiusUnit;
        TLabel *laCircleAreaUnit;
        TLabel *laAngle;
        TLabel *laAngleUnit;
        TPanel *Panel23;
        TLMDButton *btnCircleCompute;
        TPanel *pnlCircleP1X;
        TPanel *pnlCircleP1Y;
        TPanel *pnlCircleP2X;
        TPanel *pnlCircleP2Y;
        TPanel *pnlCircleP3X;
        TPanel *pnlCircleP3Y;
        TPanel *pnlCircleCenterX;
        TPanel *pnlCircleRadius;
        TPanel *pnlCircleArea;
        TLMDButton *btnCircleP1Set;
        TLMDButton *btnCircleP2Set;
        TLMDButton *btnCircleP3Set;
        TPanel *pnlCircleP1Status;
        TPanel *pnlCircleP2Status;
        TPanel *pnlCircleP3Status;
        TLMDButton *btnCircleExit;
        TPanel *pnlCircleCenterY;
        TPanel *pnlAngle;
        TPanel *pnlAngle12;
        TLMDButton *btnClear;
        TLMDRadioGroup *rgCircleNo;
        TPanel *pnlLineLength;
        TCheckBox *cbWaferMap;
        TLMDTrackBar *tbZoom;
        TRadioGroup *rgWheelDirection;
        TPopupMenu *pmWaferMapping;
        TMenuItem *mnSetMeasurePoint;
        TMenuItem *mnToMeasurePoint;
        TLabel *Label517;
        TLabel *Label518;
        TPanel *pnlColum;
        TPanel *pnlRow;
        TPanel *pnlColumSelect;
        TPanel *pnlRowSelect;
        TRadioGroup *rgWaferDegree;
        TLabel *Label322;
        TLabel *Label324;
        TLMDRadioGroup *rgTableCCDMode;
        TPanel *pnlTable;
        TImage *imTable;
        TShape *shTable;
        TLabel *Label134;
        TPanel *pnlTableX;
        TLabel *Label319;
        TPanel *pnlTableY;
        TScrollBox *sbWaferMap;
        TImage *imWaferMap;
        TShape *shMapHorizontal;
        TShape *shMapVertical;
        TPanel *pnlHorizontal;
        TPanel *pnlVertical;
        TPanel *pnlFactor;
        TBitBtn *btnLoadMeasureImage;
        TImage *imMeasure;
        TLMDGroupBox *gbLens;
        TLabel *Label325;
        TLabel *Label326;
        TLabel *Label332;
        TPanel *pnlSelectedLens;
        TLMDButton *btnLens1;
        TLMDButton *btnLens2;
        TLMDButton *btnLens3;
        TLMDButton *btnLens4;
        TLMDButton *btnLens5;
        TLMDButton *btnLens6;
        TUpDown *udLEDPWMOP;
        TPanel *pnlLEDPWMOP;
        TLMDTrackBar *tbLEDPWMOP;
        TButton *btnLensMake0OP;
        TPanel *pnlLensLeft;
        TPanel *pnlLensRight;
        TLMDButton *btnSaveIllumination;
        TPanel *pnlLensSwitchResult;
        TShape *shDot1;
        TPanel *pnlPositionLength;
        TPanel *pnlPositionX;
        TPanel *pnlPositionY;
        TPanel *pnlPowerAlarm;
        TPanel *pnlEMOAlarm1;
        TPanel *pnlCDAAlarm;
        TPanel *pnlVacuumAlarm;
        TPanel *pnlXStatus;
        TPanel *pnlYStatus;
        TPanel *pnlZStatus;
        TPanel *pnlTStatus;
        TPanel *pnlRStatus;
        TYbCommDevice *ybBXFM;
        TPanel *pnlBXFMReady;
        TShape *shDot2;
        TShape *shDot3;
        TLMDButton *btnA2MAutoFocus;
        TPanel *pnlA2MAutoFocus;
        TTimer *tmAlignerORGN;
        TTimer *tmToLoadPosition;
        TCheckBox *cbRHighSpeed;
        TTimer *tmAlignerORGNDone;
        TPanel *pnlMain;
        TImage *Image3;
        TPanel *pnlMainUser;
        TPanel *pnlMainHelp;
        TImage *Image4;
        TImage *Image7;
        TImage *imMapClose;
        TPanel *pnlMainOperation;
        TPanel *pnlMainRecipe;
        TLMDTabSheet *tsYuanli;
        TPanel *pnlYuanli;
        TLabel *Label419;
        TEdit *edYuanliPort;
        TLMDButton *btnYunliReConnect;
        TPanel *pnlYuanliSocketConnect;
        TPanel *pnlSocketReady;
        TPanel *Panel52;
        TEdit *edYuanliCmd;
        TButton *btnYuanliSend;
        TMemo *mmYuanli;
        TPanel *pnlAIMessage;
        TRadioGroup *rgYuanliCommandSet;
        TRadioGroup *rgCommand;
        TPanel *pnlYuanliMessage;
        TLabel *Label415;
        TEdit *edYuanliIP;
        TLabel *Label420;
        TPanel *pnlErrorCode;
        TLabel *Label421;
        TClientSocket *csYuanliSocket;
        TPanel *pnlOlympusStatus;
        TEdit *eduanliDelayTime;
        TLabel *Label521;
        TPanel *pnlYuanliSend;
        TPanel *pnlMainInspection;
        TImage *Image8;
        TImage *Image9;
        TImage *Image10;
        TCheckBox *cb2Code;
        TTimer *tmChuckMeasure;
        TTimer *tmMacroDemo;
        TTimer *tmRobotLS;
        TPanel *pnlMainVersion;
        TPanel *pnlMainMode;
        TPanel *pnlMainSignal;
        TPanel *pnlMainDevice;
        TImage *Image18;
        TPanel *pnlMainTroubles;
        TImage *Image17;
        TPanel *pnlMainMaintenance;
        TImage *Image19;
        TPanel *pnlMainMeasurement;
        TImage *Image20;
        TPanel *pnlEFEM2;
        TPanel *pnlEMO;
        TPanel *pnlMoving;
        TPanel *pnlAxisError;
        TPanel *pnlStartMessage;
        TMemo *mmError;
        TPanel *pnlWidthValue;
        TImage *Image1;
        TImage *Image2;
        TLMDCheckBox *cbMeasure;
        TPanel *pnlUnit;
        TPanel *pnlCurrentOperation;
        TPanel *pnlMainMessage;
        TPanel *pnlError;
        TYbCommDevice *ybAligner2;
        TTimer *tmAligner2ORGNDone;
        TTimer *tmAligner2Reset;
        TTimer *tmAlignerAck;
        TTimer *tmAligner2Ack;
        TTimer *tmRobotAck;
        TPanel *pnlMaintenance;
        TPanel *Panel21;
        TImage *Image21;
        TPanel *pnlMaintenanceMessage;
        TPanel *Panel48;
        TPanel *Panel53;
        TImage *Image22;
        TImage *imHelp;
        TImage *imClose;
        TPageControl *pcMaintenance;
        TTabSheet *tsMotion;
        TTabSheet *tsHirata;
        TTabSheet *tsCCD;
        TTabSheet *tsLED;
        TTabSheet *tsQRCode;
        TTabSheet *tsBXFM;
        TTabSheet *tsFocus;
        TLabel *Label28;
        TLabel *Label40;
        TLMDGroupBox *gbAxisInformation;
        TLabel *Label60;
        TLabel *Label61;
        TLabel *Label72;
        TLabel *laXAxis;
        TLabel *laTAxis;
        TLabel *laYAxis;
        TLabel *Label23;
        TLabel *Label24;
        TLabel *Label25;
        TLabel *laZAxis;
        TLabel *laW1;
        TLMDEdit *edCommandX;
        TLMDEdit *edCounterX;
        TLMDEdit *edComparatorX;
        TLMDEdit *edCommandY;
        TLMDEdit *edCounterY;
        TLMDEdit *edComparatorY;
        TLMDEdit *edCommandT;
        TLMDEdit *edCounterT;
        TLMDEdit *edComparatorT;
        TPanel *pnlStatusX;
        TPanel *pnlStatusY;
        TPanel *pnlStatusT;
        TPanel *pnlMotionStatusX;
        TPanel *pnlMotionStatusY;
        TPanel *pnlMotionStatusT;
        TPanel *pnlIOStatusX;
        TPanel *pnlIOStatusY;
        TPanel *pnlIOStatusT;
        TLMDEdit *edCommandZ;
        TLMDEdit *edCounterZ;
        TLMDEdit *edComparatorZ;
        TPanel *pnlMotionStatusZ;
        TPanel *pnlIOStatusZ;
        TPanel *pnlStatusZ;
        TLMDEdit *edCommandR;
        TLMDEdit *edCounterR;
        TLMDEdit *edComparatorR;
        TPanel *pnlStatusR;
        TPanel *pnlMotionStatusR;
        TPanel *pnlIOStatusR;
        TLMDGroupBox *LMDGroupBox19;
        TLabel *Label69;
        TLabel *Label70;
        TLabel *Label71;
        TPanel *pnl7856Status;
        TPanel *pnlInStatus;
        TPanel *pnlOutStatus;
        TLMDGroupBox *LMDGroupBox20;
        TComboBox *combAxis;
        TLMDButton *btnMove;
        TLMDButton *btnBackward;
        TLMDButton *btnHomeOneAxis;
        TPanel *pnlHomeStatus;
        TLMDGroupBox *rgRelativeMove;
        TLabel *Label18;
        TLabel *Label31;
        TLabel *Label32;
        TLabel *Label35;
        TLabel *laW;
        TLMDButton *btnMoveX;
        TLMDButton *btnBackwardY;
        TLMDButton *btnMoveY;
        TLMDButton *btnBackwardX;
        TLMDButton *btnStop;
        TLMDButton *btnMoveT;
        TLMDButton *btnBackwardT;
        TLMDButton *btnMoveZ;
        TLMDButton *btnBackwardZ;
        TLMDButton *btnMoveR;
        TLMDButton *btnBackwardR;
        TLMDGroupBox *LMDGroupBox15;
        TLMDButton *btnMoveAbsolute;
        TLMDButton *btnStopAbsolute;
        TLMDGroupBox *gbSetting;
        TLMDButton *btnAlarmOn;
        TLMDButton *btnAlarmOff;
        TLMDButton *btnServoOn;
        TLMDButton *btnServoOff;
        TLMDButton *btnDOOn;
        TLMDButton *btnDOOff;
        TPanel *Panel132;
        TButton *btnHome;
        TPanel *pnlAllHomeStatus;
        TLMDGroupBox *rgHomeMove;
        TLMDCheckBox *cbContinueMove;
        TPanel *pnlCounter;
        TLMDGroupBox *LMDGroupBox29;
        TLabel *Label270;
        TLabel *Label271;
        TLabel *Label273;
        TLabel *Label274;
        TLabel *Label275;
        TLabel *Label276;
        TLabel *Label432;
        TLMDEdit *edSpeedGeneralLow;
        TLMDEdit *edSpeedGeneralMiddle;
        TLMDEdit *edSpeedGeneralHigh;
        TLMDButton *btnSpeedUpdate;
        TLMDEdit *edSpeedTurboLow;
        TLMDEdit *edSpeedTurboMiddle;
        TLMDEdit *edSpeedTurboHigh;
        TLMDEdit *edSpeedZLow;
        TLMDEdit *edSpeedZMiddle;
        TLMDEdit *edSpeedZHigh;
        TLMDCheckBox *cbZSame;
        TLMDRadioGroup *rgZSpeed;
        TPanel *pnlCurrentSpeed;
        TLMDButton *btnSVONAll;
        TLMDPageControl *pcMotion;
        TLMDTabSheet *tsMotionParameter;
        TLMDTabSheet *tsMotionOperate;
        TLMDTabSheet *tsMotionSetting;
        TLMDTabSheet *tsDI;
        TLMDTabSheet *tsDO;
        TLMDTabSheet *LMDTabSheet1;
        TLMDTabSheet *tsRX;
        TLMDTabSheet *LMDTabSheet2;
        TLMDTabSheet *LMDTabSheet3;
        TLMDPageControl *pcRobot;
        TLMDTabSheet *tsRobotOperation;
        TLMDTabSheet *tsCommand;
        TLMDTabSheet *LMDTabSheet8;
        TLMDTabSheet *tsRobotTest;
        TLMDTabSheet *tsRobotStatus;
        TLMDTabSheet *tsLoadPortStatus;
        TLMDTabSheet *tsLP2Status;
        TLMDTabSheet *tsAlignerStatus;
        TPanel *Panel60;
        TLMDGroupBox *LMDGroupBox1;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label5;
        TLabel *Label336;
        TLabel *Label333;
        TLabel *Label129;
        TLabel *Label132;
        TLabel *Label209;
        TLabel *Label229;
        TLabel *Label187;
        TLabel *Label36;
        TLabel *Label41;
        TLMDRadioGroup *rgRulerColor;
        TLMDCheckBox *cbRuler;
        TBitBtn *btnReviewLive;
        TBitBtn *btnReviewGrab;
        TBitBtn *btnReviewLoad;
        TBitBtn *btnReviewSave;
        TPanel *pnlCCDX;
        TPanel *pnlCCDY;
        TPanel *pnlCCDR;
        TPanel *pnlCCDG;
        TPanel *pnlCCDB;
        TPanel *pnlCapturedNo;
        TPanel *pnlCaptureTime;
        TPanel *pnlCCDSize;
        TPanel *pnlOperation;
        TLMDEdit *edGridPixel;
        TRadioGroup *rgCCDShutter;
        TGroupBox *GroupBox35;
        TLabel *laGainMin;
        TLabel *laGainMax;
        TLabel *Label331;
        TTrackBar *tbCCDGain;
        TPanel *pnlCCDGain;
        TGroupBox *GroupBox1;
        TLabel *Label241;
        TLabel *Label249;
        TLabel *Label257;
        TTrackBar *tbContrast;
        TPanel *pnlContrast;
        TPanel *pnlLensPrecision;
        TLMDRadioGroup *rgRulerAux;
        TPanel *pnlCCDStatus;
        TLMDRadioGroup *rgRulerType;
        TEdit *edCCDPrecision;
        TPanel *pnlYPitchOP;
        TPanel *pnlISCCDInfo;
        TLMDEdit *edStdGray;
        TLMDEdit *edStdGrayTolerance;
        TPanel *pnlISCCDStaus;
        TPanel *pnlISCCDCapture;
        TPanel *pnlAverageGray;
        TLMDRadioGroup *rgCCDSpeed;
        TListBox *lbGiga;
        TPanel *Panel74;
        TPanel *Panel75;
        TPanel *Panel96;
        TMemo *mmQRCodeMessage;
        TPanel *pnlSocket;
        TEdit *edSocket;
        TButton *btnSend;
        TPanel *pnlVersion;
        TPanel *pnlMessage;
        TButton *btnReady;
        TPanel *pnlSocketConnect;
        TPanel *pnlSend;
        TLMDButton *btnQRCodeRead;
        TPanel *pnlWaferID;
        TLMDButton *btnQRCodeLog;
        TPanel *pnlQRCodeLog;
        TPanel *pnlFocusWin;
        TLabel *Label267;
        TLabel *Label279;
        TLabel *Label280;
        TLabel *Label281;
        TLabel *Label342;
        TLabel *Label343;
        TLabel *Label344;
        TLabel *Label345;
        TLabel *Label346;
        TLabel *Label311;
        TLabel *Label391;
        TLabel *Label395;
        TLabel *Label396;
        TPanel *Panel35;
        TLMDEdit *edAutoFocus;
        TLMDButton *btnAutoFocus;
        TPanel *pnlAutoFocus;
        TLMDButton *btnUpdateFocus;
        TLMDEdit *edAutoFocusOffset;
        TLMDEdit *edAutoFocusSpeed;
        TPanel *pnlUpZPos;
        TPanel *pnlDownZPos;
        TPanel *pnlUpValue;
        TPanel *pnlDownValue;
        TLMDRadioGroup *rgFocusAlgorithm;
        TPanel *pnlUpZCount;
        TPanel *pnlDownZCount;
        TLMDCheckBox *cbAIFocus;
        TLMDEdit *edDifferenceValue;
        TLMDEdit *edDifferenceRatio;
        TLMDEdit *edAutoFocusThickness;
        TPanel *Panel97;
        TLMDGroupBox *LMDGroupBox31;
        TLabel *Label282;
        TLabel *Label283;
        TLabel *Label284;
        TLabel *Label285;
        TLabel *Label128;
        TLabel *Label317;
        TPanel *Panel3;
        TLMDButton *btnSaveLED2;
        TPanel *pnlLED2CH1;
        TPanel *pnlLampControl1;
        TCheckBox *cbLampControl2;
        TButton *btnLampControl1;
        TLMDTrackBar *tbLED2CH1;
        TLMDTrackBar *tbLED2CH2;
        TPanel *pnlLED2CH2;
        TLMDTrackBar *tbLED2CH3;
        TPanel *pnlLED2CH3;
        TLMDTrackBar *tbLED2CH4;
        TPanel *pnlLED2CH4;
        TPanel *Panel20;
        TPanel *Panel4;
        TPanel *Panel1;
        TComboBox *combLED1Comport;
        TPanel *pnlLampControl2;
        TComboBox *combLED2Comport;
        TButton *btnLampControl2;
        TLabel *Label334;
        TGroupBox *GroupBox2;
        TLabel *Label45;
        TLabel *Label148;
        TLabel *Label409;
        TLabel *Label131;
        TLabel *Label335;
        TLMDButton *btnSaveQRCode;
        TCheckBox *cbTrigger;
        TEdit *edInterval;
        TEdit *edDelayTime;
        TEdit *edQRCodeIP1;
        TEdit *edQRCodePort1;
        TEdit *edQRCodeDegree;
        TLabel *Label130;
        TLMDButton *btnSaveCCD;
        TLabel *Label520;
        TTabSheet *tsErrorLog;
        TTabSheet *tsHelp1;
        TMemo *mmLog;
        TMemo *mmSystemHelp;
        TPanel *Panel99;
        TPanel *Panel100;
        TPanel *Panel103;
        TPanel *Panel107;
        TPanel *Panel108;
        TPanel *Panel111;
        TPanel *Panel112;
        TLabel *Label490;
        TLabel *Label492;
        TLabel *Label493;
        TLabel *Label494;
        TLabel *Label495;
        TLabel *Label499;
        TLabel *Label515;
        TLMDGroupBox *LMDGroupBox45;
        TLabel *Label461;
        TLabel *Label462;
        TLabel *Label464;
        TLabel *Label465;
        TLabel *Label466;
        TLabel *Label467;
        TLabel *Label469;
        TLabel *Label470;
        TLabel *Label473;
        TLabel *Label482;
        TPanel *pnlAlignerS1;
        TPanel *pnlAlignerS2;
        TPanel *pnlAlignerS3;
        TPanel *pnlAlignerS4;
        TPanel *pnlAlignerS5;
        TPanel *pnlAlignerS6;
        TPanel *pnlAlignerS7;
        TPanel *pnlAlignerS8;
        TPanel *pnlAlignerS9;
        TPanel *pnlAlignerS10;
        TPanel *pnlAligner2S1;
        TPanel *pnlAligner2S2;
        TPanel *pnlAligner2S3;
        TPanel *pnlAligner2S4;
        TPanel *pnlAligner2S5;
        TPanel *pnlAligner2S6;
        TPanel *pnlAligner2S7;
        TPanel *pnlAligner2S8;
        TPanel *pnlAligner2S9;
        TPanel *pnlAligner2S10;
        TPanel *Panel79;
        TPanel *Panel80;
        TLMDPageControl *LMDPageControl2;
        TLMDTabSheet *tsLoadPort2Status;
        TLMDTabSheet *LMDTabSheet9;
        TLabel *Label302;
        TPanel *Panel115;
        TLMDGroupBox *LMDGroupBox11;
        TLabel *Label259;
        TLabel *Label260;
        TLabel *Label261;
        TLabel *Label262;
        TLabel *Label263;
        TLabel *Label264;
        TLabel *Label265;
        TLabel *Label266;
        TLabel *Label268;
        TLabel *Label269;
        TLabel *Label277;
        TLabel *Label278;
        TLabel *Label286;
        TLabel *Label287;
        TLabel *Label288;
        TLabel *Label289;
        TLabel *Label292;
        TLabel *Label293;
        TLabel *Label295;
        TLabel *Label296;
        TLabel *Label300;
        TPanel *pnlLoadPort2S1;
        TPanel *pnlLoadPort2S2;
        TPanel *pnlLoadPort2S3;
        TPanel *pnlLoadPort2S4;
        TPanel *pnlLoadPort2S5;
        TPanel *pnlLoadPort2S6;
        TPanel *pnlLoadPort2S7;
        TPanel *pnlLoadPort2S8;
        TPanel *pnlLoadPort2S9;
        TPanel *pnlLoadPort2S10;
        TPanel *pnlLoadPort2S11;
        TPanel *pnlLoadPort2S12;
        TPanel *pnlLoadPort2S13;
        TPanel *pnlLoadPort2S14;
        TPanel *pnlLoadPort2S15;
        TPanel *pnlLoadPort2S16;
        TPanel *pnlLoadPort2S17;
        TPanel *pnlLoadPort2S18;
        TPanel *pnlLoadPort2S19;
        TPanel *pnlLoadPort2S20;
        TPanel *Panel116;
        TLabel *Label301;
        TLabel *Label491;
        TLMDPageControl *LMDPageControl1;
        TLMDTabSheet *LMDTabSheet4;
        TLMDTabSheet *LMDTabSheet6;
        TPanel *Panel119;
        TLMDGroupBox *LMDGroupBox9;
        TLabel *Label412;
        TLabel *Label413;
        TLabel *Label246;
        TLabel *Label444;
        TLabel *Label445;
        TLabel *Label247;
        TLabel *Label248;
        TLabel *Label250;
        TLabel *Label251;
        TLabel *Label252;
        TLabel *Label253;
        TLabel *Label254;
        TLabel *Label255;
        TLabel *Label454;
        TLabel *Label455;
        TLabel *Label456;
        TLabel *Label457;
        TLabel *Label458;
        TLabel *Label459;
        TLabel *Label460;
        TLabel *Label488;
        TPanel *pnlLoadPortS1;
        TPanel *pnlLoadPortS2;
        TPanel *pnlLoadPortS3;
        TPanel *pnlLoadPortS4;
        TPanel *pnlLoadPortS5;
        TPanel *pnlLoadPortS6;
        TPanel *pnlLoadPortS7;
        TPanel *pnlLoadPortS8;
        TPanel *pnlLoadPortS9;
        TPanel *pnlLoadPortS10;
        TPanel *pnlLoadPortS11;
        TPanel *pnlLoadPortS12;
        TPanel *pnlLoadPortS13;
        TPanel *pnlLoadPortS14;
        TPanel *pnlLoadPortS15;
        TPanel *pnlLoadPortS16;
        TPanel *pnlLoadPortS17;
        TPanel *pnlLoadPortS18;
        TPanel *pnlLoadPortS19;
        TPanel *pnlLoadPortS20;
        TPanel *Panel121;
        TLabel *Label496;
        TLMDGroupBox *LMDGroupBox44;
        TLabel *Label439;
        TLabel *Label440;
        TLabel *Label245;
        TLabel *Label476;
        TLabel *Label477;
        TLabel *Label478;
        TLabel *Label479;
        TLabel *Label480;
        TLabel *Label481;
        TPanel *pnlRobotE1;
        TPanel *pnlRobotE2;
        TPanel *pnlRobotAX;
        TPanel *pnlRobotAY;
        TPanel *pnlRobotAZ;
        TPanel *pnlRobotAW;
        TPanel *pnlRobotAR;
        TPanel *pnlRobotAC;
        TLMDGroupBox *LMDGroupBox21;
        TLabel *Label392;
        TLabel *Label230;
        TLabel *Label397;
        TLabel *Label398;
        TLabel *Label399;
        TLabel *Label400;
        TLabel *Label401;
        TLabel *Label231;
        TLabel *Label403;
        TLabel *Label404;
        TLabel *Label405;
        TLabel *Label406;
        TLabel *Label407;
        TLabel *Label408;
        TLabel *Label232;
        TLabel *Label410;
        TLabel *Label233;
        TLabel *Label234;
        TLabel *Label235;
        TLabel *Label425;
        TLabel *Label426;
        TLabel *Label427;
        TLabel *Label428;
        TLabel *Label236;
        TLabel *Label237;
        TLabel *Label431;
        TLabel *Label240;
        TLabel *Label433;
        TLabel *Label242;
        TLabel *Label436;
        TLabel *Label437;
        TLabel *Label438;
        TLabel *Label244;
        TPanel *pnlRobotS10;
        TPanel *pnlRobotS11;
        TPanel *pnlRobotS12;
        TPanel *pnlRobotS13;
        TPanel *pnlRobotS14;
        TPanel *pnlRobotS15;
        TPanel *pnlRobotS16;
        TPanel *pnlRobotS17;
        TPanel *pnlRobotS20;
        TPanel *pnlRobotS21;
        TPanel *pnlRobotS22;
        TPanel *pnlRobotS23;
        TPanel *pnlRobotS24;
        TPanel *pnlRobotS25;
        TPanel *pnlRobotS26;
        TPanel *pnlRobotS27;
        TPanel *pnlRobotS30;
        TPanel *pnlRobotS31;
        TPanel *pnlRobotS32;
        TPanel *pnlRobotS33;
        TPanel *pnlRobotS34;
        TPanel *pnlRobotS35;
        TPanel *pnlRobotS36;
        TPanel *pnlRobotS37;
        TPanel *pnlRobotS40;
        TPanel *pnlRobotS41;
        TPanel *pnlRobotS42;
        TPanel *pnlRobotS43;
        TPanel *pnlRobotS44;
        TPanel *pnlRobotS45;
        TPanel *pnlRobotS46;
        TPanel *pnlRobotS47;
        TLMDGroupBox *gbRobotCommand;
        TShape *Shape6;
        TButton *btnChuckVacuumOn;
        TButton *btnChuckVacuumOff;
        TLabel *Label511;
        TLabel *Label513;
        TLabel *Label516;
        TPanel *pnlAligner;
        TLabel *Label463;
        TLabel *Label474;
        TLabel *Label475;
        TPanel *pnlAlignerStatus;
        TComboBox *combAlignerComport;
        TComboBox *combAlignerBaudrate;
        TPanel *Panel101;
        TPanel *Panel102;
        TPanel *pnlAlignerSN;
        TPanel *pnlAlignerMessage;
        TPanel *Panel105;
        TLMDButton *btnAlignerConnect;
        TLMDRadioGroup *rgAlignerSet;
        TEdit *edAlignerDegree;
        TPanel *Panel86;
        TPanel *Panel88;
        TEdit *edAlignerSpeed;
        TPanel *Panel91;
        TEdit *edAlignerType;
        TLMDButton *btnAlignerSet;
        TLMDButton *btnAlignerGet;
        TMemo *mmAlignerMsg;
        TLMDButton *btnAlignerCMD;
        TEdit *edAlignerCmd;
        TLMDButton *btnAlignerORGN;
        TLMDButton *btnAlignerAlignment;
        TLMDButton *btnAlignerVacuumOff;
        TPanel *pnlAlignerOperation;
        TPanel *pnlAlignerErrorCode;
        TLMDButton *btnAlignerStas;
        TPanel *pnlAlignerWithWafer;
        TLMDButton *btnAlignerWithWafer;
        TPanel *pnlAlignerORGN;
        TPanel *pnlAlignerAlignment;
        TPanel *pnlAlignerVacuumOff;
        TLMDButton *btnAlignerReset;
        TPanel *pnlAlignerReset;
        TLMDButton *btnAlignerSave;
        TEdit *edAlignerDegreeOut;
        TPanel *pnlAlingerStatus;
        TLMDButton *btnAlignerPRP;
        TPanel *pnlAlignerPRP;
        TPanel *pnlAligner2;
        TLabel *Label505;
        TLabel *Label508;
        TLabel *Label509;
        TPanel *pnlAligner2Status;
        TComboBox *combAligner2Comport;
        TComboBox *combAligner2Baudrate;
        TPanel *Panel50;
        TPanel *Panel51;
        TPanel *pnlAligner2SN;
        TPanel *pnlAligner2Message;
        TPanel *Panel63;
        TLMDButton *btnAligner2Connect;
        TLMDRadioGroup *rgAligner2Set;
        TEdit *edAligner2Degree;
        TPanel *Panel64;
        TPanel *Panel68;
        TEdit *edAligner2Speed;
        TPanel *Panel70;
        TEdit *edAligner2Type;
        TLMDButton *btnAligner2Set;
        TLMDButton *btnAligner2Get;
        TMemo *mmAligner2Msg;
        TLMDButton *btnAligner2CMD;
        TEdit *edAligner2Cmd;
        TLMDButton *btnAligner2ORGN;
        TLMDButton *btnAligner2Alignment;
        TLMDButton *btnAligner2VacuumOff;
        TPanel *pnlAligner2Operation;
        TPanel *pnlAligner2ErrorCode;
        TLMDButton *btnAligner2Stas;
        TPanel *pnlAligner2WithWafer;
        TLMDButton *btnAligner2WithWafer;
        TPanel *pnlAligner2ORGN;
        TPanel *pnlAligner2Alignment;
        TPanel *pnlAligner2VacuumOff;
        TLMDButton *btnAligner2Reset;
        TPanel *pnlAligner2Reset;
        TLMDButton *btnAligner2Save;
        TEdit *edAligner2DegreeOut;
        TPanel *pnlAlinger2Status;
        TLMDButton *btnAligner2PRP;
        TPanel *pnlAligner2PRP;
        TLMDButton *btnAlignerEnable;
        TLMDButton *btnAligner2Enable;
        TLabel *Label510;
        TLabel *Label512;
        TLabel *Label519;
        TPanel *pnlRobot;
        TPanel *pnlRobotStatus;
        TComboBox *combRobotComport;
        TComboBox *combRobotBaudrate;
        TPanel *Panel94;
        TPanel *Panel95;
        TPanel *pnlRobotSN;
        TPanel *pnlRobotMessage;
        TPanel *Panel98;
        TLMDButton *btnRobotConnect;
        TEdit *edRobotCmd;
        TLMDButton *btnRobotCMD;
        TMemo *mmRobotMsg;
        TLMDButton *btnRobotOriginal;
        TPanel *pnlRobotOriginal;
        TLMDButton *btnRobotPos;
        TEdit *edRobotPos;
        TPanel *pnlRobotPos;
        TCheckBox *cbRobot;
        TLMDButton *btnRobotStas;
        TPanel *pnlRobotOperation;
        TPanel *pnlRobotErrorCode;
        TLMDButton *btnRobotVacuumOn;
        TLMDButton *btnRobotVacuumOff;
        TLMDButton *btnRobotWithWafer;
        TPanel *pnlRobotWithWafer;
        TLMDButton *btnRobotGetPosition;
        TPanel *pnlRobotXPosition;
        TLMDButton *btnRobotClearAlarm;
        TLMDButton *btnRobotEanble;
        TLMDButton *btnRobotDisable;
        TPanel *Panel81;
        TLMDButton *btnRobotGet;
        TPanel *pnlRobotGet;
        TLMDButton *btnRobotPut;
        TPanel *pnlRobotPut;
        TComboBox *combRobotTrayNo;
        TLMDRadioGroup *rgWaferLocation;
        TEdit *edRobotABSpeed;
        TEdit *edRobotZSpeed;
        TLMDButton *btnRobotSpeed;
        TLMDButton *btnRobotSave;
        TPanel *pnlRobotWaferStatus;
        TLMDRadioGroup *rgSide;
        TLMDRadioGroup *rgCST;
        TPanel *pnlLoadPort;
        TPanel *pnlLoadPortStatus;
        TComboBox *combLoadPortComport;
        TComboBox *combLoadPortBaudrate;
        TPanel *Panel89;
        TPanel *Panel90;
        TPanel *pnlLoadPortSN;
        TPanel *pnlLoadPortMessage;
        TPanel *Panel93;
        TLMDButton *btnLoadPortConnect;
        TMemo *mmLoadPortMsg;
        TLMDButton *btnLoadPortMap;
        TLMDButton *btnLoadPortLoad;
        TLMDButton *btnLoadPortUnLoad;
        TPanel *pnlLoadPortPresence;
        TPanel *Panel104;
        TPanel *Panel106;
        TPanel *pnlLoadPortPlacement;
        TPanel *pnlLoadPortStatus1;
        TPanel *Panel109;
        TPanel *Panel110;
        TPanel *pnlLoadPortStatus2;
        TPanel *pnlLoadPortKWF;
        TPanel *Panel113;
        TPanel *Panel114;
        TPanel *pnlLoadPortLoad;
        TPanel *pnlLoadPortUnLoad;
        TPanel *Panel117;
        TPanel *Panel118;
        TPanel *pnlLoadPortAlarm;
        TPanel *Panel120;
        TPanel *pnlLoadPortPower;
        TLMDButton *btnLoadPortCMD;
        TEdit *edLoadPortCmd;
        TPanel *pnlLoadPortOperation;
        TPanel *pnlLoadPortErrorCode;
        TLMDButton *btnLoadPortStas;
        TLMDButton *btnLoadPortLED;
        TPanel *pnlLoadPortInterLockCode;
        TLMDButton *btnLoadPortMapping;
        TLMDButton *btnLoadPortReset;
        TPanel *pnlLoadPortReset;
        TLMDButton *btnRobotEnable;
        TLMDButton *btnLoadPortEnable;
        TPanel *pnlLoadPort2;
        TPanel *pnlLoadPort2Status;
        TComboBox *combLoadPort2Comport;
        TComboBox *combLoadPort2Baudrate;
        TPanel *Panel55;
        TPanel *Panel56;
        TPanel *pnlLoad2PortSN;
        TPanel *pnlLoadPort2Message;
        TPanel *Panel59;
        TLMDButton *btnLoadPort2Connect;
        TMemo *mmLoadPort2Msg;
        TLMDButton *btnLoadPort2Map;
        TLMDButton *btnLoadPort2Load;
        TLMDButton *btnLoadPort2UnLoad;
        TPanel *pnlLoadPort2Presence;
        TPanel *Panel61;
        TPanel *Panel62;
        TPanel *pnlLoadPort2Placement;
        TPanel *pnlLoadPort2Status1;
        TPanel *Panel66;
        TPanel *Panel67;
        TPanel *pnlLoadPort2Status2;
        TPanel *pnlLoadPort2KWF;
        TPanel *Panel72;
        TPanel *Panel73;
        TPanel *pnlLoadPort2Load;
        TPanel *pnlLoadPort2UnLoad;
        TPanel *Panel83;
        TPanel *Panel85;
        TPanel *pnlLoadPort2Alarm;
        TPanel *Panel92;
        TPanel *pnlLoadPort2Power;
        TLMDButton *btnLoadPort2CMD;
        TEdit *edLoadPort2Cmd;
        TPanel *pnlLoadPort2Operation;
        TPanel *pnlLoadPort2ErrorCode;
        TLMDButton *btnLoadPort2Stas;
        TLMDButton *btnLoadPort2LED;
        TPanel *pnlLoadPort2InterLockCode;
        TLMDButton *btnLoadPort2Mapping;
        TLMDButton *btnLoadPort2Reset;
        TPanel *pnlLoadPort2Reset;
        TLMDButton *btnLoadPort2Enable;
        TPanel *Panel122;
        TLMDRadioGroup *rgWaferNo;
        TLMDButton *btnLoad;
        TLMDButton *btnToMeasurePoint;
        TLMDButton *btnLoadWafer;
        TLMDButton *btnUnloadWafer;
        TPanel *pnlWaferChuckInformation;
        TPanel *pnlChuckStatus;
        TPanel *pnlWaferAlignerInformation;
        TPanel *Panel82;
        TPanel *pnlLoadWafer;
        TPanel *pnlUnloadWafer;
        TPanel *pnlLoad;
        TPanel *pnlToMeasurePoint;
        TPanel *Panel22;
        TPanel *pnlWaferRobotInformation;
        TPanel *pnlOperationMessage;
        TLMDRadioGroup *rgLoadOrder;
        TCheckBox *cbAutoLoad;
        TLMDButton *btnReStart;
        TLMDRadioGroup *rgMeasurePoint;
        TPanel *pnlWaferMacroInformation;
        TPanel *Panel44;
        TLMDRadioGroup *rgFoupSource;
        TPanel *pnlOperationTime;
        TPanel *Panel123;
        TPanel *Panel124;
        TPanel *Panel125;
        TPanel *Panel126;
        TPanel *Panel127;
        TPanel *Panel128;
        TPanel *Panel130;
        TPanel *Panel133;
        TPanel *Panel134;
        TLMDGroupBox *gbMotion;
        TLabel *Label137;
        TLabel *Label138;
        TLabel *Label139;
        TLabel *Label140;
        TLabel *Label141;
        TLabel *Label142;
        TLabel *Label143;
        TLabel *Label121;
        TLabel *Label49;
        TLabel *Label52;
        TLabel *Label56;
        TLabel *Label58;
        TLabel *Label62;
        TLabel *Label219;
        TLabel *Label54;
        TLabel *Label17;
        TLabel *Label19;
        TLabel *Label42;
        TLabel *Label78;
        TLabel *Label87;
        TLabel *laW2;
        TLabel *Label374;
        TLabel *Label375;
        TLabel *Label377;
        TLabel *Label380;
        TLMDEdit *edStartVelX;
        TLMDEdit *edMaxVelX;
        TLMDEdit *edDistanceX;
        TLMDEdit *edTaccX;
        TLMDEdit *edTdecX;
        TLMDEdit *edPositionX;
        TLMDEdit *edHomeVelX;
        TLMDEdit *edReviewVelX;
        TLMDEdit *edReviewVelY;
        TLMDEdit *edHomeVelY;
        TLMDEdit *edPositionY;
        TLMDEdit *edTdecY;
        TLMDEdit *edTaccY;
        TLMDEdit *edDistanceY;
        TLMDEdit *edMaxVelY;
        TLMDEdit *edStartVelY;
        TLMDEdit *edReviewVelT;
        TLMDEdit *edHomeVelT;
        TLMDEdit *edPositionT;
        TLMDEdit *edTdecT;
        TLMDEdit *edTaccT;
        TLMDEdit *edDistanceT;
        TLMDEdit *edMaxVelT;
        TLMDEdit *edStartVelT;
        TLMDEdit *edUpLimitX;
        TLMDEdit *edUpLimitY;
        TLMDEdit *edUpLimitT;
        TLMDComboBox *combHomeModeX;
        TLMDComboBox *combHomeModeY;
        TLMDComboBox *combHomeModeT;
        TLMDEdit *edLowDistanceX;
        TLMDEdit *edLowDistanceY;
        TLMDEdit *edLowDistanceT;
        TLMDEdit *edReviewVelZ;
        TLMDEdit *edHomeVelZ;
        TLMDEdit *edPositionZ;
        TLMDEdit *edTdecZ;
        TLMDEdit *edTaccZ;
        TLMDEdit *edDistanceZ;
        TLMDEdit *edMaxVelZ;
        TLMDEdit *edStartVelZ;
        TLMDEdit *edUpLimitZ;
        TLMDComboBox *combHomeModeZ;
        TLMDEdit *edLowDistanceZ;
        TLMDEdit *edMinZPosition;
        TLMDEdit *edMaxZPosition;
        TLMDEdit *edTResolution;
        TLMDButton *btnSaveMotionParameter;
        TLMDEdit *edMinTPosition;
        TLMDEdit *edMaxTPosition;
        TLMDEdit *edReviewVelR;
        TLMDEdit *edHomeVelR;
        TLMDEdit *edPositionR;
        TLMDEdit *edTdecR;
        TLMDEdit *edTaccR;
        TLMDEdit *edDistanceR;
        TLMDEdit *edMaxVelR;
        TLMDEdit *edStartVelR;
        TLMDEdit *edUpLimitR;
        TLMDComboBox *combHomeModeR;
        TLMDEdit *edLowDistanceR;
        TLMDEdit *edMinXPosition;
        TLMDEdit *edMaxXPosition;
        TLMDEdit *edMinYPosition;
        TLMDEdit *edMaxYPosition;
        TLMDGroupBox *LMDGroupBox10;
        TLabel *Label107;
        TLabel *Label106;
        TLabel *Label105;
        TLabel *Label104;
        TLabel *Label103;
        TLabel *Label102;
        TLabel *Label101;
        TLabel *Label100;
        TLabel *Label99;
        TLabel *Label96;
        TLabel *Label95;
        TLabel *Label94;
        TLabel *Label85;
        TLabel *Label84;
        TLabel *Label83;
        TLabel *Label7;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label22;
        TLabel *laW3;
        TPanel *pnlRDYX;
        TPanel *pnlALMX;
        TPanel *pnlPELX;
        TPanel *pnlNELX;
        TPanel *pnlORGX;
        TPanel *pnlDIRX;
        TPanel *pnlReserved1X;
        TPanel *pnlPCSX;
        TPanel *pnlERCX;
        TPanel *pnlEZX;
        TPanel *pnlReserved2X;
        TPanel *pnlLatchX;
        TPanel *pnlSDX;
        TPanel *pnlINPX;
        TPanel *pnlSVONX;
        TPanel *pnlRDYY;
        TPanel *pnlALMY;
        TPanel *pnlPELY;
        TPanel *pnlNELY;
        TPanel *pnlORGY;
        TPanel *pnlDIRY;
        TPanel *pnlReserved1Y;
        TPanel *pnlPCSY;
        TPanel *pnlERCY;
        TPanel *pnlEZY;
        TPanel *pnlReserved2Y;
        TPanel *pnlLatchY;
        TPanel *pnlSDY;
        TPanel *pnlINPY;
        TPanel *pnlSVONY;
        TPanel *pnlRDYZ;
        TPanel *pnlALMZ;
        TPanel *pnlPELZ;
        TPanel *pnlNELZ;
        TPanel *pnlORGZ;
        TPanel *pnlDIRZ;
        TPanel *pnlReserved1Z;
        TPanel *pnlPCSZ;
        TPanel *pnlERCZ;
        TPanel *pnlEZZ;
        TPanel *pnlReserved2Z;
        TPanel *pnlLatchZ;
        TPanel *pnlSDZ;
        TPanel *pnlINPZ;
        TPanel *pnlSVONZ;
        TPanel *pnlRDYT;
        TPanel *pnlALMT;
        TPanel *pnlPELT;
        TPanel *pnlNELT;
        TPanel *pnlORGT;
        TPanel *pnlDIRT;
        TPanel *pnlReserved1T;
        TPanel *pnlPCST;
        TPanel *pnlERCT;
        TPanel *pnlEZT;
        TPanel *pnlReserved2T;
        TPanel *pnlLatchT;
        TPanel *pnlSDT;
        TPanel *pnlINPT;
        TPanel *pnlSVONT;
        TPanel *pnlRDYR;
        TPanel *pnlALMR;
        TPanel *pnlPELR;
        TPanel *pnlNELR;
        TPanel *pnlORGR;
        TPanel *pnlDIRR;
        TPanel *pnlReserved1R;
        TPanel *pnlPCSR;
        TPanel *pnlERCR;
        TPanel *pnlEZR;
        TPanel *pnlReserved2R;
        TPanel *pnlLatchR;
        TPanel *pnlSDR;
        TPanel *pnlINPR;
        TPanel *pnlSVONR;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label80;
        TLabel *Label86;
        TLabel *Label135;
        TLabel *Label136;
        TLabel *Label388;
        TLabel *Label390;
        TLMDGroupBox *gbAOICenter;
        TLabel *Label153;
        TLabel *Label154;
        TLabel *Label90;
        TLabel *Label15;
        TLabel *Label200;
        TLMDEdit *edCenterX;
        TLMDEdit *edCenterY;
        TLMDButton *btnCenterPosition;
        TLMDButton *btnToCenterPosition;
        TLMDEdit *edCenterZ;
        TPanel *pnlToCenterPosition;
        TLMDEdit *edCenterT;
        TLMDEdit *edCenterR;
        TLMDCheckBox *cbCCDXDirection;
        TLMDCheckBox *cbCCDYDirection;
        TPanel *pnlYAltOffset;
        TPanel *pnlXAltOffset;
        TPanel *pnlYMove;
        TPanel *pnlXMove;
        TLMDButton *btnSavePosition;
        TLMDGroupBox *LMDGroupBox3;
        TLabel *Label108;
        TLabel *Label109;
        TLabel *Label110;
        TLabel *Label113;
        TLabel *Label204;
        TLMDEdit *edLoadX;
        TLMDEdit *edLoadY;
        TLMDButton *btnLoadPosition;
        TLMDButton *btnToLoadPosition;
        TLMDEdit *edLoadZ;
        TPanel *pnlToLoadPosition;
        TLMDEdit *edLoadT;
        TLMDEdit *edLoadR;
        TLMDGroupBox *LMDGroupBox4;
        TLabel *Label117;
        TLabel *Label118;
        TLabel *Label119;
        TLabel *Label120;
        TLabel *Label205;
        TLMDEdit *edOriginalX;
        TLMDEdit *edOriginalY;
        TLMDButton *btnOriginalPosition;
        TLMDButton *btnToOriginalPosition;
        TLMDEdit *edOriginalZ;
        TPanel *pnlToOriginalPosition;
        TLMDEdit *edOriginalT;
        TLMDEdit *edOriginalR;
        TLMDGroupBox *LMDGroupBox36;
        TLabel *Label294;
        TLabel *Label297;
        TLabel *Label298;
        TLabel *Label299;
        TLabel *Label206;
        TLMDEdit *edCCDX;
        TLMDEdit *edCCDY;
        TLMDButton *btnCCDPosition;
        TLMDButton *btnToCCDPosition;
        TLMDEdit *edCCDZ;
        TPanel *pnlToCCDPosition;
        TLMDEdit *edCCDT;
        TLMDEdit *edCCDR;
        TLMDGroupBox *LMDGroupBox2;
        TLabel *Label16;
        TLabel *laY1;
        TLabel *Label66;
        TLabel *laAOI;
        TLabel *Label201;
        TLMDEdit *ed45X;
        TLMDEdit *ed45Y;
        TLMDButton *btn45Position;
        TLMDButton *btnTo45Position;
        TLMDEdit *ed45Z;
        TPanel *pnlTo45Position;
        TLMDEdit *ed45T;
        TLMDEdit *ed45R;
        TLMDGroupBox *LMDGroupBox7;
        TLabel *Label68;
        TLabel *laY2;
        TLabel *Label73;
        TLabel *laLoad;
        TLabel *Label203;
        TLMDEdit *ed0X;
        TLMDEdit *ed0Y;
        TLMDButton *btn0Position;
        TLMDButton *btnTo0Position;
        TLMDEdit *ed0Z;
        TPanel *pnlTo0Position;
        TLMDEdit *ed0T;
        TLMDEdit *ed0R;
        TLMDEdit *edTMoveZMax;
        TLMDCheckBox *cbLensXDirection;
        TLMDCheckBox *cbLensYDirection;
        TLMDGroupBox *LMDGroupBox8;
        TLabel *Label144;
        TLabel *Label145;
        TLabel *Label146;
        TLabel *Label147;
        TLabel *Label202;
        TLMDEdit *edR45X;
        TLMDEdit *edR45Y;
        TLMDButton *btnR45Position;
        TLMDButton *btnToR45Position;
        TLMDEdit *edR45Z;
        TPanel *pnlToR45Position;
        TLMDEdit *edR45T;
        TLMDEdit *edR45R;
        TLabel *Label63;
        TLabel *laMacrokInner;
        TLabel *laChuckInner;
        TLabel *Label81;
        TLabel *Label82;
        TLabel *Label98;
        TLabel *Label238;
        TLabel *Label218;
        TLabel *laChuckOuter;
        TLabel *Label225;
        TLabel *laMacrokOuter;
        TLabel *Label51;
        TLabel *Label53;
        TLabel *Label55;
        TLabel *Label57;
        TLabel *Label59;
        TLabel *Label64;
        TLabel *Label65;
        TLabel *Label180;
        TLabel *Label182;
        TLabel *Label183;
        TLabel *Label184;
        TLabel *Label185;
        TLabel *Label186;
        TLabel *Label188;
        TLabel *Label189;
        TLabel *Label190;
        TLabel *Label191;
        TLabel *Label192;
        TLabel *Label193;
        TLabel *Label194;
        TLabel *Label195;
        TLabel *Label196;
        TLabel *Label197;
        TLabel *Label198;
        TLabel *Label207;
        TLabel *Label208;
        TLabel *Label210;
        TLabel *Label211;
        TLabel *Label212;
        TLabel *Label216;
        TLabel *Label217;
        TLabel *Label220;
        TLabel *Label222;
        TLabel *Label223;
        TLabel *Label224;
        TLabel *Label226;
        TLabel *Label227;
        TLabel *Label228;
        TLabel *Label368;
        TLabel *Label422;
        TLabel *Label423;
        TLabel *Label424;
        TLabel *Label435;
        TLabel *Label468;
        TLabel *Label471;
        TLabel *Label472;
        TLabel *Label373;
        TPanel *pnlBit0;
        TPanel *pnlBit1;
        TPanel *pnlBit2;
        TPanel *pnlBit3;
        TPanel *pnlBit4;
        TPanel *pnlBit5;
        TPanel *pnlBit6;
        TPanel *pnlBit7;
        TPanel *pnlCarIDDI;
        TPanel *pnlDI;
        TPanel *pnlDIMsg;
        TPanel *pnlBit8;
        TPanel *pnlBit10;
        TPanel *pnlBit9;
        TPanel *pnlBit12;
        TPanel *pnlBit11;
        TPanel *pnlBit14;
        TPanel *pnlBit13;
        TPanel *pnlBit15;
        TPanel *pnlID3Bit0;
        TPanel *pnlID3Bit1;
        TPanel *pnlID3Bit2;
        TPanel *pnlID3Bit3;
        TPanel *pnlID3Bit4;
        TPanel *pnlID3Bit5;
        TPanel *pnlID3Bit6;
        TPanel *pnlID3Bit7;
        TPanel *pnlID3Bit8;
        TPanel *pnlID3Bit10;
        TPanel *pnlID3Bit9;
        TPanel *pnlID3Bit12;
        TPanel *pnlID3Bit11;
        TPanel *pnlID3Bit14;
        TPanel *pnlID3Bit13;
        TPanel *pnlID3Bit15;
        TLMDButton *btnMacroXYHome;
        TLMDButton *btnMacroZ1Down;
        TLMDButton *btnMacroT1Reset;
        TLMDButton *btnMacroZ2Up;
        TLMDButton *btnMacroZ2Home;
        TPanel *pnlMacroXYHome;
        TPanel *pnlMacroZ1Down;
        TPanel *pnlMacroT1Reset;
        TPanel *pnlMacroZ2Up;
        TPanel *pnlMacroZ2Home;
        TLMDButton *btnMacroZ2Down;
        TPanel *pnlMacroZ2Down;
        TLMDButton *btnMacroHome;
        TPanel *pnlMacroHome;
        TLMDButton *btnMacroRobotPosition;
        TPanel *pnlMacroRobotPosition;
        TLMDButton *btnMacroReadyPosition;
        TPanel *pnlMacroReadyPosition;
        TLMDButton *btnMacroInnerPosition;
        TPanel *pnlMacroInnerPosition;
        TLMDButton *btnMacroOuterPosition;
        TPanel *pnlMacroOuterPosition;
        TLMDButton *btnMacroInnerXMove;
        TPanel *pnlMacroInnerXMove;
        TLMDButton *btnMacroInnerYMove;
        TPanel *pnlMacroInnerYMove;
        TLMDButton *btnMacroOuterTCW;
        TPanel *pnlMacroOuterTCW;
        TLMDButton *btnMacroOuterTCCW;
        TPanel *pnlMacroOuterTCCW;
        TLMDButton *btnMacroInnerTMove;
        TPanel *pnlMacroInnerTMove;
        TLMDButton *btnMacroDemo;
        TPanel *pnlMacroDemo;
        TLMDButton *btnMacroZ1Up;
        TPanel *pnlMacroZ1Up;
        TLMDButton *btnMacroInnerYBack;
        TPanel *pnlMacroInnerYBack;
        TLMDButton *btnMacroInnerXBack;
        TPanel *pnlMacroInnerXBack;
        TLMDButton *btnMacroInnerTBack;
        TPanel *pnlMacroInnerTBack;
        TCheckBox *cbDemoStep;
        TLMDButton *btnMacroInnerStop;
        TPanel *pnlMacroInnerStop;
        TLMDButton *btnMacroOuterStop;
        TPanel *pnlMacroOuterStop;
        TEdit *edDemoDelay;
        TLMDButton *btnSaveMacroDelayTime;
        TCheckBox *cbMacroDemoMove;
        TEdit *edDemoInTime;
        TEdit *edDemoOutTime;
        TEdit *edDemoPointTime;
        TCheckBox *cbMacroDemoIgnoreTimeout;
        TPanel *pnlDICount;
        TPanel *pnlAskCount;
        TPanel *pnlDoCount;
        TPanel *pnlMotionStatus;
        TLabel *Label272;
        TLabel *Label239;
        TLabel *Label441;
        TLabel *Label442;
        TLabel *Label443;
        TLabel *Label174;
        TLabel *Label176;
        TLabel *Label178;
        TPanel *pnlCarIDDO;
        TPanel *pnlDO;
        TPanel *pnlMotionState;
        TCheckBox *cbBit0;
        TCheckBox *cbBit1;
        TCheckBox *cbBit2;
        TCheckBox *cbBit3;
        TCheckBox *cbBit4;
        TCheckBox *cbBit5;
        TCheckBox *cbBit6;
        TCheckBox *cbBit7;
        TPanel *pnlDOValue;
        TCheckBox *cbBit8;
        TCheckBox *cbBit9;
        TCheckBox *cbBit10;
        TCheckBox *cbBit11;
        TCheckBox *cbBit12;
        TCheckBox *cbBit13;
        TCheckBox *cbBit14;
        TCheckBox *cbBit15;
        TCheckBox *cbIdleYellow;
        TCheckBox *cbIdleGreen;
        TCheckBox *cbMeasureYellow;
        TCheckBox *cbMeasureGreen;
        TCheckBox *cbRecipeYellow;
        TCheckBox *cbRecipeGreen;
        TLMDButton *bnSaveDO;
        TCheckBox *cbID3Bit0;
        TCheckBox *cbID3Bit1;
        TCheckBox *cbID3Bit2;
        TCheckBox *cbID3Bit3;
        TCheckBox *cbID3Bit4;
        TCheckBox *cbID3Bit5;
        TCheckBox *cbID3Bit6;
        TCheckBox *cbID3Bit7;
        TCheckBox *cbID3Bit8;
        TCheckBox *cbID3Bit9;
        TCheckBox *cbID3Bit10;
        TCheckBox *cbID3Bit11;
        TCheckBox *cbID3Bit12;
        TCheckBox *cbID3Bit13;
        TCheckBox *cbID3Bit14;
        TCheckBox *cbID3Bit15;
        TCheckBox *cbID5Bit0;
        TCheckBox *cbID5Bit1;
        TCheckBox *cbID5Bit2;
        TCheckBox *cbID5Bit3;
        TCheckBox *cbID5Bit4;
        TCheckBox *cbID5Bit5;
        TCheckBox *cbID5Bit6;
        TCheckBox *cbID5Bit7;
        TCheckBox *cbID5Bit8;
        TCheckBox *cbID5Bit9;
        TCheckBox *cbID5Bit10;
        TCheckBox *cbID5Bit11;
        TCheckBox *cbID5Bit12;
        TCheckBox *cbID5Bit13;
        TCheckBox *cbID5Bit14;
        TCheckBox *cbID5Bit15;
        TCheckBox *cbID5Bit16;
        TCheckBox *cbID5Bit17;
        TCheckBox *cbID5Bit18;
        TCheckBox *cbID5Bit19;
        TCheckBox *cbID5Bit20;
        TCheckBox *cbID5Bit21;
        TCheckBox *cbID5Bit22;
        TCheckBox *cbID5Bit23;
        TCheckBox *cbID5Bit24;
        TCheckBox *cbID5Bit25;
        TCheckBox *cbID5Bit26;
        TCheckBox *cbID5Bit27;
        TCheckBox *cbID5Bit28;
        TCheckBox *cbID5Bit29;
        TCheckBox *cbID5Bit30;
        TCheckBox *cbID5Bit31;
        TLabel *Label328;
        TLMDGroupBox *LMDGroupBox27;
        TLabel *Label213;
        TLabel *Label214;
        TLabel *Label215;
        TLabel *Label243;
        TLabel *Label327;
        TLMDEdit *edLens1X;
        TLMDButton *btnSetLens1Position;
        TLMDButton *btnToLens1Position;
        TPanel *pnlToLens1Position;
        TLMDEdit *edLens2X;
        TLMDEdit *edLens1Y;
        TLMDEdit *edLens2Y;
        TLMDEdit *edLens1Z;
        TLMDEdit *edLens2Z;
        TLMDButton *btnSetLens2Position;
        TLMDButton *btnToLens2Position;
        TPanel *pnlToLens2Position;
        TLMDEdit *edLens1ZLimit;
        TLMDEdit *edLens2ZLimit;
        TLMDButton *btnSetLens1ZLimit;
        TLMDButton *btnSetLens2ZLimit;
        TPanel *pnlLens1LED;
        TPanel *pnlLens2LED;
        TLMDEdit *edLens1T;
        TLMDEdit *edLens2T;
        TLMDButton *btnSaveLensPosition;
        TPanel *pnlSaveLensPosition;
        TCheckBox *cbCalibration;
        TLabel *Label514;
        TLMDGroupBox *LMDGroupBox13;
        TLabel *laCassette1;
        TLabel *laCassette2;
        TLabel *laCassette3;
        TLabel *laCassette4;
        TLabel *Label314;
        TLMDEdit *edWPosition1;
        TLMDButton *btnSetWPosition1;
        TLMDButton *btnToWPosition1;
        TLMDButton *btnToWPosition2;
        TLMDButton *btnSetWPosition2;
        TLMDEdit *edWPosition2;
        TLMDButton *btnToWPosition3;
        TLMDButton *btnSetWPosition3;
        TLMDEdit *edWPosition3;
        TLMDButton *btnToWPosition4;
        TLMDButton *btnSetWPosition4;
        TLMDEdit *edWPosition4;
        TPanel *pnlToWStatus;
        TPanel *Panel344;
        TImage *Image5;
        TButton *btnSaveWPosition;
        TLMDButton *btnToWPosition5;
        TLMDButton *btnSetWPosition5;
        TLMDEdit *edWPosition5;
        TPanel *Panel57;
        TImage *Image11;
        TImage *Image12;
        TImage *Image13;
        TImage *Image14;
        TImage *Image15;
        TImage *Image16;
        TLabel *Label506;
        TLabel *Label507;
        TLabel *Label313;
        TLabel *Label323;
        TImage *Image6;
        TLabel *Label315;
        TPanel *Panel58;
        TLabel *Label365;
        TLabel *Label369;
        TLabel *Label370;
        TLabel *Label371;
        TLabel *Label372;
        TLabel *laJoystick9;
        TLabel *laJoystick7;
        TLabel *laJoystick8;
        TLabel *Label376;
        TLabel *laJoystick10;
        TLabel *Label378;
        TLabel *Label379;
        TLabel *Label381;
        TLabel *Label382;
        TLabel *Label133;
        TLabel *Label446;
        TLabel *Label447;
        TLabel *Label448;
        TLabel *Label449;
        TLabel *Label450;
        TLabel *Label451;
        TLabel *Label452;
        TLabel *Label453;
        TPanel *pnlJoystick1;
        TPanel *pnlJoystick2;
        TPanel *pnlJoystick3;
        TPanel *pnlJoystick4;
        TPanel *pnlJoystick5;
        TPanel *pnlJoystick6;
        TPanel *pnlJoystick7;
        TPanel *pnlJoystick8;
        TPanel *pnlJoystick9;
        TPanel *pnlJoystick10;
        TPanel *pnlJoystickX;
        TPanel *pnlJoystickY;
        TPanel *pnlJoystickZ;
        TPanel *pnlJoystickLeft;
        TPanel *pnlJoystickRight;
        TLMDRadioGroup *rgJoystickSpeed;
        TLMDGroupBox *LMDGroupBox46;
        TLabel *Label383;
        TLabel *Label30;
        TLabel *Label429;
        TLMDEdit *edJoystickStart;
        TLMDEdit *edJoystickStop;
        TLMDButton *btnJoystickSave;
        TLMDCheckBox *cbAISpeed;
        TLMDGroupBox *LMDGroupBox47;
        TLabel *Label385;
        TLabel *Label386;
        TLabel *Label430;
        TLMDEdit *edAISpeedStart;
        TLMDButton *btnJoystickSaveAI;
        TLMDEdit *edAISpeedChange;
        TLMDRadioGroup *rgJoystickDefaultSpeed;
        TLMDRadioGroup *rgJoystickEnabled;
        TLMDRadioGroup *rgZSource;
        TCheckBox *cbJoystickYDirection;
        TCheckBox *cbJoystick2LockZ;
        TPanel *pnlJoystickRZ;
        TPanel *pnlJoystickVZ;
        TPanel *pnlJoystickVRZ;
        TPanel *pnlJoystickAZ;
        TPanel *pnlJoystickARZ;
        TPanel *pnlJoystickFZ;
        TPanel *pnlJoystickFRZ;
        TPanel *pnlJoystickPOV1;
        TPanel *pnlJoystickPOV2;
        TPanel *pnlJoystickPOV3;
        TPanel *pnlJoystickPOV4;
        TLMDEdit *edJoystickLowSpeed;
        TLMDEdit *edJoystickHighSpeed;
        TLabel *Label417;
        TLabel *Label387;
        TShape *Shape1;
        TLabel *Label38;
        TLabel *Label39;
        TLabel *Label43;
        TLabel *Label47;
        TLabel *Label48;
        TLabel *Label67;
        TLabel *Label74;
        TLabel *Label76;
        TPanel *pnlLeftUpMX;
        TPanel *pnlLeftUpMY;
        TPanel *pnlLeftUpBX;
        TPanel *pnlLeftUpBY;
        TLMDButton *btnLeftUp;
        TPanel *Panel77;
        TLMDButton *btnRightDown;
        TPanel *pnlRightDownMX;
        TPanel *pnlRightDownMY;
        TPanel *pnlRightDownBX;
        TPanel *pnlRightDownBY;
        TLMDButton *btnRangeSave;
        TLMDRadioGroup *rgRangeMode;
        TPanel *Panel76;
        TShape *Shape2;
        TCheckBox *cbNotLS;
        TCheckBox *cbEFEM;
        TLMDButton *btnMacroWithWafer;
        TPanel *pnlMacroWithWafer;
        TLMDButton *btnChuckWithWafer;
        TPanel *pnlChuckWithWafer;
        TLMDButton *btnMacroInverse;
        TPanel *pnlMacroInverse;
        TICImagingControl *ICImagingControl2;
        TICImagingControl *ICImagingControl3;
        TTimer *tmISLeftCCD;
        TTimer *tmISRightCCD;
        TPanel *pnlISCCD2Staus;
        TPanel *pnlISCCD2Capture;
        TPanel *pnlISCCD3Staus;
        TPanel *pnlISCCD3Capture;
        TPanel *pnlISCCD2Info;
        TPanel *pnlISCCD3Info;
        TComboBox *combCCDSelect;
        TLabel *Label522;
        TPanel *pnlCapturedNo2;
        TPanel *pnlCapturedNo3;
        TPanel *pnlCCD2Status;
        TPanel *pnlCCD3Status;
        TLMDButton *btnCW1x;
        TLMDButton *btnCCW1x;
        TLMDButton *btnBackwardX1;
        TLMDButton *btnMoveX1;
        TLMDButton *btnBackwardY1;
        TLMDButton *btnMoveY1;
        TLMDButton *btnBackwardZ1;
        TLMDButton *btnMoveZ1;
        TLMDButton *btnMoveR1;
        TLMDButton *btnBackwardR1;
        TRadioGroup *rgMoveStep;
        TLMDRadioGroup *rgSpeed;
        TLMDRadioGroup *rgJoystickMode;
        TLMDTabSheet *tsCST;
        TPanel *pnlCST;
        TStringGrid *sgCassette;
        TLMDButton *btnCassetteLoad;
        TPanel *pnlCassetteLoad;
        TPanel *pnlCassetteStatus;
        TLabel *Label483;
        TLabel *Label484;
        TLabel *Label485;
        TLabel *Label486;
        TLabel *Label487;
        TLabel *Label489;
        TPanel *Panel84;
        TPanel *pnlTray25;
        TPanel *pnlTray24;
        TPanel *pnlTray23;
        TPanel *pnlTray22;
        TPanel *pnlTray21;
        TPanel *pnlTray20;
        TPanel *pnlTray19;
        TPanel *pnlTray18;
        TPanel *pnlTray17;
        TPanel *pnlTray16;
        TPanel *pnlTray15;
        TPanel *pnlTray14;
        TPanel *pnlTray13;
        TPanel *pnlTray12;
        TPanel *pnlTray11;
        TPanel *pnlTray10;
        TPanel *pnlTray9;
        TPanel *pnlTray8;
        TPanel *pnlTray7;
        TPanel *pnlTray6;
        TPanel *pnlTray5;
        TPanel *pnlTray4;
        TPanel *pnlTray3;
        TPanel *pnlTray2;
        TPanel *pnlTray1;
        TPanel *pnlTray1Select;
        TPanel *pnlTray2Select;
        TPanel *pnlTray3Select;
        TPanel *pnlTray4Select;
        TPanel *pnlTray5Select;
        TPanel *pnlTray6Select;
        TPanel *pnlTray7Select;
        TPanel *pnlTray8Select;
        TPanel *pnlTray9Select;
        TPanel *pnlTray10Select;
        TPanel *pnlTray11Select;
        TPanel *pnlTray12Select;
        TPanel *pnlTray13Select;
        TPanel *pnlTray14Select;
        TPanel *pnlTray15Select;
        TPanel *pnlTray16Select;
        TPanel *pnlTray17Select;
        TPanel *pnlTray18Select;
        TPanel *pnlTray19Select;
        TPanel *pnlTray20Select;
        TPanel *pnlTray21Select;
        TPanel *pnlTray22Select;
        TPanel *pnlTray23Select;
        TPanel *pnlTray24Select;
        TPanel *pnlTray25Select;
        TPanel *Panel24;
        TLMDButton *btnAllSelect;
        TLMDButton *btnCassette2Load;
        TPanel *pnlCassette2Load;
        TPanel *pnlCassette2Status;
        TLabel *Label497;
        TLabel *Label498;
        TLabel *Label500;
        TLabel *Label501;
        TLabel *Label502;
        TLabel *Label503;
        TPanel *Panel54;
        TPanel *pnlTray2_25;
        TPanel *pnlTray2_24;
        TPanel *pnlTray2_23;
        TPanel *pnlTray2_22;
        TPanel *pnlTray2_21;
        TPanel *pnlTray2_20;
        TPanel *pnlTray2_19;
        TPanel *pnlTray2_18;
        TPanel *pnlTray2_17;
        TPanel *pnlTray2_16;
        TPanel *pnlTray2_15;
        TPanel *pnlTray2_14;
        TPanel *pnlTray2_13;
        TPanel *pnlTray2_12;
        TPanel *pnlTray2_11;
        TPanel *pnlTray2_10;
        TPanel *pnlTray2_9;
        TPanel *pnlTray2_8;
        TPanel *pnlTray2_7;
        TPanel *pnlTray2_6;
        TPanel *pnlTray2_5;
        TPanel *pnlTray2_4;
        TPanel *pnlTray2_3;
        TPanel *pnlTray2_2;
        TPanel *pnlTray2_1;
        TPanel *pnlTray2_1Select;
        TPanel *pnlTray2_2Select;
        TPanel *pnlTray2_3Select;
        TPanel *pnlTray2_4Select;
        TPanel *pnlTray2_5Select;
        TPanel *pnlTray2_6Select;
        TPanel *pnlTray2_7Select;
        TPanel *pnlTray2_8Select;
        TPanel *pnlTray2_9Select;
        TPanel *pnlTray2_10Select;
        TPanel *pnlTray2_11Select;
        TPanel *pnlTray2_12Select;
        TPanel *pnlTray2_13Select;
        TPanel *pnlTray2_14Select;
        TPanel *pnlTray2_15Select;
        TPanel *pnlTray2_16Select;
        TPanel *pnlTray2_17Select;
        TPanel *pnlTray2_18Select;
        TPanel *pnlTray2_19Select;
        TPanel *pnlTray2_20Select;
        TPanel *pnlTray2_21Select;
        TPanel *pnlTray2_22Select;
        TPanel *pnlTray2_23Select;
        TPanel *pnlTray2_24Select;
        TPanel *pnlTray2_25Select;
        TPanel *Panel151;
        TLMDButton *btnAllSelect2;
        TPanel *Panel30;
        TShape *Shape5;
        TShape *Shape7;
        TShape *Shape8;
        TShape *Shape9;
        TShape *Shape10;
        TLabel *Label337;
        TLabel *Label338;
        TLabel *Label348;
        TLabel *Label349;
        TLabel *Label350;
        TPanel *pnlSystemOperation;
        TImage *imWaferPosition;
        TTabSheet *tsMainCCD;
        TPanel *Panel138;
        TImage *imOperation;
        TPanel *Panel45;
        TImage *imAdd;
        TPanel *Panel47;
        TImage *imDec;
        TPanel *pnlMainFactor;
        TLabel *Label351;
        TTabSheet *tsMainStatus;
        TLabel *Label418;
        TLabel *Label258;
        TLabel *Label416;
        TLabel *Label303;
        TPanel *pnlLoadPortReady;
        TPanel *pnlLoadPortErrorCode1;
        TPanel *pnlLoadPortErrorMsg;
        TPanel *pnlLoadPortInterLockCode1;
        TPanel *pnlLoadPortInterLockMsg;
        TPanel *pnlAlignerReady;
        TPanel *pnlAlignerErrorCode1;
        TPanel *pnlAlignerErrorMsg;
        TPanel *pnlRobotReady;
        TPanel *pnlRobotErrorCode1;
        TPanel *pnlRobotErrorMsg;
        TPanel *pnlLoadPort2Ready;
        TPanel *pnlLoadPort2ErrorCode1;
        TPanel *pnlLoadPort2ErrorMsg;
        TPanel *pnlLoadPort2InterLockCode1;
        TPanel *pnlLoadPort2InterLockMsg;
        TPanel *pnlAligner2Ready;
        TPanel *pnlAligner2ErrorCode1;
        TPanel *pnlAligner2ErrorMsg;
        TMemo *mmMainLog;
        TEdit *edQRCodeIP2;
        TEdit *edQRCodePort2;
        TLabel *Label523;
        TButton *btnReady2;
        TPanel *pnlSocketConnect2;
        TLMDButton *btnQRCodeRead2;
        TPanel *pnlWaferID2;
        TLMDButton *btnQRCodeLog2;
        TPanel *pnlQRCodeLog2;
        TClientSocket *csQRCode2;
        TMemo *mmQRCodeMessage2;
        TEdit *edSocket2;
        TButton *btnSend2;
        TPanel *pnlSend2;
        TPanel *pnlSocket2;
        TLMDTabSheet *tsRFID;
        TPanel *Panel140;
        TPanel *pnlRFID;
        TButton *btnRFIDSend;
        TPanel *pnlRFID12;
        TPanel *pnlRFID8;
        TEdit *edRFIDCommand;
        TRadioGroup *rgRFIDLoadPort;
        TButton *btnRFIDRead;
        TButton *btnRFIDSerialNumber;
        TMemo *mmRFID;
        TPanel *pnlCSTCode;
        TPanel *pnlRFIDResult;
        TComboBox *combRFID12Comport;
        TComboBox *combRFID12Baudrate;
        TButton *btnRFIDInit;
        TButton *btnRFIDTest;
        TEdit *edRFIDTest;
        TRadioGroup *rgRFIDTagType;
        TPanel *Panel78;
        TPanel *pnlJoystickStatus;
        TPanel *pnlJoystickActive;
        TLMDCheckBox *cbContinueMove1;
        TGroupBox *GroupBox36;
        TCheckBox *cbWBAuto;
        TLMDButton *btnWBOneTouch;
        TYbCommDevice *ybRFID8;
        TYbCommDevice *ybRFID12;
        TLabel *Label524;
        TComboBox *combRFID8Comport;
        TComboBox *combRFID8Baudrate;
        TTimer *tmActiveSocket2;
        TTimer *tmStatus2;
        TLabel *Label525;
        TLabel *Label526;
        TLMDEdit *edLens3X;
        TLMDButton *btnSetLens3Position;
        TLMDButton *btnToLens3Position;
        TPanel *pnlToLens3Position;
        TLMDEdit *edLens4X;
        TLMDEdit *edLens3Y;
        TLMDEdit *edLens4Y;
        TLMDEdit *edLens3Z;
        TLMDEdit *edLens4Z;
        TLMDButton *btnSetLens4Position;
        TLMDButton *btnToLens4Position;
        TPanel *pnlToLens4Position;
        TLMDEdit *edLens3ZLimit;
        TLMDEdit *edLens4ZLimit;
        TLMDButton *btnSetLens3ZLimit;
        TLMDButton *btnSetLens4ZLimit;
        TPanel *pnlLens3LED;
        TPanel *pnlLens4LED;
        TLMDEdit *edLens3T;
        TLMDEdit *edLens4T;
        TLMDEdit *edLens5X;
        TLMDEdit *edLens5Y;
        TLMDEdit *edLens5Z;
        TLMDButton *btnSetLens5Position;
        TLMDButton *btnToLens5Position;
        TPanel *pnlToLens5Position;
        TLMDEdit *edLens5ZLimit;
        TLMDButton *btnSetLens5ZLimit;
        TPanel *pnlLens5LED;
        TLMDEdit *edLens5T;
        TGroupBox *GroupBox15;
        TLabel *Label527;
        TLabel *Label528;
        TEdit *edCCDPrecision1;
        TEdit *edCCDPrecision2;
        TEdit *edCCDPrecision3;
        TEdit *edCCDPrecision4;
        TEdit *edCCDPrecision5;
        TLabel *laCCDPrecision1;
        TLabel *laCCDPrecision2;
        TLabel *laCCDPrecision3;
        TLabel *laCCDPrecision4;
        TLabel *laCCDPrecision5;
        TPanel *pnlCSTCode2;
        TLabel *Label529;
        TTimer *tmWaitRFID;
        TPanel *pnlRFIDOperation;
        TPanel *pnlRFIDTestCount;
        TPanel *pnlRFIDRet;
        TPanel *pnlAlarm;
        TPanel *pnlSystemCDAVacuum1;
        TPanel *pnlEFEMReady1;
        TPanel *pnlBXFMReady1;
        TPanel *pnlCCDStatus2;
        TPanel *pnlAOILamp1;
        TPanel *pnlQRCodeConnect1;
        TPanel *pnlPCI78561;
        TPanel *pnlEFEMCassetteA;
        TPanel *pnlEFEMCassette2A;
        TLMDButton *btnBXFMEnable;
        TPanel *pnlBXFM;
        TLabel *Label339;
        TLabel *Label340;
        TLabel *Label341;
        TLabel *Label347;
        TLabel *Label352;
        TLabel *Label353;
        TLabel *Label359;
        TLabel *Label360;
        TLabel *Label361;
        TLabel *Label362;
        TLabel *Label363;
        TLabel *Label364;
        TLabel *Label366;
        TLabel *Label367;
        TLabel *Label393;
        TLabel *Label504;
        TLabel *Label1;
        TLabel *Label530;
        TLabel *Label531;
        TLabel *Label532;
        TPanel *pnlBXFMStatus;
        TComboBox *combBXFMComport;
        TComboBox *combBXFMBaudrate;
        TPanel *Panel27;
        TPanel *Panel28;
        TPanel *pnlBXFMSN;
        TPanel *pnlBXFMMessage;
        TPanel *Panel33;
        TLMDButton *btnBXFMConnect;
        TMemo *mmBXFMMsg;
        TLMDButton *btnBXFMCMD;
        TEdit *edBXFMCmd;
        TPanel *pnlBXFMOperation;
        TPanel *pnlBXFMErrorCode;
        TLMDButton *btnBXFMGetLMPStatus;
        TLMDButton *btnBXFMSave;
        TLMDRadioGroup *rgBXFMControl;
        TLMDRadioGroup *rgBXFMLMPSwitch;
        TLMDRadioGroup *rgBXFMLens;
        TLMDRadioGroup *rgBXFMCube;
        TLMDRadioGroup *rgBXFMLMP;
        TLMDTrackBar *tbBXFMLMP;
        TLMDTrackBar *tbBXFMAS;
        TPanel *Panel26;
        TPanel *pnlBXFMLampStatus;
        TLMDButton *btnBXFMGetLMP;
        TPanel *pnlBXFMLampSet;
        TPanel *pnlBXFMLMPValue;
        TPanel *pnlBXFMControl;
        TPanel *pnlBXFMLens;
        TPanel *pnlBXFMLMPSwitch;
        TPanel *pnlBXFMLMP;
        TPanel *pnlBXFMAS;
        TPanel *pnlBXFMCube;
        TPanel *Panel25;
        TLMDRadioGroup *rgBXFMFilter2;
        TPanel *pnlBXFMFilter2;
        TEdit *edA2MZNear;
        TEdit *edA2MZFar;
        TPanel *Panel31;
        TPanel *Panel34;
        TEdit *edA2MAFNear;
        TEdit *edA2MAFFar;
        TPanel *Panel38;
        TPanel *Panel40;
        TEdit *edA2MLens2Z;
        TEdit *edA2MLens1Z;
        TEdit *edA2MLens1AF;
        TEdit *edA2MLens2AF;
        TPanel *Panel41;
        TPanel *Panel42;
        TEdit *edA2MLens4Z;
        TEdit *edA2MLens3Z;
        TEdit *edA2MLens3AF;
        TEdit *edA2MLens4AF;
        TPanel *Panel43;
        TEdit *edA2MLens5Z;
        TEdit *edA2MLens5AF;
        TLMDButton *btnA2MORG;
        TLMDButton *btnA2MAF;
        TLMDButton *btnA2MSet;
        TLMDButton *btnA2MAFNearFarSet;
        TLMDRadioGroup *rgA2MControl;
        TPanel *pnlA2MControl;
        TPanel *pnlA2MMessage;
        TLMDButton *btnA2MCMD;
        TEdit *edA2MCmd;
        TMemo *mmA2MMsg;
        TLMDRadioGroup *rgA2MAFMode;
        TPanel *pnlA2MAF;
        TPanel *pnlA2MAFNearSet;
        TPanel *pnlA2MAFFarSet;
        TPanel *pnlA2MSet;
        TPanel *pnlA2MAFTBL;
        TPanel *pnlA2MAFStatus;
        TLMDButton *btnA2MZNearFarSet;
        TPanel *pnlA2MZNearSet;
        TPanel *pnlA2MZFarSet;
        TPanel *pnlA2MORG;
        TLMDButton *btnA2MZMove;
        TPanel *pnlA2MZMove;
        TPanel *pnlA2MRET;
        TEdit *edA2MZMoveStep;
        TLMDRadioGroup *rgA2MZDirection;
        TLMDButton *btnA2MZMoveUp;
        TLMDButton *btnA2MZMoveDown;
        TLMDRadioGroup *rgA2MRatio;
        TPanel *pnlA2MPos;
        TLMDButton *btnA2MPos;
        TLMDButton *btnA2MAPos;
        TPanel *pnlA2MAPos;
        TLMDButton *btnA2MSave;
        TPanel *Panel131;
        TPanel *Panel135;
        TLMDRadioGroup *rgDICControl;
        TPanel *pnlDICControl;
        TPanel *pnlDICMessage;
        TLMDButton *btnDICCMD;
        TEdit *edDICCmd;
        TMemo *mmDICMsg;
        TLMDButton *btnDICSave;
        TLMDRadioGroup *rgDICSet;
        TPanel *pnlDICSet;
        TEdit *edDICFCNear;
        TEdit *edDICFCFar;
        TLMDButton *btnDICFCNearFarSet;
        TPanel *pnlDICFCNearFarSet;
        TLMDRadioGroup *rgDICFC;
        TLMDRadioGroup *rgBXFMFilter1;
        TPanel *pnlBXFMFilter1;
        TComboBox *combDICComport;
        TComboBox *combDICBaudrate;
        TLMDButton *btnDICConnect;
        TPanel *pnlDICStatus;
        TPanel *pnlDICOperation;
        TPanel *pnlDICErrorCode;
        TYbCommDevice *ybDIC;
        TLabel *Label50;
        TPanel *pnlDICFCValue;
        TGroupBox *gbTest;
        TLMDProgress *pbWafer;
        TLabel *Label394;
        TLabel *Label411;
        TLMDGroupBox *gbLoopTest;
        TLMDButton *btnLoopTest;
        TLMDButton *btnLoopTestStop;
        TEdit *edLoopTestCount;
        TPanel *pnlLoopTestCount;
        TLMDRadioGroup *rgLoopTest;
        TCheckBox *cbLoopPause;
        TCheckBox *cbPause;
        TPanel *pnlLoopTest;
        TCheckBox *cbUnloadNotch;
        TCheckBox *cbAlignerAlignment;
        TCheckBox *cbChuckDemo;
        TEdit *edRobotABScanSpeed;
        TCheckBox *cbDoubleWafer;
        TCheckBox *cbAlignerQRCode;
        TLabel *laGainRange;
        TCheckBox *cbAutoExposure;
        TLMDButton *btnAutoClearWafer;
        TPanel *pnlAutoClearWafer;
        TPanel *pnlClearMessage;
        TPanel *pnlClearLocation;
        TPanel *pnlClearFoup;
        TTimer *tmGet_NextWafer;
        TTimer *tmRobotWithWafer;
        TLabel *laWafer8ZDown;
        TLMDEdit *edWafer8ZDown;
        TTimer *tmUnload_MacroJudge;
        TTimer *tmAligner2DoORGN;
        TTimer *tmAligner1DoORGN;
        TCheckBox *cbChuckScan;
        TCheckBox *cbAutoUnload;
        TLMDCheckBox *cbJoystickMoveContinue;
        TTabSheet *tsSECS;
        TTabSheet *tsUser;
        TGroupBox *GroupBox3;
        TLabel *Label290;
        TLabel *Label291;
        TLabel *Label44;
        TLMDButton *btnUserNew;
        TLMDButton *btnUserDelete;
        TLMDButton *btnUserRefresh;
        TStringGrid *sgUser;
        TLMDEdit *edUserName;
        TLMDEdit *edUserPassword;
        TLMDRadioGroup *rgLotNoGenerate;
        TLabel *Label75;
        TLabel *Label77;
        TLabel *Label79;
        TLabel *Label149;
        TLMDButton *btnSECSReConnect;
        TPanel *pnlSECSConnect;
        TEdit *edSECSPort;
        TPanel *pnlSECS;
        TEdit *edSECSCmd;
        TButton *btnSECSSend;
        TMemo *mmSECS;
        TPanel *pnlSECSMessage;
        TPanel *Panel2;
        TRadioGroup *rgEQStatus;
        TRadioGroup *rgEQExist;
        TRadioGroup *rgLowArmExist;
        TRadioGroup *rgUpArmExist;
        TRadioGroup *rgPortStatus;
        TRadioGroup *rgSECSResult;
        TEdit *edLowArmID;
        TEdit *edUpArmID;
        TEdit *edMapData;
        TRadioGroup *rgCSTExist;
        TRadioGroup *rgSECSCommand;
        TRadioGroup *rgSecsCommandSet;
        TPanel *pnlSECSCSTID;
        TPanel *pnlSECSRCP;
        TPanel *pnlSECSPriority;
        TPanel *pnlSECSPortNo;
        TEdit *edEQID;
        TPanel *pnlSECSWaferID;
        TRadioGroup *rgSECSType;
        TEdit *edSECSRecipeID;
        TPanel *pnlSECSDateTime;
        TRadioGroup *rgSECSPortNo;
        TRadioGroup *rgSECSStatus;
        TEdit *edSECSWaferID;
        TEdit *edSECSSlotNo;
        TEdit *edSECSSourceWID;
        TEdit *edSECSReadWID;
        TPanel *pnlSECSAlarmCode;
        TRadioGroup *rgSECSOccur;
        TRadioGroup *rgSECSLevel;
        TRadioGroup *rgSECSRecipe;
        TLMDCheckBox *cbTableDirection;
        TPanel *pnlLoopTestWaferCount;
        TLabel *Label150;
        TCheckBox *cbChuckVacuum;
        TCheckBox *cbMacroDemo;
        TLabel *Label151;
        TPanel *pnlLooptestTime;
        TEdit *edChuckDemoNo;
        TTabSheet *tsUserPosition;
        TLMDGroupBox *LMDGroupBox5;
        TLabel *Label173;
        TLMDEdit *edPos1X;
        TLMDEdit *edPos1Y;
        TLMDButton *btnPos1Set;
        TLMDEdit *edPos1Z;
        TLMDEdit *edPos1T;
        TLMDButton *btnPos1Go;
        TLMDGroupBox *LMDGroupBox6;
        TLabel *Label175;
        TLMDEdit *edPos2X;
        TLMDEdit *edPos2Y;
        TLMDButton *btnPos2Set;
        TLMDEdit *edPos2Z;
        TLMDEdit *edPos2T;
        TLMDButton *btnPos2Go;
        TLMDGroupBox *LMDGroupBox16;
        TLabel *Label177;
        TLMDEdit *edPos3X;
        TLMDEdit *edPos3Y;
        TLMDButton *btnPos3Set;
        TLMDEdit *edPos3Z;
        TLMDEdit *edPos3T;
        TLMDButton *btnPos3Go;
        TLMDGroupBox *LMDGroupBox17;
        TLabel *Label179;
        TLMDEdit *edPos4X;
        TLMDEdit *edPos4Y;
        TLMDButton *btnPos4Set;
        TLMDEdit *edPos4Z;
        TLMDEdit *edPos4T;
        TLMDButton *btnPos4Go;
        TLMDGroupBox *LMDGroupBox18;
        TLabel *Label181;
        TLMDEdit *edPos5X;
        TLMDEdit *edPos5Y;
        TLMDButton *btnPos5Set;
        TLMDEdit *edPos5Z;
        TLMDEdit *edPos5T;
        TLMDButton *btnPos5Go;
        TLMDGroupBox *LMDGroupBox39;
        TLabel *Label354;
        TLMDEdit *edPos6X;
        TLMDEdit *edPos6Y;
        TLMDButton *btnPos6Set;
        TLMDEdit *edPos6Z;
        TLMDEdit *edPos6T;
        TLMDButton *btnPos6Go;
        TLMDGroupBox *LMDGroupBox40;
        TLabel *Label355;
        TLMDEdit *edPos7X;
        TLMDEdit *edPos7Y;
        TLMDButton *btnPos7Set;
        TLMDEdit *edPos7Z;
        TLMDEdit *edPos7T;
        TLMDButton *btnPos7Go;
        TLMDGroupBox *LMDGroupBox41;
        TLabel *Label356;
        TLMDEdit *edPos8X;
        TLMDEdit *edPos8Y;
        TLMDButton *btnPos8Set;
        TLMDEdit *edPos8Z;
        TLMDEdit *edPos8T;
        TLMDButton *btnPos8Go;
        TLMDGroupBox *LMDGroupBox42;
        TLabel *Label357;
        TLMDEdit *edPos9X;
        TLMDEdit *edPos9Y;
        TLMDButton *btnPos9Set;
        TLMDEdit *edPos9Z;
        TLMDEdit *edPos9T;
        TLMDButton *btnPos9Go;
        TLMDGroupBox *LMDGroupBox43;
        TLabel *Label358;
        TLMDEdit *edPos10X;
        TLMDEdit *edPos10Y;
        TLMDButton *btnPos10Set;
        TLMDEdit *edPos10Z;
        TLMDEdit *edPos10T;
        TLMDButton *btnPos10Go;
        TPanel *pnlToUserPosition;
        TLMDButton *btnPositionSave;
        TPanel *pnlPositionGo;
        TCheckBox *cbCheckLightBar;
        TYbCommDevice *ybLoadPort2;
        TServerSocket *ssSECSSocket;
        TCheckBox *cbOffLine;
        TPanel *pnlLocal;
        TLMDButton *btnLocal;
        TPanel *pnlRemote;
        TLMDButton *btnRemote;
        TTimer *tmDecodeSECS;
        TPanel *pnlSECSSend;
        TCheckBox *cbSECSAtuo;
        TCheckBox *cbOffLineAtuo;
        TTimer *tmAutoLoadWafer;
        TLMDRadioGroup *rgLoadPortSelectTemp;
        TLMDRadioGroup *rgLoadPortSelect;
        TTimer *tmDoLoadUnload;
        TTimer *tmAutoUnload;
        TLMDButton *btnSECSConnect;
        TPanel *pnlSECSStatus;
        TPanel *pnlStatus;
        TButton *btnSetFFU;
        TLabel *Label4;
        TLMDButton *btnQRCodeReadImage;
        TLMDButton *btnQRCodeReadBMP;
        TCheckBox *cbTowerControl;
        void __fastcall pnlSocketClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnSendClick(TObject *Sender);
        void __fastcall mnSaveToFileClick(TObject *Sender);
        void __fastcall mnClearMessageClick(TObject *Sender);
        void __fastcall pnlVersionClick(TObject *Sender);
        void __fastcall csQRCode1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall csQRCode1Connect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall btnReadyClick(TObject *Sender);
        void __fastcall csQRCode1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall csQRCode1Read(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall tmActiveSocketTimer(TObject *Sender);
        void __fastcall tmISCCDTimer(TObject *Sender);
        void __fastcall rgCCDShutterClick(TObject *Sender);
        void __fastcall tbCCDGainChange(TObject *Sender);
        void __fastcall rgRulerColorClick(TObject *Sender);
        void __fastcall tbContrastChange(TObject *Sender);
        void __fastcall rgRulerAuxChange(TObject *Sender, int ButtonIndex);
        void __fastcall btnReviewLoadClick(TObject *Sender);
        void __fastcall cbRulerChange(TObject *Sender);
        void __fastcall btnLampControl1Click(TObject *Sender);
        void __fastcall btnSaveLED2Click(TObject *Sender);
        void __fastcall tbLED2CH1Change(TObject *Sender);
        void __fastcall tbLED2CH1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbLED2CH2Change(TObject *Sender);
        void __fastcall tbLED2CH2MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbLED2CH3Change(TObject *Sender);
        void __fastcall tbLED2CH4Change(TObject *Sender);
        void __fastcall tbLED2CH3MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbLED2CH4MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tmMotionPollingTimer(TObject *Sender);
        void __fastcall btnReviewLiveClick(TObject *Sender);
        void __fastcall btnToCenterPositionClick(TObject *Sender);
        void __fastcall btnCenterPositionClick(TObject *Sender);
        void __fastcall btnHomeClick(TObject *Sender);
        void __fastcall imCCDMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall btnMoveClick(TObject *Sender);
        void __fastcall btnMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardClick(TObject *Sender);
        void __fastcall btnBackwardMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnHomeOneAxisClick(TObject *Sender);
        void __fastcall btnAlarmOnClick(TObject *Sender);
        void __fastcall btnAlarmOffClick(TObject *Sender);
        void __fastcall btnServoOnClick(TObject *Sender);
        void __fastcall btnServoOffClick(TObject *Sender);
        void __fastcall btnDOOnClick(TObject *Sender);
        void __fastcall btnDOOffClick(TObject *Sender);
        void __fastcall btnBackwardXClick(TObject *Sender);
        void __fastcall btnBackwardXMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardXMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardYClick(TObject *Sender);
        void __fastcall btnBackwardYMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardYMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveYClick(TObject *Sender);
        void __fastcall btnMoveYMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveYMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveXClick(TObject *Sender);
        void __fastcall btnMoveXMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveXMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveTClick(TObject *Sender);
        void __fastcall btnMoveTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardZClick(TObject *Sender);
        void __fastcall btnBackwardZMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardZMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveZClick(TObject *Sender);
        void __fastcall btnMoveZMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveZMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardTClick(TObject *Sender);
        void __fastcall btnBackwardTMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardTMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnStopClick(TObject *Sender);
        void __fastcall btnMoveAbsoluteClick(TObject *Sender);
        void __fastcall btnStopAbsoluteClick(TObject *Sender);
        void __fastcall cbContinueMoveChange(TObject *Sender);
        void __fastcall cbContinueMove1Change(TObject *Sender);
        void __fastcall pnlWidthValueClick(TObject *Sender);
        void __fastcall tmSystemTimer(TObject *Sender);
        void __fastcall tm400msTimer(TObject *Sender);
        void __fastcall btnSavePositionClick(TObject *Sender);
        void __fastcall btnSaveMotionParameterClick(TObject *Sender);
        void __fastcall btnSaveCCDClick(TObject *Sender);
        void __fastcall btnSaveQRCodeClick(TObject *Sender);
        void __fastcall laCIMClick(TObject *Sender);
        void __fastcall edCCDPrecisionExit(TObject *Sender);
        void __fastcall pnlQRCodeConnectClick(TObject *Sender);
        void __fastcall cbBit0Click(TObject *Sender);
        void __fastcall tmStatusTimer(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall btnPasswordClick(TObject *Sender);
        void __fastcall btnLogClick(TObject *Sender);
        void __fastcall btnUserNewClick(TObject *Sender);
        void __fastcall btnUserDeleteClick(TObject *Sender);
        void __fastcall btnUserRefreshClick(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall btnToOriginalPositionClick(TObject *Sender);
        void __fastcall btnPos1SetClick(TObject *Sender);
        void __fastcall btnPos2SetClick(TObject *Sender);
        void __fastcall btnPos3SetClick(TObject *Sender);
        void __fastcall btnPos4SetClick(TObject *Sender);
        void __fastcall btnPos5SetClick(TObject *Sender);
        void __fastcall btnPos6SetClick(TObject *Sender);
        void __fastcall btnPos7SetClick(TObject *Sender);
        void __fastcall btnPos8SetClick(TObject *Sender);
        void __fastcall btnPos9SetClick(TObject *Sender);
        void __fastcall btnPos10SetClick(TObject *Sender);
        void __fastcall btnPos1GoClick(TObject *Sender);
        void __fastcall btnPos2GoClick(TObject *Sender);
        void __fastcall btnPos3GoClick(TObject *Sender);
        void __fastcall btnPos4GoClick(TObject *Sender);
        void __fastcall btnPos5GoClick(TObject *Sender);
        void __fastcall btnPos6GoClick(TObject *Sender);
        void __fastcall btnPos7GoClick(TObject *Sender);
        void __fastcall btnPos8GoClick(TObject *Sender);
        void __fastcall btnPos9GoClick(TObject *Sender);
        void __fastcall btnPos10GoClick(TObject *Sender);
        void __fastcall btnToLoadPositionClick(TObject *Sender);
        void __fastcall btnLeftUpClick(TObject *Sender);
        void __fastcall btnRightDownClick(TObject *Sender);
        void __fastcall btnRangeSaveClick(TObject *Sender);
        void __fastcall rgJoystickSpeedClick(TObject *Sender);
        void __fastcall rgJoystickDefaultSpeedChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall pnlJoystick7Click(TObject *Sender);
        void __fastcall pnlJoystick8Click(TObject *Sender);
        void __fastcall pnlJoystick9Click(TObject *Sender);
        void __fastcall pnlJoystick10Click(TObject *Sender);
        void __fastcall rgJoystickEnabledChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnJoystickSaveClick(TObject *Sender);
        void __fastcall btnJoystickSaveAIClick(TObject *Sender);
        void __fastcall imTableMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall imTableMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btn45PositionClick(TObject *Sender);
        void __fastcall btn0PositionClick(TObject *Sender);
        void __fastcall btnLoadPositionClick(TObject *Sender);
        void __fastcall btnOriginalPositionClick(TObject *Sender);
        void __fastcall btnCCDPositionClick(TObject *Sender);
        void __fastcall btnTo45PositionClick(TObject *Sender);
        void __fastcall btnToCCDPositionClick(TObject *Sender);
        void __fastcall btnTo0PositionClick(TObject *Sender);
        void __fastcall btnSpeedUpdateClick(TObject *Sender);
        void __fastcall btnAutoFocusClick(TObject *Sender);
        void __fastcall tmJoystickTimer(TObject *Sender);
        void __fastcall tmTriggerTimer(TObject *Sender);
        void __fastcall tmXTimer(TObject *Sender);
        void __fastcall tmYTimer(TObject *Sender);
        void __fastcall tmZTimer(TObject *Sender);
        void __fastcall imCCDDblClick(TObject *Sender);
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
        void __fastcall shCenterMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shCenterMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shCenterMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall cbStdPatternClick(TObject *Sender);
        void __fastcall btnEraseMatchBoxClick(TObject *Sender);
        void __fastcall btnMatchToCenterClick(TObject *Sender);
        void __fastcall btnSaveMatchParameterClick(TObject *Sender);
        void __fastcall btnUpdateFocusClick(TObject *Sender);
        void __fastcall mnToLensClick(TObject *Sender);
        void __fastcall mnToCCDClick(TObject *Sender);
        void __fastcall cbBit2Click(TObject *Sender);
        void __fastcall cbBit1Click(TObject *Sender);
        void __fastcall cbBit11Click(TObject *Sender);
        void __fastcall cbBit12Click(TObject *Sender);
        void __fastcall cbBit13Click(TObject *Sender);
        void __fastcall cbBit14Click(TObject *Sender);
        void __fastcall cbBit15Click(TObject *Sender);
        void __fastcall imCCDMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnSetLens1PositionClick(TObject *Sender);
        void __fastcall btnSetLens2PositionClick(TObject *Sender);
        void __fastcall btnToLens1PositionClick(TObject *Sender);
        void __fastcall btnToLens2PositionClick(TObject *Sender);
        void __fastcall btnSetLens1ZLimitClick(TObject *Sender);
        void __fastcall btnSetLens2ZLimitClick(TObject *Sender);
        void __fastcall btnSaveLensPositionClick(TObject *Sender);
        void __fastcall pnlTableXClick(TObject *Sender);
        void __fastcall btnR45PositionClick(TObject *Sender);
        void __fastcall btnToR45PositionClick(TObject *Sender);
        void __fastcall pcSystemChange(TObject *Sender);
        void __fastcall bnSaveDOClick(TObject *Sender);
        void __fastcall btnUpdateMotionParameterClick(TObject *Sender);
        void __fastcall btnType1LeftClick(TObject *Sender);
        void __fastcall btnType1RightClick(TObject *Sender);
        void __fastcall btnType1BackClick(TObject *Sender);
        void __fastcall btnType1FrontClick(TObject *Sender);
        void __fastcall btnType2LeftClick(TObject *Sender);
        void __fastcall btnType3LeftClick(TObject *Sender);
        void __fastcall btnType4LeftClick(TObject *Sender);
        void __fastcall btnType2RightClick(TObject *Sender);
        void __fastcall btnType3RightClick(TObject *Sender);
        void __fastcall btnType4RightClick(TObject *Sender);
        void __fastcall btnType2BackClick(TObject *Sender);
        void __fastcall btnType3BackClick(TObject *Sender);
        void __fastcall btnType4BackClick(TObject *Sender);
        void __fastcall btnType2FrontClick(TObject *Sender);
        void __fastcall btnType3FrontClick(TObject *Sender);
        void __fastcall btnType4FrontClick(TObject *Sender);
        void __fastcall btnUpdateArrayDieClick(TObject *Sender);
        void __fastcall cbCCDFullImageChange(TObject *Sender);
        void __fastcall btnReCalibrationClick(TObject *Sender);
        void __fastcall cbBit3Click(TObject *Sender);
        void __fastcall rgZSourceChange(TObject *Sender, int ButtonIndex);
        void __fastcall cbJoystickYDirectionClick(TObject *Sender);
        void __fastcall cbJoystick2LockZClick(TObject *Sender);
        void __fastcall btnMacroXYHomeClick(TObject *Sender);
        void __fastcall btnMacroZ1DownClick(TObject *Sender);
        void __fastcall btnMacroT1ResetClick(TObject *Sender);
        void __fastcall btnMacroZ2UpClick(TObject *Sender);
        void __fastcall btnMacroZ2HomeClick(TObject *Sender);
        void __fastcall btnMacroZ2DownClick(TObject *Sender);
        void __fastcall btnMacroZ1UpClick(TObject *Sender);
        void __fastcall btnMacroHomeClick(TObject *Sender);
        void __fastcall cbID3Bit0Click(TObject *Sender);
        void __fastcall cbID5Bit0Click(TObject *Sender);
        void __fastcall btnMacroRobotPositionClick(TObject *Sender);
        void __fastcall btnMacroReadyPositionClick(TObject *Sender);
        void __fastcall btnMacroInnerPositionClick(TObject *Sender);
        void __fastcall btnMacroInnerYMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterPositionClick(TObject *Sender);
        void __fastcall btnMacroOuterTCWMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCWMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCCWMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCCWMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall rgJoystickModeChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnMacroDemoClick(TObject *Sender);
        void __fastcall btnMacroInnerStopClick(TObject *Sender);
        void __fastcall btnMacroOuterStopClick(TObject *Sender);
        void __fastcall btnMacroInnerYMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerYBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerXMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerXBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTBackMouseExit(TObject *Sender);
        void __fastcall btnMacroOuterTCWMouseExit(TObject *Sender);
        void __fastcall btnMacroOuterTCCWMouseExit(TObject *Sender);
        void __fastcall btnMoveRClick(TObject *Sender);
        void __fastcall btnMoveRMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveRMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardRClick(TObject *Sender);
        void __fastcall btnBackwardRMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardRMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tmSetupEFEMTimer(TObject *Sender);
        void __fastcall btnRobotOriginalClick(TObject *Sender);
        void __fastcall btnLoadPortStasClick(TObject *Sender);
        void __fastcall btnLoadPortCMDClick(TObject *Sender);
        void __fastcall btnLoadPortUnLoadClick(TObject *Sender);
        void __fastcall btnAlignerORGNClick(TObject *Sender);
        void __fastcall btnRobotCMDClick(TObject *Sender);
        void __fastcall btnAlignerCMDClick(TObject *Sender);
        void __fastcall btnRobotGetPositionClick(TObject *Sender);
        void __fastcall cbEFEMClick(TObject *Sender);
        void __fastcall ybRobotCommNotify(TObject *Sender, int NotifyType);
        void __fastcall ybLoadPortCommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall ybAlignerCommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall btnLoadPortMapClick(TObject *Sender);
        void __fastcall tmLoadUnloadTimer(TObject *Sender);
        void __fastcall btnAlignerStasClick(TObject *Sender);
        void __fastcall btnRobotEnableClick(TObject *Sender);
        void __fastcall btnLoadPortEnableClick(TObject *Sender);
        void __fastcall btnAlignerEnableClick(TObject *Sender);
        void __fastcall btnRobotStasClick(TObject *Sender);
        void __fastcall combRobotComportChange(TObject *Sender);
        void __fastcall combLoadPortComportChange(TObject *Sender);
        void __fastcall combAlignerComportChange(TObject *Sender);
        void __fastcall combRobotBaudrateChange(TObject *Sender);
        void __fastcall combLoadPortBaudrateChange(TObject *Sender);
        void __fastcall combAlignerBaudrateChange(TObject *Sender);
        void __fastcall tmBeepTimer(TObject *Sender);
        void __fastcall btnRobotConnectClick(TObject *Sender);
        void __fastcall btnRobotPosClick(TObject *Sender);
        void __fastcall btnRobotGetClick(TObject *Sender);
        void __fastcall btnRobotPutClick(TObject *Sender);
        void __fastcall btnRobotVacuumOnClick(TObject *Sender);
        void __fastcall btnRobotVacuumOffClick(TObject *Sender);
        void __fastcall btnRobotWithWaferClick(TObject *Sender);
        void __fastcall btnRobotClearAlarmClick(TObject *Sender);
        void __fastcall btnRobotEanbleClick(TObject *Sender);
        void __fastcall btnRobotDisableClick(TObject *Sender);
        void __fastcall btnLoadPortConnectClick(TObject *Sender);
        void __fastcall btnLoadPortLoadClick(TObject *Sender);
        void __fastcall btnLoadPortLEDClick(TObject *Sender);
        void __fastcall btnLoadPortMappingClick(TObject *Sender);
        void __fastcall btnLoadPortResetClick(TObject *Sender);
        void __fastcall btnAlignerConnectClick(TObject *Sender);
        void __fastcall btnAlignerVacuumOffClick(TObject *Sender);
        void __fastcall btnAlignerAlignmentClick(TObject *Sender);
        void __fastcall btnAlignerWithWaferClick(TObject *Sender);
        void __fastcall btnAlignerSetClick(TObject *Sender);
        void __fastcall btnAlignerGetClick(TObject *Sender);
        void __fastcall btnAlignerResetClick(TObject *Sender);
        void __fastcall btnAlignerSaveClick(TObject *Sender);
        void __fastcall combLoadPort2ComportChange(TObject *Sender);
        void __fastcall combLoadPort2BaudrateChange(TObject *Sender);
        void __fastcall btnLoadPort2ConnectClick(TObject *Sender);
        void __fastcall btnLoadPort2CMDClick(TObject *Sender);
        void __fastcall btnLoadPort2EnableClick(TObject *Sender);
        void __fastcall tmAlignerORGTimer(TObject *Sender);
        void __fastcall btnLoadPort2LoadClick(TObject *Sender);
        void __fastcall btnLoadPort2UnLoadClick(TObject *Sender);
        void __fastcall btnLoadPort2LEDClick(TObject *Sender);
        void __fastcall btnLoadPort2MapClick(TObject *Sender);
        void __fastcall btnLoadPort2StasClick(TObject *Sender);
        void __fastcall btnLoadPort2MappingClick(TObject *Sender);
        void __fastcall btnLoadPort2ResetClick(TObject *Sender);
        void __fastcall ybLoadPort3CommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall tmLoadUnload2Timer(TObject *Sender);
        void __fastcall btnCassetteLoadClick(TObject *Sender);
        void __fastcall btnCassette2LoadClick(TObject *Sender);
        void __fastcall btnSaveWPositionClick(TObject *Sender);
        void __fastcall btnSetWPosition1Click(TObject *Sender);
        void __fastcall btnSetWPosition2Click(TObject *Sender);
        void __fastcall btnSetWPosition3Click(TObject *Sender);
        void __fastcall btnSetWPosition4Click(TObject *Sender);
        void __fastcall btnSetWPosition5Click(TObject *Sender);
        void __fastcall btnToWPosition1Click(TObject *Sender);
        void __fastcall btnToWPosition2Click(TObject *Sender);
        void __fastcall btnToWPosition3Click(TObject *Sender);
        void __fastcall btnToWPosition4Click(TObject *Sender);
        void __fastcall btnToWPosition5Click(TObject *Sender);
        void __fastcall combLED1ComportChange(TObject *Sender);
        void __fastcall combLED2ComportChange(TObject *Sender);
        void __fastcall FormMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall FormMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall cbMeasureChange(TObject *Sender);
        void __fastcall mnClearClick(TObject *Sender);
        void __fastcall mnSaveFileClick(TObject *Sender);
        void __fastcall shMeasureMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shMeasureMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shMeasureMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shEllipseMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall shEllipseMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall shEllipseMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCircleClick(TObject *Sender);
        void __fastcall btnAngleClick(TObject *Sender);
        void __fastcall mnPoint1Click(TObject *Sender);
        void __fastcall mnPoint2Click(TObject *Sender);
        void __fastcall mnPoint3Click(TObject *Sender);
        void __fastcall btnCircleComputeClick(TObject *Sender);
        void __fastcall btnCircleExitClick(TObject *Sender);
        void __fastcall Panel23Click(TObject *Sender);
        void __fastcall cbWaferMapClick(TObject *Sender);
        void __fastcall imWaferMapMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imWaferMapMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall sbWaferMapMouseWheel(TObject *Sender,
          TShiftState Shift, int WheelDelta, TPoint &MousePos,
          bool &Handled);
        void __fastcall sbWaferMapMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall tbZoomMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall sbWaferMapMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall sbWaferMapMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imWaferMapMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbZoomChange(TObject *Sender);
        void __fastcall btnLoadMeasureImageClick(TObject *Sender);
        void __fastcall imWaferMapDblClick(TObject *Sender);
        void __fastcall combBXFMComportChange(TObject *Sender);
        void __fastcall combBXFMBaudrateChange(TObject *Sender);
        void __fastcall btnBXFMConnectClick(TObject *Sender);
        void __fastcall btnBXFMCMDClick(TObject *Sender);
        void __fastcall rgBXFMControlChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall rgBXFMLensChange(TObject *Sender, int ButtonIndex);
        void __fastcall rgBXFMCubeChange(TObject *Sender, int ButtonIndex);
        void __fastcall rgBXFMLMPChange(TObject *Sender, int ButtonIndex);
        void __fastcall tbBXFMLMPChange(TObject *Sender);
        void __fastcall rgBXFMLMPSwitchChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnBXFMGetLMPStatusClick(TObject *Sender);
        void __fastcall tbBXFMASChange(TObject *Sender);
        void __fastcall ybBXFMCommNotify(TObject *Sender, int NotifyType);
        void __fastcall pnlBXFMErrorCodeClick(TObject *Sender);
        void __fastcall btnBXFMGetLMPClick(TObject *Sender);
        void __fastcall rgBXFMFilter2Change(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnBXFMSaveClick(TObject *Sender);
        void __fastcall btnBXFMEnableClick(TObject *Sender);
        void __fastcall tbBXFMLMPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall tbBXFMASMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Panel27Click(TObject *Sender);
        void __fastcall btnQRCodeReadClick(TObject *Sender);
        void __fastcall btnResetClick(TObject *Sender);
        void __fastcall btnQRCodeLogClick(TObject *Sender);
        void __fastcall cbPauseClick(TObject *Sender);
        void __fastcall btnLens1Click(TObject *Sender);
        void __fastcall btnLens2Click(TObject *Sender);
        void __fastcall btnLens3Click(TObject *Sender);
        void __fastcall btnLens4Click(TObject *Sender);
        void __fastcall btnLens5Click(TObject *Sender);
        void __fastcall btnA2MORGClick(TObject *Sender);
        void __fastcall btnA2MAFClick(TObject *Sender);
        void __fastcall btnA2MSetClick(TObject *Sender);
        void __fastcall btnA2MAFNearFarSetClick(TObject *Sender);
        void __fastcall btnA2MCMDClick(TObject *Sender);
        void __fastcall rgA2MControlChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnLoadWaferClick(TObject *Sender);
        void __fastcall btnA2MZNearFarSetClick(TObject *Sender);
        void __fastcall btnA2MAutoFocusClick(TObject *Sender);
        void __fastcall btnUnloadWaferClick(TObject *Sender);
        void __fastcall btnToMeasurePointClick(TObject *Sender);
        void __fastcall btnLoadClick(TObject *Sender);
        void __fastcall btnA2MZMoveClick(TObject *Sender);
        void __fastcall tmAlignerORGNTimer(TObject *Sender);
        void __fastcall btnA2MZMoveUpClick(TObject *Sender);
        void __fastcall btnA2MZMoveDownClick(TObject *Sender);
        void __fastcall tmToLoadPositionTimer(TObject *Sender);
        void __fastcall btnA2MPosClick(TObject *Sender);
        void __fastcall btnPositionSaveClick(TObject *Sender);
        void __fastcall btnA2MAPosClick(TObject *Sender);
        void __fastcall btnSaveMacroDelayTimeClick(TObject *Sender);
        void __fastcall btnA2MSaveClick(TObject *Sender);
        void __fastcall tmAlignerORGNDoneTimer(TObject *Sender);
        void __fastcall btnRobotSpeedClick(TObject *Sender);
        void __fastcall rgCCDSpeedChange(TObject *Sender, int ButtonIndex);
        void __fastcall btnRobotSaveClick(TObject *Sender);
        void __fastcall btnAllSelectClick(TObject *Sender);
        void __fastcall btnAllSelect2Click(TObject *Sender);
        void __fastcall pnlTraySelectClick(TObject *Sender);
        void __fastcall pnlTray2SelectClick(TObject *Sender);
        void __fastcall btnLoopTestClick(TObject *Sender);
        void __fastcall btnLoopTestStopClick(TObject *Sender);
        void __fastcall cbAutoLoadClick(TObject *Sender);
        void __fastcall imMapCloseClick(TObject *Sender);
        void __fastcall csYuanliSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall csYuanliSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall csYuanliSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall csYuanliSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall rgYuanliCommandSetClick(TObject *Sender);
        void __fastcall btnYunliReConnectClick(TObject *Sender);
        void __fastcall btnYuanliSendClick(TObject *Sender);
        void __fastcall pnlMainRecipeClick(TObject *Sender);
        void __fastcall pnlMainInspectionClick(TObject *Sender);
        void __fastcall pnlMainClick(TObject *Sender);
        void __fastcall pnlMovingClick(TObject *Sender);
        void __fastcall tmChuckMeasureTimer(TObject *Sender);
        void __fastcall tmMacroDemoTimer(TObject *Sender);
        void __fastcall tmRobotLSTimer(TObject *Sender);
        void __fastcall pnlStartMessageClick(TObject *Sender);
        void __fastcall pnlMainUserClick(TObject *Sender);
        void __fastcall pnlMainDeviceClick(TObject *Sender);
        void __fastcall pnlMainTroublesClick(TObject *Sender);
        void __fastcall pnlMainMaintenanceClick(TObject *Sender);
        void __fastcall pnlMainMeasurementClick(TObject *Sender);
        void __fastcall pnlMainOperationClick(TObject *Sender);
        void __fastcall pnlMainSignalClick(TObject *Sender);
        void __fastcall pnlMainModeClick(TObject *Sender);
        void __fastcall pnlMainVersionClick(TObject *Sender);
        void __fastcall pnlMainHelpClick(TObject *Sender);
        void __fastcall pnlEFEMReadyClick(TObject *Sender);
        void __fastcall imOperationMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall imAddClick(TObject *Sender);
        void __fastcall imDecClick(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall pnlCurrentOperationClick(TObject *Sender);
        void __fastcall btnSVONAllClick(TObject *Sender);
        void __fastcall pcMainChange(TObject *Sender);
        void __fastcall imOperationMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall imOperationMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pnlXClick(TObject *Sender);
        void __fastcall pnlYClick(TObject *Sender);
        void __fastcall combAligner2ComportChange(TObject *Sender);
        void __fastcall combAligner2BaudrateChange(TObject *Sender);
        void __fastcall btnAligner2ConnectClick(TObject *Sender);
        void __fastcall btnAligner2CMDClick(TObject *Sender);
        void __fastcall btnAligner2ORGNClick(TObject *Sender);
        void __fastcall btnAligner2VacuumOffClick(TObject *Sender);
        void __fastcall btnAligner2AlignmentClick(TObject *Sender);
        void __fastcall btnAligner2StasClick(TObject *Sender);
        void __fastcall btnAligner2WithWaferClick(TObject *Sender);
        void __fastcall btnAligner2SetClick(TObject *Sender);
        void __fastcall btnAligner2GetClick(TObject *Sender);
        void __fastcall btnAligner2ResetClick(TObject *Sender);
        void __fastcall btnAligner2SaveClick(TObject *Sender);
        void __fastcall btnAligner2EnableClick(TObject *Sender);
        void __fastcall ybAligner2CommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall tmAligner2ORGNDoneTimer(TObject *Sender);
        void __fastcall tmAligner2ResetTimer(TObject *Sender);
        void __fastcall tmAlignerAckTimer(TObject *Sender);
        void __fastcall tmAligner2AckTimer(TObject *Sender);
        void __fastcall tmRobotAckTimer(TObject *Sender);
        void __fastcall btnAlignerPRPClick(TObject *Sender);
        void __fastcall btnAligner2PRPClick(TObject *Sender);
        void __fastcall imCloseClick(TObject *Sender);
        void __fastcall btnMacroWithWaferClick(TObject *Sender);
        void __fastcall btnChuckWithWaferClick(TObject *Sender);
        void __fastcall btnMacroInverseClick(TObject *Sender);
        void __fastcall combCCDSelectChange(TObject *Sender);
        void __fastcall tmISLeftCCDTimer(TObject *Sender);
        void __fastcall tmISRightCCDTimer(TObject *Sender);
        void __fastcall cbWBAutoClick(TObject *Sender);
        void __fastcall ybRFID8CommNotify(TObject *Sender, int NotifyType);
        void __fastcall ybRFID12CommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall tmActiveSocket2Timer(TObject *Sender);
        void __fastcall pnlSocket2Click(TObject *Sender);
        void __fastcall btnSend2Click(TObject *Sender);
        void __fastcall btnReady2Click(TObject *Sender);
        void __fastcall btnQRCodeRead2Click(TObject *Sender);
        void __fastcall csQRCode2Connect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall csQRCode2Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall csQRCode2Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall csQRCode2Read(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall tmStatus2Timer(TObject *Sender);
        void __fastcall btnRFIDReadClick(TObject *Sender);
        void __fastcall pnlRFIDOperationClick(TObject *Sender);
        void __fastcall tmWaitRFIDTimer(TObject *Sender);
        void __fastcall btnRFIDTestClick(TObject *Sender);
        void __fastcall btnAutoMatchClick(TObject *Sender);
        void __fastcall pnlMaintenanceMessageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pnlMaintenanceMessageMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall pnlMaintenanceMessageMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall pnlAlarmClick(TObject *Sender);
        void __fastcall rgDICControlChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnDICCMDClick(TObject *Sender);
        void __fastcall btnDICSaveClick(TObject *Sender);
        void __fastcall rgDICSetChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall btnDICFCNearFarSetClick(TObject *Sender);
        void __fastcall rgBXFMFilter1Change(TObject *Sender,
          int ButtonIndex);
        void __fastcall combDICComportChange(TObject *Sender);
        void __fastcall combDICBaudrateChange(TObject *Sender);
        void __fastcall btnDICConnectClick(TObject *Sender);
        void __fastcall ybDICCommNotify(TObject *Sender, int NotifyType);
        void __fastcall pnlDICFCValueClick(TObject *Sender);
        void __fastcall cbDemoStepClick(TObject *Sender);
        void __fastcall cbAutoExposureClick(TObject *Sender);
        void __fastcall btnAutoClearWaferClick(TObject *Sender);
        void __fastcall tmGet_NextWaferTimer(TObject *Sender);
        void __fastcall tmRobotWithWaferTimer(TObject *Sender);
        void __fastcall tmUnload_MacroJudgeTimer(TObject *Sender);
        void __fastcall tmAligner2DoORGNTimer(TObject *Sender);
        void __fastcall tmAligner1DoORGNTimer(TObject *Sender);
        void __fastcall cbTableDirectionClick(TObject *Sender);
        void __fastcall cbJoystickMoveContinueClick(TObject *Sender);
        void __fastcall Panel33Click(TObject *Sender);
        void __fastcall ybLoadPort2CommNotify(TObject *Sender,
          int NotifyType);
        void __fastcall btnSECSReConnectClick(TObject *Sender);
        void __fastcall btnSECSSendClick(TObject *Sender);
        void __fastcall rgSecsCommandSetClick(TObject *Sender);
        void __fastcall Label75Click(TObject *Sender);
        void __fastcall ssSECSSocketClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ssSECSSocketClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ssSECSSocketClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall ssSECSSocketClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall tmDecodeSECSTimer(TObject *Sender);
        void __fastcall tmAutoLoadWaferTimer(TObject *Sender);
        void __fastcall tmDoLoadUnloadTimer(TObject *Sender);
        void __fastcall tmAutoUnloadTimer(TObject *Sender);
        void __fastcall btnLocalClick(TObject *Sender);
        void __fastcall btnRemoteClick(TObject *Sender);
        void __fastcall cbOffLineClick(TObject *Sender);
        void __fastcall pnlStatusClick(TObject *Sender);
        void __fastcall btnSetFFUClick(TObject *Sender);
        void __fastcall btnQRCodeReadImageClick(TObject *Sender);
        void __fastcall btnQRCodeReadBMPClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrnMain(TComponent* Owner);

   AnsiString SystemDirectory;                              //系統根目錄

   // 2019 8 13 - chc port
   #define SYSTEM_INI_FILENAME                     "\\WLS3200.ini"
   AnsiString SystemINIFilename;
   void __fastcall TfrnMain::LoadSystemParameter();
   int QRCodePort1;
   AnsiString QRCodeIP1;
   int QRCodePort2;
   AnsiString QRCodeIP2;

   void __fastcall TfrnMain::WriteSystemLog(AnsiString msg);
   AnsiString LogFileName;
   AnsiString LogDirectory;

   // CCD的影像顯示改由EImageBW8 - CPU效能問題!!!!
   EImageBW8 CCDImageAry[3];
   EROIBW8 CCDImageROIAry[3];
   EROIBW8 CCDImageMarkROIAry[3];
   EImageC24 usb_ImageAry[3];
   EROIC24 usb_ImageROIAry[3];
   EROIC24 usb_ImageMarkROIAry[3];

   void __fastcall TfrnMain::SetupCCD(int ccdtype);
   bool boolCCD,boolCCD2,boolCCD3;
   int numDevices;
   // CCD FOV vs CCDImage Area
   int CCDRangeX,CCDRangeY,CCDDx,CCDDy;
   int LaserCenterX,LaserCenterY;
   void __fastcall TfrnMain::CheckRange(int *sx, int *sy);
   void __fastcall TfrnMain::GetImageXY(int width,int height,int sx,int sy,int w,int h,int *x,int *y);
   void TfrnMain::GetTimeTic(long *time, short *millitm);
   // ImagingSource Name
   AnsiString ISCCDName, ISCCDSerialNo;

   // 2022 12 26 - chc ImagingSource Name: DFK 33GP1300 + 44614266
   AnsiString ISCCD2Name, ISCCD2SerialNo;
   AnsiString ISCCD3Name, ISCCD3SerialNo;

   // Init ImagingSource CCD
   void __fastcall TfrnMain::MakeDeviceSettings();
   int CCDShutter,CCDGain,CCDContrast;
   void __fastcall TfrnMain::DrawRuler();
   void __fastcall TfrnMain::DrawGridRuler();
   void __fastcall TfrnMain::ClearGridImage();
   AnsiString BitmapDirectory,RecipeDirectory,ImageDirectory;
   // LED Controller
   #define LED_PARAMETER_SECTION         "LED Parameter"
   #define LED2_CH1_VALUE                "LED2 CH1 Value"
   #define LED2_CH2_VALUE                "LED2 CH2 Value"
   #define LED2_CH3_VALUE                "LED2 CH3 Value"
   #define LED2_CH4_VALUE                "LED2 CH4 Value"
   #define LED_INI_FILENAME              "\\LEDParameter.ini"
   AnsiString RecipeName;
   int LED2CH1Value,LED2CH2Value,LED2CH3Value,LED2CH4Value;
   bool LampControlStatus1;
   bool LampControlStatus2;
   void __fastcall TfrnMain::UpdateLampStatus1();
   void __fastcall TfrnMain::InitLampControl1();
   void __fastcall TfrnMain::UpdateLampStatus2();
   void __fastcall TfrnMain::InitLampControl2();
   // 讀取亮度值
   void __fastcall TfrnMain::ReadLEDValue();
   void __fastcall TfrnMain::SetLED2Value(int chno, int value);

   // 202 2 24 - chc Init ImagingSource CCD
   void __fastcall TfrnMain::MakeDeviceSettings_2();
   void __fastcall TfrnMain::MakeDeviceSettings_3();

   // Motion
   // 分配軸
   // 2023 2 26 - chc MAX_AXIS_NO: Parameter
   //#define MAX_AXIS_NO     5
   #define MAX_AXIS_NO_DEFAULT     5

   #define X_AXIS          0
   #define Y_AXIS          1
   #define Z_AXIS          2
   #define T_AXIS          3
   #define R_AXIS          4
   void __fastcall TfrnMain::SetupMotion();
   int CurrentAxisNo;
   struct MOTION_STRU   {
      int StartVel,MaxVel,Distance,Position,HomeVel,ReviewVel,UpLimit,HomeMode;
      bool ORGLogic;
      double Tacc,Tdec;
      // Home補償量(僅用於Z軸)
      int HomeOffset;
      // Low Distance(慢速用)
      int LowDistance;
   };

   // 2023 2 26 - chc MAX_AXIS_NO: Parameter
   //struct MOTION_STRU   MotionParameter[MAX_AXIS_NO];
   struct MOTION_STRU   MotionParameter[MAX_AXIS_NO_DEFAULT];

   // T軸旋轉危險區域
   int MinZPosition,MaxZPosition;
   #define T_RESOLUTION    0.002

   //#define X_RESOLUTION    0.4
   double X_RESOLUTION;
   //#define Y_RESOLUTION    0.1
   double Y_RESOLUTION;
   #define Z_RESOLUTION    0.1
   // 記錄是否在Update?
   bool boolInMotionUpdate;
   bool boolDIO;
   bool boolSystemCDA;
   // 是否執行過Home?
   bool boolHomeDone;
   // 記錄是否有開門或按下EMO
   bool boolDoorEMO;
   void __fastcall TfrnMain::MotionStatus(bool mode);
   // Blinking
   bool boolStartState;
   bool boolAllHomeState;
   void __fastcall TfrnMain::SetMotionStatus();
   // 記錄在move中, 不可以同時Continue move
   bool boolInAxisMove;
   // 連動
   bool boolJogMove;
   bool __fastcall TfrnMain::CheckCDAVacuum();
   bool boolCDAVacuum;
   void __fastcall TfrnMain::MoveMouseUp();
   void __fastcall TfrnMain::BackwardMouseUp();
   AnsiString AxisName;
   void __fastcall TfrnMain::AlarmSound();
   void __fastcall TfrnMain::AxisSVON();
   void __fastcall TfrnMain::UpdateDeviceStatus();
   bool __fastcall TfrnMain::DOServoOnOff(int axisno, int cnt);
   #define MAX_POSITION_NO              7
   #define POSITION_CENTER              0
   #define POSITION_45                  1
   #define POSITION_0                   2
   #define POSITION_LOAD                3
   #define POSITION_ORIGINAL            4
   #define POSITION_CCD                 5
   #define POSITION_R45                 6
   struct POSITION_STRU {
      int X,Y,Z,T;
   };
   struct POSITION_STRU  Position[MAX_POSITION_NO];
   #define POSITION_PARAMETER_SECTION      "Position"
   #define MOTION_INFORMATION_SECTION      "Motion"
   // 各軸參數
   #define START_VEL          " Start Vel"
   #define MAX_VEL            " Max Vel"
   #define DISTANCE           " Distance"
   #define TACC               " Tacc"
   #define TDEC               " Tdec"
   #define POSITION           " Position"
   #define HOME_VEL           " Home Vel"
   #define REVIEW_VEL         " Review Vel"
   #define UP_LIMIT           " Up Limit"
   #define ORG_LOGIC          " ORG Logic"
   #define HOME_MODE          " Home Mode"
   #define LOW_DISTANCE       " Low Distance"
   #define CCD_PRECISION      "CCD Precision"
   double CCDPrecision;
   int GridPixel;
   // Ruler Color
   int iRulerColor;
   // Cross Mark
   int iCrossMark;

   void __fastcall TfrnMain::AddPerformanceMessage(AnsiString putmsg);
   void __fastcall TfrnMain::Set400Width();
   void __fastcall TfrnMain::SetMoving(bool mode);
   void __fastcall TfrnMain::ComputeAverageGray();

   void __fastcall TfrnMain::SetLevel(bool boolset);
   void __fastcall TfrnMain::SwitchLevel();
   #define DEVICE_MN            0
   #define DEVICE_NQC           1
   #define PRIORITY_OP          0
   #define PRIORITY_PE          1
   #define PRIORITY_AD          2
   #define PRIORITY_EE          3
   int PriorityLevel;
   #define PASSWORD_SECTION     "Password"
   #define USER_NUMS            "UserNums"
   #define USER                 "User"
   #define NAME                 "Name"
   #define LEVEL                "Level"
   #define PASSWORD             "Password"
   int UserNums;
   #define MAX_USER             30
   struct PASSWORD_STRU {
      AnsiString Name;
      AnsiString Level;
      AnsiString Password;
   };
   struct PASSWORD_STRU UserList[MAX_USER];
   void __fastcall TfrnMain::LoadUser();
   void __fastcall TfrnMain::UserTitle();
   #define USER_FIELD_NO                  0               // No
   #define USER_FIELD_NAME                1               // Name
   #define USER_FIELD_PASSWORD            2               // Password
   #define USER_FIELD_LEVEL               3               // Level
   void __fastcall TfrnMain::SaveUser();
   AnsiString __fastcall TfrnMain::GetPasswordCode(AnsiString password);

   // 2021 9 4 - chc 存檔
   void __fastcall TfrnMain::SaveWindowScreen(TForm *Form);
   int SaveBMPSerialNo;

   // 2021 11 11 - chc 心跳包Interval
   int StatusInterval;

   // 2021 11 23 - chc 角度微調: TOffsetAdjust
   double TOffsetAdjust;

   // 2021 12 1 - chc 角度微調: TOffsetDirection
   bool boolTOffsetDirection;

   //void __fastcall TfrnMain::ToUserPositionFun(int no);
   #define JOG_LOW_SPEED        0
   #define JOG_MIDDLE_SPEED     1
   #define JOG_HIGH_SPEED       2
   int JogSpeed;
   #define JOYSTICK_DEFAULT_SPEED               "Joystick Default Speed"
   int JoystickDefaultSpeed;
   void __fastcall TfrnMain::LoadJoystickDefaultSpeed();
   void __fastcall TfrnMain::LoadJoystick();

   void __fastcall TfrnMain::ToUserPositionFun(int no);

   // Focus
   // 影像對焦演算法: Sobel
   // FocusParameter.ini
   #define FOCUS_INI_FILENAME                  "\\FocusParameter.ini"
   AnsiString FocusINIFilename;
   #define FOCUS_INFORMATION_SECTION           "Focus Information"
   // SpeedParameter.ini
   #define SPEED_INI_FILENAME                  "\\SpeedParameter.ini"
   AnsiString SpeedINIFilename;
   #define SPEED_INFORMATION_SECTION           "Speed Information"
   void __fastcall TfrnMain::LoadSpeedParameter();
   bool boolJoystick;
   // Joystick參數儲存
   #define JOYSTICK_START                   "Joystick Start"
   #define JOYSTICK_STOP                    "Joystick Stop"
   #define JOYSTICK_LOW_SPEED               "Joystick Low Speed"
   #define JOYSTICK_HIGH_SPEED              "Joystick High Speed"
   int JoystickStart,JoystickStop;
   void __fastcall TfrnMain::SetJoystickActive(bool flag);
   void __fastcall TfrnMain::DoMoveX();
   void __fastcall TfrnMain::DoBackwardX();
   void __fastcall TfrnMain::DoMoveY();
   void __fastcall TfrnMain::DoBackwardY();
   void __fastcall TfrnMain::DoMoveZ();
   void __fastcall TfrnMain::DoBackwardZ();
   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
   int MoveAxis, MoveDirection;
   bool MoveIgnore;
   int MoveIgnoreAxis,MoveIgnoreDirection;
   // 指定7/8/9/10何者為Enable/Disable
   int JoystickEnabledKey,JoystickEnabledMode;
   void __fastcall TfrnMain::SetJoystickEnabledKey();
   void __fastcall TfrnMain::SetJoystickEnabledName();
   // Joystick參數儲存
   #define AISPEED_START                   "AISpeed Start"
   #define AISPEED_CHANGE                  "AISpeed change"
   int AISpeedStart,AISpeedChange;
   #define BOOL_AISPEED                    "Bool AISpeed"
   bool boolAISpeed;
   // 記錄各軸是否運作中
   #define NO_MOVE              0
   #define FORWARD_MOVE         1
   #define BACKWARD_MOVE        -1

   // 2023 2 26 - chc MAX_AXIS_NO: Parameter
   //int iAxisMove[MAX_AXIS_NO];
   int iAxisMove[MAX_AXIS_NO_DEFAULT];

   int InAIMoveXSpeed;
   int InAIMoveYSpeed;

   // 2022 6 6a - chc 給AutoFocus.cpp用
   struct CCDINFO_STRU        {
      int Width,Height;
      int No;
      int PixelSize;
      int Type;
      int TotalSize;
      AnsiString Name;
      AnsiString DeviceID;
   };
   struct CCDINFO_STRU        CCDInfoAry[3];
   // timeout from 5sec to 30sec
   #define MOTION_TIMEOUT                  30000
   // 到入料點/原點用最高速
   bool boolUseMaxSpeed;
   void __fastcall TfrnMain::UpdateCenterDotPosition();
   // Recipe
   #define MAX_BASE_NO               2
   #define MAX_DOT_NO                1000
   #define DOT_FIELD_INDEX                0               // index
   #define DOT_FIELD_X                    1
   #define DOT_FIELD_Y                    2
   #define DOT_FIELD_Z                    3
   #define DOT_FIELD_RECIPE               4
   struct BASE_STRU       {
      int X,Y,Z,T;
   };
   struct DOT_STRU       {
      int X,Y,Z;
      AnsiString Recipe;
   };
   struct RECIPE_STRU   {
      AnsiString Name;
      int RecipeType;
      int TypeWidth[4];
      int TypeHeight[4];

      int Size;
      // Base/Area/Dot information
      struct DOT_STRU DotBuf[MAX_DOT_NO];
      struct BASE_STRU BaseBuf[MAX_BASE_NO];
      int DotNo;

      // 圓/矩形Recipe
      int Diameter;
      int Dx,Dy,XNo,YNo;
      int Direction;

      // LED亮度與CCD: Shutter & Gain值
      int LEDValue[4];
      int Shutter,Gain;

      // Match Parameter
      double MinScore;
      int MatchMode;
      int MinReduceArea;

   };
   struct RECIPE_STRU RecipeBufferEdit;
   struct RECIPE_STRU RecipeBufferRun;
   #define MODE_RUN             1
   #define MODE_EDIT            2
   #define RECIPE_INFORMATION_SECTION           "Recipe Information"
   #define RECICPE_NAME                         "Name"
   #define LED_VALUE                 "LED Value"
   #define CCD_SHUTTER               "CCD Shutter"
   #define CCD_GAIN                  "CCD Gain"
   double __fastcall TfrnMain::GetLensPrecision();
   #define CCD_BW                0
   #define CCD_COLOR             1

   // ToLens
   int LensNo;
   void __fastcall TfrnMain::CCDToLens(int lensno, int mode);
   struct ALARM_STRU    {
      bool boolCDA;
      bool boolVacuum;
      bool boolMotion;
      bool boolCCD;
      bool boolLED;
      bool boolDI;
      bool boolDO;
      bool boolEMO;
      bool boolPower;
      bool boolBXFM;
      bool boolOperation;
      bool boolInDoor;

      // 2022 12 5 - chc 光柵
      bool boolLPLight;

   };
   
   struct ALARM_STRU    AlarmStatus;
   void __fastcall TfrnMain::InitAlarmStatus();
   void __fastcall TfrnMain::ForceToOriginal();
   void __fastcall TfrnMain::CloseAllTimer();
   // CCD X Direction
   bool boolCCDXDirection;
   // CCD Y Direction
   bool boolCCDYDirection;
   // Lasr Cneter Of Each Lens
   struct LENS_LASER_STRU     {
      int X,Y,Z,T;
      int LaserX,LaserY;
      // Z Limit
      int ZLimit;
      // Precision
      double Precision;
   };
   struct LENS_LASER_STRU     LensPosition[6];
   // Lens Position
   #define LENS_INI_FILENAME                  "\\LensPosition.ini"
   #define LENS_POSITION_SECTION              "Lens Position"
   AnsiString LensINIFilename;
   // Load Lens Position
   void __fastcall TfrnMain::LoadLensPosition(int lensno);
   bool boolLensXDirection;
   bool boolLensYDirection;
   void __fastcall TfrnMain::UpdateTablePosition();
   void __fastcall TfrnMain::ReadRangeData();
   bool __fastcall TfrnMain::IsHeadTilt();
   bool __fastcall TfrnMain::IsHeadHorizontal();
   bool __fastcall TfrnMain::IsHeadRTilt();
   #define SYSTEM_IDLE          0
   #define SYSTEM_MEASURE       1
   #define SYSTEM_RECIPE        2
   #define SYSTEM_ALARM         3
   #define DOOR_ALARM           4
   #define POWER_ALARM          5
   #define CDA_ALARM            6
   #define VACUUM_ALARM         7
   #define LIGHT_ALARM          8
   void __fastcall TfrnMain::SetLight(int mode);
   void __fastcall TfrnMain::UpdateAlarmStatus();
   // Idle/Measure/Recipe Lamp
   bool boolIdleYellow,boolIdleGreen;
   bool boolMeasureYellow,boolMeasureGreen;
   bool boolRecipeYellow,boolRecipeGreen;
   #define BOOL_IDLE_YELLOW             "Bool Idle Yellow"
   #define BOOL_IDLE_GREEN              "Bool Idle Green"
   #define BOOL_MEASURE_YELLOW          "Bool Measure Yellow"
   #define BOOL_MEASURE_GREEN           "Bool Measure Green"
   #define BOOL_RECIPE_YELLOW           "Bool Recipe Yellow"
   #define BOOL_RECIPE_GREEN            "Bool Recipe Green"
   void __fastcall TfrnMain::TypeMoveLeftRight(int no, int dir);
   void __fastcall TfrnMain::TypeMoveFrontBack(int no, int dir);

   // 2022 6 29 - chc Z Source, Y Direciton
   int JoystickZSource;
   bool boolJoystickYDirection;
   bool boolJoystick2LockZ;

   void __fastcall TfrnMain::WaitTime(int no);
   bool __fastcall TfrnMain::CheckMacroReady();
   bool __fastcall TfrnMain::CheckMacroZ2Up();
   bool __fastcall TfrnMain::CheckMacroZ1Up();

   #define JOYSTICK_TABLE               0
   #define JOYSTICK_MACRO_INNER         1
   #define JOYSTICK_MACRO_OUTER         2
   int MacroInnerXStatus;
   int MacroInnerYStatus;
   int MacroInnerTStatus;
   int MacroOuterYStatus;

   // 2022 7 6 - chc EFEM
   // Robot
   bool boolRobotStatus,boolAlignerStatus,boolLoadPortStatus,boolAligner2Status;
   void __fastcall TfrnMain::UpdateRobotStatus();
   void __fastcall TfrnMain::UpdateLoadPortStatus();
   void __fastcall TfrnMain::UpdateAlignerStatus();

   // 2022 12 5 - chc Full or LP+Robot
   //bool __fastcall TfrnMain::SetupEFEM();
   bool __fastcall TfrnMain::SetupEFEM(int mode);

   // Robot Recv
   void __fastcall TfrnMain::AddRobotSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddRobotRecvMessage(AnsiString msg);
   // LoadPort Recv
   void __fastcall TfrnMain::AddLoadPortSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddLoadPortRecvMessage(AnsiString msg);
   // Aligner Recv
   void __fastcall TfrnMain::AddAlignerSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddAlignerRecvMessage(AnsiString msg);
   bool __fastcall TfrnMain::WaitRobotDone(timems);
   // Robot點位
   bool __fastcall TfrnMain::SendRobotPosCMD(int pot);
   void __fastcall TfrnMain::UpdateCassetteState(int mode);
   void __fastcall TfrnMain::UpdateLoadPortStatus_1();
   void __fastcall TfrnMain::UpdateAlignerStatus_1();
   void __fastcall TfrnMain::UpdateRobotStatus_1(int mode);
   bool __fastcall TfrnMain::WaitLoadPortDone(timems);
   #define ROBOT_WAFER_UNKOWN           0
   #define ROBOT_WAFER_YES              1
   #define ROBOT_WAFER_NO               2
   int RobotWithWafer;
   // LoadPortLED[8]
   void __fastcall TfrnMain::UpdateLoadPortLED_1();
   bool __fastcall TfrnMain::ChckChuckVacuumOn();
   bool __fastcall TfrnMain::ChckChuckVacuumOff();
   // Init ComPort
   void __fastcall TfrnMain::InitRobotCom();
   void __fastcall TfrnMain::InitLoadPortCom();
   void __fastcall TfrnMain::InitAlignerCom();
   // Enable/Disable
   void __fastcall TfrnMain::RobotButton();
   void __fastcall TfrnMain::LoadPortButton(int mode);
   void __fastcall TfrnMain::AlignerButton();
   // Robot是否已動作?
   bool boolRobot4401;
   bool __fastcall TfrnMain::BeforeLoadPortLoadUnload();
   void __fastcall TfrnMain::PutRobotErrorCodeMsg();
   bool boolLoadUnloadStatus;
   void __fastcall TfrnMain::PutLoadPortInterLockMsg();
   // TSMC修改
   bool boolPCDoor,boolPower,boolLeftMaintenanceDoor,boolRightMaintenanceDoor,boolInspectDoor;
   bool boolRedLamp,boolBuzzer,boolEFEMError;
   void __fastcall TfrnMain::PutLoadPortErrorCodeMsg();
   void __fastcall TfrnMain::PutAlignerCodeMsg();
   void __fastcall TfrnMain::AddErrorLog(AnsiString msg);
   AnsiString ErrorLogFileName,SystemHelpFileName;
   void __fastcall TfrnMain::InitLoadPort2Com();
   bool boolLoadPort2Status;
   void __fastcall TfrnMain::LoadPort2Button(int mode);
   void __fastcall TfrnMain::LoadAlignerParameter();
   // 選擇角度: AlignmentDegreeMode
   #define ALIGNMENT_DEGREE_LOAD          1
   #define ALIGNMENT_DEGREE_UNLOAD        2
   int AlignmentDegreeMode;
   // Aligner
   int AlignerDegree,AlignerDegreeOut;
   int AlignerSpeed;
   int AlignerType;
   int Aligner2Degree,Aligner2DegreeOut;
   int Aligner2Speed;
   int Aligner2Type;
   void __fastcall TfrnMain::UpdateLoadPort2Status();
   // 2022 7 10 - chc 先做Robot ORGN(GP1/GP0) & 偵測pnlLoadPortS12是否為1?
   void __fastcall TfrnMain::AddLoadPort2SendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddLoadPort2RecvMessage(AnsiString msg);
   bool boolLoadUnloadStatus2;
   void __fastcall TfrnMain::UpdateCassette2State(int mode);
   void __fastcall TfrnMain::PutLoadPort2InterLockMsg();
   void __fastcall TfrnMain::UpdateLoadPort2Status_1();
   void __fastcall TfrnMain::UpdateLoadPort2LED_1();
   void __fastcall TfrnMain::PutLoadPort2ErrorCodeMsg();
   bool __fastcall TfrnMain::BeforeUnload();

   // 2022 7 13 - chc 4個W點位
   void __fastcall TfrnMain::LoadWPosition();
   // RobotW.ini檔名與目錄名
   #define ROBOTW_INI_FILENAME             "\\RobotW.ini"
   AnsiString RobotWINIFilename;
   #define ROBOTW_PARAMETER_SECTION        "RobotW Parameter"
   #define MAX_W_POSITION              5
   int WPositionSet[MAX_W_POSITION];
   #define WPOS_LOADPORT1                  0
   #define WPOS_LOADPORT2                  1
   #define WPOS_ALIGNER                    2
   #define WPOS_CHUCK                      3
   #define WPOS_MACRO                      4
   void __fastcall TfrnMain::InitLED1Com();
   void __fastcall TfrnMain::InitLED2Com();
   // 2022 7 15 - chc Measure
   int MeasureState;
   int Mx1,My1,Mx2,My2;
   #define MM_TO_UM        10000.0
   void __fastcall TfrnMain::AddMeasureMessage(AnsiString msg);
   AnsiString ExportDirectory;
   struct WAFER_STRU    {
      int X,Y,Dx,Dy;
      bool boolSet;
   };
   struct WAFER_STRU Wafer;
   void __fastcall TfrnMain::InitWaferMap();
   void __fastcall TfrnMain::ZoomImage();
   #define CCD_TYPE_NONE                0
   #define CCD_TYPE_IS                  1
   #define CCD_TYPE_BASLER              2
   int CCDType;
   bool boolBaslerGrabbedAry[3];
   int scA1300ShutterAry[3],scA1300GainAry[3],scA1300GrayAry[3];
   EROIBW8 scA1300ImageROIAry[3];
   void __fastcall TfrnMain::GigaBuffer1(int ccdtype);
   void __fastcall TfrnMain::GigaBuffer2(int ccdtype);
   void __fastcall TfrnMain::GigaBuffer3(int ccdtype);
   void __fastcall TfrnMain::GigaCapture1();
   void __fastcall TfrnMain::GigaCapture2();
   void __fastcall TfrnMain::GigaCapture3();
   void __fastcall TfrnMain::CaptureGigaFrame1(int no, int mode);
   void __fastcall TfrnMain::CaptureGigaFrame2(int no, int mode);
   void __fastcall TfrnMain::CaptureGigaFrame3(int no, int mode);
   AnsiString CCDSN;
   #define MAX_CCD_NO           3
   long BaslerCaptureStartTimeAry[MAX_CCD_NO],BaslerCaptureStopTimeAry[MAX_CCD_NO],BaslerCaptureElapsedmsAry[MAX_CCD_NO];
   short BaslerCaptureStartTickAry[MAX_CCD_NO],BaslerCaptureStopTickAry[MAX_CCD_NO];
   void __fastcall TfrnMain::AdjustCenterLine();
   bool boolBXFMStatus,boolDICStatus;
   void __fastcall TfrnMain::BXFMButton();
   void __fastcall TfrnMain::UpdateBXFMStatus();
   void __fastcall TfrnMain::InitBXFMCom();
   void __fastcall TfrnMain::AddBXFMSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddBXFMRecvMessage(AnsiString msg);

   // 2022 8 23 - chc Load BXFM/A2M Parameter: 但不執行
   //void __fastcall TfrnMain::ReadBXFMParameter();
   void __fastcall TfrnMain::ReadBXFMParameter(int mode);

   #define BXFM_CONTROL_LOCAL                   0
   #define BXFM_CONTROL_REMOTE                  1
   #define BXFM_CUBE_BF                         0
   #define BXFM_CUBE_DF                         1
   #define BXFM_LMP_DIA                         0
   #define BXFM_LMP_EPI                         1
   #define BXFM_LMP_SWITCH_ON                   0
   #define BXFM_LMP_SWITCH_OFF                  1
   #define DIC_IN                               0
   #define DIC_OUT                              1
   void __fastcall TfrnMain::SetBXFMValue();
   double QRCodeDegree;
   void __fastcall TfrnMain::ChuckVacuum(bool mode);
   bool __fastcall TfrnMain::GoRobotWAxis(int mode);

   // BSFM	1LMP 1~120
   void __fastcall TfrnMain::SetBXFMLMP();
   void __fastcall TfrnMain::SetBXFMAS();
   void __fastcall TfrnMain::SelectLens(int no);
   void __fastcall TfrnMain::AddA2MSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddA2MRecvMessage(AnsiString msg);

   // 記錄目前是否在做取退片?
   bool boolInLoadWafer,boolInUnloadWafer;
   bool boolForTest;
   bool boolCSTWithoutWafer;
   bool boolChuckWithoutWafer;
   bool boolAlignerWithoutWafer;
   bool boolCSTWithWafer;
   bool boolChuckWithWaferError;
   bool boolAlignerWithWafer;
   bool boolWaferNotSpecified;
   #define LOADPORT_1                 0
   #define LOADPORT_2                 1
   void __fastcall TfrnMain::SetFristWafer();
   #define LOCATION_LP1               0
   #define LOCATION_LP2               1
   #define LOCATION_ALIGNER1          2
   #define LOCATION_ALIGNER2          3
   #define LOCATION_CHUCK             4
   #define LOCATION_MACRO             5
   int __fastcall TfrnMain::ToNextWafer(int no);
   void __fastcall TfrnMain::UpdateProgress(int no);
   bool boolDoAlignerORGN;
   void __fastcall TfrnMain::LoadUserPosition();
   bool __fastcall TfrnMain::WaitLoadPort2Done(timems);
   #define USER_INI_FILENAME                  "\\UserPosition.ini"
   AnsiString UserINIFilename;
   #define USER_POSITION_SECTION           "User Position"
   void __fastcall TfrnMain::LoadMacroParameter();

   // 2022 8 23 - chc Load BXFM/A2M Parameter: 但不執行
   //void __fastcall TfrnMain::ReadA2MParameter();
   void __fastcall TfrnMain::ReadA2MParameter(int mode);

   int MASTER_SCREEN_WIDTH, MASTER_SCREEN_HEIGHT;
   void __fastcall TfrnMain::LoadCOMPort();

   // 2022 8 16 - chc Robot Speed control
   int RobotABSpeed,RobotZSpeed,RobotABScanSpeed;
   void __fastcall TfrnMain::LoadRobotParameter();
   #define WAFER_GET            0
   #define WAFER_PUT            1
   void __fastcall TfrnMain::ChangeWaferPosition(int pos, int getput, int step);
   int __fastcall TfrnMain::GetFirstNo();
   int __fastcall TfrnMain::GetLastNo();
   int __fastcall TfrnMain::GetNextNo(int cno);
   bool __fastcall TfrnMain::CheckWithWafer(int cno);

   struct STAGE_STRU {
      bool boolWithWafer;
      int SlotNo;
      int FoupNo;
      AnsiString WaferID;
   };
   #define STAGE_ROBOT               0
   #define STAGE_ALIGNER             1
   #define STAGE_MACRO               2
   #define STAGE_CHUCK               3
   #define STAGE_MAX_NO              4
   struct STAGE_STRU  StageInformation[STAGE_MAX_NO];
   void __fastcall TfrnMain::InitStageInformation();
   // YuanLi Socket
   bool boolYuanliSocket;
   bool boolSendYuanliCommand;
   // Sockeet
   #define CMD_MAX                 7
   // to YUANLI: YuanLi, Move, Busy, Invalid
   #define CMD_YUANLI              0
   #define CMD_MOVE                1
   #define CMD_BUSY                2
   #define CMD_INVALID             3
   // to Motin: Motion, GetErr
   #define CMD_MOTION              4
   #define CMD_GET_ERR             5
   #define CMD_SCANDONE            6
   AnsiString CmdSet[CMD_MAX];
   void __fastcall TfrnMain::SetCmdSet();
   int __fastcall TfrnMain::GetCmdSet(AnsiString cmd);
   int __fastcall TfrnMain::WaitYuanliCmd(int mode);
   #define CMD_ACK_WAIT              0
   #define CMD_ACK_E                 1
   #define CMD_ACK_X                 2
   #define CMD_ACK_TIMEOUT           3
   int YuanliCmdStatus[CMD_MAX];
   bool __fastcall TfrnMain::YuanLiConnect();
   void __fastcall TfrnMain::AddYuanliMessage(AnsiString msg);
   void __fastcall TfrnMain::Decode(AnsiString scmd, AnsiString *spar, int parcnt, int mode);
   void __fastcall TfrnMain::SnedSocket(AnsiString str, int mode);

   // 2022 9 2 - chc Get/Put前先將Arm收回
   bool __fastcall TfrnMain::RobotArmWithDraw();

   // 2022 9 5 - chc 雙片作業
   void __fastcall TfrnMain::DoChuckMeasure();
   // 5 點移動
   // 2022 9 12 - chc 分離全部做或分段做
   //bool __fastcall TfrnMain::ChuckMeasure();
   bool __fastcall TfrnMain::ChuckMeasure(int mode);

   void __fastcall TfrnMain::WaitTimeChuck(int no);
   void __fastcall TfrnMain::WaitTimeMacro(int no);

   // 2022 12 5 - chc Tool
   #define ST_MAIN              0
   #define ST_LEVEL             1
   #define ST_RECIPE            2
   #define ST_INSPECTION        3
   #define ST_DEVICE            4
   #define ST_TROUBLES          5
   #define ST_MAINTENANCE       6
   #define ST_MEASUREMENT       7
   #define ST_OPERATION         8
   #define ST_SIGNALTOWER       9
   #define ST_MODE              10
   #define ST_VERSION           11
   #define ST_HELP              12
   void __fastcall TfrnMain::SetST(int no, bool mode);
   // 2022 12 5 - chc Init BXFM
   bool __fastcall TfrnMain::SetupBXFM();
   void __fastcall TfrnMain::AddMainLog(AnsiString msg, int ErrorCode, bool mode);
   AnsiString MainLogFileName;
   // EFEM
   #define MAIN_LIGHT_ERR             1
   #define MAIN_LIGHT_OK              2
   #define EFEM_RESTART               3
   #define EFEM_RESTART_ERR           4
   #define EFEM_RESTART_OK            5
   // Robot
   #define ROBOT_ARM_GET_ERR          10
   #define ROBOT_ARM_PUT_ERR          11
   #define ROBOT_ORGN_ERR             12
   #define ROBOT_COM_ERR              13
   // R-Axis
   #define RAXIS_GET_ERR              20
   #define RAXIS_PUT_ERR              21
   // X/Y/Z/T-Axis
   #define XAXIS_MOVE_ERR             22
   #define YAXIS_MOVE_ERR             23
   #define ZAXIS_MOVE_ERR             24
   #define TAXIS_MOVE_ERR             25
   // Stage
   #define STAGE_MOVE_ERR             31
   #define DISABLE_COLOR              (TColor)0x00C9B3AD
   #define ENABLE_COLOR               (TColor)0x006A4E47
   // Wafer
   #define WAFER_GET_ERR              40
   #define WAFER_PUT_ERR              41
   // System
   #define EMO_ERR                    50
   #define CDA_ERR                    51
   #define VACUUM_ERR                 52
   #define POWER_ERR                  53
   #define DOOR_ERR                   54
   #define WAFER_EXIST_ERR            55
   // Aligner
   #define ALIGNER_ORGN_ERR           60
   #define ALIGNER_ALIGN_ERR          61
   #define ALIGNER_RPR_ERR            62
   #define ALIGNER_RESET_ERR          63
   #define ALIGNER_COM_ERR            64
   // None
   #define NONE_ERR                   0
   int SystemErrorCode;

   double MainImageFactor;
   long LevelStartTime,LevelStopTime,LevelElapsedms;
   short LevelStartTick,LevelStopTick;
   void __fastcall TfrnMain::CloseAllWindow();
   // Set Flag
   bool boolInInspecting;

   // 2023 2 23p - chc 分離Macro/Micro
   //int iWaitJudge;
   int iWaitMacroJudge;
   int iWaitMicroJudge;

   void __fastcall TfrnMain::InitAligner2Com();
   void __fastcall TfrnMain::UpdateAligner2Status_1();
   void __fastcall TfrnMain::UpdateAligner2Status();
   void __fastcall TfrnMain::Aligner2Button();
   void __fastcall TfrnMain::AddAligner2SendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddAligner2RecvMessage(AnsiString msg);
   void __fastcall TfrnMain::PutAligner2CodeMsg();
   bool boolRobotAck;
   bool __fastcall TfrnMain::WaitTMRobotDone(int timems);
   bool __fastcall TfrnMain::WaitTMAlignerDone(int timems);
   bool __fastcall TfrnMain::WaitTMAligner2Done(int timems);
   void __fastcall TfrnMain::MacroVacuum(bool mode);
   bool boolAllHome;
   int RobotMode;
   #define ROBOT_MODE_HIRATA            1
   #define ROBOT_MODE_TAZMO             2
   #define WAFER_TOP                0
   #define WAFER_BACK               1

   // 2023 2 24 - chc LoadPortMode
   int LoadPortMode;
   #define LOADPORT_SINGLE              1
   #define LOADPORT_DOUBLE              2

   // 2023 4 16 - chc WaferID
   bool boolDoWaferID;

   bool boolDoMacroTop,boolDoMacroBackEdge,boolDoMacroBackCenter,boolDoMicro;
   #define CCD_TOP                0
   #define CCD_LEFT               1
   #define CCD_RIGHT              2
   void __fastcall TfrnMain::SetCCDProperty();
   bool boolRFID12Status,boolRFID8Status;
   void __fastcall TfrnMain::UpdateRFID12Status();
   void __fastcall TfrnMain::UpdateRFID8Status();
   void __fastcall TfrnMain::RFID12Button();
   void __fastcall TfrnMain::RFID8Button();
   void __fastcall TfrnMain::InitRFIDCom();
   void __fastcall TfrnMain::GetRFIDCheckcode(char *buf,char *code1,char *code2);
   EROIBW8 PatternROI;
   EROIC24 CPatternROI;
   void __fastcall TfrnMain::AddRFIDSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddRFIDRecvMessage(AnsiString msg);
   EROIBW8 MatchImageROI;
   EImageBW8 StdImage;
   EImageBW8 ProcessImage;
   EROIC24 CMatchImageROI;
   EImageC24 CStdImage;
   EImageC24 CProcessImage;

   // DIC
   void __fastcall TfrnMain::AddDICSendMessage(AnsiString msg);
   void __fastcall TfrnMain::AddDICRecvMessage(AnsiString msg);
   void __fastcall TfrnMain::ReadDICParameter(int mmode);
   void __fastcall TfrnMain::InitDICCom();
   void __fastcall TfrnMain::DICButton();
   void __fastcall TfrnMain::UpdateDICStatus();

   bool __fastcall TfrnMain::RobotORGN();
   int __fastcall TfrnMain::WaferInSystem();
   bool boolAFStatus;
   bool __fastcall TfrnMain::SwitchAF(bool mode);
   void __fastcall TfrnMain::AFMoveUp(double fv);
   void __fastcall TfrnMain::AFMoveDown(double fv);
   void __fastcall TfrnMain::DICFCMove(int mode, int fc);
   void __fastcall TfrnMain::StopMotion();

   // InspectWaferNo,
   int InspectWaferNo;
   // ZP1612010_8_PSS_1_1.JPG
   // ZP1612010, 20230202_ZP1612010
   AnsiString InspectLotID,InspectLotPath;
   // C:\WV3200v\Result\20230202_ZP1612010
   AnsiString InspectDataPath;
   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01
   AnsiString InspectWaferPath;
   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01\MacroPhoto : *.jpeg
   AnsiString InspectMacroPhotoPath;
   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01\MicroPhoto : *.jpeg
   AnsiString InspectMicroPhotoPath;
   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01\Results    : .csv, *.txt
   AnsiString InspectResultsPath;
   // C:\WV3200v\Result\20230202_ZP1612010\Wafer01\MeasureData: .csv
   AnsiString InspectMeasureDataPath;
   void __fastcall TfrnMain::GeneateDefaultPath();
   int InspectDieRow,InspectDieCol,InspectDieSerialNo;
   EImageC24 ImageAry;
   // ClearWafer
   bool __fastcall TfrnMain::ClearInfo(AnsiString msg,int *cstno,int *slotno);
   bool __fastcall TfrnMain::ClearGetWafer(int stageno,int cstno,int slotno);
   bool __fastcall TfrnMain::ClearPutWafer(int stageno,int cstno,int slotno);
   bool __fastcall TfrnMain::ClearVacuumCheck(int stageno);
   bool __fastcall TfrnMain::LensCalibration(int ono,int nno);

   // 2023 2 8 - chc Tazmo: 先到Aligner
   bool boolDoubleWaferMacro;
   bool __fastcall TfrnMain::Get_ToChuck(int no);
   bool __fastcall TfrnMain::Get_FrAligner(int no);
   bool __fastcall TfrnMain::Get_ToAligner(int no);
   bool __fastcall TfrnMain::Get_FrMacro(int no);
   bool __fastcall TfrnMain::Get_ToMacro(int no);
   bool __fastcall TfrnMain::Get_FrCassette(int no, bool bask);
   bool boolDoMacroTopNext,boolDoMacroBackCenterNext,boolDoMicroNext,boolDoMacroBackEdgeNext;
   int iMacroJudgeWaferNo;
   bool __fastcall TfrnMain::Get_DoMacroJudge();

   void __fastcall TfrnMain::btnLoadWaferClick_old(TObject *Sender);
   void __fastcall TfrnMain::btnUnloadWaferClick_old(TObject *Sender);

   //2023 2 16 - chc DIC : boolDIC
   bool boolDIC;
   // 2023 2 16 - chc Check System Error
   bool __fastcall TfrnMain::CheckSystemError();
   bool __fastcall TfrnMain::DICSwitch(int mode);
   bool __fastcall TfrnMain::WaitCube();
   bool __fastcall TfrnMain::WaitFilter1();
   bool __fastcall TfrnMain::WaitFilter2();
   bool __fastcall TfrnMain::WaitDIC();
   bool boolFromJoystick;
   void __fastcall TfrnMain::ToLoadT();
   bool boolRecipeSelect;

   void __fastcall  TfrnMain::Set12EdgePosition(int no);
   void __fastcall  TfrnMain::Set8EdgePosition(int no);
   void __fastcall  TfrnMain::To12EdgePosition(int no);
   void __fastcall  TfrnMain::To8EdgePosition(int no);

   // 2023 3 21 - chc 執行Macro init
   bool boolMacroInitDone;

   // --------------------------------------------------------------------------
   // 2023 3 25 - chc SECS
   //#define ROBOT_STAGE_LP1            0
   //#define ROBOT_STAGE_LP2            1
   //#define ROBOT_STAGE_ALIGNER        2
   //#define ROBOT_STAGE_STAGE          3
   //#define ROBOT_ARM_UP               0
   //#define ROBOT_ARM_LOW              1
   #define LOADPORT_1                 0
   #define LOADPORT_2                 1
   #define ROBOT_HOME_GET             0
   #define ROBOT_HOME_PUT             1
   #define RECIPE_CHANGE              0
   #define RECIPE_CREATE              1
   #define RECIPE_MODIFY              2
   #define RECIPE_DELETE              3
   #define STATUS_FETCH               0
   #define STATUS_STORE               1
   #define STATUS_RECEIVE             2
   #define STATUS_SEND                3
   #define STATUS_PROCESSEND          4

   // 2021 10 21 - chc 由Idle/Run/Down改成Init/Idle/Setup/Ready/Execution/Pause
   //#define EQ_STATUS_IDLE             0
   //#define EQ_STATUS_RUN              1
   //#define EQ_STATUS_DOWN             2
   #define EQ_STATUS_INIT             0
   #define EQ_STATUS_IDLE             1
   #define EQ_STATUS_SETUP            2
   #define EQ_STATUS_READY            3
   #define EQ_STATUS_EXECUTING        4
   #define EQ_STATUS_PAUSE            5

   #define WAFER_EXIST_NO             0
   #define WAFER_EXIST_YES            1
   //
   #define PORT_STATUS_LR             0
   #define PORT_STATUS_LC             1
   #define PORT_STATUS_MAP            2
   #define PORT_STATUS_PROC           3
   #define PORT_STATUS_LOTEND         4
   #define PORT_STATUS_UR             5
   #define PORT_STATUS_UC             6
   #define PORT_STATUS_CSTREAD        7
   // Size
   #define ALIGNER_WAFER_SIZE_12       0
   #define ALIGNER_WAFER_SIZE_8        1
   #define ALIGNER_WAFER_SIZE_6        2
   // 2021 5 4 - chc SEC init
   void __fastcall TfrnMain::InitSECSStatus();

   // 是否送出Command
   bool boolSendSCESCommand;
   // mode: 0-未加入長度, 1-已加入長度
   void __fastcall TfrnMain::SendSECSSocket(AnsiString str, int mode);
// STAT, PortNo, PID, RC, CRCP, Stop, DateT
#define SECS_CMD_MAX               14
// to Motin: STAT, PortNo, PID, RC, CRCP, Stop, DateT
#define SECS_CMD_STAT              0
#define SECS_CMD_PTST              1
#define SECS_CMD_PIDN              2
#define SECS_CMD_RCMD              3
#define SECS_CMD_CRCP              4
#define SECS_CMD_STOP              5
#define SECS_CMD_DATE              6
// to SECS: oSTAT, oPTST, oPREV, oWIDR, oRCPC, oALRM, oSTWR
#define SECS_CMD_STAT_1            7
#define SECS_CMD_PTST_1            8
#define SECS_CMD_PREV              9
#define SECS_CMD_WIDR              10
#define SECS_CMD_RCPM              11
#define SECS_CMD_ALRM              12
#define SECS_CMD_STWR              13
   AnsiString SECSCmdSet[SECS_CMD_MAX];
   void __fastcall TfrnMain::SetSECSCmdSet();
   int __fastcall TfrnMain::GetSECSCmdSet(AnsiString cmd);
   void __fastcall TfrnMain::DecodeSECS(AnsiString scmd, AnsiString *spar, int parcnt, int mode);
   void __fastcall TfrnMain::AddSECSMessage(AnsiString msg);
   void __fastcall TfrnMain::ssSECSClientReadTest(AnsiString cmd);
   struct WAFERINFO_STRU {
      AnsiString ID;
      int Priority;
      AnsiString RecipeName;
      int Status;
   };
   struct WAFERINFO_STRU WaferBuffer[25];
   void __fastcall TfrnMain::ClearWaferBuffer();
   AnsiString sErrorCode;
         // 2021 5 5 - chc Error Control: 1-10000
         // 00 - System
         //      001 - Motion Error
         //      002 - CDA Error
         //      003 - Vacuum Error
         //      004 -
         // 01 - EFEM
         //      001 - Connect Error
         //      002 - CDA Error
         //      003 - Vacuum Error
         // 02 - Yuanli Socket
         // 03 - SECS Socket
         //      001 -
         // 04 - RFID
         //      001 - Connect Error
         //      002 - Read Error
   #define SYSTEM_ERROR_HEAD            "00"
   #define EFEM_ERROR_HEAD              "01"
   #define YUANLI_ERROR_HEAD            "02"
   #define SECS_ERROR_HEAD              "03"
   // SECS Alarm Code/Message
   int SECSAlarmCode,SECSAlarmLevel,SECSAlarmOccur;
   AnsiString SECSAlarmMessage;
   void __fastcall TfrnMain::SetSECSAlarm(int code,int resetset,int level,AnsiString mmessage);
   // 送出Status: Init/Idle/Setup/Ready/Execution/Pause
   void __fastcall TfrnMain::SendSECSStatus(int status);
   // from SECS Ack
   bool boolSECS_STAT,boolSECS_PTST,boolSECS_PREV,boolSECS_WIDR;
   bool boolSECS_RCPM,boolSECS_ALRM,boolSECS_STWR;
   bool __fastcall TfrnMain::SECSConnect();
   AnsiString WorkingWaferID;
   // RCMD: 1-Cancel, 2-Abort SECSRemoteCommand
   #define SECS_REMOTE_NONE             0
   #define SECS_REMOTE_CANCEL           1
   #define SECS_REMOTE_ABORT            2
   #define SECS_REMOTE_MAPPING          3
   int SECSRemoteCommand;
   void __fastcall TfrnMain::BlueLamp(bool mode);
   void __fastcall TfrnMain::RedLamp(bool mode);
   void __fastcall TfrnMain::GreenLamp(bool mode);
   void __fastcall TfrnMain::YellowLamp(bool mode);
   bool boolSECSSocket;
   void __fastcall TfrnMain::SetToSECS(int type);
   // --------------------------------------------------------------------------

   // 2023 3 27 - chc Inspection: 若為AutoSave則要自動Save及離開: btnMacroTopToClick
   bool __fastcall TfrnMain::DoMacroAuto();

   // 2023 4 5 - chc Inspection: 若為AutoSave則要到各點位自動Save後離開
   bool __fastcall TfrnMain::DoMicroAuto();
   // 2023 4 5 - chc Switch Macro Parameter: 區分top/Back
   void __fastcall TfrnMain::SetMacroParameter();

   // 2023 4 15 - chc Lens Switch
   bool __fastcall TfrnMain::GetLenxXY(int lensno,int *x, int *y);

   // 2023 4 18 - chc Run
   void __fastcall TfrnMain::ExecuteProgram(int mode, AnsiString programName);
   bool __fastcall TfrnMain::CheckRunning(AnsiString programTitle);

   // 2023 4 25 - chc Center Finish?  boolCenterFinish
   bool boolCenterFinish;

   // LightTower:
   //  Run:                     綠燈
   //  Run結束:                 綠轉黃燈+Beep短音+UnLoad
   //  取走CST:                 綠燈閃爍
   //  Error:                   閃紅燈
   //  PM(EMO/開門) or 停機():  紅+黃
   //  Idle:                    綠燈閃爍
   //  Init:                    閃黃燈
   //  SECS:                    藍燈
   #define TOWER_RUN             0
   #define TOWER_ENDRUN          1
   #define TOWER_ERROR           2
   #define TOWER_STOP            3
   #define TOWER_CST_REMOVE      4
   #define TOWER_IDLE            5
   #define TOWER_NONE            6
   #define TOWER_INIT            7
   int TowerType,TowerTypeOld;
   bool boolTowerBuzzer;
   bool __fastcall TfrnMain::WaitAlignerORG(int no);

   // 2023 4 29 - chc 記錄各處Wafer狀態
   bool boolWaferInChuck,boolWaferInMacroInner,boolWaferInAligner1,boolWaferInAligner2,boolWaferInRobot,boolWaferInMacroOuter;

   // 2023 5 5 - chc ExitYes
   bool ExitYes;

   // 2023 5 9 - chc Lamp Life time
   int LampLifeTime;
   int LampLifeCount;
   void __fastcall TfrnMain::UpdateLampLifeTime();

};
//---------------------------------------------------------------------------
extern PACKAGE TfrnMain *frnMain;
//---------------------------------------------------------------------------
#endif
