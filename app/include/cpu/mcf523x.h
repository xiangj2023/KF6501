
#ifndef __MCF523X_H__
#define __MCF523X_H__

#include "mcf523x_fmpll.h"
#include "mcf523x_ccm.h"
#include "mcf523x_cs.h"
#include "mcf523x_eport.h"
#include "mcf523x_fec.h"
#include "mcf523x_can.h"
#include "mcf523x_gpio.h"
#include "mcf523x_i2c.h"
#include "mcf523x_intc0.h"
#include "mcf523x_intc1.h"
#include "mcf523x_mdha.h"
#include "mcf523x_pit.h"
#include "mcf523x_qspi.h"
#include "mcf523x_rcm.h"
#include "mcf523x_rng.h"
#include "mcf523x_scm.h"
#include "mcf523x_sdramc.h"
#include "mcf523x_skha.h"
#include "mcf523x_sram.h"
#include "mcf523x_timer.h"
#include "mcf523x_uart.h"
#include "mcf523x_wtm.h"
#include "mcf523x_etpu.h"

//mcf523x中断向量号定义
//对于INTC0,向量号=64+中断资源号
#define MCF_INTC0_LVL_LVL1        	 (65)
#define MCF_INTC0_LVL_LVL2        	 (66)
#define MCF_INTC0_LVL_LVL3        	 (67)
#define MCF_INTC0_LVL_LVL4        	 (68)
#define MCF_INTC0_LVL_LVL5        	 (69)
#define MCF_INTC0_LVL_LVL6        	 (70)
#define MCF_INTC0_LVL_LVL7        	 (71)
#define MCF_INTC0_LVL_LVL8        	 (72)
#define MCF_INTC0_LVL_LVL9        	 (73)
#define MCF_INTC0_LVL_LVL10       	 (74)
#define MCF_INTC0_LVL_LVL11       	 (75)
#define MCF_INTC0_LVL_LVL12       	 (76)
#define MCF_INTC0_LVL_UART0       	 (77)
#define MCF_INTC0_LVL_UART1       	 (78)
#define MCF_INTC0_LVL_UART2       	 (79)
#define MCF_INTC0_LVL_LVL16       	 (80)
#define MCF_INTC0_LVL_IIC       	 (81)
#define MCF_INTC0_LVL_SPI       	 (82)
#define MCF_INTC0_LVL_TMR0     		 (83)
#define MCF_INTC0_LVL_TMR1     		 (84)
#define MCF_INTC0_LVL_TMR2     		 (85)
#define MCF_INTC0_LVL_TMR3     		 (86)
#define MCF_INTC0_LVL_FEC_X_INTF     (87)
#define MCF_INTC0_LVL_FEC_X_INTB     (88)
#define MCF_INTC0_LVL_FEC_UN         (89)
#define MCF_INTC0_LVL_FEC_RL         (90)
#define MCF_INTC0_LVL_FEC_R_INTF     (91)
#define MCF_INTC0_LVL_FEC_R_INTB     (92)
#define MCF_INTC0_LVL_FEC_MII        (93)
#define MCF_INTC0_LVL_FEC_LC         (94)
#define MCF_INTC0_LVL_FEC_HBERR      (95)

#define MCF_INTC0_LVL_FEC_GRA       (96)
#define MCF_INTC0_LVL_FEC_EBERR     (97)
#define MCF_INTC0_LVL_FEC_BABT      (98)
#define MCF_INTC0_LVL_FEC_BABR      (99)
#define MCF_INTC0_LVL_PIT0     		(100)
#define MCF_INTC0_LVL_PIT1     		(101)
#define MCF_INTC0_LVL_PIT2     		(102)
#define MCF_INTC0_LVL_PIT3     		(103)
#define MCF_INTC0_LVL_RNG           (104)
#define MCF_INTC0_INT_LVL41         (105)
#define MCF_INTC0_INT_LVL42         (106)
#define MCF_INTC0_LVL_CAN1_BUF0     (107)
#define MCF_INTC0_LVL_CAN1_BUF1     (108)
#define MCF_INTC0_LVL_CAN1_BUF2     (109)
#define MCF_INTC0_LVL_CAN1_BUF3     (110)
#define MCF_INTC0_LVL_CAN1_BUF4     (111)
#define MCF_INTC0_LVL_CAN1_BUF5     (112)
#define MCF_INTC0_LVL_CAN1_BUF6     (113)
#define MCF_INTC0_LVL_CAN1_BUF7     (114)
#define MCF_INTC0_LVL_CAN1_BUF8     (115)
#define MCF_INTC0_LVL_CAN1_BUF9     (116)
#define MCF_INTC0_LVL_CAN1_BUF10    (117)
#define MCF_INTC0_LVL_CAN1_BUF11    (118)
#define MCF_INTC0_LVL_CAN1_BUF12    (119)
#define MCF_INTC0_LVL_CAN1_BUF13    (120)
#define MCF_INTC0_LVL_CAN1_BUF14    (121)
#define MCF_INTC0_LVL_CAN1_BUF15    (122)
#define MCF_INTC0_LVL_CAN1_ERR      (123)
#define MCF_INTC0_LVL_CAN1_BOFF     (124)
#define MCF_INTC0_INT_LVL61         (125)
#define MCF_INTC0_INT_LVL62         (126)
#define MCF_INTC0_INT_LVL63         (127)
                        
