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
#define	          RW_LPC50HA    2	//LPC50HA-�߲�A��������ѭ�����Ƽ̵�����
#define	          RW_LPC50HB    3	//LPC50HB-�߲�B��������ѭ�����Ƽ̵�����
#define	          RW_LPC50HC    4	//LPC50HC-�߲�C��������ѭ�����Ƽ̵�����
#define	         RW_LPC50LA1    5	//LPC50LA1-�Ͳ�A����I�α���ѭ�����Ƽ̵�����
#define	         RW_LPC50LB1    6	//LPC50LB1-�Ͳ�B����I�α���ѭ�����Ƽ̵�����
#define	        RW_LPC50OLA1    7	//LPC50OLA1-A�������1����ѭ�����Ƽ̵�����
#define	        RW_LPC50OLB1    8	//LPC50OLB1-B�������1����ѭ�����Ƽ̵�����
#define	        RW_LPC50OLC1    9	//LPC50OLC1-C�������1����ѭ�����Ƽ̵�����
#define	        RW_LPC50OLA2   10	//LPC50OLA2-A�������2����ѭ�����Ƽ̵�����
#define	        RW_LPC50OLB2   11	//LPC50OLB2-B�������2����ѭ�����Ƽ̵�����
#define	        RW_LPC50OLC2   12	//LPC50OLC2-C�������2����ѭ�����Ƽ̵�����
#define	           RW_LPC50N   13	//LPC50N-�����������ѭ�����Ƽ̵�����
#define	           RW_LPC59N   14	//LPC59N-�����ѹ����ѭ�����Ƽ̵�����
#define	         RW_LPC50DCG   15	//LPC50DCG-��϶��������ѭ�����Ƽ̵�����
#define	         RW_LPC50LA2   16	//LPC50LA2-�Ͳ�A����II�α���ѭ�����Ƽ̵�����
#define	         RW_LPC50LB2   17	//LPC50LB2-�Ͳ�B����II�α���ѭ�����Ƽ̵�����
#define	           RW_PI50HA   18	//PI50HA-�߲�A������μ̵�����
#define	           RW_TR50HA   19	//TR50HA-�߲�A���������̵�����
#define	           RW_PI50HB   20	//PI50HB-�߲�B������μ̵�����
#define	           RW_TR50HB   21	//TR50HB-�߲�B���������̵�����
#define	           RW_PI50HC   22	//PI50HC-�߲�C������μ̵�����
#define	           RW_TR50HC   23	//TR50HC-�߲�C���������̵�����
#define	          RW_PI50LA1   24	//PI50LA1-�Ͳ�A����I����μ̵�����
#define	          RW_TR50LA1   25	//TR50LA1-�Ͳ�A����I�ζ����̵�����
#define	          RW_PI50LB1   26	//PI50LB1-�Ͳ�B����I����μ̵�����
#define	          RW_TR50LB1   27	//TR50LB1-�Ͳ�B����I�ζ����̵�����
#define	         RW_PI50OLA1   28	//PI50OLA1-A�������1��μ̵�����
#define	         RW_TR50OLA1   29	//TR50OLA1-A�������1�����̵�����
#define	         RW_PI50OLB1   30	//PI50OLB1-B�������1��μ̵�����
#define	         RW_TR50OLB1   31	//TR50OLB1-B�������1�����̵�����
#define	         RW_PI50OLC1   32	//PI50OLC1-C�������1��μ̵�����
#define	         RW_TR50OLC1   33	//TR50OLC1-C�������1�����̵�����
#define	         RW_PI50OLA2   34	//PI50OLA2-A�������2��μ̵�����
#define	         RW_TR50OLA2   35	//TR50OLA2-A�������2�����̵�����
#define	         RW_PI50OLB2   36	//PI50OLB2-B�������2��μ̵�����
#define	         RW_TR50OLB2   37	//TR50OLB2-B�������2�����̵�����
#define	         RW_PI50OLC2   38	//PI50OLC2-C�������2��μ̵�����
#define	         RW_TR50OLC2   39	//TR50OLC2-C�������2�����̵�����
#define	            RW_PI50N   40	//PI50N-���������μ̵�����
#define	            RW_TR50N   41	//TR50N-������������̵�����
#define	            RW_PI59N   42	//PI59N-�����ѹ��μ̵�����
#define	            RW_TR59N   43	//TR59N-�����ѹ�����̵�����
#define	          RW_PI50DCG   44	//PI50DCG-��϶������μ̵�����
#define	          RW_TR50DCG   45	//TR50DCG-��϶���������̵�����
#define	             RW_PI27   46	//PI27-�߲�ʧѹ��μ̵�����
#define	             RW_TR27   47	//TR27-�߲�ʧѹ�����̵�����
#define	            RW_AR49A   48	//AR49A-A�෴ʱ�޹����ɸ澯�̵�����
#define	            RW_TR49A   49	//TR49A-A�෴ʱ�޹����ɶ����̵�����
#define	            RW_AR49B   50	//AR49B-B�෴ʱ�޸澯�̵�����
#define	            RW_TR49B   51	//TR49B-B�෴ʱ�޶����̵�����
#define	            RW_AR49C   52	//AR49C-C�෴ʱ�޸澯�̵�����
#define	            RW_TR49C   53	//TR49C-C�෴ʱ�޶����̵�����
#define	          RW_PI50LA2   54	//PI50LA2-�Ͳ�A�����II����μ̵�����
#define	          RW_TR50LA2   55	//TR50LA2-�Ͳ�A����II�ζ����̵�����
#define	          RW_PI50LB2   56	//PI50LB2-�Ͳ�B����II����μ̵�����
#define	          RW_TR50LB2   57	//TR50LB2-�Ͳ�B����II�ζ����̵�����
#define	          RW_PI74PTH   58	//PI74PTH-�߲�PT������μ̵�����
#define	          RW_AR74PTH   59	//AR74PTH-�߲�PT���߸澯�̵�����
#define	         RW_PI74PTLA   60	//PI74PTLA-�Ͳ�A��PT������μ̵�����
#define	         RW_AR74PTLA   61	//AR74PTLA-�Ͳ�A��PT���߶����̵�����
#define	         RW_PI74PTLB   62	//PI74PTLB-�Ͳ�B��PT������μ̵�����
#define	         RW_AR74PTLB   63	//AR74PTLB-�Ͳ�B��PT���߶����̵�����
#define	           RW_PICKUP   64	//PICKUP-ģֵͻ���������̵�����
#define	             RW_50HA   65	//50HA-IHA�����̵�����
#define	             RW_37HA   66	//37HA-IHA�����̵�����
#define	             RW_50HB   67	//50HB-IHB�����̵�����
#define	             RW_37HB   68	//37HB-IHB�����̵�����
#define	             RW_50HC   69	//50HC-IHC�����̵�����
#define	             RW_37HC   70	//37HC-IHC�����̵�����
#define	             RW_50LA   71	//50LA-ILA�����̵�����
#define	             RW_37LA   72	//37LA-ILA�����̵�����
#define	             RW_50LB   73	//50LB-ILB�����̵�����
#define	             RW_37LB   74	//37LB-ILB�����̵�����
#define	              RW_50N   75	//50N-I0�����̵�����
#define	              RW_37N   76	//37N-I0�����̵�����
#define	             RW_50JX   77	//50JX-��϶���������̵�����
#define	             RW_37JX   78	//37JX-��϶���������̵�����
#define	             RW_59HA   79	//59HA-UHA��ѹ�̵�����
#define	             RW_27HA   80	//27HA-UHA��ѹ�̵�����
#define	             RW_59HB   81	//59HB-UHB��ѹ�̵�����
#define	             RW_27HB   82	//27HB-UHB��ѹ�̵�����
#define	             RW_59HC   83	//59HC-UHC��ѹ�̵�����
#define	             RW_27HC   84	//27HC-UHC��ѹ�̵�����
#define	             RW_59LA   85	//59LA-ULA��ѹ�̵�����
#define	             RW_27LA   86	//27LA-ULA��ѹ�̵�����
#define	             RW_59LB   87	//59LB-ULB��ѹ�̵�����
#define	             RW_27LB   88	//27LB-ULB��ѹ�̵�����
#define	              RW_59N   89	//59N-U0��ѹ�̵�����
#define	              RW_27N   90	//27N-U0��ѹ�̵�����
#define	          RW_ET11ERR   91	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   92	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   93	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   94	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   95	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   96	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   97	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   98	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   99	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR  100	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR  101	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR  102	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR  103	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR  104	//NULL1ERR-NULL1����״̬�̵�����
#define	         RW_NULL2ERR  105	//NULL2ERR-NULL2����״̬�̵�����
#define	         RW_NULL3ERR  106	//NULL3ERR-NULL3����״̬�̵�����
#define	        RW_MAINTLINK  107	//MAINTLINK-ά��״̬�̵�����
#define	         RW_LKPI50HA  108	//LKPI50HA-�߲�A������α����̵�����
#define	        RW_LKRST50HA  109	//LKRST50HA-�߲�A�������ر����̵�����
#define	         RW_LKPI50HB  110	//LKPI50HB-�߲�B������α����̵�����
#define	        RW_LKRST50HB  111	//LKRST50HB-�߲�B�������ر����̵�����
#define	         RW_LKPI50HC  112	//LKPI50HC-�߲�C������α����̵�����
#define	        RW_LKRST50HC  113	//LKRST50HC-�߲�C�������ر����̵�����
#define	        RW_LKPI50LA1  114	//LKPI50LA1-�Ͳ�A����I����α����̵�����
#define	       RW_LKRST50LA1  115	//LKRST50LA1-�Ͳ�A����I�η��ر����̵�����
#define	        RW_LKPI50LB1  116	//LKPI50LB1-�Ͳ�B����I����α����̵�����
#define	       RW_LKRST50LB1  117	//LKRST50LB1-�Ͳ�B����I�η��ر����̵�����
#define	       RW_LKPI50OLA1  118	//LKPI50OLA1-A������1��α����̵�����
#define	      RW_LKRST50OLA1  119	//LKRST50OLA1-A������1���ر����̵�����
#define	       RW_LKPI50OLB1  120	//LKPI50OLB1-B������1��α����̵�����
#define	      RW_LKRST50OLB1  121	//LKRST50OLB1-B������1���ر����̵�����
#define	       RW_LKPI50OLC1  122	//LKPI50OLC1-C������1��α����̵�����
#define	      RW_LKRST50OLC1  123	//LKRST50OLC1-C������1���ر����̵�����
#define	       RW_LKPI50OLA2  124	//LKPI50OLA2-A������2��α����̵�����
#define	      RW_LKRST50OLA2  125	//LKRST50OLA2-A������2���ر����̵�����
#define	       RW_LKPI50OLB2  126	//LKPI50OLB2-B������2��α����̵�����
#define	      RW_LKRST50OLB2  127	//LKRST50OLB2-B������2���ر����̵�����
#define	       RW_LKPI50OLC2  128	//LKPI50OLC2-C������2��α����̵�����
#define	      RW_LKRST50OLC2  129	//LKRST50OLC2-C������2���ر����̵�����
#define	          RW_LKPI50N  130	//LKPI50N-���������α����̵�����
#define	         RW_LKRST50N  131	//LKRST50N-����������ر����̵�����
#define	          RW_LKPI59N  132	//LKPI59N-�����ѹ��α����̵�����
#define	         RW_LKRST59N  133	//LKRST59N-�����ѹ���ر����̵�����
#define	        RW_LKPI50DCG  134	//LKPI50DCG-��϶������α����̵�����
#define	       RW_LKRST50DCG  135	//LKRST50DCG-��϶�������ر����̵�����
#define	           RW_LKPI27  136	//LKPI27-�߲�ʧѹ��α����̵�����
#define	          RW_LKRST27  137	//LKRST27-�߲�ʧѹ���ر����̵�����
#define	          RW_LKPIPTH  138	//LKPIPTH-�߲�PT������α����̵�����
#define	         RW_LKPIPTLA  139	//LKPIPTLA-�Ͳ�A��PT������α����̵�����
#define	         RW_LKPIPTLB  140	//LKPIPTLB-�Ͳ�B��PT������α����̵�����
#define	        RW_LKPI50LA2  141	//LKPI50LA2-�Ͳ�A����II����α����̵�����
#define	       RW_LKRST50LA2  142	//LKRST50LA2-�Ͳ�A����II�η��ر����̵�����
#define	        RW_LKPI50LB2  143	//LKPI50LB2-�Ͳ�B����II����α����̵�����
#define	       RW_LKRST50LB2  144	//LKRST50LB2-�Ͳ�B����II�η��ر����̵�����
#define	             RW_LKCH  145	//LKCH-�߲��·����բ�����̵�����
#define	             RW_LKTH  146	//LKTH-�߲��·����բ�����̵�����
#define	            RW_LKCLA  147	//LKCLA-�Ͳ��·��A��բ�����̵�����
#define	            RW_LKTLA  148	//LKTLA-�Ͳ��·��A��բ�����̵�����
#define	            RW_LKCLB  149	//LKCLB-�Ͳ��·��B��բ�����̵�����
#define	            RW_LKTLB  150	//LKTLB-�Ͳ��·��B��բ�����̵�����
#define	             RW_52AH  151	//52AH-��ѹ���·����λ
#define	             RW_52BH  152	//52BH-��ѹ���·����λ
#define	            RW_52ALA  153	//52ALA-�Ͳ�A��·����λ
#define	            RW_52BLA  154	//52BLA-�Ͳ�A��·����λ
#define	            RW_52ALB  155	//52ALB-�Ͳ�B��·����λ
#define	            RW_52BLB  156	//52BLB-�Ͳ�B��·����λ
#define	              RW_REC  157	//REC-¼���̵�����
#define	             RW_69IN  158	//69IN-Զ�������м�̵�����
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
#define	               RW_H1  223	//H1-�Ա����м�̵�����1
#define	               RW_H2  224	//H2-�Ա����м�̵�����2
#define	               RW_H3  225	//H3-�Ա����м�̵�����3
#define	               RW_H4  226	//H4-�Ա����м�̵�����4
#define	               RW_H5  227	//H5-�Ա����м�̵�����5
#define	               RW_H6  228	//H6-�Ա����м�̵�����6
#define	               RW_H7  229	//H7-�Ա����м�̵�����7
#define	               RW_H8  230	//H8-�Ա����м�̵�����8
#define	               RW_H9  231	//H9-�Ա����м�̵�����9
#define	              RW_H10  232	//H10-�Ա����м�̵�����10
#define	              RW_H11  233	//H11-�Ա����м�̵�����11
#define	              RW_H12  234	//H12-�Ա����м�̵�����12
#define	              RW_H13  235	//H13-�Ա����м�̵�����13
#define	              RW_H14  236	//H14-�Ա����м�̵�����14
#define	              RW_H15  237	//H15-�Ա����м�̵�����15
#define	              RW_H16  238	//H16-�Ա����м�̵�����16
#define	              RW_H17  239	//H17-�Ա����м�̵�����17
#define	              RW_H18  240	//H18-�Ա����м�̵�����18
#define	              RW_H19  241	//H19-�Ա����м�̵�����19
#define	              RW_H20  242	//H20-�Ա����м�̵�����20
#define	              RW_H21  243	//H21-�Ա����м�̵�����21
#define	              RW_H22  244	//H22-�Ա����м�̵�����22
#define	              RW_H23  245	//H23-�Ա����м�̵�����23
#define	              RW_H24  246	//H24-�Ա����м�̵�����24
#define	              RW_H25  247	//H25-�Ա����м�̵�����25
#define	              RW_H26  248	//H26-�Ա����м�̵�����26
#define	              RW_H27  249	//H27-�Ա����м�̵�����27
#define	              RW_H28  250	//H28-�Ա����м�̵�����28
#define	              RW_H29  251	//H29-�Ա����м�̵�����29
#define	              RW_H30  252	//H30-�Ա����м�̵�����30
#define	              RW_H31  253	//H31-�Ա����м�̵�����31
#define	              RW_H32  254	//H32-�Ա����м�̵�����32
#define	            RW_52BFH  255	//52BFH-�߲��·����բʧ�̵ܼ�����
#define	           RW_52BFLA  256	//52BFLA-�Ͳ�A��·����բʧ�̵ܼ�����
#define	           RW_52BFLB  257	//52BFLB-�Ͳ�B��·����բʧ�̵ܼ�����
#define	               RW_T1  258	//T1-T1
#define	               RW_T2  259	//T2-T2
#define	               RW_T3  260	//T3-T3
#define	               RW_T4  261	//T4-T4
#define	               RW_T5  262	//T5-T5
#define	               RW_T6  263	//T6-T6
#define	               RW_T7  264	//T7-T7
#define	               RW_T8  265	//T8-T8
#define	               RW_T9  266	//T9-T9
#define	              RW_T10  267	//T10-T10
#define	              RW_T11  268	//T11-T11
#define	              RW_T12  269	//T12-T12
#define	              RW_T13  270	//T13-T13
#define	              RW_T14  271	//T14-T14
#define	              RW_T15  272	//T15-T15
#define	              RW_T16  273	//T16-T16
#define	             RW_TCB1  274	//TCB1-TCB1
#define	             RW_TCB2  275	//TCB2-TCB2
#define	             RW_TCB3  276	//TCB3-TCB3
#define	             RW_TCB4  277	//TCB4-TCB4
#define	             RW_TCB5  278	//TCB5-TCB5
#define	             RW_TCB6  279	//TCB6-TCB6
#define	             RW_TCB7  280	//TCB7-TCB7
#define	             RW_TCB8  281	//TCB8-TCB8
#define	             RW_TSW1  282	//TSW1-TSW1
#define	             RW_TSW2  283	//TSW2-TSW2
#define	             RW_TSW3  284	//TSW3-TSW3
#define	             RW_TSW4  285	//TSW4-TSW4
#define	             RW_TSW5  286	//TSW5-TSW5
#define	             RW_TSW6  287	//TSW6-TSW6
#define	             RW_TSW7  288	//TSW7-TSW7
#define	             RW_TSW8  289	//TSW8-TSW8
#define	           RW_T52BFH  290	//T52BFH-T52BFH
#define	          RW_T52BFLA  291	//T52BFLA-T52BFLA
#define	          RW_T52BFLB  292	//T52BFLB-T52BFLB
#define	              RW_TR1  293	//TR1-��բ�̵���1
#define	              RW_TR2  294	//TR2-��բ�̵���2
#define	              RW_TR3  295	//TR3-��բ�̵���3
#define	              RW_TR4  296	//TR4-��բ�̵���4
#define	              RW_VR1  297	//VR1-����̵���1
#define	              RW_VR2  298	//VR2-����̵���2
#define	              RW_VR3  299	//VR3-����̵���3
#define	              RW_VR4  300	//VR4-����̵���4
#define	              RW_VR5  301	//VR5-����̵���5
#define	              RW_VR6  302	//VR6-����̵���6
#define	              RW_VR7  303	//VR7-����̵���7
#define	              RW_VR8  304	//VR8-����̵���8
#define	              RW_VR9  305	//VR9-����̵���9
#define	             RW_VR10  306	//VR10-����̵���10
#define	             RW_VR11  307	//VR11-����̵���11
#define	             RW_VR12  308	//VR12-����̵���12
#define	             RW_VR13  309	//VR13-����̵���13
#define	             RW_VR14  310	//VR14-����̵���14
#define	             RW_VR15  311	//VR15-����̵���15
#define	             RW_VR16  312	//VR16-����̵���16
#define	             RW_VR17  313	//VR17-����̵���17
#define	             RW_VR18  314	//VR18-����̵���18
#define	             RW_VR19  315	//VR19-����̵���19
#define	             RW_VR20  316	//VR20-����̵���20
#define	             RW_VR21  317	//VR21-����̵���21
#define	             RW_VR22  318	//VR22-����̵���22
#define	             RW_VR23  319	//VR23-����̵���23
#define	             RW_VR24  320	//VR24-����̵���24
#define	             RW_VR25  321	//VR25-����̵���25
#define	             RW_VR26  322	//VR26-����̵���26
#define	             RW_VR27  323	//VR27-����̵���27
#define	             RW_VR28  324	//VR28-����̵���28
#define	             RW_VR29  325	//VR29-����̵���29
#define	             RW_VR30  326	//VR30-����̵���30
#define	             RW_VR31  327	//VR31-����̵���31
#define	             RW_VR32  328	//VR32-����̵���32
#define	              RW_IN1  329	//IN1-IN1
#define	              RW_IN2  330	//IN2-IN2
#define	              RW_IN3  331	//IN3-IN3
#define	              RW_IN4  332	//IN4-IN4
#define	              RW_IN5  333	//IN5-IN5
#define	              RW_IN6  334	//IN6-IN6
#define	              RW_IN7  335	//IN7-IN7
#define	              RW_IN8  336	//IN8-IN8
#define	              RW_IN9  337	//IN9-IN9
#define	             RW_IN10  338	//IN10-IN10
#define	             RW_IN11  339	//IN11-IN11
#define	             RW_IN12  340	//IN12-IN12
#define	             RW_IN13  341	//IN13-IN13
#define	             RW_IN14  342	//IN14-IN14
#define	             RW_IN15  343	//IN15-IN15
#define	             RW_IN16  344	//IN16-IN16
#define	             RW_IN17  345	//IN17-IN17
#define	             RW_IN18  346	//IN18-IN18
#define	             RW_IN19  347	//IN19-IN19
#define	             RW_IN20  348	//IN20-IN20
#define	             RW_IN21  349	//IN21-IN21
#define	             RW_IN22  350	//IN22-IN22
#define	             RW_IN23  351	//IN23-IN23
#define	             RW_IN24  352	//IN24-IN24
#define	             RW_IN25  353	//IN25-IN25
#define	             RW_IN26  354	//IN26-IN26
#define	             RW_IN27  355	//IN27-IN27
#define	             RW_IN28  356	//IN28-IN28
#define	             RW_IN29  357	//IN29-IN29
#define	             RW_IN30  358	//IN30-IN30
#define	             RW_IN31  359	//IN31-IN31
#define	             RW_IN32  360	//IN32-IN32
#define	             RW_IN33  361	//IN33-IN33
#define	             RW_IN34  362	//IN34-IN34
#define	             RW_IN35  363	//IN35-�źŸ���
#define	             RW_IN36  364	//IN36-�Ͳ�A��·����λ
#define	             RW_IN37  365	//IN37-�Ͳ�A��·����λ
#define	             RW_IN38  366	//IN38-�Ͳ�A��·��ң��Ԥ��
#define	             RW_IN39  367	//IN39-�Ͳ�A������ң��1Ԥ��
#define	             RW_IN40  368	//IN40-�Ͳ�A������ң��2Ԥ��
#define	             RW_IN41  369	//IN41-�Ͳ�B��·����λ
#define	             RW_IN42  370	//IN42-�Ͳ�B��·����λ
#define	             RW_IN43  371	//IN43-�Ͳ�B��·��ң��Ԥ��
#define	             RW_IN44  372	//IN44-�Ͳ�B������ң��1Ԥ��
#define	             RW_IN45  373	//IN45-�Ͳ�B������ң��2Ԥ��
#define	             RW_IN46  374	//IN46-�߲��·����λ
#define	             RW_IN47  375	//IN47-�߲��·����λ
#define	             RW_IN48  376	//IN48-�߲������ң��Ԥ��
#define	             RW_IN49  377	//IN49-�߲������ң��1Ԥ��
#define	             RW_IN50  378	//IN50-�߲������ң��2Ԥ��
#define	           RW_INVIN1  379	//INVIN1-VIN1
#define	           RW_INVIN2  380	//INVIN2-VIN2
#define	           RW_INVIN3  381	//INVIN3-VIN3
#define	           RW_INVIN4  382	//INVIN4-VIN4
#define	           RW_INVIN5  383	//INVIN5-VIN5
#define	           RW_INVIN6  384	//INVIN6-VIN6
#define	           RW_INVIN7  385	//INVIN7-VIN7
#define	           RW_INVIN8  386	//INVIN8-VIN8
#define	           RW_INVIN9  387	//INVIN9-VIN9
#define	          RW_INVIN10  388	//INVIN10-VIN10
#define	          RW_INVIN11  389	//INVIN11-VIN11
#define	          RW_INVIN12  390	//INVIN12-VIN12
#define	          RW_INVIN13  391	//INVIN13-VIN13
#define	          RW_INVIN14  392	//INVIN14-VIN14
#define	          RW_INVIN15  393	//INVIN15-VIN15
#define	          RW_INVIN16  394	//INVIN16-VIN16
#define	          RW_INVIN17  395	//INVIN17-VIN17
#define	          RW_INVIN18  396	//INVIN18-VIN18
#define	          RW_INVIN19  397	//INVIN19-VIN19
#define	          RW_INVIN20  398	//INVIN20-VIN20
#define	          RW_INVIN21  399	//INVIN21-VIN21
#define	          RW_INVIN22  400	//INVIN22-VIN22
#define	          RW_INVIN23  401	//INVIN23-VIN23
#define	          RW_INVIN24  402	//INVIN24-VIN24
#define	          RW_INVIN25  403	//INVIN25-VIN25
#define	          RW_INVIN26  404	//INVIN26-VIN26
#define	          RW_INVIN27  405	//INVIN27-VIN27
#define	          RW_INVIN28  406	//INVIN28-VIN28
#define	          RW_INVIN29  407	//INVIN29-VIN29
#define	          RW_INVIN30  408	//INVIN30-VIN30
#define	          RW_INVIN31  409	//INVIN31-VIN31
#define	          RW_INVIN32  410	//INVIN32-VIN32
#define	             RW_OUT1  411	//OUT1-����1�̵�����
#define	             RW_OUT2  412	//OUT2-����2�̵�����
#define	             RW_OUT3  413	//OUT3-����3�̵�����
#define	             RW_OUT4  414	//OUT4-����4�̵�����
#define	             RW_OUT5  415	//OUT5-����5�̵�����
#define	             RW_OUT6  416	//OUT6-����6�̵�����
#define	             RW_OUT7  417	//OUT7-����7�̵�����
#define	             RW_OUT8  418	//OUT8-����8�̵�����
#define	             RW_OUT9  419	//OUT9-����9�̵�����
#define	            RW_OUT10  420	//OUT10-����10�̵�����
#define	            RW_OUT11  421	//OUT11-����11�̵�����
#define	            RW_OUT12  422	//OUT12-����12�̵�����
#define	            RW_OUT13  423	//OUT13-����13�̵�����
#define	            RW_OUT14  424	//OUT14-����14�̵�����
#define	            RW_OUT15  425	//OUT15-����15�̵�����
#define	            RW_OUT16  426	//OUT16-����16�̵�����
#define	            RW_OUT17  427	//OUT17-����17�̵�����
#define	            RW_OUT18  428	//OUT18-����18�̵�����
#define	            RW_OUT19  429	//OUT19-����19�̵�����
#define	            RW_OUT20  430	//OUT20-����20�̵�����
#define	            RW_OUT21  431	//OUT21-����21�̵�����
#define	            RW_OUT22  432	//OUT22-����22�̵�����
#define	            RW_OUT23  433	//OUT23-����23�̵�����
#define	            RW_OUT24  434	//OUT24-����24�̵�����
#define	            RW_OUT25  435	//OUT25-����25�̵�����
#define	            RW_OUT26  436	//OUT26-����26�̵�����
#define	            RW_OUT27  437	//OUT27-����27�̵�����
#define	            RW_OUT28  438	//OUT28-����28�̵�����
#define	            RW_OUT29  439	//OUT29-����29�̵�����
#define	            RW_OUT30  440	//OUT30-����30�̵�����
#define	            RW_OUT31  441	//OUT31-����31�̵�����
#define	            RW_OUT32  442	//OUT32-����32�̵�����
#define	            RW_OUT33  443	//OUT33-����33�̵�����
#define	            RW_OUT34  444	//OUT34-����34�̵�����
#define	            RW_OUT35  445	//OUT35-����35�̵�����
#define	            RW_OUT36  446	//OUT36-����36�̵�����
#define	            RW_OUT37  447	//OUT37-����37�̵�����
#define	            RW_OUT38  448	//OUT38-����38�̵�����
#define	            RW_OUT39  449	//OUT39-����39�̵�����
#define	            RW_OUT40  450	//OUT40-����40�̵�����
#define	            RW_OUT41  451	//OUT41-����41�̵�����
#define	            RW_SL50H  452	//SL50H-�߲����������ѹ��̵�����
#define	             RW_SL49  453	//SL49-��ʱ�޹�������ѹ��̵�����
#define	            RW_SL50L  454	//SL50L-�Ͳ����������ѹ��̵�����
#define	           RW_SL50OL  455	//SL50OL-��������ѹ��̵�����
#define	             RW_SL27  456	//SL27-�߲�ʧѹ������ѹ��̵�����
#define	            RW_SL50N  457	//SL50N-���������ѹ��̵�����
#define	            RW_SL59N  458	//SL59N-�����ѹ��ѹ��̵�����
#define	          RW_SL50DCG  459	//SL50DCG-��϶������ѹ��̵�����
#define	          RW_SLMAINT  460	//SLMAINT-����ѹ��
#define	           RW_SLBAK1  461	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  462	//SLBAK2-����ѹ��2
#define	            RW_F50HA  463	//F50HA-�߲�A��������ü̵�����
#define	            RW_F50HB  464	//F50HB-�߲�B��������ü̵�����
#define	            RW_F50HC  465	//F50HC-�߲�C��������ü̵�����
#define	          RW_F50H_27  466	//F50H_27-�߲��ѹ�������ü̵�����
#define	              RW_F49  467	//F49-��ʱ�޹��������ü̵�����
#define	           RW_F50LA1  468	//F50LA1-�Ͳ�A�����I�����ü̵�����
#define	         RW_F50LA_27  469	//F50LA_27-�Ͳ�A��ѹ�������ü̵�����
#define	           RW_F50LB1  470	//F50LB1-�Ͳ�B�����I�����ü̵�����
#define	         RW_F50LB_27  471	//F50LB_27-�Ͳ�B���ѹ�������ü̵�����
#define	          RW_F50OLA1  472	//F50OLA1-A�����ɸ澯���ü̵�����
#define	          RW_F50OLB1  473	//F50OLB1-B�����ɸ澯���ü̵�����
#define	          RW_F50OLC1  474	//F50OLC1-C�����ɸ澯���ü̵�����
#define	          RW_F50OLA2  475	//F50OLA2-A��������բ���ü̵�����
#define	          RW_F50OLB2  476	//F50OLB2-B��������բ���ü̵�����
#define	          RW_F50OLC2  477	//F50OLC2-C��������բ���ü̵�����
#define	              RW_F27  478	//F27-�߲�ʧѹ�������ü̵�����
#define	             RW_F50N  479	//F50N-��������������ü̵�����
#define	             RW_F59N  480	//F59N-�����ѹ�������ü̵�����
#define	            RW_F74PT  481	//F74PT-PT���߼�����ü̵�����
#define	           RW_F50DCG  482	//F50DCG-��϶�����������ü̵�����
#define	           RW_F50LA2  483	//F50LA2-�Ͳ�A�����II�����ü̵�����
#define	           RW_F50LB2  484	//F50LB2-�Ͳ�B�����II�����ü̵�����
#define	            RW_F52BF  485	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  486	//FBAK1-��������1
#define	            RW_FBAK2  487	//FBAK2-��������2
#define	              RW_PW1  488	//PW1-����̵���1
#define	              RW_PW2  489	//PW2-����̵���2
#define	              RW_PW3  490	//PW3-����̵���3
#define	              RW_PW4  491	//PW4-����̵���4
#define	              RW_PW5  492	//PW5-����̵���5
#define	              RW_PW6  493	//PW6-����̵���6
#define	              RW_PW7  494	//PW7-����̵���7
#define	              RW_PW8  495	//PW8-����̵���8
#define	              RW_PW9  496	//PW9-����̵���9
#define	             RW_PW10  497	//PW10-����̵���10
#define	             RW_PW11  498	//PW11-����̵���11
#define	             RW_PW12  499	//PW12-����̵���12
#define	             RW_PW13  500	//PW13-����̵���13
#define	             RW_PW14  501	//PW14-����̵���14
#define	             RW_PW15  502	//PW15-����̵���15
#define	             RW_PW16  503	//PW16-����̵���16
#define	              RW_TO1  504	//TO1-�����̵���1
#define	              RW_TO2  505	//TO2-�����̵���2
#define	              RW_TO3  506	//TO3-�����̵���3
#define	              RW_TO4  507	//TO4-�����̵���4
#define	              RW_TO5  508	//TO5-�����̵���5
#define	              RW_TO6  509	//TO6-�����̵���6
#define	              RW_TO7  510	//TO7-�����̵���7
#define	              RW_TO8  511	//TO8-�����̵���8
#define	              RW_TO9  512	//TO9-�����̵���9
#define	             RW_TO10  513	//TO10-�����̵���10
#define	             RW_TO11  514	//TO11-�����̵���11
#define	             RW_TO12  515	//TO12-�����̵���12
#define	             RW_TO13  516	//TO13-�����̵���13
#define	             RW_TO14  517	//TO14-�����̵���14
#define	             RW_TO15  518	//TO15-�����̵���15
#define	             RW_TO16  519	//TO16-�����̵���16
#define	             RW_TO17  520	//TO17-�����̵���17
#define	             RW_TO18  521	//TO18-�����̵���18
#define	             RW_TO19  522	//TO19-�����̵���19
#define	             RW_TO20  523	//TO20-�����̵���20
#define	             RW_TO21  524	//TO21-�����̵���21
#define	             RW_SIG1  525	//SIG1-�źż̵���1
#define	             RW_SIG2  526	//SIG2-�źż̵���2
#define	             RW_SIG3  527	//SIG3-�źż̵���3
#define	             RW_SIG4  528	//SIG4-�źż̵���4
#define	             RW_SIG5  529	//SIG5-�źż̵���5
#define	             RW_SIG6  530	//SIG6-�źż̵���6
#define	             RW_SIG7  531	//SIG7-�źż̵���7
#define	             RW_SIG8  532	//SIG8-�źż̵���8
#define	             RW_SIG9  533	//SIG9-�źż̵���9
#define	            RW_SIG10  534	//SIG10-�źż̵���10
#define	            RW_SIG11  535	//SIG11-�źż̵���11
#define	            RW_SIG12  536	//SIG12-�źż̵���12
#define	            RW_SIG13  537	//SIG13-�źż̵���13
#define	            RW_SIG14  538	//SIG14-�źż̵���14
#define	            RW_SIG15  539	//SIG15-�źż̵���15
#define	            RW_SIG16  540	//SIG16-�źż̵���16
#define	            RW_LED1G  541	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  542	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  543	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  544	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  545	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  546	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  547	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  548	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  549	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  550	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  551	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  552	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  553	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  554	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  555	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  556	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  557	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  558	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  559	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  560	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  561	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  562	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  563	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  564	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  565	//VIN1-VIN1
#define	             RW_VIN2  566	//VIN2-VIN2
#define	             RW_VIN3  567	//VIN3-VIN3
#define	             RW_VIN4  568	//VIN4-VIN4
#define	             RW_VIN5  569	//VIN5-VIN5
#define	             RW_VIN6  570	//VIN6-VIN6
#define	             RW_VIN7  571	//VIN7-VIN7
#define	             RW_VIN8  572	//VIN8-VIN8
#define	             RW_VIN9  573	//VIN9-VIN9
#define	            RW_VIN10  574	//VIN10-VIN10
#define	            RW_VIN11  575	//VIN11-VIN11
#define	            RW_VIN12  576	//VIN12-VIN12
#define	            RW_VIN13  577	//VIN13-VIN13
#define	            RW_VIN14  578	//VIN14-VIN14
#define	            RW_VIN15  579	//VIN15-VIN15
#define	            RW_VIN16  580	//VIN16-VIN16
#define	            RW_VIN17  581	//VIN17-VIN17
#define	            RW_VIN18  582	//VIN18-VIN18
#define	            RW_VIN19  583	//VIN19-VIN19
#define	            RW_VIN20  584	//VIN20-VIN20
#define	            RW_VIN21  585	//VIN21-VIN21
#define	            RW_VIN22  586	//VIN22-VIN22
#define	            RW_VIN23  587	//VIN23-VIN23
#define	            RW_VIN24  588	//VIN24-VIN24
#define	            RW_VIN25  589	//VIN25-VIN25
#define	            RW_VIN26  590	//VIN26-VIN26
#define	            RW_VIN27  591	//VIN27-VIN27
#define	            RW_VIN28  592	//VIN28-VIN28
#define	            RW_VIN29  593	//VIN29-VIN29
#define	            RW_VIN30  594	//VIN30-VIN30
#define	            RW_VIN31  595	//VIN31-VIN31
#define	            RW_VIN32  596	//VIN32-VIN32
#define	             RW_RCP1  597	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  598	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  599	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  600	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  601	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  602	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  603	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  604	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  605	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  606	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  607	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  608	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  609	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  610	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  611	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  612	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  613	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  614	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  615	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  616	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  617	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  618	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  619	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  620	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  621	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  622	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  623	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  624	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  625	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  626	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  627	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  628	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  629	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  630	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  631	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  632	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  633	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  634	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  635	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  636	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  637	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  638	//RCP42-ң��Ԥ�ü̵���42
#define	            RW_RCP43  639	//RCP43-ң��Ԥ�ü̵���43
#define	            RW_RCP44  640	//RCP44-ң��Ԥ�ü̵���44
#define	             RW_RCE1  641	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  642	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  643	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  644	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  645	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  646	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  647	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  648	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  649	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  650	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  651	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  652	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  653	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  654	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  655	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  656	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  657	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  658	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  659	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  660	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  661	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  662	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  663	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  664	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  665	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  666	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  667	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  668	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  669	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  670	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  671	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  672	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  673	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  674	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  675	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  676	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  677	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  678	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  679	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  680	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  681	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  682	//RCE42-ң��ִ�м̵���42
#define	            RW_RCE43  683	//RCE43-ң��ִ�м̵���43
#define	            RW_RCE44  684	//RCE44-ң��ִ�м̵���44
#define	             RW_RCV1  685	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  686	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  687	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  688	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  689	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  690	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  691	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  692	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  693	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  694	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  695	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  696	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  697	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  698	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  699	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  700	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  701	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  702	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  703	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  704	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  705	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  706	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  707	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  708	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  709	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  710	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  711	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  712	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  713	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  714	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  715	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  716	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  717	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  718	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  719	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  720	//RCV36-ң�ط�У�̵���36
#define	            RW_RCV37  721	//RCV37-ң�ط�У�̵���37
#define	            RW_RCV38  722	//RCV38-ң�ط�У�̵���38
#define	            RW_RCV39  723	//RCV39-ң�ط�У�̵���39
#define	            RW_RCV40  724	//RCV40-ң�ط�У�̵���40
#define	            RW_RCV41  725	//RCV41-ң�ط�У�̵���41
#define	            RW_RCV42  726	//RCV42-ң�ط�У�̵���42
#define	            RW_RCV43  727	//RCV43-ң�ط�У�̵���43
#define	            RW_RCV44  728	//RCV44-ң�ط�У�̵���44
#define	              RW_GE1  729	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  730	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  731	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  732	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  733	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  734	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  735	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  736	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  737	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  738	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  739	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  740	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  741	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  742	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  743	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  744	//CONST8-����ʱ��̵���8
#define	              RW_TX1  745	//TX1-�������Ӽ̵�����1
#define	              RW_TX2  746	//TX2-�������Ӽ̵�����2
#define	              RW_TX3  747	//TX3-�������Ӽ̵�����3
#define	              RW_TX4  748	//TX4-�������Ӽ̵�����4
#define	              RW_TX5  749	//TX5-�������Ӽ̵�����5
#define	              RW_TX6  750	//TX6-�������Ӽ̵�����6
#define	              RW_TX7  751	//TX7-�������Ӽ̵�����7
#define	              RW_TX8  752	//TX8-�������Ӽ̵�����8
#define	              RW_TX9  753	//TX9-�������Ӽ̵�����9
#define	             RW_TX10  754	//TX10-�������Ӽ̵�����10
#define	             RW_TX11  755	//TX11-�������Ӽ̵�����11
#define	             RW_TX12  756	//TX12-�������Ӽ̵�����12
#define	             RW_TX13  757	//TX13-�������Ӽ̵�����13
#define	             RW_TX14  758	//TX14-�������Ӽ̵�����14
#define	             RW_TX15  759	//TX15-�������Ӽ̵�����15
#define	             RW_TX16  760	//TX16-�������Ӽ̵�����16
#define	              RW_RX1  761	//RX1-�������Ӽ̵�����1
#define	              RW_RX2  762	//RX2-�������Ӽ̵�����2
#define	              RW_RX3  763	//RX3-�������Ӽ̵�����3
#define	              RW_RX4  764	//RX4-�������Ӽ̵�����4
#define	              RW_RX5  765	//RX5-�������Ӽ̵�����5
#define	              RW_RX6  766	//RX6-�������Ӽ̵�����6
#define	              RW_RX7  767	//RX7-�������Ӽ̵�����7
#define	              RW_RX8  768	//RX8-�������Ӽ̵�����8
#define	              RW_RX9  769	//RX9-�������Ӽ̵�����9
#define	             RW_RX10  770	//RX10-�������Ӽ̵�����10
#define	             RW_RX11  771	//RX11-�������Ӽ̵�����11
#define	             RW_RX12  772	//RX12-�������Ӽ̵�����12
#define	             RW_RX13  773	//RX13-�������Ӽ̵�����13
#define	             RW_RX14  774	//RX14-�������Ӽ̵�����14
#define	             RW_RX15  775	//RX15-�������Ӽ̵�����15
#define	             RW_RX16  776	//RX16-�������Ӽ̵�����16
#define	             RW_CNT1  777	//CNT1-�����̵�����1
#define	             RW_CNT2  778	//CNT2-�����̵�����2
#define	             RW_CNT3  779	//CNT3-�����̵�����3
#define	             RW_CNT4  780	//CNT4-�����̵�����4
#define	             RW_CNT5  781	//CNT5-�����̵�����5
#define	             RW_CNT6  782	//CNT6-�����̵�����6
#define	             RW_CNT7  783	//CNT7-�����̵�����7
#define	             RW_CNT8  784	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			785		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			106	

