//---------------------------------------------------------------------------

#pragma hdrstop

#include "AutoFocus.h"
#include "frmMain.h"

void __fastcall GetPosition(int axisno, I32 *pos);
bool __fastcall MoveToZ(int position, int speed, int mode);
extern I32 StartAxisNo, FirstAxisNo, TotalAxisNo;
void __fastcall StopOneAxis(int axisno);

//---------------------------------------------------------------------------
// Load Focus Parameter
void __fastcall LoadFocusParameter()
{
AnsiString msg;
TIniFile *pSystemFile;
AnsiString szfocus,szfocusoffset,szfocusspeed;

   frnMain->WriteSystemLog(">>LoadFocusParameter() - 1");

   // Read .ini
   frnMain->pnlSystemMessage->Caption = "載入對焦參數...";
   frnMain->pnlAlarmMessage->Caption = "載入對焦參數...";

   // ini檔名與目錄
   pSystemFile = new TIniFile(frnMain->FocusINIFilename);
   szfocus = pSystemFile->ReadString(FOCUS_INFORMATION_SECTION,"Z"              ,"51036");
   szfocusoffset = pSystemFile->ReadString(FOCUS_INFORMATION_SECTION,"Z Offset" ,"5000");
   szfocusspeed = pSystemFile->ReadString(FOCUS_INFORMATION_SECTION,"Z Speed"   ,"200");

   frnMain->WriteSystemLog(">>LoadFocusParameter() - 2");

   // 加入Quick Mode & Algorithm
   bool boolquickmode;
   boolquickmode = pSystemFile->ReadBool(FOCUS_INFORMATION_SECTION,"Z Quick Mode"           ,false);
   frnMain->cbAIFocus->Checked = boolquickmode;
   int ialgorithm;
   ialgorithm = pSystemFile->ReadInteger(FOCUS_INFORMATION_SECTION,"Z Focus Algorithm"      ,0);
   frnMain->rgFocusAlgorithm->ItemIndex = ialgorithm;

   // Focus加入對比量: edDifferenceValue
   int differencevalue;
   differencevalue = pSystemFile->ReadInteger(FOCUS_INFORMATION_SECTION,"Difference Value"        ,250);
   if(differencevalue < 200 || differencevalue > 250)
      differencevalue = 250;
   frnMain->edDifferenceValue->Text = IntToStr(differencevalue);

   frnMain->WriteSystemLog(">>LoadFocusParameter() - 3");

   // 加入加權: edDifferenceRatio
   int differenceratio;
   differenceratio = pSystemFile->ReadInteger(FOCUS_INFORMATION_SECTION,"Difference Ratio"        ,10);
   if(differenceratio < 10 || differenceratio > 60)
      differenceratio = 10;
   frnMain->edDifferenceRatio->Text = IntToStr(differenceratio);

   // 加入厚度: edAutoFocusThickness
   AnsiString szfocusthickness = pSystemFile->ReadString(FOCUS_INFORMATION_SECTION,"Z Thickness"           ,"200");
   frnMain->edAutoFocusThickness->Text = szfocusthickness;

   delete pSystemFile;

   frnMain->WriteSystemLog(">>LoadFocusParameter() - 4");

   // 填入元件
   frnMain->edAutoFocus->Text = szfocus;
   frnMain->edAutoFocusOffset->Text = szfocusoffset;
   frnMain->edAutoFocusSpeed->Text = szfocusspeed;
   frnMain->pnlSystemMessage->Caption = "載入對焦參數完成.";

   frnMain->WriteSystemLog(">>LoadFocusParameter() - 5");

}

