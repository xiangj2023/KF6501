#ifndef _ST7529_H_
#define _ST7529_H_

#include "system.h"

#include "lcdcfg.h"

#if(LCD_CONTRLER == LCD_ST7529)


/*
	ST7529控制器最大支持160X255分辨率显示，每个点最大支持32级灰度。在本应用中，采用左上角为坐标
	原点。在内存映像中，行用线标识(line)，最大160线。由于每个点有32级灰度，需要用5位来描述，当
	使用8位操作时，可以用2个字节来描述3个点的信息(2B3P)，也可以用3个字节描述3个点的信息(3B3P)，
	而使用16位口时，1个字即可以完成3个点的信息，因此对于x方向3个连续点定义为1栏(可以从左到又，
	也可以从又到左，根据设置定义)；在y方向，连续4线为1块。在x方向上最小操作单位为栏，在y方向上
	最小操作单位为线，在本系统中液晶采用240X160点阵，因此为80栏X160线。

					   (0,0)------------------------> x(255,0)
							|
							|
							|
							|
							|
							|
							|
							\/
							y(0,160)
*/

#define 	ST7529_COLUMN	3		//定义1栏的象素数，栏为x方向的最小操作单位
#define		ST7529_BLOCK	4		//定义1块的线数

//ST7529液晶控制器的命令
#define      ExtIn       0x0030
#define      ExtOut      0x0031

#define 	BYTE_BIT_LEN	8	//定义每个字节的位数

//定义灰度显示
#define		GRAY_BLACK	0x00		//黑色
#define 	GRAY_WHITE	0xf8		//白色

//Ext=0                                     
#define      DISON       0x00AF		//显示打开
#define      DISOFF      0x00AE		//显示关闭
#define      DISNOR      0x00A6		//正常显示
#define      DISINV      0x00A7		//反显，不修改显示内存
#define      COMSCN      0x00BB		//COM驱动输出扫描方向定义
#define      DISCTRL     0x00CA		//显示控制，设置分频值、块个数等
#define      SLPIN       0x0095		//进入睡眠模式
#define      SLPOUT      0x0094		//退出睡眠模式
#define      LASET       0x0075		//线地址设置，设置起始和结束线
#define      CASET       0x0015		//栏地址设置，设置起始和结束栏
#define      DATSDR      0x00BC		//数据扫描方向设置，即设置原点及X和Y的方向。
#define      RAMWR       0x005C		//内存写
#define      RAMRD       0x005D		//内存读
#define      PTLIN       0x00A8		//设置局部显示区域(起始块和结束块)，并进入局部显示模式
#define      PTLOUT      0x00A9		//退出局部显示模式
#define      RMWIN       0x00E0		//进入读-修改-写模式
#define      RMWOUT      0x00EE		//退出读-修改-写模式
#define      ASCSET      0x00AA		//滚屏区域设置(包括滚屏方式、固定区域、滚屏区域)
#define      SCSTART     0x00AB		//滚屏起始块地址设置
#define      OSCON       0x00D1		//内部晶振打开
#define      OSCOFF      0x00D2		//内部晶振关闭
#define      PWRCTRL     0x0020		//电源控制设置
#define      VOLCTRL     0x0081		//电压调节控制(直接控制到指定值)
#define      VOLUP       0x00D6		//单步电压升(每次为0.04v)
#define      VOLDOWN     0x00D7		//单步电压降(每次为0.04v)
#define      RESERVED    0x0082		//
#define      EPSRRD1     0x007C		//读寄存器1
#define      EPSRRD2     0x007D		//读寄存器2
#define      NOP         0x0025		//空操作
#define      EPINT       0x0007		//初始化操作

//Ext=1
#define      Gray1Set    0x0020		//设置灰度
#define      Gray2Set    0x0021		//设置灰度
#define      ANASET      0x0032		//频率设置等
#define      SWINT       0x0034		//软件初始化
#define      EPCTIN      0x00CD		//eeprom控制读写模式
#define      EPCOUT      0x00CC		//eeprom读写模式退出
#define      EPMWR       0x00FC		//eeprom写操作
#define      EPMRD       0x00FD 	//eeprom读操作

