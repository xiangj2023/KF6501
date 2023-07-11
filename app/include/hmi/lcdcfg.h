#ifndef _LCDCFG_H_
#define _LCDCFG_H_

//����Һ������������
#define LCD_ST7529			1
#define LCD_SED1330			2
#define LCD_SED1335			3

#define LCD_CONTRLER		LCD_ST7529		//����Һ������������


//����Һ����ʾ���󣬱���С�ڻ����Һ����������֧�ֵĵ�������
#define LCD_XSIZE	240
#define LCD_YSIZE	160

#define LCD_BITSPERPIXEL	1		//��������ÿ��������Ҫ��λ��
#define LCD_PIXELINDEX		BYTE	//��������ÿ�����ص����������

//���嵥ɫ��ʾʱ���ڰ���ɫֵ��ÿλ��ʶһ�����ص�
#define LCD_BLACK			1
#define LCD_WHITE			0

//���ST7529��������Һ�������ӿڶ���
#if(LCD_CONTRLER == LCD_ST7529)

	#include "st7529.h"
	
	#define LcdBackLightOn		St7529BackLightOn		//�����
	#define LcdBackLightOff		St7529BackLightOff		//����ر�
	#define LcdBackLightUp		St7529BackLightUp		//���ӱ�������
	#define LcdBackLightDown	St7529BackLightDown		//������������
	#define LcdBackLightSave	St7529WriteEeprom		//���汳�����
	
	#define LcdCtlerInit		St7529Init				//Һ����������ʼ��
	#define LcdSetPixel			St7529SetPixel			//�����ص�
	#define LcdGetPixel			St7529GetPixel			//��ȡ���ص���Ϣ
	#define LcdDrawHLine		St7529DrawHLine			//�����������
	#define LcdDrawVLine		St7529DrawVLine			//�����������
	#define LcdDrawRect			St7529DrawRect			//�����α߿�
	#define LcdFillRect			St7529FillRect			//�����������
	#define LcdDrawBitLine1BPP	St7529DrawBitLine1BPP	//����������Ϣɨ�赥����	
	#define LcdDrawBitMap		St7529DramBitMap		//������������ɨ��λͼ�ռ�
	
#endif

//����ֵ����

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
	#define KEY_COMB		0x7FBC	//"+"��"-"��"C"��ϼ�
	#define KEY_LCDC        0x7F7C	//"+"��"-"��"ENTER"��ϼ�

	#define KEY_REC			0x7EFF		//���鰴��
	
	#define KEY_INVALID		0x7FFF

#endif

#endif
