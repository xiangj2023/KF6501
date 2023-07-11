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
#define	          RW_LPC50_0    2	//LPC50_0-�����ٶ�ѭ�����Ƽ̵�����
#define	          RW_LPC50_1    3	//LPC50_1-��������ѭ�����Ƽ̵�����
#define	            RW_LPC59    4	//LPC59-��ѹ����ѭ�����Ƽ̵�����
#define	            RW_LPC27    5	//LPC27-��ѹ����ѭ�����Ƽ̵�����
#define	           RW_LPC50H    6	//LPC50H-г����������ѭ�����Ƽ̵�����
#define	          RW_LPC60I1    7	//LPC60I1-֧·1��������ѭ�����Ƽ̵�����
#define	          RW_LPC60I2    8	//LPC60I2-֧·2��������ѭ�����Ƽ̵�����
#define	          RW_LPC50ST    9	//LPC50ST-֧·��������ѭ�����Ƽ̵�����
#define	          RW_LPC60U1   10	//LPC60U1-֧·1��ѹ����ѭ�����Ƽ̵�����
#define	          RW_LPC60U2   11	//LPC60U2-֧·2��ѹ����ѭ�����Ƽ̵�����
#define	          RW_LPC60U3   12	//LPC60U3-֧·3��ѹ����ѭ�����Ƽ̵�����
#define	           RW_PI50_0   13	//PI50_0-�����ٶ���μ̵�����
#define	           RW_TR50_0   14	//TR50_0-�����ٶ϶����̵�����
#define	           RW_PI50_1   15	//PI50_1-����������μ̵�����
#define	           RW_TR50_1   16	//TR50_1-�������������̵�����
#define	             RW_PI59   17	//PI59-��ѹ������μ̵�����
#define	             RW_TR59   18	//TR59-��ѹ���������̵�����
#define	             RW_PI27   19	//PI27-��ѹ������μ̵�����
#define	             RW_TR27   20	//TR27-��ѹ���������̵�����
#define	            RW_PI50H   21	//PI50H-г������������μ̵�����
#define	            RW_TR50H   22	//TR50H-г���������������̵�����
#define	           RW_PI60I1   23	//PI60I1-֧·1����������μ̵�����
#define	           RW_TR60I1   24	//TR60I1-֧·1�������������̵�����
#define	           RW_PI60I2   25	//PI60I2-֧·2����������μ̵�����
#define	           RW_TR60I2   26	//TR60I2-֧·2�������������̵�����
#define	           RW_PI50ST   27	//PI50ST-֧·����������μ̵�����
#define	           RW_TR50ST   28	//TR50ST-֧·�������������̵�����
#define	           RW_PI60U1   29	//PI60U1-֧·1��ѹ������μ̵�����
#define	           RW_TR60U1   30	//TR60U1-֧·1��ѹ���������̵�����
#define	           RW_PI60U2   31	//PI60U2-֧·2��ѹ������μ̵�����
#define	           RW_TR60U2   32	//TR60U2-֧·2��ѹ���������̵�����
#define	           RW_PI60U3   33	//PI60U3-֧·3��ѹ������μ̵�����
#define	           RW_TR60U3   34	//TR60U3-֧·3��ѹ���������̵�����
#define	           RW_PI74PT   35	//PI74PT-PT������μ̵�����
#define	           RW_AR74PT   36	//AR74PT-PT���߸澯�̵�����
#define	           RW_PICTBR   37	//PICTBR-CT������μ̵�����
#define	           RW_ARCTBR   38	//ARCTBR-CT���߸澯�̵�����
#define	           RW_PICKUP   39	//PICKUP-ģֵͻ���������̵�����
#define	           RW_BTMODE   40	//BTMODE-ֱ����ʽ�̵�����
#define	           RW_ATMODE   41	//ATMODE-AT���緽ʽ�̵�����
#define	             RW_50IP   42	//50IP-Ip�����̵�����
#define	             RW_37IP   43	//37IP-Ip�����̵�����
#define	             RW_50I1   44	//50I1-I1�����̵�����
#define	             RW_37I1   45	//37I1-I1�����̵�����
#define	             RW_50I2   46	//50I2-I2�����̵�����
#define	             RW_37I2   47	//37I2-I2�����̵�����
#define	             RW_50I3   48	//50I3-I3�����̵�����
#define	             RW_37I3   49	//37I3-I3�����̵�����
#define	             RW_59UP   50	//59UP-Up��ѹ�̵�����
#define	             RW_27UP   51	//27UP-Up��ѹ�̵�����
#define	             RW_59U1   52	//59U1-U1��ѹ�̵�����
#define	             RW_27U1   53	//27U1-U1��ѹ�̵�����
#define	             RW_59U2   54	//59U2-U2��ѹ�̵�����
#define	             RW_27U2   55	//27U2-U2��ѹ�̵�����
#define	            RW_59UD1   56	//59UD1-Ud1��ѹ�̵�����
#define	            RW_27UD1   57	//27UD1-Ud1��ѹ�̵�����
#define	            RW_59UD2   58	//59UD2-Ud2��ѹ�̵�����
#define	            RW_27UD2   59	//27UD2-Ud2��ѹ�̵�����
#define	            RW_59UD3   60	//59UD3-Ud3��ѹ�̵�����
#define	            RW_27UD3   61	//27UD3-Ud3��ѹ�̵�����
#define	          RW_ET11ERR   62	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   63	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   64	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   65	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   66	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   67	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   68	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   69	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   70	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   71	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   72	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   73	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   74	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   75	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   76	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   77	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK   78	//MAINTLINK-ά��״̬�̵�����
#define	         RW_LKPI50_0   79	//LKPI50_0-�����ٶ���α����̵�����
#define	        RW_LKRST50_0   80	//LKRST50_0-�����ٶϷ��ر����̵�����
#define	         RW_LKPI50_1   81	//LKPI50_1-����������α����̵�����
#define	        RW_LKRST50_1   82	//LKRST50_1-�����������ر����̵�����
#define	           RW_LKPI59   83	//LKPI59-��ѹ������α����̵�����
#define	          RW_LKRST59   84	//LKRST59-��ѹ�������ر����̵�����
#define	           RW_LKPI27   85	//LKPI27-��ѹ������α����̵�����
#define	          RW_LKRST27   86	//LKRST27-��ѹ�������ر����̵�����
#define	          RW_LKPI50H   87	//LKPI50H-г��������α����̵�����
#define	         RW_LKRST50H   88	//LKRST50H-г���������ر����̵�����
#define	         RW_LKPI60I1   89	//LKPI60I1-����1��α����̵�����
#define	        RW_LKRST60I1   90	//LKRST60I1-����1���ر����̵�����
#define	         RW_LKPI60I2   91	//LKPI60I2-����2��α����̵�����
#define	        RW_LKRST60I2   92	//LKRST60I2-����2���ر����̵�����
#define	         RW_LKPI50ST   93	//LKPI50ST-֧·������α����̵�����
#define	        RW_LKRST50ST   94	//LKRST50ST-֧·�������ر����̵�����
#define	         RW_LKPI60U1   95	//LKPI60U1-֧·1��ѹ��α����̵�����
#define	        RW_LKRST60U1   96	//LKRST60U1-֧·1��ѹ���ر����̵�����
#define	         RW_LKPI60U2   97	//LKPI60U2-֧·2��ѹ��α����̵�����
#define	        RW_LKRST60U2   98	//LKRST60U2-֧·2��ѹ���ر����̵�����
#define	         RW_LKPI60U3   99	//LKPI60U3-֧·3��ѹ��α����̵�����
#define	        RW_LKRST60U3  100	//LKRST60U3-֧·3��ѹ���ر����̵�����
#define	             RW_69IN  101	//69IN-Զ�������м�̵�����
#define	              RW_52A  102	//52A-��λ
#define	              RW_52B  103	//52B-��λ
#define	               RW_M1  104	//M1-�м�̵�����M1
#define	               RW_M2  105	//M2-�м�̵�����M2
#define	               RW_M3  106	//M3-�м�̵�����M3
#define	               RW_M4  107	//M4-�м�̵�����M4
#define	               RW_M5  108	//M5-�м�̵�����M5
#define	               RW_M6  109	//M6-�м�̵�����M6
#define	               RW_M7  110	//M7-�м�̵�����M7
#define	               RW_M8  111	//M8-�м�̵�����M8
#define	               RW_M9  112	//M9-�м�̵�����M9
#define	              RW_M10  113	//M10-�м�̵�����M10
#define	              RW_M11  114	//M11-�м�̵�����M11
#define	              RW_M12  115	//M12-�м�̵�����M12
#define	              RW_M13  116	//M13-�м�̵�����M13
#define	              RW_M14  117	//M14-�м�̵�����M14
#define	              RW_M15  118	//M15-�м�̵�����M15
#define	              RW_M16  119	//M16-�м�̵�����M16
#define	              RW_M17  120	//M17-�м�̵�����M17
#define	              RW_M18  121	//M18-�м�̵�����M18
#define	              RW_M19  122	//M19-�м�̵�����M19
#define	              RW_M20  123	//M20-�м�̵�����M20
#define	              RW_M21  124	//M21-�м�̵�����M21
#define	              RW_M22  125	//M22-�м�̵�����M22
#define	              RW_M23  126	//M23-�м�̵�����M23
#define	              RW_M24  127	//M24-�м�̵�����M24
#define	              RW_M25  128	//M25-�м�̵�����M25
#define	              RW_M26  129	//M26-�м�̵�����M26
#define	              RW_M27  130	//M27-�м�̵�����M27
#define	              RW_M28  131	//M28-�м�̵�����M28
#define	              RW_M29  132	//M29-�м�̵�����M29
#define	              RW_M30  133	//M30-�м�̵�����M30
#define	              RW_M31  134	//M31-�м�̵�����M31
#define	              RW_M32  135	//M32-�м�̵�����M32
#define	              RW_M33  136	//M33-�м�̵�����M33
#define	              RW_M34  137	//M34-�м�̵�����M34
#define	              RW_M35  138	//M35-�м�̵�����M35
#define	              RW_M36  139	//M36-�м�̵�����M36
#define	              RW_M37  140	//M37-�м�̵�����M37
#define	              RW_M38  141	//M38-�м�̵�����M38
#define	              RW_M39  142	//M39-�м�̵�����M39
#define	              RW_M40  143	//M40-�м�̵�����M40
#define	              RW_M41  144	//M41-�м�̵�����M41
#define	              RW_M42  145	//M42-�м�̵�����M42
#define	              RW_M43  146	//M43-�м�̵�����M43
#define	              RW_M44  147	//M44-�м�̵�����M44
#define	              RW_M45  148	//M45-�м�̵�����M45
#define	              RW_M46  149	//M46-�м�̵�����M46
#define	              RW_M47  150	//M47-�м�̵�����M47
#define	              RW_M48  151	//M48-�м�̵�����M48
#define	              RW_M49  152	//M49-�м�̵�����M49
#define	              RW_M50  153	//M50-�м�̵�����M50
#define	              RW_M51  154	//M51-�м�̵�����M51
#define	              RW_M52  155	//M52-�м�̵�����M52
#define	              RW_M53  156	//M53-�м�̵�����M53
#define	              RW_M54  157	//M54-�м�̵�����M54
#define	              RW_M55  158	//M55-�м�̵�����M55
#define	              RW_M56  159	//M56-�м�̵�����M56
#define	              RW_M57  160	//M57-�м�̵�����M57
#define	              RW_M58  161	//M58-�м�̵�����M58
#define	              RW_M59  162	//M59-�м�̵�����M59
#define	              RW_M60  163	//M60-�м�̵�����M60
#define	              RW_M61  164	//M61-�м�̵�����M61
#define	              RW_M62  165	//M62-�м�̵�����M62
#define	              RW_M63  166	//M63-�м�̵�����M63
#define	              RW_M64  167	//M64-�м�̵�����M64
#define	              RW_REC  168	//REC-¼���̵�����
#define	             RW_LKCL  169	//LKCL-��բ�����̵�����
#define	             RW_LKTR  170	//LKTR-��բ�����̵�����
#define	               RW_H1  171	//H1-�Ա����м�̵�����1
#define	               RW_H2  172	//H2-�Ա����м�̵�����2
#define	               RW_H3  173	//H3-�Ա����м�̵�����3
#define	               RW_H4  174	//H4-�Ա����м�̵�����4
#define	               RW_H5  175	//H5-�Ա����м�̵�����5
#define	               RW_H6  176	//H6-�Ա����м�̵�����6
#define	               RW_H7  177	//H7-�Ա����м�̵�����7
#define	               RW_H8  178	//H8-�Ա����м�̵�����8
#define	               RW_H9  179	//H9-�Ա����м�̵�����9
#define	              RW_H10  180	//H10-�Ա����м�̵�����10
#define	              RW_H11  181	//H11-�Ա����м�̵�����11
#define	              RW_H12  182	//H12-�Ա����м�̵�����12
#define	              RW_H13  183	//H13-�Ա����м�̵�����13
#define	              RW_H14  184	//H14-�Ա����м�̵�����14
#define	              RW_H15  185	//H15-�Ա����м�̵�����15
#define	              RW_H16  186	//H16-�Ա����м�̵�����16
#define	              RW_H17  187	//H17-�Ա����м�̵�����17
#define	              RW_H18  188	//H18-�Ա����м�̵�����18
#define	              RW_H19  189	//H19-�Ա����м�̵�����19
#define	              RW_H20  190	//H20-�Ա����м�̵�����20
#define	              RW_H21  191	//H21-�Ա����м�̵�����21
#define	              RW_H22  192	//H22-�Ա����м�̵�����22
#define	              RW_H23  193	//H23-�Ա����м�̵�����23
#define	              RW_H24  194	//H24-�Ա����м�̵�����24
#define	              RW_H25  195	//H25-�Ա����м�̵�����25
#define	              RW_H26  196	//H26-�Ա����м�̵�����26
#define	              RW_H27  197	//H27-�Ա����м�̵�����27
#define	              RW_H28  198	//H28-�Ա����м�̵�����28
#define	              RW_H29  199	//H29-�Ա����м�̵�����29
#define	              RW_H30  200	//H30-�Ա����м�̵�����30
#define	              RW_H31  201	//H31-�Ա����м�̵�����31
#define	              RW_H32  202	//H32-�Ա����м�̵�����31
#define	             RW_52BF  203	//52BF-��·����բʧ�̵ܼ�����
#define	               RW_T1  204	//T1-T1
#define	               RW_T2  205	//T2-T2
#define	               RW_T3  206	//T3-T3
#define	               RW_T4  207	//T4-T4
#define	               RW_T5  208	//T5-T5
#define	               RW_T6  209	//T6-T6
#define	               RW_T7  210	//T7-T7
#define	               RW_T8  211	//T8-T8
#define	               RW_T9  212	//T9-T9
#define	              RW_T10  213	//T10-T10
#define	              RW_T11  214	//T11-T11
#define	              RW_T12  215	//T12-T12
#define	              RW_T13  216	//T13-T13
#define	              RW_T14  217	//T14-T14
#define	              RW_T15  218	//T15-T15
#define	              RW_T16  219	//T16-T16
#define	             RW_TSW1  220	//TSW1-TSW1
#define	             RW_TSW2  221	//TSW2-TSW2
#define	             RW_TSW3  222	//TSW3-TSW3
#define	             RW_TSW4  223	//TSW4-TSW4
#define	             RW_TSW5  224	//TSW5-TSW5
#define	             RW_TSW6  225	//TSW6-TSW6
#define	             RW_TSW7  226	//TSW7-TSW7
#define	             RW_TSW8  227	//TSW8-TSW8
#define	             RW_TCB1  228	//TCB1-TCB1
#define	             RW_TCB2  229	//TCB2-TCB2
#define	             RW_TCB3  230	//TCB3-TCB3
#define	             RW_TCB4  231	//TCB4-TCB4
#define	             RW_TCB5  232	//TCB5-TCB5
#define	             RW_TCB6  233	//TCB6-TCB6
#define	             RW_TCB7  234	//TCB7-TCB7
#define	             RW_TCB8  235	//TCB8-TCB8
#define	            RW_T52BF  236	//T52BF-T52BF
#define	              RW_TR1  237	//TR1-��բ�̵���1
#define	              RW_TR2  238	//TR2-��բ�̵���2
#define	              RW_TR3  239	//TR3-��բ�̵���3
#define	              RW_VR1  240	//VR1-����̵�����1
#define	              RW_VR2  241	//VR2-����̵�����2
#define	              RW_VR3  242	//VR3-����̵�����3
#define	              RW_VR4  243	//VR4-����̵�����4
#define	              RW_VR5  244	//VR5-����̵�����5
#define	              RW_VR6  245	//VR6-����̵�����6
#define	              RW_VR7  246	//VR7-����̵�����7
#define	              RW_VR8  247	//VR8-����̵�����8
#define	              RW_VR9  248	//VR9-����̵�����9
#define	             RW_VR10  249	//VR10-����̵�����10
#define	             RW_VR11  250	//VR11-����̵�����11
#define	             RW_VR12  251	//VR12-����̵�����12
#define	             RW_VR13  252	//VR13-����̵�����13
#define	             RW_VR14  253	//VR14-����̵�����14
#define	             RW_VR15  254	//VR15-����̵�����15
#define	             RW_VR16  255	//VR16-����̵�����16
#define	             RW_VR17  256	//VR17-����̵�����17
#define	             RW_VR18  257	//VR18-����̵�����18
#define	             RW_VR19  258	//VR19-����̵�����19
#define	             RW_VR20  259	//VR20-����̵�����20
#define	             RW_VR21  260	//VR21-����̵�����21
#define	             RW_VR22  261	//VR22-����̵�����22
#define	             RW_VR23  262	//VR23-����̵�����23
#define	             RW_VR24  263	//VR24-����̵�����24
#define	             RW_VR25  264	//VR25-����̵�����25
#define	             RW_VR26  265	//VR26-����̵�����26
#define	             RW_VR27  266	//VR27-����̵�����27
#define	             RW_VR28  267	//VR28-����̵�����28
#define	             RW_VR29  268	//VR29-����̵�����29
#define	             RW_VR30  269	//VR30-����̵�����30
#define	             RW_VR31  270	//VR31-����̵�����31
#define	             RW_VR32  271	//VR32-����̵�����32
#define	              RW_IN1  272	//IN1-����̵�����1
#define	              RW_IN2  273	//IN2-����̵�����2
#define	              RW_IN3  274	//IN3-����̵�����3
#define	              RW_IN4  275	//IN4-����̵�����4
#define	              RW_IN5  276	//IN5-����̵�����5
#define	              RW_IN6  277	//IN6-����̵�����6
#define	              RW_IN7  278	//IN7-����̵�����7
#define	              RW_IN8  279	//IN8-����̵�����8
#define	              RW_IN9  280	//IN9-����̵�����9
#define	             RW_IN10  281	//IN10-����̵�����10
#define	             RW_IN11  282	//IN11-����̵�����11
#define	             RW_IN12  283	//IN12-����̵�����12
#define	             RW_IN13  284	//IN13-����̵�����13
#define	             RW_IN14  285	//IN14-����̵�����14
#define	             RW_IN15  286	//IN15-����̵�����15
#define	             RW_IN16  287	//IN16-����̵�����16
#define	             RW_IN17  288	//IN17-����̵�����17
#define	             RW_IN18  289	//IN18-����̵�����18
#define	             RW_IN19  290	//IN19-����̵�����19
#define	             RW_IN20  291	//IN20-����̵�����20
#define	             RW_IN21  292	//IN21-����̵�����21
#define	             RW_IN22  293	//IN22-����̵�����22
#define	             RW_IN23  294	//IN23-����̵�����23
#define	             RW_IN24  295	//IN24-����̵�����24
#define	             RW_IN25  296	//IN25-����̵�����25
#define	             RW_IN26  297	//IN26-����̵�����26
#define	             RW_IN27  298	//IN27-����̵�����27
#define	             RW_IN28  299	//IN28-����̵�����28
#define	             RW_IN29  300	//IN29-����̵�����29
#define	             RW_IN30  301	//IN30-����̵�����30
#define	             RW_IN31  302	//IN31-����̵�����31
#define	             RW_IN32  303	//IN32-����̵�����32
#define	             RW_IN33  304	//IN33-����̵�����33
#define	             RW_IN34  305	//IN34-����̵�����34
#define	             RW_IN35  306	//IN35-�źŸ���
#define	             RW_IN36  307	//IN36-��բλ��
#define	             RW_IN37  308	//IN37-��բλ��
#define	             RW_IN38  309	//IN38-ң��Ԥ��
#define	             RW_IN39  310	//IN39-ң��1Ԥ��
#define	             RW_IN40  311	//IN40-ң��2Ԥ��
#define	           RW_INVIN1  312	//INVIN1-����̵�����41
#define	           RW_INVIN2  313	//INVIN2-����̵�����42
#define	           RW_INVIN3  314	//INVIN3-����̵�����43
#define	           RW_INVIN4  315	//INVIN4-����̵�����44
#define	           RW_INVIN5  316	//INVIN5-����̵�����45
#define	           RW_INVIN6  317	//INVIN6-����̵�����46
#define	           RW_INVIN7  318	//INVIN7-����̵�����47
#define	           RW_INVIN8  319	//INVIN8-����̵�����48
#define	           RW_INVIN9  320	//INVIN9-����̵�����49
#define	          RW_INVIN10  321	//INVIN10-����̵�����50
#define	          RW_INVIN11  322	//INVIN11-����̵�����51
#define	          RW_INVIN12  323	//INVIN12-����̵�����52
#define	          RW_INVIN13  324	//INVIN13-����̵�����53
#define	          RW_INVIN14  325	//INVIN14-����̵�����54
#define	          RW_INVIN15  326	//INVIN15-����̵�����55
#define	          RW_INVIN16  327	//INVIN16-����̵�����56
#define	             RW_OUT1  328	//OUT1-����1�̵�����
#define	             RW_OUT2  329	//OUT2-����2�̵�����
#define	             RW_OUT3  330	//OUT3-����3�̵�����
#define	             RW_OUT4  331	//OUT4-����4�̵�����
#define	             RW_OUT5  332	//OUT5-����5�̵�����
#define	             RW_OUT6  333	//OUT6-����6�̵�����
#define	             RW_OUT7  334	//OUT7-����7�̵�����
#define	             RW_OUT8  335	//OUT8-����8�̵�����
#define	             RW_OUT9  336	//OUT9-����9�̵�����
#define	            RW_OUT10  337	//OUT10-����10�̵�����
#define	            RW_OUT11  338	//OUT11-����11�̵�����
#define	            RW_OUT12  339	//OUT12-����12�̵�����
#define	            RW_OUT13  340	//OUT13-����13�̵�����
#define	            RW_OUT14  341	//OUT14-����14�̵�����
#define	           RW_SL50_0  342	//SL50_0-�����ٶ�ѹ��̵�����
#define	           RW_SL50_1  343	//SL50_1-��������ѹ��̵�����
#define	             RW_SL59  344	//SL59-��ѹ����ѹ��̵�����
#define	             RW_SL27  345	//SL27-��ѹ����ѹ��̵�����
#define	            RW_SL50H  346	//SL50H-г����������ѹ��̵�����
#define	           RW_SL50ST  347	//SL50ST-֧·��������ѹ��̵�����
#define	            RW_SL60I  348	//SL60I-��������ѹ��̵�����
#define	            RW_SL60U  349	//SL60U-��ѹ����ѹ��̵�����
#define	          RW_SLMAINT  350	//SLMAINT-����ѹ��ѹ��̵�����
#define	           RW_SLBAK1  351	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  352	//SLBAK2-����ѹ��2
#define	            RW_F50_0  353	//F50_0-�ٶϱ������ü̵�����
#define	            RW_F50_1  354	//F50_1-�����������ü̵�����
#define	             RW_F50H  355	//F50H-г�������������ü̵�����
#define	              RW_F59  356	//F59-��ѹ�������ü̵�����
#define	              RW_F27  357	//F27-��ѹ�������ü̵�����
#define	            RW_F60I1  358	//F60I1-֧·1�������ü̵�����
#define	            RW_F60I2  359	//F60I2-֧·2�������ü̵�����
#define	            RW_F50ST  360	//F50ST-֧·�������ü̵�����
#define	            RW_F60U1  361	//F60U1-֧·1��ѹ���ü̵�����
#define	            RW_F60U2  362	//F60U2-֧·2��ѹ���ü̵�����
#define	            RW_F60U3  363	//F60U3-֧·3��ѹ���ü̵�����
#define	            RW_F74PT  364	//F74PT-PT���߼�����ü̵�����
#define	            RW_FCTBR  365	//FCTBR-CT���߼�����ü̵�����
#define	            RW_F52BF  366	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  367	//FBAK1-��������1
#define	            RW_FBAK2  368	//FBAK2-��������2
#define	              RW_PW1  369	//PW1-����̵���1
#define	              RW_PW2  370	//PW2-����̵���2
#define	              RW_PW3  371	//PW3-����̵���3
#define	              RW_PW4  372	//PW4-����̵���4
#define	              RW_PW5  373	//PW5-����̵���5
#define	              RW_PW6  374	//PW6-����̵���6
#define	              RW_PW7  375	//PW7-����̵���7
#define	              RW_PW8  376	//PW8-����̵���8
#define	              RW_PW9  377	//PW9-����̵���9
#define	             RW_PW10  378	//PW10-����̵���10
#define	             RW_PW11  379	//PW11-����̵���11
#define	             RW_PW12  380	//PW12-����̵���12
#define	             RW_PW13  381	//PW13-����̵���13
#define	             RW_PW14  382	//PW14-����̵���14
#define	             RW_PW15  383	//PW15-����̵���15
#define	             RW_PW16  384	//PW16-����̵���16
#define	              RW_TO1  385	//TO1-�����̵���1
#define	              RW_TO2  386	//TO2-�����̵���2
#define	              RW_TO3  387	//TO3-�����̵���3
#define	              RW_TO4  388	//TO4-�����̵���4
#define	              RW_TO5  389	//TO5-�����̵���5
#define	              RW_TO6  390	//TO6-�����̵���6
#define	              RW_TO7  391	//TO7-�����̵���7
#define	             RW_SIG1  392	//SIG1-�źż̵���1
#define	             RW_SIG2  393	//SIG2-�źż̵���2
#define	             RW_SIG3  394	//SIG3-�źż̵���3
#define	             RW_SIG4  395	//SIG4-�źż̵���4
#define	             RW_SIG5  396	//SIG5-�źż̵���5
#define	             RW_SIG6  397	//SIG6-�źż̵���6
#define	             RW_SIG7  398	//SIG7-�źż̵���7
#define	             RW_SIG8  399	//SIG8-�źż̵���8
#define	             RW_SIG9  400	//SIG9-�źż̵���9
#define	            RW_SIG10  401	//SIG10-�źż̵���10
#define	            RW_SIG11  402	//SIG11-�źż̵���11
#define	            RW_SIG12  403	//SIG12-�źż̵���12
#define	            RW_SIG13  404	//SIG13-�źż̵���13
#define	            RW_SIG14  405	//SIG14-�źż̵���14
#define	            RW_SIG15  406	//SIG15-�źż̵���15
#define	            RW_SIG16  407	//SIG16-�źż̵���16
#define	            RW_LED1G  408	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  409	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  410	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  411	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  412	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  413	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  414	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  415	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  416	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  417	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  418	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  419	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  420	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  421	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  422	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  423	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  424	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  425	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  426	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  427	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  428	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  429	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  430	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  431	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  432	//VIN1-���⿪��̵�����1
#define	             RW_VIN2  433	//VIN2-���⿪��̵�����2
#define	             RW_VIN3  434	//VIN3-���⿪��̵�����3
#define	             RW_VIN4  435	//VIN4-���⿪��̵�����4
#define	             RW_VIN5  436	//VIN5-���⿪��̵�����5
#define	             RW_VIN6  437	//VIN6-���⿪��̵�����6
#define	             RW_VIN7  438	//VIN7-���⿪��̵�����7
#define	             RW_VIN8  439	//VIN8-���⿪��̵�����8
#define	             RW_VIN9  440	//VIN9-���⿪��̵�����9
#define	            RW_VIN10  441	//VIN10-���⿪��̵�����10
#define	            RW_VIN11  442	//VIN11-���⿪��̵�����11
#define	            RW_VIN12  443	//VIN12-���⿪��̵�����12
#define	            RW_VIN13  444	//VIN13-���⿪��̵�����13
#define	            RW_VIN14  445	//VIN14-���⿪��̵�����14
#define	            RW_VIN15  446	//VIN15-���⿪��̵�����15
#define	            RW_VIN16  447	//VIN16-���⿪��̵�����16
#define	             RW_RCP1  448	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  449	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  450	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  451	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  452	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  453	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  454	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  455	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  456	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  457	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  458	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  459	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  460	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  461	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  462	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  463	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  464	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  465	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  466	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  467	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  468	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  469	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  470	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  471	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  472	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  473	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  474	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  475	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  476	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  477	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  478	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  479	//RCP32-ң��Ԥ�ü̵���32
#define	             RW_RCE1  480	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  481	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  482	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  483	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  484	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  485	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  486	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  487	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  488	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  489	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  490	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  491	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  492	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  493	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  494	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  495	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  496	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  497	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  498	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  499	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  500	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  501	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  502	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  503	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  504	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  505	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  506	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  507	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  508	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  509	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  510	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  511	//RCE32-ң��ִ�м̵���32
#define	             RW_RCV1  512	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  513	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  514	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  515	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  516	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  517	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  518	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  519	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  520	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  521	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  522	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  523	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  524	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  525	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  526	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  527	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  528	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  529	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  530	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  531	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  532	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  533	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  534	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  535	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  536	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  537	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  538	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  539	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  540	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  541	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  542	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  543	//RCV32-ң�ط�У�̵���32
#define	              RW_GE1  544	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  545	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  546	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  547	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  548	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  549	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  550	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  551	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  552	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  553	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  554	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  555	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  556	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  557	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  558	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  559	//CONST8-����ʱ��̵���8
#define	              RW_TX1  560	//TX1-�������Ӽ̵�����1
#define	              RW_TX2  561	//TX2-�������Ӽ̵�����2
#define	              RW_TX3  562	//TX3-�������Ӽ̵�����3
#define	              RW_TX4  563	//TX4-�������Ӽ̵�����4
#define	              RW_TX5  564	//TX5-�������Ӽ̵�����5
#define	              RW_TX6  565	//TX6-�������Ӽ̵�����6
#define	              RW_TX7  566	//TX7-�������Ӽ̵�����7
#define	              RW_TX8  567	//TX8-�������Ӽ̵�����8
#define	              RW_TX9  568	//TX9-�������Ӽ̵�����9
#define	             RW_TX10  569	//TX10-�������Ӽ̵�����10
#define	             RW_TX11  570	//TX11-�������Ӽ̵�����11
#define	             RW_TX12  571	//TX12-�������Ӽ̵�����12
#define	             RW_TX13  572	//TX13-�������Ӽ̵�����13
#define	             RW_TX14  573	//TX14-�������Ӽ̵�����14
#define	             RW_TX15  574	//TX15-�������Ӽ̵�����15
#define	             RW_TX16  575	//TX16-�������Ӽ̵�����16
#define	              RW_RX1  576	//RX1-�������Ӽ̵�����1
#define	              RW_RX2  577	//RX2-�������Ӽ̵�����2
#define	              RW_RX3  578	//RX3-�������Ӽ̵�����3
#define	              RW_RX4  579	//RX4-�������Ӽ̵�����4
#define	              RW_RX5  580	//RX5-�������Ӽ̵�����5
#define	              RW_RX6  581	//RX6-�������Ӽ̵�����6
#define	              RW_RX7  582	//RX7-�������Ӽ̵�����7
#define	              RW_RX8  583	//RX8-�������Ӽ̵�����8
#define	              RW_RX9  584	//RX9-�������Ӽ̵�����9
#define	             RW_RX10  585	//RX10-�������Ӽ̵�����10
#define	             RW_RX11  586	//RX11-�������Ӽ̵�����11
#define	             RW_RX12  587	//RX12-�������Ӽ̵�����12
#define	             RW_RX13  588	//RX13-�������Ӽ̵�����13
#define	             RW_RX14  589	//RX14-�������Ӽ̵�����14
#define	             RW_RX15  590	//RX15-�������Ӽ̵�����15
#define	             RW_RX16  591	//RX16-�������Ӽ̵�����16
#define	             RW_CNT1  592	//CNT1-�����̵�����1
#define	             RW_CNT2  593	//CNT2-�����̵�����2
#define	             RW_CNT3  594	//CNT3-�����̵�����3
#define	             RW_CNT4  595	//CNT4-�����̵�����4
#define	             RW_CNT5  596	//CNT5-�����̵�����5
#define	             RW_CNT6  597	//CNT6-�����̵�����6
#define	             RW_CNT7  598	//CNT7-�����̵�����7
#define	             RW_CNT8  599	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			600		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			77	

