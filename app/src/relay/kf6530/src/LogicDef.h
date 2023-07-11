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
#define	        RW_LK68IRAH2   22	//LK68IRAH2-A�ƶ���������г�������̵�����
#define	         RW_LK68IDAH   23	//LK68IDAH-A�����ۺ�г�������̵�����
#define	        RW_LK68IDBH2   24	//LK68IDBH2-B��������г�������̵�����
#define	        RW_LK68IDBH5   25	//LK68IDBH5-B�������г�������̵�����
#define	        RW_LK68IRBH2   26	//LK68IRBH2-B�ƶ���������г�������̵�����
#define	         RW_LK68IDBH   27	//LK68IDBH-B�����ۺ�г�������̵�����
#define	        RW_LK68IDCH2   28	//LK68IDCH2-C��������г�������̵�����
#define	        RW_LK68IDCH5   29	//LK68IDCH5-C�������г�������̵�����
#define	        RW_LK68IRCH2   30	//LK68IRCH2-C�ƶ��������δ�г�������̵�����
#define	         RW_LK68IDCH   31	//LK68IDCH-C�����ۺ�г�������̵�����
#define	             RW_AR87   32	//AR87-����Խ�޸澯�̵�����
#define	           RW_PICKUP   33	//PICKUP-ģֵͻ���������̵�����
#define	             RW_50HA   34	//50HA-IHA�����̵�����
#define	             RW_37HA   35	//37HA-IHA�����̵�����
#define	             RW_50HB   36	//50HB-IHB�����̵�����
#define	             RW_37HB   37	//37HB-IHB�����̵�����
#define	             RW_50HC   38	//50HC-IHC�����̵�����
#define	             RW_37HC   39	//37HC-IHC�����̵�����
#define	             RW_50L1   40	//50L1-IL1�����̵�����
#define	             RW_37L1   41	//37L1-IL1�����̵�����
#define	             RW_50L2   42	//50L2-IL2�����̵�����
#define	             RW_37L2   43	//37L2-IL2�����̵�����
#define	             RW_50L3   44	//50L3-IL3�����̵�����
#define	             RW_37L3   45	//37L3-IL3�����̵�����
#define	             RW_50L4   46	//50L4-IL4�����̵�����
#define	             RW_37L4   47	//37L4-IL4�����̵�����
#define	          RW_ET11ERR   48	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   49	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   50	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   51	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   52	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   53	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   54	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   55	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   56	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   57	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   58	//COM1ERR-��������״̬�̵�����
#define	          RW_CAN1ERR   59	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   60	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   61	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   62	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   63	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK   64	//MAINTLINK-ά��״̬�̵�����
#define	             RW_59UA   65	//59UA-UA��ѹ�̵�����
#define	             RW_27UA   66	//27UA-UA��ѹ�̵�����
#define	             RW_59UB   67	//59UB-UB��ѹ�̵�����
#define	             RW_27UB   68	//27UB-UB��ѹ�̵�����
#define	             RW_59UC   69	//59UC-UC��ѹ�̵�����
#define	             RW_27UC   70	//27UC-UC��ѹ�̵�����
#define	        RW_LKPI87U_A   71	//LKPI87U_A-A���ٶ���α����̵�����
#define	       RW_LKRST87U_A   72	//LKRST87U_A-A���ٶϷ��ر����̵�����
#define	        RW_LKPI87U_B   73	//LKPI87U_B-B���ٶ���α����̵�����
#define	       RW_LKRST87U_B   74	//LKRST87U_B-B���ٶϷ��ر����̵�����
#define	        RW_LKPI87U_C   75	//LKPI87U_C-C���ٶ���α����̵�����
#define	       RW_LKRST87U_C   76	//LKRST87U_C-C���ٶϷ��ر����̵�����
#define	        RW_LKPI87R_A   77	//LKPI87R_A-A����ʲ��α����̵�����
#define	       RW_LKRST87R_A   78	//LKRST87R_A-A����ʲ���ر����̵�����
#define	        RW_LKPI87R_B   79	//LKPI87R_B-B����ʲ��α����̵�����
#define	       RW_LKRST87R_B   80	//LKRST87R_B-B����ʲ���ر����̵�����
#define	        RW_LKPI87R_C   81	//LKPI87R_C-C����ʲ��α����̵�����
#define	       RW_LKRST87R_C   82	//LKRST87R_C-C����ʲ���ر����̵�����
#define	               RW_M1   83	//M1-�м�̵�����M1
#define	               RW_M2   84	//M2-�м�̵�����M2
#define	               RW_M3   85	//M3-�м�̵�����M3
#define	               RW_M4   86	//M4-�м�̵�����M4
#define	               RW_M5   87	//M5-�м�̵�����M5
#define	               RW_M6   88	//M6-�м�̵�����M6
#define	               RW_M7   89	//M7-�м�̵�����M7
#define	               RW_M8   90	//M8-�м�̵�����M8
#define	               RW_M9   91	//M9-�м�̵�����M9
#define	              RW_M10   92	//M10-�м�̵�����M10
#define	              RW_M11   93	//M11-�м�̵�����M11
#define	              RW_M12   94	//M12-�м�̵�����M12
#define	              RW_M13   95	//M13-�м�̵�����M13
#define	              RW_M14   96	//M14-�м�̵�����M14
#define	              RW_M15   97	//M15-�м�̵�����M15
#define	              RW_M16   98	//M16-�м�̵�����M16
#define	              RW_M17   99	//M17-�м�̵�����M17
#define	              RW_M18  100	//M18-�м�̵�����M18
#define	              RW_M19  101	//M19-�м�̵�����M19
#define	              RW_M20  102	//M20-�м�̵�����M20
#define	              RW_M21  103	//M21-�м�̵�����M21
#define	              RW_M22  104	//M22-�м�̵�����M22
#define	              RW_M23  105	//M23-�м�̵�����M23
#define	              RW_M24  106	//M24-�м�̵�����M24
#define	              RW_M25  107	//M25-�м�̵�����M25
#define	              RW_M26  108	//M26-�м�̵�����M26
#define	              RW_M27  109	//M27-�м�̵�����M27
#define	              RW_M28  110	//M28-�м�̵�����M28
#define	              RW_M29  111	//M29-�м�̵�����M29
#define	              RW_M30  112	//M30-�м�̵�����M30
#define	              RW_M31  113	//M31-�м�̵�����M31
#define	              RW_M32  114	//M32-�м�̵�����M32
#define	              RW_M33  115	//M33-�м�̵�����M33
#define	              RW_M34  116	//M34-�м�̵�����M34
#define	              RW_M35  117	//M35-�м�̵�����M35
#define	              RW_M36  118	//M36-�м�̵�����M36
#define	              RW_M37  119	//M37-�м�̵�����M37
#define	              RW_M38  120	//M38-�м�̵�����M38
#define	              RW_M39  121	//M39-�м�̵�����M39
#define	              RW_M40  122	//M40-�м�̵�����M40
#define	              RW_M41  123	//M41-�м�̵�����M41
#define	              RW_M42  124	//M42-�м�̵�����M42
#define	              RW_M43  125	//M43-�м�̵�����M43
#define	              RW_M44  126	//M44-�м�̵�����M44
#define	              RW_M45  127	//M45-�м�̵�����M45
#define	              RW_M46  128	//M46-�м�̵�����M46
#define	              RW_M47  129	//M47-�м�̵�����M47
#define	              RW_M48  130	//M48-�м�̵�����M48
#define	              RW_M49  131	//M49-�м�̵�����M49
#define	              RW_M50  132	//M50-�м�̵�����M50
#define	              RW_M51  133	//M51-�м�̵�����M51
#define	              RW_M52  134	//M52-�м�̵�����M52
#define	              RW_M53  135	//M53-�м�̵�����M53
#define	              RW_M54  136	//M54-�м�̵�����M54
#define	              RW_M55  137	//M55-�м�̵�����M55
#define	              RW_M56  138	//M56-�м�̵�����M56
#define	              RW_M57  139	//M57-�м�̵�����M57
#define	              RW_M58  140	//M58-�м�̵�����M58
#define	              RW_M59  141	//M59-�м�̵�����M59
#define	              RW_M60  142	//M60-�м�̵�����M60
#define	              RW_M61  143	//M61-�м�̵�����M61
#define	              RW_M62  144	//M62-�м�̵�����M62
#define	              RW_M63  145	//M63-�м�̵�����M63
#define	              RW_M64  146	//M64-�м�̵�����M64
#define	              RW_REC  147	//REC-¼���̵�����
#define	             RW_69IN  148	//69IN-Զ�������м�̵�����
#define	             RW_52AH  149	//52AH-��λ
#define	             RW_52BH  150	//52BH-��λ
#define	            RW_52ALA  151	//52ALA-�Ͳ�A��·����λ
#define	            RW_52BLA  152	//52BLA-�Ͳ�A��·����λ
#define	            RW_52ALB  153	//52ALB-�Ͳ�B��·����λ
#define	            RW_52BLB  154	//52BLB-�Ͳ�B��·����λ
#define	               RW_H1  155	//H1-�Ա����м�̵�����1
#define	               RW_H2  156	//H2-�Ա����м�̵�����2
#define	               RW_H3  157	//H3-�Ա����м�̵�����3
#define	               RW_H4  158	//H4-�Ա����м�̵�����4
#define	               RW_H5  159	//H5-�Ա����м�̵�����5
#define	               RW_H6  160	//H6-�Ա����м�̵�����6
#define	               RW_H7  161	//H7-�Ա����м�̵�����7
#define	               RW_H8  162	//H8-�Ա����м�̵�����8
#define	               RW_H9  163	//H9-�Ա����м�̵�����9
#define	              RW_H10  164	//H10-�Ա����м�̵�����10
#define	              RW_H11  165	//H11-�Ա����м�̵�����11
#define	              RW_H12  166	//H12-�Ա����м�̵�����12
#define	              RW_H13  167	//H13-�Ա����м�̵�����13
#define	              RW_H14  168	//H14-�Ա����м�̵�����14
#define	              RW_H15  169	//H15-�Ա����м�̵�����15
#define	              RW_H16  170	//H16-�Ա����м�̵�����16
#define	              RW_H17  171	//H17-�Ա����м�̵�����17
#define	              RW_H18  172	//H18-�Ա����м�̵�����18
#define	              RW_H19  173	//H19-�Ա����м�̵�����19
#define	              RW_H20  174	//H20-�Ա����м�̵�����20
#define	              RW_H21  175	//H21-�Ա����м�̵�����21
#define	              RW_H22  176	//H22-�Ա����м�̵�����22
#define	              RW_H23  177	//H23-�Ա����м�̵�����23
#define	              RW_H24  178	//H24-�Ա����м�̵�����24
#define	              RW_H25  179	//H25-�Ա����м�̵�����25
#define	              RW_H26  180	//H26-�Ա����м�̵�����26
#define	              RW_H27  181	//H27-�Ա����м�̵�����27
#define	              RW_H28  182	//H28-�Ա����м�̵�����28
#define	              RW_H29  183	//H29-�Ա����м�̵�����29
#define	              RW_H30  184	//H30-�Ա����м�̵�����30
#define	              RW_H31  185	//H31-�Ա����м�̵�����31
#define	              RW_H32  186	//H32-�Ա����м�̵�����31
#define	            RW_52BFH  187	//52BFH-�߲��·����բʧ�̵ܼ�����
#define	           RW_52BFLA  188	//52BFLA-�Ͳ�A��·����բʧ�̵ܼ�����
#define	           RW_52BFLB  189	//52BFLB-�Ͳ�B��·����բʧ�̵ܼ�����
#define	               RW_T1  190	//T1-T1
#define	               RW_T2  191	//T2-T2
#define	               RW_T3  192	//T3-T3
#define	               RW_T4  193	//T4-T4
#define	               RW_T5  194	//T5-T5
#define	               RW_T6  195	//T6-T6
#define	               RW_T7  196	//T7-T7
#define	               RW_T8  197	//T8-T8
#define	               RW_T9  198	//T9-T9
#define	              RW_T10  199	//T10-T10
#define	              RW_T11  200	//T11-T11
#define	              RW_T12  201	//T12-T12
#define	              RW_T13  202	//T13-T13
#define	              RW_T14  203	//T14-T14
#define	              RW_T15  204	//T15-T15
#define	              RW_T16  205	//T16-T16
#define	             RW_TCB1  206	//TCB1-TCB1
#define	             RW_TCB2  207	//TCB2-TCB2
#define	             RW_TCB3  208	//TCB3-TCB3
#define	             RW_TCB4  209	//TCB4-TCB4
#define	             RW_TCB5  210	//TCB5-TCB5
#define	             RW_TCB6  211	//TCB6-TCB6
#define	             RW_TCB7  212	//TCB7-TCB7
#define	             RW_TCB8  213	//TCB8-TCB8
#define	             RW_TSW1  214	//TSW1-TSW1
#define	             RW_TSW2  215	//TSW2-TSW2
#define	             RW_TSW3  216	//TSW3-TSW3
#define	             RW_TSW4  217	//TSW4-TSW4
#define	             RW_TSW5  218	//TSW5-TSW5
#define	             RW_TSW6  219	//TSW6-TSW6
#define	             RW_TSW7  220	//TSW7-TSW7
#define	             RW_TSW8  221	//TSW8-TSW8
#define	            RW_T52BF  222	//T52BF-ʧ�鱣��ʱ��
#define	              RW_TR1  223	//TR1-��բ�̵���1
#define	              RW_TR2  224	//TR2-��բ�̵���2
#define	              RW_TR3  225	//TR3-��բ�̵���3
#define	              RW_VR1  226	//VR1-����̵�����1
#define	              RW_VR2  227	//VR2-����̵�����2
#define	              RW_VR3  228	//VR3-����̵�����3
#define	              RW_VR4  229	//VR4-����̵�����4
#define	              RW_VR5  230	//VR5-����̵�����5
#define	              RW_VR6  231	//VR6-����̵�����6
#define	              RW_VR7  232	//VR7-����̵�����7
#define	              RW_VR8  233	//VR8-����̵�����8
#define	              RW_VR9  234	//VR9-����̵�����9
#define	             RW_VR10  235	//VR10-����̵�����10
#define	             RW_VR11  236	//VR11-����̵�����11
#define	             RW_VR12  237	//VR12-����̵�����12
#define	             RW_VR13  238	//VR13-����̵�����13
#define	             RW_VR14  239	//VR14-����̵�����14
#define	             RW_VR15  240	//VR15-����̵�����15
#define	             RW_VR16  241	//VR16-����̵�����16
#define	             RW_VR17  242	//VR17-����̵�����17
#define	             RW_VR18  243	//VR18-����̵�����18
#define	             RW_VR19  244	//VR19-����̵�����19
#define	             RW_VR20  245	//VR20-����̵�����20
#define	             RW_VR21  246	//VR21-����̵�����21
#define	             RW_VR22  247	//VR22-����̵�����22
#define	             RW_VR23  248	//VR23-����̵�����23
#define	             RW_VR24  249	//VR24-����̵�����24
#define	             RW_VR25  250	//VR25-����̵�����25
#define	             RW_VR26  251	//VR26-����̵�����26
#define	             RW_VR27  252	//VR27-����̵�����27
#define	             RW_VR28  253	//VR28-����̵�����28
#define	             RW_VR29  254	//VR29-����̵�����29
#define	             RW_VR30  255	//VR30-����̵�����30
#define	             RW_VR31  256	//VR31-����̵�����31
#define	             RW_VR32  257	//VR32-����̵�����32
#define	              RW_IN1  258	//IN1-����̵�����1
#define	              RW_IN2  259	//IN2-����̵�����2
#define	              RW_IN3  260	//IN3-����̵�����3
#define	              RW_IN4  261	//IN4-����̵�����4
#define	              RW_IN5  262	//IN5-����̵�����5
#define	              RW_IN6  263	//IN6-����̵�����6
#define	              RW_IN7  264	//IN7-����̵�����7
#define	              RW_IN8  265	//IN8-����̵�����8
#define	              RW_IN9  266	//IN9-����̵�����9
#define	             RW_IN10  267	//IN10-����̵�����10
#define	             RW_IN11  268	//IN11-����̵�����11
#define	             RW_IN12  269	//IN12-����̵�����12
#define	             RW_IN13  270	//IN13-����̵�����13
#define	             RW_IN14  271	//IN14-����̵�����14
#define	             RW_IN15  272	//IN15-����̵�����15
#define	             RW_IN16  273	//IN16-����̵�����16
#define	             RW_IN17  274	//IN17-����̵�����17
#define	             RW_IN18  275	//IN18-����̵�����18
#define	             RW_IN19  276	//IN19-����̵�����19
#define	             RW_IN20  277	//IN20-����̵�����20
#define	             RW_IN21  278	//IN21-����̵�����21
#define	             RW_IN22  279	//IN22-����̵�����22
#define	             RW_IN23  280	//IN23-����̵�����23
#define	             RW_IN24  281	//IN24-����̵�����24
#define	             RW_IN25  282	//IN25-����̵�����25
#define	             RW_IN26  283	//IN26-����̵�����26
#define	             RW_IN27  284	//IN27-����̵�����27
#define	             RW_IN28  285	//IN28-����̵�����28
#define	             RW_IN29  286	//IN29-����̵�����29
#define	             RW_IN30  287	//IN30-����̵�����30
#define	             RW_IN31  288	//IN31-����̵�����31
#define	             RW_IN32  289	//IN32-����̵�����32
#define	             RW_IN33  290	//IN33-����̵�����33
#define	             RW_IN34  291	//IN34-����̵�����34
#define	             RW_IN35  292	//IN35-�źŸ���
#define	             RW_IN36  293	//IN36-����˹2
#define	             RW_IN37  294	//IN37-ѹ���ͷ�2
#define	             RW_IN38  295	//IN38-������բ2
#define	             RW_IN39  296	//IN39-����˹2
#define	             RW_IN40  297	//IN40-����˹1
#define	             RW_IN41  298	//IN41-ѹ���ͷ�1
#define	             RW_IN42  299	//IN42-������բ1
#define	             RW_IN43  300	//IN43-����˹1
#define	             RW_IN44  301	//IN44-��λ�쳣1
#define	             RW_IN45  302	//IN45-�¶ȹ���1
#define	             RW_IN46  303	//IN46-ʧ��澯1
#define	             RW_IN47  304	//IN47-��λ�쳣2
#define	             RW_IN48  305	//IN48-�¶ȹ���2
#define	             RW_IN49  306	//IN49-ʧ��澯2
#define	           RW_INVIN1  307	//INVIN1-����̵�����50
#define	           RW_INVIN2  308	//INVIN2-����̵�����51
#define	           RW_INVIN3  309	//INVIN3-����̵�����52
#define	           RW_INVIN4  310	//INVIN4-����̵�����53
#define	           RW_INVIN5  311	//INVIN5-����̵�����54
#define	           RW_INVIN6  312	//INVIN6-����̵�����55
#define	           RW_INVIN7  313	//INVIN7-����̵�����56
#define	           RW_INVIN8  314	//INVIN8-����̵�����57
#define	           RW_INVIN9  315	//INVIN9-����̵�����58
#define	          RW_INVIN10  316	//INVIN10-����̵�����59
#define	          RW_INVIN11  317	//INVIN11-����̵�����60
#define	          RW_INVIN12  318	//INVIN12-����̵�����61
#define	          RW_INVIN13  319	//INVIN13-����̵�����62
#define	          RW_INVIN14  320	//INVIN14-����̵�����63
#define	          RW_INVIN15  321	//INVIN15-����̵�����64
#define	          RW_INVIN16  322	//INVIN16-����̵�����65
#define	             RW_OUT1  323	//OUT1-����1�̵�����
#define	             RW_OUT2  324	//OUT2-����2�̵�����
#define	             RW_OUT3  325	//OUT3-����3�̵�����
#define	             RW_OUT4  326	//OUT4-����4�̵�����
#define	             RW_OUT5  327	//OUT5-����5�̵�����
#define	             RW_OUT6  328	//OUT6-����6�̵�����
#define	             RW_OUT7  329	//OUT7-����7�̵�����
#define	             RW_OUT8  330	//OUT8-����8�̵�����
#define	             RW_OUT9  331	//OUT9-����9�̵�����
#define	            RW_OUT10  332	//OUT10-����10�̵�����
#define	            RW_OUT11  333	//OUT11-����11�̵�����
#define	            RW_OUT12  334	//OUT12-����12�̵�����
#define	            RW_OUT13  335	//OUT13-����13�̵�����
#define	            RW_OUT14  336	//OUT14-����14�̵�����
#define	            RW_OUT15  337	//OUT15-����15�̵�����
#define	            RW_OUT16  338	//OUT16-����16�̵�����
#define	            RW_OUT17  339	//OUT17-����17�̵�����
#define	            RW_OUT18  340	//OUT18-����18�̵�����
#define	            RW_OUT19  341	//OUT19-����19�̵�����
#define	            RW_OUT20  342	//OUT20-����20�̵�����
#define	            RW_OUT21  343	//OUT21-����21�̵�����
#define	            RW_SL87U  344	//SL87U-�����ٶ���ѹ��̵�����
#define	            RW_SL87R  345	//SL87R-���ʲ��ѹ��̵�����
#define	          RW_SLMAINT  346	//SLMAINT-����ѹ��
#define	           RW_SLBAK1  347	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  348	//SLBAK2-����ѹ��2
#define	           RW_F87U_A  349	//F87U_A-A���ٶ����ü̵�����
#define	           RW_F87U_B  350	//F87U_B-B������ٶ����ü̵�����
#define	           RW_F87U_C  351	//F87U_C-C������ٶ����ü̵�����
#define	           RW_F87R_A  352	//F87R_A-A����ʲ���ü̵�����
#define	           RW_F87R_B  353	//F87R_B-B����ʲ���ü̵�����
#define	           RW_F87R_C  354	//F87R_C-C����ʲ���ü̵�����
#define	         RW_F68IDH2M  355	//F68IDH2M-��������г�������������ü̵�����
#define	         RW_F68IDH2C  356	//F68IDH2C-��������г���ۺ���������ü̵�����
#define	         RW_F68IDH2I  357	//F68IDH2I-��������г������������ü̵�����
#define	         RW_F68IRH2I  358	//F68IRH2I-�ƶ�����г���������ü̵�����
#define	         RW_F68IDH5I  359	//F68IDH5I-�������г���������ü̵�����
#define	         RW_F68IDMUL  360	//F68IDMUL-�����ۺ�г������
#define	            RW_F52BF  361	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  362	//FBAK1-��������1
#define	            RW_FBAK2  363	//FBAK2-��������2
#define	              RW_PW1  364	//PW1-����̵���1
#define	              RW_PW2  365	//PW2-����̵���2
#define	              RW_PW3  366	//PW3-����̵���3
#define	              RW_PW4  367	//PW4-����̵���4
#define	              RW_PW5  368	//PW5-����̵���5
#define	              RW_PW6  369	//PW6-����̵���6
#define	              RW_PW7  370	//PW7-����̵���7
#define	              RW_PW8  371	//PW8-����̵���8
#define	              RW_PW9  372	//PW9-����̵���9
#define	             RW_PW10  373	//PW10-����̵���10
#define	             RW_PW11  374	//PW11-����̵���11
#define	             RW_PW12  375	//PW12-����̵���12
#define	             RW_PW13  376	//PW13-����̵���13
#define	             RW_PW14  377	//PW14-����̵���14
#define	             RW_PW15  378	//PW15-����̵���15
#define	             RW_PW16  379	//PW16-����̵���16
#define	              RW_TO1  380	//TO1-���ʲ����
#define	              RW_TO2  381	//TO2-��ٶ϶���
#define	              RW_TO3  382	//TO3-��������
#define	              RW_TO4  383	//TO4-��źŸ���
#define	              RW_TO5  384	//TO5-ʧ�鱣������
#define	              RW_TO6  385	//TO6-����1����
#define	              RW_TO7  386	//TO7-����1����1
#define	              RW_TO8  387	//TO8-����1����2
#define	              RW_TO9  388	//TO9-����1����3
#define	             RW_TO10  389	//TO10-����1����4
#define	             RW_TO11  390	//TO11-����2����
#define	             RW_TO12  391	//TO12-����2����1
#define	             RW_TO13  392	//TO13-����2����2
#define	             RW_TO14  393	//TO14-����2����3
#define	             RW_TO15  394	//TO15-����2����4
#define	             RW_SIG1  395	//SIG1-�źż̵���1
#define	             RW_SIG2  396	//SIG2-�źż̵���2
#define	             RW_SIG3  397	//SIG3-�źż̵���3
#define	             RW_SIG4  398	//SIG4-�źż̵���4
#define	             RW_SIG5  399	//SIG5-�źż̵���5
#define	             RW_SIG6  400	//SIG6-�źż̵���6
#define	             RW_SIG7  401	//SIG7-�źż̵���7
#define	             RW_SIG8  402	//SIG8-�źż̵���8
#define	             RW_SIG9  403	//SIG9-�źż̵���9
#define	            RW_SIG10  404	//SIG10-�źż̵���10
#define	            RW_SIG11  405	//SIG11-�źż̵���11
#define	            RW_SIG12  406	//SIG12-�źż̵���12
#define	            RW_SIG13  407	//SIG13-�źż̵���13
#define	            RW_SIG14  408	//SIG14-�źż̵���14
#define	            RW_SIG15  409	//SIG15-�źż̵���15
#define	            RW_SIG16  410	//SIG16-�źż̵���16
#define	            RW_LED1G  411	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  412	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  413	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  414	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  415	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  416	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  417	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  418	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  419	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  420	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  421	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  422	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  423	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  424	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  425	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  426	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  427	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  428	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  429	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  430	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  431	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  432	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  433	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  434	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  435	//VIN1-���⿪��̵�����1
#define	             RW_VIN2  436	//VIN2-���⿪��̵�����2
#define	             RW_VIN3  437	//VIN3-���⿪��̵�����3
#define	             RW_VIN4  438	//VIN4-���⿪��̵�����4
#define	             RW_VIN5  439	//VIN5-���⿪��̵�����5
#define	             RW_VIN6  440	//VIN6-���⿪��̵�����6
#define	             RW_VIN7  441	//VIN7-���⿪��̵�����7
#define	             RW_VIN8  442	//VIN8-���⿪��̵�����8
#define	             RW_VIN9  443	//VIN9-���⿪��̵�����9
#define	            RW_VIN10  444	//VIN10-���⿪��̵�����10
#define	            RW_VIN11  445	//VIN11-���⿪��̵�����11
#define	            RW_VIN12  446	//VIN12-���⿪��̵�����12
#define	            RW_VIN13  447	//VIN13-���⿪��̵�����13
#define	            RW_VIN14  448	//VIN14-���⿪��̵�����14
#define	            RW_VIN15  449	//VIN15-���⿪��̵�����15
#define	            RW_VIN16  450	//VIN16-���⿪��̵�����16
#define	             RW_RCP1  451	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  452	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  453	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  454	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  455	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  456	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  457	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  458	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  459	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  460	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  461	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  462	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  463	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  464	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  465	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  466	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  467	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  468	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  469	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  470	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  471	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  472	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  473	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  474	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  475	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  476	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  477	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  478	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  479	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  480	//RCP30-ң��Ԥ�ü̵���30
#define	             RW_RCE1  481	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  482	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  483	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  484	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  485	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  486	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  487	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  488	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  489	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  490	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  491	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  492	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  493	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  494	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  495	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  496	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  497	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  498	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  499	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  500	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  501	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  502	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  503	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  504	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  505	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  506	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  507	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  508	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  509	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  510	//RCE30-ң��ִ�м̵���30
#define	             RW_RCV1  511	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  512	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  513	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  514	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  515	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  516	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  517	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  518	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  519	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  520	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  521	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  522	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  523	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  524	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  525	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  526	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  527	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  528	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  529	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  530	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  531	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  532	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  533	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  534	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  535	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  536	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  537	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  538	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  539	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  540	//RCV30-ң�ط�У�̵���30
#define	              RW_GE1  541	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  542	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  543	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  544	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  545	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  546	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  547	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  548	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  549	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  550	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  551	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  552	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  553	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  554	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  555	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  556	//CONST8-����ʱ��̵���8
#define	              RW_TX1  557	//TX1-�����ͼ̵�����1
#define	              RW_TX2  558	//TX2-�����ͼ̵�����2
#define	              RW_TX3  559	//TX3-�����ͼ̵�����3
#define	              RW_TX4  560	//TX4-�����ͼ̵�����4
#define	              RW_TX5  561	//TX5-�����ͼ̵�����5
#define	              RW_TX6  562	//TX6-�����ͼ̵�����6
#define	              RW_TX7  563	//TX7-�����ͼ̵�����7
#define	              RW_TX8  564	//TX8-�����ͼ̵�����8
#define	              RW_TX9  565	//TX9-�����ͼ̵�����9
#define	             RW_TX10  566	//TX10-�����ͼ̵�����10
#define	             RW_TX11  567	//TX11-�����ͼ̵�����11
#define	             RW_TX12  568	//TX12-�����ͼ̵�����12
#define	             RW_TX13  569	//TX13-�����ͼ̵�����13
#define	             RW_TX14  570	//TX14-�����ͼ̵�����14
#define	             RW_TX15  571	//TX15-�����ͼ̵�����15
#define	             RW_TX16  572	//TX16-�����ͼ̵�����16
#define	              RW_RX1  573	//RX1-������ռ̵�����1
#define	              RW_RX2  574	//RX2-������ռ̵�����2
#define	              RW_RX3  575	//RX3-������ռ̵�����3
#define	              RW_RX4  576	//RX4-������ռ̵�����4
#define	              RW_RX5  577	//RX5-������ռ̵�����5
#define	              RW_RX6  578	//RX6-������ռ̵�����6
#define	              RW_RX7  579	//RX7-������ռ̵�����7
#define	              RW_RX8  580	//RX8-������ռ̵�����8
#define	              RW_RX9  581	//RX9-������ռ̵�����9
#define	             RW_RX10  582	//RX10-������ռ̵�����10
#define	             RW_RX11  583	//RX11-������ռ̵�����11
#define	             RW_RX12  584	//RX12-������ռ̵�����12
#define	             RW_RX13  585	//RX13-������ռ̵�����13
#define	             RW_RX14  586	//RX14-������ռ̵�����14
#define	             RW_RX15  587	//RX15-������ռ̵�����15
#define	             RW_RX16  588	//RX16-������ռ̵�����16
#define	             RW_CNT1  589	//CNT1-�����̵�����1
#define	             RW_CNT2  590	//CNT2-�����̵�����2
#define	             RW_CNT3  591	//CNT3-�����̵�����3
#define	             RW_CNT4  592	//CNT4-�����̵�����4
#define	             RW_CNT5  593	//CNT5-�����̵�����5
#define	             RW_CNT6  594	//CNT6-�����̵�����6
#define	             RW_CNT7  595	//CNT7-�����̵�����7
#define	             RW_CNT8  596	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			597		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			69	

