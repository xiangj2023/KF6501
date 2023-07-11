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
#define	       RW_LK68IDAMUL   22	//LK68IDAMUL-A差流综合谐波闭锁继电器字
#define	        RW_LK68IRAH2   23	//LK68IRAH2-A制动电流二次谐波闭锁继电器字
#define	        RW_LK68IDBH2   24	//LK68IDBH2-B差流二次谐波闭锁继电器字
#define	        RW_LK68IDBH5   25	//LK68IDBH5-B差流五次谐波闭锁继电器字
#define	       RW_LK68IDBMUL   26	//LK68IDBMUL-B差流综合谐波闭锁继电器字
#define	        RW_LK68IRBH2   27	//LK68IRBH2-B制动电流二次谐波闭锁继电器字
#define	        RW_LK68IDCH2   28	//LK68IDCH2-C差流二次谐波闭锁继电器字
#define	        RW_LK68IDCH5   29	//LK68IDCH5-C差流五次谐波闭锁继电器字
#define	       RW_LK68IDCMUL   30	//LK68IDCMUL-C差流综合谐波闭锁继电器字
#define	        RW_LK68IRCH2   31	//LK68IRCH2-C制动电流二次次谐波闭锁继电器字
#define	             RW_AR87   32	//AR87-差流越限告警继电器字
#define	          RW_PI50IH1   33	//PI50IH1-高压侧过流Ⅰ段入段继电器字
#define	          RW_TR50IH1   34	//TR50IH1-高压侧过流Ⅰ段动作继电器字
#define	         RW_LPC50IH1   35	//LPC50IH1-高压侧过流Ⅰ段保护循环控制继电器字
#define	          RW_PI50IH2   36	//PI50IH2-高压侧过流Ⅱ段入段继电器字
#define	          RW_TR50IH2   37	//TR50IH2-高压侧过流Ⅱ段动作继电器字
#define	         RW_LPC50IH2   38	//LPC50IH2-高压侧过流Ⅱ段保护循环控制继电器字
#define	             RW_47UH   39	//47UH-高压侧过流复压闭锁继电器字
#define	             RW_PFHA   40	//PFHA-高压侧A相功率方向正向继电器字
#define	             RW_PRHA   41	//PRHA-高压侧A相功率方向反向继电器字
#define	             RW_PFHB   42	//PFHB-高压侧B相功率方向正向继电器字
#define	             RW_PRHB   43	//PRHB-高压侧B相功率方向反向继电器字
#define	             RW_PFHC   44	//PFHC-高压侧C相功率方向正向继电器字
#define	             RW_PRHC   45	//PRHC-高压侧C相功率方向反向继电器字
#define	          RW_PI50IM1   46	//PI50IM1-中压侧过流Ⅰ段入段继电器字
#define	          RW_TR50IM1   47	//TR50IM1-中压侧过流Ⅰ段动作继电器字
#define	         RW_LPC50IM1   48	//LPC50IM1-中压侧过流Ⅰ段保护循环控制继电器字
#define	          RW_PI50IM2   49	//PI50IM2-中压侧过流Ⅱ段入段继电器字
#define	          RW_TR50IM2   50	//TR50IM2-中压侧过流Ⅱ段动作继电器字
#define	         RW_LPC50IM2   51	//LPC50IM2-中压侧过流Ⅱ段保护循环控制继电器字
#define	          RW_PI50IL1   52	//PI50IL1-低压侧过流Ⅰ段入段继电器字
#define	          RW_TR50IL1   53	//TR50IL1-低压侧过流Ⅰ段动作继电器字
#define	         RW_LPC50IL1   54	//LPC50IL1-低压侧过流Ⅰ段保护循环控制继电器字
#define	          RW_PI50IL2   55	//PI50IL2-低压侧过流Ⅱ段入段继电器字
#define	          RW_TR50IL2   56	//TR50IL2-低压侧过流Ⅱ段动作继电器字
#define	         RW_LPC50IL2   57	//LPC50IL2-低压侧过流Ⅱ段保护循控制环继电器字
#define	          RW_PI50OL1   58	//PI50OL1-过负荷Ⅰ段入段继电器字
#define	          RW_TR50OL1   59	//TR50OL1-过负荷Ⅰ段动作继电器字
#define	         RW_LPC50OL1   60	//LPC50OL1-过负荷Ⅰ段保护循环控制继电器字
#define	          RW_PI50OL2   61	//PI50OL2-过负荷Ⅱ段入段继电器字
#define	          RW_TR50OL2   62	//TR50OL2-过负荷Ⅱ段动作继电器字
#define	         RW_LPC50OL2   63	//LPC50OL2-过负荷Ⅱ段保护循环控制继电器字
#define	          RW_PI50OL3   64	//PI50OL3-过负荷Ⅲ段入段继电器字
#define	          RW_TR50OL3   65	//TR50OL3-过负荷Ⅲ段动作继电器字
#define	         RW_LPC50OL3   66	//LPC50OL3-过负荷Ⅲ段保护循环控制继电器字
#define	            RW_PI27U   67	//PI27U-失压保护入段继电器字
#define	            RW_TR27U   68	//TR27U-失压保护动作继电器字
#define	           RW_LPC27U   69	//LPC27U-失压保护循环控制继电器字
#define	           RW_PI59UN   70	//PI59UN-零序过压保护入段继电器字
#define	           RW_TR59UN   71	//TR59UN-零序过压保护动作继电器字
#define	          RW_LPC59UN   72	//LPC59UN-零序过压保护循环控制继电器字
#define	           RW_PI50IN   73	//PI50IN-零序过流保护入段继电器字
#define	           RW_TR50IN   74	//TR50IN-零序过流保护动作继电器字
#define	          RW_LPC50IN   75	//LPC50IN-零序过流保护循环控制继电器字
#define	          RW_TR50IJX   76	//TR50IJX-间隙过流保护动作继电器字
#define	          RW_PI50IJX   77	//PI50IJX-间隙过流保护入段继电器字
#define	         RW_LPC50IJX   78	//LPC50IJX-间隙过流保护循环控制继电器字
#define	             RW_74VT   79	//74VT-高压侧PT断线继电器字
#define	           RW_PICKUP   80	//PICKUP-模值突变量启动继电器字
#define	             RW_59UA   81	//59UA-UA有压继电器字
#define	             RW_27UA   82	//27UA-UA无压继电器字
#define	             RW_59UB   83	//59UB-UB有压继电器字
#define	             RW_27UB   84	//27UB-UB无压继电器字
#define	             RW_59UC   85	//59UC-UC有压继电器字
#define	             RW_27UC   86	//27UC-UC无压继电器字
#define	             RW_59UN   87	//59UN-UN有压继电器字
#define	             RW_27UN   88	//27UN-UN无压继电器字
#define	             RW_50HA   89	//50HA-IHA有流继电器字
#define	             RW_37HA   90	//37HA-IHA无流继电器字
#define	             RW_50HB   91	//50HB-IHB有流继电器字
#define	             RW_37HB   92	//37HB-IHB无流继电器字
#define	             RW_50HC   93	//50HC-IHC有流继电器字
#define	             RW_37HC   94	//37HC-IHC无流继电器字
#define	             RW_50MA   95	//50MA-IMA有流继电器字
#define	             RW_37MA   96	//37MA-IMA无流继电器字
#define	             RW_50MB   97	//50MB-IMB有流继电器字
#define	             RW_37MB   98	//37MB-IMB无流继电器字
#define	             RW_50MC   99	//50MC-IMC有流继电器字
#define	             RW_37MC  100	//37MC-IMC无流继电器子
#define	             RW_50LA  101	//50LA-ILA有流继电器字
#define	             RW_37LA  102	//37LA-ILA无流继电器字
#define	             RW_50LB  103	//50LB-ILB有流继电器字
#define	             RW_37LB  104	//37LB-ILB无流继电器字
#define	             RW_50LC  105	//50LC-ILC有流继电器字
#define	             RW_37LC  106	//37LC-ILC无流继电器字
#define	             RW_50IN  107	//50IN-IN有流继电器字
#define	             RW_37IN  108	//37IN-IN无流继电器字
#define	            RW_50IJX  109	//50IJX-IJX有流继电器字
#define	            RW_37IJX  110	//37IJX-IJX无流继电器字
#define	          RW_ET11ERR  111	//ET11ERR-以太网1连接1状态继电器字
#define	          RW_ET12ERR  112	//ET12ERR-以太网1连接2状态继电器字
#define	          RW_ET13ERR  113	//ET13ERR-以太网1连接3状态继电器字
#define	          RW_ET14ERR  114	//ET14ERR-以太网1连接4状态继电器字
#define	          RW_ET15ERR  115	//ET15ERR-以太网1连接5状态继电器字
#define	          RW_ET21ERR  116	//ET21ERR-以太网2连接1状态继电器字
#define	          RW_ET22ERR  117	//ET22ERR-以太网2连接2状态继电器字
#define	          RW_ET23ERR  118	//ET23ERR-以太网2连接3状态继电器字
#define	          RW_ET24ERR  119	//ET24ERR-以太网2连接4状态继电器字
#define	          RW_ET25ERR  120	//ET25ERR-以太网2连接5状态继电器字
#define	          RW_COM1ERR  121	//COM1ERR-串口连接状态继电器字
#define	          RW_CAN1ERR  122	//CAN1ERR-CAN网1连接状态继电器字
#define	          RW_CAN2ERR  123	//CAN2ERR-CAN网2连接状态继电器字
#define	         RW_NULL1ERR  124	//NULL1ERR-空1连接状态继电器字
#define	         RW_NULL2ERR  125	//NULL2ERR-空2连接状态继电器字
#define	         RW_NULL3ERR  126	//NULL3ERR-空3连接状态继电器字
#define	        RW_MAINTLINK  127	//MAINTLINK-维护状态继电器字
#define	        RW_LKPI87U_A  128	//LKPI87U_A-A相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_A  129	//LKRST87U_A-A相差动速断返回闭锁继电器字
#define	        RW_LKPI87U_B  130	//LKPI87U_B-B相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_B  131	//LKRST87U_B-B相差动速断返回闭锁继电器字
#define	        RW_LKPI87U_C  132	//LKPI87U_C-C相差动速断入段闭锁继电器字
#define	       RW_LKRST87U_C  133	//LKRST87U_C-C相差动速断返回闭锁继电器字
#define	        RW_LKPI87R_A  134	//LKPI87R_A-A相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_A  135	//LKRST87R_A-A相比率差动返回闭锁继电器字
#define	        RW_LKPI87R_B  136	//LKPI87R_B-B相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_B  137	//LKRST87R_B-B相比率差动返回闭锁继电器字
#define	        RW_LKPI87R_C  138	//LKPI87R_C-C相比率差动入段闭锁继电器字
#define	       RW_LKRST87R_C  139	//LKRST87R_C-C相比率差动返回闭锁继电器字
#define	        RW_LKPI50IH1  140	//LKPI50IH1-高压侧过流Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50IH1  141	//LKRST50IH1-高压侧过流Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50IH2  142	//LKPI50IH2-高压侧过流Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50IH2  143	//LKRST50IH2-高压侧过流Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50IM1  144	//LKPI50IM1-中压侧过流Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50IM1  145	//LKRST50IM1-中压侧过流Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50IM2  146	//LKPI50IM2-中压侧过流Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50IM2  147	//LKRST50IM2-中压侧过流Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50IL1  148	//LKPI50IL1-低压侧过流Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50IL1  149	//LKRST50IL1-低压侧过流Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50IL2  150	//LKPI50IL2-低压侧过流Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50IL2  151	//LKRST50IL2-低压侧过流Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50OL1  152	//LKPI50OL1-过负荷Ⅰ段入段闭锁继电器字
#define	       RW_LKRST50OL1  153	//LKRST50OL1-过负荷Ⅰ段返回闭锁继电器字
#define	        RW_LKPI50OL2  154	//LKPI50OL2-过负荷Ⅱ段入段闭锁继电器字
#define	       RW_LKRST50OL2  155	//LKRST50OL2-过负荷Ⅱ段返回闭锁继电器字
#define	        RW_LKPI50OL3  156	//LKPI50OL3-过负荷Ⅲ段入段闭锁继电器字
#define	       RW_LKRST50OL3  157	//LKRST50OL3-过负荷Ⅲ段返回闭锁继电器字
#define	          RW_LKPI27U  158	//LKPI27U-失压保护入段闭锁继电器字
#define	         RW_LKRST27U  159	//LKRST27U-失压保护返回闭锁继电器字
#define	         RW_LKPI59UN  160	//LKPI59UN-零序过压入段闭锁继电器字
#define	        RW_LKRST59UN  161	//LKRST59UN-零序过压返回闭锁继电器字
#define	         RW_LKPI50IN  162	//LKPI50IN-零序过流入段闭锁继电器字
#define	        RW_LKRST50IN  163	//LKRST50IN-零序过流返回闭锁继电器字
#define	        RW_LKPI50IJX  164	//LKPI50IJX-间隙过流入段闭锁继电器字
#define	       RW_LKRST50IJX  165	//LKRST50IJX-间隙过流返回闭锁继电器字
#define	              RW_REC  166	//REC-录波继电器字
#define	             RW_69IN  167	//69IN-远方当地中间继电器字
#define	             RW_52AH  168	//52AH-高压侧断路器合位
#define	             RW_52BH  169	//52BH-高压侧断路器分位
#define	             RW_52AM  170	//52AM-中压侧断路器合位
#define	             RW_52BM  171	//52BM-中压侧断路器分位
#define	             RW_52AL  172	//52AL-低压侧断路器合位
#define	             RW_52BL  173	//52BL-低压侧断路器分位
#define	               RW_M1  174	//M1-中间继电器字M1
#define	               RW_M2  175	//M2-中间继电器字M2
#define	               RW_M3  176	//M3-中间继电器字M3
#define	               RW_M4  177	//M4-中间继电器字M4
#define	               RW_M5  178	//M5-中间继电器字M5
#define	               RW_M6  179	//M6-中间继电器字M6
#define	               RW_M7  180	//M7-中间继电器字M7
#define	               RW_M8  181	//M8-中间继电器字M8
#define	               RW_M9  182	//M9-中间继电器字M9
#define	              RW_M10  183	//M10-中间继电器字M10
#define	              RW_M11  184	//M11-中间继电器字M11
#define	              RW_M12  185	//M12-中间继电器字M12
#define	              RW_M13  186	//M13-中间继电器字M13
#define	              RW_M14  187	//M14-中间继电器字M14
#define	              RW_M15  188	//M15-中间继电器字M15
#define	              RW_M16  189	//M16-中间继电器字M16
#define	              RW_M17  190	//M17-中间继电器字M17
#define	              RW_M18  191	//M18-中间继电器字M18
#define	              RW_M19  192	//M19-中间继电器字M19
#define	              RW_M20  193	//M20-中间继电器字M20
#define	              RW_M21  194	//M21-中间继电器字M21
#define	              RW_M22  195	//M22-中间继电器字M22
#define	              RW_M23  196	//M23-中间继电器字M23
#define	              RW_M24  197	//M24-中间继电器字M24
#define	              RW_M25  198	//M25-中间继电器字M25
#define	              RW_M26  199	//M26-中间继电器字M26
#define	              RW_M27  200	//M27-中间继电器字M27
#define	              RW_M28  201	//M28-中间继电器字M28
#define	              RW_M29  202	//M29-中间继电器字M29
#define	              RW_M30  203	//M30-中间继电器字M30
#define	              RW_M31  204	//M31-中间继电器字M31
#define	              RW_M32  205	//M32-中间继电器字M32
#define	              RW_M33  206	//M33-中间继电器字M33
#define	              RW_M34  207	//M34-中间继电器字M34
#define	              RW_M35  208	//M35-中间继电器字M35
#define	              RW_M36  209	//M36-中间继电器字M36
#define	              RW_M37  210	//M37-中间继电器字M37
#define	              RW_M38  211	//M38-中间继电器字M38
#define	              RW_M39  212	//M39-中间继电器字M39
#define	              RW_M40  213	//M40-中间继电器字M40
#define	              RW_M41  214	//M41-中间继电器字M41
#define	              RW_M42  215	//M42-中间继电器字M42
#define	              RW_M43  216	//M43-中间继电器字M43
#define	              RW_M44  217	//M44-中间继电器字M44
#define	              RW_M45  218	//M45-中间继电器字M45
#define	              RW_M46  219	//M46-中间继电器字M46
#define	              RW_M47  220	//M47-中间继电器字M47
#define	              RW_M48  221	//M48-中间继电器字M48
#define	              RW_M49  222	//M49-中间继电器字M49
#define	              RW_M50  223	//M50-中间继电器字M50
#define	              RW_M51  224	//M51-中间继电器字M51
#define	              RW_M52  225	//M52-中间继电器字M52
#define	              RW_M53  226	//M53-中间继电器字M53
#define	              RW_M54  227	//M54-中间继电器字M54
#define	              RW_M55  228	//M55-中间继电器字M55
#define	              RW_M56  229	//M56-中间继电器字M56
#define	              RW_M57  230	//M57-中间继电器字M57
#define	              RW_M58  231	//M58-中间继电器字M58
#define	              RW_M59  232	//M59-中间继电器字M59
#define	              RW_M60  233	//M60-中间继电器字M60
#define	              RW_M61  234	//M61-中间继电器字M61
#define	              RW_M62  235	//M62-中间继电器字M62
#define	              RW_M63  236	//M63-中间继电器字M63
#define	              RW_M64  237	//M64-中间继电器字M64
#define	               RW_H1  238	//H1-自保持中间继电器字1
#define	               RW_H2  239	//H2-自保持中间继电器字2
#define	               RW_H3  240	//H3-自保持中间继电器字3
#define	               RW_H4  241	//H4-自保持中间继电器字4
#define	               RW_H5  242	//H5-自保持中间继电器字5
#define	               RW_H6  243	//H6-自保持中间继电器字6
#define	               RW_H7  244	//H7-自保持中间继电器字7
#define	               RW_H8  245	//H8-自保持中间继电器字8
#define	               RW_H9  246	//H9-自保持中间继电器字9
#define	              RW_H10  247	//H10-自保持中间继电器字10
#define	              RW_H11  248	//H11-自保持中间继电器字11
#define	              RW_H12  249	//H12-自保持中间继电器字12
#define	              RW_H13  250	//H13-自保持中间继电器字13
#define	              RW_H14  251	//H14-自保持中间继电器字14
#define	              RW_H15  252	//H15-自保持中间继电器字15
#define	              RW_H16  253	//H16-自保持中间继电器字16
#define	              RW_H17  254	//H17-自保持中间继电器字17
#define	              RW_H18  255	//H18-自保持中间继电器字18
#define	              RW_H19  256	//H19-自保持中间继电器字19
#define	              RW_H20  257	//H20-自保持中间继电器字20
#define	              RW_H21  258	//H21-自保持中间继电器字21
#define	              RW_H22  259	//H22-自保持中间继电器字22
#define	              RW_H23  260	//H23-自保持中间继电器字23
#define	              RW_H24  261	//H24-自保持中间继电器字24
#define	              RW_H25  262	//H25-自保持中间继电器字25
#define	              RW_H26  263	//H26-自保持中间继电器字26
#define	              RW_H27  264	//H27-自保持中间继电器字27
#define	              RW_H28  265	//H28-自保持中间继电器字28
#define	              RW_H29  266	//H29-自保持中间继电器字29
#define	              RW_H30  267	//H30-自保持中间继电器字30
#define	              RW_H31  268	//H31-自保持中间继电器字31
#define	              RW_H32  269	//H32-自保持中间继电器字32
#define	            RW_52BFH  270	//52BFH-高压侧断路器跳闸失败继电器字
#define	            RW_52BFM  271	//52BFM-中压侧断路器跳闸失败继电器字
#define	            RW_52BFL  272	//52BFL-低压侧断路器跳闸失败继电器字
#define	         RW_52BF_ALL  273	//52BF_ALL-断路器跳闸失败继电器字
#define	               RW_T1  274	//T1-T1
#define	               RW_T2  275	//T2-T2
#define	               RW_T3  276	//T3-T3
#define	               RW_T4  277	//T4-T4
#define	               RW_T5  278	//T5-T5
#define	               RW_T6  279	//T6-T6
#define	               RW_T7  280	//T7-T7
#define	               RW_T8  281	//T8-T8
#define	               RW_T9  282	//T9-T9
#define	              RW_T10  283	//T10-T10
#define	              RW_T11  284	//T11-T11
#define	              RW_T12  285	//T12-T12
#define	              RW_T13  286	//T13-T13
#define	              RW_T14  287	//T14-T14
#define	              RW_T15  288	//T15-T15
#define	              RW_T16  289	//T16-T16
#define	             RW_TCB1  290	//TCB1-TCB1
#define	             RW_TCB2  291	//TCB2-TCB2
#define	             RW_TCB3  292	//TCB3-TCB3
#define	             RW_TCB4  293	//TCB4-TCB4
#define	             RW_TCB5  294	//TCB5-TCB5
#define	             RW_TCB6  295	//TCB6-TCB6
#define	             RW_TCB7  296	//TCB7-TCB7
#define	             RW_TCB8  297	//TCB8-TCB8
#define	             RW_TSW1  298	//TSW1-TSW1
#define	             RW_TSW2  299	//TSW2-TSW2
#define	             RW_TSW3  300	//TSW3-TSW3
#define	             RW_TSW4  301	//TSW4-TSW4
#define	             RW_TSW5  302	//TSW5-TSW5
#define	             RW_TSW6  303	//TSW6-TSW6
#define	             RW_TSW7  304	//TSW7-TSW7
#define	             RW_TSW8  305	//TSW8-TSW8
#define	           RW_T52BFH  306	//T52BFH-T52BFH
#define	           RW_T52BFM  307	//T52BFM-T52BFM
#define	           RW_T52BFL  308	//T52BFL-T52BFL
#define	              RW_TR1  309	//TR1-跳闸继电器1
#define	              RW_TR2  310	//TR2-跳闸继电器2
#define	              RW_TR3  311	//TR3-跳闸继电器3
#define	              RW_VR1  312	//VR1-虚拟继电器字1
#define	              RW_VR2  313	//VR2-虚拟继电器字2
#define	              RW_VR3  314	//VR3-虚拟继电器字3
#define	              RW_VR4  315	//VR4-虚拟继电器字4
#define	              RW_VR5  316	//VR5-虚拟继电器字5
#define	              RW_VR6  317	//VR6-虚拟继电器字6
#define	              RW_VR7  318	//VR7-虚拟继电器字7
#define	              RW_VR8  319	//VR8-虚拟继电器字8
#define	              RW_VR9  320	//VR9-虚拟继电器字9
#define	             RW_VR10  321	//VR10-虚拟继电器字10
#define	             RW_VR11  322	//VR11-虚拟继电器字11
#define	             RW_VR12  323	//VR12-虚拟继电器字12
#define	             RW_VR13  324	//VR13-虚拟继电器字13
#define	             RW_VR14  325	//VR14-虚拟继电器字14
#define	             RW_VR15  326	//VR15-虚拟继电器字15
#define	             RW_VR16  327	//VR16-虚拟继电器字16
#define	             RW_VR17  328	//VR17-虚拟继电器字17
#define	             RW_VR18  329	//VR18-虚拟继电器字18
#define	             RW_VR19  330	//VR19-虚拟继电器字19
#define	             RW_VR20  331	//VR20-虚拟继电器字20
#define	             RW_VR21  332	//VR21-虚拟继电器字21
#define	             RW_VR22  333	//VR22-虚拟继电器字22
#define	             RW_VR23  334	//VR23-虚拟继电器字23
#define	             RW_VR24  335	//VR24-虚拟继电器字24
#define	             RW_VR25  336	//VR25-虚拟继电器字25
#define	             RW_VR26  337	//VR26-虚拟继电器字26
#define	             RW_VR27  338	//VR27-虚拟继电器字27
#define	             RW_VR28  339	//VR28-虚拟继电器字28
#define	             RW_VR29  340	//VR29-虚拟继电器字29
#define	             RW_VR30  341	//VR30-虚拟继电器字30
#define	             RW_VR31  342	//VR31-虚拟继电器字31
#define	             RW_VR32  343	//VR32-虚拟继电器字32
#define	              RW_IN1  344	//IN1-IN1
#define	              RW_IN2  345	//IN2-IN2
#define	              RW_IN3  346	//IN3-IN3
#define	              RW_IN4  347	//IN4-IN4
#define	              RW_IN5  348	//IN5-IN5
#define	              RW_IN6  349	//IN6-IN6
#define	              RW_IN7  350	//IN7-IN7
#define	              RW_IN8  351	//IN8-IN8
#define	              RW_IN9  352	//IN9-IN9
#define	             RW_IN10  353	//IN10-IN10
#define	             RW_IN11  354	//IN11-IN11
#define	             RW_IN12  355	//IN12-IN12
#define	             RW_IN13  356	//IN13-IN13
#define	             RW_IN14  357	//IN14-IN14
#define	             RW_IN15  358	//IN15-IN15
#define	             RW_IN16  359	//IN16-IN16
#define	             RW_IN17  360	//IN17-IN17
#define	             RW_IN18  361	//IN18-IN18
#define	             RW_IN19  362	//IN19-IN19
#define	             RW_IN20  363	//IN20-IN20
#define	             RW_IN21  364	//IN21-IN21
#define	             RW_IN22  365	//IN22-IN22
#define	             RW_IN23  366	//IN23-IN23
#define	             RW_IN24  367	//IN24-IN24
#define	             RW_IN25  368	//IN25-IN25
#define	             RW_IN26  369	//IN26-IN26
#define	             RW_IN27  370	//IN27-IN27
#define	             RW_IN28  371	//IN28-IN28
#define	             RW_IN29  372	//IN29-IN29
#define	             RW_IN30  373	//IN30-IN30
#define	             RW_IN31  374	//IN31-IN31
#define	             RW_IN32  375	//IN32-IN32
#define	             RW_IN33  376	//IN33-IN33
#define	             RW_IN34  377	//IN34-IN34
#define	             RW_IN35  378	//IN35-信号复归
#define	             RW_IN36  379	//IN36-中压侧断路器分位
#define	             RW_IN37  380	//IN37-中压侧断路器合位
#define	             RW_IN38  381	//IN38-中压侧断路器遥控预置
#define	             RW_IN39  382	//IN39-中压侧遥控1预置
#define	             RW_IN40  383	//IN40-中压侧遥控2预置
#define	             RW_IN41  384	//IN41-低压侧断路器分位
#define	             RW_IN42  385	//IN42-低压侧断路器合位
#define	             RW_IN43  386	//IN43-低压侧断路器遥控预置
#define	             RW_IN44  387	//IN44-低压侧遥控1预置
#define	             RW_IN45  388	//IN45-低压侧遥控2预置
#define	             RW_IN46  389	//IN46-高压侧断路器分位
#define	             RW_IN47  390	//IN47-高压侧断路器合位
#define	             RW_IN48  391	//IN48-高压侧断路器遥控预置
#define	             RW_IN49  392	//IN49-高压侧遥控1预置
#define	             RW_IN50  393	//IN50-高压侧遥控2预置
#define	           RW_INVIN1  394	//INVIN1-VIN1
#define	           RW_INVIN2  395	//INVIN2-VIN2
#define	           RW_INVIN3  396	//INVIN3-VIN3
#define	           RW_INVIN4  397	//INVIN4-VIN4
#define	           RW_INVIN5  398	//INVIN5-VIN5
#define	           RW_INVIN6  399	//INVIN6-VIN6
#define	           RW_INVIN7  400	//INVIN7-VIN7
#define	           RW_INVIN8  401	//INVIN8-VIN8
#define	           RW_INVIN9  402	//INVIN9-VIN9
#define	          RW_INVIN10  403	//INVIN10-VIN10
#define	          RW_INVIN11  404	//INVIN11-VIN11
#define	          RW_INVIN12  405	//INVIN12-VIN12
#define	          RW_INVIN13  406	//INVIN13-VIN13
#define	          RW_INVIN14  407	//INVIN14-VIN14
#define	          RW_INVIN15  408	//INVIN15-VIN15
#define	          RW_INVIN16  409	//INVIN16-VIN16
#define	             RW_OUT1  410	//OUT1-开出1继电器字
#define	             RW_OUT2  411	//OUT2-开出2继电器字
#define	             RW_OUT3  412	//OUT3-开出3继电器字
#define	             RW_OUT4  413	//OUT4-开出4继电器字
#define	             RW_OUT5  414	//OUT5-开出5继电器字
#define	             RW_OUT6  415	//OUT6-开出6继电器字
#define	             RW_OUT7  416	//OUT7-开出7继电器字
#define	             RW_OUT8  417	//OUT8-开出8继电器字
#define	             RW_OUT9  418	//OUT9-开出9继电器字
#define	            RW_OUT10  419	//OUT10-开出10继电器字
#define	            RW_OUT11  420	//OUT11-开出11继电器字
#define	            RW_OUT12  421	//OUT12-开出12继电器字
#define	            RW_OUT13  422	//OUT13-开出13继电器字
#define	            RW_OUT14  423	//OUT14-开出14继电器字
#define	            RW_OUT15  424	//OUT15-开出15继电器字
#define	            RW_OUT16  425	//OUT16-开出16继电器字
#define	            RW_OUT17  426	//OUT17-开出17继电器字
#define	            RW_OUT18  427	//OUT18-开出18继电器字
#define	            RW_OUT19  428	//OUT19-开出19继电器字
#define	            RW_OUT20  429	//OUT20-开出20继电器字
#define	            RW_OUT21  430	//OUT21-开出21继电器字
#define	            RW_OUT22  431	//OUT22-开出22继电器字
#define	            RW_OUT23  432	//OUT23-开出23继电器字
#define	            RW_OUT24  433	//OUT24-开出24继电器字
#define	            RW_OUT25  434	//OUT25-开出25继电器字
#define	            RW_OUT26  435	//OUT26-开出26继电器字
#define	            RW_OUT27  436	//OUT27-开出27继电器字
#define	            RW_OUT28  437	//OUT28-开出28继电器字
#define	            RW_OUT29  438	//OUT29-开出29继电器字
#define	            RW_OUT30  439	//OUT30-开出30继电器字
#define	            RW_OUT31  440	//OUT31-开出31继电器字
#define	            RW_OUT32  441	//OUT32-开出32继电器字
#define	            RW_OUT33  442	//OUT33-开出33继电器字
#define	            RW_OUT34  443	//OUT34-开出34继电器字
#define	            RW_OUT35  444	//OUT35-开出35继电器字
#define	            RW_OUT36  445	//OUT36-开出36继电器字
#define	            RW_OUT37  446	//OUT37-开出37继电器字
#define	            RW_OUT38  447	//OUT38-开出38继电器字
#define	            RW_OUT39  448	//OUT39-开出39继电器字
#define	            RW_OUT40  449	//OUT40-开出40继电器字
#define	            RW_OUT41  450	//OUT41-开出41继电器字
#define	            RW_SL87U  451	//SL87U-差流速断软压板继电器字
#define	            RW_SL87R  452	//SL87R-比率差动软压板继电器字
#define	           RW_SL50IH  453	//SL50IH-高压侧过流软压板继电器字
#define	           RW_SL50IM  454	//SL50IM-中压侧过流软压板继电器字
#define	           RW_SL50IL  455	//SL50IL-低压侧过流软压板继电器字
#define	           RW_SL50OL  456	//SL50OL-过负荷软压板继电器字
#define	            RW_SL27U  457	//SL27U-失压保护软压板继电器字
#define	           RW_SL59UN  458	//SL59UN-零序过压软压板继电器字
#define	           RW_SL50IN  459	//SL50IN-零序过流软压板继电器字
#define	          RW_SL50IJX  460	//SL50IJX-间隙过流软压板继电器字
#define	          RW_SLMAINT  461	//SLMAINT-检修压板
#define	           RW_SLBAK1  462	//SLBAK1-备用压板1
#define	           RW_SLBAK2  463	//SLBAK2-备用压板2
#define	           RW_F87U_A  464	//F87U_A-A相差动速断配置继电器字
#define	           RW_F87U_B  465	//F87U_B-B相差流速断配置继电器字
#define	           RW_F87U_C  466	//F87U_C-C相差流速断配置继电器字
#define	           RW_F87R_A  467	//F87R_A-A相比率差动配置继电器字
#define	           RW_F87R_B  468	//F87R_B-B相比率差动配置继电器字
#define	           RW_F87R_C  469	//F87R_C-C相比率差动配置继电器字
#define	       RW_FDCRES_87R  470	//FDCRES_87R-差动非周期分量抑制配置继电器字
#define	         RW_F68IDH2M  471	//F68IDH2M-差流二次谐波最大相闭锁配置继电器字
#define	         RW_F68IDH2C  472	//F68IDH2C-差流二次谐波综合相闭锁配置继电器字
#define	         RW_F68IDH2I  473	//F68IDH2I-差流二次谐波分相闭锁配置继电器字
#define	         RW_F68IRH2I  474	//F68IRH2I-制动二次谐波闭锁配置继电器字
#define	         RW_F68IDH5I  475	//F68IDH5I-差流五次谐波闭锁配置继电器字
#define	        RW_F68IDHMUL  476	//F68IDHMUL-差流综合谐波闭锁配置继电器字
#define	            RW_FMBph  477	//FMBph-中压侧两相配置继电器字
#define	            RW_FLBph  478	//FLBph-低压侧两相配置继电器字
#define	           RW_F50IH1  479	//F50IH1-高压侧过流Ⅰ段配置继电器字
#define	        RW_F50IH1_PD  480	//F50IH1_PD-高过流Ⅰ段功率方向配置继电器字
#define	       RW_F50IH1_47U  481	//F50IH1_47U-高过流Ⅰ段复压闭锁配置继电器字
#define	           RW_F50IH2  482	//F50IH2-高压侧过流Ⅱ段配置继电器字
#define	        RW_F50IH2_PD  483	//F50IH2_PD-高过流Ⅱ段功率方向配置继电器字
#define	       RW_F50IH2_47U  484	//F50IH2_47U-高过流Ⅱ段复压闭锁配置继电器字
#define	           RW_F50IM1  485	//F50IM1-中压侧过流Ⅰ段配置继电器字
#define	           RW_F50IM2  486	//F50IM2-中压侧过流Ⅱ段配置继电器字
#define	           RW_F50IL1  487	//F50IL1-低压侧过流Ⅰ段配置继电器字
#define	           RW_F50IL2  488	//F50IL2-低压侧过流Ⅱ段配置继电器字
#define	           RW_F50OL1  489	//F50OL1-过负荷Ⅰ段配置继电器字
#define	           RW_F50OL2  490	//F50OL2-过负荷Ⅱ段配置继电器字
#define	           RW_F50OL3  491	//F50OL3-过负荷Ⅲ段配置继电器字
#define	             RW_F27U  492	//F27U-失压保护配置继电器字
#define	         RW_F27U_50I  493	//F27U_50I-失压保护电流闭锁配置继电器字
#define	            RW_F59UN  494	//F59UN-零序过压配置继电器字
#define	            RW_F50IN  495	//F50IN-零序过流配置继电器字
#define	           RW_F50IJX  496	//F50IJX-间隙过流配置继电器字
#define	            RW_F74VT  497	//F74VT-PT断线配置继电器字
#define	            RW_F52BF  498	//F52BF-失灵保护配置继电器字
#define	            RW_FBAK1  499	//FBAK1-备用配置1
#define	            RW_FBAK2  500	//FBAK2-备用配置2
#define	              RW_PW1  501	//PW1-脉冲继电器1
#define	              RW_PW2  502	//PW2-脉冲继电器2
#define	              RW_PW3  503	//PW3-脉冲继电器3
#define	              RW_PW4  504	//PW4-脉冲继电器4
#define	              RW_PW5  505	//PW5-脉冲继电器5
#define	              RW_PW6  506	//PW6-脉冲继电器6
#define	              RW_PW7  507	//PW7-脉冲继电器7
#define	              RW_PW8  508	//PW8-脉冲继电器8
#define	              RW_PW9  509	//PW9-脉冲继电器9
#define	             RW_PW10  510	//PW10-脉冲继电器10
#define	             RW_PW11  511	//PW11-脉冲继电器11
#define	             RW_PW12  512	//PW12-脉冲继电器12
#define	             RW_PW13  513	//PW13-脉冲继电器13
#define	             RW_PW14  514	//PW14-脉冲继电器14
#define	             RW_PW15  515	//PW15-脉冲继电器15
#define	             RW_PW16  516	//PW16-脉冲继电器16
#define	              RW_TO1  517	//TO1-保护跳高压侧断路器
#define	              RW_TO2  518	//TO2-高压侧断路器遥控分
#define	              RW_TO3  519	//TO3-高压侧断路器遥控合
#define	              RW_TO4  520	//TO4-高压侧遥控1分
#define	              RW_TO5  521	//TO5-高压侧遥控1合
#define	              RW_TO6  522	//TO6-高压侧遥控2分
#define	              RW_TO7  523	//TO7-高压侧遥控2合
#define	              RW_TO8  524	//TO8-保护跳中压侧断路器
#define	              RW_TO9  525	//TO9-中压侧断路器遥控分
#define	             RW_TO10  526	//TO10-中压侧断路器遥控合
#define	             RW_TO11  527	//TO11-中压侧遥控1分
#define	             RW_TO12  528	//TO12-中压侧遥控1合
#define	             RW_TO13  529	//TO13-中压侧遥控2分
#define	             RW_TO14  530	//TO14-中压侧遥控2合
#define	             RW_TO15  531	//TO15-保护跳低压侧断路器
#define	             RW_TO16  532	//TO16-低压侧断路器遥控分
#define	             RW_TO17  533	//TO17-低压侧断路器遥控合
#define	             RW_TO18  534	//TO18-低压侧遥控1分
#define	             RW_TO19  535	//TO19-低压侧遥控1合
#define	             RW_TO20  536	//TO20-低压侧遥控2分
#define	             RW_TO21  537	//TO21-低压侧遥控2合
#define	             RW_SIG1  538	//SIG1-信号继电器1
#define	             RW_SIG2  539	//SIG2-信号继电器2
#define	             RW_SIG3  540	//SIG3-信号继电器3
#define	             RW_SIG4  541	//SIG4-信号继电器4
#define	             RW_SIG5  542	//SIG5-信号继电器5
#define	             RW_SIG6  543	//SIG6-信号继电器6
#define	             RW_SIG7  544	//SIG7-信号继电器7
#define	             RW_SIG8  545	//SIG8-信号继电器8
#define	             RW_SIG9  546	//SIG9-信号继电器9
#define	            RW_SIG10  547	//SIG10-信号继电器10
#define	            RW_SIG11  548	//SIG11-信号继电器11
#define	            RW_SIG12  549	//SIG12-信号继电器12
#define	            RW_SIG13  550	//SIG13-信号继电器13
#define	            RW_SIG14  551	//SIG14-信号继电器14
#define	            RW_SIG15  552	//SIG15-信号继电器15
#define	            RW_SIG16  553	//SIG16-信号继电器16
#define	            RW_LED1G  554	//LED1G-LED1绿灯继电器字
#define	            RW_LED1R  555	//LED1R-LED1红灯继电器字
#define	            RW_LED2G  556	//LED2G-LED2绿灯继电器字
#define	            RW_LED2R  557	//LED2R-LED2红灯继电器字
#define	            RW_LED3G  558	//LED3G-LED3绿灯继电器字
#define	            RW_LED3R  559	//LED3R-LED3红灯继电器字
#define	            RW_LED4G  560	//LED4G-LED4绿灯继电器字
#define	            RW_LED4R  561	//LED4R-LED4红灯继电器字
#define	            RW_LED5G  562	//LED5G-LED5绿灯继电器字
#define	            RW_LED5R  563	//LED5R-LED5红灯继电器字
#define	            RW_LED6G  564	//LED6G-LED6绿灯继电器字
#define	            RW_LED6R  565	//LED6R-LED6红灯继电器字
#define	            RW_LED7G  566	//LED7G-LED7绿灯继电器字
#define	            RW_LED7R  567	//LED7R-LED7红灯继电器字
#define	            RW_LED8G  568	//LED8G-LED8绿灯继电器字
#define	            RW_LED8R  569	//LED8R-LED8红灯继电器字
#define	            RW_LED9G  570	//LED9G-LED9绿灯继电器字
#define	            RW_LED9R  571	//LED9R-LED9红灯继电器字
#define	           RW_LED10G  572	//LED10G-LED10绿灯继电器字
#define	           RW_LED10R  573	//LED10R-LED10红灯继电器字
#define	           RW_LED11G  574	//LED11G-LED11绿灯继电器字
#define	           RW_LED11R  575	//LED11R-LED11红灯继电器字
#define	           RW_LED12G  576	//LED12G-LED12绿灯继电器字
#define	           RW_LED12R  577	//LED12R-LED12红灯继电器字
#define	             RW_VIN1  578	//VIN1-VIN1
#define	             RW_VIN2  579	//VIN2-VIN2
#define	             RW_VIN3  580	//VIN3-VIN3
#define	             RW_VIN4  581	//VIN4-VIN4
#define	             RW_VIN5  582	//VIN5-VIN5
#define	             RW_VIN6  583	//VIN6-VIN6
#define	             RW_VIN7  584	//VIN7-VIN7
#define	             RW_VIN8  585	//VIN8-VIN8
#define	             RW_VIN9  586	//VIN9-VIN9
#define	            RW_VIN10  587	//VIN10-VIN10
#define	            RW_VIN11  588	//VIN11-VIN11
#define	            RW_VIN12  589	//VIN12-VIN12
#define	            RW_VIN13  590	//VIN13-VIN13
#define	            RW_VIN14  591	//VIN14-VIN14
#define	            RW_VIN15  592	//VIN15-VIN15
#define	            RW_VIN16  593	//VIN16-VIN16
#define	             RW_RCP1  594	//RCP1-遥控预置继电器1
#define	             RW_RCP2  595	//RCP2-遥控预置继电器2
#define	             RW_RCP3  596	//RCP3-遥控预置继电器3
#define	             RW_RCP4  597	//RCP4-遥控预置继电器4
#define	             RW_RCP5  598	//RCP5-遥控预置继电器5
#define	             RW_RCP6  599	//RCP6-遥控预置继电器6
#define	             RW_RCP7  600	//RCP7-遥控预置继电器7
#define	             RW_RCP8  601	//RCP8-遥控预置继电器8
#define	             RW_RCP9  602	//RCP9-遥控预置继电器9
#define	            RW_RCP10  603	//RCP10-遥控预置继电器10
#define	            RW_RCP11  604	//RCP11-遥控预置继电器11
#define	            RW_RCP12  605	//RCP12-遥控预置继电器12
#define	            RW_RCP13  606	//RCP13-遥控预置继电器13
#define	            RW_RCP14  607	//RCP14-遥控预置继电器14
#define	            RW_RCP15  608	//RCP15-遥控预置继电器15
#define	            RW_RCP16  609	//RCP16-遥控预置继电器16
#define	            RW_RCP17  610	//RCP17-遥控预置继电器17
#define	            RW_RCP18  611	//RCP18-遥控预置继电器18
#define	            RW_RCP19  612	//RCP19-遥控预置继电器19
#define	            RW_RCP20  613	//RCP20-遥控预置继电器20
#define	            RW_RCP21  614	//RCP21-遥控预置继电器21
#define	            RW_RCP22  615	//RCP22-遥控预置继电器22
#define	            RW_RCP23  616	//RCP23-遥控预置继电器23
#define	            RW_RCP24  617	//RCP24-遥控预置继电器24
#define	            RW_RCP25  618	//RCP25-遥控预置继电器25
#define	            RW_RCP26  619	//RCP26-遥控预置继电器26
#define	            RW_RCP27  620	//RCP27-遥控预置继电器27
#define	            RW_RCP28  621	//RCP28-遥控预置继电器28
#define	            RW_RCP29  622	//RCP29-遥控预置继电器29
#define	            RW_RCP30  623	//RCP30-遥控预置继电器30
#define	            RW_RCP31  624	//RCP31-遥控预置继电器31
#define	            RW_RCP32  625	//RCP32-遥控预置继电器32
#define	            RW_RCP33  626	//RCP33-遥控预置继电器33
#define	            RW_RCP34  627	//RCP34-遥控预置继电器34
#define	            RW_RCP35  628	//RCP35-遥控预置继电器35
#define	            RW_RCP36  629	//RCP36-遥控预置继电器36
#define	            RW_RCP37  630	//RCP37-遥控预置继电器37
#define	            RW_RCP38  631	//RCP38-遥控预置继电器38
#define	            RW_RCP39  632	//RCP39-遥控预置继电器39
#define	            RW_RCP40  633	//RCP40-遥控预置继电器40
#define	            RW_RCP41  634	//RCP41-遥控预置继电器41
#define	            RW_RCP42  635	//RCP42-遥控预置继电器42
#define	            RW_RCP43  636	//RCP43-遥控预置继电器43
#define	            RW_RCP44  637	//RCP44-遥控预置继电器44
#define	             RW_RCE1  638	//RCE1-遥控执行继电器1
#define	             RW_RCE2  639	//RCE2-遥控执行继电器2
#define	             RW_RCE3  640	//RCE3-遥控执行继电器3
#define	             RW_RCE4  641	//RCE4-遥控执行继电器4
#define	             RW_RCE5  642	//RCE5-遥控执行继电器5
#define	             RW_RCE6  643	//RCE6-遥控执行继电器6
#define	             RW_RCE7  644	//RCE7-遥控执行继电器7
#define	             RW_RCE8  645	//RCE8-遥控执行继电器8
#define	             RW_RCE9  646	//RCE9-遥控执行继电器9
#define	            RW_RCE10  647	//RCE10-遥控执行继电器10
#define	            RW_RCE11  648	//RCE11-遥控执行继电器11
#define	            RW_RCE12  649	//RCE12-遥控执行继电器12
#define	            RW_RCE13  650	//RCE13-遥控执行继电器13
#define	            RW_RCE14  651	//RCE14-遥控执行继电器14
#define	            RW_RCE15  652	//RCE15-遥控执行继电器15
#define	            RW_RCE16  653	//RCE16-遥控执行继电器16
#define	            RW_RCE17  654	//RCE17-遥控执行继电器17
#define	            RW_RCE18  655	//RCE18-遥控执行继电器18
#define	            RW_RCE19  656	//RCE19-遥控执行继电器19
#define	            RW_RCE20  657	//RCE20-遥控执行继电器20
#define	            RW_RCE21  658	//RCE21-遥控执行继电器21
#define	            RW_RCE22  659	//RCE22-遥控执行继电器22
#define	            RW_RCE23  660	//RCE23-遥控执行继电器23
#define	            RW_RCE24  661	//RCE24-遥控执行继电器24
#define	            RW_RCE25  662	//RCE25-遥控执行继电器25
#define	            RW_RCE26  663	//RCE26-遥控执行继电器26
#define	            RW_RCE27  664	//RCE27-遥控执行继电器27
#define	            RW_RCE28  665	//RCE28-遥控执行继电器28
#define	            RW_RCE29  666	//RCE29-遥控执行继电器29
#define	            RW_RCE30  667	//RCE30-遥控执行继电器30
#define	            RW_RCE31  668	//RCE31-遥控执行继电器31
#define	            RW_RCE32  669	//RCE32-遥控执行继电器32
#define	            RW_RCE33  670	//RCE33-遥控执行继电器33
#define	            RW_RCE34  671	//RCE34-遥控执行继电器34
#define	            RW_RCE35  672	//RCE35-遥控执行继电器35
#define	            RW_RCE36  673	//RCE36-遥控执行继电器36
#define	            RW_RCE37  674	//RCE37-遥控执行继电器37
#define	            RW_RCE38  675	//RCE38-遥控执行继电器38
#define	            RW_RCE39  676	//RCE39-遥控执行继电器39
#define	            RW_RCE40  677	//RCE40-遥控执行继电器40
#define	            RW_RCE41  678	//RCE41-遥控执行继电器41
#define	            RW_RCE42  679	//RCE42-遥控执行继电器42
#define	            RW_RCE43  680	//RCE43-遥控执行继电器43
#define	            RW_RCE44  681	//RCE44-遥控执行继电器44
#define	             RW_RCV1  682	//RCV1-遥控反校继电器1
#define	             RW_RCV2  683	//RCV2-遥控反校继电器2
#define	             RW_RCV3  684	//RCV3-遥控反校继电器3
#define	             RW_RCV4  685	//RCV4-遥控反校继电器4
#define	             RW_RCV5  686	//RCV5-遥控反校继电器5
#define	             RW_RCV6  687	//RCV6-遥控反校继电器6
#define	             RW_RCV7  688	//RCV7-遥控反校继电器7
#define	             RW_RCV8  689	//RCV8-遥控反校继电器8
#define	             RW_RCV9  690	//RCV9-遥控反校继电器9
#define	            RW_RCV10  691	//RCV10-遥控反校继电器10
#define	            RW_RCV11  692	//RCV11-遥控反校继电器11
#define	            RW_RCV12  693	//RCV12-遥控反校继电器12
#define	            RW_RCV13  694	//RCV13-遥控反校继电器13
#define	            RW_RCV14  695	//RCV14-遥控反校继电器14
#define	            RW_RCV15  696	//RCV15-遥控反校继电器15
#define	            RW_RCV16  697	//RCV16-遥控反校继电器16
#define	            RW_RCV17  698	//RCV17-遥控反校继电器17
#define	            RW_RCV18  699	//RCV18-遥控反校继电器18
#define	            RW_RCV19  700	//RCV19-遥控反校继电器19
#define	            RW_RCV20  701	//RCV20-遥控反校继电器20
#define	            RW_RCV21  702	//RCV21-遥控反校继电器21
#define	            RW_RCV22  703	//RCV22-遥控反校继电器22
#define	            RW_RCV23  704	//RCV23-遥控反校继电器23
#define	            RW_RCV24  705	//RCV24-遥控反校继电器24
#define	            RW_RCV25  706	//RCV25-遥控反校继电器25
#define	            RW_RCV26  707	//RCV26-遥控反校继电器26
#define	            RW_RCV27  708	//RCV27-遥控反校继电器27
#define	            RW_RCV28  709	//RCV28-遥控反校继电器28
#define	            RW_RCV29  710	//RCV29-遥控反校继电器29
#define	            RW_RCV30  711	//RCV30-遥控反校继电器30
#define	            RW_RCV31  712	//RCV31-压控反校继电器31
#define	            RW_RCV32  713	//RCV32-压控反校继电器32
#define	            RW_RCV33  714	//RCV33-压控反校继电器33
#define	            RW_RCV34  715	//RCV34-压控反校继电器34
#define	            RW_RCV35  716	//RCV35-压控反校继电器35
#define	            RW_RCV36  717	//RCV36-压控反校继电器36
#define	            RW_RCV37  718	//RCV37-压控反校继电器37
#define	            RW_RCV38  719	//RCV38-压控反校继电器38
#define	            RW_RCV39  720	//RCV39-压控反校继电器39
#define	            RW_RCV40  721	//RCV40-压控反校继电器40
#define	            RW_RCV41  722	//RCV41-压控反校继电器41
#define	            RW_RCV42  723	//RCV42-压控反校继电器42
#define	            RW_RCV43  724	//RCV43-压控反校继电器43
#define	            RW_RCV44  725	//RCV44-压控反校继电器44
#define	              RW_GE1  726	//GE1-大于等于继电器1
#define	              RW_GE2  727	//GE2-大于等于继电器2
#define	              RW_GE3  728	//GE3-大于等于继电器3
#define	              RW_GE4  729	//GE4-大于等于继电器4
#define	              RW_GE5  730	//GE5-大于等于继电器5
#define	              RW_GE6  731	//GE6-大于等于继电器6
#define	              RW_GE7  732	//GE7-大于等于继电器7
#define	              RW_GE8  733	//GE8-大于等于继电器8
#define	           RW_CONST1  734	//CONST1-常数时间继电器1
#define	           RW_CONST2  735	//CONST2-常数时间继电器2
#define	           RW_CONST3  736	//CONST3-常数时间继电器3
#define	           RW_CONST4  737	//CONST4-常数时间继电器4
#define	           RW_CONST5  738	//CONST5-常数时间继电器5
#define	           RW_CONST6  739	//CONST6-常数时间继电器6
#define	           RW_CONST7  740	//CONST7-常数时间继电器7
#define	           RW_CONST8  741	//CONST8-常数时间继电器8
#define	              RW_TX1  742	//TX1-镜像发送继电器字1
#define	              RW_TX2  743	//TX2-镜像发送继电器字2
#define	              RW_TX3  744	//TX3-镜像发送继电器字3
#define	              RW_TX4  745	//TX4-镜像发送继电器字4
#define	              RW_TX5  746	//TX5-镜像发送继电器字5
#define	              RW_TX6  747	//TX6-镜像发送继电器字6
#define	              RW_TX7  748	//TX7-镜像发送继电器字7
#define	              RW_TX8  749	//TX8-镜像发送继电器字8
#define	              RW_TX9  750	//TX9-镜像发送继电器字9
#define	             RW_TX10  751	//TX10-镜像发送继电器字10
#define	             RW_TX11  752	//TX11-镜像发送继电器字11
#define	             RW_TX12  753	//TX12-镜像发送继电器字12
#define	             RW_TX13  754	//TX13-镜像发送继电器字13
#define	             RW_TX14  755	//TX14-镜像发送继电器字14
#define	             RW_TX15  756	//TX15-镜像发送继电器字15
#define	             RW_TX16  757	//TX16-镜像发送继电器字16
#define	              RW_RX1  758	//RX1-镜像接收继电器字1
#define	              RW_RX2  759	//RX2-镜像接收继电器字2
#define	              RW_RX3  760	//RX3-镜像接收继电器字3
#define	              RW_RX4  761	//RX4-镜像接收继电器字4
#define	              RW_RX5  762	//RX5-镜像接收继电器字5
#define	              RW_RX6  763	//RX6-镜像接收继电器字6
#define	              RW_RX7  764	//RX7-镜像接收继电器字7
#define	              RW_RX8  765	//RX8-镜像接收继电器字8
#define	              RW_RX9  766	//RX9-镜像接收继电器字9
#define	             RW_RX10  767	//RX10-镜像接收继电器字10
#define	             RW_RX11  768	//RX11-镜像接收继电器字11
#define	             RW_RX12  769	//RX12-镜像接收继电器字12
#define	             RW_RX13  770	//RX13-镜像接收继电器字13
#define	             RW_RX14  771	//RX14-镜像接收继电器字14
#define	             RW_RX15  772	//RX15-镜像接收继电器字15
#define	             RW_RX16  773	//RX16-镜像接收继电器字16
#define	             RW_CNT1  774	//CNT1-计数继电器字1
#define	             RW_CNT2  775	//CNT2-计数继电器字2
#define	             RW_CNT3  776	//CNT3-计数继电器字3
#define	             RW_CNT4  777	//CNT4-计数继电器字4
#define	             RW_CNT5  778	//CNT5-计数继电器字5
#define	             RW_CNT6  779	//CNT6-计数继电器字6
#define	             RW_CNT7  780	//CNT7-计数继电器字7
#define	             RW_CNT8  781	//CNT8-计数继电器字8
#define	RELAY_WORD_NUM			782		//继电器总数

