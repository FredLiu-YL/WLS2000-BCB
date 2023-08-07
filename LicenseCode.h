//---------------------------------------------------------------------------

#ifndef LicenseCodeH
#define LicenseCodeH

#include <vcl.h>
#include <stdio.h>

#define CODE_LENGTH             20
//---------------------------------------------------------------------------
class WVCodeGenerate
{
private:
   AnsiString SourceCode;
   AnsiString LicenseCode;
   bool Active;

protected:
   void __fastcall PutSourceCode();
   void __fastcall ConvertReturnCode();
   void __fastcall GenerateCode(int mode);

public:
   __fastcall WVCodeGenerate();
   void __fastcall SetCode(AnsiString code);
   AnsiString __fastcall GetCode(int mode);

};
//---------------------------------------------------------------------------
#endif
