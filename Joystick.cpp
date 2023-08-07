//---------------------------------------------------------------------------

#pragma hdrstop

#include "Joystick.h"
#include "frmMain.h"

// 2020 2 13 - chc AI速度
// 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
//#include "pci_8154.h"
extern I32 StartAxisNo, FirstAxisNo, TotalAxisNo;

#define STRICT
#define DIRECTINPUT_VERSION 0x0800
//#include <windows.h>
#include <commctrl.h>
#include <basetsd.h>
#include <dinput.h>
#pragma link "dinput8.lib"
#pragma link "dxguid.lib"

// 2020 2 17 - chc Joystick停止時還要檢查其它軸
void __fastcall StopAllAxis();

bool BoolJoystick;
int JoystickType;

void __fastcall WriteDO(int idno);
extern I32 IO1_ID;
extern I32 IO2_ID;
extern I32 IO3_ID;

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
HRESULT InitDirectInput( HWND hDlg );
void    FreeDirectInput();
//HRESULT UpdateInputState( HWND hDlg );
HRESULT UpdateInputState( );

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8       g_pDI              = NULL;
LPDIRECTINPUTDEVICE8 g_pJoystick        = NULL;

//---------------------------------------------------------------------------
void __fastcall InitJoystick(HWND Handle)
{

   BoolJoystick = false;
   frnMain->boolJoystick = false;
   JoystickType = JOYSTICK_GENERAL;

   //if(FAILED(InitDirectInput(Handle))) {
   if(InitDirectInput(Handle) == S_FALSE) {
      //Application->MessageBox("Error Initializing DirectInput","DirectInput", MB_ICONERROR | MB_OK);
      frnMain->pnlJoystickStatus->Color = clSilver;
   }
   else {
      // Set a timer to go off 30 times a second. At every timer message
      // the input device will be read
      //frnMain->tmJoystick->Enabled = true;
      BoolJoystick = true;
      frnMain->pnlJoystickStatus->Color = clLime;
      frnMain->boolJoystick = true;
   }
}
//---------------------------------------------------------------------------
// JoyStick
//-----------------------------------------------------------------------------
// Name: InitDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
HRESULT InitDirectInput( HWND hDlg )
{
HRESULT hr;
static bool inputcreated=false,devices=false;

   // Register with the DirectInput subsystem and get a pointer
   // to a IDirectInput interface we can use.
   // Create a DInput object
   // => 已Create
   if(inputcreated == false) {
      if(FAILED(hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                       IID_IDirectInput8, (VOID**)&g_pDI, NULL)))
         return S_FALSE;
   }

   // Look for a simple joystick we can use for this sample program.
   // 已Create
   if(devices == false) {
      if(FAILED(hr = g_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                      EnumJoysticksCallback,
                                      NULL, DIEDFL_ATTACHEDONLY)))
      return S_FALSE;
   }

   // Make sure we got a joystick
   if(NULL == g_pJoystick ) {
      //Application->MessageBox("Joystick not found. The sample will now exit.","DirectInput Sample",MB_ICONERROR | MB_OK);
      //return S_OK;
      return S_FALSE;
   }

   // Set the data format to "simple joystick" - a predefined data format
   //
   // A data format specifies which controls on a device we are interested in,
   // and how they should be reported. This tells DInput that we will be
   // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
   if(FAILED(hr = g_pJoystick->SetDataFormat( &c_dfDIJoystick2)))
      //return hr;
      return S_FALSE;

   // Set the cooperative level to let DInput know how this device should
   // interact with the system and with other DInput applications.
   // 等級	 說明
   //  DISCL_EXCLUSIVE	 獨占，就只能自己用啦！
   //  DISCL_NONEXCLUSIVE	 共同使用，不會干擾到其他程式。
   //  DISCL_FOREGROUND	 前景讀取，需要是(Focus)情況下才能讀取資料，失焦時會自動變成無法讀取。
   //  DISCL_BACKGROUND	 不管在縮小或其他程式中，一樣可讀取資料。
   //  DISCL_NOWINKEY	 取消 Windows 商標按鍵的作用。
   //if(FAILED(hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_EXCLUSIVE |
   //                                                        DISCL_FOREGROUND)))
   if(FAILED(hr = g_pJoystick->SetCooperativeLevel( hDlg, DISCL_NONEXCLUSIVE |
                                                           DISCL_BACKGROUND)))

      //return hr;
      return S_FALSE;

   // Enumerate the joystick objects. The callback function enabled user
   // interface elements for objects that are found, and sets the min/max
   // values property for discovered axes.
   if(FAILED(hr = g_pJoystick->EnumObjects( EnumObjectsCallback,
                                           (VOID*)hDlg, DIDFT_ALL)))
      //return hr;
      return S_FALSE;

   return S_OK;
}
//---------------------------------------------------------------------------
// Free Joystick
void __fastcall FreeJoystick()
{

   //if(BoolJoystick == true) {
   if(frnMain->boolJoystick == true) {
      FreeDirectInput();
      frnMain->tmJoystick->Enabled = false;
   }
}
//---------------------------------------------------------------------------
// 2020 2 12 - chc 不用Handle
//void __fastcall UpdateJoystick(HWND Handle)
void __fastcall UpdateJoystick()
{
static bool active = false;

   if(active == true)
      return;
   active = true;
   g_pJoystick->Acquire();
   //UpdateInputState(Handle);
   UpdateInputState();
   active = false;
}
//---------------------------------------------------------------------------
// Name: UpdateInputState()
// Desc: Get the input device's state and display it.
//-----------------------------------------------------------------------------
// Scroll Bar Commands
//  #define SB_LINEUP           0
//  #define SB_LINELEFT         0
//  #define SB_LINEDOWN         1
//  #define SB_LINERIGHT        1
//  #define SB_PAGEUP           2
//  #define SB_PAGELEFT         2
//  #define SB_PAGEDOWN         3
//  #define SB_PAGERIGHT        3
//  #define SB_THUMBPOSITION    4
//  #define SB_THUMBTRACK       5
//  #define SB_TOP              6
//  #define SB_LEFT             6
//  #define SB_BOTTOM           7
//  #define SB_RIGHT            7
//  #define SB_ENDSCROLL        8
//
//winuser.h
//#define WM_INITDIALOG                   0x0110
//#define WM_COMMAND                      0x0111
//#define WM_SYSCOMMAND                   0x0112
//#define WM_TIMER                        0x0113
//#define WM_HSCROLL                      0x0114
//#define WM_VSCROLL                      0x0115
//#define WM_INITMENU                     0x0116
//#define WM_INITMENUPOPUP                0x0117
//#define WM_MENUSELECT                   0x011F
//#define WM_MENUCHAR                     0x0120
//#define WM_ENTERIDLE                    0x0121
//#define WM_CTLCOLORMSGBOX               0x0132
//#define WM_CTLCOLOREDIT                 0x0133
//#define WM_CTLCOLORLISTBOX              0x0134
//#define WM_CTLCOLORBTN                  0x0135
//#define WM_CTLCOLORDLG                  0x0136
//#define WM_CTLCOLORSCROLLBAR            0x0137
//#define WM_CTLCOLORSTATIC               0x0138
//
//#define WM_MOUSEFIRST                   0x0200
//#define WM_MOUSEMOVE                    0x0200
//#define WM_LBUTTONDOWN                  0x0201
//#define WM_LBUTTONUP                    0x0202
//#define WM_LBUTTONDBLCLK                0x0203
//#define WM_RBUTTONDOWN                  0x0204
//#define WM_RBUTTONUP                    0x0205
//#define WM_RBUTTONDBLCLK                0x0206
//#define WM_MBUTTONDOWN                  0x0207
//#define WM_MBUTTONUP                    0x0208
//#define WM_MBUTTONDBLCLK                0x0209
// 及其它
//
// 單螢幕: 在主畫面運作
// 雙螢幕: 在人員螢幕運作
//
// General Joysitck
//                   左                  右
//   前一張     XAxis(lX=-1000)          1
//   後一張     XAxis(lX=+1000)          4
//   前一頁     YAxis(lY=-1000)          2
//   後一頁     YAxis(lY=+1000)          3
//   首頁            5                   6
//   末頁            7                   8
//
// XBox Joysitck
//                   左                  右
//   前一張     XAxis(lX=-1000)          X(02)-3
//   後一張     XAxis(lX=+1000)          B(01)-2
//   前一頁     YAxis(lY=-1000)          Y(03)-4
//   後一頁     YAxis(lY=+1000)          A(00)-1
//   首頁          LB(04)-5             RB(05)-6
//   末頁         Back(06)-7           Start(07)-8
//

