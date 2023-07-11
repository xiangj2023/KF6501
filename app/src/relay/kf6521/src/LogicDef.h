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
#define	           RW_PI50I1    2	//PI50I1-过流Ⅰ段入段继电器字
#define	           RW_TR50I1    3	//TR50I1-过流Ⅰ段动作继电器字
#define	          RW_LPC50I1    4	//LPC50I1-过流Ⅰ段循环控制继电器字
#define	        RW_TR50I1_94    5	//TR50I1_94-过流Ⅰ段加速动作继电器字
#define	       RW_LK50I1_47U    6	//LK50I1_47U-过流Ⅰ段复压闭锁继电器字
#define	           RW_PI50I2    7	//PI50I2-过流Ⅱ段入段继电器字
#define	           RW_TR50I2    8	//TR50I2-过流Ⅱ段动作继电器字
#define	          RW_LPC50I2    9	//LPC50I2-过流Ⅱ段循环控制继电器字
#define	        RW_TR50I2_94   10	//TR50I2_94-过流Ⅱ段加速动作继电器字
#define	       RW_LK50I2_47U   11	//LK50I2_47U-过流Ⅱ段复压闭锁继电器字
#define	           RW_PI50I3   12	//PI50I3-过流Ⅲ段入段继电器字
#define	           RW_TR50I3   13	//TR50I3-过流Ⅲ段动作继电器字
#define	          RW_LPC50I3   14	//LPC50I3-过流Ⅲ段循环控制继电器字
#define	        RW_TR50I3_94   15	//TR50I3_94-过流Ⅲ段加速动作继电器字
#define	       RW_LK50I3_47U   16	//LK50I3_47U-过流Ⅲ段复压闭锁继电器字
#define	              RW_APF   17	//APF-A相功率正向继电器字
#define	              RW_APR   18	//APR-A相功率反向继电器字
#define	              RW_BPF   19	//BPF-B相功率正向继电器字
#define	              RW_BPR   20	//BPR-B相功率反向继电器字
#define	              RW_CPF   21	//CPF-C相功率正向继电器字
#define	              RW_CPR   22	//CPR-C相功率反向继电器字
#define	           RW_PI50N1   23	//PI50N1-零流Ⅰ段入段继电器字
#define	           RW_TR50N1   24	//TR50N1-零流Ⅰ段动作继电器字
#define	          RW_LPC50N1   25	//LPC50N1-零流Ⅰ段循环控制继电器字
#define	           RW_PI50N2   26	//PI50N2-零流Ⅱ段入段继电器字
#define	           RW_TR50N2   27	//TR50N2-零流Ⅱ段动作继电器字
#define	          RW_LPC50N2   28	//LPC50N2-零流Ⅱ段循环控制继电器字
#define	            RW_PI27U   29	//PI27U-低压保护入段继电器字
#define	            RW_TR27U   30	//TR27U-低压保护动作继电器字
#define	           RW_LPC27U   31	//LPC27U-低压保护循环控制继电器字
#define	            RW_PIPT1   32	//PIPT1-母线PT断线入段继电器字
#define	            RW_ARPT1   33	//ARPT1-母线PT断线告警继电器字
#define	            RW_PIPT2   34	//PIPT2-线路PT断线入段继电器字
#define	            RW_ARPT2   35	//ARPT2-线路PT断线告警继电器字
#define	            RW_PI59N   36	//PI59N-零压入段继电器字
#define	            RW_TR59N   37	//TR59N-零压动作继电器字
#define	           RW_LPC59N   38	//LPC59N-零压保护循环控制继电器字
#define	           RW_PI50I0   39	//PI50I0-电流速断入段继电器字
#define	           RW_TR50I0   40	//TR50I0-电流速断动作继电器字
#define	          RW_LPC50I0   41	//LPC50I0-电流速断循环控制继电器字
#define	       RW_LK50I0_47U   42	//LK50I0_47U-电流速断复压闭锁继电器字
#define	          RW_PI50IOL   43	//PI50IOL-过负荷入段继电器字
#define	          RW_TR50IOL   44	//TR50IOL-过负荷动作继电器字
#define	         RW_LPC50IOL   45	//LPC50IOL-过负荷保护循环控制继电器字
#define	          RW_PI50ISH   46	//PI50ISH-母充保护入段继电器字
#define	          RW_TR50ISH   47	//TR50ISH-母充保护动作继电器字
#define	         RW_LPC50ISH   48	//LPC50ISH-母充保护循环控制继电器字
#define	             RW_59UA   49	//59UA-UA有压继电器字
#define	             RW_27UA   50	//27UA-UA无压继电器字
#define	             RW_59UB   51	//59UB-UB有压继电器字
#define	             RW_27UB   52	//27UB-UB无压继电器字
#define	             RW_59UC   53	//59UC-UC有压继电器字
#define	             RW_27UC   54	//27UC-UC无压继电器字
#define	             RW_59U0   55	//59U0-U0有压继电器字
#define	             RW_27U0   56	//27U0-U0无压继电器字
#define	             RW_59U1   57	//59U1-U1有压继电器字
#define	             RW_27U1   58	//27U1-U1无压继电器字
#define	             RW_59U2   59	//59U2-U2有压继电器字
#define	             RW_27U2   60	//27U2-U2无压继电器字
#define	             RW_59U3   61	//59U3-U3有压继电器字
#define	             RW_27U3   62	//27U3-U3无压继电器字
#define	             RW_50IA   63	//50IA-IA有流继电器字
#define	             RW_37IA   64	//37IA-IA无流继电器字
#define	             RW_50IB   65	//50IB-IB有流继电器字
#define	             RW_37IB   66	//37IB-IB无流继电器字
#define	             RW_50IC   67	//50IC-IC有流继电器字
#define	             RW_37IC   68	//37IC-IC无流继电器字
#define	             RW_50I0   69	//50I0-I0有流继电器字
#define	             RW_37I0   70	//37I0-I0无流继电器字
#define	             RW_50I1   71	//50I1-I1有流继电器字
#define	             RW_37I1   72	//37I1-I1无流继电器字
#define	            RW_79SH0   73	//79SH0-重合闸动作状态继电器字一
#define	            RW_79SH1   74	//79SH1-重合闸动作状态继电器字二
#define	             RW_79TO   75	//79TO-重合闸延时到逻辑继电器字
#define	            RW_79CYS   76	//79CYS-重合闸周期状态继电器字
#define	            RW_79RSS   77	//79RSS-重合闸复归状态继电器字
#define	            RW_79LOS   78	//79LOS-重合闸闭锁状态继电器字
#define	             RW_79CL   79	//79CL-重合闸动作逻辑继电器字
#define	           RW_PICKUP   80	//PICKUP-模值突变量启动继电器字
#define	          RW_ET11ERR   81	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   82	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   83	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   84	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   85	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   86	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   87	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   88	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   89	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   90	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   91	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   92	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   93	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   94	//NULL1ERR-空连接1状态继电器字
#define	         RW_NULL2ERR   95	//NULL2ERR-空连接2状态继电器字
#define	         RW_NULL3ERR   96	//NULL3ERR-空连接3状态继电器字
#define	             RW_25CL   97	//25CL-检同期动作继电器字
#define	          RW_25_27UM   98	//25_27UM-母线电压小于检无压定值继电器字
#define	          RW_25_27UL   99	//25_27UL-线路电压小于检无压定值继电器字
#define	         RW_PICTBR_A  100	//PICTBR_A-A相CT断线入段继电器字
#define	         RW_ARCTBR_A  101	//ARCTBR_A-A相CT断线告警继电器字
#define	         RW_PICTBR_B  102	//PICTBR_B-B相CT断线入段继电器字
#define	         RW_ARCTBR_B  103	//ARCTBR_B-B相CT断线告警继电器字
#define	         RW_PICTBR_C  104	//PICTBR_C-C相CT断线入段继电器字
#define	         RW_ARCTBR_C  105	//ARCTBR_C-C相CT断线告警继电器字
#define	        RW_MAINTLINK  106	//MAINTLINK-维护状态继电器字
#define	               RW_M1  107	//M1-中间继电器字M1
#define	               RW_M2  108	//M2-中间继电器字M2
#define	               RW_M3  109	//M3-中间继电器字M3
#define	               RW_M4  110	//M4-中间继电器字M4
#define	               RW_M5  111	//M5-中间继电器字M5
#define	               RW_M6  112	//M6-中间继电器字M6
#define	               RW_M7  113	//M7-中间继电器字M7
#define	               RW_M8  114	//M8-中间继电器字M8
#define	               RW_M9  115	//M9-中间继电器字M9
#define	              RW_M10  116	//M10-中间继电器字M10
#define	              RW_M11  117	//M11-中间继电器字M11
#define	              RW_M12  118	//M12-中间继电器字M12
#define	              RW_M13  119	//M13-中间继电器字M13
#define	              RW_M14  120	//M14-中间继电器字M14
#define	              RW_M15  121	//M15-中间继电器字M15
#define	              RW_M16  122	//M16-中间继电器字M16
#define	              RW_M17  123	//M17-中间继电器字M17
#define	              RW_M18  124	//M18-中间继电器字M18
#define	              RW_M19  125	//M19-中间继电器字M19
#define	              RW_M20  126	//M20-中间继电器字M20
#define	              RW_M21  127	//M21-中间继电器字M21
#define	              RW_M22  128	//M22-中间继电器字M22
#define	              RW_M23  129	//M23-中间继电器字M23
#define	              RW_M24  130	//M24-中间继电器字M24
#define	              RW_M25  131	//M25-中间继电器字M25
#define	              RW_M26  132	//M26-中间继电器字M26
#define	              RW_M27  133	//M27-中间继电器字M27
#define	              RW_M28  134	//M28-中间继电器字M28
#define	              RW_M29  135	//M29-中间继电器字M29
#define	              RW_M30  136	//M30-中间继电器字M30
#define	              RW_M31  137	//M31-中间继电器字M31
#define	              RW_M32  138	//M32-中间继电器字M32
#define	              RW_M33  139	//M33-中间继电器字M33
#define	              RW_M34  140	//M34-中间继电器字M34
#define	              RW_M35  141	//M35-中间继电器字M35
#define	              RW_M36  142	//M36-中间继电器字M36
#define	              RW_M37  143	//M37-中间继电器字M37
#define	              RW_M38  144	//M38-中间继电器字M38
#define	              RW_M39  145	//M39-中间继电器字M39
#define	              RW_M40  146	//M40-中间继电器字M40
#define	              RW_M41  147	//M41-中间继电器字M41
#define	              RW_M42  148	//M42-中间继电器字M42
#define	              RW_M43  149	//M43-中间继电器字M43
#define	              RW_M44  150	//M44-中间继电器字M44
#define	              RW_M45  151	//M45-中间继电器字M45
#define	              RW_M46  152	//M46-中间继电器字M46
#define	              RW_M47  153	//M47-中间继电器字M47
#define	              RW_M48  154	//M48-中间继电器字M48
#define	              RW_M49  155	//M49-中间继电器字M49
#define	              RW_M50  156	//M50-中间继电器字M50
#define	              RW_M51  157	//M51-中间继电器字M51
#define	              RW_M52  158	//M52-中间继电器字M52
#define	              RW_M53  159	//M53-中间继电器字M53
#define	              RW_M54  160	//M54-中间继电器字M54
#define	              RW_M55  161	//M55-中间继电器字M55
#define	              RW_M56  162	//M56-中间继电器字M56
#define	              RW_M57  163	//M57-中间继电器字M57
#define	              RW_M58  164	//M58-中间继电器字M58
#define	              RW_M59  165	//M59-中间继电器字M59
#define	              RW_M60  166	//M60-中间继电器字M60
#define	              RW_M61  167	//M61-中间继电器字M61
#define	              RW_M62  168	//M62-中间继电器字M62
#define	              RW_M63  169	//M63-中间继电器字M63
#define	              RW_M64  170	//M64-中间继电器字M64
#define	         RW_LKPI50I1  171	//LKPI50I1-过流Ⅰ段入段闭锁继电器字
#define	        RW_LKRST50I1  172	//LKRST50I1-过流Ⅰ段返回闭锁继电器字
#define	         RW_LKPI50I2  173	//LKPI50I2-过流Ⅱ段入段闭锁继电器字
#define	        RW_LKRST50I2  174	//LKRST50I2-过流Ⅱ段返回闭锁继电器字
#define	         RW_LKPI50I3  175	//LKPI50I3-过流Ⅲ段入段闭锁继电器字
#define	        RW_LKRST50I3  176	//LKRST50I3-过流Ⅲ段返回闭锁继电器字
#define	         RW_LKPI50N1  177	//LKPI50N1-零流Ⅰ段入段闭锁继电器字
#define	        RW_LKRST50N1  178	//LKRST50N1-零流Ⅰ段返回闭锁继电器字
#define	         RW_LKPI50N2  179	//LKPI50N2-零流Ⅱ段入段闭锁继电器字
#define	        RW_LKRST50N2  180	//LKRST50N2-零流Ⅱ段返回闭锁继电器字
#define	           RW_LKPI27  181	//LKPI27-失压保护入段闭锁继电器字
#define	          RW_LKRST27  182	//LKRST27-失压保护返回闭锁继电器字
#define	         RW_LKPI50I0  183	//LKPI50I0-电流速断入段闭锁继电器字
#define	        RW_LKRST50I0  184	//LKRST50I0-电流速断返回闭锁继电器字
#define	        RW_LKPI50IOL  185	//LKPI50IOL-过负荷入段闭锁继电器字
#define	       RW_LKRST50IOL  186	//LKRST50IOL-过负荷返回闭锁继电器字
#define	        RW_LKPI50ISH  187	//LKPI50ISH-母充保护入段闭锁继电器字
#define	       RW_LKRST50ISH  188	//LKRST50ISH-母充保护返回闭锁继电器字
#define	            RW_79PIS  189	//79PIS-重合闸启动监视逻辑继电器字
#define	            RW_79CLC  190	//79CLC-重合闸动作命令撤销继电器字
#define	             RW_79LK  191	//79LK-重合闸闭锁逻辑继电器字
#define	            RW_79CLS  192	//79CLS-重合闸动作监视逻辑继电器字
#define	            RW_CFG79  193	//CFG79-重合闸配置继电器字
#define	           RW_79TRCK  194	//79TRCK-重合闸启动判断继电器字
#define	             RW_69IN  195	//69IN-远方当地中间继电器字
#define	              RW_52A  196	//52A-合位
#define	              RW_52B  197	//52B-分位
#define	              RW_REC  198	//REC-录波继电器字
#define	             RW_25LK  199	//25LK-检同期闭锁合闸继电器字
#define	            RW_25INT  200	//25INT-检同期中断逻辑继电器字
#define	               RW_H1  201	//H1-自保持中间继电器字1
#define	               RW_H2  202	//H2-自保持中间继电器字2
#define	               RW_H3  203	//H3-自保持中间继电器字3
#define	               RW_H4  204	//H4-自保持中间继电器字4
#define	               RW_H5  205	//H5-自保持中间继电器字5
#define	               RW_H6  206	//H6-自保持中间继电器字6
#define	               RW_H7  207	//H7-自保持中间继电器字7
#define	               RW_H8  208	//H8-自保持中间继电器字8
#define	               RW_H9  209	//H9-自保持中间继电器字9
#define	              RW_H10  210	//H10-自保持中间继电器字10
#define	              RW_H11  211	//H11-自保持中间继电器字11
#define	              RW_H12  212	//H12-自保持中间继电器字12
#define	              RW_H13  213	//H13-自保持中间继电器字13
#define	              RW_H14  214	//H14-自保持中间继电器字14
#define	              RW_H15  215	//H15-自保持中间继电器字15
#define	              RW_H16  216	//H16-自保持中间继电器字16
#define	              RW_H17  217	//H17-自保持中间继电器字17
#define	              RW_H18  218	//H18-自保持中间继电器字18
#define	              RW_H19  219	//H19-自保持中间继电器字19
#define	              RW_H20  220	//H20-自保持中间继电器字20
#define	              RW_H21  221	//H21-自保持中间继电器字21
#define	              RW_H22  222	//H22-自保持中间继电器字22
#define	              RW_H23  223	//H23-自保持中间继电器字23
#define	              RW_H24  224	//H24-自保持中间继电器字24
#define	              RW_H25  225	//H25-自保持中间继电器字25
#define	              RW_H26  226	//H26-自保持中间继电器字26
#define	              RW_H27  227	//H27-自保持中间继电器字27
#define	              RW_H28  228	//H28-自保持中间继电器字28
#define	              RW_H29  229	//H29-自保持中间继电器字29
#define	              RW_H30  230	//H30-自保持中间继电器字30
#define	              RW_H31  231	//H31-自保持中间继电器字31
#define	              RW_H32  232	//H32-自保持中间继电器字31
#define	             RW_79PI  233	//79PI-重合闸启动逻辑继电器字
#define	             RW_52BF  234	//52BF-断路器跳闸失败继电器字
#define	              RW_PI1  235	//PI1-备自投启动继电器字1
#define	              RW_PI2  236	//PI2-备自投启动继电器字2
#define	              RW_PI3  237	//PI3-备自投启动继电器字3
#define	              RW_PI4  238	//PI4-备自投启动继电器字4
#define	             RW_CHR1  239	//CHR1-备自投模式继电器字1
#define	             RW_CHR2  240	//CHR2-备自投模式继电器字2
#define	             RW_CHR3  241	//CHR3-备自投模式继电器字3
#define	             RW_CHR4  242	//CHR4-备自投模式继电器字4
#define	             RW_LOCK  243	//LOCK-备自投闭锁继电器字
#define	             RW_25PI  244	//25PI-检同期启动继电器字
#define	          RW_79_25PI  245	//79_25PI-重合闸启动检同期继电器字
#define	         RW_MCL_25PI  246	//MCL_25PI-手动启动检同期继电器字
#define	               RW_T1  247	//T1-T1
#define	               RW_T2  248	//T2-T2
#define	               RW_T3  249	//T3-T3
#define	               RW_T4  250	//T4-T4
#define	               RW_T5  251	//T5-T5
#define	               RW_T6  252	//T6-T6
#define	               RW_T7  253	//T7-T7
#define	               RW_T8  254	//T8-T8
#define	               RW_T9  255	//T9-T9
#define	              RW_T10  256	//T10-T10
#define	              RW_T11  257	//T11-T11
#define	              RW_T12  258	//T12-T12
#define	              RW_T13  259	//T13-T13
#define	              RW_T14  260	//T14-T14
#define	              RW_T15  261	//T15-T15
#define	              RW_T16  262	//T16-T16
#define	             RW_TSW1  263	//TSW1-TSW1
#define	             RW_TSW2  264	//TSW2-TSW2
#define	             RW_TSW3  265	//TSW3-TSW3
#define	             RW_TSW4  266	//TSW4-TSW4
#define	             RW_TSW5  267	//TSW5-TSW5
#define	             RW_TSW6  268	//TSW6-TSW6
#define	             RW_TSW7  269	//TSW7-TSW7
#define	             RW_TSW8  270	//TSW8-TSW8
#define	             RW_TCB1  271	//TCB1-TCB1
#define	             RW_TCB2  272	//TCB2-TCB2
#define	             RW_TCB3  273	//TCB3-TCB3
#define	             RW_TCB4  274	//TCB4-TCB4
#define	             RW_TCB5  275	//TCB5-TCB5
#define	             RW_TCB6  276	//TCB6-TCB6
#define	             RW_TCB7  277	//TCB7-TCB7
#define	             RW_TCB8  278	//TCB8-TCB8
#define	            RW_TSOTF  279	//TSOTF-重合于故障逻辑继电器字
#define	            RW_T52BF  280	//T52BF-断路器跳闸失败延时继电器字
#define	              RW_TSH  281	//TSH-母充保护投入时间继电器字
#define	            RW_TCHR1  282	//TCHR1-备自投充电方式继电器字1
#define	            RW_TCHR2  283	//TCHR2-备自投充电方式继电器字2
#define	            RW_TCHR3  284	//TCHR3-备自投充电方式继电器字3
#define	            RW_TCHR4  285	//TCHR4-备自投充电方式继电器字4
#define	              RW_TR1  286	//TR1-跳闸继电器1
#define	              RW_TR2  287	//TR2-跳闸继电器2
#define	              RW_TR3  288	//TR3-跳闸继电器3
#define	              RW_VR1  289	//VR1-虚拟继电器字1
#define	              RW_VR2  290	//VR2-虚拟继电器字2
#define	              RW_VR3  291	//VR3-虚拟继电器字3
#define	              RW_VR4  292	//VR4-虚拟继电器字4
#define	              RW_VR5  293	//VR5-虚拟继电器字5
#define	              RW_VR6  294	//VR6-虚拟继电器字6
#define	              RW_VR7  295	//VR7-虚拟继电器字7
#define	              RW_VR8  296	//VR8-虚拟继电器字8
#define	              RW_VR9  297	//VR9-虚拟继电器字9
#define	             RW_VR10  298	//VR10-虚拟继电器字10
#define	             RW_VR11  299	//VR11-虚拟继电器字11
#define	             RW_VR12  300	//VR12-虚拟继电器字12
#define	             RW_VR13  301	//VR13-虚拟继电器字13
#define	             RW_VR14  302	//VR14-虚拟继电器字14
#define	             RW_VR15  303	//VR15-虚拟继电器字15
#define	             RW_VR16  304	//VR16-虚拟继电器字16
#define	             RW_VR17  305	//VR17-虚拟继电器字17
#define	             RW_VR18  306	//VR18-虚拟继电器字18
#define	             RW_VR19  307	//VR19-虚拟继电器字19
#define	             RW_VR20  308	//VR20-虚拟继电器字20
#define	             RW_VR21  309	//VR21-虚拟继电器字21
#define	             RW_VR22  310	//VR22-虚拟继电器字22
#define	             RW_VR23  311	//VR23-虚拟继电器字23
#define	             RW_VR24  312	//VR24-虚拟继电器字24
#define	             RW_VR25  313	//VR25-虚拟继电器字25
#define	             RW_VR26  314	//VR26-虚拟继电器字26
#define	             RW_VR27  315	//VR27-虚拟继电器字27
#define	             RW_VR28  316	//VR28-虚拟继电器字28
#define	             RW_VR29  317	//VR29-虚拟继电器字29
#define	             RW_VR30  318	//VR30-虚拟继电器字30
#define	             RW_VR31  319	//VR31-虚拟继电器字31
#define	             RW_VR32  320	//VR32-虚拟继电器字32
#define	              RW_IN1  321	//IN1-IN1
#define	              RW_IN2  322	//IN2-IN2
#define	              RW_IN3  323	//IN3-IN3
#define	              RW_IN4  324	//IN4-IN4
#define	              RW_IN5  325	//IN5-IN5
#define	              RW_IN6  326	//IN6-IN6
#define	              RW_IN7  327	//IN7-IN7
#define	              RW_IN8  328	//IN8-IN8
#define	              RW_IN9  329	//IN9-IN9
#define	             RW_IN10  330	//IN10-IN10
#define	             RW_IN11  331	//IN11-IN11
#define	             RW_IN12  332	//IN12-IN12
#define	             RW_IN13  333	//IN13-IN13
#define	             RW_IN14  334	//IN14-IN14
#define	             RW_IN15  335	//IN15-IN15
#define	             RW_IN16  336	//IN16-IN16
#define	             RW_IN17  337	//IN17-IN17
#define	             RW_IN18  338	//IN18-IN18
#define	             RW_IN19  339	//IN19-IN19
#define	             RW_IN20  340	//IN20-IN20
#define	             RW_IN21  341	//IN21-IN21
#define	             RW_IN22  342	//IN22-IN22
#define	             RW_IN23  343	//IN23-IN23
#define	             RW_IN24  344	//IN24-IN24
#define	             RW_IN25  345	//IN25-IN25
#define	             RW_IN26  346	//IN26-IN26
#define	             RW_IN27  347	//IN27-IN27
#define	             RW_IN28  348	//IN28-IN28
#define	             RW_IN29  349	//IN29-IN29
#define	             RW_IN30  350	//IN30-IN30
#define	             RW_IN31  351	//IN31-IN31
#define	             RW_IN32  352	//IN32-IN32
#define	             RW_IN33  353	//IN33-IN33
#define	             RW_IN34  354	//IN34-IN34
#define	             RW_IN35  355	//IN35-信号复归
#define	             RW_IN36  356	//IN36-跳闸位置
#define	             RW_IN37  357	//IN37-合闸位置
#define	             RW_IN38  358	//IN38-手跳重动
#define	             RW_IN39  359	//IN39-遥控预置
#define	             RW_IN40  360	//IN40-遥控1预置
#define	             RW_IN41  361	//IN41-遥控2预置
#define	             RW_IN42  362	//IN42-遥控3预置
#define	           RW_INVIN1  363	//INVIN1-VIN1
#define	           RW_INVIN2  364	//INVIN2-VIN2
#define	           RW_INVIN3  365	//INVIN3-VIN3
#define	           RW_INVIN4  366	//INVIN4-VIN4
#define	           RW_INVIN5  367	//INVIN5-VIN5
#define	           RW_INVIN6  368	//INVIN6-VIN6
#define	           RW_INVIN7  369	//INVIN7-VIN7
#define	           RW_INVIN8  370	//INVIN8-VIN8
#define	           RW_INVIN9  371	//INVIN9-VIN9
#define	          RW_INVIN10  372	//INVIN10-VIN10
#define	          RW_INVIN11  373	//INVIN11-VIN11
#define	          RW_INVIN12  374	//INVIN12-VIN12
#define	          RW_INVIN13  375	//INVIN13-VIN13
#define	          RW_INVIN14  376	//INVIN14-VIN14
#define	          RW_INVIN15  377	//INVIN15-VIN15
#define	          RW_INVIN16  378	//INVIN16-VIN16
#define	             RW_OUT1  379	//OUT1-开出1继电器字
#define	             RW_OUT2  380	//OUT2-开出2继电器字
#define	             RW_OUT3  381	//OUT3-开出3继电器字
#define	             RW_OUT4  382	//OUT4-开出4继电器字
#define	             RW_OUT5  383	//OUT5-开出5继电器字
#define	             RW_OUT6  384	//OUT6-开出6继电器字
#define	             RW_OUT7  385	//OUT7-开出7继电器字
#define	             RW_OUT8  386	//OUT8-开出8继电器字
#define	             RW_OUT9  387	//OUT9-开出9继电器字
#define	            RW_OUT10  388	//OUT10-开出10继电器字
#define	            RW_OUT11  389	//OUT11-开出11继电器字
#define	            RW_OUT12  390	//OUT12-开出12继电器字
#define	            RW_OUT13  391	//OUT13-开出13继电器字
#define	            RW_OUT14  392	//OUT14-开出14继电器字
#define	            RW_OUT15  393	//OUT15-开出15继电器字
#define	            RW_OUT16  394	//OUT16-开出16继电器字
#define	            RW_OUT17  395	//OUT17-开出17继电器字
#define	            RW_OUT18  396	//OUT18-开出18继电器字
#define	            RW_OUT19  397	//OUT19-开出19继电器字
#define	           RW_SL50I0  398	//SL50I0-电流速断压板继电器字
#define	           RW_SL50I1  399	//SL50I1-过流Ⅰ段软压板继电器字
#define	           RW_SL50I2  400	//SL50I2-过流Ⅱ段软压板继电器字
#define	           RW_SL50I3  401	//SL50I3-过流Ⅲ段软压板继电器字
#define	           RW_SL50N1  402	//SL50N1-零序过流Ⅰ段软压板继电器字
#define	           RW_SL50N2  403	//SL50N2-零序过流Ⅱ段软压板继电器字
#define	          RW_SL50ISH  404	//SL50ISH-母线充电保护压板继电器字
#define	             RW_SL27  405	//SL27-低压保护软压板继电器字
#define	             RW_SL79  406	//SL79-重合闸软压板继电器字
#define	             RW_SLJD  407	//SLJD-接地选线软压板继电器字
#define	             RW_SL25  408	//SL25-检同期软压板继电器字
#define	            RW_SLBZT  409	//SLBZT-备自投软压板继电器字
#define	          RW_SL50IOL  410	//SL50IOL-过负荷告警压板继电器字
#define	          RW_SLMAINT  411	//SLMAINT-检修压板继电器字
#define	           RW_SLBAK1  412	//SLBAK1-备用压板1
#define	           RW_SLBAK2  413	//SLBAK2-备用压板2
#define	            RW_F50I0  414	//F50I0-电流速断配置继电器字
#define	         RW_F50I0_47  415	//F50I0_47-电流速断复压闭锁配置继电器字
#define	         RW_F50I0_67  416	//F50I0_67-电流速断功率方向配置继电器字
#define	         RW_F50I0_PD  417	//F50I0_PD-电流速断方向正向配置继电器字
#define	            RW_F50I1  418	//F50I1-过流I段配置继电器字
#define	         RW_F50I1_47  419	//F50I1_47-过流Ⅰ段复压闭锁配置继电器字
#define	         RW_F50I1_67  420	//F50I1_67-过流Ⅰ段功率方向配置继电器字
#define	         RW_F50I1_PD  421	//F50I1_PD-过流Ⅰ段正向配置继电器字
#define	            RW_F50I2  422	//F50I2-过流Ⅱ段配置继电器字
#define	         RW_F50I2_47  423	//F50I2_47-过流Ⅱ段复压闭锁配置继电器字
#define	         RW_F50I2_67  424	//F50I2_67-过流Ⅱ段功率方向配置继电器字
#define	         RW_F50I2_PD  425	//F50I2_PD-过流Ⅱ段正向配置继电器字
#define	            RW_F50I3  426	//F50I3-过流Ⅲ段配置继电器字
#define	         RW_F50I3_47  427	//F50I3_47-过流Ⅲ段复压闭锁配置继电器字
#define	         RW_F50I3_67  428	//F50I3_67-过流Ⅲ段功率方向配置继电器字
#define	         RW_F50I3_PD  429	//F50I3_PD-过流Ⅲ段正向配置继电器字
#define	            RW_F50N1  430	//F50N1-零序过流Ⅰ段配置继电器字
#define	            RW_F50N2  431	//F50N2-零序过流Ⅱ段配置继电器字
#define	           RW_F50ISH  432	//F50ISH-母充保护配置继电器字
#define	             RW_F27U  433	//F27U-低压保护配置继电器字
#define	         RW_F50I0_79  434	//F50I0_79-电流速断重合配置继电器字
#define	         RW_F50I1_79  435	//F50I1_79-过流Ⅰ段重合配置继电器字
#define	         RW_F50I2_79  436	//F50I2_79-过流Ⅱ段重合配置继电器字
#define	         RW_F50I3_79  437	//F50I3_79-过流Ⅲ段重合配置继电器字
#define	            RW_F79UC  438	//F79UC-偷跳重合闸配置继电器字
#define	           RW_F79_25  439	//F79_25-重合闸检同期配置继电器字
#define	           RW_F79_27  440	//F79_27-重合闸检无压配置继电器字
#define	           RW_F79_59  441	//F79_59-重合闸检有压配置继电器字
#define	         RW_F50I1_94  442	//F50I1_94-过流Ⅰ段加速配置继电器字
#define	         RW_F50I2_94  443	//F50I2_94-过流Ⅱ段加速配置继电器字
#define	         RW_F50I3_94  444	//F50I3_94-过流Ⅲ段加速配置继电器字
#define	          RW_F59N_74  445	//F59N_74-零压告警配置继电器字
#define	            RW_FJDXX  446	//FJDXX-接地选线配置继电器字
#define	            RW_FXHXQ  447	//FXHXQ-消弧线圈接地配置继电器字
#define	            RW_F74PT  448	//F74PT-PT断线检查配置继电器字
#define	              RW_F25  449	//F25-手合检同期配置继电器字
#define	           RW_FSH_27  450	//FSH_27-手合检无压配置继电器字
#define	             RW_FBZT  451	//FBZT-备自投配置继电器字
#define	           RW_F50IOL  452	//F50IOL-过负荷告警继电器字
#define	            RW_FCTBR  453	//FCTBR-CT断线检查配置继电器字
#define	            RW_F52BF  454	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  455	//FBAK1-备用1配置继电器字
#define	            RW_FBAK2  456	//FBAK2-备用2配置继电器字
#define	              RW_PW1  457	//PW1-脉冲继电器1
#define	              RW_PW2  458	//PW2-脉冲继电器2
#define	              RW_PW3  459	//PW3-脉冲继电器3
#define	              RW_PW4  460	//PW4-脉冲继电器4
#define	              RW_PW5  461	//PW5-脉冲继电器5
#define	              RW_PW6  462	//PW6-脉冲继电器6
#define	              RW_PW7  463	//PW7-脉冲继电器7
#define	              RW_PW8  464	//PW8-脉冲继电器8
#define	              RW_PW9  465	//PW9-脉冲继电器9
#define	             RW_PW10  466	//PW10-脉冲继电器10
#define	             RW_PW11  467	//PW11-脉冲继电器11
#define	             RW_PW12  468	//PW12-脉冲继电器12
#define	             RW_PW13  469	//PW13-脉冲继电器13
#define	             RW_PW14  470	//PW14-脉冲继电器14
#define	             RW_PW15  471	//PW15-脉冲继电器15
#define	             RW_PW16  472	//PW16-脉冲继电器16
#define	              RW_TO1  473	//TO1-传动继电器1
#define	              RW_TO2  474	//TO2-传动继电器2
#define	              RW_TO3  475	//TO3-传动继电器3
#define	              RW_TO4  476	//TO4-传动继电器4
#define	              RW_TO5  477	//TO5-传动继电器5
#define	              RW_TO6  478	//TO6-传动继电器6
#define	              RW_TO7  479	//TO7-传动继电器7
#define	              RW_TO8  480	//TO8-传动继电器8
#define	              RW_TO9  481	//TO9-传动继电器9
#define	             RW_TO10  482	//TO10-传动继电器10
#define	             RW_TO11  483	//TO11-传动继电器11
#define	             RW_SIG1  484	//SIG1-信号继电器1
#define	             RW_SIG2  485	//SIG2-信号继电器2
#define	             RW_SIG3  486	//SIG3-信号继电器3
#define	             RW_SIG4  487	//SIG4-信号继电器4
#define	             RW_SIG5  488	//SIG5-信号继电器5
#define	             RW_SIG6  489	//SIG6-信号继电器6
#define	             RW_SIG7  490	//SIG7-信号继电器7
#define	             RW_SIG8  491	//SIG8-信号继电器8
#define	             RW_SIG9  492	//SIG9-信号继电器9
#define	            RW_SIG10  493	//SIG10-信号继电器10
#define	            RW_SIG11  494	//SIG11-信号继电器11
#define	            RW_SIG12  495	//SIG12-信号继电器12
#define	            RW_SIG13  496	//SIG13-信号继电器13
#define	            RW_SIG14  497	//SIG14-信号继电器14
#define	            RW_SIG15  498	//SIG15-信号继电器15
#define	            RW_SIG16  499	//SIG16-信号继电器16
#define	            RW_LED1G  500	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  501	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  502	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  503	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  504	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  505	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  506	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  507	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  508	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  509	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  510	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  511	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  512	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  513	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  514	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  515	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  516	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  517	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  518	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  519	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  520	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  521	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  522	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  523	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  524	//VIN1-VIN1
#define	             RW_VIN2  525	//VIN2-VIN2
#define	             RW_VIN3  526	//VIN3-VIN3
#define	             RW_VIN4  527	//VIN4-VIN4
#define	             RW_VIN5  528	//VIN5-VIN5
#define	             RW_VIN6  529	//VIN6-VIN6
#define	             RW_VIN7  530	//VIN7-VIN7
#define	             RW_VIN8  531	//VIN8-VIN8
#define	             RW_VIN9  532	//VIN9-VIN9
#define	            RW_VIN10  533	//VIN10-VIN10
#define	            RW_VIN11  534	//VIN11-VIN11
#define	            RW_VIN12  535	//VIN12-VIN12
#define	            RW_VIN13  536	//VIN13-VIN13
#define	            RW_VIN14  537	//VIN14-VIN14
#define	            RW_VIN15  538	//VIN15-VIN15
#define	            RW_VIN16  539	//VIN16-VIN16
#define	             RW_RCP1  540	//RCP1-遥控预置继电器1
#define	             RW_RCP2  541	//RCP2-遥控预置继电器2
#define	             RW_RCP3  542	//RCP3-遥控预置继电器3
#define	             RW_RCP4  543	//RCP4-遥控预置继电器4
#define	             RW_RCP5  544	//RCP5-遥控预置继电器5
#define	             RW_RCP6  545	//RCP6-遥控预置继电器6
#define	             RW_RCP7  546	//RCP7-遥控预置继电器7
#define	             RW_RCP8  547	//RCP8-遥控预置继电器8
#define	             RW_RCP9  548	//RCP9-遥控预置继电器9
#define	            RW_RCP10  549	//RCP10-遥控预置继电器10
#define	            RW_RCP11  550	//RCP11-遥控预置继电器11
#define	            RW_RCP12  551	//RCP12-遥控预置继电器12
#define	            RW_RCP13  552	//RCP13-遥控预置继电器13
#define	            RW_RCP14  553	//RCP14-遥控预置继电器14
#define	            RW_RCP15  554	//RCP15-遥控预置继电器15
#define	            RW_RCP16  555	//RCP16-遥控预置继电器16
#define	            RW_RCP17  556	//RCP17-遥控预置继电器17
#define	            RW_RCP18  557	//RCP18-遥控预置继电器18
#define	            RW_RCP19  558	//RCP19-遥控预置继电器19
#define	            RW_RCP20  559	//RCP20-遥控预置继电器20
#define	            RW_RCP21  560	//RCP21-遥控预置继电器21
#define	            RW_RCP22  561	//RCP22-遥控预置继电器22
#define	            RW_RCP23  562	//RCP23-遥控预置继电器23
#define	            RW_RCP24  563	//RCP24-遥控预置继电器24
#define	            RW_RCP25  564	//RCP25-遥控预置继电器25
#define	            RW_RCP26  565	//RCP26-遥控预置继电器26
#define	            RW_RCP27  566	//RCP27-遥控预置继电器27
#define	            RW_RCP28  567	//RCP28-遥控预置继电器28
#define	            RW_RCP29  568	//RCP29-遥控预置继电器29
#define	            RW_RCP30  569	//RCP30-遥控预置继电器30
#define	            RW_RCP31  570	//RCP31-遥控预置继电器31
#define	            RW_RCP32  571	//RCP32-遥控预置继电器32
#define	            RW_RCP33  572	//RCP33-遥控预置继电器33
#define	            RW_RCP34  573	//RCP34-遥控预置继电器34
#define	             RW_RCE1  574	//RCE1-遥控执行继电器1
#define	             RW_RCE2  575	//RCE2-遥控执行继电器2
#define	             RW_RCE3  576	//RCE3-遥控执行继电器3
#define	             RW_RCE4  577	//RCE4-遥控执行继电器4
#define	             RW_RCE5  578	//RCE5-遥控执行继电器5
#define	             RW_RCE6  579	//RCE6-遥控执行继电器6
#define	             RW_RCE7  580	//RCE7-遥控执行继电器7
#define	             RW_RCE8  581	//RCE8-遥控执行继电器8
#define	             RW_RCE9  582	//RCE9-遥控执行继电器9
#define	            RW_RCE10  583	//RCE10-遥控执行继电器10
#define	            RW_RCE11  584	//RCE11-遥控执行继电器11
#define	            RW_RCE12  585	//RCE12-遥控执行继电器12
#define	            RW_RCE13  586	//RCE13-遥控执行继电器13
#define	            RW_RCE14  587	//RCE14-遥控执行继电器14
#define	            RW_RCE15  588	//RCE15-遥控执行继电器15
#define	            RW_RCE16  589	//RCE16-遥控执行继电器16
#define	            RW_RCE17  590	//RCE17-遥控执行继电器17
#define	            RW_RCE18  591	//RCE18-遥控执行继电器18
#define	            RW_RCE19  592	//RCE19-遥控执行继电器19
#define	            RW_RCE20  593	//RCE20-遥控执行继电器20
#define	            RW_RCE21  594	//RCE21-遥控执行继电器21
#define	            RW_RCE22  595	//RCE22-遥控执行继电器22
#define	            RW_RCE23  596	//RCE23-遥控执行继电器23
#define	            RW_RCE24  597	//RCE24-遥控执行继电器24
#define	            RW_RCE25  598	//RCE25-遥控执行继电器25
#define	            RW_RCE26  599	//RCE26-遥控执行继电器26
#define	            RW_RCE27  600	//RCE27-遥控执行继电器27
#define	            RW_RCE28  601	//RCE28-遥控执行继电器28
#define	            RW_RCE29  602	//RCE29-遥控执行继电器29
#define	            RW_RCE30  603	//RCE30-遥控执行继电器30
#define	            RW_RCE31  604	//RCE31-遥控执行继电器31
#define	            RW_RCE32  605	//RCE32-遥控执行继电器32
#define	            RW_RCE33  606	//RCE33-遥控执行继电器33
#define	            RW_RCE34  607	//RCE34-遥控执行继电器34
#define	             RW_RCV1  608	//RCV1-遥控反校继电器1
#define	             RW_RCV2  609	//RCV2-遥控反校继电器2
#define	             RW_RCV3  610	//RCV3-遥控反校继电器3
#define	             RW_RCV4  611	//RCV4-遥控反校继电器4
#define	             RW_RCV5  612	//RCV5-遥控反校继电器5
#define	             RW_RCV6  613	//RCV6-遥控反校继电器6
#define	             RW_RCV7  614	//RCV7-遥控反校继电器7
#define	             RW_RCV8  615	//RCV8-遥控反校继电器8
#define	             RW_RCV9  616	//RCV9-遥控反校继电器9
#define	            RW_RCV10  617	//RCV10-遥控反校继电器10
#define	            RW_RCV11  618	//RCV11-遥控反校继电器11
#define	            RW_RCV12  619	//RCV12-遥控反校继电器12
#define	            RW_RCV13  620	//RCV13-遥控反校继电器13
#define	            RW_RCV14  621	//RCV14-遥控反校继电器14
#define	            RW_RCV15  622	//RCV15-遥控反校继电器15
#define	            RW_RCV16  623	//RCV16-遥控反校继电器16
#define	            RW_RCV17  624	//RCV17-遥控反校继电器17
#define	            RW_RCV18  625	//RCV18-遥控反校继电器18
#define	            RW_RCV19  626	//RCV19-遥控反校继电器19
#define	            RW_RCV20  627	//RCV20-遥控反校继电器20
#define	            RW_RCV21  628	//RCV21-遥控反校继电器21
#define	            RW_RCV22  629	//RCV22-遥控反校继电器22
#define	            RW_RCV23  630	//RCV23-遥控反校继电器23
#define	            RW_RCV24  631	//RCV24-遥控反校继电器24
#define	            RW_RCV25  632	//RCV25-遥控反校继电器25
#define	            RW_RCV26  633	//RCV26-遥控反校继电器26
#define	            RW_RCV27  634	//RCV27-遥控反校继电器27
#define	            RW_RCV28  635	//RCV28-遥控反校继电器28
#define	            RW_RCV29  636	//RCV29-遥控反校继电器29
#define	            RW_RCV30  637	//RCV30-遥控反校继电器30
#define	            RW_RCV31  638	//RCV31-遥控反校继电器31
#define	            RW_RCV32  639	//RCV32-遥控反校继电器32
#define	            RW_RCV33  640	//RCV33-遥控反校继电器33
#define	            RW_RCV34  641	//RCV34-遥控反校继电器34
#define	              RW_GE1  642	//GE1-大于等于继电器1
#define	              RW_GE2  643	//GE2-大于等于继电器2
#define	              RW_GE3  644	//GE3-大于等于继电器3
#define	              RW_GE4  645	//GE4-大于等于继电器4
#define	              RW_GE5  646	//GE5-大于等于继电器5
#define	              RW_GE6  647	//GE6-大于等于继电器6
#define	              RW_GE7  648	//GE7-大于等于继电器7
#define	              RW_GE8  649	//GE8-大于等于继电器8
#define	           RW_CONST1  650	//CONST1-常数时间继电器1
#define	           RW_CONST2  651	//CONST2-常数时间继电器2
#define	           RW_CONST3  652	//CONST3-常数时间继电器3
#define	           RW_CONST4  653	//CONST4-常数时间继电器4
#define	           RW_CONST5  654	//CONST5-常数时间继电器5
#define	           RW_CONST6  655	//CONST6-常数时间继电器6
#define	           RW_CONST7  656	//CONST7-常数时间继电器7
#define	           RW_CONST8  657	//CONST8-常数时间继电器8
#define	              RW_TX1  658	//TX1-镜像发送继电器字1
#define	              RW_TX2  659	//TX2-镜像发送继电器字2
#define	              RW_TX3  660	//TX3-镜像发送继电器字3
#define	              RW_TX4  661	//TX4-镜像发送继电器字4
#define	              RW_TX5  662	//TX5-镜像发送继电器字5
#define	              RW_TX6  663	//TX6-镜像发送继电器字6
#define	              RW_TX7  664	//TX7-镜像发送继电器字7
#define	              RW_TX8  665	//TX8-镜像发送继电器字8
#define	              RW_TX9  666	//TX9-镜像发送继电器字9
#define	             RW_TX10  667	//TX10-镜像发送继电器字10
#define	             RW_TX11  668	//TX11-镜像发送继电器字11
#define	             RW_TX12  669	//TX12-镜像发送继电器字12
#define	             RW_TX13  670	//TX13-镜像发送继电器字13
#define	             RW_TX14  671	//TX14-镜像发送继电器字14
#define	             RW_TX15  672	//TX15-镜像发送继电器字15
#define	             RW_TX16  673	//TX16-镜像发送继电器字16
#define	              RW_RX1  674	//RX1-镜像接收继电器字1
#define	              RW_RX2  675	//RX2-镜像接收继电器字2
#define	              RW_RX3  676	//RX3-镜像接收继电器字3
#define	              RW_RX4  677	//RX4-镜像接收继电器字4
#define	              RW_RX5  678	//RX5-镜像接收继电器字5
#define	              RW_RX6  679	//RX6-镜像接收继电器字6
#define	              RW_RX7  680	//RX7-镜像接收继电器字7
#define	              RW_RX8  681	//RX8-镜像接收继电器字8
#define	              RW_RX9  682	//RX9-镜像接收继电器字9
#define	             RW_RX10  683	//RX10-镜像接收继电器字10
#define	             RW_RX11  684	//RX11-镜像接收继电器字11
#define	             RW_RX12  685	//RX12-镜像接收继电器字12
#define	             RW_RX13  686	//RX13-镜像接收继电器字13
#define	             RW_RX14  687	//RX14-镜像接收继电器字14
#define	             RW_RX15  688	//RX15-镜像接收继电器字15
#define	             RW_RX16  689	//RX16-镜像接收继电器字16
#define	             RW_CNT1  690	//CNT1-计数继电器字1
#define	             RW_CNT2  691	//CNT2-计数继电器字2
#define	             RW_CNT3  692	//CNT3-计数继电器字3
#define	             RW_CNT4  693	//CNT4-计数继电器字4
#define	             RW_CNT5  694	//CNT5-计数继电器字5
#define	             RW_CNT6  695	//CNT6-计数继电器字6
#define	             RW_CNT7  696	//CNT7-计数继电器字7
#define	             RW_CNT8  697	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			698		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			105	

