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
#define	           RW_PI74PT    2	//PI74PT-PT断线入段继电器字
#define	           RW_AR74PT    3	//AR74PT-PT断线告警继电器字
#define	             RW_50I1    4	//50I1-IT1有流继电器字
#define	             RW_37I1    5	//37I1-IT1无流继电器字
#define	             RW_50I2    6	//50I2-IT2有流继电器字
#define	             RW_37I2    7	//37I2-IT2无流继电器字
#define	             RW_50I3    8	//50I3-IT3有流继电器字
#define	             RW_37I3    9	//37I3-IF3无流继电器字
#define	             RW_59U1   10	//59U1-U1有压继电器字
#define	             RW_27U1   11	//27U1-U1无压继电器字
#define	             RW_59U2   12	//59U2-U2有压继电器字
#define	             RW_27U2   13	//27U2-U2无压继电器字
#define	             RW_59U3   14	//59U3-U3有压继电器字
#define	             RW_27U3   15	//27U3-U3无压继电器字
#define	             RW_59U4   16	//59U4-U4有压继电器字
#define	             RW_27U4   17	//27U4-U4无压继电器字
#define	         RW_PI50DI_1   18	//PI50DI_1-下行增量保护入段继电器字
#define	         RW_TR50DI_1   19	//TR50DI_1-下行增量保护动作继电器字
#define	        RW_LPC50DI_1   20	//LPC50DI_1-下行增量保护循环控制继电器字
#define	         RW_PI50DI_2   21	//PI50DI_2-上行增量保护入段继电器字
#define	         RW_TR50DI_2   22	//TR50DI_2-上行增量保护动作继电器字
#define	        RW_LPC50DI_2   23	//LPC50DI_2-上行增量保护循环控制继电器字
#define	             RW_PI21   24	//PI21-距离保护入段继电器字
#define	             RW_TR21   25	//TR21-距离保护动作继电器字
#define	            RW_LPC21   26	//LPC21-距离保护循环控制继电器字
#define	            RW_PI50I   27	//PI50I-过流保护入段继电器字
#define	            RW_TR50I   28	//TR50I-过流保护动作继电器字
#define	           RW_LPC50I   29	//LPC50I-过流保护循环控制继电器字
#define	             RW_LKH2   30	//LKH2-二次谐波闭锁继电器字
#define	        RW_RELPICKUP   31	//RELPICKUP-保护启动继电器字
#define	        RW_AFLPICKUP   32	//AFLPICKUP-测距启动继电器字
#define	        RW_MFLPICKUP   33	//MFLPICKUP-实验测距继电器字
#define	           RW_AFLACT   34	//AFLACT-保护测距动作继电器字
#define	           RW_MFLACT   35	//MFLACT-实验测距动作继电器字
#define	           RW_CH1ERR   36	//CH1ERR-通道1故障继电器字
#define	           RW_CH2ERR   37	//CH2ERR-通道2故障继电器字
#define	           RW_CH3ERR   38	//CH3ERR-通道3故障继电器字
#define	           RW_CH4ERR   39	//CH4ERR-通道4故障继电器字
#define	           RW_CH5ERR   40	//CH5ERR-通道5故障继电器字
#define	          RW_ET11ERR   41	//ET11ERR-以太网1连接1继电器字
#define	          RW_ET12ERR   42	//ET12ERR-以太网1连接2继电器字
#define	          RW_ET13ERR   43	//ET13ERR-以太网1连接3继电器字
#define	          RW_ET14ERR   44	//ET14ERR-以太网1连接4继电器字
#define	          RW_ET15ERR   45	//ET15ERR-以太网1连接5继电器字
#define	          RW_ET21ERR   46	//ET21ERR-以太网2连接1继电器字
#define	          RW_ET22ERR   47	//ET22ERR-以太网2连接2继电器字
#define	          RW_ET23ERR   48	//ET23ERR-以太网2连接3继电器字
#define	          RW_ET24ERR   49	//ET24ERR-以太网2连接4继电器字
#define	          RW_ET25ERR   50	//ET25ERR-以太网2连接5继电器字
#define	          RW_COM1ERR   51	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   52	//CAN1ERR-CAN1连接状态继电器字
#define	          RW_CAN2ERR   53	//CAN2ERR-CAN2连接状态继电器字
#define	         RW_NULL1ERR   54	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR   55	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR   56	//NULL3ERR-空3连接状态继电器字
#define	     RW_HANDOVERMODE   57	//HANDOVERMODE-越区供电模式
#define	           RW_GPSERR   58	//GPSERR-本所GPS故障
#define	        RW_MAINTLINK   59	//MAINTLINK-维护状态继电器字
#define	             RW_69IN   60	//69IN-远方当地中间继电器字
#define	              RW_52A   61	//52A-合位
#define	              RW_52B   62	//52B-分位
#define	               RW_M1   63	//M1-中间继电器字M1
#define	               RW_M2   64	//M2-中间继电器字M2
#define	               RW_M3   65	//M3-中间继电器字M3
#define	               RW_M4   66	//M4-中间继电器字M4
#define	               RW_M5   67	//M5-中间继电器字M5
#define	               RW_M6   68	//M6-中间继电器字M6
#define	               RW_M7   69	//M7-中间继电器字M7
#define	               RW_M8   70	//M8-中间继电器字M8
#define	               RW_M9   71	//M9-中间继电器字M9
#define	              RW_M10   72	//M10-中间继电器字M10
#define	              RW_M11   73	//M11-中间继电器字M11
#define	              RW_M12   74	//M12-中间继电器字M12
#define	              RW_M13   75	//M13-中间继电器字M13
#define	              RW_M14   76	//M14-中间继电器字M14
#define	              RW_M15   77	//M15-中间继电器字M15
#define	              RW_M16   78	//M16-中间继电器字M16
#define	              RW_M17   79	//M17-中间继电器字M17
#define	              RW_M18   80	//M18-中间继电器字M18
#define	              RW_M19   81	//M19-中间继电器字M19
#define	              RW_M20   82	//M20-中间继电器字M20
#define	              RW_M21   83	//M21-中间继电器字M21
#define	              RW_M22   84	//M22-中间继电器字M22
#define	              RW_M23   85	//M23-中间继电器字M23
#define	              RW_M24   86	//M24-中间继电器字M24
#define	              RW_M25   87	//M25-中间继电器字M25
#define	              RW_M26   88	//M26-中间继电器字M26
#define	              RW_M27   89	//M27-中间继电器字M27
#define	              RW_M28   90	//M28-中间继电器字M28
#define	              RW_M29   91	//M29-中间继电器字M29
#define	              RW_M30   92	//M30-中间继电器字M30
#define	              RW_M31   93	//M31-中间继电器字M31
#define	              RW_M32   94	//M32-中间继电器字M32
#define	              RW_M33   95	//M33-中间继电器字M33
#define	              RW_M34   96	//M34-中间继电器字M34
#define	              RW_M35   97	//M35-中间继电器字M35
#define	              RW_M36   98	//M36-中间继电器字M36
#define	              RW_M37   99	//M37-中间继电器字M37
#define	              RW_M38  100	//M38-中间继电器字M38
#define	              RW_M39  101	//M39-中间继电器字M39
#define	              RW_M40  102	//M40-中间继电器字M40
#define	              RW_M41  103	//M41-中间继电器字M41
#define	              RW_M42  104	//M42-中间继电器字M42
#define	              RW_M43  105	//M43-中间继电器字M43
#define	              RW_M44  106	//M44-中间继电器字M44
#define	              RW_M45  107	//M45-中间继电器字M45
#define	              RW_M46  108	//M46-中间继电器字M46
#define	              RW_M47  109	//M47-中间继电器字M47
#define	              RW_M48  110	//M48-中间继电器字M48
#define	              RW_M49  111	//M49-中间继电器字M49
#define	              RW_M50  112	//M50-中间继电器字M50
#define	              RW_M51  113	//M51-中间继电器字M51
#define	              RW_M52  114	//M52-中间继电器字M52
#define	              RW_M53  115	//M53-中间继电器字M53
#define	              RW_M54  116	//M54-中间继电器字M54
#define	              RW_M55  117	//M55-中间继电器字M55
#define	              RW_M56  118	//M56-中间继电器字M56
#define	              RW_M57  119	//M57-中间继电器字M57
#define	              RW_M58  120	//M58-中间继电器字M58
#define	              RW_M59  121	//M59-中间继电器字M59
#define	              RW_M60  122	//M60-中间继电器字M60
#define	              RW_M61  123	//M61-中间继电器字M61
#define	              RW_M62  124	//M62-中间继电器字M62
#define	              RW_M63  125	//M63-中间继电器字M63
#define	              RW_M64  126	//M64-中间继电器字M64
#define	              RW_REC  127	//REC-录波继电器字
#define	             RW_LKCL  128	//LKCL-合闸闭锁继电器字
#define	             RW_LKTR  129	//LKTR-分闸闭锁继电器字
#define	       RW_LKPI50DI_1  130	//LKPI50DI_1-下行增量保护入段闭锁继电器字
#define	      RW_LKRST50DI_1  131	//LKRST50DI_1-下行增量保护返回闭锁继电器字
#define	       RW_LKPI50DI_2  132	//LKPI50DI_2-上行增量保护入段闭锁继电器字
#define	      RW_LKRST50DI_2  133	//LKRST50DI_2-上行增量保护返回闭锁继电器字
#define	           RW_LKPI21  134	//LKPI21-距离保护入段闭锁继电器字
#define	          RW_LKRST21  135	//LKRST21-距离保护返回闭锁继电器字
#define	          RW_LKPI50I  136	//LKPI50I-过流保护入段闭锁继电器字
#define	         RW_LKRST50I  137	//LKRST50I-过流保护返回闭锁继电器字
#define	           RW_MSTART  138	//MSTART-实验启动测距继电器字
#define	       RW_GETHISTORY  139	//GETHISTORY-召测历史数据
#define	        RW_GETLOWVOL  140	//GETLOWVOL-召测低压锁定数据
#define	           RW_LKFLPI  141	//LKFLPI-测距启动闭锁继电器字
#define	         RW_HANDOVER  142	//HANDOVER-越区切换继电器字
#define	          RW_RECOVER  143	//RECOVER-越区恢复继电器字
#define	          RW_BGK152A  144	//BGK152A-备用隔开1下行位置继电器字
#define	          RW_BGK252A  145	//BGK252A-备用隔开2上行位置继电器字
#define	               RW_H1  146	//H1-自保持中间继电器字1
#define	               RW_H2  147	//H2-自保持中间继电器字2
#define	               RW_H3  148	//H3-自保持中间继电器字3
#define	               RW_H4  149	//H4-自保持中间继电器字4
#define	               RW_H5  150	//H5-自保持中间继电器字5
#define	               RW_H6  151	//H6-自保持中间继电器字6
#define	               RW_H7  152	//H7-自保持中间继电器字7
#define	               RW_H8  153	//H8-自保持中间继电器字8
#define	               RW_H9  154	//H9-自保持中间继电器字9
#define	              RW_H10  155	//H10-自保持中间继电器字10
#define	              RW_H11  156	//H11-自保持中间继电器字11
#define	              RW_H12  157	//H12-自保持中间继电器字12
#define	              RW_H13  158	//H13-自保持中间继电器字13
#define	              RW_H14  159	//H14-自保持中间继电器字14
#define	              RW_H15  160	//H15-自保持中间继电器字15
#define	              RW_H16  161	//H16-自保持中间继电器字16
#define	              RW_H17  162	//H17-自保持中间继电器字17
#define	              RW_H18  163	//H18-自保持中间继电器字18
#define	              RW_H19  164	//H19-自保持中间继电器字19
#define	              RW_H20  165	//H20-自保持中间继电器字20
#define	              RW_H21  166	//H21-自保持中间继电器字21
#define	              RW_H22  167	//H22-自保持中间继电器字22
#define	              RW_H23  168	//H23-自保持中间继电器字23
#define	              RW_H24  169	//H24-自保持中间继电器字24
#define	              RW_H25  170	//H25-自保持中间继电器字25
#define	              RW_H26  171	//H26-自保持中间继电器字26
#define	              RW_H27  172	//H27-自保持中间继电器字27
#define	              RW_H28  173	//H28-自保持中间继电器字28
#define	              RW_H29  174	//H29-自保持中间继电器字29
#define	              RW_H30  175	//H30-自保持中间继电器字30
#define	              RW_H31  176	//H31-自保持中间继电器字31
#define	              RW_H32  177	//H32-自保持中间继电器字32
#define	             RW_52BF  178	//52BF-断路器跳闸失败继电器字
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
#define	              RW_TR1  212	//TR1-跳闸继电器1
#define	              RW_TR2  213	//TR2-跳闸继电器2
#define	              RW_VR1  214	//VR1-虚拟继电器字1
#define	              RW_VR2  215	//VR2-虚拟继电器字2
#define	              RW_VR3  216	//VR3-虚拟继电器字3
#define	              RW_VR4  217	//VR4-虚拟继电器字4
#define	              RW_VR5  218	//VR5-虚拟继电器字5
#define	              RW_VR6  219	//VR6-虚拟继电器字6
#define	              RW_VR7  220	//VR7-虚拟继电器字7
#define	              RW_VR8  221	//VR8-虚拟继电器字8
#define	              RW_VR9  222	//VR9-虚拟继电器字9
#define	             RW_VR10  223	//VR10-虚拟继电器字10
#define	             RW_VR11  224	//VR11-虚拟继电器字11
#define	             RW_VR12  225	//VR12-虚拟继电器字12
#define	             RW_VR13  226	//VR13-虚拟继电器字13
#define	             RW_VR14  227	//VR14-虚拟继电器字14
#define	             RW_VR15  228	//VR15-虚拟继电器字15
#define	             RW_VR16  229	//VR16-虚拟继电器字16
#define	             RW_VR17  230	//VR17-虚拟继电器字17
#define	             RW_VR18  231	//VR18-虚拟继电器字18
#define	             RW_VR19  232	//VR19-虚拟继电器字19
#define	             RW_VR20  233	//VR20-虚拟继电器字20
#define	             RW_VR21  234	//VR21-虚拟继电器字21
#define	             RW_VR22  235	//VR22-虚拟继电器字22
#define	             RW_VR23  236	//VR23-虚拟继电器字23
#define	             RW_VR24  237	//VR24-虚拟继电器字24
#define	             RW_VR25  238	//VR25-虚拟继电器字25
#define	             RW_VR26  239	//VR26-虚拟继电器字26
#define	             RW_VR27  240	//VR27-虚拟继电器字27
#define	             RW_VR28  241	//VR28-虚拟继电器字28
#define	             RW_VR29  242	//VR29-虚拟继电器字29
#define	             RW_VR30  243	//VR30-虚拟继电器字30
#define	             RW_VR31  244	//VR31-虚拟继电器字31
#define	             RW_VR32  245	//VR32-虚拟继电器字32
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
#define	             RW_IN35  280	//IN35-信号复归
#define	             RW_IN36  281	//IN36-跳闸位置
#define	             RW_IN37  282	//IN37-合闸位置
#define	             RW_IN38  283	//IN38-遥控预置
#define	             RW_IN39  284	//IN39-遥控1预置
#define	             RW_IN40  285	//IN40-遥控2预置
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
#define	             RW_OUT1  302	//OUT1-开出1继电器字
#define	             RW_OUT2  303	//OUT2-开出2继电器字
#define	             RW_OUT3  304	//OUT3-开出3继电器字
#define	             RW_OUT4  305	//OUT4-开出4继电器字
#define	             RW_OUT5  306	//OUT5-开出5继电器字
#define	             RW_OUT6  307	//OUT6-开出6继电器字
#define	             RW_OUT7  308	//OUT7-开出7继电器字
#define	             RW_OUT8  309	//OUT8-开出8继电器字
#define	             RW_OUT9  310	//OUT9-开出9继电器字
#define	            RW_OUT10  311	//OUT10-开出10继电器字
#define	            RW_OUT11  312	//OUT11-开出11继电器字
#define	            RW_OUT12  313	//OUT12-开出12继电器字
#define	            RW_OUT13  314	//OUT13-开出13继电器字
#define	            RW_OUT14  315	//OUT14-开出14继电器字
#define	             RW_SLFL  316	//SLFL-故障测距压板继电器字
#define	          RW_SLMAINT  317	//SLMAINT-检修压板压板继电器字
#define	          RW_SLRESD1  318	//SLRESD1-备用压板1
#define	          RW_SLRESD2  319	//SLRESD2-备用压板2
#define	            RW_FPI_Z  320	//FPI_Z-阻抗启动配置继电器字
#define	            RW_FPI_C  321	//FPI_C-过流启动配置继电器字
#define	          RW_FPI_50D  322	//FPI_50D-增量启动配置继电器字
#define	         RW_FFL_AUTO  323	//FFL_AUTO-测距自适应
#define	          RW_FCCMODE  324	//FCCMODE-电流合成方式
#define	              RW_FAT  325	//FAT-AT吸上电流比配置继电器字
#define	              RW_FUD  326	//FUD-上下行电流比配置继电器字
#define	              RW_FNL  327	//FNL-吸馈电流比配置继电器字
#define	         RW_FPHASESC  328	//FPHASESC-相间短路判断配置继电器字
#define	            RW_FNOAT  329	//FNOAT-变电所无AT配置继电器字
#define	            RW_FISSS  330	//FISSS-本所为变电所配置继电器字
#define	          RW_F21_H_S  331	//F21_H_S-距离谐波抑制配置继电器字
#define	          RW_F21_H_L  332	//F21_H_L-距离谐波闭锁配置继电器字
#define	          RW_F50_H_S  333	//F50_H_S-电流谐波抑制配置继电器字
#define	          RW_F50_H_L  334	//F50_H_L-电流谐波抑制配置继电器字
#define	         RW_FPARALLG  335	//FPARALLG-平行四边形配置继电器字
#define	          RW_F50_27V  336	//F50_27V-过流低压闭锁配置继电器字
#define	            RW_FCALL  337	//FCALL-召测取低压数据
#define	             RW_FGPS  338	//FGPS-GPS故障检测
#define	         RW_FCHANNEL  339	//FCHANNEL-通道自动故障检测
#define	           RW_FCROSS  340	//FCROSS-允许越区配置继电器字
#define	           RW_FRESD1  341	//FRESD1-备用配置1
#define	           RW_FRESD2  342	//FRESD2-备用配置2
#define	              RW_PW1  343	//PW1-脉冲继电器1
#define	              RW_PW2  344	//PW2-脉冲继电器2
#define	              RW_PW3  345	//PW3-脉冲继电器3
#define	              RW_PW4  346	//PW4-脉冲继电器4
#define	              RW_PW5  347	//PW5-脉冲继电器5
#define	              RW_PW6  348	//PW6-脉冲继电器6
#define	              RW_PW7  349	//PW7-脉冲继电器7
#define	              RW_PW8  350	//PW8-脉冲继电器8
#define	              RW_PW9  351	//PW9-脉冲继电器9
#define	             RW_PW10  352	//PW10-脉冲继电器10
#define	             RW_PW11  353	//PW11-脉冲继电器11
#define	             RW_PW12  354	//PW12-脉冲继电器12
#define	             RW_PW13  355	//PW13-脉冲继电器13
#define	             RW_PW14  356	//PW14-脉冲继电器14
#define	             RW_PW15  357	//PW15-脉冲继电器15
#define	             RW_PW16  358	//PW16-脉冲继电器16
#define	              RW_TO1  359	//TO1-传动继电器1
#define	              RW_TO2  360	//TO2-传动继电器2
#define	              RW_TO3  361	//TO3-传动继电器3
#define	              RW_TO4  362	//TO4-传动继电器4
#define	              RW_TO5  363	//TO5-传动继电器5
#define	              RW_TO6  364	//TO6-传动继电器6
#define	              RW_TO7  365	//TO7-传动继电器7
#define	             RW_SIG1  366	//SIG1-信号继电器1
#define	             RW_SIG2  367	//SIG2-信号继电器2
#define	             RW_SIG3  368	//SIG3-信号继电器3
#define	             RW_SIG4  369	//SIG4-信号继电器4
#define	             RW_SIG5  370	//SIG5-信号继电器5
#define	             RW_SIG6  371	//SIG6-信号继电器6
#define	             RW_SIG7  372	//SIG7-信号继电器7
#define	             RW_SIG8  373	//SIG8-信号继电器8
#define	             RW_SIG9  374	//SIG9-信号继电器9
#define	            RW_SIG10  375	//SIG10-信号继电器10
#define	            RW_SIG11  376	//SIG11-信号继电器11
#define	            RW_SIG12  377	//SIG12-信号继电器12
#define	            RW_SIG13  378	//SIG13-信号继电器13
#define	            RW_SIG14  379	//SIG14-信号继电器14
#define	            RW_SIG15  380	//SIG15-信号继电器15
#define	            RW_SIG16  381	//SIG16-信号继电器16
#define	            RW_LED1G  382	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  383	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  384	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  385	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  386	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  387	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  388	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  389	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  390	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  391	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  392	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  393	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  394	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  395	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  396	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  397	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  398	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  399	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  400	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  401	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  402	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  403	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  404	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  405	//LED12R-LED12红灯继电器字
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
#define	             RW_RCP1  422	//RCP1-遥控预置继电器1
#define	             RW_RCP2  423	//RCP2-遥控预置继电器2
#define	             RW_RCP3  424	//RCP3-遥控预置继电器3
#define	             RW_RCP4  425	//RCP4-遥控预置继电器4
#define	             RW_RCP5  426	//RCP5-遥控预置继电器5
#define	             RW_RCP6  427	//RCP6-遥控预置继电器6
#define	             RW_RCP7  428	//RCP7-遥控预置继电器7
#define	             RW_RCP8  429	//RCP8-遥控预置继电器8
#define	             RW_RCP9  430	//RCP9-遥控预置继电器9
#define	            RW_RCP10  431	//RCP10-遥控预置继电器10
#define	            RW_RCP11  432	//RCP11-遥控预置继电器11
#define	            RW_RCP12  433	//RCP12-遥控预置继电器12
#define	            RW_RCP13  434	//RCP13-遥控预置继电器13
#define	            RW_RCP14  435	//RCP14-遥控预置继电器14
#define	            RW_RCP15  436	//RCP15-遥控预置继电器15
#define	            RW_RCP16  437	//RCP16-遥控预置继电器16
#define	            RW_RCP17  438	//RCP17-遥控预置继电器17
#define	            RW_RCP18  439	//RCP18-遥控预置继电器18
#define	            RW_RCP19  440	//RCP19-遥控预置继电器19
#define	            RW_RCP20  441	//RCP20-遥控预置继电器20
#define	            RW_RCP21  442	//RCP21-遥控预置继电器21
#define	            RW_RCP22  443	//RCP22-遥控预置继电器22
#define	            RW_RCP23  444	//RCP23-遥控预置继电器23
#define	            RW_RCP24  445	//RCP24-遥控预置继电器24
#define	            RW_RCP25  446	//RCP25-遥控预置继电器25
#define	            RW_RCP26  447	//RCP26-遥控预置继电器26
#define	            RW_RCP27  448	//RCP27-遥控预置继电器27
#define	            RW_RCP28  449	//RCP28-遥控预置继电器28
#define	            RW_RCP29  450	//RCP29-遥控预置继电器29
#define	            RW_RCP30  451	//RCP30-遥控预置继电器30
#define	            RW_RCP31  452	//RCP31-遥控预置继电器31
#define	            RW_RCP32  453	//RCP32-遥控预置继电器32
#define	            RW_RCP33  454	//RCP33-遥控预置继电器33
#define	            RW_RCP34  455	//RCP34-遥控预置继电器34
#define	            RW_RCP35  456	//RCP35-遥控预置继电器35
#define	            RW_RCP36  457	//RCP36-遥控预置继电器36
#define	             RW_RCE1  458	//RCE1-遥控执行继电器1
#define	             RW_RCE2  459	//RCE2-遥控执行继电器2
#define	             RW_RCE3  460	//RCE3-遥控执行继电器3
#define	             RW_RCE4  461	//RCE4-遥控执行继电器4
#define	             RW_RCE5  462	//RCE5-遥控执行继电器5
#define	             RW_RCE6  463	//RCE6-遥控执行继电器6
#define	             RW_RCE7  464	//RCE7-遥控执行继电器7
#define	             RW_RCE8  465	//RCE8-遥控执行继电器8
#define	             RW_RCE9  466	//RCE9-遥控执行继电器9
#define	            RW_RCE10  467	//RCE10-遥控执行继电器10
#define	            RW_RCE11  468	//RCE11-遥控执行继电器11
#define	            RW_RCE12  469	//RCE12-遥控执行继电器12
#define	            RW_RCE13  470	//RCE13-遥控执行继电器13
#define	            RW_RCE14  471	//RCE14-遥控执行继电器14
#define	            RW_RCE15  472	//RCE15-遥控执行继电器15
#define	            RW_RCE16  473	//RCE16-遥控执行继电器16
#define	            RW_RCE17  474	//RCE17-遥控执行继电器17
#define	            RW_RCE18  475	//RCE18-遥控执行继电器18
#define	            RW_RCE19  476	//RCE19-遥控执行继电器19
#define	            RW_RCE20  477	//RCE20-遥控执行继电器20
#define	            RW_RCE21  478	//RCE21-遥控执行继电器21
#define	            RW_RCE22  479	//RCE22-遥控执行继电器22
#define	            RW_RCE23  480	//RCE23-遥控执行继电器23
#define	            RW_RCE24  481	//RCE24-遥控执行继电器24
#define	            RW_RCE25  482	//RCE25-遥控执行继电器25
#define	            RW_RCE26  483	//RCE26-遥控执行继电器26
#define	            RW_RCE27  484	//RCE27-遥控执行继电器27
#define	            RW_RCE28  485	//RCE28-遥控执行继电器28
#define	            RW_RCE29  486	//RCE29-遥控执行继电器29
#define	            RW_RCE30  487	//RCE30-遥控执行继电器30
#define	            RW_RCE31  488	//RCE31-遥控执行继电器31
#define	            RW_RCE32  489	//RCE32-遥控执行继电器32
#define	            RW_RCE33  490	//RCE33-遥控执行继电器33
#define	            RW_RCE34  491	//RCE34-遥控执行继电器34
#define	            RW_RCE35  492	//RCE35-遥控执行继电器35
#define	            RW_RCE36  493	//RCE36-遥控执行继电器36
#define	             RW_RCV1  494	//RCV1-遥控反校继电器1
#define	             RW_RCV2  495	//RCV2-遥控反校继电器2
#define	             RW_RCV3  496	//RCV3-遥控反校继电器3
#define	             RW_RCV4  497	//RCV4-遥控反校继电器4
#define	             RW_RCV5  498	//RCV5-遥控反校继电器5
#define	             RW_RCV6  499	//RCV6-遥控反校继电器6
#define	             RW_RCV7  500	//RCV7-遥控反校继电器7
#define	             RW_RCV8  501	//RCV8-遥控反校继电器8
#define	             RW_RCV9  502	//RCV9-遥控反校继电器9
#define	            RW_RCV10  503	//RCV10-遥控反校继电器10
#define	            RW_RCV11  504	//RCV11-遥控反校继电器11
#define	            RW_RCV12  505	//RCV12-遥控反校继电器12
#define	            RW_RCV13  506	//RCV13-遥控反校继电器13
#define	            RW_RCV14  507	//RCV14-遥控反校继电器14
#define	            RW_RCV15  508	//RCV15-遥控反校继电器15
#define	            RW_RCV16  509	//RCV16-遥控反校继电器16
#define	            RW_RCV17  510	//RCV17-遥控反校继电器17
#define	            RW_RCV18  511	//RCV18-遥控反校继电器18
#define	            RW_RCV19  512	//RCV19-遥控反校继电器19
#define	            RW_RCV20  513	//RCV20-遥控反校继电器20
#define	            RW_RCV21  514	//RCV21-遥控反校继电器21
#define	            RW_RCV22  515	//RCV22-遥控反校继电器22
#define	            RW_RCV23  516	//RCV23-遥控反校继电器23
#define	            RW_RCV24  517	//RCV24-遥控反校继电器24
#define	            RW_RCV25  518	//RCV25-遥控反校继电器25
#define	            RW_RCV26  519	//RCV26-遥控反校继电器26
#define	            RW_RCV27  520	//RCV27-遥控反校继电器27
#define	            RW_RCV28  521	//RCV28-遥控反校继电器28
#define	            RW_RCV29  522	//RCV29-遥控反校继电器29
#define	            RW_RCV30  523	//RCV30-遥控反校继电器30
#define	            RW_RCV31  524	//RCV31-遥控反校继电器31
#define	            RW_RCV32  525	//RCV32-遥控反校继电器32
#define	            RW_RCV33  526	//RCV33-遥控反校继电器33
#define	            RW_RCV34  527	//RCV34-遥控反校继电器34
#define	            RW_RCV35  528	//RCV35-遥控反校继电器35
#define	            RW_RCV36  529	//RCV36-遥控反校继电器36
#define	              RW_GE1  530	//GE1-大于等于继电器1
#define	              RW_GE2  531	//GE2-大于等于继电器2
#define	              RW_GE3  532	//GE3-大于等于继电器3
#define	              RW_GE4  533	//GE4-大于等于继电器4
#define	              RW_GE5  534	//GE5-大于等于继电器5
#define	              RW_GE6  535	//GE6-大于等于继电器6
#define	              RW_GE7  536	//GE7-大于等于继电器7
#define	              RW_GE8  537	//GE8-大于等于继电器8
#define	           RW_CONST1  538	//CONST1-常数时间继电器1
#define	           RW_CONST2  539	//CONST2-常数时间继电器2
#define	           RW_CONST3  540	//CONST3-常数时间继电器3
#define	           RW_CONST4  541	//CONST4-常数时间继电器4
#define	           RW_CONST5  542	//CONST5-常数时间继电器5
#define	           RW_CONST6  543	//CONST6-常数时间继电器6
#define	           RW_CONST7  544	//CONST7-常数时间继电器7
#define	             RW_TLR1  545	//TLR1-发送链接继电器字1
#define	             RW_TLR2  546	//TLR2-发送链接继电器字2
#define	             RW_TLR3  547	//TLR3-发送链接继电器字3
#define	             RW_TLR4  548	//TLR4-发送链接继电器字4
#define	             RW_TLR5  549	//TLR5-发送链接继电器字5
#define	             RW_TLR6  550	//TLR6-发送链接继电器字6
#define	             RW_TLR7  551	//TLR7-发送链接继电器字7
#define	             RW_TLR8  552	//TLR8-发送链接继电器字8
#define	             RW_TLR9  553	//TLR9-发送链接继电器字9
#define	            RW_TLR10  554	//TLR10-发送链接继电器字10
#define	            RW_TLR11  555	//TLR11-发送链接继电器字11
#define	            RW_TLR12  556	//TLR12-发送链接继电器字12
#define	            RW_TLR13  557	//TLR13-发送链接继电器字13
#define	            RW_TLR14  558	//TLR14-发送链接继电器字14
#define	            RW_TLR15  559	//TLR15-发送链接继电器字15
#define	            RW_TLR16  560	//TLR16-发送链接继电器字16
#define	             RW_RLR1  561	//RLR1-接收链接继电器字1
#define	             RW_RLR2  562	//RLR2-接收链接继电器字2
#define	             RW_RLR3  563	//RLR3-接收链接继电器字3
#define	             RW_RLR4  564	//RLR4-接收链接继电器字4
#define	             RW_RLR5  565	//RLR5-接收链接继电器字5
#define	             RW_RLR6  566	//RLR6-接收链接继电器字6
#define	             RW_RLR7  567	//RLR7-接收链接继电器字7
#define	             RW_RLR8  568	//RLR8-接收链接继电器字8
#define	             RW_RLR9  569	//RLR9-接收链接继电器字9
#define	            RW_RLR10  570	//RLR10-接收链接继电器字10
#define	            RW_RLR11  571	//RLR11-接收链接继电器字11
#define	            RW_RLR12  572	//RLR12-接收链接继电器字12
#define	            RW_RLR13  573	//RLR13-接收链接继电器字13
#define	            RW_RLR14  574	//RLR14-接收链接继电器字14
#define	            RW_RLR15  575	//RLR15-接收链接继电器字15
#define	            RW_RLR16  576	//RLR16-接收链接继电器字16
#define	             RW_CNT1  577	//CNT1-计数器继电器字1
#define	             RW_CNT2  578	//CNT2-计数器继电器字2
#define	             RW_CNT3  579	//CNT3-计数器继电器字3
#define	             RW_CNT4  580	//CNT4-计数器继电器字4
#define	             RW_CNT5  581	//CNT5-计数器继电器字5
#define	             RW_CNT6  582	//CNT6-计数器继电器字6
#define	             RW_CNT7  583	//CNT7-计数器继电器字7
#define	             RW_CNT8  584	//CNT8-计数器继电器字8
#define	RELAY_WORD_NUM			585		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			58	

