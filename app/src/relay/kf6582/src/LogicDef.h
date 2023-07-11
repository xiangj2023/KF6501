/*************************************************************************
* 
*   版权所有(C) 天津凯发电气股份有限公司(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   文件名称：
*			Logicdef.h
* 
*   软件模块：逻辑方程
*
* 
*	描述 继电器宏定义， 继电器执行类型定义
* 
* 
* 
*   函数
*
*
*	修改历史
* 
*		修改人		修改日期		修改内容
* 
*		kangqian	2009-1-15		初始创建
* 
*************************************************************************/

#ifndef __Logicdef_H
#define __Logicdef_H

/**********************/

#define	 MAX_FUNCTYPE 	 12 	 //最大函数类型
#define	              FALSE1    0	//FALSE-FALSE
#define	               TRUE1    1	//TRUE-TRUE
#define	             RW_59U1    2	//59U1-U1有压
#define	             RW_27U1    3	//27U1-U1无压
#define	             RW_59U2    4	//59U2-U2有压
#define	             RW_27U2    5	//27U2-U2无压
#define	             RW_59U3    6	//59U3-U3有压
#define	             RW_27U3    7	//27U3-U3无压
#define	             RW_59U4    8	//59U4-U4有压
#define	             RW_27U4    9	//27U4-U4无压
#define	             RW_59U5   10	//59U5-U5有压
#define	             RW_27U5   11	//27U5-U5无压
#define	             RW_59U6   12	//59U6-U6有压
#define	             RW_27U6   13	//27U6-U6无压
#define	             RW_59U7   14	//59U7-U7有压
#define	             RW_27U7   15	//27U7-U7无压
#define	             RW_59U8   16	//59U8-U8有压
#define	             RW_27U8   17	//27U8-U8无压
#define	             RW_50I1   18	//50I1-I1有流
#define	             RW_37I1   19	//37I1-I1无流
#define	             RW_50I2   20	//50I2-I2有流
#define	             RW_37I2   21	//37I2-I2无流
#define	             RW_50I3   22	//50I3-I3有流
#define	             RW_37I3   23	//37I3-I3无流
#define	             RW_50I4   24	//50I4-I4有流
#define	             RW_37I4   25	//37I4-I4无流
#define	             RW_50I5   26	//50I5-I5有流
#define	             RW_37I5   27	//37I5-I5无流
#define	             RW_50I6   28	//50I6-I6有流
#define	             RW_37I6   29	//37I6-I6无流
#define	             RW_50I7   30	//50I7-I7有流
#define	             RW_37I7   31	//37I7-I7无流
#define	             RW_50I8   32	//50I8-I8有流
#define	             RW_37I8   33	//37I8-I8无流
#define	          RW_ET11ERR   34	//ET11ERR-以太网1链接1状态继电器字
#define	          RW_ET12ERR   35	//ET12ERR-以太网1链接2状态继电器字
#define	          RW_ET13ERR   36	//ET13ERR-以太网1链接3状态继电器字
#define	          RW_ET14ERR   37	//ET14ERR-以太网1链接4状态继电器字
#define	          RW_ET15ERR   38	//ET15ERR-以太网1链接5状态继电器字
#define	          RW_ET21ERR   39	//ET21ERR-以太网2链接1状态继电器字
#define	          RW_ET22ERR   40	//ET22ERR-以太网2链接2状态继电器字
#define	          RW_ET23ERR   41	//ET23ERR-以太网2链接3状态继电器字
#define	          RW_ET24ERR   42	//ET24ERR-以太网2链接4状态继电器字
#define	          RW_ET25ERR   43	//ET25ERR-以太网2链接5状态继电器字
#define	          RW_COM1ERR   44	//COM1ERR-串口1链接状态继电器字
#define	          RW_CAN1ERR   45	//CAN1ERR-CAN1链接状态继电器字
#define	          RW_CAN2ERR   46	//CAN2ERR-CAN2链接状态继电器字
#define	         RW_NULL1ERR   47	//NULL1ERR-空1链接状态继电器字
#define	         RW_NULL2ERR   48	//NULL2ERR-空2链接状态继电器字
#define	         RW_NULL3ERR   49	//NULL3ERR-空3链接状态继电器字
#define	        RW_MAINTLINK   50	//MAINTLINK-维护状态继电器字
#define	              RW_REC   51	//REC-录波继电器字
#define	               RW_M1   52	//M1-中间继电器1
#define	               RW_M2   53	//M2-中间继电器2
#define	               RW_M3   54	//M3-中间继电器3
#define	               RW_M4   55	//M4-中间继电器4
#define	               RW_M5   56	//M5-中间继电器5
#define	               RW_M6   57	//M6-中间继电器6
#define	               RW_M7   58	//M7-中间继电器7
#define	               RW_M8   59	//M8-中间继电器8
#define	               RW_M9   60	//M9-中间继电器9
#define	              RW_M10   61	//M10-中间继电器10
#define	              RW_M11   62	//M11-中间继电器11
#define	              RW_M12   63	//M12-中间继电器12
#define	              RW_M13   64	//M13-中间继电器13
#define	              RW_M14   65	//M14-中间继电器14
#define	              RW_M15   66	//M15-中间继电器15
#define	              RW_M16   67	//M16-中间继电器16
#define	              RW_M17   68	//M17-中间继电器17
#define	              RW_M18   69	//M18-中间继电器18
#define	              RW_M19   70	//M19-中间继电器19
#define	              RW_M20   71	//M20-中间继电器20
#define	              RW_M21   72	//M21-中间继电器21
#define	              RW_M22   73	//M22-中间继电器22
#define	              RW_M23   74	//M23-中间继电器23
#define	              RW_M24   75	//M24-中间继电器24
#define	              RW_M25   76	//M25-中间继电器25
#define	              RW_M26   77	//M26-中间继电器26
#define	              RW_M27   78	//M27-中间继电器27
#define	              RW_M28   79	//M28-中间继电器28
#define	              RW_M29   80	//M29-中间继电器29
#define	              RW_M30   81	//M30-中间继电器30
#define	              RW_M31   82	//M31-中间继电器31
#define	              RW_M32   83	//M32-中间继电器32
#define	              RW_M33   84	//M33-中间继电器33
#define	              RW_M34   85	//M34-中间继电器34
#define	              RW_M35   86	//M35-中间继电器35
#define	              RW_M36   87	//M36-中间继电器36
#define	              RW_M37   88	//M37-中间继电器37
#define	              RW_M38   89	//M38-中间继电器38
#define	              RW_M39   90	//M39-中间继电器39
#define	              RW_M40   91	//M40-中间继电器40
#define	              RW_M41   92	//M41-中间继电器41
#define	              RW_M42   93	//M42-中间继电器42
#define	              RW_M43   94	//M43-中间继电器43
#define	              RW_M44   95	//M44-中间继电器44
#define	              RW_M45   96	//M45-中间继电器45
#define	              RW_M46   97	//M46-中间继电器46
#define	              RW_M47   98	//M47-中间继电器47
#define	              RW_M48   99	//M48-中间继电器48
#define	              RW_M49  100	//M49-中间继电器49
#define	              RW_M50  101	//M50-中间继电器50
#define	              RW_M51  102	//M51-中间继电器51
#define	              RW_M52  103	//M52-中间继电器52
#define	              RW_M53  104	//M53-中间继电器53
#define	              RW_M54  105	//M54-中间继电器54
#define	              RW_M55  106	//M55-中间继电器55
#define	              RW_M56  107	//M56-中间继电器56
#define	              RW_M57  108	//M57-中间继电器57
#define	              RW_M58  109	//M58-中间继电器58
#define	              RW_M59  110	//M59-中间继电器59
#define	              RW_M60  111	//M60-中间继电器60
#define	              RW_M61  112	//M61-中间继电器61
#define	              RW_M62  113	//M62-中间继电器62
#define	              RW_M63  114	//M63-中间继电器63
#define	              RW_M64  115	//M64-中间继电器64
#define	              RW_M65  116	//M65-中间继电器65
#define	              RW_M66  117	//M66-中间继电器66
#define	              RW_M67  118	//M67-中间继电器67
#define	              RW_M68  119	//M68-中间继电器68
#define	              RW_M69  120	//M69-中间继电器69
#define	              RW_M70  121	//M70-中间继电器70
#define	              RW_M71  122	//M71-中间继电器71
#define	              RW_M72  123	//M72-中间继电器72
#define	              RW_M73  124	//M73-中间继电器73
#define	              RW_M74  125	//M74-中间继电器74
#define	              RW_M75  126	//M75-中间继电器75
#define	              RW_M76  127	//M76-中间继电器76
#define	              RW_M77  128	//M77-中间继电器77
#define	              RW_M78  129	//M78-中间继电器78
#define	              RW_M79  130	//M79-中间继电器79
#define	              RW_M80  131	//M80-中间继电器80
#define	              RW_M81  132	//M81-中间继电器81
#define	              RW_M82  133	//M82-中间继电器82
#define	              RW_M83  134	//M83-中间继电器83
#define	              RW_M84  135	//M84-中间继电器84
#define	              RW_M85  136	//M85-中间继电器85
#define	              RW_M86  137	//M86-中间继电器86
#define	              RW_M87  138	//M87-中间继电器87
#define	              RW_M88  139	//M88-中间继电器88
#define	              RW_M89  140	//M89-中间继电器89
#define	              RW_M90  141	//M90-中间继电器90
#define	              RW_M91  142	//M91-中间继电器91
#define	              RW_M92  143	//M92-中间继电器92
#define	              RW_M93  144	//M93-中间继电器93
#define	              RW_M94  145	//M94-中间继电器94
#define	              RW_M95  146	//M95-中间继电器95
#define	              RW_M96  147	//M96-中间继电器96
#define	              RW_M97  148	//M97-中间继电器97
#define	              RW_M98  149	//M98-中间继电器98
#define	              RW_M99  150	//M99-中间继电器99
#define	             RW_M100  151	//M100-中间继电器100
#define	             RW_M101  152	//M101-中间继电器101
#define	             RW_M102  153	//M102-中间继电器102
#define	             RW_M103  154	//M103-中间继电器103
#define	             RW_M104  155	//M104-中间继电器104
#define	             RW_M105  156	//M105-中间继电器105
#define	             RW_M106  157	//M106-中间继电器106
#define	             RW_M107  158	//M107-中间继电器107
#define	             RW_M108  159	//M108-中间继电器108
#define	             RW_M109  160	//M109-中间继电器109
#define	             RW_M110  161	//M110-中间继电器110
#define	             RW_M111  162	//M111-中间继电器111
#define	             RW_M112  163	//M112-中间继电器112
#define	             RW_M113  164	//M113-中间继电器113
#define	             RW_M114  165	//M114-中间继电器114
#define	             RW_M115  166	//M115-中间继电器115
#define	             RW_M116  167	//M116-中间继电器116
#define	             RW_M117  168	//M117-中间继电器117
#define	             RW_M118  169	//M118-中间继电器118
#define	             RW_M119  170	//M119-中间继电器119
#define	             RW_M120  171	//M120-中间继电器120
#define	             RW_M121  172	//M121-中间继电器121
#define	             RW_M122  173	//M122-中间继电器122
#define	             RW_M123  174	//M123-中间继电器123
#define	             RW_M124  175	//M124-中间继电器124
#define	             RW_M125  176	//M125-中间继电器125
#define	             RW_M126  177	//M126-中间继电器126
#define	             RW_M127  178	//M127-中间继电器127
#define	             RW_M128  179	//M128-中间继电器128
#define	          RW_INRESET  180	//INRESET-遥信复归
#define	            RW_INBZT  181	//INBZT-遥信投入备自投
#define	            RW_74PT1  182	//74PT1-PT1告警继电器字
#define	            RW_74PT2  183	//74PT2-PT2告警继电器字
#define	            RW_74PT3  184	//74PT3-PT3告警继电器字
#define	            RW_74PT4  185	//74PT4-PT4告警继电器字
#define	               RW_H1  186	//H1-自保持中间继电器字1
#define	               RW_H2  187	//H2-自保持中间继电器字2
#define	               RW_H3  188	//H3-自保持中间继电器字3
#define	               RW_H4  189	//H4-自保持中间继电器字4
#define	               RW_H5  190	//H5-自保持中间继电器字5
#define	               RW_H6  191	//H6-自保持中间继电器字6
#define	               RW_H7  192	//H7-自保持中间继电器字7
#define	               RW_H8  193	//H8-自保持中间继电器字8
#define	               RW_H9  194	//H9-自保持中间继电器字9
#define	              RW_H10  195	//H10-自保持中间继电器字10
#define	              RW_H11  196	//H11-自保持中间继电器字11
#define	              RW_H12  197	//H12-自保持中间继电器字12
#define	              RW_H13  198	//H13-自保持中间继电器字13
#define	              RW_H14  199	//H14-自保持中间继电器字14
#define	              RW_H15  200	//H15-自保持中间继电器字15
#define	              RW_H16  201	//H16-自保持中间继电器字16
#define	              RW_H17  202	//H17-自保持中间继电器字17
#define	              RW_H18  203	//H18-自保持中间继电器字18
#define	              RW_H19  204	//H19-自保持中间继电器字19
#define	              RW_H20  205	//H20-自保持中间继电器字20
#define	              RW_H21  206	//H21-自保持中间继电器字21
#define	              RW_H22  207	//H22-自保持中间继电器字22
#define	              RW_H23  208	//H23-自保持中间继电器字23
#define	              RW_H24  209	//H24-自保持中间继电器字24
#define	              RW_H25  210	//H25-自保持中间继电器字25
#define	              RW_H26  211	//H26-自保持中间继电器字26
#define	              RW_H27  212	//H27-自保持中间继电器字27
#define	              RW_H28  213	//H28-自保持中间继电器字28
#define	              RW_H29  214	//H29-自保持中间继电器字29
#define	              RW_H30  215	//H30-自保持中间继电器字30
#define	              RW_H31  216	//H31-自保持中间继电器字31
#define	              RW_H32  217	//H32-自保持中间继电器字32
#define	              RW_H33  218	//H33-自保持中间继电器字33
#define	              RW_H34  219	//H34-自保持中间继电器字34
#define	              RW_H35  220	//H35-自保持中间继电器字35
#define	              RW_H36  221	//H36-自保持中间继电器字36
#define	              RW_H37  222	//H37-自保持中间继电器字37
#define	              RW_H38  223	//H38-自保持中间继电器字38
#define	              RW_H39  224	//H39-自保持中间继电器字39
#define	              RW_H40  225	//H40-自保持中间继电器字40
#define	              RW_H41  226	//H41-自保持中间继电器字41
#define	              RW_H42  227	//H42-自保持中间继电器字42
#define	              RW_H43  228	//H43-自保持中间继电器字43
#define	              RW_H44  229	//H44-自保持中间继电器字44
#define	              RW_H45  230	//H45-自保持中间继电器字45
#define	              RW_H46  231	//H46-自保持中间继电器字46
#define	              RW_H47  232	//H47-自保持中间继电器字47
#define	              RW_H48  233	//H48-自保持中间继电器字48
#define	              RW_H49  234	//H49-自保持中间继电器字49
#define	              RW_H50  235	//H50-自保持中间继电器字50
#define	              RW_H51  236	//H51-自保持中间继电器字51
#define	              RW_H52  237	//H52-自保持中间继电器字52
#define	              RW_H53  238	//H53-自保持中间继电器字53
#define	              RW_H54  239	//H54-自保持中间继电器字54
#define	              RW_H55  240	//H55-自保持中间继电器字55
#define	              RW_H56  241	//H56-自保持中间继电器字56
#define	              RW_H57  242	//H57-自保持中间继电器字57
#define	              RW_H58  243	//H58-自保持中间继电器字58
#define	              RW_H59  244	//H59-自保持中间继电器字59
#define	              RW_H60  245	//H60-自保持中间继电器字60
#define	              RW_H61  246	//H61-自保持中间继电器字61
#define	              RW_H62  247	//H62-自保持中间继电器字62
#define	              RW_H63  248	//H63-自保持中间继电器字63
#define	              RW_H64  249	//H64-自保持中间继电器字64
#define	             RW_LOCK  250	//LOCK-闭锁继电器字
#define	             RW_CHR1  251	//CHR1-充电方式一继电器字
#define	             RW_CHR2  252	//CHR2-充电方式二继电器字
#define	             RW_CHR3  253	//CHR3-充电方式三继电器字
#define	             RW_CHR4  254	//CHR4-充电方式四继电器字
#define	             RW_CHR5  255	//CHR5-充电方式五继电器字
#define	             RW_CHR6  256	//CHR6-充电方式六继电器字
#define	              RW_PI1  257	//PI1-备自投启动继电器字1
#define	              RW_PI2  258	//PI2-备自投启动继电器字2
#define	              RW_PI3  259	//PI3-备自投启动继电器字3
#define	              RW_PI4  260	//PI4-备自投启动继电器字4
#define	              RW_PI5  261	//PI5-备自投启动继电器字5
#define	              RW_PI6  262	//PI6-备自投启动继电器字6
#define	              RW_PI7  263	//PI7-备自投启动继电器字7
#define	              RW_PI8  264	//PI8-备自投启动继电器字8
#define	              RW_PI9  265	//PI9-备自投启动继电器字9
#define	             RW_PI10  266	//PI10-备自投启动继电器字10
#define	             RW_PI11  267	//PI11-备自投启动继电器字11
#define	             RW_PI12  268	//PI12-备自投启动继电器字12
#define	            RW_ZB1GZ  269	//ZB1GZ-主变1故障
#define	            RW_ZB2GZ  270	//ZB2GZ-主变2故障
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
#define	           RW_TREBZT  367	//TREBZT-备自投重新投入时间继电器字
#define	            RW_TCHR1  368	//TCHR1-充电方式1时间继电器字
#define	            RW_TCHR2  369	//TCHR2-充电方式2时间继电器字
#define	            RW_TCHR3  370	//TCHR3-充电方式3时间继电器字
#define	            RW_TCHR4  371	//TCHR4-充电方式4时间继电器字
#define	            RW_TCHR5  372	//TCHR5-充电方式5时间继电器字
#define	            RW_TCHR6  373	//TCHR6-充电方式6时间继电器字
#define	              RW_TR1  374	//TR1-跳闸继电器1
#define	              RW_TR2  375	//TR2-跳闸继电器2
#define	              RW_VR1  376	//VR1-调试继电器
#define	              RW_VR2  377	//VR2-虚拟继电器2
#define	              RW_VR3  378	//VR3-虚拟继电器3
#define	              RW_VR4  379	//VR4-虚拟继电器4
#define	              RW_VR5  380	//VR5-虚拟继电器5
#define	              RW_VR6  381	//VR6-虚拟继电器6
#define	              RW_VR7  382	//VR7-虚拟继电器7
#define	              RW_VR8  383	//VR8-虚拟继电器8
#define	              RW_VR9  384	//VR9-虚拟继电器9
#define	             RW_VR10  385	//VR10-虚拟继电器10
#define	             RW_VR11  386	//VR11-虚拟继电器11
#define	             RW_VR12  387	//VR12-虚拟继电器12
#define	             RW_VR13  388	//VR13-虚拟继电器13
#define	             RW_VR14  389	//VR14-虚拟继电器14
#define	             RW_VR15  390	//VR15-虚拟继电器15
#define	             RW_VR16  391	//VR16-虚拟继电器16
#define	             RW_VR17  392	//VR17-虚拟继电器17
#define	             RW_VR18  393	//VR18-虚拟继电器18
#define	             RW_VR19  394	//VR19-虚拟继电器19
#define	             RW_VR20  395	//VR20-虚拟继电器20
#define	             RW_VR21  396	//VR21-虚拟继电器21
#define	             RW_VR22  397	//VR22-虚拟继电器22
#define	             RW_VR23  398	//VR23-虚拟继电器23
#define	             RW_VR24  399	//VR24-虚拟继电器24
#define	             RW_VR25  400	//VR25-虚拟继电器25
#define	             RW_VR26  401	//VR26-虚拟继电器26
#define	             RW_VR27  402	//VR27-虚拟继电器27
#define	             RW_VR28  403	//VR28-虚拟继电器28
#define	             RW_VR29  404	//VR29-虚拟继电器29
#define	             RW_VR30  405	//VR30-虚拟继电器30
#define	             RW_VR31  406	//VR31-虚拟继电器31
#define	             RW_VR32  407	//VR32-虚拟继电器32
#define	              RW_IN1  408	//IN1-开入继电器1
#define	              RW_IN2  409	//IN2-开入继电器2
#define	              RW_IN3  410	//IN3-开入继电器3
#define	              RW_IN4  411	//IN4-开入继电器4
#define	              RW_IN5  412	//IN5-开入继电器5
#define	              RW_IN6  413	//IN6-开入继电器6
#define	              RW_IN7  414	//IN7-开入继电器7
#define	              RW_IN8  415	//IN8-开入继电器8
#define	              RW_IN9  416	//IN9-开入继电器9
#define	             RW_IN10  417	//IN10-开入继电器10
#define	             RW_IN11  418	//IN11-开入继电器11
#define	             RW_IN12  419	//IN12-开入继电器12
#define	             RW_IN13  420	//IN13-开入继电器13
#define	             RW_IN14  421	//IN14-开入继电器14
#define	             RW_IN15  422	//IN15-开入继电器15
#define	             RW_IN16  423	//IN16-开入继电器16
#define	             RW_IN17  424	//IN17-开入继电器17
#define	             RW_IN18  425	//IN18-开入继电器18
#define	             RW_IN19  426	//IN19-开入继电器19
#define	             RW_IN20  427	//IN20-开入继电器20
#define	             RW_IN21  428	//IN21-开入继电器21
#define	             RW_IN22  429	//IN22-开入继电器22
#define	             RW_IN23  430	//IN23-开入继电器23
#define	             RW_IN24  431	//IN24-开入继电器24
#define	             RW_IN25  432	//IN25-开入继电器25
#define	             RW_IN26  433	//IN26-开入继电器26
#define	             RW_IN27  434	//IN27-开入继电器27
#define	             RW_IN28  435	//IN28-开入继电器28
#define	             RW_IN29  436	//IN29-开入继电器29
#define	             RW_IN30  437	//IN30-开入继电器30
#define	             RW_IN31  438	//IN31-开入继电器31
#define	             RW_IN32  439	//IN32-开入继电器32
#define	             RW_IN33  440	//IN33-开入继电器33
#define	             RW_IN34  441	//IN34-开入继电器34
#define	             RW_IN35  442	//IN35-开入继电器35
#define	             RW_IN36  443	//IN36-开入继电器36
#define	             RW_IN37  444	//IN37-开入继电器37
#define	             RW_IN38  445	//IN38-开入继电器38
#define	             RW_IN39  446	//IN39-开入继电器39
#define	             RW_IN40  447	//IN40-开入继电器40
#define	             RW_IN41  448	//IN41-开入继电器41
#define	             RW_IN42  449	//IN42-开入继电器42
#define	             RW_IN43  450	//IN43-开入继电器43
#define	             RW_IN44  451	//IN44-开入继电器44
#define	             RW_IN45  452	//IN45-开入继电器45
#define	             RW_IN46  453	//IN46-开入继电器46
#define	             RW_IN47  454	//IN47-开入继电器47
#define	             RW_IN48  455	//IN48-开入继电器48
#define	             RW_IN49  456	//IN49-开入继电器49
#define	             RW_IN50  457	//IN50-开入继电器50
#define	             RW_IN51  458	//IN51-开入继电器51
#define	             RW_IN52  459	//IN52-开入继电器52
#define	             RW_IN53  460	//IN53-开入继电器53
#define	             RW_IN54  461	//IN54-开入继电器54
#define	             RW_IN55  462	//IN55-开入继电器55
#define	             RW_IN56  463	//IN56-开入继电器56
#define	             RW_IN57  464	//IN57-开入继电器57
#define	             RW_IN58  465	//IN58-开入继电器58
#define	             RW_IN59  466	//IN59-开入继电器59
#define	             RW_IN60  467	//IN60-开入继电器60
#define	             RW_IN61  468	//IN61-开入继电器61
#define	             RW_IN62  469	//IN62-开入继电器62
#define	             RW_IN63  470	//IN63-开入继电器63
#define	             RW_IN64  471	//IN64-开入继电器64
#define	             RW_IN65  472	//IN65-开入继电器65
#define	             RW_IN66  473	//IN66-开入继电器66
#define	             RW_IN67  474	//IN67-开入继电器67
#define	             RW_IN68  475	//IN68-开入继电器68
#define	             RW_IN69  476	//IN69-信号复归
#define	             RW_IN70  477	//IN70-遥控1预置
#define	             RW_IN71  478	//IN71-遥控2预置
#define	             RW_IN72  479	//IN72-遥控3预置
#define	             RW_IN73  480	//IN73-遥控4预置
#define	             RW_IN74  481	//IN74-遥控5预置
#define	             RW_IN75  482	//IN75-遥控6预置
#define	             RW_IN76  483	//IN76-遥控7预置
#define	             RW_IN77  484	//IN77-遥控8预置
#define	             RW_IN78  485	//IN78-遥控9预置
#define	             RW_IN79  486	//IN79-遥控10预置
#define	             RW_IN80  487	//IN80-遥控11预置
#define	             RW_IN81  488	//IN81-遥控12预置
#define	             RW_IN82  489	//IN82-遥控13预置
#define	             RW_IN83  490	//IN83-遥控14预置
#define	           RW_INVIN1  491	//INVIN1-开入继电器84
#define	           RW_INVIN2  492	//INVIN2-开入继电器85
#define	           RW_INVIN3  493	//INVIN3-开入继电器86
#define	           RW_INVIN4  494	//INVIN4-开入继电器87
#define	           RW_INVIN5  495	//INVIN5-开入继电器88
#define	           RW_INVIN6  496	//INVIN6-开入继电器89
#define	           RW_INVIN7  497	//INVIN7-开入继电器90
#define	           RW_INVIN8  498	//INVIN8-开入继电器91
#define	           RW_INVIN9  499	//INVIN9-开入继电器92
#define	          RW_INVIN10  500	//INVIN10-开入继电器93
#define	          RW_INVIN11  501	//INVIN11-开入继电器94
#define	          RW_INVIN12  502	//INVIN12-开入继电器95
#define	          RW_INVIN13  503	//INVIN13-开入继电器96
#define	          RW_INVIN14  504	//INVIN14-开入继电器97
#define	          RW_INVIN15  505	//INVIN15-开入继电器98
#define	          RW_INVIN16  506	//INVIN16-开入继电器99
#define	             RW_OUT1  507	//OUT1-开出1继电器字
#define	             RW_OUT2  508	//OUT2-开出2继电器字
#define	             RW_OUT3  509	//OUT3-开出3继电器字
#define	             RW_OUT4  510	//OUT4-开出4继电器字
#define	             RW_OUT5  511	//OUT5-开出5继电器字
#define	             RW_OUT6  512	//OUT6-开出6继电器字
#define	             RW_OUT7  513	//OUT7-开出7继电器字
#define	             RW_OUT8  514	//OUT8-开出8继电器字
#define	             RW_OUT9  515	//OUT9-开出9继电器字
#define	            RW_OUT10  516	//OUT10-开出10继电器字
#define	            RW_OUT11  517	//OUT11-开出11继电器字
#define	            RW_OUT12  518	//OUT12-开出12继电器字
#define	            RW_OUT13  519	//OUT13-开出13继电器字
#define	            RW_OUT14  520	//OUT14-开出14继电器字
#define	            RW_OUT15  521	//OUT15-开出15继电器字
#define	            RW_OUT16  522	//OUT16-开出16继电器字
#define	            RW_OUT17  523	//OUT17-开出17继电器字
#define	            RW_OUT18  524	//OUT18-开出18继电器字
#define	            RW_OUT19  525	//OUT19-开出19继电器字
#define	            RW_OUT20  526	//OUT20-开出20继电器字
#define	            RW_OUT21  527	//OUT21-开出21继电器字
#define	            RW_OUT22  528	//OUT22-开出22继电器字
#define	            RW_OUT23  529	//OUT23-开出23继电器字
#define	            RW_OUT24  530	//OUT24-开出24继电器字
#define	            RW_OUT25  531	//OUT25-开出25继电器字
#define	            RW_OUT26  532	//OUT26-开出26继电器字
#define	            RW_OUT27  533	//OUT27-开出27继电器字
#define	            RW_OUT28  534	//OUT28-开出28继电器字
#define	            RW_OUT29  535	//OUT29-开出29继电器字
#define	            RW_OUT30  536	//OUT30-开出30继电器字
#define	            RW_OUT31  537	//OUT31-开出31继电器字
#define	            RW_OUT32  538	//OUT32-开出32继电器字
#define	            RW_OUT33  539	//OUT33-开出33继电器字
#define	            RW_OUT34  540	//OUT34-开出34继电器字
#define	            RW_OUT35  541	//OUT35-开出35继电器字
#define	            RW_OUT36  542	//OUT36-开出36继电器字
#define	            RW_OUT37  543	//OUT37-开出37继电器字
#define	            RW_OUT38  544	//OUT38-开出38继电器字
#define	            RW_OUT39  545	//OUT39-开出39继电器字
#define	            RW_OUT40  546	//OUT40-开出40继电器字
#define	            RW_OUT41  547	//OUT41-开出41继电器字
#define	            RW_OUT42  548	//OUT42-开出42继电器字
#define	            RW_OUT43  549	//OUT43-开出43继电器字
#define	            RW_OUT44  550	//OUT44-开出44继电器字
#define	            RW_OUT45  551	//OUT45-开出45继电器字
#define	            RW_OUT46  552	//OUT46-开出46继电器字
#define	            RW_OUT47  553	//OUT47-开出47继电器字
#define	            RW_OUT48  554	//OUT48-开出48继电器字
#define	            RW_SLBZT  555	//SLBZT-备自投软压板
#define	          RW_SLZBBZT  556	//SLZBBZT-主变备自投软压板
#define	          RW_SLSYBZT  557	//SLSYBZT-失压备自投软压板
#define	          RW_SLMAINT  558	//SLMAINT-检修压板
#define	           RW_SLBAK1  559	//SLBAK1-备用压板1
#define	           RW_SLBAK2  560	//SLBAK2-备用压板2
#define	           RW_SLBAK3  561	//SLBAK3-备用压板3
#define	           RW_SLBAK4  562	//SLBAK4-备用压板4
#define	             RW_FBZT  563	//FBZT-备自投功能配置继电器字
#define	           RW_FREBZT  564	//FREBZT-备自投重新投入功能配置继电器字
#define	           RW_FZBBZT  565	//FZBBZT-主变备自投配置继电器字
#define	           RW_FSYBZT  566	//FSYBZT-失压备自投配置继电器字
#define	            RW_FZLYX  567	//FZLYX-直列优先配置继电器字
#define	           RW_F74PT1  568	//F74PT1-PT1断线检查配置继电器字
#define	           RW_F74PT2  569	//F74PT2-PT2断线检查配置继电器字
#define	           RW_F74PT3  570	//F74PT3-PT3断线检查配置继电器字
#define	           RW_F74PT4  571	//F74PT4-PT4断线检查配置继电器字
#define	            RW_FBAK1  572	//FBAK1-备用配置1
#define	            RW_FBAK2  573	//FBAK2-备用配置2
#define	            RW_FBAK3  574	//FBAK3-备用配置3
#define	            RW_FBAK4  575	//FBAK4-备用配置4
#define	              RW_PW1  576	//PW1-脉冲继电器1
#define	              RW_PW2  577	//PW2-脉冲继电器2
#define	              RW_PW3  578	//PW3-脉冲继电器3
#define	              RW_PW4  579	//PW4-脉冲继电器4
#define	              RW_PW5  580	//PW5-脉冲继电器5
#define	              RW_PW6  581	//PW6-脉冲继电器6
#define	              RW_PW7  582	//PW7-脉冲继电器7
#define	              RW_PW8  583	//PW8-脉冲继电器8
#define	              RW_PW9  584	//PW9-脉冲继电器9
#define	             RW_PW10  585	//PW10-脉冲继电器10
#define	             RW_PW11  586	//PW11-脉冲继电器11
#define	             RW_PW12  587	//PW12-脉冲继电器12
#define	             RW_PW13  588	//PW13-脉冲继电器13
#define	             RW_PW14  589	//PW14-脉冲继电器14
#define	             RW_PW15  590	//PW15-脉冲继电器15
#define	             RW_PW16  591	//PW16-脉冲继电器16
#define	              RW_TO1  592	//TO1-传动继电器1
#define	              RW_TO2  593	//TO2-传动继电器2
#define	              RW_TO3  594	//TO3-传动继电器3
#define	              RW_TO4  595	//TO4-传动继电器4
#define	              RW_TO5  596	//TO5-传动继电器5
#define	              RW_TO6  597	//TO6-传动继电器6
#define	              RW_TO7  598	//TO7-传动继电器7
#define	              RW_TO8  599	//TO8-传动继电器8
#define	              RW_TO9  600	//TO9-传动继电器9
#define	             RW_TO10  601	//TO10-传动继电器10
#define	             RW_TO11  602	//TO11-传动继电器11
#define	             RW_TO12  603	//TO12-传动继电器12
#define	             RW_TO13  604	//TO13-传动继电器13
#define	             RW_TO14  605	//TO14-传动继电器14
#define	             RW_TO15  606	//TO15-传动继电器15
#define	             RW_TO16  607	//TO16-传动继电器16
#define	             RW_TO17  608	//TO17-传动继电器17
#define	             RW_TO18  609	//TO18-传动继电器18
#define	             RW_TO19  610	//TO19-传动继电器19
#define	             RW_TO20  611	//TO20-传动继电器20
#define	             RW_TO21  612	//TO21-传动继电器21
#define	             RW_TO22  613	//TO22-传动继电器22
#define	             RW_TO23  614	//TO23-传动继电器23
#define	             RW_TO24  615	//TO24-传动继电器24
#define	             RW_TO25  616	//TO25-传动继电器25
#define	             RW_TO26  617	//TO26-传动继电器26
#define	             RW_TO27  618	//TO27-传动继电器27
#define	             RW_TO28  619	//TO28-传动继电器28
#define	             RW_TO29  620	//TO29-传动继电器29
#define	             RW_TO30  621	//TO30-传动继电器30
#define	             RW_TO31  622	//TO31-传动继电器31
#define	             RW_TO32  623	//TO32-传动继电器32
#define	             RW_SIG1  624	//SIG1-信号继电器1
#define	             RW_SIG2  625	//SIG2-信号继电器2
#define	             RW_SIG3  626	//SIG3-信号继电器3
#define	             RW_SIG4  627	//SIG4-信号继电器4
#define	             RW_SIG5  628	//SIG5-信号继电器5
#define	             RW_SIG6  629	//SIG6-信号继电器6
#define	             RW_SIG7  630	//SIG7-信号继电器7
#define	             RW_SIG8  631	//SIG8-信号继电器8
#define	             RW_SIG9  632	//SIG9-信号继电器9
#define	            RW_SIG10  633	//SIG10-信号继电器10
#define	            RW_SIG11  634	//SIG11-信号继电器11
#define	            RW_SIG12  635	//SIG12-信号继电器12
#define	            RW_SIG13  636	//SIG13-信号继电器13
#define	            RW_SIG14  637	//SIG14-信号继电器14
#define	            RW_SIG15  638	//SIG15-信号继电器15
#define	            RW_SIG16  639	//SIG16-信号继电器16
#define	            RW_LED1G  640	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  641	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  642	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  643	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  644	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  645	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  646	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  647	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  648	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  649	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  650	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  651	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  652	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  653	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  654	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  655	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  656	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  657	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  658	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  659	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  660	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  661	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  662	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  663	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  664	//VIN1-开入继电器84
#define	             RW_VIN2  665	//VIN2-开入继电器85
#define	             RW_VIN3  666	//VIN3-开入继电器86
#define	             RW_VIN4  667	//VIN4-开入继电器87
#define	             RW_VIN5  668	//VIN5-开入继电器88
#define	             RW_VIN6  669	//VIN6-开入继电器89
#define	             RW_VIN7  670	//VIN7-开入继电器90
#define	             RW_VIN8  671	//VIN8-开入继电器91
#define	             RW_VIN9  672	//VIN9-开入继电器92
#define	            RW_VIN10  673	//VIN10-开入继电器93
#define	            RW_VIN11  674	//VIN11-开入继电器94
#define	            RW_VIN12  675	//VIN12-开入继电器95
#define	            RW_VIN13  676	//VIN13-开入继电器96
#define	            RW_VIN14  677	//VIN14-开入继电器97
#define	            RW_VIN15  678	//VIN15-开入继电器98
#define	            RW_VIN16  679	//VIN16-开入继电器99
#define	             RW_RCP1  680	//RCP1-遥控预置继电器1
#define	             RW_RCP2  681	//RCP2-遥控预置继电器2
#define	             RW_RCP3  682	//RCP3-遥控预置继电器3
#define	             RW_RCP4  683	//RCP4-遥控预置继电器4
#define	             RW_RCP5  684	//RCP5-遥控预置继电器5
#define	             RW_RCP6  685	//RCP6-遥控预置继电器6
#define	             RW_RCP7  686	//RCP7-遥控预置继电器7
#define	             RW_RCP8  687	//RCP8-遥控预置继电器8
#define	             RW_RCP9  688	//RCP9-遥控预置继电器9
#define	            RW_RCP10  689	//RCP10-遥控预置继电器10
#define	            RW_RCP11  690	//RCP11-遥控预置继电器11
#define	            RW_RCP12  691	//RCP12-遥控预置继电器12
#define	            RW_RCP13  692	//RCP13-遥控预置继电器13
#define	            RW_RCP14  693	//RCP14-遥控预置继电器14
#define	            RW_RCP15  694	//RCP15-遥控预置继电器15
#define	            RW_RCP16  695	//RCP16-遥控预置继电器16
#define	            RW_RCP17  696	//RCP17-遥控预置继电器17
#define	            RW_RCP18  697	//RCP18-遥控预置继电器18
#define	            RW_RCP19  698	//RCP19-遥控预置继电器19
#define	            RW_RCP20  699	//RCP20-遥控预置继电器20
#define	            RW_RCP21  700	//RCP21-遥控预置继电器21
#define	            RW_RCP22  701	//RCP22-遥控预置继电器22
#define	            RW_RCP23  702	//RCP23-遥控预置继电器23
#define	            RW_RCP24  703	//RCP24-遥控预置继电器24
#define	            RW_RCP25  704	//RCP25-遥控预置继电器25
#define	            RW_RCP26  705	//RCP26-遥控预置继电器26
#define	            RW_RCP27  706	//RCP27-遥控预置继电器27
#define	            RW_RCP28  707	//RCP28-遥控预置继电器28
#define	            RW_RCP29  708	//RCP29-遥控预置继电器29
#define	            RW_RCP30  709	//RCP30-遥控预置继电器30
#define	            RW_RCP31  710	//RCP31-遥控预置继电器31
#define	            RW_RCP32  711	//RCP32-遥控预置继电器32
#define	            RW_RCP33  712	//RCP33-遥控预置继电器33
#define	            RW_RCP34  713	//RCP34-遥控预置继电器34
#define	            RW_RCP35  714	//RCP35-遥控预置继电器35
#define	            RW_RCP36  715	//RCP36-遥控预置继电器36
#define	            RW_RCP37  716	//RCP37-遥控预置继电器37
#define	            RW_RCP38  717	//RCP38-遥控预置继电器38
#define	            RW_RCP39  718	//RCP39-遥控预置继电器39
#define	            RW_RCP40  719	//RCP40-遥控预置继电器40
#define	            RW_RCP41  720	//RCP41-遥控预置继电器41
#define	            RW_RCP42  721	//RCP42-遥控预置继电器42
#define	            RW_RCP43  722	//RCP43-遥控预置继电器43
#define	            RW_RCP44  723	//RCP44-遥控预置继电器44
#define	            RW_RCP45  724	//RCP45-遥控预置继电器45
#define	            RW_RCP46  725	//RCP46-遥控预置继电器46
#define	            RW_RCP47  726	//RCP47-遥控预置继电器47
#define	            RW_RCP48  727	//RCP48-遥控预置继电器48
#define	            RW_RCP49  728	//RCP49-遥控预置继电器49
#define	            RW_RCP50  729	//RCP50-遥控预置继电器50
#define	            RW_RCP51  730	//RCP51-遥控预置继电器51
#define	            RW_RCP52  731	//RCP52-遥控预置继电器52
#define	            RW_RCP53  732	//RCP53-遥控预置继电器53
#define	            RW_RCP54  733	//RCP54-遥控预置继电器54
#define	            RW_RCP55  734	//RCP55-遥控预置继电器55
#define	            RW_RCP56  735	//RCP56-遥控预置继电器56
#define	            RW_RCP57  736	//RCP57-遥控预置继电器57
#define	            RW_RCP58  737	//RCP58-遥控预置继电器58
#define	             RW_RCE1  738	//RCE1-遥控执行继电器1
#define	             RW_RCE2  739	//RCE2-遥控执行继电器2
#define	             RW_RCE3  740	//RCE3-遥控执行继电器3
#define	             RW_RCE4  741	//RCE4-遥控执行继电器4
#define	             RW_RCE5  742	//RCE5-遥控执行继电器5
#define	             RW_RCE6  743	//RCE6-遥控执行继电器6
#define	             RW_RCE7  744	//RCE7-遥控执行继电器7
#define	             RW_RCE8  745	//RCE8-遥控执行继电器8
#define	             RW_RCE9  746	//RCE9-遥控执行继电器9
#define	            RW_RCE10  747	//RCE10-遥控执行继电器10
#define	            RW_RCE11  748	//RCE11-遥控执行继电器11
#define	            RW_RCE12  749	//RCE12-遥控执行继电器12
#define	            RW_RCE13  750	//RCE13-遥控执行继电器13
#define	            RW_RCE14  751	//RCE14-遥控执行继电器14
#define	            RW_RCE15  752	//RCE15-遥控执行继电器15
#define	            RW_RCE16  753	//RCE16-遥控执行继电器16
#define	            RW_RCE17  754	//RCE17-遥控执行继电器17
#define	            RW_RCE18  755	//RCE18-遥控执行继电器18
#define	            RW_RCE19  756	//RCE19-遥控执行继电器19
#define	            RW_RCE20  757	//RCE20-遥控执行继电器20
#define	            RW_RCE21  758	//RCE21-遥控执行继电器21
#define	            RW_RCE22  759	//RCE22-遥控执行继电器22
#define	            RW_RCE23  760	//RCE23-遥控执行继电器23
#define	            RW_RCE24  761	//RCE24-遥控执行继电器24
#define	            RW_RCE25  762	//RCE25-遥控执行继电器25
#define	            RW_RCE26  763	//RCE26-遥控执行继电器26
#define	            RW_RCE27  764	//RCE27-遥控执行继电器27
#define	            RW_RCE28  765	//RCE28-遥控执行继电器28
#define	            RW_RCE29  766	//RCE29-遥控执行继电器29
#define	            RW_RCE30  767	//RCE30-遥控执行继电器30
#define	            RW_RCE31  768	//RCE31-遥控执行继电器31
#define	            RW_RCE32  769	//RCE32-遥控执行继电器32
#define	            RW_RCE33  770	//RCE33-遥控执行继电器33
#define	            RW_RCE34  771	//RCE34-遥控执行继电器34
#define	            RW_RCE35  772	//RCE35-遥控执行继电器35
#define	            RW_RCE36  773	//RCE36-遥控执行继电器36
#define	            RW_RCE37  774	//RCE37-遥控执行继电器37
#define	            RW_RCE38  775	//RCE38-遥控执行继电器38
#define	            RW_RCE39  776	//RCE39-遥控执行继电器39
#define	            RW_RCE40  777	//RCE40-遥控执行继电器40
#define	            RW_RCE41  778	//RCE41-遥控执行继电器41
#define	            RW_RCE42  779	//RCE42-遥控执行继电器42
#define	            RW_RCE43  780	//RCE43-遥控执行继电器43
#define	            RW_RCE44  781	//RCE44-遥控执行继电器44
#define	            RW_RCE45  782	//RCE45-遥控执行继电器45
#define	            RW_RCE46  783	//RCE46-遥控执行继电器46
#define	            RW_RCE47  784	//RCE47-遥控执行继电器47
#define	            RW_RCE48  785	//RCE48-遥控执行继电器48
#define	            RW_RCE49  786	//RCE49-遥控执行继电器49
#define	            RW_RCE50  787	//RCE50-遥控执行继电器50
#define	            RW_RCE51  788	//RCE51-遥控执行继电器51
#define	            RW_RCE52  789	//RCE52-遥控执行继电器52
#define	            RW_RCE53  790	//RCE53-遥控执行继电器53
#define	            RW_RCE54  791	//RCE54-遥控执行继电器54
#define	            RW_RCE55  792	//RCE55-遥控执行继电器55
#define	            RW_RCE56  793	//RCE56-遥控执行继电器56
#define	            RW_RCE57  794	//RCE57-遥控执行继电器57
#define	            RW_RCE58  795	//RCE58-遥控执行继电器58
#define	             RW_RCV1  796	//RCV1-遥控反校继电器1
#define	             RW_RCV2  797	//RCV2-遥控反校继电器2
#define	             RW_RCV3  798	//RCV3-遥控反校继电器3
#define	             RW_RCV4  799	//RCV4-遥控反校继电器4
#define	             RW_RCV5  800	//RCV5-遥控反校继电器5
#define	             RW_RCV6  801	//RCV6-遥控反校继电器6
#define	             RW_RCV7  802	//RCV7-遥控反校继电器7
#define	             RW_RCV8  803	//RCV8-遥控反校继电器8
#define	             RW_RCV9  804	//RCV9-遥控反校继电器9
#define	            RW_RCV10  805	//RCV10-遥控反校继电器10
#define	            RW_RCV11  806	//RCV11-遥控反校继电器11
#define	            RW_RCV12  807	//RCV12-遥控反校继电器12
#define	            RW_RCV13  808	//RCV13-遥控反校继电器13
#define	            RW_RCV14  809	//RCV14-遥控反校继电器14
#define	            RW_RCV15  810	//RCV15-遥控反校继电器15
#define	            RW_RCV16  811	//RCV16-遥控反校继电器16
#define	            RW_RCV17  812	//RCV17-遥控反校继电器17
#define	            RW_RCV18  813	//RCV18-遥控反校继电器18
#define	            RW_RCV19  814	//RCV19-遥控反校继电器19
#define	            RW_RCV20  815	//RCV20-遥控反校继电器20
#define	            RW_RCV21  816	//RCV21-遥控反校继电器21
#define	            RW_RCV22  817	//RCV22-遥控反校继电器22
#define	            RW_RCV23  818	//RCV23-遥控反校继电器23
#define	            RW_RCV24  819	//RCV24-遥控反校继电器24
#define	            RW_RCV25  820	//RCV25-遥控反校继电器25
#define	            RW_RCV26  821	//RCV26-遥控反校继电器26
#define	            RW_RCV27  822	//RCV27-遥控反校继电器27
#define	            RW_RCV28  823	//RCV28-遥控反校继电器28
#define	            RW_RCV29  824	//RCV29-遥控反校继电器29
#define	            RW_RCV30  825	//RCV30-遥控反校继电器30
#define	            RW_RCV31  826	//RCV31-遥控反校继电器31
#define	            RW_RCV32  827	//RCV32-遥控反校继电器32
#define	            RW_RCV33  828	//RCV33-遥控反校继电器33
#define	            RW_RCV34  829	//RCV34-遥控反校继电器34
#define	            RW_RCV35  830	//RCV35-遥控反校继电器35
#define	            RW_RCV36  831	//RCV36-遥控反校继电器36
#define	            RW_RCV37  832	//RCV37-遥控反校继电器37
#define	            RW_RCV38  833	//RCV38-遥控反校继电器38
#define	            RW_RCV39  834	//RCV39-遥控反校继电器39
#define	            RW_RCV40  835	//RCV40-遥控反校继电器40
#define	            RW_RCV41  836	//RCV41-遥控反校继电器41
#define	            RW_RCV42  837	//RCV42-遥控反校继电器42
#define	            RW_RCV43  838	//RCV43-遥控反校继电器43
#define	            RW_RCV44  839	//RCV44-遥控反校继电器44
#define	            RW_RCV45  840	//RCV45-遥控反校继电器45
#define	            RW_RCV46  841	//RCV46-遥控反校继电器46
#define	            RW_RCV47  842	//RCV47-遥控反校继电器47
#define	            RW_RCV48  843	//RCV48-遥控反校继电器48
#define	            RW_RCV49  844	//RCV49-遥控反校继电器49
#define	            RW_RCV50  845	//RCV50-遥控反校继电器50
#define	            RW_RCV51  846	//RCV51-遥控反校继电器51
#define	            RW_RCV52  847	//RCV52-遥控反校继电器52
#define	            RW_RCV53  848	//RCV53-遥控反校继电器53
#define	            RW_RCV54  849	//RCV54-遥控反校继电器54
#define	            RW_RCV55  850	//RCV55-遥控反校继电器55
#define	            RW_RCV56  851	//RCV56-遥控反校继电器56
#define	            RW_RCV57  852	//RCV57-遥控反校继电器57
#define	            RW_RCV58  853	//RCV58-遥控反校继电器58
#define	              RW_GE1  854	//GE1-大于等于继电器1
#define	              RW_GE2  855	//GE2-大于等于继电器2
#define	              RW_GE3  856	//GE3-大于等于继电器3
#define	              RW_GE4  857	//GE4-大于等于继电器4
#define	              RW_GE5  858	//GE5-大于等于继电器5
#define	              RW_GE6  859	//GE6-大于等于继电器6
#define	              RW_GE7  860	//GE7-大于等于继电器7
#define	              RW_GE8  861	//GE8-大于等于继电器8
#define	           RW_CONST1  862	//CONST1-常数时间继电器1
#define	           RW_CONST2  863	//CONST2-常数时间继电器2
#define	           RW_CONST3  864	//CONST3-常数时间继电器3
#define	           RW_CONST4  865	//CONST4-常数时间继电器4
#define	           RW_CONST5  866	//CONST5-常数时间继电器5
#define	           RW_CONST6  867	//CONST6-常数时间继电器6
#define	           RW_CONST7  868	//CONST7-常数时间继电器7
#define	           RW_CONST8  869	//CONST8-常数时间继电器8
#define	              RW_TX1  870	//TX1-发送链接继电器字1
#define	              RW_TX2  871	//TX2-发送链接继电器字2
#define	              RW_TX3  872	//TX3-发送链接继电器字3
#define	              RW_TX4  873	//TX4-发送链接继电器字4
#define	              RW_TX5  874	//TX5-发送链接继电器字5
#define	              RW_TX6  875	//TX6-发送链接继电器字6
#define	              RW_TX7  876	//TX7-发送链接继电器字7
#define	              RW_TX8  877	//TX8-发送链接继电器字8
#define	              RW_TX9  878	//TX9-发送链接继电器字9
#define	             RW_TX10  879	//TX10-发送链接继电器字10
#define	             RW_TX11  880	//TX11-发送链接继电器字11
#define	             RW_TX12  881	//TX12-发送链接继电器字12
#define	             RW_TX13  882	//TX13-发送链接继电器字13
#define	             RW_TX14  883	//TX14-发送链接继电器字14
#define	             RW_TX15  884	//TX15-发送链接继电器字15
#define	             RW_TX16  885	//TX16-发送链接继电器字16
#define	              RW_RX1  886	//RX1-接收链接继电器字1
#define	              RW_RX2  887	//RX2-接收链接继电器字2
#define	              RW_RX3  888	//RX3-接收链接继电器字3
#define	              RW_RX4  889	//RX4-接收链接继电器字4
#define	              RW_RX5  890	//RX5-接收链接继电器字5
#define	              RW_RX6  891	//RX6-接收链接继电器字6
#define	              RW_RX7  892	//RX7-接收链接继电器字7
#define	              RW_RX8  893	//RX8-接收链接继电器字8
#define	              RW_RX9  894	//RX9-接收链接继电器字9
#define	             RW_RX10  895	//RX10-接收链接继电器字10
#define	             RW_RX11  896	//RX11-接收链接继电器字11
#define	             RW_RX12  897	//RX12-接收链接继电器字12
#define	             RW_RX13  898	//RX13-接收链接继电器字13
#define	             RW_RX14  899	//RX14-接收链接继电器字14
#define	             RW_RX15  900	//RX15-接收链接继电器字15
#define	             RW_RX16  901	//RX16-接收链接继电器字16
#define	             RW_CNT1  902	//CNT1-计数继电器字1
#define	             RW_CNT2  903	//CNT2-计数继电器字2
#define	             RW_CNT3  904	//CNT3-计数继电器字3
#define	             RW_CNT4  905	//CNT4-计数继电器字4
#define	             RW_CNT5  906	//CNT5-计数继电器字5
#define	             RW_CNT6  907	//CNT6-计数继电器字6
#define	             RW_CNT7  908	//CNT7-计数继电器字7
#define	             RW_CNT8  909	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			910		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			49	

