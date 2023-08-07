//---------------------------------------------------------------------------

#pragma hdrstop

#include <math.h>
#include "LicenseCode.h"

//---------------------------------------------------------------------------
__fastcall WVCodeGenerate::WVCodeGenerate()
{

   Active = false;
}
//---------------------------------------------------------------------------
void __fastcall WVCodeGenerate::SetCode(AnsiString code)
{

   SourceCode = code;
}
//---------------------------------------------------------------------------
int ReturnCode[CODE_LENGTH+11];
int sum;

AnsiString __fastcall WVCodeGenerate::GetCode(int mode)
{

   PutSourceCode();
   GenerateCode(mode);
   ConvertReturnCode();
   return LicenseCode;
}
//---------------------------------------------------------------------------
// 將SourceCode填到ReturnCode去
// 大小寫相同
void __fastcall WVCodeGenerate::PutSourceCode()
{
int length,i,j;
char c;

   length = SourceCode.Length();
   j = 1;
   sum = 0;
   for(i=0 ; i<CODE_LENGTH+11 ; i++) {
      c = SourceCode.SubString(j,1).c_str()[0];
      if(c >= 'a')
         ReturnCode[i] = (c - 'a');
      else if(c >= 'A')
         ReturnCode[i] = (c - 'A');
      else
         ReturnCode[i] = c - '0';
      if(ReturnCode[i] < 0)
         ReturnCode[i] = 11;
      j++;
      if(j > length)
         j = 1;
      sum += ReturnCode[i];
   }
}
//---------------------------------------------------------------------------
// 將ReturnCode轉成AnsiString(LicenseCode)
// 數字: 0 1 2 3 4 5 6 7 8 9  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y   Z
//                            10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35
void __fastcall WVCodeGenerate::ConvertReturnCode()
{
int length,i,code;
char c;

   LicenseCode = "";
   for(i=0 ; i<CODE_LENGTH ; i++) {
      code = ReturnCode[i];
      code = (code % 36);
      if(code >=0 && code <= 9)
         c = code + '0';
      else
         c = (code-10) + 'A';
      if((i%5) == 0 && i != 0)
         LicenseCode += '-';
      LicenseCode += c;
   }
}
//---------------------------------------------------------------------------
// 產生ReturnCode
// 1- 將前後兩碼先加再減(Loop), 取絕對值
// 2- 逐項加上Table的值
//  ==> 以MAC Address來做編碼: DD08B9CD55(SONY NB),0BAB47BBCA(光寶Encoder)
//
int CodeTable[20]      = {6,18,15,9,10,16,13,5,12,17,2,3,4,8,19,11,7,1,20,14};
int CodeTableUser[20]  = {13,5,12,1,20,17,2,3,4,6,18,15,9,10,16,8,19,11,7,14};
// input: mode - 0 Password
//               1 Server
void __fastcall WVCodeGenerate::GenerateCode(int mode)
{
int i,no;
int *buf;

   if(mode == 0)
      buf = CodeTableUser;
   else
      buf = CodeTable;

   for(i=0 ; i<CODE_LENGTH ; i++) {
      if((i % 2) == 0) {
         no = ReturnCode[i] + sum;
         no = (no % (CODE_LENGTH+11));
         ReturnCode[i] = ReturnCode[i] + ReturnCode[no];
      }
      else {
         no = ReturnCode[i] + sum*2;
         no = (no % (CODE_LENGTH+11));
         ReturnCode[i] = abs(ReturnCode[i] - ReturnCode[no]);
      }
      ReturnCode[i] += buf[i];
   }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
