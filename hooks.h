#ifndef _HOOKS_H
#define  _HOOKS_H
#include "sys.h"



/**@name 手柄输入信号判断
 *@{	
 */
extern u8 IsBackRequest(void);		/*!<“后退”是否按下   */
extern u8 IsForwardRequest(void);	/*!<“前进”是否按下   */
extern u8 IsUpRequest(void);		/*!<“上升”是否按下   */
extern u8 IsDownRequest(void);		/*!<“下降”是否按下   */
extern u8 IsSettingRequest(void);	/*!<“设置”是否按下   */
extern u8 IsPowerRequest(void);		/*!<“电源”是否按下   */
 
/**@} */
 

extern u8 IsContact(void);			/*!<检测充电座与铜柱是否接触良好   */


/**@name 限位判断
 *@{	
 */
extern u8 IsLimitUp(void);		/*!< 是否有上限位  */
extern u8 IsLimitDown(void);	/*!< 是否有下限位  */
extern u8 IsCharge(void);		/*!< 是否有后限位  */
extern u8 IsInPlace(void);		/*!< 是否有前限位  */
 
/**@} */
 



/**
 * @brief:     手柄输入检测，是否手柄按下
 * @param[in]: void
 * @param[out]:
 * @return:     u8
 * @retval:    0：无按下
 * @retval:    1：有按下
 * @note:	   
*/
extern u8 IsKeyPress(void);


/**
 * @brief:    按键检测 返回键代码
 * @param[in]: void
 * @param[out]:
 * @return:    CMD
 * @retval:  STOP		停止
 * @retval:  UP			上升
 * @retval:  DOWN		下降
 * @retval:  FORWARD	前进
 * @retval:  BACK		后退
 * @retval:  POWER		电源
 * @note:	   
*/
CMD keyScan(void);

#endif // !_HOOKS_H
