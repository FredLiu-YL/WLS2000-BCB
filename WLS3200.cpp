//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("frmMain.cpp", frnMain);
USEFORM("LogoFrm.cpp", frmLogo);
USEFORM("RecipeForm.cpp", frmRecipe);
USEFORM("InspectionForm.cpp", frmInspection);
USEFORM("SystemLogForm.cpp", frmSystemLog);
USEFORM("CassetteIDForm.cpp", frmCassetteID);
USEFORM("MaintenanceForm.cpp", frmMaintenance);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "WLS3200";
                 Application->CreateForm(__classid(TfrnMain), &frnMain);
                 Application->CreateForm(__classid(TfrmLogo), &frmLogo);
                 Application->CreateForm(__classid(TfrmRecipe), &frmRecipe);
                 Application->CreateForm(__classid(TfrmInspection), &frmInspection);
                 Application->CreateForm(__classid(TfrmSystemLog), &frmSystemLog);
                 Application->CreateForm(__classid(TfrmCassetteID), &frmCassetteID);
                 Application->CreateForm(__classid(TfrmMaintenance), &frmMaintenance);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
