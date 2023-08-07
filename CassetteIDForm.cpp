//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CassetteIDForm.h"
#include "frmMain.h"
#include "RecipeForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCassetteID *frmCassetteID;
//---------------------------------------------------------------------------
__fastcall TfrmCassetteID::TfrmCassetteID(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmCassetteID::btnOKClick(TObject *Sender)
{

   frmRecipe->CassetteIDConfirm = 1;

   // 2023 3 31 - chc Set CSTID
   frmRecipe->pnlCassetteID->Caption = frmCassetteID->edCassetteID->Text;
   frmRecipe->pnlPort->Caption = frmCassetteID->pnlPortNo->Caption;

   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCassetteID::btnCancelClick(TObject *Sender)
{

   frmRecipe->CassetteIDConfirm = 2;

   // 2023 3 31 - chc Set CSTID
   frmRecipe->pnlCassetteID->Caption = "";
   frmRecipe->pnlPort->Caption = "";

   Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmCassetteID::imCloseClick(TObject *Sender)
{

   Close();
}
//---------------------------------------------------------------------------