//保护继电器字
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			126	

//中间继电器字
#define	MID_RELAY_WORD_START			128	
#define	MID_RELAY_WORD_NUM			110	

//自保持中间继电器字
#define	KEEP_RELAY_WORD_START			238	
#define	KEEP_RELAY_WORD_NUM			36	

//时间继电器字
#define	TIME_RELAY_WORD_START			274	
#define	TIME_RELAY_WORD_NUM			35	

//跳闸继电器
#define	TRIP_RELAY_WORD_START			309	
#define	TRIP_RELAY_WORD_NUM			3	

//虚拟继电器
#define	VIRTUAL_RELAY_WORD_START			312	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//开入继电器
#define	INPUT_RELAY_WORD_START			344	
#define	INPUT_RELAY_WORD_NUM			66	

//开出继电器
#define	OUTPUT_RELAY_WORD_START			410	
#define	OUTPUT_RELAY_WORD_NUM			41	

//压板继电器
#define	SW_RELAY_WORD_START			451	
#define	SW_RELAY_WORD_NUM			13	

//配置继电器
#define	CFG_RELAY_WORD_START			464	
#define	CFG_RELAY_WORD_NUM			37	

//脉冲继电器
#define	PULSE_RELAY_WORD_START			501	
#define	PULSE_RELAY_WORD_NUM			16	

