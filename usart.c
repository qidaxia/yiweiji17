#include "usart.h"


#pragma interrupt_handler uart_rx:12

static u8 reciveLen = 0;
static u8 reciveBuff[256];//256
extern const   u8 cmdStart[cmdSTART_LEN] = { 's','t','a','r','t' };
extern const  u8 cmdStop[cmdSTOP_LEN] = { 'e','n','d' };

extern void usartInit(u16 baud)
{
	UCSRB = 0x00;
	UCSRA = 0x00;
	UCSRC = (1 << URSEL) | (0 << UPM0) | (3 << UCSZ0);
	baud = mclk / 16 / baud - 1;
	UBRRL = baud;
	UBRRH = baud >> 8;
	UCSRB = (1 << TXEN) | (1 << RXEN) | (1 << RXCIE);
	SREG = BIT(7);
	DDRD |= 0X02;
}

extern void uartSendB(u8 dat)
{
	_WDR();
	while (!(UCSRA&(BIT(UDRE))));
	UDR = dat;
	while (!(UCSRA&(BIT(TXC))));
	UCSRA |= BIT(TXC);
}

extern void sendCMD(u8 *buf, u8 len)
{
	u8 i = 0;
	for (i = 0; i < len; i++)
	{
		uartSendB(buf[i]);
	}
}

static void uart_rx(void)
{
	UCSRB &= ~BIT(RXCIE);
	reciveBuff[reciveLen++] = UDR;
	_WDR();
	UCSRB |= BIT(RXCIE);
}

static void clearReciveBuf(void)
{
	reciveLen = 0;
}

static u8 getReciveLen(void)
{
	return reciveLen;
}

/*!
* @brief:  Find the command header
success: return the corresponding index (the index of the command code start)
fail:	 to return 0xff
*/
static u8 searchCMDHead(u8 len)
{
	u8 i = 0;
	u8 j = 0;
	u8 headIndex = 0xff;
	_WDR();
	for (i = 0; i < len - 4; i++)
	{
		_WDR();
		if (reciveBuff[i] == cmdStart[0])
		{
			j = 1;
			while (reciveBuff[i + j] == cmdStart[j] && j < cmdSTART_LEN)
			{
				j++;
				_WDR();
			}
			if (j >= cmdSTART_LEN)//The match is successful
			{
				headIndex = i + cmdSTART_LEN;
				break;
			}
		}
	}
	return headIndex;
}

/*!
* @brief:  Find the command tail,
return YES successfully,
and fail to return NO.
*/
static Whether searchCmdTail(u8 len)
{
	u8 i = 0;

	while (reciveBuff[len - 1 - i] == cmdStop[cmdSTOP_LEN - 1 - i] && i < cmdSTOP_LEN)
	{
		_WDR();
		i++;
	}
	if (i >= cmdSTOP_LEN)
	{
		return YES;
	}
	return NO;
}

/*!
* @brief:  
Gets the first address of the data frame received in the buffer, 
as well as the length of the data (not including the command header and command tail), 
and if the data is empty, the null pointer is returned.
*/ 
extern u8 * getFrameData(u8 *datLen)
{
	u8 len = getReciveLen();
	u8 headIndex = 0xff;
	_WDR();
	if (len < CMDLEN_MIN)//Incomplete data
	{
		return (void *)0;
	}
	if (len > CMDLEN_MAX)//error
	{
		goto END;
	}
	//Find command tail
	if (searchCmdTail(len) != YES)//error
	{
		goto END;
	}
	//Find command head
	headIndex = searchCMDHead(len);
	if (headIndex == 0xff)//error
	{
		goto END;
	}
	clearReciveBuf();
	*datLen = len - headIndex -  cmdSTOP_LEN;
	return reciveBuff + headIndex;

END:
	clearReciveBuf();
	*datLen = 0;
	return (void *)0;
}


