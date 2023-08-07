//---------------------------------------------------------------------------

#ifndef SystemLogFormH
#define SystemLogFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmSystemLog : public TForm
{
__published:	// IDE-managed Components
        TMemo *mmMainLog;
        TPopupMenu *pmSystemMessages;
        TMenuItem *mnSaveToFile;
        TMenuItem *mnClearMessage;
        TSaveDialog *sdSystemMessages;
        TPanel *Panel1;
        TImage *Image1;
        TPanel *pnlLogoUser;
        TPanel *Panel3;
        TImage *imClose;
        void __fastcall mnSaveToFileClick(TObject *Sender);
        void __fastcall mnClearMessageClick(TObject *Sender);
        void __fastcall imCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmSystemLog(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSystemLog *frmSystemLog;
//---------------------------------------------------------------------------
#endif
