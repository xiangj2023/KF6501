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
#define	          RW_PI87U_A    2	//PI87U_A-A相差动速断入段继电器字
#define	          RW_TR87U_A    3	//TR87U_A-A相差动速断动作继电器字
#define	         RW_LPC87U_A    4	//LPC87U_A-A相差动速断保护循环控制继电器字
#define	          RW_PI87U_B    5	//PI87U_B-B相差动速断入段继电器字
#define	          RW_TR87U_B    6	//TR87U_B-B相差动速断动作继电器字
#define	         RW_LPC87U_B    7	//LPC87U_B-B相差动速断保护循环控制继电器字
#define	          RW_PI87U_C    8	//PI87U_C-C相差流速断入段继电器字
#define	          RW_TR87U_C    9	//TR87U_C-C相差流速断动作继电器字
#define	         RW_LPC87U_C   10	//LPC87U_C-C相差流速断保护循环控制继电器字
#define	          RW_PI87R_A   11	//PI87R_A-A相比率差动入段继电器字
#define	          RW_TR87R_A   12	//TR87R_A-A相比率差动动作继电器字
#define	         RW_LPC87R_A   13	//LPC87R_A-A相比率差动保护循环控制继电器字
#define	          RW_PI87R_B   14	//PI87R_B-B相比率差动入段继电器字
#define	          RW_TR87R_B   15	//TR87R_B-B相比率差动动作继电器字
#define	         RW_LPC87R_B   16	//LPC87R_B-B相比率差动保护循环控制继电器字
#define	          RW_PI87R_C   17	//PI87R_C-C相比率差动入段继电器字
#define	          RW_TR87R_C   18	//TR87R_C-C相比率差动动作继电器字
#define	         RW_LPC87R_C   19	//LPC87R_C-C相比率差动保护循环控制继电器字
#define	        RW_LK68IDAH2   20	//LK68IDAH2-A差流二次谐波闭锁继电器字
#define	        RW_LK68IDAH5   21	//LK68IDAH5-A差流五次谐波闭锁继电器字
#define	        RW_LK68IRAH2   22	//LK68IRAH2-A制动电流二次谐波闭锁继电器字
#define	         RW_LK68IDAH   23	//LK68IDAH-A差流综合谐波闭锁继电器字
#define	        RW_LK68IDBH2   24	//LK68IDBH2-B差流二次谐波闭锁继电器字
#define	        RW_LK68IDBH5   25	//LK68IDBH5-B差流五次谐波闭锁继电器字
#define	        RW_LK68IRBH2   26	//LK68IRBH2-B制动电流二次谐波闭锁继电器字
#define	         RW_LK68IDBH   27	//LK68IDBH-B差流综合谐波闭锁继电器字
#define	        RW_LK68IDCH2   28	//LK68IDCH2-C差流二次谐波闭锁继电器字
#define	        RW_LK68IDCH5   29	//LK68IDCH5-C差流五次谐波闭锁继电器字
#define	        RW_LK68IRCH2   30	//LK68IRCH2-C制动电流二次次谐波闭锁继电器字
#define	         RW_LK68IDCH   31	//LK68IDCH-C差流综合谐波闭锁继电器字
#define	             RW_AR87   32	//AR87-差流越限告警继电器字
#define	           RW_PICKUP   33	//PICKUP-模值突变量启动继电器字
#define	             RW_50HA   34	//50HA-IHA有流继电器字
#define	             RW_37HA   35	//37HA-IHA无流继电器字
#define	             RW_50HB   36	//50HB-IHB有流继电器字
#define	             RW_37HB   37	//37HB-IHB无流继电器字
#define	             RW_50HC   38	//50HC-IHC有流继电器字
#define	             RW_37HC   39	//37HC-IHC无流继电器字
#define	             RW_50L1   40	//50L1-IL1有流继电器字
#define	             RW_37L1   41	//37L1-IL1无流继电器字
#define	             RW_50L2   42	//50L2-IL2有流继电器字
#define	             RW_37L2   43	//37L2-IL2无流继电器字
#define	             RW_50L3   44	//50L3-IL3有流继电器字
#define	             RW_37L3   45	//37L3-IL3无流继电器字
#define	             RW_50L4   46	//50L4-IL4有流继电器字
#define	             RW_37L4   47	//37L4-IL4无流继电器字
#define	          RW_ET11ERR   48	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   49	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   50	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   51	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   52	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   53	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   54	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   55	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   56	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   57	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   58	//COM1ERR-串口连接状态继电器字
#define	          RW_CAN1ERR   59	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   60	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   61	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR   62	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR   63	//NULL3ERR-空3连接状态继电器字
#define	        RW_MAINTLINK   64	//MAINTLINK-维护状态继电器字
#define	             RW_59UA   65	//59UA-UA有压继电器字
#define	             RW_27UA   66	//27UA-UA无压继电器字
#define	             RW_59UB   67	//59UB-UB有压继电器字
#define	             RW_27UB   68	//27UB-UB无压继电器字
#define	             RW_59UC   69	//59UC-UC有压继电器字
#define	             RW_27UC   70	//27UC-UC无压继电器字
#define	        RW_LKPI87U_A   71	//LKPI87U_A-A相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_A   72	//LKRST87U_A-A相差动速断返回闭锁继电器字
#define	        RW_LKPI87U_B   73	//LKPI87U_B-B相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_B   74	//LKRST87U_B-B相差动速断返回闭锁继电器字
#define	        RW_LKPI87U_C   75	//LKPI87U_C-C相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_C   76	//LKRST87U_C-C相差动速断返回闭锁继电器字
#define	        RW_LKPI87R_A   77	//LKPI87R_A-A相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_A   78	//LKRST87R_A-A相比率差动返回闭锁继电器字
#define	        RW_LKPI87R_B   79	//LKPI87R_B-B相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_B   80	//LKRST87R_B-B相比率差动返回闭锁继电器字
#define	        RW_LKPI87R_C   81	//LKPI87R_C-C相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_C   82	//LKRST87R_C-C相比率差动返回闭锁继电器字
#define	               RW_M1   83	//M1-中间继电器字M1
#define	               RW_M2   84	//M2-中间继电器字M2
#define	               RW_M3   85	//M3-中间继电器字M3
#define	               RW_M4   86	//M4-中间继电器字M4
#define	               RW_M5   87	//M5-中间继电器字M5
#define	               RW_M6   88	//M6-中间继电器字M6
#define	               RW_M7   89	//M7-中间继电器字M7
#define	               RW_M8   90	//M8-中间继电器字M8
#define	               RW_M9   91	//M9-中间继电器字M9
#define	              RW_M10   92	//M10-中间继电器字M10
#define	              RW_M11   93	//M11-中间继电器字M11
#define	              RW_M12   94	//M12-中间继电器字M12
#define	              RW_M13   95	//M13-中间继电器字M13
#define	              RW_M14   96	//M14-中间继电器字M14
#define	              RW_M15   97	//M15-中间继电器字M15
#define	              RW_M16   98	//M16-中间继电器字M16
#define	              RW_M17   99	//M17-中间继电器字M17
#define	              RW_M18  100	//M18-中间继电器字M18
#define	              RW_M19  101	//M19-中间继电器字M19
#define	              RW_M20  102	//M20-中间继电器字M20
#define	              RW_M21  103	//M21-中间继电器字M21
#define	              RW_M22  104	//M22-中间继电器字M22
#define	              RW_M23  105	//M23-中间继电器字M23
#define	              RW_M24  106	//M24-中间继电器字M24
#define	              RW_M25  107	//M25-中间继电器字M25
#define	              RW_M26  108	//M26-中间继电器字M26
#define	              RW_M27  109	//M27-中间继电器字M27
#define	              RW_M28  110	//M28-中间继电器字M28
#define	              RW_M29  111	//M29-中间继电器字M29
#define	              RW_M30  112	//M30-中间继电器字M30
#define	              RW_M31  113	//M31-中间继电器字M31
#define	              RW_M32  114	//M32-中间继电器字M32
#define	              RW_M33  115	//M33-中间继电器字M33
#define	              RW_M34  116	//M34-中间继电器字M34
#define	              RW_M35  117	//M35-中间继电器字M35
#define	              RW_M36  118	//M36-中间继电器字M36
#define	              RW_M37  119	//M37-中间继电器字M37
#define	              RW_M38  120	//M38-中间继电器字M38
#define	              RW_M39  121	//M39-中间继电器字M39
#define	              RW_M40  122	//M40-中间继电器字M40
#define	              RW_M41  123	//M41-中间继电器字M41
#define	              RW_M42  124	//M42-中间继电器字M42
#define	              RW_M43  125	//M43-中间继电器字M43
#define	              RW_M44  126	//M44-中间继电器字M44
#define	              RW_M45  127	//M45-中间继电器字M45
#define	              RW_M46  128	//M46-中间继电器字M46
#define	              RW_M47  129	//M47-中间继电器字M47
#define	              RW_M48  130	//M48-中间继电器字M48
#define	              RW_M49  131	//M49-中间继电器字M49
#define	              RW_M50  132	//M50-中间继电器字M50
#define	              RW_M51  133	//M51-中间继电器字M51
#define	              RW_M52  134	//M52-中间继电器字M52
#define	              RW_M53  135	//M53-中间继电器字M53
#define	              RW_M54  136	//M54-中间继电器字M54
#define	              RW_M55  137	//M55-中间继电器字M55
#define	              RW_M56  138	//M56-中间继电器字M56
#define	              RW_M57  139	//M57-中间继电器字M57
#define	              RW_M58  140	//M58-中间继电器字M58
#define	              RW_M59  141	//M59-中间继电器字M59
#define	              RW_M60  142	//M60-中间继电器字M60
#define	              RW_M61  143	//M61-中间继电器字M61
#define	              RW_M62  144	//M62-中间继电器字M62
#define	              RW_M63  145	//M63-中间继电器字M63
#define	              RW_M64  146	//M64-中间继电器字M64
#define	              RW_REC  147	//REC-录波继电器字
#define	             RW_69IN  148	//69IN-远方当地中间继电器字
#define	             RW_52AH  149	//52AH-合位
#define	             RW_52BH  150	//52BH-分位
#define	            RW_52ALA  151	//52ALA-低侧A断路器合位
#define	            RW_52BLA  152	//52BLA-低侧A断路器分位
#define	            RW_52ALB  153	//52ALB-低侧B断路器合位
#define	            RW_52BLB  154	//52BLB-低侧B断路器分位
#define	               RW_H1  155	//H1-自保持中间继电器字1
#define	               RW_H2  156	//H2-自保持中间继电器字2
#define	               RW_H3  157	//H3-自保持中间继电器字3
#define	               RW_H4  158	//H4-自保持中间继电器字4
#define	               RW_H5  159	//H5-自保持中间继电器字5
#define	               RW_H6  160	//H6-自保持中间继电器字6
#define	               RW_H7  161	//H7-自保持中间继电器字7
#define	               RW_H8  162	//H8-自保持中间继电器字8
#define	               RW_H9  163	//H9-自保持中间继电器字9
#define	              RW_H10  164	//H10-自保持中间继电器字10
#define	              RW_H11  165	//H11-自保持中间继电器字11
#define	              RW_H12  166	//H12-自保持中间继电器字12
#define	              RW_H13  167	//H13-自保持中间继电器字13
#define	              RW_H14  168	//H14-自保持中间继电器字14
#define	              RW_H15  169	//H15-自保持中间继电器字15
#define	              RW_H16  170	//H16-自保持中间继电器字16
#define	              RW_H17  171	//H17-自保持中间继电器字17
#define	              RW_H18  172	//H18-自保持中间继电器字18
#define	              RW_H19  173	//H19-自保持中间继电器字19
#define	              RW_H20  174	//H20-自保持中间继电器字20
#define	              RW_H21  175	//H21-自保持中间继电器字21
#define	              RW_H22  176	//H22-自保持中间继电器字22
#define	              RW_H23  177	//H23-自保持中间继电器字23
#define	              RW_H24  178	//H24-自保持中间继电器字24
#define	              RW_H25  179	//H25-自保持中间继电器字25
#define	              RW_H26  180	//H26-自保持中间继电器字26
#define	              RW_H27  181	//H27-自保持中间继电器字27
#define	              RW_H28  182	//H28-自保持中间继电器字28
#define	              RW_H29  183	//H29-自保持中间继电器字29
#define	              RW_H30  184	//H30-自保持中间继电器字30
#define	              RW_H31  185	//H31-自保持中间继电器字31
#define	              RW_H32  186	//H32-自保持中间继电器字31
#define	            RW_52BFH  187	//52BFH-高侧断路器跳闸失败继电器字
#define	           RW_52BFLA  188	//52BFLA-低侧A断路器跳闸失败继电器字
#define	           RW_52BFLB  189	//52BFLB-低侧B断路器跳闸失败继电器字
#define	               RW_T1  190	//T1-T1
#define	               RW_T2  191	//T2-T2
#define	               RW_T3  192	//T3-T3
#define	               RW_T4  193	//T4-T4
#define	               RW_T5  194	//T5-T5
#define	               RW_T6  195	//T6-T6
#define	               RW_T7  196	//T7-T7
#define	               RW_T8  197	//T8-T8
#define	               RW_T9  198	//T9-T9
#define	              RW_T10  199	//T10-T10
#define	              RW_T11  200	//T11-T11
#define	              RW_T12  201	//T12-T12
#define	              RW_T13  202	//T13-T13
#define	              RW_T14  203	//T14-T14
#define	              RW_T15  204	//T15-T15
#define	              RW_T16  205	//T16-T16
#define	             RW_TCB1  206	//TCB1-TCB1
#define	             RW_TCB2  207	//TCB2-TCB2
#define	             RW_TCB3  208	//TCB3-TCB3
#define	             RW_TCB4  209	//TCB4-TCB4
#define	             RW_TCB5  210	//TCB5-TCB5
#define	             RW_TCB6  211	//TCB6-TCB6
#define	             RW_TCB7  212	//TCB7-TCB7
#define	             RW_TCB8  213	//TCB8-TCB8
#define	             RW_TSW1  214	//TSW1-TSW1
#define	             RW_TSW2  215	//TSW2-TSW2
#define	             RW_TSW3  216	//TSW3-TSW3
#define	             RW_TSW4  217	//TSW4-TSW4
#define	             RW_TSW5  218	//TSW5-TSW5
#define	             RW_TSW6  219	//TSW6-TSW6
#define	             RW_TSW7  220	//TSW7-TSW7
#define	             RW_TSW8  221	//TSW8-TSW8
#define	            RW_T52BF  222	//T52BF-失灵保护时间
#define	              RW_TR1  223	//TR1-跳闸继电器1
#define	              RW_TR2  224	//TR2-跳闸继电器2
#define	              RW_TR3  225	//TR3-跳闸继电器3
#define	              RW_VR1  226	//VR1-虚拟继电器字1
#define	              RW_VR2  227	//VR2-虚拟继电器字2
#define	              RW_VR3  228	//VR3-虚拟继电器字3
#define	              RW_VR4  229	//VR4-虚拟继电器字4
#define	              RW_VR5  230	//VR5-虚拟继电器字5
#define	              RW_VR6  231	//VR6-虚拟继电器字6
#define	              RW_VR7  232	//VR7-虚拟继电器字7
#define	              RW_VR8  233	//VR8-虚拟继电器字8
#define	              RW_VR9  234	//VR9-虚拟继电器字9
#define	             RW_VR10  235	//VR10-虚拟继电器字10
#define	             RW_VR11  236	//VR11-虚拟继电器字11
#define	             RW_VR12  237	//VR12-虚拟继电器字12
#define	             RW_VR13  238	//VR13-虚拟继电器字13
#define	             RW_VR14  239	//VR14-虚拟继电器字14
#define	             RW_VR15  240	//VR15-虚拟继电器字15
#define	             RW_VR16  241	//VR16-虚拟继电器字16
#define	             RW_VR17  242	//VR17-虚拟继电器字17
#define	             RW_VR18  243	//VR18-虚拟继电器字18
#define	             RW_VR19  244	//VR19-虚拟继电器字19
#define	             RW_VR20  245	//VR20-虚拟继电器字20
#define	             RW_VR21  246	//VR21-虚拟继电器字21
#define	             RW_VR22  247	//VR22-虚拟继电器字22
#define	             RW_VR23  248	//VR23-虚拟继电器字23
#define	             RW_VR24  249	//VR24-虚拟继电器字24
#define	             RW_VR25  250	//VR25-虚拟继电器字25
#define	             RW_VR26  251	//VR26-虚拟继电器字26
#define	             RW_VR27  252	//VR27-虚拟继电器字27
#define	             RW_VR28  253	//VR28-虚拟继电器字28
#define	             RW_VR29  254	//VR29-虚拟继电器字29
#define	             RW_VR30  255	//VR30-虚拟继电器字30
#define	             RW_VR31  256	//VR31-虚拟继电器字31
#define	             RW_VR32  257	//VR32-虚拟继电器字32
#define	              RW_IN1  258	//IN1-开入继电器字1
#define	              RW_IN2  259	//IN2-开入继电器字2
#define	              RW_IN3  260	//IN3-开入继电器字3
#define	              RW_IN4  261	//IN4-开入继电器字4
#define	              RW_IN5  262	//IN5-开入继电器字5
#define	              RW_IN6  263	//IN6-开入继电器字6
#define	              RW_IN7  264	//IN7-开入继电器字7
#define	              RW_IN8  265	//IN8-开入继电器字8
#define	              RW_IN9  266	//IN9-开入继电器字9
#define	             RW_IN10  267	//IN10-开入继电器字10
#define	             RW_IN11  268	//IN11-开入继电器字11
#define	             RW_IN12  269	//IN12-开入继电器字12
#define	             RW_IN13  270	//IN13-开入继电器字13
#define	             RW_IN14  271	//IN14-开入继电器字14
#define	             RW_IN15  272	//IN15-开入继电器字15
#define	             RW_IN16  273	//IN16-开入继电器字16
#define	             RW_IN17  274	//IN17-开入继电器字17
#define	             RW_IN18  275	//IN18-开入继电器字18
#define	             RW_IN19  276	//IN19-开入继电器字19
#define	             RW_IN20  277	//IN20-开入继电器字20
#define	             RW_IN21  278	//IN21-开入继电器字21
#define	             RW_IN22  279	//IN22-开入继电器字22
#define	             RW_IN23  280	//IN23-开入继电器字23
#define	             RW_IN24  281	//IN24-开入继电器字24
#define	             RW_IN25  282	//IN25-开入继电器字25
#define	             RW_IN26  283	//IN26-开入继电器字26
#define	             RW_IN27  284	//IN27-开入继电器字27
#define	             RW_IN28  285	//IN28-开入继电器字28
#define	             RW_IN29  286	//IN29-开入继电器字29
#define	             RW_IN30  287	//IN30-开入继电器字30
#define	             RW_IN31  288	//IN31-开入继电器字31
#define	             RW_IN32  289	//IN32-开入继电器字32
#define	             RW_IN33  290	//IN33-开入继电器字33
#define	             RW_IN34  291	//IN34-开入继电器字34
#define	             RW_IN35  292	//IN35-信号复归
#define	             RW_IN36  293	//IN36-重瓦斯2
#define	             RW_IN37  294	//IN37-压力释放2
#define	             RW_IN38  295	//IN38-超温跳闸2
#define	             RW_IN39  296	//IN39-轻瓦斯2
#define	             RW_IN40  297	//IN40-重瓦斯1
#define	             RW_IN41  298	//IN41-压力释放1
#define	             RW_IN42  299	//IN42-超温跳闸1
#define	             RW_IN43  300	//IN43-轻瓦斯1
#define	             RW_IN44  301	//IN44-油位异常1
#define	             RW_IN45  302	//IN45-温度过高1
#define	             RW_IN46  303	//IN46-失电告警1
#define	             RW_IN47  304	//IN47-油位异常2
#define	             RW_IN48  305	//IN48-温度过高2
#define	             RW_IN49  306	//IN49-失电告警2
#define	           RW_INVIN1  307	//INVIN1-开入继电器字50
#define	           RW_INVIN2  308	//INVIN2-开入继电器字51
#define	           RW_INVIN3  309	//INVIN3-开入继电器字52
#define	           RW_INVIN4  310	//INVIN4-开入继电器字53
#define	           RW_INVIN5  311	//INVIN5-开入继电器字54
#define	           RW_INVIN6  312	//INVIN6-开入继电器字55
#define	           RW_INVIN7  313	//INVIN7-开入继电器字56
#define	           RW_INVIN8  314	//INVIN8-开入继电器字57
#define	           RW_INVIN9  315	//INVIN9-开入继电器字58
#define	          RW_INVIN10  316	//INVIN10-开入继电器字59
#define	          RW_INVIN11  317	//INVIN11-开入继电器字60
#define	          RW_INVIN12  318	//INVIN12-开入继电器字61
#define	          RW_INVIN13  319	//INVIN13-开入继电器字62
#define	          RW_INVIN14  320	//INVIN14-开入继电器字63
#define	          RW_INVIN15  321	//INVIN15-开入继电器字64
#define	          RW_INVIN16  322	//INVIN16-开入继电器字65
#define	             RW_OUT1  323	//OUT1-开出1继电器字
#define	             RW_OUT2  324	//OUT2-开出2继电器字
#define	             RW_OUT3  325	//OUT3-开出3继电器字
#define	             RW_OUT4  326	//OUT4-开出4继电器字
#define	             RW_OUT5  327	//OUT5-开出5继电器字
#define	             RW_OUT6  328	//OUT6-开出6继电器字
#define	             RW_OUT7  329	//OUT7-开出7继电器字
#define	             RW_OUT8  330	//OUT8-开出8继电器字
#define	             RW_OUT9  331	//OUT9-开出9继电器字
#define	            RW_OUT10  332	//OUT10-开出10继电器字
#define	            RW_OUT11  333	//OUT11-开出11继电器字
#define	            RW_OUT12  334	//OUT12-开出12继电器字
#define	            RW_OUT13  335	//OUT13-开出13继电器字
#define	            RW_OUT14  336	//OUT14-开出14继电器字
#define	            RW_OUT15  337	//OUT15-开出15继电器字
#define	            RW_OUT16  338	//OUT16-开出16继电器字
#define	            RW_OUT17  339	//OUT17-开出17继电器字
#define	            RW_OUT18  340	//OUT18-开出18继电器字
#define	            RW_OUT19  341	//OUT19-开出19继电器字
#define	            RW_OUT20  342	//OUT20-开出20继电器字
#define	            RW_OUT21  343	//OUT21-开出21继电器字
#define	            RW_SL87U  344	//SL87U-差流速断软压板继电器字
#define	            RW_SL87R  345	//SL87R-比率差动软压板继电器字
#define	          RW_SLMAINT  346	//SLMAINT-检修压板
#define	           RW_SLBAK1  347	//SLBAK1-备用压板1
#define	           RW_SLBAK2  348	//SLBAK2-备用压板2
#define	           RW_F87U_A  349	//F87U_A-A相差动速断配置继电器字
#define	           RW_F87U_B  350	//F87U_B-B相差流速断配置继电器字
#define	           RW_F87U_C  351	//F87U_C-C相差流速断配置继电器字
#define	           RW_F87R_A  352	//F87R_A-A相比率差动配置继电器字
#define	           RW_F87R_B  353	//F87R_B-B相比率差动配置继电器字
#define	           RW_F87R_C  354	//F87R_C-C相比率差动配置继电器字
#define	         RW_F68IDH2M  355	//F68IDH2M-差流二次谐波最大相闭锁配置继电器字
#define	         RW_F68IDH2C  356	//F68IDH2C-差流二次谐波综合相闭锁配置继电器字
#define	         RW_F68IDH2I  357	//F68IDH2I-差流二次谐波分相闭锁配置继电器字
#define	         RW_F68IRH2I  358	//F68IRH2I-制动二次谐波闭锁配置继电器字
#define	         RW_F68IDH5I  359	//F68IDH5I-差流五次谐波闭锁配置继电器字
#define	         RW_F68IDMUL  360	//F68IDMUL-差流综合谐波闭锁
#define	            RW_F52BF  361	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  362	//FBAK1-备用配置1
#define	            RW_FBAK2  363	//FBAK2-备用配置2
#define	              RW_PW1  364	//PW1-脉冲继电器1
#define	              RW_PW2  365	//PW2-脉冲继电器2
#define	              RW_PW3  366	//PW3-脉冲继电器3
#define	              RW_PW4  367	//PW4-脉冲继电器4
#define	              RW_PW5  368	//PW5-脉冲继电器5
#define	              RW_PW6  369	//PW6-脉冲继电器6
#define	              RW_PW7  370	//PW7-脉冲继电器7
#define	              RW_PW8  371	//PW8-脉冲继电器8
#define	              RW_PW9  372	//PW9-脉冲继电器9
#define	             RW_PW10  373	//PW10-脉冲继电器10
#define	             RW_PW11  374	//PW11-脉冲继电器11
#define	             RW_PW12  375	//PW12-脉冲继电器12
#define	             RW_PW13  376	//PW13-脉冲继电器13
#define	             RW_PW14  377	//PW14-脉冲继电器14
#define	             RW_PW15  378	//PW15-脉冲继电器15
#define	             RW_PW16  379	//PW16-脉冲继电器16
#define	              RW_TO1  380	//TO1-比率差动动作
#define	              RW_TO2  381	//TO2-差动速断动作
#define	              RW_TO3  382	//TO3-保护动作
#define	              RW_TO4  383	//TO4-差动信号复归
#define	              RW_TO5  384	//TO5-失灵保护出口
#define	              RW_TO6  385	//TO6-本体1复归
#define	              RW_TO7  386	//TO7-本体1出口1
#define	              RW_TO8  387	//TO8-本体1出口2
#define	              RW_TO9  388	//TO9-本体1出口3
#define	             RW_TO10  389	//TO10-本体1出口4
#define	             RW_TO11  390	//TO11-本体2复归
#define	             RW_TO12  391	//TO12-本体2出口1
#define	             RW_TO13  392	//TO13-本体2出口2
#define	             RW_TO14  393	//TO14-本体2出口3
#define	             RW_TO15  394	//TO15-本体2出口4
#define	             RW_SIG1  395	//SIG1-信号继电器1
#define	             RW_SIG2  396	//SIG2-信号继电器2
#define	             RW_SIG3  397	//SIG3-信号继电器3
#define	             RW_SIG4  398	//SIG4-信号继电器4
#define	             RW_SIG5  399	//SIG5-信号继电器5
#define	             RW_SIG6  400	//SIG6-信号继电器6
#define	             RW_SIG7  401	//SIG7-信号继电器7
#define	             RW_SIG8  402	//SIG8-信号继电器8
#define	             RW_SIG9  403	//SIG9-信号继电器9
#define	            RW_SIG10  404	//SIG10-信号继电器10
#define	            RW_SIG11  405	//SIG11-信号继电器11
#define	            RW_SIG12  406	//SIG12-信号继电器12
#define	            RW_SIG13  407	//SIG13-信号继电器13
#define	            RW_SIG14  408	//SIG14-信号继电器14
#define	            RW_SIG15  409	//SIG15-信号继电器15
#define	            RW_SIG16  410	//SIG16-信号继电器16
#define	            RW_LED1G  411	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  412	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  413	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  414	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  415	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  416	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  417	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  418	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  419	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  420	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  421	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  422	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  423	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  424	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  425	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  426	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  427	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  428	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  429	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  430	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  431	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  432	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  433	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  434	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  435	//VIN1-虚拟开入继电器字1
#define	             RW_VIN2  436	//VIN2-虚拟开入继电器字2
#define	             RW_VIN3  437	//VIN3-虚拟开入继电器字3
#define	             RW_VIN4  438	//VIN4-虚拟开入继电器字4
#define	             RW_VIN5  439	//VIN5-虚拟开入继电器字5
#define	             RW_VIN6  440	//VIN6-虚拟开入继电器字6
#define	             RW_VIN7  441	//VIN7-虚拟开入继电器字7
#define	             RW_VIN8  442	//VIN8-虚拟开入继电器字8
#define	             RW_VIN9  443	//VIN9-虚拟开入继电器字9
#define	            RW_VIN10  444	//VIN10-虚拟开入继电器字10
#define	            RW_VIN11  445	//VIN11-虚拟开入继电器字11
#define	            RW_VIN12  446	//VIN12-虚拟开入继电器字12
#define	            RW_VIN13  447	//VIN13-虚拟开入继电器字13
#define	            RW_VIN14  448	//VIN14-虚拟开入继电器字14
#define	            RW_VIN15  449	//VIN15-虚拟开入继电器字15
#define	            RW_VIN16  450	//VIN16-虚拟开入继电器字16
#define	             RW_RCP1  451	//RCP1-遥控预置继电器1
#define	             RW_RCP2  452	//RCP2-遥控预置继电器2
#define	             RW_RCP3  453	//RCP3-遥控预置继电器3
#define	             RW_RCP4  454	//RCP4-遥控预置继电器4
#define	             RW_RCP5  455	//RCP5-遥控预置继电器5
#define	             RW_RCP6  456	//RCP6-遥控预置继电器6
#define	             RW_RCP7  457	//RCP7-遥控预置继电器7
#define	             RW_RCP8  458	//RCP8-遥控预置继电器8
#define	             RW_RCP9  459	//RCP9-遥控预置继电器9
#define	            RW_RCP10  460	//RCP10-遥控预置继电器10
#define	            RW_RCP11  461	//RCP11-遥控预置继电器11
#define	            RW_RCP12  462	//RCP12-遥控预置继电器12
#define	            RW_RCP13  463	//RCP13-遥控预置继电器13
#define	            RW_RCP14  464	//RCP14-遥控预置继电器14
#define	            RW_RCP15  465	//RCP15-遥控预置继电器15
#define	            RW_RCP16  466	//RCP16-遥控预置继电器16
#define	            RW_RCP17  467	//RCP17-遥控预置继电器17
#define	            RW_RCP18  468	//RCP18-遥控预置继电器18
#define	            RW_RCP19  469	//RCP19-遥控预置继电器19
#define	            RW_RCP20  470	//RCP20-遥控预置继电器20
#define	            RW_RCP21  471	//RCP21-遥控预置继电器21
#define	            RW_RCP22  472	//RCP22-遥控预置继电器22
#define	            RW_RCP23  473	//RCP23-遥控预置继电器23
#define	            RW_RCP24  474	//RCP24-遥控预置继电器24
#define	            RW_RCP25  475	//RCP25-遥控预置继电器25
#define	            RW_RCP26  476	//RCP26-遥控预置继电器26
#define	            RW_RCP27  477	//RCP27-遥控预置继电器27
#define	            RW_RCP28  478	//RCP28-遥控预置继电器28
#define	            RW_RCP29  479	//RCP29-遥控预置继电器29
#define	            RW_RCP30  480	//RCP30-遥控预置继电器30
#define	             RW_RCE1  481	//RCE1-遥控执行继电器1
#define	             RW_RCE2  482	//RCE2-遥控执行继电器2
#define	             RW_RCE3  483	//RCE3-遥控执行继电器3
#define	             RW_RCE4  484	//RCE4-遥控执行继电器4
#define	             RW_RCE5  485	//RCE5-遥控执行继电器5
#define	             RW_RCE6  486	//RCE6-遥控执行继电器6
#define	             RW_RCE7  487	//RCE7-遥控执行继电器7
#define	             RW_RCE8  488	//RCE8-遥控执行继电器8
#define	             RW_RCE9  489	//RCE9-遥控执行继电器9
#define	            RW_RCE10  490	//RCE10-遥控执行继电器10
#define	            RW_RCE11  491	//RCE11-遥控执行继电器11
#define	            RW_RCE12  492	//RCE12-遥控执行继电器12
#define	            RW_RCE13  493	//RCE13-遥控执行继电器13
#define	            RW_RCE14  494	//RCE14-遥控执行继电器14
#define	            RW_RCE15  495	//RCE15-遥控执行继电器15
#define	            RW_RCE16  496	//RCE16-遥控执行继电器16
#define	            RW_RCE17  497	//RCE17-遥控执行继电器17
#define	            RW_RCE18  498	//RCE18-遥控执行继电器18
#define	            RW_RCE19  499	//RCE19-遥控执行继电器19
#define	            RW_RCE20  500	//RCE20-遥控执行继电器20
#define	            RW_RCE21  501	//RCE21-遥控执行继电器21
#define	            RW_RCE22  502	//RCE22-遥控执行继电器22
#define	            RW_RCE23  503	//RCE23-遥控执行继电器23
#define	            RW_RCE24  504	//RCE24-遥控执行继电器24
#define	            RW_RCE25  505	//RCE25-遥控执行继电器25
#define	            RW_RCE26  506	//RCE26-遥控执行继电器26
#define	            RW_RCE27  507	//RCE27-遥控执行继电器27
#define	            RW_RCE28  508	//RCE28-遥控执行继电器28
#define	            RW_RCE29  509	//RCE29-遥控执行继电器29
#define	            RW_RCE30  510	//RCE30-遥控执行继电器30
#define	             RW_RCV1  511	//RCV1-遥控反校继电器1
#define	             RW_RCV2  512	//RCV2-遥控反校继电器2
#define	             RW_RCV3  513	//RCV3-遥控反校继电器3
#define	             RW_RCV4  514	//RCV4-遥控反校继电器4
#define	             RW_RCV5  515	//RCV5-遥控反校继电器5
#define	             RW_RCV6  516	//RCV6-遥控反校继电器6
#define	             RW_RCV7  517	//RCV7-遥控反校继电器7
#define	             RW_RCV8  518	//RCV8-遥控反校继电器8
#define	             RW_RCV9  519	//RCV9-遥控反校继电器9
#define	            RW_RCV10  520	//RCV10-遥控反校继电器10
#define	            RW_RCV11  521	//RCV11-遥控反校继电器11
#define	            RW_RCV12  522	//RCV12-遥控反校继电器12
#define	            RW_RCV13  523	//RCV13-遥控反校继电器13
#define	            RW_RCV14  524	//RCV14-遥控反校继电器14
#define	            RW_RCV15  525	//RCV15-遥控反校继电器15
#define	            RW_RCV16  526	//RCV16-遥控反校继电器16
#define	            RW_RCV17  527	//RCV17-遥控反校继电器17
#define	            RW_RCV18  528	//RCV18-遥控反校继电器18
#define	            RW_RCV19  529	//RCV19-遥控反校继电器19
#define	            RW_RCV20  530	//RCV20-遥控反校继电器20
#define	            RW_RCV21  531	//RCV21-遥控反校继电器21
#define	            RW_RCV22  532	//RCV22-遥控反校继电器22
#define	            RW_RCV23  533	//RCV23-遥控反校继电器23
#define	            RW_RCV24  534	//RCV24-遥控反校继电器24
#define	            RW_RCV25  535	//RCV25-遥控反校继电器25
#define	            RW_RCV26  536	//RCV26-遥控反校继电器26
#define	            RW_RCV27  537	//RCV27-遥控反校继电器27
#define	            RW_RCV28  538	//RCV28-遥控反校继电器28
#define	            RW_RCV29  539	//RCV29-遥控反校继电器29
#define	            RW_RCV30  540	//RCV30-遥控反校继电器30
#define	              RW_GE1  541	//GE1-大于等于继电器1
#define	              RW_GE2  542	//GE2-大于等于继电器2
#define	              RW_GE3  543	//GE3-大于等于继电器3
#define	              RW_GE4  544	//GE4-大于等于继电器4
#define	              RW_GE5  545	//GE5-大于等于继电器5
#define	              RW_GE6  546	//GE6-大于等于继电器6
#define	              RW_GE7  547	//GE7-大于等于继电器7
#define	              RW_GE8  548	//GE8-大于等于继电器8
#define	           RW_CONST1  549	//CONST1-常数时间继电器1
#define	           RW_CONST2  550	//CONST2-常数时间继电器2
#define	           RW_CONST3  551	//CONST3-常数时间继电器3
#define	           RW_CONST4  552	//CONST4-常数时间继电器4
#define	           RW_CONST5  553	//CONST5-常数时间继电器5
#define	           RW_CONST6  554	//CONST6-常数时间继电器6
#define	           RW_CONST7  555	//CONST7-常数时间继电器7
#define	           RW_CONST8  556	//CONST8-常数时间继电器8
#define	              RW_TX1  557	//TX1-镜像发送继电器字1
#define	              RW_TX2  558	//TX2-镜像发送继电器字2
#define	              RW_TX3  559	//TX3-镜像发送继电器字3
#define	              RW_TX4  560	//TX4-镜像发送继电器字4
#define	              RW_TX5  561	//TX5-镜像发送继电器字5
#define	              RW_TX6  562	//TX6-镜像发送继电器字6
#define	              RW_TX7  563	//TX7-镜像发送继电器字7
#define	              RW_TX8  564	//TX8-镜像发送继电器字8
#define	              RW_TX9  565	//TX9-镜像发送继电器字9
#define	             RW_TX10  566	//TX10-镜像发送继电器字10
#define	             RW_TX11  567	//TX11-镜像发送继电器字11
#define	             RW_TX12  568	//TX12-镜像发送继电器字12
#define	             RW_TX13  569	//TX13-镜像发送继电器字13
#define	             RW_TX14  570	//TX14-镜像发送继电器字14
#define	             RW_TX15  571	//TX15-镜像发送继电器字15
#define	             RW_TX16  572	//TX16-镜像发送继电器字16
#define	              RW_RX1  573	//RX1-镜像接收继电器字1
#define	              RW_RX2  574	//RX2-镜像接收继电器字2
#define	              RW_RX3  575	//RX3-镜像接收继电器字3
#define	              RW_RX4  576	//RX4-镜像接收继电器字4
#define	              RW_RX5  577	//RX5-镜像接收继电器字5
#define	              RW_RX6  578	//RX6-镜像接收继电器字6
#define	              RW_RX7  579	//RX7-镜像接收继电器字7
#define	              RW_RX8  580	//RX8-镜像接收继电器字8
#define	              RW_RX9  581	//RX9-镜像接收继电器字9
#define	             RW_RX10  582	//RX10-镜像接收继电器字10
#define	             RW_RX11  583	//RX11-镜像接收继电器字11
#define	             RW_RX12  584	//RX12-镜像接收继电器字12
#define	             RW_RX13  585	//RX13-镜像接收继电器字13
#define	             RW_RX14  586	//RX14-镜像接收继电器字14
#define	             RW_RX15  587	//RX15-镜像接收继电器字15
#define	             RW_RX16  588	//RX16-镜像接收继电器字16
#define	             RW_CNT1  589	//CNT1-计数继电器字1
#define	             RW_CNT2  590	//CNT2-计数继电器字2
#define	             RW_CNT3  591	//CNT3-计数继电器字3
#define	             RW_CNT4  592	//CNT4-计数继电器字4
#define	             RW_CNT5  593	//CNT5-计数继电器字5
#define	             RW_CNT6  594	//CNT6-计数继电器字6
#define	             RW_CNT7  595	//CNT7-计数继电器字7
#define	             RW_CNT8  596	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			597		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			69	

