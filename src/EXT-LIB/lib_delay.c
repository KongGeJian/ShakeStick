/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : 精确延时用 定时器
* Tip(s)  : 说明：
*             + 延迟函数库（软件延时，不同的芯片参数不同）。 系统频率 12.000M Hz
*             + while 循环 % 等会增加误差，实际软件延迟时间比预期值大很多
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "lib_delay.h"



/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/
// TODO 切换芯片后，这里重新调试 

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

void Delay1000ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 8;
	j = 154;
	k = 122;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




/*
*********************************************************************************************************
* Description: 延时初始化函数
*
* Note(s)    : none.
*********************************************************************************************************
*/
void delay_init(void)
{
    delay_us(0);
    delay_ms(0);
}

/*
*********************************************************************************************************
* Description: 延时n微妙
*
* Arguments  : nus  -  n微妙
*
* Note(s)    : 为了尽量减少误差，这里使用了u8。
*********************************************************************************************************
*/
void delay_us(u8 nus)
{
	nus = nus / 3;
    while (nus--);
}

/*
*********************************************************************************************************
* Description: 延时n毫秒
*
* Arguments  : nms  -  n毫秒
*
* Note(s)    : 
*********************************************************************************************************
*/
void delay_ms(u8 nms)
{
    while (nms--)
        Delay1ms();
}

/*
*********************************************************************************************************
* Description: 延时n秒
*
* Arguments  : ns  -  n秒
*
* Note(s)    : 
*********************************************************************************************************
*/
void delay_s(u8 ns)
{
    while (ns--)
        Delay1000ms();
}

/**********************************************RND******************************************************/
