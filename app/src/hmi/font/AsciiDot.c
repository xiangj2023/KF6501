/*************************************************************************
*
*   Copyright (C), 1999-2004, keyvia. Co., Ltd.
*
*   �ļ����ƣ�AsciiDot.c
*   ����ģ�飺<...>
*   �� �� �ţ�1.0
*   �������ڣ�2008-4-3
*   ��    �ߣ���������
*   ��    �ܣ�<Ascii�ַ���16���8������ֿ�>
*
*************************************************************************/
unsigned char ASCII16[] = 
{
/*!*/
0,0,0,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0,0,0X18,0X18,0,0,
/*"*/
0,0X12,0X36,0X24,0X48,0,0,0,0,0,0,0,0,0,0,0,
/*#*/
0,0,0,0X24,0X24,0X24,0XFE,0X48,0X48,0X48,0XFE,0X48,0X48,0X48,0,0,
/*$*/
0,0,0X10,0X38,0X54,0X54,0X50,0X30,0X18,0X14,0X14,0X54,0X54,0X38,0X10,0X10,
/*%*/
0,0,0,0X44,0XA4,0XA8,0XA8,0XA8,0X54,0X1A,0X2A,0X2A,0X2A,0X44,0,0,
/*&*/
0,0,0,0X44,0XA4,0XA0,0XA0,0XA0,0X54,0X12,0X22,0X22,0X22,0X44,0,0,
/*'*/
0,0X60,0X60,0X20,0XC0,0,0,0,0,0,0,0,0,0,0,0,
/*(*/
0,0X2,0X4,0X8,0X8,0X10,0X10,0X10,0X10,0X10,0X10,0X8,0X8,0X4,0X2,0,
/*)*/
0,0X40,0X20,0X10,0X10,0X8,0X8,0X8,0X8,0X8,0X8,0X10,0X10,0X20,0X40,0,
/***/
0,0,0,0,0X10,0X10,0XD6,0X38,0X38,0XD6,0X10,0X10,0,0,0,0,
/*+*/
0,0,0,0,0X10,0X10,0X10,0X10,0XFE,0X10,0X10,0X10,0X10,0,0,0,
/*,*/
0,0,0,0,0,0,0,0,0,0,0,0,0X60,0X60,0X20,0XC0,
/*-*/
0,0,0,0,0,0,0,0,0X7F,0,0,0,0,0,0,0,
/*.*/
0,0,0,0,0,0,0,0,0,0,0,0,0X60,0X60,0,0,
/*/*/
0,0,0X1,0X2,0X2,0X4,0X4,0X8,0X8,0X10,0X10,0X20,0X20,0X40,0X40,0,
/*0*/
0,0,0,0X18,0X24,0X42,0X42,0X42,0X42,0X42,0X42,0X42,0X24,0X18,0,0,
/*1*/
0,0,0,0X10,0X70,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X7C,0,0,
/*2*/
0,0,0,0X3C,0X42,0X42,0X42,0X4,0X4,0X8,0X10,0X20,0X42,0X7E,0,0,
/*3*/
0,0,0,0X3C,0X42,0X42,0X4,0X18,0X4,0X2,0X2,0X42,0X44,0X38,0,0,
/*4*/
0,0,0,0X4,0XC,0X14,0X24,0X24,0X44,0X44,0X7E,0X4,0X4,0X1E,0,0,
/*5*/
0,0,0,0X7E,0X40,0X40,0X40,0X58,0X64,0X2,0X2,0X42,0X44,0X38,0,0,
/*6*/
0,0,0,0X1C,0X24,0X40,0X40,0X58,0X64,0X42,0X42,0X42,0X24,0X18,0,0,
/*7*/
0,0,0,0X7E,0X44,0X44,0X8,0X8,0X10,0X10,0X10,0X10,0X10,0X10,0,0,
/*8*/
0,0,0,0X3C,0X42,0X42,0X42,0X24,0X18,0X24,0X42,0X42,0X42,0X3C,0,0,
/*9*/
0,0,0,0X18,0X24,0X42,0X42,0X42,0X26,0X1A,0X2,0X2,0X24,0X38,0,0,
/*:*/
0,0,0,0,0,0,0X18,0X18,0,0,0,0,0X18,0X18,0,0,
/*;*/
0,0,0,0,0,0,0,0X10,0,0,0,0,0,0X10,0X10,0X20,
/*<*/
0,0,0,0X2,0X4,0X8,0X10,0X20,0X40,0X20,0X10,0X8,0X4,0X2,0,0,
/*=*/
0,0,0,0,0,0,0XFE,0,0,0,0XFE,0,0,0,0,0,
/*>*/
0,0,0,0X40,0X20,0X10,0X8,0X4,0X2,0X4,0X8,0X10,0X20,0X40,0,0,
/*?*/
0,0,0,0X3C,0X42,0X42,0X62,0X2,0X4,0X8,0X8,0,0X18,0X18,0,0,
/*@*/
0,0,0,0X38,0X44,0X5A,0XAA,0XAA,0XAA,0XAA,0XB4,0X42,0X44,0X38,0,0,
/*A*/
0,0,0,0X10,0X10,0X18,0X28,0X28,0X24,0X3C,0X44,0X42,0X42,0XE7,0,0,
/*B*/
0,0,0,0XF8,0X44,0X44,0X44,0X78,0X44,0X42,0X42,0X42,0X44,0XF8,0,0,
/*C*/
0,0,0,0X3E,0X42,0X42,0X80,0X80,0X80,0X80,0X80,0X42,0X44,0X38,0,0,
/*D*/
0,0,0,0XF8,0X44,0X42,0X42,0X42,0X42,0X42,0X42,0X42,0X44,0XF8,0,0,
/*E*/
0,0,0,0XFC,0X42,0X48,0X48,0X78,0X48,0X48,0X40,0X42,0X42,0XFC,0,0,
/*F*/
0,0,0,0XFC,0X42,0X48,0X48,0X78,0X48,0X48,0X40,0X40,0X40,0XE0,0,0,
/*G*/
0,0,0,0X3C,0X44,0X44,0X80,0X80,0X80,0X8E,0X84,0X44,0X44,0X38,0,0,
/*H*/
0,0,0,0XE7,0X42,0X42,0X42,0X42,0X7E,0X42,0X42,0X42,0X42,0XE7,0,0,
/*I*/
0,0,0,0X7C,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X7C,0,0,
/*J*/
0,0,0,0X3E,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X88,0XF0,
/*K*/
0,0,0,0XEE,0X44,0X48,0X50,0X70,0X50,0X48,0X48,0X44,0X44,0XEE,0,0,
/*L*/
0,0,0,0XE0,0X40,0X40,0X40,0X40,0X40,0X40,0X40,0X40,0X42,0XFE,0,0,
/*M*/
0,0,0,0XEE,0X6C,0X6C,0X6C,0X6C,0X54,0X54,0X54,0X54,0X54,0XD6,0,0,
/*N*/
0,0,0,0XC7,0X62,0X62,0X52,0X52,0X4A,0X4A,0X4A,0X46,0X46,0XE2,0,0,
/*O*/
0,0,0,0X38,0X44,0X82,0X82,0X82,0X82,0X82,0X82,0X82,0X44,0X38,0,0,
/*P*/
0,0,0,0XFC,0X42,0X42,0X42,0X42,0X7C,0X40,0X40,0X40,0X40,0XE0,0,0,
/*Q*/
0,0,0,0X38,0X44,0X82,0X82,0X82,0X82,0X82,0XB2,0XCA,0X4C,0X38,0X6,0,
/*R*/
0,0,0,0XFC,0X42,0X42,0X42,0X7C,0X48,0X48,0X44,0X44,0X42,0XE3,0,0,
/*S*/
0,0,0,0X3E,0X42,0X42,0X40,0X20,0X18,0X4,0X2,0X42,0X42,0X7C,0,0,
/*T*/
0,0,0,0XFE,0X92,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X38,0,0,
/*U*/
0,0,0,0XE7,0X42,0X42,0X42,0X42,0X42,0X42,0X42,0X42,0X42,0X3C,0,0,
/*V*/
0,0,0,0XE7,0X42,0X42,0X44,0X24,0X24,0X28,0X28,0X18,0X10,0X10,0,0,
/*W*/
0,0,0,0XD6,0X92,0X92,0X92,0X92,0XAA,0XAA,0X6C,0X44,0X44,0X44,0,0,
/*X*/
0,0,0,0XE7,0X42,0X24,0X24,0X18,0X18,0X18,0X24,0X24,0X42,0XE7,0,0,
/*Y*/
0,0,0,0XEE,0X44,0X44,0X28,0X28,0X10,0X10,0X10,0X10,0X10,0X38,0,0,
/*Z*/
0,0,0,0X7E,0X84,0X4,0X8,0X8,0X10,0X20,0X20,0X42,0X42,0XFC,0,0,
/*[*/
0,0X1E,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X1E,0,
/*\*/
0,0,0X40,0X40,0X20,0X20,0X10,0X10,0X10,0X8,0X8,0X4,0X4,0X4,0X2,0X2,
/*]*/
0,0X78,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X78,0,
/*^*/
0,0X1C,0X22,0,0,0,0,0,0,0,0,0,0,0,0,0,
/*_*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0XFF,
/*`*/
0,0X60,0X10,0,0,0,0,0,0,0,0,0,0,0,0,0,
/*a*/
0,0,0,0,0,0,0,0X3C,0X42,0X1E,0X22,0X42,0X42,0X3F,0,0,
/*b*/
0,0,0,0XC0,0X40,0X40,0X40,0X58,0X64,0X42,0X42,0X42,0X64,0X58,0,0,
/*c*/
0,0,0,0,0,0,0,0X1C,0X22,0X40,0X40,0X40,0X22,0X1C,0,0,
/*d*/
0,0,0,0X6,0X2,0X2,0X2,0X1E,0X22,0X42,0X42,0X42,0X26,0X1B,0,0,
/*e*/
0,0,0,0,0,0,0,0X3C,0X42,0X7E,0X40,0X40,0X42,0X3C,0,0,
/*f*/
0,0,0,0XF,0X11,0X10,0X10,0X7E,0X10,0X10,0X10,0X10,0X10,0X7C,0,0,
/*g*/
0,0,0,0,0,0,0,0X3E,0X44,0X44,0X38,0X40,0X3C,0X42,0X42,0X3C,
/*h*/
0,0,0,0XC0,0X40,0X40,0X40,0X5C,0X62,0X42,0X42,0X42,0X42,0XE7,0,0,
/*i*/
0,0,0,0X30,0X30,0,0,0X70,0X10,0X10,0X10,0X10,0X10,0X7C,0,0,
/*j*/
0,0,0,0XC,0XC,0,0,0X1C,0X4,0X4,0X4,0X4,0X4,0X4,0X44,0X78,
/*k*/
0,0,0,0XC0,0X40,0X40,0X40,0X4E,0X48,0X50,0X68,0X48,0X44,0XEE,0,0,
/*l*/
0,0,0,0X70,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X7C,0,0,
/*m*/
0,0,0,0,0,0,0,0XFE,0X49,0X49,0X49,0X49,0X49,0XED,0,0,
/*n*/
0,0,0,0,0,0,0,0XDC,0X62,0X42,0X42,0X42,0X42,0XE7,0,0,
/*o*/
0,0,0,0,0,0,0,0X3C,0X42,0X42,0X42,0X42,0X42,0X3C,0,0,
/*p*/
0,0,0,0,0,0,0,0XD8,0X64,0X42,0X42,0X42,0X44,0X78,0X40,0XE0,
/*q*/
0,0,0,0,0,0,0,0X1E,0X22,0X42,0X42,0X42,0X22,0X1E,0X2,0X7,
/*r*/
0,0,0,0,0,0,0,0XEE,0X32,0X20,0X20,0X20,0X20,0XF8,0,0,
/*s*/
0,0,0,0,0,0,0,0X3E,0X42,0X40,0X3C,0X2,0X42,0X7C,0,0,
/*t*/
0,0,0,0,0,0X10,0X10,0X7C,0X10,0X10,0X10,0X10,0X10,0XC,0,0,
/*u*/
0,0,0,0,0,0,0,0XC6,0X42,0X42,0X42,0X42,0X46,0X3B,0,0,
/*v*/
0,0,0,0,0,0,0,0XE7,0X42,0X24,0X24,0X28,0X10,0X10,0,0,
/*w*/
0,0,0,0,0,0,0,0XD7,0X92,0X92,0XAA,0XAA,0X44,0X44,0,0,
/*x*/
0,0,0,0,0,0,0,0X6E,0X24,0X18,0X18,0X18,0X24,0X76,0,0,
/*y*/
0,0,0,0,0,0,0,0XE7,0X42,0X24,0X24,0X28,0X18,0X10,0X10,0XE0,
/*z*/
0,0,0,0,0,0,0,0X7E,0X44,0X8,0X10,0X10,0X22,0X7E,0,0,
/*{*/
0,0X3,0X4,0X4,0X4,0X4,0X4,0X8,0X4,0X4,0X4,0X4,0X4,0X4,0X3,0,
/*|*/
0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,0X8,
/*}*/
0,0X60,0X10,0X10,0X10,0X10,0X10,0X8,0X10,0X10,0X10,0X10,0X10,0X10,0X60,0,
/*~*/
0X30,0X4C,0X43,0,0,0,0,0,0,0,0,0,0,0,0,0,
/*��*/
0X1,0,0X3,0X80,0X7,0XC0,0XF,0XE0,0X1F,0XF0,0X3F,0XF8,0X7F,0XFC,0XFF,0XFE,
/*��*/
0XFF,0XFE,0X7F,0XFC,0X3F,0XF8,0X1F,0XF0,0XF,0XE0,0X7,0XC0,0X3,0X80,0X1,0,
};
unsigned char ASCII12[] = 
{
/*!*/
0,0,0X20,0X20,0X20,0X20,0X20,0X20,0,0X20,0,0,
/*"*/
0,0X28,0X50,0X50,0,0,0,0,0,0,0,0,
/*#*/
0,0,0X28,0X28,0XFC,0X28,0X50,0XFC,0X50,0X50,0,0,
/*$*/
0,0X20,0X78,0XA8,0XA0,0X60,0X30,0X28,0XA8,0XF0,0X20,0,
/*%*/
0,0,0X48,0XA8,0XB0,0X50,0X28,0X34,0X54,0X48,0,0,
/*&*/
0,0,0X48,0XA8,0XA0,0X40,0X28,0X24,0X44,0X48,0,0,
/*'*/
0,0X40,0X40,0X80,0,0,0,0,0,0,0,0,
/*(*/
0,0X4,0X8,0X10,0X10,0X10,0X10,0X10,0X10,0X8,0X4,0,
/*)*/
0,0X40,0X20,0X10,0X10,0X10,0X10,0X10,0X10,0X20,0X40,0,
/***/
0,0,0,0X20,0XA8,0X70,0X70,0XA8,0X20,0,0,0,
/*+*/
0,0,0X20,0X20,0X20,0XF8,0X20,0X20,0X20,0,0,0,
/*,*/
0,0,0,0,0,0,0,0,0,0X40,0X40,0X80,
/*-*/
0,0,0,0,0,0XF8,0,0,0,0,0,0,
/*.*/
0,0,0,0,0,0,0,0,0,0X40,0,0,
/*/*/
0,0X8,0X10,0X10,0X10,0X20,0X20,0X40,0X40,0X40,0X80,0,
/*0*/
0,0,0X70,0X88,0X88,0X88,0X88,0X88,0X88,0X70,0,0,
/*1*/
0,0,0X20,0X60,0X20,0X20,0X20,0X20,0X20,0X70,0,0,
/*2*/
0,0,0X70,0X88,0X88,0X10,0X20,0X40,0X80,0XF8,0,0,
/*3*/
0,0,0X70,0X88,0X8,0X30,0X8,0X8,0X88,0X70,0,0,
/*4*/
0,0,0X10,0X30,0X50,0X50,0X90,0X78,0X10,0X18,0,0,
/*5*/
0,0,0XF8,0X80,0X80,0XF0,0X8,0X8,0X88,0X70,0,0,
/*6*/
0,0,0X70,0X90,0X80,0XF0,0X88,0X88,0X88,0X70,0,0,
/*7*/
0,0,0XF8,0X90,0X10,0X20,0X20,0X20,0X20,0X20,0,0,
/*8*/
0,0,0X70,0X88,0X88,0X70,0X88,0X88,0X88,0X70,0,0,
/*9*/
0,0,0X70,0X88,0X88,0X88,0X78,0X8,0X48,0X70,0,0,
/*:*/
0,0,0,0,0X20,0,0,0,0,0X20,0,0,
/*;*/
0,0,0,0,0,0X20,0,0,0,0X20,0X20,0,
/*<*/
0,0X4,0X8,0X10,0X20,0X40,0X20,0X10,0X8,0X4,0,0,
/*=*/
0,0,0,0,0XF8,0,0,0XF8,0,0,0,0,
/*>*/
0,0X40,0X20,0X10,0X8,0X4,0X8,0X10,0X20,0X40,0,0,
/*?*/
0,0,0X70,0X88,0X88,0X10,0X20,0X20,0,0X20,0,0,
/*@*/
0,0,0X70,0X88,0X98,0XA8,0XA8,0XB8,0X80,0X78,0,0,
/*A*/
0,0,0X20,0X20,0X30,0X50,0X50,0X78,0X48,0XCC,0,0,
/*B*/
0,0,0XF0,0X48,0X48,0X70,0X48,0X48,0X48,0XF0,0,0,
/*C*/
0,0,0X78,0X88,0X80,0X80,0X80,0X80,0X88,0X70,0,0,
/*D*/
0,0,0XF0,0X48,0X48,0X48,0X48,0X48,0X48,0XF0,0,0,
/*E*/
0,0,0XF8,0X48,0X50,0X70,0X50,0X40,0X48,0XF8,0,0,
/*F*/
0,0,0XF8,0X48,0X50,0X70,0X50,0X40,0X40,0XE0,0,0,
/*G*/
0,0,0X38,0X48,0X80,0X80,0X9C,0X88,0X48,0X30,0,0,
/*H*/
0,0,0XCC,0X48,0X48,0X78,0X48,0X48,0X48,0XCC,0,0,
/*I*/
0,0,0XF8,0X20,0X20,0X20,0X20,0X20,0X20,0XF8,0,0,
/*J*/
0,0,0X7C,0X10,0X10,0X10,0X10,0X10,0X10,0X90,0XE0,0,
/*K*/
0,0,0XEC,0X48,0X50,0X60,0X50,0X50,0X48,0XEC,0,0,
/*L*/
0,0,0XE0,0X40,0X40,0X40,0X40,0X40,0X44,0XFC,0,0,
/*M*/
0,0,0XD8,0XD8,0XD8,0XD8,0XA8,0XA8,0XA8,0XA8,0,0,
/*N*/
0,0,0XDC,0X48,0X68,0X68,0X58,0X58,0X48,0XE8,0,0,
/*O*/
0,0,0X70,0X88,0X88,0X88,0X88,0X88,0X88,0X70,0,0,
/*P*/
0,0,0XF0,0X48,0X48,0X70,0X40,0X40,0X40,0XE0,0,0,
/*Q*/
0,0,0X70,0X88,0X88,0X88,0X88,0XE8,0X98,0X70,0X18,0,
/*R*/
0,0,0XF0,0X48,0X48,0X70,0X50,0X48,0X48,0XEC,0,0,
/*S*/
0,0,0X78,0X88,0X80,0X60,0X10,0X8,0X88,0XF0,0,0,
/*T*/
0,0,0XF8,0XA8,0X20,0X20,0X20,0X20,0X20,0X70,0,0,
/*U*/
0,0,0XCC,0X48,0X48,0X48,0X48,0X48,0X48,0X30,0,0,
/*V*/
0,0,0XCC,0X48,0X48,0X50,0X50,0X30,0X20,0X20,0,0,
/*W*/
0,0,0XA8,0XA8,0XA8,0X70,0X50,0X50,0X50,0X50,0,0,
/*X*/
0,0,0XD8,0X50,0X50,0X20,0X20,0X50,0X50,0XD8,0,0,
/*Y*/
0,0,0XD8,0X50,0X50,0X20,0X20,0X20,0X20,0X70,0,0,
/*Z*/
0,0,0XF8,0X90,0X10,0X20,0X20,0X40,0X48,0XF8,0,0,
/*[*/
0,0X38,0X20,0X20,0X20,0X20,0X20,0X20,0X20,0X20,0X38,0,
/*\*/
0,0X40,0X40,0X40,0X20,0X20,0X10,0X10,0X10,0X8,0,0,
/*]*/
0,0X70,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X70,0,
/*^*/
0,0X20,0X50,0,0,0,0,0,0,0,0,0,
/*_*/
0,0,0,0,0,0,0,0,0,0,0,0XFC,
/*`*/
0,0X20,0,0,0,0,0,0,0,0,0,0,
/*a*/
0,0,0,0,0,0X30,0X48,0X38,0X48,0X3C,0,0,
/*b*/
0,0,0XC0,0X40,0X40,0X70,0X48,0X48,0X48,0X70,0,0,
/*c*/
0,0,0,0,0,0X38,0X48,0X40,0X40,0X38,0,0,
/*d*/
0,0,0X18,0X8,0X8,0X38,0X48,0X48,0X48,0X3C,0,0,
/*e*/
0,0,0,0,0,0X30,0X48,0X78,0X40,0X38,0,0,
/*f*/
0,0,0X1C,0X20,0X20,0X78,0X20,0X20,0X20,0X78,0,0,
/*g*/
0,0,0,0,0,0X3C,0X48,0X30,0X40,0X78,0X44,0X38,
/*h*/
0,0,0XC0,0X40,0X40,0X70,0X48,0X48,0X48,0XEC,0,0,
/*i*/
0,0,0X20,0,0,0X60,0X20,0X20,0X20,0X70,0,0,
/*j*/
0,0,0X10,0,0,0X30,0X10,0X10,0X10,0X10,0X10,0XE0,
/*k*/
0,0,0XC0,0X40,0X40,0X5C,0X50,0X70,0X48,0XEC,0,0,
/*l*/
0,0,0XE0,0X20,0X20,0X20,0X20,0X20,0X20,0XF8,0,0,
/*m*/
0,0,0,0,0,0XF0,0XA8,0XA8,0XA8,0XA8,0,0,
/*n*/
0,0,0,0,0,0XF0,0X48,0X48,0X48,0XEC,0,0,
/*o*/
0,0,0,0,0,0X30,0X48,0X48,0X48,0X30,0,0,
/*p*/
0,0,0,0,0,0XF0,0X48,0X48,0X48,0X70,0X40,0XE0,
/*q*/
0,0,0,0,0,0X38,0X48,0X48,0X48,0X38,0X8,0X1C,
/*r*/
0,0,0,0,0,0XD8,0X60,0X40,0X40,0XE0,0,0,
/*s*/
0,0,0,0,0,0X78,0X40,0X30,0X8,0X78,0,0,
/*t*/
0,0,0,0X20,0X20,0X70,0X20,0X20,0X20,0X18,0,0,
/*u*/
0,0,0,0,0,0XD8,0X48,0X48,0X48,0X3C,0,0,
/*v*/
0,0,0,0,0,0XEC,0X48,0X50,0X30,0X20,0,0,
/*w*/
0,0,0,0,0,0XA8,0XA8,0X70,0X50,0X50,0,0,
/*x*/
0,0,0,0,0,0XD8,0X50,0X20,0X50,0XD8,0,0,
/*y*/
0,0,0,0,0,0XEC,0X48,0X50,0X30,0X20,0X20,0XC0,
/*z*/
0,0,0,0,0,0X78,0X10,0X20,0X20,0X78,0,0,
/*{*/
0,0X18,0X10,0X10,0X10,0X20,0X10,0X10,0X10,0X10,0X18,0,
/*|*/
0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,0X10,
/*}*/
0,0X60,0X20,0X20,0X20,0X10,0X20,0X20,0X20,0X20,0X60,0,
/*~*/
0X40,0XA4,0X18,0,0,0,0,0,0,0,0,0,
/*��*/
0X4,0,0XE,0,0X1F,0,0X3F,0X80,0X7F,0XC0,0XFF,0XE0,
/*��*/
0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,0XCC,
};