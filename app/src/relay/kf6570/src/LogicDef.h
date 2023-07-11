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
#define	             RW_59HA    2	//59HA-UHA有压继电器字
#define	             RW_27HA    3	//27HA-UHA无压继电器字
#define	             RW_59HB    4	//59HB-UHB有压继电器字
#define	             RW_27HB    5	//27HB-UHB无压继电器字
#define	             RW_59HC    6	//59HC-UHC有压继电器字
#define	             RW_27HC    7	//27HC-UHC无压继电器字
#define	              RW_59N    8	//59N-U0有压继电器字
#define	              RW_27N    9	//27N-U0无压继电器字
#define	             RW_59LA   10	//59LA-UHA有压继电器字
#define	             RW_27LA   11	//27LA-UHA无压继电器字
#define	             RW_59LB   12	//59LB-UHB有压继电器字
#define	             RW_27LB   13	//27LB-UHB无压继电器字
#define	             RW_59LC   14	//59LC-ULC有压继电器字
#define	             RW_27LC   15	//27LC-ULC无压继电器字
#define	             RW_50HA   16	//50HA-IHA有流继电器字
#define	             RW_37HA   17	//37HA-IHA无流继电器字
#define	             RW_50HB   18	//50HB-IHB有流继电器字
#define	             RW_37HB   19	//37HB-IHB无流继电器字
#define	             RW_50HC   20	//50HC-IHC有流继电器字
#define	             RW_37HC   21	//37HC-IHC无流继电器字
#define	             RW_50LA   22	//50LA-ILA有流继电器字
#define	             RW_37LA   23	//37LA-ILA无流继电器字
#define	             RW_50LB   24	//50LB-ILB有流继电器字
#define	             RW_37LB   25	//37LB-ILB无流继电器字
#define	             RW_50LC   26	//50LC-ILC有流继电器字
#define	             RW_37LC   27	//37LC-ILC无流继电器字
#define	             RW_50I1   28	//50I1-I1有流继电器字
#define	             RW_37I1   29	//37I1-I1无流继电器字
#define	             RW_50I2   30	//50I2-I2有流继电器字
#define	             RW_37I2   31	//37I2-I2无流继电器字
#define	          RW_ET11ERR   32	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   33	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   34	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   35	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   36	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   37	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   38	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   39	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   40	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   41	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   42	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   43	//CAN1ERR-CAN1连接状态继电器字
#define	          RW_CAN2ERR   44	//CAN2ERR-CAN2连接状态继电器字
#define	         RW_NULL1ERR   45	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR   46	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR   47	//NULL3ERR-空3连接状态继电器字
#define	        RW_MAINTLINK   48	//MAINTLINK-维护状态继电器字
#define	              RW_REC   49	//REC-录波继电器字
#define	               RW_M1   50	//M1-中间继电器1
#define	               RW_M2   51	//M2-中间继电器2
#define	               RW_M3   52	//M3-中间继电器3
#define	               RW_M4   53	//M4-中间继电器4
#define	               RW_M5   54	//M5-中间继电器5
#define	               RW_M6   55	//M6-中间继电器6
#define	               RW_M7   56	//M7-中间继电器7
#define	               RW_M8   57	//M8-中间继电器8
#define	               RW_M9   58	//M9-中间继电器9
#define	              RW_M10   59	//M10-中间继电器10
#define	              RW_M11   60	//M11-中间继电器11
#define	              RW_M12   61	//M12-中间继电器12
#define	              RW_M13   62	//M13-中间继电器13
#define	              RW_M14   63	//M14-中间继电器14
#define	              RW_M15   64	//M15-中间继电器15
#define	              RW_M16   65	//M16-中间继电器16
#define	              RW_M17   66	//M17-中间继电器17
#define	              RW_M18   67	//M18-中间继电器18
#define	              RW_M19   68	//M19-中间继电器19
#define	              RW_M20   69	//M20-中间继电器20
#define	              RW_M21   70	//M21-中间继电器21
#define	              RW_M22   71	//M22-中间继电器22
#define	              RW_M23   72	//M23-中间继电器23
#define	              RW_M24   73	//M24-中间继电器24
#define	              RW_M25   74	//M25-中间继电器25
#define	              RW_M26   75	//M26-中间继电器26
#define	              RW_M27   76	//M27-中间继电器27
#define	              RW_M28   77	//M28-中间继电器28
#define	              RW_M29   78	//M29-中间继电器29
#define	              RW_M30   79	//M30-中间继电器30
#define	              RW_M31   80	//M31-中间继电器31
#define	              RW_M32   81	//M32-中间继电器32
#define	              RW_M33   82	//M33-中间继电器33
#define	              RW_M34   83	//M34-中间继电器34
#define	              RW_M35   84	//M35-中间继电器35
#define	              RW_M36   85	//M36-中间继电器36
#define	              RW_M37   86	//M37-中间继电器37
#define	              RW_M38   87	//M38-中间继电器38
#define	              RW_M39   88	//M39-中间继电器39
#define	              RW_M40   89	//M40-中间继电器40
#define	              RW_M41   90	//M41-中间继电器41
#define	              RW_M42   91	//M42-中间继电器42
#define	              RW_M43   92	//M43-中间继电器43
#define	              RW_M44   93	//M44-中间继电器44
#define	              RW_M45   94	//M45-中间继电器45
#define	              RW_M46   95	//M46-中间继电器46
#define	              RW_M47   96	//M47-中间继电器47
#define	              RW_M48   97	//M48-中间继电器48
#define	              RW_M49   98	//M49-中间继电器49
#define	              RW_M50   99	//M50-中间继电器50
#define	              RW_M51  100	//M51-中间继电器51
#define	              RW_M52  101	//M52-中间继电器52
#define	              RW_M53  102	//M53-中间继电器53
#define	              RW_M54  103	//M54-中间继电器54
#define	              RW_M55  104	//M55-中间继电器55
#define	              RW_M56  105	//M56-中间继电器56
#define	              RW_M57  106	//M57-中间继电器57
#define	              RW_M58  107	//M58-中间继电器58
#define	              RW_M59  108	//M59-中间继电器59
#define	              RW_M60  109	//M60-中间继电器60
#define	              RW_M61  110	//M61-中间继电器61
#define	              RW_M62  111	//M62-中间继电器62
#define	              RW_M63  112	//M63-中间继电器63
#define	              RW_M64  113	//M64-中间继电器64
#define	              RW_M65  114	//M65-中间继电器65
#define	              RW_M66  115	//M66-中间继电器66
#define	              RW_M67  116	//M67-中间继电器67
#define	              RW_M68  117	//M68-中间继电器68
#define	              RW_M69  118	//M69-中间继电器69
#define	              RW_M70  119	//M70-中间继电器70
#define	              RW_M71  120	//M71-中间继电器71
#define	              RW_M72  121	//M72-中间继电器72
#define	              RW_M73  122	//M73-中间继电器73
#define	              RW_M74  123	//M74-中间继电器74
#define	              RW_M75  124	//M75-中间继电器75
#define	              RW_M76  125	//M76-中间继电器76
#define	              RW_M77  126	//M77-中间继电器77
#define	              RW_M78  127	//M78-中间继电器78
#define	              RW_M79  128	//M79-中间继电器79
#define	              RW_M80  129	//M80-中间继电器80
#define	              RW_M81  130	//M81-中间继电器81
#define	              RW_M82  131	//M82-中间继电器82
#define	              RW_M83  132	//M83-中间继电器83
#define	              RW_M84  133	//M84-中间继电器84
#define	              RW_M85  134	//M85-中间继电器85
#define	              RW_M86  135	//M86-中间继电器86
#define	              RW_M87  136	//M87-中间继电器87
#define	              RW_M88  137	//M88-中间继电器88
#define	              RW_M89  138	//M89-中间继电器89
#define	              RW_M90  139	//M90-中间继电器90
#define	              RW_M91  140	//M91-中间继电器91
#define	              RW_M92  141	//M92-中间继电器92
#define	              RW_M93  142	//M93-中间继电器93
#define	              RW_M94  143	//M94-中间继电器94
#define	              RW_M95  144	//M95-中间继电器95
#define	              RW_M96  145	//M96-中间继电器96
#define	              RW_M97  146	//M97-中间继电器97
#define	              RW_M98  147	//M98-中间继电器98
#define	              RW_M99  148	//M99-中间继电器99
#define	             RW_M100  149	//M100-中间继电器100
#define	             RW_M101  150	//M101-中间继电器101
#define	             RW_M102  151	//M102-中间继电器102
#define	             RW_M103  152	//M103-中间继电器103
#define	             RW_M104  153	//M104-中间继电器104
#define	             RW_M105  154	//M105-中间继电器105
#define	             RW_M106  155	//M106-中间继电器106
#define	             RW_M107  156	//M107-中间继电器107
#define	             RW_M108  157	//M108-中间继电器108
#define	             RW_M109  158	//M109-中间继电器109
#define	             RW_M110  159	//M110-中间继电器110
#define	             RW_M111  160	//M111-中间继电器111
#define	             RW_M112  161	//M112-中间继电器112
#define	             RW_M113  162	//M113-中间继电器113
#define	             RW_M114  163	//M114-中间继电器114
#define	             RW_M115  164	//M115-中间继电器115
#define	             RW_M116  165	//M116-中间继电器116
#define	             RW_M117  166	//M117-中间继电器117
#define	             RW_M118  167	//M118-中间继电器118
#define	             RW_M119  168	//M119-中间继电器119
#define	             RW_M120  169	//M120-中间继电器120
#define	             RW_M121  170	//M121-中间继电器121
#define	             RW_M122  171	//M122-中间继电器122
#define	             RW_M123  172	//M123-中间继电器123
#define	             RW_M124  173	//M124-中间继电器124
#define	             RW_M125  174	//M125-中间继电器125
#define	             RW_M126  175	//M126-中间继电器126
#define	             RW_M127  176	//M127-中间继电器127
#define	             RW_M128  177	//M128-中间继电器128
#define	               RW_H1  178	//H1-自保持中间继电器字1
#define	               RW_H2  179	//H2-自保持中间继电器字2
#define	               RW_H3  180	//H3-自保持中间继电器字3
#define	               RW_H4  181	//H4-自保持中间继电器字4
#define	               RW_H5  182	//H5-自保持中间继电器字5
#define	               RW_H6  183	//H6-自保持中间继电器字6
#define	               RW_H7  184	//H7-自保持中间继电器字7
#define	               RW_H8  185	//H8-自保持中间继电器字8
#define	               RW_H9  186	//H9-自保持中间继电器字9
#define	              RW_H10  187	//H10-自保持中间继电器字10
#define	              RW_H11  188	//H11-自保持中间继电器字11
#define	              RW_H12  189	//H12-自保持中间继电器字12
#define	              RW_H13  190	//H13-自保持中间继电器字13
#define	              RW_H14  191	//H14-自保持中间继电器字14
#define	              RW_H15  192	//H15-自保持中间继电器字15
#define	              RW_H16  193	//H16-自保持中间继电器字16
#define	              RW_H17  194	//H17-自保持中间继电器字17
#define	              RW_H18  195	//H18-自保持中间继电器字18
#define	              RW_H19  196	//H19-自保持中间继电器字19
#define	              RW_H20  197	//H20-自保持中间继电器字20
#define	              RW_H21  198	//H21-自保持中间继电器字21
#define	              RW_H22  199	//H22-自保持中间继电器字22
#define	              RW_H23  200	//H23-自保持中间继电器字23
#define	              RW_H24  201	//H24-自保持中间继电器字24
#define	              RW_H25  202	//H25-自保持中间继电器字25
#define	              RW_H26  203	//H26-自保持中间继电器字26
#define	              RW_H27  204	//H27-自保持中间继电器字27
#define	              RW_H28  205	//H28-自保持中间继电器字28
#define	              RW_H29  206	//H29-自保持中间继电器字29
#define	              RW_H30  207	//H30-自保持中间继电器字30
#define	              RW_H31  208	//H31-自保持中间继电器字31
#define	              RW_H32  209	//H32-自保持中间继电器字32
#define	              RW_H33  210	//H33-自保持中间继电器字33
#define	              RW_H34  211	//H34-自保持中间继电器字34
#define	              RW_H35  212	//H35-自保持中间继电器字35
#define	              RW_H36  213	//H36-自保持中间继电器字36
#define	              RW_H37  214	//H37-自保持中间继电器字37
#define	              RW_H38  215	//H38-自保持中间继电器字38
#define	              RW_H39  216	//H39-自保持中间继电器字39
#define	              RW_H40  217	//H40-自保持中间继电器字40
#define	              RW_H41  218	//H41-自保持中间继电器字41
#define	              RW_H42  219	//H42-自保持中间继电器字42
#define	              RW_H43  220	//H43-自保持中间继电器字43
#define	              RW_H44  221	//H44-自保持中间继电器字44
#define	              RW_H45  222	//H45-自保持中间继电器字45
#define	              RW_H46  223	//H46-自保持中间继电器字46
#define	              RW_H47  224	//H47-自保持中间继电器字47
#define	              RW_H48  225	//H48-自保持中间继电器字48
#define	              RW_H49  226	//H49-自保持中间继电器字49
#define	              RW_H50  227	//H50-自保持中间继电器字50
#define	              RW_H51  228	//H51-自保持中间继电器字51
#define	              RW_H52  229	//H52-自保持中间继电器字52
#define	              RW_H53  230	//H53-自保持中间继电器字53
#define	              RW_H54  231	//H54-自保持中间继电器字54
#define	              RW_H55  232	//H55-自保持中间继电器字55
#define	              RW_H56  233	//H56-自保持中间继电器字56
#define	              RW_H57  234	//H57-自保持中间继电器字57
#define	              RW_H58  235	//H58-自保持中间继电器字58
#define	              RW_H59  236	//H59-自保持中间继电器字59
#define	              RW_H60  237	//H60-自保持中间继电器字60
#define	              RW_H61  238	//H61-自保持中间继电器字61
#define	              RW_H62  239	//H62-自保持中间继电器字62
#define	              RW_H63  240	//H63-自保持中间继电器字63
#define	              RW_H64  241	//H64-自保持中间继电器字64
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
#define	              RW_TR1  322	//TR1-跳闸继电器一
#define	              RW_VR1  323	//VR1-调试继电器
#define	              RW_VR2  324	//VR2-虚拟继电器2
#define	              RW_VR3  325	//VR3-虚拟继电器3
#define	              RW_VR4  326	//VR4-虚拟继电器4
#define	              RW_VR5  327	//VR5-虚拟继电器5
#define	              RW_VR6  328	//VR6-虚拟继电器6
#define	              RW_VR7  329	//VR7-虚拟继电器7
#define	              RW_VR8  330	//VR8-虚拟继电器8
#define	              RW_VR9  331	//VR9-虚拟继电器9
#define	             RW_VR10  332	//VR10-虚拟继电器10
#define	             RW_VR11  333	//VR11-虚拟继电器11
#define	             RW_VR12  334	//VR12-虚拟继电器12
#define	             RW_VR13  335	//VR13-虚拟继电器13
#define	             RW_VR14  336	//VR14-虚拟继电器14
#define	             RW_VR15  337	//VR15-虚拟继电器15
#define	             RW_VR16  338	//VR16-虚拟继电器16
#define	             RW_VR17  339	//VR17-虚拟继电器17
#define	             RW_VR18  340	//VR18-虚拟继电器18
#define	             RW_VR19  341	//VR19-虚拟继电器19
#define	             RW_VR20  342	//VR20-虚拟继电器20
#define	             RW_VR21  343	//VR21-虚拟继电器21
#define	             RW_VR22  344	//VR22-虚拟继电器22
#define	             RW_VR23  345	//VR23-虚拟继电器23
#define	             RW_VR24  346	//VR24-虚拟继电器24
#define	             RW_VR25  347	//VR25-虚拟继电器25
#define	             RW_VR26  348	//VR26-虚拟继电器26
#define	             RW_VR27  349	//VR27-虚拟继电器27
#define	             RW_VR28  350	//VR28-虚拟继电器28
#define	             RW_VR29  351	//VR29-虚拟继电器29
#define	             RW_VR30  352	//VR30-虚拟继电器30
#define	             RW_VR31  353	//VR31-虚拟继电器31
#define	             RW_VR32  354	//VR32-虚拟继电器32
#define	              RW_IN1  355	//IN1-开入继电器1
#define	              RW_IN2  356	//IN2-开入继电器2
#define	              RW_IN3  357	//IN3-开入继电器3
#define	              RW_IN4  358	//IN4-开入继电器4
#define	              RW_IN5  359	//IN5-开入继电器5
#define	              RW_IN6  360	//IN6-开入继电器6
#define	              RW_IN7  361	//IN7-开入继电器7
#define	              RW_IN8  362	//IN8-开入继电器8
#define	              RW_IN9  363	//IN9-开入继电器9
#define	             RW_IN10  364	//IN10-开入继电器10
#define	             RW_IN11  365	//IN11-开入继电器11
#define	             RW_IN12  366	//IN12-开入继电器12
#define	             RW_IN13  367	//IN13-开入继电器13
#define	             RW_IN14  368	//IN14-开入继电器14
#define	             RW_IN15  369	//IN15-开入继电器15
#define	             RW_IN16  370	//IN16-开入继电器16
#define	             RW_IN17  371	//IN17-开入继电器17
#define	             RW_IN18  372	//IN18-开入继电器18
#define	             RW_IN19  373	//IN19-开入继电器19
#define	             RW_IN20  374	//IN20-开入继电器20
#define	             RW_IN21  375	//IN21-开入继电器21
#define	             RW_IN22  376	//IN22-开入继电器22
#define	             RW_IN23  377	//IN23-开入继电器23
#define	             RW_IN24  378	//IN24-开入继电器24
#define	             RW_IN25  379	//IN25-开入继电器25
#define	             RW_IN26  380	//IN26-开入继电器26
#define	             RW_IN27  381	//IN27-开入继电器27
#define	             RW_IN28  382	//IN28-开入继电器28
#define	             RW_IN29  383	//IN29-开入继电器29
#define	             RW_IN30  384	//IN30-开入继电器30
#define	             RW_IN31  385	//IN31-开入继电器31
#define	             RW_IN32  386	//IN32-开入继电器32
#define	             RW_IN33  387	//IN33-开入继电器33
#define	             RW_IN34  388	//IN34-开入继电器34
#define	             RW_IN35  389	//IN35-开入继电器35
#define	             RW_IN36  390	//IN36-开入继电器36
#define	             RW_IN37  391	//IN37-开入继电器37
#define	             RW_IN38  392	//IN38-开入继电器38
#define	             RW_IN39  393	//IN39-开入继电器39
#define	             RW_IN40  394	//IN40-开入继电器40
#define	             RW_IN41  395	//IN41-开入继电器41
#define	             RW_IN42  396	//IN42-开入继电器42
#define	             RW_IN43  397	//IN43-开入继电器43
#define	             RW_IN44  398	//IN44-开入继电器44
#define	             RW_IN45  399	//IN45-开入继电器45
#define	             RW_IN46  400	//IN46-开入继电器46
#define	             RW_IN47  401	//IN47-开入继电器47
#define	             RW_IN48  402	//IN48-开入继电器48
#define	             RW_IN49  403	//IN49-开入继电器49
#define	             RW_IN50  404	//IN50-开入继电器50
#define	             RW_IN51  405	//IN51-开入继电器51
#define	             RW_IN52  406	//IN52-开入继电器52
#define	             RW_IN53  407	//IN53-开入继电器53
#define	             RW_IN54  408	//IN54-开入继电器54
#define	             RW_IN55  409	//IN55-开入继电器55
#define	             RW_IN56  410	//IN56-开入继电器56
#define	             RW_IN57  411	//IN57-开入继电器57
#define	             RW_IN58  412	//IN58-开入继电器58
#define	             RW_IN59  413	//IN59-开入继电器59
#define	             RW_IN60  414	//IN60-开入继电器60
#define	             RW_IN61  415	//IN61-开入继电器61
#define	             RW_IN62  416	//IN62-开入继电器62
#define	             RW_IN63  417	//IN63-开入继电器63
#define	             RW_IN64  418	//IN64-开入继电器64
#define	             RW_IN65  419	//IN65-开入继电器65
#define	             RW_IN66  420	//IN66-开入继电器66
#define	             RW_IN67  421	//IN67-开入继电器67
#define	             RW_IN68  422	//IN68-开入继电器68
#define	             RW_IN69  423	//IN69-信号复归
#define	             RW_IN70  424	//IN70-遥控1预置
#define	             RW_IN71  425	//IN71-遥控2预置
#define	             RW_IN72  426	//IN72-遥控3预置
#define	             RW_IN73  427	//IN73-遥控4预置
#define	             RW_IN74  428	//IN74-遥控5预置
#define	             RW_IN75  429	//IN75-遥控6预置
#define	             RW_IN76  430	//IN76-遥控7预置
#define	             RW_IN77  431	//IN77-遥控8预置
#define	             RW_IN78  432	//IN78-遥控9预置
#define	             RW_IN79  433	//IN79-遥控10预置
#define	             RW_IN80  434	//IN80-遥控11预置
#define	             RW_IN81  435	//IN81-遥控12预置
#define	             RW_IN82  436	//IN82-遥控13预置
#define	             RW_IN83  437	//IN83-遥控14预置
#define	           RW_INVIN1  438	//INVIN1-开入继电器84
#define	           RW_INVIN2  439	//INVIN2-开入继电器85
#define	           RW_INVIN3  440	//INVIN3-开入继电器86
#define	           RW_INVIN4  441	//INVIN4-开入继电器87
#define	           RW_INVIN5  442	//INVIN5-开入继电器88
#define	           RW_INVIN6  443	//INVIN6-开入继电器89
#define	           RW_INVIN7  444	//INVIN7-开入继电器90
#define	           RW_INVIN8  445	//INVIN8-开入继电器91
#define	           RW_INVIN9  446	//INVIN9-开入继电器92
#define	          RW_INVIN10  447	//INVIN10-开入继电器93
#define	          RW_INVIN11  448	//INVIN11-开入继电器94
#define	          RW_INVIN12  449	//INVIN12-开入继电器95
#define	          RW_INVIN13  450	//INVIN13-开入继电器96
#define	          RW_INVIN14  451	//INVIN14-开入继电器97
#define	          RW_INVIN15  452	//INVIN15-开入继电器98
#define	          RW_INVIN16  453	//INVIN16-开入继电器99
#define	             RW_OUT1  454	//OUT1-开出1继电器字
#define	             RW_OUT2  455	//OUT2-开出2继电器字
#define	             RW_OUT3  456	//OUT3-开出3继电器字
#define	             RW_OUT4  457	//OUT4-开出4继电器字
#define	             RW_OUT5  458	//OUT5-开出5继电器字
#define	             RW_OUT6  459	//OUT6-开出6继电器字
#define	             RW_OUT7  460	//OUT7-开出7继电器字
#define	             RW_OUT8  461	//OUT8-开出8继电器字
#define	             RW_OUT9  462	//OUT9-开出9继电器字
#define	            RW_OUT10  463	//OUT10-开出10继电器字
#define	            RW_OUT11  464	//OUT11-开出11继电器字
#define	            RW_OUT12  465	//OUT12-开出12继电器字
#define	            RW_OUT13  466	//OUT13-开出13继电器字
#define	            RW_OUT14  467	//OUT14-开出14继电器字
#define	            RW_OUT15  468	//OUT15-开出15继电器字
#define	            RW_OUT16  469	//OUT16-开出16继电器字
#define	            RW_OUT17  470	//OUT17-开出17继电器字
#define	            RW_OUT18  471	//OUT18-开出18继电器字
#define	            RW_OUT19  472	//OUT19-开出19继电器字
#define	            RW_OUT20  473	//OUT20-开出20继电器字
#define	            RW_OUT21  474	//OUT21-开出21继电器字
#define	            RW_OUT22  475	//OUT22-开出22继电器字
#define	            RW_OUT23  476	//OUT23-开出23继电器字
#define	            RW_OUT24  477	//OUT24-开出24继电器字
#define	            RW_OUT25  478	//OUT25-开出25继电器字
#define	            RW_OUT26  479	//OUT26-开出26继电器字
#define	            RW_OUT27  480	//OUT27-开出27继电器字
#define	            RW_OUT28  481	//OUT28-开出28继电器字
#define	            RW_OUT29  482	//OUT29-开出29继电器字
#define	            RW_OUT30  483	//OUT30-开出30继电器字
#define	            RW_OUT31  484	//OUT31-开出31继电器字
#define	            RW_OUT32  485	//OUT32-开出32继电器字
#define	            RW_OUT33  486	//OUT33-开出33继电器字
#define	            RW_OUT34  487	//OUT34-开出34继电器字
#define	            RW_OUT35  488	//OUT35-开出35继电器字
#define	            RW_OUT36  489	//OUT36-开出36继电器字
#define	            RW_OUT37  490	//OUT37-开出37继电器字
#define	            RW_OUT38  491	//OUT38-开出38继电器字
#define	            RW_OUT39  492	//OUT39-开出39继电器字
#define	            RW_OUT40  493	//OUT40-开出40继电器字
#define	            RW_OUT41  494	//OUT41-开出41继电器字
#define	            RW_OUT42  495	//OUT42-开出42继电器字
#define	            RW_OUT43  496	//OUT43-开出43继电器字
#define	            RW_OUT44  497	//OUT44-开出44继电器字
#define	            RW_OUT45  498	//OUT45-开出45继电器字
#define	            RW_OUT46  499	//OUT46-开出46继电器字
#define	            RW_OUT47  500	//OUT47-开出47继电器字
#define	            RW_OUT48  501	//OUT48-开出48继电器字
#define	          RW_SLMAINT  502	//SLMAINT-检修压板继电器字
#define	           RW_SLBAK1  503	//SLBAK1-备用压板1
#define	           RW_SLBAK2  504	//SLBAK2-备用压板2
#define	           RW_SLBAK3  505	//SLBAK3-备用压板3
#define	           RW_SLBAK4  506	//SLBAK4-备用压板4
#define	          RW_FMODSET  507	//FMODSET-定值修改功能
#define	            RW_FBAK1  508	//FBAK1-备用配置1
#define	            RW_FBAK2  509	//FBAK2-备用配置2
#define	            RW_FBAK3  510	//FBAK3-备用配置3
#define	            RW_FBAK4  511	//FBAK4-备用配置4
#define	              RW_PW1  512	//PW1-脉冲继电器1
#define	              RW_PW2  513	//PW2-脉冲继电器2
#define	              RW_PW3  514	//PW3-脉冲继电器3
#define	              RW_PW4  515	//PW4-脉冲继电器4
#define	              RW_PW5  516	//PW5-脉冲继电器5
#define	              RW_PW6  517	//PW6-脉冲继电器6
#define	              RW_PW7  518	//PW7-脉冲继电器7
#define	              RW_PW8  519	//PW8-脉冲继电器8
#define	              RW_PW9  520	//PW9-脉冲继电器9
#define	             RW_PW10  521	//PW10-脉冲继电器10
#define	             RW_PW11  522	//PW11-脉冲继电器11
#define	             RW_PW12  523	//PW12-脉冲继电器12
#define	             RW_PW13  524	//PW13-脉冲继电器13
#define	             RW_PW14  525	//PW14-脉冲继电器14
#define	             RW_PW15  526	//PW15-脉冲继电器15
#define	             RW_PW16  527	//PW16-脉冲继电器16
#define	              RW_TO1  528	//TO1-传动继电器1
#define	              RW_TO2  529	//TO2-传动继电器2
#define	              RW_TO3  530	//TO3-传动继电器3
#define	              RW_TO4  531	//TO4-传动继电器4
#define	              RW_TO5  532	//TO5-传动继电器5
#define	              RW_TO6  533	//TO6-传动继电器6
#define	              RW_TO7  534	//TO7-传动继电器7
#define	              RW_TO8  535	//TO8-传动继电器8
#define	              RW_TO9  536	//TO9-传动继电器9
#define	             RW_TO10  537	//TO10-传动继电器10
#define	             RW_TO11  538	//TO11-传动继电器11
#define	             RW_TO12  539	//TO12-传动继电器12
#define	             RW_TO13  540	//TO13-传动继电器13
#define	             RW_TO14  541	//TO14-传动继电器14
#define	             RW_TO15  542	//TO15-传动继电器15
#define	             RW_TO16  543	//TO16-传动继电器16
#define	             RW_TO17  544	//TO17-传动继电器17
#define	             RW_TO18  545	//TO18-传动继电器18
#define	             RW_TO19  546	//TO19-传动继电器19
#define	             RW_TO20  547	//TO20-传动继电器20
#define	             RW_TO21  548	//TO21-传动继电器21
#define	             RW_TO22  549	//TO22-传动继电器22
#define	             RW_TO23  550	//TO23-传动继电器23
#define	             RW_TO24  551	//TO24-传动继电器24
#define	             RW_TO25  552	//TO25-传动继电器25
#define	             RW_TO26  553	//TO26-传动继电器26
#define	             RW_TO27  554	//TO27-传动继电器27
#define	             RW_TO28  555	//TO28-传动继电器28
#define	             RW_TO29  556	//TO29-传动继电器29
#define	             RW_TO30  557	//TO30-传动继电器30
#define	             RW_TO31  558	//TO31-传动继电器31
#define	             RW_TO32  559	//TO32-传动继电器32
#define	             RW_SIG1  560	//SIG1-信号继电器1
#define	             RW_SIG2  561	//SIG2-信号继电器2
#define	             RW_SIG3  562	//SIG3-信号继电器3
#define	             RW_SIG4  563	//SIG4-信号继电器4
#define	             RW_SIG5  564	//SIG5-信号继电器5
#define	             RW_SIG6  565	//SIG6-信号继电器6
#define	             RW_SIG7  566	//SIG7-信号继电器7
#define	             RW_SIG8  567	//SIG8-信号继电器8
#define	            RW_LED1G  568	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  569	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  570	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  571	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  572	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  573	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  574	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  575	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  576	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  577	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  578	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  579	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  580	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  581	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  582	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  583	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  584	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  585	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  586	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  587	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  588	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  589	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  590	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  591	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  592	//VIN1-开入继电器84
#define	             RW_VIN2  593	//VIN2-开入继电器85
#define	             RW_VIN3  594	//VIN3-开入继电器86
#define	             RW_VIN4  595	//VIN4-开入继电器87
#define	             RW_VIN5  596	//VIN5-开入继电器88
#define	             RW_VIN6  597	//VIN6-开入继电器89
#define	             RW_VIN7  598	//VIN7-开入继电器90
#define	             RW_VIN8  599	//VIN8-开入继电器91
#define	             RW_VIN9  600	//VIN9-开入继电器92
#define	            RW_VIN10  601	//VIN10-开入继电器93
#define	            RW_VIN11  602	//VIN11-开入继电器94
#define	            RW_VIN12  603	//VIN12-开入继电器95
#define	            RW_VIN13  604	//VIN13-开入继电器96
#define	            RW_VIN14  605	//VIN14-开入继电器97
#define	            RW_VIN15  606	//VIN15-开入继电器98
#define	            RW_VIN16  607	//VIN16-开入继电器99
#define	             RW_RCP1  608	//RCP1-遥控预置继电器1
#define	             RW_RCP2  609	//RCP2-遥控预置继电器2
#define	             RW_RCP3  610	//RCP3-遥控预置继电器3
#define	             RW_RCP4  611	//RCP4-遥控预置继电器4
#define	             RW_RCP5  612	//RCP5-遥控预置继电器5
#define	             RW_RCP6  613	//RCP6-遥控预置继电器6
#define	             RW_RCP7  614	//RCP7-遥控预置继电器7
#define	             RW_RCP8  615	//RCP8-遥控预置继电器8
#define	             RW_RCP9  616	//RCP9-遥控预置继电器9
#define	            RW_RCP10  617	//RCP10-遥控预置继电器10
#define	            RW_RCP11  618	//RCP11-遥控预置继电器11
#define	            RW_RCP12  619	//RCP12-遥控预置继电器12
#define	            RW_RCP13  620	//RCP13-遥控预置继电器13
#define	            RW_RCP14  621	//RCP14-遥控预置继电器14
#define	            RW_RCP15  622	//RCP15-遥控预置继电器15
#define	            RW_RCP16  623	//RCP16-遥控预置继电器16
#define	            RW_RCP17  624	//RCP17-遥控预置继电器17
#define	            RW_RCP18  625	//RCP18-遥控预置继电器18
#define	            RW_RCP19  626	//RCP19-遥控预置继电器19
#define	            RW_RCP20  627	//RCP20-遥控预置继电器20
#define	            RW_RCP21  628	//RCP21-遥控预置继电器21
#define	            RW_RCP22  629	//RCP22-遥控预置继电器22
#define	            RW_RCP23  630	//RCP23-遥控预置继电器23
#define	            RW_RCP24  631	//RCP24-遥控预置继电器24
#define	            RW_RCP25  632	//RCP25-遥控预置继电器25
#define	            RW_RCP26  633	//RCP26-遥控预置继电器26
#define	            RW_RCP27  634	//RCP27-遥控预置继电器27
#define	            RW_RCP28  635	//RCP28-遥控预置继电器28
#define	            RW_RCP29  636	//RCP29-遥控预置继电器29
#define	            RW_RCP30  637	//RCP30-遥控预置继电器30
#define	            RW_RCP31  638	//RCP31-遥控预置继电器31
#define	            RW_RCP32  639	//RCP32-遥控预置继电器32
#define	            RW_RCP33  640	//RCP33-遥控预置继电器33
#define	            RW_RCP34  641	//RCP34-遥控预置继电器34
#define	            RW_RCP35  642	//RCP35-遥控预置继电器35
#define	            RW_RCP36  643	//RCP36-遥控预置继电器36
#define	            RW_RCP37  644	//RCP37-遥控预置继电器37
#define	            RW_RCP38  645	//RCP38-遥控预置继电器38
#define	            RW_RCP39  646	//RCP39-遥控预置继电器39
#define	            RW_RCP40  647	//RCP40-遥控预置继电器40
#define	            RW_RCP41  648	//RCP41-遥控预置继电器41
#define	            RW_RCP42  649	//RCP42-遥控预置继电器42
#define	            RW_RCP43  650	//RCP43-遥控预置继电器43
#define	            RW_RCP44  651	//RCP44-遥控预置继电器44
#define	            RW_RCP45  652	//RCP45-遥控预置继电器45
#define	            RW_RCP46  653	//RCP46-遥控预置继电器46
#define	            RW_RCP47  654	//RCP47-遥控预置继电器47
#define	            RW_RCP48  655	//RCP48-遥控预置继电器48
#define	            RW_RCP49  656	//RCP49-遥控预置继电器49
#define	            RW_RCP50  657	//RCP50-遥控预置继电器50
#define	            RW_RCP51  658	//RCP51-遥控预置继电器51
#define	            RW_RCP52  659	//RCP52-遥控预置继电器52
#define	            RW_RCP53  660	//RCP53-遥控预置继电器53
#define	            RW_RCP54  661	//RCP54-遥控预置继电器54
#define	            RW_RCP55  662	//RCP55-遥控预置继电器55
#define	            RW_RCP56  663	//RCP56-遥控预置继电器56
#define	             RW_RCE1  664	//RCE1-遥控执行继电器1
#define	             RW_RCE2  665	//RCE2-遥控执行继电器2
#define	             RW_RCE3  666	//RCE3-遥控执行继电器3
#define	             RW_RCE4  667	//RCE4-遥控执行继电器4
#define	             RW_RCE5  668	//RCE5-遥控执行继电器5
#define	             RW_RCE6  669	//RCE6-遥控执行继电器6
#define	             RW_RCE7  670	//RCE7-遥控执行继电器7
#define	             RW_RCE8  671	//RCE8-遥控执行继电器8
#define	             RW_RCE9  672	//RCE9-遥控执行继电器9
#define	            RW_RCE10  673	//RCE10-遥控执行继电器10
#define	            RW_RCE11  674	//RCE11-遥控执行继电器11
#define	            RW_RCE12  675	//RCE12-遥控执行继电器12
#define	            RW_RCE13  676	//RCE13-遥控执行继电器13
#define	            RW_RCE14  677	//RCE14-遥控执行继电器14
#define	            RW_RCE15  678	//RCE15-遥控执行继电器15
#define	            RW_RCE16  679	//RCE16-遥控执行继电器16
#define	            RW_RCE17  680	//RCE17-遥控执行继电器17
#define	            RW_RCE18  681	//RCE18-遥控执行继电器18
#define	            RW_RCE19  682	//RCE19-遥控执行继电器19
#define	            RW_RCE20  683	//RCE20-遥控执行继电器20
#define	            RW_RCE21  684	//RCE21-遥控执行继电器21
#define	            RW_RCE22  685	//RCE22-遥控执行继电器22
#define	            RW_RCE23  686	//RCE23-遥控执行继电器23
#define	            RW_RCE24  687	//RCE24-遥控执行继电器24
#define	            RW_RCE25  688	//RCE25-遥控执行继电器25
#define	            RW_RCE26  689	//RCE26-遥控执行继电器26
#define	            RW_RCE27  690	//RCE27-遥控执行继电器27
#define	            RW_RCE28  691	//RCE28-遥控执行继电器28
#define	            RW_RCE29  692	//RCE29-遥控执行继电器29
#define	            RW_RCE30  693	//RCE30-遥控执行继电器30
#define	            RW_RCE31  694	//RCE31-遥控执行继电器31
#define	            RW_RCE32  695	//RCE32-遥控执行继电器32
#define	            RW_RCE33  696	//RCE33-遥控执行继电器33
#define	            RW_RCE34  697	//RCE34-遥控执行继电器34
#define	            RW_RCE35  698	//RCE35-遥控执行继电器35
#define	            RW_RCE36  699	//RCE36-遥控执行继电器36
#define	            RW_RCE37  700	//RCE37-遥控执行继电器37
#define	            RW_RCE38  701	//RCE38-遥控执行继电器38
#define	            RW_RCE39  702	//RCE39-遥控执行继电器39
#define	            RW_RCE40  703	//RCE40-遥控执行继电器40
#define	            RW_RCE41  704	//RCE41-遥控执行继电器41
#define	            RW_RCE42  705	//RCE42-遥控执行继电器42
#define	            RW_RCE43  706	//RCE43-遥控执行继电器43
#define	            RW_RCE44  707	//RCE44-遥控执行继电器44
#define	            RW_RCE45  708	//RCE45-遥控执行继电器45
#define	            RW_RCE46  709	//RCE46-遥控执行继电器46
#define	            RW_RCE47  710	//RCE47-遥控执行继电器47
#define	            RW_RCE48  711	//RCE48-遥控执行继电器48
#define	            RW_RCE49  712	//RCE49-遥控执行继电器49
#define	            RW_RCE50  713	//RCE50-遥控执行继电器50
#define	            RW_RCE51  714	//RCE51-遥控执行继电器51
#define	            RW_RCE52  715	//RCE52-遥控执行继电器52
#define	            RW_RCE53  716	//RCE53-遥控执行继电器53
#define	            RW_RCE54  717	//RCE54-遥控执行继电器54
#define	            RW_RCE55  718	//RCE55-遥控执行继电器55
#define	            RW_RCE56  719	//RCE56-遥控执行继电器56
#define	             RW_RCV1  720	//RCV1-遥控反校继电器1
#define	             RW_RCV2  721	//RCV2-遥控反校继电器2
#define	             RW_RCV3  722	//RCV3-遥控反校继电器3
#define	             RW_RCV4  723	//RCV4-遥控反校继电器4
#define	             RW_RCV5  724	//RCV5-遥控反校继电器5
#define	             RW_RCV6  725	//RCV6-遥控反校继电器6
#define	             RW_RCV7  726	//RCV7-遥控反校继电器7
#define	             RW_RCV8  727	//RCV8-遥控反校继电器8
#define	             RW_RCV9  728	//RCV9-遥控反校继电器9
#define	            RW_RCV10  729	//RCV10-遥控反校继电器10
#define	            RW_RCV11  730	//RCV11-遥控反校继电器11
#define	            RW_RCV12  731	//RCV12-遥控反校继电器12
#define	            RW_RCV13  732	//RCV13-遥控反校继电器13
#define	            RW_RCV14  733	//RCV14-遥控反校继电器14
#define	            RW_RCV15  734	//RCV15-遥控反校继电器15
#define	            RW_RCV16  735	//RCV16-遥控反校继电器16
#define	            RW_RCV17  736	//RCV17-遥控反校继电器17
#define	            RW_RCV18  737	//RCV18-遥控反校继电器18
#define	            RW_RCV19  738	//RCV19-遥控反校继电器19
#define	            RW_RCV20  739	//RCV20-遥控反校继电器20
#define	            RW_RCV21  740	//RCV21-遥控反校继电器21
#define	            RW_RCV22  741	//RCV22-遥控反校继电器22
#define	            RW_RCV23  742	//RCV23-遥控反校继电器23
#define	            RW_RCV24  743	//RCV24-遥控反校继电器24
#define	            RW_RCV25  744	//RCV25-遥控反校继电器25
#define	            RW_RCV26  745	//RCV26-遥控反校继电器26
#define	            RW_RCV27  746	//RCV27-遥控反校继电器27
#define	            RW_RCV28  747	//RCV28-遥控反校继电器28
#define	            RW_RCV29  748	//RCV29-遥控反校继电器29
#define	            RW_RCV30  749	//RCV30-遥控反校继电器30
#define	            RW_RCV31  750	//RCV31-遥控反校继电器31
#define	            RW_RCV32  751	//RCV32-遥控反校继电器32
#define	            RW_RCV33  752	//RCV33-遥控反校继电器33
#define	            RW_RCV34  753	//RCV34-遥控反校继电器34
#define	            RW_RCV35  754	//RCV35-遥控反校继电器35
#define	            RW_RCV36  755	//RCV36-遥控反校继电器36
#define	            RW_RCV37  756	//RCV37-遥控反校继电器37
#define	            RW_RCV38  757	//RCV38-遥控反校继电器38
#define	            RW_RCV39  758	//RCV39-遥控反校继电器39
#define	            RW_RCV40  759	//RCV40-遥控反校继电器40
#define	            RW_RCV41  760	//RCV41-遥控反校继电器41
#define	            RW_RCV42  761	//RCV42-遥控反校继电器42
#define	            RW_RCV43  762	//RCV43-遥控反校继电器43
#define	            RW_RCV44  763	//RCV44-遥控反校继电器44
#define	            RW_RCV45  764	//RCV45-遥控反校继电器45
#define	            RW_RCV46  765	//RCV46-遥控反校继电器46
#define	            RW_RCV47  766	//RCV47-遥控反校继电器47
#define	            RW_RCV48  767	//RCV48-遥控反校继电器48
#define	            RW_RCV49  768	//RCV49-遥控反校继电器49
#define	            RW_RCV50  769	//RCV50-遥控反校继电器50
#define	            RW_RCV51  770	//RCV51-遥控反校继电器51
#define	            RW_RCV52  771	//RCV52-遥控反校继电器52
#define	            RW_RCV53  772	//RCV53-遥控反校继电器53
#define	            RW_RCV54  773	//RCV54-遥控反校继电器54
#define	            RW_RCV55  774	//RCV55-遥控反校继电器55
#define	            RW_RCV56  775	//RCV56-遥控反校继电器56
#define	              RW_GE1  776	//GE1-大于等于继电器1
#define	              RW_GE2  777	//GE2-大于等于继电器2
#define	              RW_GE3  778	//GE3-大于等于继电器3
#define	              RW_GE4  779	//GE4-大于等于继电器4
#define	              RW_GE5  780	//GE5-大于等于继电器5
#define	              RW_GE6  781	//GE6-大于等于继电器6
#define	              RW_GE7  782	//GE7-大于等于继电器7
#define	              RW_GE8  783	//GE8-大于等于继电器8
#define	              RW_CT1  784	//CT1-常数时间继电器1
#define	              RW_CT2  785	//CT2-常数时间继电器2
#define	              RW_CT3  786	//CT3-常数时间继电器3
#define	              RW_CT4  787	//CT4-常数时间继电器4
#define	              RW_CT5  788	//CT5-常数时间继电器5
#define	              RW_CT6  789	//CT6-常数时间继电器6
#define	              RW_CT7  790	//CT7-常数时间继电器7
#define	              RW_CT8  791	//CT8-常数时间继电器8
#define	              RW_TX1  792	//TX1-发送链接继电器字1
#define	              RW_TX2  793	//TX2-发送链接继电器字2
#define	              RW_TX3  794	//TX3-发送链接继电器字3
#define	              RW_TX4  795	//TX4-发送链接继电器字4
#define	              RW_TX5  796	//TX5-发送链接继电器字5
#define	              RW_TX6  797	//TX6-发送链接继电器字6
#define	              RW_TX7  798	//TX7-发送链接继电器字7
#define	              RW_TX8  799	//TX8-发送链接继电器字7
#define	              RW_TX9  800	//TX9-发送链接继电器字9
#define	             RW_TX10  801	//TX10-发送链接继电器字10
#define	             RW_TX11  802	//TX11-发送链接继电器字11
#define	             RW_TX12  803	//TX12-发送链接继电器字12
#define	             RW_TX13  804	//TX13-发送链接继电器字13
#define	             RW_TX14  805	//TX14-发送链接继电器字14
#define	             RW_TX15  806	//TX15-发送链接继电器字15
#define	             RW_TX16  807	//TX16-发送链接继电器字16
#define	              RW_RX1  808	//RX1-接收链接继电器字1
#define	              RW_RX2  809	//RX2-接收链接继电器字2
#define	              RW_RX3  810	//RX3-接收链接继电器字3
#define	              RW_RX4  811	//RX4-接收链接继电器字4
#define	              RW_RX5  812	//RX5-接收链接继电器字5
#define	              RW_RX6  813	//RX6-接收链接继电器字6
#define	              RW_RX7  814	//RX7-接收链接继电器字7
#define	              RW_RX8  815	//RX8-接收链接继电器字8
#define	              RW_RX9  816	//RX9-接收链接继电器字9
#define	             RW_RX10  817	//RX10-接收链接继电器字10
#define	             RW_RX11  818	//RX11-接收链接继电器字11
#define	             RW_RX12  819	//RX12-接收链接继电器字12
#define	             RW_RX13  820	//RX13-接收链接继电器字13
#define	             RW_RX14  821	//RX14-接收链接继电器字14
#define	             RW_RX15  822	//RX15-接收链接继电器字15
#define	             RW_RX16  823	//RX16-接收链接继电器字16
#define	             RW_CNT1  824	//CNT1-计数继电器字1
#define	             RW_CNT2  825	//CNT2-计数继电器字2
#define	             RW_CNT3  826	//CNT3-计数继电器字3
#define	             RW_CNT4  827	//CNT4-计数继电器字4
#define	             RW_CNT5  828	//CNT5-计数继电器字5
#define	             RW_CNT6  829	//CNT6-计数继电器字6
#define	             RW_CNT7  830	//CNT7-计数继电器字7
#define	             RW_CNT8  831	//CNT8-计数继电器字7
#define	RELAY_WORD_NUM			832		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			47	

