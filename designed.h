#ifndef _DESIGNED_H
#define _DESIGNED_H
#include "sys.h"



/**
 *	1:旧驱动器(bldc/zdbldc，需要网线设置)
 *	0:新驱动器(bldc，需要面板设置)
 */
#define DRIVE_OLD		1

/**
 *	1:使能充电管理
 *	0:不使能充电管理
 */
#define EN_CHARGE_MANAGE 1

/**@name 驱动器端口
 *@{	
 */
#define  IO_EN_M1	PA1
#define  IO_T_EN	PA2
#define  IO_T_FR	PA3
#define  IO_T_BRK	PA4
#define  IO_W_DIR	PA5
#define  IO_W_BRK	PA6
#define  IO_EN_M2	PA7
 
/**@} */
 


/**@name 手柄端口
 *@{	
 */
#define IO_BACK		PC0
#define IO_FORWARD	PC1
#define IO_SETTING	PC2
#define IO_POWER	PC3
#define IO_UP		PC4
#define IO_DOWN		PC5

 
/**@} */
 

/**@name 限位端口
 *@{	
 */
#define ST1				PD2
#define ST2				PD3
#define ST3				PD4
#define ST4				PD5

#define IO_LIMIT_UP		ST1
#define IO_LIMIT_DOWN	ST2
#define IO_INPLACE		ST3
#define IO_CHARGE		ST4
 
/**@} */
 

/**@name 其他端口
 *@{	
 */
#define IO_LED_CHAGRGE	PD6
#define IO_BEEP			PD7
#define IO_CONTACT		PC6
#define IO_LED			PC7
 
/**@} */
 


/*!
* @brief: 调试操作
*/
#define LED_ON			PORTC&=~BIT(IO_LED)
#define LED_OFF			PORTC|=BIT(IO_LED)
#define LED_CHARGE_ON	PORTD&=~BIT(IO_LED_CHAGRGE)
#define LED_CHARGE_OFF	PORTD|=BIT(IO_LED_CHAGRGE)
#define BEEP_ON			PORTD|=BIT(IO_BEEP)
#define BEEP_OFF		PORTD&=~BIT(IO_BEEP)

/*!
* @brief:  驱动器操作
*/

#if DRIVE_OLD==1
#define GO_UP			PORTA|=BIT(IO_T_FR)			/*!< 顺时针  */
#define GO_DOWN			PORTA&=~BIT(IO_T_FR)		/*!< 逆时针  */
#else
#define GO_UP			PORTA&=~BIT(IO_T_FR)		/*!< 顺时针  */
#define GO_DOWN			PORTA|=BIT(IO_T_FR)			/*!< 逆时针  */

#endif


/*!
* @brief:  电磁刹车（断电抱死刹车）
*/
#define EN_BRK_T		PORTA&=~BIT(IO_T_BRK)
#define DIS_BRK_T		PORTA|=BIT(IO_T_BRK)

#if DRIVE_OLD==1
#define DIS_T			PORTA&=~BIT(IO_T_EN)
#define EN_T			PORTA|=BIT(IO_T_EN)
#else
#define EN_T			PORTA&=~BIT(IO_T_EN)
#define DIS_T			PORTA|=BIT(IO_T_EN)
#endif


#define GO_FORWARD		PORTA&=~BIT(IO_W_DIR)
#define GO_BACK			PORTA|=BIT(IO_W_DIR)
#define EN_BRK_W		PORTA|=BIT(IO_W_BRK)
#define DIS_BRK_W		PORTA&=~BIT(IO_W_BRK)

/**@name 电源管理
 *@{
 */
#define POWER_OPEN		PORTA&=~BIT(IO_EN_M1)
#define POWER_CLOS		PORTA|=BIT(IO_EN_M1) 

 /**@} */


/**@name 外部变量extern
 *@{
 */
