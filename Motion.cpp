//---------------------------------------------------------------------------

#pragma hdrstop

#include "Motion.h"
#include "frmMain.h"

// Timer����
#include <time.h>
#include <sys\timeb.h>

// 2022 12 5 - chc Logo Form
#include "LogoFrm.h"

// 2022 12 12 - chc �[�JRecipe/Inspection form
#include "InspectionForm.h"
#include "RecipeForm.h"
#include "RecipeSetForm.h"

// �]�wDelayTime
#define MOTION_SLEEP_TIME               0

// APS PCI-7856
extern bool boolMotion,boolDI,boolDO;
extern I32 BoardID, BoardName, BoardHSL, BoardMNET;
extern I32 Board7856;
extern I32 StartAxisNo, FirstAxisNo, TotalAxisNo;
extern I32 StartAxisNo2, FirstAxisNo2, TotalAxisNo2;
extern char MNET_BUS, HSL_BUS;
extern bool bool_APS_Status;
// DIO
extern bool bool_IO_Status;

// EMO
extern bool boolInEMO;

// IO1: 1, 16in/16out
// IO2: 3, 16in/16out
// IO3: 5, 32out
extern I32 IO1_ID;
extern I32 IO2_ID;
extern I32 IO3_ID;

extern bool HSL_DI_InStatus1[32];
extern bool HSL_DI_InStatus2[32];
extern bool HSL_DI_InStatus3[32];

extern bool HSL_DO_OutStatus1[32];
extern bool HSL_DO_OutStatus2[32];
extern bool HSL_DO_OutStatus3[32];

extern I32 input_DI1;
extern I32 input_DI2;
extern I32 input_DI3;

//variables definition
// DI
I16 err = 0;
U32 DIValue = 0;                                // returned DI value
// DO: 32Bits�����: Low/High/LED�T�զ@96�I

extern U32 DOValue1;                                // DO value
extern U32 DOValue2;                                // DO value
extern U32 DOValue3;                                // DO value

// Motion State
bool boolMotionState;
// ���`�b�O
int AxisErrorNo;

// 2022 8 24 - chc �O�����h����
extern bool boolInLoad;
extern bool boolInUnLoad;

// 2022 9 15 - chc �O���O�_�b��Z1 Up
extern bool boolZ1InUp;
extern bool boolZ1InDown;

// 2022 9 22 - chc DI Count
int DICount = 0;

// 2023 2 26 - chc MAX_AXIS_NO: Parameter
extern int MAX_AXIS_NO;

// 2023 3 1 - chc In Looptest
extern bool boolInLoopTest;

//---------------------------------------------------------------------------
// Init Motion - PCI-7856
//#define X_AXIS          0
//#define Y_AXIS          1
//#define Z_AXIS          2
//#define T_AXIS          3
//---------------------------------------------------------------------------
void __fastcall InitMotion()
{
U16 CardID_InBit;
I16 RET = 666;
I16 i=0;
I16 AxisNum = 0;
I16 outmode;
I16 iptmode;
I16 direction;
I16 feedback;
I32 ret;
AnsiString fname,msg;

   // Message
   frnMain->WriteSystemLog(">Motion : Init");

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("InitMotion(1): " + IntToStr(MAX_AXIS_NO));

   // EMC/Door Open Alarm Counter
   DoorCounter = -1;
   EMCCounter = -1;

   // APS
   //---------------------------------------------------------------------------
   // APS(7856)
   // �U�b�̤j�t: 5�U/--/2�d/20�U
   //---------------------------------------------------------------------------
   // Motion Status
   boolMotion = false;
   boolDI = false;
   boolDO = false;
   bool_APS_Status = true;

   // 2021 6 6 - chc �Ұ�Interrupt
   // Bit 6 : 6 5 4 3 2 1 0
   // Option to select system mode. (PCI(e)-7856 Only)
   // (0) �V Polling mode. (Not support motion interrupt)
   // (1) �V Interrupt mode. (Support motion interrupt)
   if(APS_initial(&BoardID, 0) != 0){                                           // Initial Card, CardID is assigned by system.
   //if(APS_initial(&BoardID, 0x40) != 0){                                          // Initial Card, CardID is assigned by system.

      bool_APS_Status = false;
      frnMain->WriteSystemLog(">>Motion Initial����!");
   }
   else {
      frnMain->WriteSystemLog(">>Motion Initial���\, BoardID = " + IntToStr(BoardID));
      // �n�]�w��0, Ū�^��BaordID��1!!
      //BoardHSL = BoardID;
      //BoardMNET = BoardID;
      BoardHSL = 0;
      BoardMNET = 0;

      HSL_BUS = 0;
      MNET_BUS = 1;
      FirstAxisNo = 1500;
      StartAxisNo = 1500;
      FirstAxisNo2 = 1504;
      StartAxisNo2 = 1504;

      // Start HSL(Default : 1/2/0/0)
      // 2021 8 17 - chc �LDIO�O
      //=> �]�n����
      // DIO: 16/16
      ret = APS_set_field_bus_param(BoardHSL, HSL_BUS, 0x00, 1);                        // Full Duplex
      if(ret != 0)
         frnMain->WriteSystemLog(">>APS_set_field_bus_param 0x00 Error");
      ret = APS_set_field_bus_param(BoardHSL, HSL_BUS, 0x01, 2);                        // 6Mps(3M/6M/12M)
      if(ret != 0)
         frnMain->WriteSystemLog(">>APS_set_field_bus_param 0x01 Error");
      ret = APS_set_field_bus_param(BoardHSL, HSL_BUS, 0x02, 0);
      if(ret != 0)
         frnMain->WriteSystemLog(">>APS_set_field_bus_param 0x02 Error");
      ret = APS_set_field_bus_param(BoardHSL, HSL_BUS, 0x03, 0);                        // Reset DO to Zero
      if(ret != 0)
         frnMain->WriteSystemLog(">>APS_set_field_bus_param 0x03 Error");
      ret = APS_set_field_bus_param(BoardHSL, HSL_BUS, 0x04, 7);                        // Error Count
      if(ret != 0)
         frnMain->WriteSystemLog(">>APS_set_field_bus_param 0x04 Error");
      ret = APS_start_field_bus(BoardHSL, HSL_BUS, StartAxisNo);                        // �Ұ�Bus

      // HSL Start Error
      if(ret != 0) {
         frnMain->WriteSystemLog(">>HSL_IO Start����! ret = " + IntToStr(ret) + "BoardHSL-HSLBUS-StartAxisNo: " + IntToStr(BoardHSL) + "-" + IntToStr(HSL_BUS) + "-" + IntToStr(StartAxisNo));
         bool_APS_Status = false;
         frnMain->WriteSystemLog(">>Set 7856 RED - 1");
         frnMain->pnl7856Status->Color = clRed;
         frnMain->pnlPCI7856->Color = clRed;
         frnMain->shMotionStatus->Brush->Color = clRed;
         bool_IO_Status = false;
         frnMain->pnlInStatus->Color = clRed;
         frnMain->pnlOutStatus->Color = clRed;

         // Set Error
         frnMain->AlarmStatus.boolDI = true;
         frnMain->AlarmStatus.boolDO = true;

         // 2021 8 17 - chc
         bool_APS_Status = true;
         goto domnet;

      }
      // Start MNET & Load file
      else {
         // Set Error
         frnMain->AlarmStatus.boolDI = false;
         frnMain->AlarmStatus.boolDO = false;
         frnMain->WriteSystemLog(">>HSL_BUS Start���\.");

// 2021 8 17 - chc
domnet:

         frnMain->pnlInStatus->Color = clLime;
         frnMain->pnlOutStatus->Color = clLime;
         bool_IO_Status = true;
         frnMain->pnl7856Status->Color = clLime;
         frnMain->pnlPCI7856->Color = clLime;
         frnMain->shMotionStatus->Brush->Color = clLime;
         frnMain->WriteSystemLog(">>Set APS: BoardMNET, MNET_BUS, StartAxisNo=" + IntToStr(BoardMNET) + "," + IntToStr(MNET_BUS) + "," + IntToStr(StartAxisNo));
         // �]�wPolling
         boolDI = true;
         boolDO = true;

         // IO1: 1, 16in/16out
         // IO2: 3, 16in/16out
         // IO3: 5, 32out
         IO1_ID = 1;
         IO2_ID = 3;
         IO3_ID = 5;

         if(boolDI == true && boolDO == true) {
            frnMain->tmMotionPolling->Enabled = true;
            frnMain->WriteSystemLog(">Motion : DIO ok, Polling");
         }

// 2021 8 17 - chc
//domnet:

         // Set Transfer rate
         // 0/1/2/3 - 2.5M/5M/10M/20M [10M]
         ret = APS_set_field_bus_param(BoardMNET, MNET_BUS, PRF_TRANSFER_RATE, 2);

         if(ret != 0)
            frnMain->WriteSystemLog(">>Error APS_set_field_bus_param: " + IntToStr(ret));

         frnMain->WriteSystemLog("Board,Bus,Start= " + IntToStr(BoardMNET) + ","  + IntToStr(MNET_BUS) + "," + IntToStr(StartAxisNo));

         // Start Field bus
         ret = APS_start_field_bus(BoardMNET, MNET_BUS, StartAxisNo);
         if(ret != 0) {
            frnMain->WriteSystemLog(">>MNET_BUS Start����! " + IntToStr(ret));
            bool_APS_Status = false;
         }
         else {
            // X/Y/Z/T/R
            frnMain->WriteSystemLog(">>MNET_BUS Start���\.");
            // MNET ModuleID = 0(Board ID=0, 10MHz)
            // Get First/Total Axis No
            ret = APS_get_field_bus_slave_first_axisno(BoardMNET, MNET_BUS, 0, &FirstAxisNo, &TotalAxisNo);
            if(ret != 0 || TotalAxisNo != 4 || FirstAxisNo != StartAxisNo) {
               frnMain->WriteSystemLog(">>MNET_BUS(0) First-TotalŪ������! " + IntToStr(FirstAxisNo) + "," + IntToStr(TotalAxisNo));
               bool_APS_Status = false;
            }
            else {
               frnMain->WriteSystemLog(">>MNET_BUS(0) First-TotalŪ�����\. " + IntToStr(FirstAxisNo) + ",(" + IntToStr(TotalAxisNo));
               // �]�wAxis No: 1500 ~ 1503, �����B���s��0-3
               //X_Axis = 0;
               //Y_Axis = 1;
               //Z_Axis = 2;
               //T_Axis = 3;
               I32 axisno, boardid, portid, moduleid, ret1;
               // Get Information
               ret = APS_get_axis_info(FirstAxisNo, &boardid, &axisno, &portid, &moduleid);
               msg.sprintf(">>MNET 0: First,Total,Boardid,Ansixno,Portid,Moduleid=%d,%d,%d,%d,%d,%d",FirstAxisNo,TotalAxisNo,boardid,
                                             axisno,portid,moduleid);
               frnMain->WriteSystemLog(msg);
               //2021/03/22 15:59:47:002->>MNET 0: First,Total,Boardid,Ansixno,Portid,Moduleid=1500,4,0,0,1,0
               //                                  1500  4     0       0       1      0
               //if(boardid != 0 || axisno != 0 || portid != 0 || moduleid != 0) {
               if(moduleid != 0) {
                  bool_APS_Status = false;
                  frnMain->WriteSystemLog("ModuleID=0����!");
               }
            }

            // ID1
            // Get First/Total Axis No
            // 2023 2 24 - chc �LW: LoadPortMode
            if(frnMain->LoadPortMode == LOADPORT_DOUBLE) {

               ret = APS_get_field_bus_slave_first_axisno(BoardMNET, MNET_BUS, 1, &FirstAxisNo2, &TotalAxisNo2);
               if(ret != 0 || TotalAxisNo2 != 4 || FirstAxisNo2 != StartAxisNo2) {
                  frnMain->WriteSystemLog(">>MNET_BUS(1) First-TotalŪ������! " + IntToStr(FirstAxisNo2) + "," + IntToStr(TotalAxisNo2));
                  bool_APS_Status = false;
               }
               else {
                  frnMain->WriteSystemLog(">>MNET_BUS(1) First-TotalŪ�����\. " + IntToStr(FirstAxisNo2) + ",(" + IntToStr(TotalAxisNo2) + ")");
                  // �]�wAxis No: 1500 ~ 1503, �����B���s��0-3
                  //X_Axis = 0;
                  //Y_Axis = 1;
                  //Z_Axis = 2;
                  //T_Axis = 3;
                  I32 axisno, boardid, portid, moduleid, ret1;
                  // Get Information
                  ret = APS_get_axis_info(FirstAxisNo2, &boardid, &axisno, &portid, &moduleid);
                  msg.sprintf(">>MNET 1: First,Total,Boardid,Ansixno,Portid,Moduleid=%d,%d,%d,%d,%d,%d",FirstAxisNo2,TotalAxisNo2,boardid,
                                             axisno,portid,moduleid);
                  frnMain->WriteSystemLog(msg);
                  //2021/03/22 15:59:47:002->>MNET 0: First,Total,Boardid,Ansixno,Portid,Moduleid=1500,4,0,0,1,0
                  //                                  1500  4     0       0       1      0
                  //if(boardid != 0 || axisno != 0 || portid != 0 || moduleid != 0) {
                  if(moduleid != 1) {
                     bool_APS_Status = false;
                     frnMain->WriteSystemLog("ModuleID=1����!");
                  }
               }
            }

         }
      }
   }

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("InitMotion(2): " + IntToStr(MAX_AXIS_NO));

   if(bool_APS_Status == false) {
      frnMain->WriteSystemLog(">>Register Motion Card Error!");
      frnMain->WriteSystemLog(">>Set 7856 RED - 2");
      frnMain->pnl7856Status->Color = clRed;
      frnMain->pnlPCI7856->Color = clRed;
      frnMain->shMotionStatus->Brush->Color = clRed;
      frnMain->tsMotion->Enabled = false;
      frnMain->sbSystem->Panels->Items[1]->Text = "Register Motion Card Error!";
      frnMain->AlarmStatus.boolMotion == true;
   }
   else {
      frnMain->AlarmStatus.boolMotion == false;
      boolMotion = true;
      if(frnMain->shMotionStatus->Brush->Color != clLime) {
         frnMain->shMotionStatus->Brush->Color = clLime;
      }
      if(frnMain->pnlMotionReady->Color != clLime) {
         frnMain->pnlMotionReady->Color = clLime;
      }
      frnMain->tsMotion->Enabled = true;
      frnMain->WriteSystemLog(">>Register Motion Card Ok.");
      frnMain->pnl7856Status->Color = clLime;
      frnMain->pnlPCI7856->Color = clLime;
      frnMain->shMotionStatus->Brush->Color = clLime;
      frnMain->sbSystem->Panels->Items[1]->Text = "Register Motion Card Ok.";
      // Load �Ѽ���(�|��IPT_MODE/OPT_MODE/IPT_LOGIC����...)
      fname = frnMain->SystemDirectory + "\\Motion.xml";
      ret = APS_load_param_from_file(fname.c_str());
      // �T�{�O�_��Ū��?
      if(ret != ERR_NoError) {
         Application->MessageBox((AnsiString("XML��Ū������!") + fname + ":" + IntToStr(ret)).c_str(),"XML��Ū���@�~    ",MB_OK);

         // 2023 2 24 - chc Log
         frnMain->WriteSystemLog(">>XML file: " + fname);

      }

      // Load��n��Set: IPT_MODE/OPT_MODE/IPT_LOGIC��Load�J�t��
      // ����2.8���ᤣ���A��Set�ʧ@, Load�N�|�N�ѼƼg�J
      //SetMotionParameter();
      // �U�b�n�]�wINP Condition(MDN CONDI; Motion done condition) - �i�bXML�ɤ��]�w, ���ε{���U�R�O
      // 0: Control command done (default)
      // 1: Command done with INP
      //APS_set_axis_param(axisno, PRA_MDN_CONDI, 1);

   }
   //---------------------------------------------------------------------------
   frnMain->WriteSystemLog(">Motion Completed.");

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("InitMotion(3): " + IntToStr(MAX_AXIS_NO));

}
//---------------------------------------------------------------------------
// 2023 2 26 - chc MAX_AXIS_NO: Parameter
//I16 MotionStatusCode[MAX_AXIS_NO];
I16 MotionStatusCode[MAX_AXIS_NO_DEFAULT];

