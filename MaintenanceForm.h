//---------------------------------------------------------------------------

#ifndef MaintenanceFormH
#define MaintenanceFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "LMDButton.hpp"
#include "LMDCustomButton.hpp"
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "LMDControl.hpp"
#include "LMDCustomBevelPanel.hpp"
#include "LMDCustomButtonGroup.hpp"
#include "LMDCustomControl.hpp"
#include "LMDCustomGroupBox.hpp"
#include "LMDCustomPanel.hpp"
#include "LMDCustomParentPanel.hpp"
#include "LMDCustomRadioGroup.hpp"
#include "LMDCustomTrackBar.hpp"
#include "LMDRadioGroup.hpp"
#include "LMDTrackBar.hpp"
#include "LMDBaseControl.hpp"
#include "LMDBaseGraphicControl.hpp"
#include "LMDBaseMeter.hpp"
#include "LMDCustomProgress.hpp"
#include "LMDGraphicControl.hpp"
#include "LMDGroupBox.hpp"
#include "LMDProgress.hpp"
#include "LMDBaseEdit.hpp"
#include "LMDCustomEdit.hpp"
#include "LMDEdit.hpp"
//---------------------------------------------------------------------------
class TfrmMaintenance : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *tsLightTower;
        TTabSheet *tsMacro;
        TTabSheet *tsMotion;
        TTabSheet *tsEFEM;
        TPanel *Panel1;
        TImage *Image1;
        TPanel *pnlMaintenanceMessage;
        TPanel *Panel3;
        TImage *Image2;
        TImage *imHelp;
        TImage *imClose;
        TPanel *Panel2;
        TPanel *Panel4;
        TPanel *Panel5;
        TPanel *Panel6;
        TCheckBox *cbID5Bit11;
        TCheckBox *cbID5Bit12;
        TCheckBox *cbID5Bit13;
        TCheckBox *cbID5Bit14;
        TCheckBox *cbID5Bit15;
        TButton *btnLightTest;
        TPanel *Panel7;
        TLMDButton *btnMacroXYHome;
        TPanel *pnlMacroXYHome;
        TLMDButton *btnMacroZ1Down;
        TPanel *pnlMacroZ1Down;
        TLMDButton *btnMacroT1Reset;
        TPanel *pnlMacroT1Reset;
        TLMDButton *btnMacroZ2Up;
        TPanel *pnlMacroZ2Up;
        TLMDButton *btnMacroZ2Home;
        TPanel *pnlMacroZ2Home;
        TLMDButton *btnMacroZ2Down;
        TPanel *pnlMacroZ2Down;
        TLMDButton *btnMacroZ1Up;
        TPanel *pnlMacroZ1Up;
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
        TLMDButton *btnMacroInnerYBack;
        TPanel *pnlMacroInnerYBack;
        TLMDButton *btnMacroInnerXBack;
        TPanel *pnlMacroInnerXBack;
        TLMDButton *btnMacroInnerTBack;
        TPanel *pnlMacroInnerTBack;
        TLMDButton *btnMacroInnerStop;
        TPanel *pnlMacroInnerStop;
        TLMDButton *btnMacroOuterStop;
        TPanel *pnlMacroOuterStop;
        TPanel *pnlDICount;
        TCheckBox *cbMacroDemoMove;
        TLMDButton *btnMacroDemo;
        TPanel *pnlMacroDemo;
        TCheckBox *cbDemoStep;
        TLabel *Label1;
        TEdit *edLoopNo;
        TLMDButton *btnInnerLoop;
        TLMDButton *btnOuterLoop;
        TPanel *pnlLoopNo;
        TCheckBox *cbMacroDemoIgnoreTimeout;
        TLMDButton *btnStop;
        TTabSheet *tsBXFM;
        TPanel *Panel8;
        TLMDRadioGroup *rgBXFMLens;
        TLMDTrackBar *tbBXFMLMP;
        TPanel *pnlBXFMLMPValue;
        TPanel *pnlBXFMLens;
        TPanel *pnlBXFMLampSet;
        TLMDRadioGroup *rgBXFMLMPSwitch;
        TPanel *pnlBXFMLMPSwitch;
        TGroupBox *GroupBox1;
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
        TCheckBox *cbChuckScan;
        TCheckBox *cbDoubleWafer;
        TGroupBox *GroupBox2;
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
        TTabSheet *tsParameter;
        TPanel *Panel9;
        TLMDGroupBox *gbMotion;
        TLabel *Label137;
        TLabel *Label138;
        TLabel *Label140;
        TLabel *Label141;
        TLabel *Label143;
        TLabel *Label121;
        TLabel *Label49;
        TLabel *Label52;
        TLabel *Label56;
        TLabel *Label58;
        TLabel *Label54;
        TLabel *Label171;
        TLabel *Label374;
        TLabel *Label375;
        TLMDEdit *edStartVelX;
        TLMDEdit *edMaxVelX;
        TLMDEdit *edTaccX;
        TLMDEdit *edTdecX;
        TLMDEdit *edHomeVelX;
        TLMDEdit *edReviewVelX;
        TLMDEdit *edReviewVelY;
        TLMDEdit *edHomeVelY;
        TLMDEdit *edTdecY;
        TLMDEdit *edTaccY;
        TLMDEdit *edMaxVelY;
        TLMDEdit *edStartVelY;
        TLMDEdit *edReviewVelT;
        TLMDEdit *edHomeVelT;
        TLMDEdit *edTdecT;
        TLMDEdit *edTaccT;
        TLMDEdit *edMaxVelT;
        TLMDEdit *edStartVelT;
        TLMDEdit *edUpLimitX;
        TLMDEdit *edUpLimitY;
        TLMDEdit *edUpLimitT;
        TLMDEdit *edReviewVelZ;
        TLMDEdit *edHomeVelZ;
        TLMDEdit *edTdecZ;
        TLMDEdit *edTaccZ;
        TLMDEdit *edMaxVelZ;
        TLMDEdit *edStartVelZ;
        TLMDEdit *edUpLimitZ;
        TLMDEdit *edMinZPosition;
        TLMDEdit *edMaxZPosition;
        TLMDButton *btnSaveMotionParameter;
        TLMDEdit *edMinTPosition;
        TLMDEdit *edMaxTPosition;
        TLMDEdit *edReviewVelR;
        TLMDEdit *edHomeVelR;
        TLMDEdit *edTdecR;
        TLMDEdit *edTaccR;
        TLMDEdit *edMaxVelR;
        TLMDEdit *edStartVelR;
        TLMDEdit *edUpLimitR;
        TLMDEdit *edMinXPosition;
        TLMDEdit *edMaxXPosition;
        TLMDEdit *edMinYPosition;
        TLMDEdit *edMaxYPosition;
        TLMDEdit *LMDEdit1;
        TLMDEdit *LMDEdit2;
        TLabel *Label339;
        TLabel *Label340;
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
        TTabSheet *tsEFEM2;
        TPanel *Panel10;
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
        TLabel *Label513;
        TLabel *Label511;
        TLabel *Label516;
        TLabel *Label510;
        TLabel *Label512;
        TLabel *Label519;
        TLMDButton *btnRobotEnable;
        TLMDButton *btnLoadPortEnable;
        TLMDButton *btnLoadPort2Enable;
        void __fastcall btnLightTestClick(TObject *Sender);
        void __fastcall cbID5Bit11Click(TObject *Sender);
        void __fastcall cbID5Bit12Click(TObject *Sender);
        void __fastcall cbID5Bit13Click(TObject *Sender);
        void __fastcall cbID5Bit14Click(TObject *Sender);
        void __fastcall cbID5Bit15Click(TObject *Sender);
        void __fastcall btnInnerLoopClick(TObject *Sender);
        void __fastcall btnMacroInnerPositionClick(TObject *Sender);
        void __fastcall btnMacroInnerStopClick(TObject *Sender);
        void __fastcall btnOuterLoopClick(TObject *Sender);
        void __fastcall btnMacroOuterPositionClick(TObject *Sender);
        void __fastcall btnMacroOuterStopClick(TObject *Sender);
        void __fastcall imCloseClick(TObject *Sender);
        void __fastcall btnMacroHomeClick(TObject *Sender);
        void __fastcall btnMacroRobotPositionClick(TObject *Sender);
        void __fastcall btnMacroInnerYMoveClick(TObject *Sender);
        void __fastcall btnMacroInnerXBackClick(TObject *Sender);
        void __fastcall btnMacroInnerTMoveClick(TObject *Sender);
        void __fastcall btnMacroOuterTCCWClick(TObject *Sender);
        void __fastcall btnMacroReadyPositionClick(TObject *Sender);
        void __fastcall btnMacroInnerYBackClick(TObject *Sender);
        void __fastcall btnMacroInnerXMoveClick(TObject *Sender);
        void __fastcall btnMacroInnerTBackClick(TObject *Sender);
        void __fastcall btnMacroOuterTCWClick(TObject *Sender);
        void __fastcall btnMacroXYHomeClick(TObject *Sender);
        void __fastcall btnMacroZ1DownClick(TObject *Sender);
        void __fastcall btnMacroT1ResetClick(TObject *Sender);
        void __fastcall btnMacroZ2UpClick(TObject *Sender);
        void __fastcall btnMacroZ2HomeClick(TObject *Sender);
        void __fastcall btnMacroZ2DownClick(TObject *Sender);
        void __fastcall btnMacroZ1UpClick(TObject *Sender);
        void __fastcall btnMacroDemoClick(TObject *Sender);
        void __fastcall btnStopClick(TObject *Sender);
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
        void __fastcall btnMacroInnerTMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCCWMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCCWMouseExit(TObject *Sender);
        void __fastcall btnMacroOuterTCCWMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerYBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerYBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerXMoveMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerXMoveMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroInnerTBackMouseExit(TObject *Sender);
        void __fastcall btnMacroInnerTBackMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCWMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMacroOuterTCWMouseExit(TObject *Sender);
        void __fastcall btnMacroOuterTCWMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall rgBXFMLensChange(TObject *Sender, int ButtonIndex);
        void __fastcall tbBXFMLMPMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall rgBXFMLMPSwitchChange(TObject *Sender,
          int ButtonIndex);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnBackwardX1Click(TObject *Sender);
        void __fastcall btnBackwardX1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardX1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardY1Click(TObject *Sender);
        void __fastcall btnBackwardY1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardY1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveY1Click(TObject *Sender);
        void __fastcall btnMoveY1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveY1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveX1Click(TObject *Sender);
        void __fastcall btnMoveX1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveX1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardZ1Click(TObject *Sender);
        void __fastcall btnBackwardZ1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardZ1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveZ1Click(TObject *Sender);
        void __fastcall btnMoveZ1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveZ1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCW1xClick(TObject *Sender);
        void __fastcall btnCW1xMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCW1xMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveR1Click(TObject *Sender);
        void __fastcall btnMoveR1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnMoveR1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardR1Click(TObject *Sender);
        void __fastcall btnBackwardR1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnBackwardR1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCCW1xClick(TObject *Sender);
        void __fastcall btnCCW1xMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall btnCCW1xMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall rgMoveStepClick(TObject *Sender);
        void __fastcall btnLoopTestClick(TObject *Sender);
        void __fastcall cbPauseClick(TObject *Sender);
        void __fastcall btnLoopTestStopClick(TObject *Sender);
        void __fastcall combRobotComportChange(TObject *Sender);
        void __fastcall combRobotBaudrateChange(TObject *Sender);
        void __fastcall btnRobotConnectClick(TObject *Sender);
        void __fastcall btnRobotCMDClick(TObject *Sender);
        void __fastcall btnRobotOriginalClick(TObject *Sender);
        void __fastcall btnRobotPosClick(TObject *Sender);
        void __fastcall btnRobotGetClick(TObject *Sender);
        void __fastcall btnRobotWithWaferClick(TObject *Sender);
        void __fastcall btnLoadPortStasClick(TObject *Sender);
        void __fastcall btnLoadPort2StasClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmMaintenance(TComponent* Owner);

   void __fastcall TfrmMaintenance::RobotButton();
   void __fastcall TfrmMaintenance::UpdateRobotStatus();
   void __fastcall TfrmMaintenance::AddRobotSendMessage(AnsiString msg);
   bool __fastcall TfrmMaintenance::WaitLoadPortDone(timems);
   bool __fastcall TfrmMaintenance::WaitLoadPort2Done(timems);
   bool __fastcall TfrmMaintenance::WaitTMRobotDone(timems);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMaintenance *frmMaintenance;
//---------------------------------------------------------------------------
#endif