//中间继电器字
#define	MID_RELAY_WORD_START			51	
#define	MID_RELAY_WORD_NUM			135	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			186	
#define	KEEP_RELAY_WORD_NUM			85	

//时间继电器字
#define	TIME_RELAY_WORD_START			271	
#define	TIME_RELAY_WORD_NUM			103	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			374	
#define	TRIP_RELAY_WORD_NUM			2	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			376	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			408	
#define	INPUT_RELAY_WORD_NUM			99	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			507	
#define	OUTPUT_RELAY_WORD_NUM			48	

//压板继电器
#define	SW_RELAY_WORD_START			555	
#define	SW_RELAY_WORD_NUM			8	

//配置继电器
#define	CFG_RELAY_WORD_START			563	
#define	CFG_RELAY_WORD_NUM			13	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			576	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			592	
#define	CTRLTEST_RELAY_WORD_NUM			32	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			624	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			640	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			664	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			680	
#define	RCP_RELAY_WORD_NUM			58	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			738	
#define	RCE_RELAY_WORD_NUM			58	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			796	
#define	RCV_RELAY_WORD_NUM			58	

//大于等于继电器
#define	GE_RELAY_WORD_START			854	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			862	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			870	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			886	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			902	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			902	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			902	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			902	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			902	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			902	
#define	CNT_RELAY_WORD_NUM			8	


#endif