// ��sMotion Status
void __fastcall UpdateMotionStatus()
{
I16 Motion_status;
AnsiString str;
U16 IO_status;
I32 ret;

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   // 2022 9 23 - chc Display
   frnMain->pnlMotionStatus->Color = clLime;
   frnMain->pnlMotionStatus->Refresh();

   // 2021 6 15 - chc New
   // �O���O�_�bUpdate?
   frnMain->boolInMotionUpdate = true;

   // 2022 9 23 - chc Log
   //frnMain->WriteSystemLog("DI��sEnter...");

   // 2022 9 23 - chc �O���O�_�b��Z1 Down
   //if(boolZ1InUp == true || boolZ1InDown == true) {
   //   frnMain->WriteSystemLog("DI��s(Z1 Up,Down)Enter...");
   //}

   // ��sI/O Status: Ū��DIO - 7856 + DIO Board
   // 2021 8 17 - chc �LDIO
   // ID1: 16in/16out
   if(bool_IO_Status == true) {

      // 2022 9 22 - chc DI Count
      DICount++;
      if(DICount > 10000)
         DICount = 1;
      frnMain->pnlDICount->Caption = IntToStr(DICount);
      frnMain->pnlDICount->Refresh();

      // 2022 9 23 - chc �O���O�_�b��Z1 Down
      // 2023 2 18 - chc ��Log
      //if(boolZ1InUp == true || boolZ1InDown == true) {
      //   frnMain->WriteSystemLog("DI��s(Z1 Up,Down)...");
      //}

      // Digital Input, PCI-7856(APS)
      // ��sDI - 32bits
      //int hslret = APS_get_field_bus_d_input(BoardHSL, HSL_BUS, DI_ID, &input_DI);
      // 2022 6 29 - chc ����! ���~
      //I32 ret = APS_field_bus_d_get_input(BoardMNET, MNET_BUS, 0, &input_DI);
      ret = APS_get_field_bus_d_input(BoardHSL, HSL_BUS, IO1_ID, &input_DI1);

      if(ret == ERR_NoError) {

         // 2022 9 15 - chc �O���O�_�b��Z1 Up
         // 2023 2 18 - chc ��Log
         //if(boolZ1InUp == true)
         //   frnMain->WriteSystemLog("DI��s(Z1 Up): DI1 Ok.");

         // 2022 9 23 - chc �O���O�_�b��Z1 Down
         // 2023 2 18 - chc ��Log
         //if(boolZ1InDown == true)
         //   frnMain->WriteSystemLog("DI��s(Z1 Down): DI1 Ok.");

         unsigned int bit;
         TPanel *panel;
         bit = 1;

         // 16bit
         for(int no=0 ; no<16 ; no++) {
            panel = (TPanel *)(frnMain->FindComponent("pnlBit" + IntToStr(no)));
            if((input_DI1 & bit) != 0) {
               panel->Color = clLime;

               // 2022 9 22 - chc Refresh
               panel->Refresh();

               HSL_DI_InStatus1[no] = true;

               // EMO
               if(no == 0) {
                  if(frnMain->AlarmStatus.boolEMO == true)
                     frnMain->AddMainLog("EMO Released.", EMO_ERR,false);
                  frnMain->AlarmStatus.boolEMO = false;
                  if(frnMain->pnlEMO->Visible == true) {
                     frnMain->pnlEMO->Visible = false;
                  }
               }
               // Power
               if(no == 1) {
                  if(frnMain->AlarmStatus.boolPower == true && frnMain->AlarmStatus.boolInDoor == false && frnMain->AlarmStatus.boolLPLight == false) {
                     if(frnMain->pnlError->Visible == true) {
                        frnMain->pnlError->Visible = false;
                        frnMain->pnlAlarm->Caption = "";
                     }
                  }
                  if(frnMain->AlarmStatus.boolPower == true)
                     frnMain->AddMainLog("Power Released.", POWER_ERR,false);
                  frnMain->AlarmStatus.boolPower = false;
               }
               // �J�ƪ�: Interlock
               if(no == 2) {
                  if(frnMain->AlarmStatus.boolInDoor == true && frnMain->AlarmStatus.boolPower == false && frnMain->AlarmStatus.boolLPLight == false) {
                     if(frnMain->pnlError->Visible == true) {
                        frnMain->pnlError->Visible = false;
                        frnMain->pnlAlarm->Caption = "";
                     }
                  }
                  if(frnMain->AlarmStatus.boolInDoor == true)
                     frnMain->AddMainLog("Door Released.", MAIN_LIGHT_ERR,false);
                  frnMain->AlarmStatus.boolInDoor = false;
                  frnMain->pnlInDoor->Color = clLime;
               }
               // CDA
               if(no == 3) {
                  frnMain->AlarmStatus.boolCDA = false;
               }
               // Vacuum
               if(no == 4) {
                  frnMain->AlarmStatus.boolVacuum = false;
               }

               // 2022 8 2 - chc System Status: pnlSystemCDAVacuum
               if(frnMain->AlarmStatus.boolEMO == false && frnMain->AlarmStatus.boolPower == false &&
                  frnMain->AlarmStatus.boolCDA == false && frnMain->AlarmStatus.boolVacuum == false)
                  frnMain->pnlSystemCDAVacuum->Color = clLime;

               // EMO / Power
               if(boolInEMO == true) {
                  if(frnMain->AlarmStatus.boolEMO == false && frnMain->AlarmStatus.boolPower == false &&
                     frnMain->pnlID3Bit15->Color == clSilver) {
                     boolInEMO = false;
                     frmLogo->pnlHome3->Font->Color = clWhite;
                     frmLogo->pnlLogo1->Visible = false;
                     frmLogo->pnlHome3->Visible = true;
                     frmLogo->pnlHome3->Enabled = true;
                  }
               }

            }
            else {
               panel->Color = clSilver;

               // 2022 9 22 - chc Refresh
               panel->Refresh();

               HSL_DI_InStatus1[no] = false;

               // EMO
               if(no == 0) {
                  frnMain->AlarmStatus.boolEMO = true;

                  // 2023 4 27 - chc Light Tower
                  if(frnMain->TowerType != TOWER_STOP) {
                     frnMain->TowerType = TOWER_STOP;
                     frnMain->WriteSystemLog("UpdateMotionStatus: TOWER_STOP");
                  }

               }
               // Power
               if(no == 1) {
                  if(frnMain->AlarmStatus.boolPower == false) {
                     frnMain->AddMainLog("Power Error!", POWER_ERR,true);
                  }
                  frnMain->AlarmStatus.boolPower = true;
               }
               // �J�ƪ�
               if(no == 2) {
                  if(frnMain->AlarmStatus.boolInDoor == false) {
                     frnMain->AddMainLog("Door Error!", DOOR_ERR,true);
                  }
                  frnMain->AlarmStatus.boolInDoor = true;
                  frnMain->pnlInDoor->Color = clSilver;
               }

               // CDA
               if(no == 3) {

                  // 2022 12 13 - chc CDA Alarm
                  if(frnMain->AlarmStatus.boolCDA == false) {
                     frnMain->AddMainLog("CDA Error!", CDA_ERR,true);
                  }

                  frnMain->AlarmStatus.boolCDA = true;
               }
               // Vacuum
               if(no == 4) {

                  // 2023 2 3 - chc Vacuum�ɤ�����
                  if(frnMain->cbBit0->Checked == false && frnMain->cbBit1->Checked == false &&
                     frnMain->cbBit2->Checked == false && frnMain->cbBit3->Checked == false) {

                     // 2022 12 13 - chc Vacuum Alarm
                     if(frnMain->AlarmStatus.boolVacuum == false) {
                        frnMain->AddMainLog("Vacuum Error!", VACUUM_ERR,true);
                     }

                     frnMain->AlarmStatus.boolVacuum = true;
                  }
               }

               // 2022 8 2 - chc System Status: pnlSystemCDAVacuum
               if(frnMain->AlarmStatus.boolEMO == true || frnMain->AlarmStatus.boolPower == true ||
                  frnMain->AlarmStatus.boolCDA == true || frnMain->AlarmStatus.boolVacuum == true)
                  frnMain->pnlSystemCDAVacuum->Color = clRed;

               // EMO / Power
               if(no == 0 || no == 1) {
                  if(boolInEMO == false) {
                     boolInEMO = true;
                     frnMain->CloseAllWindow();
                  }
               }

            }
            bit = bit << 1;
         }

      }

      // 2022 9 23 - chc Else
      else {
         frnMain->WriteSystemLog("****DI1 Device Error!");
      }

      // 2022 9 15 - chc �O���O�_�b��Z1 Up
      // 2023 2 18 - chc ��Log
      //if(boolZ1InUp == true)
      //   frnMain->WriteSystemLog("DI��s(Z1 Up): DI2...");

      // 2022 9 23 - chc �O���O�_�b��Z1 Down
      // 2023 2 18 - chc ��Log
      //if(boolZ1InDown == true)
      //   frnMain->WriteSystemLog("DI��s(Z1 Down): DI2...");

      // ID3: 16in/16out
      ret = APS_get_field_bus_d_input(BoardHSL, HSL_BUS, IO2_ID, &input_DI2);
      if(ret == ERR_NoError) {

         // 2022 9 15 - chc �O���O�_�b��Z1 Up
         // 2023 2 18 - chc ��Log
         //if(boolZ1InUp == true)
         //   frnMain->WriteSystemLog("DI��s(Z1 Up): DI2 Ok.");

         // 2022 9 23 - chc �O���O�_�b��Z1 Down
         // 2023 2 18 - chc ��Log
         //if(boolZ1InDown == true)
         //   frnMain->WriteSystemLog("DI��s(Z1 Down): DI2 Ok.");

         unsigned int bit;
         TPanel *panel;
         bit = 1;
         // 16bit
         for(int no=0 ; no<16 ; no++) {
            panel = (TPanel *)(frnMain->FindComponent("pnlID3Bit" + IntToStr(no)));
            if((input_DI2 & bit) != 0) {

               // 2022 12 5 - chc ���]: On -> Off�n�B�z
               // On: �J�I
               if(no == 15) {
                  if(boolInEMO == false) {

                     // 2023 3 1 - chc Looptest���i���w���ˬd���]
                     if(boolInLoopTest == true && frnMain->cbCheckLightBar->Checked == true) {
                        frnMain->pnlAlarm->Caption = "Safety Area Error(Looptest)!";
                        frnMain->pnlEFEMReady->Color = clRed;
                        frnMain->WriteSystemLog("���]�I�J(Looptest)!");
                        Beep(500);
                     }
                     else {

                        frnMain->AlarmStatus.boolLPLight = true;
                        if(panel->Color != clLime) {
                           frnMain->pnlAlarmMessage->Caption = "���]�I�J!";
                           frnMain->pnlEFEMReady->Color = clRed;
                           frnMain->WriteSystemLog("���]�I�J!");
                           Beep(500);
                           frnMain->AddMainLog("Safety Area Error!", MAIN_LIGHT_ERR,true);
                           // => On: Stage XYZW�n����, Aligner�nStop
                           // => On: LP/Robot�_�q: �n��RS232���}�l��
                           frnMain->pcSystem->Enabled = false;
                           frnMain->pnlEFEM->Enabled = false;
                           // �j���
                           bool boolstop = false;
                           if(frnMain->iAxisMove[X_AXIS] != NO_MOVE) {
                              frnMain->WriteSystemLog("X�b�|������! �j���");
                              frnMain->iAxisMove[X_AXIS] = NO_MOVE;
                              boolstop = true;
                           }
                           if(frnMain->iAxisMove[Y_AXIS] != NO_MOVE) {
                              frnMain->WriteSystemLog("Y�b�|������! �j���");
                              frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
                              boolstop = true;
                           }
                           if(frnMain->iAxisMove[Z_AXIS] != NO_MOVE) {
                              frnMain->WriteSystemLog("Z�b�|������! �j���");
                              frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
                              boolstop = true;
                           }
                           if(frnMain->iAxisMove[T_AXIS] != NO_MOVE) {
                              frnMain->WriteSystemLog("T�b�|������! �j���");
                              frnMain->iAxisMove[T_AXIS] = NO_MOVE;
                              boolstop = true;
                           }
                           if(boolstop == true)
                              StopAllAxis();
                        }
                     }
                  }
               }

               panel->Color = clLime;
               panel->Refresh();
               HSL_DI_InStatus2[no] = true;
            }
            else {

               // 2022 12 5 - chc ���]: On -> Off�n�B�z
               // Off: �D�J�I
               if(no == 15) {
                  if(boolInEMO == false) {

                     if(frnMain->AlarmStatus.boolLPLight == true && frnMain->AlarmStatus.boolPower == false && frnMain->AlarmStatus.boolInDoor == false) {
                        if(frnMain->pnlError->Visible == true) {
                           frnMain->pnlError->Visible = false;
                           frnMain->pnlAlarm->Caption = "";
                        }
                     }
                     if(frnMain->AlarmStatus.boolLPLight == true)
                        frnMain->AddMainLog("Safety Area Ok.", MAIN_LIGHT_OK,false);

                     frnMain->AlarmStatus.boolLPLight = false;
                     if(panel->Color == clLime) {
                        frnMain->WriteSystemLog("���]�I�J�Ѱ�.");
                        if(frnMain->pnlAlarmMessage->Caption == "���]�I�J!")
                           frnMain->pnlAlarmMessage->Caption = "";
                        if(frnMain->pnlAlarm->Caption == "Safety Area Error")
                           frnMain->pnlAlarm->Caption = "";
                        // EFEM Init
                        // => On: LP/Robot�_�q, off: �n��RS232���}�l��
                        // Full or LP+Robot
                        // 2023 2 10 - chc WLS3200�~�n��
                        if(frnMain->RobotMode == ROBOT_MODE_TAZMO) {
                           frnMain->pnlEFEMReady->Color = clLime;
                           frnMain->pcRobot->Enabled = true;
                           frnMain->pnlSystemMessage->Caption = "EFEM���`! �t��Ready.";
                           frnMain->pcSystem->Enabled = true;
                           frnMain->pnlEFEM->Enabled = true;
                        }
                        else {

                           if(Application->MessageBox("���]�I�J�ƥ�Ѱ�, ����EFEM���Ұ�?      ", "EFEM Init�@�~", MB_YESNO|MB_SYSTEMMODAL) == IDYES) {

                              frnMain->WriteSystemLog("SetupEFEM(1): UpdateMotionStatus() Enter...");

                              if(frnMain->SetupEFEM(2) == false) {
                                 frnMain->pnlEFEMReady->Color = clRed;
                                 frnMain->pcRobot->Enabled = false;
                                 frnMain->pnlSystemMessage->Caption = "EFEM���`! �L�k�@�~.";
                                 frnMain->pnlAlarmMessage->Caption = "EFEM���`! �L�k�@�~.";
                              }
                              else {
                                 frnMain->pnlEFEMReady->Color = clLime;
                                 frnMain->pcRobot->Enabled = true;
                                 frnMain->pnlSystemMessage->Caption = "EFEM���`! �t��Ready.";
                              }
                              frnMain->pcSystem->Enabled = true;
                              frnMain->pnlEFEM->Enabled = true;
                           }
                        }
                     }
                  }
               }

               panel->Color = clSilver;
               panel->Refresh();
               HSL_DI_InStatus2[no] = false;
            }
            bit = bit << 1;
         }
      }

      // 2022 9 15 - chc �O���O�_�b��Z1 Up
      else {

         // 2022 9 23 - chc Else
         frnMain->WriteSystemLog("****DI2 Device Error!");

         if(boolZ1InUp == true)
            frnMain->WriteSystemLog("DI��s(Z1 Up): DI2 Fail!");

         // 2022 9 23 - chc �O���O�_�b��Z1 Down
         if(boolZ1InDown == true)
            frnMain->WriteSystemLog("DI��s(Z1 Down): DI2 Fail!");

      }

   }

   // 2022 9 23 - chc Else
   else {
      if(frnMain->boolForTest == false)
         frnMain->WriteSystemLog("****DIO Device Error!");
   }

   // 2022 9 23 - chc Log
   //frnMain->WriteSystemLog("DI��sEnter DI Done.");

   // ��Delay
   // 2022 9 23 - chc �����ݬ�
   //Sleep(1);

   // 2022 9 22a - chc �����ݬ�
   // �i��|�d��, �����U����!
   //Application->ProcessMessages();

   // �b��: X/Y/Z/T/R
   // 2023 2 26 - chc MAX_AXIS_NO: Parameter
   //TLMDEdit *counterarray[MAX_AXIS_NO]    = {frnMain->edCounterX,       frnMain->edCounterY,       frnMain->edCounterZ,       frnMain->edCounterT,        frnMain->edCounterR};
   //TLMDEdit *commandarray[MAX_AXIS_NO]    = {frnMain->edCommandX,       frnMain->edCommandY,       frnMain->edCommandZ,       frnMain->edCommandT,        frnMain->edCommandR};
   //TPanel *motionstatusarray[MAX_AXIS_NO] = {frnMain->pnlMotionStatusX, frnMain->pnlMotionStatusY, frnMain->pnlMotionStatusZ, frnMain->pnlMotionStatusT,  frnMain->pnlMotionStatusR};
   //TPanel *iostatusarray[MAX_AXIS_NO]     = {frnMain->pnlIOStatusX,     frnMain->pnlIOStatusY,     frnMain->pnlIOStatusZ,     frnMain->pnlIOStatusT,      frnMain->pnlIOStatusR};
   TLMDEdit *counterarray[MAX_AXIS_NO_DEFAULT]    = {frnMain->edCounterX,       frnMain->edCounterY,       frnMain->edCounterZ,       frnMain->edCounterT,        frnMain->edCounterR};
   TLMDEdit *commandarray[MAX_AXIS_NO_DEFAULT]    = {frnMain->edCommandX,       frnMain->edCommandY,       frnMain->edCommandZ,       frnMain->edCommandT,        frnMain->edCommandR};
   TPanel *motionstatusarray[MAX_AXIS_NO_DEFAULT] = {frnMain->pnlMotionStatusX, frnMain->pnlMotionStatusY, frnMain->pnlMotionStatusZ, frnMain->pnlMotionStatusT,  frnMain->pnlMotionStatusR};
   TPanel *iostatusarray[MAX_AXIS_NO_DEFAULT]     = {frnMain->pnlIOStatusX,     frnMain->pnlIOStatusY,     frnMain->pnlIOStatusZ,     frnMain->pnlIOStatusT,      frnMain->pnlIOStatusR};

   I32 ipos,istatus;
   for(int axisno=0 ; axisno<MAX_AXIS_NO ; axisno++) {
      // Position
      APS_get_position(axisno+StartAxisNo,&ipos);
      str = IntToStr(ipos);
      counterarray[axisno]->Text = str;
      if(axisno == X_AXIS) {
         frnMain->pnlX->Caption = str;

         // 2022 12 28 - chc Stop Capture
         if(frmInspection->Visible == true && frmInspection->pcInspection->ActivePage == frmInspection->tsMicroInspection)
            frmInspection->pnlX->Caption = str;
         if(frmRecipeSet->Visible == true && frmRecipeSet->pcRecipeSet->ActivePage == frmRecipeSet->tsWaferInformation &&
            frmRecipeSet->pcWaferInformation->ActivePage == frmRecipeSet->tsMicro)
            frmRecipeSet->pnlX->Caption = str;

      }
      else if(axisno == Y_AXIS) {
         frnMain->pnlY->Caption = str;

         // 2022 12 28 - chc Stop Capture
         if(frmInspection->Visible == true && frmInspection->pcInspection->ActivePage == frmInspection->tsMicroInspection)
            frmInspection->pnlY->Caption = str;
         if(frmRecipeSet->Visible == true && frmRecipeSet->pcRecipeSet->ActivePage == frmRecipeSet->tsWaferInformation &&
            frmRecipeSet->pcWaferInformation->ActivePage == frmRecipeSet->tsMicro)
            frmRecipeSet->pnlY->Caption = str;

      }
      else if(axisno == Z_AXIS)
         frnMain->pnlZ->Caption = str;
      else if(axisno == T_AXIS) {

         // 2022 8 14 - chc T�令���� : edTResolution
         str.sprintf("%.3f",ipos * frnMain->edTResolution->Text.ToDouble());

         frnMain->pnlT->Caption = str;
      }
      else if(axisno == R_AXIS) {
         frnMain->pnlR->Caption = str;
      }

      // Command
      APS_get_command(axisno+StartAxisNo,&ipos);
      str = IntToStr(ipos);
      commandarray[axisno]->Text = str;

      // Motion Status: istatus
      istatus = APS_motion_status(axisno+StartAxisNo);
      Motion_status = istatus;
      str = IntToHex(Motion_status,4);
      motionstatusarray[axisno]->Caption = str;
      if(axisno < MAX_AXIS_NO)
         MotionStatusCode[axisno] = Motion_status;

      // IO Status
      istatus = APS_motion_io_status(axisno+StartAxisNo);
      IO_status = istatus;
      DecodeIOStatus(axisno,IO_status);
      str = IntToHex(IO_status,4);
      iostatusarray[axisno]->Caption = str;
   }

   // ��Delay
   // 2022 9 23 - chc �����ݬ�
   //Sleep(1);

   // 2022 9 23 - chc Log
   //frnMain->WriteSystemLog("DI��sMotion Done.");

   // 2022 9 23 - chc �����ݬ�
   //=> �n��, �_�h�@��Status�C�⤣�|update!
   //=> �|�d��
   //Application->ProcessMessages();

   // 2022 9 23 - chc Log
   //frnMain->WriteSystemLog("DI��sProcessMessages Done.");

   // �O���O�_�}�l��DIO
   frnMain->boolDIO = true;

   // �YMotion�b�B�@���h�����O
   boolMotionState = false;
   for(int i=0 ; i<MAX_AXIS_NO ; i++) {
      if(MotionStatusCode[i] != 0 && MotionStatusCode[i] != 17 && MotionStatusCode[i] != 18)
         boolMotionState = true;
   }
   if(boolMotionState == true)
      frnMain->pnlMotionState->Color = clLime;
   else
      frnMain->pnlMotionState->Color = clSilver;
   SetGreenLamp(boolMotionState);

   // 2022 9 5 - chc ����
   //Sleep(1);
   //Application->ProcessMessages();

   // �O���O�_�bUpdate?
   frnMain->boolInMotionUpdate = false;

   // 2022 9 23 - chc Display
   frnMain->pnlMotionStatus->Color = clSilver;
   frnMain->pnlMotionStatus->Refresh();

   // 2022 9 23 - chc Log
   //frnMain->WriteSystemLog("DI��sLeave.");

}
//---------------------------------------------------------------------------
// 4�b�P��Home
#define HOME_START              0
#define HOME_EL                 1
#define HOME_MOVE               2
#define HOME_DONE               3

//HomeAll
//1. X : -EL
//2. T : Home & ��w��
//3. X/Z : Home(0)

void __fastcall AllHome()
{
I16 axisno,homemode;
U16 motionstatus,iostatus;
int pos;
I32 cmd;
I16 ret;
int counter = 1;
I16 Motion_status;
int no;

// 2023 2 26 - chc MAX_AXIS_NO: Parameter
//int status[MAX_AXIS_NO],cnt;
int status[MAX_AXIS_NO_DEFAULT],cnt;

F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
bool boolnel = true;
AnsiString name;
TLMDComboBox* combobox;

// �Ĥ@���i�ӭn�Ұ�CCD Live
static bool first = true;
bool boolmotiondone = false;

// 2022 67 - chc Motion Done��n��Timer�Ӱ�
static bool efemfirst = true;

bool boolhome = true;

// 2023 2 17 - chc AdjustXPos
bool boolAdjustXPos = false;

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("AllHome(1): " + IntToStr(MAX_AXIS_NO));

   frnMain->pnlAllHomeStatus->Color = clSilver;
   frnMain->pnlSystemMessage->Caption = "Motion Homing...";
   frnMain->pnlMainOperation->Caption = frnMain->pnlSystemMessage->Caption;
   // ���]�w��X��HomeMode
   homemode = frnMain->combHomeModeX->ItemIndex;
   frnMain->pnlStatusX->Color = clSilver;
   frnMain->pnlStatusY->Color = clSilver;
   frnMain->pnlStatusT->Color = clSilver;
   frnMain->pnlStatusZ->Color = clSilver;
   frnMain->pnlStatusR->Color = clSilver;

   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;

   for(no=0 ; no<MAX_AXIS_NO ; no++) {
      status[no] = HOME_START;
   }

   // 2023 3 20 - chc Not Do Table Home
   // do
   //goto allhomedone;

/*
   // 2022 6 8 - chc Z����-EL
   //1. Z : -EL
   counter = 1;
   axisno = Z_AXIS;
   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   DelayTimeM3(100);
   frnMain->WriteSystemLog(">-EL: Z...");
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   // ��homevel
   // => ��MaxVel
   //maxvel = homevel;
   frnMain->pnlSystemMessage->Caption = "Z -EL Vel: " + IntToStr(maxvel);
   frnMain->pnlStatusZ->Color = clYellow;
   frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   // Message
   // ���ϦV��-EL(�]�w��0�I)
   // ret: 0-off, 1-on
   boolnel = GetNELStatus(axisno);
   if(boolnel == true)
      frnMain->pnlNELZ->Color = clRed;
   else
      frnMain->pnlNELZ->Color = clSilver;
   // ���b-EL, Do
   if(boolnel == false) {
      I32 ipos;
      APS_get_position(axisno+StartAxisNo,&ipos);
      pos = ipos;
      pos = -9000000;                                                           // 90mm
      frnMain->WriteSystemLog(">Home -EL: Z �I�sAPS_relative_move()");
      ret = APS_relative_move(axisno+StartAxisNo,(int)pos,(int)maxvel);
      // ��Z��-EL
      frnMain->pnlSystemMessage->Caption = "Wait Z -EL Move...";
      while(1) {
         boolnel = GetNELStatus(axisno);
         if(boolnel == true)
            frnMain->pnlNELZ->Color = clRed;
         else
            frnMain->pnlNELZ->Color = clSilver;
         if(boolnel == true) {
            frnMain->pnlStatusZ->Color = clGreen;
            // Message
            frnMain->WriteSystemLog("> Z -EL Done");
            break;
         }
         DelayTimeM3(100);

         counter++;
         // 20sec
         if(counter > 200) {
            frnMain->pnlSystemMessage->Caption = "Z -EL Timeout!";
            frnMain->WriteSystemLog(">Z -EL Fail: Timeout!");
            frnMain->pnlAllHomeStatus->Color = clRed;
            // Beep�n
            Beep(500);
            // Alarm Message
            frnMain->pnlSystemMessage->Caption = " Z -EL Error!";
            // ReStart System
            frnMain->pnlStartMessage->Caption = "�b�����`!";
            //frnMain->pnlMoving->Caption = "�Э��Ұʨt��";
            //frnMain->pnlMoving->Visible = true;
            //pnlMoving->Refresh();
            frnMain->pnl7856Status->Color = clRed;
            frnMain->pnlPCI7856->Color = clRed;
            frnMain->shMotionStatus->Brush->Color = clRed;
            boolhome = false;
            goto end;
         }

      }
      DelayTimeM3(1000);
   }

   // Pause
   if(Application->MessageBox("Z��-EL: T��Home, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
      goto end;
   }
*/

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("AllHome(2): " + IntToStr(MAX_AXIS_NO));

   // 2.Z�n����Home
   axisno = Z_AXIS;
   frnMain->pnlStatusZ->Color = clGreen;
   APS_home_move(axisno+StartAxisNo);
   Sleep(100);
   frnMain->WriteSystemLog(">Wait Z Homing...");
   status[axisno] = HOME_MOVE;
   frnMain->pnlSystemMessage->Caption = "Wait Homing...";
   frnMain->WriteSystemLog("====== Z Homing �}�l =======");
   frnMain->pnlMainOperation->Caption = "Z Homing...";
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
   counter = 1;
   while(1) {
      cnt = 0;
      boolmotiondone = GetMotionDoneStatus(axisno);
      if(status[axisno] == HOME_DONE)
         cnt++;

      // 2022 6 21 - chc �n�[��pnlINPT
      //else if(boolmotiondone == true && status[axisno] != HOME_DONE)  {
      else if(boolmotiondone == true && status[axisno] != HOME_DONE && frnMain->pnlINPT->Color == clRed)  {

         frnMain->pnlStatusZ->Color = clLime;
         frnMain->WriteSystemLog(">ZAxis Home Done.");
         status[axisno] = HOME_DONE;
         frnMain->combAxis->ItemIndex = axisno;
         DelayTimeM3(200);
         cnt++;
      }
      if(cnt >= 1) {
         APS_set_position(axisno+StartAxisNo,0);
         APS_set_command(axisno+StartAxisNo,0);
         break;
      }

      DelayTimeM3(50);
      counter++;
      frnMain->pnlSystemMessage->Caption = "Wait Z Homing... " + IntToStr(counter);
      frnMain->WriteSystemLog(">Wait Z Homing... " + IntToStr(counter));
      if(frmLogo->Visible == true)
         frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
      // 10sec: 50*200
      // �����ɶ�150sec
      if(counter > 3000) {
         frnMain->pnlSystemMessage->Caption = "Z Home Timeout!";
         frnMain->WriteSystemLog(">Z Home Fail: Timeout!");
         frnMain->pnlAllHomeStatus->Color = clRed;
         // Beep�n
         Beep(500);
         // Alarm Message
         frnMain->pnlSystemMessage->Caption = " Z Home Error!";
         if(frmLogo->Visible == true)
            frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
         // ReStart System
         frnMain->pnlStartMessage->Caption = "�b�����`!";
         frnMain->pnlStartMessage->Refresh();
         if(frmLogo->Visible == true)
            frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
         //frnMain->pnlMoving->Caption = "�Э��Ұʨt��";
         //frnMain->pnlMoving->Visible = true;
         //pnlMoving->Refresh();
         frnMain->pnl7856Status->Color = clRed;
         frnMain->WriteSystemLog(">>Set 7856 RED - 3");
         frnMain->pnlPCI7856->Color = clRed;
         frnMain->shMotionStatus->Brush->Color = clRed;
         boolhome = false;
         goto end;
      }
   }
   frnMain->pnlStatusZ->Color = clLime;

   // Pause
   // 2022 9 12 - chc ����
   //if(Application->MessageBox("Z��Home: XYTR Home, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
   //   goto end;
   //}

/*
   // ��w��: 0�ת�T
   // ������
   int t;
   t = frnMain->ed0T->Text.ToInt();
   frnMain->WriteSystemLog("AllHome: T��w��" + IntToStr(t));
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   frnMain->WriteSystemLog("T �}�l����(t) = " + IntToStr(t) + "," + IntToStr(maxvel));
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   Sleep(500);
   // 100sec
   WaitMotionDone(axisno,100000);
   frnMain->WriteSystemLog("T ��������(t) = " + IntToStr(t));

   // Pause
   if(Application->MessageBox("T��w��: XYZ��Home, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
      goto end;
   }
*/

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("AllHome(3): " + IntToStr(MAX_AXIS_NO));

   //3. X/Y/T/R : Home(0)
   frnMain->pnlStatusX->Color = clGreen;
   frnMain->pnlStatusY->Color = clGreen;
   frnMain->pnlStatusT->Color = clGreen;
   frnMain->pnlStatusR->Color = clGreen;
   APS_home_move(X_AXIS+StartAxisNo);
   Sleep(100);
   APS_home_move(Y_AXIS+StartAxisNo);
   Sleep(100);

   // 2022 12 19 - chc T����
   // 2023 2 3 - chc T�n��: ��]����
   APS_home_move(T_AXIS+StartAxisNo);
   Sleep(100);

   // 2023 2 26 - chc Double LP�~�n
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      frnMain->WriteSystemLog(">Wait XYT Homing...");
   }
   else {
      APS_home_move(R_AXIS+StartAxisNo);
      Sleep(100);
      frnMain->WriteSystemLog(">Wait XYTW Homing...");
   }
   status[X_AXIS] = HOME_MOVE;
   status[Y_AXIS] = HOME_MOVE;

   // 2022 12 19 - chc T����
   // 2023 2 3 - chc T�n��: ��]����
   status[T_AXIS] = HOME_MOVE;
   //status[T_AXIS] = HOME_DONE;

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog(">Axis Home AxisTNo: " + IntToStr(MAX_AXIS_NO));

   status[R_AXIS] = HOME_MOVE;
   // ��XYZMotion Done
   boolmotiondone = false;
   frnMain->pnlSystemMessage->Caption = "Wait XYTW Homing...";
   frnMain->pnlMainOperation->Caption = frnMain->pnlSystemMessage->Caption;
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
   counter = 1;

   // 2023 2 26 - chc Log
   frnMain->WriteSystemLog("AllHome(4): " + IntToStr(MAX_AXIS_NO));

   while(1) {
      cnt = 0;

      // 2023 2 26 - chc MAX_AXIS_NO: Parameter
      //for(no=X_AXIS ; no<=R_AXIS ; no++) {
      for(no=X_AXIS ; no<=MAX_AXIS_NO-1 ; no++) {

         if(no == Z_AXIS)
            continue;
         axisno = no;
         boolmotiondone = GetMotionDoneStatus(axisno);
         if(status[axisno] == HOME_DONE) {
            cnt++;

            // 2023 2 26 - chc Log
            frnMain->WriteSystemLog(">Axis Home Axis,cnt: " + IntToStr(axisno) + "," + IntToStr(cnt));

         }
         else if(boolmotiondone == true && status[axisno] != HOME_DONE)  {
            if(axisno == X_AXIS) {
               // ��INP
               if(frnMain->pnlINPX->Color != clRed) {
                  frnMain->pnlStatusX->Color = clGreen;
               }
               else {
                  frnMain->pnlStatusX->Color = clLime;
                  frnMain->WriteSystemLog(">XAxis Home Done.");
                  status[axisno] = HOME_DONE;
                  cnt++;
               }
            }
            else if(axisno == Y_AXIS) {
               // ��INP
               if(frnMain->pnlINPY->Color != clRed) {
                  frnMain->pnlStatusY->Color = clGreen;
               }
               else {
                  frnMain->pnlStatusY->Color = clLime;
                  frnMain->WriteSystemLog(">YAxis Home Done.");
                  status[axisno] = HOME_DONE;
                  cnt++;
               }
            }
            else if(axisno == T_AXIS) {
               // ��INP
               if(frnMain->pnlINPT->Color != clRed) {
                  frnMain->pnlStatusT->Color = clGreen;
               }
               else {
                  frnMain->pnlStatusT->Color = clLime;
                  frnMain->WriteSystemLog(">TAxis Home Done.");
                  status[axisno] = HOME_DONE;
                  cnt++;
               }
            }
            else if(axisno == R_AXIS) {
               // ��INP
               if(frnMain->pnlINPR->Color != clRed) {
                  frnMain->pnlStatusR->Color = clGreen;
               }
               else {
                  frnMain->pnlStatusR->Color = clLime;
                  frnMain->WriteSystemLog(">RAxis Home Done.");
                  status[axisno] = HOME_DONE;
                  cnt++;
               }
            }

/*
            // @@ Z�S�O�B�z: �|��0�A������20�U, �A�^��0
            // => �ܤ֭n5��H�W
            // 2022 6 20 - chc �w�ק�b���Ѽ�, ���ίS�O�B�z
            // => �٬O�n��!!! �令10sec
            // 2022 6 21 - chc 199�N���\! ��200�令300
            if(axisno == Z_AXIS) {
               if(counter > 300) {

                  // 2022 6 21 - chc �n�[��pnlINPZ
                  if(frnMain->pnlINPZ->Color != clRed) {
                     frnMain->pnlStatusZ->Color = clGreen;
                     frnMain->WriteSystemLog(">ZAxis Home Done(NG-INP)." + IntToStr(counter));
                  }
                  else {

                     status[axisno] = HOME_DONE;
                     frnMain->WriteSystemLog(">ZAxis Home Done(OK)." + IntToStr(counter));
                  }
               }
               else {
                  frnMain->pnlStatusZ->Color = clGreen;
                  frnMain->WriteSystemLog(">ZAxis Home Done(NG-Time)." + IntToStr(counter));
               }
            }
            else
               status[axisno] = HOME_DONE;
            cnt++;
*/

            frnMain->combAxis->ItemIndex = axisno;
            DelayTimeM3(200);
         }
      }

      // 2023 2 26 - chc MAX_AXIS_NO: Parameter
      //if(cnt >= 4) {
      frnMain->WriteSystemLog(">Axis Home cnt: " + IntToStr(cnt) + "," + IntToStr(MAX_AXIS_NO));
      if(cnt >= MAX_AXIS_NO-1) {
         frnMain->WriteSystemLog(">Axis Home cnt(ok): " + IntToStr(cnt) + "," + IntToStr(MAX_AXIS_NO));

/*
         // 2022 6 21 - chc �A�ˬd�@��Z INP
         if(frnMain->pnlINPZ->Color != clRed) {
            frnMain->pnlStatusZ->Color = clGreen;
            frnMain->WriteSystemLog(">ZAxis Home Done(NG-INP Again)." + IntToStr(counter));
            goto again;
         }
*/

         // All to 0
         // 2021 8 17 - chc ������0&�]0
         /*
         int errcnt = 0;
         for(no=X_AXIS ; no<=Z_AXIS ; no++) {
            axisno = no;
            GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
            // Absolute move
            maxvel = (maxvel*2) / 3;
            pos = 0;
            GetAxisName((int)axisno);
            name = frnMain->AxisName;
            frnMain->WriteSystemLog(">" + name + "Axis Move to 0...");
            ret = AbsoluteMove(axisno,pos,startvel,maxvel,tacc,tdec);
            if(ret == ERR_NoError)
               frnMain->WriteSystemLog(">" + name + "Axis Move to 0 done.");
            else {
               frnMain->WriteSystemLog(">" + name + "Axis Move to 0 fail!");
               errcnt++;
            }
            Sleep(100);
         }
         // Error
         if(errcnt > 0) {
            frnMain->pnlSystemMessage->Caption = "Home Done, Move 0 fail!";
            frnMain->WriteSystemLog(">Home Done, Move 0 fail!");
            goto homeerr;
         }
         */
         // 2023 2 17 - chc AdjustXPos
         //APS_set_position(X_AXIS+StartAxisNo,0);
         //APS_set_command(X_AXIS+StartAxisNo,0);
         AdjustXPos();
         boolAdjustXPos = true;

         APS_set_position(Y_AXIS+StartAxisNo,0);
         APS_set_command(Y_AXIS+StartAxisNo,0);
         APS_set_position(T_AXIS+StartAxisNo,0);
         APS_set_command(T_AXIS+StartAxisNo,0);

         // 2023 2 26 - chc Double LP�~�n
         if(frnMain->LoadPortMode == LOADPORT_DOUBLE) {

            APS_set_position(R_AXIS+StartAxisNo,0);
            APS_set_command(R_AXIS+StartAxisNo,0);
         }
         break;
      }

again:
      DelayTimeM3(50);
      counter++;
      frnMain->pnlSystemMessage->Caption = "Wait XYTR Homing... " + IntToStr(counter);
      frnMain->pnlMainOperation->Caption = frnMain->pnlSystemMessage->Caption;
      frnMain->WriteSystemLog(">Wait Homing... " + IntToStr(counter));
      if(frmLogo->Visible == true)
         frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
      // 10sec: 50*200
      // �����ɶ�100sec
      if(counter > 2000) {
         frnMain->pnlSystemMessage->Caption = "Home Done Timeout!";
         frnMain->WriteSystemLog(">Home Done Fail: Timeout!");
         if(frmLogo->Visible == true)
            frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
homeerr:
         frnMain->pnlAllHomeStatus->Color = clRed;
         // Beep�n
         Beep(500);
         // Alarm Message
         frnMain->pnlSystemMessage->Caption = " AllHome(Done) Error!";
         if(frmLogo->Visible == true)
            frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;
         // ReStart System
         frnMain->pnlStartMessage->Caption = "�b�����`!";
         frnMain->pnlStartMessage->Refresh();
         //frnMain->pnlMoving->Caption = "�Э��Ұʨt��";
         //frnMain->pnlMoving->Visible = true;
         //pnlMoving->Refresh();
         frnMain->pnl7856Status->Color = clRed;
         frnMain->WriteSystemLog(">>Set 7856 RED - 4");
         frnMain->pnlPCI7856->Color = clRed;
         frnMain->shMotionStatus->Brush->Color = clRed;
         boolhome = false;

         // 2022 12 22 - chc
         //return;
         goto end;

      }
   }

   frnMain->pnlStatusX->Color = clLime;
   frnMain->pnlStatusY->Color = clLime;
   frnMain->pnlStatusT->Color = clLime;
   frnMain->pnlStatusR->Color = clLime;

