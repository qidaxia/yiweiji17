/**
* @file		TLC5615.h
* @brief	TLC5615操作
* @details	定义了TLC5615的一些操作和常用宏
* @author   Qidaxia
* @date     2017-6-24
* @version  V8.1
* @par Copyright (c):  JED
*/
#ifndef _TLC5615_H
#define _TLC5615_H
#include "sys.h"


#define START_DELAY		2 //2
#define STOP_DELAY		2 //2

/**@name	SPI端口定义
*  @{
*/
#define SPI_SCK			PB7		/*!<时钟线*/
#define SPI_MISO		PB6		/*!<主机输入，从机输出*/
#define SPI_MOSI		PB5		/*!<主机输出，从机输入*/
#define SPI_SS			PB4		/*!<片选端口*/
/**@}*/

/**@name	TLC5615操作
* @{
*/
#define TLC5615_CS		PB4							/*!<TLC5615 使能端口*/
#define TLC5615_ENABLE	PORTB &=~BIT(TLC5615_CS)	/*!<使能TLC5615*/
#define TLC5615_DISABLE	PORTB |=BIT(TLC5615_CS)		/*!<关闭TLC5615*/
/**
* @}
*/


/**
 * @brief:     spi初始化
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void SPI_MasterInit(void);


/**
 * @brief:     spi主机发送
 * @param[in]: iData 要发送的数据
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void SPI_MasterTransmit(u16 iData);


#endif