// 2020 3 18 - chc 移到Global
static bool booljostickmovex = false;
static bool booljostickmovey = false;
static bool booljostickmovez = false;

//HRESULT UpdateInputState( HWND hDlg )
HRESULT UpdateInputState( )
{
HRESULT     hr;
TCHAR       strText[128];                                                       // Device state text
DIJOYSTATE2 js;                                                                 // DInput joystick state
TCHAR*      str;
TPanel      *panel;
bool        pressed = false;
static bool released = false;
//TDBCtrlGrid *firstdbctrlgrid,*seconddbctrlgrid;

// 2020 2 12 - chc 記錄是否作動
// 2020 3 18 - chc 移到Global
//static bool booljostickmovex = false;
//static bool booljostickmovey = false;
//static bool booljostickmovez = false;

static bool bool9 = false;

// 2020 4 5 - chc 手動存檔使用搖桿3
static bool bool3 = false;

// 2020 2 13 - chc AISpeed
static int xspeedvalue = 0;
static int yspeedvalue = 0;
static int zspeedvalue = 0;

   if(NULL == g_pJoystick)
      return S_OK;

   // 2022 7 4 - chc 若為Macro則可以動
   if(frnMain->rgJoystickMode->ItemIndex == JOYSTICK_TABLE) {

      // 2020 4 5 - chc 若Power Off則不可動
      if(frnMain->pnlAxisError->Visible == true)
         return S_OK;

      // 2020 4 7 - chc Power off不可以動
      if(frnMain->pnlBit1->Color != clLime)
         return S_OK;

      // 2020 1 5 - chc home前不可以使用
      //if(frnMain->boolHomeDone == false) {
      //   return S_OK;
      //}
   }

   // Poll the device to read the current state
   hr = g_pJoystick->Poll();
   if(FAILED(hr)) {
      // 記錄為Joystick Alarm
      frnMain->pnlJoystickStatus->Color = clRed;
      frnMain->sbSystem->Panels->Items[6]->Text = "Joystick Disconnect!";
      // DInput is telling us that the input stream has been
      // interrupted. We aren't tracking any state between polls, so
      // we don't have any special reset that needs to be done. We
      // just re-acquire and try again.
      hr = g_pJoystick->Acquire();

      // 不再Wait, 會造成開機後初次執行系統會無法使用Joysitck, 且離開時會有Error Message
      //while(hr == DIERR_INPUTLOST)
      //    hr = g_pJoystick->Acquire();

      // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
      // may occur when the app is minimized or in the process of
      // switching, so just try again later
      return S_OK;
   }

   // Get the input's device state
   if(FAILED( hr = g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js)))
      return hr; // The device should have been acquired during the Poll()

   // 記錄為Joystick Alarm
   frnMain->pnlJoystickStatus->Color = clLime;

   // 2022 6 29 - chc Z Source, Y Direciton: JoystickZSource,JoystickYDirection
   if(frnMain->JoystickZSource == 1)
      js.lZ = js.lRz;
   if(frnMain->boolJoystickYDirection == true)
      js.lY = 0 - js.lY;
   // 2 LockZ
   if(frnMain->boolJoystick2LockZ == true) {
      if((js.rgbButtons[1] & 0x80) == 0) {
         js.lZ = 0;
      }
   }

   frnMain->pnlJX->Caption = IntToStr(js.lX);
   frnMain->pnlJY->Caption = IntToStr(js.lY);
   frnMain->pnlJZ->Caption = IntToStr(js.lZ);

   // 2020 2 12 - chc 顯示Joystick XYZ Value
   frnMain->pnlJoystickX->Caption = IntToStr(js.lX);
   frnMain->pnlJoystickY->Caption = IntToStr(js.lY);
   frnMain->pnlJoystickZ->Caption = IntToStr(js.lZ);

   // 2022 6 29 - chc fot test
   frnMain->pnlJoystickRZ->Caption = IntToStr(js.lRz);
   frnMain->pnlJoystickVZ->Caption = IntToStr(js.lVZ);
   frnMain->pnlJoystickVRZ->Caption = IntToStr(js.lVRz);
   frnMain->pnlJoystickAZ->Caption = IntToStr(js.lAZ);
   frnMain->pnlJoystickARZ->Caption = IntToStr(js.lARz);
   frnMain->pnlJoystickFZ->Caption = IntToStr(js.lFZ);
   frnMain->pnlJoystickFRZ->Caption = IntToStr(js.lFRz);
   frnMain->pnlJoystickPOV1->Caption = IntToStr(js.rgdwPOV[0]);
   frnMain->pnlJoystickPOV2->Caption = IntToStr(js.rgdwPOV[1]);
   frnMain->pnlJoystickPOV3->Caption = IntToStr(js.rgdwPOV[2]);
   frnMain->pnlJoystickPOV4->Caption = IntToStr(js.rgdwPOV[3]);

   // 2022 7 2 - chc Macro
   if(frnMain->rgJoystickMode->ItemIndex != JOYSTICK_TABLE) {
      // Inner: X/Y/Z(T)
      if(frnMain->rgJoystickMode->ItemIndex == JOYSTICK_MACRO_INNER) {
         // Y: 前後, 0/1/2 - 停/前/後
         if(js.lY <= 0-frnMain->JoystickStart) {
            if(frnMain->MacroInnerYStatus == 2)
               MacroInnerYRelease();
            MacroInnerYMove();
            frnMain->MacroInnerYStatus = 1;
         }
         else if(js.lY >= frnMain->JoystickStart) {
            if(frnMain->MacroInnerYStatus == 1)
               MacroInnerYRelease();
            MacroInnerYBack();
            frnMain->MacroInnerYStatus = 2;
         }
         else {
            if(frnMain->MacroInnerYStatus != 0) {
               MacroInnerYRelease();
               frnMain->MacroInnerYStatus = 0;
            }
         }
         // X: 左右
         if(js.lX >= frnMain->JoystickStart) {
            if(frnMain->MacroInnerXStatus == 2)
               MacroInnerXRelease();
            MacroInnerXMove();
            frnMain->MacroInnerXStatus = 1;
         }
         else if(js.lX <= 0-frnMain->JoystickStart) {
            if(frnMain->MacroInnerXStatus == 1)
               MacroInnerXRelease();
            MacroInnerXBack();
            frnMain->MacroInnerXStatus = 2;
         }
         else {
            if(frnMain->MacroInnerXStatus != 0) {
               MacroInnerXRelease();
               frnMain->MacroInnerXStatus = 0;
            }
         }
         // Z: 正反轉
         if(js.lZ >= frnMain->JoystickStart) {
            if(frnMain->MacroInnerTStatus == 2)
               MacroInnerTRelease();
            MacroInnerTMove();
            frnMain->MacroInnerTStatus = 1;
         }
         else if(js.lZ <= 0-frnMain->JoystickStart) {
            if(frnMain->MacroInnerTStatus == 1)
               MacroInnerTRelease();
            MacroInnerTBack();
            frnMain->MacroInnerTStatus = 2;
         }
         else {
            if(frnMain->MacroInnerTStatus != 0) {
               MacroInnerTRelease();
               frnMain->MacroInnerTStatus = 0;
            }
         }
      }
      // Outer
      else {
         // Y: 前後
         if(js.lY >= frnMain->JoystickStart) {
            if(frnMain->MacroOuterYStatus == 2)
               MacroOuterTRelease();
            MacroOuterTCW();
            frnMain->MacroOuterYStatus = 1;
         }
         else if(js.lY <= 0-frnMain->JoystickStart) {
            if(frnMain->MacroOuterYStatus == 1)
               MacroOuterTRelease();

            // 2022 9 15 - chc 可分是否檢查?
            //MacroOuterTCCW();
            MacroOuterTCCW(true);

            frnMain->MacroOuterYStatus = 2;
         }
         else {
            if(frnMain->MacroOuterYStatus != 0) {
               MacroOuterTRelease();
               frnMain->MacroOuterYStatus = 0;
            }
         }
      }
      return S_OK;
   }

   // 2023 2 13 - chc new Step By Step
   // 2023 2 14a - chc 先取消: 會當 => 無關
   //
   if(frnMain->cbJoystickMoveContinue->Checked == false) {
      frnMain->JoystickStart = frnMain->edJoystickStart->Text.ToInt();
      frnMain->JoystickStop = frnMain->edJoystickStop->Text.ToInt();
      if(js.lX >= frnMain->JoystickStart) {
         if(frnMain->iAxisMove[X_AXIS] == NO_MOVE)
            frnMain->btnMoveXClick(frnMain);
      }
      if(js.lX <= 0-frnMain->JoystickStart) {
         if(frnMain->iAxisMove[X_AXIS] == NO_MOVE)
            frnMain->btnBackwardXClick(frnMain);
      }
      if(js.lY >= frnMain->JoystickStart) {
         if(frnMain->iAxisMove[Y_AXIS] == NO_MOVE)
            frnMain->btnMoveYClick(frnMain);
      }
      if(js.lY <= 0-frnMain->JoystickStart) {
         if(frnMain->iAxisMove[Y_AXIS] == NO_MOVE)
            frnMain->btnBackwardYClick(frnMain);
      }
      if(js.lZ >= frnMain->JoystickStart) {
         if(frnMain->iAxisMove[Z_AXIS] == NO_MOVE)
            frnMain->btnMoveZClick(frnMain);
      }
      if(js.lZ <= 0-frnMain->JoystickStart) {
         if(frnMain->iAxisMove[Z_AXIS] == NO_MOVE)
            frnMain->btnBackwardZClick(frnMain);
      }
      goto todone;
   }
   //

   // 2020 3 24 - chc 將Motion歸0
   int limit;
   if(frnMain->cbAISpeed->Checked == true)
      limit = frnMain->edAISpeedStart->Text.ToInt();
   else
      limit = frnMain->edJoystickStop->Text.ToInt();
   if(abs(js.lX) < limit) {
      if(booljostickmovex == true) {
         frnMain->WriteSystemLog("X軸提前停止.");
         XStop(1);
      }
   }
   if(abs(js.lY) < limit) {
      if(booljostickmovey == true) {
         frnMain->WriteSystemLog("Y軸提前停止.");
         YStop(1);
      }
   }
   if(abs(js.lZ) < limit) {
      if(booljostickmovez == true) {
         frnMain->WriteSystemLog("Z軸提前停止.");
         ZStop(1);
      }
   }

   // 2020 2 14 - chc 要更新
   frnMain->JoystickStart = frnMain->edJoystickStart->Text.ToInt();
   frnMain->JoystickStop = frnMain->edJoystickStop->Text.ToInt();

   //firstdbctrlgrid = frnMain->dbcgLCDDefect;
   //seconddbctrlgrid = frnMain->dbcgLCDMatDefect;

   //
   // BcB 6.0
   //---------------------------------------------------------------------------
   // X-Axes (-)
   //---------------------------------------------------------------------------
   // 2020 3 24 - chc 每次為一軸
   if(booljostickmovey == true)
      goto toy;
   if(booljostickmovez == true)
      goto toz;

   if((frnMain->cbAISpeed->Checked == false && js.lX <= 0-frnMain->JoystickStart) ||
      (frnMain->cbAISpeed->Checked == true && js.lX <= 0-frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovex == true && frnMain->MoveDirection == 1) {
         frnMain->WriteSystemLog("-X反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題
         //goto xstop;
         XStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovey == true || booljostickmovez == true)) {
      //   frnMain->WriteSystemLog("-X(YZ動): Stop");
      //   booljostickmovey = false;
      //   booljostickmovez = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //      StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "X: Left";
      pressed = true;
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {
         booljostickmovex = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else
            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         frnMain->WriteSystemLog("X: Left");

         // 2020 2 14 - chc 設定最大速: 1mm/50mm
         if(frnMain->cbAISpeed->Checked == true) {
            xspeedvalue = frnMain->pnlJoystickX->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)10000, 1);
               I32 speed;
               speed = (1*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X-設最大速10000. " + IntToStr(xspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)500000, 1);
               I32 speed;
               speed = (50*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X-設最大速500000. " + IntToStr(xspeedvalue));
            }
         }

         frnMain->DoBackwardX();
      }

      // 2020 2 14 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovex == true && abs(xspeedvalue-js.lX) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickX->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 10000 / (1000 - base);
            //else
            //   unit = 500000 / (1000 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100) / 10000;
            //else
            //   ratio = (double)(maxvel * 100) / 500000;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) 
               speed = (1*1000) / frnMain->X_RESOLUTION;
            else
               speed = (50*1000) / frnMain->X_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(X_AXIS, ratio, 0.1);
            APS_speed_override(X_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: X-, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            xspeedvalue = value;
         }
      }

   }
   //---------------------------------------------------------------------------
   // X-Axes (+)
   //---------------------------------------------------------------------------
   else if((frnMain->cbAISpeed->Checked == false && js.lX >= frnMain->JoystickStart) ||
           (frnMain->cbAISpeed->Checked == true && js.lX >= frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovex == true && frnMain->MoveDirection == 2) {
         frnMain->WriteSystemLog("+X反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題: XStop()
         //goto xstop;
         XStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovey == true || booljostickmovez == true)) {
      //   frnMain->WriteSystemLog("+X(YZ動): Stop");
      //   booljostickmovey = false;
      //   booljostickmovez = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //     StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "X: Right";
      pressed = true;

      // 2012 2 13 - chc 也要看x
      //if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovey == false && booljostickmovez == false)) {
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {

         frnMain->WriteSystemLog("X: Right");
         booljostickmovex = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else

            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         // 2020 2 13 - chc 設定最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            xspeedvalue = frnMain->pnlJoystickX->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)10000, 1);
               I32 speed;
               speed = (1*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X+設最大速10000. " + IntToStr(xspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)500000, 1);
               I32 speed;
               speed = (50*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X+設最大速500000. " + IntToStr(xspeedvalue));
            }
         }

         frnMain->DoMoveX();
      }

      // 2020 2 13 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovex == true && abs(xspeedvalue-js.lX) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickX->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 10000 / (1000 - base);
            //else
            //   unit = 500000 / (1000 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100) / 10000;
            //else
            //   ratio = (double)(maxvel * 100) / 500000;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) 
               speed = (1*1000) / frnMain->X_RESOLUTION;
            else
               speed = (50*1000) / frnMain->X_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(X_AXIS, ratio, 0.1);
            APS_speed_override(X_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: X+, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            xspeedvalue = value;
         }
      }

   }
   // 2020 2 13 - chc AI Speed: edAISpeedStart
   //else if(abs(js.lX) <= frnMain->JoystickStop) {
   else if((frnMain->cbAISpeed->Checked == false && abs(js.lX) <= frnMain->JoystickStop) ||
           (frnMain->cbAISpeed->Checked == true && abs(js.lX) < frnMain->edAISpeedStart->Text.ToInt())) {
xstop:

      // 2020 3 26 - chc Log
      if(booljostickmovex == true) {
         frnMain->WriteSystemLog("Joystick X在停止區間.");

         XStop(1);
      }
   }

   // 2020 3 24 - chc 每次為一軸
