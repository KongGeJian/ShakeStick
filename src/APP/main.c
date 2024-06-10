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


// u8 idata kbd_stack[30];
// u8 idata led_stack[30];

byte idata *call_run_sp;
u8 led_model_index;


/*T1触发按键扫描*/
#define T1_10MS (65536-FOSC/12/100)   //12T模式 10ms

void run();


/*T1中断服务程序。和main使用同样的bank0，这里会push ACC~AR7*/
void T1_ISR() interrupt 3
{
    TL1 = T1_10MS;      //重装
    TH1 = T1_10MS;

    BSP_KBD_Scan();     //扫描一次按键

    if (BSP_KBD_IsShort(K_FUN)) //功能短按
    {
        led_model_index = (led_model_index + 1) % led_model.len;
        // led_model_index = math_random((P2 << 8 | P0), led_model.len - 1, 0);
        BSP_LED_ModelSwitch(led_model_index);
        SP = call_run_sp;   //修改SP，重新运行run
    }
}


void setup()
{
    TMOD = 0x10;    //T1 模式1(16-bit)
    TL1 = T1_10MS;
    TH1 = T1_10MS;
    ET1 = 1;        //开启T1中断
    TR1 = 1;        //T1运行

    led_model_index = 0;
}


void run()
{
    /*以下是先依次压入 &run、ACC~AR7，然后缓存SP，下次模式转换，直接从SP弹栈*/
    _push_(ACC); //随便push两个字节来占位，用来缓存run地址
    _push_(ACC);
    *((byte idata *)(SP-1)) = (word)&run;
    *((byte idata *)SP)     = (word)&run >> 8;
    #pragma ASM
    PUSH    ACC
    PUSH    B
    PUSH    DPH
    PUSH    DPL
    PUSH    PSW
    
    PUSH    AR0
    PUSH    AR1
    PUSH    AR2
    PUSH    AR3
    PUSH    AR4
    PUSH    AR5
    PUSH    AR6
    PUSH    AR7
    #pragma ENDASM
    call_run_sp = (byte *) SP; //记录call run 的SP call_run_sp

    /*LED loop*/
    while(1)
    {
        BSP_LED_Run();
    }
}


void main()
{
    BSP_Init();
    setup();
    run();
}