//�м�̵�����
#define	MID_RELAY_WORD_START			108	
#define	MID_RELAY_WORD_NUM			115	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			223	
#define	KEEP_RELAY_WORD_NUM			35	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			258	
#define	TIME_RELAY_WORD_NUM			35	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			293	
#define	TRIP_RELAY_WORD_NUM			4	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			297	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			329	
#define	INPUT_RELAY_WORD_NUM			82	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			411	
#define	OUTPUT_RELAY_WORD_NUM			41	

//ѹ��̵���
#define	SW_RELAY_WORD_START			452	
#define	SW_RELAY_WORD_NUM			11	

//���ü̵���
#define	CFG_RELAY_WORD_START			463	
#define	CFG_RELAY_WORD_NUM			25	

//����̵���
#define	PULSE_RELAY_WORD_START			488	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			504	
#define	CTRLTEST_RELAY_WORD_NUM			21	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			525	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			541	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			565	
#define	VIN_RELAY_WORD_NUM			32	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			597	
#define	RCP_RELAY_WORD_NUM			44	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			641	
#define	RCE_RELAY_WORD_NUM			44	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			685	
#define	RCV_RELAY_WORD_NUM			44	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			729	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			737	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			745	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			761	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			777	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			777	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			777	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			777	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			777	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			777	
#define	CNT_RELAY_WORD_NUM			8	


#endif