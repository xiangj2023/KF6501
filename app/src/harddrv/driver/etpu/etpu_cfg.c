/************************************************************************
 *    This file was generated by eTPU Graphical Configuration Tool        
 *                                                                        
 *    DO NOT EDIT !!! DO NOT EDIT !!! DO NOT EDIT !!! DO NOT EDIT !!!     
 *                                                                        
 *  DESCRIPTION:
 *  This file contains my_system_etpu_init and my_system_etpu_start
 *  functions and related items. The function initialize the eTPU 
 *  global and channel settings, based on the user settings 
 *  in the eTPU GCT, and runs the eTPU.               
 *  A header file of the same name .h is also generated.                   
 *
 *  Creation Date:  5.4.2005 9:04:22 
 *
 **************************************************************************/

/* general routines */
#include "etpu_util.h"             /* Utility routines for working eTPU */
#include "mcf523x_vars.h"          /* Processor dependent variables */

/* eTPU function set used */
#include "etpu_set1.h"       

//#include "etpu_app_bldcmhsl1.h" /* BLDC Motor with Hall Sensors and Speed Closed Loop */
#include "etpu_pwm.h" /* GPIO */

#include "etpu_ic.h"
#include "etpu_gpio.h"
//#include "etpu_ppa.h"

/* our private header file (generated by eTPU GCT) */
#include "etpu_cfg.h"

#include "channel.h"

//#define SAMP_POINT		32	//每周波采样点数

//ETPU初始化参数

struct etpu_config_t my_etpu_config = {
	FS_ETPU_MISC_DISABLE,	//MCR register

	FS_ETPU_MISC,			//MISC value from eTPU compiler link file

	//Configure eTPU engine A
	FS_ETPU_FILTER_CLOCK_DIV8 +
	FS_ETPU_CHAN_FILTER_3SAMPLE +
	FS_ETPU_ENTRY_TABLE,

	//Configure eTPU engine A timebases
	FS_ETPU_TCR2CTL_DIV8 +
	( 7 << 16) +     //TCR2 prescaler of 8 (7+1)
	FS_ETPU_TCR1CTL_DIV2 +
	3,               //TCR1 prescaler of 4 (3+1)

	0,

	//Configure eTPU engine b
	FS_ETPU_FILTER_CLOCK_DIV4 +
    FS_ETPU_CHAN_FILTER_3SAMPLE +
	FS_ETPU_ENTRY_TABLE,

	//Configure eTPU engine B timebases
	FS_ETPU_TCR2CTL_DIV8 +
	( 7 << 16) +     //TCR2 prescaler of 8 (7+1)
	FS_ETPU_TCR1CTL_DIV2 +
	3,

	0
	};

/**************************************************************************
 * 
 * Global Variables
 *
 **************************************************************************/

uint32 *fs_free_param;

/**************************************************************************
 *
 * CPU and clock configuration:
 *
 *   CPU = MCF523x
 *   SYSCLK = 150 MHz
 *   Input_TCRCLK_A = OFF
 *   Input_TCRCLK_B = OFF
 *   
 **************************************************************************/
/*
	uint32 etpu_a_tcr1_freq = 37500000;
	uint32 etpu_a_tcr2_freq = 9375000;
	uint32 etpu_b_tcr1_freq = 0;
	uint32 etpu_b_tcr2_freq = 9375000;
*/

uint32 etpu_a_tcr1_freq = 37500000/4;
uint32 etpu_a_tcr2_freq = 781250;
uint32 etpu_b_tcr1_freq = 37500000;
uint32 etpu_b_tcr2_freq = 9375000;


/**************************************************************************
 *
 * Interrupt Enable, DMA Enable and Output Disable options
 * filled accodring to the user settings
 *
 **************************************************************************/

#define ETPU_CIE_A    0x000000ff	//中断允许
#define ETPU_DTRE_A   0x00000000
#define ETPU_ODIS_A   0x00000000
#define ETPU_OPOL_A   0x00000000

