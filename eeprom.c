#include "eeprom.h"

extern void eepromWrite(u16 address, u8 dat)
{
	/*等待上一次写操作结束*/
	while (EECR &BIT(EEWE));
	/*设置地址和数据寄存器*/
	EEAR = address;
	EEDR = dat;
	/*置位EEMWE*/
	EECR |= BIT(EEMWE);
	/*置位EEWE以启动写操作*/
	SREG &= ~BIT(7);
	EECR |= BIT(EEWE);
	SREG |= BIT(7);
	delay_ms(1);
	return;
}

extern u8 eepromRead(u16 address)
{
	while (EECR&BIT(EEWE));
	/*设置地址寄存器*/
	EEAR = address;
	/*设置EERE以启动读操作*/
	EECR |= BIT(EERE);
	return EEDR;
}

