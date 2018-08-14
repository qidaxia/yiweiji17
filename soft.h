#ifndef _SOFT_H_
#define _SOFT_H_
#include "sys.h"
#include "TLC5615.h"


/**
* @brief:     切换为最大速度
* @param[in]:  dir 方向
* @param[out]:
* @return:
* @retval:
* @note:
*/
extern void speedToMax(DIRECT dir);
/**
* @brief:     切换为最小速度
* @param[in]:  dir 方向
* @param[out]:
* @return:
* @retval:
* @note:
*/
extern void speedToMin(DIRECT dir);



/**
 * @brief:     软启动过程
 * @param[in]: speedType SPEED_UP/SPEED_DOWN
 * @param[in]: dir	方向
 * @param[in]: LimitA 限位函数
 * @param[in]: requestFun	请求函数
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void soft(SpeedType speedType, DIRECT dir, u8(*LimitA)(void), u8(*requestFun)(void));

#endif // !_SOFT_H_
