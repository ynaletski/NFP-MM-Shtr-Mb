//06.02.2020 YN  -----\\//-----
  // функции драйвера:

#define START_FWD_VESPER  0x12
#define STOP_FWD_VESPER   0x11

#define START_REV_VESPER  0x22
#define STOP_REV_VESPER   0x21

#define MSK_RUN_VESPER   0x03
#define RUN_VESPER       0x03
#define ACS_VESPER       0x01
#define STP_VESPER       0x00

  // установка управления приводом через управляющее слово
//#define Vsp_SET_CTR  1

  // сброс привода (ошибок )
#define Vsp_RES_ERR  2

 // чтение последней причины отключения (ошибки)
#define Vsp_RD_ERR   4


#define Vsp_SV_OUT   5
#define Vsp_SA_OUT   6
#define Vsp_SD_OUT   7
#define Vsp_CTRL_OUT 8
#define Vsp_SET_MODE 9

#define Vsp_RD   16

// бит "Привод исправен" в слове состояния
#define Vsp_OK 0x01

// бит "Сторожевой таймер" в слове управления
#define Vsp_WDT 0x4000
#define MVsp_Vsp_WDT 0xbfff

#define ToutAnsVsp_rtu 100

#define Vesper_max_out_err 10
#define Vesper_max_inp_err 10

void f_Vesper(int ii);
void f_Vesper_rd(int ii);
void f_init_Vesper(int ii);
void  f_Vsp_wd( int ii);
void f_Vesper_WD_rd(int ii);

extern struct s_icp_dev Vesper[];
extern struct COM_rqst Vesper_Rqst[];

extern int Vesper_State[];  // слово состояния
extern int Vesper_Ctrl[];   // упраляющее слово
extern int Vesper_Ctrl_c[]; // текущее состояние упраляющего слова
extern int Vesper_SV[];     // значение частоты вращения
extern int Vesper_SVc[];    // текущее состояние  частоты вращения
extern int Vesper_Err_cod[];// ошибка последнего отключения

extern int Vesper_SA[];  // текущее состояние  ускорения 2 сек на 100Гц
extern int Vesper_SAc[];  // текущее состояние  ускорения

extern int Vesper_SD[];  // текущее состояние  замедления 2 сек на 100Гц
extern int Vesper_SDc[]; // текущее состояние замедления
extern int Vesper_fn[];     // регистр функций драйвера

extern int Vesper_fl[];
extern int Vsp_wd[];

extern long int Vsp_pool_period[];
extern struct COM_rqst Vesper_WD_Rqst[];

extern int Vesper_current[];


