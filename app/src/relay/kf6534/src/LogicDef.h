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
#define	        RW_PI60I_AT1    2	//PI60I_AT1-1AT������μ̵�����
#define	        RW_TR60I_AT1    3	//TR60I_AT1-1AT���������̵�����
#define	        RW_PI50T_AT1    4	//PI50T_AT1-1ATT�߹�����μ̵�����
#define	        RW_TR50T_AT1    5	//TR50T_AT1-1ATT�߹��������̵�����
#define	        RW_PI50F_AT1    6	//PI50F_AT1-1ATF�߹�����μ̵�����
#define	        RW_TR50F_AT1    7	//TR50F_AT1-1ATF�߹��������̵�����
#define	       RW_PI50CS_AT1    8	//PI50CS_AT1-1AT���ǹ�����μ̵�����
#define	       RW_TR50CS_AT1    9	//TR50CS_AT1-1AT���ǹ��������̵�����
#define	        RW_PI60I_AT2   10	//PI60I_AT2-2AT������μ̵�����
#define	        RW_TR60I_AT2   11	//TR60I_AT2-2AT���������̵�����
#define	        RW_PI50T_AT2   12	//PI50T_AT2-2ATT�߹�����μ̵�����
#define	        RW_TR50T_AT2   13	//TR50T_AT2-2ATT�߹��������̵�����
#define	        RW_PI50F_AT2   14	//PI50F_AT2-2ATF�߹�����μ̵�����
#define	        RW_TR50F_AT2   15	//TR50F_AT2-2ATF�߹��������̵�����
#define	       RW_PI50CS_AT2   16	//PI50CS_AT2-2AT���ǹ�����μ̵�����
#define	       RW_TR50CS_AT2   17	//TR50CS_AT2-2AT���ǹ��������̵�����
#define	          RW_PI74PT1   18	//PI74PT1-PT1�����μ̵�����
#define	          RW_AR74PT1   19	//AR74PT1-PT1���澯�̵�����
#define	          RW_PI74PT2   20	//PI74PT2-PT2�����μ̵�����
#define	          RW_AR74PT2   21	//AR74PT2-PT2���澯�̵�����
#define	          RW_PI74PT3   22	//PI74PT3-PT3�����μ̵�����
#define	          RW_AR74PT3   23	//AR74PT3-PT3���澯�̵�����
#define	          RW_PI74PT4   24	//PI74PT4-PT4�����μ̵�����
#define	          RW_AR74PT4   25	//AR74PT4-PT4���澯�̵�����
#define	             RW_59U1   26	//59U1-U1��ѹ
#define	             RW_27U1   27	//27U1-U1��ѹ
#define	             RW_59U2   28	//59U2-U2��ѹ
#define	             RW_27U2   29	//27U2-U2��ѹ
#define	             RW_59U3   30	//59U3-U3��ѹ
#define	             RW_27U3   31	//27U3-U3��ѹ
#define	             RW_59U4   32	//59U4-U4��ѹ
#define	             RW_27U4   33	//27U4-U4��ѹ
#define	             RW_59U5   34	//59U5-U5��ѹ
#define	             RW_27U5   35	//27U5-U5��ѹ
#define	             RW_59U6   36	//59U6-U6��ѹ
#define	             RW_27U6   37	//27U6-U6��ѹ
#define	             RW_59U7   38	//59U7-U7��ѹ
#define	             RW_27U7   39	//27U7-U7��ѹ
#define	             RW_59U8   40	//59U8-U8��ѹ
#define	             RW_27U8   41	//27U8-U8��ѹ
#define	             RW_50I1   42	//50I1-I1����
#define	             RW_37I1   43	//37I1-I1����
#define	             RW_50I2   44	//50I2-I2����
#define	             RW_37I2   45	//37I2-I2����
#define	             RW_50I3   46	//50I3-I3����
#define	             RW_37I3   47	//37I3-I3����
#define	             RW_50I4   48	//50I4-I4����
#define	             RW_37I4   49	//37I4-I4����
#define	             RW_50I5   50	//50I5-I5����
#define	             RW_37I5   51	//37I5-I5����
#define	             RW_50I6   52	//50I6-I6����
#define	             RW_37I6   53	//37I6-I6����
#define	             RW_50I7   54	//50I7-I7����
#define	             RW_37I7   55	//37I7-I7����
#define	             RW_50I8   56	//50I8-I8����
#define	             RW_37I8   57	//37I8-I8����
#define	       RW_LPC60I_AT1   58	//LPC60I_AT1-1AT��������ѭ���̵�����
#define	       RW_LPC50T_AT1   59	//LPC50T_AT1-1ATT�߹���ѭ���̵�����
#define	       RW_LPC50F_AT1   60	//LPC50F_AT1-1ATF�߹���ѭ���̵�����
#define	      RW_LPC50CS_AT1   61	//LPC50CS_AT1-1AT���ǹ���ѭ���̵�����
#define	       RW_LPC60I_AT2   62	//LPC60I_AT2-2AT��������ѭ���̵�����
#define	       RW_LPC50T_AT2   63	//LPC50T_AT2-2ATT�߹���ѭ���̵�����
#define	       RW_LPC50F_AT2   64	//LPC50F_AT2-2ATF�߹���ѭ���̵�����
#define	      RW_LPC50CS_AT2   65	//LPC50CS_AT2-2AT���ǹ���ѭ���̵�����
#define	       RW_PICKUP_AT1   66	//PICKUP_AT1-AT1���������̵�����
#define	       RW_PICKUP_AT2   67	//PICKUP_AT2-AT2���������̵�����
#define	          RW_ET11ERR   68	//ET11ERR-��̫��1����1״̬�̵�����
#define	          RW_ET12ERR   69	//ET12ERR-��̫��1����2״̬�̵�����
#define	          RW_ET13ERR   70	//ET13ERR-��̫��1����3״̬�̵�����
#define	          RW_ET14ERR   71	//ET14ERR-��̫��1����4״̬�̵�����
#define	          RW_ET15ERR   72	//ET15ERR-��̫��1����5״̬�̵�����
#define	          RW_ET21ERR   73	//ET21ERR-��̫��2����1״̬�̵�����
#define	          RW_ET22ERR   74	//ET22ERR-��̫��2����2״̬�̵�����
#define	          RW_ET23ERR   75	//ET23ERR-��̫��2����3״̬�̵�����
#define	          RW_ET24ERR   76	//ET24ERR-��̫��2����4״̬�̵�����
#define	          RW_ET25ERR   77	//ET25ERR-��̫��2����5״̬�̵�����
#define	          RW_COM1ERR   78	//COM1ERR-����1����״̬�̵�����
#define	          RW_CAN1ERR   79	//CAN1ERR-CAN��1����״̬�̵�����
#define	          RW_CAN2ERR   80	//CAN2ERR-CAN��2����״̬�̵�����
#define	         RW_NULL1ERR   81	//NULL1ERR-��1����״̬�̵�����
#define	         RW_NULL2ERR   82	//NULL2ERR-��2����״̬�̵�����
#define	         RW_NULL3ERR   83	//NULL3ERR-��3����״̬�̵�����
#define	        RW_MAINTLINK   84	//MAINTLINK-ά��״̬�̵�����
#define	              RW_REC   85	//REC-¼���̵�����
#define	               RW_M1   86	//M1-�м�̵���1
#define	               RW_M2   87	//M2-�м�̵���2
#define	               RW_M3   88	//M3-�м�̵���3
#define	               RW_M4   89	//M4-�м�̵���4
#define	               RW_M5   90	//M5-�м�̵���5
#define	               RW_M6   91	//M6-�м�̵���6
#define	               RW_M7   92	//M7-�м�̵���7
#define	               RW_M8   93	//M8-�м�̵���8
#define	               RW_M9   94	//M9-�м�̵���9
#define	              RW_M10   95	//M10-�м�̵���10
#define	              RW_M11   96	//M11-�м�̵���11
#define	              RW_M12   97	//M12-�м�̵���12
#define	              RW_M13   98	//M13-�м�̵���13
#define	              RW_M14   99	//M14-�м�̵���14
#define	              RW_M15  100	//M15-�м�̵���15
#define	              RW_M16  101	//M16-�м�̵���16
#define	              RW_M17  102	//M17-�м�̵���17
#define	              RW_M18  103	//M18-�м�̵���18
#define	              RW_M19  104	//M19-�м�̵���19
#define	              RW_M20  105	//M20-�м�̵���20
#define	              RW_M21  106	//M21-�м�̵���21
#define	              RW_M22  107	//M22-�м�̵���22
#define	              RW_M23  108	//M23-�м�̵���23
#define	              RW_M24  109	//M24-�м�̵���24
#define	              RW_M25  110	//M25-�м�̵���25
#define	              RW_M26  111	//M26-�м�̵���26
#define	              RW_M27  112	//M27-�м�̵���27
#define	              RW_M28  113	//M28-�м�̵���28
#define	              RW_M29  114	//M29-�м�̵���29
#define	              RW_M30  115	//M30-�м�̵���30
#define	              RW_M31  116	//M31-�м�̵���31
#define	              RW_M32  117	//M32-�м�̵���32
#define	              RW_M33  118	//M33-�м�̵���33
#define	              RW_M34  119	//M34-�м�̵���34
#define	              RW_M35  120	//M35-�м�̵���35
#define	              RW_M36  121	//M36-�м�̵���36
#define	              RW_M37  122	//M37-�м�̵���37
#define	              RW_M38  123	//M38-�м�̵���38
#define	              RW_M39  124	//M39-�м�̵���39
#define	              RW_M40  125	//M40-�м�̵���40
#define	              RW_M41  126	//M41-�м�̵���41
#define	              RW_M42  127	//M42-�м�̵���42
#define	              RW_M43  128	//M43-�м�̵���43
#define	              RW_M44  129	//M44-�м�̵���44
#define	              RW_M45  130	//M45-�м�̵���45
#define	              RW_M46  131	//M46-�м�̵���46
#define	              RW_M47  132	//M47-�м�̵���47
#define	              RW_M48  133	//M48-�м�̵���48
#define	              RW_M49  134	//M49-�м�̵���49
#define	              RW_M50  135	//M50-�м�̵���50
#define	              RW_M51  136	//M51-�м�̵���51
#define	              RW_M52  137	//M52-�м�̵���52
#define	              RW_M53  138	//M53-�м�̵���53
#define	              RW_M54  139	//M54-�м�̵���54
#define	              RW_M55  140	//M55-�м�̵���55
#define	              RW_M56  141	//M56-�м�̵���56
#define	              RW_M57  142	//M57-�м�̵���57
#define	              RW_M58  143	//M58-�м�̵���58
#define	              RW_M59  144	//M59-�м�̵���59
#define	              RW_M60  145	//M60-�м�̵���60
#define	              RW_M61  146	//M61-�м�̵���61
#define	              RW_M62  147	//M62-�м�̵���62
#define	              RW_M63  148	//M63-�м�̵���63
#define	              RW_M64  149	//M64-�м�̵���64
#define	              RW_M65  150	//M65-�м�̵���65
#define	              RW_M66  151	//M66-�м�̵���66
#define	              RW_M67  152	//M67-�м�̵���67
#define	              RW_M68  153	//M68-�м�̵���68
#define	              RW_M69  154	//M69-�м�̵���69
#define	              RW_M70  155	//M70-�м�̵���70
#define	              RW_M71  156	//M71-�м�̵���71
#define	              RW_M72  157	//M72-�м�̵���72
#define	              RW_M73  158	//M73-�м�̵���73
#define	              RW_M74  159	//M74-�м�̵���74
#define	              RW_M75  160	//M75-�м�̵���75
#define	              RW_M76  161	//M76-�м�̵���76
#define	              RW_M77  162	//M77-�м�̵���77
#define	              RW_M78  163	//M78-�м�̵���78
#define	              RW_M79  164	//M79-�м�̵���79
#define	              RW_M80  165	//M80-�м�̵���80
#define	              RW_M81  166	//M81-�м�̵���81
#define	              RW_M82  167	//M82-�м�̵���82
#define	              RW_M83  168	//M83-�м�̵���83
#define	              RW_M84  169	//M84-�м�̵���84
#define	              RW_M85  170	//M85-�м�̵���85
#define	              RW_M86  171	//M86-�м�̵���86
#define	              RW_M87  172	//M87-�м�̵���87
#define	              RW_M88  173	//M88-�м�̵���88
#define	              RW_M89  174	//M89-�м�̵���89
#define	              RW_M90  175	//M90-�м�̵���90
#define	              RW_M91  176	//M91-�м�̵���91
#define	              RW_M92  177	//M92-�м�̵���92
#define	              RW_M93  178	//M93-�м�̵���93
#define	              RW_M94  179	//M94-�м�̵���94
#define	              RW_M95  180	//M95-�м�̵���95
#define	              RW_M96  181	//M96-�м�̵���96
#define	              RW_M97  182	//M97-�м�̵���97
#define	              RW_M98  183	//M98-�м�̵���98
#define	              RW_M99  184	//M99-�м�̵���99
#define	             RW_M100  185	//M100-�м�̵���100
#define	             RW_M101  186	//M101-�м�̵���101
#define	             RW_M102  187	//M102-�м�̵���102
#define	             RW_M103  188	//M103-�м�̵���103
#define	             RW_M104  189	//M104-�м�̵���104
#define	             RW_M105  190	//M105-�м�̵���105
#define	             RW_M106  191	//M106-�м�̵���106
#define	             RW_M107  192	//M107-�м�̵���107
#define	             RW_M108  193	//M108-�м�̵���108
#define	             RW_M109  194	//M109-�м�̵���109
#define	             RW_M110  195	//M110-�м�̵���110
#define	             RW_M111  196	//M111-�м�̵���111
#define	             RW_M112  197	//M112-�м�̵���112
#define	             RW_M113  198	//M113-�м�̵���113
#define	             RW_M114  199	//M114-�м�̵���114
#define	             RW_M115  200	//M115-�м�̵���115
#define	             RW_M116  201	//M116-�м�̵���116
#define	             RW_M117  202	//M117-�м�̵���117
#define	             RW_M118  203	//M118-�м�̵���118
#define	             RW_M119  204	//M119-�м�̵���119
#define	             RW_M120  205	//M120-�м�̵���120
#define	             RW_M121  206	//M121-�м�̵���121
#define	             RW_M122  207	//M122-�м�̵���122
#define	             RW_M123  208	//M123-�м�̵���123
#define	             RW_M124  209	//M124-�м�̵���124
#define	             RW_M125  210	//M125-�м�̵���125
#define	             RW_M126  211	//M126-�м�̵���126
#define	             RW_M127  212	//M127-�м�̵���127
#define	             RW_M128  213	//M128-�м�̵���128
#define	          RW_INRESET  214	//INRESET-ң�Ÿ���
#define	            RW_INBZT  215	//INBZT-ң��Ͷ�뱸��Ͷ
#define	      RW_LKPI60I_AT1  216	//LKPI60I_AT1-AT1����������α����̵�����
#define	     RW_LKRST60I_AT1  217	//LKRST60I_AT1-AT1�����������ر����̵�����
#define	      RW_LKPI50T_AT1  218	//LKPI50T_AT1-AT1T�߹�����α����̵�����
#define	     RW_LKRST50T_AT1  219	//LKRST50T_AT1-AT1T�߹������ر����̵�����
#define	      RW_LKPI50F_AT1  220	//LKPI50F_AT1-AT1F�߹�����α����̵�����
#define	     RW_LKRST50F_AT1  221	//LKRST50F_AT1-AT1F�߹������ر����̵�����
#define	     RW_LKPI50CS_AT1  222	//LKPI50CS_AT1-AT1���ǹ�����α����̵�����
#define	    RW_LKRST50CS_AT1  223	//LKRST50CS_AT1-AT1���ǹ������ر����̵�����
#define	      RW_LKPI60I_AT2  224	//LKPI60I_AT2-AT2����������α����̵�����
#define	     RW_LKRST60I_AT2  225	//LKRST60I_AT2-AT2�����������ر����̵�����
#define	      RW_LKPI50T_AT2  226	//LKPI50T_AT2-AT2T�߹�����α����̵�����
#define	     RW_LKRST50T_AT2  227	//LKRST50T_AT2-AT2T�߹������ر����̵�����
#define	      RW_LKPI50F_AT2  228	//LKPI50F_AT2-AT2F�߹�����α����̵�����
#define	     RW_LKRST50F_AT2  229	//LKRST50F_AT2-AT2F�߹������ر����̵�����
#define	     RW_LKPI50CS_AT2  230	//LKPI50CS_AT2-AT2���ǹ�����α����̵�����
#define	    RW_LKRST50CS_AT2  231	//LKRST50CS_AT2-AT2���ǹ������ر����̵�����
#define	             RW_69IN  232	//69IN-����λ�����̵�����
#define	          RW_52A_AT1  233	//52A_AT1-AT1��·����λ
#define	          RW_52B_AT1  234	//52B_AT1-AT1��·����λ
#define	          RW_52A_AT2  235	//52A_AT2-AT2��·����λ
#define	          RW_52B_AT2  236	//52B_AT2-AT2��·����λ
#define	               RW_H1  237	//H1-�Ա����м�̵�����1
#define	               RW_H2  238	//H2-�Ա����м�̵�����2
#define	               RW_H3  239	//H3-�Ա����м�̵�����3
#define	               RW_H4  240	//H4-�Ա����м�̵�����4
#define	               RW_H5  241	//H5-�Ա����м�̵�����5
#define	               RW_H6  242	//H6-�Ա����м�̵�����6
#define	               RW_H7  243	//H7-�Ա����м�̵�����7
#define	               RW_H8  244	//H8-�Ա����м�̵�����8
#define	               RW_H9  245	//H9-�Ա����м�̵�����9
#define	              RW_H10  246	//H10-�Ա����м�̵�����10
#define	              RW_H11  247	//H11-�Ա����м�̵�����11
#define	              RW_H12  248	//H12-�Ա����м�̵�����12
#define	              RW_H13  249	//H13-�Ա����м�̵�����13
#define	              RW_H14  250	//H14-�Ա����м�̵�����14
#define	              RW_H15  251	//H15-�Ա����м�̵�����15
#define	              RW_H16  252	//H16-�Ա����м�̵�����16
#define	              RW_H17  253	//H17-�Ա����м�̵�����17
#define	              RW_H18  254	//H18-�Ա����м�̵�����18
#define	              RW_H19  255	//H19-�Ա����м�̵�����19
#define	              RW_H20  256	//H20-�Ա����м�̵�����20
#define	              RW_H21  257	//H21-�Ա����м�̵�����21
#define	              RW_H22  258	//H22-�Ա����м�̵�����22
#define	              RW_H23  259	//H23-�Ա����м�̵�����23
#define	              RW_H24  260	//H24-�Ա����м�̵�����24
#define	              RW_H25  261	//H25-�Ա����м�̵�����25
#define	              RW_H26  262	//H26-�Ա����м�̵�����26
#define	              RW_H27  263	//H27-�Ա����м�̵�����27
#define	              RW_H28  264	//H28-�Ա����м�̵�����28
#define	              RW_H29  265	//H29-�Ա����м�̵�����29
#define	              RW_H30  266	//H30-�Ա����м�̵�����30
#define	              RW_H31  267	//H31-�Ա����м�̵�����31
#define	              RW_H32  268	//H32-�Ա����м�̵�����32
#define	              RW_H33  269	//H33-�Ա����м�̵�����33
#define	              RW_H34  270	//H34-�Ա����м�̵�����34
#define	              RW_H35  271	//H35-�Ա����м�̵�����35
#define	              RW_H36  272	//H36-�Ա����м�̵�����36
#define	              RW_H37  273	//H37-�Ա����м�̵�����37
#define	              RW_H38  274	//H38-�Ա����м�̵�����38
#define	              RW_H39  275	//H39-�Ա����м�̵�����39
#define	              RW_H40  276	//H40-�Ա����м�̵�����40
#define	              RW_H41  277	//H41-�Ա����м�̵�����41
#define	              RW_H42  278	//H42-�Ա����м�̵�����42
#define	              RW_H43  279	//H43-�Ա����м�̵�����43
#define	              RW_H44  280	//H44-�Ա����м�̵�����44
#define	              RW_H45  281	//H45-�Ա����м�̵�����45
#define	              RW_H46  282	//H46-�Ա����м�̵�����46
#define	              RW_H47  283	//H47-�Ա����м�̵�����47
#define	              RW_H48  284	//H48-�Ա����м�̵�����48
#define	              RW_H49  285	//H49-�Ա����м�̵�����49
#define	              RW_H50  286	//H50-�Ա����м�̵�����50
#define	              RW_H51  287	//H51-�Ա����м�̵�����51
#define	              RW_H52  288	//H52-�Ա����м�̵�����52
#define	              RW_H53  289	//H53-�Ա����м�̵�����53
#define	              RW_H54  290	//H54-�Ա����м�̵�����54
#define	              RW_H55  291	//H55-�Ա����м�̵�����55
#define	              RW_H56  292	//H56-�Ա����м�̵�����56
#define	              RW_H57  293	//H57-�Ա����м�̵�����57
#define	              RW_H58  294	//H58-�Ա����м�̵�����58
#define	              RW_H59  295	//H59-�Ա����м�̵�����59
#define	              RW_H60  296	//H60-�Ա����м�̵�����60
#define	              RW_H61  297	//H61-�Ա����м�̵�����61
#define	              RW_H62  298	//H62-�Ա����м�̵�����62
#define	              RW_H63  299	//H63-�Ա����м�̵�����63
#define	              RW_H64  300	//H64-�Ա����м�̵�����64
#define	             RW_LOCK  301	//LOCK-�����̵�����
#define	             RW_CHR1  302	//CHR1-��緽ʽһ�̵�����
#define	             RW_CHR2  303	//CHR2-��緽ʽ���̵�����
#define	             RW_CHR3  304	//CHR3-��緽ʽ���̵�����
#define	             RW_CHR4  305	//CHR4-��緽ʽ�ļ̵�����
#define	             RW_CHR5  306	//CHR5-��緽ʽ��̵�����
#define	             RW_CHR6  307	//CHR6-��緽ʽ���̵�����
#define	              RW_PI1  308	//PI1-����Ͷ�����̵�����1
#define	              RW_PI2  309	//PI2-����Ͷ�����̵�����2
#define	              RW_PI3  310	//PI3-����Ͷ�����̵�����3
#define	              RW_PI4  311	//PI4-����Ͷ�����̵�����4
#define	              RW_PI5  312	//PI5-����Ͷ�����̵�����5
#define	              RW_PI6  313	//PI6-����Ͷ�����̵�����6
#define	              RW_PI7  314	//PI7-����Ͷ�����̵�����7
#define	              RW_PI8  315	//PI8-����Ͷ�����̵�����8
#define	              RW_PI9  316	//PI9-����Ͷ�����̵�����9
#define	             RW_PI10  317	//PI10-����Ͷ�����̵�����10
#define	             RW_PI11  318	//PI11-����Ͷ�����̵�����11
#define	             RW_PI12  319	//PI12-����Ͷ�����̵�����12
#define	            RW_AT1GZ  320	//AT1GZ-AT1����
#define	            RW_AT2GZ  321	//AT2GZ-AT2����
#define	         RW_52BF_AT1  322	//52BF_AT1-AT1��·����բʧ��
#define	         RW_52BF_AT2  323	//52BF_AT2-AT2��·����բʧ��
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
#define	           RW_TREBZT  420	//TREBZT-����Ͷ����Ͷ��ʱ��̵�����
#define	            RW_TCHR1  421	//TCHR1-��緽ʽ1ʱ��̵�����
#define	            RW_TCHR2  422	//TCHR2-��緽ʽ2ʱ��̵�����
#define	            RW_TCHR3  423	//TCHR3-��緽ʽ3ʱ��̵�����
#define	            RW_TCHR4  424	//TCHR4-��緽ʽ4ʱ��̵�����
#define	            RW_TCHR5  425	//TCHR5-��緽ʽ5ʱ��̵�����
#define	            RW_TCHR6  426	//TCHR6-��緽ʽ6ʱ��̵�����
#define	        RW_T52BF_AT1  427	//T52BF_AT1-��·����բʧ��ʱ��̵�����
#define	        RW_T52BF_AT2  428	//T52BF_AT2-��·����բʧ��ʱ��̵�����
#define	              RW_TR1  429	//TR1-��բ�̵���1
#define	              RW_TR2  430	//TR2-��բ�̵���2
#define	              RW_TR3  431	//TR3-��բ�̵���3
#define	              RW_TR4  432	//TR4-��բ�̵���4
#define	              RW_VR1  433	//VR1-���Լ̵���
#define	              RW_VR2  434	//VR2-����̵���2
#define	              RW_VR3  435	//VR3-����̵���3
#define	              RW_VR4  436	//VR4-����̵���4
#define	              RW_VR5  437	//VR5-����̵���5
#define	              RW_VR6  438	//VR6-����̵���6
#define	              RW_VR7  439	//VR7-����̵���7
#define	              RW_VR8  440	//VR8-����̵���8
#define	              RW_VR9  441	//VR9-����̵���9
#define	             RW_VR10  442	//VR10-����̵���10
#define	             RW_VR11  443	//VR11-����̵���11
#define	             RW_VR12  444	//VR12-����̵���12
#define	             RW_VR13  445	//VR13-����̵���13
#define	             RW_VR14  446	//VR14-����̵���14
#define	             RW_VR15  447	//VR15-����̵���15
#define	             RW_VR16  448	//VR16-����̵���16
#define	             RW_VR17  449	//VR17-����̵���17
#define	             RW_VR18  450	//VR18-����̵���18
#define	             RW_VR19  451	//VR19-����̵���19
#define	             RW_VR20  452	//VR20-����̵���20
#define	             RW_VR21  453	//VR21-����̵���21
#define	             RW_VR22  454	//VR22-����̵���22
#define	             RW_VR23  455	//VR23-����̵���23
#define	             RW_VR24  456	//VR24-����̵���24
#define	             RW_VR25  457	//VR25-����̵���25
#define	             RW_VR26  458	//VR26-����̵���26
#define	             RW_VR27  459	//VR27-����̵���27
#define	             RW_VR28  460	//VR28-����̵���28
#define	             RW_VR29  461	//VR29-����̵���29
#define	             RW_VR30  462	//VR30-����̵���30
#define	             RW_VR31  463	//VR31-����̵���31
#define	             RW_VR32  464	//VR32-����̵���32
#define	              RW_IN1  465	//IN1-����̵���1
#define	              RW_IN2  466	//IN2-����̵���2
#define	              RW_IN3  467	//IN3-����̵���3
#define	              RW_IN4  468	//IN4-����̵���4
#define	              RW_IN5  469	//IN5-����̵���5
#define	              RW_IN6  470	//IN6-����̵���6
#define	              RW_IN7  471	//IN7-����̵���7
#define	              RW_IN8  472	//IN8-����̵���8
#define	              RW_IN9  473	//IN9-����̵���9
#define	             RW_IN10  474	//IN10-����̵���10
#define	             RW_IN11  475	//IN11-����̵���11
#define	             RW_IN12  476	//IN12-����̵���12
#define	             RW_IN13  477	//IN13-����̵���13
#define	             RW_IN14  478	//IN14-����̵���14
#define	             RW_IN15  479	//IN15-����̵���15
#define	             RW_IN16  480	//IN16-����̵���16
#define	             RW_IN17  481	//IN17-����̵���17
#define	             RW_IN18  482	//IN18-����̵���18
#define	             RW_IN19  483	//IN19-����̵���19
#define	             RW_IN20  484	//IN20-����̵���20
#define	             RW_IN21  485	//IN21-����̵���21
#define	             RW_IN22  486	//IN22-����̵���22
#define	             RW_IN23  487	//IN23-1AT����˹
#define	             RW_IN24  488	//IN24-1ATѹ���ͷ�
#define	             RW_IN25  489	//IN25-1AT������բ
#define	             RW_IN26  490	//IN26-1AT����˹
#define	             RW_IN27  491	//IN27-1AT���¸澯
#define	             RW_IN28  492	//IN28-1AT�¶ȸ澯
#define	             RW_IN29  493	//IN29-2AT����˹
#define	             RW_IN30  494	//IN30-2ATѹ���ͷ�
#define	             RW_IN31  495	//IN31-2AT������բ
#define	             RW_IN32  496	//IN32-2AT����˹
#define	             RW_IN33  497	//IN33-2AT���¸澯
#define	             RW_IN34  498	//IN34-2AT�¶ȸ澯
#define	             RW_IN35  499	//IN35-�źŸ���
#define	             RW_IN36  500	//IN36-2AT��·����λ
#define	             RW_IN37  501	//IN37-2AT��·����λ
#define	             RW_IN38  502	//IN38-2AT��·��Ԥ��
#define	             RW_IN39  503	//IN39-2ATң��1Ԥ��
#define	             RW_IN40  504	//IN40-2ATң��2Ԥ��
#define	             RW_IN41  505	//IN41-1AT��·����λ
#define	             RW_IN42  506	//IN42-1AT��·����λ
#define	             RW_IN43  507	//IN43-1AT��·��Ԥ��
#define	             RW_IN44  508	//IN44-1ATң��1Ԥ��
#define	             RW_IN45  509	//IN45-1ATң��2Ԥ��
#define	             RW_IN46  510	//IN46-ң��1Ԥ��
#define	           RW_INVIN1  511	//INVIN1-����̵���47
#define	           RW_INVIN2  512	//INVIN2-����̵���48
#define	           RW_INVIN3  513	//INVIN3-����̵���48
#define	           RW_INVIN4  514	//INVIN4-����̵���50
#define	           RW_INVIN5  515	//INVIN5-����̵���51
#define	           RW_INVIN6  516	//INVIN6-����̵���52
#define	           RW_INVIN7  517	//INVIN7-����̵���53
#define	           RW_INVIN8  518	//INVIN8-����̵���54
#define	           RW_INVIN9  519	//INVIN9-����̵���55
#define	          RW_INVIN10  520	//INVIN10-����̵���56
#define	          RW_INVIN11  521	//INVIN11-����̵���57
#define	          RW_INVIN12  522	//INVIN12-����̵���58
#define	          RW_INVIN13  523	//INVIN13-����̵���59
#define	          RW_INVIN14  524	//INVIN14-����̵���60
#define	          RW_INVIN15  525	//INVIN15-����̵���61
#define	          RW_INVIN16  526	//INVIN16-����̵���62
#define	             RW_OUT1  527	//OUT1-����1�̵�����
#define	             RW_OUT2  528	//OUT2-����2�̵�����
#define	             RW_OUT3  529	//OUT3-����3�̵�����
#define	             RW_OUT4  530	//OUT4-����4�̵�����
#define	             RW_OUT5  531	//OUT5-����5�̵�����
#define	             RW_OUT6  532	//OUT6-����6�̵�����
#define	             RW_OUT7  533	//OUT7-����7�̵�����
#define	             RW_OUT8  534	//OUT8-����8�̵�����
#define	             RW_OUT9  535	//OUT9-����9�̵�����
#define	            RW_OUT10  536	//OUT10-����10�̵�����
#define	            RW_OUT11  537	//OUT11-����11�̵�����
#define	            RW_OUT12  538	//OUT12-����12�̵�����
#define	            RW_OUT13  539	//OUT13-����13�̵�����
#define	            RW_OUT14  540	//OUT14-����14�̵�����
#define	            RW_OUT15  541	//OUT15-����15�̵�����
#define	            RW_OUT16  542	//OUT16-����16�̵�����
#define	            RW_OUT17  543	//OUT17-����17�̵�����
#define	            RW_OUT18  544	//OUT18-����18�̵�����
#define	            RW_OUT19  545	//OUT19-����19�̵�����
#define	            RW_OUT20  546	//OUT20-����20�̵�����
#define	            RW_OUT21  547	//OUT21-����21�̵�����
#define	            RW_OUT22  548	//OUT22-����22�̵�����
#define	            RW_OUT23  549	//OUT23-����23�̵�����
#define	            RW_OUT24  550	//OUT24-����24�̵�����
#define	            RW_OUT25  551	//OUT25-����25�̵�����
#define	            RW_OUT26  552	//OUT26-����26�̵�����
#define	            RW_OUT27  553	//OUT27-����27�̵�����
#define	            RW_OUT28  554	//OUT28-����28�̵�����
#define	            RW_OUT29  555	//OUT29-����29�̵�����
#define	            RW_OUT30  556	//OUT30-����30�̵�����
#define	            RW_OUT31  557	//OUT31-����31�̵�����
#define	            RW_OUT32  558	//OUT32-����32�̵�����
#define	            RW_OUT33  559	//OUT33-����33�̵�����
#define	            RW_OUT34  560	//OUT34-����34�̵�����
#define	            RW_OUT35  561	//OUT35-����35�̵�����
#define	            RW_OUT36  562	//OUT36-����36�̵�����
#define	            RW_OUT37  563	//OUT37-����37�̵�����
#define	            RW_OUT38  564	//OUT38-����38�̵�����
#define	            RW_OUT39  565	//OUT39-����39�̵�����
#define	            RW_OUT40  566	//OUT40-����40�̵�����
#define	            RW_OUT41  567	//OUT41-����41�̵�����
#define	            RW_OUT42  568	//OUT42-����42�̵�����
#define	            RW_OUT43  569	//OUT43-����43�̵�����
#define	            RW_SLBZT  570	//SLBZT-����Ͷ��ѹ��
#define	          RW_SLACAT1  571	//SLACAT1-1AT�Զ�����
#define	          RW_SLACAT2  572	//SLACAT2-2AT�Զ�����
#define	        RW_SL60I_AT1  573	//SL60I_AT1-1AT��������
#define	        RW_SL50T_AT1  574	//SL50T_AT1-1ATT�߹���
#define	        RW_SL50F_AT1  575	//SL50F_AT1-1ATF�߹���
#define	       RW_SL50CS_AT1  576	//SL50CS_AT1-1AT���ǹ���
#define	        RW_SL60I_AT2  577	//SL60I_AT2-2AT��������
#define	        RW_SL50T_AT2  578	//SL50T_AT2-2ATT�߹���
#define	        RW_SL50F_AT2  579	//SL50F_AT2-2ATF�߹���
#define	       RW_SL50CS_AT2  580	//SL50CS_AT2-2AT���ǹ���
#define	          RW_SLMAINT  581	//SLMAINT-����ѹ��
#define	           RW_SLBAK1  582	//SLBAK1-����ѹ��1
#define	           RW_SLBAK2  583	//SLBAK2-����ѹ��2
#define	           RW_SLBAK3  584	//SLBAK3-����ѹ��3
#define	           RW_SLBAK4  585	//SLBAK4-����ѹ��4
#define	             RW_FBZT  586	//FBZT-����Ͷ�������ü̵�����
#define	           RW_FREBZT  587	//FREBZT-����Ͷ����Ͷ�빦�����ü̵�����
#define	           RW_FACAT1  588	//FACAT1-1AT�Զ����ƹ������ü̵�����
#define	           RW_FACAT2  589	//FACAT2-2AT�Զ����ƹ������ü̵�����
#define	         RW_F60I_AT1  590	//F60I_AT1-1AT�������ü̵�����
#define	         RW_F60I_AT2  591	//F60I_AT2-2AT�������ü̵�����
#define	         RW_F50T_AT1  592	//F50T_AT1-1ATT�߹������ü̵�����
#define	         RW_F50F_AT1  593	//F50F_AT1-1ATF�߹������ü̵�����
#define	        RW_F50CS_AT1  594	//F50CS_AT1-1AT���ǹ������ü̵�����
#define	         RW_F50T_AT2  595	//F50T_AT2-2ATT�߹������ü̵�����
#define	         RW_F50F_AT2  596	//F50F_AT2-2ATF�߹������ü̵�����
#define	        RW_F50CS_AT2  597	//F50CS_AT2-2AT���ǹ������ü̵�����
#define	           RW_F74PT1  598	//F74PT1-PT1���߼�����ü̵�����
#define	           RW_F74PT2  599	//F74PT2-PT2���߼�����ü̵�����
#define	           RW_F74PT3  600	//F74PT3-PT3���߼�����ü̵�����
#define	           RW_F74PT4  601	//F74PT4-PT4���߼�����ü̵�����
#define	            RW_FBAK1  602	//FBAK1-��������1
#define	            RW_FBAK2  603	//FBAK2-��������2
#define	            RW_FBAK3  604	//FBAK3-��������3
#define	            RW_FBAK4  605	//FBAK4-��������4
#define	              RW_PW1  606	//PW1-����̵���1
#define	              RW_PW2  607	//PW2-����̵���2
#define	              RW_PW3  608	//PW3-����̵���3
#define	              RW_PW4  609	//PW4-����̵���4
#define	              RW_PW5  610	//PW5-����̵���5
#define	              RW_PW6  611	//PW6-����̵���6
#define	              RW_PW7  612	//PW7-����̵���7
#define	              RW_PW8  613	//PW8-����̵���8
#define	              RW_PW9  614	//PW9-����̵���9
#define	             RW_PW10  615	//PW10-����̵���10
#define	             RW_PW11  616	//PW11-����̵���11
#define	             RW_PW12  617	//PW12-����̵���12
#define	             RW_PW13  618	//PW13-����̵���13
#define	             RW_PW14  619	//PW14-����̵���14
#define	             RW_PW15  620	//PW15-����̵���15
#define	             RW_PW16  621	//PW16-����̵���16
#define	              RW_TO1  622	//TO1-X4�屣����բ
#define	              RW_TO2  623	//TO2-X4��ң�ض�·����
#define	              RW_TO3  624	//TO3-X4��ң�ض�·����
#define	              RW_TO4  625	//TO4-X4��ң��1��
#define	              RW_TO5  626	//TO5-X4��ң��1��
#define	              RW_TO6  627	//TO6-X4��ң��2��
#define	              RW_TO7  628	//TO7-X4��ң��2��
#define	              RW_TO8  629	//TO8-X5�屣����բ
#define	              RW_TO9  630	//TO9-X5��ң�ض�·����
#define	             RW_TO10  631	//TO10-X5��ң�ض�·����
#define	             RW_TO11  632	//TO11-X5��ң��1��
#define	             RW_TO12  633	//TO12-X5��ң��1��
#define	             RW_TO13  634	//TO13-X5��ң��2��
#define	             RW_TO14  635	//TO14-X5��ң��2��
#define	             RW_TO15  636	//TO15-X6��ң��1��
#define	             RW_TO16  637	//TO16-X6��ң��1��
#define	             RW_TO17  638	//TO17-X6��ң��2��
#define	             RW_TO18  639	//TO18-X6��ң��2��
#define	             RW_TO19  640	//TO19-X6��ң��3��
#define	             RW_TO20  641	//TO20-X6��ң��3��
#define	             RW_TO21  642	//TO21-X6��ң��4��
#define	             RW_TO22  643	//TO22-X6��ң��4��
#define	             RW_TO23  644	//TO23-X6��ң��5��
#define	             RW_TO24  645	//TO24-X6��ң��5��
#define	             RW_TO25  646	//TO25-X6��ң��6��
#define	             RW_TO26  647	//TO26-X6��ң��6��
#define	             RW_TO27  648	//TO27-X6��ң��7��
#define	             RW_TO28  649	//TO28-X6��ң��7��
#define	             RW_SIG1  650	//SIG1-�źż̵���1
#define	             RW_SIG2  651	//SIG2-�źż̵���2
#define	             RW_SIG3  652	//SIG3-�źż̵���3
#define	             RW_SIG4  653	//SIG4-�źż̵���4
#define	             RW_SIG5  654	//SIG5-�źż̵���5
#define	             RW_SIG6  655	//SIG6-�źż̵���6
#define	             RW_SIG7  656	//SIG7-�źż̵���7
#define	             RW_SIG8  657	//SIG8-�źż̵���8
#define	             RW_SIG9  658	//SIG9-�źż̵���9
#define	            RW_SIG10  659	//SIG10-�źż̵���10
#define	            RW_SIG11  660	//SIG11-�źż̵���11
#define	            RW_SIG12  661	//SIG12-�źż̵���12
#define	            RW_SIG13  662	//SIG13-�źż̵���13
#define	            RW_SIG14  663	//SIG14-�źż̵���14
#define	            RW_SIG15  664	//SIG15-�źż̵���15
#define	            RW_SIG16  665	//SIG16-�źż̵���16
#define	            RW_LED1G  666	//LED1G-LED1�̵Ƽ̵�����
#define	            RW_LED1R  667	//LED1R-LED1��Ƽ̵�����
#define	            RW_LED2G  668	//LED2G-LED2�̵Ƽ̵�����
#define	            RW_LED2R  669	//LED2R-LED2��Ƽ̵�����
#define	            RW_LED3G  670	//LED3G-LED3�̵Ƽ̵�����
#define	            RW_LED3R  671	//LED3R-LED3��Ƽ̵�����
#define	            RW_LED4G  672	//LED4G-LED4�̵Ƽ̵�����
#define	            RW_LED4R  673	//LED4R-LED4��Ƽ̵�����
#define	            RW_LED5G  674	//LED5G-LED5�̵Ƽ̵�����
#define	            RW_LED5R  675	//LED5R-LED5��Ƽ̵�����
#define	            RW_LED6G  676	//LED6G-LED6�̵Ƽ̵�����
#define	            RW_LED6R  677	//LED6R-LED6��Ƽ̵�����
#define	            RW_LED7G  678	//LED7G-LED7�̵Ƽ̵�����
#define	            RW_LED7R  679	//LED7R-LED7��Ƽ̵�����
#define	            RW_LED8G  680	//LED8G-LED8�̵Ƽ̵�����
#define	            RW_LED8R  681	//LED8R-LED8��Ƽ̵�����
#define	            RW_LED9G  682	//LED9G-LED9�̵Ƽ̵�����
#define	            RW_LED9R  683	//LED9R-LED9��Ƽ̵�����
#define	           RW_LED10G  684	//LED10G-LED10�̵Ƽ̵�����
#define	           RW_LED10R  685	//LED10R-LED10��Ƽ̵�����
#define	           RW_LED11G  686	//LED11G-LED11�̵Ƽ̵�����
#define	           RW_LED11R  687	//LED11R-LED11��Ƽ̵�����
#define	           RW_LED12G  688	//LED12G-LED12�̵Ƽ̵�����
#define	           RW_LED12R  689	//LED12R-LED12��Ƽ̵�����
#define	             RW_VIN1  690	//VIN1-����̵���47
#define	             RW_VIN2  691	//VIN2-����̵���48
#define	             RW_VIN3  692	//VIN3-����̵���48
#define	             RW_VIN4  693	//VIN4-����̵���50
#define	             RW_VIN5  694	//VIN5-����̵���51
#define	             RW_VIN6  695	//VIN6-����̵���52
#define	             RW_VIN7  696	//VIN7-����̵���53
#define	             RW_VIN8  697	//VIN8-����̵���54
#define	             RW_VIN9  698	//VIN9-����̵���55
#define	            RW_VIN10  699	//VIN10-����̵���56
#define	            RW_VIN11  700	//VIN11-����̵���57
#define	            RW_VIN12  701	//VIN12-����̵���58
#define	            RW_VIN13  702	//VIN13-����̵���59
#define	            RW_VIN14  703	//VIN14-����̵���60
#define	            RW_VIN15  704	//VIN15-����̵���61
#define	            RW_VIN16  705	//VIN16-����̵���62
#define	             RW_RCP1  706	//RCP1-ң��Ԥ�ü̵���1
#define	             RW_RCP2  707	//RCP2-ң��Ԥ�ü̵���2
#define	             RW_RCP3  708	//RCP3-ң��Ԥ�ü̵���3
#define	             RW_RCP4  709	//RCP4-ң��Ԥ�ü̵���4
#define	             RW_RCP5  710	//RCP5-ң��Ԥ�ü̵���5
#define	             RW_RCP6  711	//RCP6-ң��Ԥ�ü̵���6
#define	             RW_RCP7  712	//RCP7-ң��Ԥ�ü̵���7
#define	             RW_RCP8  713	//RCP8-ң��Ԥ�ü̵���8
#define	             RW_RCP9  714	//RCP9-ң��Ԥ�ü̵���9
#define	            RW_RCP10  715	//RCP10-ң��Ԥ�ü̵���10
#define	            RW_RCP11  716	//RCP11-ң��Ԥ�ü̵���11
#define	            RW_RCP12  717	//RCP12-ң��Ԥ�ü̵���12
#define	            RW_RCP13  718	//RCP13-ң��Ԥ�ü̵���13
#define	            RW_RCP14  719	//RCP14-ң��Ԥ�ü̵���14
#define	            RW_RCP15  720	//RCP15-ң��Ԥ�ü̵���15
#define	            RW_RCP16  721	//RCP16-ң��Ԥ�ü̵���16
#define	            RW_RCP17  722	//RCP17-ң��Ԥ�ü̵���17
#define	            RW_RCP18  723	//RCP18-ң��Ԥ�ü̵���18
#define	            RW_RCP19  724	//RCP19-ң��Ԥ�ü̵���19
#define	            RW_RCP20  725	//RCP20-ң��Ԥ�ü̵���20
#define	            RW_RCP21  726	//RCP21-ң��Ԥ�ü̵���21
#define	            RW_RCP22  727	//RCP22-ң��Ԥ�ü̵���22
#define	            RW_RCP23  728	//RCP23-ң��Ԥ�ü̵���23
#define	            RW_RCP24  729	//RCP24-ң��Ԥ�ü̵���24
#define	            RW_RCP25  730	//RCP25-ң��Ԥ�ü̵���25
#define	            RW_RCP26  731	//RCP26-ң��Ԥ�ü̵���26
#define	            RW_RCP27  732	//RCP27-ң��Ԥ�ü̵���27
#define	            RW_RCP28  733	//RCP28-ң��Ԥ�ü̵���28
#define	            RW_RCP29  734	//RCP29-ң��Ԥ�ü̵���29
#define	            RW_RCP30  735	//RCP30-ң��Ԥ�ü̵���30
#define	            RW_RCP31  736	//RCP31-ң��Ԥ�ü̵���31
#define	            RW_RCP32  737	//RCP32-ң��Ԥ�ü̵���32
#define	            RW_RCP33  738	//RCP33-ң��Ԥ�ü̵���33
#define	            RW_RCP34  739	//RCP34-ң��Ԥ�ü̵���34
#define	            RW_RCP35  740	//RCP35-ң��Ԥ�ü̵���35
#define	            RW_RCP36  741	//RCP36-ң��Ԥ�ü̵���36
#define	            RW_RCP37  742	//RCP37-ң��Ԥ�ü̵���37
#define	            RW_RCP38  743	//RCP38-ң��Ԥ�ü̵���38
#define	            RW_RCP39  744	//RCP39-ң��Ԥ�ü̵���39
#define	            RW_RCP40  745	//RCP40-ң��Ԥ�ü̵���40
#define	            RW_RCP41  746	//RCP41-ң��Ԥ�ü̵���41
#define	            RW_RCP42  747	//RCP42-ң��Ԥ�ü̵���42
#define	            RW_RCP43  748	//RCP43-ң��Ԥ�ü̵���43
#define	            RW_RCP44  749	//RCP44-ң��Ԥ�ü̵���44
#define	            RW_RCP45  750	//RCP45-ң��Ԥ�ü̵���45
#define	            RW_RCP46  751	//RCP46-ң��Ԥ�ü̵���46
#define	            RW_RCP47  752	//RCP47-ң��Ԥ�ü̵���47
#define	            RW_RCP48  753	//RCP48-ң��Ԥ�ü̵���48
#define	            RW_RCP49  754	//RCP49-ң��Ԥ�ü̵���49
#define	            RW_RCP50  755	//RCP50-ң��Ԥ�ü̵���50
#define	            RW_RCP51  756	//RCP51-ң��Ԥ�ü̵���51
#define	            RW_RCP52  757	//RCP52-ң��Ԥ�ü̵���52
#define	             RW_RCE1  758	//RCE1-ң��ִ�м̵���1
#define	             RW_RCE2  759	//RCE2-ң��ִ�м̵���2
#define	             RW_RCE3  760	//RCE3-ң��ִ�м̵���3
#define	             RW_RCE4  761	//RCE4-ң��ִ�м̵���4
#define	             RW_RCE5  762	//RCE5-ң��ִ�м̵���5
#define	             RW_RCE6  763	//RCE6-ң��ִ�м̵���6
#define	             RW_RCE7  764	//RCE7-ң��ִ�м̵���7
#define	             RW_RCE8  765	//RCE8-ң��ִ�м̵���8
#define	             RW_RCE9  766	//RCE9-ң��ִ�м̵���9
#define	            RW_RCE10  767	//RCE10-ң��ִ�м̵���10
#define	            RW_RCE11  768	//RCE11-ң��ִ�м̵���11
#define	            RW_RCE12  769	//RCE12-ң��ִ�м̵���12
#define	            RW_RCE13  770	//RCE13-ң��ִ�м̵���13
#define	            RW_RCE14  771	//RCE14-ң��ִ�м̵���14
#define	            RW_RCE15  772	//RCE15-ң��ִ�м̵���15
#define	            RW_RCE16  773	//RCE16-ң��ִ�м̵���16
#define	            RW_RCE17  774	//RCE17-ң��ִ�м̵���17
#define	            RW_RCE18  775	//RCE18-ң��ִ�м̵���18
#define	            RW_RCE19  776	//RCE19-ң��ִ�м̵���19
#define	            RW_RCE20  777	//RCE20-ң��ִ�м̵���20
#define	            RW_RCE21  778	//RCE21-ң��ִ�м̵���21
#define	            RW_RCE22  779	//RCE22-ң��ִ�м̵���22
#define	            RW_RCE23  780	//RCE23-ң��ִ�м̵���23
#define	            RW_RCE24  781	//RCE24-ң��ִ�м̵���24
#define	            RW_RCE25  782	//RCE25-ң��ִ�м̵���25
#define	            RW_RCE26  783	//RCE26-ң��ִ�м̵���26
#define	            RW_RCE27  784	//RCE27-ң��ִ�м̵���27
#define	            RW_RCE28  785	//RCE28-ң��ִ�м̵���28
#define	            RW_RCE29  786	//RCE29-ң��ִ�м̵���29
#define	            RW_RCE30  787	//RCE30-ң��ִ�м̵���30
#define	            RW_RCE31  788	//RCE31-ң��ִ�м̵���31
#define	            RW_RCE32  789	//RCE32-ң��ִ�м̵���32
#define	            RW_RCE33  790	//RCE33-ң��ִ�м̵���33
#define	            RW_RCE34  791	//RCE34-ң��ִ�м̵���34
#define	            RW_RCE35  792	//RCE35-ң��ִ�м̵���35
#define	            RW_RCE36  793	//RCE36-ң��ִ�м̵���36
#define	            RW_RCE37  794	//RCE37-ң��ִ�м̵���37
#define	            RW_RCE38  795	//RCE38-ң��ִ�м̵���38
#define	            RW_RCE39  796	//RCE39-ң��ִ�м̵���39
#define	            RW_RCE40  797	//RCE40-ң��ִ�м̵���40
#define	            RW_RCE41  798	//RCE41-ң��ִ�м̵���41
#define	            RW_RCE42  799	//RCE42-ң��ִ�м̵���42
#define	            RW_RCE43  800	//RCE43-ң��ִ�м̵���43
#define	            RW_RCE44  801	//RCE44-ң��ִ�м̵���44
#define	            RW_RCE45  802	//RCE45-ң��ִ�м̵���45
#define	            RW_RCE46  803	//RCE46-ң��ִ�м̵���46
#define	            RW_RCE47  804	//RCE47-ң��ִ�м̵���47
#define	            RW_RCE48  805	//RCE48-ң��ִ�м̵���48
#define	            RW_RCE49  806	//RCE49-ң��ִ�м̵���49
#define	            RW_RCE50  807	//RCE50-ң��ִ�м̵���50
#define	            RW_RCE51  808	//RCE51-ң��ִ�м̵���51
#define	            RW_RCE52  809	//RCE52-ң��ִ�м̵���52
#define	             RW_RCV1  810	//RCV1-ң�ط�У�̵���1
#define	             RW_RCV2  811	//RCV2-ң�ط�У�̵���2
#define	             RW_RCV3  812	//RCV3-ң�ط�У�̵���3
#define	             RW_RCV4  813	//RCV4-ң�ط�У�̵���4
#define	             RW_RCV5  814	//RCV5-ң�ط�У�̵���5
#define	             RW_RCV6  815	//RCV6-ң�ط�У�̵���6
#define	             RW_RCV7  816	//RCV7-ң�ط�У�̵���7
#define	             RW_RCV8  817	//RCV8-ң�ط�У�̵���8
#define	             RW_RCV9  818	//RCV9-ң�ط�У�̵���9
#define	            RW_RCV10  819	//RCV10-ң�ط�У�̵���10
#define	            RW_RCV11  820	//RCV11-ң�ط�У�̵���11
#define	            RW_RCV12  821	//RCV12-ң�ط�У�̵���12
#define	            RW_RCV13  822	//RCV13-ң�ط�У�̵���13
#define	            RW_RCV14  823	//RCV14-ң�ط�У�̵���14
#define	            RW_RCV15  824	//RCV15-ң�ط�У�̵���15
#define	            RW_RCV16  825	//RCV16-ң�ط�У�̵���16
#define	            RW_RCV17  826	//RCV17-ң�ط�У�̵���17
#define	            RW_RCV18  827	//RCV18-ң�ط�У�̵���18
#define	            RW_RCV19  828	//RCV19-ң�ط�У�̵���19
#define	            RW_RCV20  829	//RCV20-ң�ط�У�̵���20
#define	            RW_RCV21  830	//RCV21-ң�ط�У�̵���21
#define	            RW_RCV22  831	//RCV22-ң�ط�У�̵���22
#define	            RW_RCV23  832	//RCV23-ң�ط�У�̵���23
#define	            RW_RCV24  833	//RCV24-ң�ط�У�̵���24
#define	            RW_RCV25  834	//RCV25-ң�ط�У�̵���25
#define	            RW_RCV26  835	//RCV26-ң�ط�У�̵���26
#define	            RW_RCV27  836	//RCV27-ң�ط�У�̵���27
#define	            RW_RCV28  837	//RCV28-ң�ط�У�̵���28
#define	            RW_RCV29  838	//RCV29-ң�ط�У�̵���29
#define	            RW_RCV30  839	//RCV30-ң�ط�У�̵���30
#define	            RW_RCV31  840	//RCV31-ң�ط�У�̵���31
#define	            RW_RCV32  841	//RCV32-ң�ط�У�̵���32
#define	            RW_RCV33  842	//RCV33-ң�ط�У�̵���33
#define	            RW_RCV34  843	//RCV34-ң�ط�У�̵���34
#define	            RW_RCV35  844	//RCV35-ң�ط�У�̵���35
#define	            RW_RCV36  845	//RCV36-ң�ط�У�̵���36
#define	            RW_RCV37  846	//RCV37-ң�ط�У�̵���37
#define	            RW_RCV38  847	//RCV38-ң�ط�У�̵���38
#define	            RW_RCV39  848	//RCV39-ң�ط�У�̵���39
#define	            RW_RCV40  849	//RCV40-ң�ط�У�̵���40
#define	            RW_RCV41  850	//RCV41-ң�ط�У�̵���41
#define	            RW_RCV42  851	//RCV42-ң�ط�У�̵���42
#define	            RW_RCV43  852	//RCV43-ң�ط�У�̵���43
#define	            RW_RCV44  853	//RCV44-ң�ط�У�̵���44
#define	            RW_RCV45  854	//RCV45-ң�ط�У�̵���45
#define	            RW_RCV46  855	//RCV46-ң�ط�У�̵���46
#define	            RW_RCV47  856	//RCV47-ң�ط�У�̵���47
#define	            RW_RCV48  857	//RCV48-ң�ط�У�̵���48
#define	            RW_RCV49  858	//RCV49-ң�ط�У�̵���49
#define	            RW_RCV50  859	//RCV50-ң�ط�У�̵���50
#define	            RW_RCV51  860	//RCV51-ң�ط�У�̵���51
#define	            RW_RCV52  861	//RCV52-ң�ط�У�̵���52
#define	              RW_GE1  862	//GE1-���ڵ��ڼ̵���1
#define	              RW_GE2  863	//GE2-���ڵ��ڼ̵���2
#define	              RW_GE3  864	//GE3-���ڵ��ڼ̵���3
#define	              RW_GE4  865	//GE4-���ڵ��ڼ̵���4
#define	              RW_GE5  866	//GE5-���ڵ��ڼ̵���5
#define	              RW_GE6  867	//GE6-���ڵ��ڼ̵���6
#define	              RW_GE7  868	//GE7-���ڵ��ڼ̵���7
#define	              RW_GE8  869	//GE8-���ڵ��ڼ̵���8
#define	           RW_CONST1  870	//CONST1-����ʱ��̵���1
#define	           RW_CONST2  871	//CONST2-����ʱ��̵���2
#define	           RW_CONST3  872	//CONST3-����ʱ��̵���3
#define	           RW_CONST4  873	//CONST4-����ʱ��̵���4
#define	           RW_CONST5  874	//CONST5-����ʱ��̵���5
#define	           RW_CONST6  875	//CONST6-����ʱ��̵���6
#define	           RW_CONST7  876	//CONST7-����ʱ��̵���7
#define	           RW_CONST8  877	//CONST8-����ʱ��̵���8
#define	              RW_TX1  878	//TX1-����1�̵�����
#define	              RW_TX2  879	//TX2-����2�̵�����
#define	              RW_TX3  880	//TX3-����3�̵�����
#define	              RW_TX4  881	//TX4-����4�̵�����
#define	              RW_TX5  882	//TX5-����5�̵�����
#define	              RW_TX6  883	//TX6-����6�̵�����
#define	              RW_TX7  884	//TX7-����7�̵�����
#define	              RW_TX8  885	//TX8-����8�̵�����
#define	              RW_TX9  886	//TX9-����9�̵�����
#define	             RW_TX10  887	//TX10-����10�̵�����
#define	             RW_TX11  888	//TX11-����11�̵�����
#define	             RW_TX12  889	//TX12-����12�̵�����
#define	             RW_TX13  890	//TX13-����13�̵�����
#define	             RW_TX14  891	//TX14-����14�̵�����
#define	             RW_TX15  892	//TX15-����15�̵�����
#define	             RW_TX16  893	//TX16-����16�̵�����
#define	              RW_RX1  894	//RX1-����1�̵�����
#define	              RW_RX2  895	//RX2-����2�̵�����
#define	              RW_RX3  896	//RX3-����3�̵�����
#define	              RW_RX4  897	//RX4-����4�̵�����
#define	              RW_RX5  898	//RX5-����5�̵�����
#define	              RW_RX6  899	//RX6-����6�̵�����
#define	              RW_RX7  900	//RX7-����7�̵�����
#define	              RW_RX8  901	//RX8-����8�̵�����
#define	              RW_RX9  902	//RX9-����9�̵�����
#define	             RW_RX10  903	//RX10-����10�̵�����
#define	             RW_RX11  904	//RX11-����11�̵�����
#define	             RW_RX12  905	//RX12-����12�̵�����
#define	             RW_RX13  906	//RX13-����13�̵�����
#define	             RW_RX14  907	//RX14-����14�̵�����
#define	             RW_RX15  908	//RX15-����15�̵�����
#define	             RW_RX16  909	//RX16-����16�̵�����
#define	             RW_CNT1  910	//CNT1-�����̵�����1
#define	             RW_CNT2  911	//CNT2-�����̵�����2
#define	             RW_CNT3  912	//CNT3-�����̵�����3
#define	             RW_CNT4  913	//CNT4-�����̵�����4
#define	             RW_CNT5  914	//CNT5-�����̵�����5
#define	             RW_CNT6  915	//CNT6-�����̵�����6
#define	             RW_CNT7  916	//CNT7-�����̵�����7
#define	             RW_CNT8  917	//CNT8-�����̵�����8
#define	RELAY_WORD_NUM			918		//�̵�������

