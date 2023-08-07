//---------------------------------------------------------------------------

#ifndef LogoFrmH
#define LogoFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "LMDButton.hpp"
#include "LMDCustomButton.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TfrmLogo : public TForm
{
__published:	// IDE-managed Components
        TTimer *tmLogo;
        TPanel *pnlLogoMain;
        TPanel *Panel1;
        TImage *imMain;
        TPanel *pnlLogoUser;
        TPanel *Panel3;
        TImage *imSystemLog;
        TImage *imHelp;
        TImage *imClose;
        TPanel *pnlStart;
        TPanel *pnlHome3;
        TPanel *pnlHome3Status;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TPanel *pnlWaferUnit;
        TPanel *pnlMacroUnit;
        TPanel *pnlMicroUnit;
        TPanel *pnlLogo1;
        TLabel *Label1;
        TLabel *Label2;
        TLMDButton *btnMainInspection;
        TLMDButton *btnMainRecipeSetup;
        TLMDButton *btnMainRecipeData;
        TLMDButton *btnMainHardwareSetup;
        TLMDButton *btnMainUserSet;
        TLMDButton *btnMainProcessResult;
        TPanel *pnlMessage;
        TLabel *Label6;
        TPanel *pnlSignalTowerUnit;
        TPanel *pnlLampLifeTime;
        TPopupMenu *pmTimeReset;
        TMenuItem *mnTimeReset;
  void __fastcall tmLogoTimer(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall imCloseClick(TObject *Sender);
        void __fastcall btnMainInspectionClick(TObject *Sender);
        void __fastcall btnMainRecipeSetupClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall pnlHome3Click(TObject *Sender);
        void __fastcall pnlLogoUserClick(TObject *Sender);
        void __fastcall btnMainHardwareSetupClick(TObject *Sender);
        void __fastcall btnMainRecipeDataClick(TObject *Sender);
        void __fastcall btnMainUserSetClick(TObject *Sender);
        void __fastcall btnMainProcessResultClick(TObject *Sender);
        void __fastcall imSystemLogClick(TObject *Sender);
        void __fastcall imHelpClick(TObject *Sender);
        void __fastcall imMainClick(TObject *Sender);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Panel1Click(TObject *Sender);
        void __fastcall mnTimeResetClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TfrmLogo(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogo *frmLogo;
//---------------------------------------------------------------------------
#endif
