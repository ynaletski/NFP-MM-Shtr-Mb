
#define Test 0  // 1 - ��⮢� ०�� ��� MVD

#define AIR_SW  0  // 1 - OUT4 ���뢥� ������ ����㯠 ������ � ᫨���� ઢ ��୥�������

#define CUT_SW  1  // 1 - OUT 4 ���뢥� ���筮� ������ �� ���᪥


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\timeb.h>

#include "device.h"

#include "PROC.h"


#if defined(MMI_ICP)
#include "MMI.h"
#endif

#if defined(MMI_NEW)
#include "MMI_NEW.h"
#endif


//#include "ekd.h"

#include "queue.h"
#include "icp.h"
#include "host.h"
#include "7017c.h"
#include "i7060.h"
#include "i7041.h"
#include "ofp.h"

///qqq///////////////////////////
#if defined(Press_mb)
#include "pr_mb.h"
#endif
/////////////////////////////////

///www//////////////////////////
#if defined(Press_415m)
#include "pr415m.h"
#endif
////////////////////////////////

///rrr/////////////////////////
#if defined(Temp_tpu)
#include "TPU0304.h"
#endif
///////////////////////////////

#include "eee.h"
#include "rtu.h"
#include "sk.h"

#if defined(RTU_Delta)
#include "delta.h"
#endif

#if defined(RTU_Delta_C)
#include "deltaC.h"
#endif

#include "MVD.h"
#include "tsd.h"
#include "arch.h"
#include "avt_ctrl.h"
#include "reg.h"
#include "cor_adc.h"
#include "slave.h"
#include "valve.h"
#include "ref_t.h"
#include "md5.h"

//06.02.2020 YN  -----\\//-----
#if defined(RTU_Vesper)
#include "Vesper.h"
#endif
//06.02.2020 YN  -----//\\-----