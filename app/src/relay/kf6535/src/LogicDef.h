/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Logicdef.h
* 
*   ���ģ�飺�߼�����
*
* 
*	���� �̵����궨�壬 �̵���ִ�����Ͷ���
* 
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-1-15		��ʼ����
* 
*************************************************************************/

#ifndef __Logicdef_H
#define __Logicdef_H

/**********************/

#define	 MAX_FUNCTYPE 	 12 	 //���������
#define	              FALSE1    0	//FALSE-FALSE
#define	               TRUE1    1	//TRUE-TRUE
#define	          RW_PI87U_A    2	//PI87U_A-A���ٶ���μ̵�����
#define	          RW_TR87U_A    3	//TR87U_A-A���ٶ϶����̵�����
#define	         RW_LPC87U_A    4	//LPC87U_A-A���ٶϱ���ѭ�����Ƽ̵�����
#define	          RW_PI87U_B    5	//PI87U_B-B���ٶ���μ̵�����
#define	          RW_TR87U_B    6	//TR87U_B-B���ٶ϶����̵�����
#define	         RW_LPC87U_B    7	//LPC87U_B-B���ٶϱ���ѭ�����Ƽ̵�����
#define	          RW_PI87U_C    8	//PI87U_C-C������ٶ���μ̵�����
#define	          RW_TR87U_C    9	//TR87U_C-C������ٶ϶����̵�����
#define	         RW_LPC87U_C   10	//LPC87U_C-C������ٶϱ���ѭ�����Ƽ̵�����
#define	          RW_PI87R_A   11	//PI87R_A-A����ʲ��μ̵�����
#define	          RW_TR87R_A   12	//TR87R_A-A����ʲ�����̵�����
#define	         RW_LPC87R_A   13	//LPC87R_A-A����ʲ����ѭ�����Ƽ̵�����
#define	          RW_PI87R_B   14	//PI87R_B-B����ʲ��μ̵�����
#define	          RW_TR87R_B   15	//TR87R_B-B����ʲ�����̵�����
#define	         RW_LPC87R_B   16	//LPC87R_B-B����ʲ����ѭ�����Ƽ̵�����
#define	          RW_PI87R_C   17	//PI87R_C-C����ʲ��μ̵�����
#define	          RW_TR87R_C   18	//TR87R_C-C����ʲ�����̵�����
#define	         RW_LPC87R_C   19	//LPC87R_C-C����ʲ����ѭ�����Ƽ̵�����
#define	        RW_LK68IDAH2   20	//LK68IDAH2-A��������г�������̵�����
#define	        RW_LK68IDAH5   21	//LK68IDAH5-A�������г�������̵�����
#define	       RW_LK68IDAMUL   22	//LK68IDAMUL-A�����ۺ�г�������̵�����
#define	        RW_LK68IRAH2   23	//LK68IRAH2-A�ƶ���������г�������̵�����
#define	        RW_LK68IDBH2   24	//LK68IDBH2-B��������г�������̵�����
#define	        RW_LK68IDBH5   25	//LK68IDBH5-B�������г�������̵�����
#define	       RW_LK68IDBMUL   26	//LK68IDBMUL-B�����ۺ�г�������̵�����
#define	        RW_LK68IRBH2   27	//LK68IRBH2-B�ƶ���������г�������̵�����
#define	        RW_LK68IDCH2   28	//LK68IDCH2-C��������г�������̵�����
#define	        RW_LK68IDCH5   29	//LK68IDCH5-C�������г�������̵�����
#define	       RW_LK68IDCMUL   30	//LK68IDCMUL-C�����ۺ�г�������̵�����
#define	        RW_LK68IRCH2   31	//LK68IRCH2-C�ƶ��������δ�г�������̵�����
#define	             RW_AR87   32	//AR87-����Խ�޸澯�̵�����
#define	          RW_PI50IH1   33	//PI50IH1-��ѹ����������μ̵�����
#define	          RW_TR50IH1   34	//TR50IH1-��ѹ�������ζ����̵�����
#define	         RW_LPC50IH1   35	//LPC50IH1-��ѹ�������α���ѭ�����Ƽ̵�����
#define	          RW_PI50IH2   36	//PI50IH2-��ѹ����������μ̵�����
#define	          RW_TR50IH2   37	//TR50IH2-��ѹ�������ζ����̵�����
#define	         RW_LPC50IH2   38	//LPC50IH2-��ѹ�������α���ѭ�����Ƽ̵�����
#define	             RW_47UH   39	//47UH-��ѹ�������ѹ�����̵�����
#define	             RW_PFHA   40	//PFHA-��ѹ��A�๦�ʷ�������̵�����
#define	             RW_PRHA   41	//PRHA-��ѹ��A�๦�ʷ�����̵�����
#define	             RW_PFHB   42	//PFHB-��ѹ��B�๦�ʷ�������̵�����
#define	             RW_PRHB   43	//PRHB-��ѹ��B�๦�ʷ�����̵�����
#define	             RW_PFHC   44	//PFHC-��ѹ��C�๦�ʷ�������̵�����
#define	             RW_PRHC   45	//PRHC-��ѹ��C�๦�ʷ�����̵�����
#define	          RW_PI50IM1   46	//PI50IM1-��ѹ����������μ̵�����
#define	          RW_TR50IM1   47	//TR50IM1-��ѹ�������ζ����̵�����
#define	         RW_LPC50IM1   48	//LPC50IM1-��ѹ�������α���ѭ�����Ƽ̵�����
#define	          RW_PI50IM2   49	//PI50IM2-��ѹ����������μ̵�����
#define	          RW_TR50IM2   50	//TR50IM2-��ѹ�������ζ����̵�����
#define	         RW_LPC50IM2   51	//LPC50IM2-��ѹ�������α���ѭ�����Ƽ̵�����
#define	          RW_PI50IL1   52	//PI50IL1-��ѹ����������μ̵�����
#define	          RW_TR50IL1   53	//TR50IL1-��ѹ�������ζ����̵�����
#define	         RW_LPC50IL1   54	//LPC50IL1-��ѹ�������α���ѭ�����Ƽ̵�����
#define	          RW_PI50IL2   55	//PI50IL2-��ѹ����������μ̵�����
#define	          RW_TR50IL2   56	//TR50IL2-��ѹ�������ζ����̵�����
#define	         RW_LPC50IL2   57	//LPC50IL2-��ѹ�������α���ѭ���ƻ��̵�����
#define	          RW_PI50OL1   58	//PI50OL1-�����ɢ����μ̵�����
#define	          RW_TR50OL1   59	//TR50OL1-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL1   60	//LPC50OL1-�����ɢ�α���ѭ�����Ƽ̵�����
#define	          RW_PI50OL2   61	//PI50OL2-�����ɢ����μ̵�����
#define	          RW_TR50OL2   62	//TR50OL2-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL2   63	//LPC50OL2-�����ɢ�α���ѭ�����Ƽ̵�����
#define	          RW_PI50OL3   64	//PI50OL3-�����ɢ����μ̵�����
#define	          RW_TR50OL3   65	//TR50OL3-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL3   66	//LPC50OL3-�����ɢ�α���ѭ�����Ƽ̵�����
#define	            RW_PI27U   67	//PI27U-ʧѹ������μ̵�����
#define	            RW_TR27U   68	//TR27U-ʧѹ���������̵�����
#define	           RW_LPC27U   69	//LPC27U-ʧѹ����ѭ�����Ƽ̵�����
#define	           RW_PI59UN   70	//PI59UN-�����ѹ������μ̵�����
#define	           RW_TR59UN   71	//TR59UN-�����ѹ���������̵�����
#define	          RW_LPC59UN   72	//LPC59UN-�����ѹ����ѭ�����Ƽ̵�����
#define	           RW_PI50IN   73	//PI50IN-�������������μ̵�����
#define	           RW_TR50IN   74	//TR50IN-����������������̵�����
#define	          RW_LPC50IN   75	//LPC50IN-�����������ѭ�����Ƽ̵�����
#define	          RW_TR50IJX   76	//TR50IJX-��϶�������������̵�����
#define	          RW_PI50IJX   77	//PI50IJX-��϶����������μ̵�����
#define	         RW_LPC50IJX   78	//LPC50IJX-��϶��������ѭ�����Ƽ̵�����
#define	             RW_74VT   79	//74VT-��ѹ��PT���߼̵�����
#define	           RW_PICKUP   80	//PICKUP-ģֵͻ���������̵�����
#define	             RW_59UA   81	//59UA-UA��ѹ�̵�����
#define	             RW_27UA   82	//27UA-UA��ѹ�̵�����
#define	             RW_59UB   83	//59UB-UB��ѹ�̵�����
#define	             RW_27UB   84	//27UB-UB��ѹ�̵�����
#define	             RW_59UC   85	//59UC-UC��ѹ�̵�����
#define	             RW_27UC   86	//27UC-UC��ѹ�̵�����
#define	             RW_59UN   87	//59UN-UN��ѹ�̵�����
#define	             RW_27UN   88	//27UN-UN��ѹ�̵�����
#define	             RW_50HA   89	//50HA-IHA�����̵�����
#define	             RW_37HA   90	//37HA-IHA�����̵�����
#define	             RW_50HB   91	//50HB-IHB�����̵�����
#define	             RW_37HB   92	//37HB-IHB�����̵�����
#define	             RW_50HC   93	//50HC-IHC�����̵�����
#define	             RW_37HC   94	//37HC-IHC�����̵�����
#define	             RW_50MA   95	//50MA-IMA�����̵�����
#define	             RW_37MA   96	//37MA-IMA�����̵�����
#define	             RW_50MB   97	//50MB-IMB�����̵�����
#define	             RW_37MB   98	//37MB-IMB�����̵�����
#define	             RW_50MC   99	//50MC-IMC�����̵�����
#define	             RW_37MC  100	//37MC-IMC�����̵�����
#define	             RW_50LA  101	//50LA-ILA�����̵�����
#define	             RW_37LA  102	//37LA-ILA�����̵�����
#define	             RW_50LB  103	//50LB-ILB�����̵�����
#define	             RW_37LB  104	//37LB-ILB�����̵�����
#define	             RW_50LC  105	//50LC-ILC�����̵�����
#define	             RW_37LC  106	//37LC-ILC�����̵�����
#define	             RW_50IN  107	//50IN-IN�����̵�����
#define	             RW_37IN  108	//37IN-IN�����̵�����
#define	            RW_50IJX  109	//50IJX-IJX�����̵�����
#define	            RW_37IJX  110	//37IJX-IJX�����̵�����
#define	          RW_ET11ERR  111	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR  112	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR  113	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR  114	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR  115	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR  116	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR  117	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR  118	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR  119	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR  120	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR  121	//COM1ERR-��������״̬�̵�����
#define	          RW_CAN1ERR  122	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR  123	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR  124	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR  125	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR  126	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK  127	//MAINTLINK-ά��״̬�̵�����
#define	        RW_LKPI87U_A  128	//LKPI87U_A-A���ٶ���α����̵�����
#define	       RW_LKRST87U_A  129	//LKRST87U_A-A���ٶϷ��ر����̵�����
#define	        RW_LKPI87U_B  130	//LKPI87U_B-B���ٶ���α����̵�����
#define	       RW_LKRST87U_B  131	//LKRST87U_B-B���ٶϷ��ر����̵�����
#define	        RW_LKPI87U_C  132	//LKPI87U_C-C���ٶ���α����̵�����
#define	       RW_LKRST87U_C  133	//LKRST87U_C-C���ٶϷ��ر����̵�����
#define	        RW_LKPI87R_A  134	//LKPI87R_A-A����ʲ��α����̵�����
#define	       RW_LKRST87R_A  135	//LKRST87R_A-A����ʲ���ر����̵�����
#define	        RW_LKPI87R_B  136	//LKPI87R_B-B����ʲ��α����̵�����
#define	       RW_LKRST87R_B  137	//LKRST87R_B-B����ʲ���ر����̵�����
#define	        RW_LKPI87R_C  138	//LKPI87R_C-C����ʲ��α����̵�����
#define	       RW_LKRST87R_C  139	//LKRST87R_C-C����ʲ���ر����̵�����
#define	        RW_LKPI50IH1  140	//LKPI50IH1-��ѹ����������α����̵�����
#define	       RW_LKRST50IH1  141	//LKRST50IH1-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IH2  142	//LKPI50IH2-��ѹ����������α����̵�����
#define	       RW_LKRST50IH2  143	//LKRST50IH2-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IM1  144	//LKPI50IM1-��ѹ����������α����̵�����
#define	       RW_LKRST50IM1  145	//LKRST50IM1-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IM2  146	//LKPI50IM2-��ѹ����������α����̵�����
#define	       RW_LKRST50IM2  147	//LKRST50IM2-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IL1  148	//LKPI50IL1-��ѹ����������α����̵�����
#define	       RW_LKRST50IL1  149	//LKRST50IL1-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IL2  150	//LKPI50IL2-��ѹ����������α����̵�����
#define	       RW_LKRST50IL2  151	//LKRST50IL2-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50OL1  152	//LKPI50OL1-�����ɢ����α����̵�����
#define	       RW_LKRST50OL1  153	//LKRST50OL1-�����ɢ�η��ر����̵�����
#define	        RW_LKPI50OL2  154	//LKPI50OL2-�����ɢ����α����̵�����
#define	       RW_LKRST50OL2  155	//LKRST50OL2-�����ɢ�η��ر����̵�����
#define	        RW_LKPI50OL3  156	//LKPI50OL3-�����ɢ����α����̵�����
#define	       RW_LKRST50OL3  157	//LKRST50OL3-�����ɢ�η��ر����̵�����
#define	          RW_LKPI27U  158	//LKPI27U-ʧѹ������α����̵�����
#define	         RW_LKRST27U  159	//LKRST27U-ʧѹ�������ر����̵�����
#define	         RW_LKPI59UN  160	//LKPI59UN-�����ѹ��α����̵�����
#define	        RW_LKRST59UN  161	//LKRST59UN-�����ѹ���ر����̵�����
#define	         RW_LKPI50IN  162	//LKPI50IN-���������α����̵�����
#define	        RW_LKRST50IN  163	//LKRST50IN-����������ر����̵�����
#define	        RW_LKPI50IJX  164	//LKPI50IJX-��϶������α����̵�����
#define	       RW_LKRST50IJX  165	//LKRST50IJX-��϶�������ر����̵�����
#define	              RW_REC  166	//REC-¼���̵�����
#define	             RW_69IN  167	//69IN-Զ�������м�̵�����
#define	             RW_52AH  168	//52AH-��ѹ���·����λ
#define	             RW_52BH  169	//52BH-��ѹ���·����λ
#define	             RW_52AM  170	//52AM-��ѹ���·����λ
#define	             RW_52BM  171	//52BM-��ѹ���·����λ
#define	             RW_52AL  172	//52AL-��ѹ���·����λ
#define	             RW_52BL  173	//52BL-��ѹ���·����λ
#define	               RW_M1  174	//M1-�м�̵�����M1
#define	               RW_M2  175	//M2-�м�̵�����M2
#define	               RW_M3  176	//M3-�м�̵�����M3
#define	               RW_M4  177	//M4-�м�̵�����M4
#define	               RW_M5  178	//M5-�м�̵�����M5
#define	               RW_M6  179	//M6-�м�̵�����M6
#define	               RW_M7  180	//M7-�м�̵�����M7
#define	               RW_M8  181	//M8-�м�̵�����M8
#define	               RW_M9  182	//M9-�м�̵�����M9
#define	              RW_M10  183	//M10-�м�̵�����M10
#define	              RW_M11  184	//M11-�м�̵�����M11
#define	              RW_M12  185	//M12-�м�̵�����M12
#define	              RW_M13  186	//M13-�м�̵�����M13
#define	              RW_M14  187	//M14-�м�̵�����M14
#define	              RW_M15  188	//M15-�м�̵�����M15
#define	              RW_M16  189	//M16-�м�̵�����M16
#define	              RW_M17  190	//M17-�м�̵�����M17
#define	              RW_M18  191	//M18-�м�̵�����M18
#define	              RW_M19  192	//M19-�м�̵�����M19
#define	              RW_M20  193	//M20-�м�̵�����M20
#define	              RW_M21  194	//M21-�м�̵�����M21
#define	              RW_M22  195	//M22-�м�̵�����M22
#define	              RW_M23  196	//M23-�м�̵�����M23
#define	              RW_M24  197	//M24-�м�̵�����M24
#define	              RW_M25  198	//M25-�м�̵�����M25
#define	              RW_M26  199	//M26-�м�̵�����M26
#define	              RW_M27  200	//M27-�м�̵�����M27
#define	              RW_M28  201	//M28-�м�̵�����M28
#define	              RW_M29  202	//M29-�м�̵�����M29
#define	              RW_M30  203	//M30-�м�̵�����M30
#define	              RW_M31  204	//M31-�м�̵�����M31
#define	              RW_M32  205	//M32-�м�̵�����M32
#define	              RW_M33  206	//M33-�м�̵�����M33
#define	              RW_M34  207	//M34-�м�̵�����M34
#define	              RW_M35  208	//M35-�м�̵�����M35
#define	              RW_M36  209	//M36-�м�̵�����M36
#define	              RW_M37  210	//M37-�м�̵�����M37
#define	              RW_M38  211	//M38-�м�̵�����M38
#define	              RW_M39  212	//M39-�м�̵�����M39
#define	              RW_M40  213	//M40-�м�̵�����M40
#define	              RW_M41  214	//M41-�м�̵�����M41
#define	              RW_M42  215	//M42-�м�̵�����M42
#define	              RW_M43  216	//M43-�м�̵�����M43
#define	              RW_M44  217	//M44-�м�̵�����M44
#define	              RW_M45  218	//M45-�м�̵�����M45
#define	              RW_M46  219	//M46-�м�̵�����M46
#define	              RW_M47  220	//M47-�м�̵�����M47
#define	              RW_M48  221	//M48-�м�̵�����M48
#define	              RW_M49  222	//M49-�м�̵�����M49
#define	              RW_M50  223	//M50-�м�̵�����M50
#define	              RW_M51  224	//M51-�м�̵�����M51
#define	              RW_M52  225	//M52-�м�̵�����M52
#define	              RW_M53  226	//M53-�м�̵�����M53
#define	              RW_M54  227	//M54-�м�̵�����M54
#define	              RW_M55  228	//M55-�м�̵�����M55
#define	              RW_M56  229	//M56-�м�̵�����M56
#define	              RW_M57  230	//M57-�м�̵�����M57
#define	              RW_M58  231	//M58-�м�̵�����M58
#define	              RW_M59  232	//M59-�м�̵�����M59
#define	              RW_M60  233	//M60-�м�̵�����M60
#define	              RW_M61  234	//M61-�м�̵�����M61
#define	              RW_M62  235	//M62-�м�̵�����M62
#define	              RW_M63  236	//M63-�м�̵�����M63
#define	              RW_M64  237	//M64-�м�̵�����M64
#define	               RW_H1  238	//H1-�Ա����м�̵�����1
#define	               RW_H2  239	//H2-�Ա����м�̵�����2
#define	               RW_H3  240	//H3-�Ա����м�̵�����3
#define	               RW_H4  241	//H4-�Ա����м�̵�����4
#define	               RW_H5  242	//H5-�Ա����м�̵�����5
#define	               RW_H6  243	//H6-�Ա����м�̵�����6
#define	               RW_H7  244	//H7-�Ա����м�̵�����7
#define	               RW_H8  245	//H8-�Ա����м�̵�����8
#define	               RW_H9  246	//H9-�Ա����м�̵�����9
#define	              RW_H10  247	//H10-�Ա����м�̵�����10
#define	              RW_H11  248	//H11-�Ա����м�̵�����11
#define	              RW_H12  249	//H12-�Ա����м�̵�����12
#define	              RW_H13  250	//H13-�Ա����м�̵�����13
#define	              RW_H14  251	//H14-�Ա����м�̵�����14
#define	              RW_H15  252	//H15-�Ա����м�̵�����15
#define	              RW_H16  253	//H16-�Ա����м�̵�����16
#define	              RW_H17  254	//H17-�Ա����м�̵�����17
#define	              RW_H18  255	//H18-�Ա����м�̵�����18
#define	              RW_H19  256	//H19-�Ա����м�̵�����19
#define	              RW_H20  257	//H20-�Ա����м�̵�����20
#define	              RW_H21  258	//H21-�Ա����м�̵�����21
#define	              RW_H22  259	//H22-�Ա����м�̵�����22
#define	              RW_H23  260	//H23-�Ա����м�̵�����23
#define	              RW_H24  261	//H24-�Ա����м�̵�����24
#define	              RW_H25  262	//H25-�Ա����м�̵�����25
#define	              RW_H26  263	//H26-�Ա����м�̵�����26
#define	              RW_H27  264	//H27-�Ա����м�̵�����27
#define	              RW_H28  265	//H28-�Ա����м�̵�����28
#define	              RW_H29  266	//H29-�Ա����м�̵�����29
#define	              RW_H30  267	//H30-�Ա����м�̵�����30
#define	              RW_H31  268	//H31-�Ա����м�̵�����31
#define	              RW_H32  269	//H32-�Ա����м�̵�����32
#define	            RW_52BFH  270	//52BFH-��ѹ���·����բʧ�̵ܼ�����
#define	            RW_52BFM  271	//52BFM-��ѹ���·����բʧ�̵ܼ�����
#define	            RW_52BFL  272	//52BFL-��ѹ���·����բʧ�̵ܼ�����
#define	         RW_52BF_ALL  273	//52BF_ALL-��·����բʧ�̵ܼ�����
#define	               RW_T1  274	//T1-T1
#define	               RW_T2  275	//T2-T2
#define	               RW_T3  276	//T3-T3
#define	               RW_T4  277	//T4-T4
#define	               RW_T5  278	//T5-T5
#define	               RW_T6  279	//T6-T6
#define	               RW_T7  280	//T7-T7
#define	               RW_T8  281	//T8-T8
#define	               RW_T9  282	//T9-T9
#define	              RW_T10  283	//T10-T10
#define	              RW_T11  284	//T11-T11
#define	              RW_T12  285	//T12-T12
#define	              RW_T13  286	//T13-T13
#define	              RW_T14  287	//T14-T14
#define	              RW_T15  288	//T15-T15
#define	              RW_T16  289	//T16-T16
#define	             RW_TCB1  290	//TCB1-TCB1
#define	             RW_TCB2  291	//TCB2-TCB2
#define	             RW_TCB3  292	//TCB3-TCB3
#define	             RW_TCB4  293	//TCB4-TCB4
#define	             RW_TCB5  294	//TCB5-TCB5
#define	             RW_TCB6  295	//TCB6-TCB6
#define	             RW_TCB7  296	//TCB7-TCB7
#define	             RW_TCB8  297	//TCB8-TCB8
#define	             RW_TSW1  298	//TSW1-TSW1
#define	             RW_TSW2  299	//TSW2-TSW2
#define	             RW_TSW3  300	//TSW3-TSW3
#define	             RW_TSW4  301	//TSW4-TSW4
#define	             RW_TSW5  302	//TSW5-TSW5
#define	             RW_TSW6  303	//TSW6-TSW6
#define	             RW_TSW7  304	//TSW7-TSW7
#define	             RW_TSW8  305	//TSW8-TSW8
#define	           RW_T52BFH  306	//T52BFH-T52BFH
#define	           RW_T52BFM  307	//T52BFM-T52BFM
#define	           RW_T52BFL  308	//T52BFL-T52BFL
#define	              RW_TR1  309	//TR1-��բ�̵���1
#define	              RW_TR2  310	//TR2-��բ�̵���2
#define	              RW_TR3  311	//TR3-��բ�̵���3
#define	              RW_VR1  312	//VR1-����̵�����1
#define	              RW_VR2  313	//VR2-����̵�����2
#define	              RW_VR3  314	//VR3-����̵�����3
#define	              RW_VR4  315	//VR4-����̵�����4
#define	              RW_VR5  316	//VR5-����̵�����5
#define	              RW_VR6  317	//VR6-����̵�����6
#define	              RW_VR7  318	//VR7-����̵�����7
#define	              RW_VR8  319	//VR8-����̵�����8
#define	              RW_VR9  320	//VR9-����̵�����9
#define	             RW_VR10  321	//VR10-����̵�����10
#define	             RW_VR11  322	//VR11-����̵�����11
#define	             RW_VR12  323	//VR12-����̵�����12
#define	             RW_VR13  324	//VR13-����̵�����13
#define	             RW_VR14  325	//VR14-����̵�����14
#define	             RW_VR15  326	//VR15-����̵�����15
#define	             RW_VR16  327	//VR16-����̵�����16
#define	             RW_VR17  328	//VR17-����̵�����17
#define	             RW_VR18  329	//VR18-����̵�����18
#define	             RW_VR19  330	//VR19-����̵�����19
#define	             RW_VR20  331	//VR20-����̵�����20
#define	             RW_VR21  332	//VR21-����̵�����21
#define	             RW_VR22  333	//VR22-����̵�����22
#define	             RW_VR23  334	//VR23-����̵�����23
#define	             RW_VR24  335	//VR24-����̵�����24
#define	             RW_VR25  336	//VR25-����̵�����25
#define	             RW_VR26  337	//VR26-����̵�����26
#define	             RW_VR27  338	//VR27-����̵�����27
#define	             RW_VR28  339	//VR28-����̵�����28
#define	             RW_VR29  340	//VR29-����̵�����29
#define	             RW_VR30  341	//VR30-����̵�����30
#define	             RW_VR31  342	//VR31-����̵�����31
#define	             RW_VR32  343	//VR32-����̵�����32
#define	              RW_IN1  344	//IN1-IN1
#define	              RW_IN2  345	//IN2-IN2
#define	              RW_IN3  346	//IN3-IN3
#define	              RW_IN4  347	//IN4-IN4
#define	              RW_IN5  348	//IN5-IN5
#define	              RW_IN6  349	//IN6-IN6
#define	              RW_IN7  350	//IN7-IN7
#define	              RW_IN8  351	//IN8-IN8
#define	              RW_IN9  352	//IN9-IN9
#define	             RW_IN10  353	//IN10-IN10
#define	             RW_IN11  354	//IN11-IN11
#define	             RW_IN12  355	//IN12-IN12
#define	             RW_IN13  356	//IN13-IN13
#define	             RW_IN14  357	//IN14-IN14
#define	             RW_IN15  358	//IN15-IN15
#define	             RW_IN16  359	//IN16-IN16
#define	             RW_IN17  360	//IN17-IN17
#define	             RW_IN18  361	//IN18-IN18
#define	             RW_IN19  362	//IN19-IN19
#define	             RW_IN20  363	//IN20-IN20
#define	             RW_IN21  364	//IN21-IN21
#define	             RW_IN22  365	//IN22-IN22
#define	             RW_IN23  366	//IN23-IN23
#define	             RW_IN24  367	//IN24-IN24
#define	             RW_IN25  368	//IN25-IN25
#define	             RW_IN26  369	//IN26-IN26
#define	             RW_IN27  370	//IN27-IN27
#define	             RW_IN28  371	//IN28-IN28
#define	             RW_IN29  372	//IN29-IN29
#define	             RW_IN30  373	//IN30-IN30
#define	             RW_IN31  374	//IN31-IN31
#define	             RW_IN32  375	//IN32-IN32
#define	             RW_IN33  376	//IN33-IN33
#define	             RW_IN34  377	//IN34-IN34
#define	             RW_IN35  378	//IN35-�źŸ���
#define	             RW_IN36  379	//IN36-��ѹ���·����λ
#define	             RW_IN37  380	//IN37-��ѹ���·����λ
#define	             RW_IN38  381	//IN38-��ѹ���·��ң��Ԥ��
#define	             RW_IN39  382	//IN39-��ѹ��ң��1Ԥ��
#define	             RW_IN40  383	//IN40-��ѹ��ң��2Ԥ��
#define	             RW_IN41  384	//IN41-��ѹ���·����λ
#define	             RW_IN42  385	//IN42-��ѹ���·����λ
#define	             RW_IN43  386	//IN43-��ѹ���·��ң��Ԥ��
#define	             RW_IN44  387	//IN44-��ѹ��ң��1Ԥ��
#define	             RW_IN45  388	//IN45-��ѹ��ң��2Ԥ��
#define	             RW_IN46  389	//IN46-��ѹ���·����λ
#define	             RW_IN47  390	//IN47-��ѹ���·����λ
#define	             RW_IN48  391	//IN48-��ѹ���·��ң��Ԥ��
#define	             RW_IN49  392	//IN49-��ѹ��ң��1Ԥ��
#define	             RW_IN50  393	//IN50-��ѹ��ң��2Ԥ��
#define	           RW_INVIN1  394	//INVIN1-VIN1
#define	           RW_INVIN2  395	//INVIN2-VIN2
#define	           RW_INVIN3  396	//INVIN3-VIN3
#define	           RW_INVIN4  397	//INVIN4-VIN4
#define	           RW_INVIN5  398	//INVIN5-VIN5
#define	           RW_INVIN6  399	//INVIN6-VIN6
#define	           RW_INVIN7  400	//INVIN7-VIN7
#define	           RW_INVIN8  401	//INVIN8-VIN8
#define	           RW_INVIN9  402	//INVIN9-VIN9
#define	          RW_INVIN10  403	//INVIN10-VIN10
#define	          RW_INVIN11  404	//INVIN11-VIN11
#define	          RW_INVIN12  405	//INVIN12-VIN12
#define	          RW_INVIN13  406	//INVIN13-VIN13
#define	          RW_INVIN14  407	//INVIN14-VIN14
#define	          RW_INVIN15  408	//INVIN15-VIN15
#define	          RW_INVIN16  409	//INVIN16-VIN16
#define	             RW_OUT1  410	//OUT1-����1�̵�����
#define	             RW_OUT2  411	//OUT2-����2�̵�����
#define	             RW_OUT3  412	//OUT3-����3�̵�����
#define	             RW_OUT4  413	//OUT4-����4�̵�����
#define	             RW_OUT5  414	//OUT5-����5�̵�����
#define	             RW_OUT6  415	//OUT6-����6�̵�����
#define	             RW_OUT7  416	//OUT7-����7�̵�����
#define	             RW_OUT8  417	//OUT8-����8�̵�����
#define	             RW_OUT9  418	//OUT9-����9�̵�����
#define	            RW_OUT10  419	//OUT10-����10�̵�����
#define	            RW_OUT11  420	//OUT11-����11�̵�����
#define	            RW_OUT12  421	//OUT12-����12�̵�����
#define	            RW_OUT13  422	//OUT13-����13�̵�����
#define	            RW_OUT14  423	//OUT14-����14�̵�����
#define	            RW_OUT15  424	//OUT15-����15�̵�����
#define	            RW_OUT16  425	//OUT16-����16�̵�����
#define	            RW_OUT17  426	//OUT17-����17�̵�����
#define	            RW_OUT18  427	//OUT18-����18�̵�����
#define	            RW_OUT19  428	//OUT19-����19�̵�����
#define	            RW_OUT20  429	//OUT20-����20�̵�����
#define	            RW_OUT21  430	//OUT21-����21�̵�����
#define	            RW_OUT22  431	//OUT22-����22�̵�����
#define	            RW_OUT23  432	//OUT23-����23�̵�����
#define	            RW_OUT24  433	//OUT24-����24�̵�����
#define	            RW_OUT25  434	//OUT25-����25�̵�����
#define	            RW_OUT26  435	//OUT26-����26�̵�����
#define	            RW_OUT27  436	//OUT27-����27�̵�����
#define	            RW_OUT28  437	//OUT28-����28�̵�����
#define	            RW_OUT29  438	//OUT29-����29�̵�����
#define	            RW_OUT30  439	//OUT30-����30�̵�����
#define	            RW_OUT31  440	//OUT31-����31�̵�����
#define	            RW_OUT32  441	//OUT32-����32�̵�����
#define	            RW_OUT33  442	//OUT33-����33�̵�����
#define	            RW_OUT34  443	//OUT34-����34�̵�����
#define	            RW_OUT35  444	//OUT35-����35�̵�����
#define	            RW_OUT36  445	//OUT36-����36�̵�����
#define	            RW_OUT37  446	//OUT37-����37�̵�����
#define	            RW_OUT38  447	//OUT38-����38�̵�����
#define	            RW_OUT39  448	//OUT39-����39�̵�����
#define	            RW_OUT40  449	//OUT40-����40�̵�����
#define	            RW_OUT41  450	//OUT41-����41�̵�����
#define	            RW_SL87U  451	//SL87U-�����ٶ���ѹ��̵�����
#define	            RW_SL87R  452	//SL87R-���ʲ��ѹ��̵�����
#define	           RW_SL50IH  453	//SL50IH-��ѹ�������ѹ��̵�����
#define	           RW_SL50IM  454	//SL50IM-��ѹ�������ѹ��̵�����
#define	           RW_SL50IL  455	//SL50IL-��ѹ�������ѹ��̵�����
#define	           RW_SL50OL  456	//SL50OL-��������ѹ��̵�����
#define	            RW_SL27U  457	//SL27U-ʧѹ������ѹ��̵�����
#define	           RW_SL59UN  458	//SL59UN-�����ѹ��ѹ��̵�����
#define	           RW_SL50IN  459	//SL50IN-���������ѹ��̵�����
#define	          RW_SL50IJX  460	//SL50IJX-��϶������ѹ��̵�����
#define	          RW_SLMAINT  461	//SLMAINT-����ѹ��
#define	           RW_SLBAK1  462	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  463	//SLBAK2-����ѹ��2
#define	           RW_F87U_A  464	//F87U_A-A���ٶ����ü̵�����
#define	           RW_F87U_B  465	//F87U_B-B������ٶ����ü̵�����
#define	           RW_F87U_C  466	//F87U_C-C������ٶ����ü̵�����
#define	           RW_F87R_A  467	//F87R_A-A����ʲ���ü̵�����
#define	           RW_F87R_B  468	//F87R_B-B����ʲ���ü̵�����
#define	           RW_F87R_C  469	//F87R_C-C����ʲ���ü̵�����
#define	       RW_FDCRES_87R  470	//FDCRES_87R-������ڷ����������ü̵�����
#define	         RW_F68IDH2M  471	//F68IDH2M-��������г�������������ü̵�����
#define	         RW_F68IDH2C  472	//F68IDH2C-��������г���ۺ���������ü̵�����
#define	         RW_F68IDH2I  473	//F68IDH2I-��������г������������ü̵�����
#define	         RW_F68IRH2I  474	//F68IRH2I-�ƶ�����г���������ü̵�����
#define	         RW_F68IDH5I  475	//F68IDH5I-�������г���������ü̵�����
#define	        RW_F68IDHMUL  476	//F68IDHMUL-�����ۺ�г���������ü̵�����
#define	            RW_FMBph  477	//FMBph-��ѹ���������ü̵�����
#define	            RW_FLBph  478	//FLBph-��ѹ���������ü̵�����
#define	           RW_F50IH1  479	//F50IH1-��ѹ�����������ü̵�����
#define	        RW_F50IH1_PD  480	//F50IH1_PD-�߹�����ι��ʷ������ü̵�����
#define	       RW_F50IH1_47U  481	//F50IH1_47U-�߹�����θ�ѹ�������ü̵�����
#define	           RW_F50IH2  482	//F50IH2-��ѹ�����������ü̵�����
#define	        RW_F50IH2_PD  483	//F50IH2_PD-�߹�����ι��ʷ������ü̵�����
#define	       RW_F50IH2_47U  484	//F50IH2_47U-�߹�����θ�ѹ�������ü̵�����
#define	           RW_F50IM1  485	//F50IM1-��ѹ�����������ü̵�����
#define	           RW_F50IM2  486	//F50IM2-��ѹ�����������ü̵�����
#define	           RW_F50IL1  487	//F50IL1-��ѹ�����������ü̵�����
#define	           RW_F50IL2  488	//F50IL2-��ѹ�����������ü̵�����
#define	           RW_F50OL1  489	//F50OL1-�����ɢ�����ü̵�����
#define	           RW_F50OL2  490	//F50OL2-�����ɢ�����ü̵�����
#define	           RW_F50OL3  491	//F50OL3-�����ɢ�����ü̵�����
#define	             RW_F27U  492	//F27U-ʧѹ�������ü̵�����
#define	         RW_F27U_50I  493	//F27U_50I-ʧѹ���������������ü̵�����
#define	            RW_F59UN  494	//F59UN-�����ѹ���ü̵�����
#define	            RW_F50IN  495	//F50IN-����������ü̵�����
#define	           RW_F50IJX  496	//F50IJX-��϶�������ü̵�����
#define	            RW_F74VT  497	//F74VT-PT�������ü̵�����
#define	            RW_F52BF  498	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  499	//FBAK1-��������1
#define	            RW_FBAK2  500	//FBAK2-��������2
#define	              RW_PW1  501	//PW1-����̵���1
#define	              RW_PW2  502	//PW2-����̵���2
#define	              RW_PW3  503	//PW3-����̵���3
#define	              RW_PW4  504	//PW4-����̵���4
#define	              RW_PW5  505	//PW5-����̵���5
#define	              RW_PW6  506	//PW6-����̵���6
#define	              RW_PW7  507	//PW7-����̵���7
#define	              RW_PW8  508	//PW8-����̵���8
#define	              RW_PW9  509	//PW9-����̵���9
#define	             RW_PW10  510	//PW10-����̵���10
#define	             RW_PW11  511	//PW11-����̵���11
#define	             RW_PW12  512	//PW12-����̵���12
#define	             RW_PW13  513	//PW13-����̵���13
#define	             RW_PW14  514	//PW14-����̵���14
#define	             RW_PW15  515	//PW15-����̵���15
#define	             RW_PW16  516	//PW16-����̵���16
#define	              RW_TO1  517	//TO1-��������ѹ���·��
#define	              RW_TO2  518	//TO2-��ѹ���·��ң�ط�
#define	              RW_TO3  519	//TO3-��ѹ���·��ң�غ�
#define	              RW_TO4  520	//TO4-��ѹ��ң��1��
#define	              RW_TO5  521	//TO5-��ѹ��ң��1��
#define	              RW_TO6  522	//TO6-��ѹ��ң��2��
#define	              RW_TO7  523	//TO7-��ѹ��ң��2��
#define	              RW_TO8  524	//TO8-��������ѹ���·��
#define	              RW_TO9  525	//TO9-��ѹ���·��ң�ط�
#define	             RW_TO10  526	//TO10-��ѹ���·��ң�غ�
#define	             RW_TO11  527	//TO11-��ѹ��ң��1��
#define	             RW_TO12  528	//TO12-��ѹ��ң��1��
#define	             RW_TO13  529	//TO13-��ѹ��ң��2��
#define	             RW_TO14  530	//TO14-��ѹ��ң��2��
#define	             RW_TO15  531	//TO15-��������ѹ���·��
#define	             RW_TO16  532	//TO16-��ѹ���·��ң�ط�
#define	             RW_TO17  533	//TO17-��ѹ���·��ң�غ�
#define	             RW_TO18  534	//TO18-��ѹ��ң��1��
#define	             RW_TO19  535	//TO19-��ѹ��ң��1��
#define	             RW_TO20  536	//TO20-��ѹ��ң��2��
#define	             RW_TO21  537	//TO21-��ѹ��ң��2��
#define	             RW_SIG1  538	//SIG1-�źż̵���1
#define	             RW_SIG2  539	//SIG2-�źż̵���2
#define	             RW_SIG3  540	//SIG3-�źż̵���3
#define	             RW_SIG4  541	//SIG4-�źż̵���4
#define	             RW_SIG5  542	//SIG5-�źż̵���5
#define	             RW_SIG6  543	//SIG6-�źż̵���6
#define	             RW_SIG7  544	//SIG7-�źż̵���7
#define	             RW_SIG8  545	//SIG8-�źż̵���8
#define	             RW_SIG9  546	//SIG9-�źż̵���9
#define	            RW_SIG10  547	//SIG10-�źż̵���10
#define	            RW_SIG11  548	//SIG11-�źż̵���11
#define	            RW_SIG12  549	//SIG12-�źż̵���12
#define	            RW_SIG13  550	//SIG13-�źż̵���13
#define	            RW_SIG14  551	//SIG14-�źż̵���14
#define	            RW_SIG15  552	//SIG15-�źż̵���15
#define	            RW_SIG16  553	//SIG16-�źż̵���16
#define	            RW_LED1G  554	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  555	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  556	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  557	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  558	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  559	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  560	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  561	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  562	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  563	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  564	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  565	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  566	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  567	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  568	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  569	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  570	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  571	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  572	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  573	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  574	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  575	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  576	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  577	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  578	//VIN1-VIN1
#define	             RW_VIN2  579	//VIN2-VIN2
#define	             RW_VIN3  580	//VIN3-VIN3
#define	             RW_VIN4  581	//VIN4-VIN4
#define	             RW_VIN5  582	//VIN5-VIN5
#define	             RW_VIN6  583	//VIN6-VIN6
#define	             RW_VIN7  584	//VIN7-VIN7
#define	             RW_VIN8  585	//VIN8-VIN8
#define	             RW_VIN9  586	//VIN9-VIN9
#define	            RW_VIN10  587	//VIN10-VIN10
#define	            RW_VIN11  588	//VIN11-VIN11
#define	            RW_VIN12  589	//VIN12-VIN12
#define	            RW_VIN13  590	//VIN13-VIN13
#define	            RW_VIN14  591	//VIN14-VIN14
#define	            RW_VIN15  592	//VIN15-VIN15
#define	            RW_VIN16  593	//VIN16-VIN16
#define	             RW_RCP1  594	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  595	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  596	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  597	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  598	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  599	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  600	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  601	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  602	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  603	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  604	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  605	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  606	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  607	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  608	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  609	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  610	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  611	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  612	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  613	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  614	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  615	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  616	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  617	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  618	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  619	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  620	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  621	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  622	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  623	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  624	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  625	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  626	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  627	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  628	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  629	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  630	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  631	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  632	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  633	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  634	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  635	//RCP42-ң��Ԥ�ü̵���42
#define	            RW_RCP43  636	//RCP43-ң��Ԥ�ü̵���43
#define	            RW_RCP44  637	//RCP44-ң��Ԥ�ü̵���44
#define	             RW_RCE1  638	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  639	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  640	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  641	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  642	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  643	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  644	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  645	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  646	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  647	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  648	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  649	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  650	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  651	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  652	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  653	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  654	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  655	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  656	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  657	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  658	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  659	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  660	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  661	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  662	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  663	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  664	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  665	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  666	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  667	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  668	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  669	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  670	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  671	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  672	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  673	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  674	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  675	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  676	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  677	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  678	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  679	//RCE42-ң��ִ�м̵���42
#define	            RW_RCE43  680	//RCE43-ң��ִ�м̵���43
#define	            RW_RCE44  681	//RCE44-ң��ִ�м̵���44
#define	             RW_RCV1  682	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  683	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  684	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  685	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  686	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  687	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  688	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  689	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  690	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  691	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  692	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  693	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  694	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  695	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  696	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  697	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  698	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  699	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  700	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  701	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  702	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  703	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  704	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  705	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  706	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  707	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  708	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  709	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  710	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  711	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  712	//RCV31-ѹ�ط�У�̵���31
#define	            RW_RCV32  713	//RCV32-ѹ�ط�У�̵���32
#define	            RW_RCV33  714	//RCV33-ѹ�ط�У�̵���33
#define	            RW_RCV34  715	//RCV34-ѹ�ط�У�̵���34
#define	            RW_RCV35  716	//RCV35-ѹ�ط�У�̵���35
#define	            RW_RCV36  717	//RCV36-ѹ�ط�У�̵���36
#define	            RW_RCV37  718	//RCV37-ѹ�ط�У�̵���37
#define	            RW_RCV38  719	//RCV38-ѹ�ط�У�̵���38
#define	            RW_RCV39  720	//RCV39-ѹ�ط�У�̵���39
#define	            RW_RCV40  721	//RCV40-ѹ�ط�У�̵���40
#define	            RW_RCV41  722	//RCV41-ѹ�ط�У�̵���41
#define	            RW_RCV42  723	//RCV42-ѹ�ط�У�̵���42
#define	            RW_RCV43  724	//RCV43-ѹ�ط�У�̵���43
#define	            RW_RCV44  725	//RCV44-ѹ�ط�У�̵���44
#define	              RW_GE1  726	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  727	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  728	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  729	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  730	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  731	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  732	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  733	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  734	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  735	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  736	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  737	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  738	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  739	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  740	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  741	//CONST8-����ʱ��̵���8
#define	              RW_TX1  742	//TX1-�����ͼ̵�����1
#define	              RW_TX2  743	//TX2-�����ͼ̵�����2
#define	              RW_TX3  744	//TX3-�����ͼ̵�����3
#define	              RW_TX4  745	//TX4-�����ͼ̵�����4
#define	              RW_TX5  746	//TX5-�����ͼ̵�����5
#define	              RW_TX6  747	//TX6-�����ͼ̵�����6
#define	              RW_TX7  748	//TX7-�����ͼ̵�����7
#define	              RW_TX8  749	//TX8-�����ͼ̵�����8
#define	              RW_TX9  750	//TX9-�����ͼ̵�����9
#define	             RW_TX10  751	//TX10-�����ͼ̵�����10
#define	             RW_TX11  752	//TX11-�����ͼ̵�����11
#define	             RW_TX12  753	//TX12-�����ͼ̵�����12
#define	             RW_TX13  754	//TX13-�����ͼ̵�����13
#define	             RW_TX14  755	//TX14-�����ͼ̵�����14
#define	             RW_TX15  756	//TX15-�����ͼ̵�����15
#define	             RW_TX16  757	//TX16-�����ͼ̵�����16
#define	              RW_RX1  758	//RX1-������ռ̵�����1
#define	              RW_RX2  759	//RX2-������ռ̵�����2
#define	              RW_RX3  760	//RX3-������ռ̵�����3
#define	              RW_RX4  761	//RX4-������ռ̵�����4
#define	              RW_RX5  762	//RX5-������ռ̵�����5
#define	              RW_RX6  763	//RX6-������ռ̵�����6
#define	              RW_RX7  764	//RX7-������ռ̵�����7
#define	              RW_RX8  765	//RX8-������ռ̵�����8
#define	              RW_RX9  766	//RX9-������ռ̵�����9
#define	             RW_RX10  767	//RX10-������ռ̵�����10
#define	             RW_RX11  768	//RX11-������ռ̵�����11
#define	             RW_RX12  769	//RX12-������ռ̵�����12
#define	             RW_RX13  770	//RX13-������ռ̵�����13
#define	             RW_RX14  771	//RX14-������ռ̵�����14
#define	             RW_RX15  772	//RX15-������ռ̵�����15
#define	             RW_RX16  773	//RX16-������ռ̵�����16
#define	             RW_CNT1  774	//CNT1-�����̵�����1
#define	             RW_CNT2  775	//CNT2-�����̵�����2
#define	             RW_CNT3  776	//CNT3-�����̵�����3
#define	             RW_CNT4  777	//CNT4-�����̵�����4
#define	             RW_CNT5  778	//CNT5-�����̵�����5
#define	             RW_CNT6  779	//CNT6-�����̵�����6
#define	             RW_CNT7  780	//CNT7-�����̵�����7
#define	             RW_CNT8  781	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			782		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			126	

