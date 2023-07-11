#ifndef _SCDEFS_H_
#define _SCDEFS_H_

#include "resm5234.h"
#include "sysdefs.h"

//����ģ���и������Ͱ��������ã����л�����Ͱ���:ң��/ң�ذ���ݹ��ܸ���һ�顣
#define MAX_AI_BOARD_NUM	3
#define MAX_BO_BOARD_NUM	3
#define MAX_DI_BOARD_NUM	3

//����ɼ��������,ע����뱣֤�Ƕ�Ӧ���Ͷ˿ڿ�ȵ�������
#define MAX_AI_NUM			96
#define MAX_BO_NUM			72
#define MAX_DI_NUM			160

//���嵥��ң�ذ����ң�ص���Ŀ
#define MAX_BO_NUM_PER_BOARD	24	//ң������ռ��1·

//���嵥��ң�ذ���ڲ�ң��·��
#define MAX_IN_DI_NUM_PER_BOARD	12	

//���嵥��ң�Ű����·��
#define MAX_DI_NUM_PER_BOARD	34

//����������Ͷ˿ڿ��
#define AI_BOARD_WIDTH		16
#define BO_BOARD_WIDTH		8
#define DI_BOARD_WIDTH		8

//����ģ�����Ͷ��������ã��Լ��������Ӧ��ַ��·��
#if(MODULE_TYPE == MD_KV3523)

	//������ְ�����ø���
	#define AI_BOARD_NUM	1
	#define BO_BOARD_NUM	1
	#define DI_BOARD_NUM	1

	//�������AI��ĵ�ַ���ɼ�·��,����ʱ���������ö��壬û�еĶ���Ϊ0
	#define AI_BOARD1_ADDR		0x60000000
	#define AI_BOARD1_AINUM		12
	#define AI_BOARD2_ADDR		0
	#define AI_BOARD2_AINUM		0
	#define AI_BOARD3_ADDR		0
	#define AI_BOARD3_AINUM		0

	//�������BO���ַ��·��
	#define BO_BOARD1_ADDR		0x60002000
	#define BO_BOARD1_BONUM		24
	#define BO_BOARD2_ADDR		0
	#define BO_BOARD2_BONUM		0
	#define BO_BOARD3_ADDR		0
	#define BO_BOARD3_BONUM		0

	//�������DI���ַ��·��	
	#define DI_BOARD1_ADDR		BIA_CS_BASE
	#define DI_BOARD1_DINUM		34
	#define DI_BOARD2_ADDR		0
	#define DI_BOARD2_DINUM 	0
	#define DI_BOARD3_ADDR		0
	#define DI_BOARD3_DINUM 	0
	
#endif


#endif
