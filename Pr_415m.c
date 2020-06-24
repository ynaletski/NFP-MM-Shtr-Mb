
#define Press_415m
///www///////////////////////////
#define max_pr_415 2

//---------------
struct s_icp_dev Pr_415m[max_pr_415]=
{
// device 1
0,                // status
2,                // port
6,               // addr
"Pr_415m",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.2",           // firmware
0,                // protocol
1,                // CRC_flag
400,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Pr_415m_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
66,               // addr
"Pr_415/2",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B2.2",           // firmware
0,                // protocol
1,                // CRC_flag
150,              // pool_time
0,                // time_stamp_pool
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Pr_415m_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst Pr_415m_Rqst[max_pr_415]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Pr_415m_rd,            //  answ_com
f_Pr_415m_rd,            //  answ_flt
"Pr_415m",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Pr_415m[0],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Pr_415m_rd,            //  answ_com
f_Pr_415m_rd,            //  answ_flt
"Pr_415/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Pr_415m[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

unsigned char Pr_415m_RD[] ={6,4,0x00,0x54,0x00,0x02};  //строка запроса два регистра
void f_Pr_415m(int ii)
{

int i;
char buf_tmp[20];
int i1,i2;

 if(Pr_415m[ii].status == 0) return;

 if( f_timer(Pr_415m[ii].time_stamp_pool,Pr_415m[ii].pool_time ))
 {
  if((Pr_415m_Rqst[ii].status == Req_Flt) ||
     (Pr_415m_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(Pr_415m[ii].time_stamp_pool,(Pr_415m[ii].pool_time+Pr_415m[ii].pool_time) ))
           goto m1;
   return;
m1:
        i1=6;
        _fmemcpy(buf_tmp,Pr_415m_RD,(long int) 6);

           buf_tmp[0]=Pr_415m[ii].addr;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Pr_415m_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Pr_415m_Rqst[ii].cmd_lgth=i2;
           Pr_415m_Rqst[ii].time_stamp=TimeStamp;
           Pr_415m[ii].time_stamp_pool=TimeStamp;
           Pr_415m[ii].n_transaction++;
           f_queue_put(Pr_415m[ii].port, &Pr_415m_Rqst[ii]);
 }
}
/*----------------------------*/

float var_pr_415m=0.0;
unsigned int Pr_415m_inp_err[max_pr_415]={0,0};

void f_Pr_415m_rd(int ii)
{
// ii - номер порта
//
 struct COM_rqst *request;
 int i,nn,itmp;

 int count;

 count=n_bcom[ii];  // длина команды
 request=COM_que[ii].request[COM_que[ii].empt_ptr];
 nn=request->n_dev; // номер устройства OFP (0,1..)

 if(count < 3)
   {
      goto m_tout_err;
   }
 if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
 if(cb_COM[ii][1] & 0x80)
   {
//     OFP[nn].exc=cb_COM[ii][2];
//     OFP_fn[nn]=F_FAULT ;
     goto m_end;
   }

   if(count < 5)
   {
       goto m_tout_err;
   }

  var_pr_415m = f_get_float( &cb_COM[ii][0], 1);
  var_pr_415m /= 1000;

  s_MVD[0].Press= var_pr_415m;

//  INP_OFP[nn]= f_get_int( &cb_COM[ii][0]); ;

 m_end:
  Pr_415m[nn].n_success++;
  Pr_415m_inp_err[nn]=0;
  f_queue_free(ii,&Pr_415m_Rqst[nn]);
  return;

m_crc_err:
 Pr_415m[nn].time_stamp_rcv=0;
Pr_415m[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Pr_415m[nn].time_stamp_rcv=0;
 Pr_415m[nn].n_timeout_error++;

m_err:
 Pr_415m_inp_err[nn]++;

 if(Pr_415m_inp_err[nn]>=Pr_415m_max_inp_err)
 {
   Pr_415m_inp_err[nn]=Pr_415m_max_inp_err;
   f_icp_error(&Pr_415m[nn],RD_ERR );
 }
 f_queue_free(ii,&Pr_415m_Rqst[nn]);
}