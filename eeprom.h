#ifndef _EEPROM__H
#define _EEPROM__H
#include "sys.h"

/**
 * @brief:     ��eepromָ����ַд��ָ������
 * @param[in]: address д��ĵ�ַ
 * @param[in]: dat д�������
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void eepromWrite(u16 address, u8 dat);


/**
 * @brief:     ����eeprom��ָ����ַ������
 * @param[in]:  address Ҫ���ĵ�ַ
 * @param[out]:
 * @return:     u8
 * @retval:    ����
 * @note:	   
*/
extern u8   eepromRead(u16 address);

#endif // !_EEPROM__H