//中间继电器字
#define	MID_RELAY_WORD_START			60	
#define	MID_RELAY_WORD_NUM			86	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			146	
#define	KEEP_RELAY_WORD_NUM			33	

//时间继电器字
#define	TIME_RELAY_WORD_START			179	
#define	TIME_RELAY_WORD_NUM			33	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			212	
#define	TRIP_RELAY_WORD_NUM			2	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			214	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			246	
#define	INPUT_RELAY_WORD_NUM			56	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			302	
#define	OUTPUT_RELAY_WORD_NUM			14	

//压板继电器
#define	SW_RELAY_WORD_START			316	
#define	SW_RELAY_WORD_NUM			4	

//配置继电器
#define	CFG_RELAY_WORD_START			320	
#define	CFG_RELAY_WORD_NUM			23	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			343	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			359	
#define	CTRLTEST_RELAY_WORD_NUM			7	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			366	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			382	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			406	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			422	
#define	RCP_RELAY_WORD_NUM			36	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			458	
#define	RCE_RELAY_WORD_NUM			36	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			494	
#define	RCV_RELAY_WORD_NUM			36	

//大于等于继电器
#define	GE_RELAY_WORD_START			530	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			538	
#define	CONST_TIME_RW_NUM			7	

//发送链接继电器
#define	TXLINK_RW_START			545	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			561	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			577	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			577	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			577	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			577	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			577	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			577	
#define	CNT_RELAY_WORD_NUM			8	


#endif