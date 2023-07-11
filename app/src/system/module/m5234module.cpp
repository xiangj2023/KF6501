#include "system.h"
#include "resm5234.h"

void M5234CsInit();
extern void M5234ModuleInit();


//初始化化除了基本系统外的其它片选,由于扩展以太网在引导时加载，所以需要在引导中初始化该片选
void M5234CsInit()
{
	//CS3，ADC片选，16位，0x30000000,寻址空间定义为1M
	MCF_CS_CSAR3 = MCF_CS_CSAR_BA(ADC_CS_BASE);
	MCF_CS_CSCR3 = (0
		| MCF_CS_CSCR_IWS(0)
		| MCF_CS_CSCR_AA
		| MCF_CS_CSCR_PS_16);
	MCF_CS_CSMR3 = MCF_CS_CSMR_BAM_1M | MCF_CS_CSMR_V;
	
	//CS5，扩展CAN网片选，8位，0x50000000，寻址空间定义为1M，3个初始插入等待
	MCF_CS_CSAR5 = MCF_CS_CSAR_BA(EX_CAN_CS_BASE);
	MCF_CS_CSCR5 = (0
		| MCF_CS_CSCR_IWS(3)
		| MCF_CS_CSCR_AA
		| MCF_CS_CSCR_PS_8);
	MCF_CS_CSMR5 = MCF_CS_CSMR_BAM_1M | MCF_CS_CSMR_V;
	
	//CS6，YX、YK、LCD、LED、KEY及其他备用片选，8位，0x60000000，寻址空间1M,5个初始插入等待
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