toy:
   if(booljostickmovex == true)
      goto todone;
   if(booljostickmovez == true)
      goto toz;

   //---------------------------------------------------------------------------
   // Y-Axes (-)
   //---------------------------------------------------------------------------
   if((frnMain->cbAISpeed->Checked == false && js.lY <= 0-frnMain->JoystickStart) ||
      (frnMain->cbAISpeed->Checked == true && js.lY <= 0-frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovey == true && frnMain->MoveDirection == 2) {
         frnMain->WriteSystemLog("-Y反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題: XStop()
         //goto ystop;
         YStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == true || booljostickmovez == true)) {
      //   frnMain->WriteSystemLog("-Y(XZ動): Stop");
      //   booljostickmovex = false;
      //   booljostickmovez = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //      StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "Y: Front";
      pressed = true;

      // 2012 2 13 - chc 也要看y
      //if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovez == false)) {
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {

         frnMain->WriteSystemLog("Y: Front");
         booljostickmovey = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else

            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         // 2020 2 14 - chc 設定最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            yspeedvalue = frnMain->pnlJoystickY->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)10000, 1);
               I32 speed;
               speed = (1*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y-設最大速10000. " + IntToStr(yspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)500000, 1);
               I32 speed;
               speed = (50*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y-設最大速500000. " + IntToStr(yspeedvalue));
            }
         }

         frnMain->DoMoveY();

      }

      // 2020 2 14 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovey == true && abs(yspeedvalue-js.lY) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickY->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 10000 / (1000 - base);
            //else
            //   unit = 500000 / (1000 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100) / 10000;
            //else
            //   ratio = (double)(maxvel * 100) / 500000;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0)
               speed = (1*1000) / frnMain->Y_RESOLUTION;
            else
               speed = (50*1000) / frnMain->Y_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(Y_AXIS, ratio, 0.1);
            APS_speed_override(Y_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: Y-, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            yspeedvalue = value;
         }
      }

   }
   //---------------------------------------------------------------------------
   // Y-Axes (+)
   //---------------------------------------------------------------------------
   else if((frnMain->cbAISpeed->Checked == false && js.lY >= frnMain->JoystickStart) ||
           (frnMain->cbAISpeed->Checked == true && js.lY >= frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovey == true && frnMain->MoveDirection == 1) {
         frnMain->WriteSystemLog("+Y反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題: XStop()
         //goto ystop;
         YStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == true || booljostickmovez == true)) {
      //   frnMain->WriteSystemLog("+Y(XZ動): Stop");
      //   booljostickmovex = false;
      //   booljostickmovez = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //      StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "Y: Rear";
      pressed = true;

      // 2012 2 13 - chc 也要看y
      //if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovez == false)) {
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {

         frnMain->WriteSystemLog("Y: Rear");
         booljostickmovey = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else

            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         // 2020 2 14 - chc 設定最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            yspeedvalue = frnMain->pnlJoystickY->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)10000, 1);
               I32 speed;
               speed = (1*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y+設最大速10000. " + IntToStr(yspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)500000, 1);
               I32 speed;
               speed = (50*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y+設最大速500000. " + IntToStr(yspeedvalue));
            }
         }

         frnMain->DoBackwardY();
      }

      // 2020 2 13 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovey == true && abs(yspeedvalue-js.lY) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickY->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 10000 / (1000 - base);
            //else
            //   unit = 500000 / (1000 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100) / 10000;
            //else
            //   ratio = (double)(maxvel * 100) / 500000;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0)
               speed = (1*1000) / frnMain->Y_RESOLUTION;
            else
               speed = (50*1000) / frnMain->Y_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(Y_AXIS, ratio, 0.1);
            APS_speed_override(Y_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: Y+, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            yspeedvalue = value;
         }
      }

   }
   // 2020 2 14a - chc 取消
   //else if(abs(js.lY) <= frnMain->JoystickStop) {
   else if((frnMain->cbAISpeed->Checked == false && abs(js.lY) <= frnMain->JoystickStop) ||
           (frnMain->cbAISpeed->Checked == true && abs(js.lY) < frnMain->edAISpeedStart->Text.ToInt())) {

ystop:
      // 2020 3 26 - chc Log
      if(booljostickmovey == true) {
         frnMain->WriteSystemLog("Joystick Y在停止區間.");

         YStop(1);
      }
   }

   // 2020 3 24 - chc 每次為一軸
