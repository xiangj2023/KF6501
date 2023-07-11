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
#define	           RW_PI21_1    2	//PI21_1-����I����μ̵�����
#define	           RW_TR21_1    3	//TR21_1-����I�ζ����̵�����
#define	           RW_PI21_2    4	//PI21_2-����II����μ̵�����
#define	           RW_TR21_2    5	//TR21_2-����II�ζ����̵�����
#define	        RW_TR21_2_94    6	//TR21_2_94-����II�μ��ٶ����̵�����
#define	           RW_PI21_3    7	//PI21_3-����III����μ̵�����
#define	           RW_TR21_3    8	//TR21_3-����III�ζ����̵�����
#define	        RW_TR21_3_94    9	//TR21_3_94-����III�μ��ٶ����̵�����
#define	           RW_PI21_4   10	//PI21_4-����IV����μ̵�����
#define	           RW_TR21_4   11	//TR21_4-����IV�ζ����̵�����
#define	        RW_TR21_4_94   12	//TR21_4_94-����IV�μ��ٶ����̵�����
#define	           RW_PI50_0   13	//PI50_0-�ٶ���μ̵�����
#define	           RW_TR50_0   14	//TR50_0-�ٶ϶����̵�����
#define	           RW_PI50_1   15	//PI50_1-����I����μ̵�����
#define	           RW_TR50_1   16	//TR50_1-����I�ζ����̵�����
#define	        RW_TR50_1_94   17	//TR50_1_94-����I�μ��ٶ����̵�����
#define	           RW_PI50_2   18	//PI50_2-����II����μ̵�����
#define	           RW_TR50_2   19	//TR50_2-����II�ζ����̵�����
#define	        RW_TR50_2_94   20	//TR50_2_94-����II�μ��ٶ����̵�����
#define	           RW_PI50_3   21	//PI50_3-����III����μ̵�����
#define	           RW_TR50_3   22	//TR50_3-����III�ζ����̵�����
#define	        RW_TR50_3_94   23	//TR50_3_94-����III�μ��ٶ����̵�����
#define	             RW_PI27   24	//PI27-ʧѹ������μ̵�����
#define	             RW_TR27   25	//TR27-ʧѹ���������̵�����
#define	             RW_AR27   26	//AR27-ʧѹ�����澯�̵�����
#define	             RW_PI51   27	//PI51-��ʱ����μ̵�����
#define	             RW_TR51   28	//TR51-��ʱ�޶����̵�����
#define	            RW_PI50D   29	//PI50D-����������μ̵�����
#define	            RW_TR50D   30	//TR50D-�������������̵�����
#define	            RW_ARVT1   31	//ARVT1-PT1���߸澯�̵�����
#define	            RW_PIVT1   32	//PIVT1-PT1������μ̵�����
#define	            RW_ARVT2   33	//ARVT2-PT2���߸澯�̵�����
#define	            RW_PIVT2   34	//PIVT2-PT2������μ̵�����
#define	            RW_ARCT1   35	//ARCT1-CT1���߸澯�̵�����
#define	            RW_PICT1   36	//PICT1-CT1������μ̵�����
#define	            RW_ARCT2   37	//ARCT2-CT2���߸澯�̵�����
#define	            RW_PICT2   38	//PICT2-CT2������μ̵�����
#define	             RW_LKH2   39	//LKH2-����г�������̵�����
#define	              RW_32F   40	//32F-���ʷ�������̵�����
#define	              RW_32R   41	//32R-���ʷ�����̵�����
#define	             RW_50IP   42	//50IP-IP�����̵�����
#define	             RW_37IP   43	//37IP-IP�����̵�����
#define	             RW_50IT   44	//50IT-IT�����̵�����
#define	             RW_37IT   45	//37IT-IT�����̵�����
#define	             RW_50IF   46	//50IF-IF�����̵�����
#define	             RW_37IF   47	//37IF-IF�����̵�����
#define	             RW_59UP   48	//59UP-UP��ѹ�̵�����
#define	             RW_27UP   49	//27UP-UP��ѹ�̵�����
#define	             RW_59U1   50	//59U1-U1��ѹ�̵�����
#define	             RW_27U1   51	//27U1-U1��ѹ�̵�����
#define	             RW_59U2   52	//59U2-U2��ѹ�̵�����
#define	             RW_27U2   53	//27U2-U2��ѹ�̵�����
#define	             RW_59U3   54	//59U3-U3��ѹ�̵�����
#define	             RW_27U3   55	//27U3-U3��ѹ�̵�����
#define	             RW_59U4   56	//59U4-U4��ѹ�̵�����
#define	             RW_27U4   57	//27U4-U4��ѹ�̵�����
#define	            RW_LPC51   58	//LPC51-��ʱ�ޱ���ѭ�����Ƽ̵�����
#define	            RW_79SH0   59	//79SH0-�غ�բ����״̬�̵�����һ
#define	            RW_79SH1   60	//79SH1-�غ�բ����״̬�̵����ֶ�
#define	             RW_79TO   61	//79TO-�غ�բ��ʱ���߼��̵�����
#define	            RW_79CYS   62	//79CYS-�غ�բ����״̬�̵�����
#define	            RW_79RSS   63	//79RSS-�غ�բ����״̬�̵�����
#define	            RW_79LOS   64	//79LOS-�غ�բ����״̬�̵�����
#define	             RW_79CL   65	//79CL-�غ�բ�����߼��̵�����
#define	           RW_BTMODE   66	//BTMODE-ֱ����ʽ����
#define	          RW_AT1MODE   67	//AT1MODE-AT1��ʽ����(��ͨAT)
#define	          RW_AT2MODE   68	//AT2MODE-AT2��ʽ����(��רAT)
#define	           RW_PICKUP   69	//PICKUP-ģֵͻ���������̵�����
#define	          RW_LPC21_1   70	//LPC21_1-����I�α���ѭ�����Ƽ̵�����
#define	          RW_LPC21_2   71	//LPC21_2-����II�α���ѭ�����Ƽ̵�����
#define	          RW_LPC21_3   72	//LPC21_3-����III�α���ѭ�����Ƽ̵�����
#define	          RW_LPC21_4   73	//LPC21_4-����IV�α���ѭ�����Ƽ̵�����
#define	          RW_LPC50_0   74	//LPC50_0-�ٶϱ���ѭ�����Ƽ̵�����
#define	          RW_LPC50_1   75	//LPC50_1-����I�α���ѭ�����Ƽ̵�����
#define	          RW_LPC50_2   76	//LPC50_2-����II�α���ѭ�����Ƽ̵�����
#define	          RW_LPC50_3   77	//LPC50_3-����III�α���ѭ�����Ƽ̵�����
#define	            RW_LPC27   78	//LPC27-ʧѹ����ѭ�����Ƽ̵�����
#define	           RW_LPC50D   79	//LPC50D-��������ѭ�����Ƽ̵�����
#define	          RW_ET11ERR   80	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   81	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   82	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   83	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   84	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   85	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   86	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   87	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   88	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   89	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   90	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   91	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   92	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   93	//NULL1ERR-������1״̬�̵�����
#define	         RW_NULL2ERR   94	//NULL2ERR-������2״̬�̵�����
#define	         RW_NULL3ERR   95	//NULL3ERR-������3״̬�̵�����
#define	        RW_MAINTLINK   96	//MAINTLINK-ά��״̬�̵�����
#define	               RW_M1   97	//M1-�м�̵�����M1
#define	               RW_M2   98	//M2-�м�̵�����M2
#define	               RW_M3   99	//M3-�м�̵�����M3
#define	               RW_M4  100	//M4-�м�̵�����M4
#define	               RW_M5  101	//M5-�м�̵�����M5
#define	               RW_M6  102	//M6-�м�̵�����M6
#define	               RW_M7  103	//M7-�м�̵�����M7
#define	               RW_M8  104	//M8-�м�̵�����M8
#define	               RW_M9  105	//M9-�м�̵�����M9
#define	              RW_M10  106	//M10-�м�̵�����M10
#define	              RW_M11  107	//M11-�м�̵�����M11
#define	              RW_M12  108	//M12-�м�̵�����M12
#define	              RW_M13  109	//M13-�м�̵�����M13
#define	              RW_M14  110	//M14-�м�̵�����M14
#define	              RW_M15  111	//M15-�м�̵�����M15
#define	              RW_M16  112	//M16-�м�̵�����M16
#define	              RW_M17  113	//M17-�м�̵�����M17
#define	              RW_M18  114	//M18-�м�̵�����M18
#define	              RW_M19  115	//M19-�м�̵�����M19
#define	              RW_M20  116	//M20-�м�̵�����M20
#define	              RW_M21  117	//M21-�м�̵�����M21
#define	              RW_M22  118	//M22-�м�̵�����M22
#define	              RW_M23  119	//M23-�м�̵�����M23
#define	              RW_M24  120	//M24-�м�̵�����M24
#define	              RW_M25  121	//M25-�м�̵�����M25
#define	              RW_M26  122	//M26-�м�̵�����M26
#define	              RW_M27  123	//M27-�м�̵�����M27
#define	              RW_M28  124	//M28-�м�̵�����M28
#define	              RW_M29  125	//M29-�м�̵�����M29
#define	              RW_M30  126	//M30-�м�̵�����M30
#define	              RW_M31  127	//M31-�м�̵�����M31
#define	              RW_M32  128	//M32-�м�̵�����M32
#define	              RW_M33  129	//M33-�м�̵�����M33
#define	              RW_M34  130	//M34-�м�̵�����M34
#define	              RW_M35  131	//M35-�м�̵�����M35
#define	              RW_M36  132	//M36-�м�̵�����M36
#define	              RW_M37  133	//M37-�м�̵�����M37
#define	              RW_M38  134	//M38-�м�̵�����M38
#define	              RW_M39  135	//M39-�м�̵�����M39
#define	              RW_M40  136	//M40-�м�̵�����M40
#define	              RW_M41  137	//M41-�м�̵�����M41
#define	              RW_M42  138	//M42-�м�̵�����M42
#define	              RW_M43  139	//M43-�м�̵�����M43
#define	              RW_M44  140	//M44-�м�̵�����M44
#define	              RW_M45  141	//M45-�м�̵�����M45
#define	              RW_M46  142	//M46-�м�̵�����M46
#define	              RW_M47  143	//M47-�м�̵�����M47
#define	              RW_M48  144	//M48-�м�̵�����M48
#define	              RW_M49  145	//M49-�м�̵�����M49
#define	              RW_M50  146	//M50-�м�̵�����M50
#define	              RW_M51  147	//M51-�м�̵�����M51
#define	              RW_M52  148	//M52-�м�̵�����M52
#define	              RW_M53  149	//M53-�м�̵�����M53
#define	              RW_M54  150	//M54-�м�̵�����M54
#define	              RW_M55  151	//M55-�м�̵�����M55
#define	              RW_M56  152	//M56-�м�̵�����M56
#define	              RW_M57  153	//M57-�м�̵�����M57
#define	              RW_M58  154	//M58-�м�̵�����M58
#define	              RW_M59  155	//M59-�м�̵�����M59
#define	              RW_M60  156	//M60-�м�̵�����M60
#define	              RW_M61  157	//M61-�м�̵�����M61
#define	              RW_M62  158	//M62-�м�̵�����M62
#define	              RW_M63  159	//M63-�м�̵�����M63
#define	              RW_M64  160	//M64-�м�̵�����M64
#define	         RW_LKPI21_1  161	//LKPI21_1-����I����α����̵�����
#define	         RW_LKPI21_2  162	//LKPI21_2-����II����α����̵�����
#define	         RW_LKPI21_3  163	//LKPI21_3-����III����α����̵�����
#define	         RW_LKPI21_4  164	//LKPI21_4-����IV����α����̵�����
#define	         RW_LKPI50_0  165	//LKPI50_0-�ٶ���α����̵�����
#define	        RW_LKRST50_0  166	//LKRST50_0-�ٶϷ��ر����̵�����
#define	         RW_LKPI50_1  167	//LKPI50_1-����I����α����̵�����
#define	        RW_LKRST50_1  168	//LKRST50_1-����I�η��ر����̵�����
#define	         RW_LKPI50_2  169	//LKPI50_2-����II����α����̵�����
#define	        RW_LKRST50_2  170	//LKRST50_2-����II�η��ر����̵�����
#define	         RW_LKPI50_3  171	//LKPI50_3-����III����α����̵�����
#define	        RW_LKRST50_3  172	//LKRST50_3-����III�η��ر����̵�����
#define	           RW_LKPI27  173	//LKPI27-ʧѹ������α����̵�����
#define	          RW_LKRST27  174	//LKRST27-ʧѹ�������ر����̵�����
#define	           RW_LKPI51  175	//LKPI51-��ʱ����α����̵�����
#define	          RW_LKRST51  176	//LKRST51-��ʱ�޷��ر����̵�����
#define	          RW_LKPI50D  177	//LKPI50D-������α����̵�����
#define	         RW_LKRST50D  178	//LKRST50D-�������ر����̵�����
#define	            RW_79PIS  179	//79PIS-�غ�բ���������߼��̵�����
#define	            RW_79CLC  180	//79CLC-�غ�բ����������̵�����
#define	             RW_79LK  181	//79LK-�غ�բ�����߼��̵�����
#define	            RW_79CLS  182	//79CLS-�غ�բ���������߼��̵�����
#define	           RW_79TRCK  183	//79TRCK-�غ�բ���������жϼ̵�����
#define	              RW_F79  184	//F79-�غ�բ���ü̵�����
#define	             RW_69IN  185	//69IN-Զ�������м�̵�����
#define	              RW_52A  186	//52A-��λ
#define	              RW_52B  187	//52B-��λ
#define	              RW_REC  188	//REC-¼���̵�����
#define	               RW_H1  189	//H1-�Ա����м�̵�����1
#define	               RW_H2  190	//H2-�Ա����м�̵�����2
#define	               RW_H3  191	//H3-�Ա����м�̵�����3
#define	               RW_H4  192	//H4-�Ա����м�̵�����4
#define	               RW_H5  193	//H5-�Ա����м�̵�����5
#define	               RW_H6  194	//H6-�Ա����м�̵�����6
#define	               RW_H7  195	//H7-�Ա����м�̵�����7
#define	               RW_H8  196	//H8-�Ա����м�̵�����8
#define	               RW_H9  197	//H9-�Ա����м�̵�����9
#define	              RW_H10  198	//H10-�Ա����м�̵�����10
#define	              RW_H11  199	//H11-�Ա����м�̵�����11
#define	              RW_H12  200	//H12-�Ա����м�̵�����12
#define	              RW_H13  201	//H13-�Ա����м�̵�����13
#define	              RW_H14  202	//H14-�Ա����м�̵�����14
#define	              RW_H15  203	//H15-�Ա����м�̵�����15
#define	              RW_H16  204	//H16-�Ա����м�̵�����16
#define	              RW_H17  205	//H17-�Ա����м�̵�����17
#define	              RW_H18  206	//H18-�Ա����м�̵�����18
#define	              RW_H19  207	//H19-�Ա����м�̵�����19
#define	              RW_H20  208	//H20-�Ա����м�̵�����20
#define	              RW_H21  209	//H21-�Ա����м�̵�����21
#define	              RW_H22  210	//H22-�Ա����м�̵�����22
#define	              RW_H23  211	//H23-�Ա����м�̵�����23
#define	              RW_H24  212	//H24-�Ա����м�̵�����24
#define	              RW_H25  213	//H25-�Ա����м�̵�����25
#define	              RW_H26  214	//H26-�Ա����м�̵�����26
#define	              RW_H27  215	//H27-�Ա����м�̵�����27
#define	              RW_H28  216	//H28-�Ա����м�̵�����28
#define	              RW_H29  217	//H29-�Ա����м�̵�����29
#define	              RW_H30  218	//H30-�Ա����м�̵�����30
#define	              RW_H31  219	//H31-�Ա����м�̵�����31
#define	              RW_H32  220	//H32-�Ա����м�̵�����32
#define	             RW_79PI  221	//79PI-�غ�բ�����߼��̵�����
#define	             RW_52BF  222	//52BF-��·����բʧ�̵ܼ�����
#define	               RW_T1  223	//T1-T1
#define	               RW_T2  224	//T2-T2
#define	               RW_T3  225	//T3-T3
#define	               RW_T4  226	//T4-T4
#define	               RW_T5  227	//T5-T5
#define	               RW_T6  228	//T6-T6
#define	               RW_T7  229	//T7-T7
#define	               RW_T8  230	//T8-T8
#define	               RW_T9  231	//T9-T9
#define	              RW_T10  232	//T10-T10
#define	              RW_T11  233	//T11-T11
#define	              RW_T12  234	//T12-T12
#define	              RW_T13  235	//T13-T13
#define	              RW_T14  236	//T14-T14
#define	              RW_T15  237	//T15-T15
#define	              RW_T16  238	//T16-T16
#define	             RW_TSW1  239	//TSW1-TSW1
#define	             RW_TSW2  240	//TSW2-TSW2
#define	             RW_TSW3  241	//TSW3-TSW3
#define	             RW_TSW4  242	//TSW4-TSW4
#define	             RW_TSW5  243	//TSW5-TSW5
#define	             RW_TSW6  244	//TSW6-TSW6
#define	             RW_TSW7  245	//TSW7-TSW7
#define	             RW_TSW8  246	//TSW8-TSW8
#define	             RW_TCB1  247	//TCB1-TCB1
#define	             RW_TCB2  248	//TCB2-TCB2
#define	             RW_TCB3  249	//TCB3-TCB3
#define	             RW_TCB4  250	//TCB4-TCB4
#define	             RW_TCB5  251	//TCB5-TCB5
#define	             RW_TCB6  252	//TCB6-TCB6
#define	             RW_TCB7  253	//TCB7-TCB7
#define	             RW_TCB8  254	//TCB8-TCB8
#define	            RW_TSOTF  255	//TSOTF-�غ�բ���ڹ����߼��̵�����
#define	            RW_T52BF  256	//T52BF-T52BF
#define	              RW_TR1  257	//TR1-��բ�̵���1
#define	              RW_TR2  258	//TR2-��բ�̵���2
#define	              RW_VR1  259	//VR1-����̵�����1
#define	              RW_VR2  260	//VR2-����̵�����2
#define	              RW_VR3  261	//VR3-����̵�����3
#define	              RW_VR4  262	//VR4-����̵�����4
#define	              RW_VR5  263	//VR5-����̵�����5
#define	              RW_VR6  264	//VR6-����̵�����6
#define	              RW_VR7  265	//VR7-����̵�����7
#define	              RW_VR8  266	//VR8-����̵�����8
#define	              RW_VR9  267	//VR9-����̵�����9
#define	             RW_VR10  268	//VR10-����̵�����10
#define	             RW_VR11  269	//VR11-����̵�����11
#define	             RW_VR12  270	//VR12-����̵�����12
#define	             RW_VR13  271	//VR13-����̵�����13
#define	             RW_VR14  272	//VR14-����̵�����14
#define	             RW_VR15  273	//VR15-����̵�����15
#define	             RW_VR16  274	//VR16-����̵�����16
#define	             RW_VR17  275	//VR17-����̵�����17
#define	             RW_VR18  276	//VR18-����̵�����18
#define	             RW_VR19  277	//VR19-����̵�����19
#define	             RW_VR20  278	//VR20-����̵�����20
#define	             RW_VR21  279	//VR21-����̵�����21
#define	             RW_VR22  280	//VR22-����̵�����22
#define	             RW_VR23  281	//VR23-����̵�����23
#define	             RW_VR24  282	//VR24-����̵�����24
#define	             RW_VR25  283	//VR25-����̵�����25
#define	             RW_VR26  284	//VR26-����̵�����26
#define	             RW_VR27  285	//VR27-����̵�����27
#define	             RW_VR28  286	//VR28-����̵�����28
#define	             RW_VR29  287	//VR29-����̵�����29
#define	             RW_VR30  288	//VR30-����̵�����30
#define	             RW_VR31  289	//VR31-����̵�����31
#define	             RW_VR32  290	//VR32-����̵�����32
#define	              RW_IN1  291	//IN1-����̵�����1
#define	              RW_IN2  292	//IN2-����̵�����2
#define	              RW_IN3  293	//IN3-����̵�����3
#define	              RW_IN4  294	//IN4-����̵�����4
#define	              RW_IN5  295	//IN5-����̵�����5
#define	              RW_IN6  296	//IN6-����̵�����6
#define	              RW_IN7  297	//IN7-����̵�����7
#define	              RW_IN8  298	//IN8-����̵�����8
#define	              RW_IN9  299	//IN9-����̵�����9
#define	             RW_IN10  300	//IN10-����̵�����10
#define	             RW_IN11  301	//IN11-����̵�����11
#define	             RW_IN12  302	//IN12-����̵�����12
#define	             RW_IN13  303	//IN13-����̵�����13
#define	             RW_IN14  304	//IN14-����̵�����14
#define	             RW_IN15  305	//IN15-����̵�����15
#define	             RW_IN16  306	//IN16-����̵�����16
#define	             RW_IN17  307	//IN17-����̵�����17
#define	             RW_IN18  308	//IN18-����̵�����18
#define	             RW_IN19  309	//IN19-����̵�����19
#define	             RW_IN20  310	//IN20-����̵�����20
#define	             RW_IN21  311	//IN21-����̵�����21
#define	             RW_IN22  312	//IN22-����̵�����22
#define	             RW_IN23  313	//IN23-����̵�����23
#define	             RW_IN24  314	//IN24-����̵�����24
#define	             RW_IN25  315	//IN25-����̵�����25
#define	             RW_IN26  316	//IN26-����̵�����26
#define	             RW_IN27  317	//IN27-����̵�����27
#define	             RW_IN28  318	//IN28-����̵�����28
#define	             RW_IN29  319	//IN29-����̵�����29
#define	             RW_IN30  320	//IN30-����̵�����30
#define	             RW_IN31  321	//IN31-����̵�����31
#define	             RW_IN32  322	//IN32-����̵�����32
#define	             RW_IN33  323	//IN33-����̵�����33
#define	             RW_IN34  324	//IN34-����̵�����34
#define	             RW_IN35  325	//IN35-�źŸ���
#define	             RW_IN36  326	//IN36-��բλ��
#define	             RW_IN37  327	//IN37-��բλ��
#define	             RW_IN38  328	//IN38-��բ�ź�
#define	             RW_IN39  329	//IN39-��բ�ź�
#define	             RW_IN40  330	//IN40-�����ض�
#define	             RW_IN41  331	//IN41-ң��Ԥ��
#define	             RW_IN42  332	//IN42-ң��1Ԥ��
#define	             RW_IN43  333	//IN43-ң��2Ԥ��
#define	             RW_IN44  334	//IN44-ң��3Ԥ��
#define	             RW_IN45  335	//IN45-ң��4Ԥ��
#define	           RW_INVIN1  336	//INVIN1-����̵�����46
#define	           RW_INVIN2  337	//INVIN2-����̵�����47
#define	           RW_INVIN3  338	//INVIN3-����̵�����48
#define	           RW_INVIN4  339	//INVIN4-����̵�����49
#define	           RW_INVIN5  340	//INVIN5-����̵�����50
#define	           RW_INVIN6  341	//INVIN6-����̵�����51
#define	           RW_INVIN7  342	//INVIN7-����̵�����52
#define	           RW_INVIN8  343	//INVIN8-����̵�����53
#define	           RW_INVIN9  344	//INVIN9-����̵�����54
#define	          RW_INVIN10  345	//INVIN10-����̵�����55
#define	          RW_INVIN11  346	//INVIN11-����̵�����56
#define	          RW_INVIN12  347	//INVIN12-����̵�����57
#define	          RW_INVIN13  348	//INVIN13-����̵�����58
#define	          RW_INVIN14  349	//INVIN14-����̵�����59
#define	          RW_INVIN15  350	//INVIN15-����̵�����60
#define	          RW_INVIN16  351	//INVIN16-����̵�����61
#define	             RW_OUT1  352	//OUT1-����1�̵�����
#define	             RW_OUT2  353	//OUT2-����2�̵�����
#define	             RW_OUT3  354	//OUT3-����3�̵�����
#define	             RW_OUT4  355	//OUT4-����4�̵�����
#define	             RW_OUT5  356	//OUT5-����5�̵�����
#define	             RW_OUT6  357	//OUT6-����6�̵�����
#define	             RW_OUT7  358	//OUT7-����7�̵�����
#define	             RW_OUT8  359	//OUT8-����8�̵�����
#define	             RW_OUT9  360	//OUT9-����9�̵�����
#define	            RW_OUT10  361	//OUT10-����10�̵�����
#define	            RW_OUT11  362	//OUT11-����11�̵�����
#define	            RW_OUT12  363	//OUT12-����12�̵�����
#define	            RW_OUT13  364	//OUT13-����13�̵�����
#define	            RW_OUT14  365	//OUT14-����14�̵�����
#define	            RW_OUT15  366	//OUT15-����15�̵�����
#define	            RW_OUT16  367	//OUT16-����16�̵�����
#define	            RW_OUT17  368	//OUT17-����17�̵�����
#define	            RW_OUT18  369	//OUT18-����18�̵�����
#define	            RW_OUT19  370	//OUT19-����19�̵�����
#define	            RW_OUT20  371	//OUT20-����20�̵�����
#define	            RW_OUT21  372	//OUT21-����21�̵�����
#define	             RW_SL21  373	//SL21-���뱣����ѹ��̵�����
#define	           RW_SL50_0  374	//SL50_0-�����ٶ���ѹ��̵�����
#define	         RW_SL50_123  375	//SL50_123-����������ѹ��̵�����
#define	            RW_SL50D  376	//SL50D-��������������ѹ��̵�����
#define	             RW_SL27  377	//SL27-��ѹ������ѹ��̵�����
#define	             RW_SL79  378	//SL79-�غ�բ��ѹ��̵�����
#define	          RW_SLMAINT  379	//SLMAINT-����ѹ��̵�����
#define	          RW_SLRESD1  380	//SLRESD1-����ѹ��1
#define	          RW_SLRESD2  381	//SLRESD2-����ѹ��2
#define	            RW_F21_1  382	//F21_1-����I�����ü̵�����
#define	           RW_F21_1F  383	//F21_1F-����I���������ü̵�����
#define	            RW_F21_2  384	//F21_2-����II�����ü̵�����
#define	           RW_F21_2F  385	//F21_2F-����II���������ü̵�����
#define	            RW_F21_3  386	//F21_3-����III�����ü̵�����
#define	           RW_F21_3F  387	//F21_3F-����III���������ü̵�����
#define	            RW_F21_4  388	//F21_4-����IV�����ü̵�����
#define	           RW_F21_4F  389	//F21_4F-����IV�������ü̵�����
#define	         RW_FPARALLE  390	//FPARALLE-ƽ���ı������ü̵�����
#define	          RW_F21_HLK  391	//F21_HLK-����г���������ü̵�����
#define	         RW_F21_HRES  392	//F21_HRES-����г���������ü̵�����
#define	            RW_F50_0  393	//F50_0-�����ٶ����ü̵�����
#define	         RW_F50_0_27  394	//F50_0_27-�ٶϵ�ѹ�������ü̵�����
#define	        RW_F50_0_HLK  395	//F50_0_HLK-�ٶ�г���������ü̵�����
#define	       RW_F50_0_HRES  396	//F50_0_HRES-�ٶ�г���������ü̵�����
#define	           RW_F50_0F  397	//F50_0F-�ٶϱ�������
#define	         RW_F50_0_67  398	//F50_0_67-�ٶϹ��ʷ������ü̵�����
#define	            RW_F50_1  399	//F50_1-����I�����ü̵�����
#define	           RW_F50_1F  400	//F50_1F-����I������
#define	         RW_F50_1_67  401	//F50_1_67-����I�ι��ʷ������ü̵�����
#define	         RW_F50_1_27  402	//F50_1_27-����I�ε�ѹ�������ü̵�����
#define	            RW_F50_2  403	//F50_2-����II�����ü̵�����
#define	           RW_F50_2F  404	//F50_2F-����II������
#define	         RW_F50_2_67  405	//F50_2_67-����II�ι��ʷ������ü̵�����
#define	         RW_F50_2_27  406	//F50_2_27-����II�ε�ѹ�������ü̵�����
#define	            RW_F50_3  407	//F50_3-����III�����ü̵�����
#define	       RW_F50_123HLK  408	//F50_123HLK-����г���������ü̵�����
#define	      RW_F50_123HRES  409	//F50_123HRES-����г���������ü̵�����
#define	              RW_F51  410	//F51-��ʱ�޹������ü̵�����
#define	             RW_F50D  411	//F50D-�����������ü̵�����
#define	              RW_F27  412	//F27-��ѹ�������ü̵�����
#define	            RW_F52BF  413	//F52BF-ʧ�鱣�����ü̵�����
#define	           RW_F74VT1  414	//F74VT1-PT1���߼�����ü̵�����
#define	           RW_F74VT2  415	//F74VT2-PT2���߼�����ü̵�����
#define	         RW_FFLOCATE  416	//FFLOCATE-���ϲ�����ü̵�����
#define	           RW_F79ITR  417	//F79ITR-͵���غ�բ���ü̵�����
#define	           RW_F79_51  418	//F79_51-��ʱ���غ�
#define	           RW_F79_21  419	//F79_21-���뱣���غ�
#define	         RW_F79_50_0  420	//F79_50_0-�ٶϱ����غ�
#define	       RW_F79_50_123  421	//F79_50_123-���������غ�
#define	          RW_F79_50D  422	//F79_50D-���������غ�
#define	         RW_F21_2_94  423	//F21_2_94-����II�μ������ü̵�����
#define	         RW_F21_3_94  424	//F21_3_94-����III�μ������ü̵�����
#define	         RW_F21_4_94  425	//F21_4_94-����IV�μ������ü̵�����
#define	         RW_F50_1_94  426	//F50_1_94-����I�μ������ü̵�����
#define	         RW_F50_2_94  427	//F50_2_94-����II�μ������ü̵�����
#define	         RW_F50_3_94  428	//F50_3_94-����III�μ������ü̵�����
#define	           RW_F79_59  429	//F79_59-�غ�բ����ѹ���ü̵�����
#define	          RW_FDUISUP  430	//FDUISUP-����¼�����ü̵�����
#define	            RW_FCTBR  431	//FCTBR-CT���߼�����ü̵�����
#define	           RW_FRESD1  432	//FRESD1-��������1
#define	           RW_FRESD2  433	//FRESD2-��������2
#define	              RW_PW1  434	//PW1-����̵���1
#define	              RW_PW2  435	//PW2-����̵���2
#define	              RW_PW3  436	//PW3-����̵���3
#define	              RW_PW4  437	//PW4-����̵���4
#define	              RW_PW5  438	//PW5-����̵���5
#define	              RW_PW6  439	//PW6-����̵���6
#define	              RW_PW7  440	//PW7-����̵���7
#define	              RW_PW8  441	//PW8-����̵���8
#define	              RW_PW9  442	//PW9-����̵���9
#define	             RW_PW10  443	//PW10-����̵���10
#define	             RW_PW11  444	//PW11-����̵���11
#define	             RW_PW12  445	//PW12-����̵���12
#define	             RW_PW13  446	//PW13-����̵���13
#define	             RW_PW14  447	//PW14-����̵���14
#define	             RW_PW15  448	//PW15-����̵���15
#define	             RW_PW16  449	//PW16-����̵���16
#define	              RW_TO1  450	//TO1-�����̵���1
#define	              RW_TO2  451	//TO2-�����̵���2
#define	              RW_TO3  452	//TO3-�����̵���3
#define	              RW_TO4  453	//TO4-�����̵���4
#define	              RW_TO5  454	//TO5-�����̵���5
#define	              RW_TO6  455	//TO6-�����̵���6
#define	              RW_TO7  456	//TO7-�����̵���7
#define	              RW_TO8  457	//TO8-�����̵���8
#define	              RW_TO9  458	//TO9-�����̵���9
#define	             RW_TO10  459	//TO10-�����̵���10
#define	             RW_TO11  460	//TO11-�����̵���11
#define	             RW_TO12  461	//TO12-�����̵���12
#define	             RW_TO13  462	//TO13-�����̵���13
#define	             RW_SIG1  463	//SIG1-�źż̵���1
#define	             RW_SIG2  464	//SIG2-�źż̵���2
#define	             RW_SIG3  465	//SIG3-�źż̵���3
#define	             RW_SIG4  466	//SIG4-�źż̵���4
#define	             RW_SIG5  467	//SIG5-�źż̵���5
#define	             RW_SIG6  468	//SIG6-�źż̵���6
#define	             RW_SIG7  469	//SIG7-�źż̵���7
#define	             RW_SIG8  470	//SIG8-�źż̵���8
#define	             RW_SIG9  471	//SIG9-�źż̵���9
#define	            RW_SIG10  472	//SIG10-�źż̵���10
#define	            RW_SIG11  473	//SIG11-�źż̵���11
#define	            RW_SIG12  474	//SIG12-�źż̵���12
#define	            RW_SIG13  475	//SIG13-�źż̵���13
#define	            RW_SIG14  476	//SIG14-�źż̵���14
#define	            RW_SIG15  477	//SIG15-�źż̵���15
#define	            RW_SIG16  478	//SIG16-�źż̵���16
#define	            RW_LED1G  479	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  480	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  481	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  482	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  483	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  484	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  485	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  486	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  487	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  488	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  489	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  490	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  491	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  492	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  493	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  494	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  495	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  496	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  497	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  498	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  499	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  500	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  501	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  502	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  503	//VIN1-���⿪��̵�����1
#define	             RW_VIN2  504	//VIN2-���⿪��̵�����2
#define	             RW_VIN3  505	//VIN3-���⿪��̵�����3
#define	             RW_VIN4  506	//VIN4-���⿪��̵�����4
#define	             RW_VIN5  507	//VIN5-���⿪��̵�����5
#define	             RW_VIN6  508	//VIN6-���⿪��̵�����6
#define	             RW_VIN7  509	//VIN7-���⿪��̵�����7
#define	             RW_VIN8  510	//VIN8-���⿪��̵�����8
#define	             RW_VIN9  511	//VIN9-���⿪��̵�����9
#define	            RW_VIN10  512	//VIN10-���⿪��̵�����10
#define	            RW_VIN11  513	//VIN11-���⿪��̵�����11
#define	            RW_VIN12  514	//VIN12-���⿪��̵�����12
#define	            RW_VIN13  515	//VIN13-���⿪��̵�����13
#define	            RW_VIN14  516	//VIN14-���⿪��̵�����14
#define	            RW_VIN15  517	//VIN15-���⿪��̵�����15
#define	            RW_VIN16  518	//VIN16-���⿪��̵�����16
#define	             RW_RCP1  519	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  520	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  521	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  522	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  523	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  524	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  525	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  526	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  527	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  528	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  529	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  530	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  531	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  532	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  533	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  534	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  535	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  536	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  537	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  538	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  539	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  540	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  541	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  542	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  543	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  544	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  545	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  546	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  547	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  548	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  549	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  550	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  551	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  552	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  553	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  554	//RCP36-ң��Ԥ�ü̵���36
#define	             RW_RCE1  555	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  556	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  557	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  558	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  559	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  560	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  561	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  562	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  563	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  564	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  565	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  566	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  567	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  568	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  569	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  570	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  571	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  572	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  573	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  574	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  575	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  576	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  577	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  578	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  579	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  580	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  581	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  582	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  583	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  584	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  585	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  586	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  587	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  588	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  589	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  590	//RCE36-ң��ִ�м̵���36
#define	             RW_RCV1  591	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  592	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  593	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  594	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  595	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  596	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  597	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  598	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  599	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  600	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  601	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  602	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  603	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  604	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  605	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  606	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  607	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  608	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  609	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  610	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  611	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  612	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  613	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  614	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  615	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  616	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  617	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  618	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  619	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  620	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  621	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  622	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  623	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  624	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  625	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  626	//RCV36-ң�ط�У�̵���36
#define	              RW_GE1  627	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  628	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  629	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  630	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  631	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  632	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  633	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  634	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  635	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  636	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  637	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  638	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  639	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  640	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  641	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  642	//CONST8-����ʱ��̵���8
#define	             RW_TLR1  643	//TLR1-�����ͼ̵�����1
#define	             RW_TLR2  644	//TLR2-�����ͼ̵�����2
#define	             RW_TLR3  645	//TLR3-�����ͼ̵�����3
#define	             RW_TLR4  646	//TLR4-�����ͼ̵�����4
#define	             RW_TLR5  647	//TLR5-�����ͼ̵�����5
#define	             RW_TLR6  648	//TLR6-�����ͼ̵�����6
#define	             RW_TLR7  649	//TLR7-�����ͼ̵�����7
#define	             RW_TLR8  650	//TLR8-�����ͼ̵�����8
#define	             RW_TLR9  651	//TLR9-�����ͼ̵�����9
#define	            RW_TLR10  652	//TLR10-�����ͼ̵�����10
#define	            RW_TLR11  653	//TLR11-�����ͼ̵�����11
#define	            RW_TLR12  654	//TLR12-�����ͼ̵�����12
#define	            RW_TLR13  655	//TLR13-�����ͼ̵�����13
#define	            RW_TLR14  656	//TLR14-�����ͼ̵�����14
#define	            RW_TLR15  657	//TLR15-�����ͼ̵�����15
#define	            RW_TLR16  658	//TLR16-�����ͼ̵�����16
#define	             RW_RLR1  659	//RLR1-������ռ̵�����1
#define	             RW_RLR2  660	//RLR2-������ռ̵�����2
#define	             RW_RLR3  661	//RLR3-������ռ̵�����3
#define	             RW_RLR4  662	//RLR4-������ռ̵�����4
#define	             RW_RLR5  663	//RLR5-������ռ̵�����5
#define	             RW_RLR6  664	//RLR6-������ռ̵�����6
#define	             RW_RLR7  665	//RLR7-������ռ̵�����7
#define	             RW_RLR8  666	//RLR8-������ռ̵�����8
#define	             RW_RLR9  667	//RLR9-������ռ̵�����9
#define	            RW_RLR10  668	//RLR10-������ռ̵�����10
#define	            RW_RLR11  669	//RLR11-������ռ̵�����11
#define	            RW_RLR12  670	//RLR12-������ռ̵�����12
#define	            RW_RLR13  671	//RLR13-������ռ̵�����13
#define	            RW_RLR14  672	//RLR14-������ռ̵�����14
#define	            RW_RLR15  673	//RLR15-������ռ̵�����15
#define	            RW_RLR16  674	//RLR16-������ռ̵�����16
#define	             RW_CNT1  675	//CNT1-�����̵�����1
#define	             RW_CNT2  676	//CNT2-�����̵�����2
#define	             RW_CNT3  677	//CNT3-�����̵�����3
#define	             RW_CNT4  678	//CNT4-�����̵�����4
#define	             RW_CNT5  679	//CNT5-�����̵�����5
#define	             RW_CNT6  680	//CNT6-�����̵�����6
#define	             RW_CNT7  681	//CNT7-�����̵�����7
#define	             RW_CNT8  682	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			683		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			95	