allhomedone:

   // 2023 2 17 - chc AdjustXPos
   if(boolAdjustXPos == false)
      AdjustXPos();

   frnMain->WriteSystemLog(">HomeAll-Motion Completed");
   frnMain->pnlSystemMessage->Caption = "Home-Motion Completed.";
   frnMain->pnlMainOperation->Caption = frnMain->pnlSystemMessage->Caption;
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;

   // Pause
   // 2022 9 12 - chc ����
   //if(Application->MessageBox("Home����: �]���ˬd, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
   //   boolhome = false;
   //   goto end;
   //}

   // �O�_����LHome?
   frnMain->boolHomeDone = true;
   frnMain->pnlAllHomeStatus->Color = clLime;
   frnMain->pnlSystemMessage->Caption = "";
   frnMain->pcSystem->Enabled = true;
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlSystemMessage->Caption;

   // ����Motion Home
   if(frnMain->pnlStartMessage->Visible == true) {
      frnMain->pnlStartMessage->Visible = false;
      // �YCCD��Live, �۰ʱҰʬ�Live: �Ĥ@���i�ӭn�Ұ�CCD Live
      if(frnMain->btnReviewLive->Caption == "Live" && first == true) {
         frnMain->btnReviewLiveClick(frnMain);
         first = false;
      }
   }

   // Door�}��/EMO �n����Home�~���~��
   if(frnMain->boolDoorEMO == true) {
      frnMain->boolDoorEMO = false;
      if(frnMain->boolHomeDone == true)
         frnMain->WriteSystemLog("EMO: Do SetMotionStatus(). - boolHomeDone is True");
      else
         frnMain->WriteSystemLog("EMO: Do SetMotionStatus(). - boolHomeDone is False");
   }

   // �A�]�w�@��!!!
   frnMain->boolHomeDone = true;

   // 2022 7 26 - chc Macro Home
   // 2022 9 12 - chc ����
   //if(Application->MessageBox("Home����: Setup Macro, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
   //   boolhome = false;
   //   goto end;
   //}

   // 2022 12 6 - chc Logo Form
   if(frmLogo->Visible == true) {
      frmLogo->pnlMicroUnit->Caption = "Initialized.";
      frmLogo->pnlMacroUnit->Caption = "Initializing...";
   }

   // 2022 9 22 - chc Macro Setup Message
   // 2023 3 21 - chc Macro��timer�Ӱ��� => �H��A��
   frnMain->pnlStartMessage->Caption = "Macro Initializing...";
   frnMain->pnlStartMessage->Visible = true;
   frnMain->pnlMainOperation->Caption = frnMain->pnlStartMessage->Caption;
   frnMain->pnlStartMessage->Refresh();
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = frnMain->pnlStartMessage->Caption;

   frnMain->WriteSystemLog(">Macro Home...");
   frnMain->btnMacroHomeClick(frnMain);
   frnMain->WriteSystemLog(">Macro Home.");

   // 2023 3 21 - chc ����Macro init
   frnMain->boolMacroInitDone = true;

   // 2022 9 22 - chc Macro Setup Message
   frnMain->pnlStartMessage->Visible = true;

   if(frnMain->pnlMacroHome->Color != clLime) {
      frnMain->pnlAlarmMessage->Caption = "Macro Home����!";
      frnMain->WriteSystemLog(">Macro Home����!");
      frnMain->pnlMainOperation->Caption = frnMain->pnlAlarmMessage->Caption;
      boolhome = false;
      goto end;

   }
   else
      frnMain->pnlMainOperation->Caption = "Macro Home Done.";

   // 2023 1 15 - chc Logo Form
   if(frmLogo->Visible == true) {
      frnMain->WriteSystemLog("Logo Macro Done.");
      frmLogo->pnlMacroUnit->Caption = "Initialized.";
      frmLogo->pnlSignalTowerUnit->Caption = "Initializing...";
   }

   frnMain->pnlMainOperation->Caption = "Light Tower Testing";
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = "Light Tower Testing";

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   // 2022 12 5 - chc ����Signal Tower: R/Y/G/B/Buzzer
   //Application->MessageBox("��O���դ@���@�u, �нT�{.    ","��O���է@�~    ",MB_ICONINFORMATION|MB_OK);
   TestSignalTower();

   // 2023 1 15 - chc Logo Form
   if(frmLogo->Visible == true) {
      frmLogo->pnlSignalTowerUnit->Caption = "Initialized.";
   };

   // Pause
   // 2022 9 12 - chc ����
   //if(Application->MessageBox("Home����: Setup EFEM, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
   //   goto end;
   //}

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = "Setup EFEM";

   // 2022 7 6 - chc EFEM: Motion Done��n��Timer�Ӱ�
   if(efemfirst == true) {
      frnMain->pnlMainOperation->Caption = "SetupEFEM...";
      frnMain->WriteSystemLog("AllHome: SetupEFEM...");
      efemfirst = false;

      frnMain->WriteSystemLog("SetupEFEM(1): AllHome() Enter...");

      // 2022 12 5 - chc Full or LP+Robot
      //if(frnMain->SetupEFEM() == false) {
      if(frnMain->SetupEFEM(1) == false) {

         frnMain->pnlEFEMReady->Color = clRed;
         frnMain->pnlCCD->Color = clGray;
         frnMain->pnlMainOperation->Caption = "SetupEFEM Fail!";
         boolhome = false;
      }
      else {
         frnMain->pnlEFEMReady->Color = clLime;
         frnMain->pnlCCD->Color = clSilver;
         frnMain->pnlMainOperation->Caption = "SetupEFEM Succ!";

         // TSMC�ק�: �G���O(�wUnLoad), �G��O(��Load)
         // OneLine, Door Open
         if(frnMain->pnlLoadPortS2->Caption == "0" && frnMain->pnlLoadPortS11->Caption == "0") {
            frnMain->btnCassetteLoad->Caption = "UnLoad";
            frmRecipe->btnCassetteLoad->Caption = "UnLoad";

            // 2023 4 27 - chc  �Τ@����
            //SetGreenLamp(true);
            //SetYellowLamp(false);
         }
         else {
            frnMain->btnCassetteLoad->Caption = "Load";
            frmRecipe->btnCassetteLoad->Caption = "Load";

            // 2023 4 27 - chc  �Τ@����
            //SetGreenLamp(false);
            //SetYellowLamp(true);
         }
      }

// 2023 3 2 - chc ����
//
      // 2022 12 5 - chc Init BXFM
      frnMain->pnlMainOperation->Caption = "SetupBXFM...";
      if(frnMain->SetupBXFM() == false) {
         frnMain->pnlMainOperation->Caption = "SetupBXFM Fail!";
         boolhome = false;
      }
      else
         frnMain->pnlMainOperation->Caption = "SetupBXFM Succ";

      // �YHomeDone�ˬdVacuum, CDA
      frnMain->WriteSystemLog("SetupEFEM����(MotionHome): �ˬdVacuum, CDA");
      frnMain->CheckCDAVacuum();
      // ���ʫ�A���
      //frnMain->sgCassette->Visible = true;
      //frnMain->pnlCassetteStatus->Visible = true;
      //frnMain->btnCassetteLoad->Visible = true;
      //frnMain->pnlCassetteLoad->Visible = true;
//
   }

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   UpdateMotionStatus();
   frnMain->WriteSystemLog("����Motion Home.");
   if(frmLogo->Visible == true)
      frmLogo->pnlMessage->Caption = "Motion Home Done.";
   if(boolhome == true)
      frnMain->pnlMainOperation->Caption = "System Initialized.";
   else
      frnMain->pnlMainOperation->Caption = "System Initialize Fail!";
   frnMain->pnlMainOperation->Refresh();

   // 2023 4 10 - chc SetFFU
   frnMain->btnSetFFUClick(frnMain);

   frnMain->WriteSystemLog("Ask��J���I");
   // Pause
//   if(Application->MessageBox("Home,�]���ˬd����: ��J���I, �~��?      ", "AllHome", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
//      goto end;
//   }

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   // ��Robot�J���I
   frnMain->SetMoving(true);
   frnMain->WriteSystemLog(">�}�lToLoadPosition...");
   frnMain->btnToLoadPositionClick(frnMain);
   frnMain->SetMoving(false);
   frnMain->WriteSystemLog(">����ToLoadPosition");
   frnMain->pnlRun->Caption = "Ready";

