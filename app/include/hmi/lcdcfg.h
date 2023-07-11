#ifndef _LCDCFG_H_
#define _LCDCFG_H_

//定义液晶控制器类型
#define LCD_ST7529			1
#define LCD_SED1330			2
#define LCD_SED1335			3

#define LCD_CONTRLER		LCD_ST7529		//定义液晶控制器类型


//定义液晶显示点阵，必须小于或等于液晶控制器所支持的点阵数。
#define LCD_XSIZE	240
#define LCD_YSIZE	160

#define LCD_BITSPERPIXEL	1		//定义描述每个象素需要的位数
#define LCD_PIXELINDEX		BYTE	//定义描述每个象素点的数据类型

//定义单色显示时，黑白颜色值，每位标识一个象素点
#define LCD_BLACK			1
#define LCD_WHITE			0

//针对ST7529控制器的液晶操作接口定义
#if(LCD_CONTRLER == LCD_ST7529)

	#include "st7529.h"
	
	#define LcdBackLightOn		St7529BackLightOn		//背光打开
	#define LcdBackLightOff		St7529BackLightOff		//背光关闭
	#define LcdBackLightUp		St7529BackLightUp		//增加背光亮度
	#define LcdBackLightDown	St7529BackLightDown		//减弱背光亮度
	#define LcdBackLightSave	St7529WriteEeprom		//保存背光调节
	
	#define LcdCtlerInit		St7529Init				//液晶控制器初始化
	#define LcdSetPixel			St7529SetPixel			//画象素点
	#define LcdGetPixel			St7529GetPixel			//读取象素点信息
	#define LcdDrawHLine		St7529DrawHLine			//画或擦除横线
	#define LcdDrawVLine		St7529DrawVLine			//画或擦除竖线
	#define LcdDrawRect			St7529DrawRect			//画矩形边框
	#define LcdFillRect			St7529FillRect			//矩形填充或擦除
	#define LcdDrawBitLine1BPP	St7529DrawBitLine1BPP	//根据象素信息扫描单横线	
	#define LcdDrawBitMap		St7529DramBitMap		//根据象素内容扫描位图空间
	
#endif

//键盘值定义

#if(HARD_WARE_VER == HW_VERSION_10)
	#define	KEY_UP			0xFF7F   
	#define	KEY_DOWN		0xFFEF
	#define	KEY_LEFT     	0xFFFD
	#define	KEY_RIGHT    	0xFFFB

	#define	KEY_ENTER   	0xFFFE
	#define	KEY_ESCAPE   	0xFFBF

	#define KEY_ADD			0xFFDF
	#define KEY_MINUS		0xFFF7

	#define KEY_INVALID		0xFFFF

#elif(HARD_WARE_VER == HW_VERSION_11 ||HARD_WARE_VER == HW_VERSION_12)
	#define	KEY_UP			0x7FFB   
	#define	KEY_DOWN		0x7FF7
	#define	KEY_LEFT     	0x7FEF
	#define	KEY_RIGHT    	0x7FDF

	#define	KEY_ENTER   	0x7F7F
	#define	KEY_ESCAPE   	0x7FBF

	#define KEY_ADD			0x7FFE
	#define KEY_MINUS		0x7FFD
	#define KEY_COMB		0x7FBC	//"+"、"-"与"C"组合键
	#define KEY_LCDC        0x7F7C	//"+"、"-"与"ENTER"组合键

	#define KEY_REC			0x7EFF		//复归按键
	
	#define KEY_INVALID		0x7FFF

#endif

#endif
