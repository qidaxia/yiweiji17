#ifndef _USART_H__
#define _USART_H__


#include "sys.h"


/**@name 命令长度限定
 *@{	
 */
#define CMDLEN_MAX		20	/*!< 最大长度  */
#define CMDLEN_MIN		12	/*!< 最小长度  */
#define cmdSTART_LEN	5	/*!< 命令头长度  */
#define cmdSTOP_LEN		3	/*!< 命令尾长度  */
 
/**@} */
 


/**@name 命令头，命令尾
 *@{	
 */
extern const u8 cmdStart[cmdSTART_LEN];	/*!< 命令头  */
extern const u8 cmdStop[cmdSTOP_LEN];	/*!< 命令尾  */
 
/**@} */
 




/**
 * @brief:     usart初始化
 * @param[in]: baud 波特率
 * @param[out]:
 * @return:   
 * @retval:    
 * @note:	   
*/
extern void usartInit(u16 baud);

/**
 * @brief:     串口发送
 * @param[in]: dat 待发送数据
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void uartSendB(u8 dat);

/**
 * @brief:     串口发送一条命令
 * @param[in]: buf 命令首地址
 * @param[in]: len 命令长度
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void sendCMD(u8 *buf, u8 len);


/**
 * @brief:     获取缓冲区中命令字节，返回首地址和命令长度
 * @param[in]: datLen 获取的命令长度
 * @param[out]:
 * @return:    u8 
 * @retval:    命令首地址
 * @note:	   
*/
u8 * getFrameData(u8 *datLen);




#endif // !_USART_H

