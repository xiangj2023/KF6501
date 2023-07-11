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
#define	           RW_PI50I1    2	//PI50I1-����������μ̵�����
#define	           RW_TR50I1    3	//TR50I1-�������������̵�����
#define	          RW_LPC50I1    4	//LPC50I1-��������ѭ�����Ƽ̵�����
#define	           RW_PI50I0    5	//PI50I0-�ٶϱ�����μ̵�����
#define	           RW_TR50I0    6	//TR50I0-�ٶϱ��������̵�����
#define	          RW_LPC50I0    7	//LPC50I0-�ٶϱ���ѭ�����Ƽ̵�����
#define	            RW_PI59U    8	//PI59U-��ѹ������μ̵�����
#define	            RW_TR59U    9	//TR59U-��ѹ���������̵�����
#define	           RW_LPC59U   10	//LPC59U-��ѹ����ѭ�����Ƽ̵�����
#define	            RW_PI60I   11	//PI60I-��ƽ�����������μ̵�����
#define	            RW_TR60I   12	//TR60I-��ƽ��������������̵�����
#define	           RW_LPC60I   13	//LPC60I-��ƽ���������ѭ�����Ƽ̵�����
#define	            RW_PI60U   14	//PI60U-��ƽ���ѹ������μ̵�����
#define	            RW_TR60U   15	//TR60U-��ƽ���ѹ���������̵�����
#define	           RW_LPC60U   16	//LPC60U-��ƽ���ѹ����ѭ�����Ƽ̵�����
#define	            RW_PI27U   17	//PI27U-��ѹ������μ̵�����
#define	            RW_TR27U   18	//TR27U-��ѹ���������̵�����
#define	           RW_LPC27U   19	//LPC27U-��ѹ����ѭ�����Ƽ̵�����
#define	            RW_PIPT1   20	//PIPT1-ĸ��PT������μ̵�����
#define	            RW_ARPT1   21	//ARPT1-ĸ��PT���߸澯�̵�����
#define	            RW_PI59N   22	//PI59N-��ѹ��μ̵�����
#define	            RW_TR59N   23	//TR59N-��ѹ�����̵�����
#define	           RW_LPC59N   24	//LPC59N-��ѹ����ѭ�����Ƽ̵�����
#define	             RW_59UA   25	//59UA-UA��ѹ�̵�����
#define	             RW_27UA   26	//27UA-UA��ѹ�̵�����
#define	             RW_59UB   27	//59UB-UB��ѹ�̵�����
#define	             RW_27UB   28	//27UB-UB��ѹ�̵�����
#define	             RW_59UC   29	//59UC-UC��ѹ�̵�����
#define	             RW_27UC   30	//27UC-UC��ѹ�̵�����
#define	             RW_59U0   31	//59U0-U0��ѹ�̵�����
#define	             RW_27U0   32	//27U0-U0��ѹ�̵�����
#define	             RW_59U1   33	//59U1-U1��ѹ�̵�����
#define	             RW_27U1   34	//27U1-U1��ѹ�̵�����
#define	             RW_59U2   35	//59U2-U2��ѹ�̵�����
#define	             RW_27U2   36	//27U2-U2��ѹ�̵�����
#define	             RW_59U3   37	//59U3-U3��ѹ�̵�����
#define	             RW_27U3   38	//27U3-U3��ѹ�̵�����
#define	             RW_50IA   39	//50IA-IA�����̵�����
#define	             RW_37IA   40	//37IA-IA�����̵�����
#define	             RW_50IB   41	//50IB-IB�����̵�����
#define	             RW_37IB   42	//37IB-IB�����̵�����
#define	             RW_50IC   43	//50IC-IC�����̵�����
#define	             RW_37IC   44	//37IC-IC�����̵�����
#define	             RW_50I0   45	//50I0-I0�����̵�����
#define	             RW_37I0   46	//37I0-I0�����̵�����
#define	             RW_50I1   47	//50I1-I1�����̵�����
#define	             RW_37I1   48	//37I1-I1�����̵�����
#define	           RW_PICKUP   49	//PICKUP-ģֵͻ���������̵�����
#define	          RW_ET11ERR   50	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   51	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   52	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   53	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   54	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   55	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   56	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   57	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   58	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   59	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   60	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   61	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   62	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   63	//NULL1ERR-������1״̬�̵�����
#define	         RW_NULL2ERR   64	//NULL2ERR-������2״̬�̵�����
#define	         RW_NULL3ERR   65	//NULL3ERR-������3״̬�̵�����
#define	         RW_PICTBR_A   66	//PICTBR_A-A��CT������μ̵�����
#define	         RW_ARCTBR_A   67	//ARCTBR_A-A��CT���߸澯�̵�����
#define	         RW_PICTBR_B   68	//PICTBR_B-B��CT������μ̵�����
#define	         RW_ARCTBR_B   69	//ARCTBR_B-B��CT���߸澯�̵�����
#define	         RW_PICTBR_C   70	//PICTBR_C-C��CT������μ̵�����
#define	         RW_ARCTBR_C   71	//ARCTBR_C-C��CT���߸澯�̵�����
#define	        RW_MAINTLINK   72	//MAINTLINK-ά��״̬�̵�����
#define	               RW_M1   73	//M1-�м�̵�����M1
#define	               RW_M2   74	//M2-�м�̵�����M2
#define	               RW_M3   75	//M3-�м�̵�����M3
#define	               RW_M4   76	//M4-�м�̵�����M4
#define	               RW_M5   77	//M5-�м�̵�����M5
#define	               RW_M6   78	//M6-�м�̵�����M6
#define	               RW_M7   79	//M7-�м�̵�����M7
#define	               RW_M8   80	//M8-�м�̵�����M8
#define	               RW_M9   81	//M9-�м�̵�����M9
#define	              RW_M10   82	//M10-�м�̵�����M10
#define	              RW_M11   83	//M11-�м�̵�����M11
#define	              RW_M12   84	//M12-�м�̵�����M12
#define	              RW_M13   85	//M13-�м�̵�����M13
#define	              RW_M14   86	//M14-�м�̵�����M14
#define	              RW_M15   87	//M15-�м�̵�����M15
#define	              RW_M16   88	//M16-�м�̵�����M16
#define	              RW_M17   89	//M17-�м�̵�����M17
#define	              RW_M18   90	//M18-�м�̵�����M18
#define	              RW_M19   91	//M19-�м�̵�����M19
#define	              RW_M20   92	//M20-�м�̵�����M20
#define	              RW_M21   93	//M21-�м�̵�����M21
#define	              RW_M22   94	//M22-�м�̵�����M22
#define	              RW_M23   95	//M23-�м�̵�����M23
#define	              RW_M24   96	//M24-�м�̵�����M24
#define	              RW_M25   97	//M25-�м�̵�����M25
#define	              RW_M26   98	//M26-�м�̵�����M26
#define	              RW_M27   99	//M27-�м�̵�����M27
#define	              RW_M28  100	//M28-�м�̵�����M28
#define	              RW_M29  101	//M29-�м�̵�����M29
#define	              RW_M30  102	//M30-�м�̵�����M30
#define	              RW_M31  103	//M31-�м�̵�����M31
#define	              RW_M32  104	//M32-�м�̵�����M32
#define	              RW_M33  105	//M33-�м�̵�����M33
#define	              RW_M34  106	//M34-�м�̵�����M34
#define	              RW_M35  107	//M35-�м�̵�����M35
#define	              RW_M36  108	//M36-�м�̵�����M36
#define	              RW_M37  109	//M37-�м�̵�����M37
#define	              RW_M38  110	//M38-�м�̵�����M38
#define	              RW_M39  111	//M39-�м�̵�����M39
#define	              RW_M40  112	//M40-�м�̵�����M40
#define	              RW_M41  113	//M41-�м�̵�����M41
#define	              RW_M42  114	//M42-�м�̵�����M42
#define	              RW_M43  115	//M43-�м�̵�����M43
#define	              RW_M44  116	//M44-�м�̵�����M44
#define	              RW_M45  117	//M45-�м�̵�����M45
#define	              RW_M46  118	//M46-�м�̵�����M46
#define	              RW_M47  119	//M47-�м�̵�����M47
#define	              RW_M48  120	//M48-�м�̵�����M48
#define	              RW_M49  121	//M49-�м�̵�����M49
#define	              RW_M50  122	//M50-�м�̵�����M50
#define	              RW_M51  123	//M51-�м�̵�����M51
#define	              RW_M52  124	//M52-�м�̵�����M52
#define	              RW_M53  125	//M53-�м�̵�����M53
#define	              RW_M54  126	//M54-�м�̵�����M54
#define	              RW_M55  127	//M55-�м�̵�����M55
#define	              RW_M56  128	//M56-�м�̵�����M56
#define	              RW_M57  129	//M57-�м�̵�����M57
#define	              RW_M58  130	//M58-�м�̵�����M58
#define	              RW_M59  131	//M59-�м�̵�����M59
#define	              RW_M60  132	//M60-�м�̵�����M60
#define	              RW_M61  133	//M61-�м�̵�����M61
#define	              RW_M62  134	//M62-�м�̵�����M62
#define	              RW_M63  135	//M63-�м�̵�����M63
#define	              RW_M64  136	//M64-�м�̵�����M64
#define	         RW_LKPI50I1  137	//LKPI50I1-������α����̵�����
#define	        RW_LKRST50I1  138	//LKRST50I1-�������ر����̵�����
#define	         RW_LKPI50I0  139	//LKPI50I0-�ٶ���α����̵�����
#define	        RW_LKRST50I0  140	//LKRST50I0-�ٶϷ��ر����̵�����
#define	          RW_LKPI59U  141	//LKPI59U-��ѹ��α����̵�����
#define	         RW_LKRST59U  142	//LKRST59U-��ѹ���ر����̵�����
#define	          RW_LKPI60I  143	//LKPI60I-��ƽ�������α����̵�����
#define	         RW_LKRST60I  144	//LKRST60I-��ƽ��������ر����̵�����
#define	          RW_LKPI60U  145	//LKPI60U-��ƽ���ѹ��α����̵�����
#define	         RW_LKRST60U  146	//LKRST60U-��ƽ���ѹ���ر����̵�����
#define	           RW_LKPI27  147	//LKPI27-��ѹ������α����̵�����
#define	          RW_LKRST27  148	//LKRST27-��ѹ�������ر����̵�����
#define	             RW_69IN  149	//69IN-Զ�������м�̵�����
#define	              RW_52A  150	//52A-��λ
#define	              RW_52B  151	//52B-��λ
#define	              RW_REC  152	//REC-¼���̵�����
#define	               RW_H1  153	//H1-�Ա����м�̵�����1
#define	               RW_H2  154	//H2-�Ա����м�̵�����2
#define	               RW_H3  155	//H3-�Ա����м�̵�����3
#define	               RW_H4  156	//H4-�Ա����м�̵�����4
#define	               RW_H5  157	//H5-�Ա����м�̵�����5
#define	               RW_H6  158	//H6-�Ա����м�̵�����6
#define	               RW_H7  159	//H7-�Ա����м�̵�����7
#define	               RW_H8  160	//H8-�Ա����м�̵�����8
#define	               RW_H9  161	//H9-�Ա����м�̵�����9
#define	              RW_H10  162	//H10-�Ա����м�̵�����10
#define	              RW_H11  163	//H11-�Ա����м�̵�����11
#define	              RW_H12  164	//H12-�Ա����м�̵�����12
#define	              RW_H13  165	//H13-�Ա����м�̵�����13
#define	              RW_H14  166	//H14-�Ա����м�̵�����14
#define	              RW_H15  167	//H15-�Ա����м�̵�����15
#define	              RW_H16  168	//H16-�Ա����м�̵�����16
#define	              RW_H17  169	//H17-�Ա����м�̵�����17
#define	              RW_H18  170	//H18-�Ա����м�̵�����18
#define	              RW_H19  171	//H19-�Ա����м�̵�����19
#define	              RW_H20  172	//H20-�Ա����м�̵�����20
#define	              RW_H21  173	//H21-�Ա����м�̵�����21
#define	              RW_H22  174	//H22-�Ա����м�̵�����22
#define	              RW_H23  175	//H23-�Ա����м�̵�����23
#define	              RW_H24  176	//H24-�Ա����м�̵�����24
#define	              RW_H25  177	//H25-�Ա����м�̵�����25
#define	              RW_H26  178	//H26-�Ա����м�̵�����26
#define	              RW_H27  179	//H27-�Ա����м�̵�����27
#define	              RW_H28  180	//H28-�Ա����м�̵�����28
#define	              RW_H29  181	//H29-�Ա����м�̵�����29
#define	              RW_H30  182	//H30-�Ա����м�̵�����30
#define	              RW_H31  183	//H31-�Ա����м�̵�����31
#define	              RW_H32  184	//H32-�Ա����м�̵�����31
#define	             RW_52BF  185	//52BF-��·����բʧ�̵ܼ�����
#define	               RW_T1  186	//T1-T1
#define	               RW_T2  187	//T2-T2
#define	               RW_T3  188	//T3-T3
#define	               RW_T4  189	//T4-T4
#define	               RW_T5  190	//T5-T5
#define	               RW_T6  191	//T6-T6
#define	               RW_T7  192	//T7-T7
#define	               RW_T8  193	//T8-T8
#define	               RW_T9  194	//T9-T9
#define	              RW_T10  195	//T10-T10
#define	              RW_T11  196	//T11-T11
#define	              RW_T12  197	//T12-T12
#define	              RW_T13  198	//T13-T13
#define	              RW_T14  199	//T14-T14
#define	              RW_T15  200	//T15-T15
#define	              RW_T16  201	//T16-T16
#define	             RW_TSW1  202	//TSW1-TSW1
#define	             RW_TSW2  203	//TSW2-TSW2
#define	             RW_TSW3  204	//TSW3-TSW3
#define	             RW_TSW4  205	//TSW4-TSW4
#define	             RW_TSW5  206	//TSW5-TSW5
#define	             RW_TSW6  207	//TSW6-TSW6
#define	             RW_TSW7  208	//TSW7-TSW7
#define	             RW_TSW8  209	//TSW8-TSW8
#define	             RW_TCB1  210	//TCB1-TCB1
#define	             RW_TCB2  211	//TCB2-TCB2
#define	             RW_TCB3  212	//TCB3-TCB3
#define	             RW_TCB4  213	//TCB4-TCB4
#define	             RW_TCB5  214	//TCB5-TCB5
#define	             RW_TCB6  215	//TCB6-TCB6
#define	             RW_TCB7  216	//TCB7-TCB7
#define	             RW_TCB8  217	//TCB8-TCB8
#define	            RW_T52BF  218	//T52BF-��·����բʧ����ʱ�̵�����
#define	              RW_TR1  219	//TR1-��բ�̵���1
#define	              RW_TR2  220	//TR2-��բ�̵���2
#define	              RW_TR3  221	//TR3-��բ�̵���3
#define	              RW_VR1  222	//VR1-����̵�����1
#define	              RW_VR2  223	//VR2-����̵�����2
#define	              RW_VR3  224	//VR3-����̵�����3
#define	              RW_VR4  225	//VR4-����̵�����4
#define	              RW_VR5  226	//VR5-����̵�����5
#define	              RW_VR6  227	//VR6-����̵�����6
#define	              RW_VR7  228	//VR7-����̵�����7
#define	              RW_VR8  229	//VR8-����̵�����8
#define	              RW_VR9  230	//VR9-����̵�����9
#define	             RW_VR10  231	//VR10-����̵�����10
#define	             RW_VR11  232	//VR11-����̵�����11
#define	             RW_VR12  233	//VR12-����̵�����12
#define	             RW_VR13  234	//VR13-����̵�����13
#define	             RW_VR14  235	//VR14-����̵�����14
#define	             RW_VR15  236	//VR15-����̵�����15
#define	             RW_VR16  237	//VR16-����̵�����16
#define	             RW_VR17  238	//VR17-����̵�����17
#define	             RW_VR18  239	//VR18-����̵�����18
#define	             RW_VR19  240	//VR19-����̵�����19
#define	             RW_VR20  241	//VR20-����̵�����20
#define	             RW_VR21  242	//VR21-����̵�����21
#define	             RW_VR22  243	//VR22-����̵�����22
#define	             RW_VR23  244	//VR23-����̵�����23
#define	             RW_VR24  245	//VR24-����̵�����24
#define	             RW_VR25  246	//VR25-����̵�����25
#define	             RW_VR26  247	//VR26-����̵�����26
#define	             RW_VR27  248	//VR27-����̵�����27
#define	             RW_VR28  249	//VR28-����̵�����28
#define	             RW_VR29  250	//VR29-����̵�����29
#define	             RW_VR30  251	//VR30-����̵�����30
#define	             RW_VR31  252	//VR31-����̵�����31
#define	             RW_VR32  253	//VR32-����̵�����32
#define	              RW_IN1  254	//IN1-IN1
#define	              RW_IN2  255	//IN2-IN2
#define	              RW_IN3  256	//IN3-IN3
#define	              RW_IN4  257	//IN4-IN4
#define	              RW_IN5  258	//IN5-IN5
#define	              RW_IN6  259	//IN6-IN6
#define	              RW_IN7  260	//IN7-IN7
#define	              RW_IN8  261	//IN8-IN8
#define	              RW_IN9  262	//IN9-IN9
#define	             RW_IN10  263	//IN10-IN10
#define	             RW_IN11  264	//IN11-IN11
#define	             RW_IN12  265	//IN12-IN12
#define	             RW_IN13  266	//IN13-IN13
#define	             RW_IN14  267	//IN14-IN14
#define	             RW_IN15  268	//IN15-IN15
#define	             RW_IN16  269	//IN16-IN16
#define	             RW_IN17  270	//IN17-IN17
#define	             RW_IN18  271	//IN18-IN18
#define	             RW_IN19  272	//IN19-IN19
#define	             RW_IN20  273	//IN20-IN20
#define	             RW_IN21  274	//IN21-IN21
#define	             RW_IN22  275	//IN22-IN22
#define	             RW_IN23  276	//IN23-IN23
#define	             RW_IN24  277	//IN24-IN24
#define	             RW_IN25  278	//IN25-IN25
#define	             RW_IN26  279	//IN26-IN26
#define	             RW_IN27  280	//IN27-IN27
#define	             RW_IN28  281	//IN28-IN28
#define	             RW_IN29  282	//IN29-IN29
#define	             RW_IN30  283	//IN30-IN30
#define	             RW_IN31  284	//IN31-IN31
#define	             RW_IN32  285	//IN32-IN32
#define	             RW_IN33  286	//IN33-IN33
#define	             RW_IN34  287	//IN34-IN34
#define	             RW_IN35  288	//IN35-�źŸ���
#define	             RW_IN36  289	//IN36-��բλ��
#define	             RW_IN37  290	//IN37-��բλ��
#define	             RW_IN38  291	//IN38-�����ض�
#define	             RW_IN39  292	//IN39-ң��Ԥ��
#define	             RW_IN40  293	//IN40-ң��1Ԥ��
#define	             RW_IN41  294	//IN41-ң��2Ԥ��
#define	             RW_IN42  295	//IN42-ң��3Ԥ��
#define	           RW_INVIN1  296	//INVIN1-VIN1
#define	           RW_INVIN2  297	//INVIN2-VIN2
#define	           RW_INVIN3  298	//INVIN3-VIN3
#define	           RW_INVIN4  299	//INVIN4-VIN4
#define	           RW_INVIN5  300	//INVIN5-VIN5
#define	           RW_INVIN6  301	//INVIN6-VIN6
#define	           RW_INVIN7  302	//INVIN7-VIN7
#define	           RW_INVIN8  303	//INVIN8-VIN8
#define	           RW_INVIN9  304	//INVIN9-VIN9
#define	          RW_INVIN10  305	//INVIN10-VIN10
#define	          RW_INVIN11  306	//INVIN11-VIN11
#define	          RW_INVIN12  307	//INVIN12-VIN12
#define	          RW_INVIN13  308	//INVIN13-VIN13
#define	          RW_INVIN14  309	//INVIN14-VIN14
#define	          RW_INVIN15  310	//INVIN15-VIN15
#define	          RW_INVIN16  311	//INVIN16-VIN16
#define	             RW_OUT1  312	//OUT1-����1�̵�����
#define	             RW_OUT2  313	//OUT2-����2�̵�����
#define	             RW_OUT3  314	//OUT3-����3�̵�����
#define	             RW_OUT4  315	//OUT4-����4�̵�����
#define	             RW_OUT5  316	//OUT5-����5�̵�����
#define	             RW_OUT6  317	//OUT6-����6�̵�����
#define	             RW_OUT7  318	//OUT7-����7�̵�����
#define	             RW_OUT8  319	//OUT8-����8�̵�����
#define	             RW_OUT9  320	//OUT9-����9�̵�����
#define	            RW_OUT10  321	//OUT10-����10�̵�����
#define	            RW_OUT11  322	//OUT11-����11�̵�����
#define	            RW_OUT12  323	//OUT12-����12�̵�����
#define	            RW_OUT13  324	//OUT13-����13�̵�����
#define	            RW_OUT14  325	//OUT14-����14�̵�����
#define	            RW_OUT15  326	//OUT15-����15�̵�����
#define	            RW_OUT16  327	//OUT16-����16�̵�����
#define	            RW_OUT17  328	//OUT17-����17�̵�����
#define	            RW_OUT18  329	//OUT18-����18�̵�����
#define	            RW_OUT19  330	//OUT19-����19�̵�����
#define	           RW_SL50I0  331	//SL50I0-�����ٶ���ѹ��̵�����
#define	           RW_SL50I1  332	//SL50I1-������ѹ��̵�����
#define	            RW_SL59U  333	//SL59U-��ѹ������ѹ��̵�����
#define	            RW_SL27U  334	//SL27U-��ѹ������ѹ��̵�����
#define	            RW_SL60I  335	//SL60I-��ƽ�������ѹ��̵�����
#define	            RW_SL60U  336	//SL60U-��ƽ���ѹ��ѹ��̵�����
#define	           RW_SLJDXX  337	//SLJDXX-�ӵ�ѡ����ѹ��̵�����
#define	          RW_SLMAINT  338	//SLMAINT-����ѹ��̵�����
#define	           RW_SLBAK1  339	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  340	//SLBAK2-����ѹ��2
#define	            RW_F50I0  341	//F50I0-�����ٶ����ü̵�����
#define	            RW_F50I1  342	//F50I1-�����������ü̵�����
#define	             RW_F59U  343	//F59U-��ѹ�������ü̵�����
#define	             RW_F27U  344	//F27U-��ѹ�������ü̵�����
#define	             RW_F60I  345	//F60I-��ƽ������������ü̵�����
#define	             RW_F60U  346	//F60U-��ƽ���ѹ�������ü̵�����
#define	             RW_F59N  347	//F59N-��ѹ�澯���ü̵�����
#define	            RW_FJDXX  348	//FJDXX-�ӵ�ѡ�����ü̵�����
#define	            RW_FXHXQ  349	//FXHXQ-������Ȧ�ӵ����ü̵�����
#define	            RW_F74PT  350	//F74PT-PT���߼�����ü̵�����
#define	            RW_FCTBR  351	//FCTBR-CT�������ü̵�����
#define	            RW_F52BF  352	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  353	//FBAK1-����1���ü̵�����
#define	            RW_FBAK2  354	//FBAK2-����2���ü̵�����
#define	              RW_PW1  355	//PW1-����̵���1
#define	              RW_PW2  356	//PW2-����̵���2
#define	              RW_PW3  357	//PW3-����̵���3
#define	              RW_PW4  358	//PW4-����̵���4
#define	              RW_PW5  359	//PW5-����̵���5
#define	              RW_PW6  360	//PW6-����̵���6
#define	              RW_PW7  361	//PW7-����̵���7
#define	              RW_PW8  362	//PW8-����̵���8
#define	              RW_PW9  363	//PW9-����̵���9
#define	             RW_PW10  364	//PW10-����̵���10
#define	             RW_PW11  365	//PW11-����̵���11
#define	             RW_PW12  366	//PW12-����̵���12
#define	             RW_PW13  367	//PW13-����̵���13
#define	             RW_PW14  368	//PW14-����̵���14
#define	             RW_PW15  369	//PW15-����̵���15
#define	             RW_PW16  370	//PW16-����̵���16
#define	              RW_TO1  371	//TO1-�����̵���1
#define	              RW_TO2  372	//TO2-�����̵���2
#define	              RW_TO3  373	//TO3-�����̵���3
#define	              RW_TO4  374	//TO4-�����̵���4
#define	              RW_TO5  375	//TO5-�����̵���5
#define	              RW_TO6  376	//TO6-�����̵���6
#define	              RW_TO7  377	//TO7-�����̵���7
#define	              RW_TO8  378	//TO8-�����̵���8
#define	              RW_TO9  379	//TO9-�����̵���9
#define	             RW_TO10  380	//TO10-�����̵���10
#define	             RW_TO11  381	//TO11-�����̵���11
#define	             RW_SIG1  382	//SIG1-�źż̵���1
#define	             RW_SIG2  383	//SIG2-�źż̵���2
#define	             RW_SIG3  384	//SIG3-�źż̵���3
#define	             RW_SIG4  385	//SIG4-�źż̵���4
#define	             RW_SIG5  386	//SIG5-�źż̵���5
#define	             RW_SIG6  387	//SIG6-�źż̵���6
#define	             RW_SIG7  388	//SIG7-�źż̵���7
#define	             RW_SIG8  389	//SIG8-�źż̵���8
#define	             RW_SIG9  390	//SIG9-�źż̵���9
#define	            RW_SIG10  391	//SIG10-�źż̵���10
#define	            RW_SIG11  392	//SIG11-�źż̵���11
#define	            RW_SIG12  393	//SIG12-�źż̵���12
#define	            RW_SIG13  394	//SIG13-�źż̵���13
#define	            RW_SIG14  395	//SIG14-�źż̵���14
#define	            RW_SIG15  396	//SIG15-�źż̵���15
#define	            RW_SIG16  397	//SIG16-�źż̵���16
#define	            RW_LED1G  398	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  399	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  400	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  401	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  402	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  403	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  404	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  405	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  406	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  407	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  408	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  409	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  410	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  411	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  412	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  413	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  414	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  415	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  416	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  417	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  418	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  419	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  420	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  421	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  422	//VIN1-VIN1
#define	             RW_VIN2  423	//VIN2-VIN2
#define	             RW_VIN3  424	//VIN3-VIN3
#define	             RW_VIN4  425	//VIN4-VIN4
#define	             RW_VIN5  426	//VIN5-VIN5
#define	             RW_VIN6  427	//VIN6-VIN6
#define	             RW_VIN7  428	//VIN7-VIN7
#define	             RW_VIN8  429	//VIN8-VIN8
#define	             RW_VIN9  430	//VIN9-VIN9
#define	            RW_VIN10  431	//VIN10-VIN10
#define	            RW_VIN11  432	//VIN11-VIN11
#define	            RW_VIN12  433	//VIN12-VIN12
#define	            RW_VIN13  434	//VIN13-VIN13
#define	            RW_VIN14  435	//VIN14-VIN14
#define	            RW_VIN15  436	//VIN15-VIN15
#define	            RW_VIN16  437	//VIN16-VIN16
#define	             RW_RCP1  438	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  439	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  440	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  441	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  442	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  443	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  444	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  445	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  446	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  447	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  448	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  449	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  450	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  451	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  452	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  453	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  454	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  455	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  456	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  457	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  458	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  459	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  460	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  461	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  462	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  463	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  464	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  465	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  466	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  467	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  468	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  469	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  470	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  471	//RCP34-ң��Ԥ�ü̵���34
#define	             RW_RCE1  472	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  473	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  474	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  475	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  476	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  477	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  478	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  479	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  480	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  481	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  482	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  483	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  484	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  485	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  486	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  487	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  488	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  489	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  490	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  491	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  492	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  493	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  494	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  495	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  496	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  497	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  498	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  499	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  500	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  501	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  502	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  503	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  504	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  505	//RCE34-ң��ִ�м̵���34
#define	             RW_RCV1  506	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  507	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  508	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  509	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  510	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  511	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  512	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  513	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  514	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  515	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  516	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  517	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  518	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  519	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  520	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  521	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  522	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  523	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  524	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  525	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  526	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  527	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  528	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  529	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  530	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  531	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  532	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  533	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  534	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  535	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  536	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  537	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  538	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  539	//RCV34-ң�ط�У�̵���34
#define	              RW_GE1  540	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  541	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  542	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  543	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  544	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  545	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  546	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  547	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  548	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  549	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  550	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  551	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  552	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  553	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  554	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  555	//CONST8-����ʱ��̵���8
#define	              RW_TX1  556	//TX1-�����ͼ̵�����1
#define	              RW_TX2  557	//TX2-�����ͼ̵�����2
#define	              RW_TX3  558	//TX3-�����ͼ̵�����3
#define	              RW_TX4  559	//TX4-�����ͼ̵�����4
#define	              RW_TX5  560	//TX5-�����ͼ̵�����5
#define	              RW_TX6  561	//TX6-�����ͼ̵�����6
#define	              RW_TX7  562	//TX7-�����ͼ̵�����7
#define	              RW_TX8  563	//TX8-�����ͼ̵�����8
#define	              RW_TX9  564	//TX9-�����ͼ̵�����9
#define	             RW_TX10  565	//TX10-�����ͼ̵�����10
#define	             RW_TX11  566	//TX11-�����ͼ̵�����11
#define	             RW_TX12  567	//TX12-�����ͼ̵�����12
#define	             RW_TX13  568	//TX13-�����ͼ̵�����13
#define	             RW_TX14  569	//TX14-�����ͼ̵�����14
#define	             RW_TX15  570	//TX15-�����ͼ̵�����15
#define	             RW_TX16  571	//TX16-�����ͼ̵�����16
#define	              RW_RX1  572	//RX1-������ռ̵�����1
#define	              RW_RX2  573	//RX2-������ռ̵�����2
#define	              RW_RX3  574	//RX3-������ռ̵�����3
#define	              RW_RX4  575	//RX4-������ռ̵�����4
#define	              RW_RX5  576	//RX5-������ռ̵�����5
#define	              RW_RX6  577	//RX6-������ռ̵�����6
#define	              RW_RX7  578	//RX7-������ռ̵�����7
#define	              RW_RX8  579	//RX8-������ռ̵�����8
#define	              RW_RX9  580	//RX9-������ռ̵�����9
#define	             RW_RX10  581	//RX10-������ռ̵�����10
#define	             RW_RX11  582	//RX11-������ռ̵�����11
#define	             RW_RX12  583	//RX12-������ռ̵�����12
#define	             RW_RX13  584	//RX13-������ռ̵�����13
#define	             RW_RX14  585	//RX14-������ռ̵�����14
#define	             RW_RX15  586	//RX15-������ռ̵�����15
#define	             RW_RX16  587	//RX16-������ռ̵�����16
#define	             RW_CNT1  588	//CNT1-�����̵�����1
#define	             RW_CNT2  589	//CNT2-�����̵�����2
#define	             RW_CNT3  590	//CNT3-�����̵�����3
#define	             RW_CNT4  591	//CNT4-�����̵�����4
#define	             RW_CNT5  592	//CNT5-�����̵�����5
#define	             RW_CNT6  593	//CNT6-�����̵�����6
#define	             RW_CNT7  594	//CNT7-�����̵�����7
#define	             RW_CNT8  595	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			596		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			71	