//�м�̵�����
#define	MID_RELAY_WORD_START			128	
#define	MID_RELAY_WORD_NUM			110	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			238	
#define	KEEP_RELAY_WORD_NUM			36	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			274	
#define	TIME_RELAY_WORD_NUM			35	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			309	
#define	TRIP_RELAY_WORD_NUM			3	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			312	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			344	
#define	INPUT_RELAY_WORD_NUM			66	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			410	
#define	OUTPUT_RELAY_WORD_NUM			41	

//ѹ��̵���
#define	SW_RELAY_WORD_START			451	
#define	SW_RELAY_WORD_NUM			13	

//���ü̵���
#define	CFG_RELAY_WORD_START			464	
#define	CFG_RELAY_WORD_NUM			37	

//����̵���
#define	PULSE_RELAY_WORD_START			501	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			517	
#define	CTRLTEST_RELAY_WORD_NUM			21	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			538	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			554	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			578	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			594	
#define	RCP_RELAY_WORD_NUM			44	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			638	
#define	RCE_RELAY_WORD_NUM			44	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			682	
#define	RCV_RELAY_WORD_NUM			44	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			726	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			734	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			742	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			758	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			774	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			774	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			774	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			774	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			774	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			774	
#define	CNT_RELAY_WORD_NUM			8	


#endif