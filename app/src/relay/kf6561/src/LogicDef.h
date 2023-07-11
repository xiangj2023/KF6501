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
#define	           RW_PI50I1    2	//PI50I1-过流保护入段继电器字
#define	           RW_TR50I1    3	//TR50I1-过流保护动作继电器字
#define	          RW_LPC50I1    4	//LPC50I1-过流保护循环控制继电器字
#define	           RW_PI50I0    5	//PI50I0-速断保护入段继电器字
#define	           RW_TR50I0    6	//TR50I0-速断保护动作继电器字
#define	          RW_LPC50I0    7	//LPC50I0-速断保护循环控制继电器字
#define	            RW_PI59U    8	//PI59U-过压保护入段继电器字
#define	            RW_TR59U    9	//TR59U-过压保护动作继电器字
#define	           RW_LPC59U   10	//LPC59U-过压保护循环控制继电器字
#define	            RW_PI60I   11	//PI60I-不平衡电流保护入段继电器字
#define	            RW_TR60I   12	//TR60I-不平衡电流保护动作继电器字
#define	           RW_LPC60I   13	//LPC60I-不平衡电流保护循环控制继电器字
#define	            RW_PI60U   14	//PI60U-不平衡电压保护入段继电器字
#define	            RW_TR60U   15	//TR60U-不平衡电压保护动作继电器字
#define	           RW_LPC60U   16	//LPC60U-不平衡电压保护循环控制继电器字
#define	            RW_PI27U   17	//PI27U-低压保护入段继电器字
#define	            RW_TR27U   18	//TR27U-低压保护动作继电器字
#define	           RW_LPC27U   19	//LPC27U-低压保护循环控制继电器字
#define	            RW_PIPT1   20	//PIPT1-母线PT断线入段继电器字
#define	            RW_ARPT1   21	//ARPT1-母线PT断线告警继电器字
#define	            RW_PI59N   22	//PI59N-零压入段继电器字
#define	            RW_TR59N   23	//TR59N-零压动作继电器字
#define	           RW_LPC59N   24	//LPC59N-零压保护循环控制继电器字
#define	             RW_59UA   25	//59UA-UA有压继电器字
#define	             RW_27UA   26	//27UA-UA无压继电器字
#define	             RW_59UB   27	//59UB-UB有压继电器字
#define	             RW_27UB   28	//27UB-UB无压继电器字
#define	             RW_59UC   29	//59UC-UC有压继电器字
#define	             RW_27UC   30	//27UC-UC无压继电器字
#define	             RW_59U0   31	//59U0-U0有压继电器字
#define	             RW_27U0   32	//27U0-U0无压继电器字
#define	             RW_59U1   33	//59U1-U1有压继电器字
#define	             RW_27U1   34	//27U1-U1无压继电器字
#define	             RW_59U2   35	//59U2-U2有压继电器字
#define	             RW_27U2   36	//27U2-U2无压继电器字
#define	             RW_59U3   37	//59U3-U3有压继电器字
#define	             RW_27U3   38	//27U3-U3无压继电器字
#define	             RW_50IA   39	//50IA-IA有流继电器字
#define	             RW_37IA   40	//37IA-IA无流继电器字
#define	             RW_50IB   41	//50IB-IB有流继电器字
#define	             RW_37IB   42	//37IB-IB无流继电器字
#define	             RW_50IC   43	//50IC-IC有流继电器字
#define	             RW_37IC   44	//37IC-IC无流继电器字
#define	             RW_50I0   45	//50I0-I0有流继电器字
#define	             RW_37I0   46	//37I0-I0无流继电器字
#define	             RW_50I1   47	//50I1-I1有流继电器字
#define	             RW_37I1   48	//37I1-I1无流继电器字
#define	           RW_PICKUP   49	//PICKUP-模值突变量启动继电器字
#define	          RW_ET11ERR   50	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   51	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   52	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   53	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   54	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   55	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   56	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   57	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   58	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   59	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   60	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   61	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   62	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   63	//NULL1ERR-空连接1状态继电器字
#define	         RW_NULL2ERR   64	//NULL2ERR-空连接2状态继电器字
#define	         RW_NULL3ERR   65	//NULL3ERR-空连接3状态继电器字
#define	         RW_PICTBR_A   66	//PICTBR_A-A相CT断线入段继电器字
#define	         RW_ARCTBR_A   67	//ARCTBR_A-A相CT断线告警继电器字
#define	         RW_PICTBR_B   68	//PICTBR_B-B相CT断线入段继电器字
#define	         RW_ARCTBR_B   69	//ARCTBR_B-B相CT断线告警继电器字
#define	         RW_PICTBR_C   70	//PICTBR_C-C相CT断线入段继电器字
#define	         RW_ARCTBR_C   71	//ARCTBR_C-C相CT断线告警继电器字
#define	        RW_MAINTLINK   72	//MAINTLINK-维护状态继电器字
#define	               RW_M1   73	//M1-中间继电器字M1
#define	               RW_M2   74	//M2-中间继电器字M2
#define	               RW_M3   75	//M3-中间继电器字M3
#define	               RW_M4   76	//M4-中间继电器字M4
#define	               RW_M5   77	//M5-中间继电器字M5
#define	               RW_M6   78	//M6-中间继电器字M6
#define	               RW_M7   79	//M7-中间继电器字M7
#define	               RW_M8   80	//M8-中间继电器字M8
#define	               RW_M9   81	//M9-中间继电器字M9
#define	              RW_M10   82	//M10-中间继电器字M10
#define	              RW_M11   83	//M11-中间继电器字M11
#define	              RW_M12   84	//M12-中间继电器字M12
#define	              RW_M13   85	//M13-中间继电器字M13
#define	              RW_M14   86	//M14-中间继电器字M14
#define	              RW_M15   87	//M15-中间继电器字M15
#define	              RW_M16   88	//M16-中间继电器字M16
#define	              RW_M17   89	//M17-中间继电器字M17
#define	              RW_M18   90	//M18-中间继电器字M18
#define	              RW_M19   91	//M19-中间继电器字M19
#define	              RW_M20   92	//M20-中间继电器字M20
#define	              RW_M21   93	//M21-中间继电器字M21
#define	              RW_M22   94	//M22-中间继电器字M22
#define	              RW_M23   95	//M23-中间继电器字M23
#define	              RW_M24   96	//M24-中间继电器字M24
#define	              RW_M25   97	//M25-中间继电器字M25
#define	              RW_M26   98	//M26-中间继电器字M26
#define	              RW_M27   99	//M27-中间继电器字M27
#define	              RW_M28  100	//M28-中间继电器字M28
#define	              RW_M29  101	//M29-中间继电器字M29
#define	              RW_M30  102	//M30-中间继电器字M30
#define	              RW_M31  103	//M31-中间继电器字M31
#define	              RW_M32  104	//M32-中间继电器字M32
#define	              RW_M33  105	//M33-中间继电器字M33
#define	              RW_M34  106	//M34-中间继电器字M34
#define	              RW_M35  107	//M35-中间继电器字M35
#define	              RW_M36  108	//M36-中间继电器字M36
#define	              RW_M37  109	//M37-中间继电器字M37
#define	              RW_M38  110	//M38-中间继电器字M38
#define	              RW_M39  111	//M39-中间继电器字M39
#define	              RW_M40  112	//M40-中间继电器字M40
#define	              RW_M41  113	//M41-中间继电器字M41
#define	              RW_M42  114	//M42-中间继电器字M42
#define	              RW_M43  115	//M43-中间继电器字M43
#define	              RW_M44  116	//M44-中间继电器字M44
#define	              RW_M45  117	//M45-中间继电器字M45
#define	              RW_M46  118	//M46-中间继电器字M46
#define	              RW_M47  119	//M47-中间继电器字M47
#define	              RW_M48  120	//M48-中间继电器字M48
#define	              RW_M49  121	//M49-中间继电器字M49
#define	              RW_M50  122	//M50-中间继电器字M50
#define	              RW_M51  123	//M51-中间继电器字M51
#define	              RW_M52  124	//M52-中间继电器字M52
#define	              RW_M53  125	//M53-中间继电器字M53
#define	              RW_M54  126	//M54-中间继电器字M54
#define	              RW_M55  127	//M55-中间继电器字M55
#define	              RW_M56  128	//M56-中间继电器字M56
#define	              RW_M57  129	//M57-中间继电器字M57
#define	              RW_M58  130	//M58-中间继电器字M58
#define	              RW_M59  131	//M59-中间继电器字M59
#define	              RW_M60  132	//M60-中间继电器字M60
#define	              RW_M61  133	//M61-中间继电器字M61
#define	              RW_M62  134	//M62-中间继电器字M62
#define	              RW_M63  135	//M63-中间继电器字M63
#define	              RW_M64  136	//M64-中间继电器字M64
#define	         RW_LKPI50I1  137	//LKPI50I1-过流入段闭锁继电器字
#define	        RW_LKRST50I1  138	//LKRST50I1-过流返回闭锁继电器字
#define	         RW_LKPI50I0  139	//LKPI50I0-速断入段闭锁继电器字
#define	        RW_LKRST50I0  140	//LKRST50I0-速断返回闭锁继电器字
#define	          RW_LKPI59U  141	//LKPI59U-过压入段闭锁继电器字
#define	         RW_LKRST59U  142	//LKRST59U-过压返回闭锁继电器字
#define	          RW_LKPI60I  143	//LKPI60I-不平衡电流入段闭锁继电器字
#define	         RW_LKRST60I  144	//LKRST60I-不平衡电流返回闭锁继电器字
#define	          RW_LKPI60U  145	//LKPI60U-不平衡电压入段闭锁继电器字
#define	         RW_LKRST60U  146	//LKRST60U-不平衡电压返回闭锁继电器字
#define	           RW_LKPI27  147	//LKPI27-低压保护入段闭锁继电器字
#define	          RW_LKRST27  148	//LKRST27-低压保护返回闭锁继电器字
#define	             RW_69IN  149	//69IN-远方当地中间继电器字
#define	              RW_52A  150	//52A-合位
#define	              RW_52B  151	//52B-分位
#define	              RW_REC  152	//REC-录波继电器字
#define	               RW_H1  153	//H1-自保持中间继电器字1
#define	               RW_H2  154	//H2-自保持中间继电器字2
#define	               RW_H3  155	//H3-自保持中间继电器字3
#define	               RW_H4  156	//H4-自保持中间继电器字4
#define	               RW_H5  157	//H5-自保持中间继电器字5
#define	               RW_H6  158	//H6-自保持中间继电器字6
#define	               RW_H7  159	//H7-自保持中间继电器字7
#define	               RW_H8  160	//H8-自保持中间继电器字8
#define	               RW_H9  161	//H9-自保持中间继电器字9
#define	              RW_H10  162	//H10-自保持中间继电器字10
#define	              RW_H11  163	//H11-自保持中间继电器字11
#define	              RW_H12  164	//H12-自保持中间继电器字12
#define	              RW_H13  165	//H13-自保持中间继电器字13
#define	              RW_H14  166	//H14-自保持中间继电器字14
#define	              RW_H15  167	//H15-自保持中间继电器字15
#define	              RW_H16  168	//H16-自保持中间继电器字16
#define	              RW_H17  169	//H17-自保持中间继电器字17
#define	              RW_H18  170	//H18-自保持中间继电器字18
#define	              RW_H19  171	//H19-自保持中间继电器字19
#define	              RW_H20  172	//H20-自保持中间继电器字20
#define	              RW_H21  173	//H21-自保持中间继电器字21
#define	              RW_H22  174	//H22-自保持中间继电器字22
#define	              RW_H23  175	//H23-自保持中间继电器字23
#define	              RW_H24  176	//H24-自保持中间继电器字24
#define	              RW_H25  177	//H25-自保持中间继电器字25
#define	              RW_H26  178	//H26-自保持中间继电器字26
#define	              RW_H27  179	//H27-自保持中间继电器字27
#define	              RW_H28  180	//H28-自保持中间继电器字28
#define	              RW_H29  181	//H29-自保持中间继电器字29
#define	              RW_H30  182	//H30-自保持中间继电器字30
#define	              RW_H31  183	//H31-自保持中间继电器字31
#define	              RW_H32  184	//H32-自保持中间继电器字31
#define	             RW_52BF  185	//52BF-断路器跳闸失败继电器字
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
#define	            RW_T52BF  218	//T52BF-断路器跳闸失败延时继电器字
#define	              RW_TR1  219	//TR1-跳闸继电器1
#define	              RW_TR2  220	//TR2-跳闸继电器2
#define	              RW_TR3  221	//TR3-跳闸继电器3
#define	              RW_VR1  222	//VR1-虚拟继电器字1
#define	              RW_VR2  223	//VR2-虚拟继电器字2
#define	              RW_VR3  224	//VR3-虚拟继电器字3
#define	              RW_VR4  225	//VR4-虚拟继电器字4
#define	              RW_VR5  226	//VR5-虚拟继电器字5
#define	              RW_VR6  227	//VR6-虚拟继电器字6
#define	              RW_VR7  228	//VR7-虚拟继电器字7
#define	              RW_VR8  229	//VR8-虚拟继电器字8
#define	              RW_VR9  230	//VR9-虚拟继电器字9
#define	             RW_VR10  231	//VR10-虚拟继电器字10
#define	             RW_VR11  232	//VR11-虚拟继电器字11
#define	             RW_VR12  233	//VR12-虚拟继电器字12
#define	             RW_VR13  234	//VR13-虚拟继电器字13
#define	             RW_VR14  235	//VR14-虚拟继电器字14
#define	             RW_VR15  236	//VR15-虚拟继电器字15
#define	             RW_VR16  237	//VR16-虚拟继电器字16
#define	             RW_VR17  238	//VR17-虚拟继电器字17
#define	             RW_VR18  239	//VR18-虚拟继电器字18
#define	             RW_VR19  240	//VR19-虚拟继电器字19
#define	             RW_VR20  241	//VR20-虚拟继电器字20
#define	             RW_VR21  242	//VR21-虚拟继电器字21
#define	             RW_VR22  243	//VR22-虚拟继电器字22
#define	             RW_VR23  244	//VR23-虚拟继电器字23
#define	             RW_VR24  245	//VR24-虚拟继电器字24
#define	             RW_VR25  246	//VR25-虚拟继电器字25
#define	             RW_VR26  247	//VR26-虚拟继电器字26
#define	             RW_VR27  248	//VR27-虚拟继电器字27
#define	             RW_VR28  249	//VR28-虚拟继电器字28
#define	             RW_VR29  250	//VR29-虚拟继电器字29
#define	             RW_VR30  251	//VR30-虚拟继电器字30
#define	             RW_VR31  252	//VR31-虚拟继电器字31
#define	             RW_VR32  253	//VR32-虚拟继电器字32
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
#define	             RW_IN35  288	//IN35-信号复归
#define	             RW_IN36  289	//IN36-跳闸位置
#define	             RW_IN37  290	//IN37-合闸位置
#define	             RW_IN38  291	//IN38-手跳重动
#define	             RW_IN39  292	//IN39-遥控预置
#define	             RW_IN40  293	//IN40-遥控1预置
#define	             RW_IN41  294	//IN41-遥控2预置
#define	             RW_IN42  295	//IN42-遥控3预置
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
#define	             RW_OUT1  312	//OUT1-开出1继电器字
#define	             RW_OUT2  313	//OUT2-开出2继电器字
#define	             RW_OUT3  314	//OUT3-开出3继电器字
#define	             RW_OUT4  315	//OUT4-开出4继电器字
#define	             RW_OUT5  316	//OUT5-开出5继电器字
#define	             RW_OUT6  317	//OUT6-开出6继电器字
#define	             RW_OUT7  318	//OUT7-开出7继电器字
#define	             RW_OUT8  319	//OUT8-开出8继电器字
#define	             RW_OUT9  320	//OUT9-开出9继电器字
#define	            RW_OUT10  321	//OUT10-开出10继电器字
#define	            RW_OUT11  322	//OUT11-开出11继电器字
#define	            RW_OUT12  323	//OUT12-开出12继电器字
#define	            RW_OUT13  324	//OUT13-开出13继电器字
#define	            RW_OUT14  325	//OUT14-开出14继电器字
#define	            RW_OUT15  326	//OUT15-开出15继电器字
#define	            RW_OUT16  327	//OUT16-开出16继电器字
#define	            RW_OUT17  328	//OUT17-开出17继电器字
#define	            RW_OUT18  329	//OUT18-开出18继电器字
#define	            RW_OUT19  330	//OUT19-开出19继电器字
#define	           RW_SL50I0  331	//SL50I0-电流速断软压板继电器字
#define	           RW_SL50I1  332	//SL50I1-过流软压板继电器字
#define	            RW_SL59U  333	//SL59U-过压保护软压板继电器字
#define	            RW_SL27U  334	//SL27U-低压保护软压板继电器字
#define	            RW_SL60I  335	//SL60I-不平衡电流软压板继电器字
#define	            RW_SL60U  336	//SL60U-不平衡电压软压板继电器字
#define	           RW_SLJDXX  337	//SLJDXX-接地选线软压板继电器字
#define	          RW_SLMAINT  338	//SLMAINT-检修压板继电器字
#define	           RW_SLBAK1  339	//SLBAK1-备用压板1
#define	           RW_SLBAK2  340	//SLBAK2-备用压板2
#define	            RW_F50I0  341	//F50I0-电流速断配置继电器字
#define	            RW_F50I1  342	//F50I1-过流保护配置继电器字
#define	             RW_F59U  343	//F59U-过压保护配置继电器字
#define	             RW_F27U  344	//F27U-低压保护配置继电器字
#define	             RW_F60I  345	//F60I-不平衡电流保护配置继电器字
#define	             RW_F60U  346	//F60U-不平衡电压保护配置继电器字
#define	             RW_F59N  347	//F59N-零压告警配置继电器字
#define	            RW_FJDXX  348	//FJDXX-接地选线配置继电器字
#define	            RW_FXHXQ  349	//FXHXQ-消弧线圈接地配置继电器字
#define	            RW_F74PT  350	//F74PT-PT断线检查配置继电器字
#define	            RW_FCTBR  351	//FCTBR-CT断线配置继电器字
#define	            RW_F52BF  352	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  353	//FBAK1-备用1配置继电器字
#define	            RW_FBAK2  354	//FBAK2-备用2配置继电器字
#define	              RW_PW1  355	//PW1-脉冲继电器1
#define	              RW_PW2  356	//PW2-脉冲继电器2
#define	              RW_PW3  357	//PW3-脉冲继电器3
#define	              RW_PW4  358	//PW4-脉冲继电器4
#define	              RW_PW5  359	//PW5-脉冲继电器5
#define	              RW_PW6  360	//PW6-脉冲继电器6
#define	              RW_PW7  361	//PW7-脉冲继电器7
#define	              RW_PW8  362	//PW8-脉冲继电器8
#define	              RW_PW9  363	//PW9-脉冲继电器9
#define	             RW_PW10  364	//PW10-脉冲继电器10
#define	             RW_PW11  365	//PW11-脉冲继电器11
#define	             RW_PW12  366	//PW12-脉冲继电器12
#define	             RW_PW13  367	//PW13-脉冲继电器13
#define	             RW_PW14  368	//PW14-脉冲继电器14
#define	             RW_PW15  369	//PW15-脉冲继电器15
#define	             RW_PW16  370	//PW16-脉冲继电器16
#define	              RW_TO1  371	//TO1-传动继电器1
#define	              RW_TO2  372	//TO2-传动继电器2
#define	              RW_TO3  373	//TO3-传动继电器3
#define	              RW_TO4  374	//TO4-传动继电器4
#define	              RW_TO5  375	//TO5-传动继电器5
#define	              RW_TO6  376	//TO6-传动继电器6
#define	              RW_TO7  377	//TO7-传动继电器7
#define	              RW_TO8  378	//TO8-传动继电器8
#define	              RW_TO9  379	//TO9-传动继电器9
#define	             RW_TO10  380	//TO10-传动继电器10
#define	             RW_TO11  381	//TO11-传动继电器11
#define	             RW_SIG1  382	//SIG1-信号继电器1
#define	             RW_SIG2  383	//SIG2-信号继电器2
#define	             RW_SIG3  384	//SIG3-信号继电器3
#define	             RW_SIG4  385	//SIG4-信号继电器4
#define	             RW_SIG5  386	//SIG5-信号继电器5
#define	             RW_SIG6  387	//SIG6-信号继电器6
#define	             RW_SIG7  388	//SIG7-信号继电器7
#define	             RW_SIG8  389	//SIG8-信号继电器8
#define	             RW_SIG9  390	//SIG9-信号继电器9
#define	            RW_SIG10  391	//SIG10-信号继电器10
#define	            RW_SIG11  392	//SIG11-信号继电器11
#define	            RW_SIG12  393	//SIG12-信号继电器12
#define	            RW_SIG13  394	//SIG13-信号继电器13
#define	            RW_SIG14  395	//SIG14-信号继电器14
#define	            RW_SIG15  396	//SIG15-信号继电器15
#define	            RW_SIG16  397	//SIG16-信号继电器16
#define	            RW_LED1G  398	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  399	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  400	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  401	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  402	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  403	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  404	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  405	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  406	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  407	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  408	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  409	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  410	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  411	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  412	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  413	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  414	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  415	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  416	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  417	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  418	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  419	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  420	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  421	//LED12R-LED12红灯继电器字
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
#define	             RW_RCP1  438	//RCP1-遥控预置继电器1
#define	             RW_RCP2  439	//RCP2-遥控预置继电器2
#define	             RW_RCP3  440	//RCP3-遥控预置继电器3
#define	             RW_RCP4  441	//RCP4-遥控预置继电器4
#define	             RW_RCP5  442	//RCP5-遥控预置继电器5
#define	             RW_RCP6  443	//RCP6-遥控预置继电器6
#define	             RW_RCP7  444	//RCP7-遥控预置继电器7
#define	             RW_RCP8  445	//RCP8-遥控预置继电器8
#define	             RW_RCP9  446	//RCP9-遥控预置继电器9
#define	            RW_RCP10  447	//RCP10-遥控预置继电器10
#define	            RW_RCP11  448	//RCP11-遥控预置继电器11
#define	            RW_RCP12  449	//RCP12-遥控预置继电器12
#define	            RW_RCP13  450	//RCP13-遥控预置继电器13
#define	            RW_RCP14  451	//RCP14-遥控预置继电器14
#define	            RW_RCP15  452	//RCP15-遥控预置继电器15
#define	            RW_RCP16  453	//RCP16-遥控预置继电器16
#define	            RW_RCP17  454	//RCP17-遥控预置继电器17
#define	            RW_RCP18  455	//RCP18-遥控预置继电器18
#define	            RW_RCP19  456	//RCP19-遥控预置继电器19
#define	            RW_RCP20  457	//RCP20-遥控预置继电器20
#define	            RW_RCP21  458	//RCP21-遥控预置继电器21
#define	            RW_RCP22  459	//RCP22-遥控预置继电器22
#define	            RW_RCP23  460	//RCP23-遥控预置继电器23
#define	            RW_RCP24  461	//RCP24-遥控预置继电器24
#define	            RW_RCP25  462	//RCP25-遥控预置继电器25
#define	            RW_RCP26  463	//RCP26-遥控预置继电器26
#define	            RW_RCP27  464	//RCP27-遥控预置继电器27
#define	            RW_RCP28  465	//RCP28-遥控预置继电器28
#define	            RW_RCP29  466	//RCP29-遥控预置继电器29
#define	            RW_RCP30  467	//RCP30-遥控预置继电器30
#define	            RW_RCP31  468	//RCP31-遥控预置继电器31
#define	            RW_RCP32  469	//RCP32-遥控预置继电器32
#define	            RW_RCP33  470	//RCP33-遥控预置继电器33
#define	            RW_RCP34  471	//RCP34-遥控预置继电器34
#define	             RW_RCE1  472	//RCE1-遥控执行继电器1
#define	             RW_RCE2  473	//RCE2-遥控执行继电器2
#define	             RW_RCE3  474	//RCE3-遥控执行继电器3
#define	             RW_RCE4  475	//RCE4-遥控执行继电器4
#define	             RW_RCE5  476	//RCE5-遥控执行继电器5
#define	             RW_RCE6  477	//RCE6-遥控执行继电器6
#define	             RW_RCE7  478	//RCE7-遥控执行继电器7
#define	             RW_RCE8  479	//RCE8-遥控执行继电器8
#define	             RW_RCE9  480	//RCE9-遥控执行继电器9
#define	            RW_RCE10  481	//RCE10-遥控执行继电器10
#define	            RW_RCE11  482	//RCE11-遥控执行继电器11
#define	            RW_RCE12  483	//RCE12-遥控执行继电器12
#define	            RW_RCE13  484	//RCE13-遥控执行继电器13
#define	            RW_RCE14  485	//RCE14-遥控执行继电器14
#define	            RW_RCE15  486	//RCE15-遥控执行继电器15
#define	            RW_RCE16  487	//RCE16-遥控执行继电器16
#define	            RW_RCE17  488	//RCE17-遥控执行继电器17
#define	            RW_RCE18  489	//RCE18-遥控执行继电器18
#define	            RW_RCE19  490	//RCE19-遥控执行继电器19
#define	            RW_RCE20  491	//RCE20-遥控执行继电器20
#define	            RW_RCE21  492	//RCE21-遥控执行继电器21
#define	            RW_RCE22  493	//RCE22-遥控执行继电器22
#define	            RW_RCE23  494	//RCE23-遥控执行继电器23
#define	            RW_RCE24  495	//RCE24-遥控执行继电器24
#define	            RW_RCE25  496	//RCE25-遥控执行继电器25
#define	            RW_RCE26  497	//RCE26-遥控执行继电器26
#define	            RW_RCE27  498	//RCE27-遥控执行继电器27
#define	            RW_RCE28  499	//RCE28-遥控执行继电器28
#define	            RW_RCE29  500	//RCE29-遥控执行继电器29
#define	            RW_RCE30  501	//RCE30-遥控执行继电器30
#define	            RW_RCE31  502	//RCE31-遥控执行继电器31
#define	            RW_RCE32  503	//RCE32-遥控执行继电器32
#define	            RW_RCE33  504	//RCE33-遥控执行继电器33
#define	            RW_RCE34  505	//RCE34-遥控执行继电器34
#define	             RW_RCV1  506	//RCV1-遥控反校继电器1
#define	             RW_RCV2  507	//RCV2-遥控反校继电器2
#define	             RW_RCV3  508	//RCV3-遥控反校继电器3
#define	             RW_RCV4  509	//RCV4-遥控反校继电器4
#define	             RW_RCV5  510	//RCV5-遥控反校继电器5
#define	             RW_RCV6  511	//RCV6-遥控反校继电器6
#define	             RW_RCV7  512	//RCV7-遥控反校继电器7
#define	             RW_RCV8  513	//RCV8-遥控反校继电器8
#define	             RW_RCV9  514	//RCV9-遥控反校继电器9
#define	            RW_RCV10  515	//RCV10-遥控反校继电器10
#define	            RW_RCV11  516	//RCV11-遥控反校继电器11
#define	            RW_RCV12  517	//RCV12-遥控反校继电器12
#define	            RW_RCV13  518	//RCV13-遥控反校继电器13
#define	            RW_RCV14  519	//RCV14-遥控反校继电器14
#define	            RW_RCV15  520	//RCV15-遥控反校继电器15
#define	            RW_RCV16  521	//RCV16-遥控反校继电器16
#define	            RW_RCV17  522	//RCV17-遥控反校继电器17
#define	            RW_RCV18  523	//RCV18-遥控反校继电器18
#define	            RW_RCV19  524	//RCV19-遥控反校继电器19
#define	            RW_RCV20  525	//RCV20-遥控反校继电器20
#define	            RW_RCV21  526	//RCV21-遥控反校继电器21
#define	            RW_RCV22  527	//RCV22-遥控反校继电器22
#define	            RW_RCV23  528	//RCV23-遥控反校继电器23
#define	            RW_RCV24  529	//RCV24-遥控反校继电器24
#define	            RW_RCV25  530	//RCV25-遥控反校继电器25
#define	            RW_RCV26  531	//RCV26-遥控反校继电器26
#define	            RW_RCV27  532	//RCV27-遥控反校继电器27
#define	            RW_RCV28  533	//RCV28-遥控反校继电器28
#define	            RW_RCV29  534	//RCV29-遥控反校继电器29
#define	            RW_RCV30  535	//RCV30-遥控反校继电器30
#define	            RW_RCV31  536	//RCV31-遥控反校继电器31
#define	            RW_RCV32  537	//RCV32-遥控反校继电器32
#define	            RW_RCV33  538	//RCV33-遥控反校继电器33
#define	            RW_RCV34  539	//RCV34-遥控反校继电器34
#define	              RW_GE1  540	//GE1-大于等于继电器1
#define	              RW_GE2  541	//GE2-大于等于继电器2
#define	              RW_GE3  542	//GE3-大于等于继电器3
#define	              RW_GE4  543	//GE4-大于等于继电器4
#define	              RW_GE5  544	//GE5-大于等于继电器5
#define	              RW_GE6  545	//GE6-大于等于继电器6
#define	              RW_GE7  546	//GE7-大于等于继电器7
#define	              RW_GE8  547	//GE8-大于等于继电器8
#define	           RW_CONST1  548	//CONST1-常数时间继电器1
#define	           RW_CONST2  549	//CONST2-常数时间继电器2
#define	           RW_CONST3  550	//CONST3-常数时间继电器3
#define	           RW_CONST4  551	//CONST4-常数时间继电器4
#define	           RW_CONST5  552	//CONST5-常数时间继电器5
#define	           RW_CONST6  553	//CONST6-常数时间继电器6
#define	           RW_CONST7  554	//CONST7-常数时间继电器7
#define	           RW_CONST8  555	//CONST8-常数时间继电器8
#define	              RW_TX1  556	//TX1-镜像发送继电器字1
#define	              RW_TX2  557	//TX2-镜像发送继电器字2
#define	              RW_TX3  558	//TX3-镜像发送继电器字3
#define	              RW_TX4  559	//TX4-镜像发送继电器字4
#define	              RW_TX5  560	//TX5-镜像发送继电器字5
#define	              RW_TX6  561	//TX6-镜像发送继电器字6
#define	              RW_TX7  562	//TX7-镜像发送继电器字7
#define	              RW_TX8  563	//TX8-镜像发送继电器字8
#define	              RW_TX9  564	//TX9-镜像发送继电器字9
#define	             RW_TX10  565	//TX10-镜像发送继电器字10
#define	             RW_TX11  566	//TX11-镜像发送继电器字11
#define	             RW_TX12  567	//TX12-镜像发送继电器字12
#define	             RW_TX13  568	//TX13-镜像发送继电器字13
#define	             RW_TX14  569	//TX14-镜像发送继电器字14
#define	             RW_TX15  570	//TX15-镜像发送继电器字15
#define	             RW_TX16  571	//TX16-镜像发送继电器字16
#define	              RW_RX1  572	//RX1-镜像接收继电器字1
#define	              RW_RX2  573	//RX2-镜像接收继电器字2
#define	              RW_RX3  574	//RX3-镜像接收继电器字3
#define	              RW_RX4  575	//RX4-镜像接收继电器字4
#define	              RW_RX5  576	//RX5-镜像接收继电器字5
#define	              RW_RX6  577	//RX6-镜像接收继电器字6
#define	              RW_RX7  578	//RX7-镜像接收继电器字7
#define	              RW_RX8  579	//RX8-镜像接收继电器字8
#define	              RW_RX9  580	//RX9-镜像接收继电器字9
#define	             RW_RX10  581	//RX10-镜像接收继电器字10
#define	             RW_RX11  582	//RX11-镜像接收继电器字11
#define	             RW_RX12  583	//RX12-镜像接收继电器字12
#define	             RW_RX13  584	//RX13-镜像接收继电器字13
#define	             RW_RX14  585	//RX14-镜像接收继电器字14
#define	             RW_RX15  586	//RX15-镜像接收继电器字15
#define	             RW_RX16  587	//RX16-镜像接收继电器字16
#define	             RW_CNT1  588	//CNT1-计数继电器字1
#define	             RW_CNT2  589	//CNT2-计数继电器字2
#define	             RW_CNT3  590	//CNT3-计数继电器字3
#define	             RW_CNT4  591	//CNT4-计数继电器字4
#define	             RW_CNT5  592	//CNT5-计数继电器字5
#define	             RW_CNT6  593	//CNT6-计数继电器字6
#define	             RW_CNT7  594	//CNT7-计数继电器字7
#define	             RW_CNT8  595	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			596		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			71	

