/*************************************************************************
* 
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
* 
*   文件名称：LCDWin.cpp
*   软件模块：<...>
*   版 本 号：1.0
*   生成日期：2008-3-4
*   作    者：kangqian
*   功    能：<...>
* 	修改记录:  2011-4-26 kangqian  在Data_Draw等函数中增加对绘图范围的判断，修正
*************************************************************************/
#include "LcdDef.h"
#include "LcdCfg.h"
extern unsigned char ASCII16[];
extern  unsigned char ASCII12[];
extern unsigned char *HZK16;
char LcdData[LCD_YSIZE][LCD_XSIZE/8]; //240*160 


#ifdef __PL_WIN_SIMULATION__
#include "../stdafx.h"
#include <winuser.h>
//@#include "LCD.H"
//@#include "LCD_Private.h"              /* include LCDConf.h */
//@#include "GUI_Private.h"
extern CFile g_file;
#include "../WinGui.h"
#include "../WinGuiDlg.h"
#endif



#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned long   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */



#define LCD_BITSPERPIXEL (1)
#define LCD_NUM_COLORS (1L<<LCD_BITSPERPIXEL)

#if LCD_BITSPERPIXEL <= 8
  #define PIXELINDEX U8
#else
  #define PIXELINDEX WORD
#endif

#ifndef LCD_DISPLAY_INDEX
  #define LCD_DISPLAY_INDEX 0
#endif

#if LCD_BITSPERPIXEL <=8
  #define LCD_PIXELINDEX U8
#else
  #define LCD_PIXELINDEX U16
#endif


/*
        *********************************************************
        *                                                       *
        *       ID translation table                            *
        *                                                       *
        *********************************************************

This table contains 0, 1, 2, ... and serves as translation table for DDBs

*/

#define INTS(Base)  Base+0,Base+1,Base+2,Base+3,Base+4,Base+5,   \
                    Base+6,Base+7,Base+8,Base+9,Base+10,Base+11, \
                    Base+12,Base+13,Base+14,Base+15

/*static void XorPixel   (int x, int y) {
  unsigned int Index = LCD_L0_GetPixelIndex(x,y);
  LCDSIM_SetPixelIndex(x, y, LCD_NUM_COLORS-1-Index);
}*/


/*
        *********************************************************
        *                                                       *
        *       LCD_L0_SetColorIndex                            *
        *       LCD_L0_SetBkColorIndex                          *
        *                                                       *
        *********************************************************
*/


/*
        *********************************************************
        *                                                       *
        *       LCD_L0_DrawPixel                                   *
        *                                                       *
        *********************************************************

Purpose:  This routine is called by emWin. It writes 1 pixel into the
          display.

*/
void Data_SetPixelIndex(int x ,int y, int ColorIndex)
{
	
	int i,j;
	unsigned char mask;
 	int data; 

	if((y>= LCD_YSIZE)|| y<0)
		return;
	if((x>= LCD_XSIZE) || x<0)
		return;

	j = x%8; //第几位
	i = (x-j)/8; //第几个字节
	//颜色
	if(ColorIndex)
 	data = 1;
    else
	data = 0;


	if(data)
	{
	  mask = 0x01<<(7-j);
	  LcdData[y][i] = LcdData[y][i]|mask;
	}
	else
	{
	  mask = 0x01<<(7-j);
	  mask = ~mask;
	  LcdData[y][i] = LcdData[y][i]&mask;
	}

}
void Data_DrawPixel(int x, int y, int ColorIndex)
{
	
	if((y>= LCD_YSIZE)|| y<0)
		return;
	if((x>= LCD_XSIZE) || x<0)
		return;
	 Data_SetPixelIndex(x, y, ColorIndex);
}
unsigned Data_GetPixelIndex(int x, int y) 
{
	
	if((y>= LCD_YSIZE)|| y<0)
		return 0;
	if((x>= LCD_XSIZE) || x<0)
		return 0;
  int i,j;
  unsigned char data, mask;

  j = x%8;
  i = (x-j)/8;
  mask = 0x01<<(7-j);
  data = LcdData[y][i];
  data = data&mask;
  if(data)
  	return 1;
  else
	return 0;
}
void Data_XorPixel(int x, int y)
{
	
 if((y>= LCD_YSIZE)|| y<0)
	return;
 if((x>= LCD_XSIZE) || x<0)
	return;
  int i;
  i = Data_GetPixelIndex(x, y);
  Data_DrawPixel(x,y,2-1-i);
}

