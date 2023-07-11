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
#define	           RW_PI74PT    2	//PI74PT-PT������μ̵�����
#define	           RW_AR74PT    3	//AR74PT-PT���߸澯�̵�����
#define	             RW_50I1    4	//50I1-IT1�����̵�����
#define	             RW_37I1    5	//37I1-IT1�����̵�����
#define	             RW_50I2    6	//50I2-IT2�����̵�����
#define	             RW_37I2    7	//37I2-IT2�����̵�����
#define	             RW_50I3    8	//50I3-IT3�����̵�����
#define	             RW_37I3    9	//37I3-IF3�����̵�����
#define	             RW_59U1   10	//59U1-U1��ѹ�̵�����
#define	             RW_27U1   11	//27U1-U1��ѹ�̵�����
#define	             RW_59U2   12	//59U2-U2��ѹ�̵�����
#define	             RW_27U2   13	//27U2-U2��ѹ�̵�����
#define	             RW_59U3   14	//59U3-U3��ѹ�̵�����
#define	             RW_27U3   15	//27U3-U3��ѹ�̵�����
#define	             RW_59U4   16	//59U4-U4��ѹ�̵�����
#define	             RW_27U4   17	//27U4-U4��ѹ�̵�����
#define	         RW_PI50DI_1   18	//PI50DI_1-��������������μ̵�����
#define	         RW_TR50DI_1   19	//TR50DI_1-�����������������̵�����
#define	        RW_LPC50DI_1   20	//LPC50DI_1-������������ѭ�����Ƽ̵�����
#define	         RW_PI50DI_2   21	//PI50DI_2-��������������μ̵�����
#define	         RW_TR50DI_2   22	//TR50DI_2-�����������������̵�����
#define	        RW_LPC50DI_2   23	//LPC50DI_2-������������ѭ�����Ƽ̵�����
#define	             RW_PI21   24	//PI21-���뱣����μ̵�����
#define	             RW_TR21   25	//TR21-���뱣�������̵�����
#define	            RW_LPC21   26	//LPC21-���뱣��ѭ�����Ƽ̵�����
#define	            RW_PI50I   27	//PI50I-����������μ̵�����
#define	            RW_TR50I   28	//TR50I-�������������̵�����
#define	           RW_LPC50I   29	//LPC50I-��������ѭ�����Ƽ̵�����
#define	             RW_LKH2   30	//LKH2-����г�������̵�����
#define	        RW_RELPICKUP   31	//RELPICKUP-���������̵�����
#define	        RW_AFLPICKUP   32	//AFLPICKUP-��������̵�����
#define	        RW_MFLPICKUP   33	//MFLPICKUP-ʵ����̵�����
#define	           RW_AFLACT   34	//AFLACT-������ද���̵�����
#define	           RW_MFLACT   35	//MFLACT-ʵ���ද���̵�����
#define	           RW_CH1ERR   36	//CH1ERR-ͨ��1���ϼ̵�����
#define	           RW_CH2ERR   37	//CH2ERR-ͨ��2���ϼ̵�����
#define	           RW_CH3ERR   38	//CH3ERR-ͨ��3���ϼ̵�����
#define	           RW_CH4ERR   39	//CH4ERR-ͨ��4���ϼ̵�����
#define	           RW_CH5ERR   40	//CH5ERR-ͨ��5���ϼ̵�����
#define	          RW_ET11ERR   41	//ET11ERR-��̫��1����1�̵�����
#define	          RW_ET12ERR   42	//ET12ERR-��̫��1����2�̵�����
#define	          RW_ET13ERR   43	//ET13ERR-��̫��1����3�̵�����
#define	          RW_ET14ERR   44	//ET14ERR-��̫��1����4�̵�����
#define	          RW_ET15ERR   45	//ET15ERR-��̫��1����5�̵�����
#define	          RW_ET21ERR   46	//ET21ERR-��̫��2����1�̵�����
#define	          RW_ET22ERR   47	//ET22ERR-��̫��2����2�̵�����
#define	          RW_ET23ERR   48	//ET23ERR-��̫��2����3�̵�����
#define	          RW_ET24ERR   49	//ET24ERR-��̫��2����4�̵�����
#define	          RW_ET25ERR   50	//ET25ERR-��̫��2����5�̵�����
#define	          RW_COM1ERR   51	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   52	//CAN1ERR-CAN1����״̬�̵�����
#define	          RW_CAN2ERR   53	//CAN2ERR-CAN2����״̬�̵�����
#define	         RW_NULL1ERR   54	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   55	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   56	//NULL3ERR-��3����״̬�̵�����
#define	     RW_HANDOVERMODE   57	//HANDOVERMODE-Խ������ģʽ
#define	           RW_GPSERR   58	//GPSERR-����GPS����
#define	        RW_MAINTLINK   59	//MAINTLINK-ά��״̬�̵�����
#define	             RW_69IN   60	//69IN-Զ�������м�̵�����
#define	              RW_52A   61	//52A-��λ
#define	              RW_52B   62	//52B-��λ
#define	               RW_M1   63	//M1-�м�̵�����M1
#define	               RW_M2   64	//M2-�м�̵�����M2
#define	               RW_M3   65	//M3-�м�̵�����M3
#define	               RW_M4   66	//M4-�м�̵�����M4
#define	               RW_M5   67	//M5-�м�̵�����M5
#define	               RW_M6   68	//M6-�м�̵�����M6
#define	               RW_M7   69	//M7-�м�̵�����M7
#define	               RW_M8   70	//M8-�м�̵�����M8
#define	               RW_M9   71	//M9-�м�̵�����M9
#define	              RW_M10   72	//M10-�м�̵�����M10
#define	              RW_M11   73	//M11-�м�̵�����M11
#define	              RW_M12   74	//M12-�м�̵�����M12
#define	              RW_M13   75	//M13-�м�̵�����M13
#define	              RW_M14   76	//M14-�м�̵�����M14
#define	              RW_M15   77	//M15-�м�̵�����M15
#define	              RW_M16   78	//M16-�м�̵�����M16
#define	              RW_M17   79	//M17-�м�̵�����M17
#define	              RW_M18   80	//M18-�м�̵�����M18
#define	              RW_M19   81	//M19-�м�̵�����M19
#define	              RW_M20   82	//M20-�м�̵�����M20
#define	              RW_M21   83	//M21-�м�̵�����M21
#define	              RW_M22   84	//M22-�м�̵�����M22
#define	              RW_M23   85	//M23-�м�̵�����M23
#define	              RW_M24   86	//M24-�м�̵�����M24
#define	              RW_M25   87	//M25-�м�̵�����M25
#define	              RW_M26   88	//M26-�м�̵�����M26
#define	              RW_M27   89	//M27-�м�̵�����M27
#define	              RW_M28   90	//M28-�м�̵�����M28
#define	              RW_M29   91	//M29-�м�̵�����M29
#define	              RW_M30   92	//M30-�м�̵�����M30
#define	              RW_M31   93	//M31-�м�̵�����M31
#define	              RW_M32   94	//M32-�м�̵�����M32
#define	              RW_M33   95	//M33-�м�̵�����M33
#define	              RW_M34   96	//M34-�м�̵�����M34
#define	              RW_M35   97	//M35-�м�̵�����M35
#define	              RW_M36   98	//M36-�м�̵�����M36
#define	              RW_M37   99	//M37-�м�̵�����M37
#define	              RW_M38  100	//M38-�м�̵�����M38
#define	              RW_M39  101	//M39-�м�̵�����M39
#define	              RW_M40  102	//M40-�м�̵�����M40
#define	              RW_M41  103	//M41-�м�̵�����M41
#define	              RW_M42  104	//M42-�м�̵�����M42
#define	              RW_M43  105	//M43-�м�̵�����M43
#define	              RW_M44  106	//M44-�м�̵�����M44
#define	              RW_M45  107	//M45-�м�̵�����M45
#define	              RW_M46  108	//M46-�м�̵�����M46
#define	              RW_M47  109	//M47-�м�̵�����M47
#define	              RW_M48  110	//M48-�м�̵�����M48
#define	              RW_M49  111	//M49-�м�̵�����M49
#define	              RW_M50  112	//M50-�м�̵�����M50
#define	              RW_M51  113	//M51-�м�̵�����M51
#define	              RW_M52  114	//M52-�м�̵�����M52
#define	              RW_M53  115	//M53-�м�̵�����M53
#define	              RW_M54  116	//M54-�м�̵�����M54
#define	              RW_M55  117	//M55-�м�̵�����M55
#define	              RW_M56  118	//M56-�м�̵�����M56
#define	              RW_M57  119	//M57-�м�̵�����M57
#define	              RW_M58  120	//M58-�м�̵�����M58
#define	              RW_M59  121	//M59-�м�̵�����M59
#define	              RW_M60  122	//M60-�м�̵�����M60
#define	              RW_M61  123	//M61-�м�̵�����M61
#define	              RW_M62  124	//M62-�м�̵�����M62
#define	              RW_M63  125	//M63-�м�̵�����M63
#define	              RW_M64  126	//M64-�м�̵�����M64
#define	              RW_REC  127	//REC-¼���̵�����
#define	             RW_LKCL  128	//LKCL-��բ�����̵�����
#define	             RW_LKTR  129	//LKTR-��բ�����̵�����
#define	       RW_LKPI50DI_1  130	//LKPI50DI_1-��������������α����̵�����
#define	      RW_LKRST50DI_1  131	//LKRST50DI_1-���������������ر����̵�����
#define	       RW_LKPI50DI_2  132	//LKPI50DI_2-��������������α����̵�����
#define	      RW_LKRST50DI_2  133	//LKRST50DI_2-���������������ر����̵�����
#define	           RW_LKPI21  134	//LKPI21-���뱣����α����̵�����
#define	          RW_LKRST21  135	//LKRST21-���뱣�����ر����̵�����
#define	          RW_LKPI50I  136	//LKPI50I-����������α����̵�����
#define	         RW_LKRST50I  137	//LKRST50I-�����������ر����̵�����
#define	           RW_MSTART  138	//MSTART-ʵ���������̵�����
#define	       RW_GETHISTORY  139	//GETHISTORY-�ٲ���ʷ����
#define	        RW_GETLOWVOL  140	//GETLOWVOL-�ٲ��ѹ��������
#define	           RW_LKFLPI  141	//LKFLPI-������������̵�����
#define	         RW_HANDOVER  142	//HANDOVER-Խ���л��̵�����
#define	          RW_RECOVER  143	//RECOVER-Խ���ָ��̵�����
#define	          RW_BGK152A  144	//BGK152A-���ø���1����λ�ü̵�����
#define	          RW_BGK252A  145	//BGK252A-���ø���2����λ�ü̵�����
#define	               RW_H1  146	//H1-�Ա����м�̵�����1
#define	               RW_H2  147	//H2-�Ա����м�̵�����2
#define	               RW_H3  148	//H3-�Ա����м�̵�����3
#define	               RW_H4  149	//H4-�Ա����м�̵�����4
#define	               RW_H5  150	//H5-�Ա����м�̵�����5
#define	               RW_H6  151	//H6-�Ա����м�̵�����6
#define	               RW_H7  152	//H7-�Ա����м�̵�����7
#define	               RW_H8  153	//H8-�Ա����м�̵�����8
#define	               RW_H9  154	//H9-�Ա����м�̵�����9
#define	              RW_H10  155	//H10-�Ա����м�̵�����10
#define	              RW_H11  156	//H11-�Ա����м�̵�����11
#define	              RW_H12  157	//H12-�Ա����м�̵�����12
#define	              RW_H13  158	//H13-�Ա����м�̵�����13
#define	              RW_H14  159	//H14-�Ա����м�̵�����14
#define	              RW_H15  160	//H15-�Ա����м�̵�����15
#define	              RW_H16  161	//H16-�Ա����м�̵�����16
#define	              RW_H17  162	//H17-�Ա����м�̵�����17
#define	              RW_H18  163	//H18-�Ա����м�̵�����18
#define	              RW_H19  164	//H19-�Ա����м�̵�����19
#define	              RW_H20  165	//H20-�Ա����м�̵�����20
#define	              RW_H21  166	//H21-�Ա����м�̵�����21
#define	              RW_H22  167	//H22-�Ա����м�̵�����22
#define	              RW_H23  168	//H23-�Ա����м�̵�����23
#define	              RW_H24  169	//H24-�Ա����м�̵�����24
#define	              RW_H25  170	//H25-�Ա����м�̵�����25
#define	              RW_H26  171	//H26-�Ա����м�̵�����26
#define	              RW_H27  172	//H27-�Ա����м�̵�����27
#define	              RW_H28  173	//H28-�Ա����м�̵�����28
#define	              RW_H29  174	//H29-�Ա����м�̵�����29
#define	              RW_H30  175	//H30-�Ա����м�̵�����30
#define	              RW_H31  176	//H31-�Ա����м�̵�����31
#define	              RW_H32  177	//H32-�Ա����м�̵�����32
#define	             RW_52BF  178	//52BF-��·����բʧ�̵ܼ�����
#define	               RW_T1  179	//T1-T1
#define	               RW_T2  180	//T2-T2
#define	               RW_T3  181	//T3-T3
#define	               RW_T4  182	//T4-T4
#define	               RW_T5  183	//T5-T5
#define	               RW_T6  184	//T6-T6
#define	               RW_T7  185	//T7-T7
#define	               RW_T8  186	//T8-T8
#define	               RW_T9  187	//T9-T9
#define	              RW_T10  188	//T10-T10
#define	              RW_T11  189	//T11-T11
#define	              RW_T12  190	//T12-T12
#define	              RW_T13  191	//T13-T13
#define	              RW_T14  192	//T14-T14
#define	              RW_T15  193	//T15-T15
#define	              RW_T16  194	//T16-T16
#define	             RW_TSW1  195	//TSW1-TSW1
#define	             RW_TSW2  196	//TSW2-TSW2
#define	             RW_TSW3  197	//TSW3-TSW3
#define	             RW_TSW4  198	//TSW4-TSW4
#define	             RW_TSW5  199	//TSW5-TSW5
#define	             RW_TSW6  200	//TSW6-TSW6
#define	             RW_TSW7  201	//TSW7-TSW7
#define	             RW_TSW8  202	//TSW8-TSW8
#define	             RW_TCB1  203	//TCB1-TCB1
#define	             RW_TCB2  204	//TCB2-TCB2
#define	             RW_TCB3  205	//TCB3-TCB3
#define	             RW_TCB4  206	//TCB4-TCB4
#define	             RW_TCB5  207	//TCB5-TCB5
#define	             RW_TCB6  208	//TCB6-TCB6
#define	             RW_TCB7  209	//TCB7-TCB7
#define	             RW_TCB8  210	//TCB8-TCB8
#define	            RW_T52BF  211	//T52BF-T52BF
#define	              RW_TR1  212	//TR1-��բ�̵���1
#define	              RW_TR2  213	//TR2-��բ�̵���2
#define	              RW_VR1  214	//VR1-����̵�����1
#define	              RW_VR2  215	//VR2-����̵�����2
#define	              RW_VR3  216	//VR3-����̵�����3
#define	              RW_VR4  217	//VR4-����̵�����4
#define	              RW_VR5  218	//VR5-����̵�����5
#define	              RW_VR6  219	//VR6-����̵�����6
#define	              RW_VR7  220	//VR7-����̵�����7
#define	              RW_VR8  221	//VR8-����̵�����8
#define	              RW_VR9  222	//VR9-����̵�����9
#define	             RW_VR10  223	//VR10-����̵�����10
#define	             RW_VR11  224	//VR11-����̵�����11
#define	             RW_VR12  225	//VR12-����̵�����12
#define	             RW_VR13  226	//VR13-����̵�����13
#define	             RW_VR14  227	//VR14-����̵�����14
#define	             RW_VR15  228	//VR15-����̵�����15
#define	             RW_VR16  229	//VR16-����̵�����16
#define	             RW_VR17  230	//VR17-����̵�����17
#define	             RW_VR18  231	//VR18-����̵�����18
#define	             RW_VR19  232	//VR19-����̵�����19
#define	             RW_VR20  233	//VR20-����̵�����20
#define	             RW_VR21  234	//VR21-����̵�����21
#define	             RW_VR22  235	//VR22-����̵�����22
#define	             RW_VR23  236	//VR23-����̵�����23
#define	             RW_VR24  237	//VR24-����̵�����24
#define	             RW_VR25  238	//VR25-����̵�����25
#define	             RW_VR26  239	//VR26-����̵�����26
#define	             RW_VR27  240	//VR27-����̵�����27
#define	             RW_VR28  241	//VR28-����̵�����28
#define	             RW_VR29  242	//VR29-����̵�����29
#define	             RW_VR30  243	//VR30-����̵�����30
#define	             RW_VR31  244	//VR31-����̵�����31
#define	             RW_VR32  245	//VR32-����̵�����32
#define	              RW_IN1  246	//IN1-IN1
#define	              RW_IN2  247	//IN2-IN2
#define	              RW_IN3  248	//IN3-IN3
#define	              RW_IN4  249	//IN4-IN4
#define	              RW_IN5  250	//IN5-IN5
#define	              RW_IN6  251	//IN6-IN6
#define	              RW_IN7  252	//IN7-IN7
#define	              RW_IN8  253	//IN8-IN8
#define	              RW_IN9  254	//IN9-IN9
#define	             RW_IN10  255	//IN10-IN10
#define	             RW_IN11  256	//IN11-IN11
#define	             RW_IN12  257	//IN12-IN12
#define	             RW_IN13  258	//IN13-IN13
#define	             RW_IN14  259	//IN14-IN14
#define	             RW_IN15  260	//IN15-IN15
#define	             RW_IN16  261	//IN16-IN16
#define	             RW_IN17  262	//IN17-IN17
#define	             RW_IN18  263	//IN18-IN18
#define	             RW_IN19  264	//IN19-IN19
#define	             RW_IN20  265	//IN20-IN20
#define	             RW_IN21  266	//IN21-IN21
#define	             RW_IN22  267	//IN22-IN22
#define	             RW_IN23  268	//IN23-IN23
#define	             RW_IN24  269	//IN24-IN24
#define	             RW_IN25  270	//IN25-IN25
#define	             RW_IN26  271	//IN26-IN26
#define	             RW_IN27  272	//IN27-IN27
#define	             RW_IN28  273	//IN28-IN28
#define	             RW_IN29  274	//IN29-IN29
#define	             RW_IN30  275	//IN30-IN30
#define	             RW_IN31  276	//IN31-IN31
#define	             RW_IN32  277	//IN32-IN32
#define	             RW_IN33  278	//IN33-IN33
#define	             RW_IN34  279	//IN34-IN34
#define	             RW_IN35  280	//IN35-�źŸ���
#define	             RW_IN36  281	//IN36-��բλ��
#define	             RW_IN37  282	//IN37-��բλ��
#define	             RW_IN38  283	//IN38-ң��Ԥ��
#define	             RW_IN39  284	//IN39-ң��1Ԥ��
#define	             RW_IN40  285	//IN40-ң��2Ԥ��
#define	           RW_INVIN1  286	//INVIN1-VIN1
#define	           RW_INVIN2  287	//INVIN2-VIN2
#define	           RW_INVIN3  288	//INVIN3-VIN3
#define	           RW_INVIN4  289	//INVIN4-VIN4
#define	           RW_INVIN5  290	//INVIN5-VIN5
#define	           RW_INVIN6  291	//INVIN6-VIN6
#define	           RW_INVIN7  292	//INVIN7-VIN7
#define	           RW_INVIN8  293	//INVIN8-VIN8
#define	           RW_INVIN9  294	//INVIN9-VIN9
#define	          RW_INVIN10  295	//INVIN10-VIN10
#define	          RW_INVIN11  296	//INVIN11-VIN11
#define	          RW_INVIN12  297	//INVIN12-VIN12
#define	          RW_INVIN13  298	//INVIN13-VIN13
#define	          RW_INVIN14  299	//INVIN14-VIN14
#define	          RW_INVIN15  300	//INVIN15-VIN15
#define	          RW_INVIN16  301	//INVIN16-VIN16
#define	             RW_OUT1  302	//OUT1-����1�̵�����
#define	             RW_OUT2  303	//OUT2-����2�̵�����
#define	             RW_OUT3  304	//OUT3-����3�̵�����
#define	             RW_OUT4  305	//OUT4-����4�̵�����
#define	             RW_OUT5  306	//OUT5-����5�̵�����
#define	             RW_OUT6  307	//OUT6-����6�̵�����
#define	             RW_OUT7  308	//OUT7-����7�̵�����
#define	             RW_OUT8  309	//OUT8-����8�̵�����
#define	             RW_OUT9  310	//OUT9-����9�̵�����
#define	            RW_OUT10  311	//OUT10-����10�̵�����
#define	            RW_OUT11  312	//OUT11-����11�̵�����
#define	            RW_OUT12  313	//OUT12-����12�̵�����
#define	            RW_OUT13  314	//OUT13-����13�̵�����
#define	            RW_OUT14  315	//OUT14-����14�̵�����
#define	             RW_SLFL  316	//SLFL-���ϲ��ѹ��̵�����
#define	          RW_SLMAINT  317	//SLMAINT-����ѹ��ѹ��̵�����
#define	          RW_SLRESD1  318	//SLRESD1-����ѹ��1
#define	          RW_SLRESD2  319	//SLRESD2-����ѹ��2
#define	            RW_FPI_Z  320	//FPI_Z-�迹�������ü̵�����
#define	            RW_FPI_C  321	//FPI_C-�����������ü̵�����
#define	          RW_FPI_50D  322	//FPI_50D-�����������ü̵�����
#define	         RW_FFL_AUTO  323	//FFL_AUTO-�������Ӧ
#define	          RW_FCCMODE  324	//FCCMODE-�����ϳɷ�ʽ
#define	              RW_FAT  325	//FAT-AT���ϵ��������ü̵�����
#define	              RW_FUD  326	//FUD-�����е��������ü̵�����
#define	              RW_FNL  327	//FNL-�������������ü̵�����
#define	         RW_FPHASESC  328	//FPHASESC-����·�ж����ü̵�����
#define	            RW_FNOAT  329	//FNOAT-�������AT���ü̵�����
#define	            RW_FISSS  330	//FISSS-����Ϊ��������ü̵�����
#define	          RW_F21_H_S  331	//F21_H_S-����г���������ü̵�����
#define	          RW_F21_H_L  332	//F21_H_L-����г���������ü̵�����
#define	          RW_F50_H_S  333	//F50_H_S-����г���������ü̵�����
#define	          RW_F50_H_L  334	//F50_H_L-����г���������ü̵�����
#define	         RW_FPARALLG  335	//FPARALLG-ƽ���ı������ü̵�����
#define	          RW_F50_27V  336	//F50_27V-������ѹ�������ü̵�����
#define	            RW_FCALL  337	//FCALL-�ٲ�ȡ��ѹ����
#define	             RW_FGPS  338	//FGPS-GPS���ϼ��
#define	         RW_FCHANNEL  339	//FCHANNEL-ͨ���Զ����ϼ��
#define	           RW_FCROSS  340	//FCROSS-����Խ�����ü̵�����
#define	           RW_FRESD1  341	//FRESD1-��������1
#define	           RW_FRESD2  342	//FRESD2-��������2
#define	              RW_PW1  343	//PW1-����̵���1
#define	              RW_PW2  344	//PW2-����̵���2
#define	              RW_PW3  345	//PW3-����̵���3
#define	              RW_PW4  346	//PW4-����̵���4
#define	              RW_PW5  347	//PW5-����̵���5
#define	              RW_PW6  348	//PW6-����̵���6
#define	              RW_PW7  349	//PW7-����̵���7
#define	              RW_PW8  350	//PW8-����̵���8
#define	              RW_PW9  351	//PW9-����̵���9
#define	             RW_PW10  352	//PW10-����̵���10
#define	             RW_PW11  353	//PW11-����̵���11
#define	             RW_PW12  354	//PW12-����̵���12
#define	             RW_PW13  355	//PW13-����̵���13
#define	             RW_PW14  356	//PW14-����̵���14
#define	             RW_PW15  357	//PW15-����̵���15
#define	             RW_PW16  358	//PW16-����̵���16
#define	              RW_TO1  359	//TO1-�����̵���1
#define	              RW_TO2  360	//TO2-�����̵���2
#define	              RW_TO3  361	//TO3-�����̵���3
#define	              RW_TO4  362	//TO4-�����̵���4
#define	              RW_TO5  363	//TO5-�����̵���5
#define	              RW_TO6  364	//TO6-�����̵���6
#define	              RW_TO7  365	//TO7-�����̵���7
#define	             RW_SIG1  366	//SIG1-�źż̵���1
#define	             RW_SIG2  367	//SIG2-�źż̵���2
#define	             RW_SIG3  368	//SIG3-�źż̵���3
#define	             RW_SIG4  369	//SIG4-�źż̵���4
#define	             RW_SIG5  370	//SIG5-�źż̵���5
#define	             RW_SIG6  371	//SIG6-�źż̵���6
#define	             RW_SIG7  372	//SIG7-�źż̵���7
#define	             RW_SIG8  373	//SIG8-�źż̵���8
#define	             RW_SIG9  374	//SIG9-�źż̵���9
#define	            RW_SIG10  375	//SIG10-�źż̵���10
#define	            RW_SIG11  376	//SIG11-�źż̵���11
#define	            RW_SIG12  377	//SIG12-�źż̵���12
#define	            RW_SIG13  378	//SIG13-�źż̵���13
#define	            RW_SIG14  379	//SIG14-�źż̵���14
#define	            RW_SIG15  380	//SIG15-�źż̵���15
#define	            RW_SIG16  381	//SIG16-�źż̵���16
#define	            RW_LED1G  382	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  383	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  384	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  385	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  386	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  387	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  388	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  389	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  390	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  391	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  392	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  393	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  394	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  395	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  396	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  397	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  398	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  399	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  400	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  401	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  402	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  403	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  404	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  405	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  406	//VIN1-VIN1
#define	             RW_VIN2  407	//VIN2-VIN2
#define	             RW_VIN3  408	//VIN3-VIN3
#define	             RW_VIN4  409	//VIN4-VIN4
#define	             RW_VIN5  410	//VIN5-VIN5
#define	             RW_VIN6  411	//VIN6-VIN6
#define	             RW_VIN7  412	//VIN7-VIN7
#define	             RW_VIN8  413	//VIN8-VIN8
#define	             RW_VIN9  414	//VIN9-VIN9
#define	            RW_VIN10  415	//VIN10-VIN10
#define	            RW_VIN11  416	//VIN11-VIN11
#define	            RW_VIN12  417	//VIN12-VIN12
#define	            RW_VIN13  418	//VIN13-VIN13
#define	            RW_VIN14  419	//VIN14-VIN14
#define	            RW_VIN15  420	//VIN15-VIN15
#define	            RW_VIN16  421	//VIN16-VIN16
#define	             RW_RCP1  422	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  423	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  424	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  425	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  426	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  427	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  428	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  429	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  430	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  431	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  432	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  433	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  434	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  435	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  436	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  437	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  438	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  439	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  440	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  441	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  442	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  443	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  444	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  445	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  446	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  447	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  448	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  449	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  450	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  451	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  452	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  453	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  454	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  455	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  456	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  457	//RCP36-ң��Ԥ�ü̵���36
#define	             RW_RCE1  458	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  459	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  460	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  461	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  462	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  463	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  464	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  465	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  466	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  467	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  468	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  469	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  470	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  471	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  472	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  473	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  474	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  475	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  476	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  477	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  478	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  479	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  480	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  481	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  482	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  483	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  484	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  485	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  486	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  487	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  488	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  489	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  490	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  491	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  492	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  493	//RCE36-ң��ִ�м̵���36
#define	             RW_RCV1  494	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  495	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  496	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  497	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  498	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  499	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  500	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  501	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  502	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  503	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  504	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  505	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  506	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  507	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  508	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  509	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  510	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  511	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  512	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  513	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  514	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  515	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  516	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  517	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  518	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  519	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  520	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  521	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  522	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  523	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  524	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  525	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  526	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  527	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  528	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  529	//RCV36-ң�ط�У�̵���36
#define	              RW_GE1  530	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  531	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  532	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  533	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  534	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  535	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  536	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  537	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  538	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  539	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  540	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  541	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  542	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  543	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  544	//CONST7-����ʱ��̵���7
#define	             RW_TLR1  545	//TLR1-�������Ӽ̵�����1
#define	             RW_TLR2  546	//TLR2-�������Ӽ̵�����2
#define	             RW_TLR3  547	//TLR3-�������Ӽ̵�����3
#define	             RW_TLR4  548	//TLR4-�������Ӽ̵�����4
#define	             RW_TLR5  549	//TLR5-�������Ӽ̵�����5
#define	             RW_TLR6  550	//TLR6-�������Ӽ̵�����6
#define	             RW_TLR7  551	//TLR7-�������Ӽ̵�����7
#define	             RW_TLR8  552	//TLR8-�������Ӽ̵�����8
#define	             RW_TLR9  553	//TLR9-�������Ӽ̵�����9
#define	            RW_TLR10  554	//TLR10-�������Ӽ̵�����10
#define	            RW_TLR11  555	//TLR11-�������Ӽ̵�����11
#define	            RW_TLR12  556	//TLR12-�������Ӽ̵�����12
#define	            RW_TLR13  557	//TLR13-�������Ӽ̵�����13
#define	            RW_TLR14  558	//TLR14-�������Ӽ̵�����14
#define	            RW_TLR15  559	//TLR15-�������Ӽ̵�����15
#define	            RW_TLR16  560	//TLR16-�������Ӽ̵�����16
#define	             RW_RLR1  561	//RLR1-�������Ӽ̵�����1
#define	             RW_RLR2  562	//RLR2-�������Ӽ̵�����2
#define	             RW_RLR3  563	//RLR3-�������Ӽ̵�����3
#define	             RW_RLR4  564	//RLR4-�������Ӽ̵�����4
#define	             RW_RLR5  565	//RLR5-�������Ӽ̵�����5
#define	             RW_RLR6  566	//RLR6-�������Ӽ̵�����6
#define	             RW_RLR7  567	//RLR7-�������Ӽ̵�����7
#define	             RW_RLR8  568	//RLR8-�������Ӽ̵�����8
#define	             RW_RLR9  569	//RLR9-�������Ӽ̵�����9
#define	            RW_RLR10  570	//RLR10-�������Ӽ̵�����10
#define	            RW_RLR11  571	//RLR11-�������Ӽ̵�����11
#define	            RW_RLR12  572	//RLR12-�������Ӽ̵�����12
#define	            RW_RLR13  573	//RLR13-�������Ӽ̵�����13
#define	            RW_RLR14  574	//RLR14-�������Ӽ̵�����14
#define	            RW_RLR15  575	//RLR15-�������Ӽ̵�����15
#define	            RW_RLR16  576	//RLR16-�������Ӽ̵�����16
#define	             RW_CNT1  577	//CNT1-�������̵�����1
#define	             RW_CNT2  578	//CNT2-�������̵�����2
#define	             RW_CNT3  579	//CNT3-�������̵�����3
#define	             RW_CNT4  580	//CNT4-�������̵�����4
#define	             RW_CNT5  581	//CNT5-�������̵�����5
#define	             RW_CNT6  582	//CNT6-�������̵�����6
#define	             RW_CNT7  583	//CNT7-�������̵�����7
#define	             RW_CNT8  584	//CNT8-�������̵�����8
#define	RELAY_WORD_NUM			585		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			58	