//�м�̵�����
#define	MID_RELAY_WORD_START			79	
#define	MID_RELAY_WORD_NUM			92	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			171	
#define	KEEP_RELAY_WORD_NUM			33	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			204	
#define	TIME_RELAY_WORD_NUM			33	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			237	
#define	TRIP_RELAY_WORD_NUM			3	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			240	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			272	
#define	INPUT_RELAY_WORD_NUM			56	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			328	
#define	OUTPUT_RELAY_WORD_NUM			14	

//ѹ��̵���
#define	SW_RELAY_WORD_START			342	
#define	SW_RELAY_WORD_NUM			11	

//���ü̵���
#define	CFG_RELAY_WORD_START			353	
#define	CFG_RELAY_WORD_NUM			16	

//����̵���
#define	PULSE_RELAY_WORD_START			369	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			385	
#define	CTRLTEST_RELAY_WORD_NUM			7	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			392	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			408	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			432	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			448	
#define	RCP_RELAY_WORD_NUM			32	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			480	
#define	RCE_RELAY_WORD_NUM			32	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			512	
#define	RCV_RELAY_WORD_NUM			32	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			544	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			552	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			560	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			576	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			592	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			592	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			592	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			592	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			592	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			592	
#define	CNT_RELAY_WORD_NUM			8	


#endif