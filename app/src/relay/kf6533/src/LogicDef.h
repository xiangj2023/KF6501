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
#define	           RW_PI87UA    2	//PI87UA-A相差动速断保护入段继电器字
#define	          RW_LPC87UA    3	//LPC87UA-A相差动速断保护循环控制继电器字
#define	           RW_PI87RA    4	//PI87RA-A相比率差动保护入段继电器字
#define	          RW_LPC87RA    5	//LPC87RA-A相比率差动保护循环控制继电器字
#define	        RW_LK68IDH2A    6	//LK68IDH2A-差流A二次谐波闭锁继电器字
#define	        RW_LK68IRH2A    7	//LK68IRH2A-制动电流A二次谐波闭锁继电器字
#define	         RW_LK68IDHA    8	//LK68IDHA-差流A综合谐波闭锁继电器字
#define	           RW_PI87UB    9	//PI87UB-B相差动速断保护入段继电器字
#define	          RW_LPC87UB   10	//LPC87UB-B相差动速断保护循环控制继电器字
#define	           RW_PI87RB   11	//PI87RB-B相比率差动保护入段继电器字
#define	          RW_LPC87RB   12	//LPC87RB-B相比率差动保护循环控制继电器字
#define	        RW_LK68IDH2B   13	//LK68IDH2B-差流B二次谐波闭锁继电器字
#define	        RW_LK68IRH2B   14	//LK68IRH2B-制动电流B二次谐波闭锁继电器字
#define	         RW_LK68IDHB   15	//LK68IDHB-差流B综合谐波闭锁继电器字
#define	           RW_PI87UC   16	//PI87UC-C相差动速断保护入段继电器字
#define	          RW_LPC87UC   17	//LPC87UC-C相差动速断保护循环控制继电器字
#define	           RW_PI87RC   18	//PI87RC-C相比率差动入段继电器字
#define	          RW_LPC87RC   19	//LPC87RC-C相比率差动循环控制继电器字
#define	        RW_LK68IDH2C   20	//LK68IDH2C-C相差流二次谐波闭锁继电器字
#define	        RW_LK68IRH2C   21	//LK68IRH2C-C相制动电流二次谐波闭锁继电器字
#define	         RW_LK68IDHC   22	//LK68IDHC-差流C综合谐波闭锁继电器字
#define	          RW_PI50IH1   23	//PI50IH1-高压侧过流Ⅰ段入段继电器字
#define	          RW_TR50IH1   24	//TR50IH1-高压侧过流Ⅰ段动作继电器字
#define	         RW_LPC50IH1   25	//LPC50IH1-高压侧过流Ⅰ段循环控制继电器字
#define	          RW_PI50IH2   26	//PI50IH2-高压侧过流Ⅱ段入段继电器字
#define	          RW_TR50IH2   27	//TR50IH2-高压侧过流Ⅱ段动作继电器字
#define	         RW_LPC50IH2   28	//LPC50IH2-高压侧过流Ⅱ段循环控制继电器字
#define	          RW_PI50IH3   29	//PI50IH3-高压侧过流Ⅲ段入段继电器字
#define	          RW_TR50IH3   30	//TR50IH3-高压侧过流Ⅲ段动作继电器字
#define	         RW_LPC50IH3   31	//LPC50IH3-高压侧过流Ⅲ段循环控制继电器字
#define	          RW_PI50IL1   32	//PI50IL1-低压侧过流Ⅰ段入段继电器字
#define	          RW_TR50IL1   33	//TR50IL1-低压侧过流Ⅰ段动作继电器字
#define	         RW_LPC50IL1   34	//LPC50IL1-低压侧过流Ⅰ段循环控制继电器字
#define	          RW_PI50IL2   35	//PI50IL2-低压侧过流Ⅱ段入段继电器字
#define	          RW_TR50IL2   36	//TR50IL2-低压侧过流Ⅱ段动作继电器字
#define	         RW_LPC50IL2   37	//LPC50IL2-低压侧过流Ⅱ段循环控制继电器字
#define	             RW_47UH   38	//47UH-高压侧复合电压闭锁继电器字
#define	             RW_47UL   39	//47UL-低压侧复合电压闭锁继电器字
#define	          RW_PI50ISH   40	//PI50ISH-母充保护入段继电器字
#define	          RW_TR50ISH   41	//TR50ISH-母充保护动作继电器字
#define	         RW_LPC50ISH   42	//LPC50ISH-母充保护循环控制继电器字
#define	           RW_PI50N1   43	//PI50N1-零序过流Ⅰ段入段继电器字
#define	           RW_TR50N1   44	//TR50N1-零序过流Ⅰ段动作继电器字
#define	          RW_LPC50N1   45	//LPC50N1-零序过流Ⅰ段循环控制继电器字
#define	           RW_PI50N2   46	//PI50N2-零序过流Ⅱ段入段继电器字
#define	           RW_TR50N2   47	//TR50N2-零序过流Ⅱ段动作继电器字
#define	          RW_LPC50N2   48	//LPC50N2-零序过流Ⅱ段循环控制继电器字
#define	           RW_AR49IA   49	//AR49IA-A相反时限过负荷告警继电器字
#define	           RW_TR49IA   50	//TR49IA-A相反时限过负荷动作继电器字
#define	           RW_AR49IB   51	//AR49IB-B相反时限过负荷告警继电器字
#define	           RW_TR49IB   52	//TR49IB-B相反时限过负荷动作继电器字
#define	           RW_AR49IC   53	//AR49IC-C相反时限过负荷告警继电器字
#define	           RW_TR49IC   54	//TR49IC-C相反时限过负荷动作继电器字
#define	           RW_PI59NH   55	//PI59NH-高压侧零序过压入段继电器字
#define	           RW_TR59NH   56	//TR59NH-高压侧零序过压动作继电器字
#define	          RW_LPC59NH   57	//LPC59NH-高压侧零序过压循环控制继电器字
#define	           RW_PI59NL   58	//PI59NL-低压侧零序过压入段继电器字
#define	           RW_TR59NL   59	//TR59NL-低压侧零序过压动作继电器字
#define	          RW_LPC59NL   60	//LPC59NL-低压侧零序过压循环控制继电器字
#define	          RW_PI50OL1   61	//PI50OL1-过负荷Ⅰ段入段继电器字
#define	          RW_TR50OL1   62	//TR50OL1-过负荷Ⅰ段动作继电器字
#define	         RW_LPC50OL1   63	//LPC50OL1-过负荷Ⅰ段循环控制继电器字
#define	          RW_PI50OL2   64	//PI50OL2-过负荷Ⅱ段入段继电器字
#define	          RW_TR50OL2   65	//TR50OL2-过负荷Ⅱ段动作继电器字
#define	         RW_LPC50OL2   66	//LPC50OL2-过负荷Ⅱ段循环控制继电器字
#define	          RW_PI50OL3   67	//PI50OL3-过负荷Ⅲ段入段继电器字
#define	          RW_TR50OL3   68	//TR50OL3-过负荷Ⅲ段动作继电器字
#define	         RW_LPC50OL3   69	//LPC50OL3-过负荷Ⅲ段循环控制继电器字
#define	          RW_PI50IJX   70	//PI50IJX-间隙过流入段继电器字
#define	          RW_TR50IJX   71	//TR50IJX-间隙过流动作继电器字
#define	         RW_LPC50IJX   72	//LPC50IJX-间隙过流循环控制继电器字
#define	            RW_PI27U   73	//PI27U-失压保护入段继电器字
#define	            RW_TR27U   74	//TR27U-失压保护动作继电器字
#define	           RW_LPC27U   75	//LPC27U-失压保护循环控制继电器字
#define	              RW_PTH   76	//PTH-高压侧PT断线继电器字
#define	              RW_PTL   77	//PTL-低压侧PT断线继电器字
#define	            RW_59UHA   78	//59UHA-UA有压继电器字
#define	            RW_27UHA   79	//27UHA-UA无压继电器字
#define	            RW_59UHB   80	//59UHB-UB有压继电器字
#define	            RW_27UHB   81	//27UHB-UB无压继电器字
#define	            RW_59UHC   82	//59UHC-UC有压继电器字
#define	            RW_27UHC   83	//27UHC-UC无压继电器字
#define	            RW_59U0H   84	//59U0H-U0h有压继电器字
#define	            RW_27U0H   85	//27U0H-U0h无压继电器字
#define	            RW_59ULA   86	//59ULA-Ua有压继电器字
#define	            RW_27ULA   87	//27ULA-Ua无压继电器字
#define	            RW_59ULB   88	//59ULB-Ub有压继电器字
#define	            RW_27ULB   89	//27ULB-Ub无压继电器字
#define	            RW_59ULC   90	//59ULC-Uc有压继电器字
#define	            RW_27ULC   91	//27ULC-Uc无压继电器字
#define	            RW_59U0L   92	//59U0L-U0l有压继电器字
#define	            RW_27U0L   93	//27U0L-U0l无压继电器字
#define	            RW_50IHA   94	//50IHA-IA有流继电器字
#define	            RW_37IHA   95	//37IHA-IA无流继电器字
#define	            RW_50IHB   96	//50IHB-IB有流继电器字
#define	            RW_37IHB   97	//37IHB-IB无流继电器字
#define	            RW_50IHC   98	//50IHC-IC有流继电器字
#define	            RW_37IHC   99	//37IHC-IC无流继电器字
#define	            RW_50ILA  100	//50ILA-Ia有流继电器字
#define	            RW_37ILA  101	//37ILA-Ia无流继电器字
#define	            RW_50ILB  102	//50ILB-Ib有流继电器字
#define	            RW_37ILB  103	//37ILB-Ib无流继电器字
#define	            RW_50ILC  104	//50ILC-Ic有流继电器字
#define	            RW_37ILC  105	//37ILC-Ic无流继电器字
#define	             RW_50I0  106	//50I0-I0有流继电器字
#define	             RW_37I0  107	//37I0-I0无流继电器字
#define	            RW_50IJX  108	//50IJX-Ijx有流继电器字
#define	            RW_37IJX  109	//37IJX-Ijx无流继电器字
#define	             RW_PFHA  110	//PFHA-高侧A相功率正向继电器字
#define	             RW_PRHA  111	//PRHA-高侧A相功率反向继电器字
#define	             RW_PFHB  112	//PFHB-高侧B相功率正向继电器字
#define	             RW_PRHB  113	//PRHB-高侧B相功率反向继电器字
#define	             RW_PFHC  114	//PFHC-高侧C相功率正向继电器字
#define	             RW_PRHC  115	//PRHC-高侧C相功率反向继电器字
#define	             RW_PFLA  116	//PFLA-低侧A相功率正向继电器字
#define	             RW_PRLA  117	//PRLA-低侧A相功率反向继电器字
#define	             RW_PFLB  118	//PFLB-低侧B相功率正向继电器字
#define	             RW_PRLB  119	//PRLB-低侧B相功率反向继电器字
#define	             RW_PFLC  120	//PFLC-低侧C相功率正向继电器字
#define	             RW_PRLC  121	//PRLC-低侧C相功率反向继电器字
#define	           RW_PICKUP  122	//PICKUP-保护启动继电器字
#define	        RW_LK68IDAH2  123	//LK68IDAH2-差流A二次谐波闭锁继电器字
#define	        RW_LK68IDBH2  124	//LK68IDBH2-差流B二次谐波闭锁继电器字
#define	        RW_LK68IDCH2  125	//LK68IDCH2-差流C二次谐波闭锁继电器字
#define	         RW_LK68IDAH  126	//LK68IDAH-差流A综合谐波闭锁继电器字
#define	         RW_LK68IDBH  127	//LK68IDBH-差流B综合谐波闭锁继电器字
#define	         RW_LK68IDCH  128	//LK68IDCH-差流C综合谐波闭锁继电器字
#define	        RW_LK68IRAH2  129	//LK68IRAH2-制动电流A二次谐波闭锁继电器字
#define	        RW_LK68IRBH2  130	//LK68IRBH2-制动电流B二次谐波闭锁继电器字
#define	        RW_LK68IRCH2  131	//LK68IRCH2-制动电流C二次谐波闭锁继电器字
#define	             RW_AR87  132	//AR87-差流越限告警继电器字
#define	           RW_TR87UA  133	//TR87UA-差流A速断动作继电器字
#define	           RW_TR87RA  134	//TR87RA-比率差动A动作继电器字
#define	           RW_TR87UB  135	//TR87UB-差速B动作继电器字
#define	           RW_TR87RB  136	//TR87RB-比率差动B动作继电器字
#define	           RW_TR87UC  137	//TR87UC-差流C动作继电器字
#define	           RW_TR87RC  138	//TR87RC-比率差动C动作继电器字
#define	          RW_ET11ERR  139	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR  140	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR  141	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR  142	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR  143	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR  144	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR  145	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR  146	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR  147	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR  148	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR  149	//COM1ERR-串口连接1状态继电器字
#define	          RW_CAN1ERR  150	//CAN1ERR-CAN1连接状态继电器字
#define	          RW_CAN2ERR  151	//CAN2ERR-CAN2连接状态继电器字
#define	         RW_NULL1ERR  152	//NULL1ERR-空连接1状态继电器字
#define	         RW_NULL2ERR  153	//NULL2ERR-空连接2状态继电器字
#define	         RW_NULL3ERR  154	//NULL3ERR-空连接3状态继电器字
#define	        RW_MAINTLINK  155	//MAINTLINK-维护状态继电器字
#define	          RW_PI50OLA  156	//PI50OLA-过负荷告警入段继电器字
#define	          RW_TR50OLA  157	//TR50OLA-过负荷告警动作继电器字
#define	         RW_LPC50OLA  158	//LPC50OLA-过负荷告警循环控制继电器字
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
#define	              RW_REC  223	//REC-录波继电器字
#define	         RW_LKPI87UA  224	//LKPI87UA-差动速断A入段闭锁继电器字
#define	        RW_LKRST87UA  225	//LKRST87UA-差动速断A返回闭锁继电器字
#define	         RW_LKPI87UB  226	//LKPI87UB-差动速断B入段闭锁继电器字
#define	        RW_LKRST87UB  227	//LKRST87UB-差动速断B返回闭锁继电器字
#define	         RW_LKPI87UC  228	//LKPI87UC-差动速断C入段闭锁继电器字
#define	        RW_LKRST87UC  229	//LKRST87UC-差动速断C返回闭锁继电器字
#define	         RW_LKPI87RA  230	//LKPI87RA-比率差动A入段闭锁继电器字
#define	        RW_LKRST87RA  231	//LKRST87RA-比率差动A返回闭锁继电器字
#define	         RW_LKPI87RB  232	//LKPI87RB-比率差动B入段闭锁继电器字
#define	        RW_LKRST87RB  233	//LKRST87RB-比率差动B返回闭锁继电器字
#define	         RW_LKPI87RC  234	//LKPI87RC-比率差动C入段闭锁继电器字
#define	        RW_LKRST87RC  235	//LKRST87RC-比率差动C返回闭锁继电器字
#define	        RW_LKPI50IH1  236	//LKPI50IH1-高压侧过流Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50IH1  237	//LKRST50IH1-高压侧过流Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50IH2  238	//LKPI50IH2-高压侧过流Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50IH2  239	//LKRST50IH2-高压侧过流Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50IH3  240	//LKPI50IH3-高压侧过流Ⅲ段入段闭锁继电器字
#define	       RW_LKRST50IH3  241	//LKRST50IH3-高压侧过流Ⅲ段返回闭锁继电器字
#define	        RW_LKPI50IL1  242	//LKPI50IL1-低压侧过流Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50IL1  243	//LKRST50IL1-低压侧过流Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50IL2  244	//LKPI50IL2-低压侧过流Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50IL2  245	//LKRST50IL2-低压侧过流Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50ISH  246	//LKPI50ISH-母充保护入段闭锁继电器字
#define	       RW_LKRST50ISH  247	//LKRST50ISH-母充保护返回闭锁继电器字
#define	         RW_LKPI50N1  248	//LKPI50N1-零流Ⅰ段入段闭锁继电器字
#define	        RW_LKRST50N1  249	//LKRST50N1-零流Ⅰ段返回闭锁继电器字
#define	         RW_LKPI50N2  250	//LKPI50N2-零流Ⅱ段入段闭锁继电器字
#define	        RW_LKRST50N2  251	//LKRST50N2-零流Ⅱ段返回闭锁继电器字
#define	         RW_LKPI59NH  252	//LKPI59NH-高压侧零压过压入段闭锁继电器字
#define	        RW_LKRST59NH  253	//LKRST59NH-高压侧零压过压返回闭锁继电器字
#define	         RW_LKPI59NL  254	//LKPI59NL-低压侧零压过压入段闭锁继电器字
#define	        RW_LKRST59NL  255	//LKRST59NL-低压侧零压过压返回闭锁继电器字
#define	        RW_LKPI50OL1  256	//LKPI50OL1-过负荷Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50OL1  257	//LKRST50OL1-过负荷Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50OL2  258	//LKPI50OL2-过负荷Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50OL2  259	//LKRST50OL2-过负荷Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50OL3  260	//LKPI50OL3-过负荷Ⅲ段入段闭锁继电器字
#define	       RW_LKRST50OL3  261	//LKRST50OL3-过负荷Ⅲ段返回闭锁继电器字
#define	        RW_LKPI50IJX  262	//LKPI50IJX-间隙过流入段闭锁继电器字
#define	       RW_LKRST50IJX  263	//LKRST50IJX-间隙过流返回闭锁继电器字
#define	          RW_LKPI27U  264	//LKPI27U-失压保护入段闭锁继电器字
#define	         RW_LKRST27U  265	//LKRST27U-失压保护返回闭锁继电器字
#define	             RW_69IN  266	//69IN-当地控制继电器字
#define	            RW_52A_A  267	//52A_A-A断路器合位
#define	            RW_52B_A  268	//52B_A-A断路器分位
#define	            RW_52A_B  269	//52A_B-B断路器合位
#define	            RW_52B_B  270	//52B_B-B断路器分位
#define	             RW_LKCA  271	//LKCA-A断路器合闸闭锁继电器字
#define	             RW_LKTA  272	//LKTA-A断路器分闸闭锁继电器字
#define	             RW_LKCB  273	//LKCB-B断路器合闸闭锁继电器字
#define	             RW_LKTB  274	//LKTB-B断路器分闸闭锁继电器字
#define	        RW_LKPI50OLA  275	//LKPI50OLA-过负荷告警入段闭锁继电器字
#define	       RW_LKRST50OLA  276	//LKRST50OLA-过负荷告警返回闭锁继电器字
#define	               RW_H1  277	//H1-自保持中间继电器字1
#define	               RW_H2  278	//H2-自保持中间继电器字2
#define	               RW_H3  279	//H3-自保持中间继电器字3
#define	               RW_H4  280	//H4-自保持中间继电器字4
#define	               RW_H5  281	//H5-自保持中间继电器字5
#define	               RW_H6  282	//H6-自保持中间继电器字6
#define	               RW_H7  283	//H7-自保持中间继电器字7
#define	               RW_H8  284	//H8-自保持中间继电器字8
#define	               RW_H9  285	//H9-自保持中间继电器字9
#define	              RW_H10  286	//H10-自保持中间继电器字10
#define	              RW_H11  287	//H11-自保持中间继电器字11
#define	              RW_H12  288	//H12-自保持中间继电器字12
#define	              RW_H13  289	//H13-自保持中间继电器字13
#define	              RW_H14  290	//H14-自保持中间继电器字14
#define	              RW_H15  291	//H15-自保持中间继电器字15
#define	              RW_H16  292	//H16-自保持中间继电器字16
#define	              RW_H17  293	//H17-自保持中间继电器字17
#define	              RW_H18  294	//H18-自保持中间继电器字18
#define	              RW_H19  295	//H19-自保持中间继电器字19
#define	              RW_H20  296	//H20-自保持中间继电器字20
#define	              RW_H21  297	//H21-自保持中间继电器字21
#define	              RW_H22  298	//H22-自保持中间继电器字22
#define	              RW_H23  299	//H23-自保持中间继电器字23
#define	              RW_H24  300	//H24-自保持中间继电器字24
#define	              RW_H25  301	//H25-自保持中间继电器字25
#define	              RW_H26  302	//H26-自保持中间继电器字26
#define	              RW_H27  303	//H27-自保持中间继电器字27
#define	              RW_H28  304	//H28-自保持中间继电器字28
#define	              RW_H29  305	//H29-自保持中间继电器字29
#define	              RW_H30  306	//H30-自保持中间继电器字30
#define	              RW_H31  307	//H31-自保持中间继电器字31
#define	              RW_H32  308	//H32-自保持中间继电器字31
#define	            RW_52BFA  309	//52BFA-A断路器分闸失败继电器字
#define	            RW_52BFB  310	//52BFB-A断路器分闸失败继电器字
#define	               RW_T1  311	//T1-T1
#define	               RW_T2  312	//T2-T2
#define	               RW_T3  313	//T3-T3
#define	               RW_T4  314	//T4-T4
#define	               RW_T5  315	//T5-T5
#define	               RW_T6  316	//T6-T6
#define	               RW_T7  317	//T7-T7
#define	               RW_T8  318	//T8-T8
#define	               RW_T9  319	//T9-T9
#define	              RW_T10  320	//T10-T10
#define	              RW_T11  321	//T11-T11
#define	              RW_T12  322	//T12-T12
#define	              RW_T13  323	//T13-T13
#define	              RW_T14  324	//T14-T14
#define	              RW_T15  325	//T15-T15
#define	              RW_T16  326	//T16-T16
#define	             RW_TSW1  327	//TSW1-TSW1
#define	             RW_TSW2  328	//TSW2-TSW2
#define	             RW_TSW3  329	//TSW3-TSW3
#define	             RW_TSW4  330	//TSW4-TSW4
#define	             RW_TSW5  331	//TSW5-TSW5
#define	             RW_TSW6  332	//TSW6-TSW6
#define	             RW_TSW7  333	//TSW7-TSW7
#define	             RW_TSW8  334	//TSW8-TSW8
#define	             RW_TCB1  335	//TCB1-TCB1
#define	             RW_TCB2  336	//TCB2-TCB2
#define	             RW_TCB3  337	//TCB3-TCB3
#define	             RW_TCB4  338	//TCB4-TCB4
#define	             RW_TCB5  339	//TCB5-TCB5
#define	             RW_TCB6  340	//TCB6-TCB6
#define	             RW_TCB7  341	//TCB7-TCB7
#define	             RW_TCB8  342	//TCB8-TCB8
#define	              RW_TSH  343	//TSH-TSH
#define	            RW_T52BF  344	//T52BF-T52BF
#define	              RW_TR1  345	//TR1-跳闸继电器1
#define	              RW_TR2  346	//TR2-跳闸继电器2
#define	              RW_TR3  347	//TR3-跳闸继电器3
#define	              RW_TR4  348	//TR4-跳闸继电器4
#define	              RW_VR1  349	//VR1-调试继电器
#define	              RW_VR2  350	//VR2-虚拟继电器2
#define	              RW_VR3  351	//VR3-VR3
#define	              RW_VR4  352	//VR4-VR4
#define	              RW_VR5  353	//VR5-VR5
#define	              RW_VR6  354	//VR6-VR6
#define	              RW_VR7  355	//VR7-VR7
#define	              RW_VR8  356	//VR8-VR8
#define	              RW_VR9  357	//VR9-VR9
#define	             RW_VR10  358	//VR10-VR10
#define	             RW_VR11  359	//VR11-VR11
#define	             RW_VR12  360	//VR12-VR12
#define	             RW_VR13  361	//VR13-VR13
#define	             RW_VR14  362	//VR14-VR14
#define	             RW_VR15  363	//VR15-VR15
#define	             RW_VR16  364	//VR16-VR16
#define	             RW_VR17  365	//VR17-VR17
#define	             RW_VR18  366	//VR18-VR18
#define	             RW_VR19  367	//VR19-VR19
#define	             RW_VR20  368	//VR20-VR20
#define	             RW_VR21  369	//VR21-VR21
#define	             RW_VR22  370	//VR22-VR22
#define	             RW_VR23  371	//VR23-VR23
#define	             RW_VR24  372	//VR24-VR24
#define	             RW_VR25  373	//VR25-VR25
#define	             RW_VR26  374	//VR26-VR26
#define	             RW_VR27  375	//VR27-VR27
#define	             RW_VR28  376	//VR28-VR28
#define	             RW_VR29  377	//VR29-VR29
#define	             RW_VR30  378	//VR30-VR30
#define	             RW_VR31  379	//VR31-VR31
#define	             RW_VR32  380	//VR32-VR32
#define	              RW_IN1  381	//IN1-IN1
#define	              RW_IN2  382	//IN2-IN2
#define	              RW_IN3  383	//IN3-IN3
#define	              RW_IN4  384	//IN4-IN4
#define	              RW_IN5  385	//IN5-IN5
#define	              RW_IN6  386	//IN6-IN6
#define	              RW_IN7  387	//IN7-IN7
#define	              RW_IN8  388	//IN8-IN8
#define	              RW_IN9  389	//IN9-IN9
#define	             RW_IN10  390	//IN10-IN10
#define	             RW_IN11  391	//IN11-IN11
#define	             RW_IN12  392	//IN12-IN12
#define	             RW_IN13  393	//IN13-IN13
#define	             RW_IN14  394	//IN14-IN14
#define	             RW_IN15  395	//IN15-IN15
#define	             RW_IN16  396	//IN16-IN16
#define	             RW_IN17  397	//IN17-IN17
#define	             RW_IN18  398	//IN18-IN18
#define	             RW_IN19  399	//IN19-IN19
#define	             RW_IN20  400	//IN20-IN20
#define	             RW_IN21  401	//IN21-IN21
#define	             RW_IN22  402	//IN22-IN22
#define	             RW_IN23  403	//IN23-IN23
#define	             RW_IN24  404	//IN24-IN24
#define	             RW_IN25  405	//IN25-IN25
#define	             RW_IN26  406	//IN26-IN26
#define	             RW_IN27  407	//IN27-IN27
#define	             RW_IN28  408	//IN28-IN28
#define	             RW_IN29  409	//IN29-IN29
#define	             RW_IN30  410	//IN30-IN30
#define	             RW_IN31  411	//IN31-IN31
#define	             RW_IN32  412	//IN32-IN32
#define	             RW_IN33  413	//IN33-IN33
#define	             RW_IN34  414	//IN34-IN34
#define	             RW_IN35  415	//IN35-信号复归
#define	             RW_IN36  416	//IN36-出口板1跳闸位置
#define	             RW_IN37  417	//IN37-出口板1合闸位置
#define	             RW_IN38  418	//IN38-出口板1遥控预置
#define	             RW_IN39  419	//IN39-出口板1遥控1预置
#define	             RW_IN40  420	//IN40-出口板1遥控2预置
#define	             RW_IN41  421	//IN41-出口板2跳闸位置
#define	             RW_IN42  422	//IN42-出口板2合闸位置
#define	             RW_IN43  423	//IN43-出口板2遥控预置
#define	             RW_IN44  424	//IN44-出口板2遥控1预置
#define	             RW_IN45  425	//IN45-出口板2遥控2预置
#define	             RW_IN46  426	//IN46-重瓦斯
#define	             RW_IN47  427	//IN47-压力释放
#define	             RW_IN48  428	//IN48-超温跳闸
#define	             RW_IN49  429	//IN49-轻瓦斯
#define	             RW_IN50  430	//IN50-油位异常
#define	             RW_IN51  431	//IN51-温度过高
#define	             RW_IN52  432	//IN52-失电告警
#define	           RW_INVIN1  433	//INVIN1-VIN1
#define	           RW_INVIN2  434	//INVIN2-VIN2
#define	           RW_INVIN3  435	//INVIN3-VIN3
#define	           RW_INVIN4  436	//INVIN4-VIN4
#define	           RW_INVIN5  437	//INVIN5-VIN5
#define	           RW_INVIN6  438	//INVIN6-VIN6
#define	           RW_INVIN7  439	//INVIN7-VIN7
#define	           RW_INVIN8  440	//INVIN8-VIN8
#define	           RW_INVIN9  441	//INVIN9-VIN9
#define	          RW_INVIN10  442	//INVIN10-VIN10
#define	          RW_INVIN11  443	//INVIN11-VIN11
#define	          RW_INVIN12  444	//INVIN12-VIN12
#define	          RW_INVIN13  445	//INVIN13-VIN13
#define	          RW_INVIN14  446	//INVIN14-VIN14
#define	          RW_INVIN15  447	//INVIN15-VIN15
#define	          RW_INVIN16  448	//INVIN16-VIN16
#define	             RW_OUT1  449	//OUT1-开出1继电器字
#define	             RW_OUT2  450	//OUT2-开出2继电器字
#define	             RW_OUT3  451	//OUT3-开出3继电器字
#define	             RW_OUT4  452	//OUT4-开出4继电器字
#define	             RW_OUT5  453	//OUT5-开出5继电器字
#define	             RW_OUT6  454	//OUT6-开出6继电器字
#define	             RW_OUT7  455	//OUT7-开出7继电器字
#define	             RW_OUT8  456	//OUT8-开出8继电器字
#define	             RW_OUT9  457	//OUT9-开出9继电器字
#define	            RW_OUT10  458	//OUT10-开出10继电器字
#define	            RW_OUT11  459	//OUT11-开出11继电器字
#define	            RW_OUT12  460	//OUT12-开出12继电器字
#define	            RW_OUT13  461	//OUT13-开出13继电器字
#define	            RW_OUT14  462	//OUT14-开出14继电器字
#define	            RW_OUT15  463	//OUT15-开出15继电器字
#define	            RW_OUT16  464	//OUT16-开出16继电器字
#define	            RW_OUT17  465	//OUT17-开出17继电器字
#define	            RW_OUT18  466	//OUT18-开出18继电器字
#define	            RW_OUT19  467	//OUT19-开出19继电器字
#define	            RW_OUT20  468	//OUT20-开出20继电器字
#define	            RW_OUT21  469	//OUT21-开出21继电器字
#define	            RW_OUT22  470	//OUT22-开出22继电器字
#define	            RW_OUT23  471	//OUT23-开出23继电器字
#define	            RW_OUT24  472	//OUT24-开出24继电器字
#define	            RW_OUT25  473	//OUT25-开出25继电器字
#define	            RW_OUT26  474	//OUT26-开出26继电器字
#define	            RW_OUT27  475	//OUT27-开出27继电器字
#define	            RW_OUT28  476	//OUT28-开出28继电器字
#define	            RW_OUT29  477	//OUT29-开出29继电器字
#define	            RW_OUT30  478	//OUT30-开出30继电器字
#define	            RW_OUT31  479	//OUT31-开出31继电器字
#define	            RW_OUT32  480	//OUT32-开出32继电器字
#define	            RW_OUT33  481	//OUT33-开出33继电器字
#define	            RW_SL87U  482	//SL87U-差动速断压板继电器字
#define	            RW_SL87R  483	//SL87R-比率差动压板继电器字
#define	          RW_SL50I_H  484	//SL50I_H-高压侧过流压板继电器字
#define	          RW_SL50I_L  485	//SL50I_L-低压侧过流压板继电器字
#define	         RW_SL50I_SH  486	//SL50I_SH-母充保护压板继电器字
#define	            RW_SL50N  487	//SL50N-零序过流压板继电器字
#define	            RW_SL59N  488	//SL59N-零序过压压板继电器字
#define	           RW_SL50OL  489	//SL50OL-过负荷压板继电器字
#define	         RW_SL50I_JX  490	//SL50I_JX-间隙过流压板继电器字
#define	            RW_SL49I  491	//SL49I-反时限过负荷压板继电器字
#define	            RW_SL27U  492	//SL27U-失压保护压板继电器字
#define	          RW_SLMAINT  493	//SLMAINT-检修压板继电器字
#define	           RW_SLBAK1  494	//SLBAK1-备用1压板继电器字
#define	           RW_SLBAK2  495	//SLBAK2-备用2压板继电器字
#define	             RW_F87U  496	//F87U-差动速断保护
#define	             RW_F87R  497	//F87R-比率差动保护
#define	     RW_F68IDH2MAXLK  498	//F68IDH2MAXLK-二次谐波最大相闭锁
#define	     RW_F68IDH2MULLK  499	//F68IDH2MULLK-二次谐波综合相闭锁
#define	     RW_F68IDH2SPLLK  500	//F68IDH2SPLLK-二次谐波分相闭锁
#define	        RW_F68IRH2LK  501	//F68IRH2LK-制动电流二次谐波闭锁
#define	       RW_F68IDMULLK  502	//F68IDMULLK-综合谐波闭锁
#define	       RW_FHCT1CLOCK  503	//FHCT1CLOCK-高压侧CT1点校正
#define	      RW_FHCT11CLOCK  504	//FHCT11CLOCK-高压侧CT11点校正
#define	       RW_FLCT1CLOCK  505	//FLCT1CLOCK-低压侧CT1点校正
#define	      RW_FLCT11CLOCK  506	//FLCT11CLOCK-低压侧CT11点校正
#define	             RW_FHAB  507	//FHAB-高压侧两相
#define	          RW_F50I_H1  508	//F50I_H1-高压侧过流Ⅰ段
#define	       RW_F50I_H1_PD  509	//F50I_H1_PD-高压侧过流Ⅰ段功率方向闭锁
#define	      RW_F50I_H1_47U  510	//F50I_H1_47U-高压侧过流Ⅰ段复压闭锁
#define	          RW_F50I_H2  511	//F50I_H2-高压侧过流Ⅱ段
#define	       RW_F50I_H2_PD  512	//F50I_H2_PD-高压侧过流Ⅱ段功率方向
#define	      RW_F50I_H2_47U  513	//F50I_H2_47U-高压侧过流Ⅱ段复压闭锁
#define	          RW_F50I_H3  514	//F50I_H3-高压侧过流Ⅲ段
#define	       RW_F50I_H3_PD  515	//F50I_H3_PD-高压侧过流Ⅲ段功率方向
#define	      RW_F50I_H3_47U  516	//F50I_H3_47U-高压侧过流Ⅲ段复压闭锁
#define	          RW_F50I_L1  517	//F50I_L1-低压侧过流Ⅰ段
#define	       RW_F50I_L1_PD  518	//F50I_L1_PD-低压侧过流Ⅰ段功率方向
#define	      RW_F50I_L1_47U  519	//F50I_L1_47U-低压侧过流Ⅰ段复压闭锁
#define	          RW_F50I_L2  520	//F50I_L2-低压侧过流Ⅱ段
#define	       RW_F50I_L2_PD  521	//F50I_L2_PD-低压侧过流Ⅱ段功率方向
#define	      RW_F50I_L2_47U  522	//F50I_L2_47U-低压侧过流Ⅱ段复压闭锁
#define	          RW_F50I_SH  523	//F50I_SH-母线充电保护
#define	             RW_F49I  524	//F49I-反时限过负荷保护
#define	          RW_F50I_N1  525	//F50I_N1-零序过流Ⅰ段
#define	          RW_F50I_N2  526	//F50I_N2-零序过流Ⅱ段
#define	          RW_F59U_NH  527	//F59U_NH-高压侧零序过压保护
#define	          RW_F59U_NL  528	//F59U_NL-低压侧零序过压告警
#define	          RW_F50OL_1  529	//F50OL_1-过负荷Ⅰ段
#define	          RW_F50OL_2  530	//F50OL_2-过负荷Ⅱ段
#define	          RW_F50OL_3  531	//F50OL_3-过负荷Ⅲ段
#define	          RW_F50OL_A  532	//F50OL_A-过负荷告警
#define	          RW_F50I_JX  533	//F50I_JX-间隙过流保护
#define	             RW_F27U  534	//F27U-三相失压保护
#define	         RW_F27U_50I  535	//F27U_50I-失压保护电流闭锁
#define	            RW_FPTCK  536	//FPTCK-PT断线检测
#define	            RW_F52BF  537	//F52BF-失灵保护
#define	            RW_FBAK1  538	//FBAK1-备用配置1
#define	            RW_FBAK2  539	//FBAK2-备用配置2
#define	              RW_PW1  540	//PW1-灯光1
#define	              RW_PW2  541	//PW2-灯光2
#define	              RW_PW3  542	//PW3-灯光3
#define	              RW_PW4  543	//PW4-灯光4
#define	              RW_PW5  544	//PW5-灯光5
#define	              RW_PW6  545	//PW6-灯光6
#define	              RW_PW7  546	//PW7-灯光7
#define	              RW_PW8  547	//PW8-灯光8
#define	              RW_PW9  548	//PW9-灯光9
#define	             RW_PW10  549	//PW10-灯光10
#define	             RW_PW11  550	//PW11-灯光11
#define	             RW_PW12  551	//PW12-灯光12
#define	             RW_PW13  552	//PW13-灯光13
#define	             RW_PW14  553	//PW14-灯光14
#define	             RW_PW15  554	//PW15-灯光15
#define	             RW_PW16  555	//PW16-灯光16
#define	              RW_TO1  556	//TO1-TO1
#define	              RW_TO2  557	//TO2-TO2
#define	              RW_TO3  558	//TO3-TO3
#define	              RW_TO4  559	//TO4-TO4
#define	              RW_TO5  560	//TO5-TO5
#define	              RW_TO6  561	//TO6-TO6
#define	              RW_TO7  562	//TO7-TO7
#define	              RW_TO8  563	//TO8-TO8
#define	              RW_TO9  564	//TO9-TO9
#define	             RW_TO10  565	//TO10-TO10
#define	             RW_TO11  566	//TO11-TO11
#define	             RW_TO12  567	//TO12-TO12
#define	             RW_TO13  568	//TO13-TO13
#define	             RW_TO14  569	//TO14-TO14
#define	             RW_TO15  570	//TO15-TO15
#define	             RW_TO16  571	//TO16-TO16
#define	             RW_TO17  572	//TO17-TO17
#define	             RW_TO18  573	//TO18-TO18
#define	             RW_TO19  574	//TO19-TO19
#define	             RW_SIG1  575	//SIG1-SIG1
#define	             RW_SIG2  576	//SIG2-SIG2
#define	             RW_SIG3  577	//SIG3-SIG3
#define	             RW_SIG4  578	//SIG4-SIG4
#define	             RW_SIG5  579	//SIG5-SIG5
#define	             RW_SIG6  580	//SIG6-SIG6
#define	             RW_SIG7  581	//SIG7-SIG7
#define	             RW_SIG8  582	//SIG8-SIG8
#define	             RW_SIG9  583	//SIG9-SIG9
#define	            RW_SIG10  584	//SIG10-SIG10
#define	            RW_SIG11  585	//SIG11-SIG11
#define	            RW_SIG12  586	//SIG12-SIG12
#define	            RW_SIG13  587	//SIG13-SIG13
#define	            RW_SIG14  588	//SIG14-SIG14
#define	            RW_SIG15  589	//SIG15-SIG15
#define	            RW_SIG16  590	//SIG16-SIG16
#define	            RW_LED1G  591	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  592	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  593	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  594	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  595	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  596	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  597	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  598	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  599	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  600	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  601	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  602	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  603	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  604	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  605	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  606	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  607	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  608	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  609	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  610	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  611	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  612	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  613	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  614	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  615	//VIN1-VIN1
#define	             RW_VIN2  616	//VIN2-VIN2
#define	             RW_VIN3  617	//VIN3-VIN3
#define	             RW_VIN4  618	//VIN4-VIN4
#define	             RW_VIN5  619	//VIN5-VIN5
#define	             RW_VIN6  620	//VIN6-VIN6
#define	             RW_VIN7  621	//VIN7-VIN7
#define	             RW_VIN8  622	//VIN8-VIN8
#define	             RW_VIN9  623	//VIN9-VIN9
#define	            RW_VIN10  624	//VIN10-VIN10
#define	            RW_VIN11  625	//VIN11-VIN11
#define	            RW_VIN12  626	//VIN12-VIN12
#define	            RW_VIN13  627	//VIN13-VIN13
#define	            RW_VIN14  628	//VIN14-VIN14
#define	            RW_VIN15  629	//VIN15-VIN15
#define	            RW_VIN16  630	//VIN16-VIN16
#define	             RW_RCP1  631	//RCP1-遥控预置继电器1
#define	             RW_RCP2  632	//RCP2-遥控预置继电器2
#define	             RW_RCP3  633	//RCP3-遥控预置继电器3
#define	             RW_RCP4  634	//RCP4-遥控预置继电器4
#define	             RW_RCP5  635	//RCP5-遥控预置继电器5
#define	             RW_RCP6  636	//RCP6-遥控预置继电器6
#define	             RW_RCP7  637	//RCP7-遥控预置继电器7
#define	             RW_RCP8  638	//RCP8-遥控预置继电器8
#define	             RW_RCP9  639	//RCP9-遥控预置继电器9
#define	            RW_RCP10  640	//RCP10-遥控预置继电器10
#define	            RW_RCP11  641	//RCP11-遥控预置继电器11
#define	            RW_RCP12  642	//RCP12-遥控预置继电器12
#define	            RW_RCP13  643	//RCP13-遥控预置继电器13
#define	            RW_RCP14  644	//RCP14-遥控预置继电器14
#define	            RW_RCP15  645	//RCP15-遥控预置继电器15
#define	            RW_RCP16  646	//RCP16-遥控预置继电器16
#define	            RW_RCP17  647	//RCP17-遥控预置继电器17
#define	            RW_RCP18  648	//RCP18-遥控预置继电器18
#define	            RW_RCP19  649	//RCP19-遥控预置继电器19
#define	            RW_RCP20  650	//RCP20-遥控预置继电器20
#define	            RW_RCP21  651	//RCP21-遥控预置继电器21
#define	            RW_RCP22  652	//RCP22-遥控预置继电器22
#define	            RW_RCP23  653	//RCP23-遥控预置继电器23
#define	            RW_RCP24  654	//RCP24-遥控预置继电器24
#define	            RW_RCP25  655	//RCP25-遥控预置继电器25
#define	            RW_RCP26  656	//RCP26-遥控预置继电器26
#define	            RW_RCP27  657	//RCP27-遥控预置继电器27
#define	            RW_RCP28  658	//RCP28-遥控预置继电器28
#define	            RW_RCP29  659	//RCP29-遥控预置继电器29
#define	            RW_RCP30  660	//RCP30-遥控预置继电器30
#define	            RW_RCP31  661	//RCP31-遥控预置继电器31
#define	            RW_RCP32  662	//RCP32-遥控预置继电器32
#define	            RW_RCP33  663	//RCP33-遥控预置继电器33
#define	            RW_RCP34  664	//RCP34-遥控预置继电器34
#define	            RW_RCP35  665	//RCP35-遥控预置继电器35
#define	            RW_RCP36  666	//RCP36-遥控预置继电器36
#define	            RW_RCP37  667	//RCP37-遥控预置继电器37
#define	            RW_RCP38  668	//RCP38-遥控预置继电器38
#define	            RW_RCP39  669	//RCP39-遥控预置继电器39
#define	            RW_RCP40  670	//RCP40-遥控预置继电器40
#define	            RW_RCP41  671	//RCP41-遥控预置继电器41
#define	            RW_RCP42  672	//RCP42-遥控预置继电器42
#define	             RW_RCE1  673	//RCE1-遥控执行继电器1
#define	             RW_RCE2  674	//RCE2-遥控执行继电器2
#define	             RW_RCE3  675	//RCE3-遥控执行继电器3
#define	             RW_RCE4  676	//RCE4-遥控执行继电器4
#define	             RW_RCE5  677	//RCE5-遥控执行继电器5
#define	             RW_RCE6  678	//RCE6-遥控执行继电器6
#define	             RW_RCE7  679	//RCE7-遥控执行继电器7
#define	             RW_RCE8  680	//RCE8-遥控执行继电器8
#define	             RW_RCE9  681	//RCE9-遥控执行继电器9
#define	            RW_RCE10  682	//RCE10-遥控执行继电器10
#define	            RW_RCE11  683	//RCE11-遥控执行继电器11
#define	            RW_RCE12  684	//RCE12-遥控执行继电器12
#define	            RW_RCE13  685	//RCE13-遥控执行继电器13
#define	            RW_RCE14  686	//RCE14-遥控执行继电器14
#define	            RW_RCE15  687	//RCE15-遥控执行继电器15
#define	            RW_RCE16  688	//RCE16-遥控执行继电器16
#define	            RW_RCE17  689	//RCE17-遥控执行继电器17
#define	            RW_RCE18  690	//RCE18-遥控执行继电器18
#define	            RW_RCE19  691	//RCE19-遥控执行继电器19
#define	            RW_RCE20  692	//RCE20-遥控执行继电器20
#define	            RW_RCE21  693	//RCE21-遥控执行继电器21
#define	            RW_RCE22  694	//RCE22-遥控执行继电器22
#define	            RW_RCE23  695	//RCE23-遥控执行继电器23
#define	            RW_RCE24  696	//RCE24-遥控执行继电器24
#define	            RW_RCE25  697	//RCE25-遥控执行继电器25
#define	            RW_RCE26  698	//RCE26-遥控执行继电器26
#define	            RW_RCE27  699	//RCE27-遥控执行继电器27
#define	            RW_RCE28  700	//RCE28-遥控执行继电器28
#define	            RW_RCE29  701	//RCE29-遥控执行继电器29
#define	            RW_RCE30  702	//RCE30-遥控执行继电器30
#define	            RW_RCE31  703	//RCE31-遥控执行继电器31
#define	            RW_RCE32  704	//RCE32-遥控执行继电器32
#define	            RW_RCE33  705	//RCE33-遥控执行继电器33
#define	            RW_RCE34  706	//RCE34-遥控执行继电器34
#define	            RW_RCE35  707	//RCE35-遥控执行继电器35
#define	            RW_RCE36  708	//RCE36-遥控执行继电器36
#define	            RW_RCE37  709	//RCE37-遥控执行继电器37
#define	            RW_RCE38  710	//RCE38-遥控执行继电器38
#define	            RW_RCE39  711	//RCE39-遥控执行继电器39
#define	            RW_RCE40  712	//RCE40-遥控执行继电器40
#define	            RW_RCE41  713	//RCE41-遥控执行继电器41
#define	            RW_RCE42  714	//RCE42-遥控执行继电器42
#define	             RW_RCV1  715	//RCV1-遥控反校继电器1
#define	             RW_RCV2  716	//RCV2-遥控反校继电器2
#define	             RW_RCV3  717	//RCV3-遥控反校继电器3
#define	             RW_RCV4  718	//RCV4-遥控反校继电器4
#define	             RW_RCV5  719	//RCV5-遥控反校继电器5
#define	             RW_RCV6  720	//RCV6-遥控反校继电器6
#define	             RW_RCV7  721	//RCV7-遥控反校继电器7
#define	             RW_RCV8  722	//RCV8-遥控反校继电器8
#define	             RW_RCV9  723	//RCV9-遥控反校继电器9
#define	            RW_RCV10  724	//RCV10-遥控反校继电器10
#define	            RW_RCV11  725	//RCV11-遥控反校继电器11
#define	            RW_RCV12  726	//RCV12-遥控反校继电器12
#define	            RW_RCV13  727	//RCV13-遥控反校继电器13
#define	            RW_RCV14  728	//RCV14-遥控反校继电器14
#define	            RW_RCV15  729	//RCV15-遥控反校继电器15
#define	            RW_RCV16  730	//RCV16-遥控反校继电器16
#define	            RW_RCV17  731	//RCV17-遥控反校继电器17
#define	            RW_RCV18  732	//RCV18-遥控反校继电器18
#define	            RW_RCV19  733	//RCV19-遥控反校继电器19
#define	            RW_RCV20  734	//RCV20-遥控反校继电器20
#define	            RW_RCV21  735	//RCV21-遥控反校继电器21
#define	            RW_RCV22  736	//RCV22-遥控反校继电器22
#define	            RW_RCV23  737	//RCV23-遥控反校继电器23
#define	            RW_RCV24  738	//RCV24-遥控反校继电器24
#define	            RW_RCV25  739	//RCV25-遥控反校继电器25
#define	            RW_RCV26  740	//RCV26-遥控反校继电器26
#define	            RW_RCV27  741	//RCV27-遥控反校继电器27
#define	            RW_RCV28  742	//RCV28-遥控反校继电器28
#define	            RW_RCV29  743	//RCV29-遥控反校继电器29
#define	            RW_RCV30  744	//RCV30-遥控反校继电器30
#define	            RW_RCV31  745	//RCV31-遥控反校继电器31
#define	            RW_RCV32  746	//RCV32-遥控反校继电器32
#define	            RW_RCV33  747	//RCV33-遥控反校继电器33
#define	            RW_RCV34  748	//RCV34-遥控反校继电器34
#define	            RW_RCV35  749	//RCV35-遥控反校继电器35
#define	            RW_RCV36  750	//RCV36-遥控反校继电器36
#define	            RW_RCV37  751	//RCV37-遥控反校继电器37
#define	            RW_RCV38  752	//RCV38-遥控反校继电器38
#define	            RW_RCV39  753	//RCV39-遥控反校继电器39
#define	            RW_RCV40  754	//RCV40-遥控反校继电器40
#define	            RW_RCV41  755	//RCV41-遥控反校继电器41
#define	            RW_RCV42  756	//RCV42-遥控反校继电器42
#define	              RW_GE1  757	//GE1-GE1
#define	              RW_GE2  758	//GE2-GE2
#define	              RW_GE3  759	//GE3-GE3
#define	              RW_GE4  760	//GE4-GE4
#define	              RW_GE5  761	//GE5-GE5
#define	              RW_GE6  762	//GE6-GE6
#define	              RW_GE7  763	//GE7-GE7
#define	              RW_GE8  764	//GE8-GE8
#define	           RW_CONST1  765	//CONST1-CONST1
#define	           RW_CONST2  766	//CONST2-CONST2
#define	           RW_CONST3  767	//CONST3-CONST3
#define	           RW_CONST4  768	//CONST4-CONST4
#define	           RW_CONST5  769	//CONST5-CONST5
#define	           RW_CONST6  770	//CONST6-CONST6
#define	           RW_CONST7  771	//CONST7-CONST7
#define	           RW_CONST8  772	//CONST8-CONST8
#define	             RW_TLR1  773	//TLR1-发送链接继电器字1
#define	             RW_TLR2  774	//TLR2-发送链接继电器字2
#define	             RW_TLR3  775	//TLR3-发送链接继电器字3
#define	             RW_TLR4  776	//TLR4-发送链接继电器字4
#define	             RW_TLR5  777	//TLR5-发送链接继电器字5
#define	             RW_TLR6  778	//TLR6-发送链接继电器字6
#define	             RW_TLR7  779	//TLR7-发送链接继电器字7
#define	             RW_TLR8  780	//TLR8-发送链接继电器字8
#define	             RW_TLR9  781	//TLR9-发送链接继电器字9
#define	            RW_TLR10  782	//TLR10-发送链接继电器字10
#define	            RW_TLR11  783	//TLR11-发送链接继电器字11
#define	            RW_TLR12  784	//TLR12-发送链接继电器字12
#define	            RW_TLR13  785	//TLR13-发送链接继电器字13
#define	            RW_TLR14  786	//TLR14-发送链接继电器字14
#define	            RW_TLR15  787	//TLR15-发送链接继电器字15
#define	            RW_TLR16  788	//TLR16-发送链接继电器字16
#define	             RW_RLR1  789	//RLR1-接收链接继电器字1
#define	             RW_RLR2  790	//RLR2-接收链接继电器字2
#define	             RW_RLR3  791	//RLR3-接收链接继电器字3
#define	             RW_RLR4  792	//RLR4-接收链接继电器字4
#define	             RW_RLR5  793	//RLR5-接收链接继电器字5
#define	             RW_RLR6  794	//RLR6-接收链接继电器字6
#define	             RW_RLR7  795	//RLR7-接收链接继电器字7
#define	             RW_RLR8  796	//RLR8-接收链接继电器字8
#define	             RW_RLR9  797	//RLR9-接收链接继电器字9
#define	            RW_RLR10  798	//RLR10-接收链接继电器字10
#define	            RW_RLR11  799	//RLR11-接收链接继电器字11
#define	            RW_RLR12  800	//RLR12-接收链接继电器字12
#define	            RW_RLR13  801	//RLR13-接收链接继电器字13
#define	            RW_RLR14  802	//RLR14-接收链接继电器字14
#define	            RW_RLR15  803	//RLR15-接收链接继电器字15
#define	            RW_RLR16  804	//RLR16-接收链接继电器字16
#define	             RW_CNT1  805	//CNT1-计数继电器字1
#define	             RW_CNT2  806	//CNT2-计数继电器字2
#define	             RW_CNT3  807	//CNT3-计数继电器字3
#define	             RW_CNT4  808	//CNT4-计数继电器字4
#define	             RW_CNT5  809	//CNT5-计数继电器字5
#define	             RW_CNT6  810	//CNT6-计数继电器字6
#define	             RW_CNT7  811	//CNT7-计数继电器字7
#define	             RW_CNT8  812	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			813		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			157	

