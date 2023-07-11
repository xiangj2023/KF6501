#include "system.h"
#include "resm5234.h"

void M5234CsInit();
extern void M5234ModuleInit();


//��ʼ�������˻���ϵͳ�������Ƭѡ,������չ��̫��������ʱ���أ�������Ҫ�������г�ʼ����Ƭѡ
void M5234CsInit()
{
	//CS3��ADCƬѡ��16λ��0x30000000,Ѱַ�ռ䶨��Ϊ1M
	MCF_CS_CSAR3 = MCF_CS_CSAR_BA(ADC_CS_BASE);
	MCF_CS_CSCR3 = (0
		| MCF_CS_CSCR_IWS(0)
		| MCF_CS_CSCR_AA
		| MCF_CS_CSCR_PS_16);
	MCF_CS_CSMR3 = MCF_CS_CSMR_BAM_1M | MCF_CS_CSMR_V;
	
	//CS5����չCAN��Ƭѡ��8λ��0x50000000��Ѱַ�ռ䶨��Ϊ1M��3����ʼ����ȴ�
	MCF_CS_CSAR5 = MCF_CS_CSAR_BA(EX_CAN_CS_BASE);
	MCF_CS_CSCR5 = (0
		| MCF_CS_CSCR_IWS(3)
		| MCF_CS_CSCR_AA
		| MCF_CS_CSCR_PS_8);
	MCF_CS_CSMR5 = MCF_CS_CSMR_BAM_1M | MCF_CS_CSMR_V;
	
	//CS6��YX��YK��LCD��LED��KEY����������Ƭѡ��8λ��0x60000000��Ѱַ�ռ�1M,5����ʼ����ȴ�
	MCF_CS_CSAR6 = MCF_CS_CSAR_BA(EXTER_IF_BASE);
	MCF_CS_CSCR6 = (0
		| MCF_CS_CSCR_IWS(5)
		| MCF_CS_CSCR_AA
		| MCF_CS_CSCR_PS_8);
	MCF_CS_CSMR6 = MCF_CS_CSMR_BAM_1M | MCF_CS_CSMR_V;
}


void M5234ModuleInit()
{
	M5234CsInit();
}