void Data_XorRect(int x, int y, int xSize, int ySize)
{
	int i,j;
	//2011-4-25 kang 判断范围
	if((x>=LCD_XSIZE) || x<0)
		return;
	
	if((y>=LCD_YSIZE) || y<0)
		return;

	if((x+xSize)>LCD_XSIZE)
		xSize =  LCD_XSIZE - x;
	if((y+ySize)>LCD_YSIZE)
		ySize = LCD_YSIZE -y;
	
	for(i=0; i<xSize; i++)
	{
		for(j=0; j<ySize; j++)
		{
			Data_XorPixel(x+i, y+j);

		}
	}
}
void Data_DrawText(int x, int y, unsigned char* txt, int color, int bkcolor,int nFontSize)
{
	int i,pos;
	int nQh, nWh;
	const unsigned char* pData;
	const unsigned char* pFontBase;
	pos = x;
	i= 0;
	int index;
	char unsigned data[32];

	if(nFontSize == 16)
		pFontBase = &ASCII16[0];
	else if(nFontSize == 12)
		pFontBase = &ASCII12[0];

		
	
	//调试用， 不用文件用字符串先
	const unsigned char*	pFontBaseHzk = HZK16;

	LONG nCurline = 0;



	while(txt[i] != '\0')
	{
		if (txt[i] == '\n')
		{
			nCurline++;
			pos = x;
			i++;
			continue;
		}
		//16点assic码
		else if(txt[i] == 0x20)
		{
			pos = pos+nFontSize/2;
			i++;
		}
		else if(txt[i]<=0x7e&&txt[i]>0x20)
		{

			pData = &(pFontBase[(txt[i]-0x21)*nFontSize]);
			Data_DrawBitmap(pos, y + nCurline*(nFontSize+2), nFontSize/2, nFontSize, 1, pData, color, bkcolor);
			pos = pos+nFontSize/2;
			i++;
		}
		else if(txt[i] == 0x7f || txt[i] == 0x80)
		{
			pData = &(pFontBase[(txt[i]-0x21)*nFontSize]);
			Data_DrawBitmap(pos, y + nCurline*(nFontSize+2), 16, nFontSize/2, 2, pData, color, bkcolor);
			pos = pos+16;
			i++;

		}
		else if((txt[i]>0xA0) && (txt[i+1]>0xA0))
		{
			nQh = txt[i] - 0xA0;
			nWh = txt[i+1] - 0xA0;
			//判断区位号是否合法
			index = (nQh-1)*94 + nWh-1;

#ifdef 	__PL_WIN_SIMULATION__
			g_file.Seek(index*16*2, CFile::begin);
			pData = &data[0];
			g_file.Read((void*)pData, 16*2);
#endif

			pData = &(pFontBaseHzk[index*16*2]);
			Data_DrawBitmap(pos, y + nCurline*(nFontSize+2), 16, 16, 2, pData, color, bkcolor);
			pos = pos+16;
			i = i+2;
		}
		else 
		{
			pos = pos +nFontSize/2;
			i++;
		}
	}

}



void Data_DrawHLine(int x0, int y, int x1, int ColorIndex)

