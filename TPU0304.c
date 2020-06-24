
#define Temp_tpu
///rrr////////////////////////////////
#define max_Temp_TPU 2

//---------------
struct s_icp_dev Temp_TPU[max_Temp_TPU]=
{
// device 1
0,                // status
2,                // port
8,               // addr
"Temp_Mb",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B3.3",           // firmware
0,                // protocol
1,                // CRC_flag
500,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Temp_TPU_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
2,                // port
88,               // addr
"TPU/2",        // name[8]
06,               // baudrate_cod
0xd,              // type
0x40,             // format
0xff,             // channels
"B3.3",           // firmware
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
&Temp_TPU_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst Temp_TPU_Rqst[max_Temp_TPU]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Temp_TPU_rd,            //  answ_com
f_Temp_TPU_rd,            //  answ_flt
"Temp_TPU",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Temp_TPU[0],            //  *ICP_dd
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
f_Temp_TPU_rd,            //  answ_com
f_Temp_TPU_rd,            //  answ_flt
"TPU/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Temp_TPU[1],            //  *ICP_dd
0,                     //  текущая функция
100,                   //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/

unsigned char Temp_TPU_RD[] ={8,3,0x00,0x00,0x00,0x02};  //строка запроса 2 регистра
void f_Temp_tpu(int ii)
{

int i;
char buf_tmp[20];
int i1,i2;

 if(Temp_TPU[ii].status == 0) return;

 if( f_timer(Temp_TPU[ii].time_stamp_pool,Temp_TPU[ii].pool_time ))
 {
  if((Temp_TPU_Rqst[ii].status == Req_Flt) ||
     (Temp_TPU_Rqst[ii].status == Req_OK)) goto m1;
  else if( f_timer(Temp_TPU[ii].time_stamp_pool,(Temp_TPU[ii].pool_time+Temp_TPU[ii].pool_time) ))
           goto m1;
   return;
m1:
        i1=6;
        _fmemcpy(buf_tmp,Temp_TPU_RD,(long int) 6);

           buf_tmp[0]=Temp_TPU[ii].addr;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Temp_TPU_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Temp_TPU_Rqst[ii].cmd_lgth=i2;
           Temp_TPU_Rqst[ii].time_stamp=TimeStamp;
           Temp_TPU[ii].time_stamp_pool=TimeStamp;
           Temp_TPU[ii].n_transaction++;
           f_queue_put(Temp_TPU[ii].port, &Temp_TPU_Rqst[ii]);
 }
}
/*----------------------------*/

float var_Temp_TPU=0.0;
unsigned int Temp_TPU_inp_err[max_Temp_TPU]={0,0};

void f_Temp_TPU_rd(int ii)
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

  var_Temp_TPU = f_get_float( &cb_COM[ii][0], 0);
  s_MVD[0].TempR= var_Temp_TPU;
//  INP_OFP[nn]= f_get_int( &cb_COM[ii][0]); ;

 m_end:
  Temp_TPU[nn].n_success++;
  Temp_TPU_inp_err[nn]=0;
  f_queue_free(ii,&Temp_TPU_Rqst[nn]);
  return;

m_crc_err:
 Temp_TPU[nn].time_stamp_rcv=0;
Temp_TPU[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Temp_TPU[nn].time_stamp_rcv=0;
 Temp_TPU[nn].n_timeout_error++;

m_err:
 Temp_TPU_inp_err[nn]++;

 if(Temp_TPU_inp_err[nn]>=Temp_TPU_max_inp_err)
 {
   Temp_TPU_inp_err[nn]=Temp_TPU_max_inp_err;
   f_icp_error(&Temp_TPU[nn],RD_ERR );
 }
 f_queue_free(ii,&Temp_TPU_Rqst[nn]);
}