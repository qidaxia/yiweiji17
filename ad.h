#ifndef AD_H__
#define AD_H__
#include "sys.h"


/**
 * @brief:     adc配置
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   
*/
extern void adc_init(void);


/**
 * @brief:     滤波后返回
 * @param[in]: void
 * @param[out]:
 * @return:     u16
 * @retval:    adc值
 * @note:	   范围：0--1024
*/
extern u16 get_adc(void);

#endif
