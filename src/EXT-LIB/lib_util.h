/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : 工具函数库
*********************************************************************************************************
*/

#ifndef __LIB_UTIL_H__
#define __LIB_UTIL_H__

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include "lib_stdint.h"

#include "lib_math.h"


/*
*********************************************************************************************************
*                                               DEFINE
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          DEFINE FUNCTION
*********************************************************************************************************
*/

//置位。对应位 置1，dat从 左->右 => 高->低
#define util_set(dat, i) \
                ( dat | (0x01 << i) )
//清位。对应位 清0，dat从 左->右 => 高->低
#define util_reset(dat, i) \
                ( dat & ~(0x01 << i) )

/*
转换调节
val：原始值
step：调节步长
lower：范围下限
upper：范围上线
*/
#define util_turn_adj(val, step, lower, upper) \
                ( (step >= 0) ? \
                    math_min(val + step, upper) : \
                    math_max(val + step, lower) \
                )

/*
回环调节
val：原始值
step：调节步长
lower：范围下限
upper：范围上线
*/
#define util_rotate_adj(val, step, lower, upper) \
                ( (step >= 0) ? \
                    ( upper - val >= step ? (val + step) : (lower + (step - (upper - val)) - 1) ) : \
                    ( lower - val <= step ? (val + step) : (upper + (step - (lower - val)) + 1) ) \
                )

/*
长度
obj：变量对象
type：对象对应的类型
*/
#define util_size(obj, type) \
                ( sizeof(obj) / sizeof(type) )

/*
*********************************************************************************************************
*                                             FUNCTION
*********************************************************************************************************
*/

// extern u8   util_set(byte dat, u8 i) compact reentrant;
// extern u8   util_reset(u8 dat, u8 i) compact reentrant;

#endif
