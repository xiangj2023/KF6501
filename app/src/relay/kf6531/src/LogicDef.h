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
#define	          RW_LPC50HA    2	//LPC50HA-高侧A过流保护循环控制继电器字
#define	          RW_LPC50HB    3	//LPC50HB-高侧B过流保护循环控制继电器字
#define	          RW_LPC50HC    4	//LPC50HC-高侧C过流保护循环控制继电器字
#define	         RW_LPC50LA1    5	//LPC50LA1-低侧A过流I段保护循环控制继电器字
#define	         RW_LPC50LB1    6	//LPC50LB1-低侧B过流I段保护循环控制继电器字
#define	        RW_LPC50OLA1    7	//LPC50OLA1-A相过负荷1保护循环控制继电器字
#define	        RW_LPC50OLB1    8	//LPC50OLB1-B相过负荷1保护循环控制继电器字
#define	        RW_LPC50OLC1    9	//LPC50OLC1-C相过负荷1保护循环控制继电器字
#define	        RW_LPC50OLA2   10	//LPC50OLA2-A相过负荷2保护循环控制继电器字
#define	        RW_LPC50OLB2   11	//LPC50OLB2-B相过负荷2保护循环控制继电器字
#define	        RW_LPC50OLC2   12	//LPC50OLC2-C相过负荷2保护循环控制继电器字
#define	           RW_LPC50N   13	//LPC50N-零序过流保护循环控制继电器字
#define	           RW_LPC59N   14	//LPC59N-零序过压保护循环控制继电器字
#define	         RW_LPC50DCG   15	//LPC50DCG-间隙过流保护循环控制继电器字
#define	         RW_LPC50LA2   16	//LPC50LA2-低侧A过流II段保护循环控制继电器字
#define	         RW_LPC50LB2   17	//LPC50LB2-低侧B过流II段保护循环控制继电器字
#define	           RW_PI50HA   18	//PI50HA-高侧A过流入段继电器字
#define	           RW_TR50HA   19	//TR50HA-高侧A过流动作继电器字
#define	           RW_PI50HB   20	//PI50HB-高侧B过流入段继电器字
#define	           RW_TR50HB   21	//TR50HB-高侧B过流动作继电器字
#define	           RW_PI50HC   22	//PI50HC-高侧C过流入段继电器字
#define	           RW_TR50HC   23	//TR50HC-高侧C过流动作继电器字
#define	          RW_PI50LA1   24	//PI50LA1-低侧A过流I段入段继电器字
#define	          RW_TR50LA1   25	//TR50LA1-低侧A过流I段动作继电器字
#define	          RW_PI50LB1   26	//PI50LB1-低侧B过流I段入段继电器字
#define	          RW_TR50LB1   27	//TR50LB1-低侧B过流I段动作继电器字
#define	         RW_PI50OLA1   28	//PI50OLA1-A相过负荷1入段继电器字
#define	         RW_TR50OLA1   29	//TR50OLA1-A相过负荷1动作继电器字
#define	         RW_PI50OLB1   30	//PI50OLB1-B相过负荷1入段继电器字
#define	         RW_TR50OLB1   31	//TR50OLB1-B相过负荷1动作继电器字
#define	         RW_PI50OLC1   32	//PI50OLC1-C相过负荷1入段继电器字
#define	         RW_TR50OLC1   33	//TR50OLC1-C相过负荷1动作继电器字
#define	         RW_PI50OLA2   34	//PI50OLA2-A相过负荷2入段继电器字
#define	         RW_TR50OLA2   35	//TR50OLA2-A相过负荷2动作继电器字
#define	         RW_PI50OLB2   36	//PI50OLB2-B相过负荷2入段继电器字
#define	         RW_TR50OLB2   37	//TR50OLB2-B相过负荷2动作继电器字
#define	         RW_PI50OLC2   38	//PI50OLC2-C相过负荷2入段继电器字
#define	         RW_TR50OLC2   39	//TR50OLC2-C相过负荷2动作继电器字
#define	            RW_PI50N   40	//PI50N-零序过流入段继电器字
#define	            RW_TR50N   41	//TR50N-零序过流动作继电器字
#define	            RW_PI59N   42	//PI59N-零序过压入段继电器字
#define	            RW_TR59N   43	//TR59N-零序过压动作继电器字
#define	          RW_PI50DCG   44	//PI50DCG-间隙过流入段继电器字
#define	          RW_TR50DCG   45	//TR50DCG-间隙过流动作继电器字
#define	             RW_PI27   46	//PI27-高侧失压入段继电器字
#define	             RW_TR27   47	//TR27-高侧失压动作继电器字
#define	            RW_AR49A   48	//AR49A-A相反时限过负荷告警继电器字
#define	            RW_TR49A   49	//TR49A-A相反时限过负荷动作继电器字
#define	            RW_AR49B   50	//AR49B-B相反时限告警继电器字
#define	            RW_TR49B   51	//TR49B-B相反时限动作继电器字
#define	            RW_AR49C   52	//AR49C-C相反时限告警继电器字
#define	            RW_TR49C   53	//TR49C-C相反时限动作继电器字
#define	          RW_PI50LA2   54	//PI50LA2-低侧A相过流II段入段继电器字
#define	          RW_TR50LA2   55	//TR50LA2-低侧A过流II段动作继电器字
#define	          RW_PI50LB2   56	//PI50LB2-低侧B过流II段入段继电器字
#define	          RW_TR50LB2   57	//TR50LB2-低侧B过流II段动作继电器字
#define	          RW_PI74PTH   58	//PI74PTH-高侧PT断线入段继电器字
#define	          RW_AR74PTH   59	//AR74PTH-高侧PT断线告警继电器字
#define	         RW_PI74PTLA   60	//PI74PTLA-低侧A相PT断线入段继电器字
#define	         RW_AR74PTLA   61	//AR74PTLA-低侧A相PT断线动作继电器字
#define	         RW_PI74PTLB   62	//PI74PTLB-低侧B相PT断线入段继电器字
#define	         RW_AR74PTLB   63	//AR74PTLB-低侧B相PT断线动作继电器字
#define	           RW_PICKUP   64	//PICKUP-模值突变量启动继电器字
#define	             RW_50HA   65	//50HA-IHA有流继电器字
#define	             RW_37HA   66	//37HA-IHA无流继电器字
#define	             RW_50HB   67	//50HB-IHB有流继电器字
#define	             RW_37HB   68	//37HB-IHB无流继电器字
#define	             RW_50HC   69	//50HC-IHC有流继电器字
#define	             RW_37HC   70	//37HC-IHC无流继电器字
#define	             RW_50LA   71	//50LA-ILA有流继电器字
#define	             RW_37LA   72	//37LA-ILA无流继电器字
#define	             RW_50LB   73	//50LB-ILB有流继电器字
#define	             RW_37LB   74	//37LB-ILB无流继电器字
#define	              RW_50N   75	//50N-I0有流继电器字
#define	              RW_37N   76	//37N-I0无流继电器字
#define	             RW_50JX   77	//50JX-间隙电流有流继电器字
#define	             RW_37JX   78	//37JX-间隙电流无流继电器字
#define	             RW_59HA   79	//59HA-UHA有压继电器字
#define	             RW_27HA   80	//27HA-UHA无压继电器字
#define	             RW_59HB   81	//59HB-UHB有压继电器字
#define	             RW_27HB   82	//27HB-UHB无压继电器字
#define	             RW_59HC   83	//59HC-UHC有压继电器字
#define	             RW_27HC   84	//27HC-UHC无压继电器字
#define	             RW_59LA   85	//59LA-ULA有压继电器字
#define	             RW_27LA   86	//27LA-ULA无压继电器字
#define	             RW_59LB   87	//59LB-ULB有压继电器字
#define	             RW_27LB   88	//27LB-ULB无压继电器字
#define	              RW_59N   89	//59N-U0有压继电器字
#define	              RW_27N   90	//27N-U0无压继电器字
#define	          RW_ET11ERR   91	//ET11ERR-以太网1链接1状态继电器字
#define	          RW_ET12ERR   92	//ET12ERR-以太网1链接2状态继电器字
#define	          RW_ET13ERR   93	//ET13ERR-以太网1链接3状态继电器字
#define	          RW_ET14ERR   94	//ET14ERR-以太网1链接4状态继电器字
#define	          RW_ET15ERR   95	//ET15ERR-以太网1链接5状态继电器字
#define	          RW_ET21ERR   96	//ET21ERR-以太网2链接1状态继电器字
#define	          RW_ET22ERR   97	//ET22ERR-以太网2链接2状态继电器字
#define	          RW_ET23ERR   98	//ET23ERR-以太网2链接3状态继电器字
#define	          RW_ET24ERR   99	//ET24ERR-以太网2链接4状态继电器字
#define	          RW_ET25ERR  100	//ET25ERR-以太网2链接5状态继电器字
#define	          RW_COM1ERR  101	//COM1ERR-串口1链接状态继电器字
#define	          RW_CAN1ERR  102	//CAN1ERR-CAN网1链接状态继电器字
#define	          RW_CAN2ERR  103	//CAN2ERR-CAN网2链接状态继电器字
#define	         RW_NULL1ERR  104	//NULL1ERR-NULL1链接状态继电器字
#define	         RW_NULL2ERR  105	//NULL2ERR-NULL2链接状态继电器字
#define	         RW_NULL3ERR  106	//NULL3ERR-NULL3链接状态继电器字
#define	        RW_MAINTLINK  107	//MAINTLINK-维护状态继电器字
#define	         RW_LKPI50HA  108	//LKPI50HA-高侧A过流入段闭锁继电器字
#define	        RW_LKRST50HA  109	//LKRST50HA-高侧A过流返回闭锁继电器字
#define	         RW_LKPI50HB  110	//LKPI50HB-高侧B过流入段闭锁继电器字
#define	        RW_LKRST50HB  111	//LKRST50HB-高侧B过流返回闭锁继电器字
#define	         RW_LKPI50HC  112	//LKPI50HC-高侧C过流入段闭锁继电器字
#define	        RW_LKRST50HC  113	//LKRST50HC-高侧C过流返回闭锁继电器字
#define	        RW_LKPI50LA1  114	//LKPI50LA1-低侧A过流I段入段闭锁继电器字
#define	       RW_LKRST50LA1  115	//LKRST50LA1-低侧A过流I段返回闭锁继电器字
#define	        RW_LKPI50LB1  116	//LKPI50LB1-低侧B过流I段入段闭锁继电器字
#define	       RW_LKRST50LB1  117	//LKRST50LB1-低侧B过流I段返回闭锁继电器字
#define	       RW_LKPI50OLA1  118	//LKPI50OLA1-A过负荷1入段闭锁继电器字
#define	      RW_LKRST50OLA1  119	//LKRST50OLA1-A过负荷1返回闭锁继电器字
#define	       RW_LKPI50OLB1  120	//LKPI50OLB1-B过负荷1入段闭锁继电器字
#define	      RW_LKRST50OLB1  121	//LKRST50OLB1-B过负荷1返回闭锁继电器字
#define	       RW_LKPI50OLC1  122	//LKPI50OLC1-C过负荷1入段闭锁继电器字
#define	      RW_LKRST50OLC1  123	//LKRST50OLC1-C过负荷1返回闭锁继电器字
#define	       RW_LKPI50OLA2  124	//LKPI50OLA2-A过负荷2入段闭锁继电器字
#define	      RW_LKRST50OLA2  125	//LKRST50OLA2-A过负荷2返回闭锁继电器字
#define	       RW_LKPI50OLB2  126	//LKPI50OLB2-B过负荷2入段闭锁继电器字
#define	      RW_LKRST50OLB2  127	//LKRST50OLB2-B过负荷2返回闭锁继电器字
#define	       RW_LKPI50OLC2  128	//LKPI50OLC2-C过负荷2入段闭锁继电器字
#define	      RW_LKRST50OLC2  129	//LKRST50OLC2-C过负荷2返回闭锁继电器字
#define	          RW_LKPI50N  130	//LKPI50N-零序过流入段闭锁继电器字
#define	         RW_LKRST50N  131	//LKRST50N-零序过流返回闭锁继电器字
#define	          RW_LKPI59N  132	//LKPI59N-零序过压入段闭锁继电器字
#define	         RW_LKRST59N  133	//LKRST59N-零序过压返回闭锁继电器字
#define	        RW_LKPI50DCG  134	//LKPI50DCG-间隙过流入段闭锁继电器字
#define	       RW_LKRST50DCG  135	//LKRST50DCG-间隙过流返回闭锁继电器字
#define	           RW_LKPI27  136	//LKPI27-高侧失压入段闭锁继电器字
#define	          RW_LKRST27  137	//LKRST27-高侧失压返回闭锁继电器字
#define	          RW_LKPIPTH  138	//LKPIPTH-高侧PT断线入段闭锁继电器字
#define	         RW_LKPIPTLA  139	//LKPIPTLA-低侧A相PT断线入段闭锁继电器字
#define	         RW_LKPIPTLB  140	//LKPIPTLB-低侧B相PT断线入段闭锁继电器字
#define	        RW_LKPI50LA2  141	//LKPI50LA2-低侧A过流II段入段闭锁继电器字
#define	       RW_LKRST50LA2  142	//LKRST50LA2-低侧A过流II段返回闭锁继电器字
#define	        RW_LKPI50LB2  143	//LKPI50LB2-低侧B过流II段入段闭锁继电器字
#define	       RW_LKRST50LB2  144	//LKRST50LB2-低侧B过流II段返回闭锁继电器字
#define	             RW_LKCH  145	//LKCH-高侧断路器合闸闭锁继电器字
#define	             RW_LKTH  146	//LKTH-高侧断路器跳闸闭锁继电器字
#define	            RW_LKCLA  147	//LKCLA-低侧断路器A合闸闭锁继电器字
#define	            RW_LKTLA  148	//LKTLA-低侧断路器A跳闸闭锁继电器字
#define	            RW_LKCLB  149	//LKCLB-低侧断路器B合闸闭锁继电器字
#define	            RW_LKTLB  150	//LKTLB-低侧断路器B跳闸闭锁继电器字
#define	             RW_52AH  151	//52AH-高压侧断路器合位
#define	             RW_52BH  152	//52BH-高压侧断路器分位
#define	            RW_52ALA  153	//52ALA-低侧A断路器合位
#define	            RW_52BLA  154	//52BLA-低侧A断路器分位
#define	            RW_52ALB  155	//52ALB-低侧B断路器合位
#define	            RW_52BLB  156	//52BLB-低侧B断路器分位
#define	              RW_REC  157	//REC-录波继电器字
#define	             RW_69IN  158	//69IN-远方当地中间继电器字
#define	               RW_M1  159	//M1-中间继电器字M1
#define	               RW_M2  160	//M2-中间继电器字M2
#define	               RW_M3  161	//M3-中间继电器字M3
#define	               RW_M4  162	//M4-中间继电器字M4
#define	               RW_M5  163	//M5-中间继电器字M5
#define	               RW_M6  164	//M6-中间继电器字M6
#define	               RW_M7  165	//M7-中间继电器字M7
#define	               RW_M8  166	//M8-中间继电器字M8
#define	               RW_M9  167	//M9-中间继电器字M9
#define	              RW_M10  168	//M10-中间继电器字M10
#define	              RW_M11  169	//M11-中间继电器字M11
#define	              RW_M12  170	//M12-中间继电器字M12
#define	              RW_M13  171	//M13-中间继电器字M13
#define	              RW_M14  172	//M14-中间继电器字M14
#define	              RW_M15  173	//M15-中间继电器字M15
#define	              RW_M16  174	//M16-中间继电器字M16
#define	              RW_M17  175	//M17-中间继电器字M17
#define	              RW_M18  176	//M18-中间继电器字M18
#define	              RW_M19  177	//M19-中间继电器字M19
#define	              RW_M20  178	//M20-中间继电器字M20
#define	              RW_M21  179	//M21-中间继电器字M21
#define	              RW_M22  180	//M22-中间继电器字M22
#define	              RW_M23  181	//M23-中间继电器字M23
#define	              RW_M24  182	//M24-中间继电器字M24
#define	              RW_M25  183	//M25-中间继电器字M25
#define	              RW_M26  184	//M26-中间继电器字M26
#define	              RW_M27  185	//M27-中间继电器字M27
#define	              RW_M28  186	//M28-中间继电器字M28
#define	              RW_M29  187	//M29-中间继电器字M29
#define	              RW_M30  188	//M30-中间继电器字M30
#define	              RW_M31  189	//M31-中间继电器字M31
#define	              RW_M32  190	//M32-中间继电器字M32
#define	              RW_M33  191	//M33-中间继电器字M33
#define	              RW_M34  192	//M34-中间继电器字M34
#define	              RW_M35  193	//M35-中间继电器字M35
#define	              RW_M36  194	//M36-中间继电器字M36
#define	              RW_M37  195	//M37-中间继电器字M37
#define	              RW_M38  196	//M38-中间继电器字M38
#define	              RW_M39  197	//M39-中间继电器字M39
#define	              RW_M40  198	//M40-中间继电器字M40
#define	              RW_M41  199	//M41-中间继电器字M41
#define	              RW_M42  200	//M42-中间继电器字M42
#define	              RW_M43  201	//M43-中间继电器字M43
#define	              RW_M44  202	//M44-中间继电器字M44
#define	              RW_M45  203	//M45-中间继电器字M45
#define	              RW_M46  204	//M46-中间继电器字M46
#define	              RW_M47  205	//M47-中间继电器字M47
#define	              RW_M48  206	//M48-中间继电器字M48
#define	              RW_M49  207	//M49-中间继电器字M49
#define	              RW_M50  208	//M50-中间继电器字M50
#define	              RW_M51  209	//M51-中间继电器字M51
#define	              RW_M52  210	//M52-中间继电器字M52
#define	              RW_M53  211	//M53-中间继电器字M53
#define	              RW_M54  212	//M54-中间继电器字M54
#define	              RW_M55  213	//M55-中间继电器字M55
#define	              RW_M56  214	//M56-中间继电器字M56
#define	              RW_M57  215	//M57-中间继电器字M57
#define	              RW_M58  216	//M58-中间继电器字M58
#define	              RW_M59  217	//M59-中间继电器字M59
#define	              RW_M60  218	//M60-中间继电器字M60
#define	              RW_M61  219	//M61-中间继电器字M61
#define	              RW_M62  220	//M62-中间继电器字M62
#define	              RW_M63  221	//M63-中间继电器字M63
#define	              RW_M64  222	//M64-中间继电器字M64
#define	               RW_H1  223	//H1-自保持中间继电器字1
#define	               RW_H2  224	//H2-自保持中间继电器字2
#define	               RW_H3  225	//H3-自保持中间继电器字3
#define	               RW_H4  226	//H4-自保持中间继电器字4
#define	               RW_H5  227	//H5-自保持中间继电器字5
#define	               RW_H6  228	//H6-自保持中间继电器字6
#define	               RW_H7  229	//H7-自保持中间继电器字7
#define	               RW_H8  230	//H8-自保持中间继电器字8
#define	               RW_H9  231	//H9-自保持中间继电器字9
#define	              RW_H10  232	//H10-自保持中间继电器字10
#define	              RW_H11  233	//H11-自保持中间继电器字11
#define	              RW_H12  234	//H12-自保持中间继电器字12
#define	              RW_H13  235	//H13-自保持中间继电器字13
#define	              RW_H14  236	//H14-自保持中间继电器字14
#define	              RW_H15  237	//H15-自保持中间继电器字15
#define	              RW_H16  238	//H16-自保持中间继电器字16
#define	              RW_H17  239	//H17-自保持中间继电器字17
#define	              RW_H18  240	//H18-自保持中间继电器字18
#define	              RW_H19  241	//H19-自保持中间继电器字19
#define	              RW_H20  242	//H20-自保持中间继电器字20
#define	              RW_H21  243	//H21-自保持中间继电器字21
#define	              RW_H22  244	//H22-自保持中间继电器字22
#define	              RW_H23  245	//H23-自保持中间继电器字23
#define	              RW_H24  246	//H24-自保持中间继电器字24
#define	              RW_H25  247	//H25-自保持中间继电器字25
#define	              RW_H26  248	//H26-自保持中间继电器字26
#define	              RW_H27  249	//H27-自保持中间继电器字27
#define	              RW_H28  250	//H28-自保持中间继电器字28
#define	              RW_H29  251	//H29-自保持中间继电器字29
#define	              RW_H30  252	//H30-自保持中间继电器字30
#define	              RW_H31  253	//H31-自保持中间继电器字31
#define	              RW_H32  254	//H32-自保持中间继电器字32
#define	            RW_52BFH  255	//52BFH-高侧断路器跳闸失败继电器字
#define	           RW_52BFLA  256	//52BFLA-低侧A断路器跳闸失败继电器字
#define	           RW_52BFLB  257	//52BFLB-低侧B断路器跳闸失败继电器字
#define	               RW_T1  258	//T1-T1
#define	               RW_T2  259	//T2-T2
#define	               RW_T3  260	//T3-T3
#define	               RW_T4  261	//T4-T4
#define	               RW_T5  262	//T5-T5
#define	               RW_T6  263	//T6-T6
#define	               RW_T7  264	//T7-T7
#define	               RW_T8  265	//T8-T8
#define	               RW_T9  266	//T9-T9
#define	              RW_T10  267	//T10-T10
#define	              RW_T11  268	//T11-T11
#define	              RW_T12  269	//T12-T12
#define	              RW_T13  270	//T13-T13
#define	              RW_T14  271	//T14-T14
#define	              RW_T15  272	//T15-T15
#define	              RW_T16  273	//T16-T16
#define	             RW_TCB1  274	//TCB1-TCB1
#define	             RW_TCB2  275	//TCB2-TCB2
#define	             RW_TCB3  276	//TCB3-TCB3
#define	             RW_TCB4  277	//TCB4-TCB4
#define	             RW_TCB5  278	//TCB5-TCB5
#define	             RW_TCB6  279	//TCB6-TCB6
#define	             RW_TCB7  280	//TCB7-TCB7
#define	             RW_TCB8  281	//TCB8-TCB8
#define	             RW_TSW1  282	//TSW1-TSW1
#define	             RW_TSW2  283	//TSW2-TSW2
#define	             RW_TSW3  284	//TSW3-TSW3
#define	             RW_TSW4  285	//TSW4-TSW4
#define	             RW_TSW5  286	//TSW5-TSW5
#define	             RW_TSW6  287	//TSW6-TSW6
#define	             RW_TSW7  288	//TSW7-TSW7
#define	             RW_TSW8  289	//TSW8-TSW8
#define	           RW_T52BFH  290	//T52BFH-T52BFH
#define	          RW_T52BFLA  291	//T52BFLA-T52BFLA
#define	          RW_T52BFLB  292	//T52BFLB-T52BFLB
#define	              RW_TR1  293	//TR1-跳闸继电器1
#define	              RW_TR2  294	//TR2-跳闸继电器2
#define	              RW_TR3  295	//TR3-跳闸继电器3
#define	              RW_TR4  296	//TR4-跳闸继电器4
#define	              RW_VR1  297	//VR1-虚拟继电器1
#define	              RW_VR2  298	//VR2-虚拟继电器2
#define	              RW_VR3  299	//VR3-虚拟继电器3
#define	              RW_VR4  300	//VR4-虚拟继电器4
#define	              RW_VR5  301	//VR5-虚拟继电器5
#define	              RW_VR6  302	//VR6-虚拟继电器6
#define	              RW_VR7  303	//VR7-虚拟继电器7
#define	              RW_VR8  304	//VR8-虚拟继电器8
#define	              RW_VR9  305	//VR9-虚拟继电器9
#define	             RW_VR10  306	//VR10-虚拟继电器10
#define	             RW_VR11  307	//VR11-虚拟继电器11
#define	             RW_VR12  308	//VR12-虚拟继电器12
#define	             RW_VR13  309	//VR13-虚拟继电器13
#define	             RW_VR14  310	//VR14-虚拟继电器14
#define	             RW_VR15  311	//VR15-虚拟继电器15
#define	             RW_VR16  312	//VR16-虚拟继电器16
#define	             RW_VR17  313	//VR17-虚拟继电器17
#define	             RW_VR18  314	//VR18-虚拟继电器18
#define	             RW_VR19  315	//VR19-虚拟继电器19
#define	             RW_VR20  316	//VR20-虚拟继电器20
#define	             RW_VR21  317	//VR21-虚拟继电器21
#define	             RW_VR22  318	//VR22-虚拟继电器22
#define	             RW_VR23  319	//VR23-虚拟继电器23
#define	             RW_VR24  320	//VR24-虚拟继电器24
#define	             RW_VR25  321	//VR25-虚拟继电器25
#define	             RW_VR26  322	//VR26-虚拟继电器26
#define	             RW_VR27  323	//VR27-虚拟继电器27
#define	             RW_VR28  324	//VR28-虚拟继电器28
#define	             RW_VR29  325	//VR29-虚拟继电器29
#define	             RW_VR30  326	//VR30-虚拟继电器30
#define	             RW_VR31  327	//VR31-虚拟继电器31
#define	             RW_VR32  328	//VR32-虚拟继电器32
#define	              RW_IN1  329	//IN1-IN1
#define	              RW_IN2  330	//IN2-IN2
#define	              RW_IN3  331	//IN3-IN3
#define	              RW_IN4  332	//IN4-IN4
#define	              RW_IN5  333	//IN5-IN5
#define	              RW_IN6  334	//IN6-IN6
#define	              RW_IN7  335	//IN7-IN7
#define	              RW_IN8  336	//IN8-IN8
#define	              RW_IN9  337	//IN9-IN9
#define	             RW_IN10  338	//IN10-IN10
#define	             RW_IN11  339	//IN11-IN11
#define	             RW_IN12  340	//IN12-IN12
#define	             RW_IN13  341	//IN13-IN13
#define	             RW_IN14  342	//IN14-IN14
#define	             RW_IN15  343	//IN15-IN15
#define	             RW_IN16  344	//IN16-IN16
#define	             RW_IN17  345	//IN17-IN17
#define	             RW_IN18  346	//IN18-IN18
#define	             RW_IN19  347	//IN19-IN19
#define	             RW_IN20  348	//IN20-IN20
#define	             RW_IN21  349	//IN21-IN21
#define	             RW_IN22  350	//IN22-IN22
#define	             RW_IN23  351	//IN23-IN23
#define	             RW_IN24  352	//IN24-IN24
#define	             RW_IN25  353	//IN25-IN25
#define	             RW_IN26  354	//IN26-IN26
#define	             RW_IN27  355	//IN27-IN27
#define	             RW_IN28  356	//IN28-IN28
#define	             RW_IN29  357	//IN29-IN29
#define	             RW_IN30  358	//IN30-IN30
#define	             RW_IN31  359	//IN31-IN31
#define	             RW_IN32  360	//IN32-IN32
#define	             RW_IN33  361	//IN33-IN33
#define	             RW_IN34  362	//IN34-IN34
#define	             RW_IN35  363	//IN35-信号复归
#define	             RW_IN36  364	//IN36-低侧A断路器跳位
#define	             RW_IN37  365	//IN37-低侧A断路器合位
#define	             RW_IN38  366	//IN38-低侧A断路器遥控预置
#define	             RW_IN39  367	//IN39-低侧A操作箱遥控1预置
#define	             RW_IN40  368	//IN40-低侧A操作箱遥控2预置
#define	             RW_IN41  369	//IN41-低侧B断路器跳位
#define	             RW_IN42  370	//IN42-低侧B断路器合位
#define	             RW_IN43  371	//IN43-低侧B断路器遥控预置
#define	             RW_IN44  372	//IN44-低侧B操作箱遥控1预置
#define	             RW_IN45  373	//IN45-低侧B操作箱遥控2预置
#define	             RW_IN46  374	//IN46-高侧断路器跳位
#define	             RW_IN47  375	//IN47-高侧断路器合位
#define	             RW_IN48  376	//IN48-高侧操作箱遥控预置
#define	             RW_IN49  377	//IN49-高侧操作箱遥控1预置
#define	             RW_IN50  378	//IN50-高侧操作箱遥控2预置
#define	           RW_INVIN1  379	//INVIN1-VIN1
#define	           RW_INVIN2  380	//INVIN2-VIN2
#define	           RW_INVIN3  381	//INVIN3-VIN3
#define	           RW_INVIN4  382	//INVIN4-VIN4
#define	           RW_INVIN5  383	//INVIN5-VIN5
#define	           RW_INVIN6  384	//INVIN6-VIN6
#define	           RW_INVIN7  385	//INVIN7-VIN7
#define	           RW_INVIN8  386	//INVIN8-VIN8
#define	           RW_INVIN9  387	//INVIN9-VIN9
#define	          RW_INVIN10  388	//INVIN10-VIN10
#define	          RW_INVIN11  389	//INVIN11-VIN11
#define	          RW_INVIN12  390	//INVIN12-VIN12
#define	          RW_INVIN13  391	//INVIN13-VIN13
#define	          RW_INVIN14  392	//INVIN14-VIN14
#define	          RW_INVIN15  393	//INVIN15-VIN15
#define	          RW_INVIN16  394	//INVIN16-VIN16
#define	          RW_INVIN17  395	//INVIN17-VIN17
#define	          RW_INVIN18  396	//INVIN18-VIN18
#define	          RW_INVIN19  397	//INVIN19-VIN19
#define	          RW_INVIN20  398	//INVIN20-VIN20
#define	          RW_INVIN21  399	//INVIN21-VIN21
#define	          RW_INVIN22  400	//INVIN22-VIN22
#define	          RW_INVIN23  401	//INVIN23-VIN23
#define	          RW_INVIN24  402	//INVIN24-VIN24
#define	          RW_INVIN25  403	//INVIN25-VIN25
#define	          RW_INVIN26  404	//INVIN26-VIN26
#define	          RW_INVIN27  405	//INVIN27-VIN27
#define	          RW_INVIN28  406	//INVIN28-VIN28
#define	          RW_INVIN29  407	//INVIN29-VIN29
#define	          RW_INVIN30  408	//INVIN30-VIN30
#define	          RW_INVIN31  409	//INVIN31-VIN31
#define	          RW_INVIN32  410	//INVIN32-VIN32
#define	             RW_OUT1  411	//OUT1-开出1继电器字
#define	             RW_OUT2  412	//OUT2-开出2继电器字
#define	             RW_OUT3  413	//OUT3-开出3继电器字
#define	             RW_OUT4  414	//OUT4-开出4继电器字
#define	             RW_OUT5  415	//OUT5-开出5继电器字
#define	             RW_OUT6  416	//OUT6-开出6继电器字
#define	             RW_OUT7  417	//OUT7-开出7继电器字
#define	             RW_OUT8  418	//OUT8-开出8继电器字
#define	             RW_OUT9  419	//OUT9-开出9继电器字
#define	            RW_OUT10  420	//OUT10-开出10继电器字
#define	            RW_OUT11  421	//OUT11-开出11继电器字
#define	            RW_OUT12  422	//OUT12-开出12继电器字
#define	            RW_OUT13  423	//OUT13-开出13继电器字
#define	            RW_OUT14  424	//OUT14-开出14继电器字
#define	            RW_OUT15  425	//OUT15-开出15继电器字
#define	            RW_OUT16  426	//OUT16-开出16继电器字
#define	            RW_OUT17  427	//OUT17-开出17继电器字
#define	            RW_OUT18  428	//OUT18-开出18继电器字
#define	            RW_OUT19  429	//OUT19-开出19继电器字
#define	            RW_OUT20  430	//OUT20-开出20继电器字
#define	            RW_OUT21  431	//OUT21-开出21继电器字
#define	            RW_OUT22  432	//OUT22-开出22继电器字
#define	            RW_OUT23  433	//OUT23-开出23继电器字
#define	            RW_OUT24  434	//OUT24-开出24继电器字
#define	            RW_OUT25  435	//OUT25-开出25继电器字
#define	            RW_OUT26  436	//OUT26-开出26继电器字
#define	            RW_OUT27  437	//OUT27-开出27继电器字
#define	            RW_OUT28  438	//OUT28-开出28继电器字
#define	            RW_OUT29  439	//OUT29-开出29继电器字
#define	            RW_OUT30  440	//OUT30-开出30继电器字
#define	            RW_OUT31  441	//OUT31-开出31继电器字
#define	            RW_OUT32  442	//OUT32-开出32继电器字
#define	            RW_OUT33  443	//OUT33-开出33继电器字
#define	            RW_OUT34  444	//OUT34-开出34继电器字
#define	            RW_OUT35  445	//OUT35-开出35继电器字
#define	            RW_OUT36  446	//OUT36-开出36继电器字
#define	            RW_OUT37  447	//OUT37-开出37继电器字
#define	            RW_OUT38  448	//OUT38-开出38继电器字
#define	            RW_OUT39  449	//OUT39-开出39继电器字
#define	            RW_OUT40  450	//OUT40-开出40继电器字
#define	            RW_OUT41  451	//OUT41-开出41继电器字
#define	            RW_SL50H  452	//SL50H-高侧过流保护软压板继电器字
#define	             RW_SL49  453	//SL49-反时限过负荷软压板继电器字
#define	            RW_SL50L  454	//SL50L-低侧过流保护软压板继电器字
#define	           RW_SL50OL  455	//SL50OL-过负荷软压板继电器字
#define	             RW_SL27  456	//SL27-高侧失压保护软压板继电器字
#define	            RW_SL50N  457	//SL50N-零序过流软压板继电器字
#define	            RW_SL59N  458	//SL59N-零序过压软压板继电器字
#define	          RW_SL50DCG  459	//SL50DCG-间隙过流软压板继电器字
#define	          RW_SLMAINT  460	//SLMAINT-检修压板
#define	           RW_SLBAK1  461	//SLBAK1-备用压板1
#define	           RW_SLBAK2  462	//SLBAK2-备用压板2
#define	            RW_F50HA  463	//F50HA-高侧A相过流配置继电器字
#define	            RW_F50HB  464	//F50HB-高侧B相过流配置继电器字
#define	            RW_F50HC  465	//F50HC-高侧C相过流配置继电器字
#define	          RW_F50H_27  466	//F50H_27-高侧低压启动配置继电器字
#define	              RW_F49  467	//F49-反时限过负荷配置继电器字
#define	           RW_F50LA1  468	//F50LA1-低侧A相过流I段配置继电器字
#define	         RW_F50LA_27  469	//F50LA_27-低侧A低压启动配置继电器字
#define	           RW_F50LB1  470	//F50LB1-低侧B相过流I段配置继电器字
#define	         RW_F50LB_27  471	//F50LB_27-低侧B相低压启动配置继电器字
#define	          RW_F50OLA1  472	//F50OLA1-A过负荷告警配置继电器字
#define	          RW_F50OLB1  473	//F50OLB1-B过负荷告警配置继电器字
#define	          RW_F50OLC1  474	//F50OLC1-C过负荷告警配置继电器字
#define	          RW_F50OLA2  475	//F50OLA2-A过负荷跳闸配置继电器字
#define	          RW_F50OLB2  476	//F50OLB2-B过负荷跳闸配置继电器字
#define	          RW_F50OLC2  477	//F50OLC2-C过负荷跳闸配置继电器字
#define	              RW_F27  478	//F27-高侧失压保护配置继电器字
#define	             RW_F50N  479	//F50N-零序过流保护配置继电器字
#define	             RW_F59N  480	//F59N-零序过压保护配置继电器字
#define	            RW_F74PT  481	//F74PT-PT断线检测配置继电器字
#define	           RW_F50DCG  482	//F50DCG-间隙过流保护配置继电器字
#define	           RW_F50LA2  483	//F50LA2-低侧A相过流II段配置继电器字
#define	           RW_F50LB2  484	//F50LB2-低侧B相过流II段配置继电器字
#define	            RW_F52BF  485	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  486	//FBAK1-备用配置1
#define	            RW_FBAK2  487	//FBAK2-备用配置2
#define	              RW_PW1  488	//PW1-脉冲继电器1
#define	              RW_PW2  489	//PW2-脉冲继电器2
#define	              RW_PW3  490	//PW3-脉冲继电器3
#define	              RW_PW4  491	//PW4-脉冲继电器4
#define	              RW_PW5  492	//PW5-脉冲继电器5
#define	              RW_PW6  493	//PW6-脉冲继电器6
#define	              RW_PW7  494	//PW7-脉冲继电器7
#define	              RW_PW8  495	//PW8-脉冲继电器8
#define	              RW_PW9  496	//PW9-脉冲继电器9
#define	             RW_PW10  497	//PW10-脉冲继电器10
#define	             RW_PW11  498	//PW11-脉冲继电器11
#define	             RW_PW12  499	//PW12-脉冲继电器12
#define	             RW_PW13  500	//PW13-脉冲继电器13
#define	             RW_PW14  501	//PW14-脉冲继电器14
#define	             RW_PW15  502	//PW15-脉冲继电器15
#define	             RW_PW16  503	//PW16-脉冲继电器16
#define	              RW_TO1  504	//TO1-传动继电器1
#define	              RW_TO2  505	//TO2-传动继电器2
#define	              RW_TO3  506	//TO3-传动继电器3
#define	              RW_TO4  507	//TO4-传动继电器4
#define	              RW_TO5  508	//TO5-传动继电器5
#define	              RW_TO6  509	//TO6-传动继电器6
#define	              RW_TO7  510	//TO7-传动继电器7
#define	              RW_TO8  511	//TO8-传动继电器8
#define	              RW_TO9  512	//TO9-传动继电器9
#define	             RW_TO10  513	//TO10-传动继电器10
#define	             RW_TO11  514	//TO11-传动继电器11
#define	             RW_TO12  515	//TO12-传动继电器12
#define	             RW_TO13  516	//TO13-传动继电器13
#define	             RW_TO14  517	//TO14-传动继电器14
#define	             RW_TO15  518	//TO15-传动继电器15
#define	             RW_TO16  519	//TO16-传动继电器16
#define	             RW_TO17  520	//TO17-传动继电器17
#define	             RW_TO18  521	//TO18-传动继电器18
#define	             RW_TO19  522	//TO19-传动继电器19
#define	             RW_TO20  523	//TO20-传动继电器20
#define	             RW_TO21  524	//TO21-传动继电器21
#define	             RW_SIG1  525	//SIG1-信号继电器1
#define	             RW_SIG2  526	//SIG2-信号继电器2
#define	             RW_SIG3  527	//SIG3-信号继电器3
#define	             RW_SIG4  528	//SIG4-信号继电器4
#define	             RW_SIG5  529	//SIG5-信号继电器5
#define	             RW_SIG6  530	//SIG6-信号继电器6
#define	             RW_SIG7  531	//SIG7-信号继电器7
#define	             RW_SIG8  532	//SIG8-信号继电器8
#define	             RW_SIG9  533	//SIG9-信号继电器9
#define	            RW_SIG10  534	//SIG10-信号继电器10
#define	            RW_SIG11  535	//SIG11-信号继电器11
#define	            RW_SIG12  536	//SIG12-信号继电器12
#define	            RW_SIG13  537	//SIG13-信号继电器13
#define	            RW_SIG14  538	//SIG14-信号继电器14
#define	            RW_SIG15  539	//SIG15-信号继电器15
#define	            RW_SIG16  540	//SIG16-信号继电器16
#define	            RW_LED1G  541	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  542	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  543	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  544	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  545	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  546	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  547	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  548	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  549	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  550	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  551	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  552	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  553	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  554	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  555	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  556	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  557	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  558	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  559	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  560	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  561	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  562	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  563	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  564	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  565	//VIN1-VIN1
#define	             RW_VIN2  566	//VIN2-VIN2
#define	             RW_VIN3  567	//VIN3-VIN3
#define	             RW_VIN4  568	//VIN4-VIN4
#define	             RW_VIN5  569	//VIN5-VIN5
#define	             RW_VIN6  570	//VIN6-VIN6
#define	             RW_VIN7  571	//VIN7-VIN7
#define	             RW_VIN8  572	//VIN8-VIN8
#define	             RW_VIN9  573	//VIN9-VIN9
#define	            RW_VIN10  574	//VIN10-VIN10
#define	            RW_VIN11  575	//VIN11-VIN11
#define	            RW_VIN12  576	//VIN12-VIN12
#define	            RW_VIN13  577	//VIN13-VIN13
#define	            RW_VIN14  578	//VIN14-VIN14
#define	            RW_VIN15  579	//VIN15-VIN15
#define	            RW_VIN16  580	//VIN16-VIN16
#define	            RW_VIN17  581	//VIN17-VIN17
#define	            RW_VIN18  582	//VIN18-VIN18
#define	            RW_VIN19  583	//VIN19-VIN19
#define	            RW_VIN20  584	//VIN20-VIN20
#define	            RW_VIN21  585	//VIN21-VIN21
#define	            RW_VIN22  586	//VIN22-VIN22
#define	            RW_VIN23  587	//VIN23-VIN23
#define	            RW_VIN24  588	//VIN24-VIN24
#define	            RW_VIN25  589	//VIN25-VIN25
#define	            RW_VIN26  590	//VIN26-VIN26
#define	            RW_VIN27  591	//VIN27-VIN27
#define	            RW_VIN28  592	//VIN28-VIN28
#define	            RW_VIN29  593	//VIN29-VIN29
#define	            RW_VIN30  594	//VIN30-VIN30
#define	            RW_VIN31  595	//VIN31-VIN31
#define	            RW_VIN32  596	//VIN32-VIN32
#define	             RW_RCP1  597	//RCP1-遥控预置继电器1
#define	             RW_RCP2  598	//RCP2-遥控预置继电器2
#define	             RW_RCP3  599	//RCP3-遥控预置继电器3
#define	             RW_RCP4  600	//RCP4-遥控预置继电器4
#define	             RW_RCP5  601	//RCP5-遥控预置继电器5
#define	             RW_RCP6  602	//RCP6-遥控预置继电器6
#define	             RW_RCP7  603	//RCP7-遥控预置继电器7
#define	             RW_RCP8  604	//RCP8-遥控预置继电器8
#define	             RW_RCP9  605	//RCP9-遥控预置继电器9
#define	            RW_RCP10  606	//RCP10-遥控预置继电器10
#define	            RW_RCP11  607	//RCP11-遥控预置继电器11
#define	            RW_RCP12  608	//RCP12-遥控预置继电器12
#define	            RW_RCP13  609	//RCP13-遥控预置继电器13
#define	            RW_RCP14  610	//RCP14-遥控预置继电器14
#define	            RW_RCP15  611	//RCP15-遥控预置继电器15
#define	            RW_RCP16  612	//RCP16-遥控预置继电器16
#define	            RW_RCP17  613	//RCP17-遥控预置继电器17
#define	            RW_RCP18  614	//RCP18-遥控预置继电器18
#define	            RW_RCP19  615	//RCP19-遥控预置继电器19
#define	            RW_RCP20  616	//RCP20-遥控预置继电器20
#define	            RW_RCP21  617	//RCP21-遥控预置继电器21
#define	            RW_RCP22  618	//RCP22-遥控预置继电器22
#define	            RW_RCP23  619	//RCP23-遥控预置继电器23
#define	            RW_RCP24  620	//RCP24-遥控预置继电器24
#define	            RW_RCP25  621	//RCP25-遥控预置继电器25
#define	            RW_RCP26  622	//RCP26-遥控预置继电器26
#define	            RW_RCP27  623	//RCP27-遥控预置继电器27
#define	            RW_RCP28  624	//RCP28-遥控预置继电器28
#define	            RW_RCP29  625	//RCP29-遥控预置继电器29
#define	            RW_RCP30  626	//RCP30-遥控预置继电器30
#define	            RW_RCP31  627	//RCP31-遥控预置继电器31
#define	            RW_RCP32  628	//RCP32-遥控预置继电器32
#define	            RW_RCP33  629	//RCP33-遥控预置继电器33
#define	            RW_RCP34  630	//RCP34-遥控预置继电器34
#define	            RW_RCP35  631	//RCP35-遥控预置继电器35
#define	            RW_RCP36  632	//RCP36-遥控预置继电器36
#define	            RW_RCP37  633	//RCP37-遥控预置继电器37
#define	            RW_RCP38  634	//RCP38-遥控预置继电器38
#define	            RW_RCP39  635	//RCP39-遥控预置继电器39
#define	            RW_RCP40  636	//RCP40-遥控预置继电器40
#define	            RW_RCP41  637	//RCP41-遥控预置继电器41
#define	            RW_RCP42  638	//RCP42-遥控预置继电器42
#define	            RW_RCP43  639	//RCP43-遥控预置继电器43
#define	            RW_RCP44  640	//RCP44-遥控预置继电器44
#define	             RW_RCE1  641	//RCE1-遥控执行继电器1
#define	             RW_RCE2  642	//RCE2-遥控执行继电器2
#define	             RW_RCE3  643	//RCE3-遥控执行继电器3
#define	             RW_RCE4  644	//RCE4-遥控执行继电器4
#define	             RW_RCE5  645	//RCE5-遥控执行继电器5
#define	             RW_RCE6  646	//RCE6-遥控执行继电器6
#define	             RW_RCE7  647	//RCE7-遥控执行继电器7
#define	             RW_RCE8  648	//RCE8-遥控执行继电器8
#define	             RW_RCE9  649	//RCE9-遥控执行继电器9
#define	            RW_RCE10  650	//RCE10-遥控执行继电器10
#define	            RW_RCE11  651	//RCE11-遥控执行继电器11
#define	            RW_RCE12  652	//RCE12-遥控执行继电器12
#define	            RW_RCE13  653	//RCE13-遥控执行继电器13
#define	            RW_RCE14  654	//RCE14-遥控执行继电器14
#define	            RW_RCE15  655	//RCE15-遥控执行继电器15
#define	            RW_RCE16  656	//RCE16-遥控执行继电器16
#define	            RW_RCE17  657	//RCE17-遥控执行继电器17
#define	            RW_RCE18  658	//RCE18-遥控执行继电器18
#define	            RW_RCE19  659	//RCE19-遥控执行继电器19
#define	            RW_RCE20  660	//RCE20-遥控执行继电器20
#define	            RW_RCE21  661	//RCE21-遥控执行继电器21
#define	            RW_RCE22  662	//RCE22-遥控执行继电器22
#define	            RW_RCE23  663	//RCE23-遥控执行继电器23
#define	            RW_RCE24  664	//RCE24-遥控执行继电器24
#define	            RW_RCE25  665	//RCE25-遥控执行继电器25
#define	            RW_RCE26  666	//RCE26-遥控执行继电器26
#define	            RW_RCE27  667	//RCE27-遥控执行继电器27
#define	            RW_RCE28  668	//RCE28-遥控执行继电器28
#define	            RW_RCE29  669	//RCE29-遥控执行继电器29
#define	            RW_RCE30  670	//RCE30-遥控执行继电器30
#define	            RW_RCE31  671	//RCE31-遥控执行继电器31
#define	            RW_RCE32  672	//RCE32-遥控执行继电器32
#define	            RW_RCE33  673	//RCE33-遥控执行继电器33
#define	            RW_RCE34  674	//RCE34-遥控执行继电器34
#define	            RW_RCE35  675	//RCE35-遥控执行继电器35
#define	            RW_RCE36  676	//RCE36-遥控执行继电器36
#define	            RW_RCE37  677	//RCE37-遥控执行继电器37
#define	            RW_RCE38  678	//RCE38-遥控执行继电器38
#define	            RW_RCE39  679	//RCE39-遥控执行继电器39
#define	            RW_RCE40  680	//RCE40-遥控执行继电器40
#define	            RW_RCE41  681	//RCE41-遥控执行继电器41
#define	            RW_RCE42  682	//RCE42-遥控执行继电器42
#define	            RW_RCE43  683	//RCE43-遥控执行继电器43
#define	            RW_RCE44  684	//RCE44-遥控执行继电器44
#define	             RW_RCV1  685	//RCV1-遥控反校继电器1
#define	             RW_RCV2  686	//RCV2-遥控反校继电器2
#define	             RW_RCV3  687	//RCV3-遥控反校继电器3
#define	             RW_RCV4  688	//RCV4-遥控反校继电器4
#define	             RW_RCV5  689	//RCV5-遥控反校继电器5
#define	             RW_RCV6  690	//RCV6-遥控反校继电器6
#define	             RW_RCV7  691	//RCV7-遥控反校继电器7
#define	             RW_RCV8  692	//RCV8-遥控反校继电器8
#define	             RW_RCV9  693	//RCV9-遥控反校继电器9
#define	            RW_RCV10  694	//RCV10-遥控反校继电器10
#define	            RW_RCV11  695	//RCV11-遥控反校继电器11
#define	            RW_RCV12  696	//RCV12-遥控反校继电器12
#define	            RW_RCV13  697	//RCV13-遥控反校继电器13
#define	            RW_RCV14  698	//RCV14-遥控反校继电器14
#define	            RW_RCV15  699	//RCV15-遥控反校继电器15
#define	            RW_RCV16  700	//RCV16-遥控反校继电器16
#define	            RW_RCV17  701	//RCV17-遥控反校继电器17
#define	            RW_RCV18  702	//RCV18-遥控反校继电器18
#define	            RW_RCV19  703	//RCV19-遥控反校继电器19
#define	            RW_RCV20  704	//RCV20-遥控反校继电器20
#define	            RW_RCV21  705	//RCV21-遥控反校继电器21
#define	            RW_RCV22  706	//RCV22-遥控反校继电器22
#define	            RW_RCV23  707	//RCV23-遥控反校继电器23
#define	            RW_RCV24  708	//RCV24-遥控反校继电器24
#define	            RW_RCV25  709	//RCV25-遥控反校继电器25
#define	            RW_RCV26  710	//RCV26-遥控反校继电器26
#define	            RW_RCV27  711	//RCV27-遥控反校继电器27
#define	            RW_RCV28  712	//RCV28-遥控反校继电器28
#define	            RW_RCV29  713	//RCV29-遥控反校继电器29
#define	            RW_RCV30  714	//RCV30-遥控反校继电器30
#define	            RW_RCV31  715	//RCV31-遥控反校继电器31
#define	            RW_RCV32  716	//RCV32-遥控反校继电器32
#define	            RW_RCV33  717	//RCV33-遥控反校继电器33
#define	            RW_RCV34  718	//RCV34-遥控反校继电器34
#define	            RW_RCV35  719	//RCV35-遥控反校继电器35
#define	            RW_RCV36  720	//RCV36-遥控反校继电器36
#define	            RW_RCV37  721	//RCV37-遥控反校继电器37
#define	            RW_RCV38  722	//RCV38-遥控反校继电器38
#define	            RW_RCV39  723	//RCV39-遥控反校继电器39
#define	            RW_RCV40  724	//RCV40-遥控反校继电器40
#define	            RW_RCV41  725	//RCV41-遥控反校继电器41
#define	            RW_RCV42  726	//RCV42-遥控反校继电器42
#define	            RW_RCV43  727	//RCV43-遥控反校继电器43
#define	            RW_RCV44  728	//RCV44-遥控反校继电器44
#define	              RW_GE1  729	//GE1-大于等于继电器1
#define	              RW_GE2  730	//GE2-大于等于继电器2
#define	              RW_GE3  731	//GE3-大于等于继电器3
#define	              RW_GE4  732	//GE4-大于等于继电器4
#define	              RW_GE5  733	//GE5-大于等于继电器5
#define	              RW_GE6  734	//GE6-大于等于继电器6
#define	              RW_GE7  735	//GE7-大于等于继电器7
#define	              RW_GE8  736	//GE8-大于等于继电器8
#define	           RW_CONST1  737	//CONST1-常数时间继电器1
#define	           RW_CONST2  738	//CONST2-常数时间继电器2
#define	           RW_CONST3  739	//CONST3-常数时间继电器3
#define	           RW_CONST4  740	//CONST4-常数时间继电器4
#define	           RW_CONST5  741	//CONST5-常数时间继电器5
#define	           RW_CONST6  742	//CONST6-常数时间继电器6
#define	           RW_CONST7  743	//CONST7-常数时间继电器7
#define	           RW_CONST8  744	//CONST8-常数时间继电器8
#define	              RW_TX1  745	//TX1-发送链接继电器字1
#define	              RW_TX2  746	//TX2-发送链接继电器字2
#define	              RW_TX3  747	//TX3-发送链接继电器字3
#define	              RW_TX4  748	//TX4-发送链接继电器字4
#define	              RW_TX5  749	//TX5-发送链接继电器字5
#define	              RW_TX6  750	//TX6-发送链接继电器字6
#define	              RW_TX7  751	//TX7-发送链接继电器字7
#define	              RW_TX8  752	//TX8-发送链接继电器字8
#define	              RW_TX9  753	//TX9-发送链接继电器字9
#define	             RW_TX10  754	//TX10-发送链接继电器字10
#define	             RW_TX11  755	//TX11-发送链接继电器字11
#define	             RW_TX12  756	//TX12-发送链接继电器字12
#define	             RW_TX13  757	//TX13-发送链接继电器字13
#define	             RW_TX14  758	//TX14-发送链接继电器字14
#define	             RW_TX15  759	//TX15-发送链接继电器字15
#define	             RW_TX16  760	//TX16-发送链接继电器字16
#define	              RW_RX1  761	//RX1-接收链接继电器字1
#define	              RW_RX2  762	//RX2-接收链接继电器字2
#define	              RW_RX3  763	//RX3-接收链接继电器字3
#define	              RW_RX4  764	//RX4-接收链接继电器字4
#define	              RW_RX5  765	//RX5-接收链接继电器字5
#define	              RW_RX6  766	//RX6-接收链接继电器字6
#define	              RW_RX7  767	//RX7-接收链接继电器字7
#define	              RW_RX8  768	//RX8-接收链接继电器字8
#define	              RW_RX9  769	//RX9-接收链接继电器字9
#define	             RW_RX10  770	//RX10-接收链接继电器字10
#define	             RW_RX11  771	//RX11-接收链接继电器字11
#define	             RW_RX12  772	//RX12-接收链接继电器字12
#define	             RW_RX13  773	//RX13-接收链接继电器字13
#define	             RW_RX14  774	//RX14-接收链接继电器字14
#define	             RW_RX15  775	//RX15-接收链接继电器字15
#define	             RW_RX16  776	//RX16-接收链接继电器字16
#define	             RW_CNT1  777	//CNT1-计数继电器字1
#define	             RW_CNT2  778	//CNT2-计数继电器字2
#define	             RW_CNT3  779	//CNT3-计数继电器字3
#define	             RW_CNT4  780	//CNT4-计数继电器字4
#define	             RW_CNT5  781	//CNT5-计数继电器字5
#define	             RW_CNT6  782	//CNT6-计数继电器字6
#define	             RW_CNT7  783	//CNT7-计数继电器字7
#define	             RW_CNT8  784	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			785		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			106	

