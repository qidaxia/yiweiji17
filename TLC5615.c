#include "TLC5615.h"
#include "sys.h"


extern void SPI_MasterInit(void)
{
	DDRB |= BIT(SPI_MOSI);
	DDRB |= BIT(SPI_SCK);
	DDRB |= BIT(TLC5615_CS);
	/*!<使能spi主机模式，速率fck/16*/
	SPCR = BIT(SPE) | BIT(MSTR) | BIT(SPR0);
}


extern void SPI_MasterTransmit(u16 iData)
{
	_WDR();
	iData <<= 2;
	TLC5615_ENABLE;
	SPDR = (iData >> 8);
	while (!(SPSR&BIT(SPIF)));
	SPDR = iData;
	while (!(SPSR&BIT(SPIF)));
	TLC5615_DISABLE;
}
