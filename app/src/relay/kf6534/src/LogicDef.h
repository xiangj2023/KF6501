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
#define	        RW_PI60I_AT1    2	//PI60I_AT1-1AT差流入段继电器字
#define	        RW_TR60I_AT1    3	//TR60I_AT1-1AT差流动作继电器字
#define	        RW_PI50T_AT1    4	//PI50T_AT1-1ATT线过流入段继电器字
#define	        RW_TR50T_AT1    5	//TR50T_AT1-1ATT线过流动作继电器字
#define	        RW_PI50F_AT1    6	//PI50F_AT1-1ATF线过流入段继电器字
#define	        RW_TR50F_AT1    7	//TR50F_AT1-1ATF线过流动作继电器字
#define	       RW_PI50CS_AT1    8	//PI50CS_AT1-1AT碰壳过流入段继电器字
#define	       RW_TR50CS_AT1    9	//TR50CS_AT1-1AT碰壳过流动作继电器字
#define	        RW_PI60I_AT2   10	//PI60I_AT2-2AT差流入段继电器字
#define	        RW_TR60I_AT2   11	//TR60I_AT2-2AT差流动作继电器字
#define	        RW_PI50T_AT2   12	//PI50T_AT2-2ATT线过流入段继电器字
#define	        RW_TR50T_AT2   13	//TR50T_AT2-2ATT线过流动作继电器字
#define	        RW_PI50F_AT2   14	//PI50F_AT2-2ATF线过流入段继电器字
#define	        RW_TR50F_AT2   15	//TR50F_AT2-2ATF线过流动作继电器字
#define	       RW_PI50CS_AT2   16	//PI50CS_AT2-2AT碰壳过流入段继电器字
#define	       RW_TR50CS_AT2   17	//TR50CS_AT2-2AT碰壳过流动作继电器字
#define	          RW_PI74PT1   18	//PI74PT1-PT1检查入段继电器字
#define	          RW_AR74PT1   19	//AR74PT1-PT1检查告警继电器字
#define	          RW_PI74PT2   20	//PI74PT2-PT2检查入段继电器字
#define	          RW_AR74PT2   21	//AR74PT2-PT2检查告警继电器字
#define	          RW_PI74PT3   22	//PI74PT3-PT3检查入段继电器字
#define	          RW_AR74PT3   23	//AR74PT3-PT3检查告警继电器字
#define	          RW_PI74PT4   24	//PI74PT4-PT4检查入段继电器字
#define	          RW_AR74PT4   25	//AR74PT4-PT4检查告警继电器字
#define	             RW_59U1   26	//59U1-U1有压
#define	             RW_27U1   27	//27U1-U1无压
#define	             RW_59U2   28	//59U2-U2有压
#define	             RW_27U2   29	//27U2-U2无压
#define	             RW_59U3   30	//59U3-U3有压
#define	             RW_27U3   31	//27U3-U3无压
#define	             RW_59U4   32	//59U4-U4有压
#define	             RW_27U4   33	//27U4-U4无压
#define	             RW_59U5   34	//59U5-U5有压
#define	             RW_27U5   35	//27U5-U5无压
#define	             RW_59U6   36	//59U6-U6有压
#define	             RW_27U6   37	//27U6-U6无压
#define	             RW_59U7   38	//59U7-U7有压
#define	             RW_27U7   39	//27U7-U7无压
#define	             RW_59U8   40	//59U8-U8有压
#define	             RW_27U8   41	//27U8-U8无压
#define	             RW_50I1   42	//50I1-I1有流
#define	             RW_37I1   43	//37I1-I1无流
#define	             RW_50I2   44	//50I2-I2有流
#define	             RW_37I2   45	//37I2-I2无流
#define	             RW_50I3   46	//50I3-I3有流
#define	             RW_37I3   47	//37I3-I3无流
#define	             RW_50I4   48	//50I4-I4有流
#define	             RW_37I4   49	//37I4-I4无流
#define	             RW_50I5   50	//50I5-I5有流
#define	             RW_37I5   51	//37I5-I5无流
#define	             RW_50I6   52	//50I6-I6有流
#define	             RW_37I6   53	//37I6-I6无流
#define	             RW_50I7   54	//50I7-I7有流
#define	             RW_37I7   55	//37I7-I7无流
#define	             RW_50I8   56	//50I8-I8有流
#define	             RW_37I8   57	//37I8-I8无流
#define	       RW_LPC60I_AT1   58	//LPC60I_AT1-1AT差流保护循环继电器字
#define	       RW_LPC50T_AT1   59	//LPC50T_AT1-1ATT线过流循环继电器字
#define	       RW_LPC50F_AT1   60	//LPC50F_AT1-1ATF线过流循环继电器字
#define	      RW_LPC50CS_AT1   61	//LPC50CS_AT1-1AT碰壳过流循环继电器字
#define	       RW_LPC60I_AT2   62	//LPC60I_AT2-2AT差流保护循环继电器字
#define	       RW_LPC50T_AT2   63	//LPC50T_AT2-2ATT线过流循环继电器字
#define	       RW_LPC50F_AT2   64	//LPC50F_AT2-2ATF线过流循环继电器字
#define	      RW_LPC50CS_AT2   65	//LPC50CS_AT2-2AT碰壳过流循环继电器字
#define	       RW_PICKUP_AT1   66	//PICKUP_AT1-AT1保护启动继电器字
#define	       RW_PICKUP_AT2   67	//PICKUP_AT2-AT2保护启动继电器字
#define	          RW_ET11ERR   68	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR   69	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR   70	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR   71	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR   72	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR   73	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR   74	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR   75	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR   76	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR   77	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR   78	//COM1ERR-串口1连接状态继电器字
#define	          RW_CAN1ERR   79	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR   80	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR   81	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR   82	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR   83	//NULL3ERR-空3连接状态继电器字
#define	        RW_MAINTLINK   84	//MAINTLINK-维护状态继电器字
#define	              RW_REC   85	//REC-录波继电器字
#define	               RW_M1   86	//M1-中间继电器1
#define	               RW_M2   87	//M2-中间继电器2
#define	               RW_M3   88	//M3-中间继电器3
#define	               RW_M4   89	//M4-中间继电器4
#define	               RW_M5   90	//M5-中间继电器5
#define	               RW_M6   91	//M6-中间继电器6
#define	               RW_M7   92	//M7-中间继电器7
#define	               RW_M8   93	//M8-中间继电器8
#define	               RW_M9   94	//M9-中间继电器9
#define	              RW_M10   95	//M10-中间继电器10
#define	              RW_M11   96	//M11-中间继电器11
#define	              RW_M12   97	//M12-中间继电器12
#define	              RW_M13   98	//M13-中间继电器13
#define	              RW_M14   99	//M14-中间继电器14
#define	              RW_M15  100	//M15-中间继电器15
#define	              RW_M16  101	//M16-中间继电器16
#define	              RW_M17  102	//M17-中间继电器17
#define	              RW_M18  103	//M18-中间继电器18
#define	              RW_M19  104	//M19-中间继电器19
#define	              RW_M20  105	//M20-中间继电器20
#define	              RW_M21  106	//M21-中间继电器21
#define	              RW_M22  107	//M22-中间继电器22
#define	              RW_M23  108	//M23-中间继电器23
#define	              RW_M24  109	//M24-中间继电器24
#define	              RW_M25  110	//M25-中间继电器25
#define	              RW_M26  111	//M26-中间继电器26
#define	              RW_M27  112	//M27-中间继电器27
#define	              RW_M28  113	//M28-中间继电器28
#define	              RW_M29  114	//M29-中间继电器29
#define	              RW_M30  115	//M30-中间继电器30
#define	              RW_M31  116	//M31-中间继电器31
#define	              RW_M32  117	//M32-中间继电器32
#define	              RW_M33  118	//M33-中间继电器33
#define	              RW_M34  119	//M34-中间继电器34
#define	              RW_M35  120	//M35-中间继电器35
#define	              RW_M36  121	//M36-中间继电器36
#define	              RW_M37  122	//M37-中间继电器37
#define	              RW_M38  123	//M38-中间继电器38
#define	              RW_M39  124	//M39-中间继电器39
#define	              RW_M40  125	//M40-中间继电器40
#define	              RW_M41  126	//M41-中间继电器41
#define	              RW_M42  127	//M42-中间继电器42
#define	              RW_M43  128	//M43-中间继电器43
#define	              RW_M44  129	//M44-中间继电器44
#define	              RW_M45  130	//M45-中间继电器45
#define	              RW_M46  131	//M46-中间继电器46
#define	              RW_M47  132	//M47-中间继电器47
#define	              RW_M48  133	//M48-中间继电器48
#define	              RW_M49  134	//M49-中间继电器49
#define	              RW_M50  135	//M50-中间继电器50
#define	              RW_M51  136	//M51-中间继电器51
#define	              RW_M52  137	//M52-中间继电器52
#define	              RW_M53  138	//M53-中间继电器53
#define	              RW_M54  139	//M54-中间继电器54
#define	              RW_M55  140	//M55-中间继电器55
#define	              RW_M56  141	//M56-中间继电器56
#define	              RW_M57  142	//M57-中间继电器57
#define	              RW_M58  143	//M58-中间继电器58
#define	              RW_M59  144	//M59-中间继电器59
#define	              RW_M60  145	//M60-中间继电器60
#define	              RW_M61  146	//M61-中间继电器61
#define	              RW_M62  147	//M62-中间继电器62
#define	              RW_M63  148	//M63-中间继电器63
#define	              RW_M64  149	//M64-中间继电器64
#define	              RW_M65  150	//M65-中间继电器65
#define	              RW_M66  151	//M66-中间继电器66
#define	              RW_M67  152	//M67-中间继电器67
#define	              RW_M68  153	//M68-中间继电器68
#define	              RW_M69  154	//M69-中间继电器69
#define	              RW_M70  155	//M70-中间继电器70
#define	              RW_M71  156	//M71-中间继电器71
#define	              RW_M72  157	//M72-中间继电器72
#define	              RW_M73  158	//M73-中间继电器73
#define	              RW_M74  159	//M74-中间继电器74
#define	              RW_M75  160	//M75-中间继电器75
#define	              RW_M76  161	//M76-中间继电器76
#define	              RW_M77  162	//M77-中间继电器77
#define	              RW_M78  163	//M78-中间继电器78
#define	              RW_M79  164	//M79-中间继电器79
#define	              RW_M80  165	//M80-中间继电器80
#define	              RW_M81  166	//M81-中间继电器81
#define	              RW_M82  167	//M82-中间继电器82
#define	              RW_M83  168	//M83-中间继电器83
#define	              RW_M84  169	//M84-中间继电器84
#define	              RW_M85  170	//M85-中间继电器85
#define	              RW_M86  171	//M86-中间继电器86
#define	              RW_M87  172	//M87-中间继电器87
#define	              RW_M88  173	//M88-中间继电器88
#define	              RW_M89  174	//M89-中间继电器89
#define	              RW_M90  175	//M90-中间继电器90
#define	              RW_M91  176	//M91-中间继电器91
#define	              RW_M92  177	//M92-中间继电器92
#define	              RW_M93  178	//M93-中间继电器93
#define	              RW_M94  179	//M94-中间继电器94
#define	              RW_M95  180	//M95-中间继电器95
#define	              RW_M96  181	//M96-中间继电器96
#define	              RW_M97  182	//M97-中间继电器97
#define	              RW_M98  183	//M98-中间继电器98
#define	              RW_M99  184	//M99-中间继电器99
#define	             RW_M100  185	//M100-中间继电器100
#define	             RW_M101  186	//M101-中间继电器101
#define	             RW_M102  187	//M102-中间继电器102
#define	             RW_M103  188	//M103-中间继电器103
#define	             RW_M104  189	//M104-中间继电器104
#define	             RW_M105  190	//M105-中间继电器105
#define	             RW_M106  191	//M106-中间继电器106
#define	             RW_M107  192	//M107-中间继电器107
#define	             RW_M108  193	//M108-中间继电器108
#define	             RW_M109  194	//M109-中间继电器109
#define	             RW_M110  195	//M110-中间继电器110
#define	             RW_M111  196	//M111-中间继电器111
#define	             RW_M112  197	//M112-中间继电器112
#define	             RW_M113  198	//M113-中间继电器113
#define	             RW_M114  199	//M114-中间继电器114
#define	             RW_M115  200	//M115-中间继电器115
#define	             RW_M116  201	//M116-中间继电器116
#define	             RW_M117  202	//M117-中间继电器117
#define	             RW_M118  203	//M118-中间继电器118
#define	             RW_M119  204	//M119-中间继电器119
#define	             RW_M120  205	//M120-中间继电器120
#define	             RW_M121  206	//M121-中间继电器121
#define	             RW_M122  207	//M122-中间继电器122
#define	             RW_M123  208	//M123-中间继电器123
#define	             RW_M124  209	//M124-中间继电器124
#define	             RW_M125  210	//M125-中间继电器125
#define	             RW_M126  211	//M126-中间继电器126
#define	             RW_M127  212	//M127-中间继电器127
#define	             RW_M128  213	//M128-中间继电器128
#define	          RW_INRESET  214	//INRESET-遥信复归
#define	            RW_INBZT  215	//INBZT-遥信投入备自投
#define	      RW_LKPI60I_AT1  216	//LKPI60I_AT1-AT1差流保护入段闭锁继电器字
#define	     RW_LKRST60I_AT1  217	//LKRST60I_AT1-AT1差流保护返回闭锁继电器字
#define	      RW_LKPI50T_AT1  218	//LKPI50T_AT1-AT1T线过流入段闭锁继电器字
#define	     RW_LKRST50T_AT1  219	//LKRST50T_AT1-AT1T线过流返回闭锁继电器字
#define	      RW_LKPI50F_AT1  220	//LKPI50F_AT1-AT1F线过流入段闭锁继电器字
#define	     RW_LKRST50F_AT1  221	//LKRST50F_AT1-AT1F线过流返回闭锁继电器字
#define	     RW_LKPI50CS_AT1  222	//LKPI50CS_AT1-AT1碰壳过流入段闭锁继电器字
#define	    RW_LKRST50CS_AT1  223	//LKRST50CS_AT1-AT1碰壳过流返回闭锁继电器字
#define	      RW_LKPI60I_AT2  224	//LKPI60I_AT2-AT2差流保护入段闭锁继电器字
#define	     RW_LKRST60I_AT2  225	//LKRST60I_AT2-AT2差流保护返回闭锁继电器字
#define	      RW_LKPI50T_AT2  226	//LKPI50T_AT2-AT2T线过流入段闭锁继电器字
#define	     RW_LKRST50T_AT2  227	//LKRST50T_AT2-AT2T线过流返回闭锁继电器字
#define	      RW_LKPI50F_AT2  228	//LKPI50F_AT2-AT2F线过流入段闭锁继电器字
#define	     RW_LKRST50F_AT2  229	//LKRST50F_AT2-AT2F线过流返回闭锁继电器字
#define	     RW_LKPI50CS_AT2  230	//LKPI50CS_AT2-AT2碰壳过流入段闭锁继电器字
#define	    RW_LKRST50CS_AT2  231	//LKRST50CS_AT2-AT2碰壳过流返回闭锁继电器字
#define	             RW_69IN  232	//69IN-当地位操作继电器字
#define	          RW_52A_AT1  233	//52A_AT1-AT1断路器合位
#define	          RW_52B_AT1  234	//52B_AT1-AT1断路器分位
#define	          RW_52A_AT2  235	//52A_AT2-AT2断路器合位
#define	          RW_52B_AT2  236	//52B_AT2-AT2断路器分位
#define	               RW_H1  237	//H1-自保持中间继电器字1
#define	               RW_H2  238	//H2-自保持中间继电器字2
#define	               RW_H3  239	//H3-自保持中间继电器字3
#define	               RW_H4  240	//H4-自保持中间继电器字4
#define	               RW_H5  241	//H5-自保持中间继电器字5
#define	               RW_H6  242	//H6-自保持中间继电器字6
#define	               RW_H7  243	//H7-自保持中间继电器字7
#define	               RW_H8  244	//H8-自保持中间继电器字8
#define	               RW_H9  245	//H9-自保持中间继电器字9
#define	              RW_H10  246	//H10-自保持中间继电器字10
#define	              RW_H11  247	//H11-自保持中间继电器字11
#define	              RW_H12  248	//H12-自保持中间继电器字12
#define	              RW_H13  249	//H13-自保持中间继电器字13
#define	              RW_H14  250	//H14-自保持中间继电器字14
#define	              RW_H15  251	//H15-自保持中间继电器字15
#define	              RW_H16  252	//H16-自保持中间继电器字16
#define	              RW_H17  253	//H17-自保持中间继电器字17
#define	              RW_H18  254	//H18-自保持中间继电器字18
#define	              RW_H19  255	//H19-自保持中间继电器字19
#define	              RW_H20  256	//H20-自保持中间继电器字20
#define	              RW_H21  257	//H21-自保持中间继电器字21
#define	              RW_H22  258	//H22-自保持中间继电器字22
#define	              RW_H23  259	//H23-自保持中间继电器字23
#define	              RW_H24  260	//H24-自保持中间继电器字24
#define	              RW_H25  261	//H25-自保持中间继电器字25
#define	              RW_H26  262	//H26-自保持中间继电器字26
#define	              RW_H27  263	//H27-自保持中间继电器字27
#define	              RW_H28  264	//H28-自保持中间继电器字28
#define	              RW_H29  265	//H29-自保持中间继电器字29
#define	              RW_H30  266	//H30-自保持中间继电器字30
#define	              RW_H31  267	//H31-自保持中间继电器字31
#define	              RW_H32  268	//H32-自保持中间继电器字32
#define	              RW_H33  269	//H33-自保持中间继电器字33
#define	              RW_H34  270	//H34-自保持中间继电器字34
#define	              RW_H35  271	//H35-自保持中间继电器字35
#define	              RW_H36  272	//H36-自保持中间继电器字36
#define	              RW_H37  273	//H37-自保持中间继电器字37
#define	              RW_H38  274	//H38-自保持中间继电器字38
#define	              RW_H39  275	//H39-自保持中间继电器字39
#define	              RW_H40  276	//H40-自保持中间继电器字40
#define	              RW_H41  277	//H41-自保持中间继电器字41
#define	              RW_H42  278	//H42-自保持中间继电器字42
#define	              RW_H43  279	//H43-自保持中间继电器字43
#define	              RW_H44  280	//H44-自保持中间继电器字44
#define	              RW_H45  281	//H45-自保持中间继电器字45
#define	              RW_H46  282	//H46-自保持中间继电器字46
#define	              RW_H47  283	//H47-自保持中间继电器字47
#define	              RW_H48  284	//H48-自保持中间继电器字48
#define	              RW_H49  285	//H49-自保持中间继电器字49
#define	              RW_H50  286	//H50-自保持中间继电器字50
#define	              RW_H51  287	//H51-自保持中间继电器字51
#define	              RW_H52  288	//H52-自保持中间继电器字52
#define	              RW_H53  289	//H53-自保持中间继电器字53
#define	              RW_H54  290	//H54-自保持中间继电器字54
#define	              RW_H55  291	//H55-自保持中间继电器字55
#define	              RW_H56  292	//H56-自保持中间继电器字56
#define	              RW_H57  293	//H57-自保持中间继电器字57
#define	              RW_H58  294	//H58-自保持中间继电器字58
#define	              RW_H59  295	//H59-自保持中间继电器字59
#define	              RW_H60  296	//H60-自保持中间继电器字60
#define	              RW_H61  297	//H61-自保持中间继电器字61
#define	              RW_H62  298	//H62-自保持中间继电器字62
#define	              RW_H63  299	//H63-自保持中间继电器字63
#define	              RW_H64  300	//H64-自保持中间继电器字64
#define	             RW_LOCK  301	//LOCK-闭锁继电器字
#define	             RW_CHR1  302	//CHR1-充电方式一继电器字
#define	             RW_CHR2  303	//CHR2-充电方式二继电器字
#define	             RW_CHR3  304	//CHR3-充电方式三继电器字
#define	             RW_CHR4  305	//CHR4-充电方式四继电器字
#define	             RW_CHR5  306	//CHR5-充电方式五继电器字
#define	             RW_CHR6  307	//CHR6-充电方式六继电器字
#define	              RW_PI1  308	//PI1-备自投启动继电器字1
#define	              RW_PI2  309	//PI2-备自投启动继电器字2
#define	              RW_PI3  310	//PI3-备自投启动继电器字3
#define	              RW_PI4  311	//PI4-备自投启动继电器字4
#define	              RW_PI5  312	//PI5-备自投启动继电器字5
#define	              RW_PI6  313	//PI6-备自投启动继电器字6
#define	              RW_PI7  314	//PI7-备自投启动继电器字7
#define	              RW_PI8  315	//PI8-备自投启动继电器字8
#define	              RW_PI9  316	//PI9-备自投启动继电器字9
#define	             RW_PI10  317	//PI10-备自投启动继电器字10
#define	             RW_PI11  318	//PI11-备自投启动继电器字11
#define	             RW_PI12  319	//PI12-备自投启动继电器字12
#define	            RW_AT1GZ  320	//AT1GZ-AT1故障
#define	            RW_AT2GZ  321	//AT2GZ-AT2故障
#define	         RW_52BF_AT1  322	//52BF_AT1-AT1断路器跳闸失败
#define	         RW_52BF_AT2  323	//52BF_AT2-AT2断路器跳闸失败
#define	               RW_T1  324	//T1-T1
#define	               RW_T2  325	//T2-T2
#define	               RW_T3  326	//T3-T3
#define	               RW_T4  327	//T4-T4
#define	               RW_T5  328	//T5-T5
#define	               RW_T6  329	//T6-T6
#define	               RW_T7  330	//T7-T7
#define	               RW_T8  331	//T8-T8
#define	               RW_T9  332	//T9-T9
#define	              RW_T10  333	//T10-T10
#define	              RW_T11  334	//T11-T11
#define	              RW_T12  335	//T12-T12
#define	              RW_T13  336	//T13-T13
#define	              RW_T14  337	//T14-T14
#define	              RW_T15  338	//T15-T15
#define	              RW_T16  339	//T16-T16
#define	              RW_T17  340	//T17-T17
#define	              RW_T18  341	//T18-T18
#define	              RW_T19  342	//T19-T19
#define	              RW_T20  343	//T20-T20
#define	              RW_T21  344	//T21-T21
#define	              RW_T22  345	//T22-T22
#define	              RW_T23  346	//T23-T23
#define	              RW_T24  347	//T24-T24
#define	              RW_T25  348	//T25-T25
#define	              RW_T26  349	//T26-T26
#define	              RW_T27  350	//T27-T27
#define	              RW_T28  351	//T28-T28
#define	              RW_T29  352	//T29-T29
#define	              RW_T30  353	//T30-T30
#define	              RW_T31  354	//T31-T31
#define	              RW_T32  355	//T32-T32
#define	             RW_TCB1  356	//TCB1-TCB1
#define	             RW_TCB2  357	//TCB2-TCB2
#define	             RW_TCB3  358	//TCB3-TCB3
#define	             RW_TCB4  359	//TCB4-TCB4
#define	             RW_TCB5  360	//TCB5-TCB5
#define	             RW_TCB6  361	//TCB6-TCB6
#define	             RW_TCB7  362	//TCB7-TCB7
#define	             RW_TCB8  363	//TCB8-TCB8
#define	             RW_TCB9  364	//TCB9-TCB9
#define	            RW_TCB10  365	//TCB10-TCB10
#define	            RW_TCB11  366	//TCB11-TCB11
#define	            RW_TCB12  367	//TCB12-TCB2
#define	            RW_TCB13  368	//TCB13-TCB13
#define	            RW_TCB14  369	//TCB14-TCB14
#define	            RW_TCB15  370	//TCB15-TCB15
#define	            RW_TCB16  371	//TCB16-TCB16
#define	            RW_TCB17  372	//TCB17-TCB17
#define	            RW_TCB18  373	//TCB18-TCB18
#define	            RW_TCB19  374	//TCB19-TCB19
#define	            RW_TCB20  375	//TCB20-TCB20
#define	            RW_TCB21  376	//TCB21-TCB21
#define	            RW_TCB22  377	//TCB22-TCB22
#define	            RW_TCB23  378	//TCB23-TCB23
#define	            RW_TCB24  379	//TCB24-TCB24
#define	            RW_TCB25  380	//TCB25-TCB25
#define	            RW_TCB26  381	//TCB26-TCB26
#define	            RW_TCB27  382	//TCB27-TCB27
#define	            RW_TCB28  383	//TCB28-TCB28
#define	            RW_TCB29  384	//TCB29-TCB29
#define	            RW_TCB30  385	//TCB30-TCB30
#define	            RW_TCB31  386	//TCB31-TCB31
#define	            RW_TCB32  387	//TCB32-TCB32
#define	             RW_TSW1  388	//TSW1-TSW1
#define	             RW_TSW2  389	//TSW2-TSW2
#define	             RW_TSW3  390	//TSW3-TSW3
#define	             RW_TSW4  391	//TSW4-TSW4
#define	             RW_TSW5  392	//TSW5-TSW5
#define	             RW_TSW6  393	//TSW6-TSW6
#define	             RW_TSW7  394	//TSW7-TSW7
#define	             RW_TSW8  395	//TSW8-TSW8
#define	             RW_TSW9  396	//TSW9-TSW9
#define	            RW_TSW10  397	//TSW10-TSW10
#define	            RW_TSW11  398	//TSW11-TSW11
#define	            RW_TSW12  399	//TSW12-TSW12
#define	            RW_TSW13  400	//TSW13-TSW13
#define	            RW_TSW14  401	//TSW14-TSW14
#define	            RW_TSW15  402	//TSW15-TSW15
#define	            RW_TSW16  403	//TSW16-TSW16
#define	            RW_TSW17  404	//TSW17-TSW17
#define	            RW_TSW18  405	//TSW18-TSW18
#define	            RW_TSW19  406	//TSW19-TSW19
#define	            RW_TSW20  407	//TSW20-TSW20
#define	            RW_TSW21  408	//TSW21-TSW21
#define	            RW_TSW22  409	//TSW22-TSW22
#define	            RW_TSW23  410	//TSW23-TSW23
#define	            RW_TSW24  411	//TSW24-TSW24
#define	            RW_TSW25  412	//TSW25-TSW25
#define	            RW_TSW26  413	//TSW26-TSW26
#define	            RW_TSW27  414	//TSW27-TSW27
#define	            RW_TSW28  415	//TSW28-TSW28
#define	            RW_TSW39  416	//TSW39-TSW39
#define	            RW_TSW30  417	//TSW30-TSW30
#define	            RW_TSW31  418	//TSW31-TSW31
#define	            RW_TSW32  419	//TSW32-TSW2
#define	           RW_TREBZT  420	//TREBZT-备自投重新投入时间继电器字
#define	            RW_TCHR1  421	//TCHR1-充电方式1时间继电器字
#define	            RW_TCHR2  422	//TCHR2-充电方式2时间继电器字
#define	            RW_TCHR3  423	//TCHR3-充电方式3时间继电器字
#define	            RW_TCHR4  424	//TCHR4-充电方式4时间继电器字
#define	            RW_TCHR5  425	//TCHR5-充电方式5时间继电器字
#define	            RW_TCHR6  426	//TCHR6-充电方式6时间继电器字
#define	        RW_T52BF_AT1  427	//T52BF_AT1-断路器跳闸失败时间继电器字
#define	        RW_T52BF_AT2  428	//T52BF_AT2-断路器跳闸失败时间继电器字
#define	              RW_TR1  429	//TR1-跳闸继电器1
#define	              RW_TR2  430	//TR2-跳闸继电器2
#define	              RW_TR3  431	//TR3-跳闸继电器3
#define	              RW_TR4  432	//TR4-跳闸继电器4
#define	              RW_VR1  433	//VR1-调试继电器
#define	              RW_VR2  434	//VR2-虚拟继电器2
#define	              RW_VR3  435	//VR3-虚拟继电器3
#define	              RW_VR4  436	//VR4-虚拟继电器4
#define	              RW_VR5  437	//VR5-虚拟继电器5
#define	              RW_VR6  438	//VR6-虚拟继电器6
#define	              RW_VR7  439	//VR7-虚拟继电器7
#define	              RW_VR8  440	//VR8-虚拟继电器8
#define	              RW_VR9  441	//VR9-虚拟继电器9
#define	             RW_VR10  442	//VR10-虚拟继电器10
#define	             RW_VR11  443	//VR11-虚拟继电器11
#define	             RW_VR12  444	//VR12-虚拟继电器12
#define	             RW_VR13  445	//VR13-虚拟继电器13
#define	             RW_VR14  446	//VR14-虚拟继电器14
#define	             RW_VR15  447	//VR15-虚拟继电器15
#define	             RW_VR16  448	//VR16-虚拟继电器16
#define	             RW_VR17  449	//VR17-虚拟继电器17
#define	             RW_VR18  450	//VR18-虚拟继电器18
#define	             RW_VR19  451	//VR19-虚拟继电器19
#define	             RW_VR20  452	//VR20-虚拟继电器20
#define	             RW_VR21  453	//VR21-虚拟继电器21
#define	             RW_VR22  454	//VR22-虚拟继电器22
#define	             RW_VR23  455	//VR23-虚拟继电器23
#define	             RW_VR24  456	//VR24-虚拟继电器24
#define	             RW_VR25  457	//VR25-虚拟继电器25
#define	             RW_VR26  458	//VR26-虚拟继电器26
#define	             RW_VR27  459	//VR27-虚拟继电器27
#define	             RW_VR28  460	//VR28-虚拟继电器28
#define	             RW_VR29  461	//VR29-虚拟继电器29
#define	             RW_VR30  462	//VR30-虚拟继电器30
#define	             RW_VR31  463	//VR31-虚拟继电器31
#define	             RW_VR32  464	//VR32-虚拟继电器32
#define	              RW_IN1  465	//IN1-开入继电器1
#define	              RW_IN2  466	//IN2-开入继电器2
#define	              RW_IN3  467	//IN3-开入继电器3
#define	              RW_IN4  468	//IN4-开入继电器4
#define	              RW_IN5  469	//IN5-开入继电器5
#define	              RW_IN6  470	//IN6-开入继电器6
#define	              RW_IN7  471	//IN7-开入继电器7
#define	              RW_IN8  472	//IN8-开入继电器8
#define	              RW_IN9  473	//IN9-开入继电器9
#define	             RW_IN10  474	//IN10-开入继电器10
#define	             RW_IN11  475	//IN11-开入继电器11
#define	             RW_IN12  476	//IN12-开入继电器12
#define	             RW_IN13  477	//IN13-开入继电器13
#define	             RW_IN14  478	//IN14-开入继电器14
#define	             RW_IN15  479	//IN15-开入继电器15
#define	             RW_IN16  480	//IN16-开入继电器16
#define	             RW_IN17  481	//IN17-开入继电器17
#define	             RW_IN18  482	//IN18-开入继电器18
#define	             RW_IN19  483	//IN19-开入继电器19
#define	             RW_IN20  484	//IN20-开入继电器20
#define	             RW_IN21  485	//IN21-开入继电器21
#define	             RW_IN22  486	//IN22-开入继电器22
#define	             RW_IN23  487	//IN23-1AT重瓦斯
#define	             RW_IN24  488	//IN24-1AT压力释放
#define	             RW_IN25  489	//IN25-1AT超温跳闸
#define	             RW_IN26  490	//IN26-1AT轻瓦斯
#define	             RW_IN27  491	//IN27-1AT油温告警
#define	             RW_IN28  492	//IN28-1AT温度告警
#define	             RW_IN29  493	//IN29-2AT重瓦斯
#define	             RW_IN30  494	//IN30-2AT压力释放
#define	             RW_IN31  495	//IN31-2AT超温跳闸
#define	             RW_IN32  496	//IN32-2AT轻瓦斯
#define	             RW_IN33  497	//IN33-2AT油温告警
#define	             RW_IN34  498	//IN34-2AT温度告警
#define	             RW_IN35  499	//IN35-信号复归
#define	             RW_IN36  500	//IN36-2AT断路器跳位
#define	             RW_IN37  501	//IN37-2AT断路器合位
#define	             RW_IN38  502	//IN38-2AT断路器预置
#define	             RW_IN39  503	//IN39-2AT遥控1预置
#define	             RW_IN40  504	//IN40-2AT遥控2预置
#define	             RW_IN41  505	//IN41-1AT断路器跳位
#define	             RW_IN42  506	//IN42-1AT断路器合位
#define	             RW_IN43  507	//IN43-1AT断路器预置
#define	             RW_IN44  508	//IN44-1AT遥控1预置
#define	             RW_IN45  509	//IN45-1AT遥控2预置
#define	             RW_IN46  510	//IN46-遥控1预置
#define	           RW_INVIN1  511	//INVIN1-开入继电器47
#define	           RW_INVIN2  512	//INVIN2-开入继电器48
#define	           RW_INVIN3  513	//INVIN3-开入继电器48
#define	           RW_INVIN4  514	//INVIN4-开入继电器50
#define	           RW_INVIN5  515	//INVIN5-开入继电器51
#define	           RW_INVIN6  516	//INVIN6-开入继电器52
#define	           RW_INVIN7  517	//INVIN7-开入继电器53
#define	           RW_INVIN8  518	//INVIN8-开入继电器54
#define	           RW_INVIN9  519	//INVIN9-开入继电器55
#define	          RW_INVIN10  520	//INVIN10-开入继电器56
#define	          RW_INVIN11  521	//INVIN11-开入继电器57
#define	          RW_INVIN12  522	//INVIN12-开入继电器58
#define	          RW_INVIN13  523	//INVIN13-开入继电器59
#define	          RW_INVIN14  524	//INVIN14-开入继电器60
#define	          RW_INVIN15  525	//INVIN15-开入继电器61
#define	          RW_INVIN16  526	//INVIN16-开入继电器62
#define	             RW_OUT1  527	//OUT1-开出1继电器字
#define	             RW_OUT2  528	//OUT2-开出2继电器字
#define	             RW_OUT3  529	//OUT3-开出3继电器字
#define	             RW_OUT4  530	//OUT4-开出4继电器字
#define	             RW_OUT5  531	//OUT5-开出5继电器字
#define	             RW_OUT6  532	//OUT6-开出6继电器字
#define	             RW_OUT7  533	//OUT7-开出7继电器字
#define	             RW_OUT8  534	//OUT8-开出8继电器字
#define	             RW_OUT9  535	//OUT9-开出9继电器字
#define	            RW_OUT10  536	//OUT10-开出10继电器字
#define	            RW_OUT11  537	//OUT11-开出11继电器字
#define	            RW_OUT12  538	//OUT12-开出12继电器字
#define	            RW_OUT13  539	//OUT13-开出13继电器字
#define	            RW_OUT14  540	//OUT14-开出14继电器字
#define	            RW_OUT15  541	//OUT15-开出15继电器字
#define	            RW_OUT16  542	//OUT16-开出16继电器字
#define	            RW_OUT17  543	//OUT17-开出17继电器字
#define	            RW_OUT18  544	//OUT18-开出18继电器字
#define	            RW_OUT19  545	//OUT19-开出19继电器字
#define	            RW_OUT20  546	//OUT20-开出20继电器字
#define	            RW_OUT21  547	//OUT21-开出21继电器字
#define	            RW_OUT22  548	//OUT22-开出22继电器字
#define	            RW_OUT23  549	//OUT23-开出23继电器字
#define	            RW_OUT24  550	//OUT24-开出24继电器字
#define	            RW_OUT25  551	//OUT25-开出25继电器字
#define	            RW_OUT26  552	//OUT26-开出26继电器字
#define	            RW_OUT27  553	//OUT27-开出27继电器字
#define	            RW_OUT28  554	//OUT28-开出28继电器字
#define	            RW_OUT29  555	//OUT29-开出29继电器字
#define	            RW_OUT30  556	//OUT30-开出30继电器字
#define	            RW_OUT31  557	//OUT31-开出31继电器字
#define	            RW_OUT32  558	//OUT32-开出32继电器字
#define	            RW_OUT33  559	//OUT33-开出33继电器字
#define	            RW_OUT34  560	//OUT34-开出34继电器字
#define	            RW_OUT35  561	//OUT35-开出35继电器字
#define	            RW_OUT36  562	//OUT36-开出36继电器字
#define	            RW_OUT37  563	//OUT37-开出37继电器字
#define	            RW_OUT38  564	//OUT38-开出38继电器字
#define	            RW_OUT39  565	//OUT39-开出39继电器字
#define	            RW_OUT40  566	//OUT40-开出40继电器字
#define	            RW_OUT41  567	//OUT41-开出41继电器字
#define	            RW_OUT42  568	//OUT42-开出42继电器字
#define	            RW_OUT43  569	//OUT43-开出43继电器字
#define	            RW_SLBZT  570	//SLBZT-备自投软压板
#define	          RW_SLACAT1  571	//SLACAT1-1AT自动控制
#define	          RW_SLACAT2  572	//SLACAT2-2AT自动控制
#define	        RW_SL60I_AT1  573	//SL60I_AT1-1AT差流保护
#define	        RW_SL50T_AT1  574	//SL50T_AT1-1ATT线过流
#define	        RW_SL50F_AT1  575	//SL50F_AT1-1ATF线过流
#define	       RW_SL50CS_AT1  576	//SL50CS_AT1-1AT碰壳过流
#define	        RW_SL60I_AT2  577	//SL60I_AT2-2AT差流保护
#define	        RW_SL50T_AT2  578	//SL50T_AT2-2ATT线过流
#define	        RW_SL50F_AT2  579	//SL50F_AT2-2ATF线过流
#define	       RW_SL50CS_AT2  580	//SL50CS_AT2-2AT碰壳过流
#define	          RW_SLMAINT  581	//SLMAINT-检修压板
#define	           RW_SLBAK1  582	//SLBAK1-备用压板1
#define	           RW_SLBAK2  583	//SLBAK2-备用压板2
#define	           RW_SLBAK3  584	//SLBAK3-备用压板3
#define	           RW_SLBAK4  585	//SLBAK4-备用压板4
#define	             RW_FBZT  586	//FBZT-备自投功能配置继电器字
#define	           RW_FREBZT  587	//FREBZT-备自投重新投入功能配置继电器字
#define	           RW_FACAT1  588	//FACAT1-1AT自动控制功能配置继电器字
#define	           RW_FACAT2  589	//FACAT2-2AT自动控制功能配置继电器字
#define	         RW_F60I_AT1  590	//F60I_AT1-1AT差流配置继电器字
#define	         RW_F60I_AT2  591	//F60I_AT2-2AT差流配置继电器字
#define	         RW_F50T_AT1  592	//F50T_AT1-1ATT线过流配置继电器字
#define	         RW_F50F_AT1  593	//F50F_AT1-1ATF线过流配置继电器字
#define	        RW_F50CS_AT1  594	//F50CS_AT1-1AT碰壳过流配置继电器字
#define	         RW_F50T_AT2  595	//F50T_AT2-2ATT线过流配置继电器字
#define	         RW_F50F_AT2  596	//F50F_AT2-2ATF线过流配置继电器字
#define	        RW_F50CS_AT2  597	//F50CS_AT2-2AT碰壳过流配置继电器字
#define	           RW_F74PT1  598	//F74PT1-PT1断线检查配置继电器字
#define	           RW_F74PT2  599	//F74PT2-PT2断线检查配置继电器字
#define	           RW_F74PT3  600	//F74PT3-PT3断线检查配置继电器字
#define	           RW_F74PT4  601	//F74PT4-PT4断线检查配置继电器字
#define	            RW_FBAK1  602	//FBAK1-备用配置1
#define	            RW_FBAK2  603	//FBAK2-备用配置2
#define	            RW_FBAK3  604	//FBAK3-备用配置3
#define	            RW_FBAK4  605	//FBAK4-备用配置4
#define	              RW_PW1  606	//PW1-脉冲继电器1
#define	              RW_PW2  607	//PW2-脉冲继电器2
#define	              RW_PW3  608	//PW3-脉冲继电器3
#define	              RW_PW4  609	//PW4-脉冲继电器4
#define	              RW_PW5  610	//PW5-脉冲继电器5
#define	              RW_PW6  611	//PW6-脉冲继电器6
#define	              RW_PW7  612	//PW7-脉冲继电器7
#define	              RW_PW8  613	//PW8-脉冲继电器8
#define	              RW_PW9  614	//PW9-脉冲继电器9
#define	             RW_PW10  615	//PW10-脉冲继电器10
#define	             RW_PW11  616	//PW11-脉冲继电器11
#define	             RW_PW12  617	//PW12-脉冲继电器12
#define	             RW_PW13  618	//PW13-脉冲继电器13
#define	             RW_PW14  619	//PW14-脉冲继电器14
#define	             RW_PW15  620	//PW15-脉冲继电器15
#define	             RW_PW16  621	//PW16-脉冲继电器16
#define	              RW_TO1  622	//TO1-X4板保护跳闸
#define	              RW_TO2  623	//TO2-X4板遥控断路器分
#define	              RW_TO3  624	//TO3-X4板遥控断路器合
#define	              RW_TO4  625	//TO4-X4板遥控1分
#define	              RW_TO5  626	//TO5-X4板遥控1合
#define	              RW_TO6  627	//TO6-X4板遥控2分
#define	              RW_TO7  628	//TO7-X4板遥控2合
#define	              RW_TO8  629	//TO8-X5板保护跳闸
#define	              RW_TO9  630	//TO9-X5板遥控断路器分
#define	             RW_TO10  631	//TO10-X5板遥控断路器合
#define	             RW_TO11  632	//TO11-X5板遥控1分
#define	             RW_TO12  633	//TO12-X5板遥控1合
#define	             RW_TO13  634	//TO13-X5板遥控2分
#define	             RW_TO14  635	//TO14-X5板遥控2合
#define	             RW_TO15  636	//TO15-X6板遥控1分
#define	             RW_TO16  637	//TO16-X6板遥控1合
#define	             RW_TO17  638	//TO17-X6板遥控2分
#define	             RW_TO18  639	//TO18-X6板遥控2合
#define	             RW_TO19  640	//TO19-X6板遥控3分
#define	             RW_TO20  641	//TO20-X6板遥控3合
#define	             RW_TO21  642	//TO21-X6板遥控4分
#define	             RW_TO22  643	//TO22-X6板遥控4合
#define	             RW_TO23  644	//TO23-X6板遥控5分
#define	             RW_TO24  645	//TO24-X6板遥控5合
#define	             RW_TO25  646	//TO25-X6板遥控6分
#define	             RW_TO26  647	//TO26-X6板遥控6合
#define	             RW_TO27  648	//TO27-X6板遥控7分
#define	             RW_TO28  649	//TO28-X6板遥控7合
#define	             RW_SIG1  650	//SIG1-信号继电器1
#define	             RW_SIG2  651	//SIG2-信号继电器2
#define	             RW_SIG3  652	//SIG3-信号继电器3
#define	             RW_SIG4  653	//SIG4-信号继电器4
#define	             RW_SIG5  654	//SIG5-信号继电器5
#define	             RW_SIG6  655	//SIG6-信号继电器6
#define	             RW_SIG7  656	//SIG7-信号继电器7
#define	             RW_SIG8  657	//SIG8-信号继电器8
#define	             RW_SIG9  658	//SIG9-信号继电器9
#define	            RW_SIG10  659	//SIG10-信号继电器10
#define	            RW_SIG11  660	//SIG11-信号继电器11
#define	            RW_SIG12  661	//SIG12-信号继电器12
#define	            RW_SIG13  662	//SIG13-信号继电器13
#define	            RW_SIG14  663	//SIG14-信号继电器14
#define	            RW_SIG15  664	//SIG15-信号继电器15
#define	            RW_SIG16  665	//SIG16-信号继电器16
#define	            RW_LED1G  666	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  667	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  668	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  669	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  670	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  671	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  672	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  673	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  674	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  675	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  676	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  677	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  678	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  679	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  680	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  681	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  682	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  683	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  684	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  685	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  686	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  687	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  688	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  689	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  690	//VIN1-开入继电器47
#define	             RW_VIN2  691	//VIN2-开入继电器48
#define	             RW_VIN3  692	//VIN3-开入继电器48
#define	             RW_VIN4  693	//VIN4-开入继电器50
#define	             RW_VIN5  694	//VIN5-开入继电器51
#define	             RW_VIN6  695	//VIN6-开入继电器52
#define	             RW_VIN7  696	//VIN7-开入继电器53
#define	             RW_VIN8  697	//VIN8-开入继电器54
#define	             RW_VIN9  698	//VIN9-开入继电器55
#define	            RW_VIN10  699	//VIN10-开入继电器56
#define	            RW_VIN11  700	//VIN11-开入继电器57
#define	            RW_VIN12  701	//VIN12-开入继电器58
#define	            RW_VIN13  702	//VIN13-开入继电器59
#define	            RW_VIN14  703	//VIN14-开入继电器60
#define	            RW_VIN15  704	//VIN15-开入继电器61
#define	            RW_VIN16  705	//VIN16-开入继电器62
#define	             RW_RCP1  706	//RCP1-遥控预置继电器1
#define	             RW_RCP2  707	//RCP2-遥控预置继电器2
#define	             RW_RCP3  708	//RCP3-遥控预置继电器3
#define	             RW_RCP4  709	//RCP4-遥控预置继电器4
#define	             RW_RCP5  710	//RCP5-遥控预置继电器5
#define	             RW_RCP6  711	//RCP6-遥控预置继电器6
#define	             RW_RCP7  712	//RCP7-遥控预置继电器7
#define	             RW_RCP8  713	//RCP8-遥控预置继电器8
#define	             RW_RCP9  714	//RCP9-遥控预置继电器9
#define	            RW_RCP10  715	//RCP10-遥控预置继电器10
#define	            RW_RCP11  716	//RCP11-遥控预置继电器11
#define	            RW_RCP12  717	//RCP12-遥控预置继电器12
#define	            RW_RCP13  718	//RCP13-遥控预置继电器13
#define	            RW_RCP14  719	//RCP14-遥控预置继电器14
#define	            RW_RCP15  720	//RCP15-遥控预置继电器15
#define	            RW_RCP16  721	//RCP16-遥控预置继电器16
#define	            RW_RCP17  722	//RCP17-遥控预置继电器17
#define	            RW_RCP18  723	//RCP18-遥控预置继电器18
#define	            RW_RCP19  724	//RCP19-遥控预置继电器19
#define	            RW_RCP20  725	//RCP20-遥控预置继电器20
#define	            RW_RCP21  726	//RCP21-遥控预置继电器21
#define	            RW_RCP22  727	//RCP22-遥控预置继电器22
#define	            RW_RCP23  728	//RCP23-遥控预置继电器23
#define	            RW_RCP24  729	//RCP24-遥控预置继电器24
#define	            RW_RCP25  730	//RCP25-遥控预置继电器25
#define	            RW_RCP26  731	//RCP26-遥控预置继电器26
#define	            RW_RCP27  732	//RCP27-遥控预置继电器27
#define	            RW_RCP28  733	//RCP28-遥控预置继电器28
#define	            RW_RCP29  734	//RCP29-遥控预置继电器29
#define	            RW_RCP30  735	//RCP30-遥控预置继电器30
#define	            RW_RCP31  736	//RCP31-遥控预置继电器31
#define	            RW_RCP32  737	//RCP32-遥控预置继电器32
#define	            RW_RCP33  738	//RCP33-遥控预置继电器33
#define	            RW_RCP34  739	//RCP34-遥控预置继电器34
#define	            RW_RCP35  740	//RCP35-遥控预置继电器35
#define	            RW_RCP36  741	//RCP36-遥控预置继电器36
#define	            RW_RCP37  742	//RCP37-遥控预置继电器37
#define	            RW_RCP38  743	//RCP38-遥控预置继电器38
#define	            RW_RCP39  744	//RCP39-遥控预置继电器39
#define	            RW_RCP40  745	//RCP40-遥控预置继电器40
#define	            RW_RCP41  746	//RCP41-遥控预置继电器41
#define	            RW_RCP42  747	//RCP42-遥控预置继电器42
#define	            RW_RCP43  748	//RCP43-遥控预置继电器43
#define	            RW_RCP44  749	//RCP44-遥控预置继电器44
#define	            RW_RCP45  750	//RCP45-遥控预置继电器45
#define	            RW_RCP46  751	//RCP46-遥控预置继电器46
#define	            RW_RCP47  752	//RCP47-遥控预置继电器47
#define	            RW_RCP48  753	//RCP48-遥控预置继电器48
#define	            RW_RCP49  754	//RCP49-遥控预置继电器49
#define	            RW_RCP50  755	//RCP50-遥控预置继电器50
#define	            RW_RCP51  756	//RCP51-遥控预置继电器51
#define	            RW_RCP52  757	//RCP52-遥控预置继电器52
#define	             RW_RCE1  758	//RCE1-遥控执行继电器1
#define	             RW_RCE2  759	//RCE2-遥控执行继电器2
#define	             RW_RCE3  760	//RCE3-遥控执行继电器3
#define	             RW_RCE4  761	//RCE4-遥控执行继电器4
#define	             RW_RCE5  762	//RCE5-遥控执行继电器5
#define	             RW_RCE6  763	//RCE6-遥控执行继电器6
#define	             RW_RCE7  764	//RCE7-遥控执行继电器7
#define	             RW_RCE8  765	//RCE8-遥控执行继电器8
#define	             RW_RCE9  766	//RCE9-遥控执行继电器9
#define	            RW_RCE10  767	//RCE10-遥控执行继电器10
#define	            RW_RCE11  768	//RCE11-遥控执行继电器11
#define	            RW_RCE12  769	//RCE12-遥控执行继电器12
#define	            RW_RCE13  770	//RCE13-遥控执行继电器13
#define	            RW_RCE14  771	//RCE14-遥控执行继电器14
#define	            RW_RCE15  772	//RCE15-遥控执行继电器15
#define	            RW_RCE16  773	//RCE16-遥控执行继电器16
#define	            RW_RCE17  774	//RCE17-遥控执行继电器17
#define	            RW_RCE18  775	//RCE18-遥控执行继电器18
#define	            RW_RCE19  776	//RCE19-遥控执行继电器19
#define	            RW_RCE20  777	//RCE20-遥控执行继电器20
#define	            RW_RCE21  778	//RCE21-遥控执行继电器21
#define	            RW_RCE22  779	//RCE22-遥控执行继电器22
#define	            RW_RCE23  780	//RCE23-遥控执行继电器23
#define	            RW_RCE24  781	//RCE24-遥控执行继电器24
#define	            RW_RCE25  782	//RCE25-遥控执行继电器25
#define	            RW_RCE26  783	//RCE26-遥控执行继电器26
#define	            RW_RCE27  784	//RCE27-遥控执行继电器27
#define	            RW_RCE28  785	//RCE28-遥控执行继电器28
#define	            RW_RCE29  786	//RCE29-遥控执行继电器29
#define	            RW_RCE30  787	//RCE30-遥控执行继电器30
#define	            RW_RCE31  788	//RCE31-遥控执行继电器31
#define	            RW_RCE32  789	//RCE32-遥控执行继电器32
#define	            RW_RCE33  790	//RCE33-遥控执行继电器33
#define	            RW_RCE34  791	//RCE34-遥控执行继电器34
#define	            RW_RCE35  792	//RCE35-遥控执行继电器35
#define	            RW_RCE36  793	//RCE36-遥控执行继电器36
#define	            RW_RCE37  794	//RCE37-遥控执行继电器37
#define	            RW_RCE38  795	//RCE38-遥控执行继电器38
#define	            RW_RCE39  796	//RCE39-遥控执行继电器39
#define	            RW_RCE40  797	//RCE40-遥控执行继电器40
#define	            RW_RCE41  798	//RCE41-遥控执行继电器41
#define	            RW_RCE42  799	//RCE42-遥控执行继电器42
#define	            RW_RCE43  800	//RCE43-遥控执行继电器43
#define	            RW_RCE44  801	//RCE44-遥控执行继电器44
#define	            RW_RCE45  802	//RCE45-遥控执行继电器45
#define	            RW_RCE46  803	//RCE46-遥控执行继电器46
#define	            RW_RCE47  804	//RCE47-遥控执行继电器47
#define	            RW_RCE48  805	//RCE48-遥控执行继电器48
#define	            RW_RCE49  806	//RCE49-遥控执行继电器49
#define	            RW_RCE50  807	//RCE50-遥控执行继电器50
#define	            RW_RCE51  808	//RCE51-遥控执行继电器51
#define	            RW_RCE52  809	//RCE52-遥控执行继电器52
#define	             RW_RCV1  810	//RCV1-遥控反校继电器1
#define	             RW_RCV2  811	//RCV2-遥控反校继电器2
#define	             RW_RCV3  812	//RCV3-遥控反校继电器3
#define	             RW_RCV4  813	//RCV4-遥控反校继电器4
#define	             RW_RCV5  814	//RCV5-遥控反校继电器5
#define	             RW_RCV6  815	//RCV6-遥控反校继电器6
#define	             RW_RCV7  816	//RCV7-遥控反校继电器7
#define	             RW_RCV8  817	//RCV8-遥控反校继电器8
#define	             RW_RCV9  818	//RCV9-遥控反校继电器9
#define	            RW_RCV10  819	//RCV10-遥控反校继电器10
#define	            RW_RCV11  820	//RCV11-遥控反校继电器11
#define	            RW_RCV12  821	//RCV12-遥控反校继电器12
#define	            RW_RCV13  822	//RCV13-遥控反校继电器13
#define	            RW_RCV14  823	//RCV14-遥控反校继电器14
#define	            RW_RCV15  824	//RCV15-遥控反校继电器15
#define	            RW_RCV16  825	//RCV16-遥控反校继电器16
#define	            RW_RCV17  826	//RCV17-遥控反校继电器17
#define	            RW_RCV18  827	//RCV18-遥控反校继电器18
#define	            RW_RCV19  828	//RCV19-遥控反校继电器19
#define	            RW_RCV20  829	//RCV20-遥控反校继电器20
#define	            RW_RCV21  830	//RCV21-遥控反校继电器21
#define	            RW_RCV22  831	//RCV22-遥控反校继电器22
#define	            RW_RCV23  832	//RCV23-遥控反校继电器23
#define	            RW_RCV24  833	//RCV24-遥控反校继电器24
#define	            RW_RCV25  834	//RCV25-遥控反校继电器25
#define	            RW_RCV26  835	//RCV26-遥控反校继电器26
#define	            RW_RCV27  836	//RCV27-遥控反校继电器27
#define	            RW_RCV28  837	//RCV28-遥控反校继电器28
#define	            RW_RCV29  838	//RCV29-遥控反校继电器29
#define	            RW_RCV30  839	//RCV30-遥控反校继电器30
#define	            RW_RCV31  840	//RCV31-遥控反校继电器31
#define	            RW_RCV32  841	//RCV32-遥控反校继电器32
#define	            RW_RCV33  842	//RCV33-遥控反校继电器33
#define	            RW_RCV34  843	//RCV34-遥控反校继电器34
#define	            RW_RCV35  844	//RCV35-遥控反校继电器35
#define	            RW_RCV36  845	//RCV36-遥控反校继电器36
#define	            RW_RCV37  846	//RCV37-遥控反校继电器37
#define	            RW_RCV38  847	//RCV38-遥控反校继电器38
#define	            RW_RCV39  848	//RCV39-遥控反校继电器39
#define	            RW_RCV40  849	//RCV40-遥控反校继电器40
#define	            RW_RCV41  850	//RCV41-遥控反校继电器41
#define	            RW_RCV42  851	//RCV42-遥控反校继电器42
#define	            RW_RCV43  852	//RCV43-遥控反校继电器43
#define	            RW_RCV44  853	//RCV44-遥控反校继电器44
#define	            RW_RCV45  854	//RCV45-遥控反校继电器45
#define	            RW_RCV46  855	//RCV46-遥控反校继电器46
#define	            RW_RCV47  856	//RCV47-遥控反校继电器47
#define	            RW_RCV48  857	//RCV48-遥控反校继电器48
#define	            RW_RCV49  858	//RCV49-遥控反校继电器49
#define	            RW_RCV50  859	//RCV50-遥控反校继电器50
#define	            RW_RCV51  860	//RCV51-遥控反校继电器51
#define	            RW_RCV52  861	//RCV52-遥控反校继电器52
#define	              RW_GE1  862	//GE1-大于等于继电器1
#define	              RW_GE2  863	//GE2-大于等于继电器2
#define	              RW_GE3  864	//GE3-大于等于继电器3
#define	              RW_GE4  865	//GE4-大于等于继电器4
#define	              RW_GE5  866	//GE5-大于等于继电器5
#define	              RW_GE6  867	//GE6-大于等于继电器6
#define	              RW_GE7  868	//GE7-大于等于继电器7
#define	              RW_GE8  869	//GE8-大于等于继电器8
#define	           RW_CONST1  870	//CONST1-常数时间继电器1
#define	           RW_CONST2  871	//CONST2-常数时间继电器2
#define	           RW_CONST3  872	//CONST3-常数时间继电器3
#define	           RW_CONST4  873	//CONST4-常数时间继电器4
#define	           RW_CONST5  874	//CONST5-常数时间继电器5
#define	           RW_CONST6  875	//CONST6-常数时间继电器6
#define	           RW_CONST7  876	//CONST7-常数时间继电器7
#define	           RW_CONST8  877	//CONST8-常数时间继电器8
#define	              RW_TX1  878	//TX1-发送1继电器字
#define	              RW_TX2  879	//TX2-发送2继电器字
#define	              RW_TX3  880	//TX3-发送3继电器字
#define	              RW_TX4  881	//TX4-发送4继电器字
#define	              RW_TX5  882	//TX5-发送5继电器字
#define	              RW_TX6  883	//TX6-发送6继电器字
#define	              RW_TX7  884	//TX7-发送7继电器字
#define	              RW_TX8  885	//TX8-发送8继电器字
#define	              RW_TX9  886	//TX9-发送9继电器字
#define	             RW_TX10  887	//TX10-发送10继电器字
#define	             RW_TX11  888	//TX11-发送11继电器字
#define	             RW_TX12  889	//TX12-发送12继电器字
#define	             RW_TX13  890	//TX13-发送13继电器字
#define	             RW_TX14  891	//TX14-发送14继电器字
#define	             RW_TX15  892	//TX15-发送15继电器字
#define	             RW_TX16  893	//TX16-发送16继电器字
#define	              RW_RX1  894	//RX1-接收1继电器字
#define	              RW_RX2  895	//RX2-接收2继电器字
#define	              RW_RX3  896	//RX3-接收3继电器字
#define	              RW_RX4  897	//RX4-接收4继电器字
#define	              RW_RX5  898	//RX5-接收5继电器字
#define	              RW_RX6  899	//RX6-接收6继电器字
#define	              RW_RX7  900	//RX7-接收7继电器字
#define	              RW_RX8  901	//RX8-接收8继电器字
#define	              RW_RX9  902	//RX9-接收9继电器字
#define	             RW_RX10  903	//RX10-接收10继电器字
#define	             RW_RX11  904	//RX11-接收11继电器字
#define	             RW_RX12  905	//RX12-接收12继电器字
#define	             RW_RX13  906	//RX13-接收13继电器字
#define	             RW_RX14  907	//RX14-接收14继电器字
#define	             RW_RX15  908	//RX15-接收15继电器字
#define	             RW_RX16  909	//RX16-接收16继电器字
#define	             RW_CNT1  910	//CNT1-计数继电器字1
#define	             RW_CNT2  911	//CNT2-计数继电器字2
#define	             RW_CNT3  912	//CNT3-计数继电器字3
#define	             RW_CNT4  913	//CNT4-计数继电器字4
#define	             RW_CNT5  914	//CNT5-计数继电器字5
#define	             RW_CNT6  915	//CNT6-计数继电器字6
#define	             RW_CNT7  916	//CNT7-计数继电器字7
#define	             RW_CNT8  917	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			918		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			83	