/**************************************************************************
 * 
 *  FUNCTION : my_system_etpu_init
 * 
 *  PURPOSE  : Initializes the eTPU settings and channel settings, based
 *             on the user settings in the eTPU GCT:
 *
 *             1. Initialize global setting using fs_etpu_init function
 *                and the my_etpu_config structure.
 *             2. Initialize channel setting using channel function APIs 
 *                or standard eTPU routines.
 *
 *  RETURNS  : If the initialization of all channel functions passed OK, 
 *             the return value is 0. If the initialization of a channel 
 *             failed, the returned value is a number of the channel that
 *             failed to initialized increased by 1.
 *
 **************************************************************************/

int32 my_system_etpu_init()
{
  	int32 err_code;

  /*
   *  Initialization of eTPU global settings
   */

  	fs_etpu_init( my_etpu_config, (uint32 *)etpu_code, sizeof(etpu_code),
                (uint32 *)etpu_globals, sizeof(etpu_globals));

  	//初始化频率测量通道1，工作方式为GPIO的输入方式
	err_code = fs_etpu_gpio_init(ETPU_FREQ1, FS_ETPU_PRIORITY_HIGH);
	if(err_code != 0)
	  	return (ETPU_FREQ1+1);
	//捕捉任意边沿，测量正负脉冲的长度
	fs_etpu_gpio_cfg_input_trans(ETPU_FREQ1, FS_ETPU_GPIO_INPUT_EITHER);
	//fs_etpu_gpio_cfg_input_trans(ETPU_FREQ1, FS_ETPU_GPIO_INPUT_FALLING);

  	//初始化频率测量通道2，工作方式为GPIO的输入方式
	err_code = fs_etpu_gpio_init(ETPU_FREQ2, FS_ETPU_PRIORITY_HIGH);
	if(err_code != 0)
	  	return (ETPU_FREQ2+1);
	//捕捉任意边沿，测量正负脉冲的长度
	fs_etpu_gpio_cfg_input_trans(ETPU_FREQ2, FS_ETPU_GPIO_INPUT_EITHER);

/*	err_code = fs_etpu_ic_init(ETPU_FREQ1, FS_ETPU_PRIORITY_HIGH, ETPU_IC_MODE_CONT,
				FS_ETPU_TCR1, ETPU_IC_ANY_EDGE, 1);
	if(err_code != 0) 
		return (ETPU_FREQ1+1);

	err_code = fs_etpu_ic_init(ETPU_FREQ2, FS_ETPU_PRIORITY_HIGH, ETPU_IC_MODE_CONT,
				FS_ETPU_TCR1, ETPU_IC_ANY_EDGE, 1);
	if(err_code != 0) 
		return (ETPU_FREQ2+1);
*/
	//初始化解析B格式信号通道，工作方式为GPIO的输入方式
	err_code = fs_etpu_gpio_init(ETPU_IRIG1, FS_ETPU_PRIORITY_MIDDLE);
	if(err_code != 0)
	  	return (ETPU_IRIG1+1);
	//捕捉任意边沿，测量正负脉冲的长度
	fs_etpu_gpio_cfg_input_trans(ETPU_IRIG1, FS_ETPU_GPIO_INPUT_EITHER);

	
	//初始化系统电源监视信号通道，工作方式为GPIO的输入方式
	err_code = fs_etpu_gpio_init(ETPU_PFO, FS_ETPU_PRIORITY_HIGH);
	if(err_code != 0)
	  	return (ETPU_PFO+1);
	//捕捉下降沿，低电平说明电压偏低
	fs_etpu_gpio_cfg_input_trans(ETPU_PFO, FS_ETPU_GPIO_INPUT_EITHER);
	
	
	//初始化主板测量通道，工作方式为GPIO的输入方式
	err_code = fs_etpu_gpio_init(ETPU_TEMP, FS_ETPU_PRIORITY_LOW);
	if(err_code != 0)
		return (ETPU_TEMP+1);
	//捕捉任意沿，测量周期
	fs_etpu_gpio_cfg_input_trans(ETPU_TEMP, FS_ETPU_GPIO_INPUT_EITHER);
	
	
	//初始化AD启动采样通道，工作方式为PWM方式，不需要中断，需要根据外部交流频率调整PWM周期POINT_PER_PERIOD*50
	err_code = fs_etpu_pwm_init(ETPU_AD_START, FS_ETPU_PRIORITY_MIDDLE, POINT_PER_PERIOD*50, 500,\
					FS_ETPU_PWM_ACTIVEHIGH, FS_ETPU_TCR1, etpu_a_tcr1_freq);
	
	if (err_code != 0 ) return (ETPU_AD_START+1);

  	//初始化AD启动采样通道，工作方式为PWM方式，不需要中断，需要根据外部交流频率调整PWM周期
  	err_code = fs_etpu_pwm_init(ETPU_AD1_START, FS_ETPU_PRIORITY_MIDDLE, POINT_PER_PERIOD*50, 500,\
				  FS_ETPU_PWM_ACTIVEHIGH, FS_ETPU_TCR1, etpu_a_tcr1_freq);
  
  	if (err_code != 0 ) return (ETPU_AD1_START+1);
	
  /*
  //初始化输入捕捉功能
  err_code = fs_etpu_ic_init(ETPU_IC, FS_ETPU_PRIORITY_LOW, ETPU_IC_MODE_CONT,
			   FS_ETPU_TCR1, ETPU_IC_FALLING_EDGE, 5);
  if(err_code != 0)	
  	return (ETPU_IC+1);
  err_code = fs_etpu_ppa_init(ETPU_IC, FS_ETPU_PRIORITY_MIDDLE, FS_ETPU_TCR1, FS_ETPU_PPA_LOW_PULSE,\
  					1, 0x100000);
  if(err_code != 0)	
  	return (ETPU_IC+1);
*/
  /* initialize eTPU channels */
  /* initialize channel PWM0 to:						   */
  /* 10,000hz, 50%, active high middle priority using TCR1 */
/*
  err_code = fs_etpu_pwm_init(ETPU_PWM, FS_ETPU_PRIORITY_MIDDLE, 50, 5000,\
				  FS_ETPU_PWM_ACTIVEHIGH, FS_ETPU_TCR1, etpu_a_tcr1_freq);
  
  if (err_code != 0 ) return (ETPU_PWM+1);
*/
  
  	return(0);
}

