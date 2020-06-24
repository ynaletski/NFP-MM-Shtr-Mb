////06.02.2020 YN  -----\\//-----
// электропривод "VESPER"
//
//    пример включения
// Vesper_SV[0]=freq; // задание скорости
// Vesper_Ctrl[0]=START_FWD_VESPER;
// while((Vesper_State[0] & MSK_RUN_VESPER ) !=RUN_VESPER )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//  ....
//
// Vesper_Ctrl[0]=STOP_FWD_VESPER;
// while((Vesper_State[0] & MSK_RUN_VESPER ) !=STP_VESPER )
//  {
//    if(FL_err != 0) {...;break;}
//  }
//

// #define RTU_Vesper // см. device.h

#define Vesper_State     DriveState
#define Vesper_SV        DriveSV
#define Vesper_SA        DriveSA
#define Vesper_SD        DriveSD
#define Vesper_Err_cod   DriveError
#define Vesper_current   DriveCurrent

//------------------------
void f_Vesper_Run()
{
 Vesper_Ctrl[0]=START_FWD_VESPER;
}
//------------------------
void f_Vesper_Stop()
{
 Vesper_Ctrl[0]=STOP_FWD_VESPER;
}
//------------------------
int f_Vesper_St_chk()
{
 if(FL_err != 0) return -1;
 if((Vesper_State[0] & MSK_RUN_VESPER ) == STP_VESPER ) return 1;
 else return 0;
}
//------------------------
int f_Vesper_Rn_chk()
{
 return 1;  //qqq
 /*
 if(FL_err != 0) return -1;
 if((Vesper_State[0] & MSK_RUN_VESPER ) == RUN_VESPER ) return 1;
 else return 0;
 */
}
//------------------------

#define max_Vsp 2
//---------------
struct s_icp_dev Vesper[max_Vsp]=
{
// device 1
0,                // status
2,                // port
2,                // addr
"Vesper/1",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Vesper_Rqst[0],   // *request
//--------------------------------
// device 2
//--------------------------------
0,                // status
1,                // port
3,                // addr
"Vesper/2",        // name[8]
06,               // baudrate_cod
0x00,             // type
0x00,             // format
0x00,             // channels
"B1.7",           // firmware
1,                // protocol
1,                // CRC_flag
200,              // pool_time
0,                // time_stamp_pool;
0,                // time_stamp_snd
0,                // time_stamp_rcv
0,                // n_transaction
0,                // n_success
0,                // n_timeout_error
0,                // n_CRC_error
&Vesper_Rqst[1],   // *request
};
/*----------------------------*/
struct COM_rqst Vesper_Rqst[max_Vsp]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Vesper_rd,            //  answ_com
f_Vesper_rd,            //  answ_flt
"Vesper/1",              //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Vesper[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsVsp_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Vesper_rd,            //  answ_com
f_Vesper_rd,            //  answ_flt
"Vesper/2",              //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Vesper[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsVsp_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
/*----------------------------*/
int Vesper_Ctrl[max_Vsp]={0,0};   // упраляющее слово
int Vesper_Ctrl_c[max_Vsp]={0,0}; // текущее состояние упраляющего слово

/*
int Vesper_State[max_Vsp]={0,0};  // слово состояния
int Vesper_SV[max_Vsp]={5000,5000};  // значение частоты вращения
int Vesper_SA[max_Vsp]={20,20};     // текущее состояние  ускорения 2 сек
int Vesper_SD[max_Vsp]={20,20};  // текущее состояние  замедления 2 сек на 100Гц
int Vesper_current[max_Vsp]={0,0};
int Vesper_Err_cod[max_Vsp]={0,0};   // ошибка последнего отключения
*/


int Vesper_SVc[max_Vsp]={-1,-1};     // текущее состояние  частоты вращения

int Vesper_SAc[max_Vsp]={-1,-1};    // текущее состояние  ускорения

int Vesper_SDc[max_Vsp]={-1,-1}; // текущее состояние замедления

int Vesper_fn[max_Vsp]={0,0};    // регистр функций драйвера

//  Если записать значение функции в регистр функции , например:
//  Vesper_fn[0]= RES_Vsp_ERR;  драйвер выполнит заданную функцию - в данном случае
//  пошлет в устройство Vesper команду сброса привода.
//  После постановки запроса с установленной функцией в очередь на вывод в COM,
//  регистр функции очищается драйвером.

int Vesper_sw[max_Vsp]={0,0};

//unsigned int Vesper_out_err[max_Vsp]={0,0};  // счетчик ошибок вывода
unsigned int Vesper_inp_err[max_Vsp]={0,0};  // счетчик ошибок ввода