end:

   // Reset�U�bStatus
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;
   frnMain->iAxisMove[R_AXIS] = NO_MOVE;

   // 2023 5 9 - chc Exit
   if(frnMain->ExitYes == true) {
      return;
   }

   // �YHomeDone�ˬdVacuum, CDA
   frnMain->WriteSystemLog("AllHome����(MotionHome): �ˬdCDA");
   if(frnMain->CheckCDAVacuum() == false) {
      // ReStart System
      frnMain->pnlStartMessage->Caption = "CDA���`!";
      frnMain->pnlStartMessage->Refresh();
      //frnMain->pnlMoving->Caption = "�Э��Ұʨt��";
      //frnMain->pnlMoving->Visible = true;
      //pnlMoving->Refresh();
   }

   // 2022 12 22 - chc AllHome Error
   if(boolhome == false)
      frnMain->pnlMainOperation->Caption = "System Initialize Fail!";
   frnMain->boolAllHome = boolhome;
   frnMain->WriteSystemLog("AllHome����.");

}
//---------------------------------------------------------------------------
// ��O: boolMotionState = false
void __fastcall SetGreenLamp(bool mode)
{

   // 2023 4 27 - chc ����
   return;

   // �G��O
   if(mode == true) {
      //if(frnMain->cbID5Bit13->Checked == false)
      //   frnMain->cbID5Bit13->Checked = true;
      if(frnMain->cbBit13->Checked == false) {
         if(frnMain->cbBit11->Checked == false && frnMain->cbBit12->Checked == false)
            frnMain->cbBit13->Checked = true;
      }
      else {
         if(frnMain->cbBit11->Checked == true || frnMain->cbBit12->Checked == true)
            frnMain->cbBit13->Checked = false;
      }
   }
   // ����O
   else {
      //if(frnMain->cbID5Bit13->Checked == true)
      //   frnMain->cbID5Bit13->Checked = false;
      if(frnMain->cbBit13->Checked == true)
         frnMain->cbBit13->Checked = false;
   }
   //WriteDO(IO3_ID);
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// ���O
void __fastcall SetRedLamp(bool mode)
{

   // 2023 4 27 - chc ����
   return;

   // �G�O
   if(mode == true) {
      //if(frnMain->cbID5Bit11->Checked == false)
      //   frnMain->cbID5Bit11->Checked = true;
      if(frnMain->cbBit11->Checked == false)
         frnMain->cbBit11->Checked = true;

      // Off: Yellow,Green
      if(frnMain->cbBit12->Checked == true)
         frnMain->cbBit12->Checked = false;
      if(frnMain->cbBit13->Checked == true)
         frnMain->cbBit13->Checked = false;

   }
   // ���O
   else {
      //if(frnMain->cbID5Bit11->Checked == true)
      //   frnMain->cbID5Bit11->Checked = false;
      if(frnMain->cbBit11->Checked == true)
         frnMain->cbBit11->Checked = false;
   }
   //WriteDO(IO3_ID);
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// ���O
void __fastcall SetRingLamp(bool mode)
{

   // �G�O
   if(mode == true) {
      if(frnMain->cbBit3->Checked == false)
         frnMain->cbBit3->Checked = true;
   }
   // ���O
   else {
      if(frnMain->cbBit3->Checked == true)
         frnMain->cbBit3->Checked = false;
   }
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// ���O
void __fastcall SetYellowLamp(bool mode)
{

   // 2023 4 27 - chc ����
   return;

   // �G�O
   if(mode == true) {
      //if(frnMain->cbID5Bit12->Checked == false)
      //   frnMain->cbID5Bit12->Checked = true;
      if(frnMain->cbBit12->Checked == false)
         frnMain->cbBit12->Checked = true;
   }
   // ���O
   else {
      //if(frnMain->cbID5Bit12->Checked == true)
      //   frnMain->cbID5Bit12->Checked = false;
      if(frnMain->cbBit12->Checked == true)
         frnMain->cbBit12->Checked = false;
   }
   //WriteDO(IO3_ID);
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// ���O
void __fastcall SetBlueLamp(bool mode)
{

   // �G�O
   if(mode == true) {
      //if(frnMain->cbID5Bit14->Checked == false)
      //   frnMain->cbID5Bit14->Checked = true;
      if(frnMain->cbBit14->Checked == false)
         frnMain->cbBit14->Checked = true;
   }
   // ���O
   else {
      //if(frnMain->cbID5Bit14->Checked == true)
      //   frnMain->cbID5Bit14->Checked = false;
      if(frnMain->cbBit14->Checked == true)
         frnMain->cbBit14->Checked = false;
   }
   //WriteDO(IO3_ID);
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc new ok
bool __fastcall MoveForwardMoueUp()
{
TMouseButton Button;
TShiftState Shift;
int X, Y;
static bool active = false;

   // Re-Entrant
   if(active == true)
      return false;
   active = true;

   frnMain->MoveMouseUp();

   // Re-Entrant
   active = false;
   return true;
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc new ok
bool __fastcall MoveBackwardMoueUp()
{
TMouseButton Button;
TShiftState Shift;
int X, Y;
static bool active = false;

   // Re-Entrant
   if(active == true)
      return false;
   active = true;

   frnMain->BackwardMouseUp();

   // Re-Entrant
   active = false;
   return true;
}
//---------------------------------------------------------------------------
// �令PCI-8158
// 2021 5 16 - chc new
void __fastcall GetAxisData(int axisno,int *dist,int *startvel,int *maxvel,F64 *tacc,F64 *tdec,int *homevel, int *reviewvel, int *uplimit, int *lowdistance)
{
int ispeed;

   switch(axisno) {
      // X
      case X_AXIS:
         *dist       = frnMain->edDistanceX->Text.ToInt();
         *startvel   = frnMain->edStartVelX->Text.ToInt();
         *maxvel     = frnMain->edMaxVelX->Text.ToInt();
         *tacc       = frnMain->edTaccX->Text.ToDouble();
         *tdec       = frnMain->edTdecX->Text.ToDouble();
         *homevel    = frnMain->edHomeVelX->Text.ToInt();
         *reviewvel  = frnMain->edReviewVelX->Text.ToInt();
         *uplimit    = frnMain->edUpLimitX->Text.ToInt();
         *lowdistance = frnMain->edLowDistanceX->Text.ToInt();
         break;
      // Y
      case Y_AXIS:
         *dist       = frnMain->edDistanceY->Text.ToInt();
         *startvel   = frnMain->edStartVelY->Text.ToInt();
         *maxvel     = frnMain->edMaxVelY->Text.ToInt();
         *tacc       = frnMain->edTaccY->Text.ToDouble();
         *tdec       = frnMain->edTdecY->Text.ToDouble();
         *homevel    = frnMain->edHomeVelY->Text.ToInt();
         *reviewvel  = frnMain->edReviewVelY->Text.ToInt();
         *uplimit    = frnMain->edUpLimitY->Text.ToInt();
         *lowdistance = frnMain->edLowDistanceY->Text.ToInt();
         break;
      // Z
      case Z_AXIS:
         *dist       = frnMain->edDistanceZ->Text.ToInt();
         *startvel   = frnMain->edStartVelZ->Text.ToInt();
         *maxvel     = frnMain->edMaxVelZ->Text.ToInt();
         *tacc       = frnMain->edTaccZ->Text.ToDouble();
         *tdec       = frnMain->edTdecZ->Text.ToDouble();
         *homevel    = frnMain->edHomeVelZ->Text.ToInt();
         *reviewvel  = frnMain->edReviewVelZ->Text.ToInt();
         *uplimit    = frnMain->edUpLimitZ->Text.ToInt();
         *lowdistance = frnMain->edLowDistanceZ->Text.ToInt();
         break;
      // T
      case T_AXIS:
         *dist       = frnMain->edDistanceT->Text.ToInt();
         *startvel   = frnMain->edStartVelT->Text.ToInt();
         *maxvel     = frnMain->edMaxVelT->Text.ToInt();
         *tacc       = frnMain->edTaccT->Text.ToDouble();
         *tdec       = frnMain->edTdecT->Text.ToDouble();
         *homevel    = frnMain->edHomeVelT->Text.ToInt();
         *reviewvel  = frnMain->edReviewVelT->Text.ToInt();
         *uplimit    = frnMain->edUpLimitT->Text.ToInt();
         *lowdistance = frnMain->edLowDistanceT->Text.ToInt();
         break;
      // R
      case R_AXIS:
         *dist       = frnMain->edDistanceR->Text.ToInt();
         *startvel   = frnMain->edStartVelR->Text.ToInt();
         *maxvel     = frnMain->edMaxVelR->Text.ToInt();
         *tacc       = frnMain->edTaccR->Text.ToDouble();
         *tdec       = frnMain->edTdecR->Text.ToDouble();
         *homevel    = frnMain->edHomeVelR->Text.ToInt();
         *reviewvel  = frnMain->edReviewVelR->Text.ToInt();
         *uplimit    = frnMain->edUpLimitR->Text.ToInt();
         *lowdistance = frnMain->edLowDistanceR->Text.ToInt();
         break;
   }

   // 2021 6 17 - chc �n�ܰ�Distance
   if(frnMain->rgMoveStep->ItemIndex == 1)
      *dist = *dist * 10;
   else if(frnMain->rgMoveStep->ItemIndex == 2)
      *dist = *dist * 50;

   // 2022 12 6 - chc 0.1x
   else if(frnMain->rgMoveStep->ItemIndex == 3)
      *dist = *dist / 10;

   // @@ test
   frnMain->WriteSystemLog("GetAxisData() Dist = " + IntToStr(*dist));

   //---------------------------------------------------------------------------
   // Z�b�W��(0.1%, 25%, 100%): rgZSpeed
   // => cbZSame�ثe�O�Ŀ�, �G���|��
   if(axisno == Z_AXIS && frnMain->cbZSame->Checked == false) {
      if(frnMain->rgZSpeed->ItemIndex == JOG_LOW_SPEED) {
         // 2016 12 20 - chc �令0.3%
         // 2016 12 30 - chc �ϥγ]�w��
         //*maxvel *= 0.003;
         //if(*maxvel < 10)
         //   *maxvel = 10;
         double factor;
         factor = frnMain->edSpeedZLow->Text.ToDouble() / 100.0;
         ispeed = *maxvel;
         ispeed = ispeed * factor;
         *maxvel = ispeed;

      }
      else if(frnMain->rgZSpeed->ItemIndex == JOG_MIDDLE_SPEED) {
         // 2016 12 30 - chc �ϥγ]�w��
         //*maxvel *= 0.15;
         double factor;
         factor = frnMain->edSpeedZMiddle->Text.ToDouble() / 100.0;
         ispeed = *maxvel;
         ispeed = ispeed * factor;
         *maxvel = ispeed;

      }
      else if(frnMain->rgZSpeed->ItemIndex == JOG_HIGH_SPEED) {
         // 2016 12 30 - chc �ϥγ]�w��
         //*maxvel *= 1.0;
         double factor;
         factor = frnMain->edSpeedZHigh->Text.ToDouble() / 100.0;
         ispeed = *maxvel;
         ispeed = ispeed * factor;
         *maxvel = ispeed;

      }
   }
   else {

      // �b���o�ѼƮɰ������t��
      // 2015 5 15 - chc �t�פ��
      // 2016 12 8 - chc ��_�οﶵ: Low/Middle/High
      //   0.2%/50%/100%
      frnMain->JogSpeed = frnMain->rgSpeed->ItemIndex;

    // 2022 8 12 - chc W�b�T�w�ΰ��t
    if(axisno != R_AXIS || frnMain->cbRHighSpeed->Checked != true) {

      if(frnMain->rgSpeed->ItemIndex == JOG_LOW_SPEED) {

         // 2016 12 13 - chc re calculate
         //*maxvel *= 0.002;
         // 2016 12 20 - chc �令1/100
         // => �令6/1000
         ispeed = *maxvel;

         // 2016 12 30 - chc �ϥγ]�w��
         //ispeed = ispeed * 0.005;
         //if(ispeed < 10)
         //   ispeed = 10;
         double factor;

         // 2020 6 15 - chc �����γ]�w��
         //if(frnMain->pnlBit7->Color == clLime) {
         //   factor = frnMain->edSpeedTurboLow->Text.ToDouble() / 100.0;
         //   frnMain->btnTurbo->Visible = true;
         //}
         //else {

            factor = frnMain->edSpeedGeneralLow->Text.ToDouble() / 100.0;

         // 2020 6 15 - chc �����γ]�w��
         //}

         ispeed = ispeed * factor;

         *maxvel = ispeed;

      }
      else if(frnMain->rgSpeed->ItemIndex == JOG_MIDDLE_SPEED) {
         // 2016 12 20 - chc �令25%
         // => �令20%

         // 2016 12 30 - chc �ϥγ]�w��
         //*maxvel *= 0.20;
         double factor;

         // 2020 6 15 - chc �����γ]�w��
         //if(frnMain->pnlBit8->Color == clLime) {
         //   factor = frnMain->edSpeedTurboMiddle->Text.ToDouble() / 100.0;
         //   frnMain->btnTurbo->Visible = true;
         //}
         //else {

            factor = frnMain->edSpeedGeneralMiddle->Text.ToDouble() / 100.0;

         // 2020 6 15 - chc �����γ]�w��
         //}

         ispeed = *maxvel;
         ispeed = ispeed * factor;
         *maxvel = ispeed;

      }
      else if(frnMain->rgSpeed->ItemIndex == JOG_HIGH_SPEED) {

         // 2016 12 30 - chc �ϥγ]�w��
         //*maxvel *= 1.0;
         double factor;

         // 2020 6 15 - chc �����γ]�w��
         //if(frnMain->pnlBit9->Color == clLime) {
         //   factor = frnMain->edSpeedTurboHigh->Text.ToDouble() / 100.0;
         //   frnMain->btnTurbo->Visible = true;
         //}
         //else {

            factor = frnMain->edSpeedGeneralHigh->Text.ToDouble() / 100.0;

         // 2020 6 15 - chc �����γ]�w��
         //}

         ispeed = *maxvel;
         ispeed = ispeed * factor;
         *maxvel = ispeed;

      }

    // 2022 8 12 - chc W�b�T�w�ΰ��t
    }

   }
   //
   //*maxvel = (*maxvel * frnMain->tbSpeed->Position) / 100;

   // 2016 12 13 - chc Display current speed
   // 2019 7 22 - chc ����s, ���UpdateMotionStatus()�B�z
   //frnMain->pnlCurrentSpeed->Caption = IntToStr((int)*maxvel);

   // �ˬd��t
   if(*startvel > *maxvel)
      *startvel = *maxvel / 2;

   // 2016 4 21 - chc Vel�W��: 20000/20000/20000/1000000
   if(*maxvel > *uplimit)
      *maxvel = *uplimit;
   if(*homevel > *uplimit)
      *homevel = *uplimit;
   if(*reviewvel > *uplimit)
      *reviewvel = *uplimit;

   // 2016 12 13 - chc Safe
   if(axisno == X_AXIS || axisno == Y_AXIS || axisno == R_AXIS) {
      if(*maxvel > 3000000)
         *maxvel = 3000000;

      // 2020 6 15 - chc XY�b: �j��[��t��0.5
      if(*tacc < 0.5)
         *tacc = 0.5;
      if(*tdec < 0.5)
         *tdec = 0.5;

   }
   else {
      if(*maxvel > 3000000)
         *maxvel = 3000000;
   }
   //---------------------------------------------------------------------------

   // 2021 6 5 - chc �]�wAcc, Dec, StartV
   SetACC_DEC_StartV(axisno, *tacc, *tdec, *startvel, *maxvel);
   frnMain->pnlCurrentSpeed->Caption = IntToStr(axisno) + "-" + IntToStr(*maxvel);
   frnMain->pnlCurrentSpeed->Refresh();

}
//---------------------------------------------------------------------------
// Home Move Positive
void __fastcall HomeOneAxis()
{
I16 axisno,homemode;
U16 motionstatus,iostatus;
F64 pos;
I32 cmd;
I16 ret;
int counter = 1;
I16 Motion_status;
bool boolnel = true;
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
AnsiString name;
TLMDComboBox* combobox;

   // 2021 8 17 - chc ������
   return;

   frnMain->pnlHomeStatus->Color = clSilver;
   axisno = frnMain->combAxis->ItemIndex;

   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   GetAxisName((int)axisno);
   name = frnMain->AxisName;
   combobox = (TLMDComboBox *)(frnMain->FindComponent("combHomeMode" + name));
   homemode = combobox->Text.ToInt();

   frnMain->pnlSystemMessage->Caption = "Home Completed.";
   frnMain->pnlHomeStatus->Color = clLime;

}
//---------------------------------------------------------------------------
// Wait Motion done
// 2021 5 6 - chc new
bool __fastcall WaitMotionDone(int axisno, int ms)
{
static bool active = false;
I32 cmd;
I16 Motion_status;
int counter;
int max;

   // Re-Entrant
   if(active == true) {
      frnMain->WriteSystemLog("WaitMotionDone(): Retrant!");
      return false;
   }
   active = true;
   frnMain->WriteSystemLog("WaitMotionDone(): Axis= " + IntToStr(axisno));

   max = ms / 10;
   DelayTimeM2(10);
   counter = 0;
   while(1) {
      Motion_status = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_status = 1;
      if(Motion_status == 0) {
         frnMain->WriteSystemLog("WaitMotionDone(): Axis= " + IntToStr(axisno) + " done.");
         break;
      }
      else {
         counter++;
         // 10sec: 50*200
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion Timeout!";
            frnMain->WriteSystemLog("WaitMotionDone(): Axis= " + IntToStr(axisno) + " timeout!");
            // Re-Entrant
            active = false;
            return false;
         }
         DelayTimeM2(10);
      }
   }
   // Re-Entrant
   active = false;

   return true;
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall ResetCommand()
{
I16 axisno;

   for(axisno=0 ; axisno<MAX_AXIS_NO ; axisno++) {
      APS_set_command(axisno+StartAxisNo,0);
   }
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall ResetPosition()
{
I16 axisno;

   frnMain->pnlSystemMessage->Caption = "Reset Counter...";
   frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   for(axisno=0 ; axisno<MAX_AXIS_NO ; axisno++) {
      APS_set_position(axisno+StartAxisNo,0);
   }
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall ResetErrorCounter()
{

   // 7856 APS�L���R�O
   return;
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc Center
void __fastcall SetCenterPosition()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->edCenterX->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->edCenterY->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->edCenterZ->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->edCenterT->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc 45
void __fastcall Set45Position()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->ed45X->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->ed45Y->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->ed45Z->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->ed45T->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc 45
void __fastcall SetR45Position()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->edR45X->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->edR45Y->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->edR45Z->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->edR45T->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc 0
void __fastcall Set0Position()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->ed0X->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->ed0Y->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->ed0Z->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->ed0T->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall SetLoadPosition()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->edLoadX->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->edLoadY->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->edLoadZ->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->edLoadT->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc Original
void __fastcall SetOriginalPosition()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->edOriginalX->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->edOriginalY->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->edOriginalZ->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->edOriginalT->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc Original
void __fastcall SetCCDPosition()
{

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   frnMain->edCCDX->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   frnMain->edCCDY->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   frnMain->edCCDZ->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   frnMain->edCCDT->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// �줤���I - Z����, �b��T, X/Y�P�ɲ���
void __fastcall ToCenterPosition()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

   frnMain->pnlToCenterPosition->Color = clSilver;

// 2022 8 15 - chc �ϥγ̰��t: rgSpeed
int oldspeed;
   oldspeed = frnMain->rgSpeed->ItemIndex;
   frnMain->rgSpeed->ItemIndex = JOG_HIGH_SPEED;

   // �n��Edit����
   x = frnMain->edCenterX->Text.ToInt();
   y = frnMain->edCenterY->Text.ToInt();
   z = frnMain->edCenterZ->Text.ToInt();
   t = frnMain->edCenterT->Text.ToInt();

   // 2023 3 14 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      z += frnMain->edWafer8ZDown->Text.ToInt();
   }

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // ��XY�AZT
   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("ToCenter(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("ToCenter(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "�����I���ʥ���!";
      frnMain->pnlToCenterPosition->Color = clRed;
   }

   // Z
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   frnMain->WriteSystemLog("ToCenter(z) = " + IntToStr(z));
   WaitMotionDone(axisno,10000);

   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   frnMain->WriteSystemLog("ToCenter(t) = " + IntToStr(t));
   WaitMotionDone(axisno,10000);

   frnMain->WriteSystemLog("ToCenter: ���ݧ���...");
   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "�����I���ʥ���!";
      frnMain->pnlToCenterPosition->Color = clRed;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }
   else {
      // �ˬdZ?�~�t�n�b2000�H��
      WaitZFinish(z);
      frnMain->pnlSystemMessage->Caption = "�����I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlToCenterPosition->Color = clLime;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

   // 2022 8 15 - chc �ϥγ̰��t: rgSpeed
   frnMain->rgSpeed->ItemIndex = oldspeed;
   frnMain->WriteSystemLog("�����I����: Leave.");

}
//---------------------------------------------------------------------------
// ��45�I - Z����, �b��T, X/Y�P�ɲ���
// mode - 1(XYZT), 0(T)
void __fastcall To45Position(int mode)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

// 2022 6 19 - chc ���v
AnsiString msg;
int dx,dy,dz,zlimit;
bool booloffset = false;

   frnMain->pnlTo45Position->Color = clSilver;

   // �n��Edit����
   x = frnMain->ed45X->Text.ToInt();
   y = frnMain->ed45Y->Text.ToInt();
   z = frnMain->ed45Z->Text.ToInt();
   t = frnMain->ed45T->Text.ToInt();

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   if(mode == 1) {
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("To45(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);
   }

   // 2022 6 19 - chc �u��T
   if(mode == 0) {
      // �Y�O�b������m, �n���vX/Y/Z
      if(frnMain->IsHeadHorizontal() == true) {
         booloffset = true;
         zlimit = frnMain->edTMoveZMax->Text.ToInt();
         dx = frnMain->ed45X->Text.ToInt() - frnMain->ed0X->Text.ToInt();
         dy = frnMain->ed45Y->Text.ToInt() - frnMain->ed0Y->Text.ToInt();
         dz = frnMain->ed45Z->Text.ToInt() - frnMain->ed0Z->Text.ToInt();
         msg.sprintf("Check�b������m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         x += dx;
         y += dx;
         z += dx;
         if(z > zlimit)
            z = zlimit;
      }
      else if(frnMain->IsHeadRTilt() == true) {
         booloffset = true;
         zlimit = frnMain->edTMoveZMax->Text.ToInt();
         dx = frnMain->ed45X->Text.ToInt() - frnMain->edR45X->Text.ToInt();
         dy = frnMain->ed45Y->Text.ToInt() - frnMain->edR45Y->Text.ToInt();
         dz = frnMain->ed45Z->Text.ToInt() - frnMain->edR45Z->Text.ToInt();
         msg.sprintf("Check�b�k�ɱצ�m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         x += dx;
         y += dx;
         z += dx;
         if(z > zlimit)
            z = zlimit;
      }

   }

   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   frnMain->WriteSystemLog("To45(t) = " + IntToStr(t));
   WaitMotionDone(axisno,10000);

   // 2022 6 19 - chc �u��T: �Y�O�b������m, �n���vX/Y/Z
   if(mode == 0) {
      if(booloffset == true) {
         msg.sprintf("���t���v: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         frnMain->pnlSystemMessage->Caption = msg;
         int oldspeed = frnMain->rgSpeed->ItemIndex;
         frnMain->rgSpeed->ItemIndex = 1;
         DoRelativeMove(X_AXIS, dx);
         DoRelativeMove(Y_AXIS, dy);
         DoRelativeMove(Z_AXIS, dz);
         frnMain->rgSpeed->ItemIndex = oldspeed;
      }
   }

   // XY
   if(mode == 1) {
      axisno = X_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
      frnMain->WriteSystemLog("To45(x) = " + IntToStr(x));

      axisno = Y_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
      frnMain->WriteSystemLog("To45(y) = " + IntToStr(y));
   }
   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlTo45Position->Color = clRed;
      if(mode == 1)
         frnMain->pnlSystemMessage->Caption = "45���I���ʥ���!";
      else
         frnMain->pnlSystemMessage->Caption = "45���IT���ʥ���!";
   }
   else {

      // 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
      if(mode == 1)
         WaitZFinish(z);

      frnMain->pnlTo45Position->Color = clLime;
      if(mode == 1)
         frnMain->pnlSystemMessage->Caption = "45���I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      else
         frnMain->pnlSystemMessage->Caption = "45���IT���ʧ���(t): " + IntToStr(t);
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

}
//---------------------------------------------------------------------------
// ��0�I - Z����, �b��T, X/Y�P�ɲ���
// mode - 1(XYZT), 0(T)
void __fastcall To0Position(int mode)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

// 2022 6 19 - chc ���v
AnsiString msg;
int dx,dy,dz,zlimit;
bool booloffset = false;

   frnMain->pnlTo0Position->Color = clSilver;

   // �n��Edit����
   x = frnMain->ed0X->Text.ToInt();
   y = frnMain->ed0Y->Text.ToInt();
   z = frnMain->ed0Z->Text.ToInt();
   t = frnMain->ed0T->Text.ToInt();

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   if(mode == 1) {
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("To0(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);
   }

   // 2022 6 19 - chc �u��T
   if(mode == 0) {
      // �Y�O�b�ɱצ�m, �n���vX/Y/Z
      if(frnMain->IsHeadTilt() == true || frnMain->IsHeadRTilt() == true) {
         booloffset = true;
         zlimit = frnMain->edTMoveZMax->Text.ToInt();
         if(frnMain->IsHeadTilt() == true) {
            dx = frnMain->ed0X->Text.ToInt() - frnMain->ed45X->Text.ToInt();
            dy = frnMain->ed0Y->Text.ToInt() - frnMain->ed45Y->Text.ToInt();
            dz = frnMain->ed0Z->Text.ToInt() - frnMain->ed45Z->Text.ToInt();
            msg.sprintf("Check�b���ɱצ�m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         }
         else {
            dx = frnMain->ed0X->Text.ToInt() - frnMain->edR45X->Text.ToInt();
            dy = frnMain->ed0Y->Text.ToInt() - frnMain->edR45Y->Text.ToInt();
            dz = frnMain->ed0Z->Text.ToInt() - frnMain->edR45Z->Text.ToInt();
            msg.sprintf("Check�b�k�ɱצ�m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         }
         frnMain->WriteSystemLog(msg);
         x += dx;
         y += dx;
         z += dx;
         if(z > zlimit)
            z = zlimit;
      }
   }

   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   frnMain->WriteSystemLog("To0(t) = " + IntToStr(t));
   WaitMotionDone(axisno,10000);

   // 2022 6 19 - chc �u��T: �Y�O�b�ɱצ�m, �n���vX/Y/Z
   if(mode == 0) {
      if(booloffset == true) {
         msg.sprintf("���t���v: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         frnMain->pnlSystemMessage->Caption = msg;
         int oldspeed = frnMain->rgSpeed->ItemIndex;
         frnMain->rgSpeed->ItemIndex = 1;
         DoRelativeMove(X_AXIS, dx);
         DoRelativeMove(Y_AXIS, dy);
         DoRelativeMove(Z_AXIS, dz);
         frnMain->rgSpeed->ItemIndex = oldspeed;
      }
   }

   // XY
   if(mode == 1) {
      axisno = X_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
      frnMain->WriteSystemLog("To0(x) = " + IntToStr(x));

      axisno = Y_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
      frnMain->WriteSystemLog("To0(y) = " + IntToStr(y));
   }

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlTo0Position->Color = clRed;
      if(mode == 1) {
         frnMain->pnlSystemMessage->Caption = "0���I���ʥ���!";
      }
      else {
         frnMain->pnlSystemMessage->Caption = "0���IT���ʥ���!";
      }
   }
   else {

      // 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
      if(mode == 1)
         WaitZFinish(z);

      frnMain->pnlTo0Position->Color = clLime;
      if(mode == 1) {
         frnMain->pnlSystemMessage->Caption = "0���I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      }
      else {
         frnMain->pnlSystemMessage->Caption = "0���IT���ʧ���(t): " + IntToStr(t);
      }
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

}
//---------------------------------------------------------------------------
// ��J���I - T����, X/Z�P�ɲ���
void __fastcall ToLoadPosition()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

// 2022 8 15 - chc �ϥγ̰��t: rgSpeed
int oldspeed;
   oldspeed = frnMain->rgSpeed->ItemIndex;
   frnMain->rgSpeed->ItemIndex = JOG_HIGH_SPEED;

   frnMain->pnlToLoadPosition->Color = clSilver;

   // �n��Edit����
   x = frnMain->edLoadX->Text.ToInt();
   y = frnMain->edLoadY->Text.ToInt();
   z = frnMain->edLoadZ->Text.ToInt();
   t = frnMain->edLoadT->Text.ToInt();

   // 2023 3 14 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      z += frnMain->edWafer8ZDown->Text.ToInt();
   }

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   // 2023 3 16 - chc ���ʨ�̫᭱
   //frnMain->WriteSystemLog("ToLoad(z)�}�l����..." + IntToStr(z));
   //axisno = Z_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   //frnMain->WriteSystemLog("ToLoad(z) = " + IntToStr(z));
   //WaitMotionDone(axisno,10000);
   //frnMain->WriteSystemLog("ToLoad(z)��������." + IntToStr(z));

   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   frnMain->WriteSystemLog("ToLoad(t) = " + IntToStr(t));

   // 2022 8 12 - chc T/X/Y�n�P��
   //WaitMotionDone(axisno,10000);

   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("ToLoad(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("ToLoad(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "�J���I���ʥ���!";
      frnMain->pnlToLoadPosition->Color = clRed;
      frnMain->WriteSystemLog("ToLoad: Fail");
   }
   else {

      // Z
      // 2023 3 16 - chc ���ʨ�̫᭱
      frnMain->WriteSystemLog("ToLoad(z)�}�l����..." + IntToStr(z));
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("ToLoad(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);
      frnMain->WriteSystemLog("ToLoad(z)��������." + IntToStr(z));

      frnMain->WriteSystemLog("ToLoad: Ok, Check Z...");

      // 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
      WaitZFinish(z);
      frnMain->WriteSystemLog("ToLoad: Ok, Check Z done.");

      frnMain->pnlSystemMessage->Caption = "�J���I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlToLoadPosition->Color = clLime;
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

   // 2022 8 15 - chc �ϥγ̰��t: rgSpeed
   frnMain->rgSpeed->ItemIndex = oldspeed;

}
//---------------------------------------------------------------------------
// ��J���I - T����, X/Z�P�ɲ���
void __fastcall ToCCDPosition()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

   frnMain->pnlToCCDPosition->Color = clSilver;

   // �n��Edit����
   x = frnMain->edCCDX->Text.ToInt();
   y = frnMain->edCCDY->Text.ToInt();
   z = frnMain->edCCDZ->Text.ToInt();
   t = frnMain->edCCDT->Text.ToInt();

   // 2023 3 16 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      z += frnMain->edWafer8ZDown->Text.ToInt();
   }

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   // 2023 3 16 - chc ���ʨ�̫᭱
   //axisno = Z_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   //frnMain->WriteSystemLog("ToCCD(z) = " + IntToStr(z));
   //WaitMotionDone(axisno,10000);

   // T
   // => ����T
   //axisno = T_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   //frnMain->WriteSystemLog("ToCCD(t) = " + IntToStr(t));
   //WaitMotionDone(axisno,10000);

   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("ToCCD(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("ToCCD(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "CCD�I���ʥ���!";
      frnMain->pnlToCCDPosition->Color = clRed;
   }
   else {

      // 2023 3 16 - chc ���ʨ�̫᭱
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("ToCCD(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);

      // 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
      WaitZFinish(z);

      frnMain->pnlSystemMessage->Caption = "CCD�I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlToCCDPosition->Color = clLime;
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

}
//---------------------------------------------------------------------------
// Wait Motion X/Y/Z done
bool __fastcall WaitMotionXYZDone(int ms)
{
I32 cmd;
I16 mstatusx,mstatusy,mstatusz;
int counter;
int max,axisno;

   counter = 0;
   max = ms / 10;
   DelayTimeM4(10);

   while(1) {
      mstatusx = 0;
      mstatusy = 0;
      mstatusz = 0;
      I32 done;
      done = APS_motion_status(X_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusx = 1;

      done = APS_motion_status(Y_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusy = 1;

      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusz = 1;
      if(mstatusx == 0 && mstatusy == 0 && mstatusz == 0) {
         break;
      }
      else {
         DelayTimeM4(10);
         counter++;
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion XYZ Done Timeout!";
            return false;
         }
      }
   }
   return true;
}
//---------------------------------------------------------------------------
// Wait Motion X/Y/Z/T done
bool __fastcall WaitMotionXYZTDone(int ms)
{
I32 cmd;
I16 mstatusx,mstatusy,mstatusz,mstatust;
int counter;
int max,axisno;

   counter = 0;
   max = ms / 10;
   DelayTimeM4(10);

   while(1) {
      mstatusx = 0;
      mstatusy = 0;
      mstatusz = 0;
      mstatust = 0;
      I32 done;
      done = APS_motion_status(X_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusx = 1;

      done = APS_motion_status(Y_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusy = 1;

      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusz = 1;

      done = APS_motion_status(T_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatust = 1;

      if(mstatusx == 0 && mstatusy == 0 && mstatusz == 0 && mstatust == 0) {
         break;
      }
      else {
         DelayTimeM4(10);
         counter++;
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion XYZT Done Timeout!";
            return false;
         }
      }
   }
   return true;
}
//---------------------------------------------------------------------------
// ������
// mode 1 - X
//      2 - Y
//      3 - XY(�Ӧ�MoveToXY)
// 2021 5 6 - chc new
void __fastcall WaitMotionDoneXY(int mode, int ms)
{
I32 done;
I16 mstatusx,mstatusy;
int counter;
int max,axisno;

// 2023 5 3a - chc ��sfrmInspection/frmRecipeSet��m
int loop = 0;

   counter = 0;
   max = ms / 10;
   DelayTimeM5(10);

   mstatusx = 0;
   mstatusy = 0;
   if(mode == 1 || mode == 3) {
      mstatusx = 0;
      done = APS_motion_status(X_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusx = 1;
   }
   if(mode == 2 || mode == 3) {
      mstatusy = 0;
      done = APS_motion_status(Y_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatusy = 1;
   }
   while(1) {
      if(mode == 1 || mode == 3) {
         if(mstatusx != 0) {
            mstatusx = 0;
            done = APS_motion_status(X_AXIS+StartAxisNo);
            // ��bit 0 : ������0(Done)
            if((done & 0x01) == 0)
               mstatusx = 1;
         }
      }
      if(mode == 2 || mode == 3) {
         if(mstatusy != 0) {
            mstatusy = 0;
            done = APS_motion_status(Y_AXIS+StartAxisNo);
            // ��bit 0 : ������0(Done)
            if((done & 0x01) == 0)
               mstatusy = 1;
         }
      }
      if(mstatusx == 0 && mstatusy == 0) {
         break;
      }
      else {
         DelayTimeM5(10);
         counter++;
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion XY Done Timeout!";
            break;
         }
      }

      // 2023 5 3a - chc ��sfrmInspection/frmRecipeSet��m
      // 2023 5 6 - chc ���ݭn
      /*
      loop++;
      if(loop > 30) {
         loop = 0;
         if(mode == 3) {
            if(frmInspection->Visible == true && frmInspection->pcInspection->ActivePage == frmInspection->tsMicroInspection) {
               frmInspection->UpdateMotionPosition();
               frnMain->tmISCCDTimer(frnMain);
            }
            else if(frmRecipeSet->Visible == true && frmRecipeSet->pcWaferInformation->ActivePage == frmRecipeSet->tsMicro) {
               frmRecipeSet->UpdateMotionPosition();
               frnMain->tmISCCDTimer(frnMain);
            }
         }
      }
      */

   }
}
//---------------------------------------------------------------------------
// ������X/Y/Z
// mode : ����
void __fastcall WaitMotionDoneXYZ(int mode, int ms)
{
I32 done;
I16 mstatusx,mstatusy,mstatusz;
int counter;
int max,axisno;

   counter = 0;
   max = ms / 10;
   DelayTimeM5(10);

   mstatusx = 0;
   mstatusy = 0;
   mstatusz = 0;
   done = APS_motion_status(X_AXIS+StartAxisNo);
   // ��bit 0 : ������0(Done)
   if((done & 0x01) == 0)
      mstatusx = 1;

   done = APS_motion_status(Y_AXIS+StartAxisNo);
   // ��bit 0 : ������0(Done)
   if((done & 0x01) == 0)
      mstatusy = 1;

   done = APS_motion_status(Z_AXIS+StartAxisNo);
   // ��bit 0 : ������0(Done)
   if((done & 0x01) == 0)
      mstatusz = 1;
   while(1) {
      if(mstatusx != 0) {
         mstatusx = 0;
         done = APS_motion_status(X_AXIS+StartAxisNo);
         // ��bit 0 : ������0(Done)
         if((done & 0x01) == 0)
            mstatusx = 1;
      }

      if(mstatusy != 0) {
         mstatusy = 0;
         done = APS_motion_status(Y_AXIS+StartAxisNo);
         // ��bit 0 : ������0(Done)
         if((done & 0x01) == 0)
            mstatusy = 1;
      }

      if(mstatusz != 0) {
         mstatusz = 0;
         done = APS_motion_status(Z_AXIS+StartAxisNo);
         // ��bit 0 : ������0(Done)
         if((done & 0x01) == 0)
            mstatusz = 1;
      }
      if(mstatusx == 0 && mstatusy == 0 && mstatusz == 0) {
         break;
      }
      else {
         DelayTimeM5(10);
         counter++;
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion XY Done Timeout!";
            break;
         }
      }
   }
}
//---------------------------------------------------------------------------
// 2013 2 17 - chc io status
// 2021 5 6 - chc new
void __fastcall DecodeIOStatus(int axisno,U16 io_sts)
{
TPanel *rdy,*alm,*pel,*nel,*org,*dir,*reserved1,*pcs,*erc,*ez,*reserved2,*latch,*sd,*inp,*svon;
AnsiString name,msg;

// 2023 2 11 - chc ���ɶ�ALM�~�OError
static int xalmcnt = 0;
static int yalmcnt = 0;
static int zalmcnt = 0;
static int talmcnt = 0;
int max = 100;

   GetAxisName(axisno);
   name = frnMain->AxisName;

   rdy       = (TPanel *)(frnMain->FindComponent("pnlRDY" + name));
   alm       = (TPanel *)(frnMain->FindComponent("pnlALM" + name));
   pel       = (TPanel *)(frnMain->FindComponent("pnlPEL" + name));
   nel       = (TPanel *)(frnMain->FindComponent("pnlNEL" + name));
   org       = (TPanel *)(frnMain->FindComponent("pnlORG" + name));
   dir       = (TPanel *)(frnMain->FindComponent("pnlDIR" + name));
   reserved1 = (TPanel *)(frnMain->FindComponent("pnlReserved1" + name));
   pcs       = (TPanel *)(frnMain->FindComponent("pnlPCS" + name));
   erc       = (TPanel *)(frnMain->FindComponent("pnlERC" + name));
   ez        = (TPanel *)(frnMain->FindComponent("pnlEZ" + name));
   reserved2 = (TPanel *)(frnMain->FindComponent("pnlReserved2" + name));
   latch     = (TPanel *)(frnMain->FindComponent("pnlLatch" + name));
   sd        = (TPanel *)(frnMain->FindComponent("pnlSD" + name));
   inp       = (TPanel *)(frnMain->FindComponent("pnlINP" + name));
   svon      = (TPanel *)(frnMain->FindComponent("pnlSVON" + name));

   // ALM       - 0     0x0001          *
   // PEL       - 1     0x0002  *
   // NEL       - 2     0x0004  *
   // ORG       - 3     0x0008  *
   // EMG       - 4     0x0010
   // EZ        - 5     0x0020  *
   // INP       - 6     0x0040  *
   // SVON      - 7     0x0080  *
   // RDY       - 8     0x0100          *
   // DIR       - 9     0x0200  *
   // PCS       - 10    0x0400  *
   // ERC       - 11    0x0800  *
   // Reserved2 - 12    0x1000
   // Latch     - 13    0x2000  *
   // SD        - 14    0x4000  *
   // ALM
//   if(axisno == X_AXIS || axisno == Y_AXIS || axisno == Z_AXIS) {
      if(io_sts & 0x0001) {

         // 2023 2 11 - chc ���ɶ�ALM�~�OError
         if(axisno == X_AXIS) {
            xalmcnt++;
            if(xalmcnt < max)
               goto chrdy;
         }
         else if(axisno == Y_AXIS) {
            yalmcnt++;
            if(yalmcnt < max)
               goto chrdy;
         }
         else if(axisno == Z_AXIS) {
            zalmcnt++;
            if(zalmcnt < max)
               goto chrdy;
         }
         else if(axisno == T_AXIS) {
            talmcnt++;
            if(talmcnt < max)
               goto chrdy;
         }

         alm->Color = clRed;
         // �b�����`!
         if(frnMain->pnlAxisError->Visible == false) {
            AxisErrorNo = axisno;
            msg = AnsiString("Axis-") + name + "���`! Motion�L�k�@��.";
            frnMain->pnlAxisError->Caption = msg;
            //frnMain->pnlAxisError->Visible = true;
            frnMain->pnlAlarm->Caption = frnMain->pnlAxisError->Caption;
            frnMain->pnlAlarm->Font->Color = clRed;
            frnMain->MotionStatus(false);
            if(axisno == X_AXIS) {
               frnMain->AddMainLog("X-Axis Move Error!", XAXIS_MOVE_ERR,true);
            }
            else if(axisno == Y_AXIS)
               frnMain->AddMainLog("Y-Axis Move Error!", YAXIS_MOVE_ERR,true);
            else if(axisno == Z_AXIS)
               frnMain->AddMainLog("Z-Axis Move Error!", ZAXIS_MOVE_ERR,true);
            else if(axisno == T_AXIS)
               frnMain->AddMainLog("T-Axis Move Error!", TAXIS_MOVE_ERR,true);

            // 2023 4 27a - chc ����
            //SetRedLamp(true);
            //Beep(500);

            // 2023 4 28 - chc �즸
            if(frnMain->pnl7856Status->Color != clRed) {

               frnMain->pnl7856Status->Color = clRed;
               frnMain->WriteSystemLog(">>Set 7856 RED - 5");
               frnMain->pnlPCI7856->Color = clRed;
               frnMain->shMotionStatus->Brush->Color = clRed;
            }
         }
      }
      else {

         // 2023 2 11 - chc ���ɶ�ALM�~�OError
         if(axisno == X_AXIS) {
            xalmcnt = 0;
         }
         else if(axisno == Y_AXIS) {
            yalmcnt = 0;
         }
         else if(axisno == Z_AXIS) {
            zalmcnt = 0;
         }
         else if(axisno == T_AXIS) {
            talmcnt = 0;
         }

         alm->Color = clSilver;
         // �b�����`! ����Alarm Message
         if(frnMain->pnlAxisError->Visible == true && axisno == AxisErrorNo) {
            frnMain->pnlAxisError->Visible = false;
            frnMain->MotionStatus(true);
            SetRedLamp(false);
            frnMain->pnl7856Status->Color = clLime;
            frnMain->pnlPCI7856->Color = clLime;
            frnMain->shMotionStatus->Brush->Color = clLime;
         }
      }
//   }
   // T/R: �Ϭ�
/*
   else {
      if((io_sts & 0x0001) == 0) {
         alm->Color = clRed;
         // �b�����`!
         if(frnMain->pnlAxisError->Visible == false) {
            AxisErrorNo = axisno;
            msg = AnsiString("Axis-") + name + "���`! Motion�L�k�@��.";
            frnMain->pnlAxisError->Caption = msg;
            frnMain->pnlAxisError->Visible = true;
            frnMain->MotionStatus(false);
            SetRedLamp(true);
            Beep(500);
            frnMain->pnl7856Status->Color = clRed;
            frnMain->pnlPCI7856->Color = clRed;
            frnMain->shMotionStatus->Brush->Color = clRed;
         }
      }
      else {
         alm->Color = clSilver;
         // �b�����`! ����Alarm Message
         if(frnMain->pnlAxisError->Visible == true && axisno == AxisErrorNo) {
            frnMain->pnlAxisError->Visible = false;
            frnMain->MotionStatus(true);
            SetRedLamp(false);
            frnMain->pnl7856Status->Color = clLime;
            frnMain->pnlPCI7856->Color = clLime;
            frnMain->shMotionStatus->Brush->Color = clLime;
         }
      }
   }
*/

chrdy:
   // RDY
   //if(axisno == X_AXIS || axisno == Y_AXIS || axisno == Z_AXIS) {
      if(io_sts & 0x0100)
         rdy->Color = clRed;
      else
         rdy->Color = clSilver;
   //}
   //// T/R: �Ϭ�
   //else {
   //   if((io_sts & 0x0100) == 0)
   //      rdy->Color = clRed;
   //   else
   //      rdy->Color = clSilver;
   //}

   // PEL
   if(io_sts & 0x0002)
      pel->Color = clRed;
   else
      pel->Color = clSilver;
   // NEL
   if(io_sts & 0x0004)
      nel->Color = clRed;
   else
      nel->Color = clSilver;
   // ORG
   if(io_sts & 0x0008)
      org->Color = clRed;
   else
      org->Color = clSilver;
   // DIR
   if(io_sts & 0x0200)
      dir->Color = clRed;
   else
      dir->Color = clSilver;
   // Reserved1
   //if(io_sts & 0x0040)
   //   reserved1->Color = clRed;
   //else
   //   reserved1->Color = clSilver;
   // PCS
   if(io_sts & 0x0400)
      pcs->Color = clRed;
   else
      pcs->Color = clSilver;
   // ERC
   if(io_sts & 0x0800)
      erc->Color = clRed;
   else
      erc->Color = clSilver;
   // EZ
   if(io_sts & 0x0020)
      ez->Color = clRed;
   else
      ez->Color = clSilver;
   // Reserved2
   //if(io_sts & 0x0400)
   //   reserved2->Color = clRed;
   //else
   //   reserved2->Color = clSilver;
   // Latch
   if(io_sts & 0x2000)
      latch->Color = clRed;
   else
      latch->Color = clSilver;
   // SD
   if(io_sts & 0x4000)
      sd->Color = clRed;
   else
      sd->Color = clSilver;
   // INP
   if(io_sts & 0x0040)
      inp->Color = clRed;
   else
      inp->Color = clSilver;
      
   // SVON
   if(io_sts & 0x0080)
      svon->Color = clRed;
   else
      svon->Color = clSilver;
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall GetAxisPosition(int axisno, int *position)
{

   APS_get_position(axisno+StartAxisNo,(I32 *)position);
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
bool __fastcall MoveAbsolute(int axisno)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
TLMDEdit* edit;
AnsiString name;
I16 mstatus;
int counter,max,ms,ret;

   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // ���oPosition
   // 0 - 7(12 - 19)
   GetAxisName(axisno);
   name = frnMain->AxisName;
   edit = (TLMDEdit *)(frnMain->FindComponent("edPosition" + name));
   pos = edit->Text.ToInt();

   // 2023 1 15 - chc Check Limit
   CheckLimitAxis(axisno,&pos);

   // ���]��
   //if(pos > uplimit)
   //   pos = uplimit;

   // �[�JLow Distance
   ret = APS_absolute_move(axisno+StartAxisNo,(int)pos,(int)maxvel);

   // ��Done: 10Sec
   ms = 10000;
   counter = 0;
   max = ms / 10;
   DelayTimeM5(10);
   while(1) {
      mstatus = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         mstatus = 1;
      if(mstatus == 0) {
         break;
      }
      else {
         DelayTimeM5(10);
         frnMain->pnlCounter->Caption = counter;
         counter++;
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "��Motion Done����!";
            return false;
         }
      }
   }
   return true;
}
//---------------------------------------------------------------------------
// 2021 5 6 - chc new
void __fastcall StopAxis(int axisno)
{

   APS_stop_move(axisno+StartAxisNo);
}
//---------------------------------------------------------------------------
// 0 - 7(12 - 19)
// 2021 5 6 - chc new
void __fastcall GetAxisName(int axisno)
{
AnsiString name;

   switch(axisno) {
      // X
      case X_AXIS:
         name = "X";
         break;
      // Y
      case Y_AXIS:
         name = "Y";
         break;
      // Z
      case Z_AXIS:
         name = "Z";
         break;
      // T
      case T_AXIS:
         name = "T";
         break;
      // R
      case R_AXIS:
         name = "R";
         break;
   }
   frnMain->AxisName = name;
}
//---------------------------------------------------------------------------
// �]�wAlm Logic: 4&6-High, �l-Low
// 2021 5 16 - chc New
void __fastcall SetAlmOn()
{
I16 axisno;

   // ���wAxis
   axisno = frnMain->combAxis->ItemIndex;

   APS_set_axis_param(axisno+StartAxisNo, PRA_ALM_LOGIC, 1);         // High
}
//---------------------------------------------------------------------------
// �]�wAlm Logic: 4&6-High, �l-Low
// 2021 5 16 - chc New
void __fastcall SetAlmOff()
{
I16 axisno;

   // ���wAxis
   axisno = frnMain->combAxis->ItemIndex;

   APS_set_axis_param(axisno+StartAxisNo, PRA_ALM_LOGIC, 0);         // High
}
//---------------------------------------------------------------------------
// Set Servo On
// 2021 5 16 - chc New
void __fastcall SetServoOn()
{
int axisno;

   // ���wAxis
   axisno = frnMain->combAxis->ItemIndex;

   APS_set_servo_on(axisno+StartAxisNo, 1);
}
//---------------------------------------------------------------------------
// Set Servo Off
// 2021 5 16 - chc New
void __fastcall SetServoOff()
{
int axisno;

   // ���wAxis
   axisno = frnMain->combAxis->ItemIndex;

   APS_set_servo_on(axisno+StartAxisNo, 0);
}
//---------------------------------------------------------------------------
// �e�i: �n�P�wLimit���D
// 2021 5 16 - chc New
void __fastcall MoveForward()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;

   // �bMouseDown���w�@�ʶb
   axisno = frnMain->combAxis->ItemIndex;

   // T�Y�b�M�I�ϰ줣�i����
   if(axisno == T_AXIS) {
      if(CheckTMove() == false) {
         frnMain->pnlSystemMessage->Caption = "Z��m����, �L�k����T�b����";
         // Beep�n
         Beep(500);
         return;
      }
   }
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   // Disable
   frnMain->btnMove->Enabled = false;

   // �W���ˬd&�վ�
   CheckUpDownLimit(axisno,&dist,uplimit);

   // @@ test
   frnMain->WriteSystemLog("MoveForward() Dist = " + IntToStr(dist));

   // ����
   ret = APS_relative_move(axisno+StartAxisNo,dist,maxvel);

   // ��Motion done
   frnMain->pnlSystemMessage->Caption = "��Motion Done...";
   int counter = 1;
   I32 cmd;
   I16 Motion_sts;
   DelayTimeM5(100);
   while(1) {

      Motion_sts = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_sts = 1;

      if(Motion_sts == 0) {
         break;
      }
      else {
         DelayTimeM5(50);
         frnMain->pnlCounter->Caption = counter;
         counter++;
         // 10sec: 50*200
         if(counter > 200) {
            frnMain->pnlSystemMessage->Caption = "��Motion Done����!";
            return;
         }
      }
   }
   frnMain->btnMove->Enabled = true;
}
//---------------------------------------------------------------------------
// �U�b�W�U���ˬd
// dist: �۹ﲾ�ʶq
// 2021 5 16 - chc New
void __fastcall CheckUpDownLimit(int axisno,int *dist, int uplimit)
{
I32 posi;
int down,pos;

   // 2023 2 16 - chc ��_�ˬdLimit
   APS_get_position(axisno+StartAxisNo,&posi);
   pos = posi + *dist;
   CheckLimitAxis(axisno,&pos);
   *dist = pos - posi;
   return;

   // ���ˬd����
   return;

   // �u��X�b�γ]��
   if(axisno != X_AXIS)
      return;

   // 2021 7 6b - chc Limit of X-Axis
   // 2021 9 2 - chc ���n�]��
   //if(axisno == X_AXIS) {
   //   // 2mm
   //   uplimit = 2200000 - 20000;
   //   down = 84285 + 20000;
   //}

   APS_get_position(axisno+StartAxisNo,&posi);
   posi = posi + *dist;
   if(posi < 0)
      *dist = 0 - posi;
   else if(posi > uplimit)
      *dist = uplimit - posi;
}
//---------------------------------------------------------------------------
// ��h, �n�P�wLimit���D
// 2021 5 16 - chc New
void __fastcall MoveBackward()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
int axisno;

   // �s��
   if(frnMain->cbContinueMove->Checked == true)
      return;

   // �bMouseDown�����w�@�ʶb
   axisno = frnMain->combAxis->ItemIndex;

   // T�Y�b�M�I�ϰ줣�i����
   if(axisno == T_AXIS) {
      if(CheckTMove() == false) {
         frnMain->pnlSystemMessage->Caption = "Z��m����, �L�k����T�b����";
         // Beep�n
         Beep(500);
         return;
      }
   }
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   // Disable
   frnMain->btnBackward->Enabled = false;

   // �W���ˬd
   dist = 0 - dist;
   CheckUpDownLimit(axisno,&dist,uplimit);
   int ret = APS_relative_move(axisno+StartAxisNo,dist,maxvel);

   // ��Motion done
   frnMain->pnlSystemMessage->Caption = "��Motion Done...";
   int counter = 1;
   I32 cmd;
   I16 Motion_sts;

   // 2013 5 21 - chc �Τ@Sleep(1)
   //Sleep(100);
   DelayTimeM5(100);

   while(1) {
      Motion_sts = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_sts = 1;

      if(Motion_sts == 0) {
         break;
      }
      else {
         DelayTimeM5(50);
         frnMain->pnlCounter->Caption = counter;
         counter++;
         // 10sec: 50*200
         if(counter > 200) {
            frnMain->pnlSystemMessage->Caption = "��Motion Done����!";
            return;
         }
      }
   }
   frnMain->btnBackward->Enabled = true;
}
//---------------------------------------------------------------------------
// Position
// 2021 5 16 - chc New
void __fastcall GetPosition(int axisno, I32 *pos)
{

   APS_get_position(axisno+StartAxisNo,pos);
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc New
void __fastcall DoAxisTaMove(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec)
{

   int ret = AbsoluteMove(axisno,pos,startvel,maxvel,tacc,tdec);
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc New
void __fastcall DoAxisTrMove(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec)
{

   frnMain->WriteSystemLog("DoAxisTrMove: " + IntToStr(axisno) + "," + IntToStr(pos));

   int ret = RelativeMove(axisno,pos,startvel,maxvel,tacc,tdec);
}
//---------------------------------------------------------------------------
// Command
// 2021 5 16 - chc New
void __fastcall GetCommand(int axisno, I32 *cmd)
{

   APS_get_command(axisno+StartAxisNo,cmd);
}
//---------------------------------------------------------------------------
// 2021 8 17a - chc Write DO
void __fastcall WriteDO(int idno)
{
TCheckBox *bitcheck;
unsigned int bit,value;
int ret;

// 2023 5 8 - chc first do
static bool first = true;

   if(boolDO != true) {

      // 2023 5 8 - chc first do
      if(first == true) {
         first = false;

         if(frnMain->boolForTest == false) {
            frnMain->pnlSystemMessage->Caption = "DO Device Fail!";
            frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
         }
      }
      DOValue1 = 0;
      DOValue2 = 0;
      DOValue3 = 0;
      return;
   }

   bit = 0x01;
   value = 0;
   if(idno == IO1_ID) {
      for(int i=0 ; i<16 ; i++) {
         bitcheck = (TCheckBox *)(frnMain->FindComponent("cbBit" + IntToStr(i)));
         if(bitcheck->Checked == true)
            value |= bit;
         bit = bit << 1;
      }
      DOValue1 = value;
      ret = APS_set_field_bus_d_output(BoardHSL, HSL_BUS, IO1_ID, DOValue1);
   }
   else if(idno == IO2_ID) {
      for(int i=0 ; i<16 ; i++) {
         bitcheck = (TCheckBox *)(frnMain->FindComponent("cbID3Bit" + IntToStr(i)));
         if(bitcheck->Checked == true)
            value |= bit;
         bit = bit << 1;
      }
      DOValue2 = value;
      ret = APS_set_field_bus_d_output(BoardHSL, HSL_BUS, IO2_ID, DOValue2);
   }
   else if(idno == IO3_ID) {
      for(int i=0 ; i<32 ; i++) {
         bitcheck = (TCheckBox *)(frnMain->FindComponent("cbID5Bit" + IntToStr(i)));
         if(bitcheck->Checked == true)
            value |= bit;
         bit = bit << 1;
      }
      DOValue3 = value;
      ret = APS_set_field_bus_d_output(BoardHSL, HSL_BUS, IO3_ID, DOValue3);
   }

   if(ret != ERR_NoError) {
      frnMain->pnlSystemMessage->Caption = "DO Write Fail!";
   }
}
//---------------------------------------------------------------------------
// 2013 2 22 - chc Wait Routine : �b��
// 2021 5 16 - chc New ok
void __fastcall WaitRoutine1(int sleepno)
{
static bool active = false;

   if(active == false) {
      active = true;
      DelayTimeM10(sleepno);
      active = false;
   }
}
//---------------------------------------------------------------------------
// Command
// 2021 5 16 - chc New
void __fastcall SetCommand(int axisno, I32 cmd)
{

   APS_set_command(axisno+StartAxisNo,cmd);
}
//---------------------------------------------------------------------------
// Position
// 2021 5 16 - chc New
void __fastcall SetMPosition(int axisno, F64 pos)
{
I32 ipos;

   ipos = pos;
   APS_set_position(axisno+StartAxisNo,ipos);
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc New
void __fastcall GetIOStatus(int axisno, U16 *iostatus)
{
I32 istatus;

   istatus = APS_motion_io_status(axisno+StartAxisNo);
   *iostatus = istatus;
}
//---------------------------------------------------------------------------
// 2013 2 23 - chc DO Reset 0
// 2021 5 16 - chc New ok
void __fastcall InitDO()
{
TCheckBox *bitcheck;
unsigned int bit;

   if(boolDO != true) {
      if(frnMain->boolForTest == false)
         frnMain->pnlSystemMessage->Caption = "DO Device Fail!";
      return;
   }

   bit = 0x01;
   // Bit 0,4(MNET��DO�O�Ϭ�)
   DOValue1 = 0x0;
   for(int i=0 ; i<16 ; i++) {
      bitcheck = (TCheckBox *)(frnMain->FindComponent("cbBit" + IntToStr(i)));
      if((DOValue1 & bit) != 0)
         bitcheck->Checked = true;
      else
         bitcheck->Checked = false;
      bit = bit << 1;
   }

   frnMain->pnlDOValue->Caption = DOValue1;

   // �]�w��0
   WriteDO(IO1_ID);

   // ID3
   bit = 0x01;
   // Bit 0,4(MNET��DO�O�Ϭ�)
   DOValue2 = 0x0;
   for(int i=0 ; i<16 ; i++) {
      bitcheck = (TCheckBox *)(frnMain->FindComponent("cbID3Bit" + IntToStr(i)));
      if((DOValue2 & bit) != 0)
         bitcheck->Checked = true;
      else
         bitcheck->Checked = false;
      bit = bit << 1;
   }
   frnMain->pnlDOValue->Caption = DOValue2;
   // �]�w��0
   WriteDO(IO2_ID);

   // ID5
   bit = 0x01;
   // Bit 0,4(MNET��DO�O�Ϭ�)
   DOValue3 = 0x0;
   for(int i=0 ; i<32 ; i++) {
      bitcheck = (TCheckBox *)(frnMain->FindComponent("cbID5Bit" + IntToStr(i)));
      if((DOValue3 & bit) != 0)
         bitcheck->Checked = true;
      else
         bitcheck->Checked = false;
      bit = bit << 1;
   }
   frnMain->pnlDOValue->Caption = DOValue3;
   // �]�w��0
   WriteDO(IO3_ID);

}
//---------------------------------------------------------------------------
// Bit20
// 2021 5 16 - chc new
void __fastcall Beep(int mstime)
{
unsigned int value;

   // Beep
   frnMain->AlarmSound();
   //frnMain->cbID5Bit15->Checked = true;
   //WriteDO(IO3_ID);
   frnMain->cbBit15->Checked = true;
   WriteDO(IO1_ID);
   if(mstime > 0) {
      frnMain->tmBeep->Interval = mstime;
      frnMain->tmBeep->Enabled = true;
   }
}
//---------------------------------------------------------------------------
// Bit20
// 2021 5 16 - chc new
void __fastcall BeepStop()
{
unsigned int value;

   // 2023 4 27 - chc ����
   return;

   // 2013 9 28 - chc Beep
   frnMain->tmBeep->Enabled = false;
   //frnMain->cbID5Bit15->Checked = false;
   //WriteDO(IO3_ID);
   frnMain->cbBit15->Checked = false;
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// 2013 5 20 - chc Sleep : WaitMotionDone()�ϥ�
// 2021 5 16 - chc new ok
void __fastcall DelayTimeM2(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2013 5 21 - chc Sleep : AllHome()�ϥ�
// 2021 5 16 - chc new ok
void __fastcall DelayTimeM3(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2013 5 21 - chc Sleep : WaitMotionXYZDone()�ϥ�
// 2021 5 16 - chc new ok
void __fastcall DelayTimeM4(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2013 5 21 - chc Sleep : WWaitMotionDoneXY(), WaitMotionDoneXYZ(), WaitMotionDLFDone�ϥ�
// 2021 5 16 - chc new ok
void __fastcall DelayTimeM5(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   GetTimeTicM(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      // ���@��, �ťX�ɶ���CCD
      Sleep(1);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         GetTimeTicM(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2013 5 21 - chc Sleep : WaitMotionXYDoneForScan�ϥ�
// 2021 5 16 - chc new ok
void __fastcall DelayTimeM10(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2013 9 27 - chc Z�babsolute����
// 2021 5 16 - chc new
// 2022 6 16a - chc �[�Jmode: 0 - wait, 1 - nowait
//bool __fastcall MoveToZ(int position, int speed)
bool __fastcall MoveToZ(int position, int speed, int mode)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = Z_AXIS;

   // �O���U�b�O�_�B�@��
   if(frnMain->iAxisMove[axisno] != NO_MOVE)
      return false;
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // Z Limit
   int limit = frnMain->edMinZPosition->Text.ToInt();
   if(position < limit)
      position = limit;
   limit = frnMain->edMaxZPosition->Text.ToInt();

   // 2023 3 14 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      limit += frnMain->edWafer8ZDown->Text.ToInt();
   }

   if(position > limit)
      position = limit;

   pos = position;
   maxvel = speed;
   startvel = speed / 5;
   ret = APS_absolute_move(axisno+StartAxisNo,(int)pos,(int)maxvel);

   // no wait
   if(mode == 1)
      return true;

   // timeout from 5sec to 30sec : MOTION_TIMEOUT
   // 2023 2 18 - chc �n����XY/Z/W
   //if(WaitMotionDone(axisno, MOTION_TIMEOUT) == false) {
   if(WaitMotionDoneZ(axisno, MOTION_TIMEOUT) == false) {

      // �O���U�b�O�_�B�@��
      frnMain->iAxisMove[axisno] = NO_MOVE;
      return false;
   }

   // �O���U�b�O�_�B�@��
   frnMain->iAxisMove[axisno] = NO_MOVE;
   return true;
}
//---------------------------------------------------------------------------
// Y�babsolute����
bool __fastcall MoveToY(int position)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = Y_AXIS;

   // �O���U�b�O�_�B�@��
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToY: Y in moving!");
      return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // 2022 8 14 - chc Y Limit
   int limit = frnMain->edMinYPosition->Text.ToInt();
   if(position < limit)
      position = limit;
   limit = frnMain->edMaxYPosition->Text.ToInt();
   if(position > limit)
      position = limit;

   pos = position;

   ret = APS_absolute_move(axisno+StartAxisNo,pos,maxvel);
   if(WaitMotionDone(axisno, 5000) == false) {
      frnMain->iAxisMove[axisno] = NO_MOVE;
      frnMain->WriteSystemLog("MoveToY: Y Timeout!");
      return false;
   }

   frnMain->iAxisMove[axisno] = NO_MOVE;
   return true;
}
//---------------------------------------------------------------------------
// T�babsolute����
bool __fastcall MoveToT(int position)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = T_AXIS;
   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   pos = position;

   // 2022 8 14 - chc Y Limit
   int limit = frnMain->edMinTPosition->Text.ToInt();
   if(pos < limit)
      pos = limit;
   limit = frnMain->edMaxTPosition->Text.ToInt();
   if(pos > limit)
      pos = limit;

   ret = APS_absolute_move(axisno+StartAxisNo,pos,maxvel);
   if(WaitMotionDone(axisno, 10000) == false) {
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
// ���e����90��, �W����95��
void __fastcall MoveToTAdd90()
{
I16 axisno;
I32 ipos;
double degree;

   axisno = T_AXIS;
   APS_get_position(axisno+StartAxisNo,&ipos);

   // �ϥ��ܼ� : edTResolution 0.001��/Pulse
   ipos += (90.0 / frnMain->edTResolution->Text.ToDouble());
   MoveToT(ipos);
}
//---------------------------------------------------------------------------
// ���Ჾ��90��, �U����-5��
void __fastcall MoveToTSub90()
{
I16 axisno;
I32 ipos;

   axisno = T_AXIS;
   APS_get_position(axisno+StartAxisNo,&ipos);

   // �ϥ��ܼ� : edTResolution 0.001��/Pulse
   ipos -= (90.0 / frnMain->edTResolution->Text.ToDouble());

   MoveToT(ipos);
}
//---------------------------------------------------------------------------
// Timer����
void __fastcall GetTimeTicM(long *time, short *millitm)
{
struct timeb t;

   ftime(&t);
   *time = t.time;
   *millitm = t.millitm;
}
//---------------------------------------------------------------------------
// ��X��AOI�I
void __fastcall ToXAOIPosition()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
int axisno;
int x;

   frnMain->MotionStatus(false);
   // �n��Edit����
   x = frnMain->edCenterX->Text.ToInt();

   // 2022 8 14 - chc X Limit
   int limit = frnMain->edMinXPosition->Text.ToInt();
   if(x < limit)
      x = limit;
   limit = frnMain->edMaxXPosition->Text.ToInt();
   if(x > limit)
      x = limit;

   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   int ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);

   WaitMotionDoneXY(1,10000);
   frnMain->MotionStatus(true);
}
//---------------------------------------------------------------------------
// ����X
bool __fastcall MoveToXGap(int dx)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = X_AXIS;
   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   pos = dx;

   // UpDown Limmit
   CheckUpDownLimit(axisno,&pos,uplimit);

   ret = APS_relative_move(axisno+StartAxisNo,pos,maxvel);
   if(WaitMotionDone(axisno, 5000) == false) {
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
// X�babsolute����
bool __fastcall MoveToX(int position)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = X_AXIS;

   // �O���U�b�O�_�B�@��
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToX: X in moving!");
      return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // 2022 8 14 - chc X Limit
   int limit = frnMain->edMinXPosition->Text.ToInt();
   if(position < limit)
      position = limit;
   limit = frnMain->edMaxXPosition->Text.ToInt();
   if(position > limit)
      position = limit;

   pos = position;

   ret = APS_absolute_move(axisno+StartAxisNo,pos,maxvel);
   if(WaitMotionDone(axisno, 5000) == false) {
      frnMain->iAxisMove[axisno] = NO_MOVE;
      frnMain->WriteSystemLog("MoveToX: X Timeout!");
      return false;
   }

   frnMain->iAxisMove[axisno] = NO_MOVE;
   return true;
}
//---------------------------------------------------------------------------
// T����w���Ҷq: Z������
// return: true - ok, false - ���w��
bool __fastcall CheckTMove()
{
I32 ipos;
int axisno,zpos;

   // 2022 7 5 - chc ����
   return true;

   // Z: 0 - 16465
   axisno = Z_AXIS;
   APS_get_position(axisno+StartAxisNo,&ipos);
   zpos = ipos;
   // Danger
   if(zpos > frnMain->edTMoveZMax->Text.ToInt()) {
      frnMain->WriteSystemLog(">Motion : Z�L�C, T���i�H�ʧ@! " + IntToStr(zpos) + "," + frnMain->edTMoveZMax->Text);
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
// Ū��Y�b�ثe��m
int __fastcall GetYPosition()
{
int axisno;
I32 ipos;

   axisno = Y_AXIS;
   APS_get_position(axisno+StartAxisNo,&ipos);
   return ipos;
}
//---------------------------------------------------------------------------
// ret: true or false
bool __fastcall GetNELStatus(int axisno)
{

   I32 io_sts;
   io_sts = APS_motion_io_status(axisno+StartAxisNo);
   // NEL(Low/Inverse) - 2
   if((io_sts & 0x0004) != 0) {
      return true;
   }
   else {
      return false;
   }
   return false;
}
//---------------------------------------------------------------------------
// ret: ture or false
bool __fastcall GetMotionDoneStatus(int axisno)
{

   I32 mstatus;
   mstatus = APS_motion_status(axisno+StartAxisNo);
   if((mstatus & 0x01) != 0)
      return true;
   else
      return false;
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc Backlash Control
// ret : 0-Ok, -1:Fail
int __fastcall RelativeMove(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec)
{
I16 ret;

   frnMain->WriteSystemLog("RelativeMove: " + IntToStr(axisno) + "," + IntToStr(pos) + "," + IntToStr(maxvel));

   ret = APS_relative_move(axisno+StartAxisNo,(int)pos,(int)maxvel);

   // 2023 2 18 - chc �n����XY/Z/W
   //if(WaitMotionDone(axisno, 10000) == false)
   bool flag;
   if(axisno == Z_AXIS)
      flag = WaitMotionDoneZ(axisno, 15000);
   else if(axisno == R_AXIS)
      flag = WaitMotionDoneW(axisno, 15000);
   else
      flag = WaitMotionDone(axisno, 15000);
   if(flag == false)

      ret = -1;
   else
      ret = 0;
   return ret;
}
//---------------------------------------------------------------------------
// 2021 5 16 - chc Backlash Control
int __fastcall AbsoluteMove(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec)
{
I16 ret;

   // 2023 1 15 - chc Check Limit
   CheckLimitAxis(axisno,&pos);

   ret = APS_absolute_move(axisno+StartAxisNo,(int)pos,(int)maxvel);

   // 2023 2 11 - chc �令15sec
   //if(WaitMotionDone(axisno, 10000) == false)
   // 2023 2 18 - chc �n����XY/Z/W
   //if(WaitMotionDone(axisno, 15000) == false)
   bool flag;
   if(axisno == Z_AXIS)
      flag = WaitMotionDoneZ(axisno, 15000);
   else if(axisno == R_AXIS)
      flag = WaitMotionDoneW(axisno, 15000);
   else
      flag = WaitMotionDone(axisno, 15000);
   if(flag == false)

      ret = -1;
   else
      ret = 0;
   return ret;
}
//---------------------------------------------------------------------------
void __fastcall SetACC_DEC_StartV(int axisno, double acc, double dec, int startv, int maxvel)
{
I32 iacc, idec, istartv;

   iacc = (double)maxvel / acc;
   idec = (double)maxvel / dec;
   istartv = startv;
   APS_set_axis_param(axisno+StartAxisNo, 0x21, iacc);
   APS_set_axis_param(axisno+StartAxisNo, 0x22, idec);
   APS_set_axis_param(axisno+StartAxisNo, 0x23, istartv);
}
//---------------------------------------------------------------------------
// 2021 6 8 - chc X�babsolute����, Not Wait
// mode: true, Wait
//       false, NoWait
bool __fastcall MoveToXNoWait(int position,int speed, int mode)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = X_AXIS;
   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   pos = position;

   // 2023 1 15 - chc Check Limit
   CheckLimitAxis(axisno,&pos);

   maxvel = speed;

   ret = APS_absolute_move(axisno+StartAxisNo,pos,maxvel);

   if(mode == true) {
      if(WaitMotionDone(axisno, 5000) == false) {
         return false;
      }
   }
   return true;
}
//---------------------------------------------------------------------------
// 2020 3 17 - chc �ߧY����
void __fastcall StopOneAxis(int axisno)
{

   APS_stop_move(axisno+StartAxisNo);
}
//---------------------------------------------------------------------------
// 2021 8 17 - chc Wait Motion done - for CCD Capture
bool __fastcall WaitMotionDoneCCD(int axisno, int ms)
{
I32 cmd;
I16 Motion_sts;
int counter;
int max;

   max = ms / 10;
   DelayTimeM4(10);
   counter = 0;

   while(1) {

      I32 done;
      Motion_sts = 0;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_sts = 1;
      if(Motion_sts == 0) {
         break;
      }
      else {
         frnMain->pnlCounter->Caption = counter;
         counter++;
         // 10sec: 50*200
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "��Motion Done����!";
            return false;
         }
      }
      DelayTimeM4(10);
   }
   return true;
}
//---------------------------------------------------------------------------
// 2021 8 17a - chc New
void __fastcall DoAxisTrMoveNoWait(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec)
{

   if(axisno == Y_AXIS)
      return;

   frnMain->WriteSystemLog("DoAxisTrMoveNoWait: " + IntToStr(axisno) + "," + IntToStr(pos));

   int ret = RelativeMoveNoWait(axisno,pos,startvel,maxvel,tacc,tdec);
}
//---------------------------------------------------------------------------
// 2021 8 17a - chc no wait
// ret : 0-Ok, -1:Fail
int __fastcall RelativeMoveNoWait(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec)
{
I16 ret;

   if(axisno == Y_AXIS)
      return -1;

   frnMain->WriteSystemLog("RelativeMoveNoWait: " + IntToStr(axisno) + "," + IntToStr(pos) + "," + IntToStr(maxvel));

   ret = APS_relative_move(axisno+StartAxisNo,(int)pos,(int)maxvel);

   return ret;
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc ��Original�I : Z/T/XY
void __fastcall ToOriginalPosition()
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

   frnMain->pnlToOriginalPosition->Color = clSilver;

   // �n��Edit����
   x = frnMain->edOriginalX->Text.ToInt();
   y = frnMain->edOriginalY->Text.ToInt();
   z = frnMain->edOriginalZ->Text.ToInt();
   t = frnMain->edOriginalT->Text.ToInt();

   // 2023 3 16 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      z += frnMain->edWafer8ZDown->Text.ToInt();
   }

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   // 2023 3 16 - chc ���ʨ�̫᭱
   //axisno = Z_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   //frnMain->WriteSystemLog("ToOriginal(z) = " + IntToStr(z));
   //WaitMotionDone(axisno,10000);

   // T
   // => ����T
   //axisno = T_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   //frnMain->WriteSystemLog("ToOriginal(t) = " + IntToStr(t));
   //WaitMotionDone(axisno,10000);

   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("ToOriginal(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("ToOriginal(y) = " + IntToStr(x));

   if(WaitMotionXYZTDone(15000) == false) {
      frnMain->pnlSystemMessage->Caption = "���I���ʥ���!";
      frnMain->pnlToOriginalPosition->Color = clRed;
   }
   else {

      // Z
      // 2023 3 16 - chc ���ʨ�̫᭱
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("ToOriginal(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);

      frnMain->pnlSystemMessage->Caption = "���I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlToOriginalPosition->Color = clLime;
   }

   // 2022 7 7 - chc Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

}
//---------------------------------------------------------------------------
// 2022 6 16 - chc User Position
void __fastcall SetUserPosition(int no)
{
I32 cmd;
F64 fcmd;
TLMDEdit *edx,*edy,*edz,*edt;

   // Check
   if(Application->MessageBox(("Sure to set user position" + IntToStr(no) + "? ").c_str(), "User Position", MB_YESNO|MB_SYSTEMMODAL) == IDNO)
      return;

   edx = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "X"));
   edy = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "Y"));
   edz = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "Z"));
   edt = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "T"));

   // APS
   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   edx->Text = IntToStr(ipos);
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   edy->Text = IntToStr(ipos);
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   edz->Text = IntToStr(ipos);
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   edt->Text = IntToStr(ipos);
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc User Position
void __fastcall ToUserPosition(int no)
{
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
int axisno,ret;
F64 tacc,tdec;
F64 pos,x,y,z,t;
TLMDEdit *edx,*edy,*edz,*edt;

   frnMain->pnlPositionGo->Color = clSilver;
   edx = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "X"));
   edy = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "Y"));
   edz = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "Z"));
   edt = (TLMDEdit *)(frnMain->FindComponent("edPos" + IntToStr(no) + "T"));
   x = edx->Text.ToInt();
   y = edy->Text.ToInt();
   z = edz->Text.ToInt();
   t = edt->Text.ToInt();

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;


   //---------------------------------------------------------------------------
   /*
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   AbsoluteMove(axisno,z,startvel,maxvel,tacc,tdec);

   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   AbsoluteMove(axisno,t,startvel,maxvel,tacc,tdec);

   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // �ϥ�XY�P��
   MoveToXY((int)x,(int)y);
   Sleep(100);
   */
   //---------------------------------------------------------------------------
   // Z
//   frnMain->WriteSystemLog("ToUser(z)�}�l����..." + IntToStr(z));
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
//   frnMain->WriteSystemLog("ToUser(z) = " + IntToStr(z));

   // 2023 2 18 - chc �W��
   //WaitMotionDone(axisno,10000);
   WaitMotionDoneZ(axisno,10000);

//   frnMain->WriteSystemLog("ToUser(z)��������." + IntToStr(z));

   // T/X/Y�n�P��
   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
//   frnMain->WriteSystemLog("ToUser(t) = " + IntToStr(t));

   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
//   frnMain->WriteSystemLog("ToUser(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
//   frnMain->WriteSystemLog("ToUser(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "User�I���ʥ���!";
      frnMain->pnlPositionGo->Color = clRed;
      frnMain->WriteSystemLog("ToUser: Fail");
   }
   else {

      // 2022 8 24 - chc �b����������
      if(boolInLoad == false) {
         frnMain->WriteSystemLog("ToUser: Ok, Check Z...");
         // �ˬdZ?�~�t�n�b2000�H��
         WaitZFinish(z);
         frnMain->WriteSystemLog("ToUser: Ok, Check Z done.");
      }   
//      frnMain->pnlSystemMessage->Caption = "User�I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlPositionGo->Color = clLime;
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;
}
//---------------------------------------------------------------------------
// 2022 6 16 - chc Save Range: pnlLeftUp
// 1-LeftUpM, 2-RightDownM, 3-LeftUpB, 4-RightDownB
void __fastcall SetSaveRange(int mode)
{
I32 cmd;
F64 fcmd;
TPanel *panelx,*panely;
int axisno;

   switch(mode) {
      case 1:
         panelx = (TPanel *)frnMain->pnlLeftUpMX;
         panely = (TPanel *)frnMain->pnlLeftUpMY;
         break;
      case 2:
         panelx = (TPanel *)frnMain->pnlRightDownMX;
         panely = (TPanel *)frnMain->pnlRightDownMY;
         break;
      case 3:
         panelx = (TPanel *)frnMain->pnlLeftUpBX;
         panely = (TPanel *)frnMain->pnlLeftUpBY;
         break;
      case 4:
         panelx = (TPanel *)frnMain->pnlRightDownBX;
         panely = (TPanel *)frnMain->pnlRightDownBY;
         break;
   }

   axisno = X_AXIS;
   APS_get_position(axisno+StartAxisNo,&cmd);
   panelx->Caption = cmd;

   axisno = Y_AXIS;
   APS_get_position(axisno+StartAxisNo,&cmd);
   panely->Caption = cmd;

}
//---------------------------------------------------------------------------
// 2022 6 16a - chc X/Y Axis absolute move Ratio
bool __fastcall MoveToXYRatio(int xpos,int ypos, double ratio)
{
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;
F64 tacc,tdec;

   // 2023 3 21 - chc
   if(boolMotion == false) {
      return false;
   }

   // X
   axisno = X_AXIS;
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToXY: X in Moving!");
      // ����
      //return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;
   // Y
   axisno = Y_AXIS;
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToXY: Y in Moving!");
      // ����
      //return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // 2023 1 15 - chc Y Limit
   int limit = frnMain->edMinYPosition->Text.ToInt();
   if(ypos < limit)
      ypos = limit;
   limit = frnMain->edMaxYPosition->Text.ToInt();
   if(ypos > limit)
      ypos = limit;
   // X Limit
   limit = frnMain->edMinXPosition->Text.ToInt();
   if(xpos < limit)
      xpos = limit;
   limit = frnMain->edMaxXPosition->Text.ToInt();
   if(xpos > limit)
      xpos = limit;

   F64 fxpos,fypos;
   fxpos = xpos;
   fypos = ypos;
   maxvel *= ratio;
   ret = AbsoluteMoveXY(0,fxpos,fypos,startvel,maxvel,tacc,tdec);
   // timeout : MOTION_TIMEOUT
   WaitMotionDoneXY(3,MOTION_TIMEOUT);

   axisno = X_AXIS;
   frnMain->iAxisMove[axisno] = NO_MOVE;
   axisno = Y_AXIS;
   frnMain->iAxisMove[axisno] = NO_MOVE;

   return true;
}
//---------------------------------------------------------------------------
// 2022 6 16a - chc X/Y Axis absolute move
bool __fastcall MoveToXY(int xpos,int ypos)
{
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;
F64 tacc,tdec;

   // 2023 3 21 - chc
   if(boolMotion == false) {
      return false;
   }

   // X
   axisno = X_AXIS;
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToXY: X in Moving!");
      // ����
      //return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;
   // Y
   axisno = Y_AXIS;
   if(frnMain->iAxisMove[axisno] != NO_MOVE) {
      frnMain->WriteSystemLog("MoveToXY: Y in Moving!");
      // ����
      //return false;
   }
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   axisno = X_AXIS;
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);

   // 2023 1 15 - chc Y Limit
   int limit = frnMain->edMinYPosition->Text.ToInt();
   if(ypos < limit)
      ypos = limit;
   limit = frnMain->edMaxYPosition->Text.ToInt();
   if(ypos > limit)
      ypos = limit;
   // X Limit
   limit = frnMain->edMinXPosition->Text.ToInt();
   if(xpos < limit)
      xpos = limit;
   limit = frnMain->edMaxXPosition->Text.ToInt();
   if(xpos > limit)
      xpos = limit;

   F64 fxpos,fypos;
   fxpos = xpos;
   fypos = ypos;

   // 2020 4 5 - chc ��J���I/���I�γ̰��t
   if(frnMain->boolUseMaxSpeed == true)
      maxvel = frnMain->edMaxVelX->Text.ToDouble();

   ret = AbsoluteMoveXY(0,fxpos,fypos,startvel,maxvel,tacc,tdec);
   // timeout : MOTION_TIMEOUT
   WaitMotionDoneXY(3,MOTION_TIMEOUT);

   axisno = X_AXIS;
   frnMain->iAxisMove[axisno] = NO_MOVE;
   axisno = Y_AXIS;
   frnMain->iAxisMove[axisno] = NO_MOVE;

   return true;
}
//---------------------------------------------------------------------------
// 2022 6 16a - chc �s�򲾰�
// mode : 0���e, ������
// mode : 1����, �t����
// Low/High Speed�令����������, �ݩ�}��A�U������O
// �ݩw�q: �t��
// define type value: 0-wait, 1-nowait(for UpdateMotionStatus())
void __fastcall AxisMove(int mode, int type)
{
int dist,startvel,maxvel,posi,homevel,reviewvel,uplimit,lowdistance;
bool moveflag = false;
static bool active = false;
int axisno;
F64 tacc,tdec;

   // Re-Entrant
   if(active == true) {
      frnMain->WriteSystemLog(">AxisMove() Re-Entrant");

      // 2023 2 17 - chc �P�w�ӷ�: Joystick/Button
      frnMain->boolFromJoystick = false;

      return;
   }

   // T�Y�b�M�I�ϰ줣�i����
   if(frnMain->CurrentAxisNo == T_AXIS) {
      if(CheckTMove() == false) {
         frnMain->pnlSystemMessage->Caption = "Z��m����, �L�k����T�b����";
         // Beep�n
         Beep(500);

         // 2023 2 17 - chc �P�w�ӷ�: Joystick/Button
         frnMain->boolFromJoystick = false;

         return;
      }
   }

   frnMain->WriteSystemLog("AxisMove() Enter.");

   active = true;

   // �O���bmove��, ���i�H�P��Continue move
   frnMain->boolInAxisMove = true;

   frnMain->pnlSystemMessage->Caption = "Start to Move.";
   Application->ProcessMessages();
   frnMain->WriteSystemLog(">AxisMove()Enter: mode=" + IntToStr(mode) + ", type=" + IntToStr(type));

   // �O���U�b�O�_�B�@��
   axisno = frnMain->CurrentAxisNo;
   if(frnMain->iAxisMove[axisno] != NO_MOVE)
      goto mdone;
   frnMain->iAxisMove[axisno] = FORWARD_MOVE;

   while(frnMain->boolJogMove) {
      axisno = frnMain->CurrentAxisNo;
      if(moveflag == false) {
         GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
         frnMain->WriteSystemLog("AbsoluteMove: �b(Get maxvel)=" + IntToStr(axisno) + "," + IntToStr(maxvel));
         // �e�i
         if(mode == 0) {
            if(axisno == Z_AXIS) {
               posi = frnMain->edMaxZPosition->Text.ToInt();

               // 2023 3 14 - chc Tazmo: 8"�ץ�Z
               if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
                  posi += frnMain->edWafer8ZDown->Text.ToInt();
               }

            }
            else if(axisno == T_AXIS)
               posi = frnMain->edMaxTPosition->Text.ToInt();

            // 2022 8 4 - chc �[�JX/Y Min/Max Limit
            else if(axisno == X_AXIS)
               posi = frnMain->edMaxXPosition->Text.ToInt();
            else if(axisno == Y_AXIS)
               posi = frnMain->edMaxYPosition->Text.ToInt();

            else
               // �]���L����
               posi = 8000000;
         }
         // ��h
         else {
            if(axisno == Z_AXIS)
               // �令�Ѽ�
               posi = frnMain->edMinZPosition->Text.ToInt();
            else if(axisno == T_AXIS)
               posi = frnMain->edMinTPosition->Text.ToInt();

            // 2022 8 4 - chc �[�JX/Y Min/Max Limit
            else if(axisno == X_AXIS)
               posi = frnMain->edMinXPosition->Text.ToInt();
            else if(axisno == Y_AXIS)
               posi = frnMain->edMinYPosition->Text.ToInt();

            else
               posi = -8000000;
         }

         // Joystick Speed
         // 2023 2 17 - chc �P�w�ӷ�: Joystick/Button
         //if(frnMain->pnlJoystickActive->Color == clLime) {
         if(frnMain->pnlJoystickActive->Color == clLime && frnMain->boolFromJoystick == true) {

            if(axisno == Z_AXIS) {
               if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                  maxvel = (0.1 * 1000) / Z_RESOLUTION;
               else
                  maxvel = (5 * 1000) / Z_RESOLUTION;
            }
            else {

               // 2022 8 15 - chc ��ΰѼ�
               //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
               //   maxvel = (1 * 1000) / X_RESOLUTION;
               //else
               //   maxvel = (50 * 1000) / X_RESOLUTION;
               if(frnMain->rgJoystickSpeed->ItemIndex == 0) {
                  maxvel = (frnMain->edJoystickLowSpeed->Text.ToDouble() * 1000) / frnMain->X_RESOLUTION;
               }
               else {
                  maxvel = (frnMain->edJoystickHighSpeed->Text.ToDouble() * 1000) / frnMain->X_RESOLUTION;
               }

            }
            // 2020 2 13 - chc AI Speed: 100~1000, 10000~500000
            int value,base;
            double unit;
            base = frnMain->edAISpeedStart->Text.ToInt();
            if(axisno == X_AXIS)
               value = frnMain->pnlJoystickX->Caption.ToInt();
            else if(axisno == Y_AXIS)
               value = frnMain->pnlJoystickY->Caption.ToInt();
            else if(axisno == Z_AXIS)
               value = frnMain->pnlJoystickZ->Caption.ToInt();
            if(frnMain->cbAISpeed->Checked == true /*&& value > 0*/) {
               if(axisno == Z_AXIS) {

                  // 2022 6 16a - chc �Ѻ�״���
                  //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                  //   unit = 100.0 / (1000.0 - base);
                  //else
                  //   unit = 5000.0 / (1000.0 - base);
                  int speed;
                  double len;
                  if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                     len = 0.1;                                                 // mm
                  else
                     len = 5;                                                   // mm
                  speed = (len*1000) / Z_RESOLUTION;
                  unit = speed / (1000.0 - base);

               }
               else {

                  // 2022 6 16a - chc �Ѻ�״���
                  //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                  //   unit = 10000.0 / (1000.0 - base);
                  //else
                  //   unit = 500000.0 / (1000.0 - base);
                  int speed;
                  double len;

                  // 2022 8 15 - chc ��ΰѼ�
                  //if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                  //   len = 1;                                                   // mm
                  //else
                  //   len = 50;                                                  // mm
                  //speed = (len*1000) / Z_RESOLUTION;
                  //unit = speed / (1000.0 - base);
                  if(frnMain->rgJoystickSpeed->ItemIndex == 0)
                     len = frnMain->edJoystickLowSpeed->Text.ToDouble();        // mm
                  else
                     len = frnMain->edJoystickHighSpeed->Text.ToDouble();       // mm
                  speed = (len*1000) / frnMain->X_RESOLUTION;
                  unit = speed / (1000.0 - base);

               }
               maxvel = (abs(value) - base + 1) * unit;
               if(axisno == X_AXIS)
                  frnMain->WriteSystemLog(">Joystick: AI��t��(X)= " + FloatToStr(maxvel) + "," + IntToStr(value));
               else if(axisno == Y_AXIS)
                  frnMain->WriteSystemLog(">Joystick: AI��t��(Y)= " + FloatToStr(maxvel) + "," + IntToStr(value));
               else if(axisno == Z_AXIS)
                  frnMain->WriteSystemLog(">Joystick: AI��t��(Z)= " + FloatToStr(maxvel) + "," + IntToStr(value));
            }

            frnMain->WriteSystemLog(">Joystick: ta_move " + FloatToStr(maxvel));
         }

         // Absolute move
         if(axisno == X_AXIS || axisno == Y_AXIS) {
            frnMain->WriteSystemLog("AbsoluteMove: �b=" + IntToStr(axisno) + "," + IntToStr((int)posi) + "," + IntToStr(maxvel));
            AbsoluteMove(axisno,posi,startvel,maxvel,tacc,tdec);
            // �O���ثe���s�򲾰ʤ�: �b�P���ʤ�V(1/2: ��/��, 0-�S������)
            frnMain->MoveAxis = axisno;
            if(mode == 0) {
               frnMain->MoveDirection = 1;
               frnMain->WriteSystemLog("MoveDirection = 1 => 3");
            }
            else {
               frnMain->MoveDirection = 2;
               frnMain->WriteSystemLog("MoveDirection = 2 => 4");
            }
            if(frnMain->MoveIgnore == true && (frnMain->MoveIgnoreAxis != axisno || frnMain->MoveIgnoreDirection != frnMain->MoveDirection))
               frnMain->MoveIgnore = false;
         }
         else {
            if(axisno == T_AXIS) {
               AnsiString msg;
               msg.sprintf("T ta_move(posi,maxvel): %d,%d",(int)posi,(int)maxvel);
               frnMain->WriteSystemLog(msg);
            }
            // �ˬd��t
            if(startvel > maxvel / 2)
               startvel = maxvel / 2;

            // 2022 6 16a - chc ����8154
            //_8154_start_ta_move(axisno, posi, startvel, maxvel, tacc, tdec);
            AbsoluteMove(axisno,posi,startvel,maxvel,tacc,tdec);

         }
      }
      moveflag = true;
      frnMain->pnlSystemMessage->Caption = "���ʤ�...";

      // Wait
      WaitRoutine1(10);

      // no wait
      if(type == 1)
         break;
   }

   // no wait
   if(type == 0) {
      frnMain->WriteSystemLog("AbsoluteMove: emg_stop");
      APS_stop_move(axisno+StartAxisNo);
      // Joystick������٭n�ˬd�䥦�b
      // 2023 2 17 - chc �P�w�ӷ�: Joystick/Button
      //if(frnMain->pnlJoystickActive->Color == clLime) {
      if(frnMain->pnlJoystickActive->Color == clLime && frnMain->boolFromJoystick == true) {

         StopOtherAxis(axisno);
      }
   }

mdone:

   // no wait
   if(type == 0) {
      frnMain->pnlSystemMessage->Caption = "Move Done.";
      Application->ProcessMessages();
      // �O���bmove��, ���i�H�P��Continue move
      frnMain->boolInAxisMove = false;
      // �O���U�b�O�_�B�@��
      frnMain->iAxisMove[axisno] = NO_MOVE;
   }
   frnMain->WriteSystemLog("AxisMove() Done.");

   // Re-Entrant
   active = false;

   // 2023 2 17 - chc �P�w�ӷ�: Joystick/Button
   frnMain->boolFromJoystick = false;

}
//---------------------------------------------------------------------------
// 2022 6 16a - chc �����w�b�~����
void __fastcall StopOtherAxis(int axisno)
{

   for(int i=0 ; i<MAX_AXIS_NO ; i++) {
      if(i != axisno) {
         APS_stop_move(i+StartAxisNo);
      }
   }
}
//---------------------------------------------------------------------------
// 2016 7 18 - chc Backlash Control
int __fastcall AbsoluteMoveXY(I16 axisno,F64 fxpos,F64 fypos,int startvel,int maxvel,F64 tacc,F64 tdec)
{
I16 ret;
I16 xmstatus,ymstatus;
int counter = 0;
int maxtimeout;

   maxtimeout = 2000;                                                        // 100sec
   frnMain->WriteSystemLog("��AbsoluteMoveXY�ֳt, MaxVel= " + FloatToStr(maxvel));

   frnMain->WriteSystemLog("��APS_absolute_move, (px,py)= " + FloatToStr(fxpos) + "," + FloatToStr(fypos));
   ret = APS_absolute_move(X_AXIS+StartAxisNo,(int)fxpos,(int)maxvel);
   ret = APS_absolute_move(Y_AXIS+StartAxisNo,(int)fypos,(int)maxvel);
   frnMain->WriteSystemLog("��APS_absolute_move, ��Done...");

   DelayTimeM5(50);
   // Wait Done
   while(1) {
      xmstatus = 0;
      ymstatus = 0;
      I32 done;
      done = APS_motion_status(X_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         xmstatus = 1;
      done = APS_motion_status(Y_AXIS+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         ymstatus = 1;

      if((xmstatus == 0 || xmstatus == 17 || xmstatus == 18) && (ymstatus == 0 || ymstatus == 17 || ymstatus == 18)) {
         break;
      }
      else {
         DelayTimeM5(50);
         counter++;
         // 500sec: 50*maxtimeout
         if(counter > maxtimeout) {
            frnMain->pnlSystemMessage->Caption = "Wait Motion Done Timeout!";
            break;
         }
      }
   }
   return ret;
}
//---------------------------------------------------------------------------
// 2022 6 18 - chc Joystick������٭n�ˬd�䥦�b
void __fastcall StopAllAxis()
{
F64 tdec;

   for(int i=0 ; i<MAX_AXIS_NO ; i++) {
      APS_stop_move(i+StartAxisNo);
   }
}
//---------------------------------------------------------------------------
// Lens Position
void __fastcall SetLensPosition(int no)
{
I32 cmdx,cmdy,cmdz,cmdt;
F64 fcmdx,fcmdy,fcmdz,fcmdt;
TLMDEdit *editx,*edity,*editz,*editt;

   // Confirm
   if(Application->MessageBox((AnsiString("Set Center of Lens ") + IntToStr(no) + " ? ").c_str(), "Center Setting Operation", MB_YESNO|MB_SYSTEMMODAL) == IDNO) {
      return;
   }

   editx = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "X"));
   edity = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "Y"));
   editz = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "Z"));
   editt = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "T"));

   I32 ipos;
   APS_get_position(X_AXIS+StartAxisNo,&ipos);
   editx->Text = IntToStr(ipos);
   cmdx = ipos;
   APS_get_position(Y_AXIS+StartAxisNo,&ipos);
   edity->Text = IntToStr(ipos);
   cmdy = ipos;
   APS_get_position(Z_AXIS+StartAxisNo,&ipos);
   editz->Text = IntToStr(ipos);
   cmdz = ipos;
   APS_get_position(T_AXIS+StartAxisNo,&ipos);
   editt->Text = IntToStr(ipos);
   cmdt = ipos;

   // Cneter Of Each Lens
   frnMain->LensPosition[no-1].X = cmdx;
   frnMain->LensPosition[no-1].Y = cmdy;
   frnMain->LensPosition[no-1].Z = cmdz;
   frnMain->LensPosition[no-1].T = cmdt;
}
//---------------------------------------------------------------------------
// to Lens Center
void __fastcall ToLensPosition(int no)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;
TLMDEdit *editx,*edity,*editz,*editt;
TPanel *panel;

   // pnlToLens1Position
   panel = (TPanel *)(frnMain->FindComponent("pnlToLens" + IntToStr(no) + "Position"));
   editx = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "X"));
   edity = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "Y"));
   editz = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "Z"));
   editt = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "T"));

   // �n��Edit����
   x = editx->Text.ToInt();
   y = edity->Text.ToInt();
   z = editz->Text.ToInt();
   t = editt->Text.ToInt();
   panel->Color = clSilver;

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   frnMain->WriteSystemLog("ToCCD(z) = " + IntToStr(z));
   WaitMotionDone(axisno,10000);

   // T
   // => ����T
   //axisno = T_AXIS;
   //GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   //ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   //frnMain->WriteSystemLog("ToCCD(t) = " + IntToStr(t));
   //WaitMotionDone(axisno,10000);

   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("ToCCD(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("ToCCD(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "Lens�I���ʥ���!";
      frnMain->pnlToCCDPosition->Color = clRed;
   }
   else {
      frnMain->pnlSystemMessage->Caption = "Lens�I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      frnMain->pnlToCCDPosition->Color = clLime;
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;
}
//---------------------------------------------------------------------------
// Lens Z Limit
void __fastcall SetLensZLimit(int no)
{
I32 cmdz;
TLMDEdit *editz;

   editz = (TLMDEdit *)(frnMain->FindComponent("edLens" + IntToStr(no) + "ZLimit"));
   // Command

   APS_get_position(Z_AXIS+StartAxisNo,&cmdz);

   editz->Text = cmdz;
   frnMain->LensPosition[no-1].ZLimit = cmdz;
}
//---------------------------------------------------------------------------
// 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
void __fastcall WaitZFinish(int z)
{
I32 pos;
int timeout = 600;
int count = 0;
int axisno;

   axisno = Z_AXIS;
   // 30sec
   while(1) {
      DelayTimeM3(50);
      GetPosition(axisno, &pos);

      // 2022 6 20 - chc �A�[�WINP: pnlINPZ->Color = clRed;
      // 2022 6 21 - chc ��^�u��Pos
      if(abs(pos-z) < 2000) {
      //if(abs(pos-z) < 2000 && frnMain->pnlINPZ->Color == clRed)

         frnMain->WriteSystemLog("WaitZFinish(z,pos): " + IntToStr(z) + "," + IntToStr(pos));
         break;
      }
      count++;
      if(count > timeout) {
         frnMain->WriteSystemLog("WaitZFinish: Timeout.");
         break;
      }
   }
}
//---------------------------------------------------------------------------
// 2022 6 19 - chc Relative Move
void __fastcall DoRelativeMove(int axisno, int offset)
{
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
I16 ret;
F64 tacc,tdec;

   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   pos = offset;

   // �O���U�b�O�_�B�@��
   if(pos > 0)
      frnMain->iAxisMove[axisno] = FORWARD_MOVE;
   else
      frnMain->iAxisMove[axisno] = BACKWARD_MOVE;

   // Relative move
   ret = RelativeMove(axisno,pos,startvel,maxvel,tacc,tdec);
}
//---------------------------------------------------------------------------
// ��45�I - Z����, �b��T, X/Y�P�ɲ���
// mode - 1(XYZT), 0(T)
void __fastcall ToR45Position(int mode)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;

// 2022 6 19 - chc ���v
AnsiString msg;
int dx,dy,dz,zlimit;
bool booloffset = false;

   frnMain->pnlToR45Position->Color = clSilver;

   // �n��Edit����
   x = frnMain->edR45X->Text.ToInt();
   y = frnMain->edR45Y->Text.ToInt();
   z = frnMain->edR45Z->Text.ToInt();
   t = frnMain->edR45T->Text.ToInt();

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE || frnMain->iAxisMove[T_AXIS] != NO_MOVE)
      return;

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[T_AXIS] = FORWARD_MOVE;

   // Z
   if(mode == 1) {
      axisno = Z_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
      frnMain->WriteSystemLog("ToR45(z) = " + IntToStr(z));
      WaitMotionDone(axisno,10000);
   }

   // 2022 6 19 - chc �u��T
   if(mode == 0) {
      // �Y�O�b������m, �n���vX/Y/Z
      if(frnMain->IsHeadHorizontal() == true) {
         booloffset = true;
         zlimit = frnMain->edTMoveZMax->Text.ToInt();
         dx = frnMain->edR45X->Text.ToInt() - frnMain->ed0X->Text.ToInt();
         dy = frnMain->edR45Y->Text.ToInt() - frnMain->ed0Y->Text.ToInt();
         dz = frnMain->edR45Z->Text.ToInt() - frnMain->ed0Z->Text.ToInt();
         msg.sprintf("Check�b������m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         x += dx;
         y += dx;
         z += dx;
         if(z > zlimit)
            z = zlimit;
      }
      else if(frnMain->IsHeadTilt() == true) {
         booloffset = true;
         zlimit = frnMain->edTMoveZMax->Text.ToInt();
         dx = frnMain->edR45X->Text.ToInt() - frnMain->ed45X->Text.ToInt();
         dy = frnMain->edR45Y->Text.ToInt() - frnMain->ed45Y->Text.ToInt();
         dz = frnMain->edR45Z->Text.ToInt() - frnMain->ed45Z->Text.ToInt();
         msg.sprintf("Check�b���ɱצ�m: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         x += dx;
         y += dx;
         z += dx;
         if(z > zlimit)
            z = zlimit;
      }
   }

   // T
   axisno = T_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,t,maxvel);
   frnMain->WriteSystemLog("ToR45(t) = " + IntToStr(t));
   WaitMotionDone(axisno,10000);

   // 2022 6 19 - chc �u��T: �Y�O�b������m, �n���vX/Y/Z
   if(mode == 0) {
      if(booloffset == true) {
         msg.sprintf("���t���v: dx,dy,dz=%d,%d,%d",dx,dy,dz);
         frnMain->WriteSystemLog(msg);
         frnMain->pnlSystemMessage->Caption = msg;
         int oldspeed = frnMain->rgSpeed->ItemIndex;
         frnMain->rgSpeed->ItemIndex = 1;
         DoRelativeMove(X_AXIS, dx);
         DoRelativeMove(Y_AXIS, dy);
         DoRelativeMove(Z_AXIS, dz);
         frnMain->rgSpeed->ItemIndex = oldspeed;
      }
   }

   // XY
   if(mode == 1) {
      axisno = X_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
      frnMain->WriteSystemLog("ToR45(x) = " + IntToStr(x));

      axisno = Y_AXIS;
      GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
      ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
      frnMain->WriteSystemLog("ToR45(y) = " + IntToStr(y));
   }
   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlToR45Position->Color = clRed;
      if(mode == 1)
         frnMain->pnlSystemMessage->Caption = "R45���I���ʥ���!";
      else
         frnMain->pnlSystemMessage->Caption = "R45���IT���ʥ���!";
   }
   else {

      // 2022 6 19 - chc �ˬdZ?�~�t�n�b2000�H��
      if(mode == 1)
         WaitZFinish(z);

      frnMain->pnlToR45Position->Color = clLime;
      if(mode == 1)
         frnMain->pnlSystemMessage->Caption = "R45���I���ʧ���(x,y,z,t): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z) + "," + IntToStr(t);
      else
         frnMain->pnlSystemMessage->Caption = "R45���IT���ʧ���(t): " + IntToStr(t);
   }

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;
   frnMain->iAxisMove[T_AXIS] = NO_MOVE;

}
//---------------------------------------------------------------------------
I32 __fastcall GetDI(int id)
{
I32 distatus;
AnsiString msg;

   if(id == 3)
      distatus = APS_get_field_bus_d_input(BoardHSL, HSL_BUS, IO2_ID, &distatus);
   else if(id == 1)
      distatus = APS_get_field_bus_d_input(BoardHSL, HSL_BUS, IO1_ID, &distatus);
   msg.sprintf("GetDI: di,status= %d,%4X,%4X.%d,%d",id,distatus,input_DI2,distatus,input_DI2);
   frnMain->WriteSystemLog(msg);
   return distatus;
}
//---------------------------------------------------------------------------
// 2022 7 13 - chc ��W�I��, W��׬�: 1um
// 1 - 5
void __fastcall ToWPositionFun(int no)
{
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance;
TLMDEdit *edit;
I16 ret;
I32 posi,wpos;
I16 axisno;
int retry;
int tolerance;
int looptry = 0;
F64 tacc,tdec;

   // 2023 2 24 - chc LoadPortMode
   if(frnMain->LoadPortMode == LOADPORT_SINGLE) {
      frnMain->pnlToWStatus->Color = clLime;
      return;
   }

   tolerance = 40;
   frnMain->pcSystem->Enabled = false;

   // Log
   frnMain->WriteSystemLog("�����@�~����14.");

   frnMain->pnlToWStatus->Color = clSilver;
   edit = (TLMDEdit *)(frnMain->FindComponent("edWPosition" + IntToStr(no)));
   posi = edit->Text.ToInt();

   frnMain->pnlSystemMessage->Caption = "W�b�I�첾��: " + IntToStr(no) + "...";
   frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);

   // ����Absolute Position
   axisno = R_AXIS;

   // 2023 2 11 - chc �w�bW��m�W�h������
   APS_get_position(axisno+StartAxisNo,&wpos);
   if(abs(posi - wpos) <= tolerance) {
      frnMain->pnlSystemMessage->Caption = "W�b�I��w�b��m�W, ������.";
      frnMain->pnlToWStatus->Color = clLime;
      goto end;
   }

   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