//中间继电器字
#define	MID_RELAY_WORD_START			49	
#define	MID_RELAY_WORD_NUM			129	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			178	
#define	KEEP_RELAY_WORD_NUM			64	

//时间继电器字
#define	TIME_RELAY_WORD_START			242	
#define	TIME_RELAY_WORD_NUM			80	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			322	
#define	TRIP_RELAY_WORD_NUM			1	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			323	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			355	
#define	INPUT_RELAY_WORD_NUM			99	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			454	
#define	OUTPUT_RELAY_WORD_NUM			48	

//压板继电器
#define	SW_RELAY_WORD_START			502	
#define	SW_RELAY_WORD_NUM			5	

//配置继电器
#define	CFG_RELAY_WORD_START			507	
#define	CFG_RELAY_WORD_NUM			5	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			512	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			528	
#define	CTRLTEST_RELAY_WORD_NUM			32	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			560	
#define	SIGNAL_RELAY_WORD_NUM			8	

//LED继电器
#define	LED_RELAY_WORD_START			568	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			592	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			608	
#define	RCP_RELAY_WORD_NUM			56	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			664	
#define	RCE_RELAY_WORD_NUM			56	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			720	
#define	RCV_RELAY_WORD_NUM			56	

//大于等于继电器
#define	GE_RELAY_WORD_START			776	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			784	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			792	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			808	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			824	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			824	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			824	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			824	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			824	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			824	
#define	CNT_RELAY_WORD_NUM			8	


#endif