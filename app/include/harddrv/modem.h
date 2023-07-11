#ifndef _MODEM_H
#define _MODEM_H
/*---------------------------------------------------------------------*/
/*        Modem status define                                          */
/*---------------------------------------------------------------------*/
#define ModemPowerOff    0x00   /* modem power off */
#define ModemFree        0x01  /* modem �����ѳ�ʼ�����ȴ��Զ����Ż��Զ�Ӧ�� */
#define ModemConnecting  0x02 /* modem  �������� */
#define ModemConnected   0x03 /* modem �����Ӻ� */
#define ModemAutoanswer  0x04  /* modem �����Զ�Ӧ��ʽͨ�� */
#define ModemHangup      0x05  /* modem hang up */
#define ModemWaitDial    0x06  /* modem wait dialing */
/*----------------------------------------------------------------------*/
/*         Modem timer flag define                                      */
/*----------------------------------------------------------------------*/
#define MWaitNone       0x00
#define MWaitOk         0x01
#define MWaitDcd        0x02
#define MWaitHangup     0x03
#define MWaitFree       0x04
#define MTxClose        0x05
#define MWaitRedial     0x06
/*----------------------------------------------------------------------*/
/*   pc16c55x and Z85c30 chip MODEM status return                       */
/*----------------------------------------------------------------------*/
#define MODEM_CTS    0x01
#define MODEM_DCD    0x02
#define MODEM_DTR    0x01
#define MODEM_RTS    0x02
/*-----------------------------------------------------------------------*/

#endif