//---------------------------------------------------------------------------
void __fastcall AutoFocus()
{
int zpos,offset,speed;
int x1,y1,x2,y2,w,h,cx,cy,row,col,no,upvalue,downvalue;
unsigned char *ptr;
EC24 *cptr;
I32 zuppos,zdownpos;
I16 status;
double foffset;

// Sobel
int data,std,sum,width;
double xx,yy,zz,amp;
int d[25];
bool boolrobert = true;
EC24 *ptr1,*ptr2,*ptr3;

// 是Gray/Color? CCDImageAry
unsigned char *ptr1bw,*ptr2bw,*ptr3bw;

static int sx[9] = {-1, 0, 1,
                    -2, 1, 2,
                    -1, 0, 1};
static int sy[9] = {-1,-2,-1,
                     0, 0, 0,
                     1, 2, 1};
// Roberts
static int rx[9] = { 0, 0, 0,
                     0, 1, 0,
                     0, 0,-1};
static int ry[9] = { 0, 0, 0,
                     0, 0, 1,
                     0, -1,0};

// 加入各algorithm
int max,min,dcnt;
long mean = 0;
int gap = 50;
int z1,z2,z3,z4,z5,z6,z7,z8,z9,G1,G2;
// Variance分成128上與下
long meanlow = 0;
long meanhigh = 0;
int dcntlow;
int dcnthigh;
long sumlow,sumhigh;
// 差距
int diff1,diff2;
int cnt,oldsum;
I32 zupposbest,zdownposbest;
int upcount,downcount;
int bestz = 0;

   // 要設定amp
   amp = 1;
   upcount = 0;
   downcount = 0;

   frnMain->pnlAutoFocus->Color = clSilver;
   frnMain->pnlAutoFocus1->Color = clSilver;
   // 到定位點: Z軸absolute移動
   speed = frnMain->edAutoFocusSpeed->Text.ToInt();
   zpos = frnMain->edAutoFocus->Text.ToInt();
   frnMain->WriteSystemLog(">>UpFocus: MoveTo " + IntToStr(zpos) + " ...");
   MoveToZ(zpos,speed*10,0);
   // 低速往上
   offset = frnMain->edAutoFocusOffset->Text.ToInt();
   // um => pulse
   foffset = (double)offset / Z_RESOLUTION;
   frnMain->WriteSystemLog(">>UpFocus: MoveTo " + IntToStr(zpos-offset) + " ...");
   MoveToZ(zpos-offset,speed,1);
   no = 0;
   // 中間800*800
   // 2019 11 27 - chc 是Gray/Color? CCDImageAry
   if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
      w = frnMain->CCDImageAry[0].GetWidth();
      h = frnMain->CCDImageAry[0].GetHeight();
   }
   else {

      w = frnMain->usb_ImageAry[0].GetWidth();
      h = frnMain->usb_ImageAry[0].GetHeight();
   }
   cx = w / 2;
   cy = h / 2;
   x1 = cx - 400;
   y1 = cy - 400;
   x2 = x1 + 800;
   y2 = y1 + 800;
   zuppos = -1;
   zdownpos = -1;

   // 2019 11 26 - chc Reset to -1
   zupposbest = -1;
   zdownposbest = -1;

   frnMain->WriteSystemLog(">>UpFocus do...");
   // Motion Start...
   Sleep(10);
   Application->ProcessMessages();
   int tout = 0;

   // 2022 6 16a - chc APS
   I32 done;

   while(1) {

      // 2022 6 16a - chc APS
      //status = _8154_motion_done(Z_AXIS);
      status = 0;
      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // 看bit 0 : 不等於0(Done)
      if((done & 0x01) == 0)
         status = 1;

      if(status != 0 && status != 17 && status != 18)
         break;
      Sleep(1);
      Application->ProcessMessages();
      tout++;
      if(tout > 20)
         break;
   }

   // 2017 2 4 - chc 仿Laser
   upvalue = 0;
   downvalue = 0;
   frnMain->pnlUpValue->Caption = IntToStr(upvalue);
   frnMain->WriteSystemLog(">>UpFocus exe...");

   // 2019 11 19 - chc Algorithm
   frnMain->pnlSystemMessage->Caption = "Up Focus start...";
   cnt = 0;
   oldsum = 0;

   // 2019 11 26 - chc 記錄是否Z有變動, 若未變動則不做
   I32 zuppos_old,zdownpos_old;
   zuppos_old = -1;
   zdownpos_old = -1;
   // 白點數量
   int upwhitecnt = 0;
   int downwhitecnt = 0;
   int whiteratio = 10;

   // 2020 6 4 - chc 加入對比量: edDifferenceValue
   whiteratio = frnMain->edDifferenceRatio->Text.ToInt();

   while(1) {

      // 2019 11 19 - chc Algorithm
      // 2019 11 27 - chc 有比較才算
      //upcount++;
      //frnMain->pnlUpZCount->Caption = IntToStr(upcount);

      // Motion done
      // 2022 6 16a - chc APS
      //status = _8154_motion_done(Z_AXIS);
      status = 0;
      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // 看bit 0 : 不等於0(Done)
      if((done & 0x01) == 0)
         status = 1;

      if(status == 0 || status == 17 || status == 18) {
         frnMain->WriteSystemLog(">>UpFocus Motion done: " + IntToStr(status));
         break;
      }

      //=================================================
      // Sobel: 30
      //
      std = 30;
      width = x2 - x1;
      sum = 0;

      // 2019 11 27 - chc 要Reset
      upwhitecnt = 0;

      // 2019 11 26 - chc 檢測前要先讀取: AUO要改
      GetPosition(Z_AXIS, &zuppos);
      frnMain->pnlUpZPos->Caption = IntToStr((int)zuppos);
      // 記錄是否Z有變動, 若未變動則不做
      if(zuppos_old == -1) {
         zuppos_old = zuppos;
      }
      else {
         if(zuppos == zuppos_old)
            continue;
         else
            zuppos_old = zuppos;
      }

      // 2019 11 19 - chc Algorithm
      switch(frnMain->rgFocusAlgorithm->ItemIndex) {
         // 差距總和
         case 0:

            // 2019 11 27 - chc 是Gray/Color? CCDImageAry
            if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
               for(row=y1+1 ; row<y2 ; row+=1) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+3);         // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 白點數量
                     if(ptr1bw[col] > 250)
                        upwhitecnt++;
                     diff1 = abs(ptr1bw[col]-ptr1bw[col+3]);
                     diff2 = abs(ptr1bw[col]-ptr2bw[col]);
                     if(diff1 > 5)
                        sum += diff1;
                     else
                        sum += (diff1 / 2);
                     if(diff2 > 5)
                        sum += diff2;
                     else
                        sum += (diff2 / 2);
                  }
               }
            }
            else {

               for(row=y1+1 ; row<y2 ; row+=1) {
                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+3);         // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {

                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        upwhitecnt++;

                     diff1 = abs(ptr1[col].m_un8C1-ptr1[col+3].m_un8C1);
                     diff2 = abs(ptr1[col].m_un8C1-ptr2[col].m_un8C1);
                     if(diff1 > 5)
                        sum += diff1;
                     else
                        sum += (diff1 / 2);
                     if(diff2 > 5)
                        sum += diff2;
                     else
                        sum += (diff2 / 2);
                  }
               }
            }
            break;

         // Sobel
         case 1:

            for(row=y1+1 ; row<y2 ; row+=2) {
               //ptr  = ProcessImage.GetImagePtr(0,row);                                   // 取得Buffer位置

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                  ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                  ptr3bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                  ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                  ptr3 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
               }
               for(col=1 ; col<width ; col+=2) {

                  // 2019 11 26 - chc 白點數量
                  // 2019 11 27 - chc 是Gray/Color? CCDImageAry
                  if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                     if(ptr1bw[col] > 250)
                        upwhitecnt++;

                     d[0] = ptr1bw[col-1];
                     d[1] = ptr1bw[col];
                     d[2] = ptr1bw[col+1];
                     d[3] = ptr2bw[col-1];
                     d[4] = ptr2bw[col];
                     d[5] = ptr2bw[col+1];
                     d[6] = ptr3bw[col-1];
                     d[7] = ptr3bw[col];
                     d[8] = ptr3bw[col+1];
                     xx = (double)(sx[0]*d[0] + sx[1]*d[1] + sx[2]*d[2]
                              + sx[3]*d[3] + sx[4]*d[4] + sx[5]*d[5]
                              + sx[6]*d[6] + sx[7]*d[7] + sx[8]*d[8]);
                     yy = (double)(sy[0]*d[0] + sy[1]*d[1] + sy[2]*d[2]
                              + sy[3]*d[3] + sy[4]*d[4] + sy[5]*d[5]
                              + sy[6]*d[6] + sy[7]*d[7] + sy[8]*d[8]);
                     zz = (double)(amp*sqrt(xx*xx+yy*yy));
                     data = (int)zz;
                     if(data > 255)
                        data = 255;
                     //ptr[col] = (char)data;
                     if(data > std)
                        sum++;
                  }
                  else {

                     if(ptr1[col].m_un8C1 > 250)
                        upwhitecnt++;

                     d[0] = ptr1[col-1].m_un8C1;
                     d[1] = ptr1[col].m_un8C1;
                     d[2] = ptr1[col+1].m_un8C1;
                     d[3] = ptr2[col-1].m_un8C1;
                     d[4] = ptr2[col].m_un8C1;
                     d[5] = ptr2[col+1].m_un8C1;
                     d[6] = ptr3[col-1].m_un8C1;
                     d[7] = ptr3[col].m_un8C1;
                     d[8] = ptr3[col+1].m_un8C1;
                     xx = (double)(sx[0]*d[0] + sx[1]*d[1] + sx[2]*d[2]
                              + sx[3]*d[3] + sx[4]*d[4] + sx[5]*d[5]
                              + sx[6]*d[6] + sx[7]*d[7] + sx[8]*d[8]);
                     yy = (double)(sy[0]*d[0] + sy[1]*d[1] + sy[2]*d[2]
                              + sy[3]*d[3] + sy[4]*d[4] + sy[5]*d[5]
                              + sy[6]*d[6] + sy[7]*d[7] + sy[8]*d[8]);
                     zz = (double)(amp*sqrt(xx*xx+yy*yy));
                     data = (int)zz;
                     if(data > 255)
                        data = 255;
                     //ptr[col] = (char)data;
                     if(data > std)
                        sum++;
                  }
               }
               // 要加入, 否則可能無法做tmMotionPolling!!
               if((row % 50) == 0) {
                  Sleep(1);
                  Application->ProcessMessages();
               }
            }
            frnMain->WriteSystemLog(">>UpFocus sobel: " + IntToStr(sum));

         // 2019 11 19 - chc Algorithm
            break;
         // Gradient: 最大梯度值
         case 2:
            for(col=1 ; col<width ; col++) {
               for(row=y1+1 ; row<y2 ; row++) {

                  // 2019 11 26 - chc 白點數量
                  // 2019 11 27 - chc 是Gray/Color? CCDImageAry
                  if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                     ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                     ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                     ptr3bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                     if(ptr1bw[col] > 250)
                        upwhitecnt++;
                     z1 = ptr1bw[col-1];
                     z2 = ptr2bw[col-1];
                     z3 = ptr3bw[col-1];
                     z4 = ptr1bw[col];
                     z6 = ptr3bw[col];
                     z7 = ptr1bw[col+1];
                     z8 = ptr2bw[col+1];
                     z9 = ptr3bw[col+1];
                     G1 = abs(-z1 - 2*x2 - z3 + z7 + 2*z9 + z9);
                     G2 = abs(-z1 - 2*z4 - z7 + z3 + 2*z6 + z9);
                     sum += G1;
                     sum += G2;
                  }
                  else {

                     ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                     ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                     ptr3 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                     if(ptr1[col].m_un8C1 > 250)
                        upwhitecnt++;
                     z1 = ptr1[col-1].m_un8C1;
                     z2 = ptr2[col-1].m_un8C1;
                     z3 = ptr3[col-1].m_un8C1;
                     z4 = ptr1[col].m_un8C1;
                     z6 = ptr3[col].m_un8C1;
                     z7 = ptr1[col+1].m_un8C1;
                     z8 = ptr2[col+1].m_un8C1;
                     z9 = ptr3[col+1].m_un8C1;
                     G1 = abs(-z1 - 2*x2 - z3 + z7 + 2*z9 + z9);
                     G2 = abs(-z1 - 2*z4 - z7 + z3 + 2*z6 + z9);
                     sum += G1;
                     sum += G2;
                  }
               }
            }
            break;
         // Variance
         case 3:
            mean = 0;
            dcnt = 0;
            // 分128上下
            meanlow = 0;
            dcntlow = 0;
            meanhigh = 0;
            dcnthigh = 0;
            sumlow = 0;
            sumhigh = 0;
            sum = 0;
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW)
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
               else

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
               for(col=0 ; col<width ; col+=1) {

                  // 2019 11 26 - chc 白點數量
                  // 2019 11 27 - chc 是Gray/Color? CCDImageAry
                  if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                     if(ptr1bw[col] > 250)
                        upwhitecnt++;

                     // 全計
                     if(ptr1bw[col] <= 255) {
                        mean += ptr1bw[col];
                        dcnt++;
                        // 分128上下
                        if(ptr1bw[col] == 255)
                           mean += 10;
                        if(ptr1bw[col] < 128) {
                           meanlow += ptr1bw[col];
                           dcntlow++;
                        }
                        else {
                           meanhigh += ptr1bw[col];
                           dcnthigh++;
                        }
                     }
                  }
                  else {

                     if(ptr1[col].m_un8C1 > 250)
                        upwhitecnt++;

                     // 全計
                     if(ptr1[col].m_un8C1 <= 255) {
                        mean += ptr1[col].m_un8C1;
                        dcnt++;
                        // 分128上下
                        if(ptr1[col].m_un8C1 == 255)
                           mean += 10;
                        if(ptr1[col].m_un8C1 < 128) {
                           meanlow += ptr1[col].m_un8C1;
                           dcntlow++;
                        }
                        else {
                           meanhigh += ptr1[col].m_un8C1;
                           dcnthigh++;
                        }
                     }
                  }
               }
            }
            if(dcnt > 0)
               mean = mean / dcnt;
            // 分128上下
            if(dcntlow > 0)
               meanlow = meanlow / dcntlow;
            if(dcnthigh > 0)
               meanhigh = meanhigh / dcnthigh;
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 全計
                     if(ptr1bw[col] <= 255) {
                        sum += abs(ptr1bw[col]-mean);
                        // 分128上下
                        if(ptr1bw[col] < 128)
                           sumlow += abs(ptr1bw[col]-meanlow);
                        else
                           sumhigh += abs(ptr1bw[col]-meanhigh);
                     }
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 全計
                     if(ptr1[col].m_un8C1 <= 255) {
                        sum += abs(ptr1[col].m_un8C1-mean);
                        // 分128上下
                        if(ptr1[col].m_un8C1 < 128)
                           sumlow += abs(ptr1[col].m_un8C1-meanlow);
                        else
                           sumhigh += abs(ptr1[col].m_un8C1-meanhigh);
                     }
                  }
               }
            }
            break;
         // Contrast
         case 4:
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW)
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
               else

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
               max = 0;
               min = 300;
               dcnt = 0;
               for(col=0 ; col<width ; col+=1) {

                  // 2019 11 26 - chc 白點數量
                  // 2019 11 27 - chc 是Gray/Color? CCDImageAry
                  if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                     if(ptr1bw[col] > 250)
                        upwhitecnt++;

                     // 過亮不計
                     if(ptr1bw[col] <= 240) {
                        if(ptr1bw[col] < min)
                           min = ptr1bw[col];
                        if(ptr1bw[col] > max)
                           max = ptr1bw[col];
                        dcnt++;
                        if(dcnt >= gap) {
                           sum += (max - min);
                           max = 0;
                           min = 300;
                           dcnt = 0;
                        }
                     }
                  }
                  else {

                     if(ptr1[col].m_un8C1 > 250)
                        upwhitecnt++;

                     // 過亮不計
                     if(ptr1[col].m_un8C1 <= 240) {
                        if(ptr1[col].m_un8C1 < min)
                           min = ptr1[col].m_un8C1;
                        if(ptr1[col].m_un8C1 > max)
                           max = ptr1[col].m_un8C1;
                        dcnt++;
                        if(dcnt >= gap) {
                           sum += (max - min);
                           max = 0;
                           min = 300;
                           dcnt = 0;
                        }
                     }
                  }
               }
               // 確認是否有效
               if(max !=0 || min != 300)
                  sum += (max - min);
            }
            break;
      }
      //
      //=================================================

      // 2019 11 26 - chc 白點數量
      sum += (upwhitecnt * whiteratio);

      if(sum > upvalue) {

         // 2019 11 26 - chc 前面已做過: AUO要改
         //GetPosition(Z_AXIS, &zuppos);
         //frnMain->pnlUpZPos->Caption = IntToStr((int)zuppos);

         frnMain->WriteSystemLog(">>UpFocus better: " + IntToStr(upvalue) + "," + frnMain->pnlUpZPos->Caption + "," + IntToStr(upwhitecnt));
         frnMain->pnlUpZPos->Refresh();
         upvalue = sum;
         frnMain->pnlSystemMessage->Caption = "Up Focus: " + IntToStr(upvalue) + "," + IntToStr((int)zuppos);
         frnMain->pnlUpValue->Caption = IntToStr(upvalue);
         frnMain->pnlUpValue->Refresh();

         // 2019 11 19 - chc Algorithm
         zupposbest = zuppos;
         // 若連續多次變弱就可以停下
         cnt = 0;
         // 要有變化才能比較
         oldsum = sum;

         // 2019 11 27 - chc 有比較才算
         upcount++;
         frnMain->pnlUpZCount->Caption = IntToStr(upcount);

      }

      // 2019 11 19 - chc Algorithm
      else {
         // 要有變化才能比較, 降低超過1%才算
         // => 用原方式就可以
         if(sum < oldsum) {
         //if(sum < oldsum && (double)(oldsum-sum)/oldsum > 0.01) {
            oldsum = sum;
            // 250以上就算
            if(sum > 250)
               cnt++;

            frnMain->pnlSystemMessage->Caption = "Up Focus: " + IntToStr(upvalue) + "," + IntToStr((int)zuppos);
            frnMain->pnlSystemMessage->Refresh();

            // 要顯示zuppos
            frnMain->WriteSystemLog(">>UpFocus worse: " + IntToStr(sum) + "," + IntToStr((int)zuppos));
            if(frnMain->cbAIFocus->Checked == true) {
               if(cnt > 4) {
                  StopOneAxis(Z_AXIS);
                  break;
               }
            }

            // 2019 11 27 - chc 有比較才算
            upcount++;
            frnMain->pnlUpZCount->Caption = IntToStr(upcount);

         }
      }

      Sleep(1);
      Application->ProcessMessages();
   }

   //===========================
   // 2019 11 26 - chc 要指定為已移動完成
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;

   // 到定位點: Z軸absolute移動
   frnMain->WriteSystemLog(">>DownFocus: MoveTo " + IntToStr(zpos) + " ...");
   MoveToZ(zpos,speed*10,0);
   frnMain->WriteSystemLog(">>DownFocus: MoveTo " + IntToStr(zpos+offset) + " ...");
   MoveToZ(zpos+offset,speed,1);
   frnMain->WriteSystemLog(">>DownFocus do...");
   // Motion Start...
   Sleep(10);
   Application->ProcessMessages();
   tout = 0;
   while(1) {

      // 2022 6 16a - chc APS
      //status = _8154_motion_done(Z_AXIS);
      status = 0;
      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // 看bit 0 : 不等於0(Done)
      if((done & 0x01) == 0)
         status = 1;

      if(status != 0 && status != 17 && status != 18)
         break;
      Sleep(1);
      Application->ProcessMessages();
      tout++;
      if(tout > 20)
         break;
   }

   frnMain->WriteSystemLog(">>DownFocus exe...");
   frnMain->pnlDownValue->Caption = IntToStr(downvalue);

   // 2019 11 19 - chc 若連續多次變弱就可以停下
   oldsum = 0;
   cnt = 0;
   frnMain->pnlSystemMessage->Caption = "Down Focus start...";

   while(1) {

      // 2019 11 19 - chc 若連續多次變弱就可以停下
      // 2019 11 27 - chc 有比較才算
      //downcount++;
      //frnMain->pnlDownZCount->Caption = IntToStr(downcount);

      // Motion done
      // 2022 6 16a - chc APS
      //status = _8154_motion_done(Z_AXIS);
      status = 0;
      done = APS_motion_status(Z_AXIS+StartAxisNo);
      // 看bit 0 : 不等於0(Done)
      if((done & 0x01) == 0)
         status = 1;

      if(status == 0 || status == 17 || status == 18) {
         frnMain->WriteSystemLog(">>DownFocus Motion done: " + IntToStr(status));
         break;
      }

      //=================================================
      // Sobel: 30
      //
      std = 30;
      width = x2 - x1;
      sum = 0;

      // 2019 11 27 - chc 要Reset
      downwhitecnt = 0;

      // 2019 11 26 - chc 檢測前要先讀取: AUO要改
      GetPosition(Z_AXIS, &zdownpos);
      frnMain->pnlDownZPos->Caption = IntToStr((int)zdownpos);
      // 記錄是否Z有變動, 若未變動則不做
      if(zdownpos_old == -1) {
         zdownpos_old = zdownpos;
      }
      else {
         if(zdownpos == zdownpos_old)
            continue;
         else
            zdownpos_old = zdownpos;
      }

      // 2019 11 19 - chc Algorithm
      switch(frnMain->rgFocusAlgorithm->ItemIndex) {
         // 差距總和
         case 0:
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  // 小差異不記錄
                  ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+3);         // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 白點數量
                     if(ptr1bw[col] > 250)
                        downwhitecnt++;

                     // 小差異不記錄
                     diff1 = abs(ptr1bw[col]-ptr1bw[col+3]);
                     diff2 = abs(ptr1bw[col]-ptr2bw[col]);
                     if(diff1 > 5)
                        sum += diff1;
                     else
                        sum += (diff1 / 2);
                     if(diff2 > 5)
                        sum += diff2;
                     else
                        sum += (diff2 / 2);
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  // 小差異不記錄
                  ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+3);         // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {

                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        downwhitecnt++;

                     // 小差異不記錄
                     diff1 = abs(ptr1[col].m_un8C1-ptr1[col+3].m_un8C1);
                     diff2 = abs(ptr1[col].m_un8C1-ptr2[col].m_un8C1);
                     if(diff1 > 5)
                        sum += diff1;
                     else
                        sum += (diff1 / 2);
                     if(diff2 > 5)
                        sum += diff2;
                     else
                        sum += (diff2 / 2);
                  }
               }
            }
            break;
         // Sobel
         case 1:

            for(row=y1+1 ; row<y2 ; row+=2) {
               //ptr  = ProcessImage.GetImagePtr(0,row);                                   // 取得Buffer位置

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                  ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                  ptr3bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                  for(col=1 ; col<width ; col+=2) {
                     // 白點數量
                     if(ptr1bw[col] > 250)
                        downwhitecnt++;

                     d[0] = ptr1bw[col-1];
                     d[1] = ptr1bw[col];
                     d[2] = ptr1bw[col+1];
                     d[3] = ptr2bw[col-1];
                     d[4] = ptr2bw[col];
                     d[5] = ptr2bw[col+1];
                     d[6] = ptr3bw[col-1];
                     d[7] = ptr3bw[col];
                     d[8] = ptr3bw[col+1];
                     xx = (double)(sx[0]*d[0] + sx[1]*d[1] + sx[2]*d[2]
                              + sx[3]*d[3] + sx[4]*d[4] + sx[5]*d[5]
                              + sx[6]*d[6] + sx[7]*d[7] + sx[8]*d[8]);
                     yy = (double)(sy[0]*d[0] + sy[1]*d[1] + sy[2]*d[2]
                              + sy[3]*d[3] + sy[4]*d[4] + sy[5]*d[5]
                              + sy[6]*d[6] + sy[7]*d[7] + sy[8]*d[8]);
                     zz = (double)(amp*sqrt(xx*xx+yy*yy));
                     data = (int)zz;
                     if(data > 255)
                        data = 255;
                     //ptr[col] = (char)data;
                     if(data > std)
                        sum++;
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                  ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                  ptr3 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                  for(col=1 ; col<width ; col+=2) {

                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        downwhitecnt++;

                     d[0] = ptr1[col-1].m_un8C1;
                     d[1] = ptr1[col].m_un8C1;
                     d[2] = ptr1[col+1].m_un8C1;
                     d[3] = ptr2[col-1].m_un8C1;
                     d[4] = ptr2[col].m_un8C1;
                     d[5] = ptr2[col+1].m_un8C1;
                     d[6] = ptr3[col-1].m_un8C1;
                     d[7] = ptr3[col].m_un8C1;
                     d[8] = ptr3[col+1].m_un8C1;
                     xx = (double)(sx[0]*d[0] + sx[1]*d[1] + sx[2]*d[2]
                              + sx[3]*d[3] + sx[4]*d[4] + sx[5]*d[5]
                              + sx[6]*d[6] + sx[7]*d[7] + sx[8]*d[8]);
                     yy = (double)(sy[0]*d[0] + sy[1]*d[1] + sy[2]*d[2]
                              + sy[3]*d[3] + sy[4]*d[4] + sy[5]*d[5]
                              + sy[6]*d[6] + sy[7]*d[7] + sy[8]*d[8]);
                     zz = (double)(amp*sqrt(xx*xx+yy*yy));
                     data = (int)zz;
                     if(data > 255)
                        data = 255;
                     //ptr[col] = (char)data;
                     if(data > std)
                        sum++;
                  }
               }

               // 要加入, 否則可能無法做tmMotionPolling!!
               if((row % 50) == 0) {
                  Sleep(1);
                  Application->ProcessMessages();
               }

            }
            frnMain->WriteSystemLog(">>UpFocus sobel: " + IntToStr(sum));

            // 2019 11 19 - chc Algorithm
            break;
         // Gradient: 最大梯度值
         case 2:
            for(col=1 ; col<width ; col++) {
               for(row=y1+1 ; row<y2 ; row++) {

                  // 2019 11 27 - chc 是Gray/Color? CCDImageAry
                  if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                     ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                     ptr2bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                     ptr3bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                     // 2019 11 26 - chc 白點數量
                     if(ptr1bw[col] > 250)
                        downwhitecnt++;
                     z1 = ptr1bw[col-1];
                     z2 = ptr2bw[col-1];
                     z3 = ptr3bw[col-1];
                     z4 = ptr1bw[col];
                     z6 = ptr3bw[col];
                     z7 = ptr1bw[col+1];
                     z8 = ptr2bw[col+1];
                     z9 = ptr3bw[col+1];
                     G1 = abs(-z1 - 2*x2 - z3 + z7 + 2*z9 + z9);
                     G2 = abs(-z1 - 2*z4 - z7 + z3 + 2*z6 + z9);
                     sum += G1;
                     sum += G2;
                  }
                  else {

                     ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row-1);                            // 取得Buffer位置
                     ptr2 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);                              // 取得Buffer位置
                     ptr3 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row+1);                            // 取得Buffer位置
                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        downwhitecnt++;
                     z1 = ptr1[col-1].m_un8C1;
                     z2 = ptr2[col-1].m_un8C1;
                     z3 = ptr3[col-1].m_un8C1;
                     z4 = ptr1[col].m_un8C1;
                     z6 = ptr3[col].m_un8C1;
                     z7 = ptr1[col+1].m_un8C1;
                     z8 = ptr2[col+1].m_un8C1;
                     z9 = ptr3[col+1].m_un8C1;
                     G1 = abs(-z1 - 2*x2 - z3 + z7 + 2*z9 + z9);
                     G2 = abs(-z1 - 2*z4 - z7 + z3 + 2*z6 + z9);
                     sum += G1;
                     sum += G2;
                  }
               }
            }
            break;
         // Variance
         case 3:
            mean = 0;
            dcnt = 0;
            // 分128上下
            meanlow = 0;
            dcntlow = 0;
            meanhigh = 0;
            dcnthigh = 0;
            sumlow = 0;
            sumhigh = 0;
            sum = 0;
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 白點數量
                     if(ptr1bw[col] > 250)
                        downwhitecnt++;
                     // 全計
                     if(ptr1bw[col] <= 255) {
                        mean += ptr1bw[col];
                        dcnt++;
                        // 分128上下
                        if(ptr1bw[col] == 255)
                           mean += 10;
                        if(ptr1bw[col] < 128) {
                           meanlow += ptr1bw[col];
                           dcntlow++;
                        }
                        else {
                           meanhigh += ptr1bw[col];
                           dcnthigh++;
                        }
                     }
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {

                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        downwhitecnt++;

                     // 全計
                     if(ptr1[col].m_un8C1 <= 255) {
                        mean += ptr1[col].m_un8C1;
                        dcnt++;
                        // 分128上下
                        if(ptr1[col].m_un8C1 == 255)
                           mean += 10;
                        if(ptr1[col].m_un8C1 < 128) {
                           meanlow += ptr1[col].m_un8C1;
                           dcntlow++;
                        }
                        else {
                           meanhigh += ptr1[col].m_un8C1;
                           dcnthigh++;
                        }
                     }
                  }
               }
            }
            if(dcnt > 0)
               mean = mean / dcnt;
            // 分128上下
            if(dcntlow > 0)
               meanlow = meanlow / dcntlow;
            if(dcnthigh > 0)
               meanhigh = meanhigh / dcnthigh;
            for(row=y1+1 ; row<y2 ; row+=1) {

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 全計
                     if(ptr1bw[col] <= 255) {
                        sum += abs(ptr1bw[col]-mean);
                        // 分128上下
                        if(ptr1bw[col] < 128)
                           sumlow += abs(ptr1bw[col]-meanlow);
                        else
                           sumhigh += abs(ptr1bw[col]-meanhigh);
                     }
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 全計
                     if(ptr1[col].m_un8C1 <= 255) {
                        sum += abs(ptr1[col].m_un8C1-mean);
                        // 分128上下
                        if(ptr1[col].m_un8C1 < 128)
                           sumlow += abs(ptr1[col].m_un8C1-meanlow);
                        else
                           sumhigh += abs(ptr1[col].m_un8C1-meanhigh);
                     }
                  }
               }
            }
            break;
         // Contrast
         case 4:
            for(row=y1+1 ; row<y2 ; row+=1) {
               max = 0;
               min = 300;
               dcnt = 0;

               // 2019 11 27 - chc 是Gray/Color? CCDImageAry
               if(frnMain->CCDInfoAry[no].Type == CCD_BW) {
                  ptr1bw = (unsigned char *)frnMain->CCDImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {
                     // 白點數量
                     if(ptr1bw[col] > 250)
                        downwhitecnt++;
                     // 過亮不計(Up已有)
                     if(ptr1bw[col] <= 240) {
                        if(ptr1bw[col] < min)
                           min = ptr1bw[col];
                        if(ptr1bw[col] > max)
                           max = ptr1bw[col];
                        dcnt++;
                        if(dcnt >= gap) {
                           sum += (max - min);
                           max = 0;
                           min = 300;
                           dcnt = 0;
                        }
                     }
                  }
               }
               else {

                  ptr1 = (EC24 *)frnMain->usb_ImageAry[no].GetImagePtr(x1,row);           // 取得Buffer位置
                  for(col=0 ; col<width ; col+=1) {

                     // 2019 11 26 - chc 白點數量
                     if(ptr1[col].m_un8C1 > 250)
                        downwhitecnt++;

                     // 過亮不計(Up已有)
                     if(ptr1[col].m_un8C1 <= 240) {
                        if(ptr1[col].m_un8C1 < min)
                           min = ptr1[col].m_un8C1;
                        if(ptr1[col].m_un8C1 > max)
                           max = ptr1[col].m_un8C1;
                        dcnt++;
                        if(dcnt >= gap) {
                           sum += (max - min);
                           max = 0;
                           min = 300;
                           dcnt = 0;
                        }
                     }
                  }
               }
               sum += (max - min);
            }
            break;
      }


      // 2019 11 26 - chc 白點數量
      sum += (downwhitecnt * whiteratio);

      if(sum > downvalue) {

         // 2019 11 26 - chc 前面已做過: AUO要改
         //GetPosition(Z_AXIS, &zdownpos);
         //frnMain->pnlDownZPos->Caption = IntToStr((int)zdownpos);

         frnMain->WriteSystemLog(">>DownFocus better: " + IntToStr(downvalue) + "," + frnMain->pnlDownZPos->Caption + "," + IntToStr(downwhitecnt));
         frnMain->pnlDownZPos->Refresh();
         downvalue = sum;
         frnMain->pnlSystemMessage->Caption = "Down Focus: " + IntToStr(downvalue) + "," + IntToStr((int)zdownpos);
         frnMain->pnlDownValue->Caption = IntToStr(downvalue);
         frnMain->pnlDownValue->Refresh();

         // 2019 11 19 - chc 若連續多次變弱就可以停下
         cnt = 0;
         // 要有變化才能比較
         oldsum = sum;
         zdownposbest = zdownpos;

         // 2019 11 27 - chc 有比較才算
         downcount++;
         frnMain->pnlDownZCount->Caption = IntToStr(downcount);

      }

      // 2019 11 19 - chc 若連續多次變弱就可以停下
      else {
         // 要有變化才能比較, 降低超過1%才算
         // => 用原方式就可以
         if(sum < oldsum) {
         //if(sum < oldsum && (double)(oldsum-sum)/oldsum > 0.01) {

            oldsum = sum;
            // 250以上就算
            if(sum > 250)
               cnt++;

            frnMain->pnlSystemMessage->Caption = "Down Focus: " + IntToStr(downvalue) + "," + IntToStr((int)zdownpos);
            frnMain->pnlSystemMessage->Refresh();

            // 要顯示zdownpos
            frnMain->WriteSystemLog(">>DownFocus worse: " + IntToStr(sum) + "," + IntToStr((int)zdownpos));
            if(frnMain->cbAIFocus->Checked == true) {
               if(cnt > 4) {
                  StopOneAxis(Z_AXIS);
                  break;
               }
            }

            // 2019 11 27 - chc 有比較才算
            downcount++;
            frnMain->pnlDownZCount->Caption = IntToStr(downcount);

         }
      }

      Sleep(1);
      Application->ProcessMessages();
   }

   // 找定位
   if(zdownposbest == -1 && zupposbest == -1) {
      frnMain->pnlAutoFocus->Color = clRed;
      frnMain->pnlAutoFocus1->Color = clRed;
      frnMain->pnlSystemMessage->Caption = "Focus Fail!";
      frnMain->WriteSystemLog(">>Focus => 失敗");
   }
   else if(zupposbest == -1) {
      MoveToZ(zdownposbest,speed*5,0);
      frnMain->pnlAutoFocus->Color = clLime;
      frnMain->pnlAutoFocus1->Color = clLime;
      frnMain->pnlSystemMessage->Caption = "Focus Done: " + IntToStr((int)zdownposbest);
      frnMain->WriteSystemLog(">>Focus成功(Down): " + IntToStr((int)zdownposbest));
      bestz = zdownposbest;

      // 加入Loop
      //bestvalue = downvalue;

   }
   else if(zdownposbest == -1) {
      MoveToZ(zupposbest,speed*5,0);
      frnMain->pnlAutoFocus->Color = clLime;
      frnMain->pnlAutoFocus1->Color = clLime;
      frnMain->pnlSystemMessage->Caption = "Focus Done: " + IntToStr((int)zupposbest);
      frnMain->WriteSystemLog(">>Focus成功(Up): " + IntToStr((int)zupposbest));
      bestz = zupposbest;

      // 加入Loop
      //bestvalue = upvalue;

   }
   else {
      if(upvalue > downvalue) {
         MoveToZ(zupposbest,speed*5,0);
         frnMain->pnlSystemMessage->Caption = "Focus Done: " + IntToStr((int)zupposbest);
         frnMain->WriteSystemLog(">>Focus成功(Up): " + IntToStr((int)zupposbest));
         bestz = zupposbest;

         // 加入Loop
         //bestvalue = upvalue;

      }
      else {
         MoveToZ(zdownposbest,speed*5,0);
         frnMain->pnlSystemMessage->Caption = "Focus Done: " + IntToStr((int)zdownposbest);
         frnMain->WriteSystemLog(">>Focus成功(Down): " + IntToStr((int)zdownposbest));
         bestz = zdownposbest;

         // 加入Loop
         //bestvalue = downvalue;

      }
      frnMain->pnlAutoFocus->Color = clLime;
      frnMain->pnlAutoFocus1->Color = clLime;
   }
   frnMain->iAxisMove[Z_AXIS] = NO_MOVE;

   // 2022 6 19 - chc 成功才要移動
   if(frnMain->pnlAutoFocus->Color == clLime) {

      frnMain->WriteSystemLog(">>Focus成功, 移動到: " + IntToStr(bestz));
      MoveToZ(bestz,speed*5,0);
   }
}
//---------------------------------------------------------------------------