//中间继电器字
#define	MID_RELAY_WORD_START			107	
#define	MID_RELAY_WORD_NUM			94	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			201	
#define	KEEP_RELAY_WORD_NUM			46	

//时间继电器字
#define	TIME_RELAY_WORD_START			247	
#define	TIME_RELAY_WORD_NUM			39	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			286	
#define	TRIP_RELAY_WORD_NUM			3	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			289	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			321	
#define	INPUT_RELAY_WORD_NUM			58	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			379	
#define	OUTPUT_RELAY_WORD_NUM			19	

//压板继电器
#define	SW_RELAY_WORD_START			398	
#define	SW_RELAY_WORD_NUM			16	

//配置继电器
#define	CFG_RELAY_WORD_START			414	
#define	CFG_RELAY_WORD_NUM			43	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			457	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			473	
#define	CTRLTEST_RELAY_WORD_NUM			11	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			484	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			500	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			524	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			540	
#define	RCP_RELAY_WORD_NUM			34	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			574	
#define	RCE_RELAY_WORD_NUM			34	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			608	
#define	RCV_RELAY_WORD_NUM			34	

//大于等于继电器
#define	GE_RELAY_WORD_START			642	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			650	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			658	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			674	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			690	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			690	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			690	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			690	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			690	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			690	
#define	CNT_RELAY_WORD_NUM			8	


#endif