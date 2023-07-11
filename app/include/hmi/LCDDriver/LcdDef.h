/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：Gui.h
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-3-4
*   作    者：kangqian
*   功    能：<...>
* 
*************************************************************************/

#ifndef __Gui_H
#define __Gui_H

#ifdef __cplusplus
extern "C"{
#endif



#include "daefunc.h"
#include "dbfunc.h"
#include "parafunc.h"
 

//#define LCD_XSIZE      (240)   /* X-resolution of LCD, Logical coor. */
//#define LCD_YSIZE      (160)   /* Y-resolution of LCD, Logical coor. */
#define LCD_STRLEN	   64  //字符串的最大长度
#define LCD_LANGNUM		2 //支持的最大在语言数目

#define	LCD_COLORINDEX 1
#define	LCD_BKCOLORINDEX 0
#define   COLORINDEX LCD_COLORINDEX
#define BKCOLORINDEX LCD_BKCOLORINDEX

#ifndef __PL_WIN_SIMULATION__
#define WM_USER 1000  
#endif

#define GUI_KEY_BACKSPACE         (WM_USER+1)         /* ASCII: BACKSPACE Crtl-H */
#define GUI_KEY_TAB               (WM_USER+2)         /* ASCII: TAB       Crtl-I */
#define GUI_KEY_ENTER             (WM_USER+3)        /* ASCII: ENTER     Crtl-M */
#define GUI_KEY_LEFT              (WM_USER+4)
#define GUI_KEY_RIGHT             (WM_USER+5)
#define GUI_KEY_HOME              (WM_USER+6)
#define GUI_KEY_END               (WM_USER+7)
#define GUI_KEY_SHIFT             (WM_USER+8)
#define GUI_KEY_CONTROL           (WM_USER+9)
#define GUI_KEY_ESCAPE            (WM_USER+10)        /* ASCII: ESCAPE    0x1b   */
#define GUI_KEY_INSERT            (WM_USER+11)
#define GUI_KEY_DELETE            (WM_USER+12)

#define GUI_KEY_ADD               (WM_USER+13)
#define GUI_KEY_DECREASE          (WM_USER+14)

#define GUI_KEY_DOWN              (WM_USER+15)
#define GUI_KEY_UP                (WM_USER+16)

#define OS_TIMER (WM_USER+17)


unsigned LCD_L0_GetPixelIndex(int x, int y);
void LCD_L0_XorPixel(int x, int y);
void LCD_L0_SetPixelIndex(int x, int y, int ColorIndex=COLORINDEX);

//以下函数是移植Lcd驱动的时候必须要实现的, Data开始的都是绘制到缓存区的
void Data_XorRect(int x, int y, int xSize, int ySize);
void Data_DrawText(int x, int y,unsigned char* txt, int color, int bkcolor, int nFontSize);
void Data_DrawPixel(int x, int y);

void Data_DrawHLine(int x0, int y, int x1, int ColorIndex=COLORINDEX);
void Data_DrawVLine(int x, int y0, int y1,int ColorIndex=COLORINDEX);

//增加划虚线函数
void Data_DrawHDotLine(int x0, int y, int x1, int ColorIndex=COLORINDEX);
void Data_DrawVDotLine(int x, int y0, int y1,int ColorIndex=COLORINDEX);
void Data_DrawRect(int x0, int y0, int x1, int y1);

void Data_FillSolidRect(int x0,int y0, int x1, int y1, int ColorIndex=COLORINDEX);

void LCD_Paint(int x, int y, int xSize, int ySize); //重绘该区域到显示器
void Data_Clear();
void Data_DrawBitmap   (int x0, int y0,
                       int xsize, int ysize,
                       int BytesPerLine,
                       const unsigned char* pData,
					   int color, int bkcolor);

//必须实现的线程控制
#ifdef __PL_WIN_SIMULATION__
BOOL WaitForMsg(long& MsgCod, long& Data, long& reserv);
#endif

void Data_SetPixelIndex(int x, int y, int ColorIndex);
/**********************/
#ifdef __cplusplus
}
#endif

#endif /* __Gui_H */

/*************************************************************************
* 改动历史纪录：

Revision 1.0, 2008-3-4, kangqian
describe: 初始创建.
*************************************************************************/
