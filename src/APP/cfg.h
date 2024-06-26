/*
*********************************************************************************************************
*                                        CONFIG
*
* Name    : 配置
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 这里只定义全局通用配置
*********************************************************************************************************
*/

#ifndef __CFG_H__
#define __CFG_H__


#include <reg52.h>

/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

/*P0*/
sbit P00 = P0^0;
sbit P01 = P0^1;
sbit P02 = P0^2;
sbit P03 = P0^3;
sbit P04 = P0^4;
sbit P05 = P0^5;
sbit P06 = P0^6;
sbit P07 = P0^7;
/*P1*/
sbit P10 = P1^0;
sbit P11 = P1^1;
sbit P12 = P1^2;
sbit P13 = P1^3;
sbit P14 = P1^4;
sbit P15 = P1^5;
sbit P16 = P1^6;
sbit P17 = P1^7;
/*P2*/
sbit P20 = P2^0;
sbit P21 = P2^1;
sbit P22 = P2^2;
sbit P23 = P2^3;
sbit P24 = P2^4;
sbit P25 = P2^5;
sbit P26 = P2^6;
sbit P27 = P2^7;
/*P3*/
sbit P30 = P3^0;
sbit P31 = P3^1;
sbit P32 = P3^2;
sbit P33 = P3^3;
sbit P34 = P3^4;
sbit P35 = P3^5;
sbit P36 = P3^6;
sbit P37 = P3^7;



#define     FOSC        12000000uL      //系统频率 24MHz
// #define     SYSclk      FOSC/1          //系统时钟 1T
#define     SYSclk      FOSC/12         //系统时钟 12T

#define     DEBUG_USE_SIMULATOR     0   //使用模拟器Debug。0：不使用



#endif
/***********************************************END*****************************************************/
