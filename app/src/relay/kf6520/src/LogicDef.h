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
#define	           RW_PI21_1    2	//PI21_1-距离I段入段继电器字
#define	           RW_TR21_1    3	//TR21_1-距离I段动作继电器字
#define	           RW_PI21_2    4	//PI21_2-距离II段入段继电器字
#define	           RW_TR21_2    5	//TR21_2-距离II段动作继电器字
#define	        RW_TR21_2_94    6	//TR21_2_94-距离II段加速动作继电器字
#define	           RW_PI21_3    7	//PI21_3-距离III段入段继电器字
#define	           RW_TR21_3    8	//TR21_3-距离III段动作继电器字
#define	        RW_TR21_3_94    9	//TR21_3_94-距离III段加速动作继电器字
#define	           RW_PI21_4   10	//PI21_4-距离IV段入段继电器字
#define	           RW_TR21_4   11	//TR21_4-距离IV段动作继电器字
#define	        RW_TR21_4_94   12	//TR21_4_94-距离IV段加速动作继电器字
#define	           RW_PI50_0   13	//PI50_0-速断入段继电器字
#define	           RW_TR50_0   14	//TR50_0-速断动作继电器字
#define	           RW_PI50_1   15	//PI50_1-过流I段入段继电器字
#define	           RW_TR50_1   16	//TR50_1-过流I段动作继电器字
#define	        RW_TR50_1_94   17	//TR50_1_94-过流I段加速动作继电器字
#define	           RW_PI50_2   18	//PI50_2-过流II段入段继电器字
#define	           RW_TR50_2   19	//TR50_2-过流II段动作继电器字
#define	        RW_TR50_2_94   20	//TR50_2_94-过流II段加速动作继电器字
#define	           RW_PI50_3   21	//PI50_3-过流III段入段继电器字
#define	           RW_TR50_3   22	//TR50_3-过流III段动作继电器字
#define	        RW_TR50_3_94   23	//TR50_3_94-过流III段加速动作继电器字
#define	             RW_PI27   24	//PI27-失压保护入段继电器字
#define	             RW_TR27   25	//TR27-失压保护动作继电器字
#define	             RW_AR27   26	//AR27-失压保护告警继电器字
#define	             RW_PI51   27	//PI51-反时限入段继电器字
#define	             RW_TR51   28	//TR51-反时限动作继电器字
#define	            RW_PI50D   29	//PI50D-增量保护入段继电器字
#define	            RW_TR50D   30	//TR50D-增量保护动作继电器字
#define	            RW_ARVT1   31	//ARVT1-PT1断线告警继电器字
#define	            RW_PIVT1   32	//PIVT1-PT1断线入段继电器字
#define	            RW_ARVT2   33	//ARVT2-PT2断线告警继电器字
#define	            RW_PIVT2   34	//PIVT2-PT2断线入段继电器字
#define	            RW_ARCT1   35	//ARCT1-CT1断线告警继电器字
#define	            RW_PICT1   36	//PICT1-CT1断线入段继电器字
#define	            RW_ARCT2   37	//ARCT2-CT2断线告警继电器字
#define	            RW_PICT2   38	//PICT2-CT2断线入段继电器字
#define	             RW_LKH2   39	//LKH2-二次谐波闭锁继电器字
#define	              RW_32F   40	//32F-功率方向正向继电器字
#define	              RW_32R   41	//32R-功率方向反向继电器字
#define	             RW_50IP   42	//50IP-IP有流继电器字
#define	             RW_37IP   43	//37IP-IP无流继电器字
#define	             RW_50IT   44	//50IT-IT有流继电器字
#define	             RW_37IT   45	//37IT-IT无流继电器字
#define	             RW_50IF   46	//50IF-IF有流继电器字
#define	             RW_37IF   47	//37IF-IF无流继电器字
#define	             RW_59UP   48	//59UP-UP有压继电器字
#define	             RW_27UP   49	//27UP-UP无压继电器字
#define	             RW_59U1   50	//59U1-U1有压继电器字
#define	             RW_27U1   51	//27U1-U1无压继电器字
#define	             RW_59U2   52	//59U2-U2有压继电器字
#define	             RW_27U2   53	//27U2-U2无压继电器字
#define	             RW_59U3   54	//59U3-U3有压继电器字
#define	             RW_27U3   55	//27U3-U3无压继电器字
#define	             RW_59U4   56	//59U4-U4有压继电器字
#define	             RW_27U4   57	//27U4-U4无压继电器字
#define	            RW_LPC51   58	//LPC51-反时限保护循环控制继电器字
#define	            RW_79SH0   59	//79SH0-重合闸动作状态继电器字一
#define	            RW_79SH1   60	//79SH1-重合闸动作状态继电器字二
#define	             RW_79TO   61	//79TO-重合闸延时到逻辑继电器字
#define	            RW_79CYS   62	//79CYS-重合闸周期状态继电器字
#define	            RW_79RSS   63	//79RSS-重合闸复归状态继电器字
#define	            RW_79LOS   64	//79LOS-重合闸闭锁状态继电器字
#define	             RW_79CL   65	//79CL-重合闸动作逻辑继电器字
#define	           RW_BTMODE   66	//BTMODE-直供方式供电
#define	          RW_AT1MODE   67	//AT1MODE-AT1方式供电(普通AT)
#define	          RW_AT2MODE   68	//AT2MODE-AT2方式供电(客专AT)
#define	           RW_PICKUP   69	//PICKUP-模值突变量启动继电器字
#define	          RW_LPC21_1   70	//LPC21_1-距离I段保护循环控制继电器字
#define	          RW_LPC21_2   71	//LPC21_2-距离II段保护循环控制继电器字
#define	          RW_LPC21_3   72	//LPC21_3-距离III段保护循环控制继电器字
#define	          RW_LPC21_4   73	//LPC21_4-距离IV段保护循环控制继电器字
#define	          RW_LPC50_0   74	//LPC50_0-速断保护循环控制继电器字
#define	          RW_LPC50_1   75	//LPC50_1-过流I段保护循环控制继电器字
#define	          RW_LPC50_2   76	//LPC50_2-过流II段保护循环控制继电器字
#define	          RW_LPC50_3   77	//LPC50_3-过流III段保护循环控制继电器字
#define	            RW_LPC27   78	//LPC27-失压保护循环控制继电器字
#define	           RW_LPC50D   79	//LPC50D-增量保护循环控制继电器字
#define	          RW_ET11ERR   80	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   81	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   82	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   83	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   84	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   85	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   86	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   87	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   88	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   89	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   90	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   91	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   92	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   93	//NULL1ERR-空连接1状态继电器字
#define	         RW_NULL2ERR   94	//NULL2ERR-空连接2状态继电器字
#define	         RW_NULL3ERR   95	//NULL3ERR-空连接3状态继电器字
#define	        RW_MAINTLINK   96	//MAINTLINK-维护状态继电器字
#define	               RW_M1   97	//M1-中间继电器字M1
#define	               RW_M2   98	//M2-中间继电器字M2
#define	               RW_M3   99	//M3-中间继电器字M3
#define	               RW_M4  100	//M4-中间继电器字M4
#define	               RW_M5  101	//M5-中间继电器字M5
#define	               RW_M6  102	//M6-中间继电器字M6
#define	               RW_M7  103	//M7-中间继电器字M7
#define	               RW_M8  104	//M8-中间继电器字M8
#define	               RW_M9  105	//M9-中间继电器字M9
#define	              RW_M10  106	//M10-中间继电器字M10
#define	              RW_M11  107	//M11-中间继电器字M11
#define	              RW_M12  108	//M12-中间继电器字M12
#define	              RW_M13  109	//M13-中间继电器字M13
#define	              RW_M14  110	//M14-中间继电器字M14
#define	              RW_M15  111	//M15-中间继电器字M15
#define	              RW_M16  112	//M16-中间继电器字M16
#define	              RW_M17  113	//M17-中间继电器字M17
#define	              RW_M18  114	//M18-中间继电器字M18
#define	              RW_M19  115	//M19-中间继电器字M19
#define	              RW_M20  116	//M20-中间继电器字M20
#define	              RW_M21  117	//M21-中间继电器字M21
#define	              RW_M22  118	//M22-中间继电器字M22
#define	              RW_M23  119	//M23-中间继电器字M23
#define	              RW_M24  120	//M24-中间继电器字M24
#define	              RW_M25  121	//M25-中间继电器字M25
#define	              RW_M26  122	//M26-中间继电器字M26
#define	              RW_M27  123	//M27-中间继电器字M27
#define	              RW_M28  124	//M28-中间继电器字M28
#define	              RW_M29  125	//M29-中间继电器字M29
#define	              RW_M30  126	//M30-中间继电器字M30
#define	              RW_M31  127	//M31-中间继电器字M31
#define	              RW_M32  128	//M32-中间继电器字M32
#define	              RW_M33  129	//M33-中间继电器字M33
#define	              RW_M34  130	//M34-中间继电器字M34
#define	              RW_M35  131	//M35-中间继电器字M35
#define	              RW_M36  132	//M36-中间继电器字M36
#define	              RW_M37  133	//M37-中间继电器字M37
#define	              RW_M38  134	//M38-中间继电器字M38
#define	              RW_M39  135	//M39-中间继电器字M39
#define	              RW_M40  136	//M40-中间继电器字M40
#define	              RW_M41  137	//M41-中间继电器字M41
#define	              RW_M42  138	//M42-中间继电器字M42
#define	              RW_M43  139	//M43-中间继电器字M43
#define	              RW_M44  140	//M44-中间继电器字M44
#define	              RW_M45  141	//M45-中间继电器字M45
#define	              RW_M46  142	//M46-中间继电器字M46
#define	              RW_M47  143	//M47-中间继电器字M47
#define	              RW_M48  144	//M48-中间继电器字M48
#define	              RW_M49  145	//M49-中间继电器字M49
#define	              RW_M50  146	//M50-中间继电器字M50
#define	              RW_M51  147	//M51-中间继电器字M51
#define	              RW_M52  148	//M52-中间继电器字M52
#define	              RW_M53  149	//M53-中间继电器字M53
#define	              RW_M54  150	//M54-中间继电器字M54
#define	              RW_M55  151	//M55-中间继电器字M55
#define	              RW_M56  152	//M56-中间继电器字M56
#define	              RW_M57  153	//M57-中间继电器字M57
#define	              RW_M58  154	//M58-中间继电器字M58
#define	              RW_M59  155	//M59-中间继电器字M59
#define	              RW_M60  156	//M60-中间继电器字M60
#define	              RW_M61  157	//M61-中间继电器字M61
#define	              RW_M62  158	//M62-中间继电器字M62
#define	              RW_M63  159	//M63-中间继电器字M63
#define	              RW_M64  160	//M64-中间继电器字M64
#define	         RW_LKPI21_1  161	//LKPI21_1-距离I段入段闭锁继电器字
#define	         RW_LKPI21_2  162	//LKPI21_2-距离II段入段闭锁继电器字
#define	         RW_LKPI21_3  163	//LKPI21_3-距离III段入段闭锁继电器字
#define	         RW_LKPI21_4  164	//LKPI21_4-距离IV段入段闭锁继电器字
#define	         RW_LKPI50_0  165	//LKPI50_0-速断入段闭锁继电器字
#define	        RW_LKRST50_0  166	//LKRST50_0-速断返回闭锁继电器字
#define	         RW_LKPI50_1  167	//LKPI50_1-过流I段入段闭锁继电器字
#define	        RW_LKRST50_1  168	//LKRST50_1-过流I段返回闭锁继电器字
#define	         RW_LKPI50_2  169	//LKPI50_2-过流II段入段闭锁继电器字
#define	        RW_LKRST50_2  170	//LKRST50_2-过流II段返回闭锁继电器字
#define	         RW_LKPI50_3  171	//LKPI50_3-过流III段入段闭锁继电器字
#define	        RW_LKRST50_3  172	//LKRST50_3-过流III段返回闭锁继电器字
#define	           RW_LKPI27  173	//LKPI27-失压保护入段闭锁继电器字
#define	          RW_LKRST27  174	//LKRST27-失压保护返回闭锁继电器字
#define	           RW_LKPI51  175	//LKPI51-反时限入段闭锁继电器字
#define	          RW_LKRST51  176	//LKRST51-反时限返回闭锁继电器字
#define	          RW_LKPI50D  177	//LKPI50D-增量入段闭锁继电器字
#define	         RW_LKRST50D  178	//LKRST50D-增量返回闭锁继电器字
#define	            RW_79PIS  179	//79PIS-重合闸启动监视逻辑继电器字
#define	            RW_79CLC  180	//79CLC-重合闸动作命令撤销继电器字
#define	             RW_79LK  181	//79LK-重合闸闭锁逻辑继电器字
#define	            RW_79CLS  182	//79CLS-重合闸动作监视逻辑继电器字
#define	           RW_79TRCK  183	//79TRCK-重合闸启动条件判断继电器字
#define	              RW_F79  184	//F79-重合闸配置继电器字
#define	             RW_69IN  185	//69IN-远方当地中间继电器字
#define	              RW_52A  186	//52A-合位
#define	              RW_52B  187	//52B-分位
#define	              RW_REC  188	//REC-录波继电器字
#define	               RW_H1  189	//H1-自保持中间继电器字1
#define	               RW_H2  190	//H2-自保持中间继电器字2
#define	               RW_H3  191	//H3-自保持中间继电器字3
#define	               RW_H4  192	//H4-自保持中间继电器字4
#define	               RW_H5  193	//H5-自保持中间继电器字5
#define	               RW_H6  194	//H6-自保持中间继电器字6
#define	               RW_H7  195	//H7-自保持中间继电器字7
#define	               RW_H8  196	//H8-自保持中间继电器字8
#define	               RW_H9  197	//H9-自保持中间继电器字9
#define	              RW_H10  198	//H10-自保持中间继电器字10
#define	              RW_H11  199	//H11-自保持中间继电器字11
#define	              RW_H12  200	//H12-自保持中间继电器字12
#define	              RW_H13  201	//H13-自保持中间继电器字13
#define	              RW_H14  202	//H14-自保持中间继电器字14
#define	              RW_H15  203	//H15-自保持中间继电器字15
#define	              RW_H16  204	//H16-自保持中间继电器字16
#define	              RW_H17  205	//H17-自保持中间继电器字17
#define	              RW_H18  206	//H18-自保持中间继电器字18
#define	              RW_H19  207	//H19-自保持中间继电器字19
#define	              RW_H20  208	//H20-自保持中间继电器字20
#define	              RW_H21  209	//H21-自保持中间继电器字21
#define	              RW_H22  210	//H22-自保持中间继电器字22
#define	              RW_H23  211	//H23-自保持中间继电器字23
#define	              RW_H24  212	//H24-自保持中间继电器字24
#define	              RW_H25  213	//H25-自保持中间继电器字25
#define	              RW_H26  214	//H26-自保持中间继电器字26
#define	              RW_H27  215	//H27-自保持中间继电器字27
#define	              RW_H28  216	//H28-自保持中间继电器字28
#define	              RW_H29  217	//H29-自保持中间继电器字29
#define	              RW_H30  218	//H30-自保持中间继电器字30
#define	              RW_H31  219	//H31-自保持中间继电器字31
#define	              RW_H32  220	//H32-自保持中间继电器字32
#define	             RW_79PI  221	//79PI-重合闸启动逻辑继电器字
#define	             RW_52BF  222	//52BF-断路器跳闸失败继电器字
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
#define	            RW_TSOTF  255	//TSOTF-重合闸合于故障逻辑继电器字
#define	            RW_T52BF  256	//T52BF-T52BF
#define	              RW_TR1  257	//TR1-跳闸继电器1
#define	              RW_TR2  258	//TR2-跳闸继电器2
#define	              RW_VR1  259	//VR1-虚拟继电器字1
#define	              RW_VR2  260	//VR2-虚拟继电器字2
#define	              RW_VR3  261	//VR3-虚拟继电器字3
#define	              RW_VR4  262	//VR4-虚拟继电器字4
#define	              RW_VR5  263	//VR5-虚拟继电器字5
#define	              RW_VR6  264	//VR6-虚拟继电器字6
#define	              RW_VR7  265	//VR7-虚拟继电器字7
#define	              RW_VR8  266	//VR8-虚拟继电器字8
#define	              RW_VR9  267	//VR9-虚拟继电器字9
#define	             RW_VR10  268	//VR10-虚拟继电器字10
#define	             RW_VR11  269	//VR11-虚拟继电器字11
#define	             RW_VR12  270	//VR12-虚拟继电器字12
#define	             RW_VR13  271	//VR13-虚拟继电器字13
#define	             RW_VR14  272	//VR14-虚拟继电器字14
#define	             RW_VR15  273	//VR15-虚拟继电器字15
#define	             RW_VR16  274	//VR16-虚拟继电器字16
#define	             RW_VR17  275	//VR17-虚拟继电器字17
#define	             RW_VR18  276	//VR18-虚拟继电器字18
#define	             RW_VR19  277	//VR19-虚拟继电器字19
#define	             RW_VR20  278	//VR20-虚拟继电器字20
#define	             RW_VR21  279	//VR21-虚拟继电器字21
#define	             RW_VR22  280	//VR22-虚拟继电器字22
#define	             RW_VR23  281	//VR23-虚拟继电器字23
#define	             RW_VR24  282	//VR24-虚拟继电器字24
#define	             RW_VR25  283	//VR25-虚拟继电器字25
#define	             RW_VR26  284	//VR26-虚拟继电器字26
#define	             RW_VR27  285	//VR27-虚拟继电器字27
#define	             RW_VR28  286	//VR28-虚拟继电器字28
#define	             RW_VR29  287	//VR29-虚拟继电器字29
#define	             RW_VR30  288	//VR30-虚拟继电器字30
#define	             RW_VR31  289	//VR31-虚拟继电器字31
#define	             RW_VR32  290	//VR32-虚拟继电器字32
#define	              RW_IN1  291	//IN1-开入继电器字1
#define	              RW_IN2  292	//IN2-开入继电器字2
#define	              RW_IN3  293	//IN3-开入继电器字3
#define	              RW_IN4  294	//IN4-开入继电器字4
#define	              RW_IN5  295	//IN5-开入继电器字5
#define	              RW_IN6  296	//IN6-开入继电器字6
#define	              RW_IN7  297	//IN7-开入继电器字7
#define	              RW_IN8  298	//IN8-开入继电器字8
#define	              RW_IN9  299	//IN9-开入继电器字9
#define	             RW_IN10  300	//IN10-开入继电器字10
#define	             RW_IN11  301	//IN11-开入继电器字11
#define	             RW_IN12  302	//IN12-开入继电器字12
#define	             RW_IN13  303	//IN13-开入继电器字13
#define	             RW_IN14  304	//IN14-开入继电器字14
#define	             RW_IN15  305	//IN15-开入继电器字15
#define	             RW_IN16  306	//IN16-开入继电器字16
#define	             RW_IN17  307	//IN17-开入继电器字17
#define	             RW_IN18  308	//IN18-开入继电器字18
#define	             RW_IN19  309	//IN19-开入继电器字19
#define	             RW_IN20  310	//IN20-开入继电器字20
#define	             RW_IN21  311	//IN21-开入继电器字21
#define	             RW_IN22  312	//IN22-开入继电器字22
#define	             RW_IN23  313	//IN23-开入继电器字23
#define	             RW_IN24  314	//IN24-开入继电器字24
#define	             RW_IN25  315	//IN25-开入继电器字25
#define	             RW_IN26  316	//IN26-开入继电器字26
#define	             RW_IN27  317	//IN27-开入继电器字27
#define	             RW_IN28  318	//IN28-开入继电器字28
#define	             RW_IN29  319	//IN29-开入继电器字29
#define	             RW_IN30  320	//IN30-开入继电器字30
#define	             RW_IN31  321	//IN31-开入继电器字31
#define	             RW_IN32  322	//IN32-开入继电器字32
#define	             RW_IN33  323	//IN33-开入继电器字33
#define	             RW_IN34  324	//IN34-开入继电器字34
#define	             RW_IN35  325	//IN35-信号复归
#define	             RW_IN36  326	//IN36-跳闸位置
#define	             RW_IN37  327	//IN37-合闸位置
#define	             RW_IN38  328	//IN38-跳闸信号
#define	             RW_IN39  329	//IN39-合闸信号
#define	             RW_IN40  330	//IN40-手跳重动
#define	             RW_IN41  331	//IN41-遥控预置
#define	             RW_IN42  332	//IN42-遥控1预置
#define	             RW_IN43  333	//IN43-遥控2预置
#define	             RW_IN44  334	//IN44-遥控3预置
#define	             RW_IN45  335	//IN45-遥控4预置
#define	           RW_INVIN1  336	//INVIN1-开入继电器字46
#define	           RW_INVIN2  337	//INVIN2-开入继电器字47
#define	           RW_INVIN3  338	//INVIN3-开入继电器字48
#define	           RW_INVIN4  339	//INVIN4-开入继电器字49
#define	           RW_INVIN5  340	//INVIN5-开入继电器字50
#define	           RW_INVIN6  341	//INVIN6-开入继电器字51
#define	           RW_INVIN7  342	//INVIN7-开入继电器字52
#define	           RW_INVIN8  343	//INVIN8-开入继电器字53
#define	           RW_INVIN9  344	//INVIN9-开入继电器字54
#define	          RW_INVIN10  345	//INVIN10-开入继电器字55
#define	          RW_INVIN11  346	//INVIN11-开入继电器字56
#define	          RW_INVIN12  347	//INVIN12-开入继电器字57
#define	          RW_INVIN13  348	//INVIN13-开入继电器字58
#define	          RW_INVIN14  349	//INVIN14-开入继电器字59
#define	          RW_INVIN15  350	//INVIN15-开入继电器字60
#define	          RW_INVIN16  351	//INVIN16-开入继电器字61
#define	             RW_OUT1  352	//OUT1-开出1继电器字
#define	             RW_OUT2  353	//OUT2-开出2继电器字
#define	             RW_OUT3  354	//OUT3-开出3继电器字
#define	             RW_OUT4  355	//OUT4-开出4继电器字
#define	             RW_OUT5  356	//OUT5-开出5继电器字
#define	             RW_OUT6  357	//OUT6-开出6继电器字
#define	             RW_OUT7  358	//OUT7-开出7继电器字
#define	             RW_OUT8  359	//OUT8-开出8继电器字
#define	             RW_OUT9  360	//OUT9-开出9继电器字
#define	            RW_OUT10  361	//OUT10-开出10继电器字
#define	            RW_OUT11  362	//OUT11-开出11继电器字
#define	            RW_OUT12  363	//OUT12-开出12继电器字
#define	            RW_OUT13  364	//OUT13-开出13继电器字
#define	            RW_OUT14  365	//OUT14-开出14继电器字
#define	            RW_OUT15  366	//OUT15-开出15继电器字
#define	            RW_OUT16  367	//OUT16-开出16继电器字
#define	            RW_OUT17  368	//OUT17-开出17继电器字
#define	            RW_OUT18  369	//OUT18-开出18继电器字
#define	            RW_OUT19  370	//OUT19-开出19继电器字
#define	            RW_OUT20  371	//OUT20-开出20继电器字
#define	            RW_OUT21  372	//OUT21-开出21继电器字
#define	             RW_SL21  373	//SL21-距离保护软压板继电器字
#define	           RW_SL50_0  374	//SL50_0-电流速断软压板继电器字
#define	         RW_SL50_123  375	//SL50_123-过流保护软压板继电器字
#define	            RW_SL50D  376	//SL50D-电流增量保护软压板继电器字
#define	             RW_SL27  377	//SL27-低压保护软压板继电器字
#define	             RW_SL79  378	//SL79-重合闸软压板继电器字
#define	          RW_SLMAINT  379	//SLMAINT-检修压板继电器字
#define	          RW_SLRESD1  380	//SLRESD1-备用压板1
#define	          RW_SLRESD2  381	//SLRESD2-备用压板2
#define	            RW_F21_1  382	//F21_1-距离I段配置继电器字
#define	           RW_F21_1F  383	//F21_1F-距离I段正向配置继电器字
#define	            RW_F21_2  384	//F21_2-距离II段配置继电器字
#define	           RW_F21_2F  385	//F21_2F-距离II段正向配置继电器字
#define	            RW_F21_3  386	//F21_3-距离III段配置继电器字
#define	           RW_F21_3F  387	//F21_3F-距离III段正向配置继电器字
#define	            RW_F21_4  388	//F21_4-距离IV段配置继电器字
#define	           RW_F21_4F  389	//F21_4F-距离IV正向配置继电器字
#define	         RW_FPARALLE  390	//FPARALLE-平行四边形配置继电器字
#define	          RW_F21_HLK  391	//F21_HLK-距离谐波闭锁配置继电器字
#define	         RW_F21_HRES  392	//F21_HRES-距离谐波抑制配置继电器字
#define	            RW_F50_0  393	//F50_0-电流速断配置继电器字
#define	         RW_F50_0_27  394	//F50_0_27-速断低压闭锁配置继电器字
#define	        RW_F50_0_HLK  395	//F50_0_HLK-速断谐波闭锁配置继电器字
#define	       RW_F50_0_HRES  396	//F50_0_HRES-速断谐波抑制配置继电器字
#define	           RW_F50_0F  397	//F50_0F-速断保护正向
#define	         RW_F50_0_67  398	//F50_0_67-速断功率方向配置继电器字
#define	            RW_F50_1  399	//F50_1-过流I段配置继电器字
#define	           RW_F50_1F  400	//F50_1F-过流I段正向
#define	         RW_F50_1_67  401	//F50_1_67-过流I段功率方向配置继电器字
#define	         RW_F50_1_27  402	//F50_1_27-过流I段低压闭锁配置继电器字
#define	            RW_F50_2  403	//F50_2-过流II段配置继电器字
#define	           RW_F50_2F  404	//F50_2F-过流II段正向
#define	         RW_F50_2_67  405	//F50_2_67-过流II段功率方向配置继电器字
#define	         RW_F50_2_27  406	//F50_2_27-过流II段低压闭锁配置继电器字
#define	            RW_F50_3  407	//F50_3-过流III段配置继电器字
#define	       RW_F50_123HLK  408	//F50_123HLK-过流谐波闭锁配置继电器字
#define	      RW_F50_123HRES  409	//F50_123HRES-过流谐波抑制配置继电器字
#define	              RW_F51  410	//F51-反时限过流配置继电器字
#define	             RW_F50D  411	//F50D-电流增量配置继电器字
#define	              RW_F27  412	//F27-低压保护配置继电器字
#define	            RW_F52BF  413	//F52BF-失灵保护配置继电器字
#define	           RW_F74VT1  414	//F74VT1-PT1断线检查配置继电器字
#define	           RW_F74VT2  415	//F74VT2-PT2断线检查配置继电器字
#define	         RW_FFLOCATE  416	//FFLOCATE-故障测距配置继电器字
#define	           RW_F79ITR  417	//F79ITR-偷跳重合闸配置继电器字
#define	           RW_F79_51  418	//F79_51-反时限重合
#define	           RW_F79_21  419	//F79_21-距离保护重合
#define	         RW_F79_50_0  420	//F79_50_0-速断保护重合
#define	       RW_F79_50_123  421	//F79_50_123-过流保护重合
#define	          RW_F79_50D  422	//F79_50D-增量保护重合
#define	         RW_F21_2_94  423	//F21_2_94-距离II段加速配置继电器字
#define	         RW_F21_3_94  424	//F21_3_94-距离III段加速配置继电器字
#define	         RW_F21_4_94  425	//F21_4_94-距离IV段加速配置继电器字
#define	         RW_F50_1_94  426	//F50_1_94-过流I段加速配置继电器字
#define	         RW_F50_2_94  427	//F50_2_94-过流II段加速配置继电器字
#define	         RW_F50_3_94  428	//F50_3_94-过流III段加速配置继电器字
#define	           RW_F79_59  429	//F79_59-重合闸检有压配置继电器字
#define	          RW_FDUISUP  430	//FDUISUP-负荷录波配置继电器字
#define	            RW_FCTBR  431	//FCTBR-CT断线检测配置继电器字
#define	           RW_FRESD1  432	//FRESD1-备用配置1
#define	           RW_FRESD2  433	//FRESD2-备用配置2
#define	              RW_PW1  434	//PW1-脉冲继电器1
#define	              RW_PW2  435	//PW2-脉冲继电器2
#define	              RW_PW3  436	//PW3-脉冲继电器3
#define	              RW_PW4  437	//PW4-脉冲继电器4
#define	              RW_PW5  438	//PW5-脉冲继电器5
#define	              RW_PW6  439	//PW6-脉冲继电器6
#define	              RW_PW7  440	//PW7-脉冲继电器7
#define	              RW_PW8  441	//PW8-脉冲继电器8
#define	              RW_PW9  442	//PW9-脉冲继电器9
#define	             RW_PW10  443	//PW10-脉冲继电器10
#define	             RW_PW11  444	//PW11-脉冲继电器11
#define	             RW_PW12  445	//PW12-脉冲继电器12
#define	             RW_PW13  446	//PW13-脉冲继电器13
#define	             RW_PW14  447	//PW14-脉冲继电器14
#define	             RW_PW15  448	//PW15-脉冲继电器15
#define	             RW_PW16  449	//PW16-脉冲继电器16
#define	              RW_TO1  450	//TO1-传动继电器1
#define	              RW_TO2  451	//TO2-传动继电器2
#define	              RW_TO3  452	//TO3-传动继电器3
#define	              RW_TO4  453	//TO4-传动继电器4
#define	              RW_TO5  454	//TO5-传动继电器5
#define	              RW_TO6  455	//TO6-传动继电器6
#define	              RW_TO7  456	//TO7-传动继电器7
#define	              RW_TO8  457	//TO8-传动继电器8
#define	              RW_TO9  458	//TO9-传动继电器9
#define	             RW_TO10  459	//TO10-传动继电器10
#define	             RW_TO11  460	//TO11-传动继电器11
#define	             RW_TO12  461	//TO12-传动继电器12
#define	             RW_TO13  462	//TO13-传动继电器13
#define	             RW_SIG1  463	//SIG1-信号继电器1
#define	             RW_SIG2  464	//SIG2-信号继电器2
#define	             RW_SIG3  465	//SIG3-信号继电器3
#define	             RW_SIG4  466	//SIG4-信号继电器4
#define	             RW_SIG5  467	//SIG5-信号继电器5
#define	             RW_SIG6  468	//SIG6-信号继电器6
#define	             RW_SIG7  469	//SIG7-信号继电器7
#define	             RW_SIG8  470	//SIG8-信号继电器8
#define	             RW_SIG9  471	//SIG9-信号继电器9
#define	            RW_SIG10  472	//SIG10-信号继电器10
#define	            RW_SIG11  473	//SIG11-信号继电器11
#define	            RW_SIG12  474	//SIG12-信号继电器12
#define	            RW_SIG13  475	//SIG13-信号继电器13
#define	            RW_SIG14  476	//SIG14-信号继电器14
#define	            RW_SIG15  477	//SIG15-信号继电器15
#define	            RW_SIG16  478	//SIG16-信号继电器16
#define	            RW_LED1G  479	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  480	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  481	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  482	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  483	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  484	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  485	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  486	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  487	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  488	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  489	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  490	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  491	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  492	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  493	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  494	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  495	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  496	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  497	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  498	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  499	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  500	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  501	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  502	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  503	//VIN1-虚拟开入继电器字1
#define	             RW_VIN2  504	//VIN2-虚拟开入继电器字2
#define	             RW_VIN3  505	//VIN3-虚拟开入继电器字3
#define	             RW_VIN4  506	//VIN4-虚拟开入继电器字4
#define	             RW_VIN5  507	//VIN5-虚拟开入继电器字5
#define	             RW_VIN6  508	//VIN6-虚拟开入继电器字6
#define	             RW_VIN7  509	//VIN7-虚拟开入继电器字7
#define	             RW_VIN8  510	//VIN8-虚拟开入继电器字8
#define	             RW_VIN9  511	//VIN9-虚拟开入继电器字9
#define	            RW_VIN10  512	//VIN10-虚拟开入继电器字10
#define	            RW_VIN11  513	//VIN11-虚拟开入继电器字11
#define	            RW_VIN12  514	//VIN12-虚拟开入继电器字12
#define	            RW_VIN13  515	//VIN13-虚拟开入继电器字13
#define	            RW_VIN14  516	//VIN14-虚拟开入继电器字14
#define	            RW_VIN15  517	//VIN15-虚拟开入继电器字15
#define	            RW_VIN16  518	//VIN16-虚拟开入继电器字16
#define	             RW_RCP1  519	//RCP1-遥控预置继电器1
#define	             RW_RCP2  520	//RCP2-遥控预置继电器2
#define	             RW_RCP3  521	//RCP3-遥控预置继电器3
#define	             RW_RCP4  522	//RCP4-遥控预置继电器4
#define	             RW_RCP5  523	//RCP5-遥控预置继电器5
#define	             RW_RCP6  524	//RCP6-遥控预置继电器6
#define	             RW_RCP7  525	//RCP7-遥控预置继电器7
#define	             RW_RCP8  526	//RCP8-遥控预置继电器8
#define	             RW_RCP9  527	//RCP9-遥控预置继电器9
#define	            RW_RCP10  528	//RCP10-遥控预置继电器10
#define	            RW_RCP11  529	//RCP11-遥控预置继电器11
#define	            RW_RCP12  530	//RCP12-遥控预置继电器12
#define	            RW_RCP13  531	//RCP13-遥控预置继电器13
#define	            RW_RCP14  532	//RCP14-遥控预置继电器14
#define	            RW_RCP15  533	//RCP15-遥控预置继电器15
#define	            RW_RCP16  534	//RCP16-遥控预置继电器16
#define	            RW_RCP17  535	//RCP17-遥控预置继电器17
#define	            RW_RCP18  536	//RCP18-遥控预置继电器18
#define	            RW_RCP19  537	//RCP19-遥控预置继电器19
#define	            RW_RCP20  538	//RCP20-遥控预置继电器20
#define	            RW_RCP21  539	//RCP21-遥控预置继电器21
#define	            RW_RCP22  540	//RCP22-遥控预置继电器22
#define	            RW_RCP23  541	//RCP23-遥控预置继电器23
#define	            RW_RCP24  542	//RCP24-遥控预置继电器24
#define	            RW_RCP25  543	//RCP25-遥控预置继电器25
#define	            RW_RCP26  544	//RCP26-遥控预置继电器26
#define	            RW_RCP27  545	//RCP27-遥控预置继电器27
#define	            RW_RCP28  546	//RCP28-遥控预置继电器28
#define	            RW_RCP29  547	//RCP29-遥控预置继电器29
#define	            RW_RCP30  548	//RCP30-遥控预置继电器30
#define	            RW_RCP31  549	//RCP31-遥控预置继电器31
#define	            RW_RCP32  550	//RCP32-遥控预置继电器32
#define	            RW_RCP33  551	//RCP33-遥控预置继电器33
#define	            RW_RCP34  552	//RCP34-遥控预置继电器34
#define	            RW_RCP35  553	//RCP35-遥控预置继电器35
#define	            RW_RCP36  554	//RCP36-遥控预置继电器36
#define	             RW_RCE1  555	//RCE1-遥控执行继电器1
#define	             RW_RCE2  556	//RCE2-遥控执行继电器2
#define	             RW_RCE3  557	//RCE3-遥控执行继电器3
#define	             RW_RCE4  558	//RCE4-遥控执行继电器4
#define	             RW_RCE5  559	//RCE5-遥控执行继电器5
#define	             RW_RCE6  560	//RCE6-遥控执行继电器6
#define	             RW_RCE7  561	//RCE7-遥控执行继电器7
#define	             RW_RCE8  562	//RCE8-遥控执行继电器8
#define	             RW_RCE9  563	//RCE9-遥控执行继电器9
#define	            RW_RCE10  564	//RCE10-遥控执行继电器10
#define	            RW_RCE11  565	//RCE11-遥控执行继电器11
#define	            RW_RCE12  566	//RCE12-遥控执行继电器12
#define	            RW_RCE13  567	//RCE13-遥控执行继电器13
#define	            RW_RCE14  568	//RCE14-遥控执行继电器14
#define	            RW_RCE15  569	//RCE15-遥控执行继电器15
#define	            RW_RCE16  570	//RCE16-遥控执行继电器16
#define	            RW_RCE17  571	//RCE17-遥控执行继电器17
#define	            RW_RCE18  572	//RCE18-遥控执行继电器18
#define	            RW_RCE19  573	//RCE19-遥控执行继电器19
#define	            RW_RCE20  574	//RCE20-遥控执行继电器20
#define	            RW_RCE21  575	//RCE21-遥控执行继电器21
#define	            RW_RCE22  576	//RCE22-遥控执行继电器22
#define	            RW_RCE23  577	//RCE23-遥控执行继电器23
#define	            RW_RCE24  578	//RCE24-遥控执行继电器24
#define	            RW_RCE25  579	//RCE25-遥控执行继电器25
#define	            RW_RCE26  580	//RCE26-遥控执行继电器26
#define	            RW_RCE27  581	//RCE27-遥控执行继电器27
#define	            RW_RCE28  582	//RCE28-遥控执行继电器28
#define	            RW_RCE29  583	//RCE29-遥控执行继电器29
#define	            RW_RCE30  584	//RCE30-遥控执行继电器30
#define	            RW_RCE31  585	//RCE31-遥控执行继电器31
#define	            RW_RCE32  586	//RCE32-遥控执行继电器32
#define	            RW_RCE33  587	//RCE33-遥控执行继电器33
#define	            RW_RCE34  588	//RCE34-遥控执行继电器34
#define	            RW_RCE35  589	//RCE35-遥控执行继电器35
#define	            RW_RCE36  590	//RCE36-遥控执行继电器36
#define	             RW_RCV1  591	//RCV1-遥控反校继电器1
#define	             RW_RCV2  592	//RCV2-遥控反校继电器2
#define	             RW_RCV3  593	//RCV3-遥控反校继电器3
#define	             RW_RCV4  594	//RCV4-遥控反校继电器4
#define	             RW_RCV5  595	//RCV5-遥控反校继电器5
#define	             RW_RCV6  596	//RCV6-遥控反校继电器6
#define	             RW_RCV7  597	//RCV7-遥控反校继电器7
#define	             RW_RCV8  598	//RCV8-遥控反校继电器8
#define	             RW_RCV9  599	//RCV9-遥控反校继电器9
#define	            RW_RCV10  600	//RCV10-遥控反校继电器10
#define	            RW_RCV11  601	//RCV11-遥控反校继电器11
#define	            RW_RCV12  602	//RCV12-遥控反校继电器12
#define	            RW_RCV13  603	//RCV13-遥控反校继电器13
#define	            RW_RCV14  604	//RCV14-遥控反校继电器14
#define	            RW_RCV15  605	//RCV15-遥控反校继电器15
#define	            RW_RCV16  606	//RCV16-遥控反校继电器16
#define	            RW_RCV17  607	//RCV17-遥控反校继电器17
#define	            RW_RCV18  608	//RCV18-遥控反校继电器18
#define	            RW_RCV19  609	//RCV19-遥控反校继电器19
#define	            RW_RCV20  610	//RCV20-遥控反校继电器20
#define	            RW_RCV21  611	//RCV21-遥控反校继电器21
#define	            RW_RCV22  612	//RCV22-遥控反校继电器22
#define	            RW_RCV23  613	//RCV23-遥控反校继电器23
#define	            RW_RCV24  614	//RCV24-遥控反校继电器24
#define	            RW_RCV25  615	//RCV25-遥控反校继电器25
#define	            RW_RCV26  616	//RCV26-遥控反校继电器26
#define	            RW_RCV27  617	//RCV27-遥控反校继电器27
#define	            RW_RCV28  618	//RCV28-遥控反校继电器28
#define	            RW_RCV29  619	//RCV29-遥控反校继电器29
#define	            RW_RCV30  620	//RCV30-遥控反校继电器30
#define	            RW_RCV31  621	//RCV31-遥控反校继电器31
#define	            RW_RCV32  622	//RCV32-遥控反校继电器32
#define	            RW_RCV33  623	//RCV33-遥控反校继电器33
#define	            RW_RCV34  624	//RCV34-遥控反校继电器34
#define	            RW_RCV35  625	//RCV35-遥控反校继电器35
#define	            RW_RCV36  626	//RCV36-遥控反校继电器36
#define	              RW_GE1  627	//GE1-大于等于继电器1
#define	              RW_GE2  628	//GE2-大于等于继电器2
#define	              RW_GE3  629	//GE3-大于等于继电器3
#define	              RW_GE4  630	//GE4-大于等于继电器4
#define	              RW_GE5  631	//GE5-大于等于继电器5
#define	              RW_GE6  632	//GE6-大于等于继电器6
#define	              RW_GE7  633	//GE7-大于等于继电器7
#define	              RW_GE8  634	//GE8-大于等于继电器8
#define	           RW_CONST1  635	//CONST1-常数时间继电器1
#define	           RW_CONST2  636	//CONST2-常数时间继电器2
#define	           RW_CONST3  637	//CONST3-常数时间继电器3
#define	           RW_CONST4  638	//CONST4-常数时间继电器4
#define	           RW_CONST5  639	//CONST5-常数时间继电器5
#define	           RW_CONST6  640	//CONST6-常数时间继电器6
#define	           RW_CONST7  641	//CONST7-常数时间继电器7
#define	           RW_CONST8  642	//CONST8-常数时间继电器8
#define	             RW_TLR1  643	//TLR1-镜像发送继电器字1
#define	             RW_TLR2  644	//TLR2-镜像发送继电器字2
#define	             RW_TLR3  645	//TLR3-镜像发送继电器字3
#define	             RW_TLR4  646	//TLR4-镜像发送继电器字4
#define	             RW_TLR5  647	//TLR5-镜像发送继电器字5
#define	             RW_TLR6  648	//TLR6-镜像发送继电器字6
#define	             RW_TLR7  649	//TLR7-镜像发送继电器字7
#define	             RW_TLR8  650	//TLR8-镜像发送继电器字8
#define	             RW_TLR9  651	//TLR9-镜像发送继电器字9
#define	            RW_TLR10  652	//TLR10-镜像发送继电器字10
#define	            RW_TLR11  653	//TLR11-镜像发送继电器字11
#define	            RW_TLR12  654	//TLR12-镜像发送继电器字12
#define	            RW_TLR13  655	//TLR13-镜像发送继电器字13
#define	            RW_TLR14  656	//TLR14-镜像发送继电器字14
#define	            RW_TLR15  657	//TLR15-镜像发送继电器字15
#define	            RW_TLR16  658	//TLR16-镜像发送继电器字16
#define	             RW_RLR1  659	//RLR1-镜像接收继电器字1
#define	             RW_RLR2  660	//RLR2-镜像接收继电器字2
#define	             RW_RLR3  661	//RLR3-镜像接收继电器字3
#define	             RW_RLR4  662	//RLR4-镜像接收继电器字4
#define	             RW_RLR5  663	//RLR5-镜像接收继电器字5
#define	             RW_RLR6  664	//RLR6-镜像接收继电器字6
#define	             RW_RLR7  665	//RLR7-镜像接收继电器字7
#define	             RW_RLR8  666	//RLR8-镜像接收继电器字8
#define	             RW_RLR9  667	//RLR9-镜像接收继电器字9
#define	            RW_RLR10  668	//RLR10-镜像接收继电器字10
#define	            RW_RLR11  669	//RLR11-镜像接收继电器字11
#define	            RW_RLR12  670	//RLR12-镜像接收继电器字12
#define	            RW_RLR13  671	//RLR13-镜像接收继电器字13
#define	            RW_RLR14  672	//RLR14-镜像接收继电器字14
#define	            RW_RLR15  673	//RLR15-镜像接收继电器字15
#define	            RW_RLR16  674	//RLR16-镜像接收继电器字16
#define	             RW_CNT1  675	//CNT1-计数继电器字1
#define	             RW_CNT2  676	//CNT2-计数继电器字2
#define	             RW_CNT3  677	//CNT3-计数继电器字3
#define	             RW_CNT4  678	//CNT4-计数继电器字4
#define	             RW_CNT5  679	//CNT5-计数继电器字5
#define	             RW_CNT6  680	//CNT6-计数继电器字6
#define	             RW_CNT7  681	//CNT7-计数继电器字7
#define	             RW_CNT8  682	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			683		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			95	

