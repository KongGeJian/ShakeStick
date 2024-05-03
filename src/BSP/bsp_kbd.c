/*
*********************************************************************************************************
*                                        BOARD SUPPORT PACKAGE
*
* Name    : 按键
* Version : V1.0
* Author  : 空格键
* ----------------------------
* Note(s) : 外接上拉电阻。按下低电平0，松开高电平1
*           扫描周期: 10ms
* Tip(s)  : 概念：
*             + 按下：按键刚按下
*             + 释放：按键刚松开
*             + 重复：按键被一直按住，键值重复出现（1s后，每0.1s一个信号）
*             + 长按：按住一定时间不松开（大于2s）
*             + 短按：按键刚松开，并且时长小于重复（小于1s）
*           按键编码设计（支持组合按键）：
*             + make=0x80 通码，标志位 1表示按下，0表示弹起
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include "bsp_kbd.h"



/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

//按键状态
typedef struct
{
    u8 down;        //按下标志：1-按下，0-松开
    u8 down_count;  //按下计数
    u16 press_time; //按下时长，实际是扫描次数
} KEY_STATE_TYP;

/**按键引脚定义**/
#define K1 P32      //功能

/**扫描周期（单位 ms）**/
#define K_SCAN_CYCLE       10 //原值10ms

/**按键计数定义**/
#define K_CNT_SHAKE        20/K_SCAN_CYCLE //消抖20ms
#define K_CNT_REP_SPEED   100/K_SCAN_CYCLE //重复按键速度，1s 10个，一个周期是100ms，10ms扫描一次，扫描10次计一个重复。
#define K_CNT_REP_END    1000/K_SCAN_CYCLE //按下1s不放为重复键
#define K_CNT_LONG_PRESS 2000/K_SCAN_CYCLE //按下2s不放为长按

static KEY_STATE_TYP idata k_s[NR_K];      //按键状态
static u8            idata key_code[NR_K]; //按键码



/*
*********************************************************************************************************
*                                             INTERRUPT
*********************************************************************************************************
*/
//TODO 使用中断优化

/*
*********************************************************************************************************
*                                         PRIVATE FUNCTION
*********************************************************************************************************
*/

/* 按键扫描，扫描周期 K_SCAN_CYCLE */
// kx=1 && kx_s.down=0  一直未按
// kx=0 && kx_s.down=0  刚按下
// kx=0 && kx_s.down=1  一直按着
// kx=1 && kx_s.down=1  刚松开
static u8 _Key_Scan(bit kx, KEY_STATE_TYP *kx_s)
{
    u8 ret = K_NONE;

    if (kx_s->down == 1)
        ret |= K_MAKE;
    
    if (kx == 0)
    {
        kx_s->down_count++;
        kx_s->press_time++;

        if (kx_s->down == 0) //刚按下
        {
            if (kx_s->down_count == K_CNT_SHAKE) //消抖
            {
                kx_s->down = 1;
                kx_s->press_time = K_CNT_SHAKE;
                ret |= K_MAKE | K_PRESS;
            }
        }
        else //一直按着
        {
            if (kx_s->down_count == K_CNT_REP_END) //重复
            {
                kx_s->down_count -= K_CNT_REP_SPEED;
                ret |= K_REPEAT;
            }
            if (kx_s->press_time == K_CNT_LONG_PRESS) //长按
            {
                ret |= K_LONG;
            }
        }
    }
    else
    {
        if (kx_s->down == 1) //刚松开
        {
            kx_s->down = 0;
            kx_s->down_count = 0;
            ret |= K_RELEASE;
            if (kx_s->press_time < K_CNT_REP_END) //短按
                ret |= K_SHORT;
        }
        else
        {
            kx_s->down_count = 0;
        }
    }
    return ret; // 无按键按下
}

/*
*********************************************************************************************************
* Description : KBD初始化。
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_KBD_Init(void)
{
    u8 i;

    K1 = 1;      //引脚拉高

    for (i = 0; i < NR_K; i++)
    {
        k_s[i].down = 0;
        k_s[i].down_count = 0;
        k_s[i].press_time = 0;
        key_code[i] = K_NONE;
    }
}

/*
*********************************************************************************************************
* Description : 按键扫描
*
* Return(s)   : true-有按键按下，false-无按键动作
*
* Note(s)     : 扫描周期T=10ms
*********************************************************************************************************
*/
boolean BSP_KBD_Scan()
{
    key_code[0] = _Key_Scan(K1, &k_s[0]);
    return !BSP_KBD_IsAllNone();
}

/*
*********************************************************************************************************
* Description : 获取按键编码
*
* Argument(s) : kx - 按键类型
*
* Return(s)   : 按键编码
*********************************************************************************************************
*/
u8 BSP_KBD_KeyCode(KEY_E_TYP kx)
{
    if (kx == NR_K)
        return K_NONE;
    return key_code[kx];
}

/*
*********************************************************************************************************
* Description : 按键动作判断
*
* Argument(s) : kx - 按键类型
*
* Return(s)   : [boolean] true/false
*********************************************************************************************************
*/
//是否无动作
boolean BSP_KBD_IsNone(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return keyCode == K_NONE ? true : false;
}
//是否刚按下
boolean BSP_KBD_IsPress(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return (keyCode & K_MAKE) && (keyCode & K_PRESS) ? true : false;
}
//是否刚释放
boolean BSP_KBD_IsRelease(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return (keyCode & K_MAKE) && (keyCode & K_RELEASE) ? true : false;
}
//是否重复
boolean BSP_KBD_IsRepeat(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return (keyCode & K_MAKE) && (keyCode & K_REPEAT) ? true : false;
}
//是否长按
boolean BSP_KBD_IsLong(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return (keyCode & K_MAKE) && (keyCode & K_LONG) ? true : false;
}
//是否短按
boolean BSP_KBD_IsShort(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    return (keyCode & K_MAKE) && (keyCode & K_SHORT) ? true : false;
}
//是否按键全无动作
boolean BSP_KBD_IsAllNone()
{
    u8 i;
    for (i = 0; i < NR_K; i++)
    {
        if (key_code[i] != K_NONE)
            return false;
    }
    return true;
}

/*
*********************************************************************************************************
* Description : 获取按键时长
*
* Argument(s) : kx - 按键
*
* Return(s)   : [s32] 按键时长，数值表示扫描周期的倍数。-1 表示无效值
*********************************************************************************************************
*/
s32 BSP_KBD_GetPressTime(KEY_E_TYP kx)
{
    u8 keyCode = BSP_KBD_KeyCode(kx);
    if (!(keyCode & K_MAKE))
        return -1;
    
    return k_s[kx].press_time;
}

/**********************************************RND******************************************************/