/**************************************************************************
 *
 *  FUNCTION : my_system_etpu_start
 *
 *  PURPOSE  : Initializes the eTPU settings and channel settings, based
 *             on the user settings in the eTPU GCT:
 *
 *             1. Initialize
 *                - Interrupt Enable
 *                - DMA Enable
 *                - Output Disable
 *                options of each channel.
 *             2. Run eTPU using Global Timebase Enable (GTBE) bit.
 *
 **************************************************************************/

void my_system_etpu_start()
{
  /*
   *  Initialization of Interrupt Enable, DMA Enable
   *  and Output Disable channel options
   */

  uint8 i;
  uint32 odis_a;
  uint32 opol_a;

  eTPU->CIER_A.R = ETPU_CIE_A;

  eTPU->CDTRER_A.R = ETPU_DTRE_A;

  odis_a = ETPU_ODIS_A;
  opol_a = ETPU_OPOL_A;
  for(i=0; i<32; i++) {
    eTPU->CHAN[i].CR.B.ODIS = odis_a & 0x00000001;
    eTPU->CHAN[i].CR.B.OPOL = opol_a & 0x00000001;
    odis_a >>= 1;
    opol_a >>= 1;
	eTPU->CHAN[i].SCR.R = 0xc0c00000;
  }


  /*
   *  Synchronous start of all TCR time bases
   */

  fs_timer_start();
}


