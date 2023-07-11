/*************************************************************************
* 
*   ��Ȩ����(C) ��򿭷������ɷ����޹�˾(KeyVia).
* 
*************************************************************************/

/*************************************************************************
* 
*   �ļ����ƣ�
*			Logicdef.h
* 
*   ���ģ�飺�߼�����
*
* 
*	���� �̵����궨�壬 �̵���ִ�����Ͷ���
* 
* 
* 
*   ����
*
*
*	�޸���ʷ
* 
*		�޸���		�޸�����		�޸�����
* 
*		kangqian	2009-1-15		��ʼ����
* 
*************************************************************************/

#ifndef __Logicdef_H
#define __Logicdef_H

/**********************/

#define	 MAX_FUNCTYPE 	 12 	 //���������
#define	              FALSE1    0	//FALSE-FALSE
#define	               TRUE1    1	//TRUE-TRUE
#define	           RW_PI50I1    2	//PI50I1-���������μ̵�����
#define	           RW_TR50I1    3	//TR50I1-������ζ����̵�����
#define	          RW_LPC50I1    4	//LPC50I1-�������ѭ�����Ƽ̵�����
#define	        RW_TR50I1_94    5	//TR50I1_94-������μ��ٶ����̵�����
#define	       RW_LK50I1_47U    6	//LK50I1_47U-������θ�ѹ�����̵�����
#define	           RW_PI50I2    7	//PI50I2-���������μ̵�����
#define	           RW_TR50I2    8	//TR50I2-������ζ����̵�����
#define	          RW_LPC50I2    9	//LPC50I2-�������ѭ�����Ƽ̵�����
#define	        RW_TR50I2_94   10	//TR50I2_94-������μ��ٶ����̵�����
#define	       RW_LK50I2_47U   11	//LK50I2_47U-������θ�ѹ�����̵�����
#define	           RW_PI50I3   12	//PI50I3-���������μ̵�����
#define	           RW_TR50I3   13	//TR50I3-������ζ����̵�����
#define	          RW_LPC50I3   14	//LPC50I3-�������ѭ�����Ƽ̵�����
#define	        RW_TR50I3_94   15	//TR50I3_94-������μ��ٶ����̵�����
#define	       RW_LK50I3_47U   16	//LK50I3_47U-������θ�ѹ�����̵�����
#define	              RW_APF   17	//APF-A�๦������̵�����
#define	              RW_APR   18	//APR-A�๦�ʷ���̵�����
#define	              RW_BPF   19	//BPF-B�๦������̵�����
#define	              RW_BPR   20	//BPR-B�๦�ʷ���̵�����
#define	              RW_CPF   21	//CPF-C�๦������̵�����
#define	              RW_CPR   22	//CPR-C�๦�ʷ���̵�����
#define	           RW_PI50N1   23	//PI50N1-���������μ̵�����
#define	           RW_TR50N1   24	//TR50N1-������ζ����̵�����
#define	          RW_LPC50N1   25	//LPC50N1-�������ѭ�����Ƽ̵�����
#define	           RW_PI50N2   26	//PI50N2-���������μ̵�����
#define	           RW_TR50N2   27	//TR50N2-������ζ����̵�����
#define	          RW_LPC50N2   28	//LPC50N2-�������ѭ�����Ƽ̵�����
#define	            RW_PI27U   29	//PI27U-��ѹ������μ̵�����
#define	            RW_TR27U   30	//TR27U-��ѹ���������̵�����
#define	           RW_LPC27U   31	//LPC27U-��ѹ����ѭ�����Ƽ̵�����
#define	            RW_PIPT1   32	//PIPT1-ĸ��PT������μ̵�����
#define	            RW_ARPT1   33	//ARPT1-ĸ��PT���߸澯�̵�����
#define	            RW_PIPT2   34	//PIPT2-��·PT������μ̵�����
#define	            RW_ARPT2   35	//ARPT2-��·PT���߸澯�̵�����
#define	            RW_PI59N   36	//PI59N-��ѹ��μ̵�����
#define	            RW_TR59N   37	//TR59N-��ѹ�����̵�����
#define	           RW_LPC59N   38	//LPC59N-��ѹ����ѭ�����Ƽ̵�����
#define	           RW_PI50I0   39	//PI50I0-�����ٶ���μ̵�����
#define	           RW_TR50I0   40	//TR50I0-�����ٶ϶����̵�����
#define	          RW_LPC50I0   41	//LPC50I0-�����ٶ�ѭ�����Ƽ̵�����
#define	       RW_LK50I0_47U   42	//LK50I0_47U-�����ٶϸ�ѹ�����̵�����
#define	          RW_PI50IOL   43	//PI50IOL-��������μ̵�����
#define	          RW_TR50IOL   44	//TR50IOL-�����ɶ����̵�����
#define	         RW_LPC50IOL   45	//LPC50IOL-�����ɱ���ѭ�����Ƽ̵�����
#define	          RW_PI50ISH   46	//PI50ISH-ĸ�䱣����μ̵�����
#define	          RW_TR50ISH   47	//TR50ISH-ĸ�䱣�������̵�����
#define	         RW_LPC50ISH   48	//LPC50ISH-ĸ�䱣��ѭ�����Ƽ̵�����
#define	             RW_59UA   49	//59UA-UA��ѹ�̵�����
#define	             RW_27UA   50	//27UA-UA��ѹ�̵�����
#define	             RW_59UB   51	//59UB-UB��ѹ�̵�����
#define	             RW_27UB   52	//27UB-UB��ѹ�̵�����
#define	             RW_59UC   53	//59UC-UC��ѹ�̵�����
#define	             RW_27UC   54	//27UC-UC��ѹ�̵�����
#define	             RW_59U0   55	//59U0-U0��ѹ�̵�����
#define	             RW_27U0   56	//27U0-U0��ѹ�̵�����
#define	             RW_59U1   57	//59U1-U1��ѹ�̵�����
#define	             RW_27U1   58	//27U1-U1��ѹ�̵�����
#define	             RW_59U2   59	//59U2-U2��ѹ�̵�����
#define	             RW_27U2   60	//27U2-U2��ѹ�̵�����
#define	             RW_59U3   61	//59U3-U3��ѹ�̵�����
#define	             RW_27U3   62	//27U3-U3��ѹ�̵�����
#define	             RW_50IA   63	//50IA-IA�����̵�����
#define	             RW_37IA   64	//37IA-IA�����̵�����
#define	             RW_50IB   65	//50IB-IB�����̵�����
#define	             RW_37IB   66	//37IB-IB�����̵�����
#define	             RW_50IC   67	//50IC-IC�����̵�����
#define	             RW_37IC   68	//37IC-IC�����̵�����
#define	             RW_50I0   69	//50I0-I0�����̵�����
#define	             RW_37I0   70	//37I0-I0�����̵�����
#define	             RW_50I1   71	//50I1-I1�����̵�����
#define	             RW_37I1   72	//37I1-I1�����̵�����
#define	            RW_79SH0   73	//79SH0-�غ�բ����״̬�̵�����һ
#define	            RW_79SH1   74	//79SH1-�غ�բ����״̬�̵����ֶ�
#define	             RW_79TO   75	//79TO-�غ�բ��ʱ���߼��̵�����
#define	            RW_79CYS   76	//79CYS-�غ�բ����״̬�̵�����
#define	            RW_79RSS   77	//79RSS-�غ�բ����״̬�̵�����
#define	            RW_79LOS   78	//79LOS-�غ�բ����״̬�̵�����
#define	             RW_79CL   79	//79CL-�غ�բ�����߼��̵�����
#define	           RW_PICKUP   80	//PICKUP-ģֵͻ���������̵�����
#define	          RW_ET11ERR   81	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   82	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   83	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   84	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   85	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   86	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   87	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   88	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   89	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   90	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   91	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   92	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   93	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   94	//NULL1ERR-������1״̬�̵�����
#define	         RW_NULL2ERR   95	//NULL2ERR-������2״̬�̵�����
#define	         RW_NULL3ERR   96	//NULL3ERR-������3״̬�̵�����
#define	             RW_25CL   97	//25CL-��ͬ�ڶ����̵�����
#define	          RW_25_27UM   98	//25_27UM-ĸ�ߵ�ѹС�ڼ���ѹ��ֵ�̵�����
#define	          RW_25_27UL   99	//25_27UL-��·��ѹС�ڼ���ѹ��ֵ�̵�����
#define	         RW_PICTBR_A  100	//PICTBR_A-A��CT������μ̵�����
#define	         RW_ARCTBR_A  101	//ARCTBR_A-A��CT���߸澯�̵�����
#define	         RW_PICTBR_B  102	//PICTBR_B-B��CT������μ̵�����
#define	         RW_ARCTBR_B  103	//ARCTBR_B-B��CT���߸澯�̵�����
#define	         RW_PICTBR_C  104	//PICTBR_C-C��CT������μ̵�����
#define	         RW_ARCTBR_C  105	//ARCTBR_C-C��CT���߸澯�̵�����
#define	        RW_MAINTLINK  106	//MAINTLINK-ά��״̬�̵�����
#define	               RW_M1  107	//M1-�м�̵�����M1
#define	               RW_M2  108	//M2-�м�̵�����M2
#define	               RW_M3  109	//M3-�м�̵�����M3
#define	               RW_M4  110	//M4-�м�̵�����M4
#define	               RW_M5  111	//M5-�м�̵�����M5
#define	               RW_M6  112	//M6-�м�̵�����M6
#define	               RW_M7  113	//M7-�м�̵�����M7
#define	               RW_M8  114	//M8-�м�̵�����M8
#define	               RW_M9  115	//M9-�м�̵�����M9
#define	              RW_M10  116	//M10-�м�̵�����M10
#define	              RW_M11  117	//M11-�м�̵�����M11
#define	              RW_M12  118	//M12-�м�̵�����M12
#define	              RW_M13  119	//M13-�м�̵�����M13
#define	              RW_M14  120	//M14-�м�̵�����M14
#define	              RW_M15  121	//M15-�м�̵�����M15
#define	              RW_M16  122	//M16-�м�̵�����M16
#define	              RW_M17  123	//M17-�м�̵�����M17
#define	              RW_M18  124	//M18-�м�̵�����M18
#define	              RW_M19  125	//M19-�м�̵�����M19
#define	              RW_M20  126	//M20-�м�̵�����M20
#define	              RW_M21  127	//M21-�м�̵�����M21
#define	              RW_M22  128	//M22-�м�̵�����M22
#define	              RW_M23  129	//M23-�м�̵�����M23
#define	              RW_M24  130	//M24-�м�̵�����M24
#define	              RW_M25  131	//M25-�м�̵�����M25
#define	              RW_M26  132	//M26-�м�̵�����M26
#define	              RW_M27  133	//M27-�м�̵�����M27
#define	              RW_M28  134	//M28-�м�̵�����M28
#define	              RW_M29  135	//M29-�м�̵�����M29
#define	              RW_M30  136	//M30-�м�̵�����M30
#define	              RW_M31  137	//M31-�м�̵�����M31
#define	              RW_M32  138	//M32-�м�̵�����M32
#define	              RW_M33  139	//M33-�м�̵�����M33
#define	              RW_M34  140	//M34-�м�̵�����M34
#define	              RW_M35  141	//M35-�м�̵�����M35
#define	              RW_M36  142	//M36-�м�̵�����M36
#define	              RW_M37  143	//M37-�м�̵�����M37
#define	              RW_M38  144	//M38-�м�̵�����M38
#define	              RW_M39  145	//M39-�м�̵�����M39
#define	              RW_M40  146	//M40-�м�̵�����M40
#define	              RW_M41  147	//M41-�м�̵�����M41
#define	              RW_M42  148	//M42-�м�̵�����M42
#define	              RW_M43  149	//M43-�м�̵�����M43
#define	              RW_M44  150	//M44-�м�̵�����M44
#define	              RW_M45  151	//M45-�м�̵�����M45
#define	              RW_M46  152	//M46-�м�̵�����M46
#define	              RW_M47  153	//M47-�м�̵�����M47
#define	              RW_M48  154	//M48-�м�̵�����M48
#define	              RW_M49  155	//M49-�м�̵�����M49
#define	              RW_M50  156	//M50-�м�̵�����M50
#define	              RW_M51  157	//M51-�м�̵�����M51
#define	              RW_M52  158	//M52-�м�̵�����M52
#define	              RW_M53  159	//M53-�м�̵�����M53
#define	              RW_M54  160	//M54-�м�̵�����M54
#define	              RW_M55  161	//M55-�м�̵�����M55
#define	              RW_M56  162	//M56-�м�̵�����M56
#define	              RW_M57  163	//M57-�м�̵�����M57
#define	              RW_M58  164	//M58-�м�̵�����M58
#define	              RW_M59  165	//M59-�м�̵�����M59
#define	              RW_M60  166	//M60-�м�̵�����M60
#define	              RW_M61  167	//M61-�м�̵�����M61
#define	              RW_M62  168	//M62-�м�̵�����M62
#define	              RW_M63  169	//M63-�м�̵�����M63
#define	              RW_M64  170	//M64-�м�̵�����M64
#define	         RW_LKPI50I1  171	//LKPI50I1-���������α����̵�����
#define	        RW_LKRST50I1  172	//LKRST50I1-������η��ر����̵�����
#define	         RW_LKPI50I2  173	//LKPI50I2-���������α����̵�����
#define	        RW_LKRST50I2  174	//LKRST50I2-������η��ر����̵�����
#define	         RW_LKPI50I3  175	//LKPI50I3-���������α����̵�����
#define	        RW_LKRST50I3  176	//LKRST50I3-������η��ر����̵�����
#define	         RW_LKPI50N1  177	//LKPI50N1-���������α����̵�����
#define	        RW_LKRST50N1  178	//LKRST50N1-������η��ر����̵�����
#define	         RW_LKPI50N2  179	//LKPI50N2-���������α����̵�����
#define	        RW_LKRST50N2  180	//LKRST50N2-������η��ر����̵�����
#define	           RW_LKPI27  181	//LKPI27-ʧѹ������α����̵�����
#define	          RW_LKRST27  182	//LKRST27-ʧѹ�������ر����̵�����
#define	         RW_LKPI50I0  183	//LKPI50I0-�����ٶ���α����̵�����
#define	        RW_LKRST50I0  184	//LKRST50I0-�����ٶϷ��ر����̵�����
#define	        RW_LKPI50IOL  185	//LKPI50IOL-��������α����̵�����
#define	       RW_LKRST50IOL  186	//LKRST50IOL-�����ɷ��ر����̵�����
#define	        RW_LKPI50ISH  187	//LKPI50ISH-ĸ�䱣����α����̵�����
#define	       RW_LKRST50ISH  188	//LKRST50ISH-ĸ�䱣�����ر����̵�����
#define	            RW_79PIS  189	//79PIS-�غ�բ���������߼��̵�����
#define	            RW_79CLC  190	//79CLC-�غ�բ����������̵�����
#define	             RW_79LK  191	//79LK-�غ�բ�����߼��̵�����
#define	            RW_79CLS  192	//79CLS-�غ�բ���������߼��̵�����
#define	            RW_CFG79  193	//CFG79-�غ�բ���ü̵�����
#define	           RW_79TRCK  194	//79TRCK-�غ�բ�����жϼ̵�����
#define	             RW_69IN  195	//69IN-Զ�������м�̵�����
#define	              RW_52A  196	//52A-��λ
#define	              RW_52B  197	//52B-��λ
#define	              RW_REC  198	//REC-¼���̵�����
#define	             RW_25LK  199	//25LK-��ͬ�ڱ�����բ�̵�����
#define	            RW_25INT  200	//25INT-��ͬ���ж��߼��̵�����
#define	               RW_H1  201	//H1-�Ա����м�̵�����1
#define	               RW_H2  202	//H2-�Ա����м�̵�����2
#define	               RW_H3  203	//H3-�Ա����м�̵�����3
#define	               RW_H4  204	//H4-�Ա����м�̵�����4
#define	               RW_H5  205	//H5-�Ա����м�̵�����5
#define	               RW_H6  206	//H6-�Ա����м�̵�����6
#define	               RW_H7  207	//H7-�Ա����м�̵�����7
#define	               RW_H8  208	//H8-�Ա����м�̵�����8
#define	               RW_H9  209	//H9-�Ա����м�̵�����9
#define	              RW_H10  210	//H10-�Ա����м�̵�����10
#define	              RW_H11  211	//H11-�Ա����м�̵�����11
#define	              RW_H12  212	//H12-�Ա����м�̵�����12
#define	              RW_H13  213	//H13-�Ա����м�̵�����13
#define	              RW_H14  214	//H14-�Ա����м�̵�����14
#define	              RW_H15  215	//H15-�Ա����м�̵�����15
#define	              RW_H16  216	//H16-�Ա����м�̵�����16
#define	              RW_H17  217	//H17-�Ա����м�̵�����17
#define	              RW_H18  218	//H18-�Ա����м�̵�����18
#define	              RW_H19  219	//H19-�Ա����м�̵�����19
#define	              RW_H20  220	//H20-�Ա����м�̵�����20
#define	              RW_H21  221	//H21-�Ա����м�̵�����21
#define	              RW_H22  222	//H22-�Ա����м�̵�����22
#define	              RW_H23  223	//H23-�Ա����м�̵�����23
#define	              RW_H24  224	//H24-�Ա����м�̵�����24
#define	              RW_H25  225	//H25-�Ա����м�̵�����25
#define	              RW_H26  226	//H26-�Ա����м�̵�����26
#define	              RW_H27  227	//H27-�Ա����м�̵�����27
#define	              RW_H28  228	//H28-�Ա����м�̵�����28
#define	              RW_H29  229	//H29-�Ա����м�̵�����29
#define	              RW_H30  230	//H30-�Ա����м�̵�����30
#define	              RW_H31  231	//H31-�Ա����м�̵�����31
#define	              RW_H32  232	//H32-�Ա����м�̵�����31
#define	             RW_79PI  233	//79PI-�غ�բ�����߼��̵�����
#define	             RW_52BF  234	//52BF-��·����բʧ�̵ܼ�����
#define	              RW_PI1  235	//PI1-����Ͷ�����̵�����1
#define	              RW_PI2  236	//PI2-����Ͷ�����̵�����2
#define	              RW_PI3  237	//PI3-����Ͷ�����̵�����3
#define	              RW_PI4  238	//PI4-����Ͷ�����̵�����4
#define	             RW_CHR1  239	//CHR1-����Ͷģʽ�̵�����1
#define	             RW_CHR2  240	//CHR2-����Ͷģʽ�̵�����2
#define	             RW_CHR3  241	//CHR3-����Ͷģʽ�̵�����3
#define	             RW_CHR4  242	//CHR4-����Ͷģʽ�̵�����4
#define	             RW_LOCK  243	//LOCK-����Ͷ�����̵�����
#define	             RW_25PI  244	//25PI-��ͬ�������̵�����
#define	          RW_79_25PI  245	//79_25PI-�غ�բ������ͬ�ڼ̵�����
#define	         RW_MCL_25PI  246	//MCL_25PI-�ֶ�������ͬ�ڼ̵�����
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
#define	            RW_TSOTF  279	//TSOTF-�غ��ڹ����߼��̵�����
#define	            RW_T52BF  280	//T52BF-��·����բʧ����ʱ�̵�����
#define	              RW_TSH  281	//TSH-ĸ�䱣��Ͷ��ʱ��̵�����
#define	            RW_TCHR1  282	//TCHR1-����Ͷ��緽ʽ�̵�����1
#define	            RW_TCHR2  283	//TCHR2-����Ͷ��緽ʽ�̵�����2
#define	            RW_TCHR3  284	//TCHR3-����Ͷ��緽ʽ�̵�����3
#define	            RW_TCHR4  285	//TCHR4-����Ͷ��緽ʽ�̵�����4
#define	              RW_TR1  286	//TR1-��բ�̵���1
#define	              RW_TR2  287	//TR2-��բ�̵���2
#define	              RW_TR3  288	//TR3-��բ�̵���3
#define	              RW_VR1  289	//VR1-����̵�����1
#define	              RW_VR2  290	//VR2-����̵�����2
#define	              RW_VR3  291	//VR3-����̵�����3
#define	              RW_VR4  292	//VR4-����̵�����4
#define	              RW_VR5  293	//VR5-����̵�����5
#define	              RW_VR6  294	//VR6-����̵�����6
#define	              RW_VR7  295	//VR7-����̵�����7
#define	              RW_VR8  296	//VR8-����̵�����8
#define	              RW_VR9  297	//VR9-����̵�����9
#define	             RW_VR10  298	//VR10-����̵�����10
#define	             RW_VR11  299	//VR11-����̵�����11
#define	             RW_VR12  300	//VR12-����̵�����12
#define	             RW_VR13  301	//VR13-����̵�����13
#define	             RW_VR14  302	//VR14-����̵�����14
#define	             RW_VR15  303	//VR15-����̵�����15
#define	             RW_VR16  304	//VR16-����̵�����16
#define	             RW_VR17  305	//VR17-����̵�����17
#define	             RW_VR18  306	//VR18-����̵�����18
#define	             RW_VR19  307	//VR19-����̵�����19
#define	             RW_VR20  308	//VR20-����̵�����20
#define	             RW_VR21  309	//VR21-����̵�����21
#define	             RW_VR22  310	//VR22-����̵�����22
#define	             RW_VR23  311	//VR23-����̵�����23
#define	             RW_VR24  312	//VR24-����̵�����24
#define	             RW_VR25  313	//VR25-����̵�����25
#define	             RW_VR26  314	//VR26-����̵�����26
#define	             RW_VR27  315	//VR27-����̵�����27
#define	             RW_VR28  316	//VR28-����̵�����28
#define	             RW_VR29  317	//VR29-����̵�����29
#define	             RW_VR30  318	//VR30-����̵�����30
#define	             RW_VR31  319	//VR31-����̵�����31
#define	             RW_VR32  320	//VR32-����̵�����32
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
#define	             RW_IN35  355	//IN35-�źŸ���
#define	             RW_IN36  356	//IN36-��բλ��
#define	             RW_IN37  357	//IN37-��բλ��
#define	             RW_IN38  358	//IN38-�����ض�
#define	             RW_IN39  359	//IN39-ң��Ԥ��
#define	             RW_IN40  360	//IN40-ң��1Ԥ��
#define	             RW_IN41  361	//IN41-ң��2Ԥ��
#define	             RW_IN42  362	//IN42-ң��3Ԥ��
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
#define	             RW_OUT1  379	//OUT1-����1�̵�����
#define	             RW_OUT2  380	//OUT2-����2�̵�����
#define	             RW_OUT3  381	//OUT3-����3�̵�����
#define	             RW_OUT4  382	//OUT4-����4�̵�����
#define	             RW_OUT5  383	//OUT5-����5�̵�����
#define	             RW_OUT6  384	//OUT6-����6�̵�����
#define	             RW_OUT7  385	//OUT7-����7�̵�����
#define	             RW_OUT8  386	//OUT8-����8�̵�����
#define	             RW_OUT9  387	//OUT9-����9�̵�����
#define	            RW_OUT10  388	//OUT10-����10�̵�����
#define	            RW_OUT11  389	//OUT11-����11�̵�����
#define	            RW_OUT12  390	//OUT12-����12�̵�����
#define	            RW_OUT13  391	//OUT13-����13�̵�����
#define	            RW_OUT14  392	//OUT14-����14�̵�����
#define	            RW_OUT15  393	//OUT15-����15�̵�����
#define	            RW_OUT16  394	//OUT16-����16�̵�����
#define	            RW_OUT17  395	//OUT17-����17�̵�����
#define	            RW_OUT18  396	//OUT18-����18�̵�����
#define	            RW_OUT19  397	//OUT19-����19�̵�����
#define	           RW_SL50I0  398	//SL50I0-�����ٶ�ѹ��̵�����
#define	           RW_SL50I1  399	//SL50I1-���������ѹ��̵�����
#define	           RW_SL50I2  400	//SL50I2-���������ѹ��̵�����
#define	           RW_SL50I3  401	//SL50I3-���������ѹ��̵�����
#define	           RW_SL50N1  402	//SL50N1-������������ѹ��̵�����
#define	           RW_SL50N2  403	//SL50N2-������������ѹ��̵�����
#define	          RW_SL50ISH  404	//SL50ISH-ĸ�߳�籣��ѹ��̵�����
#define	             RW_SL27  405	//SL27-��ѹ������ѹ��̵�����
#define	             RW_SL79  406	//SL79-�غ�բ��ѹ��̵�����
#define	             RW_SLJD  407	//SLJD-�ӵ�ѡ����ѹ��̵�����
#define	             RW_SL25  408	//SL25-��ͬ����ѹ��̵�����
#define	            RW_SLBZT  409	//SLBZT-����Ͷ��ѹ��̵�����
#define	          RW_SL50IOL  410	//SL50IOL-�����ɸ澯ѹ��̵�����
#define	          RW_SLMAINT  411	//SLMAINT-����ѹ��̵�����
#define	           RW_SLBAK1  412	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  413	//SLBAK2-����ѹ��2
#define	            RW_F50I0  414	//F50I0-�����ٶ����ü̵�����
#define	         RW_F50I0_47  415	//F50I0_47-�����ٶϸ�ѹ�������ü̵�����
#define	         RW_F50I0_67  416	//F50I0_67-�����ٶϹ��ʷ������ü̵�����
#define	         RW_F50I0_PD  417	//F50I0_PD-�����ٶϷ����������ü̵�����
#define	            RW_F50I1  418	//F50I1-����I�����ü̵�����
#define	         RW_F50I1_47  419	//F50I1_47-������θ�ѹ�������ü̵�����
#define	         RW_F50I1_67  420	//F50I1_67-������ι��ʷ������ü̵�����
#define	         RW_F50I1_PD  421	//F50I1_PD-��������������ü̵�����
#define	            RW_F50I2  422	//F50I2-����������ü̵�����
#define	         RW_F50I2_47  423	//F50I2_47-������θ�ѹ�������ü̵�����
#define	         RW_F50I2_67  424	//F50I2_67-������ι��ʷ������ü̵�����
#define	         RW_F50I2_PD  425	//F50I2_PD-��������������ü̵�����
#define	            RW_F50I3  426	//F50I3-����������ü̵�����
#define	         RW_F50I3_47  427	//F50I3_47-������θ�ѹ�������ü̵�����
#define	         RW_F50I3_67  428	//F50I3_67-������ι��ʷ������ü̵�����
#define	         RW_F50I3_PD  429	//F50I3_PD-��������������ü̵�����
#define	            RW_F50N1  430	//F50N1-�������������ü̵�����
#define	            RW_F50N2  431	//F50N2-�������������ü̵�����
#define	           RW_F50ISH  432	//F50ISH-ĸ�䱣�����ü̵�����
#define	             RW_F27U  433	//F27U-��ѹ�������ü̵�����
#define	         RW_F50I0_79  434	//F50I0_79-�����ٶ��غ����ü̵�����
#define	         RW_F50I1_79  435	//F50I1_79-��������غ����ü̵�����
#define	         RW_F50I2_79  436	//F50I2_79-��������غ����ü̵�����
#define	         RW_F50I3_79  437	//F50I3_79-��������غ����ü̵�����
#define	            RW_F79UC  438	//F79UC-͵���غ�բ���ü̵�����
#define	           RW_F79_25  439	//F79_25-�غ�բ��ͬ�����ü̵�����
#define	           RW_F79_27  440	//F79_27-�غ�բ����ѹ���ü̵�����
#define	           RW_F79_59  441	//F79_59-�غ�բ����ѹ���ü̵�����
#define	         RW_F50I1_94  442	//F50I1_94-������μ������ü̵�����
#define	         RW_F50I2_94  443	//F50I2_94-������μ������ü̵�����
#define	         RW_F50I3_94  444	//F50I3_94-������μ������ü̵�����
#define	          RW_F59N_74  445	//F59N_74-��ѹ�澯���ü̵�����
#define	            RW_FJDXX  446	//FJDXX-�ӵ�ѡ�����ü̵�����
#define	            RW_FXHXQ  447	//FXHXQ-������Ȧ�ӵ����ü̵�����
#define	            RW_F74PT  448	//F74PT-PT���߼�����ü̵�����
#define	              RW_F25  449	//F25-�ֺϼ�ͬ�����ü̵�����
#define	           RW_FSH_27  450	//FSH_27-�ֺϼ���ѹ���ü̵�����
#define	             RW_FBZT  451	//FBZT-����Ͷ���ü̵�����
#define	           RW_F50IOL  452	//F50IOL-�����ɸ澯�̵�����
#define	            RW_FCTBR  453	//FCTBR-CT���߼�����ü̵�����
#define	            RW_F52BF  454	//F52BF-ʧ�鱣�����ü̵�����
#define	            RW_FBAK1  455	//FBAK1-����1���ü̵�����
#define	            RW_FBAK2  456	//FBAK2-����2���ü̵�����
#define	              RW_PW1  457	//PW1-����̵���1
#define	              RW_PW2  458	//PW2-����̵���2
#define	              RW_PW3  459	//PW3-����̵���3
#define	              RW_PW4  460	//PW4-����̵���4
#define	              RW_PW5  461	//PW5-����̵���5
#define	              RW_PW6  462	//PW6-����̵���6
#define	              RW_PW7  463	//PW7-����̵���7
#define	              RW_PW8  464	//PW8-����̵���8
#define	              RW_PW9  465	//PW9-����̵���9
#define	             RW_PW10  466	//PW10-����̵���10
#define	             RW_PW11  467	//PW11-����̵���11
#define	             RW_PW12  468	//PW12-����̵���12
#define	             RW_PW13  469	//PW13-����̵���13
#define	             RW_PW14  470	//PW14-����̵���14
#define	             RW_PW15  471	//PW15-����̵���15
#define	             RW_PW16  472	//PW16-����̵���16
#define	              RW_TO1  473	//TO1-�����̵���1
#define	              RW_TO2  474	//TO2-�����̵���2
#define	              RW_TO3  475	//TO3-�����̵���3
#define	              RW_TO4  476	//TO4-�����̵���4
#define	              RW_TO5  477	//TO5-�����̵���5
#define	              RW_TO6  478	//TO6-�����̵���6
#define	              RW_TO7  479	//TO7-�����̵���7
#define	              RW_TO8  480	//TO8-�����̵���8
#define	              RW_TO9  481	//TO9-�����̵���9
#define	             RW_TO10  482	//TO10-�����̵���10
#define	             RW_TO11  483	//TO11-�����̵���11
#define	             RW_SIG1  484	//SIG1-�źż̵���1
#define	             RW_SIG2  485	//SIG2-�źż̵���2
#define	             RW_SIG3  486	//SIG3-�źż̵���3
#define	             RW_SIG4  487	//SIG4-�źż̵���4
#define	             RW_SIG5  488	//SIG5-�źż̵���5
#define	             RW_SIG6  489	//SIG6-�źż̵���6
#define	             RW_SIG7  490	//SIG7-�źż̵���7
#define	             RW_SIG8  491	//SIG8-�źż̵���8
#define	             RW_SIG9  492	//SIG9-�źż̵���9
#define	            RW_SIG10  493	//SIG10-�źż̵���10
#define	            RW_SIG11  494	//SIG11-�źż̵���11
#define	            RW_SIG12  495	//SIG12-�źż̵���12
#define	            RW_SIG13  496	//SIG13-�źż̵���13
#define	            RW_SIG14  497	//SIG14-�źż̵���14
#define	            RW_SIG15  498	//SIG15-�źż̵���15
#define	            RW_SIG16  499	//SIG16-�źż̵���16
#define	            RW_LED1G  500	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  501	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  502	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  503	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  504	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  505	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  506	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  507	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  508	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  509	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  510	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  511	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  512	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  513	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  514	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  515	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  516	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  517	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  518	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  519	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  520	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  521	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  522	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  523	//LED12R-LED12��Ƽ̵�����
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
#define	             RW_RCP1  540	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  541	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  542	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  543	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  544	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  545	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  546	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  547	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  548	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  549	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  550	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  551	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  552	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  553	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  554	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  555	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  556	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  557	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  558	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  559	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  560	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  561	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  562	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  563	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  564	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  565	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  566	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  567	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  568	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  569	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  570	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  571	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  572	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  573	//RCP34-ң��Ԥ�ü̵���34
#define	             RW_RCE1  574	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  575	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  576	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  577	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  578	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  579	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  580	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  581	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  582	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  583	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  584	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  585	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  586	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  587	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  588	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  589	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  590	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  591	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  592	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  593	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  594	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  595	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  596	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  597	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  598	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  599	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  600	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  601	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  602	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  603	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  604	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  605	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  606	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  607	//RCE34-ң��ִ�м̵���34
#define	             RW_RCV1  608	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  609	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  610	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  611	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  612	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  613	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  614	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  615	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  616	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  617	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  618	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  619	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  620	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  621	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  622	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  623	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  624	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  625	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  626	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  627	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  628	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  629	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  630	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  631	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  632	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  633	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  634	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  635	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  636	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  637	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  638	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  639	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  640	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  641	//RCV34-ң�ط�У�̵���34
#define	              RW_GE1  642	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  643	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  644	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  645	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  646	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  647	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  648	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  649	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  650	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  651	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  652	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  653	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  654	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  655	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  656	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  657	//CONST8-����ʱ��̵���8
#define	              RW_TX1  658	//TX1-�����ͼ̵�����1
#define	              RW_TX2  659	//TX2-�����ͼ̵�����2
#define	              RW_TX3  660	//TX3-�����ͼ̵�����3
#define	              RW_TX4  661	//TX4-�����ͼ̵�����4
#define	              RW_TX5  662	//TX5-�����ͼ̵�����5
#define	              RW_TX6  663	//TX6-�����ͼ̵�����6
#define	              RW_TX7  664	//TX7-�����ͼ̵�����7
#define	              RW_TX8  665	//TX8-�����ͼ̵�����8
#define	              RW_TX9  666	//TX9-�����ͼ̵�����9
#define	             RW_TX10  667	//TX10-�����ͼ̵�����10
#define	             RW_TX11  668	//TX11-�����ͼ̵�����11
#define	             RW_TX12  669	//TX12-�����ͼ̵�����12
#define	             RW_TX13  670	//TX13-�����ͼ̵�����13
#define	             RW_TX14  671	//TX14-�����ͼ̵�����14
#define	             RW_TX15  672	//TX15-�����ͼ̵�����15
#define	             RW_TX16  673	//TX16-�����ͼ̵�����16
#define	              RW_RX1  674	//RX1-������ռ̵�����1
#define	              RW_RX2  675	//RX2-������ռ̵�����2
#define	              RW_RX3  676	//RX3-������ռ̵�����3
#define	              RW_RX4  677	//RX4-������ռ̵�����4
#define	              RW_RX5  678	//RX5-������ռ̵�����5
#define	              RW_RX6  679	//RX6-������ռ̵�����6
#define	              RW_RX7  680	//RX7-������ռ̵�����7
#define	              RW_RX8  681	//RX8-������ռ̵�����8
#define	              RW_RX9  682	//RX9-������ռ̵�����9
#define	             RW_RX10  683	//RX10-������ռ̵�����10
#define	             RW_RX11  684	//RX11-������ռ̵�����11
#define	             RW_RX12  685	//RX12-������ռ̵�����12
#define	             RW_RX13  686	//RX13-������ռ̵�����13
#define	             RW_RX14  687	//RX14-������ռ̵�����14
#define	             RW_RX15  688	//RX15-������ռ̵�����15
#define	             RW_RX16  689	//RX16-������ռ̵�����16
#define	             RW_CNT1  690	//CNT1-�����̵�����1
#define	             RW_CNT2  691	//CNT2-�����̵�����2
#define	             RW_CNT3  692	//CNT3-�����̵�����3
#define	             RW_CNT4  693	//CNT4-�����̵�����4
#define	             RW_CNT5  694	//CNT5-�����̵�����5
#define	             RW_CNT6  695	//CNT6-�����̵�����6
#define	             RW_CNT7  696	//CNT7-�����̵�����7
#define	             RW_CNT8  697	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			698		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			105	

