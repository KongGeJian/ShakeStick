
/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : bsp.h
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : (1) 这里是BSP预处理总头文件
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include "cfg.h"

#include "lib_stdint.h"
#include "lib_delay.h"
#include "lib_math.h"
#include "lib_util.h"


/*
*********************************************************************************************************
*                                        INCLUDE FILES - Sub module
*********************************************************************************************************
*/

// #include "bsp_uart.h"
#include "bsp_led.h"
#include "bsp_kbd.h"



/*
*********************************************************************************************************
*                                              FUNCTION
*********************************************************************************************************
*/

extern void  BSP_Init(void);



#endif
/***********************************************END*****************************************************/
