//06.02.2020 YN  -----\\//-----
  // �㭪樨 �ࠩ���:

#define START_FWD_VESPER  0x12
#define STOP_FWD_VESPER   0x11

#define START_REV_VESPER  0x22
#define STOP_REV_VESPER   0x21

#define MSK_RUN_VESPER   0x03
#define RUN_VESPER       0x03
#define ACS_VESPER       0x01
#define STP_VESPER       0x00

  // ��⠭���� �ࠢ����� �ਢ���� �१ �ࠢ���饥 ᫮��
//#define Vsp_SET_CTR  1

  // ��� �ਢ��� (�訡�� )
#define Vsp_RES_ERR  2

 // �⥭�� ��᫥���� ��稭� �⪫�祭�� (�訡��)
#define Vsp_RD_ERR   4


#define Vsp_SV_OUT   5
#define Vsp_SA_OUT   6
#define Vsp_SD_OUT   7
#define Vsp_CTRL_OUT 8
#define Vsp_SET_MODE 9

#define Vsp_RD   16

// ��� "�ਢ�� ��ࠢ��" � ᫮�� ���ﭨ�
#define Vsp_OK 0x01

// ��� "��஦���� ⠩���" � ᫮�� �ࠢ�����
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

extern int Vesper_State[];  // ᫮�� ���ﭨ�
extern int Vesper_Ctrl[];   // �ࠫ��饥 ᫮��
extern int Vesper_Ctrl_c[]; // ⥪�饥 ���ﭨ� �ࠫ��饣� ᫮��
extern int Vesper_SV[];     // ���祭�� ����� ��饭��
extern int Vesper_SVc[];    // ⥪�饥 ���ﭨ�  ����� ��饭��
extern int Vesper_Err_cod[];// �訡�� ��᫥����� �⪫�祭��

extern int Vesper_SA[];  // ⥪�饥 ���ﭨ�  �᪮७�� 2 ᥪ �� 100��
extern int Vesper_SAc[];  // ⥪�饥 ���ﭨ�  �᪮७��

extern int Vesper_SD[];  // ⥪�饥 ���ﭨ�  ���������� 2 ᥪ �� 100��
extern int Vesper_SDc[]; // ⥪�饥 ���ﭨ� ����������
extern int Vesper_fn[];     // ॣ���� �㭪権 �ࠩ���

extern int Vesper_fl[];
extern int Vsp_wd[];

extern long int Vsp_pool_period[];
extern struct COM_rqst Vesper_WD_Rqst[];

extern int Vesper_current[];