int Vesper_seq[16]={0,3,4,0,3,4,0,3,4,0,3,4,0,3,4,0};
            // последовательность функций опроса
               // 0 чтение слова состояния
               // 1 чтение слова управления
               // 2 чтение текущего SV
               // 3 чтение регистра ошибки
               // 4 чтение тока

int Vesper_fl[max_Vsp]={0,0};

// 09.00 - адрес устройства
//
// 0x0000 - адрес регистра ускорения  2.11
// 0x0000 - адрес регистра замедления 2.21
// 0x2001 - адрес регистра предустановленной скорости вращения
// 0x0906 - адрес регистра управляющего слова
// 0x2002 - 0x2 - бит сброса устройства
// 0x0201 - адрес регистра включения управляющего слова 02.01
// 0x2100 - адрес регистра ошибки
// 0x2101 - адрес регистра слова состояния

unsigned char ComResVsp[]={1,10,0x00,0x01,0x00,0x01,02,0x00,0x08};   // сброс ошибок
//                        |  |   |    |    |   |
//                        |  |   |    |    | младший байт данных
//                        |  |   |    | старший байт данных
//                        |  |   | младший байт адреса
//                        |  |  старший байт адреса
//                        | код функции (однократная запись)
//                        адрес
unsigned char Vsp_Set_mode[9]={1,10,0x01,0x02,0x00,0x01,02,0x00,0x06};  // 6 управление - посл.линия связи; опорная величина - посл.линия связи;
                                                             // 7 управление - посл.линия связи; опорная величина - цифр. панель управления

//unsigned char Vsp_Set_ctr[]={1,6,0x02,0x00,0x00,0x04};   // включение уставки скорости через RS485

unsigned char Vsp_Rd_sv[]   ={1,3,0x20,0x01,0x00,0x01};  // зад.частота
unsigned char Vsp_Rd_err[]  ={1,3,0x21,0x00,0x00,0x01};  // код ошибки

unsigned char Vsp_Rd_ctrl[] ={1,3,0x20,0x00,0x00,0x01};   // управляющее слово
unsigned char Vsp_Rd_curr[] ={1,3,0x21,0x04,0x00,0x01};   // управляющее слово (ток)
unsigned char Vsp_Rd_state[]={1,3,0x21,0x01,0x00,0x01};   // состояние