//中间继电器字
#define	MID_RELAY_WORD_START			108	
#define	MID_RELAY_WORD_NUM			115	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			223	
#define	KEEP_RELAY_WORD_NUM			35	

//时间继电器字
#define	TIME_RELAY_WORD_START			258	
#define	TIME_RELAY_WORD_NUM			35	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			293	
#define	TRIP_RELAY_WORD_NUM			4	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			297	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			329	
#define	INPUT_RELAY_WORD_NUM			82	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			411	
#define	OUTPUT_RELAY_WORD_NUM			41	

//压板继电器
#define	SW_RELAY_WORD_START			452	
#define	SW_RELAY_WORD_NUM			11	

//配置继电器
#define	CFG_RELAY_WORD_START			463	
#define	CFG_RELAY_WORD_NUM			25	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			488	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			504	
#define	CTRLTEST_RELAY_WORD_NUM			21	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			525	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			541	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			565	
#define	VIN_RELAY_WORD_NUM			32	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			597	
#define	RCP_RELAY_WORD_NUM			44	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			641	
#define	RCE_RELAY_WORD_NUM			44	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			685	
#define	RCV_RELAY_WORD_NUM			44	

//大于等于继电器
#define	GE_RELAY_WORD_START			729	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			737	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			745	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			761	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			777	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			777	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			777	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			777	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			777	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			777	
#define	CNT_RELAY_WORD_NUM			8	


#endif