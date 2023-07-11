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
#define	             RW_59HA    2	//59HA-UHA��ѹ�̵�����
#define	             RW_27HA    3	//27HA-UHA��ѹ�̵�����
#define	             RW_59HB    4	//59HB-UHB��ѹ�̵�����
#define	             RW_27HB    5	//27HB-UHB��ѹ�̵�����
#define	             RW_59HC    6	//59HC-UHC��ѹ�̵�����
#define	             RW_27HC    7	//27HC-UHC��ѹ�̵�����
#define	              RW_59N    8	//59N-U0��ѹ�̵�����
#define	              RW_27N    9	//27N-U0��ѹ�̵�����
#define	             RW_59LA   10	//59LA-UHA��ѹ�̵�����
#define	             RW_27LA   11	//27LA-UHA��ѹ�̵�����
#define	             RW_59LB   12	//59LB-UHB��ѹ�̵�����
#define	             RW_27LB   13	//27LB-UHB��ѹ�̵�����
#define	             RW_59LC   14	//59LC-ULC��ѹ�̵�����
#define	             RW_27LC   15	//27LC-ULC��ѹ�̵�����
#define	             RW_50HA   16	//50HA-IHA�����̵�����
#define	             RW_37HA   17	//37HA-IHA�����̵�����
#define	             RW_50HB   18	//50HB-IHB�����̵�����
#define	             RW_37HB   19	//37HB-IHB�����̵�����
#define	             RW_50HC   20	//50HC-IHC�����̵�����
#define	             RW_37HC   21	//37HC-IHC�����̵�����
#define	             RW_50LA   22	//50LA-ILA�����̵�����
#define	             RW_37LA   23	//37LA-ILA�����̵�����
#define	             RW_50LB   24	//50LB-ILB�����̵�����
#define	             RW_37LB   25	//37LB-ILB�����̵�����
#define	             RW_50LC   26	//50LC-ILC�����̵�����
#define	             RW_37LC   27	//37LC-ILC�����̵�����
#define	             RW_50I1   28	//50I1-I1�����̵�����
#define	             RW_37I1   29	//37I1-I1�����̵�����
#define	             RW_50I2   30	//50I2-I2�����̵�����
#define	             RW_37I2   31	//37I2-I2�����̵�����
#define	          RW_ET11ERR   32	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   33	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   34	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   35	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   36	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   37	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   38	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   39	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   40	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   41	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   42	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   43	//CAN1ERR-CAN1����״̬�̵�����
#define	          RW_CAN2ERR   44	//CAN2ERR-CAN2����״̬�̵�����
#define	         RW_NULL1ERR   45	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   46	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   47	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK   48	//MAINTLINK-ά��״̬�̵�����
#define	              RW_REC   49	//REC-¼���̵�����
#define	               RW_M1   50	//M1-�м�̵���1
#define	               RW_M2   51	//M2-�м�̵���2
#define	               RW_M3   52	//M3-�м�̵���3
#define	               RW_M4   53	//M4-�м�̵���4
#define	               RW_M5   54	//M5-�м�̵���5
#define	               RW_M6   55	//M6-�м�̵���6
#define	               RW_M7   56	//M7-�м�̵���7
#define	               RW_M8   57	//M8-�м�̵���8
#define	               RW_M9   58	//M9-�м�̵���9
#define	              RW_M10   59	//M10-�м�̵���10
#define	              RW_M11   60	//M11-�м�̵���11
#define	              RW_M12   61	//M12-�м�̵���12
#define	              RW_M13   62	//M13-�м�̵���13
#define	              RW_M14   63	//M14-�м�̵���14
#define	              RW_M15   64	//M15-�м�̵���15
#define	              RW_M16   65	//M16-�м�̵���16
#define	              RW_M17   66	//M17-�м�̵���17
#define	              RW_M18   67	//M18-�м�̵���18
#define	              RW_M19   68	//M19-�м�̵���19
#define	              RW_M20   69	//M20-�м�̵���20
#define	              RW_M21   70	//M21-�м�̵���21
#define	              RW_M22   71	//M22-�м�̵���22
#define	              RW_M23   72	//M23-�м�̵���23
#define	              RW_M24   73	//M24-�м�̵���24
#define	              RW_M25   74	//M25-�м�̵���25
#define	              RW_M26   75	//M26-�м�̵���26
#define	              RW_M27   76	//M27-�м�̵���27
#define	              RW_M28   77	//M28-�м�̵���28
#define	              RW_M29   78	//M29-�м�̵���29
#define	              RW_M30   79	//M30-�м�̵���30
#define	              RW_M31   80	//M31-�м�̵���31
#define	              RW_M32   81	//M32-�м�̵���32
#define	              RW_M33   82	//M33-�м�̵���33
#define	              RW_M34   83	//M34-�м�̵���34
#define	              RW_M35   84	//M35-�м�̵���35
#define	              RW_M36   85	//M36-�м�̵���36
#define	              RW_M37   86	//M37-�м�̵���37
#define	              RW_M38   87	//M38-�м�̵���38
#define	              RW_M39   88	//M39-�м�̵���39
#define	              RW_M40   89	//M40-�м�̵���40
#define	              RW_M41   90	//M41-�м�̵���41
#define	              RW_M42   91	//M42-�м�̵���42
#define	              RW_M43   92	//M43-�м�̵���43
#define	              RW_M44   93	//M44-�м�̵���44
#define	              RW_M45   94	//M45-�м�̵���45
#define	              RW_M46   95	//M46-�м�̵���46
#define	              RW_M47   96	//M47-�м�̵���47
#define	              RW_M48   97	//M48-�м�̵���48
#define	              RW_M49   98	//M49-�м�̵���49
#define	              RW_M50   99	//M50-�м�̵���50
#define	              RW_M51  100	//M51-�м�̵���51
#define	              RW_M52  101	//M52-�м�̵���52
#define	              RW_M53  102	//M53-�м�̵���53
#define	              RW_M54  103	//M54-�м�̵���54
#define	              RW_M55  104	//M55-�м�̵���55
#define	              RW_M56  105	//M56-�м�̵���56
#define	              RW_M57  106	//M57-�м�̵���57
#define	              RW_M58  107	//M58-�м�̵���58
#define	              RW_M59  108	//M59-�м�̵���59
#define	              RW_M60  109	//M60-�м�̵���60
#define	              RW_M61  110	//M61-�м�̵���61
#define	              RW_M62  111	//M62-�м�̵���62
#define	              RW_M63  112	//M63-�м�̵���63
#define	              RW_M64  113	//M64-�м�̵���64
#define	              RW_M65  114	//M65-�м�̵���65
#define	              RW_M66  115	//M66-�м�̵���66
#define	              RW_M67  116	//M67-�м�̵���67
#define	              RW_M68  117	//M68-�м�̵���68
#define	              RW_M69  118	//M69-�м�̵���69
#define	              RW_M70  119	//M70-�м�̵���70
#define	              RW_M71  120	//M71-�м�̵���71
#define	              RW_M72  121	//M72-�м�̵���72
#define	              RW_M73  122	//M73-�м�̵���73
#define	              RW_M74  123	//M74-�м�̵���74
#define	              RW_M75  124	//M75-�м�̵���75
#define	              RW_M76  125	//M76-�м�̵���76
#define	              RW_M77  126	//M77-�м�̵���77
#define	              RW_M78  127	//M78-�м�̵���78
#define	              RW_M79  128	//M79-�м�̵���79
#define	              RW_M80  129	//M80-�м�̵���80
#define	              RW_M81  130	//M81-�м�̵���81
#define	              RW_M82  131	//M82-�м�̵���82
#define	              RW_M83  132	//M83-�м�̵���83
#define	              RW_M84  133	//M84-�м�̵���84
#define	              RW_M85  134	//M85-�м�̵���85
#define	              RW_M86  135	//M86-�м�̵���86
#define	              RW_M87  136	//M87-�м�̵���87
#define	              RW_M88  137	//M88-�м�̵���88
#define	              RW_M89  138	//M89-�м�̵���89
#define	              RW_M90  139	//M90-�м�̵���90
#define	              RW_M91  140	//M91-�м�̵���91
#define	              RW_M92  141	//M92-�м�̵���92
#define	              RW_M93  142	//M93-�м�̵���93
#define	              RW_M94  143	//M94-�м�̵���94
#define	              RW_M95  144	//M95-�м�̵���95
#define	              RW_M96  145	//M96-�м�̵���96
#define	              RW_M97  146	//M97-�м�̵���97
#define	              RW_M98  147	//M98-�м�̵���98
#define	              RW_M99  148	//M99-�м�̵���99
#define	             RW_M100  149	//M100-�м�̵���100
#define	             RW_M101  150	//M101-�м�̵���101
#define	             RW_M102  151	//M102-�м�̵���102
#define	             RW_M103  152	//M103-�м�̵���103
#define	             RW_M104  153	//M104-�м�̵���104
#define	             RW_M105  154	//M105-�м�̵���105
#define	             RW_M106  155	//M106-�м�̵���106
#define	             RW_M107  156	//M107-�м�̵���107
#define	             RW_M108  157	//M108-�м�̵���108
#define	             RW_M109  158	//M109-�м�̵���109
#define	             RW_M110  159	//M110-�м�̵���110
#define	             RW_M111  160	//M111-�м�̵���111
#define	             RW_M112  161	//M112-�м�̵���112
#define	             RW_M113  162	//M113-�м�̵���113
#define	             RW_M114  163	//M114-�м�̵���114
#define	             RW_M115  164	//M115-�м�̵���115
#define	             RW_M116  165	//M116-�м�̵���116
#define	             RW_M117  166	//M117-�м�̵���117
#define	             RW_M118  167	//M118-�м�̵���118
#define	             RW_M119  168	//M119-�м�̵���119
#define	             RW_M120  169	//M120-�м�̵���120
#define	             RW_M121  170	//M121-�м�̵���121
#define	             RW_M122  171	//M122-�м�̵���122
#define	             RW_M123  172	//M123-�м�̵���123
#define	             RW_M124  173	//M124-�м�̵���124
#define	             RW_M125  174	//M125-�м�̵���125
#define	             RW_M126  175	//M126-�м�̵���126
#define	             RW_M127  176	//M127-�м�̵���127
#define	             RW_M128  177	//M128-�м�̵���128
#define	               RW_H1  178	//H1-�Ա����м�̵�����1
#define	               RW_H2  179	//H2-�Ա����м�̵�����2
#define	               RW_H3  180	//H3-�Ա����м�̵�����3
#define	               RW_H4  181	//H4-�Ա����м�̵�����4
#define	               RW_H5  182	//H5-�Ա����м�̵�����5
#define	               RW_H6  183	//H6-�Ա����м�̵�����6
#define	               RW_H7  184	//H7-�Ա����м�̵�����7
#define	               RW_H8  185	//H8-�Ա����м�̵�����8
#define	               RW_H9  186	//H9-�Ա����м�̵�����9
#define	              RW_H10  187	//H10-�Ա����м�̵�����10
#define	              RW_H11  188	//H11-�Ա����м�̵�����11
#define	              RW_H12  189	//H12-�Ա����м�̵�����12
#define	              RW_H13  190	//H13-�Ա����м�̵�����13
#define	              RW_H14  191	//H14-�Ա����м�̵�����14
#define	              RW_H15  192	//H15-�Ա����м�̵�����15
#define	              RW_H16  193	//H16-�Ա����м�̵�����16
#define	              RW_H17  194	//H17-�Ա����м�̵�����17
#define	              RW_H18  195	//H18-�Ա����м�̵�����18
#define	              RW_H19  196	//H19-�Ա����м�̵�����19
#define	              RW_H20  197	//H20-�Ա����м�̵�����20
#define	              RW_H21  198	//H21-�Ա����м�̵�����21
#define	              RW_H22  199	//H22-�Ա����м�̵�����22
#define	              RW_H23  200	//H23-�Ա����м�̵�����23
#define	              RW_H24  201	//H24-�Ա����м�̵�����24
#define	              RW_H25  202	//H25-�Ա����м�̵�����25
#define	              RW_H26  203	//H26-�Ա����м�̵�����26
#define	              RW_H27  204	//H27-�Ա����м�̵�����27
#define	              RW_H28  205	//H28-�Ա����м�̵�����28
#define	              RW_H29  206	//H29-�Ա����м�̵�����29
#define	              RW_H30  207	//H30-�Ա����м�̵�����30
#define	              RW_H31  208	//H31-�Ա����м�̵�����31
#define	              RW_H32  209	//H32-�Ա����м�̵�����32
#define	              RW_H33  210	//H33-�Ա����м�̵�����33
#define	              RW_H34  211	//H34-�Ա����м�̵�����34
#define	              RW_H35  212	//H35-�Ա����м�̵�����35
#define	              RW_H36  213	//H36-�Ա����м�̵�����36
#define	              RW_H37  214	//H37-�Ա����м�̵�����37
#define	              RW_H38  215	//H38-�Ա����м�̵�����38
#define	              RW_H39  216	//H39-�Ա����м�̵�����39
#define	              RW_H40  217	//H40-�Ա����м�̵�����40
#define	              RW_H41  218	//H41-�Ա����м�̵�����41
#define	              RW_H42  219	//H42-�Ա����м�̵�����42
#define	              RW_H43  220	//H43-�Ա����м�̵�����43
#define	              RW_H44  221	//H44-�Ա����м�̵�����44
#define	              RW_H45  222	//H45-�Ա����м�̵�����45
#define	              RW_H46  223	//H46-�Ա����м�̵�����46
#define	              RW_H47  224	//H47-�Ա����м�̵�����47
#define	              RW_H48  225	//H48-�Ա����м�̵�����48
#define	              RW_H49  226	//H49-�Ա����м�̵�����49
#define	              RW_H50  227	//H50-�Ա����м�̵�����50
#define	              RW_H51  228	//H51-�Ա����м�̵�����51
#define	              RW_H52  229	//H52-�Ա����м�̵�����52
#define	              RW_H53  230	//H53-�Ա����м�̵�����53
#define	              RW_H54  231	//H54-�Ա����м�̵�����54
#define	              RW_H55  232	//H55-�Ա����м�̵�����55
#define	              RW_H56  233	//H56-�Ա����м�̵�����56
#define	              RW_H57  234	//H57-�Ա����м�̵�����57
#define	              RW_H58  235	//H58-�Ա����м�̵�����58
#define	              RW_H59  236	//H59-�Ա����м�̵�����59
#define	              RW_H60  237	//H60-�Ա����м�̵�����60
#define	              RW_H61  238	//H61-�Ա����м�̵�����61
#define	              RW_H62  239	//H62-�Ա����м�̵�����62
#define	              RW_H63  240	//H63-�Ա����м�̵�����63
#define	              RW_H64  241	//H64-�Ա����м�̵�����64
#define	               RW_T1  242	//T1-T1
#define	               RW_T2  243	//T2-T2
#define	               RW_T3  244	//T3-T3
#define	               RW_T4  245	//T4-T4
#define	               RW_T5  246	//T5-T5
#define	               RW_T6  247	//T6-T6
#define	               RW_T7  248	//T7-T7
#define	               RW_T8  249	//T8-T8
#define	               RW_T9  250	//T9-T9
#define	              RW_T10  251	//T10-T10
#define	              RW_T11  252	//T11-T11
#define	              RW_T12  253	//T12-T12
#define	              RW_T13  254	//T13-T13
#define	              RW_T14  255	//T14-T14
#define	              RW_T15  256	//T15-T15
#define	              RW_T16  257	//T16-T16
#define	             RW_TCB1  258	//TCB1-TCB1
#define	             RW_TCB2  259	//TCB2-TCB2
#define	             RW_TCB3  260	//TCB3-TCB3
#define	             RW_TCB4  261	//TCB4-TCB4
#define	             RW_TCB5  262	//TCB5-TCB5
#define	             RW_TCB6  263	//TCB6-TCB6
#define	             RW_TCB7  264	//TCB7-TCB7
#define	             RW_TCB8  265	//TCB8-TCB8
#define	             RW_TCB9  266	//TCB9-TCB9
#define	            RW_TCB10  267	//TCB10-TCB10
#define	            RW_TCB11  268	//TCB11-TCB11
#define	            RW_TCB12  269	//TCB12-TCB12
#define	            RW_TCB13  270	//TCB13-TCB13
#define	            RW_TCB14  271	//TCB14-TCB14
#define	            RW_TCB15  272	//TCB15-TCB15
#define	            RW_TCB16  273	//TCB16-TCB16
#define	            RW_TCB17  274	//TCB17-TCB17
#define	            RW_TCB18  275	//TCB18-TCB18
#define	            RW_TCB19  276	//TCB19-TCB19
#define	            RW_TCB20  277	//TCB20-TCB20
#define	            RW_TCB21  278	//TCB21-TCB21
#define	            RW_TCB22  279	//TCB22-TCB22
#define	            RW_TCB23  280	//TCB23-TCB23
#define	            RW_TCB24  281	//TCB24-TCB24
#define	            RW_TCB25  282	//TCB25-TCB25
#define	            RW_TCB26  283	//TCB26-TCB26
#define	            RW_TCB27  284	//TCB27-TCB27
#define	            RW_TCB28  285	//TCB28-TCB28
#define	            RW_TCB29  286	//TCB29-TCB29
#define	            RW_TCB30  287	//TCB30-TCB30
#define	            RW_TCB31  288	//TCB31-TCB31
#define	            RW_TCB32  289	//TCB32-TCB32
#define	             RW_TSW1  290	//TSW1-TSW1
#define	             RW_TSW2  291	//TSW2-TSW2
#define	             RW_TSW3  292	//TSW3-TSW3
#define	             RW_TSW4  293	//TSW4-TSW4
#define	             RW_TSW5  294	//TSW5-TSW5
#define	             RW_TSW6  295	//TSW6-TSW6
#define	             RW_TSW7  296	//TSW7-TSW7
#define	             RW_TSW8  297	//TSW8-TSW8
#define	             RW_TSW9  298	//TSW9-TSW9
#define	            RW_TSW10  299	//TSW10-TSW10
#define	            RW_TSW11  300	//TSW11-TSW11
#define	            RW_TSW12  301	//TSW12-TSW2
#define	            RW_TSW13  302	//TSW13-TSW13
#define	            RW_TSW14  303	//TSW14-TSW14
#define	            RW_TSW15  304	//TSW15-TSW15
#define	            RW_TSW16  305	//TSW16-TSW16
#define	            RW_TSW17  306	//TSW17-TSW17
#define	            RW_TSW18  307	//TSW18-TSW18
#define	            RW_TSW19  308	//TSW19-TSW19
#define	            RW_TSW20  309	//TSW20-TSW20
#define	            RW_TSW21  310	//TSW21-TSW21
#define	            RW_TSW22  311	//TSW22-TSW22
#define	            RW_TSW23  312	//TSW23-TSW23
#define	            RW_TSW24  313	//TSW24-TSW24
#define	            RW_TSW25  314	//TSW25-TSW25
#define	            RW_TSW26  315	//TSW26-TSW26
#define	            RW_TSW27  316	//TSW27-TSW27
#define	            RW_TSW28  317	//TSW28-TSW28
#define	            RW_TSW29  318	//TSW29-TSW29
#define	            RW_TSW30  319	//TSW30-TSW30
#define	            RW_TSW31  320	//TSW31-TSW1
#define	            RW_TSW32  321	//TSW32-TSW32
#define	              RW_TR1  322	//TR1-��բ�̵���һ
#define	              RW_VR1  323	//VR1-���Լ̵���
#define	              RW_VR2  324	//VR2-����̵���2
#define	              RW_VR3  325	//VR3-����̵���3
#define	              RW_VR4  326	//VR4-����̵���4
#define	              RW_VR5  327	//VR5-����̵���5
#define	              RW_VR6  328	//VR6-����̵���6
#define	              RW_VR7  329	//VR7-����̵���7
#define	              RW_VR8  330	//VR8-����̵���8
#define	              RW_VR9  331	//VR9-����̵���9
#define	             RW_VR10  332	//VR10-����̵���10
#define	             RW_VR11  333	//VR11-����̵���11
#define	             RW_VR12  334	//VR12-����̵���12
#define	             RW_VR13  335	//VR13-����̵���13
#define	             RW_VR14  336	//VR14-����̵���14
#define	             RW_VR15  337	//VR15-����̵���15
#define	             RW_VR16  338	//VR16-����̵���16
#define	             RW_VR17  339	//VR17-����̵���17
#define	             RW_VR18  340	//VR18-����̵���18
#define	             RW_VR19  341	//VR19-����̵���19
#define	             RW_VR20  342	//VR20-����̵���20
#define	             RW_VR21  343	//VR21-����̵���21
#define	             RW_VR22  344	//VR22-����̵���22
#define	             RW_VR23  345	//VR23-����̵���23
#define	             RW_VR24  346	//VR24-����̵���24
#define	             RW_VR25  347	//VR25-����̵���25
#define	             RW_VR26  348	//VR26-����̵���26
#define	             RW_VR27  349	//VR27-����̵���27
#define	             RW_VR28  350	//VR28-����̵���28
#define	             RW_VR29  351	//VR29-����̵���29
#define	             RW_VR30  352	//VR30-����̵���30
#define	             RW_VR31  353	//VR31-����̵���31
#define	             RW_VR32  354	//VR32-����̵���32
#define	              RW_IN1  355	//IN1-����̵���1
#define	              RW_IN2  356	//IN2-����̵���2
#define	              RW_IN3  357	//IN3-����̵���3
#define	              RW_IN4  358	//IN4-����̵���4
#define	              RW_IN5  359	//IN5-����̵���5
#define	              RW_IN6  360	//IN6-����̵���6
#define	              RW_IN7  361	//IN7-����̵���7
#define	              RW_IN8  362	//IN8-����̵���8
#define	              RW_IN9  363	//IN9-����̵���9
#define	             RW_IN10  364	//IN10-����̵���10
#define	             RW_IN11  365	//IN11-����̵���11
#define	             RW_IN12  366	//IN12-����̵���12
#define	             RW_IN13  367	//IN13-����̵���13
#define	             RW_IN14  368	//IN14-����̵���14
#define	             RW_IN15  369	//IN15-����̵���15
#define	             RW_IN16  370	//IN16-����̵���16
#define	             RW_IN17  371	//IN17-����̵���17
#define	             RW_IN18  372	//IN18-����̵���18
#define	             RW_IN19  373	//IN19-����̵���19
#define	             RW_IN20  374	//IN20-����̵���20
#define	             RW_IN21  375	//IN21-����̵���21
#define	             RW_IN22  376	//IN22-����̵���22
#define	             RW_IN23  377	//IN23-����̵���23
#define	             RW_IN24  378	//IN24-����̵���24
#define	             RW_IN25  379	//IN25-����̵���25
#define	             RW_IN26  380	//IN26-����̵���26
#define	             RW_IN27  381	//IN27-����̵���27
#define	             RW_IN28  382	//IN28-����̵���28
#define	             RW_IN29  383	//IN29-����̵���29
#define	             RW_IN30  384	//IN30-����̵���30
#define	             RW_IN31  385	//IN31-����̵���31
#define	             RW_IN32  386	//IN32-����̵���32
#define	             RW_IN33  387	//IN33-����̵���33
#define	             RW_IN34  388	//IN34-����̵���34
#define	             RW_IN35  389	//IN35-����̵���35
#define	             RW_IN36  390	//IN36-����̵���36
#define	             RW_IN37  391	//IN37-����̵���37
#define	             RW_IN38  392	//IN38-����̵���38
#define	             RW_IN39  393	//IN39-����̵���39
#define	             RW_IN40  394	//IN40-����̵���40
#define	             RW_IN41  395	//IN41-����̵���41
#define	             RW_IN42  396	//IN42-����̵���42
#define	             RW_IN43  397	//IN43-����̵���43
#define	             RW_IN44  398	//IN44-����̵���44
#define	             RW_IN45  399	//IN45-����̵���45
#define	             RW_IN46  400	//IN46-����̵���46
#define	             RW_IN47  401	//IN47-����̵���47
#define	             RW_IN48  402	//IN48-����̵���48
#define	             RW_IN49  403	//IN49-����̵���49
#define	             RW_IN50  404	//IN50-����̵���50
#define	             RW_IN51  405	//IN51-����̵���51
#define	             RW_IN52  406	//IN52-����̵���52
#define	             RW_IN53  407	//IN53-����̵���53
#define	             RW_IN54  408	//IN54-����̵���54
#define	             RW_IN55  409	//IN55-����̵���55
#define	             RW_IN56  410	//IN56-����̵���56
#define	             RW_IN57  411	//IN57-����̵���57
#define	             RW_IN58  412	//IN58-����̵���58
#define	             RW_IN59  413	//IN59-����̵���59
#define	             RW_IN60  414	//IN60-����̵���60
#define	             RW_IN61  415	//IN61-����̵���61
#define	             RW_IN62  416	//IN62-����̵���62
#define	             RW_IN63  417	//IN63-����̵���63
#define	             RW_IN64  418	//IN64-����̵���64
#define	             RW_IN65  419	//IN65-����̵���65
#define	             RW_IN66  420	//IN66-����̵���66
#define	             RW_IN67  421	//IN67-����̵���67
#define	             RW_IN68  422	//IN68-����̵���68
#define	             RW_IN69  423	//IN69-�źŸ���
#define	             RW_IN70  424	//IN70-ң��1Ԥ��
#define	             RW_IN71  425	//IN71-ң��2Ԥ��
#define	             RW_IN72  426	//IN72-ң��3Ԥ��
#define	             RW_IN73  427	//IN73-ң��4Ԥ��
#define	             RW_IN74  428	//IN74-ң��5Ԥ��
#define	             RW_IN75  429	//IN75-ң��6Ԥ��
#define	             RW_IN76  430	//IN76-ң��7Ԥ��
#define	             RW_IN77  431	//IN77-ң��8Ԥ��
#define	             RW_IN78  432	//IN78-ң��9Ԥ��
#define	             RW_IN79  433	//IN79-ң��10Ԥ��
#define	             RW_IN80  434	//IN80-ң��11Ԥ��
#define	             RW_IN81  435	//IN81-ң��12Ԥ��
#define	             RW_IN82  436	//IN82-ң��13Ԥ��
#define	             RW_IN83  437	//IN83-ң��14Ԥ��
#define	           RW_INVIN1  438	//INVIN1-����̵���84
#define	           RW_INVIN2  439	//INVIN2-����̵���85
#define	           RW_INVIN3  440	//INVIN3-����̵���86
#define	           RW_INVIN4  441	//INVIN4-����̵���87
#define	           RW_INVIN5  442	//INVIN5-����̵���88
#define	           RW_INVIN6  443	//INVIN6-����̵���89
#define	           RW_INVIN7  444	//INVIN7-����̵���90
#define	           RW_INVIN8  445	//INVIN8-����̵���91
#define	           RW_INVIN9  446	//INVIN9-����̵���92
#define	          RW_INVIN10  447	//INVIN10-����̵���93
#define	          RW_INVIN11  448	//INVIN11-����̵���94
#define	          RW_INVIN12  449	//INVIN12-����̵���95
#define	          RW_INVIN13  450	//INVIN13-����̵���96
#define	          RW_INVIN14  451	//INVIN14-����̵���97
#define	          RW_INVIN15  452	//INVIN15-����̵���98
#define	          RW_INVIN16  453	//INVIN16-����̵���99
#define	             RW_OUT1  454	//OUT1-����1�̵�����
#define	             RW_OUT2  455	//OUT2-����2�̵�����
#define	             RW_OUT3  456	//OUT3-����3�̵�����
#define	             RW_OUT4  457	//OUT4-����4�̵�����
#define	             RW_OUT5  458	//OUT5-����5�̵�����
#define	             RW_OUT6  459	//OUT6-����6�̵�����
#define	             RW_OUT7  460	//OUT7-����7�̵�����
#define	             RW_OUT8  461	//OUT8-����8�̵�����
#define	             RW_OUT9  462	//OUT9-����9�̵�����
#define	            RW_OUT10  463	//OUT10-����10�̵�����
#define	            RW_OUT11  464	//OUT11-����11�̵�����
#define	            RW_OUT12  465	//OUT12-����12�̵�����
#define	            RW_OUT13  466	//OUT13-����13�̵�����
#define	            RW_OUT14  467	//OUT14-����14�̵�����
#define	            RW_OUT15  468	//OUT15-����15�̵�����
#define	            RW_OUT16  469	//OUT16-����16�̵�����
#define	            RW_OUT17  470	//OUT17-����17�̵�����
#define	            RW_OUT18  471	//OUT18-����18�̵�����
#define	            RW_OUT19  472	//OUT19-����19�̵�����
#define	            RW_OUT20  473	//OUT20-����20�̵�����
#define	            RW_OUT21  474	//OUT21-����21�̵�����
#define	            RW_OUT22  475	//OUT22-����22�̵�����
#define	            RW_OUT23  476	//OUT23-����23�̵�����
#define	            RW_OUT24  477	//OUT24-����24�̵�����
#define	            RW_OUT25  478	//OUT25-����25�̵�����
#define	            RW_OUT26  479	//OUT26-����26�̵�����
#define	            RW_OUT27  480	//OUT27-����27�̵�����
#define	            RW_OUT28  481	//OUT28-����28�̵�����
#define	            RW_OUT29  482	//OUT29-����29�̵�����
#define	            RW_OUT30  483	//OUT30-����30�̵�����
#define	            RW_OUT31  484	//OUT31-����31�̵�����
#define	            RW_OUT32  485	//OUT32-����32�̵�����
#define	            RW_OUT33  486	//OUT33-����33�̵�����
#define	            RW_OUT34  487	//OUT34-����34�̵�����
#define	            RW_OUT35  488	//OUT35-����35�̵�����
#define	            RW_OUT36  489	//OUT36-����36�̵�����
#define	            RW_OUT37  490	//OUT37-����37�̵�����
#define	            RW_OUT38  491	//OUT38-����38�̵�����
#define	            RW_OUT39  492	//OUT39-����39�̵�����
#define	            RW_OUT40  493	//OUT40-����40�̵�����
#define	            RW_OUT41  494	//OUT41-����41�̵�����
#define	            RW_OUT42  495	//OUT42-����42�̵�����
#define	            RW_OUT43  496	//OUT43-����43�̵�����
#define	            RW_OUT44  497	//OUT44-����44�̵�����
#define	            RW_OUT45  498	//OUT45-����45�̵�����
#define	            RW_OUT46  499	//OUT46-����46�̵�����
#define	            RW_OUT47  500	//OUT47-����47�̵�����
#define	            RW_OUT48  501	//OUT48-����48�̵�����
#define	          RW_SLMAINT  502	//SLMAINT-����ѹ��̵�����
#define	           RW_SLBAK1  503	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  504	//SLBAK2-����ѹ��2
#define	           RW_SLBAK3  505	//SLBAK3-����ѹ��3
#define	           RW_SLBAK4  506	//SLBAK4-����ѹ��4
#define	          RW_FMODSET  507	//FMODSET-��ֵ�޸Ĺ���
#define	            RW_FBAK1  508	//FBAK1-��������1
#define	            RW_FBAK2  509	//FBAK2-��������2
#define	            RW_FBAK3  510	//FBAK3-��������3
#define	            RW_FBAK4  511	//FBAK4-��������4
#define	              RW_PW1  512	//PW1-����̵���1
#define	              RW_PW2  513	//PW2-����̵���2
#define	              RW_PW3  514	//PW3-����̵���3
#define	              RW_PW4  515	//PW4-����̵���4
#define	              RW_PW5  516	//PW5-����̵���5
#define	              RW_PW6  517	//PW6-����̵���6
#define	              RW_PW7  518	//PW7-����̵���7
#define	              RW_PW8  519	//PW8-����̵���8
#define	              RW_PW9  520	//PW9-����̵���9
#define	             RW_PW10  521	//PW10-����̵���10
#define	             RW_PW11  522	//PW11-����̵���11
#define	             RW_PW12  523	//PW12-����̵���12
#define	             RW_PW13  524	//PW13-����̵���13
#define	             RW_PW14  525	//PW14-����̵���14
#define	             RW_PW15  526	//PW15-����̵���15
#define	             RW_PW16  527	//PW16-����̵���16
#define	              RW_TO1  528	//TO1-�����̵���1
#define	              RW_TO2  529	//TO2-�����̵���2
#define	              RW_TO3  530	//TO3-�����̵���3
#define	              RW_TO4  531	//TO4-�����̵���4
#define	              RW_TO5  532	//TO5-�����̵���5
#define	              RW_TO6  533	//TO6-�����̵���6
#define	              RW_TO7  534	//TO7-�����̵���7
#define	              RW_TO8  535	//TO8-�����̵���8
#define	              RW_TO9  536	//TO9-�����̵���9
#define	             RW_TO10  537	//TO10-�����̵���10
#define	             RW_TO11  538	//TO11-�����̵���11
#define	             RW_TO12  539	//TO12-�����̵���12
#define	             RW_TO13  540	//TO13-�����̵���13
#define	             RW_TO14  541	//TO14-�����̵���14
#define	             RW_TO15  542	//TO15-�����̵���15
#define	             RW_TO16  543	//TO16-�����̵���16
#define	             RW_TO17  544	//TO17-�����̵���17
#define	             RW_TO18  545	//TO18-�����̵���18
#define	             RW_TO19  546	//TO19-�����̵���19
#define	             RW_TO20  547	//TO20-�����̵���20
#define	             RW_TO21  548	//TO21-�����̵���21
#define	             RW_TO22  549	//TO22-�����̵���22
#define	             RW_TO23  550	//TO23-�����̵���23
#define	             RW_TO24  551	//TO24-�����̵���24
#define	             RW_TO25  552	//TO25-�����̵���25
#define	             RW_TO26  553	//TO26-�����̵���26
#define	             RW_TO27  554	//TO27-�����̵���27
#define	             RW_TO28  555	//TO28-�����̵���28
#define	             RW_TO29  556	//TO29-�����̵���29
#define	             RW_TO30  557	//TO30-�����̵���30
#define	             RW_TO31  558	//TO31-�����̵���31
#define	             RW_TO32  559	//TO32-�����̵���32
#define	             RW_SIG1  560	//SIG1-�źż̵���1
#define	             RW_SIG2  561	//SIG2-�źż̵���2
#define	             RW_SIG3  562	//SIG3-�źż̵���3
#define	             RW_SIG4  563	//SIG4-�źż̵���4
#define	             RW_SIG5  564	//SIG5-�źż̵���5
#define	             RW_SIG6  565	//SIG6-�źż̵���6
#define	             RW_SIG7  566	//SIG7-�źż̵���7
#define	             RW_SIG8  567	//SIG8-�źż̵���8
#define	            RW_LED1G  568	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  569	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  570	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  571	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  572	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  573	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  574	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  575	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  576	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  577	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  578	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  579	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  580	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  581	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  582	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  583	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  584	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  585	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  586	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  587	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  588	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  589	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  590	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  591	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  592	//VIN1-����̵���84
#define	             RW_VIN2  593	//VIN2-����̵���85
#define	             RW_VIN3  594	//VIN3-����̵���86
#define	             RW_VIN4  595	//VIN4-����̵���87
#define	             RW_VIN5  596	//VIN5-����̵���88
#define	             RW_VIN6  597	//VIN6-����̵���89
#define	             RW_VIN7  598	//VIN7-����̵���90
#define	             RW_VIN8  599	//VIN8-����̵���91
#define	             RW_VIN9  600	//VIN9-����̵���92
#define	            RW_VIN10  601	//VIN10-����̵���93
#define	            RW_VIN11  602	//VIN11-����̵���94
#define	            RW_VIN12  603	//VIN12-����̵���95
#define	            RW_VIN13  604	//VIN13-����̵���96
#define	            RW_VIN14  605	//VIN14-����̵���97
#define	            RW_VIN15  606	//VIN15-����̵���98
#define	            RW_VIN16  607	//VIN16-����̵���99
#define	             RW_RCP1  608	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  609	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  610	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  611	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  612	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  613	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  614	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  615	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  616	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  617	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  618	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  619	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  620	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  621	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  622	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  623	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  624	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  625	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  626	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  627	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  628	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  629	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  630	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  631	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  632	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  633	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  634	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  635	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  636	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  637	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  638	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  639	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  640	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  641	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  642	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  643	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  644	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  645	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  646	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  647	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  648	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  649	//RCP42-ң��Ԥ�ü̵���42
#define	            RW_RCP43  650	//RCP43-ң��Ԥ�ü̵���43
#define	            RW_RCP44  651	//RCP44-ң��Ԥ�ü̵���44
#define	            RW_RCP45  652	//RCP45-ң��Ԥ�ü̵���45
#define	            RW_RCP46  653	//RCP46-ң��Ԥ�ü̵���46
#define	            RW_RCP47  654	//RCP47-ң��Ԥ�ü̵���47
#define	            RW_RCP48  655	//RCP48-ң��Ԥ�ü̵���48
#define	            RW_RCP49  656	//RCP49-ң��Ԥ�ü̵���49
#define	            RW_RCP50  657	//RCP50-ң��Ԥ�ü̵���50
#define	            RW_RCP51  658	//RCP51-ң��Ԥ�ü̵���51
#define	            RW_RCP52  659	//RCP52-ң��Ԥ�ü̵���52
#define	            RW_RCP53  660	//RCP53-ң��Ԥ�ü̵���53
#define	            RW_RCP54  661	//RCP54-ң��Ԥ�ü̵���54
#define	            RW_RCP55  662	//RCP55-ң��Ԥ�ü̵���55
#define	            RW_RCP56  663	//RCP56-ң��Ԥ�ü̵���56
#define	             RW_RCE1  664	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  665	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  666	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  667	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  668	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  669	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  670	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  671	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  672	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  673	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  674	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  675	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  676	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  677	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  678	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  679	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  680	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  681	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  682	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  683	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  684	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  685	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  686	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  687	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  688	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  689	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  690	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  691	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  692	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  693	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  694	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  695	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  696	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  697	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  698	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  699	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  700	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  701	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  702	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  703	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  704	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  705	//RCE42-ң��ִ�м̵���42
#define	            RW_RCE43  706	//RCE43-ң��ִ�м̵���43
#define	            RW_RCE44  707	//RCE44-ң��ִ�м̵���44
#define	            RW_RCE45  708	//RCE45-ң��ִ�м̵���45
#define	            RW_RCE46  709	//RCE46-ң��ִ�м̵���46
#define	            RW_RCE47  710	//RCE47-ң��ִ�м̵���47
#define	            RW_RCE48  711	//RCE48-ң��ִ�м̵���48
#define	            RW_RCE49  712	//RCE49-ң��ִ�м̵���49
#define	            RW_RCE50  713	//RCE50-ң��ִ�м̵���50
#define	            RW_RCE51  714	//RCE51-ң��ִ�м̵���51
#define	            RW_RCE52  715	//RCE52-ң��ִ�м̵���52
#define	            RW_RCE53  716	//RCE53-ң��ִ�м̵���53
#define	            RW_RCE54  717	//RCE54-ң��ִ�м̵���54
#define	            RW_RCE55  718	//RCE55-ң��ִ�м̵���55
#define	            RW_RCE56  719	//RCE56-ң��ִ�м̵���56
#define	             RW_RCV1  720	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  721	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  722	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  723	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  724	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  725	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  726	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  727	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  728	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  729	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  730	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  731	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  732	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  733	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  734	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  735	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  736	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  737	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  738	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  739	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  740	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  741	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  742	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  743	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  744	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  745	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  746	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  747	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  748	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  749	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  750	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  751	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  752	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  753	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  754	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  755	//RCV36-ң�ط�У�̵���36
#define	            RW_RCV37  756	//RCV37-ң�ط�У�̵���37
#define	            RW_RCV38  757	//RCV38-ң�ط�У�̵���38
#define	            RW_RCV39  758	//RCV39-ң�ط�У�̵���39
#define	            RW_RCV40  759	//RCV40-ң�ط�У�̵���40
#define	            RW_RCV41  760	//RCV41-ң�ط�У�̵���41
#define	            RW_RCV42  761	//RCV42-ң�ط�У�̵���42
#define	            RW_RCV43  762	//RCV43-ң�ط�У�̵���43
#define	            RW_RCV44  763	//RCV44-ң�ط�У�̵���44
#define	            RW_RCV45  764	//RCV45-ң�ط�У�̵���45
#define	            RW_RCV46  765	//RCV46-ң�ط�У�̵���46
#define	            RW_RCV47  766	//RCV47-ң�ط�У�̵���47
#define	            RW_RCV48  767	//RCV48-ң�ط�У�̵���48
#define	            RW_RCV49  768	//RCV49-ң�ط�У�̵���49
#define	            RW_RCV50  769	//RCV50-ң�ط�У�̵���50
#define	            RW_RCV51  770	//RCV51-ң�ط�У�̵���51
#define	            RW_RCV52  771	//RCV52-ң�ط�У�̵���52
#define	            RW_RCV53  772	//RCV53-ң�ط�У�̵���53
#define	            RW_RCV54  773	//RCV54-ң�ط�У�̵���54
#define	            RW_RCV55  774	//RCV55-ң�ط�У�̵���55
#define	            RW_RCV56  775	//RCV56-ң�ط�У�̵���56
#define	              RW_GE1  776	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  777	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  778	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  779	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  780	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  781	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  782	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  783	//GE8-���ڵ��ڼ̵���8
#define	              RW_CT1  784	//CT1-����ʱ��̵���1
#define	              RW_CT2  785	//CT2-����ʱ��̵���2
#define	              RW_CT3  786	//CT3-����ʱ��̵���3
#define	              RW_CT4  787	//CT4-����ʱ��̵���4
#define	              RW_CT5  788	//CT5-����ʱ��̵���5
#define	              RW_CT6  789	//CT6-����ʱ��̵���6
#define	              RW_CT7  790	//CT7-����ʱ��̵���7
#define	              RW_CT8  791	//CT8-����ʱ��̵���8
#define	              RW_TX1  792	//TX1-�������Ӽ̵�����1
#define	              RW_TX2  793	//TX2-�������Ӽ̵�����2
#define	              RW_TX3  794	//TX3-�������Ӽ̵�����3
#define	              RW_TX4  795	//TX4-�������Ӽ̵�����4
#define	              RW_TX5  796	//TX5-�������Ӽ̵�����5
#define	              RW_TX6  797	//TX6-�������Ӽ̵�����6
#define	              RW_TX7  798	//TX7-�������Ӽ̵�����7
#define	              RW_TX8  799	//TX8-�������Ӽ̵�����7
#define	              RW_TX9  800	//TX9-�������Ӽ̵�����9
#define	             RW_TX10  801	//TX10-�������Ӽ̵�����10
#define	             RW_TX11  802	//TX11-�������Ӽ̵�����11
#define	             RW_TX12  803	//TX12-�������Ӽ̵�����12
#define	             RW_TX13  804	//TX13-�������Ӽ̵�����13
#define	             RW_TX14  805	//TX14-�������Ӽ̵�����14
#define	             RW_TX15  806	//TX15-�������Ӽ̵�����15
#define	             RW_TX16  807	//TX16-�������Ӽ̵�����16
#define	              RW_RX1  808	//RX1-�������Ӽ̵�����1
#define	              RW_RX2  809	//RX2-�������Ӽ̵�����2
#define	              RW_RX3  810	//RX3-�������Ӽ̵�����3
#define	              RW_RX4  811	//RX4-�������Ӽ̵�����4
#define	              RW_RX5  812	//RX5-�������Ӽ̵�����5
#define	              RW_RX6  813	//RX6-�������Ӽ̵�����6
#define	              RW_RX7  814	//RX7-�������Ӽ̵�����7
#define	              RW_RX8  815	//RX8-�������Ӽ̵�����8
#define	              RW_RX9  816	//RX9-�������Ӽ̵�����9
#define	             RW_RX10  817	//RX10-�������Ӽ̵�����10
#define	             RW_RX11  818	//RX11-�������Ӽ̵�����11
#define	             RW_RX12  819	//RX12-�������Ӽ̵�����12
#define	             RW_RX13  820	//RX13-�������Ӽ̵�����13
#define	             RW_RX14  821	//RX14-�������Ӽ̵�����14
#define	             RW_RX15  822	//RX15-�������Ӽ̵�����15
#define	             RW_RX16  823	//RX16-�������Ӽ̵�����16
#define	             RW_CNT1  824	//CNT1-�����̵�����1
#define	             RW_CNT2  825	//CNT2-�����̵�����2
#define	             RW_CNT3  826	//CNT3-�����̵�����3
#define	             RW_CNT4  827	//CNT4-�����̵�����4
#define	             RW_CNT5  828	//CNT5-�����̵�����5
#define	             RW_CNT6  829	//CNT6-�����̵�����6
#define	             RW_CNT7  830	//CNT7-�����̵�����7
#define	             RW_CNT8  831	//CNT8-�����̵�����7
#define	RELAY_WORD_NUM			832		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			47	