//中间继电器字
#define	MID_RELAY_WORD_START			97	
#define	MID_RELAY_WORD_NUM			92	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			189	
#define	KEEP_RELAY_WORD_NUM			34	

//时间继电器字
#define	TIME_RELAY_WORD_START			223	
#define	TIME_RELAY_WORD_NUM			34	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			257	
#define	TRIP_RELAY_WORD_NUM			2	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			259	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			291	
#define	INPUT_RELAY_WORD_NUM			61	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			352	
#define	OUTPUT_RELAY_WORD_NUM			21	

//压板继电器
#define	SW_RELAY_WORD_START			373	
#define	SW_RELAY_WORD_NUM			9	

//配置继电器
#define	CFG_RELAY_WORD_START			382	
#define	CFG_RELAY_WORD_NUM			52	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			434	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			450	
#define	CTRLTEST_RELAY_WORD_NUM			13	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			463	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			479	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			503	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			519	
#define	RCP_RELAY_WORD_NUM			36	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			555	
#define	RCE_RELAY_WORD_NUM			36	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			591	
#define	RCV_RELAY_WORD_NUM			36	

//大于等于继电器
#define	GE_RELAY_WORD_START			627	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			635	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			643	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			659	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			675	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			675	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			675	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			675	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			675	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			675	
#define	CNT_RELAY_WORD_NUM			8	


#endif