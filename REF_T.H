

float f_get_VCF(float Temp, float D_ref);
float f_get_T(float D1,float D_ref, float T_ref);
float f_get_ref_Dns(float D_obs, float Temp,float Press, float T_Ref);
extern int type_liq;  // 2 Fuel Oils   - диэтопливо

void f_init_DNSA();
void f_cnt_DnsA();
extern double Dens_cmn;
extern float Dens_cmn_f;

//23.01.2020 YN  -----\\//-----
extern float Temp_sum_YN;
extern double mas_ch_cur_old;
extern double mas_ch_cur;

extern float Press_sum_YN;
extern float Dens_sum_YN;
//23.01.2020 YN  -----//\\-----