//中间继电器字
#define	MID_RELAY_WORD_START			71	
#define	MID_RELAY_WORD_NUM			84	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			155	
#define	KEEP_RELAY_WORD_NUM			35	

//时间继电器字
#define	TIME_RELAY_WORD_START			190	
#define	TIME_RELAY_WORD_NUM			33	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			223	
#define	TRIP_RELAY_WORD_NUM			3	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			226	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			258	
#define	INPUT_RELAY_WORD_NUM			65	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			323	
#define	OUTPUT_RELAY_WORD_NUM			21	

//压板继电器
#define	SW_RELAY_WORD_START			344	
#define	SW_RELAY_WORD_NUM			5	

//配置继电器
#define	CFG_RELAY_WORD_START			349	
#define	CFG_RELAY_WORD_NUM			15	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			364	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			380	
#define	CTRLTEST_RELAY_WORD_NUM			15	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			395	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			411	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			435	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			451	
#define	RCP_RELAY_WORD_NUM			30	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			481	
#define	RCE_RELAY_WORD_NUM			30	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			511	
#define	RCV_RELAY_WORD_NUM			30	

//大于等于继电器
#define	GE_RELAY_WORD_START			541	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			549	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			557	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			573	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			589	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			589	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			589	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			589	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			589	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			589	
#define	CNT_RELAY_WORD_NUM			8	


#endif