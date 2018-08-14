#ifndef _EEPROM__H
#define _EEPROM__H
#include "sys.h"

/**
 * @brief:     向eeprom指定地址写入指定数据
 * @param[in]: address 写入的地址
 * @param[in]: dat 写入的数据
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void eepromWrite(u16 address, u8 dat);


/**
 * @brief:     读出eeprom中指定地址的数据
 * @param[in]:  address 要读的地址
 * @param[out]:
 * @return:     u8
 * @retval:    数据
 * @note:	   
*/
extern u8   eepromRead(u16 address);

#endif // !_EEPROM__H