toz:
   if(booljostickmovex == true)
      goto todone;
   if(booljostickmovey == true)
      goto todone;

   //---------------------------------------------------------------------------
   // Z-Axes (-)
   //---------------------------------------------------------------------------
   if((frnMain->cbAISpeed->Checked == false && js.lZ <= 0-frnMain->JoystickStart) ||
      (frnMain->cbAISpeed->Checked == true && js.lZ <= 0-frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovez == true && frnMain->MoveDirection == 1) {
         frnMain->WriteSystemLog("-Z反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題: XStop()
         //goto zstop;
         ZStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == true || booljostickmovey == true)) {
      //   frnMain->WriteSystemLog("-Z(XY動): Stop");
      //   booljostickmovex = false;
      //   booljostickmovey = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //      StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "Z: Up";
      pressed = true;
      // 往上
      // 2012 2 13 - chc 也要看z
      //if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false)) {
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {

         frnMain->WriteSystemLog("Z: Up");
         booljostickmovez = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else

            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         // 2020 2 14 - chc 設定最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            zspeedvalue = frnMain->pnlJoystickZ->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)100, 1);
               I32 speed;
               speed = (0.1*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z-設最大速100. " + IntToStr(zspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)5000, 1);
               I32 speed;
               speed = (5*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z-設最大速5000. " + IntToStr(zspeedvalue));
            }
         }

         frnMain->DoBackwardZ();
      }

      // 2020 2 14 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovez == true && abs(zspeedvalue-js.lZ) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickZ->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 100.0 / (1000.0 - base);
            //else
            //   unit = 5000.0 / (1000.0 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100.0) / 100.0;
            //else
            //   ratio = (double)(maxvel * 100.0) / 5000.0;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0)
               speed = (0.1*1000) / Z_RESOLUTION;
            else
               speed = (5*1000) / Z_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(Z_AXIS, ratio, 0.1);
            APS_speed_override(Z_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: Z-, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            zspeedvalue = value;
         }
      }

   }
   //---------------------------------------------------------------------------
   // Z-Axes (+)
   //---------------------------------------------------------------------------
   else if((frnMain->cbAISpeed->Checked == false && js.lZ >= frnMain->JoystickStart) ||
           (frnMain->cbAISpeed->Checked == true && js.lZ >= frnMain->edAISpeedStart->Text.ToInt())) {

      // 2020 2 17 - chc 反向?
      if(frnMain->pnlJoystickActive->Color == clLime && booljostickmovez == true && frnMain->MoveDirection == 2) {
         frnMain->WriteSystemLog("+Z反向: Stop");

         // 2020 3 18 - chc Joystick突反向問題: XStop()
         //goto zstop;
         ZStop(0);

         // 2020 3 26 - chc 先離開
         goto todone;

      }

      // 2020 3 20 - chc 若有它軸在動一律停止
      // 2020 3 26 - chc 不可以做
      //else if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == true || booljostickmovey == true)) {
      //   frnMain->WriteSystemLog("+Z(XY動): Stop");
      //   booljostickmovex = false;
      //   booljostickmovey = false;
      //   if(frnMain->cbContinueMove->Checked == true) {
      //      frnMain->boolJogMove = false;
      //      // 不會立即停下! 要下stop
      //      StopAllAxis();
      //   }
      //   // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      //   frnMain->MoveDirection = 0;
      //}

      frnMain->sbSystem->Panels->Items[6]->Text = "Z: Down";
      pressed = true;
      // 往下
      // 2012 2 13 - chc 也要看z
      //if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false)) {
      if(frnMain->pnlJoystickActive->Color == clLime && (booljostickmovex == false && booljostickmovey == false && booljostickmovez == false)) {

         frnMain->WriteSystemLog("Z: Down");
         booljostickmovez = true;

         // 2020 3 4 - chc 低速為影像, 高速為Table
         // 2020 4 7 - chc 一律為Table
         //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
         //   frnMain->cbTableDirection->Checked = false;
         //else

            // 2023 2 17 - chc 不強制
            //frnMain->cbTableDirection->Checked = true;

         // 2020 2 14 - chc 設定最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            zspeedvalue = frnMain->pnlJoystickZ->Caption.ToInt();
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)100, 1);
               I32 speed;
               speed = (0.1*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z+設最大速100. " + IntToStr(zspeedvalue));
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)5000, 1);
               I32 speed;
               speed = (5*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z+設最大速5000. " + IntToStr(zspeedvalue));
            }
         }

         frnMain->DoMoveZ();
      }

      // 2020 2 14 - chc AISpeed
      else {
         if(frnMain->cbAISpeed->Checked == true && booljostickmovez == true && abs(zspeedvalue-js.lZ) > frnMain->edAISpeedChange->Text.ToInt()) {
            double ratio,unit;
            int value,base,maxvel;
            base = frnMain->edAISpeedStart->Text.ToInt();
            value = frnMain->pnlJoystickZ->Caption.ToInt();

            // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   unit = 100.0 / (1000.0 - base);
            //else
            //   unit = 5000.0 / (1000.0 - base);
            //maxvel = (abs(value) - base) * unit;
            //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
            //   ratio = (double)(maxvel * 100.0) / 100.0;
            //else
            //   ratio = (double)(maxvel * 100.0) / 5000.0;
            I32 speed;
            if(frnMain->rgJoystickSpeed->ItemIndex == 0)
               speed = (0.1*1000) / Z_RESOLUTION;
            else
               speed = (5*1000) / Z_RESOLUTION;
            unit = speed / (1000 - base);
            maxvel = (abs(value) - base) * unit;
            ratio = (double)(maxvel * 100) / speed;

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
            //_8154_speed_override(Z_AXIS, ratio, 0.1);
            APS_speed_override(Z_AXIS+StartAxisNo, (I32)maxvel);

            frnMain->WriteSystemLog(">Joystick: Z+, AI變速度= " + FloatToStr(maxvel) + "," + IntToStr(value) + "," + FloatToStr(ratio));
            zspeedvalue = value;
         }
      }

   }
   // 2020 2 14a - chc 取消
   //else if(abs(js.lZ) <= frnMain->JoystickStop) {
   else if((frnMain->cbAISpeed->Checked == false && abs(js.lZ) <= frnMain->JoystickStop) ||
           (frnMain->cbAISpeed->Checked == true && abs(js.lZ) < frnMain->edAISpeedStart->Text.ToInt())) {

zstop:
      // 取消
      // 2020 3 18 - chc Joystick突反向問題: XStop()
      /*
      if(booljostickmovez== true) {
         booljostickmovez= false;
         if(frnMain->cbContinueMove->Checked == true) {
            frnMain->boolJogMove = false;

            // 2020 2 17 - chc 不會立即停下! 要下stop
            StopAllAxis();

         }

         // 2020 2 14 - chc 取消最大速: 50mm
         if(frnMain->cbAISpeed->Checked == true) {
            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {
               _8154_set_max_override_speed(Z_AXIS, (F64)100, 0);
               frnMain->WriteSystemLog("AISpeed: Z設最大速100.結束");
            }
            else {
               _8154_set_max_override_speed(Z_AXIS, (F64)5000, 0);
               frnMain->WriteSystemLog("AISpeed: Z設最大速5000.結束");
            }
         }

      }
      */
      // 2020 3 26 - chc Log
      if(booljostickmovez == true) {
         frnMain->WriteSystemLog("Joystick Z在停止區間.");

         ZStop(1);
      }
   }
   //---------------------------------------------------------------------------