//�����̵�����
#define	PROTECT_RELAY_WORD_START			2	
#define	PROTECT_RELAY_WORD_NUM			83	

//�м�̵�����
#define	MID_RELAY_WORD_START			85	
#define	MID_RELAY_WORD_NUM			152	

//�Ա����м�̵�����
#define	KEEP_RELAY_WORD_START			237	
#define	KEEP_RELAY_WORD_NUM			87	

//ʱ��̵�����
#define	TIME_RELAY_WORD_START			324	
#define	TIME_RELAY_WORD_NUM			105	

//��բ�̵���
#define	TRIP_RELAY_WORD_START			429	
#define	TRIP_RELAY_WORD_NUM			4	

//����̵���
#define	VIRTUAL_RELAY_WORD_START			433	
#define	VIRTURAL_RELAY_WORD_NUM			32	

//����̵���
#define	INPUT_RELAY_WORD_START			465	
#define	INPUT_RELAY_WORD_NUM			62	

//�����̵���
#define	OUTPUT_RELAY_WORD_START			527	
#define	OUTPUT_RELAY_WORD_NUM			43	

//ѹ��̵���
#define	SW_RELAY_WORD_START			570	
#define	SW_RELAY_WORD_NUM			16	

//���ü̵���
#define	CFG_RELAY_WORD_START			586	
#define	CFG_RELAY_WORD_NUM			20	