//�м�̵�����
#define	MID_RELAY_WORD_START			73	
#define	MID_RELAY_WORD_NUM			80	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			153	
#define	KEEP_RELAY_WORD_NUM			33	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			186	
#define	TIME_RELAY_WORD_NUM			33	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			219	
#define	TRIP_RELAY_WORD_NUM			3	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			222	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			254	
#define	INPUT_RELAY_WORD_NUM			58	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			312	
#define	OUTPUT_RELAY_WORD_NUM			19	

//ѹ��̵���
#define	SW_RELAY_WORD_START			331	
#define	SW_RELAY_WORD_NUM			10	

//���ü̵���
#define	CFG_RELAY_WORD_START			341	
#define	CFG_RELAY_WORD_NUM			14	

//����̵���
#define	PULSE_RELAY_WORD_START			355	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			371	
#define	CTRLTEST_RELAY_WORD_NUM			11	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			382	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			398	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			422	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			438	
#define	RCP_RELAY_WORD_NUM			34	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			472	
#define	RCE_RELAY_WORD_NUM			34	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			506	
#define	RCV_RELAY_WORD_NUM			34	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			540	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			548	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			556	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			572	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			588	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			588	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			588	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			588	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			588	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			588	
#define	CNT_RELAY_WORD_NUM			8	


#endif