void f_Vesper(int ii)
{
  // ii - номер устройства
  // Функция шлет запрос на получение данных слова состояния из
  // устройства Vesper( '01 03 0001 0001'),
  // а также текущего состояния слова управления и предустановки скорости.
  //   Период посылки Vesper[ii].pool_time мс.
  //
  //   Если состояние уставки частоты вращения (Vesper_SVc[ii])
  //  отличается от требуемого состояния Vesper_SV[ii] , будет сформирована
  //  команда записи в регистр предустановки частоты вращения.
  //   Если состояние слова управления (Vesper_Ctrl_c[ii])
  //  отличается от требуемого состояния Vesper_Ctrl[ii] , будет сформирована
  //  команда записи в регистр управляющего слова.
  //    Если в  регистр функции != 0 - функция выполняется
  //  в первую очередь.
  //

int i,fn,sw,i2,itmp;
long int i1;
unsigned char buf_tmp[20];

 if(Vesper[ii].status == 0) return;

 if( Vesper_fl[ii] >= 0 )
            f_init_Vesper(ii);
// else
//     f_Vsp_wd(ii);

 if(Vesper_fn[ii] != 0)
      fn=Vesper_fn[ii];
 else if(Vesper_Ctrl[ii] != Vesper_Ctrl_c[ii] )
      fn=Vsp_CTRL_OUT;
 else if(Vesper_SV[ii] != Vesper_SVc[ii])
      fn=Vsp_SV_OUT;
// else if(Vesper_SA[ii] != Vesper_SAc[ii])
//      fn=Vsp_SA_OUT;
// else if(Vesper_SD[ii] != Vesper_SDc[ii])
//      fn=Vsp_SD_OUT;
 else if( f_timer(Vesper[ii].time_stamp_pool,Vesper[ii].pool_time ))
      fn=Vsp_RD;
 else return;

  if((Vesper_Rqst[ii].status == Req_Flt) ||
     (Vesper_Rqst[ii].status == Req_OK)) goto m1;
//  else if( f_timer(IVesper[ii].time_stamp_pool,(IVesper[ii].pool_time+IVesper[ii].pool_time) ))
//           goto m1;
  return;

m1:
         if(f_queue_chk(Vesper[ii].port)== 0) return;// очередь переполнена
     /*    if(fn  == Vsp_SET_CTR)
          {  // включение работы через управляющее слово
            i1=6;
            _fmemcpy(buf_tmp,Vsp_Set_ctr,i1);
            buf_tmp[0]=Vesper[ii].addr;
            Vesper_fn[ii]=0;
          }*/
         else if(fn  == Vsp_SET_MODE)
          {  // включение работы через управляющее слово
            if(flag_ext_t ==0) Vsp_Set_mode[5]=3;
            else Vsp_Set_mode[5]=1;
            i1=6;
            _fmemcpy(buf_tmp,Vsp_Set_mode,i1);
            buf_tmp[0]=Vesper[ii].addr;
            Vesper_fn[ii]=0;
          }
          else if(fn  == Vsp_RES_ERR)
          {
            i1=6;
            _fmemcpy(buf_tmp,ComResVsp,i1);
            buf_tmp[0]=Vesper[ii].addr;
            Vesper_fn[ii]=0;
            Vesper_Ctrl[ii]=0;
          }
          else if(fn == Vsp_SV_OUT) // вывод уставки скорости вращения
          {
            i1=6;
            buf_tmp[0]=Vesper[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=0x20;   // 0x2001 - адрес регистра предустановленной скорости вращения 0x2001
            buf_tmp[3]=0x01; //
            buf_tmp[4]=(Vesper_SV[ii]>>8)&0xff;
            buf_tmp[5]=Vesper_SV[ii] & 0xff;
            Vesper_SVc[ii]=Vesper_SV[ii];
          }

          else if(fn == Vsp_SA_OUT) // вывод уставки ускорения
          {
            i1=6;
            buf_tmp[0]=Vesper[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=01;   // 0x0109- адрес регистра ускорения 1  1.09
            buf_tmp[3]=0x09; //
            buf_tmp[4]=(Vesper_SA[ii]>>8)&0xff;
            buf_tmp[5]=Vesper_SA[ii] & 0xff;
            Vesper_SAc[ii]=Vesper_SA[ii];
          }
          else if(fn == Vsp_SD_OUT) // вывод уставки замедления
          {
            i1=6;
            buf_tmp[0]=Vesper[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=01;   // 0x0110 - адрес регистра предустановленной скорости вращения 1.21
   //       buf_tmp[3]=0x10; //
            buf_tmp[3]=0xA; //
            buf_tmp[4]=(Vesper_SD[ii]>>8)&0xff;
            buf_tmp[5]=Vesper_SD[ii] & 0xff;
            Vesper_SDc[ii]=Vesper_SD[ii];
          }

          else if(fn == Vsp_CTRL_OUT) // вывод управляющего слова
          {
            i1=6;

            itmp=Vesper_Ctrl[ii];

            buf_tmp[0]=Vesper[ii].addr;
            buf_tmp[1]=6;
            buf_tmp[2]=0x20; // 0x2000 - адрес регистра управляющего слова (09.06)
            buf_tmp[3]=0x00; //
            buf_tmp[4]=(itmp>>8)&0xff;
            buf_tmp[5]= itmp & 0xff;
            Vesper_Ctrl_c[ii]=Vesper_Ctrl[ii];
          }
          else if(fn == Vsp_RD)  // чтение состояния
          {// формирует команды чтения
             Vesper_sw[ii]=(Vesper_sw[ii]+1) & 0xf;
             sw=Vesper_seq[Vesper_sw[ii]];
            i1=6;
             switch (sw)
             {
              case 0:  // чтение слова состояния
            _fmemcpy(buf_tmp,Vsp_Rd_state,i1);
                break;
              case 1:  // чтение слова управления
            _fmemcpy(buf_tmp,Vsp_Rd_ctrl,i1);
                break;
              case 2:  // чтение текущего SV
            _fmemcpy(buf_tmp,Vsp_Rd_sv,i1);
                break;
              case 3:  // чтение регистра ошибки
            _fmemcpy(buf_tmp,Vsp_Rd_err,i1);
                break;
              case 4:  // чтение тока
            _fmemcpy(buf_tmp,Vsp_Rd_curr,i1);
                break;
              default:
                return;
             }
             buf_tmp[0]=Vesper[ii].addr;
             fn+=sw;
          }
          else return;

           Vesper_Rqst[ii].function = fn;
           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Vesper_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Vesper_Rqst[ii].cmd_lgth=i2;
           Vesper_Rqst[ii].time_stamp=TimeStamp;
           Vesper[ii].time_stamp_pool=TimeStamp;
           Vesper[ii].n_transaction++;
           f_queue_put(Vesper[ii].port, &Vesper_Rqst[ii]) ;
}
/*----------------------------*/
#define  sw_Vsp  Vesper_fl
unsigned long int Vesperit[max_Vsp];
void f_init_Vesper(int ii)
 {
   switch (sw_Vsp[ii])
   {
    case 0:
       if(Vesper_fn[ii] != 0) break;
     // сброс ошибок
       Vesper_fn[ii]=Vsp_RES_ERR;
       sw_Vsp[ii]=1;
       break;

    case 1:
      /* if(Vesper_fn[ii] != 0) break;
      // включение уставки скорости через RS485
       Vesper_fn[ii]=Vsp_SET_CTR;*/
       sw_Vsp[ii]=2;
       break;

    case 2:
       if(Vesper_fn[ii] != 0) break;
       Vesper_fn[ii]=Vsp_SET_MODE;
     // задание источника старт/стоп
       sw_Vsp[ii]=10;
       break;

    case 10:
       if(Vesper_fn[ii] != 0) break;
       Vesperit[ii]=TimeStamp;
       sw_Vsp[ii]=11;
       break;

    case 11:
 if( f_timer(Vesperit[ii],100L) == 0) break;

       Vesper[ii].n_transaction    = 0;
       Vesper[ii].n_success        = 0;
       Vesper[ii].n_timeout_error  = 0;
       Vesper[ii].n_CRC_error      = 0;
       Vesper_Err_cod[ii] = 0;

       DriveStatus=&Vesper[0].status;

       f_Drive_St_chk= f_Vesper_St_chk;
       f_Drive_Rn_chk= f_Vesper_Rn_chk;
       f_Drive_Stop  = f_Vesper_Stop;
       f_Drive_Run   = f_Vesper_Run;

       sw_Vsp[ii]=-1;
       break;

    case -1:
       break;
    default:break;
   }
 }
//-----------------------------------------
int itmpVesper;
void f_Vesper_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn,fn;
 int count;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства Vesper (0,1..)
  fn=request->function;

  count=n_bcom[ii];  // длина команды
  if(count < 3)
   {
      goto m_tout_err;
   }
  if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
  if(( fn & 0xf0)==RD_STATUS) // ответ на команду чтения регистра
  {
      if(cb_COM[ii][1] != 3)
      {
        goto m_crc_err;
      }
      fn &= 0xf;
      switch(  fn )
      {
      case 0:  // чтение слова состояния
    //в ответ на 01 03 04 0f 00 01 приходит:
    //  01 03 02 00 05 78 47

       Vesper_State[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8) &0xff00) ;

         break;
      case 1:     // чтение слова управления
         Vesper_Ctrl_c[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;

      case 2:    // чтение текущего SV
         Vesper_SVc[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         break;
      case 3:    // чтение регистра ошибки
         Vesper_Err_cod[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;

         if( Vesper_fl[nn] < 0 )
           if(Vesper_Err_cod[nn] != 0)
           {
             if( f_get_icp_error(&Vesper[nn]) & Drive_ERR) break;
             f_icp_error(&Vesper[nn],Drive_ERR | ((Vesper_Err_cod[nn] << 8)& 0xff00) );
           }
         break;
      case 4:    // чтение тока
//       Vesper_current[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         DriveCurrent[nn]=(cb_COM[ii][4] & 0xff) | ( ((unsigned int)cb_COM[ii][3] << 8)   &0xff00)  ;
         f_reg_I();
         break;
      default: goto m_crc_err;
      }
  }
  else
  { // ответ на команду записи
    if(cb_COM[ii][1] != 6)
    {
      goto m_crc_err;
    }
  }
  Vesper[nn].n_success++;
  Vesper_inp_err[nn]=0;
  f_queue_free(ii,&Vesper_Rqst[nn]);
  return;

m_crc_err:
 Vesper[nn].time_stamp_rcv=0;
 Vesper[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Vesper[nn].time_stamp_rcv=0;
 Vesper[nn].n_timeout_error++;

m_err:
 Vesper_inp_err[nn]++;

 switch (fn)
 {
  case Vsp_SV_OUT  :
       Vesper_SVc[nn]=-1;
       break;
  case Vsp_SA_OUT  :
       Vesper_SAc[nn]=-1;
       break;
  case Vsp_SD_OUT  :
       Vesper_SDc[nn]=-1;
       break;
  case Vsp_CTRL_OUT  :
       Vesper_Ctrl_c[nn]=-1;
       break;
  case Vsp_RES_ERR  :
  case Vsp_SET_MODE :
  //case Vsp_SET_CTR :
       Vesper_fn[nn]=fn;
       break;
 }
 if(Vesper_inp_err[nn]>=Vesper_max_inp_err)
 {
   Vesper_inp_err[nn]=Vesper_max_inp_err;
   f_icp_error(&Vesper[nn],RD_ERR );
 }
 f_queue_free(ii,&Vesper_Rqst[nn]);
}
/* ---------------------------------------- */
/*

struct COM_rqst Vesper_WD_Rqst[max_Vsp]={
//----------------------
// device 1
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Vesper_WD_rd,         //  answ_com
f_Vesper_WD_rd,         //  answ_flt
"Vsp_WD/1",             //  name
0,                     //  n_dev номер устройства в группе однотипных устройств
&Vesper[0],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsVsp_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
//--------------------------
// device 2
//--------------------------
0,                     //  состояние запроса
ffgets_com_rtu,        //  gets_com
f_Vesper_WD_rd,         //  answ_com
f_Vesper_WD_rd,         //  answ_flt
"Vsp_WD/2",             //  name
1,                     //  n_dev номер устройства в группе однотипных устройств
&Vesper[1],             //  *ICP_dd
0,                     //  текущая функция
ToutAnsVsp_rtu,         //  timeout
0,                     //  time_stamp
0,                     //  com_lgth
1,                     //  CRC_flag
"",                    //  com[]
};
//----------------------------

void f_Vesper_WD_rd(int ii)
{
// ii - номер порта
// заносит принятые данные в переменные
//
 struct COM_rqst *request;
 int i,nn;
 int count;

  request=COM_que[ii].request[COM_que[ii].empt_ptr];
  nn=request->n_dev; // номер устройства Vesper (0,1..)

  count=n_bcom[ii];  // длина команды
  if(count < 3)
   {
      goto m_tout_err;
   }
  if(f_check_crc_RTU(cb_COM[ii],count)==0)
   {
      goto m_crc_err;
   }
  // ответ на команду записи
  if(cb_COM[ii][1] != 6)
  {
    goto m_crc_err;
  }

  Vesper[nn].n_success++;
  Vesper_inp_err[nn]=0;
  f_queue_free(ii,&Vesper_WD_Rqst[nn]);
  return;

m_crc_err:
 Vesper[nn].time_stamp_rcv=0;
 Vesper[nn].n_CRC_error++;
 goto m_err;

m_tout_err:
 Vesper[nn].time_stamp_rcv=0;
 Vesper[nn].n_timeout_error++;

m_err:
 Vesper_inp_err[nn]++;
 if(Vesper_inp_err[nn]>=Vesper_max_inp_err)
 {
   Vesper_inp_err[nn]=Vesper_max_inp_err;
   f_icp_error(&Vesper[nn],RD_ERR );
 }
 f_queue_free(ii,&Vesper_WD_Rqst[nn]);
}
// ----------------------------------------
long int Vsp_wd_time[max_Vsp]={0,0};
long int Vsp_pool_period[max_Vsp]={200,200};
int Vsp_wd[max_Vsp]={0,0};

void  f_Vsp_wd( int ii)
{
  // Watch Dog для Commander Vsp
  // ii - номер устройства

unsigned char buf_tmp[20];
int i,i2,itmp;
long int i1;

 if(FL_err) return;
 if( f_timer(Vsp_wd_time[ii],Vsp_pool_period[ii] ))
 {
  if((Vesper_WD_Rqst[ii].status == Req_Flt) ||
     (Vesper_WD_Rqst[ii].status == Req_OK)) ;
  else return;

           Vsp_wd[ii] ^= Vsp_WDT;

           itmp=Vesper_Ctrl[ii] | Vsp_wd[ii];
           i1=6;
           buf_tmp[0]=Vesper[ii].addr;
           buf_tmp[1]=6;
           buf_tmp[2]=0x02; // 0x0281 - адрес регистра управляющего слова 6.42
           buf_tmp[3]=0x81; //
           buf_tmp[4]=(itmp>>8)&0xff;
           buf_tmp[5]= itmp & 0xff;
           Vesper_Ctrl_c[ii]=Vesper_Ctrl[ii];

           i2=i1;
           f_prepareRTU(buf_tmp,&i2);
           _fmemcpy(Vesper_WD_Rqst[ii].Cmd,buf_tmp,(long)i2);
           Vesper_WD_Rqst[ii].cmd_lgth=i2;
           Vesper_WD_Rqst[ii].time_stamp=TimeStamp;
           Vsp_wd_time[ii]=TimeStamp;
           Vesper[ii].n_transaction++;
           f_queue_put(Vesper[ii].port, &Vesper_WD_Rqst[ii]) ;
 }
}
*/
// ----------------------------------------