//中间继电器字
#define	MID_RELAY_WORD_START			73	
#define	MID_RELAY_WORD_NUM			80	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			153	
#define	KEEP_RELAY_WORD_NUM			33	

//时间继电器字
#define	TIME_RELAY_WORD_START			186	
#define	TIME_RELAY_WORD_NUM			33	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			219	
#define	TRIP_RELAY_WORD_NUM			3	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			222	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			254	
#define	INPUT_RELAY_WORD_NUM			58	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			312	
#define	OUTPUT_RELAY_WORD_NUM			19	

//压板继电器
#define	SW_RELAY_WORD_START			331	
#define	SW_RELAY_WORD_NUM			10	

//配置继电器
#define	CFG_RELAY_WORD_START			341	
#define	CFG_RELAY_WORD_NUM			14	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			355	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			371	
#define	CTRLTEST_RELAY_WORD_NUM			11	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			382	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			398	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			422	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			438	
#define	RCP_RELAY_WORD_NUM			34	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			472	
#define	RCE_RELAY_WORD_NUM			34	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			506	
#define	RCV_RELAY_WORD_NUM			34	

//大于等于继电器
#define	GE_RELAY_WORD_START			540	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			548	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			556	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			572	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			588	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			588	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			588	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			588	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			588	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			588	
#define	CNT_RELAY_WORD_NUM			8	


#endif