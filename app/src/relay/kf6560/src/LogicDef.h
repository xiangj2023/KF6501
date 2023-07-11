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
#define	          RW_LPC50_0    2	//LPC50_0-电流速断循环控制继电器字
#define	          RW_LPC50_1    3	//LPC50_1-过流保护循环控制继电器字
#define	            RW_LPC59    4	//LPC59-过压保护循环控制继电器字
#define	            RW_LPC27    5	//LPC27-低压保护循环控制继电器字
#define	           RW_LPC50H    6	//LPC50H-谐波过流保护循环控制继电器字
#define	          RW_LPC60I1    7	//LPC60I1-支路1差流保护循环控制继电器字
#define	          RW_LPC60I2    8	//LPC60I2-支路2差流保护循环控制继电器字
#define	          RW_LPC50ST    9	//LPC50ST-支路过流保护循环控制继电器字
#define	          RW_LPC60U1   10	//LPC60U1-支路1差压保护循环控制继电器字
#define	          RW_LPC60U2   11	//LPC60U2-支路2差压保护循环控制继电器字
#define	          RW_LPC60U3   12	//LPC60U3-支路3差压保护循环控制继电器字
#define	           RW_PI50_0   13	//PI50_0-电流速断入段继电器字
#define	           RW_TR50_0   14	//TR50_0-电流速断动作继电器字
#define	           RW_PI50_1   15	//PI50_1-过流保护入段继电器字
#define	           RW_TR50_1   16	//TR50_1-过流保护动作继电器字
#define	             RW_PI59   17	//PI59-过压保护入段继电器字
#define	             RW_TR59   18	//TR59-过压保护动作继电器字
#define	             RW_PI27   19	//PI27-低压保护入段继电器字
#define	             RW_TR27   20	//TR27-低压保护动作继电器字
#define	            RW_PI50H   21	//PI50H-谐波过流保护入段继电器字
#define	            RW_TR50H   22	//TR50H-谐波过流保护动作继电器字
#define	           RW_PI60I1   23	//PI60I1-支路1差流保护入段继电器字
#define	           RW_TR60I1   24	//TR60I1-支路1差流保护动作继电器字
#define	           RW_PI60I2   25	//PI60I2-支路2差流保护入段继电器字
#define	           RW_TR60I2   26	//TR60I2-支路2差流保护动作继电器字
#define	           RW_PI50ST   27	//PI50ST-支路过流保护入段继电器字
#define	           RW_TR50ST   28	//TR50ST-支路过流保护动作继电器字
#define	           RW_PI60U1   29	//PI60U1-支路1差压保护入段继电器字
#define	           RW_TR60U1   30	//TR60U1-支路1差压保护动作继电器字
#define	           RW_PI60U2   31	//PI60U2-支路2差压保护入段继电器字
#define	           RW_TR60U2   32	//TR60U2-支路2差压保护动作继电器字
#define	           RW_PI60U3   33	//PI60U3-支路3差压保护入段继电器字
#define	           RW_TR60U3   34	//TR60U3-支路3差压保护动作继电器字
#define	           RW_PI74PT   35	//PI74PT-PT断线入段继电器字
#define	           RW_AR74PT   36	//AR74PT-PT断线告警继电器字
#define	           RW_PICTBR   37	//PICTBR-CT断线入段继电器字
#define	           RW_ARCTBR   38	//ARCTBR-CT断线告警继电器字
#define	           RW_PICKUP   39	//PICKUP-模值突变量启动继电器字
#define	           RW_BTMODE   40	//BTMODE-直供方式继电器字
#define	           RW_ATMODE   41	//ATMODE-AT供电方式继电器字
#define	             RW_50IP   42	//50IP-Ip有流继电器字
#define	             RW_37IP   43	//37IP-Ip无流继电器字
#define	             RW_50I1   44	//50I1-I1有流继电器字
#define	             RW_37I1   45	//37I1-I1无流继电器字
#define	             RW_50I2   46	//50I2-I2有流继电器字
#define	             RW_37I2   47	//37I2-I2无流继电器字
#define	             RW_50I3   48	//50I3-I3有流继电器字
#define	             RW_37I3   49	//37I3-I3无流继电器字
#define	             RW_59UP   50	//59UP-Up有压继电器字
#define	             RW_27UP   51	//27UP-Up无压继电器字
#define	             RW_59U1   52	//59U1-U1有压继电器字
#define	             RW_27U1   53	//27U1-U1无压继电器字
#define	             RW_59U2   54	//59U2-U2有压继电器字
#define	             RW_27U2   55	//27U2-U2无压继电器字
#define	            RW_59UD1   56	//59UD1-Ud1有压继电器字
#define	            RW_27UD1   57	//27UD1-Ud1无压继电器字
#define	            RW_59UD2   58	//59UD2-Ud2有压继电器字
#define	            RW_27UD2   59	//27UD2-Ud2无压继电器字
#define	            RW_59UD3   60	//59UD3-Ud3有压继电器字
#define	            RW_27UD3   61	//27UD3-Ud3无压继电器字
#define	          RW_ET11ERR   62	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   63	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   64	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   65	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   66	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   67	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   68	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   69	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   70	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   71	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   72	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   73	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   74	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   75	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR   76	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR   77	//NULL3ERR-空3连接状态继电器字
#define	        RW_MAINTLINK   78	//MAINTLINK-维护状态继电器字
#define	         RW_LKPI50_0   79	//LKPI50_0-电流速断入段闭锁继电器字
#define	        RW_LKRST50_0   80	//LKRST50_0-电流速断返回闭锁继电器字
#define	         RW_LKPI50_1   81	//LKPI50_1-过流保护入段闭锁继电器字
#define	        RW_LKRST50_1   82	//LKRST50_1-过流保护返回闭锁继电器字
#define	           RW_LKPI59   83	//LKPI59-过压保护入段闭锁继电器字
#define	          RW_LKRST59   84	//LKRST59-过压保护返回闭锁继电器字
#define	           RW_LKPI27   85	//LKPI27-低压保护入段闭锁继电器字
#define	          RW_LKRST27   86	//LKRST27-低压保护返回闭锁继电器字
#define	          RW_LKPI50H   87	//LKPI50H-谐波过流入段闭锁继电器字
#define	         RW_LKRST50H   88	//LKRST50H-谐波过流返回闭锁继电器字
#define	         RW_LKPI60I1   89	//LKPI60I1-差流1入段闭锁继电器字
#define	        RW_LKRST60I1   90	//LKRST60I1-差流1返回闭锁继电器字
#define	         RW_LKPI60I2   91	//LKPI60I2-差流2入段闭锁继电器字
#define	        RW_LKRST60I2   92	//LKRST60I2-差流2返回闭锁继电器字
#define	         RW_LKPI50ST   93	//LKPI50ST-支路过流入段闭锁继电器字
#define	        RW_LKRST50ST   94	//LKRST50ST-支路过流返回闭锁继电器字
#define	         RW_LKPI60U1   95	//LKPI60U1-支路1差压入段闭锁继电器字
#define	        RW_LKRST60U1   96	//LKRST60U1-支路1差压返回闭锁继电器字
#define	         RW_LKPI60U2   97	//LKPI60U2-支路2差压入段闭锁继电器字
#define	        RW_LKRST60U2   98	//LKRST60U2-支路2差压返回闭锁继电器字
#define	         RW_LKPI60U3   99	//LKPI60U3-支路3差压入段闭锁继电器字
#define	        RW_LKRST60U3  100	//LKRST60U3-支路3差压返回闭锁继电器字
#define	             RW_69IN  101	//69IN-远方当地中间继电器字
#define	              RW_52A  102	//52A-合位
#define	              RW_52B  103	//52B-分位
#define	               RW_M1  104	//M1-中间继电器字M1
#define	               RW_M2  105	//M2-中间继电器字M2
#define	               RW_M3  106	//M3-中间继电器字M3
#define	               RW_M4  107	//M4-中间继电器字M4
#define	               RW_M5  108	//M5-中间继电器字M5
#define	               RW_M6  109	//M6-中间继电器字M6
#define	               RW_M7  110	//M7-中间继电器字M7
#define	               RW_M8  111	//M8-中间继电器字M8
#define	               RW_M9  112	//M9-中间继电器字M9
#define	              RW_M10  113	//M10-中间继电器字M10
#define	              RW_M11  114	//M11-中间继电器字M11
#define	              RW_M12  115	//M12-中间继电器字M12
#define	              RW_M13  116	//M13-中间继电器字M13
#define	              RW_M14  117	//M14-中间继电器字M14
#define	              RW_M15  118	//M15-中间继电器字M15
#define	              RW_M16  119	//M16-中间继电器字M16
#define	              RW_M17  120	//M17-中间继电器字M17
#define	              RW_M18  121	//M18-中间继电器字M18
#define	              RW_M19  122	//M19-中间继电器字M19
#define	              RW_M20  123	//M20-中间继电器字M20
#define	              RW_M21  124	//M21-中间继电器字M21
#define	              RW_M22  125	//M22-中间继电器字M22
#define	              RW_M23  126	//M23-中间继电器字M23
#define	              RW_M24  127	//M24-中间继电器字M24
#define	              RW_M25  128	//M25-中间继电器字M25
#define	              RW_M26  129	//M26-中间继电器字M26
#define	              RW_M27  130	//M27-中间继电器字M27
#define	              RW_M28  131	//M28-中间继电器字M28
#define	              RW_M29  132	//M29-中间继电器字M29
#define	              RW_M30  133	//M30-中间继电器字M30
#define	              RW_M31  134	//M31-中间继电器字M31
#define	              RW_M32  135	//M32-中间继电器字M32
#define	              RW_M33  136	//M33-中间继电器字M33
#define	              RW_M34  137	//M34-中间继电器字M34
#define	              RW_M35  138	//M35-中间继电器字M35
#define	              RW_M36  139	//M36-中间继电器字M36
#define	              RW_M37  140	//M37-中间继电器字M37
#define	              RW_M38  141	//M38-中间继电器字M38
#define	              RW_M39  142	//M39-中间继电器字M39
#define	              RW_M40  143	//M40-中间继电器字M40
#define	              RW_M41  144	//M41-中间继电器字M41
#define	              RW_M42  145	//M42-中间继电器字M42
#define	              RW_M43  146	//M43-中间继电器字M43
#define	              RW_M44  147	//M44-中间继电器字M44
#define	              RW_M45  148	//M45-中间继电器字M45
#define	              RW_M46  149	//M46-中间继电器字M46
#define	              RW_M47  150	//M47-中间继电器字M47
#define	              RW_M48  151	//M48-中间继电器字M48
#define	              RW_M49  152	//M49-中间继电器字M49
#define	              RW_M50  153	//M50-中间继电器字M50
#define	              RW_M51  154	//M51-中间继电器字M51
#define	              RW_M52  155	//M52-中间继电器字M52
#define	              RW_M53  156	//M53-中间继电器字M53
#define	              RW_M54  157	//M54-中间继电器字M54
#define	              RW_M55  158	//M55-中间继电器字M55
#define	              RW_M56  159	//M56-中间继电器字M56
#define	              RW_M57  160	//M57-中间继电器字M57
#define	              RW_M58  161	//M58-中间继电器字M58
#define	              RW_M59  162	//M59-中间继电器字M59
#define	              RW_M60  163	//M60-中间继电器字M60
#define	              RW_M61  164	//M61-中间继电器字M61
#define	              RW_M62  165	//M62-中间继电器字M62
#define	              RW_M63  166	//M63-中间继电器字M63
#define	              RW_M64  167	//M64-中间继电器字M64
#define	              RW_REC  168	//REC-录波继电器字
#define	             RW_LKCL  169	//LKCL-合闸闭锁继电器字
#define	             RW_LKTR  170	//LKTR-分闸闭锁继电器字
#define	               RW_H1  171	//H1-自保持中间继电器字1
#define	               RW_H2  172	//H2-自保持中间继电器字2
#define	               RW_H3  173	//H3-自保持中间继电器字3
#define	               RW_H4  174	//H4-自保持中间继电器字4
#define	               RW_H5  175	//H5-自保持中间继电器字5
#define	               RW_H6  176	//H6-自保持中间继电器字6
#define	               RW_H7  177	//H7-自保持中间继电器字7
#define	               RW_H8  178	//H8-自保持中间继电器字8
#define	               RW_H9  179	//H9-自保持中间继电器字9
#define	              RW_H10  180	//H10-自保持中间继电器字10
#define	              RW_H11  181	//H11-自保持中间继电器字11
#define	              RW_H12  182	//H12-自保持中间继电器字12
#define	              RW_H13  183	//H13-自保持中间继电器字13
#define	              RW_H14  184	//H14-自保持中间继电器字14
#define	              RW_H15  185	//H15-自保持中间继电器字15
#define	              RW_H16  186	//H16-自保持中间继电器字16
#define	              RW_H17  187	//H17-自保持中间继电器字17
#define	              RW_H18  188	//H18-自保持中间继电器字18
#define	              RW_H19  189	//H19-自保持中间继电器字19
#define	              RW_H20  190	//H20-自保持中间继电器字20
#define	              RW_H21  191	//H21-自保持中间继电器字21
#define	              RW_H22  192	//H22-自保持中间继电器字22
#define	              RW_H23  193	//H23-自保持中间继电器字23
#define	              RW_H24  194	//H24-自保持中间继电器字24
#define	              RW_H25  195	//H25-自保持中间继电器字25
#define	              RW_H26  196	//H26-自保持中间继电器字26
#define	              RW_H27  197	//H27-自保持中间继电器字27
#define	              RW_H28  198	//H28-自保持中间继电器字28
#define	              RW_H29  199	//H29-自保持中间继电器字29
#define	              RW_H30  200	//H30-自保持中间继电器字30
#define	              RW_H31  201	//H31-自保持中间继电器字31
#define	              RW_H32  202	//H32-自保持中间继电器字31
#define	             RW_52BF  203	//52BF-断路器跳闸失败继电器字
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
#define	              RW_TR1  237	//TR1-跳闸继电器1
#define	              RW_TR2  238	//TR2-跳闸继电器2
#define	              RW_TR3  239	//TR3-跳闸继电器3
#define	              RW_VR1  240	//VR1-虚拟继电器字1
#define	              RW_VR2  241	//VR2-虚拟继电器字2
#define	              RW_VR3  242	//VR3-虚拟继电器字3
#define	              RW_VR4  243	//VR4-虚拟继电器字4
#define	              RW_VR5  244	//VR5-虚拟继电器字5
#define	              RW_VR6  245	//VR6-虚拟继电器字6
#define	              RW_VR7  246	//VR7-虚拟继电器字7
#define	              RW_VR8  247	//VR8-虚拟继电器字8
#define	              RW_VR9  248	//VR9-虚拟继电器字9
#define	             RW_VR10  249	//VR10-虚拟继电器字10
#define	             RW_VR11  250	//VR11-虚拟继电器字11
#define	             RW_VR12  251	//VR12-虚拟继电器字12
#define	             RW_VR13  252	//VR13-虚拟继电器字13
#define	             RW_VR14  253	//VR14-虚拟继电器字14
#define	             RW_VR15  254	//VR15-虚拟继电器字15
#define	             RW_VR16  255	//VR16-虚拟继电器字16
#define	             RW_VR17  256	//VR17-虚拟继电器字17
#define	             RW_VR18  257	//VR18-虚拟继电器字18
#define	             RW_VR19  258	//VR19-虚拟继电器字19
#define	             RW_VR20  259	//VR20-虚拟继电器字20
#define	             RW_VR21  260	//VR21-虚拟继电器字21
#define	             RW_VR22  261	//VR22-虚拟继电器字22
#define	             RW_VR23  262	//VR23-虚拟继电器字23
#define	             RW_VR24  263	//VR24-虚拟继电器字24
#define	             RW_VR25  264	//VR25-虚拟继电器字25
#define	             RW_VR26  265	//VR26-虚拟继电器字26
#define	             RW_VR27  266	//VR27-虚拟继电器字27
#define	             RW_VR28  267	//VR28-虚拟继电器字28
#define	             RW_VR29  268	//VR29-虚拟继电器字29
#define	             RW_VR30  269	//VR30-虚拟继电器字30
#define	             RW_VR31  270	//VR31-虚拟继电器字31
#define	             RW_VR32  271	//VR32-虚拟继电器字32
#define	              RW_IN1  272	//IN1-开入继电器字1
#define	              RW_IN2  273	//IN2-开入继电器字2
#define	              RW_IN3  274	//IN3-开入继电器字3
#define	              RW_IN4  275	//IN4-开入继电器字4
#define	              RW_IN5  276	//IN5-开入继电器字5
#define	              RW_IN6  277	//IN6-开入继电器字6
#define	              RW_IN7  278	//IN7-开入继电器字7
#define	              RW_IN8  279	//IN8-开入继电器字8
#define	              RW_IN9  280	//IN9-开入继电器字9
#define	             RW_IN10  281	//IN10-开入继电器字10
#define	             RW_IN11  282	//IN11-开入继电器字11
#define	             RW_IN12  283	//IN12-开入继电器字12
#define	             RW_IN13  284	//IN13-开入继电器字13
#define	             RW_IN14  285	//IN14-开入继电器字14
#define	             RW_IN15  286	//IN15-开入继电器字15
#define	             RW_IN16  287	//IN16-开入继电器字16
#define	             RW_IN17  288	//IN17-开入继电器字17
#define	             RW_IN18  289	//IN18-开入继电器字18
#define	             RW_IN19  290	//IN19-开入继电器字19
#define	             RW_IN20  291	//IN20-开入继电器字20
#define	             RW_IN21  292	//IN21-开入继电器字21
#define	             RW_IN22  293	//IN22-开入继电器字22
#define	             RW_IN23  294	//IN23-开入继电器字23
#define	             RW_IN24  295	//IN24-开入继电器字24
#define	             RW_IN25  296	//IN25-开入继电器字25
#define	             RW_IN26  297	//IN26-开入继电器字26
#define	             RW_IN27  298	//IN27-开入继电器字27
#define	             RW_IN28  299	//IN28-开入继电器字28
#define	             RW_IN29  300	//IN29-开入继电器字29
#define	             RW_IN30  301	//IN30-开入继电器字30
#define	             RW_IN31  302	//IN31-开入继电器字31
#define	             RW_IN32  303	//IN32-开入继电器字32
#define	             RW_IN33  304	//IN33-开入继电器字33
#define	             RW_IN34  305	//IN34-开入继电器字34
#define	             RW_IN35  306	//IN35-信号复归
#define	             RW_IN36  307	//IN36-跳闸位置
#define	             RW_IN37  308	//IN37-合闸位置
#define	             RW_IN38  309	//IN38-遥控预置
#define	             RW_IN39  310	//IN39-遥控1预置
#define	             RW_IN40  311	//IN40-遥控2预置
#define	           RW_INVIN1  312	//INVIN1-开入继电器字41
#define	           RW_INVIN2  313	//INVIN2-开入继电器字42
#define	           RW_INVIN3  314	//INVIN3-开入继电器字43
#define	           RW_INVIN4  315	//INVIN4-开入继电器字44
#define	           RW_INVIN5  316	//INVIN5-开入继电器字45
#define	           RW_INVIN6  317	//INVIN6-开入继电器字46
#define	           RW_INVIN7  318	//INVIN7-开入继电器字47
#define	           RW_INVIN8  319	//INVIN8-开入继电器字48
#define	           RW_INVIN9  320	//INVIN9-开入继电器字49
#define	          RW_INVIN10  321	//INVIN10-开入继电器字50
#define	          RW_INVIN11  322	//INVIN11-开入继电器字51
#define	          RW_INVIN12  323	//INVIN12-开入继电器字52
#define	          RW_INVIN13  324	//INVIN13-开入继电器字53
#define	          RW_INVIN14  325	//INVIN14-开入继电器字54
#define	          RW_INVIN15  326	//INVIN15-开入继电器字55
#define	          RW_INVIN16  327	//INVIN16-开入继电器字56
#define	             RW_OUT1  328	//OUT1-开出1继电器字
#define	             RW_OUT2  329	//OUT2-开出2继电器字
#define	             RW_OUT3  330	//OUT3-开出3继电器字
#define	             RW_OUT4  331	//OUT4-开出4继电器字
#define	             RW_OUT5  332	//OUT5-开出5继电器字
#define	             RW_OUT6  333	//OUT6-开出6继电器字
#define	             RW_OUT7  334	//OUT7-开出7继电器字
#define	             RW_OUT8  335	//OUT8-开出8继电器字
#define	             RW_OUT9  336	//OUT9-开出9继电器字
#define	            RW_OUT10  337	//OUT10-开出10继电器字
#define	            RW_OUT11  338	//OUT11-开出11继电器字
#define	            RW_OUT12  339	//OUT12-开出12继电器字
#define	            RW_OUT13  340	//OUT13-开出13继电器字
#define	            RW_OUT14  341	//OUT14-开出14继电器字
#define	           RW_SL50_0  342	//SL50_0-电流速断压板继电器字
#define	           RW_SL50_1  343	//SL50_1-过流保护压板继电器字
#define	             RW_SL59  344	//SL59-过压保护压板继电器字
#define	             RW_SL27  345	//SL27-低压保护压板继电器字
#define	            RW_SL50H  346	//SL50H-谐波过流保护压板继电器字
#define	           RW_SL50ST  347	//SL50ST-支路过流保护压板继电器字
#define	            RW_SL60I  348	//SL60I-差流保护压板继电器字
#define	            RW_SL60U  349	//SL60U-差压保护压板继电器字
#define	          RW_SLMAINT  350	//SLMAINT-检修压板压板继电器字
#define	           RW_SLBAK1  351	//SLBAK1-备用压板1
#define	           RW_SLBAK2  352	//SLBAK2-备用压板2
#define	            RW_F50_0  353	//F50_0-速断保护配置继电器字
#define	            RW_F50_1  354	//F50_1-过流保护配置继电器字
#define	             RW_F50H  355	//F50H-谐波过流保护配置继电器字
#define	              RW_F59  356	//F59-过压保护配置继电器字
#define	              RW_F27  357	//F27-低压保护配置继电器字
#define	            RW_F60I1  358	//F60I1-支路1差流配置继电器字
#define	            RW_F60I2  359	//F60I2-支路2差流配置继电器字
#define	            RW_F50ST  360	//F50ST-支路过流配置继电器字
#define	            RW_F60U1  361	//F60U1-支路1差压配置继电器字
#define	            RW_F60U2  362	//F60U2-支路2差压配置继电器字
#define	            RW_F60U3  363	//F60U3-支路3差压配置继电器字
#define	            RW_F74PT  364	//F74PT-PT断线检查配置继电器字
#define	            RW_FCTBR  365	//FCTBR-CT断线检查配置继电器字
#define	            RW_F52BF  366	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  367	//FBAK1-备用配置1
#define	            RW_FBAK2  368	//FBAK2-备用配置2
#define	              RW_PW1  369	//PW1-脉冲继电器1
#define	              RW_PW2  370	//PW2-脉冲继电器2
#define	              RW_PW3  371	//PW3-脉冲继电器3
#define	              RW_PW4  372	//PW4-脉冲继电器4
#define	              RW_PW5  373	//PW5-脉冲继电器5
#define	              RW_PW6  374	//PW6-脉冲继电器6
#define	              RW_PW7  375	//PW7-脉冲继电器7
#define	              RW_PW8  376	//PW8-脉冲继电器8
#define	              RW_PW9  377	//PW9-脉冲继电器9
#define	             RW_PW10  378	//PW10-脉冲继电器10
#define	             RW_PW11  379	//PW11-脉冲继电器11
#define	             RW_PW12  380	//PW12-脉冲继电器12
#define	             RW_PW13  381	//PW13-脉冲继电器13
#define	             RW_PW14  382	//PW14-脉冲继电器14
#define	             RW_PW15  383	//PW15-脉冲继电器15
#define	             RW_PW16  384	//PW16-脉冲继电器16
#define	              RW_TO1  385	//TO1-传动继电器1
#define	              RW_TO2  386	//TO2-传动继电器2
#define	              RW_TO3  387	//TO3-传动继电器3
#define	              RW_TO4  388	//TO4-传动继电器4
#define	              RW_TO5  389	//TO5-传动继电器5
#define	              RW_TO6  390	//TO6-传动继电器6
#define	              RW_TO7  391	//TO7-传动继电器7
#define	             RW_SIG1  392	//SIG1-信号继电器1
#define	             RW_SIG2  393	//SIG2-信号继电器2
#define	             RW_SIG3  394	//SIG3-信号继电器3
#define	             RW_SIG4  395	//SIG4-信号继电器4
#define	             RW_SIG5  396	//SIG5-信号继电器5
#define	             RW_SIG6  397	//SIG6-信号继电器6
#define	             RW_SIG7  398	//SIG7-信号继电器7
#define	             RW_SIG8  399	//SIG8-信号继电器8
#define	             RW_SIG9  400	//SIG9-信号继电器9
#define	            RW_SIG10  401	//SIG10-信号继电器10
#define	            RW_SIG11  402	//SIG11-信号继电器11
#define	            RW_SIG12  403	//SIG12-信号继电器12
#define	            RW_SIG13  404	//SIG13-信号继电器13
#define	            RW_SIG14  405	//SIG14-信号继电器14
#define	            RW_SIG15  406	//SIG15-信号继电器15
#define	            RW_SIG16  407	//SIG16-信号继电器16
#define	            RW_LED1G  408	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  409	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  410	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  411	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  412	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  413	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  414	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  415	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  416	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  417	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  418	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  419	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  420	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  421	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  422	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  423	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  424	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  425	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  426	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  427	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  428	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  429	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  430	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  431	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  432	//VIN1-虚拟开入继电器字1
#define	             RW_VIN2  433	//VIN2-虚拟开入继电器字2
#define	             RW_VIN3  434	//VIN3-虚拟开入继电器字3
#define	             RW_VIN4  435	//VIN4-虚拟开入继电器字4
#define	             RW_VIN5  436	//VIN5-虚拟开入继电器字5
#define	             RW_VIN6  437	//VIN6-虚拟开入继电器字6
#define	             RW_VIN7  438	//VIN7-虚拟开入继电器字7
#define	             RW_VIN8  439	//VIN8-虚拟开入继电器字8
#define	             RW_VIN9  440	//VIN9-虚拟开入继电器字9
#define	            RW_VIN10  441	//VIN10-虚拟开入继电器字10
#define	            RW_VIN11  442	//VIN11-虚拟开入继电器字11
#define	            RW_VIN12  443	//VIN12-虚拟开入继电器字12
#define	            RW_VIN13  444	//VIN13-虚拟开入继电器字13
#define	            RW_VIN14  445	//VIN14-虚拟开入继电器字14
#define	            RW_VIN15  446	//VIN15-虚拟开入继电器字15
#define	            RW_VIN16  447	//VIN16-虚拟开入继电器字16
#define	             RW_RCP1  448	//RCP1-遥控预置继电器1
#define	             RW_RCP2  449	//RCP2-遥控预置继电器2
#define	             RW_RCP3  450	//RCP3-遥控预置继电器3
#define	             RW_RCP4  451	//RCP4-遥控预置继电器4
#define	             RW_RCP5  452	//RCP5-遥控预置继电器5
#define	             RW_RCP6  453	//RCP6-遥控预置继电器6
#define	             RW_RCP7  454	//RCP7-遥控预置继电器7
#define	             RW_RCP8  455	//RCP8-遥控预置继电器8
#define	             RW_RCP9  456	//RCP9-遥控预置继电器9
#define	            RW_RCP10  457	//RCP10-遥控预置继电器10
#define	            RW_RCP11  458	//RCP11-遥控预置继电器11
#define	            RW_RCP12  459	//RCP12-遥控预置继电器12
#define	            RW_RCP13  460	//RCP13-遥控预置继电器13
#define	            RW_RCP14  461	//RCP14-遥控预置继电器14
#define	            RW_RCP15  462	//RCP15-遥控预置继电器15
#define	            RW_RCP16  463	//RCP16-遥控预置继电器16
#define	            RW_RCP17  464	//RCP17-遥控预置继电器17
#define	            RW_RCP18  465	//RCP18-遥控预置继电器18
#define	            RW_RCP19  466	//RCP19-遥控预置继电器19
#define	            RW_RCP20  467	//RCP20-遥控预置继电器20
#define	            RW_RCP21  468	//RCP21-遥控预置继电器21
#define	            RW_RCP22  469	//RCP22-遥控预置继电器22
#define	            RW_RCP23  470	//RCP23-遥控预置继电器23
#define	            RW_RCP24  471	//RCP24-遥控预置继电器24
#define	            RW_RCP25  472	//RCP25-遥控预置继电器25
#define	            RW_RCP26  473	//RCP26-遥控预置继电器26
#define	            RW_RCP27  474	//RCP27-遥控预置继电器27
#define	            RW_RCP28  475	//RCP28-遥控预置继电器28
#define	            RW_RCP29  476	//RCP29-遥控预置继电器29
#define	            RW_RCP30  477	//RCP30-遥控预置继电器30
#define	            RW_RCP31  478	//RCP31-遥控预置继电器31
#define	            RW_RCP32  479	//RCP32-遥控预置继电器32
#define	             RW_RCE1  480	//RCE1-遥控执行继电器1
#define	             RW_RCE2  481	//RCE2-遥控执行继电器2
#define	             RW_RCE3  482	//RCE3-遥控执行继电器3
#define	             RW_RCE4  483	//RCE4-遥控执行继电器4
#define	             RW_RCE5  484	//RCE5-遥控执行继电器5
#define	             RW_RCE6  485	//RCE6-遥控执行继电器6
#define	             RW_RCE7  486	//RCE7-遥控执行继电器7
#define	             RW_RCE8  487	//RCE8-遥控执行继电器8
#define	             RW_RCE9  488	//RCE9-遥控执行继电器9
#define	            RW_RCE10  489	//RCE10-遥控执行继电器10
#define	            RW_RCE11  490	//RCE11-遥控执行继电器11
#define	            RW_RCE12  491	//RCE12-遥控执行继电器12
#define	            RW_RCE13  492	//RCE13-遥控执行继电器13
#define	            RW_RCE14  493	//RCE14-遥控执行继电器14
#define	            RW_RCE15  494	//RCE15-遥控执行继电器15
#define	            RW_RCE16  495	//RCE16-遥控执行继电器16
#define	            RW_RCE17  496	//RCE17-遥控执行继电器17
#define	            RW_RCE18  497	//RCE18-遥控执行继电器18
#define	            RW_RCE19  498	//RCE19-遥控执行继电器19
#define	            RW_RCE20  499	//RCE20-遥控执行继电器20
#define	            RW_RCE21  500	//RCE21-遥控执行继电器21
#define	            RW_RCE22  501	//RCE22-遥控执行继电器22
#define	            RW_RCE23  502	//RCE23-遥控执行继电器23
#define	            RW_RCE24  503	//RCE24-遥控执行继电器24
#define	            RW_RCE25  504	//RCE25-遥控执行继电器25
#define	            RW_RCE26  505	//RCE26-遥控执行继电器26
#define	            RW_RCE27  506	//RCE27-遥控执行继电器27
#define	            RW_RCE28  507	//RCE28-遥控执行继电器28
#define	            RW_RCE29  508	//RCE29-遥控执行继电器29
#define	            RW_RCE30  509	//RCE30-遥控执行继电器30
#define	            RW_RCE31  510	//RCE31-遥控执行继电器31
#define	            RW_RCE32  511	//RCE32-遥控执行继电器32
#define	             RW_RCV1  512	//RCV1-遥控反校继电器1
#define	             RW_RCV2  513	//RCV2-遥控反校继电器2
#define	             RW_RCV3  514	//RCV3-遥控反校继电器3
#define	             RW_RCV4  515	//RCV4-遥控反校继电器4
#define	             RW_RCV5  516	//RCV5-遥控反校继电器5
#define	             RW_RCV6  517	//RCV6-遥控反校继电器6
#define	             RW_RCV7  518	//RCV7-遥控反校继电器7
#define	             RW_RCV8  519	//RCV8-遥控反校继电器8
#define	             RW_RCV9  520	//RCV9-遥控反校继电器9
#define	            RW_RCV10  521	//RCV10-遥控反校继电器10
#define	            RW_RCV11  522	//RCV11-遥控反校继电器11
#define	            RW_RCV12  523	//RCV12-遥控反校继电器12
#define	            RW_RCV13  524	//RCV13-遥控反校继电器13
#define	            RW_RCV14  525	//RCV14-遥控反校继电器14
#define	            RW_RCV15  526	//RCV15-遥控反校继电器15
#define	            RW_RCV16  527	//RCV16-遥控反校继电器16
#define	            RW_RCV17  528	//RCV17-遥控反校继电器17
#define	            RW_RCV18  529	//RCV18-遥控反校继电器18
#define	            RW_RCV19  530	//RCV19-遥控反校继电器19
#define	            RW_RCV20  531	//RCV20-遥控反校继电器20
#define	            RW_RCV21  532	//RCV21-遥控反校继电器21
#define	            RW_RCV22  533	//RCV22-遥控反校继电器22
#define	            RW_RCV23  534	//RCV23-遥控反校继电器23
#define	            RW_RCV24  535	//RCV24-遥控反校继电器24
#define	            RW_RCV25  536	//RCV25-遥控反校继电器25
#define	            RW_RCV26  537	//RCV26-遥控反校继电器26
#define	            RW_RCV27  538	//RCV27-遥控反校继电器27
#define	            RW_RCV28  539	//RCV28-遥控反校继电器28
#define	            RW_RCV29  540	//RCV29-遥控反校继电器29
#define	            RW_RCV30  541	//RCV30-遥控反校继电器30
#define	            RW_RCV31  542	//RCV31-遥控反校继电器31
#define	            RW_RCV32  543	//RCV32-遥控反校继电器32
#define	              RW_GE1  544	//GE1-大于等于继电器1
#define	              RW_GE2  545	//GE2-大于等于继电器2
#define	              RW_GE3  546	//GE3-大于等于继电器3
#define	              RW_GE4  547	//GE4-大于等于继电器4
#define	              RW_GE5  548	//GE5-大于等于继电器5
#define	              RW_GE6  549	//GE6-大于等于继电器6
#define	              RW_GE7  550	//GE7-大于等于继电器7
#define	              RW_GE8  551	//GE8-大于等于继电器8
#define	           RW_CONST1  552	//CONST1-常数时间继电器1
#define	           RW_CONST2  553	//CONST2-常数时间继电器2
#define	           RW_CONST3  554	//CONST3-常数时间继电器3
#define	           RW_CONST4  555	//CONST4-常数时间继电器4
#define	           RW_CONST5  556	//CONST5-常数时间继电器5
#define	           RW_CONST6  557	//CONST6-常数时间继电器6
#define	           RW_CONST7  558	//CONST7-常数时间继电器7
#define	           RW_CONST8  559	//CONST8-常数时间继电器8
#define	              RW_TX1  560	//TX1-发送链接继电器字1
#define	              RW_TX2  561	//TX2-发送链接继电器字2
#define	              RW_TX3  562	//TX3-发送链接继电器字3
#define	              RW_TX4  563	//TX4-发送链接继电器字4
#define	              RW_TX5  564	//TX5-发送链接继电器字5
#define	              RW_TX6  565	//TX6-发送链接继电器字6
#define	              RW_TX7  566	//TX7-发送链接继电器字7
#define	              RW_TX8  567	//TX8-发送链接继电器字8
#define	              RW_TX9  568	//TX9-发送链接继电器字9
#define	             RW_TX10  569	//TX10-发送链接继电器字10
#define	             RW_TX11  570	//TX11-发送链接继电器字11
#define	             RW_TX12  571	//TX12-发送链接继电器字12
#define	             RW_TX13  572	//TX13-发送链接继电器字13
#define	             RW_TX14  573	//TX14-发送链接继电器字14
#define	             RW_TX15  574	//TX15-发送链接继电器字15
#define	             RW_TX16  575	//TX16-发送链接继电器字16
#define	              RW_RX1  576	//RX1-接收链接继电器字1
#define	              RW_RX2  577	//RX2-接收链接继电器字2
#define	              RW_RX3  578	//RX3-接收链接继电器字3
#define	              RW_RX4  579	//RX4-接收链接继电器字4
#define	              RW_RX5  580	//RX5-接收链接继电器字5
#define	              RW_RX6  581	//RX6-接收链接继电器字6
#define	              RW_RX7  582	//RX7-接收链接继电器字7
#define	              RW_RX8  583	//RX8-接收链接继电器字8
#define	              RW_RX9  584	//RX9-接收链接继电器字9
#define	             RW_RX10  585	//RX10-接收链接继电器字10
#define	             RW_RX11  586	//RX11-接收链接继电器字11
#define	             RW_RX12  587	//RX12-接收链接继电器字12
#define	             RW_RX13  588	//RX13-接收链接继电器字13
#define	             RW_RX14  589	//RX14-接收链接继电器字14
#define	             RW_RX15  590	//RX15-接收链接继电器字15
#define	             RW_RX16  591	//RX16-接收链接继电器字16
#define	             RW_CNT1  592	//CNT1-计数继电器字1
#define	             RW_CNT2  593	//CNT2-计数继电器字2
#define	             RW_CNT3  594	//CNT3-计数继电器字3
#define	             RW_CNT4  595	//CNT4-计数继电器字4
#define	             RW_CNT5  596	//CNT5-计数继电器字5
#define	             RW_CNT6  597	//CNT6-计数继电器字6
#define	             RW_CNT7  598	//CNT7-计数继电器字7
#define	             RW_CNT8  599	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			600		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			77	