{
	
	int i0,j0, i1,j1;
	unsigned char mask,mask2;
	int data;

	//2011-4-26 不可超出屏幕范围
	
	if((y>=LCD_YSIZE) || (y<0))
		return;
	if(x0>x1)
	{
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if(x0<0)
		x0 =0;
	if(x0>=LCD_XSIZE)
		return;
	if(x1>=LCD_XSIZE)
		x1 = LCD_XSIZE -1;

	if(x0 == x1)
	 { Data_DrawPixel(x0, y, ColorIndex);
		return;}

	if(ColorIndex)
		data = 1;
	else 
		data = 0;


	j0 = x0%8;
	i0 = (x0-j0)/8;
	j1 = x1%8;
	i1 = (x1-j1)/8;
/* inside on byte */
	if(i1 == i0)
	{
      if(data)
  	  {
	  	mask = 0xFE<<(7-j0);
	  	mask = ~mask;
		mask2 = 0x7F>>j1;
		mask2 =~mask2;
		mask = mask&mask2;
	  	LcdData[y][i0] = LcdData[y][i0]|mask;
	  }
		else
	  {
	 	mask = 0xFE<<(7-j0);
		mask2 = 0x7F>>j1;
		mask = mask|mask2;
	 	LcdData[y][i0] = LcdData[y][i0]&mask;
	  }
	  return;
	}
		
		

/*line begin */	
    if(data)
	{
	  mask = 0xFE<<(7-j0);
	  mask = ~mask;
	  LcdData[y][i0] = LcdData[y][i0]|mask;
	 }
	else
	{
	  mask = 0xFE<<(7-j0);
	  LcdData[y][i0] = LcdData[y][i0]&mask;
	}
/*line end */
	if(i1 == i0)
		return;
	if(data)
	{
	  mask = 0x7F>>j1;
	  mask = ~mask;
	  LcdData[y][i1] = LcdData[y][i1]|mask;
	}
	else
	{
	  mask = 0x7F>>j1;
	  LcdData[y][i1] = LcdData[y][i1]&mask;
	}
	if(data)
		mask = 0xFF;
	else
		mask = 0;
	i0 = i0 + 1;
	i1 = i1 - 1;
	if(i0 > i1)
	 return;
/*line middle */
	for(; i0<=i1; i0++)
	{
	  LcdData[y][i0] = mask;
	}
}

//虚横线
void Data_DrawHDotLine(int x0, int y, int x1, int ColorIndex)
{

	if((y>=LCD_YSIZE) || (y<0))
		return;
	if(x0>x1)
	{
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
	}
	if(x0<0)
		x0 =0;
	if(x0>=LCD_XSIZE)
		return;
	if(x1>=LCD_XSIZE)
		x1 = LCD_XSIZE -1;	
	if(x0 == x1)
	{	
		Data_DrawPixel(x0,y,ColorIndex);
		return;
	 }
	  
	for(; x0 <= x1; x0++)
	{
	  if(x0%5<4)
		  Data_DrawPixel(x0, y,ColorIndex);
	  else
		  Data_DrawPixel(x0, y,(1-ColorIndex));

	}

	if(x1%5 >= 4)
		  Data_DrawPixel(x0, y,ColorIndex);

}

void Data_DrawVLine(int x, int y0, int y1, int ColorIndex)
{
	if( (x>=LCD_XSIZE) || (x<0))
		return;
	
	if(y0>y1)
	{
		int tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	if(y0<0)
		y0 =0;
	if(y0>=LCD_YSIZE)
		return;
	if(y1>=LCD_YSIZE)
		y1 = LCD_YSIZE -1;
	
	if(y0 == y1)
	{	Data_DrawPixel(x,y0,ColorIndex);
		return;
	 }
	  
	for(; y0 <= y1; y0++)
	  Data_DrawPixel(x, y0,ColorIndex);
}
//画虚速线
void Data_DrawVDotLine(int x, int y0, int y1, int ColorIndex)
{
		if( (x>=LCD_XSIZE) || (x<0))
		return;
	
	if(y0>y1)
	{
		int tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	if(y0<0)
		y0 =0;
	if(y0>=LCD_YSIZE)
		return;
	if(y1>=LCD_YSIZE)
		y1 = LCD_YSIZE -1;

	//
	if(y0 == y1)
	{	Data_DrawPixel(x,y0,ColorIndex);
		return;
	}
	  
	for(; y0 <= y1; y0++)
	{
		//2实1虚
		if(y0%5<4)
			Data_DrawPixel(x, y0,ColorIndex);
		else
			Data_DrawPixel(x, y0,(1-ColorIndex));

	}

	//结尾
	if(y1%5 >=4)
		Data_DrawPixel(x, y0,ColorIndex);


}

void Data_FillSolidRect(int x0, int y0, int x1, int y1, int ColorIndex)
{
  for (; y0 < y1; y0++) {
    Data_DrawHLine(x0,y0, x1, ColorIndex);
  }
}
void Data_DrawRect(int x0, int y0, int x1, int y1)
{
	
	//画边框
	Data_DrawHLine(x0, y0, x1); //上
	Data_DrawHLine(x0, y1,x1); //下
	
	Data_DrawVLine(x0, y0, y1); //左
	Data_DrawVLine(x1, y0, y1); //右
}

 void LCD_L0_DrawPixel(int x, int y,int ColorIndex) 
 {
	LCD_L0_SetPixelIndex(x,y, ColorIndex);
}

/*
        *********************************************************
        *                                                       *
        *          LCD_DrawLine  vertical/horizontal            *
        *          LCD_DrawRect                                 *
        *                                                       *
        *********************************************************
*/

void LCD_L0_DrawHLine  (int x0, int y,  int x1, int ColorIndex) 
{
    
    Data_DrawHLine(x0, y, x1, ColorIndex);
//	BIOSWriteLCDEx(0, y,LcdData[y],40);
//@	  AfxGetMainWnd()->SendMessage(WM_PAINT);

}

void LCD_L0_DrawVLine  (int x, int y0,  int y1,int ColorIndex) 
{

	Data_DrawVLine(x, y0, y1, ColorIndex);
//	BIOSWriteLCDEx(0, y0, LcdData[y0], (y1-y0+1)*40);
//@	  AfxGetMainWnd()->SendMessage(WM_PAINT);

}

void LCD_L0_FillRect(int x0, int y0, int x1, int y1,int ColorIndex)
 {
	Data_FillSolidRect(x0, y0, x1, y1, ColorIndex);
//	BIOSWriteLCDEx(0, y0, LcdData[y0], (y1-y0+1)*40);
//@	  AfxGetMainWnd()->SendMessage(WM_PAINT);

}




/*
  ***************************************************************
  *                                                             *
  *            Internal bitmap routines                         *
  *                                                             *
  ***************************************************************

*/


/*
    *********************************************
    *                                           *
    *      Draw Bitmap 1 BPP                    *
    *                                           *
    *********************************************
*/

void  Data_DrawBitLine1BPP(int x, int y, const unsigned char  *p,int xsize, int color, int bkcolor) 
{
	int Diff = 0;
    do {
      Data_SetPixelIndex(x++,y, (*p & (0x80>>Diff)) ? color : bkcolor);
			if (++Diff==8) {
        Diff=0;
				p++;
			}
		} while (--xsize);
}
void Data_DrawBitLine8BPP  (int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) 
{
 LCD_PIXELINDEX pixel;
    if (pTrans) 
	{

      for (;xsize > 0; xsize--,x++,p++) 
	  {
        pixel = *p;
        /*SETPIXEL(x, y, *(pTrans+pixel));*/
		Data_SetPixelIndex(x, y, *(pTrans+pixel));
      }
    } 
	else
	{
      for (;xsize > 0; xsize--,x++,p++) 
	  {
        /*SETPIXEL(x, y, *p);*/
		  Data_SetPixelIndex(x, y, *p);

      }
    }

}

/*
        *********************************************************
        *                                                       *
        *         Universal draw Bitmap routine                 *
        *                                                       *
        *********************************************************
*/
//在位置x,y绘制尺寸xsiz, ysize的 数据 为data 的图像，bytesperLine重要参数， 对于尺寸不是字节对齐的
//如xize 为 6的， 其bytesperLine可以是1（一般认为是压缩的）也可以是2（16位对齐，windows的内存图片就是这样对齐的）

void Data_DrawBitmap   (int x0, int y0,
                       int xsize, int ysize,
                       int BytesPerLine,
                       const unsigned char* pData,
					   int color, int bkcolor)
{
  int i;
    for (i=0; i<ysize; i++) 
  {
		Data_DrawBitLine1BPP(x0, i+y0, pData,xsize, color, bkcolor);
        pData += BytesPerLine;
	}

//@    AfxGetMainWnd()->SendMessage(WM_PAINT);
//BIOSWriteLCDEx(0, y0,LcdData[y0],40*ysize);
}


/********************************************************
*
*       LCD_L0_SetOrg
*
*********************************************************

Purpose:        Sets the original position of the virtual display.
                Has no function at this point with the PC-driver.
*/

int OrgX, OrgY;
void LCD_L0_SetOrg(int x, int y) {
  OrgX = x;
  OrgY = y;
}

/*
        *********************************************************
        *                                                       *
        *           Support for verification                    *
        *                                                       *
        *********************************************************

The following routines are implemented, but have no functionility
at this point. The reason is that these functions are supposed
to supervise the hardware, which for obvious reasons can not be
done in a simulation.

*/
#if LCD_VERIFY
int  LCD_GetErrStat(void) {
  return 0;
}
void LCD_ClrErrStat(void) {
}
int  LCD_GetErrCnt (void) {
  return 0;
}
#endif  

/*
        *********************************************************
        *                                                       *
        *       LCD_On                                          *
        *       LCD_Off                                         *
        *                                                       *
        *********************************************************

(Not supported in Simulation)
*/

void LCD_Off          (void) {}
void LCD_On           (void) {}

/*
        *********************************************************
        *                                                       *
        *       LUT routines (lookup tables)                    *
        *                                                       *
        *********************************************************
*/
//@void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR color) {
//@}


/*
        *********************************************************
        *                                                       *
        *       LCD_Init : Init the display                     *
        *                                                       *
        *********************************************************
*/

int  LCD_L0_Init(void) {
	int i,j;
//	BIOSInitLCD();
//	BIOSClearLCDText();
//	BIOSClearLCDGraph();
	for(i=0;i<240;i++)
		for(j =0; j<40; j++)
 		   LcdData[i][j] = 1;
		return 0;
}

int  LCD_L0_CheckInit(void) {
  return 0;
} 

/*
        ******************************************
        *                                        *
        *    Re-initialize LCD                   *
        *                                        *
        ******************************************

This routine is supplied for compatibility and interchangability of
"C"-sources with embedded versions of the driver. It has no real
effect in the PC-version as there is simply no need to re-initialize
the LCD since it is just simulated anyhow.
*/

void LCD_L0_ReInit       (void) {
	LCD_L0_Init();
}

unsigned LCD_L0_GetPixelIndex(int x, int y)  {
  int i,j;
  unsigned char data, mask;

  j = x%8;
  i = (x-j)/8;
  mask = 0x01<<(7-j);
  data = LcdData[y][i];
  data = data|mask;
  if(data)
  	return 1;
  else
	return 0;
}

/*
        *********************************************************
        *                                                       *
        *       LCD_L0_XorPixel                                 *
        *                                                       *
        *********************************************************

Purpose:  This routine is called by emWin. It writes 1 pixel into the
          display.

*/

void LCD_L0_XorPixel(int x, int y) {
  int i;
  i = LCD_L0_GetPixelIndex(x, y);
  LCD_L0_SetPixelIndex(x,y,LCD_NUM_COLORS-1-i);
}


/*
        *********************************************************
        *                                                       *
        *       LCD_L0_SetPixelIndex                            *
        *                                                       *
        *********************************************************

Purpose:  This routine is called by emWin. It writes 1 pixel into the
          display.

*/

void LCD_L0_SetPixelIndex(int x, int y, int ColorIndex) {
  Data_SetPixelIndex(x, y, ColorIndex); 
//@  AfxGetMainWnd()->SendMessage(WM_PAINT);
//  BIOSWriteLCDEx(0, y,LcdData[y],40);

}
void LCD_Paint(int x, int y, int xSize, int ySize)
{
#ifdef __PL_WIN_SIMULATION__
	AfxGetMainWnd()->SendMessage(WM_PAINT);
#else
	LcdDrawBitMap(x, y, xSize, ySize, (BYTE*)LcdData);
#endif
}
void Data_Clear()
{
	Data_FillSolidRect(0,0,LCD_XSIZE, LCD_YSIZE, LCD_BKCOLORINDEX);

}

#ifdef __PL_WIN_SIMULATION__
BOOL WaitForMsg(long& MsgCode, long& wParam, long& lParam)
{
	LPMSG lpMsg;         // address of structure with message
	HWND hWnd;          // handle of window

	BOOL ret;
	ret = GetMessage(&msg,NULL,WM_USER, WM_USER+100);
	lpMsg = &msg;
	if(ret != -1)
	{
		MsgCode = lpMsg->message;

		if( MsgCode>=GUI_KEY_BACKSPACE 
			&& 	MsgCode <= GUI_KEY_UP)
			MsgCode = HMI_KEYDOWN;

		wParam = lpMsg->wParam;
		lParam = lpMsg->lParam;
	}
	else
		MsgCode = -1;

	return ret;

}
#endif




/*************************************************************************
* 改动历史纪录：

Revision 1.0, 2008-3-4, kangqian
describe: 初始创建.
*************************************************************************/
