/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : bsp.c
* Version : V1.0
* Author  : 空格键
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "bsp.h"



/*
*********************************************************************************************************
* Description : BSP初始化方法。
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Init(void)
{
    EA = 1;             //使能总中断。

    //初始化 LIB
    delay_init();

    //初始化 BSP
    // BSP_UART_Init();
    BSP_LED_Init();
    BSP_KBD_Init();
}

/**********************************************END******************************************************/
