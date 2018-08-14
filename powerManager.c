#include "powerManager.h"
#include "designed.h"
#pragma interrupt_handler SavePowerISR:10



extern void powerManageCFG(void)
{
	TCCR0 = 0x00;
	TCNT0 = 0xB2;

	TIMSK |= BIT(TOIE0);
	TIFR |= BIT(TOV0);
	TCCR0 = 0X05;

	SREG |= BIT(7);
}


/**
 * @brief:     定时器中断服务函数
 * @param[in]: 
 * @param[out]:
 * @return:    
 * @retval:    
 * @note:	   3.8次为一秒钟,228为一分钟
*/
static void SavePowerISR(void)
{
	TCNT0 = 0XB2;
	powerTimeCNT++;
	if (powerTimeCNT >= 100 * 60 * 2)//(1 - 109)min,由变量类型得出
	{

		if (savePowerFlag == 0)
		{
			POWER_CLOS;
			savePowerFlag = 1;
		}
		powerTimeCNT = 0;
	}
}