//�м�̵�����
#define	MID_RELAY_WORD_START			71	
#define	MID_RELAY_WORD_NUM			84	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			155	
#define	KEEP_RELAY_WORD_NUM			35	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			190	
#define	TIME_RELAY_WORD_NUM			33	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			223	
#define	TRIP_RELAY_WORD_NUM			3	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			226	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			258	
#define	INPUT_RELAY_WORD_NUM			65	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			323	
#define	OUTPUT_RELAY_WORD_NUM			21	

//ѹ��̵���
#define	SW_RELAY_WORD_START			344	
#define	SW_RELAY_WORD_NUM			5	

//���ü̵���
#define	CFG_RELAY_WORD_START			349	
#define	CFG_RELAY_WORD_NUM			15	

//����̵���
#define	PULSE_RELAY_WORD_START			364	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			380	
#define	CTRLTEST_RELAY_WORD_NUM			15	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			395	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			411	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			435	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			451	
#define	RCP_RELAY_WORD_NUM			30	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			481	
#define	RCE_RELAY_WORD_NUM			30	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			511	
#define	RCV_RELAY_WORD_NUM			30	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			541	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			549	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			557	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			573	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			589	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			589	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			589	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			589	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			589	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			589	
#define	CNT_RELAY_WORD_NUM			8	


#endif