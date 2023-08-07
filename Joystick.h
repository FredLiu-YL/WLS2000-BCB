//---------------------------------------------------------------------------

#ifndef JoystickH
#define JoystickH

//#define STRICT
//#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
//#include <commctrl.h>
//#include <basetsd.h>
//#include <dinput.h>
//#pragma link "dinput8.lib"
//#pragma link "dxguid.lib"

/*
//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
BOOL CALLBACK    EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
BOOL CALLBACK    EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
HRESULT InitDirectInput( HWND hDlg );
VOID    FreeDirectInput();
HRESULT UpdateInputState( HWND hDlg );

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

LPDIRECTINPUT8       g_pDI              = NULL;
LPDIRECTINPUTDEVICE8 g_pJoystick        = NULL;
*/

// Joystick Type
#define JOYSTICK_XBOX        0
#define JOYSTICK_GENERAL     1

   void __fastcall InitJoystick(HWND Handle);
   void __fastcall FreeJoystick();
   //HRESULT UpdateInputState( HWND hDlg );
   //void FreeDirectInput();
   // 2020 2 12 - chc 不用Handle
   //void __fastcall UpdateJoystick(HWND Handle);
   void __fastcall UpdateJoystick();

   //bool BoolJoystick;
   //int JoystickType;

   // 2020 3 18 - chc Joystick突反向問題
   void XStop(int mode);
   void YStop(int mode);
   void ZStop(int mode);
   bool __fastcall MacroOuterTCW();

   // 2022 9 15 - chc 可分是否檢查?
   //bool __fastcall MacroOuterTCCW();
   bool __fastcall MacroOuterTCCW(bool mode);

   bool __fastcall MacroOuterTRelease();
   bool __fastcall MacroInnerYMove();
   bool __fastcall MacroInnerYBack();
   bool __fastcall MacroInnerYRelease();
   bool __fastcall MacroInnerXMove();
   bool __fastcall MacroInnerXBack();
   bool __fastcall MacroInnerXRelease();
   bool __fastcall MacroInnerTMove();
   bool __fastcall MacroInnerTBack();
   bool __fastcall MacroInnerTRelease();

   // 2022 9 15 - chc ID5- DO16,17
   bool __fastcall MacroInnerYBackForce();

//---------------------------------------------------------------------------
#endif