//����̵���
#define	PULSE_RELAY_WORD_START			606	
#define	PULSE_RELAY_WORD_NUM			16	

//�����̵���
#define	CTRLTEST_RELAY_WORD_START			622	
#define	CTRLTEST_RELAY_WORD_NUM			28	

//�źż̵���
#define	SIGNAL_RELAY_WORD_START			650	
#define	SIGNAL_RELAY_WORD_NUM			16	

//LED�̵���
#define	LED_RELAY_WORD_START			666	
#define	LED_RELAY_WORD_NUM			24	

//���⿪��
#define	VIN_RELAY_WORD_START			690	
#define	VIN_RELAY_WORD_NUM			16	

//ң��Ԥ�ü̵���
#define	RCP_RELAY_WORD_START			706	
#define	RCP_RELAY_WORD_NUM			52	

//ң��ִ�м̵���
#define	RCE_RELAY_WORD_START			758	
#define	RCE_RELAY_WORD_NUM			52	

//ң�ط�У�̵���
#define	RCV_RELAY_WORD_START			810	
#define	RCV_RELAY_WORD_NUM			52	

//���ڵ��ڼ̵���
#define	GE_RELAY_WORD_START			862	
#define	GE_RELAY_WORD_NUM			8	

//����ʱ��̵���
#define	CONST_TIME_RW_START			870	
#define	CONST_TIME_RW_NUM			8	

//�������Ӽ̵���
#define	TXLINK_RW_START			878	
#define	TXLINK_RW_NUM			16	

//�������Ӽ̵���
#define	RXLINK_RW_START			894	
#define	RXLINK_RW_NUM			16	

//�籣���м��
#define	NM_RW_START			910	
#define	NM_RW_NUM			0	

//�籣���Ա��ּ�
#define	NKEEP_RW_START			910	
#define	NKEEP_RW_NUM			0	

//��������̵���
#define	DEBUG_OUT_START			910	
#define	DEBUG_OUT_NUM			0	

//��������̵���
#define	DEBUG_IN_START			910	
#define	DEBUG_IN_NUM			0	

//���ؿ��Ƽ̵���
#define	LCE_RELAY_WORD_START			910	
#define	LCE_RELAY_WORD_NUM			0	

//�����̵���
#define	CNT_RELAY_WORD_START			910	
#define	CNT_RELAY_WORD_NUM			8	


#endif