//中间继电器字
#define	MID_RELAY_WORD_START			159	
#define	MID_RELAY_WORD_NUM			118	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			277	
#define	KEEP_RELAY_WORD_NUM			34	

//时间继电器字
#define	TIME_RELAY_WORD_START			311	
#define	TIME_RELAY_WORD_NUM			34	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			345	
#define	TRIP_RELAY_WORD_NUM			4	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			349	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			381	
#define	INPUT_RELAY_WORD_NUM			68	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			449	
#define	OUTPUT_RELAY_WORD_NUM			33	

//压板继电器
#define	SW_RELAY_WORD_START			482	
#define	SW_RELAY_WORD_NUM			14	

//配置继电器
#define	CFG_RELAY_WORD_START			496	
#define	CFG_RELAY_WORD_NUM			44	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			540	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			556	
#define	CTRLTEST_RELAY_WORD_NUM			19	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			575	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			591	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			615	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			631	
#define	RCP_RELAY_WORD_NUM			42	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			673	
#define	RCE_RELAY_WORD_NUM			42	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			715	
#define	RCV_RELAY_WORD_NUM			42	

//大于等于继电器
#define	GE_RELAY_WORD_START			757	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			765	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			773	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			789	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			805	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			805	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			805	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			805	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			805	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			805	
#define	CNT_RELAY_WORD_NUM			8	


#endif