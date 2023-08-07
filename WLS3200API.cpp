//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\frmMain.cpp", frnMain);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\LogoFrm.cpp", frmLogo);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\RecipeForm.cpp", frmRecipe);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\InspectionForm.cpp", frmInspection);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\SystemLogForm.cpp", frmSystemLog);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\CassetteIDForm.cpp", frmCassetteID);
USEFORM("..\..\..\工研院\EFEM\WVITRI_V11\MaintenanceForm.cpp", frmMaintenance);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "WLS3200API";
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
