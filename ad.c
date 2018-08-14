#include "ad.h"

#define  ADCBUFFERLEN	10

typedef struct
{
	u16  adcBase[ADCBUFFERLEN];
	u8	 index;
}ADCTYPE;

ADCTYPE adcManage = { {0},0 };

extern void adc_init(void)
{
	DDRA &= ~BIT(PA0);
	PORTA &= ~BIT(PA0);
	ADMUX = 0x40;//avcc
	ADCSR = 0X80;
}

static u16 adc(void)
{
	u16 addata;

	ADCSR |= BIT(ADSC);
	while (!(ADCSR&(BIT(ADIF))))
	{
		_WDR();
	}
	addata = ADCL;
	addata = addata + ADCH * 256;
	delay_ms(5);
	return addata;
}

extern u16 get_adc(void)
{
	u16 adctemp;
	u8 i = 0;
	u16 sum = 0;

	adctemp = adc();
	if (adcManage.index < ADCBUFFERLEN)
	{
		adcManage.adcBase[adcManage.index++] = adctemp;
		return adctemp;
	}
	else
	{
		/*!
		* @brief:  中位值平均滤波
		*/
		for (i = 0; i < ADCBUFFERLEN - 1; i++)
		{
			_WDR();
			sum += adcManage.adcBase[i];
			adcManage.adcBase[i] = adcManage.adcBase[i + 1];
		}
		adcManage.adcBase[ADCBUFFERLEN - 1] = adc();
		sum += adcManage.adcBase[ADCBUFFERLEN - 1];
		return sum / ADCBUFFERLEN;
	}

}