//传动继电器
#define	CTRLTEST_RELAY_WORD_START			517	
#define	CTRLTEST_RELAY_WORD_NUM			21	

//信号继电器
#define	SIGNAL_RELAY_WORD_START			538	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED继电器
#define	LED_RELAY_WORD_START			554	
#define	LED_RELAY_WORD_NUM			24	

//虚拟开入
#define	VIN_RELAY_WORD_START			578	
#define	VIN_RELAY_WORD_NUM			16	

//遥控预置继电器
#define	RCP_RELAY_WORD_START			594	
#define	RCP_RELAY_WORD_NUM			44	

//遥控执行继电器
#define	RCE_RELAY_WORD_START			638	
#define	RCE_RELAY_WORD_NUM			44	

//遥控反校继电器
#define	RCV_RELAY_WORD_START			682	
#define	RCV_RELAY_WORD_NUM			44	

//大于等于继电器
#define	GE_RELAY_WORD_START			726	
#define	GE_RELAY_WORD_NUM			8	

//常数时间继电器
#define	CONST_TIME_RW_START			734	
#define	CONST_TIME_RW_NUM			8	

//发送链接继电器
#define	TXLINK_RW_START			742	
#define	TXLINK_RW_NUM			16	

//接收链接继电器
#define	RXLINK_RW_START			758	
#define	RXLINK_RW_NUM			16	

//电保持中间继
#define	NM_RW_START			774	
#define	NM_RW_NUM			0	

//电保持自保持继
#define	NKEEP_RW_START			774	
#define	NKEEP_RW_NUM			0	

//调试输出继电器
#define	DEBUG_OUT_START			774	
#define	DEBUG_OUT_NUM			0	

//调试输入继电器
#define	DEBUG_IN_START			774	
#define	DEBUG_IN_NUM			0	

//当地控制继电器
#define	LCE_RELAY_WORD_START			774	
#define	LCE_RELAY_WORD_NUM			0	

//计数继电器
#define	CNT_RELAY_WORD_START			774	
#define	CNT_RELAY_WORD_NUM			8	


#endif