//中间继电器字
#define	MID_RELAY_WORD_START			85	
#define	MID_RELAY_WORD_NUM			152	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			237	
#define	KEEP_RELAY_WORD_NUM			87	

//时间继电器字
#define	TIME_RELAY_WORD_START			324	
#define	TIME_RELAY_WORD_NUM			105	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			429	
#define	TRIP_RELAY_WORD_NUM			4	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			433	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			465	
#define	INPUT_RELAY_WORD_NUM			62	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			527	
#define	OUTPUT_RELAY_WORD_NUM			43	

//压板继电器
#define	SW_RELAY_WORD_START			570	
#define	SW_RELAY_WORD_NUM			16	

//配置继电器
#define	CFG_RELAY_WORD_START			586	
#define	CFG_RELAY_WORD_NUM			20	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			606	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			622	
#define	CTRLTEST_RELAY_WORD_NUM			28	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			650	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			666	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			690	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			706	
#define	RCP_RELAY_WORD_NUM			52	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			758	
#define	RCE_RELAY_WORD_NUM			52	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			810	
#define	RCV_RELAY_WORD_NUM			52	

//大于等于继电器
#define	GE_RELAY_WORD_START			862	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			870	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			878	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			894	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			910	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			910	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			910	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			910	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			910	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			910	
#define	CNT_RELAY_WORD_NUM			8	


#endif