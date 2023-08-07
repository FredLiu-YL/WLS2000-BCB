//---------------------------------------------------------------------------


#pragma hdrstop

#include "Socket.h"

bool boolInSocketRead;

//---------------------------------------------------------------------------
// mode: 0-���[�J����, 1-�w�[�J����
void __fastcall SendSocket(char *str)
{
int len;
char buf[500];

   sprintf(buf,"%s\r\n",str);
   len = strlen(buf);

   frnMain->csQRCode1->Socket->SendBuf(buf,len);
}
//---------------------------------------------------------------------------
// mode: 0-���[�J����, 1-�w�[�J����
void __fastcall SendSocket2(char *str)
{
int len;
char buf[500];

   sprintf(buf,"%s\r\n",str);
   len = strlen(buf);

   frnMain->csQRCode2->Socket->SendBuf(buf,len);
}

//---------------------------------------------------------------------------
// Add Message
void __fastcall AddMessage(AnsiString msg)
{
AnsiString putmsg;
static AnsiString oldmsg = "";

   if(oldmsg == msg)                            // ���Ƥ�����
      return;
   oldmsg = msg;
   putmsg = Now().FormatString("hh':'nn':'ss':'zzz'-> '") + msg;
   // �HMemo���N, �̦h�@470 Rows
   frnMain->mmQRCodeMessage->Lines->Insert(0,putmsg);

   // 2019 4 16 - chc Realtime
   frnMain->mmQRCodeMessage->Refresh();

   if(frnMain->mmQRCodeMessage->Lines->Count > 500) {
      for(int i=470 ; i<frnMain->mmQRCodeMessage->Lines->Count ; i++)
         frnMain->mmQRCodeMessage->Lines->Delete(i);
   }
}
//---------------------------------------------------------------------------
// Add Message
void __fastcall AddMessage2(AnsiString msg)
{
AnsiString putmsg;
static AnsiString oldmsg = "";

   if(oldmsg == msg)                            // ���Ƥ�����
      return;
   oldmsg = msg;
   putmsg = Now().FormatString("hh':'nn':'ss':'zzz'-> '") + msg;
   // �HMemo���N, �̦h�@470 Rows
   frnMain->mmQRCodeMessage2->Lines->Insert(0,putmsg);

   // 2019 4 16 - chc Realtime
   frnMain->mmQRCodeMessage2->Refresh();

   if(frnMain->mmQRCodeMessage2->Lines->Count > 500) {
      for(int i=470 ; i<frnMain->mmQRCodeMessage2->Lines->Count ; i++)
         frnMain->mmQRCodeMessage2->Lines->Delete(i);
   }
}
//---------------------------------------------------------------------------
// Client Socket
// ���Ψ�
void __fastcall DoSocketRead(AnsiString text)
{
static AnsiString eceivestr;
static bool inactive = false;
AnsiString cmdstr,slen,scmd,spar[3];
bool flag,done;
int pos,parcnt,len;
char buf[1024],sendbuf[50];

   parcnt = 0;
   AddMessage("Socket ����.");
   sprintf(buf,"%s",text.c_str());
   len = text.Length();
   frnMain->WriteSystemLog("Socket Recv: " + text);

   // ���q
   while((pos = text.Pos(">")) != 0) {
      done = false;
      parcnt = 0;
      // ���o����R�O�r��
      cmdstr = text.SubString(1,pos);
      frnMain->pnlSocket->Caption = cmdstr;
      frnMain->WriteSystemLog("Socket Recv cmd: " + cmdstr);
      AddMessage("[<] " + cmdstr);

      text.Delete(1,pos);
      // Cmd
      pos = cmdstr.Pos(",");
      if(pos == 0) {
         pos = cmdstr.Pos(">");
         if(pos == 0) {
            AddMessage("[<] Invalid");
            flag = false;
            continue;
         }
         else {
            scmd = cmdstr.SubString(1,pos-1);
            cmdstr.Delete(1,pos);
            done = true;
         }
      }
      else {
         scmd = cmdstr.SubString(1,pos-1);
         cmdstr.Delete(1,pos);
      }
      AddMessage("[<] " + scmd);

      // Par
      while(done == false) {
         pos = cmdstr.Pos(",");
         if(pos == 0) {
            pos = cmdstr.Pos(">");
            if(pos == 0) {
               AddMessage("[<] Invalid");
               flag = false;
               continue;
            }
            else {
               spar[parcnt++] = cmdstr.SubString(1,pos-1);
               cmdstr.Delete(1,pos);
               done = true;
            }
         }
         else {
            spar[parcnt++] = cmdstr.SubString(1,pos-1);
            cmdstr.Delete(1,pos);
         }

         // parcnt > 0
         if(parcnt > 0)
            AddMessage("[LC] " + spar[parcnt-1] + " : " + IntToStr(parcnt));
         if(cmdstr.Length() <= 0)
            break;
      }

   }
   inactive = false;

}
//---------------------------------------------------------------------------

#pragma package(smart_init)