// 2020 3 24 - chc 每次為一軸
todone:

   // Fill up text with which buttons are pressed
   str = strText;
   for(int i=0 ; i<12 ; i++) {                                                  // 可到128個Buttoms
      if(js.rgbButtons[i] & 0x80 ) {
         //panel = (TPanel*)(frnMain->FindComponent("pnlButton" + IntToStr(i+1)));
         //panel->Color = clLime;
         frnMain->sbSystem->Panels->Items[6]->Text = "Button: " + IntToStr(i+1);

         // 2020 1 12 - chc Joystick 顯示 : pnlJoystick1
         if(i < 10) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystick" + IntToStr(i+1)));
         }
         else if(i == 10) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystickLeft"));
            frnMain->rgJoystickSpeed->ItemIndex = 0;
            frnMain->pnlSpeed->Caption = "低速";
         }
         else if(i == 11) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystickRight"));
            frnMain->rgJoystickSpeed->ItemIndex = 1;
            frnMain->pnlSpeed->Caption = "高速";
         }
         panel->Color = clLime;

         // 使用搖桿
         // 2021 4 10 - chc 指定7/8/9/10何者為Enable/Disable: JoystickEnabledKey, JoystickEnabledMode
         //if((i+1) == 9) {
         int keysel = i+1;
         if((keysel == frnMain->JoystickEnabledKey && frnMain->JoystickEnabledMode == 0) ||
            (keysel >= 7 && keysel <= 10            && frnMain->JoystickEnabledMode == 1)) {

            if(bool9 == false) {
               if(frnMain->pnlJoystickActive->Color == clSilver) {
                  // @@ Test
                  frnMain->WriteSystemLog("Joystcik: 5.Set Enable");
                  frnMain->SetJoystickActive(true);
               }
               else {
                  // @@ Test
                  frnMain->WriteSystemLog("Joystcik: 6.Set Disable");
                  frnMain->SetJoystickActive(false);
               }
               bool9 = true;
            }
         }

         // 2020 4 5 - chc 手動存檔使用搖桿3
         if((i+1) == 3) {
            if(bool3 == false) {
               if(frnMain->pnlJoystickActive->Color == clLime)

                  // 2022 6 16 - chc Null function
                  //frnMain->btnManualGetCSVClick(MainForm);
                  int no = 1;

               bool3 = true;
            }
         }

         if(released == true) {
            // XBox
            if(JoystickType == JOYSTICK_XBOX) {
               // Second: 左, 右, 上頁, 下頁, 首頁, 末頁
               /*
               if((i+1) == 3)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LINELEFT,0);
               else if((i+1) == 2)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LINERIGHT,0);
               else if((i+1) == 4)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_PAGELEFT,0);
               else if((i+1) == 1)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_PAGERIGHT,0);
               else if((i+1) == 6)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LEFT,0);
               else if((i+1) == 8)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_RIGHT,0);
               // First: 首頁, 末頁
               else if((i+1) == 5)
                  SendMessage(firstdbctrlgrid->Handle,WM_HSCROLL,SB_LEFT,0);
               else if((i+1) == 7)
                  SendMessage(firstdbctrlgrid->Handle,WM_HSCROLL,SB_RIGHT,0);
               */
            }
            else {

               // Second: 左, 右, 上頁, 下頁, 首頁, 末頁
               /*
               if((i+1) == 1)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LINELEFT,0);
               else if((i+1) == 4)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LINERIGHT,0);
               else if((i+1) == 2)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_PAGELEFT,0);
               else if((i+1) == 3)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_PAGERIGHT,0);
               else if((i+1) == 6)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_LEFT,0);
               else if((i+1) == 8)
                  SendMessage(seconddbctrlgrid->Handle,WM_HSCROLL,SB_RIGHT,0);
               // First: 首頁, 末頁
               else if((i+1) == 5)
                  SendMessage(firstdbctrlgrid->Handle,WM_HSCROLL,SB_LEFT,0);
               else if((i+1) == 7)
                  SendMessage(firstdbctrlgrid->Handle,WM_HSCROLL,SB_RIGHT,0);
               */
            }
         }
         pressed = true;
      }
      else {
         //panel = (TPanel*)(frnMain->FindComponent("pnlButton" + IntToStr(i+1)));
         //i/f(panel->Color != clSilver)
         //   panel->Color = clSilver;
         //frnMain->sbSystem->Panels->Items[6]->Text = "";

         // 2020 1 12 - chc Joystick 顯示 : pnlJoystick1
         if(i < 10) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystick" + IntToStr(i+1)));
         }
         else if(i == 10) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystickLeft"));
         }
         else if(i == 11) {
            panel = (TPanel*)(frnMain->FindComponent("pnlJoystickRight"));
         }
         panel->Color = clSilver;
         // 使用搖桿
         if((i+1) == 9)
            bool9 = false;

         // 2020 4 5 - chc 手動存檔使用搖桿3
         if((i+1) == 3)
            bool3 = false;

      }
   }
   //
   if(pressed == false) {
      frnMain->sbSystem->Panels->Items[6]->Text = "";
      released = true;
   }
   else
      released = false;

   return S_OK;
}
//-----------------------------------------------------------------------------
// Name: FreeDirectInput()
// Desc: Initialize the DirectInput variables.
//-----------------------------------------------------------------------------
void FreeDirectInput()
{

   // Unacquire the device one last time just in case
   // the app tried to exit while the device is still acquired.
   if( g_pJoystick )
      g_pJoystick->Unacquire();

   // Release any DirectInput objects.
   SAFE_RELEASE( g_pJoystick );
   SAFE_RELEASE( g_pDI );
}
//-----------------------------------------------------------------------------
// Name: EnumJoysticksCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance,
                                     VOID* pContext )
{
HRESULT hr;

   // Obtain an interface to the enumerated joystick.
   hr = g_pDI->CreateDevice( pdidInstance->guidInstance, &g_pJoystick, NULL );

   // If it failed, then we can't use this joystick. (Maybe the user unplugged
   // it while we were in the middle of enumerating it.)
   if(FAILED(hr))
      return DIENUM_CONTINUE;

   // Stop enumeration. Note: we're just taking the first joystick we get. You
   // could store all the enumerated joysticks and let the user pick.
   return DIENUM_STOP;
}
//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext )
{
HWND hDlg = (HWND)pContext;

   static int nSliderCount = 0;  // Number of returned slider controls
   static int nPOVCount = 0;     // Number of returned POV controls

   // For axes that are returned, set the DIPROP_RANGE property for the
   // enumerated axis in order to scale min/max values.
   if(pdidoi->dwType & DIDFT_AXIS) {
      DIPROPRANGE diprg;
      diprg.diph.dwSize       = sizeof(DIPROPRANGE);
      diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
      diprg.diph.dwHow        = DIPH_BYID;
      diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
      diprg.lMin              = -1000;
      diprg.lMax              = +1000;

      // Set the range for the axis
      if(FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph)))
         return DIENUM_STOP;
   }

   return DIENUM_CONTINUE;
}
//-----------------------------------------------------------------------------
// 2020 3 18 - chc Joystick突反向問題
// mode - 0: 不做速度Reset
void XStop(int mode)
{

   if(booljostickmovex == true) {
      booljostickmovex = false;
      if(frnMain->cbContinueMove->Checked == true) {
         frnMain->boolJogMove = false;

         // 2020 3 26 - chc Log
         frnMain->WriteSystemLog("XStop(): 停所有軸");
         frnMain->iAxisMove[X_AXIS] = NO_MOVE;
         frnMain->MoveDirection = 0;

         // 2020 4 7 - chc Log
         frnMain->WriteSystemLog("MoveDirection = 0 => 5");

         // 不會立即停下! 要下stop
         StopAllAxis();

         // 2020 4 7 - chc Delay
         Sleep(200);

      }
      // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      frnMain->MoveDirection = 0;

      // 2020 4 7 - chc Log
      frnMain->WriteSystemLog("MoveDirection = 0 => 6");

      // 取消最大速: 50mm
      if(mode == 1) {
         if(frnMain->cbAISpeed->Checked == true) {

            // 2020 3 26 - chc Log
            frnMain->WriteSystemLog("XStop(): 重設最大速");

            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)10000, 0);
               I32 speed;
               speed = (1*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X設最大速10000.結束");
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(X_AXIS, (F64)500000, 0);
               I32 speed;
               speed = (50*1000) / frnMain->X_RESOLUTION;
               APS_speed_override(X_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: X設最大速500000.結束");
            }
         }
      }
   }
}
//-----------------------------------------------------------------------------
// 2020 3 18 - chc Joystick突反向問題
void YStop(int mode)
{

   if(booljostickmovey == true) {
      booljostickmovey = false;
      if(frnMain->cbContinueMove->Checked == true) {
         frnMain->boolJogMove = false;

         // 2020 3 26 - chc Log
         frnMain->WriteSystemLog("YStop(): 停所有軸");
         frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
         frnMain->MoveDirection = 0;

         // 2020 4 7 - chc Log
         frnMain->WriteSystemLog("MoveDirection = 0 => 7");

         // 不會立即停下! 要下stop
         StopAllAxis();

         // 2020 4 7 - chc Delay
         Sleep(200);

      }
      // 記錄目前做連續移動中: 軸與移動方向(1/2: 正/反, 0-沒有移動)
      frnMain->MoveDirection = 0;

      // 2020 4 7 - chc Log
      frnMain->WriteSystemLog("MoveDirection = 0 => 8");

      // 2020 2 14 - chc 取消最大速: 50mm
      if(mode == 1) {
         if(frnMain->cbAISpeed->Checked == true) {

            // 2020 3 26 - chc Log
            frnMain->WriteSystemLog("YStop(): 重設最大速");

            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)10000, 0);
               I32 speed;
               speed = (1*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y設最大速10000.結束");
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Y_AXIS, (F64)500000, 0);
               I32 speed;
               speed = (50*1000) / frnMain->Y_RESOLUTION;
               APS_speed_override(Y_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Y設最大速500000.結束");
            }
         }
      }
    }
}
//-----------------------------------------------------------------------------
// 2020 3 18 - chc Joystick突反向問題
void ZStop(int mode)
{

   if(booljostickmovez == true) {
      booljostickmovez = false;
      if(frnMain->cbContinueMove->Checked == true) {
         frnMain->boolJogMove = false;

         // 2020 3 26 - chc Log
         frnMain->WriteSystemLog("ZStop(): 停所有軸");
         frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
         frnMain->MoveDirection = 0;

         // 2020 4 7 - chc Log
         frnMain->WriteSystemLog("MoveDirection = 0 => 9");

         // 不會立即停下! 要下stop
         StopAllAxis();

         // 2020 4 7 - chc Delay
         Sleep(200);

      }
      // 取消最大速: 50mm
      if(mode == 1) {
         if(frnMain->cbAISpeed->Checked == true) {

            // 2020 3 26 - chc Log
            frnMain->WriteSystemLog("ZStop(): 重設最大速");

            if(frnMain->rgJoystickSpeed->ItemIndex == 0) {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)100, 0);
               I32 speed;
               speed = (0.1*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z設最大速100.結束");
            }
            else {

               // 2022 6 16 - chc 不用8154 <= 但矽品Joystick(APS/8154)用8154! / Bruker原就用8154
               //_8154_set_max_override_speed(Z_AXIS, (F64)5000, 0);
               I32 speed;
               speed = (5*1000) / Z_RESOLUTION;
               APS_speed_override(Z_AXIS+StartAxisNo, speed);

               frnMain->WriteSystemLog("AISpeed: Z設最大速5000.結束");
            }
         }
      }
   }
}
//---------------------------------------------------------------------------
// 外環旋轉 MBR jog正轉(DO-116[ID5-DO00]), jog反轉(DO-117[ID5-DO01])
bool __fastcall MacroOuterTCW()
{

   frnMain->pnlSystemMessage->Caption = "Macro外: 前";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO00,01
      frnMain->cbID5Bit0->Checked = true;
      frnMain->cbID5Bit1->Checked = false;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2不在上位!";
      return false;
   }
   else {
      // Reset ID5 DO00,01
      frnMain->cbID5Bit0->Checked = true;
      frnMain->cbID5Bit1->Checked = false;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// 外環旋轉 MBR jog正轉(DO-116[ID5-DO00]), jog反轉(DO-117[ID5-DO01])
// 2022 9 15 - chc 可分是否檢查?
//bool __fastcall MacroOuterTCCW()
bool __fastcall MacroOuterTCCW(bool mode)
{

   frnMain->pnlSystemMessage->Caption = "Macro外: 後";

   // 2022 9 15 - chc 可分是否檢查?
   if(mode == false)
      goto doit;

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      frnMain->cbID5Bit0->Checked = false;
      frnMain->cbID5Bit1->Checked = true;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1在上位!";
      return false;
   }

   else if(frnMain->CheckMacroZ2Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2不在上位!";
      return false;
   }
   else {
doit:
      // Reset ID5 DO00,01
      frnMain->cbID5Bit0->Checked = false;
      frnMain->cbID5Bit1->Checked = true;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// 外環旋轉 MBR jog正轉(DO-116[ID5-DO00]), jog反轉(DO-117[ID5-DO01])
bool __fastcall MacroOuterTRelease()
{

   // Reset ID5 DO0,01
   frnMain->cbID5Bit0->Checked = false;
   frnMain->cbID5Bit1->Checked = false;
   WriteDO(IO3_ID);
   frnMain->pnlSystemMessage->Caption = "Macro外: 前後停";
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO16,17
bool __fastcall MacroInnerYMove()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 前";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO16,17
      frnMain->cbID5Bit16->Checked = true;
      frnMain->cbID5Bit17->Checked = false;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO16,17
      frnMain->cbID5Bit16->Checked = true;
      frnMain->cbID5Bit17->Checked = false;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO16,17
bool __fastcall MacroInnerYBack()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 後";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO16,17
      frnMain->cbID5Bit16->Checked = false;
      frnMain->cbID5Bit17->Checked = true;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO16,17
      frnMain->cbID5Bit16->Checked = false;
      frnMain->cbID5Bit17->Checked = true;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// 2022 9 15 - chc ID5- DO16,17
bool __fastcall MacroInnerYBackForce()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 後";
   // Reset ID5 DO16,17
   frnMain->cbID5Bit16->Checked = false;
   frnMain->cbID5Bit17->Checked = true;
   WriteDO(IO3_ID);
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO16,17
bool __fastcall MacroInnerYRelease()
{

   // Reset ID5 DO16,017
   frnMain->cbID5Bit16->Checked = false;
   frnMain->cbID5Bit17->Checked = false;
   WriteDO(IO3_ID);
   frnMain->pnlSystemMessage->Caption = "Macro內: 前後停";
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO22,23
bool __fastcall MacroInnerXMove()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 左";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO22,23
      frnMain->cbID5Bit22->Checked = true;
      frnMain->cbID5Bit23->Checked = false;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO22,23
      frnMain->cbID5Bit22->Checked = true;
      frnMain->cbID5Bit23->Checked = false;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO22,23
bool __fastcall MacroInnerXBack()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 右";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO22,23
      frnMain->cbID5Bit22->Checked = false;
      frnMain->cbID5Bit23->Checked = true;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO22,23
      frnMain->cbID5Bit22->Checked = false;
      frnMain->cbID5Bit23->Checked = true;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO22,23
bool __fastcall MacroInnerXRelease()
{

   // Reset ID5 DO22,23
   frnMain->cbID5Bit22->Checked = false;
   frnMain->cbID5Bit23->Checked = false;
   WriteDO(IO3_ID);
   frnMain->pnlSystemMessage->Caption = "Macro內: 左右停";
   return true;
}
//---------------------------------------------------------------------------
// 內環旋轉 MSR jog正轉(DO-122[ID5-DO06]), jog反轉(DO-123[ID5-DO07])
//	(內環升降在最上位才可操作)
bool __fastcall MacroInnerTMove()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 正轉";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO6,7
      frnMain->cbID5Bit6->Checked = true;
      frnMain->cbID5Bit7->Checked = false;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO6,7
      frnMain->cbID5Bit6->Checked = true;
      frnMain->cbID5Bit7->Checked = false;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO6,7
bool __fastcall MacroInnerTBack()
{

   frnMain->pnlSystemMessage->Caption = "Macro內: 反轉";

   // 2022 12 29 - chc Chuck Wafer? Tazmo
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
      // Reset ID5 DO6,7
      frnMain->cbID5Bit6->Checked = false;
      frnMain->cbID5Bit7->Checked = true;
      WriteDO(IO3_ID);
      return true;
   }

   if(frnMain->CheckMacroZ1Up() == false) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z1不在上位!";
      return false;
   }
   else if(frnMain->CheckMacroZ2Up() == true) {
      frnMain->pnlAlarmMessage->Caption = "Macro Z2在上位!";
      return false;
   }
   else {
      // Reset ID5 DO6,7
      frnMain->cbID5Bit6->Checked = false;
      frnMain->cbID5Bit7->Checked = true;
      WriteDO(IO3_ID);
   }
   return true;
}
//---------------------------------------------------------------------------
// ID5- DO6,7
bool __fastcall MacroInnerTRelease()
{

   // Reset ID5 DO6,7
   frnMain->cbID5Bit6->Checked = false;
   frnMain->cbID5Bit7->Checked = false;
   WriteDO(IO3_ID);
   frnMain->pnlSystemMessage->Caption = "Macro內: 正反轉停";
   return true;
}
//-----------------------------------------------------------------------------