loopagain:
   ret = AbsoluteMove(axisno,posi,startvel,maxvel,tacc,tdec);
   if(ret == ERR_NoError)
      frnMain->WriteSystemLog(">WAxis Move done.");
   else {
      frnMain->WriteSystemLog(">WAxis Move fail!");

      // 2023 2 11 - chc Try Again, �w�bW��m�W?
      for(int i=0 ; i<2 ; i++) {
         frnMain->WriteSystemLog("W�b�I��A����: " + IntToStr(i+1) + "��");
         //Sleep(200);
         DelayTimeM3(200);
         APS_get_position(axisno+StartAxisNo,&wpos);
         if(abs(posi - wpos) <= tolerance) {
            frnMain->WriteSystemLog("W�b�I��w�b��m�W, ��Retry.");
            frnMain->pnlToWStatus->Color = clLime;
            goto end;
         }
         //Sleep(200);
         DelayTimeM3(200);
         frnMain->WriteSystemLog(">WAxis Move Try again...");
         ret = AbsoluteMove(axisno,posi,startvel,maxvel,tacc,tdec);
         if(ret == ERR_NoError) {
            frnMain->WriteSystemLog(">WAxis Move done.");
            goto done;
         }
      }
      frnMain->WriteSystemLog(">WAxis Move fail!");

      frnMain->pnlToWStatus->Color = clRed;
      goto end;
   }