//中间继电器字
#define	MID_RELAY_WORD_START			79	
#define	MID_RELAY_WORD_NUM			92	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			171	
#define	KEEP_RELAY_WORD_NUM			33	

//时间继电器字
#define	TIME_RELAY_WORD_START			204	
#define	TIME_RELAY_WORD_NUM			33	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			237	
#define	TRIP_RELAY_WORD_NUM			3	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			240	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			272	
#define	INPUT_RELAY_WORD_NUM			56	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			328	
#define	OUTPUT_RELAY_WORD_NUM			14	

//压板继电器
#define	SW_RELAY_WORD_START			342	
#define	SW_RELAY_WORD_NUM			11	

//配置继电器
#define	CFG_RELAY_WORD_START			353	
#define	CFG_RELAY_WORD_NUM			16	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			369	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			385	
#define	CTRLTEST_RELAY_WORD_NUM			7	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			392	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			408	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			432	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			448	
#define	RCP_RELAY_WORD_NUM			32	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			480	
#define	RCE_RELAY_WORD_NUM			32	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			512	
#define	RCV_RELAY_WORD_NUM			32	

//大于等于继电器
#define	GE_RELAY_WORD_START			544	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			552	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			560	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			576	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			592	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			592	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			592	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			592	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			592	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			592	
#define	CNT_RELAY_WORD_NUM			8	


#endif