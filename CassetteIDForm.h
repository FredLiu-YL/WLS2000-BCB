//---------------------------------------------------------------------------

#ifndef CassetteIDFormH
#define CassetteIDFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmCassetteID : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label40;
        TLabel *Label1;
        TPanel *pnlPortNo;
        TButton *btnOK;
        TButton *btnCancel;
        TPanel *Panel2;
        TImage *Image1;
        TPanel *pnlLogoUser;
        TPanel *Panel3;
        TImage *Image2;
        TImage *imHelp;
        TImage *imClose;
        TEdit *edCassetteID;
        void __fastcall btnOKClick(TObject *Sender);
        void __fastcall btnCancelClick(TObject *Sender);
        void __fastcall imCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmCassetteID(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCassetteID *frmCassetteID;
//---------------------------------------------------------------------------
#endif