done:
   // check done?
   retry = 0;
   DelayTimeM1(200);
recheck:
   // �A���ֹ�RX Position�O�_���T
   APS_get_position(axisno+StartAxisNo,&wpos);
   if(abs(posi - wpos) > tolerance) {
      frnMain->pnlSystemMessage->Caption = "W�b�I�첾�ʥ���- ��m����! �ثeW=" + IntToStr(wpos) + ", �]�wW=" + IntToStr(posi) + ", ���զ���:" + IntToStr(retry);
      frnMain->pnlToWStatus->Color = clRed;
      DelayTimeM1(100);
      retry++;
      if(retry < 30)
         goto recheck;
      looptry++;
      if(looptry < 2) {
         frnMain->WriteSystemLog("W���ʦA�U�@���R�O.");
         goto loopagain;
      }
   }
   else {
      frnMain->pnlSystemMessage->Caption = "W�b�I�첾�ʦ��\. �ثeW= " + IntToStr(wpos) + ", �]�wW=" + IntToStr(posi) + ", ���զ���:" + IntToStr(retry);
      frnMain->pnlToWStatus->Color = clLime;
   }

end:
   frnMain->pcSystem->Enabled = true;
   frnMain->WriteSystemLog("W�b�I�첾�ʧ���");
}
//---------------------------------------------------------------------------
// 2022 7 13 - chc Sleep : ToRXPositionFun(int no)�ϥ�
void __fastcall DelayTimeM1(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2022 7 13 - chc W Set
void __fastcall SetWPosition(int no)
{
F64 fcmd;
TLMDEdit *edit;
I16 axisno;
I32 cmd;

   axisno = R_AXIS;
   edit = (TLMDEdit *)(frnMain->FindComponent("edWPosition" + IntToStr(no)));

   APS_get_position(axisno+StartAxisNo,&cmd);
   edit->Text = IntToStr(cmd);
}
//---------------------------------------------------------------------------
// 1500/1000/500/1000 => 1000/500/500/800
// 2022 12 5 - chc ����Signal Tower: R/Y/G/B/Buzzer
void __fastcall TestSignalTower()
{
bool boolr,booly,boolg,boolb;

   //boolr = frnMain->cbID5Bit11->Checked;
   //booly = frnMain->cbID5Bit12->Checked;
   //boolg = frnMain->cbID5Bit13->Checked;
   //boolb = frnMain->cbID5Bit14->Checked;
   boolr = frnMain->cbBit11->Checked;
   booly = frnMain->cbBit12->Checked;
   boolg = frnMain->cbBit13->Checked;
   boolb = frnMain->cbBit14->Checked;
   SetRedLamp(false);
   SetYellowLamp(false);
   SetGreenLamp(false);
   SetBlueLamp(false);

   // 2023 3 21 - chc 1000/500/800�Y�u��800/350/500

   // R
   frnMain->pnlMainSignal->Color = clRed;
   frnMain->pnlMainSignal->Refresh();
   SetRedLamp(true);
   Sleep(800);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   /*
   SetRedLamp(false);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = clRed;
   frnMain->pnlMainSignal->Refresh();
   SetRedLamp(true);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   */
   SetRedLamp(false);
   Sleep(500);
   Application->ProcessMessages();

   // Y
   frnMain->pnlMainSignal->Color = clYellow;
   frnMain->pnlMainSignal->Refresh();
   SetYellowLamp(true);
   Sleep(800);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   /*
   SetYellowLamp(false);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = clYellow;
   frnMain->pnlMainSignal->Refresh();
   SetYellowLamp(true);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   */
   SetYellowLamp(false);
   Sleep(500);
   Application->ProcessMessages();

   // G
   frnMain->pnlMainSignal->Color = clLime;
   frnMain->pnlMainSignal->Refresh();
   SetGreenLamp(true);
   Sleep(800);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   /*
   SetGreenLamp(false);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = clLime;
   frnMain->pnlMainSignal->Refresh();
   SetGreenLamp(true);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   */
   SetGreenLamp(false);
   Sleep(500);
   Application->ProcessMessages();

   // B
   frnMain->pnlMainSignal->Color = clBlue;
   frnMain->pnlMainSignal->Refresh();
   SetBlueLamp(true);
   Sleep(800);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   /*
   SetBlueLamp(false);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = clBlue;
   frnMain->pnlMainSignal->Refresh();
   SetBlueLamp(true);
   Sleep(350);
   Application->ProcessMessages();
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   */
   SetBlueLamp(false);
   Sleep(500);
   Application->ProcessMessages();

   // Buzzer
   frnMain->pnlMainSignal->Color = clBlack;
   frnMain->pnlMainSignal->Refresh();
   Beep(1000);
   Application->ProcessMessages();
   while(1) {
      if(frnMain->tmBeep->Enabled == false)
         break;
      Sleep(100);
      Application->ProcessMessages();
   }
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   /*
   Sleep(1000);
   frnMain->pnlMainSignal->Color = clBlack;
   frnMain->pnlMainSignal->Refresh();
   Beep(500);
   while(1) {
      if(frnMain->tmBeep->Enabled == false)
         break;
      Sleep(100);
      Application->ProcessMessages();
   }
   frnMain->pnlMainSignal->Color = (TColor)0x006A4E47;
   frnMain->pnlMainSignal->Refresh();
   */
   
   // Restore
   // 2023 4 27 - chc  �Τ@����
   //if(boolr == true)
   //   SetRedLamp(true);
   //if(booly == true)
   //   SetYellowLamp(true);
   //if(boolg == true)
   //   SetGreenLamp(true);
   //if(boolb == true)
   //   SetBlueLamp(true);

}
//---------------------------------------------------------------------------
// Z�babsolute����
bool __fastcall MoveToZ(int position)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,pos,lowdistance;
int axisno;
I16 ret;

   axisno = Z_AXIS;
   // �����b�ƥH���oStartVel/MaxVel
   frnMain->combAxis->ItemIndex = axisno;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   pos = position;

   ret = APS_absolute_move(axisno+StartAxisNo,pos,maxvel);
   if(WaitMotionDone(axisno, 10000) == false) {
      return false;
   }
   return true;
}
//---------------------------------------------------------------------------
// 2023 1 15 - chc Check Limit
void __fastcall CheckLimitAxis(int axisno,int *pos)
{
int min,max;

   // X
   if(axisno == X_AXIS) {
      min = frnMain->edMinXPosition->Text.ToInt();
      max = frnMain->edMaxXPosition->Text.ToInt();
   }
   else if(axisno == Y_AXIS) {
      min = frnMain->edMinYPosition->Text.ToInt();
      max = frnMain->edMaxYPosition->Text.ToInt();
   }
   else if(axisno == Z_AXIS) {
      min = frnMain->edMinZPosition->Text.ToInt();
      max = frnMain->edMaxZPosition->Text.ToInt();

      // 2023 3 14 - chc Tazmo: 8"�ץ�Z
      if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
         max += frnMain->edWafer8ZDown->Text.ToInt();
      }

   }
   else if(axisno == T_AXIS) {
      min = frnMain->edMinTPosition->Text.ToInt();
      max = frnMain->edMaxTPosition->Text.ToInt();
   }
   else if(axisno == R_AXIS)
      return;

   if(*pos < min)
      *pos = min;
   if(*pos > max)
      *pos = max;
}
//---------------------------------------------------------------------------
// 2023 1 15 - chc Check Limit
void __fastcall CheckLimit(int *x,int *y,int *z,int *t)
{
int limit;

   // X
   limit = frnMain->edMinXPosition->Text.ToInt();
   if(*x < limit)
      *x = limit;
   limit = frnMain->edMaxXPosition->Text.ToInt();
   if(*x > limit)
      *x = limit;
   // Y
   limit = frnMain->edMinYPosition->Text.ToInt();
   if(*y < limit)
      *y = limit;
   limit = frnMain->edMaxYPosition->Text.ToInt();
   if(*y > limit)
      *y = limit;
   // Z
   limit = frnMain->edMinZPosition->Text.ToInt();
   if(*z < limit)
      *z = limit;
   limit = frnMain->edMaxZPosition->Text.ToInt();

   // 2023 3 14 - chc Tazmo: 8"�ץ�Z
   if(frnMain->RobotMode == ROBOT_MODE_TAZMO && frnMain->rgCST->ItemIndex == LOADPORT_2) {
      limit += frnMain->edWafer8ZDown->Text.ToInt();
   }

   if(*z > limit)
      *z = limit;
   // T
   limit = frnMain->edMinTPosition->Text.ToInt();
   if(*t < limit)
      *t = limit;
   limit = frnMain->edMaxTPosition->Text.ToInt();
   if(*t > limit)
      *t = limit;

}
//---------------------------------------------------------------------------
// 2023 2 17 - chc AdjustXPos
void __fastcall AdjustXPos()
{
I32 pos,cmd,npos;
int axisno;
int offset;

   axisno = X_AXIS;
   GetPosition(axisno, &pos);
   GetCommand(axisno, &cmd);
   offset = abs(cmd - pos);
   if(offset > 20) {
      frnMain->WriteSystemLog("X Home�ե�(p,c,�t�q): " + IntToStr(offset) + "," + IntToStr(pos) + "," + IntToStr(cmd));
      MoveToX(0 - pos);
      Sleep(500);
      GetPosition(axisno, &npos);
      if(abs(npos) > 50) {
         frnMain->WriteSystemLog("X Home�ե�, cmd Set(np): " + IntToStr(npos));
         APS_set_command(axisno+StartAxisNo,npos);
      }
      frnMain->WriteSystemLog("X Home�ե���(np,c): " + IntToStr(npos) + "," + IntToStr(cmd));
   }
   else
      frnMain->WriteSystemLog("X Home���ե�(p,c): " + IntToStr(pos) + "," + IntToStr(cmd));

}
//---------------------------------------------------------------------------
// Wait Motion done
// 2023 2 18 - chc W�W��
bool __fastcall WaitMotionDoneW(int axisno, int ms)
{
static bool active = false;
I32 cmd;
I16 Motion_status;
int counter;
int max;

   // Re-Entrant
   if(active == true) {
      frnMain->WriteSystemLog("WaitMotionDoneW(): Retrant!");
      return false;
   }
   active = true;
   frnMain->WriteSystemLog("WaitMotionDoneW(): Axis= " + IntToStr(axisno));

   max = ms / 10;
   DelayTimeM2W(10);
   counter = 0;
   while(1) {
      Motion_status = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_status = 1;
      if(Motion_status == 0) {
         frnMain->WriteSystemLog("WaitMotionDoneW(): Axis= " + IntToStr(axisno) + " done.");
         break;
      }
      else {
         counter++;
         // 10sec: 50*200
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion Timeout!";
            frnMain->WriteSystemLog("WaitMotionDoneW(): Axis= " + IntToStr(axisno) + " timeout!");
            // Re-Entrant
            active = false;
            return false;
         }
         DelayTimeM2W(10);
      }
   }
   // Re-Entrant
   active = false;

   return true;
}
//---------------------------------------------------------------------------
// Wait Motion done
// 2023 2 18 - chc Z�W��
bool __fastcall WaitMotionDoneZ(int axisno, int ms)
{
static bool active = false;
I32 cmd;
I16 Motion_status;
int counter;
int max;

   // Re-Entrant
   if(active == true) {
      frnMain->WriteSystemLog("WaitMotionDoneZ(): Retrant!");
      return false;
   }
   active = true;
   frnMain->WriteSystemLog("WaitMotionDoneZ(): Axis= " + IntToStr(axisno));

   max = ms / 10;
   DelayTimeM2Z(10);
   counter = 0;
   while(1) {
      Motion_status = 0;
      I32 done;
      done = APS_motion_status(axisno+StartAxisNo);
      // ��bit 0 : ������0(Done)
      if((done & 0x01) == 0)
         Motion_status = 1;
      if(Motion_status == 0) {
         frnMain->WriteSystemLog("WaitMotionDoneZ(): Axis= " + IntToStr(axisno) + " done.");
         break;
      }
      else {
         counter++;
         // 10sec: 50*200
         if(counter > max) {
            frnMain->pnlSystemMessage->Caption = "Motion Timeout!";
            frnMain->WriteSystemLog("WaitMotionDoneZ(): Axis= " + IntToStr(axisno) + " timeout!");
            // Re-Entrant
            active = false;
            return false;
         }
         DelayTimeM2Z(10);
      }
   }
   // Re-Entrant
   active = false;

   return true;
}
//---------------------------------------------------------------------------
// 2023 2 18 - chc Sleep : WaitMotionDone()�ϥ�
// W�W��
void __fastcall DelayTimeM2W(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2023 2 18 - chc Sleep : WaitMotionDone()�ϥ�
// Z�W��
void __fastcall DelayTimeM2Z(int no)
{
long starttime,stoptime,elapsedms;
short starttick,stoptick;
int cnt = 0;

   frnMain->GetTimeTic(&starttime,&starttick);
   while(1) {
      Application->ProcessMessages();
      Sleep(MOTION_SLEEP_TIME);
      cnt++;
      if(cnt >= 10) {
         cnt = 0;
         frnMain->GetTimeTic(&stoptime,&stoptick);
         elapsedms = ((stoptime*1000+stoptick) - (starttime*1000+starttick));
         if(elapsedms > no)
            return;
      }
   }
}
//---------------------------------------------------------------------------
// 2023 2 25 - chc �}��FFU
void __fastcall SetFFU(bool mode)
{

   // On
   if(mode == true) {
      if(frnMain->cbBit4->Checked == false) {
         frnMain->cbBit4->Checked = true;
         frnMain->cbBit5->Checked = true;
      }
   }
   // Off
   else {
      if(frnMain->cbBit4->Checked == true) {
         frnMain->cbBit4->Checked = false;
         frnMain->cbBit5->Checked = false;
      }
   }
   WriteDO(IO1_ID);
}
//---------------------------------------------------------------------------
// 2023 3 14 - chc 12Edge
void __fastcall Set12EdgePos(int no)
{

   I32 iposx,iposy,iposz;
   APS_get_position(X_AXIS+StartAxisNo,&iposx);
   APS_get_position(Y_AXIS+StartAxisNo,&iposy);
   APS_get_position(Z_AXIS+StartAxisNo,&iposz);
   if(no == 1) {
      frmRecipeSet->ed12EdgeX1->Text = IntToStr(iposx);
      frmRecipeSet->ed12EdgeY1->Text = IntToStr(iposy);
      frmRecipeSet->ed12EdgeZ1->Text = IntToStr(iposz);
   }
   else if(no == 2) {
      frmRecipeSet->ed12EdgeX2->Text = IntToStr(iposx);
      frmRecipeSet->ed12EdgeY2->Text = IntToStr(iposy);
      frmRecipeSet->ed12EdgeZ2->Text = IntToStr(iposz);
   }
   else if(no == 3) {
      frmRecipeSet->ed12EdgeX3->Text = IntToStr(iposx);
      frmRecipeSet->ed12EdgeY3->Text = IntToStr(iposy);
      frmRecipeSet->ed12EdgeZ3->Text = IntToStr(iposz);
   }
}
//---------------------------------------------------------------------------
// 2023 3 14 - chc 8Edge
void __fastcall Set8EdgePos(int no)
{

   I32 iposx,iposy,iposz;
   APS_get_position(X_AXIS+StartAxisNo,&iposx);
   APS_get_position(Y_AXIS+StartAxisNo,&iposy);
   APS_get_position(Z_AXIS+StartAxisNo,&iposz);
   if(no == 1) {
      frmRecipeSet->ed8EdgeX1->Text = IntToStr(iposx);
      frmRecipeSet->ed8EdgeY1->Text = IntToStr(iposy);
      frmRecipeSet->ed8EdgeZ1->Text = IntToStr(iposz);
   }
   else if(no == 2) {
      frmRecipeSet->ed8EdgeX2->Text = IntToStr(iposx);
      frmRecipeSet->ed8EdgeY2->Text = IntToStr(iposy);
      frmRecipeSet->ed8EdgeZ2->Text = IntToStr(iposz);
   }
   else if(no == 3) {
      frmRecipeSet->ed8EdgeX3->Text = IntToStr(iposx);
      frmRecipeSet->ed8EdgeY3->Text = IntToStr(iposy);
      frmRecipeSet->ed8EdgeZ3->Text = IntToStr(iposz);
   }
}
//---------------------------------------------------------------------------
// 2023 3 14 - chc 12Edge
void __fastcall To12EdgePos(int no)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;
TPanel *panel;            

   t = 0;
   panel = (TPanel *)(frmRecipeSet->FindComponent("pnlTo12Edge" + IntToStr(no)));
   if(no == 1) {
      // �n��Edit����
      x = frmRecipeSet->ed12EdgeX1->Text.ToInt();
      y = frmRecipeSet->ed12EdgeY1->Text.ToInt();
      z = frmRecipeSet->ed12EdgeZ1->Text.ToInt();
   }
   else if(no == 2) {
      // �n��Edit����
      x = frmRecipeSet->ed12EdgeX2->Text.ToInt();
      y = frmRecipeSet->ed12EdgeY2->Text.ToInt();
      z = frmRecipeSet->ed12EdgeZ2->Text.ToInt();
   }
   else if(no == 3) {
      // �n��Edit����
      x = frmRecipeSet->ed12EdgeX3->Text.ToInt();
      y = frmRecipeSet->ed12EdgeY3->Text.ToInt();
      z = frmRecipeSet->ed12EdgeZ3->Text.ToInt();
   }
   panel->Color = clSilver;

   // �ϥγ̰��t: rgSpeed
   int oldspeed;
   oldspeed = frnMain->rgSpeed->ItemIndex;
   frnMain->rgSpeed->ItemIndex = JOG_HIGH_SPEED;

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE) {
      frnMain->WriteSystemLog("Motion Busy!");
      return;
   }

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;

   // ��XY�AZT
   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("To12Edge(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("To12Edge(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "12Edge�I���ʥ���!";
      panel->Color = clRed;
   }

   // Z
   // 2023 3 14 - chc ������Z
   /*
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   frnMain->WriteSystemLog("To12Edge(z) = " + IntToStr(z));
   WaitMotionDone(axisno,10000);

   frnMain->WriteSystemLog("To12Edge: ���ݧ���...");
   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "12Edge�I���ʥ���!";
      panel->Color = clRed;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }
   else {
      // �ˬdZ?�~�t�n�b2000�H��
      WaitZFinish(z);
      frnMain->pnlSystemMessage->Caption = "12Edge�I���ʧ���(x,y,z): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z);
      panel->Color = clLime;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }
   */

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;

   // �ϥγ̰��t: rgSpeed
   frnMain->rgSpeed->ItemIndex = oldspeed;
   frnMain->WriteSystemLog("12Edge�I����: Leave.");
}
//---------------------------------------------------------------------------
// 2023 3 14 - chc 8Edge
void __fastcall To8EdgePos(int no)
{
F64 tacc,tdec;
int dist,startvel,maxvel,homevel,reviewvel,uplimit,lowdistance,ret;
int axisno;
int pos,x,y,z,t;
TPanel *panel;

   t = 0;
   panel = (TPanel *)(frmRecipeSet->FindComponent("pnlTo8Edge" + IntToStr(no)));
   if(no == 1) {
      // �n��Edit����
      x = frmRecipeSet->ed8EdgeX1->Text.ToInt();
      y = frmRecipeSet->ed8EdgeY1->Text.ToInt();
      z = frmRecipeSet->ed8EdgeZ1->Text.ToInt();
   }
   else if(no == 2) {
      // �n��Edit����
      x = frmRecipeSet->ed8EdgeX2->Text.ToInt();
      y = frmRecipeSet->ed8EdgeY2->Text.ToInt();
      z = frmRecipeSet->ed8EdgeZ2->Text.ToInt();
   }
   else if(no == 3) {
      // �n��Edit����
      x = frmRecipeSet->ed8EdgeX3->Text.ToInt();
      y = frmRecipeSet->ed8EdgeY3->Text.ToInt();
      z = frmRecipeSet->ed8EdgeZ3->Text.ToInt();
   }
   panel->Color = clSilver;

   // �ϥγ̰��t: rgSpeed
   int oldspeed;
   oldspeed = frnMain->rgSpeed->ItemIndex;
   frnMain->rgSpeed->ItemIndex = JOG_HIGH_SPEED;

   // Check Move State
   if(frnMain->iAxisMove[X_AXIS] != NO_MOVE || frnMain->iAxisMove[Y_AXIS] != NO_MOVE ||
      frnMain->iAxisMove[Z_AXIS] != NO_MOVE)
      return;

   // 2023 1 15 - chc Check Limit
   CheckLimit(&x,&y,&z,&t);

   // Set Move State
   frnMain->iAxisMove[X_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Y_AXIS] = FORWARD_MOVE;
   frnMain->iAxisMove[Z_AXIS] = FORWARD_MOVE;

   // ��XY�AZT
   // XY
   axisno = X_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,x,maxvel);
   frnMain->WriteSystemLog("To8Edge(x) = " + IntToStr(x));

   axisno = Y_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,y,maxvel);
   frnMain->WriteSystemLog("To8Edge(y) = " + IntToStr(y));

   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "8Edge�I���ʥ���!";
      panel->Color = clRed;
   }

   // Z
   // 2023 3 14 - chc ������Z
   /*
   axisno = Z_AXIS;
   GetAxisData(axisno,&dist,&startvel,&maxvel,&tacc,&tdec,&homevel,&reviewvel,&uplimit,&lowdistance);
   ret = APS_absolute_move(axisno+StartAxisNo,z,maxvel);
   frnMain->WriteSystemLog("To8Edge(z) = " + IntToStr(z));
   WaitMotionDone(axisno,10000);

   frnMain->WriteSystemLog("To8Edge: ���ݧ���...");
   if(WaitMotionXYZTDone(30000) == false) {
      frnMain->pnlSystemMessage->Caption = "8Edge�I���ʥ���!";
      panel->Color = clRed;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }
   else {
      // �ˬdZ?�~�t�n�b2000�H��
      WaitZFinish(z);
      frnMain->pnlSystemMessage->Caption = "8Edge�I���ʧ���(x,y,z): " + IntToStr(x) + "," + IntToStr(y) + "," + IntToStr(z);
      panel->Color = clLime;
      frnMain->WriteSystemLog(frnMain->pnlSystemMessage->Caption);
   }
   */

   // Reset Move State
   frnMain->iAxisMove[X_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Y_AXIS] = NO_MOVE;
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;

   // �ϥγ̰��t: rgSpeed
   frnMain->rgSpeed->ItemIndex = oldspeed;
   frnMain->WriteSystemLog("8Edge�I����: Leave.");
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