//�м�̵�����
#define	MID_RELAY_WORD_START			60	
#define	MID_RELAY_WORD_NUM			86	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			146	
#define	KEEP_RELAY_WORD_NUM			33	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			179	
#define	TIME_RELAY_WORD_NUM			33	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			212	
#define	TRIP_RELAY_WORD_NUM			2	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			214	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			246	
#define	INPUT_RELAY_WORD_NUM			56	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			302	
#define	OUTPUT_RELAY_WORD_NUM			14	

//ѹ��̵���
#define	SW_RELAY_WORD_START			316	
#define	SW_RELAY_WORD_NUM			4	

//���ü̵���
#define	CFG_RELAY_WORD_START			320	
#define	CFG_RELAY_WORD_NUM			23	

//����̵���
#define	PULSE_RELAY_WORD_START			343	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			359	
#define	CTRLTEST_RELAY_WORD_NUM			7	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			366	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			382	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			406	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			422	
#define	RCP_RELAY_WORD_NUM			36	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			458	
#define	RCE_RELAY_WORD_NUM			36	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			494	
#define	RCV_RELAY_WORD_NUM			36	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			530	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			538	
#define	CONST_TIME_RW_NUM			7	

//�������Ӽ̵���
#define	TXLINK_RW_START			545	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			561	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			577	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			577	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			577	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			577	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			577	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			577	
#define	CNT_RELAY_WORD_NUM			8	


#endif