//�м�̵�����
#define	MID_RELAY_WORD_START			97	
#define	MID_RELAY_WORD_NUM			92	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			189	
#define	KEEP_RELAY_WORD_NUM			34	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			223	
#define	TIME_RELAY_WORD_NUM			34	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			257	
#define	TRIP_RELAY_WORD_NUM			2	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			259	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			291	
#define	INPUT_RELAY_WORD_NUM			61	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			352	
#define	OUTPUT_RELAY_WORD_NUM			21	

//ѹ��̵���
#define	SW_RELAY_WORD_START			373	
#define	SW_RELAY_WORD_NUM			9	

//���ü̵���
#define	CFG_RELAY_WORD_START			382	
#define	CFG_RELAY_WORD_NUM			52	

//����̵���
#define	PULSE_RELAY_WORD_START			434	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			450	
#define	CTRLTEST_RELAY_WORD_NUM			13	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			463	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			479	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			503	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			519	
#define	RCP_RELAY_WORD_NUM			36	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			555	
#define	RCE_RELAY_WORD_NUM			36	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			591	
#define	RCV_RELAY_WORD_NUM			36	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			627	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			635	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			643	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			659	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			675	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			675	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			675	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			675	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			675	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			675	
#define	CNT_RELAY_WORD_NUM			8	


#endif