/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : LED
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 引脚拉低（0）LED亮
*           流水灯模式，这里一般实现一个周期功能即可
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "bsp_led.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

/**LED引脚定义**/
#define LED01 P00
#define LED02 P01
#define LED03 P02
#define LED04 P03
#define LED05 P04
#define LED06 P05
#define LED07 P06
#define LED08 P07

#define LED09 P27
#define LED10 P26
#define LED11 P25
#define LED12 P24
#define LED13 P23
#define LED14 P22
#define LED15 P21
#define LED16 P20


/*LED灯模式数组， 注册*/
LED_MODEL_TYP led_model;


//当前运行模式
static LEDModeHook run_model;


/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

//流水灯1：
void water_01()
{
    u8 i;
    u16 led = 0x0001;

    for (i = 0; i < 16; i++)
    {
        P0 = ~led;
        P2 = ~math_reverse(led >> 8);
        led <<= 1;
        delay_ms(200);
    }
}
//流水灯2：
void water_02()
{
    u8 i;
    u16 led;

    led = 0x0001;
    for (i = 0; i < 16; i++)
    {
        delay_ms(50);
        P0 = ~led;
        P2 = ~math_reverse(led >> 8);
        led <<= 1;
        delay_ms(50);
    }
    led = 0x8000;
    for (i = 0; i < 16; i++)
    {
        delay_ms(50);
        P0 = ~led;
        P2 = ~math_reverse(led >> 8);
        led >>= 1;
        delay_ms(50);
    }
}

//闪烁灯1：
void flash_01()
{
    P0 = 0x00;
    P2 = 0xFF;
    delay_ms(200);
    P0 = 0xFF;
    P2 = 0x00;
    delay_ms(200);
}

//闪烁灯2：
void flash_02()
{
    P0 = 0xAA;
    P2 = math_reverse(0xAA);
    delay_ms(200);
    P0 = 0x55;
    P2 = math_reverse(0x55);
    delay_ms(200);
}

//闪烁灯3：
void flash_03()
{
    P0 = 0xCC;
    P2 = math_reverse(0xCC);
    delay_ms(200);
    P0 = 0x33;
    P2 = math_reverse(0x33);
    delay_ms(200);
}


//注册
static LEDModeHook led_mode_arr[] = {
    water_01,
    water_02,
    flash_01,
    flash_02,
    flash_03,
};

/*
*********************************************************************************************************
* Description : KBD初始化。
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_LED_Init(void)
{
    P0 = 0xFF;     //引脚拉高
    P2 = 0xFF;

    led_model.models = led_mode_arr;
    led_model.len = util_size(led_mode_arr, LEDModeHook);

    if (led_model.len == 0)
        while(1); //错误：模式未注册

    run_model = led_model.models[0];
}

/*
*********************************************************************************************************
* Description : 运行
*********************************************************************************************************
*/
void BSP_LED_Run()
{
    run_model();
}

/*
*********************************************************************************************************
* Description : 模式切换
*
*
*********************************************************************************************************
*/
void BSP_LED_ModelSwitch(u8 i)
{
    if (i >= led_model.len)
        return;
    
    run_model = led_model.models[i];
}

/**********************************************RND******************************************************/