void __fastcall UpdateFocus()
{
AnsiString msg;
TIniFile *pSystemFile;

   // Write to .ini
   frnMain->pnlSystemMessage->Caption = "更新對焦參數...";
   // ini檔名與目錄
   pSystemFile = new TIniFile(frnMain->FocusINIFilename);

   // General
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Z"                       ,frnMain->edAutoFocus->Text);
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Z Offset"                ,frnMain->edAutoFocusOffset->Text);
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Z Speed"                 ,frnMain->edAutoFocusSpeed->Text);

   // 2019 11 27 - chc 加入Quick Mode & Algorithm
   pSystemFile->WriteBool(FOCUS_INFORMATION_SECTION,"Z Quick Mode"              ,frnMain->cbAIFocus->Checked);
   pSystemFile->WriteInteger(FOCUS_INFORMATION_SECTION,"Z Focus Algorithm"      ,frnMain->rgFocusAlgorithm->ItemIndex);

   // 2020 6 4 - chc 加入對比量: edDifferenceValue
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Difference Value"        ,frnMain->edDifferenceValue->Text);

   // 2020 6 18 - chc 加入加權: edDifferenceRatio
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Difference Ratio"        ,frnMain->edDifferenceRatio->Text);

   // 2021 10 1 - chc 加入厚度: edAutoFocusThickness
   pSystemFile->WriteString(FOCUS_INFORMATION_SECTION,"Z Thickness     "        ,frnMain->edAutoFocusThickness->Text);

   delete pSystemFile;
   frnMain->pnlSystemMessage->Caption = "更新對焦參數完成.";
}
//---------------------------------------------------------------------------


#pragma package(smart_init)


