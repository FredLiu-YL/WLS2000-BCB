//---------------------------------------------------------------------------
#ifndef scA1300ThreadH
#define scA1300ThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>

#include "frmMain.h"

//---------------------------------------------------------------------------
class TscA1300 : public TThread
{
private:
protected:
        void __fastcall Execute();
public:
        __fastcall TscA1300(bool CreateSuspended, int no, AnsiString name, int type, int Width, int Height);

   // 取像Trigger
   bool boolBaslerLive;
   void __fastcall TscA1300::GigaBuffer();

   // 共有三個CCD
   int CCDNo;                                                                   // 編號
   AnsiString CCDName;                                                          // 名稱
   int CCDType;                                                                 // Type: Color(2) or BW(1)
   int CCDHeight,CCDWidth;                                                      // Size

};
//---------------------------------------------------------------------------
#endif
