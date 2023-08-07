//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "scA1300Thread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TscA1300::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

// 2015 8 19 - chc 共有三個CCD
//__fastcall TscA1300::TscA1300(bool CreateSuspended): TThread(CreateSuspended)
__fastcall TscA1300::TscA1300(bool CreateSuspended, int no, AnsiString name, int type, int Width, int Height): TThread(CreateSuspended)
{

   // 取像Trigger
   boolBaslerLive = false;
   FreeOnTerminate = true;

   // 2015 8 19 - chc 共有三個CCD
   CCDNo = no;
   CCDName = name;
   CCDType = type;
   CCDWidth = Width;
   CCDHeight = Height;

}
//---------------------------------------------------------------------------
void __fastcall TscA1300::Execute()
{

   //---- Place thread code here ----
   while(!Terminated) {
      // Live時會佔用大量CPU效能, 由10改成2
      for(int i=0 ; i<2 ; i++)
         Sleep(1);
      if(boolBaslerLive == true) {
         // Trigger Pylon取像
         // 不同CCD對應不同的程式碼
         if(CCDNo == 3)
            frnMain->GigaCapture3();
         else if(CCDNo == 2)
            frnMain->GigaCapture2();
         else
            frnMain->GigaCapture1();

         // Live時會佔用大量CPU效能, 2
         for(int i=0 ; i<2 ; i++)
            Sleep(1);

         // Move Image to Buffer
         Synchronize(GigaBuffer);
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TscA1300::GigaBuffer()
{

   // 2015 8 20 - chc 不同CCD對應不同的程式碼
   if(CCDNo == 3)
      frnMain->GigaBuffer3(CCDType);
   else if(CCDNo == 2)
      frnMain->GigaBuffer2(CCDType);
   else
      frnMain->GigaBuffer1(CCDType);
}
//---------------------------------------------------------------------------