//定义不带参数的控制命令
#define St7529LcdOn()		outportb(LCD_CMD_REG, DISON)
#define St7529LcdOff()		outportb(LCD_CMD_REG, DISOFF)
#define St7529DisNor()		outportb(LCD_CMD_REG, DISNOR)
#define St7529DisInv()		outportb(LCD_CMD_REG, DISINV)
#define St7529SlpIn()		outportb(LCD_CMD_REG, SLPIN)
#define St7529SlpOut()		outportb(LCD_CMD_REG, SLPOUT)
#define St7529PtlOut()		outportb(LCD_CMD_REG, PTLOUT)
#define St7529RmwIn()		outportb(LCD_CMD_REG, RMWIN)
#define St7529RmwOut()		outportb(LCD_CMD_REG, RMWOUT)
#define St7529OscOn()		outportb(LCD_CMD_REG, OSCON)
#define St7529OscOff()		outportb(LCD_CMD_REG, OSCOFF)
#define St7529VolUp()		outportb(LCD_CMD_REG, VOLUP)
#define St7529VolDown()		outportb(LCD_CMD_REG, VOLDOWN)
#define St7529VolDown()		outportb(LCD_CMD_REG, VOLDOWN)
#define St7529Epsrrd1()		outportb(LCD_CMD_REG, EPSRRD1)
#define St7529Epsrrd2()		outportb(LCD_CMD_REG, EPSRRD2)
#define St7529Nop()			outportb(LCD_CMD_REG, NOP)

#define St7529EpcOut()		outportb(LCD_CMD_REG, SWINT)
#define St7529SwInt()		outportb(LCD_CMD_REG, EPCOUT)
#define St7529EpmWr()		outportb(LCD_CMD_REG, EPMWR)
#define St7529EpmRd()		outportb(LCD_CMD_REG, EPMRD)

//写命令接口
//#define St7529WriteCmd(cmd)		outportb(LCD_CMD_REG, cmd)

//写数据接口
//#define St7529WriteDat(dat)		outportb(LCD_DATA_REG, dat)

//读数据接口
#define St7529ReadDat()		inportb(LCD_DATA_REG)

//电压背光调节
#define St7529BackLightUp()		outportb(LCD_CMD_REG, VOLUP)
#define St7529BackLightDown()	outportb(LCD_CMD_REG, VOLDOWN)

//二次样机背光开关控制
#define St7529BackLightOn()		outportb(LCD_BACK_LIGHT, 0x50)
#define St7529BackLightOff()	outportb(LCD_BACK_LIGHT, 0xa0)

//二次样机液晶复位控制
#define St7529HReset()			outportb(LCD_RESET_ADDR, 0xa0)

void St7529WriteCmd(BYTE cmd);
void St7529WriteDat(BYTE dat);
/*************************************************************************************
* 设置操作数据地址空间														  
*************************************************************************************/
DWORD St7529SetAdd(WORD x1, WORD y1, WORD x2, WORD y2);

/*************************************************************************************
* 设置操作数据地址空间，然后发送允许写操作命令														  
*************************************************************************************/
DWORD St7529SetWrite(WORD x1, WORD y1, WORD x2, WORD y2);

/*************************************************************************************
* 用来读取eeprom中的电压设置值														  
*************************************************************************************/
void St7529ReadEeprom(void);

/*************************************************************************************
* 将调整好的电压值保存到eeprom														  
*************************************************************************************/
void St7529WriteEeprom(void);

/*************************************************************************************
* 液晶控制器st7529初始化														  
*************************************************************************************/
void St7529Init(void);

/*************************************************************************************
* 画点函数，只考虑单色情况														  
*************************************************************************************/
void St7529SetPixel(int x, int y, LCD_PIXELINDEX pixel);

/*************************************************************************************
* 取象素点信息，仅考虑单色情况														  
*************************************************************************************/
LCD_PIXELINDEX St7529GetPixel(int x, int y);

/*************************************************************************************
* 画(擦除)横线操作														  
*************************************************************************************/
void St7529DrawHLine(int x0, int y0, int x1, BOOL isClear);

/*************************************************************************************
* 画(擦除)竖线操作														  
*************************************************************************************/
void St7529DrawVLine(int x0, int y0, int y1, BOOL isClear);

/*************************************************************************************
* 填充(擦除)矩形框														  
*************************************************************************************/
void St7529FillRect(int x0, int y0, int x1, int y1, BOOL isClear);

/*************************************************************************************
* 画矩形边框														  
*************************************************************************************/
void St7529DrawRect(int x0, int y0, int x1, int y1);

/*************************************************************************************
* 根据象素点信息，进行单行横线扫描显示														  
*************************************************************************************/
void St7529DrawBitLine1BPP(int x, int y, int xsize, BYTE *pData);

/*************************************************************************************
* 单色位图块扫描显示														  
*************************************************************************************/
void St7529DramBitMap(int x, int y, int xsize, int ysize, BYTE *pData);

void St7529Reset();

#endif

#endif