//对于INTC1,向量号=128+中断资源号
#define MCF_INTC1_INT_LVL1      	 (129)
#define MCF_INTC1_INT_LVL2      	 (130)     
#define MCF_INTC1_INT_LVL3      	 (131)     
#define MCF_INTC1_INT_LVL4      	 (132)     
#define MCF_INTC1_INT_LVL5      	 (133)     
#define MCF_INTC1_INT_LVL6      	 (134)     
#define MCF_INTC1_INT_LVL7      	 (135)     
#define MCF_INTC1_LVL_CAN0_BUF0 	 (136)     
#define MCF_INTC1_LVL_CAN0_BUF1      (137)
#define MCF_INTC1_LVL_CAN0_BUF2      (138)
#define MCF_INTC1_LVL_CAN0_BUF3      (139)
#define MCF_INTC1_LVL_CAN0_BUF4      (140)
#define MCF_INTC1_LVL_CAN0_BUF5      (141)
#define MCF_INTC1_LVL_CAN0_BUF6      (142)
#define MCF_INTC1_LVL_CAN0_BUF7      (143)
#define MCF_INTC1_LVL_CAN0_BUF8      (144)
#define MCF_INTC1_LVL_CAN0_BUF9      (145)
#define MCF_INTC1_LVL_CAN0_BUF10     (146)
#define MCF_INTC1_LVL_CAN0_BUF11     (147)
#define MCF_INTC1_LVL_CAN0_BUF12     (148)
#define MCF_INTC1_LVL_CAN0_BUF13     (149)
#define MCF_INTC1_LVL_CAN0_BUF14     (150)
#define MCF_INTC1_LVL_CAN0_BUF15     (151)
#define MCF_INTC1_LVL_CAN0_ERR       (152)
#define MCF_INTC1_LVL_CAN0_BOFF      (153)
#define MCF_INTC1_INT_LVL26          (154)
#define MCF_INTC1_INT_LVL27     	 (155)     
#define MCF_INTC1_INT_LVL28     	 (156)     
#define MCF_INTC1_INT_LVL29     	 (157)     
#define MCF_INTC1_INT_LVL30     	 (158)     
#define MCF_INTC1_INT_LVL31     	 (159)     

#define MCF_INTC1_INT_LVL32     (160)    
#define MCF_INTC1_INT_LVL33     (161)
#define MCF_INTC1_INT_LVL34     (162)
#define MCF_INTC1_INT_LVL35     (163)
#define MCF_INTC1_INT_LVL36     (164)
#define MCF_INTC1_INT_LVL37     (165)
#define MCF_INTC1_INT_LVL38     (166)
#define MCF_INTC1_INT_LVL39     (167)
#define MCF_INTC1_INT_LVL40     (168)
#define MCF_INTC1_INT_LVL41     (169)
#define MCF_INTC1_INT_LVL42     (170)
#define MCF_INTC1_INT_LVL43     (171)
#define MCF_INTC1_INT_LVL44     (172)
#define MCF_INTC1_INT_LVL45     (173)
#define MCF_INTC1_INT_LVL46     (174)
#define MCF_INTC1_INT_LVL47     (175)
#define MCF_INTC1_INT_LVL48     (176)
#define MCF_INTC1_INT_LVL49     (177)
#define MCF_INTC1_INT_LVL50     (178)
#define MCF_INTC1_INT_LVL51     (179)
#define MCF_INTC1_INT_LVL52     (180)
#define MCF_INTC1_INT_LVL53     (181)
#define MCF_INTC1_INT_LVL54     (182)
#define MCF_INTC1_INT_LVL55     (183)
#define MCF_INTC1_INT_LVL56     (184)
#define MCF_INTC1_INT_LVL57     (185)
#define MCF_INTC1_INT_LVL58     (186)
#define MCF_INTC1_INT_LVL59     (187)
#define MCF_INTC1_INT_LVL60     (188)
#define MCF_INTC1_INT_LVL61     (189)
#define MCF_INTC1_INT_LVL62     (190)
#define MCF_INTC1_INT_LVL63     (191)

#endif /* __MCF523X_H__ */
