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
#define	           RW_PI87UA    2	//PI87UA-A���ٶϱ�����μ̵�����
#define	          RW_LPC87UA    3	//LPC87UA-A���ٶϱ���ѭ�����Ƽ̵�����
#define	           RW_PI87RA    4	//PI87RA-A����ʲ������μ̵�����
#define	          RW_LPC87RA    5	//LPC87RA-A����ʲ����ѭ�����Ƽ̵�����
#define	        RW_LK68IDH2A    6	//LK68IDH2A-����A����г�������̵�����
#define	        RW_LK68IRH2A    7	//LK68IRH2A-�ƶ�����A����г�������̵�����
#define	         RW_LK68IDHA    8	//LK68IDHA-����A�ۺ�г�������̵�����
#define	           RW_PI87UB    9	//PI87UB-B���ٶϱ�����μ̵�����
#define	          RW_LPC87UB   10	//LPC87UB-B���ٶϱ���ѭ�����Ƽ̵�����
#define	           RW_PI87RB   11	//PI87RB-B����ʲ������μ̵�����
#define	          RW_LPC87RB   12	//LPC87RB-B����ʲ����ѭ�����Ƽ̵�����
#define	        RW_LK68IDH2B   13	//LK68IDH2B-����B����г�������̵�����
#define	        RW_LK68IRH2B   14	//LK68IRH2B-�ƶ�����B����г�������̵�����
#define	         RW_LK68IDHB   15	//LK68IDHB-����B�ۺ�г�������̵�����
#define	           RW_PI87UC   16	//PI87UC-C���ٶϱ�����μ̵�����
#define	          RW_LPC87UC   17	//LPC87UC-C���ٶϱ���ѭ�����Ƽ̵�����
#define	           RW_PI87RC   18	//PI87RC-C����ʲ��μ̵�����
#define	          RW_LPC87RC   19	//LPC87RC-C����ʲѭ�����Ƽ̵�����
#define	        RW_LK68IDH2C   20	//LK68IDH2C-C���������г�������̵�����
#define	        RW_LK68IRH2C   21	//LK68IRH2C-C���ƶ���������г�������̵�����
#define	         RW_LK68IDHC   22	//LK68IDHC-����C�ۺ�г�������̵�����
#define	          RW_PI50IH1   23	//PI50IH1-��ѹ����������μ̵�����
#define	          RW_TR50IH1   24	//TR50IH1-��ѹ�������ζ����̵�����
#define	         RW_LPC50IH1   25	//LPC50IH1-��ѹ��������ѭ�����Ƽ̵�����
#define	          RW_PI50IH2   26	//PI50IH2-��ѹ����������μ̵�����
#define	          RW_TR50IH2   27	//TR50IH2-��ѹ�������ζ����̵�����
#define	         RW_LPC50IH2   28	//LPC50IH2-��ѹ��������ѭ�����Ƽ̵�����
#define	          RW_PI50IH3   29	//PI50IH3-��ѹ����������μ̵�����
#define	          RW_TR50IH3   30	//TR50IH3-��ѹ�������ζ����̵�����
#define	         RW_LPC50IH3   31	//LPC50IH3-��ѹ��������ѭ�����Ƽ̵�����
#define	          RW_PI50IL1   32	//PI50IL1-��ѹ����������μ̵�����
#define	          RW_TR50IL1   33	//TR50IL1-��ѹ�������ζ����̵�����
#define	         RW_LPC50IL1   34	//LPC50IL1-��ѹ��������ѭ�����Ƽ̵�����
#define	          RW_PI50IL2   35	//PI50IL2-��ѹ����������μ̵�����
#define	          RW_TR50IL2   36	//TR50IL2-��ѹ�������ζ����̵�����
#define	         RW_LPC50IL2   37	//LPC50IL2-��ѹ��������ѭ�����Ƽ̵�����
#define	             RW_47UH   38	//47UH-��ѹ�ิ�ϵ�ѹ�����̵�����
#define	             RW_47UL   39	//47UL-��ѹ�ิ�ϵ�ѹ�����̵�����
#define	          RW_PI50ISH   40	//PI50ISH-ĸ�䱣����μ̵�����
#define	          RW_TR50ISH   41	//TR50ISH-ĸ�䱣�������̵�����
#define	         RW_LPC50ISH   42	//LPC50ISH-ĸ�䱣��ѭ�����Ƽ̵�����
#define	           RW_PI50N1   43	//PI50N1-������������μ̵�����
#define	           RW_TR50N1   44	//TR50N1-���������ζ����̵�����
#define	          RW_LPC50N1   45	//LPC50N1-����������ѭ�����Ƽ̵�����
#define	           RW_PI50N2   46	//PI50N2-������������μ̵�����
#define	           RW_TR50N2   47	//TR50N2-���������ζ����̵�����
#define	          RW_LPC50N2   48	//LPC50N2-����������ѭ�����Ƽ̵�����
#define	           RW_AR49IA   49	//AR49IA-A�෴ʱ�޹����ɸ澯�̵�����
#define	           RW_TR49IA   50	//TR49IA-A�෴ʱ�޹����ɶ����̵�����
#define	           RW_AR49IB   51	//AR49IB-B�෴ʱ�޹����ɸ澯�̵�����
#define	           RW_TR49IB   52	//TR49IB-B�෴ʱ�޹����ɶ����̵�����
#define	           RW_AR49IC   53	//AR49IC-C�෴ʱ�޹����ɸ澯�̵�����
#define	           RW_TR49IC   54	//TR49IC-C�෴ʱ�޹����ɶ����̵�����
#define	           RW_PI59NH   55	//PI59NH-��ѹ�������ѹ��μ̵�����
#define	           RW_TR59NH   56	//TR59NH-��ѹ�������ѹ�����̵�����
#define	          RW_LPC59NH   57	//LPC59NH-��ѹ�������ѹѭ�����Ƽ̵�����
#define	           RW_PI59NL   58	//PI59NL-��ѹ�������ѹ��μ̵�����
#define	           RW_TR59NL   59	//TR59NL-��ѹ�������ѹ�����̵�����
#define	          RW_LPC59NL   60	//LPC59NL-��ѹ�������ѹѭ�����Ƽ̵�����
#define	          RW_PI50OL1   61	//PI50OL1-�����ɢ����μ̵�����
#define	          RW_TR50OL1   62	//TR50OL1-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL1   63	//LPC50OL1-�����ɢ��ѭ�����Ƽ̵�����
#define	          RW_PI50OL2   64	//PI50OL2-�����ɢ����μ̵�����
#define	          RW_TR50OL2   65	//TR50OL2-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL2   66	//LPC50OL2-�����ɢ��ѭ�����Ƽ̵�����
#define	          RW_PI50OL3   67	//PI50OL3-�����ɢ����μ̵�����
#define	          RW_TR50OL3   68	//TR50OL3-�����ɢ�ζ����̵�����
#define	         RW_LPC50OL3   69	//LPC50OL3-�����ɢ��ѭ�����Ƽ̵�����
#define	          RW_PI50IJX   70	//PI50IJX-��϶������μ̵�����
#define	          RW_TR50IJX   71	//TR50IJX-��϶���������̵�����
#define	         RW_LPC50IJX   72	//LPC50IJX-��϶����ѭ�����Ƽ̵�����
#define	            RW_PI27U   73	//PI27U-ʧѹ������μ̵�����
#define	            RW_TR27U   74	//TR27U-ʧѹ���������̵�����
#define	           RW_LPC27U   75	//LPC27U-ʧѹ����ѭ�����Ƽ̵�����
#define	              RW_PTH   76	//PTH-��ѹ��PT���߼̵�����
#define	              RW_PTL   77	//PTL-��ѹ��PT���߼̵�����
#define	            RW_59UHA   78	//59UHA-UA��ѹ�̵�����
#define	            RW_27UHA   79	//27UHA-UA��ѹ�̵�����
#define	            RW_59UHB   80	//59UHB-UB��ѹ�̵�����
#define	            RW_27UHB   81	//27UHB-UB��ѹ�̵�����
#define	            RW_59UHC   82	//59UHC-UC��ѹ�̵�����
#define	            RW_27UHC   83	//27UHC-UC��ѹ�̵�����
#define	            RW_59U0H   84	//59U0H-U0h��ѹ�̵�����
#define	            RW_27U0H   85	//27U0H-U0h��ѹ�̵�����
#define	            RW_59ULA   86	//59ULA-Ua��ѹ�̵�����
#define	            RW_27ULA   87	//27ULA-Ua��ѹ�̵�����
#define	            RW_59ULB   88	//59ULB-Ub��ѹ�̵�����
#define	            RW_27ULB   89	//27ULB-Ub��ѹ�̵�����
#define	            RW_59ULC   90	//59ULC-Uc��ѹ�̵�����
#define	            RW_27ULC   91	//27ULC-Uc��ѹ�̵�����
#define	            RW_59U0L   92	//59U0L-U0l��ѹ�̵�����
#define	            RW_27U0L   93	//27U0L-U0l��ѹ�̵�����
#define	            RW_50IHA   94	//50IHA-IA�����̵�����
#define	            RW_37IHA   95	//37IHA-IA�����̵�����
#define	            RW_50IHB   96	//50IHB-IB�����̵�����
#define	            RW_37IHB   97	//37IHB-IB�����̵�����
#define	            RW_50IHC   98	//50IHC-IC�����̵�����
#define	            RW_37IHC   99	//37IHC-IC�����̵�����
#define	            RW_50ILA  100	//50ILA-Ia�����̵�����
#define	            RW_37ILA  101	//37ILA-Ia�����̵�����
#define	            RW_50ILB  102	//50ILB-Ib�����̵�����
#define	            RW_37ILB  103	//37ILB-Ib�����̵�����
#define	            RW_50ILC  104	//50ILC-Ic�����̵�����
#define	            RW_37ILC  105	//37ILC-Ic�����̵�����
#define	             RW_50I0  106	//50I0-I0�����̵�����
#define	             RW_37I0  107	//37I0-I0�����̵�����
#define	            RW_50IJX  108	//50IJX-Ijx�����̵�����
#define	            RW_37IJX  109	//37IJX-Ijx�����̵�����
#define	             RW_PFHA  110	//PFHA-�߲�A�๦������̵�����
#define	             RW_PRHA  111	//PRHA-�߲�A�๦�ʷ���̵�����
#define	             RW_PFHB  112	//PFHB-�߲�B�๦������̵�����
#define	             RW_PRHB  113	//PRHB-�߲�B�๦�ʷ���̵�����
#define	             RW_PFHC  114	//PFHC-�߲�C�๦������̵�����
#define	             RW_PRHC  115	//PRHC-�߲�C�๦�ʷ���̵�����
#define	             RW_PFLA  116	//PFLA-�Ͳ�A�๦������̵�����
#define	             RW_PRLA  117	//PRLA-�Ͳ�A�๦�ʷ���̵�����
#define	             RW_PFLB  118	//PFLB-�Ͳ�B�๦������̵�����
#define	             RW_PRLB  119	//PRLB-�Ͳ�B�๦�ʷ���̵�����
#define	             RW_PFLC  120	//PFLC-�Ͳ�C�๦������̵�����
#define	             RW_PRLC  121	//PRLC-�Ͳ�C�๦�ʷ���̵�����
#define	           RW_PICKUP  122	//PICKUP-���������̵�����
#define	        RW_LK68IDAH2  123	//LK68IDAH2-����A����г�������̵�����
#define	        RW_LK68IDBH2  124	//LK68IDBH2-����B����г�������̵�����
#define	        RW_LK68IDCH2  125	//LK68IDCH2-����C����г�������̵�����
#define	         RW_LK68IDAH  126	//LK68IDAH-����A�ۺ�г�������̵�����
#define	         RW_LK68IDBH  127	//LK68IDBH-����B�ۺ�г�������̵�����
#define	         RW_LK68IDCH  128	//LK68IDCH-����C�ۺ�г�������̵�����
#define	        RW_LK68IRAH2  129	//LK68IRAH2-�ƶ�����A����г�������̵�����
#define	        RW_LK68IRBH2  130	//LK68IRBH2-�ƶ�����B����г�������̵�����
#define	        RW_LK68IRCH2  131	//LK68IRCH2-�ƶ�����C����г�������̵�����
#define	             RW_AR87  132	//AR87-����Խ�޸澯�̵�����
#define	           RW_TR87UA  133	//TR87UA-����A�ٶ϶����̵�����
#define	           RW_TR87RA  134	//TR87RA-���ʲA�����̵�����
#define	           RW_TR87UB  135	//TR87UB-����B�����̵�����
#define	           RW_TR87RB  136	//TR87RB-���ʲB�����̵�����
#define	           RW_TR87UC  137	//TR87UC-����C�����̵�����
#define	           RW_TR87RC  138	//TR87RC-���ʲC�����̵�����
#define	          RW_ET11ERR  139	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR  140	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR  141	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR  142	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR  143	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR  144	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR  145	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR  146	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR  147	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR  148	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR  149	//COM1ERR-��������1״̬�̵�����
#define	          RW_CAN1ERR  150	//CAN1ERR-CAN1����״̬�̵�����
#define	          RW_CAN2ERR  151	//CAN2ERR-CAN2����״̬�̵�����
#define	         RW_NULL1ERR  152	//NULL1ERR-������1״̬�̵�����
#define	         RW_NULL2ERR  153	//NULL2ERR-������2״̬�̵�����
#define	         RW_NULL3ERR  154	//NULL3ERR-������3״̬�̵�����
#define	        RW_MAINTLINK  155	//MAINTLINK-ά��״̬�̵�����
#define	          RW_PI50OLA  156	//PI50OLA-�����ɸ澯��μ̵�����
#define	          RW_TR50OLA  157	//TR50OLA-�����ɸ澯�����̵�����
#define	         RW_LPC50OLA  158	//LPC50OLA-�����ɸ澯ѭ�����Ƽ̵�����
#define	               RW_M1  159	//M1-�м�̵�����M1
#define	               RW_M2  160	//M2-�м�̵�����M2
#define	               RW_M3  161	//M3-�м�̵�����M3
#define	               RW_M4  162	//M4-�м�̵�����M4
#define	               RW_M5  163	//M5-�м�̵�����M5
#define	               RW_M6  164	//M6-�м�̵�����M6
#define	               RW_M7  165	//M7-�м�̵�����M7
#define	               RW_M8  166	//M8-�м�̵�����M8
#define	               RW_M9  167	//M9-�м�̵�����M9
#define	              RW_M10  168	//M10-�м�̵�����M10
#define	              RW_M11  169	//M11-�м�̵�����M11
#define	              RW_M12  170	//M12-�м�̵�����M12
#define	              RW_M13  171	//M13-�м�̵�����M13
#define	              RW_M14  172	//M14-�м�̵�����M14
#define	              RW_M15  173	//M15-�м�̵�����M15
#define	              RW_M16  174	//M16-�м�̵�����M16
#define	              RW_M17  175	//M17-�м�̵�����M17
#define	              RW_M18  176	//M18-�м�̵�����M18
#define	              RW_M19  177	//M19-�м�̵�����M19
#define	              RW_M20  178	//M20-�м�̵�����M20
#define	              RW_M21  179	//M21-�м�̵�����M21
#define	              RW_M22  180	//M22-�м�̵�����M22
#define	              RW_M23  181	//M23-�м�̵�����M23
#define	              RW_M24  182	//M24-�м�̵�����M24
#define	              RW_M25  183	//M25-�м�̵�����M25
#define	              RW_M26  184	//M26-�м�̵�����M26
#define	              RW_M27  185	//M27-�м�̵�����M27
#define	              RW_M28  186	//M28-�м�̵�����M28
#define	              RW_M29  187	//M29-�м�̵�����M29
#define	              RW_M30  188	//M30-�м�̵�����M30
#define	              RW_M31  189	//M31-�м�̵�����M31
#define	              RW_M32  190	//M32-�м�̵�����M32
#define	              RW_M33  191	//M33-�м�̵�����M33
#define	              RW_M34  192	//M34-�м�̵�����M34
#define	              RW_M35  193	//M35-�м�̵�����M35
#define	              RW_M36  194	//M36-�м�̵�����M36
#define	              RW_M37  195	//M37-�м�̵�����M37
#define	              RW_M38  196	//M38-�м�̵�����M38
#define	              RW_M39  197	//M39-�м�̵�����M39
#define	              RW_M40  198	//M40-�м�̵�����M40
#define	              RW_M41  199	//M41-�м�̵�����M41
#define	              RW_M42  200	//M42-�м�̵�����M42
#define	              RW_M43  201	//M43-�м�̵�����M43
#define	              RW_M44  202	//M44-�м�̵�����M44
#define	              RW_M45  203	//M45-�м�̵�����M45
#define	              RW_M46  204	//M46-�м�̵�����M46
#define	              RW_M47  205	//M47-�м�̵�����M47
#define	              RW_M48  206	//M48-�м�̵�����M48
#define	              RW_M49  207	//M49-�м�̵�����M49
#define	              RW_M50  208	//M50-�м�̵�����M50
#define	              RW_M51  209	//M51-�м�̵�����M51
#define	              RW_M52  210	//M52-�м�̵�����M52
#define	              RW_M53  211	//M53-�м�̵�����M53
#define	              RW_M54  212	//M54-�м�̵�����M54
#define	              RW_M55  213	//M55-�м�̵�����M55
#define	              RW_M56  214	//M56-�м�̵�����M56
#define	              RW_M57  215	//M57-�м�̵�����M57
#define	              RW_M58  216	//M58-�м�̵�����M58
#define	              RW_M59  217	//M59-�м�̵�����M59
#define	              RW_M60  218	//M60-�м�̵�����M60
#define	              RW_M61  219	//M61-�м�̵�����M61
#define	              RW_M62  220	//M62-�м�̵�����M62
#define	              RW_M63  221	//M63-�м�̵�����M63
#define	              RW_M64  222	//M64-�м�̵�����M64
#define	              RW_REC  223	//REC-¼���̵�����
#define	         RW_LKPI87UA  224	//LKPI87UA-��ٶ�A��α����̵�����
#define	        RW_LKRST87UA  225	//LKRST87UA-��ٶ�A���ر����̵�����
#define	         RW_LKPI87UB  226	//LKPI87UB-��ٶ�B��α����̵�����
#define	        RW_LKRST87UB  227	//LKRST87UB-��ٶ�B���ر����̵�����
#define	         RW_LKPI87UC  228	//LKPI87UC-��ٶ�C��α����̵�����
#define	        RW_LKRST87UC  229	//LKRST87UC-��ٶ�C���ر����̵�����
#define	         RW_LKPI87RA  230	//LKPI87RA-���ʲA��α����̵�����
#define	        RW_LKRST87RA  231	//LKRST87RA-���ʲA���ر����̵�����
#define	         RW_LKPI87RB  232	//LKPI87RB-���ʲB��α����̵�����
#define	        RW_LKRST87RB  233	//LKRST87RB-���ʲB���ر����̵�����
#define	         RW_LKPI87RC  234	//LKPI87RC-���ʲC��α����̵�����
#define	        RW_LKRST87RC  235	//LKRST87RC-���ʲC���ر����̵�����
#define	        RW_LKPI50IH1  236	//LKPI50IH1-��ѹ����������α����̵�����
#define	       RW_LKRST50IH1  237	//LKRST50IH1-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IH2  238	//LKPI50IH2-��ѹ����������α����̵�����
#define	       RW_LKRST50IH2  239	//LKRST50IH2-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IH3  240	//LKPI50IH3-��ѹ����������α����̵�����
#define	       RW_LKRST50IH3  241	//LKRST50IH3-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IL1  242	//LKPI50IL1-��ѹ����������α����̵�����
#define	       RW_LKRST50IL1  243	//LKRST50IL1-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50IL2  244	//LKPI50IL2-��ѹ����������α����̵�����
#define	       RW_LKRST50IL2  245	//LKRST50IL2-��ѹ�������η��ر����̵�����
#define	        RW_LKPI50ISH  246	//LKPI50ISH-ĸ�䱣����α����̵�����
#define	       RW_LKRST50ISH  247	//LKRST50ISH-ĸ�䱣�����ر����̵�����
#define	         RW_LKPI50N1  248	//LKPI50N1-���������α����̵�����
#define	        RW_LKRST50N1  249	//LKRST50N1-������η��ر����̵�����
#define	         RW_LKPI50N2  250	//LKPI50N2-���������α����̵�����
#define	        RW_LKRST50N2  251	//LKRST50N2-������η��ر����̵�����
#define	         RW_LKPI59NH  252	//LKPI59NH-��ѹ����ѹ��ѹ��α����̵�����
#define	        RW_LKRST59NH  253	//LKRST59NH-��ѹ����ѹ��ѹ���ر����̵�����
#define	         RW_LKPI59NL  254	//LKPI59NL-��ѹ����ѹ��ѹ��α����̵�����
#define	        RW_LKRST59NL  255	//LKRST59NL-��ѹ����ѹ��ѹ���ر����̵�����
#define	        RW_LKPI50OL1  256	//LKPI50OL1-�����ɢ����α����̵�����
#define	       RW_LKRST50OL1  257	//LKRST50OL1-�����ɢ�η��ر����̵�����
#define	        RW_LKPI50OL2  258	//LKPI50OL2-�����ɢ����α����̵�����
#define	       RW_LKRST50OL2  259	//LKRST50OL2-�����ɢ�η��ر����̵�����
#define	        RW_LKPI50OL3  260	//LKPI50OL3-�����ɢ����α����̵�����
#define	       RW_LKRST50OL3  261	//LKRST50OL3-�����ɢ�η��ر����̵�����
#define	        RW_LKPI50IJX  262	//LKPI50IJX-��϶������α����̵�����
#define	       RW_LKRST50IJX  263	//LKRST50IJX-��϶�������ر����̵�����
#define	          RW_LKPI27U  264	//LKPI27U-ʧѹ������α����̵�����
#define	         RW_LKRST27U  265	//LKRST27U-ʧѹ�������ر����̵�����
#define	             RW_69IN  266	//69IN-���ؿ��Ƽ̵�����
#define	            RW_52A_A  267	//52A_A-A��·����λ
#define	            RW_52B_A  268	//52B_A-A��·����λ
#define	            RW_52A_B  269	//52A_B-B��·����λ
#define	            RW_52B_B  270	//52B_B-B��·����λ
#define	             RW_LKCA  271	//LKCA-A��·����բ�����̵�����
#define	             RW_LKTA  272	//LKTA-A��·����բ�����̵�����
#define	             RW_LKCB  273	//LKCB-B��·����բ�����̵�����
#define	             RW_LKTB  274	//LKTB-B��·����բ�����̵�����
#define	        RW_LKPI50OLA  275	//LKPI50OLA-�����ɸ澯��α����̵�����
#define	       RW_LKRST50OLA  276	//LKRST50OLA-�����ɸ澯���ر����̵�����
#define	               RW_H1  277	//H1-�Ա����м�̵�����1
#define	               RW_H2  278	//H2-�Ա����м�̵�����2
#define	               RW_H3  279	//H3-�Ա����м�̵�����3
#define	               RW_H4  280	//H4-�Ա����м�̵�����4
#define	               RW_H5  281	//H5-�Ա����м�̵�����5
#define	               RW_H6  282	//H6-�Ա����м�̵�����6
#define	               RW_H7  283	//H7-�Ա����м�̵�����7
#define	               RW_H8  284	//H8-�Ա����м�̵�����8
#define	               RW_H9  285	//H9-�Ա����м�̵�����9
#define	              RW_H10  286	//H10-�Ա����м�̵�����10
#define	              RW_H11  287	//H11-�Ա����м�̵�����11
#define	              RW_H12  288	//H12-�Ա����м�̵�����12
#define	              RW_H13  289	//H13-�Ա����м�̵�����13
#define	              RW_H14  290	//H14-�Ա����м�̵�����14
#define	              RW_H15  291	//H15-�Ա����м�̵�����15
#define	              RW_H16  292	//H16-�Ա����м�̵�����16
#define	              RW_H17  293	//H17-�Ա����м�̵�����17
#define	              RW_H18  294	//H18-�Ա����м�̵�����18
#define	              RW_H19  295	//H19-�Ա����м�̵�����19
#define	              RW_H20  296	//H20-�Ա����м�̵�����20
#define	              RW_H21  297	//H21-�Ա����м�̵�����21
#define	              RW_H22  298	//H22-�Ա����м�̵�����22
#define	              RW_H23  299	//H23-�Ա����м�̵�����23
#define	              RW_H24  300	//H24-�Ա����м�̵�����24
#define	              RW_H25  301	//H25-�Ա����м�̵�����25
#define	              RW_H26  302	//H26-�Ա����м�̵�����26
#define	              RW_H27  303	//H27-�Ա����м�̵�����27
#define	              RW_H28  304	//H28-�Ա����м�̵�����28
#define	              RW_H29  305	//H29-�Ա����м�̵�����29
#define	              RW_H30  306	//H30-�Ա����м�̵�����30
#define	              RW_H31  307	//H31-�Ա����м�̵�����31
#define	              RW_H32  308	//H32-�Ա����м�̵�����31
#define	            RW_52BFA  309	//52BFA-A��·����բʧ�̵ܼ�����
#define	            RW_52BFB  310	//52BFB-A��·����բʧ�̵ܼ�����
#define	               RW_T1  311	//T1-T1
#define	               RW_T2  312	//T2-T2
#define	               RW_T3  313	//T3-T3
#define	               RW_T4  314	//T4-T4
#define	               RW_T5  315	//T5-T5
#define	               RW_T6  316	//T6-T6
#define	               RW_T7  317	//T7-T7
#define	               RW_T8  318	//T8-T8
#define	               RW_T9  319	//T9-T9
#define	              RW_T10  320	//T10-T10
#define	              RW_T11  321	//T11-T11
#define	              RW_T12  322	//T12-T12
#define	              RW_T13  323	//T13-T13
#define	              RW_T14  324	//T14-T14
#define	              RW_T15  325	//T15-T15
#define	              RW_T16  326	//T16-T16
#define	             RW_TSW1  327	//TSW1-TSW1
#define	             RW_TSW2  328	//TSW2-TSW2
#define	             RW_TSW3  329	//TSW3-TSW3
#define	             RW_TSW4  330	//TSW4-TSW4
#define	             RW_TSW5  331	//TSW5-TSW5
#define	             RW_TSW6  332	//TSW6-TSW6
#define	             RW_TSW7  333	//TSW7-TSW7
#define	             RW_TSW8  334	//TSW8-TSW8
#define	             RW_TCB1  335	//TCB1-TCB1
#define	             RW_TCB2  336	//TCB2-TCB2
#define	             RW_TCB3  337	//TCB3-TCB3
#define	             RW_TCB4  338	//TCB4-TCB4
#define	             RW_TCB5  339	//TCB5-TCB5
#define	             RW_TCB6  340	//TCB6-TCB6
#define	             RW_TCB7  341	//TCB7-TCB7
#define	             RW_TCB8  342	//TCB8-TCB8
#define	              RW_TSH  343	//TSH-TSH
#define	            RW_T52BF  344	//T52BF-T52BF
#define	              RW_TR1  345	//TR1-��բ�̵���1
#define	              RW_TR2  346	//TR2-��բ�̵���2
#define	              RW_TR3  347	//TR3-��բ�̵���3
#define	              RW_TR4  348	//TR4-��բ�̵���4
#define	              RW_VR1  349	//VR1-���Լ̵���
#define	              RW_VR2  350	//VR2-����̵���2
#define	              RW_VR3  351	//VR3-VR3
#define	              RW_VR4  352	//VR4-VR4
#define	              RW_VR5  353	//VR5-VR5
#define	              RW_VR6  354	//VR6-VR6
#define	              RW_VR7  355	//VR7-VR7
#define	              RW_VR8  356	//VR8-VR8
#define	              RW_VR9  357	//VR9-VR9
#define	             RW_VR10  358	//VR10-VR10
#define	             RW_VR11  359	//VR11-VR11
#define	             RW_VR12  360	//VR12-VR12
#define	             RW_VR13  361	//VR13-VR13
#define	             RW_VR14  362	//VR14-VR14
#define	             RW_VR15  363	//VR15-VR15
#define	             RW_VR16  364	//VR16-VR16
#define	             RW_VR17  365	//VR17-VR17
#define	             RW_VR18  366	//VR18-VR18
#define	             RW_VR19  367	//VR19-VR19
#define	             RW_VR20  368	//VR20-VR20
#define	             RW_VR21  369	//VR21-VR21
#define	             RW_VR22  370	//VR22-VR22
#define	             RW_VR23  371	//VR23-VR23
#define	             RW_VR24  372	//VR24-VR24
#define	             RW_VR25  373	//VR25-VR25
#define	             RW_VR26  374	//VR26-VR26
#define	             RW_VR27  375	//VR27-VR27
#define	             RW_VR28  376	//VR28-VR28
#define	             RW_VR29  377	//VR29-VR29
#define	             RW_VR30  378	//VR30-VR30
#define	             RW_VR31  379	//VR31-VR31
#define	             RW_VR32  380	//VR32-VR32
#define	              RW_IN1  381	//IN1-IN1
#define	              RW_IN2  382	//IN2-IN2
#define	              RW_IN3  383	//IN3-IN3
#define	              RW_IN4  384	//IN4-IN4
#define	              RW_IN5  385	//IN5-IN5
#define	              RW_IN6  386	//IN6-IN6
#define	              RW_IN7  387	//IN7-IN7
#define	              RW_IN8  388	//IN8-IN8
#define	              RW_IN9  389	//IN9-IN9
#define	             RW_IN10  390	//IN10-IN10
#define	             RW_IN11  391	//IN11-IN11
#define	             RW_IN12  392	//IN12-IN12
#define	             RW_IN13  393	//IN13-IN13
#define	             RW_IN14  394	//IN14-IN14
#define	             RW_IN15  395	//IN15-IN15
#define	             RW_IN16  396	//IN16-IN16
#define	             RW_IN17  397	//IN17-IN17
#define	             RW_IN18  398	//IN18-IN18
#define	             RW_IN19  399	//IN19-IN19
#define	             RW_IN20  400	//IN20-IN20
#define	             RW_IN21  401	//IN21-IN21
#define	             RW_IN22  402	//IN22-IN22
#define	             RW_IN23  403	//IN23-IN23
#define	             RW_IN24  404	//IN24-IN24
#define	             RW_IN25  405	//IN25-IN25
#define	             RW_IN26  406	//IN26-IN26
#define	             RW_IN27  407	//IN27-IN27
#define	             RW_IN28  408	//IN28-IN28
#define	             RW_IN29  409	//IN29-IN29
#define	             RW_IN30  410	//IN30-IN30
#define	             RW_IN31  411	//IN31-IN31
#define	             RW_IN32  412	//IN32-IN32
#define	             RW_IN33  413	//IN33-IN33
#define	             RW_IN34  414	//IN34-IN34
#define	             RW_IN35  415	//IN35-�źŸ���
#define	             RW_IN36  416	//IN36-���ڰ�1��բλ��
#define	             RW_IN37  417	//IN37-���ڰ�1��բλ��
#define	             RW_IN38  418	//IN38-���ڰ�1ң��Ԥ��
#define	             RW_IN39  419	//IN39-���ڰ�1ң��1Ԥ��
#define	             RW_IN40  420	//IN40-���ڰ�1ң��2Ԥ��
#define	             RW_IN41  421	//IN41-���ڰ�2��բλ��
#define	             RW_IN42  422	//IN42-���ڰ�2��բλ��
#define	             RW_IN43  423	//IN43-���ڰ�2ң��Ԥ��
#define	             RW_IN44  424	//IN44-���ڰ�2ң��1Ԥ��
#define	             RW_IN45  425	//IN45-���ڰ�2ң��2Ԥ��
#define	             RW_IN46  426	//IN46-����˹
#define	             RW_IN47  427	//IN47-ѹ���ͷ�
#define	             RW_IN48  428	//IN48-������բ
#define	             RW_IN49  429	//IN49-����˹
#define	             RW_IN50  430	//IN50-��λ�쳣
#define	             RW_IN51  431	//IN51-�¶ȹ���
#define	             RW_IN52  432	//IN52-ʧ��澯
#define	           RW_INVIN1  433	//INVIN1-VIN1
#define	           RW_INVIN2  434	//INVIN2-VIN2
#define	           RW_INVIN3  435	//INVIN3-VIN3
#define	           RW_INVIN4  436	//INVIN4-VIN4
#define	           RW_INVIN5  437	//INVIN5-VIN5
#define	           RW_INVIN6  438	//INVIN6-VIN6
#define	           RW_INVIN7  439	//INVIN7-VIN7
#define	           RW_INVIN8  440	//INVIN8-VIN8
#define	           RW_INVIN9  441	//INVIN9-VIN9
#define	          RW_INVIN10  442	//INVIN10-VIN10
#define	          RW_INVIN11  443	//INVIN11-VIN11
#define	          RW_INVIN12  444	//INVIN12-VIN12
#define	          RW_INVIN13  445	//INVIN13-VIN13
#define	          RW_INVIN14  446	//INVIN14-VIN14
#define	          RW_INVIN15  447	//INVIN15-VIN15
#define	          RW_INVIN16  448	//INVIN16-VIN16
#define	             RW_OUT1  449	//OUT1-����1�̵�����
#define	             RW_OUT2  450	//OUT2-����2�̵�����
#define	             RW_OUT3  451	//OUT3-����3�̵�����
#define	             RW_OUT4  452	//OUT4-����4�̵�����
#define	             RW_OUT5  453	//OUT5-����5�̵�����
#define	             RW_OUT6  454	//OUT6-����6�̵�����
#define	             RW_OUT7  455	//OUT7-����7�̵�����
#define	             RW_OUT8  456	//OUT8-����8�̵�����
#define	             RW_OUT9  457	//OUT9-����9�̵�����
#define	            RW_OUT10  458	//OUT10-����10�̵�����
#define	            RW_OUT11  459	//OUT11-����11�̵�����
#define	            RW_OUT12  460	//OUT12-����12�̵�����
#define	            RW_OUT13  461	//OUT13-����13�̵�����
#define	            RW_OUT14  462	//OUT14-����14�̵�����
#define	            RW_OUT15  463	//OUT15-����15�̵�����
#define	            RW_OUT16  464	//OUT16-����16�̵�����
#define	            RW_OUT17  465	//OUT17-����17�̵�����
#define	            RW_OUT18  466	//OUT18-����18�̵�����
#define	            RW_OUT19  467	//OUT19-����19�̵�����
#define	            RW_OUT20  468	//OUT20-����20�̵�����
#define	            RW_OUT21  469	//OUT21-����21�̵�����
#define	            RW_OUT22  470	//OUT22-����22�̵�����
#define	            RW_OUT23  471	//OUT23-����23�̵�����
#define	            RW_OUT24  472	//OUT24-����24�̵�����
#define	            RW_OUT25  473	//OUT25-����25�̵�����
#define	            RW_OUT26  474	//OUT26-����26�̵�����
#define	            RW_OUT27  475	//OUT27-����27�̵�����
#define	            RW_OUT28  476	//OUT28-����28�̵�����
#define	            RW_OUT29  477	//OUT29-����29�̵�����
#define	            RW_OUT30  478	//OUT30-����30�̵�����
#define	            RW_OUT31  479	//OUT31-����31�̵�����
#define	            RW_OUT32  480	//OUT32-����32�̵�����
#define	            RW_OUT33  481	//OUT33-����33�̵�����
#define	            RW_SL87U  482	//SL87U-��ٶ�ѹ��̵�����
#define	            RW_SL87R  483	//SL87R-���ʲѹ��̵�����
#define	          RW_SL50I_H  484	//SL50I_H-��ѹ�����ѹ��̵�����
#define	          RW_SL50I_L  485	//SL50I_L-��ѹ�����ѹ��̵�����
#define	         RW_SL50I_SH  486	//SL50I_SH-ĸ�䱣��ѹ��̵�����
#define	            RW_SL50N  487	//SL50N-�������ѹ��̵�����
#define	            RW_SL59N  488	//SL59N-�����ѹѹ��̵�����
#define	           RW_SL50OL  489	//SL50OL-������ѹ��̵�����
#define	         RW_SL50I_JX  490	//SL50I_JX-��϶����ѹ��̵�����
#define	            RW_SL49I  491	//SL49I-��ʱ�޹�����ѹ��̵�����
#define	            RW_SL27U  492	//SL27U-ʧѹ����ѹ��̵�����
#define	          RW_SLMAINT  493	//SLMAINT-����ѹ��̵�����
#define	           RW_SLBAK1  494	//SLBAK1-����1ѹ��̵�����
#define	           RW_SLBAK2  495	//SLBAK2-����2ѹ��̵�����
#define	             RW_F87U  496	//F87U-��ٶϱ���
#define	             RW_F87R  497	//F87R-���ʲ����
#define	     RW_F68IDH2MAXLK  498	//F68IDH2MAXLK-����г����������
#define	     RW_F68IDH2MULLK  499	//F68IDH2MULLK-����г���ۺ������
#define	     RW_F68IDH2SPLLK  500	//F68IDH2SPLLK-����г���������
#define	        RW_F68IRH2LK  501	//F68IRH2LK-�ƶ���������г������
#define	       RW_F68IDMULLK  502	//F68IDMULLK-�ۺ�г������
#define	       RW_FHCT1CLOCK  503	//FHCT1CLOCK-��ѹ��CT1��У��
#define	      RW_FHCT11CLOCK  504	//FHCT11CLOCK-��ѹ��CT11��У��
#define	       RW_FLCT1CLOCK  505	//FLCT1CLOCK-��ѹ��CT1��У��
#define	      RW_FLCT11CLOCK  506	//FLCT11CLOCK-��ѹ��CT11��У��
#define	             RW_FHAB  507	//FHAB-��ѹ������
#define	          RW_F50I_H1  508	//F50I_H1-��ѹ��������
#define	       RW_F50I_H1_PD  509	//F50I_H1_PD-��ѹ�������ι��ʷ������
#define	      RW_F50I_H1_47U  510	//F50I_H1_47U-��ѹ�������θ�ѹ����
#define	          RW_F50I_H2  511	//F50I_H2-��ѹ��������
#define	       RW_F50I_H2_PD  512	//F50I_H2_PD-��ѹ�������ι��ʷ���
#define	      RW_F50I_H2_47U  513	//F50I_H2_47U-��ѹ�������θ�ѹ����
#define	          RW_F50I_H3  514	//F50I_H3-��ѹ��������
#define	       RW_F50I_H3_PD  515	//F50I_H3_PD-��ѹ�������ι��ʷ���
#define	      RW_F50I_H3_47U  516	//F50I_H3_47U-��ѹ�������θ�ѹ����
#define	          RW_F50I_L1  517	//F50I_L1-��ѹ��������
#define	       RW_F50I_L1_PD  518	//F50I_L1_PD-��ѹ�������ι��ʷ���
#define	      RW_F50I_L1_47U  519	//F50I_L1_47U-��ѹ�������θ�ѹ����
#define	          RW_F50I_L2  520	//F50I_L2-��ѹ��������
#define	       RW_F50I_L2_PD  521	//F50I_L2_PD-��ѹ�������ι��ʷ���
#define	      RW_F50I_L2_47U  522	//F50I_L2_47U-��ѹ�������θ�ѹ����
#define	          RW_F50I_SH  523	//F50I_SH-ĸ�߳�籣��
#define	             RW_F49I  524	//F49I-��ʱ�޹����ɱ���
#define	          RW_F50I_N1  525	//F50I_N1-����������
#define	          RW_F50I_N2  526	//F50I_N2-����������
#define	          RW_F59U_NH  527	//F59U_NH-��ѹ�������ѹ����
#define	          RW_F59U_NL  528	//F59U_NL-��ѹ�������ѹ�澯
#define	          RW_F50OL_1  529	//F50OL_1-�����ɢ��
#define	          RW_F50OL_2  530	//F50OL_2-�����ɢ��
#define	          RW_F50OL_3  531	//F50OL_3-�����ɢ��
#define	          RW_F50OL_A  532	//F50OL_A-�����ɸ澯
#define	          RW_F50I_JX  533	//F50I_JX-��϶��������
#define	             RW_F27U  534	//F27U-����ʧѹ����
#define	         RW_F27U_50I  535	//F27U_50I-ʧѹ������������
#define	            RW_FPTCK  536	//FPTCK-PT���߼��
#define	            RW_F52BF  537	//F52BF-ʧ�鱣��
#define	            RW_FBAK1  538	//FBAK1-��������1
#define	            RW_FBAK2  539	//FBAK2-��������2
#define	              RW_PW1  540	//PW1-�ƹ�1
#define	              RW_PW2  541	//PW2-�ƹ�2
#define	              RW_PW3  542	//PW3-�ƹ�3
#define	              RW_PW4  543	//PW4-�ƹ�4
#define	              RW_PW5  544	//PW5-�ƹ�5
#define	              RW_PW6  545	//PW6-�ƹ�6
#define	              RW_PW7  546	//PW7-�ƹ�7
#define	              RW_PW8  547	//PW8-�ƹ�8
#define	              RW_PW9  548	//PW9-�ƹ�9
#define	             RW_PW10  549	//PW10-�ƹ�10
#define	             RW_PW11  550	//PW11-�ƹ�11
#define	             RW_PW12  551	//PW12-�ƹ�12
#define	             RW_PW13  552	//PW13-�ƹ�13
#define	             RW_PW14  553	//PW14-�ƹ�14
#define	             RW_PW15  554	//PW15-�ƹ�15
#define	             RW_PW16  555	//PW16-�ƹ�16
#define	              RW_TO1  556	//TO1-TO1
#define	              RW_TO2  557	//TO2-TO2
#define	              RW_TO3  558	//TO3-TO3
#define	              RW_TO4  559	//TO4-TO4
#define	              RW_TO5  560	//TO5-TO5
#define	              RW_TO6  561	//TO6-TO6
#define	              RW_TO7  562	//TO7-TO7
#define	              RW_TO8  563	//TO8-TO8
#define	              RW_TO9  564	//TO9-TO9
#define	             RW_TO10  565	//TO10-TO10
#define	             RW_TO11  566	//TO11-TO11
#define	             RW_TO12  567	//TO12-TO12
#define	             RW_TO13  568	//TO13-TO13
#define	             RW_TO14  569	//TO14-TO14
#define	             RW_TO15  570	//TO15-TO15
#define	             RW_TO16  571	//TO16-TO16
#define	             RW_TO17  572	//TO17-TO17
#define	             RW_TO18  573	//TO18-TO18
#define	             RW_TO19  574	//TO19-TO19
#define	             RW_SIG1  575	//SIG1-SIG1
#define	             RW_SIG2  576	//SIG2-SIG2
#define	             RW_SIG3  577	//SIG3-SIG3
#define	             RW_SIG4  578	//SIG4-SIG4
#define	             RW_SIG5  579	//SIG5-SIG5
#define	             RW_SIG6  580	//SIG6-SIG6
#define	             RW_SIG7  581	//SIG7-SIG7
#define	             RW_SIG8  582	//SIG8-SIG8
#define	             RW_SIG9  583	//SIG9-SIG9
#define	            RW_SIG10  584	//SIG10-SIG10
#define	            RW_SIG11  585	//SIG11-SIG11
#define	            RW_SIG12  586	//SIG12-SIG12
#define	            RW_SIG13  587	//SIG13-SIG13
#define	            RW_SIG14  588	//SIG14-SIG14
#define	            RW_SIG15  589	//SIG15-SIG15
#define	            RW_SIG16  590	//SIG16-SIG16
#define	            RW_LED1G  591	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  592	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  593	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  594	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  595	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  596	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  597	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  598	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  599	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  600	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  601	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  602	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  603	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  604	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  605	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  606	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  607	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  608	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  609	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  610	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  611	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  612	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  613	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  614	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  615	//VIN1-VIN1
#define	             RW_VIN2  616	//VIN2-VIN2
#define	             RW_VIN3  617	//VIN3-VIN3
#define	             RW_VIN4  618	//VIN4-VIN4
#define	             RW_VIN5  619	//VIN5-VIN5
#define	             RW_VIN6  620	//VIN6-VIN6
#define	             RW_VIN7  621	//VIN7-VIN7
#define	             RW_VIN8  622	//VIN8-VIN8
#define	             RW_VIN9  623	//VIN9-VIN9
#define	            RW_VIN10  624	//VIN10-VIN10
#define	            RW_VIN11  625	//VIN11-VIN11
#define	            RW_VIN12  626	//VIN12-VIN12
#define	            RW_VIN13  627	//VIN13-VIN13
#define	            RW_VIN14  628	//VIN14-VIN14
#define	            RW_VIN15  629	//VIN15-VIN15
#define	            RW_VIN16  630	//VIN16-VIN16
#define	             RW_RCP1  631	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  632	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  633	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  634	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  635	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  636	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  637	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  638	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  639	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  640	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  641	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  642	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  643	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  644	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  645	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  646	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  647	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  648	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  649	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  650	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  651	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  652	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  653	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  654	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  655	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  656	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  657	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  658	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  659	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  660	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  661	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  662	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  663	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  664	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  665	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  666	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  667	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  668	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  669	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  670	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  671	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  672	//RCP42-ң��Ԥ�ü̵���42
#define	             RW_RCE1  673	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  674	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  675	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  676	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  677	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  678	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  679	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  680	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  681	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  682	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  683	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  684	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  685	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  686	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  687	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  688	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  689	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  690	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  691	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  692	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  693	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  694	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  695	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  696	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  697	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  698	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  699	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  700	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  701	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  702	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  703	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  704	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  705	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  706	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  707	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  708	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  709	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  710	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  711	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  712	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  713	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  714	//RCE42-ң��ִ�м̵���42
#define	             RW_RCV1  715	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  716	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  717	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  718	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  719	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  720	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  721	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  722	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  723	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  724	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  725	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  726	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  727	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  728	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  729	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  730	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  731	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  732	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  733	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  734	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  735	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  736	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  737	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  738	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  739	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  740	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  741	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  742	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  743	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  744	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  745	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  746	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  747	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  748	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  749	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  750	//RCV36-ң�ط�У�̵���36
#define	            RW_RCV37  751	//RCV37-ң�ط�У�̵���37
#define	            RW_RCV38  752	//RCV38-ң�ط�У�̵���38
#define	            RW_RCV39  753	//RCV39-ң�ط�У�̵���39
#define	            RW_RCV40  754	//RCV40-ң�ط�У�̵���40
#define	            RW_RCV41  755	//RCV41-ң�ط�У�̵���41
#define	            RW_RCV42  756	//RCV42-ң�ط�У�̵���42
#define	              RW_GE1  757	//GE1-GE1
#define	              RW_GE2  758	//GE2-GE2
#define	              RW_GE3  759	//GE3-GE3
#define	              RW_GE4  760	//GE4-GE4
#define	              RW_GE5  761	//GE5-GE5
#define	              RW_GE6  762	//GE6-GE6
#define	              RW_GE7  763	//GE7-GE7
#define	              RW_GE8  764	//GE8-GE8
#define	           RW_CONST1  765	//CONST1-CONST1
#define	           RW_CONST2  766	//CONST2-CONST2
#define	           RW_CONST3  767	//CONST3-CONST3
#define	           RW_CONST4  768	//CONST4-CONST4
#define	           RW_CONST5  769	//CONST5-CONST5
#define	           RW_CONST6  770	//CONST6-CONST6
#define	           RW_CONST7  771	//CONST7-CONST7
#define	           RW_CONST8  772	//CONST8-CONST8
#define	             RW_TLR1  773	//TLR1-�������Ӽ̵�����1
#define	             RW_TLR2  774	//TLR2-�������Ӽ̵�����2
#define	             RW_TLR3  775	//TLR3-�������Ӽ̵�����3
#define	             RW_TLR4  776	//TLR4-�������Ӽ̵�����4
#define	             RW_TLR5  777	//TLR5-�������Ӽ̵�����5
#define	             RW_TLR6  778	//TLR6-�������Ӽ̵�����6
#define	             RW_TLR7  779	//TLR7-�������Ӽ̵�����7
#define	             RW_TLR8  780	//TLR8-�������Ӽ̵�����8
#define	             RW_TLR9  781	//TLR9-�������Ӽ̵�����9
#define	            RW_TLR10  782	//TLR10-�������Ӽ̵�����10
#define	            RW_TLR11  783	//TLR11-�������Ӽ̵�����11
#define	            RW_TLR12  784	//TLR12-�������Ӽ̵�����12
#define	            RW_TLR13  785	//TLR13-�������Ӽ̵�����13
#define	            RW_TLR14  786	//TLR14-�������Ӽ̵�����14
#define	            RW_TLR15  787	//TLR15-�������Ӽ̵�����15
#define	            RW_TLR16  788	//TLR16-�������Ӽ̵�����16
#define	             RW_RLR1  789	//RLR1-�������Ӽ̵�����1
#define	             RW_RLR2  790	//RLR2-�������Ӽ̵�����2
#define	             RW_RLR3  791	//RLR3-�������Ӽ̵�����3
#define	             RW_RLR4  792	//RLR4-�������Ӽ̵�����4
#define	             RW_RLR5  793	//RLR5-�������Ӽ̵�����5
#define	             RW_RLR6  794	//RLR6-�������Ӽ̵�����6
#define	             RW_RLR7  795	//RLR7-�������Ӽ̵�����7
#define	             RW_RLR8  796	//RLR8-�������Ӽ̵�����8
#define	             RW_RLR9  797	//RLR9-�������Ӽ̵�����9
#define	            RW_RLR10  798	//RLR10-�������Ӽ̵�����10
#define	            RW_RLR11  799	//RLR11-�������Ӽ̵�����11
#define	            RW_RLR12  800	//RLR12-�������Ӽ̵�����12
#define	            RW_RLR13  801	//RLR13-�������Ӽ̵�����13
#define	            RW_RLR14  802	//RLR14-�������Ӽ̵�����14
#define	            RW_RLR15  803	//RLR15-�������Ӽ̵�����15
#define	            RW_RLR16  804	//RLR16-�������Ӽ̵�����16
#define	             RW_CNT1  805	//CNT1-�����̵�����1
#define	             RW_CNT2  806	//CNT2-�����̵�����2
#define	             RW_CNT3  807	//CNT3-�����̵�����3
#define	             RW_CNT4  808	//CNT4-�����̵�����4
#define	             RW_CNT5  809	//CNT5-�����̵�����5
#define	             RW_CNT6  810	//CNT6-�����̵�����6
#define	             RW_CNT7  811	//CNT7-�����̵�����7
#define	             RW_CNT8  812	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			813		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			157	

