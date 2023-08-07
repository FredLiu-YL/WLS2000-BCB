//---------------------------------------------------------------------------

#ifndef MotionH
#define MotionH

#include "type_def.h"

   void __fastcall InitMotion();
   void __fastcall SetGreenLamp(bool mode);
   void __fastcall SetYellowLamp(bool mode);
   // 更新JogSpeed
   void __fastcall UpdateMotionStatus();
   void __fastcall AllHome();
   void __fastcall HomeOneAxis();
   void __fastcall ResetCommand();
   void __fastcall ResetPosition();
   bool __fastcall MoveForwardMoueUp();
   bool __fastcall MoveBackwardMoueUp();

   void __fastcall GetAxisData(int axisno,int *dist,int *startvel,int *maxvel,F64 *tacc,F64 *tdec,int *homevel, int *reviewvel, int* uplimit, int *lowdistance);
   bool __fastcall WaitMotionDone(int axisno, int ms);
   void __fastcall SetCenterPosition();
   void __fastcall ToCenterPosition();
   void __fastcall SetLoadPosition();
   void __fastcall ToLoadPosition();
   void __fastcall Set0Position();
   void __fastcall To0Position(int mode);
   void __fastcall Set45Position();
   void __fastcall To45Position(int mode);
   void __fastcall SetOriginalPosition();
   void __fastcall ToOriginalPosition(int mode);
   void __fastcall SetCCDPosition();
   void __fastcall ToCCDPosition(int mode);

   // 2013 2 17 - chc Wait Motion X/Y/Z done
   bool __fastcall WaitMotionXYZDone(int ms);
   bool __fastcall WaitMotionXYZTDone(int ms);
   void __fastcall DecodeIOStatus(int axisno,U16 io_sts);
   void __fastcall GetAxisPosition(int axisno, int *position);
   bool __fastcall MoveAbsolute(int axisno);
   void __fastcall StopAxis(int axisno);
   void __fastcall GetAxisName(int axisno);
   void __fastcall ResetErrorCounter();
   void __fastcall SetAlmOn();
   void __fastcall SetAlmOff();
   void __fastcall SetServoOn();
   void __fastcall SetServoOff();
   // 前進: 要判定Limit問題
   void __fastcall MoveForward();
   // 各軸上下限檢查
   // dist: 相對移動量
   void __fastcall CheckUpDownLimit(int axisno,int *dist, int uplimit);
   void __fastcall GetPosition(int axisno, I32 *pos);
   void __fastcall WaitMotionDoneXY(int mode, int ms);
   void __fastcall WaitMotionDoneXYZ(int mode, int ms);
   void __fastcall DoAxisTaMove(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec);
   void __fastcall DoAxisTrMove(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec);
   void __fastcall GetCommand(int axisno, I32 *cmd);
   void __fastcall WriteDO(int idno);
   void __fastcall WaitRoutine1(int sleepno);
   void __fastcall SetCommand(int axisno, I32 cmd);
   void __fastcall SetMPosition(int axisno, F64 pos);
   void __fastcall GetIOStatus(int axisno, U16 *iostatus);
   // 2013 2 23 - chc DO Reset 0
   void __fastcall InitDO();
   void __fastcall Beep(int mstime);
   void __fastcall BeepStop();

   // 2013 4 8 - chc EMC/Door Open Alarm Counter
   int DoorCounter;
   int EMCCounter;

   // 2013 5 15 - chc Sleep - Motion
   void __fastcall DelayTimeM2(int no);                                         // WaitMotionDone()使用
   void __fastcall DelayTimeM3(int no);                                         // AllHome()使用
   void __fastcall DelayTimeM4(int no);                                         // WaitMotionXYZDone()使用
   void __fastcall DelayTimeM5(int no);                                         // WaitMotionDoneXY(), WaitMotionDoneXYZ(), WaitMotionDLFDone使用
   void __fastcall DelayTimeM10(int no);                                        // WaitMotionXYDoneForScan使用

   // 2013 9 27 - chc Z軸移動
   // 2022 6 16a - chc 加入mode: 0 - wait, 1 - nowait
   //bool __fastcall MoveToZ(int position, int speed);
   bool __fastcall MoveToZ(int position, int speed, int mode);

   bool __fastcall MoveToY(int position);
   bool __fastcall MoveToT(int position);
   void __fastcall GetTimeTicM(long *time, short *millitm);

   // 2013 10 8 - chc 到X的AOI點
   void __fastcall ToXAOIPosition();
   // 2013 10 8 - chc 移動X
   bool __fastcall MoveToXGap(int dx);

   // 2013 10 16 - chc X軸absolute移動
   bool __fastcall MoveToX(int position);
   // 2013 10 17 - chc T旋轉安全考量
   // 0: -9223
   // 90: 227777
   // 總共: 237000
   // 精度: 0.0003794度/pulse
   // 允許範圍: 正負5度(13178), [-22400,3955]
   bool __fastcall CheckTMove();

   // 2013 11 12 - chc 確認安全角度
   //#define T_RESOLUTION    0.0003795
   // 2013 11 12 - chc 讀取Y軸目前位置
   int __fastcall GetYPosition();
   // 2013 11 12 - chc 往前移動90度, 上限為93度
   void __fastcall MoveToTAdd90();

   // 2014 9 8 - chc 往後移動90度, 下限為-5度
   void __fastcall MoveToTSub90();

   // 2021 5 16 - chc new add
   bool __fastcall GetNELStatus(int axisno);
   bool __fastcall GetMotionDoneStatus(int axisno);
   int __fastcall RelativeMove(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec);
   int __fastcall AbsoluteMove(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec);
   void __fastcall SetACC_DEC_StartV(int axisno, double acc, double dec, int startv, int maxvel);
   bool __fastcall MoveToXNoWait(int position,int speed, int mode);
   void __fastcall StopOneAxis(int axisno);

   // Aligner
   bool __fastcall WaitMotionDoneCCD(int axisno, int ms);

   void __fastcall DoAxisTrMoveNoWait(int axisno, int pos, int startvel, int maxvel, F64 tacc, F64 tdec);
   int __fastcall RelativeMoveNoWait(I16 axisno,int pos,int startvel,int maxvel,F64 tacc,F64 tdec);

   // 2022 6 16a - chc
   bool __fastcall MoveToXYRatio(int xpos,int ypos, double ratio);
   bool __fastcall MoveToXY(int xpos,int ypos);
   void __fastcall AxisMove(int mode, int type);
   void __fastcall StopOtherAxis(int axisno);
   int __fastcall AbsoluteMoveXY(I16 axisno,F64 fxpos,F64 fypos,int startvel,int maxvel,F64 tacc,F64 tdec);
   void __fastcall StopAllAxis();
   void __fastcall SetBlueLamp(bool mode);
   void __fastcall SetRedLamp(bool mode);
   void __fastcall SetLensPosition(int no);
   void __fastcall ToLensPosition(int no);
   void __fastcall SetLensZLimit(int no);
   void __fastcall WaitZFinish(int z);
   void __fastcall DoRelativeMove(int axisno, int offset);
   void __fastcall ToR45Position(int mode);
   void __fastcall SetR45Position();
   void __fastcall SetRingLamp(bool mode);

   I32 __fastcall GetDI(int id);
   void __fastcall ToWPositionFun(int no);
   void __fastcall DelayTimeM1(int no);
   void __fastcall SetWPosition(int no);

   // 2022 12 5 - chc 測試Signal Tower: R/Y/G/B/Buzzer
   void __fastcall TestSignalTower();

   // 2023 1 15 - chc Check Limit
   void __fastcall CheckLimitAxis(int axisno,int *pos);
   void __fastcall CheckLimit(int *x,int *y,int *z,int *t);

   void __fastcall AdjustXPos();

   void __fastcall DelayTimeM2W(int no);
   void __fastcall DelayTimeM2Z(int no);
   bool __fastcall WaitMotionDoneW(int axisno, int ms);
   bool __fastcall WaitMotionDoneZ(int axisno, int ms);

   // 2023 2 25 - chc 開啟FFU
   void __fastcall SetFFU(bool mode);

   // 2023 3 14 - chc 8Edge
   void __fastcall Set12EdgePos(int no);
   void __fastcall Set8EdgePos(int no);
   void __fastcall To12EdgePos(int no);
   void __fastcall To8EdgePos(int no);


//---------------------------------------------------------------------------
#endif
