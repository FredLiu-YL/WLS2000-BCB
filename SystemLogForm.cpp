//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SystemLogForm.h"
#include "frmMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSystemLog *frmSystemLog;
//---------------------------------------------------------------------------
__fastcall TfrmSystemLog::TfrmSystemLog(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemLog::mnSaveToFileClick(TObject *Sender)
{

   if(mmMainLog->Lines->Count == 0) {
      Application->MessageBox("No Data!       ","WLS3200: System Error Log",MB_ICONINFORMATION|MB_OK);
      return;
   }
   if(sdSystemMessages->Execute()) {
      mmMainLog->Lines->SaveToFile(sdSystemMessages->FileName.c_str());
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemLog::mnClearMessageClick(TObject *Sender)
{

   mmMainLog->Clear();

   if(Application->MessageBox("Delete System.log?      ", "WLS3200 File Operation", MB_YESNO|MB_SYSTEMMODAL) == IDYES) {
      AnsiString fname;
      fname = frnMain->MainLogFileName;
      // §R°£
      if(FileExists(fname)) {
         DeleteFile(fname.c_str());
      }
   }
}
//---------------------------------------------------------------------------
void __fastcall TfrmSystemLog::imCloseClick(TObject *Sender)
{

   Close();        
}
//---------------------------------------------------------------------------

