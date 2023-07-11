#ifndef _ST7529_H_
#define _ST7529_H_

#include "system.h"

#include "lcdcfg.h"

#if(LCD_CONTRLER == LCD_ST7529)


/*
	ST7529���������֧��160X255�ֱ�����ʾ��ÿ�������֧��32���Ҷȡ��ڱ�Ӧ���У��������Ͻ�Ϊ����
	ԭ�㡣���ڴ�ӳ���У������߱�ʶ(line)�����160�ߡ�����ÿ������32���Ҷȣ���Ҫ��5λ����������
	ʹ��8λ����ʱ��������2���ֽ�������3�������Ϣ(2B3P)��Ҳ������3���ֽ�����3�������Ϣ(3B3P)��
	��ʹ��16λ��ʱ��1���ּ��������3�������Ϣ����˶���x����3�������㶨��Ϊ1��(���Դ����֣�
	Ҳ���Դ��ֵ��󣬸������ö���)����y��������4��Ϊ1�顣��x��������С������λΪ������y������
	��С������λΪ�ߣ��ڱ�ϵͳ��Һ������240X160�������Ϊ80��X160�ߡ�

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

#define 	ST7529_COLUMN	3		//����1��������������Ϊx�������С������λ
#define		ST7529_BLOCK	4		//����1�������

//ST7529Һ��������������
#define      ExtIn       0x0030
#define      ExtOut      0x0031

#define 	BYTE_BIT_LEN	8	//����ÿ���ֽڵ�λ��

//����Ҷ���ʾ
#define		GRAY_BLACK	0x00		//��ɫ
#define 	GRAY_WHITE	0xf8		//��ɫ

//Ext=0                                     
#define      DISON       0x00AF		//��ʾ��
#define      DISOFF      0x00AE		//��ʾ�ر�
#define      DISNOR      0x00A6		//������ʾ
#define      DISINV      0x00A7		//���ԣ����޸���ʾ�ڴ�
#define      COMSCN      0x00BB		//COM�������ɨ�跽����
#define      DISCTRL     0x00CA		//��ʾ���ƣ����÷�Ƶֵ���������
#define      SLPIN       0x0095		//����˯��ģʽ
#define      SLPOUT      0x0094		//�˳�˯��ģʽ
#define      LASET       0x0075		//�ߵ�ַ���ã�������ʼ�ͽ�����
#define      CASET       0x0015		//����ַ���ã�������ʼ�ͽ�����
#define      DATSDR      0x00BC		//����ɨ�跽�����ã�������ԭ�㼰X��Y�ķ���
#define      RAMWR       0x005C		//�ڴ�д
#define      RAMRD       0x005D		//�ڴ��
#define      PTLIN       0x00A8		//���þֲ���ʾ����(��ʼ��ͽ�����)��������ֲ���ʾģʽ
#define      PTLOUT      0x00A9		//�˳��ֲ���ʾģʽ
#define      RMWIN       0x00E0		//�����-�޸�-дģʽ
#define      RMWOUT      0x00EE		//�˳���-�޸�-дģʽ
#define      ASCSET      0x00AA		//������������(����������ʽ���̶����򡢹�������)
#define      SCSTART     0x00AB		//������ʼ���ַ����
#define      OSCON       0x00D1		//�ڲ������
#define      OSCOFF      0x00D2		//�ڲ�����ر�
#define      PWRCTRL     0x0020		//��Դ��������
#define      VOLCTRL     0x0081		//��ѹ���ڿ���(ֱ�ӿ��Ƶ�ָ��ֵ)
#define      VOLUP       0x00D6		//������ѹ��(ÿ��Ϊ0.04v)
#define      VOLDOWN     0x00D7		//������ѹ��(ÿ��Ϊ0.04v)
#define      RESERVED    0x0082		//
#define      EPSRRD1     0x007C		//���Ĵ���1
#define      EPSRRD2     0x007D		//���Ĵ���2
#define      NOP         0x0025		//�ղ���
#define      EPINT       0x0007		//��ʼ������

//Ext=1
#define      Gray1Set    0x0020		//���ûҶ�
#define      Gray2Set    0x0021		//���ûҶ�
#define      ANASET      0x0032		//Ƶ�����õ�
#define      SWINT       0x0034		//�����ʼ��
#define      EPCTIN      0x00CD		//eeprom���ƶ�дģʽ
#define      EPCOUT      0x00CC		//eeprom��дģʽ�˳�
#define      EPMWR       0x00FC		//eepromд����
#define      EPMRD       0x00FD 	//eeprom������

//���岻�������Ŀ�������
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

//д����ӿ�
//#define St7529WriteCmd(cmd)		outportb(LCD_CMD_REG, cmd)

//д���ݽӿ�
//#define St7529WriteDat(dat)		outportb(LCD_DATA_REG, dat)

//�����ݽӿ�
#define St7529ReadDat()		inportb(LCD_DATA_REG)

//��ѹ�������
#define St7529BackLightUp()		outportb(LCD_CMD_REG, VOLUP)
#define St7529BackLightDown()	outportb(LCD_CMD_REG, VOLDOWN)

//�����������⿪�ؿ���
#define St7529BackLightOn()		outportb(LCD_BACK_LIGHT, 0x50)
#define St7529BackLightOff()	outportb(LCD_BACK_LIGHT, 0xa0)

//��������Һ����λ����
#define St7529HReset()			outportb(LCD_RESET_ADDR, 0xa0)

void St7529WriteCmd(BYTE cmd);
void St7529WriteDat(BYTE dat);
/*************************************************************************************
* ���ò������ݵ�ַ�ռ�														  
*************************************************************************************/
DWORD St7529SetAdd(WORD x1, WORD y1, WORD x2, WORD y2);

