
#define Press_mb
///qqq///////////////////////////////////
#define max_pr_mb 2

//27.01.20 YN  -----\\//-----
float Press_N2=0.;
//27.01.20 YN  -----//\\-----

//---------------
struct s_icp_dev Pr_mb[max_pr_mb]=
{
// device 1
0,                // status
2,                // port
7,               // addr
"Pr_mb",        // name[8]
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
&Pr_mb_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
77,               // addr
"Pr_mb/2",        // name[8]
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
&Pr_mb_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst Pr_mb_Rqst[max_pr_mb]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Pr_mb_rd,            //  answ_com
f_Pr_mb_rd,            //  answ_flt
"Pr_mb",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Pr_mb[0],            //  *ICP_dd
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
f_Pr_mb_rd,            //  answ_com
f_Pr_mb_rd,            //  answ_flt
"Pr_mb/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Pr_mb[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

unsigned char Pressure_RD[] ={7,3,0x00,0x10,0x00,0x02};  //строка запроса два регистра
void f_Pr_mb(int ii)
{
  
int i;
char buf_tmp[20];
int i1,i2;

 if(Pr_mb[ii].status == 0) return;

 if( f_timer(Pr_mb[ii].time_stamp_pool,Pr_mb[ii].pool_time ))
 {
  if((Pr_mb_Rqst[ii].status == Req_Flt) ||
     (Pr_mb_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(Pr_mb[ii].time_stamp_pool,(Pr_mb[ii].pool_time+Pr_mb[ii].pool_time) ))
           goto m1;
   return;
m1:
        i1=6;
        _fmemcpy(buf_tmp,Pressure_RD,(long int) 6);

           buf_tmp[0]=Pr_mb[ii].addr;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Pr_mb_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Pr_mb_Rqst[ii].cmd_lgth=i2;
           Pr_mb_Rqst[ii].time_stamp=TimeStamp;
           Pr_mb[ii].time_stamp_pool=TimeStamp;
           Pr_mb[ii].n_transaction++;
           f_queue_put(Pr_mb[ii].port, &Pr_mb_Rqst[ii]);
 }
}
/*----------------------------*/

float var_pr_mb=0.0;
unsigned int Pr_mb_inp_err[max_pr_mb]={0,0};

void f_Pr_mb_rd(int ii)
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

  var_pr_mb = f_get_float( &cb_COM[ii][0], 0);

  //27.01.20 YN  -----\\//-----
  //was:
  //s_MVD[0].Press= var_pr_mb;
  if(Pr_415m[0].status == 0) 
      {
      s_MVD[0].Press= var_pr_mb;
      }
  else {
      Press_N2= var_pr_mb;
  }
  //27.01.20 YN  -----//\\-----

//  INP_OFP[nn]= f_get_int( &cb_COM[ii][0]); ;

 m_end:
  Pr_mb[nn].n_success++;
  Pr_mb_inp_err[nn]=0;
  f_queue_free(ii,&Pr_mb_Rqst[nn]);
  return;

m_crc_err:
 Pr_mb[nn].time_stamp_rcv=0;
Pr_mb[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Pr_mb[nn].time_stamp_rcv=0;
 Pr_mb[nn].n_timeout_error++;

m_err:
 Pr_mb_inp_err[nn]++;

 if(Pr_mb_inp_err[nn]>=Pr_mb_max_inp_err)
 {
   Pr_mb_inp_err[nn]=Pr_mb_max_inp_err;
   f_icp_error(&Pr_mb[nn],RD_ERR );
 }
 f_queue_free(ii,&Pr_mb_Rqst[nn]);
}