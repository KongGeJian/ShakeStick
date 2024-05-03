/*
*********************************************************************************************************
* DateTime: 2024-04-30 12:00
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : -
*********************************************************************************************************
*/

#include "bsp.h"


byte idata *call_run_sp;
u8 led_model_index;


/*T1触发按键扫描*/
#define T1_100US (256-FOSC/12/10000)   //12T模式 0.1ms
static u8 t1_counter;  //T1计数器

void run();


/*T1中断服务程序*/
// C:0x0084    C0E0     PUSH     ACC(0xE0)
// C:0x0086    C0F0     PUSH     B(0xF0)
// C:0x0088    C083     PUSH     DPH(0x83)
// C:0x008A    C082     PUSH     DPL(0x82)
// C:0x008C    C0D0     PUSH     PSW(0xD0)
void T1_ISR() interrupt 3 using 1
{
    if (t1_counter-- != 0)
        return;
    
    t1_counter = 10;    //1ms = T1_100US * 10

    BSP_KBD_Scan();     //扫描一次按键

    if (BSP_KBD_IsShort(K_FUN) && BSP_KBD_IsRelease(K_FUN)) //功能短按
    {
        led_model_index = (led_model_index + 1) % led_model.len;
        // led_model_index = math_random((P2 << 8 | P0), led_model.len - 1, 0);
        BSP_LED_ModelSwitch(led_model_index);
        // 修改SP，重新运行
        *(call_run_sp + 1) = (word)&run;
        *(call_run_sp + 2) = (word)&run >> 8;
        *(call_run_sp + 3) = *(call_run_sp);
        *(call_run_sp + 4) = *(call_run_sp - 1);
        *(call_run_sp + 5) = *(call_run_sp - 2);
        *(call_run_sp + 6) = *(call_run_sp - 3);
        *(call_run_sp + 7) = *(call_run_sp - 4);
        SP = call_run_sp + 7;
    }
}


void setup()
{
    t1_counter = 0;

    TMOD = 0x20;    //T1 模式2(8-bit自动重装)
    TL1 = T1_100US;
    TH1 = T1_100US;
    ET1 = 1;        //开启T1中断
    TR1 = 1;        //T1运行

    led_model_index = 0;
}


void run()
{
    while(1)
    {
        BSP_LED_Run();
    }
}


void main()
{
    BSP_Init();

    setup();

    _push_(ACC);
    _push_(B);
    _push_(DPH);
    _push_(DPL);
    _push_(PSW);
    call_run_sp = (byte *) SP; //记录call run 是SP call_run_sp
    run();
}