extern u8 ack[3];					/*!<成功收到指令，并校验通过   */
extern u8 ackLenError[7];			/*!<数据长度错误   */
extern u8 ackCheckError[7];			/*!<校验不通过   */
extern u8 ackIdle[4];				/*!<空闲（指令执行完成）   */
extern u8 ackOk[3];					/*!<握手应答   */
extern u8 ackCharging[5];			/*!<充电应答   */
extern u8 ackUlink[6];				/*!<接触不良应答   */
extern u8 savePowerFlag;			/*!<省电标志   */
extern u16 powerTimeCNT;			/*!<中断计数   */
extern volatile SpeedLimit speed;	/*!<速度控制变量   */
extern volatile ENERGY energy;		/*!<电量控制变量   */

/**@} */



/**@name 执行函数
 *@{	
 */
extern void toUp(void);			/*!< 上升   */
extern void toDown(void);		/*!< 下降  */
extern void toForward(void);	/*!< 前进  */
extern void toBack(void);		/*!< 后退  */
extern void toStopT(void);		/*!< 停止提升  */
extern void toStopW(void);		/*!< 停止行走  */
extern void toStop(void);		/*!< 全部停止  */
 
/**@} */
 





/**@name 处理请求函数
 *@{	
 */

/**
 * @brief:     处理提升按键停球
 * @param[in]: * dirFun 执行函数
 * @param[in]: * limitFun 限位函数
 * @param[in]: * requestFun	请求函数
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void handlerLiftForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void));

/**
* @brief:	  处理行走按键请求
* @param[in]: * dirFun 执行函数
* @param[in]: * limitFun 限位函数
* @param[in]: * requestFun	请求函数
* @param[out]:
* @return:
* @retval:
* @note:
*/
extern void handlerWalkForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void));


/**
 * @brief:     处理提升指令请求
 * @param[in]:  dir 方向
 * @param[in]: u8
 * @param[in]: limitFun	限位
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void handlerLiftForCmd(CMD dir, u8(*limitFun)(void));

/**
 * @brief:     处理行走指令请求
 * @param[in]:  dir 方向
 * @param[in]: u8
 * @param[in]: limitFun	限位
 * @param[in]: void
 * @param[out]:
 * @return:    extern void
 * @retval:    
 * @note:	   
*/
extern void handlerWalkForCmd(CMD dir, u8(*limitfun)(void));

/**
 * @brief:     处理电源
 * @param[in]: flag，是否为按键请求
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void handlerPower(u8 flag);
 
/**@} */
 



/**
 * @brief:     蜂鸣器响一声
 * @param[in]: 
 * @param[out]:
 * @return:   
 * @retval:    
 * @note:	   
*/
extern void onceBeep(void);

/**
 * @brief:     io初始化
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void ioInit(void);


/**
 * @brief:     有按键按下的时候执行一次
 * @param[in]: 
 * @param[out]:
 * @return:     
 * @retval:    
 * @note:	   
*/
extern void keyDown(void);


/**
 * @brief:     按键处理完成执行一次
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void keyOff(void); 


/**
 * @brief:     看门狗初始化
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void watchDog_init(void);


/**
 * @brief:    检查指令，并取得指令码和状态码 
 * @param[in]: 
 * @param[out]:cmd，取得指指令码
 * @return:    u8
 * @retval:		0：数据帧正确
 * @retval:		1：未收到数据
 * @retval:		2：长度错误
 * @retval:		3：校验未通过
 * @note:	   
*/
extern u8 checkFrame(CMD *cmd);



/**
 * @brief:     将各个参数上传至上位机
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void echo(void);


 


/**@name 参数读取
 *@{	
 */


/**
 * @brief:     将参数保存至eeprom中
 * @param[in]: void
 * @param[out]:
 * @return:    extern void
 * @retval:    
 * @note:	   
*/
extern void saveParameterToEeprom(void);


/**
 * @brief:     将参数从eeprom中加载
 * @param[in]: void
 * @param[out]:
 * @return:    extern void
 * @retval:    
 * @note:	   
*/
extern void readParameterAtEeprom(void);
 
/**@} */
 


/**
 * @brief:     空闲函数
 * @param[in]: 
 * @param[out]:
 * @return:   u8 
 * @retval:    1
 * @note:	   
*/
u8 idleHooks(void);

#endif // !_DESIGNED_H