//�м�̵�����
#define	MID_RELAY_WORD_START			159	
#define	MID_RELAY_WORD_NUM			118	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			277	
#define	KEEP_RELAY_WORD_NUM			34	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			311	
#define	TIME_RELAY_WORD_NUM			34	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			345	
#define	TRIP_RELAY_WORD_NUM			4	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			349	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			381	
#define	INPUT_RELAY_WORD_NUM			68	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			449	
#define	OUTPUT_RELAY_WORD_NUM			33	

//ѹ��̵���
#define	SW_RELAY_WORD_START			482	
#define	SW_RELAY_WORD_NUM			14	

//���ü̵���
#define	CFG_RELAY_WORD_START			496	
#define	CFG_RELAY_WORD_NUM			44	

//����̵���
#define	PULSE_RELAY_WORD_START			540	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			556	
#define	CTRLTEST_RELAY_WORD_NUM			19	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			575	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			591	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			615	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			631	
#define	RCP_RELAY_WORD_NUM			42	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			673	
#define	RCE_RELAY_WORD_NUM			42	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			715	
#define	RCV_RELAY_WORD_NUM			42	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			757	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			765	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			773	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			789	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			805	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			805	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			805	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			805	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			805	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			805	
#define	CNT_RELAY_WORD_NUM			8	


#endif