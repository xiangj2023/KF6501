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
#define	             RW_59U1    2	//59U1-U1��ѹ
#define	             RW_27U1    3	//27U1-U1��ѹ
#define	             RW_59U2    4	//59U2-U2��ѹ
#define	             RW_27U2    5	//27U2-U2��ѹ
#define	             RW_59U3    6	//59U3-U3��ѹ
#define	             RW_27U3    7	//27U3-U3��ѹ
#define	             RW_59U4    8	//59U4-U4��ѹ
#define	             RW_27U4    9	//27U4-U4��ѹ
#define	             RW_59U5   10	//59U5-U5��ѹ
#define	             RW_27U5   11	//27U5-U5��ѹ
#define	             RW_59U6   12	//59U6-U6��ѹ
#define	             RW_27U6   13	//27U6-U6��ѹ
#define	             RW_59U7   14	//59U7-U7��ѹ
#define	             RW_27U7   15	//27U7-U7��ѹ
#define	             RW_59U8   16	//59U8-U8��ѹ
#define	             RW_27U8   17	//27U8-U8��ѹ
#define	             RW_50I1   18	//50I1-I1����
#define	             RW_37I1   19	//37I1-I1����
#define	             RW_50I2   20	//50I2-I2����
#define	             RW_37I2   21	//37I2-I2����
#define	             RW_50I3   22	//50I3-I3����
#define	             RW_37I3   23	//37I3-I3����
#define	             RW_50I4   24	//50I4-I4����
#define	             RW_37I4   25	//37I4-I4����
#define	             RW_50I5   26	//50I5-I5����
#define	             RW_37I5   27	//37I5-I5����
#define	             RW_50I6   28	//50I6-I6����
#define	             RW_37I6   29	//37I6-I6����
#define	             RW_50I7   30	//50I7-I7����
#define	             RW_37I7   31	//37I7-I7����
#define	             RW_50I8   32	//50I8-I8����
#define	             RW_37I8   33	//37I8-I8����
#define	          RW_ET11ERR   34	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   35	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   36	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   37	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   38	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   39	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   40	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   41	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   42	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   43	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   44	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   45	//CAN1ERR-CAN1����״̬�̵�����
#define	          RW_CAN2ERR   46	//CAN2ERR-CAN2����״̬�̵�����
#define	         RW_NULL1ERR   47	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   48	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   49	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK   50	//MAINTLINK-ά��״̬�̵�����
#define	              RW_REC   51	//REC-¼���̵�����
#define	               RW_M1   52	//M1-�м�̵���1
#define	               RW_M2   53	//M2-�м�̵���2
#define	               RW_M3   54	//M3-�м�̵���3
#define	               RW_M4   55	//M4-�м�̵���4
#define	               RW_M5   56	//M5-�м�̵���5
#define	               RW_M6   57	//M6-�м�̵���6
#define	               RW_M7   58	//M7-�м�̵���7
#define	               RW_M8   59	//M8-�м�̵���8
#define	               RW_M9   60	//M9-�м�̵���9
#define	              RW_M10   61	//M10-�м�̵���10
#define	              RW_M11   62	//M11-�м�̵���11
#define	              RW_M12   63	//M12-�м�̵���12
#define	              RW_M13   64	//M13-�м�̵���13
#define	              RW_M14   65	//M14-�м�̵���14
#define	              RW_M15   66	//M15-�м�̵���15
#define	              RW_M16   67	//M16-�м�̵���16
#define	              RW_M17   68	//M17-�м�̵���17
#define	              RW_M18   69	//M18-�м�̵���18
#define	              RW_M19   70	//M19-�м�̵���19
#define	              RW_M20   71	//M20-�м�̵���20
#define	              RW_M21   72	//M21-�м�̵���21
#define	              RW_M22   73	//M22-�м�̵���22
#define	              RW_M23   74	//M23-�м�̵���23
#define	              RW_M24   75	//M24-�м�̵���24
#define	              RW_M25   76	//M25-�м�̵���25
#define	              RW_M26   77	//M26-�м�̵���26
#define	              RW_M27   78	//M27-�м�̵���27
#define	              RW_M28   79	//M28-�м�̵���28
#define	              RW_M29   80	//M29-�м�̵���29
#define	              RW_M30   81	//M30-�м�̵���30
#define	              RW_M31   82	//M31-�м�̵���31
#define	              RW_M32   83	//M32-�м�̵���32
#define	              RW_M33   84	//M33-�м�̵���33
#define	              RW_M34   85	//M34-�м�̵���34
#define	              RW_M35   86	//M35-�м�̵���35
#define	              RW_M36   87	//M36-�м�̵���36
#define	              RW_M37   88	//M37-�м�̵���37
#define	              RW_M38   89	//M38-�м�̵���38
#define	              RW_M39   90	//M39-�м�̵���39
#define	              RW_M40   91	//M40-�м�̵���40
#define	              RW_M41   92	//M41-�м�̵���41
#define	              RW_M42   93	//M42-�м�̵���42
#define	              RW_M43   94	//M43-�м�̵���43
#define	              RW_M44   95	//M44-�м�̵���44
#define	              RW_M45   96	//M45-�м�̵���45
#define	              RW_M46   97	//M46-�м�̵���46
#define	              RW_M47   98	//M47-�м�̵���47
#define	              RW_M48   99	//M48-�м�̵���48
#define	              RW_M49  100	//M49-�м�̵���49
#define	              RW_M50  101	//M50-�м�̵���50
#define	              RW_M51  102	//M51-�м�̵���51
#define	              RW_M52  103	//M52-�м�̵���52
#define	              RW_M53  104	//M53-�м�̵���53
#define	              RW_M54  105	//M54-�м�̵���54
#define	              RW_M55  106	//M55-�м�̵���55
#define	              RW_M56  107	//M56-�м�̵���56
#define	              RW_M57  108	//M57-�м�̵���57
#define	              RW_M58  109	//M58-�м�̵���58
#define	              RW_M59  110	//M59-�м�̵���59
#define	              RW_M60  111	//M60-�м�̵���60
#define	              RW_M61  112	//M61-�м�̵���61
#define	              RW_M62  113	//M62-�м�̵���62
#define	              RW_M63  114	//M63-�м�̵���63
#define	              RW_M64  115	//M64-�м�̵���64
#define	              RW_M65  116	//M65-�м�̵���65
#define	              RW_M66  117	//M66-�м�̵���66
#define	              RW_M67  118	//M67-�м�̵���67
#define	              RW_M68  119	//M68-�м�̵���68
#define	              RW_M69  120	//M69-�м�̵���69
#define	              RW_M70  121	//M70-�м�̵���70
#define	              RW_M71  122	//M71-�м�̵���71
#define	              RW_M72  123	//M72-�м�̵���72
#define	              RW_M73  124	//M73-�м�̵���73
#define	              RW_M74  125	//M74-�м�̵���74
#define	              RW_M75  126	//M75-�м�̵���75
#define	              RW_M76  127	//M76-�м�̵���76
#define	              RW_M77  128	//M77-�м�̵���77
#define	              RW_M78  129	//M78-�м�̵���78
#define	              RW_M79  130	//M79-�м�̵���79
#define	              RW_M80  131	//M80-�м�̵���80
#define	              RW_M81  132	//M81-�м�̵���81
#define	              RW_M82  133	//M82-�м�̵���82
#define	              RW_M83  134	//M83-�м�̵���83
#define	              RW_M84  135	//M84-�м�̵���84
#define	              RW_M85  136	//M85-�м�̵���85
#define	              RW_M86  137	//M86-�м�̵���86
#define	              RW_M87  138	//M87-�м�̵���87
#define	              RW_M88  139	//M88-�м�̵���88
#define	              RW_M89  140	//M89-�м�̵���89
#define	              RW_M90  141	//M90-�м�̵���90
#define	              RW_M91  142	//M91-�м�̵���91
#define	              RW_M92  143	//M92-�м�̵���92
#define	              RW_M93  144	//M93-�м�̵���93
#define	              RW_M94  145	//M94-�м�̵���94
#define	              RW_M95  146	//M95-�м�̵���95
#define	              RW_M96  147	//M96-�м�̵���96
#define	              RW_M97  148	//M97-�м�̵���97
#define	              RW_M98  149	//M98-�м�̵���98
#define	              RW_M99  150	//M99-�м�̵���99
#define	             RW_M100  151	//M100-�м�̵���100
#define	             RW_M101  152	//M101-�м�̵���101
#define	             RW_M102  153	//M102-�м�̵���102
#define	             RW_M103  154	//M103-�м�̵���103
#define	             RW_M104  155	//M104-�м�̵���104
#define	             RW_M105  156	//M105-�м�̵���105
#define	             RW_M106  157	//M106-�м�̵���106
#define	             RW_M107  158	//M107-�м�̵���107
#define	             RW_M108  159	//M108-�м�̵���108
#define	             RW_M109  160	//M109-�м�̵���109
#define	             RW_M110  161	//M110-�м�̵���110
#define	             RW_M111  162	//M111-�м�̵���111
#define	             RW_M112  163	//M112-�м�̵���112
#define	             RW_M113  164	//M113-�м�̵���113
#define	             RW_M114  165	//M114-�м�̵���114
#define	             RW_M115  166	//M115-�м�̵���115
#define	             RW_M116  167	//M116-�м�̵���116
#define	             RW_M117  168	//M117-�м�̵���117
#define	             RW_M118  169	//M118-�м�̵���118
#define	             RW_M119  170	//M119-�м�̵���119
#define	             RW_M120  171	//M120-�м�̵���120
#define	             RW_M121  172	//M121-�м�̵���121
#define	             RW_M122  173	//M122-�м�̵���122
#define	             RW_M123  174	//M123-�м�̵���123
#define	             RW_M124  175	//M124-�м�̵���124
#define	             RW_M125  176	//M125-�м�̵���125
#define	             RW_M126  177	//M126-�м�̵���126
#define	             RW_M127  178	//M127-�м�̵���127
#define	             RW_M128  179	//M128-�м�̵���128
#define	          RW_INRESET  180	//INRESET-ң�Ÿ���
#define	            RW_INBZT  181	//INBZT-ң��Ͷ�뱸��Ͷ
#define	            RW_74PT1  182	//74PT1-PT1�澯�̵�����
#define	            RW_74PT2  183	//74PT2-PT2�澯�̵�����
#define	            RW_74PT3  184	//74PT3-PT3�澯�̵�����
#define	            RW_74PT4  185	//74PT4-PT4�澯�̵�����
#define	               RW_H1  186	//H1-�Ա����м�̵�����1
#define	               RW_H2  187	//H2-�Ա����м�̵�����2
#define	               RW_H3  188	//H3-�Ա����м�̵�����3
#define	               RW_H4  189	//H4-�Ա����м�̵�����4
#define	               RW_H5  190	//H5-�Ա����м�̵�����5
#define	               RW_H6  191	//H6-�Ա����м�̵�����6
#define	               RW_H7  192	//H7-�Ա����м�̵�����7
#define	               RW_H8  193	//H8-�Ա����м�̵�����8
#define	               RW_H9  194	//H9-�Ա����м�̵�����9
#define	              RW_H10  195	//H10-�Ա����м�̵�����10
#define	              RW_H11  196	//H11-�Ա����м�̵�����11
#define	              RW_H12  197	//H12-�Ա����м�̵�����12
#define	              RW_H13  198	//H13-�Ա����м�̵�����13
#define	              RW_H14  199	//H14-�Ա����м�̵�����14
#define	              RW_H15  200	//H15-�Ա����м�̵�����15
#define	              RW_H16  201	//H16-�Ա����м�̵�����16
#define	              RW_H17  202	//H17-�Ա����м�̵�����17
#define	              RW_H18  203	//H18-�Ա����м�̵�����18
#define	              RW_H19  204	//H19-�Ա����м�̵�����19
#define	              RW_H20  205	//H20-�Ա����м�̵�����20
#define	              RW_H21  206	//H21-�Ա����м�̵�����21
#define	              RW_H22  207	//H22-�Ա����м�̵�����22
#define	              RW_H23  208	//H23-�Ա����м�̵�����23
#define	              RW_H24  209	//H24-�Ա����м�̵�����24
#define	              RW_H25  210	//H25-�Ա����м�̵�����25
#define	              RW_H26  211	//H26-�Ա����м�̵�����26
#define	              RW_H27  212	//H27-�Ա����м�̵�����27
#define	              RW_H28  213	//H28-�Ա����м�̵�����28
#define	              RW_H29  214	//H29-�Ա����м�̵�����29
#define	              RW_H30  215	//H30-�Ա����м�̵�����30
#define	              RW_H31  216	//H31-�Ա����м�̵�����31
#define	              RW_H32  217	//H32-�Ա����м�̵�����32
#define	              RW_H33  218	//H33-�Ա����м�̵�����33
#define	              RW_H34  219	//H34-�Ա����м�̵�����34
#define	              RW_H35  220	//H35-�Ա����м�̵�����35
#define	              RW_H36  221	//H36-�Ա����м�̵�����36
#define	              RW_H37  222	//H37-�Ա����м�̵�����37
#define	              RW_H38  223	//H38-�Ա����м�̵�����38
#define	              RW_H39  224	//H39-�Ա����м�̵�����39
#define	              RW_H40  225	//H40-�Ա����м�̵�����40
#define	              RW_H41  226	//H41-�Ա����м�̵�����41
#define	              RW_H42  227	//H42-�Ա����м�̵�����42
#define	              RW_H43  228	//H43-�Ա����м�̵�����43
#define	              RW_H44  229	//H44-�Ա����м�̵�����44
#define	              RW_H45  230	//H45-�Ա����м�̵�����45
#define	              RW_H46  231	//H46-�Ա����м�̵�����46
#define	              RW_H47  232	//H47-�Ա����м�̵�����47
#define	              RW_H48  233	//H48-�Ա����м�̵�����48
#define	              RW_H49  234	//H49-�Ա����м�̵�����49
#define	              RW_H50  235	//H50-�Ա����м�̵�����50
#define	              RW_H51  236	//H51-�Ա����м�̵�����51
#define	              RW_H52  237	//H52-�Ա����м�̵�����52
#define	              RW_H53  238	//H53-�Ա����м�̵�����53
#define	              RW_H54  239	//H54-�Ա����м�̵�����54
#define	              RW_H55  240	//H55-�Ա����м�̵�����55
#define	              RW_H56  241	//H56-�Ա����м�̵�����56
#define	              RW_H57  242	//H57-�Ա����м�̵�����57
#define	              RW_H58  243	//H58-�Ա����м�̵�����58
#define	              RW_H59  244	//H59-�Ա����м�̵�����59
#define	              RW_H60  245	//H60-�Ա����м�̵�����60
#define	              RW_H61  246	//H61-�Ա����м�̵�����61
#define	              RW_H62  247	//H62-�Ա����м�̵�����62
#define	              RW_H63  248	//H63-�Ա����м�̵�����63
#define	              RW_H64  249	//H64-�Ա����м�̵�����64
#define	             RW_LOCK  250	//LOCK-�����̵�����
#define	             RW_CHR1  251	//CHR1-��緽ʽһ�̵�����
#define	             RW_CHR2  252	//CHR2-��緽ʽ���̵�����
#define	             RW_CHR3  253	//CHR3-��緽ʽ���̵�����
#define	             RW_CHR4  254	//CHR4-��緽ʽ�ļ̵�����
#define	             RW_CHR5  255	//CHR5-��緽ʽ��̵�����
#define	             RW_CHR6  256	//CHR6-��緽ʽ���̵�����
#define	              RW_PI1  257	//PI1-����Ͷ�����̵�����1
#define	              RW_PI2  258	//PI2-����Ͷ�����̵�����2
#define	              RW_PI3  259	//PI3-����Ͷ�����̵�����3
#define	              RW_PI4  260	//PI4-����Ͷ�����̵�����4
#define	              RW_PI5  261	//PI5-����Ͷ�����̵�����5
#define	              RW_PI6  262	//PI6-����Ͷ�����̵�����6
#define	              RW_PI7  263	//PI7-����Ͷ�����̵�����7
#define	              RW_PI8  264	//PI8-����Ͷ�����̵�����8
#define	              RW_PI9  265	//PI9-����Ͷ�����̵�����9
#define	             RW_PI10  266	//PI10-����Ͷ�����̵�����10
#define	             RW_PI11  267	//PI11-����Ͷ�����̵�����11
#define	             RW_PI12  268	//PI12-����Ͷ�����̵�����12
#define	            RW_ZB1GZ  269	//ZB1GZ-����1����
#define	            RW_ZB2GZ  270	//ZB2GZ-����2����
#define	               RW_T1  271	//T1-T1
#define	               RW_T2  272	//T2-T2
#define	               RW_T3  273	//T3-T3
#define	               RW_T4  274	//T4-T4
#define	               RW_T5  275	//T5-T5
#define	               RW_T6  276	//T6-T6
#define	               RW_T7  277	//T7-T7
#define	               RW_T8  278	//T8-T8
#define	               RW_T9  279	//T9-T9
#define	              RW_T10  280	//T10-T10
#define	              RW_T11  281	//T11-T11
#define	              RW_T12  282	//T12-T12
#define	              RW_T13  283	//T13-T13
#define	              RW_T14  284	//T14-T14
#define	              RW_T15  285	//T15-T15
#define	              RW_T16  286	//T16-T16
#define	              RW_T17  287	//T17-T17
#define	              RW_T18  288	//T18-T18
#define	              RW_T19  289	//T19-T19
#define	              RW_T20  290	//T20-T20
#define	              RW_T21  291	//T21-T21
#define	              RW_T22  292	//T22-T22
#define	              RW_T23  293	//T23-T23
#define	              RW_T24  294	//T24-T24
#define	              RW_T25  295	//T25-T25
#define	              RW_T26  296	//T26-T26
#define	              RW_T27  297	//T27-T27
#define	              RW_T28  298	//T28-T28
#define	              RW_T29  299	//T29-T29
#define	              RW_T30  300	//T30-T30
#define	              RW_T31  301	//T31-T31
#define	              RW_T32  302	//T32-T32
#define	             RW_TCB1  303	//TCB1-TCB1
#define	             RW_TCB2  304	//TCB2-TCB2
#define	             RW_TCB3  305	//TCB3-TCB3
#define	             RW_TCB4  306	//TCB4-TCB4
#define	             RW_TCB5  307	//TCB5-TCB5
#define	             RW_TCB6  308	//TCB6-TCB6
#define	             RW_TCB7  309	//TCB7-TCB7
#define	             RW_TCB8  310	//TCB8-TCB8
#define	             RW_TCB9  311	//TCB9-TCB9
#define	            RW_TCB10  312	//TCB10-TCB10
#define	            RW_TCB11  313	//TCB11-TCB11
#define	            RW_TCB12  314	//TCB12-TCB12
#define	            RW_TCB13  315	//TCB13-TCB13
#define	            RW_TCB14  316	//TCB14-TCB14
#define	            RW_TCB15  317	//TCB15-TCB15
#define	            RW_TCB16  318	//TCB16-TCB16
#define	            RW_TCB17  319	//TCB17-TCB17
#define	            RW_TCB18  320	//TCB18-TCB18
#define	            RW_TCB19  321	//TCB19-TCB19
#define	            RW_TCB20  322	//TCB20-TCB20
#define	            RW_TCB21  323	//TCB21-TCB21
#define	            RW_TCB22  324	//TCB22-TCB22
#define	            RW_TCB23  325	//TCB23-TCB23
#define	            RW_TCB24  326	//TCB24-TCB24
#define	            RW_TCB25  327	//TCB25-TCB25
#define	            RW_TCB26  328	//TCB26-TCB26
#define	            RW_TCB27  329	//TCB27-TCB27
#define	            RW_TCB28  330	//TCB28-TCB28
#define	            RW_TCB29  331	//TCB29-TCB29
#define	            RW_TCB30  332	//TCB30-TCB30
#define	            RW_TCB31  333	//TCB31-TCB31
#define	            RW_TCB32  334	//TCB32-TCB32
#define	             RW_TSW1  335	//TSW1-TSW1
#define	             RW_TSW2  336	//TSW2-TSW2
#define	             RW_TSW3  337	//TSW3-TSW3
#define	             RW_TSW4  338	//TSW4-TSW4
#define	             RW_TSW5  339	//TSW5-TSW5
#define	             RW_TSW6  340	//TSW6-TSW6
#define	             RW_TSW7  341	//TSW7-TSW7
#define	             RW_TSW8  342	//TSW8-TSW8
#define	             RW_TSW9  343	//TSW9-TSW9
#define	            RW_TSW10  344	//TSW10-TSW10
#define	            RW_TSW11  345	//TSW11-TSW11
#define	            RW_TSW12  346	//TSW12-TSW12
#define	            RW_TSW13  347	//TSW13-TSW13
#define	            RW_TSW14  348	//TSW14-TSW14
#define	            RW_TSW15  349	//TSW15-TSW15
#define	            RW_TSW16  350	//TSW16-TSW16
#define	            RW_TSW17  351	//TSW17-TSW17
#define	            RW_TSW18  352	//TSW18-TSW18
#define	            RW_TSW19  353	//TSW19-TSW19
#define	            RW_TSW20  354	//TSW20-TSW20
#define	            RW_TSW21  355	//TSW21-TSW21
#define	            RW_TSW22  356	//TSW22-TSW22
#define	            RW_TSW23  357	//TSW23-TSW23
#define	            RW_TSW24  358	//TSW24-TSW24
#define	            RW_TSW25  359	//TSW25-TSW25
#define	            RW_TSW26  360	//TSW26-TSW26
#define	            RW_TSW27  361	//TSW27-TSW27
#define	            RW_TSW28  362	//TSW28-TSW28
#define	            RW_TSW29  363	//TSW29-TSW29
#define	            RW_TSW30  364	//TSW30-TSW30
#define	            RW_TSW31  365	//TSW31-TSW31
#define	            RW_TSW32  366	//TSW32-TSW32
#define	           RW_TREBZT  367	//TREBZT-����Ͷ����Ͷ��ʱ��̵�����
#define	            RW_TCHR1  368	//TCHR1-��緽ʽ1ʱ��̵�����
#define	            RW_TCHR2  369	//TCHR2-��緽ʽ2ʱ��̵�����
#define	            RW_TCHR3  370	//TCHR3-��緽ʽ3ʱ��̵�����
#define	            RW_TCHR4  371	//TCHR4-��緽ʽ4ʱ��̵�����
#define	            RW_TCHR5  372	//TCHR5-��緽ʽ5ʱ��̵�����
#define	            RW_TCHR6  373	//TCHR6-��緽ʽ6ʱ��̵�����
#define	              RW_TR1  374	//TR1-��բ�̵���1
#define	              RW_TR2  375	//TR2-��բ�̵���2
#define	              RW_VR1  376	//VR1-���Լ̵���
#define	              RW_VR2  377	//VR2-����̵���2
#define	              RW_VR3  378	//VR3-����̵���3
#define	              RW_VR4  379	//VR4-����̵���4
#define	              RW_VR5  380	//VR5-����̵���5
#define	              RW_VR6  381	//VR6-����̵���6
#define	              RW_VR7  382	//VR7-����̵���7
#define	              RW_VR8  383	//VR8-����̵���8
#define	              RW_VR9  384	//VR9-����̵���9
#define	             RW_VR10  385	//VR10-����̵���10
#define	             RW_VR11  386	//VR11-����̵���11
#define	             RW_VR12  387	//VR12-����̵���12
#define	             RW_VR13  388	//VR13-����̵���13
#define	             RW_VR14  389	//VR14-����̵���14
#define	             RW_VR15  390	//VR15-����̵���15
#define	             RW_VR16  391	//VR16-����̵���16
#define	             RW_VR17  392	//VR17-����̵���17
#define	             RW_VR18  393	//VR18-����̵���18
#define	             RW_VR19  394	//VR19-����̵���19
#define	             RW_VR20  395	//VR20-����̵���20
#define	             RW_VR21  396	//VR21-����̵���21
#define	             RW_VR22  397	//VR22-����̵���22
#define	             RW_VR23  398	//VR23-����̵���23
#define	             RW_VR24  399	//VR24-����̵���24
#define	             RW_VR25  400	//VR25-����̵���25
#define	             RW_VR26  401	//VR26-����̵���26
#define	             RW_VR27  402	//VR27-����̵���27
#define	             RW_VR28  403	//VR28-����̵���28
#define	             RW_VR29  404	//VR29-����̵���29
#define	             RW_VR30  405	//VR30-����̵���30
#define	             RW_VR31  406	//VR31-����̵���31
#define	             RW_VR32  407	//VR32-����̵���32
#define	              RW_IN1  408	//IN1-����̵���1
#define	              RW_IN2  409	//IN2-����̵���2
#define	              RW_IN3  410	//IN3-����̵���3
#define	              RW_IN4  411	//IN4-����̵���4
#define	              RW_IN5  412	//IN5-����̵���5
#define	              RW_IN6  413	//IN6-����̵���6
#define	              RW_IN7  414	//IN7-����̵���7
#define	              RW_IN8  415	//IN8-����̵���8
#define	              RW_IN9  416	//IN9-����̵���9
#define	             RW_IN10  417	//IN10-����̵���10
#define	             RW_IN11  418	//IN11-����̵���11
#define	             RW_IN12  419	//IN12-����̵���12
#define	             RW_IN13  420	//IN13-����̵���13
#define	             RW_IN14  421	//IN14-����̵���14
#define	             RW_IN15  422	//IN15-����̵���15
#define	             RW_IN16  423	//IN16-����̵���16
#define	             RW_IN17  424	//IN17-����̵���17
#define	             RW_IN18  425	//IN18-����̵���18
#define	             RW_IN19  426	//IN19-����̵���19
#define	             RW_IN20  427	//IN20-����̵���20
#define	             RW_IN21  428	//IN21-����̵���21
#define	             RW_IN22  429	//IN22-����̵���22
#define	             RW_IN23  430	//IN23-����̵���23
#define	             RW_IN24  431	//IN24-����̵���24
#define	             RW_IN25  432	//IN25-����̵���25
#define	             RW_IN26  433	//IN26-����̵���26
#define	             RW_IN27  434	//IN27-����̵���27
#define	             RW_IN28  435	//IN28-����̵���28
#define	             RW_IN29  436	//IN29-����̵���29
#define	             RW_IN30  437	//IN30-����̵���30
#define	             RW_IN31  438	//IN31-����̵���31
#define	             RW_IN32  439	//IN32-����̵���32
#define	             RW_IN33  440	//IN33-����̵���33
#define	             RW_IN34  441	//IN34-����̵���34
#define	             RW_IN35  442	//IN35-����̵���35
#define	             RW_IN36  443	//IN36-����̵���36
#define	             RW_IN37  444	//IN37-����̵���37
#define	             RW_IN38  445	//IN38-����̵���38
#define	             RW_IN39  446	//IN39-����̵���39
#define	             RW_IN40  447	//IN40-����̵���40
#define	             RW_IN41  448	//IN41-����̵���41
#define	             RW_IN42  449	//IN42-����̵���42
#define	             RW_IN43  450	//IN43-����̵���43
#define	             RW_IN44  451	//IN44-����̵���44
#define	             RW_IN45  452	//IN45-����̵���45
#define	             RW_IN46  453	//IN46-����̵���46
#define	             RW_IN47  454	//IN47-����̵���47
#define	             RW_IN48  455	//IN48-����̵���48
#define	             RW_IN49  456	//IN49-����̵���49
#define	             RW_IN50  457	//IN50-����̵���50
#define	             RW_IN51  458	//IN51-����̵���51
#define	             RW_IN52  459	//IN52-����̵���52
#define	             RW_IN53  460	//IN53-����̵���53
#define	             RW_IN54  461	//IN54-����̵���54
#define	             RW_IN55  462	//IN55-����̵���55
#define	             RW_IN56  463	//IN56-����̵���56
#define	             RW_IN57  464	//IN57-����̵���57
#define	             RW_IN58  465	//IN58-����̵���58
#define	             RW_IN59  466	//IN59-����̵���59
#define	             RW_IN60  467	//IN60-����̵���60
#define	             RW_IN61  468	//IN61-����̵���61
#define	             RW_IN62  469	//IN62-����̵���62
#define	             RW_IN63  470	//IN63-����̵���63
#define	             RW_IN64  471	//IN64-����̵���64
#define	             RW_IN65  472	//IN65-����̵���65
#define	             RW_IN66  473	//IN66-����̵���66
#define	             RW_IN67  474	//IN67-����̵���67
#define	             RW_IN68  475	//IN68-����̵���68
#define	             RW_IN69  476	//IN69-�źŸ���
#define	             RW_IN70  477	//IN70-ң��1Ԥ��
#define	             RW_IN71  478	//IN71-ң��2Ԥ��
#define	             RW_IN72  479	//IN72-ң��3Ԥ��
#define	             RW_IN73  480	//IN73-ң��4Ԥ��
#define	             RW_IN74  481	//IN74-ң��5Ԥ��
#define	             RW_IN75  482	//IN75-ң��6Ԥ��
#define	             RW_IN76  483	//IN76-ң��7Ԥ��
#define	             RW_IN77  484	//IN77-ң��8Ԥ��
#define	             RW_IN78  485	//IN78-ң��9Ԥ��
#define	             RW_IN79  486	//IN79-ң��10Ԥ��
#define	             RW_IN80  487	//IN80-ң��11Ԥ��
#define	             RW_IN81  488	//IN81-ң��12Ԥ��
#define	             RW_IN82  489	//IN82-ң��13Ԥ��
#define	             RW_IN83  490	//IN83-ң��14Ԥ��
#define	           RW_INVIN1  491	//INVIN1-����̵���84
#define	           RW_INVIN2  492	//INVIN2-����̵���85
#define	           RW_INVIN3  493	//INVIN3-����̵���86
#define	           RW_INVIN4  494	//INVIN4-����̵���87
#define	           RW_INVIN5  495	//INVIN5-����̵���88
#define	           RW_INVIN6  496	//INVIN6-����̵���89
#define	           RW_INVIN7  497	//INVIN7-����̵���90
#define	           RW_INVIN8  498	//INVIN8-����̵���91
#define	           RW_INVIN9  499	//INVIN9-����̵���92
#define	          RW_INVIN10  500	//INVIN10-����̵���93
#define	          RW_INVIN11  501	//INVIN11-����̵���94
#define	          RW_INVIN12  502	//INVIN12-����̵���95
#define	          RW_INVIN13  503	//INVIN13-����̵���96
#define	          RW_INVIN14  504	//INVIN14-����̵���97
#define	          RW_INVIN15  505	//INVIN15-����̵���98
#define	          RW_INVIN16  506	//INVIN16-����̵���99
#define	             RW_OUT1  507	//OUT1-����1�̵�����
#define	             RW_OUT2  508	//OUT2-����2�̵�����
#define	             RW_OUT3  509	//OUT3-����3�̵�����
#define	             RW_OUT4  510	//OUT4-����4�̵�����
#define	             RW_OUT5  511	//OUT5-����5�̵�����
#define	             RW_OUT6  512	//OUT6-����6�̵�����
#define	             RW_OUT7  513	//OUT7-����7�̵�����
#define	             RW_OUT8  514	//OUT8-����8�̵�����
#define	             RW_OUT9  515	//OUT9-����9�̵�����
#define	            RW_OUT10  516	//OUT10-����10�̵�����
#define	            RW_OUT11  517	//OUT11-����11�̵�����
#define	            RW_OUT12  518	//OUT12-����12�̵�����
#define	            RW_OUT13  519	//OUT13-����13�̵�����
#define	            RW_OUT14  520	//OUT14-����14�̵�����
#define	            RW_OUT15  521	//OUT15-����15�̵�����
#define	            RW_OUT16  522	//OUT16-����16�̵�����
#define	            RW_OUT17  523	//OUT17-����17�̵�����
#define	            RW_OUT18  524	//OUT18-����18�̵�����
#define	            RW_OUT19  525	//OUT19-����19�̵�����
#define	            RW_OUT20  526	//OUT20-����20�̵�����
#define	            RW_OUT21  527	//OUT21-����21�̵�����
#define	            RW_OUT22  528	//OUT22-����22�̵�����
#define	            RW_OUT23  529	//OUT23-����23�̵�����
#define	            RW_OUT24  530	//OUT24-����24�̵�����
#define	            RW_OUT25  531	//OUT25-����25�̵�����
#define	            RW_OUT26  532	//OUT26-����26�̵�����
#define	            RW_OUT27  533	//OUT27-����27�̵�����
#define	            RW_OUT28  534	//OUT28-����28�̵�����
#define	            RW_OUT29  535	//OUT29-����29�̵�����
#define	            RW_OUT30  536	//OUT30-����30�̵�����
#define	            RW_OUT31  537	//OUT31-����31�̵�����
#define	            RW_OUT32  538	//OUT32-����32�̵�����
#define	            RW_OUT33  539	//OUT33-����33�̵�����
#define	            RW_OUT34  540	//OUT34-����34�̵�����
#define	            RW_OUT35  541	//OUT35-����35�̵�����
#define	            RW_OUT36  542	//OUT36-����36�̵�����
#define	            RW_OUT37  543	//OUT37-����37�̵�����
#define	            RW_OUT38  544	//OUT38-����38�̵�����
#define	            RW_OUT39  545	//OUT39-����39�̵�����
#define	            RW_OUT40  546	//OUT40-����40�̵�����
#define	            RW_OUT41  547	//OUT41-����41�̵�����
#define	            RW_OUT42  548	//OUT42-����42�̵�����
#define	            RW_OUT43  549	//OUT43-����43�̵�����
#define	            RW_OUT44  550	//OUT44-����44�̵�����
#define	            RW_OUT45  551	//OUT45-����45�̵�����
#define	            RW_OUT46  552	//OUT46-����46�̵�����
#define	            RW_OUT47  553	//OUT47-����47�̵�����
#define	            RW_OUT48  554	//OUT48-����48�̵�����
#define	            RW_SLBZT  555	//SLBZT-����Ͷ��ѹ��
#define	          RW_SLZBBZT  556	//SLZBBZT-���䱸��Ͷ��ѹ��
#define	          RW_SLSYBZT  557	//SLSYBZT-ʧѹ����Ͷ��ѹ��
#define	          RW_SLMAINT  558	//SLMAINT-����ѹ��
#define	           RW_SLBAK1  559	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  560	//SLBAK2-����ѹ��2
#define	           RW_SLBAK3  561	//SLBAK3-����ѹ��3
#define	           RW_SLBAK4  562	//SLBAK4-����ѹ��4
#define	             RW_FBZT  563	//FBZT-����Ͷ�������ü̵�����
#define	           RW_FREBZT  564	//FREBZT-����Ͷ����Ͷ�빦�����ü̵�����
#define	           RW_FZBBZT  565	//FZBBZT-���䱸��Ͷ���ü̵�����
#define	           RW_FSYBZT  566	//FSYBZT-ʧѹ����Ͷ���ü̵�����
#define	            RW_FZLYX  567	//FZLYX-ֱ���������ü̵�����
#define	           RW_F74PT1  568	//F74PT1-PT1���߼�����ü̵�����
#define	           RW_F74PT2  569	//F74PT2-PT2���߼�����ü̵�����
#define	           RW_F74PT3  570	//F74PT3-PT3���߼�����ü̵�����
#define	           RW_F74PT4  571	//F74PT4-PT4���߼�����ü̵�����
#define	            RW_FBAK1  572	//FBAK1-��������1
#define	            RW_FBAK2  573	//FBAK2-��������2
#define	            RW_FBAK3  574	//FBAK3-��������3
#define	            RW_FBAK4  575	//FBAK4-��������4
#define	              RW_PW1  576	//PW1-����̵���1
#define	              RW_PW2  577	//PW2-����̵���2
#define	              RW_PW3  578	//PW3-����̵���3
#define	              RW_PW4  579	//PW4-����̵���4
#define	              RW_PW5  580	//PW5-����̵���5
#define	              RW_PW6  581	//PW6-����̵���6
#define	              RW_PW7  582	//PW7-����̵���7
#define	              RW_PW8  583	//PW8-����̵���8
#define	              RW_PW9  584	//PW9-����̵���9
#define	             RW_PW10  585	//PW10-����̵���10
#define	             RW_PW11  586	//PW11-����̵���11
#define	             RW_PW12  587	//PW12-����̵���12
#define	             RW_PW13  588	//PW13-����̵���13
#define	             RW_PW14  589	//PW14-����̵���14
#define	             RW_PW15  590	//PW15-����̵���15
#define	             RW_PW16  591	//PW16-����̵���16
#define	              RW_TO1  592	//TO1-�����̵���1
#define	              RW_TO2  593	//TO2-�����̵���2
#define	              RW_TO3  594	//TO3-�����̵���3
#define	              RW_TO4  595	//TO4-�����̵���4
#define	              RW_TO5  596	//TO5-�����̵���5
#define	              RW_TO6  597	//TO6-�����̵���6
#define	              RW_TO7  598	//TO7-�����̵���7
#define	              RW_TO8  599	//TO8-�����̵���8
#define	              RW_TO9  600	//TO9-�����̵���9
#define	             RW_TO10  601	//TO10-�����̵���10
#define	             RW_TO11  602	//TO11-�����̵���11
#define	             RW_TO12  603	//TO12-�����̵���12
#define	             RW_TO13  604	//TO13-�����̵���13
#define	             RW_TO14  605	//TO14-�����̵���14
#define	             RW_TO15  606	//TO15-�����̵���15
#define	             RW_TO16  607	//TO16-�����̵���16
#define	             RW_TO17  608	//TO17-�����̵���17
#define	             RW_TO18  609	//TO18-�����̵���18
#define	             RW_TO19  610	//TO19-�����̵���19
#define	             RW_TO20  611	//TO20-�����̵���20
#define	             RW_TO21  612	//TO21-�����̵���21
#define	             RW_TO22  613	//TO22-�����̵���22
#define	             RW_TO23  614	//TO23-�����̵���23
#define	             RW_TO24  615	//TO24-�����̵���24
#define	             RW_TO25  616	//TO25-�����̵���25
#define	             RW_TO26  617	//TO26-�����̵���26
#define	             RW_TO27  618	//TO27-�����̵���27
#define	             RW_TO28  619	//TO28-�����̵���28
#define	             RW_TO29  620	//TO29-�����̵���29
#define	             RW_TO30  621	//TO30-�����̵���30
#define	             RW_TO31  622	//TO31-�����̵���31
#define	             RW_TO32  623	//TO32-�����̵���32
#define	             RW_SIG1  624	//SIG1-�źż̵���1
#define	             RW_SIG2  625	//SIG2-�źż̵���2
#define	             RW_SIG3  626	//SIG3-�źż̵���3
#define	             RW_SIG4  627	//SIG4-�źż̵���4
#define	             RW_SIG5  628	//SIG5-�źż̵���5
#define	             RW_SIG6  629	//SIG6-�źż̵���6
#define	             RW_SIG7  630	//SIG7-�źż̵���7
#define	             RW_SIG8  631	//SIG8-�źż̵���8
#define	             RW_SIG9  632	//SIG9-�źż̵���9
#define	            RW_SIG10  633	//SIG10-�źż̵���10
#define	            RW_SIG11  634	//SIG11-�źż̵���11
#define	            RW_SIG12  635	//SIG12-�źż̵���12
#define	            RW_SIG13  636	//SIG13-�źż̵���13
#define	            RW_SIG14  637	//SIG14-�źż̵���14
#define	            RW_SIG15  638	//SIG15-�źż̵���15
#define	            RW_SIG16  639	//SIG16-�źż̵���16
#define	            RW_LED1G  640	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  641	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  642	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  643	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  644	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  645	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  646	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  647	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  648	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  649	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  650	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  651	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  652	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  653	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  654	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  655	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  656	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  657	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  658	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  659	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  660	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  661	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  662	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  663	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  664	//VIN1-����̵���84
#define	             RW_VIN2  665	//VIN2-����̵���85
#define	             RW_VIN3  666	//VIN3-����̵���86
#define	             RW_VIN4  667	//VIN4-����̵���87
#define	             RW_VIN5  668	//VIN5-����̵���88
#define	             RW_VIN6  669	//VIN6-����̵���89
#define	             RW_VIN7  670	//VIN7-����̵���90
#define	             RW_VIN8  671	//VIN8-����̵���91
#define	             RW_VIN9  672	//VIN9-����̵���92
#define	            RW_VIN10  673	//VIN10-����̵���93
#define	            RW_VIN11  674	//VIN11-����̵���94
#define	            RW_VIN12  675	//VIN12-����̵���95
#define	            RW_VIN13  676	//VIN13-����̵���96
#define	            RW_VIN14  677	//VIN14-����̵���97
#define	            RW_VIN15  678	//VIN15-����̵���98
#define	            RW_VIN16  679	//VIN16-����̵���99
#define	             RW_RCP1  680	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  681	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  682	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  683	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  684	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  685	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  686	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  687	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  688	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  689	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  690	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  691	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  692	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  693	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  694	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  695	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  696	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  697	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  698	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  699	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  700	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  701	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  702	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  703	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  704	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  705	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  706	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  707	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  708	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  709	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  710	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  711	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  712	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  713	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  714	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  715	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  716	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  717	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  718	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  719	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  720	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  721	//RCP42-ң��Ԥ�ü̵���42
#define	            RW_RCP43  722	//RCP43-ң��Ԥ�ü̵���43
#define	            RW_RCP44  723	//RCP44-ң��Ԥ�ü̵���44
#define	            RW_RCP45  724	//RCP45-ң��Ԥ�ü̵���45
#define	            RW_RCP46  725	//RCP46-ң��Ԥ�ü̵���46
#define	            RW_RCP47  726	//RCP47-ң��Ԥ�ü̵���47
#define	            RW_RCP48  727	//RCP48-ң��Ԥ�ü̵���48
#define	            RW_RCP49  728	//RCP49-ң��Ԥ�ü̵���49
#define	            RW_RCP50  729	//RCP50-ң��Ԥ�ü̵���50
#define	            RW_RCP51  730	//RCP51-ң��Ԥ�ü̵���51
#define	            RW_RCP52  731	//RCP52-ң��Ԥ�ü̵���52
#define	            RW_RCP53  732	//RCP53-ң��Ԥ�ü̵���53
#define	            RW_RCP54  733	//RCP54-ң��Ԥ�ü̵���54
#define	            RW_RCP55  734	//RCP55-ң��Ԥ�ü̵���55
#define	            RW_RCP56  735	//RCP56-ң��Ԥ�ü̵���56
#define	            RW_RCP57  736	//RCP57-ң��Ԥ�ü̵���57
#define	            RW_RCP58  737	//RCP58-ң��Ԥ�ü̵���58
#define	             RW_RCE1  738	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  739	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  740	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  741	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  742	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  743	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  744	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  745	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  746	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  747	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  748	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  749	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  750	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  751	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  752	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  753	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  754	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  755	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  756	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  757	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  758	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  759	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  760	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  761	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  762	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  763	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  764	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  765	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  766	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  767	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  768	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  769	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  770	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  771	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  772	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  773	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  774	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  775	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  776	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  777	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  778	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  779	//RCE42-ң��ִ�м̵���42
#define	            RW_RCE43  780	//RCE43-ң��ִ�м̵���43
#define	            RW_RCE44  781	//RCE44-ң��ִ�м̵���44
#define	            RW_RCE45  782	//RCE45-ң��ִ�м̵���45
#define	            RW_RCE46  783	//RCE46-ң��ִ�м̵���46
#define	            RW_RCE47  784	//RCE47-ң��ִ�м̵���47
#define	            RW_RCE48  785	//RCE48-ң��ִ�м̵���48
#define	            RW_RCE49  786	//RCE49-ң��ִ�м̵���49
#define	            RW_RCE50  787	//RCE50-ң��ִ�м̵���50
#define	            RW_RCE51  788	//RCE51-ң��ִ�м̵���51
#define	            RW_RCE52  789	//RCE52-ң��ִ�м̵���52
#define	            RW_RCE53  790	//RCE53-ң��ִ�м̵���53
#define	            RW_RCE54  791	//RCE54-ң��ִ�м̵���54
#define	            RW_RCE55  792	//RCE55-ң��ִ�м̵���55
#define	            RW_RCE56  793	//RCE56-ң��ִ�м̵���56
#define	            RW_RCE57  794	//RCE57-ң��ִ�м̵���57
#define	            RW_RCE58  795	//RCE58-ң��ִ�м̵���58
#define	             RW_RCV1  796	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  797	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  798	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  799	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  800	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  801	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  802	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  803	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  804	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  805	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  806	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  807	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  808	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  809	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  810	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  811	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  812	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  813	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  814	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  815	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  816	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  817	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  818	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  819	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  820	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  821	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  822	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  823	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  824	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  825	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  826	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  827	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  828	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  829	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  830	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  831	//RCV36-ң�ط�У�̵���36
#define	            RW_RCV37  832	//RCV37-ң�ط�У�̵���37
#define	            RW_RCV38  833	//RCV38-ң�ط�У�̵���38
#define	            RW_RCV39  834	//RCV39-ң�ط�У�̵���39
#define	            RW_RCV40  835	//RCV40-ң�ط�У�̵���40
#define	            RW_RCV41  836	//RCV41-ң�ط�У�̵���41
#define	            RW_RCV42  837	//RCV42-ң�ط�У�̵���42
#define	            RW_RCV43  838	//RCV43-ң�ط�У�̵���43
#define	            RW_RCV44  839	//RCV44-ң�ط�У�̵���44
#define	            RW_RCV45  840	//RCV45-ң�ط�У�̵���45
#define	            RW_RCV46  841	//RCV46-ң�ط�У�̵���46
#define	            RW_RCV47  842	//RCV47-ң�ط�У�̵���47
#define	            RW_RCV48  843	//RCV48-ң�ط�У�̵���48
#define	            RW_RCV49  844	//RCV49-ң�ط�У�̵���49
#define	            RW_RCV50  845	//RCV50-ң�ط�У�̵���50
#define	            RW_RCV51  846	//RCV51-ң�ط�У�̵���51
#define	            RW_RCV52  847	//RCV52-ң�ط�У�̵���52
#define	            RW_RCV53  848	//RCV53-ң�ط�У�̵���53
#define	            RW_RCV54  849	//RCV54-ң�ط�У�̵���54
#define	            RW_RCV55  850	//RCV55-ң�ط�У�̵���55
#define	            RW_RCV56  851	//RCV56-ң�ط�У�̵���56
#define	            RW_RCV57  852	//RCV57-ң�ط�У�̵���57
#define	            RW_RCV58  853	//RCV58-ң�ط�У�̵���58
#define	              RW_GE1  854	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  855	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  856	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  857	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  858	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  859	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  860	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  861	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  862	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  863	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  864	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  865	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  866	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  867	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  868	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  869	//CONST8-����ʱ��̵���8
#define	              RW_TX1  870	//TX1-�������Ӽ̵�����1
#define	              RW_TX2  871	//TX2-�������Ӽ̵�����2
#define	              RW_TX3  872	//TX3-�������Ӽ̵�����3
#define	              RW_TX4  873	//TX4-�������Ӽ̵�����4
#define	              RW_TX5  874	//TX5-�������Ӽ̵�����5
#define	              RW_TX6  875	//TX6-�������Ӽ̵�����6
#define	              RW_TX7  876	//TX7-�������Ӽ̵�����7
#define	              RW_TX8  877	//TX8-�������Ӽ̵�����8
#define	              RW_TX9  878	//TX9-�������Ӽ̵�����9
#define	             RW_TX10  879	//TX10-�������Ӽ̵�����10
#define	             RW_TX11  880	//TX11-�������Ӽ̵�����11
#define	             RW_TX12  881	//TX12-�������Ӽ̵�����12
#define	             RW_TX13  882	//TX13-�������Ӽ̵�����13
#define	             RW_TX14  883	//TX14-�������Ӽ̵�����14
#define	             RW_TX15  884	//TX15-�������Ӽ̵�����15
#define	             RW_TX16  885	//TX16-�������Ӽ̵�����16
#define	              RW_RX1  886	//RX1-�������Ӽ̵�����1
#define	              RW_RX2  887	//RX2-�������Ӽ̵�����2
#define	              RW_RX3  888	//RX3-�������Ӽ̵�����3
#define	              RW_RX4  889	//RX4-�������Ӽ̵�����4
#define	              RW_RX5  890	//RX5-�������Ӽ̵�����5
#define	              RW_RX6  891	//RX6-�������Ӽ̵�����6
#define	              RW_RX7  892	//RX7-�������Ӽ̵�����7
#define	              RW_RX8  893	//RX8-�������Ӽ̵�����8
#define	              RW_RX9  894	//RX9-�������Ӽ̵�����9
#define	             RW_RX10  895	//RX10-�������Ӽ̵�����10
#define	             RW_RX11  896	//RX11-�������Ӽ̵�����11
#define	             RW_RX12  897	//RX12-�������Ӽ̵�����12
#define	             RW_RX13  898	//RX13-�������Ӽ̵�����13
#define	             RW_RX14  899	//RX14-�������Ӽ̵�����14
#define	             RW_RX15  900	//RX15-�������Ӽ̵�����15
#define	             RW_RX16  901	//RX16-�������Ӽ̵�����16
#define	             RW_CNT1  902	//CNT1-�����̵�����1
#define	             RW_CNT2  903	//CNT2-�����̵�����2
#define	             RW_CNT3  904	//CNT3-�����̵�����3
#define	             RW_CNT4  905	//CNT4-�����̵�����4
#define	             RW_CNT5  906	//CNT5-�����̵�����5
#define	             RW_CNT6  907	//CNT6-�����̵�����6
#define	             RW_CNT7  908	//CNT7-�����̵�����7
#define	             RW_CNT8  909	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			910		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			49	