/*************************************************************************************
* ���ò������ݵ�ַ�ռ䣬Ȼ��������д��������														  
*************************************************************************************/
DWORD St7529SetWrite(WORD x1, WORD y1, WORD x2, WORD y2);

/*************************************************************************************
* ������ȡeeprom�еĵ�ѹ����ֵ														  
*************************************************************************************/
void St7529ReadEeprom(void);

/*************************************************************************************
* �������õĵ�ѹֵ���浽eeprom														  
*************************************************************************************/
void St7529WriteEeprom(void);

/*************************************************************************************
* Һ��������st7529��ʼ��														  
*************************************************************************************/
void St7529Init(void);

/*************************************************************************************
* ���㺯����ֻ���ǵ�ɫ���														  
*************************************************************************************/
void St7529SetPixel(int x, int y, LCD_PIXELINDEX pixel);

/*************************************************************************************
* ȡ���ص���Ϣ�������ǵ�ɫ���														  
*************************************************************************************/
LCD_PIXELINDEX St7529GetPixel(int x, int y);

/*************************************************************************************
* ��(����)���߲���														  
*************************************************************************************/
void St7529DrawHLine(int x0, int y0, int x1, BOOL isClear);

/*************************************************************************************
* ��(����)���߲���														  
*************************************************************************************/
void St7529DrawVLine(int x0, int y0, int y1, BOOL isClear);

/*************************************************************************************
* ���(����)���ο�														  
*************************************************************************************/
void St7529FillRect(int x0, int y0, int x1, int y1, BOOL isClear);

/*************************************************************************************
* �����α߿�														  
*************************************************************************************/
void St7529DrawRect(int x0, int y0, int x1, int y1);

/*************************************************************************************
* �������ص���Ϣ�����е��к���ɨ����ʾ														  
*************************************************************************************/
void St7529DrawBitLine1BPP(int x, int y, int xsize, BYTE *pData);

/*************************************************************************************
* ��ɫλͼ��ɨ����ʾ														  
*************************************************************************************/
void St7529DramBitMap(int x, int y, int xsize, int ysize, BYTE *pData);

void St7529Reset();

#endif

#endif