//�м�̵�����
#define	MID_RELAY_WORD_START			107	
#define	MID_RELAY_WORD_NUM			94	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			201	
#define	KEEP_RELAY_WORD_NUM			46	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			247	
#define	TIME_RELAY_WORD_NUM			39	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			286	
#define	TRIP_RELAY_WORD_NUM			3	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			289	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			321	
#define	INPUT_RELAY_WORD_NUM			58	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			379	
#define	OUTPUT_RELAY_WORD_NUM			19	

//ѹ��̵���
#define	SW_RELAY_WORD_START			398	
#define	SW_RELAY_WORD_NUM			16	

//���ü̵���
#define	CFG_RELAY_WORD_START			414	
#define	CFG_RELAY_WORD_NUM			43	

//����̵���
#define	PULSE_RELAY_WORD_START			457	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			473	
#define	CTRLTEST_RELAY_WORD_NUM			11	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			484	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			500	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			524	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			540	
#define	RCP_RELAY_WORD_NUM			34	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			574	
#define	RCE_RELAY_WORD_NUM			34	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			608	
#define	RCV_RELAY_WORD_NUM			34	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			642	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			650	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			658	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			674	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			690	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			690	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			690	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			690	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			690	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			690	
#define	CNT_RELAY_WORD_NUM			8	


#endif