//�м�̵�����
#define	MID_RELAY_WORD_START			51	
#define	MID_RELAY_WORD_NUM			135	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			186	
#define	KEEP_RELAY_WORD_NUM			85	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			271	
#define	TIME_RELAY_WORD_NUM			103	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			374	
#define	TRIP_RELAY_WORD_NUM			2	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			376	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			408	
#define	INPUT_RELAY_WORD_NUM			99	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			507	
#define	OUTPUT_RELAY_WORD_NUM			48	

//ѹ��̵���
#define	SW_RELAY_WORD_START			555	
#define	SW_RELAY_WORD_NUM			8	

//���ü̵���
#define	CFG_RELAY_WORD_START			563	
#define	CFG_RELAY_WORD_NUM			13	

//����̵���
#define	PULSE_RELAY_WORD_START			576	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			592	
#define	CTRLTEST_RELAY_WORD_NUM			32	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			624	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			640	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			664	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			680	
#define	RCP_RELAY_WORD_NUM			58	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			738	
#define	RCE_RELAY_WORD_NUM			58	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			796	
#define	RCV_RELAY_WORD_NUM			58	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			854	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			862	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			870	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			886	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			902	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			902	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			902	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			902	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			902	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			902	
#define	CNT_RELAY_WORD_NUM			8	


#endif