//�м�̵�����
#define	MID_RELAY_WORD_START			49	
#define	MID_RELAY_WORD_NUM			129	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			178	
#define	KEEP_RELAY_WORD_NUM			64	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			242	
#define	TIME_RELAY_WORD_NUM			80	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			322	
#define	TRIP_RELAY_WORD_NUM			1	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			323	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			355	
#define	INPUT_RELAY_WORD_NUM			99	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			454	
#define	OUTPUT_RELAY_WORD_NUM			48	

//ѹ��̵���
#define	SW_RELAY_WORD_START			502	
#define	SW_RELAY_WORD_NUM			5	

//���ü̵���
#define	CFG_RELAY_WORD_START			507	
#define	CFG_RELAY_WORD_NUM			5	

//����̵���
#define	PULSE_RELAY_WORD_START			512	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			528	
#define	CTRLTEST_RELAY_WORD_NUM			32	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			560	
#define	SIGNAL_RELAY_WORD_NUM			8	

//LED�̵���
#define	LED_RELAY_WORD_START			568	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			592	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			608	
#define	RCP_RELAY_WORD_NUM			56	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			664	
#define	RCE_RELAY_WORD_NUM			56	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			720	
#define	RCV_RELAY_WORD_NUM			56	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			776	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			784	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			792	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			808	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			824	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			824	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			824	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			824	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			824	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			824	
#define	CNT_RELAY_WORD_NUM			8	


#endif