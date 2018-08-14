/**
 * @file designed.c
 * @date 2018/03/09 9:08
 *
 * @author QiDaXia
 * Contact: 1176201157@qq.com
 *
 * @brief:
 BLD-300B驱动器：
	 DC+/DC- 直流电压输入端  （DC24V~DC48V）
	 REF+ 霍尔电源+ REF- 霍尔电源-
	 SV 外接调速。使用内部调速时悬空。注①
	 F/R 方向，悬空或高电平时为正转，低电平反转
	 EN 控制信号使能端 高电平停车，低运行
	 BRK  刹车，低电平时为正常工作，高电平停机
 BLD-70驱动器：
	SV:调速信号，外部模拟信号输入
	F/R:方向信号，悬空或高电平正转，与COM短接或低电平反转
	BRK:刹车信号，悬空或高电平刹车，与COM短接或低电平运行
 *
 * description:
 * 能够通过按键和上位机控制运行，也能使用上位机配置参数，参数掉电不保存，
 * 提升和行走，默认都有软启动过程
 * note:

*/

#include "designed.h"
#include "soft.h"
#include "hooks.h"
#include "usart.h"
#include "eeprom.h"




/**
 *	否已经进入了省电模式
 */
u8 savePowerFlag = 0;


/**
 *	无操作计数（extern）
 */
u16 powerTimeCNT = 0;


/**
 *	运动参数控制变量
 */
volatile SpeedLimit speed;

/**
 *	电量检测变量
 */
volatile ENERGY energy;


u8 ack[] = { 'A','C' ,'K' };
u8 ackLenError[] = { 'L','E','N','_', 'E','R','R' };
u8 ackCheckError[] = { 'C','H','K','_', 'E','R','R' };
u8 ackIdle[] = { 'I','D','L','E' };
u8 ackOk[] = { '_', 'O', 'K' };

u8 ackLimitA[] = { 'L','I','M','_','A' };
u8 ackLimitB[] = { 'L','I','M','_','B' };

u8 ackCharging[] = { 'C','H','A','R','G' };
u8 ackUlink[] = { 'U','L','L','I','N','K' };




extern void toUp(void)
{
	GO_UP;
	EN_T;
	delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_T;

}

extern void toDown(void)
{
	GO_DOWN;
	EN_T;
	delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_T;

}

extern void toForward(void)
{
	GO_FORWARD; delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_W;
}

extern void toBack(void)
{
	GO_BACK; delay_ms(speed.BrakeReleaseDelay);
	DIS_BRK_W;
}

extern void toStopT(void)
{
	speedToMin(LIFT);
	DIS_T; //delay_ms(5);
	EN_BRK_T;
}

extern void toStopW(void)
{
	speedToMin(WALK);
	EN_BRK_W;
}

extern void toStop(void)
{
	toStopT();
	toStopW();
}


extern void handlerLiftForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void))
{
	u16 timeout = 0;
	speedToMin(LIFT);
	if (savePowerFlag || !requestFun())
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendCMD(ackLimitA, 5);
		return;
	}
	//切换方向，
	dirFun();
	soft(SPEED_UP, LIFT, limitFun, requestFun);
	//speedToMax(LIFT);
	if (limitFun())
	{
		toStopT();
		sendCMD(ackLimitA, 5);
		goto END;
	}
	delay_ms(5);

	while (TRUE)
	{
		_WDR();
		timeout++;
		delay_ms(10);
		if (timeout >= 400)
		{
			timeout = 400;
		}
		if (!requestFun())//中途检测到停止
		{
			delay_ms(20);
			if (!requestFun())
			{
				break;
			}
		}
		if (limitFun())
		{
			delay_ms(10);
			if (limitFun())
			{
				toStopT();
				sendCMD(ackLimitA, 5);
				goto END;
			}
		}
	}
	if (timeout >= 400)
	{
		soft(SPEED_DOWN, LIFT, limitFun, requestFun);
	}
END:
	//立刻停止
	toStopT();
	delay_ms(600);
	return;
}

extern void handlerWalkForKey(void(*dirFun)(void), u8(*limitFun)(void), u8(*requestFun)(void))
{
	u16 timeout = 0;
	speedToMin(WALK);
	delay_ms(10);
	if (savePowerFlag || !requestFun())
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendCMD(ackLimitB, 5);
		return;
	}
	//切换方向，开始软启动
	dirFun();
	soft(SPEED_UP, WALK, limitFun, requestFun);
	//软启动过程中可能遇到限位
	if (limitFun())
	{
		toStopW();
		sendCMD(ackLimitB, 5);
		goto END;
	}

	while (TRUE)
	{
		_WDR();
		timeout++;
		delay_ms(10);
		if (timeout >= 400)
		{
			timeout = 400;
		}
		if (!requestFun())//break
		{
			delay_ms(10);
			if (!requestFun())
			{
				break;
			}
		}
		if (limitFun())
		{
			toStopW();
			sendCMD(ackLimitB, 5);
			goto END;
		}
	}
	if (timeout >= 400)
	{
		soft(SPEED_DOWN, WALK, limitFun, requestFun);
	}
END:
	//立刻停止
	toStopW();
	delay_ms(600);
	return;

}

extern void handlerLiftForCmd(CMD dir, u8(*limitFun)(void))
{
	CMD cmd = STOP;
	u16 timeout = 0;
	speedToMin(LIFT);
	delay_ms(10);
	if (savePowerFlag)
	{
		onceBeep();
		return;
	}
	if (limitFun())
	{
		sendCMD(ackLimitA, 5);
		return;
	}
	//切换方向，
	if (dir == DOWN)
	{
		toDown();
	}
	else
	{
		toUp();
	}
	soft(SPEED_UP, LIFT, limitFun, idleHooks);
	if (limitFun())
	{
		toStopT();
		sendCMD(ackLimitA, 5);
		goto END;
	}
	delay_ms(5);

	while (TRUE)
	{
		if (checkFrame(&cmd) == 0)
		{
			goto END;
		}

		if (limitFun())
		{
			toStopT();
			sendCMD(ackLimitA, 5);
			goto END;
		}
	}

END:
	//stop
	toStopT();
	delay_ms(600);
	return;
}


extern void handlerWalkForCmd(CMD dir, u8(*limitfun)(void))
{
	CMD cmd = STOP;
	u16	timeout = 0;
	speedToMin(WALK);
	if (savePowerFlag || limitfun())
	{
		onceBeep();
		return;
	}
	if (limitfun())
	{
		sendCMD(ackLimitB, 5);
		return;
	}
	if (dir == FORWARD)
	{
		toForward();
	}
	else
	{
		toBack();
	}
	soft(SPEED_UP, WALK, limitfun, idleHooks);
	if (limitfun())
	{
		toStopW();
		sendCMD(ackLimitB, 5);
		goto END;
	}

	while (TRUE)
	{
		if (checkFrame(&cmd) == 0)
		{
			goto END;
		}
		if (limitfun())
		{
			toStopW();
			sendCMD(ackLimitB, 5);
			goto END;
		}
	}

END:
	//stop 
	toStopW();
	delay_ms(600);
	return;
}


extern void handlerPower(u8 flag)
{
	if (savePowerFlag == 1)//已经进入了省电模式
	{
		POWER_OPEN;
		savePowerFlag = 0;
	}
	else
	{
		POWER_CLOS;
		savePowerFlag = 1;
	}
	onceBeep();
	if (flag)
	{
		while (IsPowerRequest())
		{
			_WDR();
		}
	}
	delay_ms(100);
}




extern void onceBeep(void)
{
	BEEP_ON;
	delay_ms(50);
	BEEP_OFF;
}

/*
IO：

复位时：单片机引脚为高阻态（注意看门狗复位时的安全问题，需要硬件有确定电平）

当引脚配置为输出时，如果不指定电平，默认是输出低电平。

在( 高阻态) 三态	({DDxn, PORTxn} = 0b00)
输出高电平		({DDxn, PORTxn} = 0b11)
两种状态之间进行切换时，
上拉电阻使能		({DDxn, PORTxn} = 0b01)
或输出低电平		({DDxn,PORTxn} = 0b10)
这两种模式必然会有一个发生。
*/


extern void ioInit(void)
{
	//---
	EN_BRK_T;
	EN_BRK_W;

	//---Vin
	DDRA &= ~BIT(0);
	PORTA &= ~BIT(0);

	DDRA |= 0xfe;
	EN_BRK_T;
	EN_BRK_W;
	POWER_OPEN;
	DIS_T;

	//---------------
	DDRC = 0x00;
	PORTC = 0xff;//20K——50K
	DDRC |= BIT(IO_LED);

	DDRD |= BIT(IO_BEEP);
	DDRD |= BIT(IO_LED_CHAGRGE);
	LED_CHARGE_OFF;
	BEEP_OFF;
	DDRD &= ~BIT(IO_LIMIT_UP);
	DDRD &= ~BIT(IO_LIMIT_DOWN);
	DDRD &= ~BIT(IO_INPLACE);
	DDRD &= ~BIT(IO_CHARGE);
	PORTD |= BIT(IO_LIMIT_DOWN);
	PORTD |= BIT(IO_LIMIT_UP);
	PORTD |= BIT(IO_INPLACE);
	PORTD |= BIT(IO_CHARGE);
	LED_OFF;
}


extern void keyDown(void)
{
	LED_ON;
	if (savePowerFlag == 0)
	{
		powerTimeCNT = 0;
	}
	_WDR();
}

extern void keyOff(void)
{
	LED_OFF;
	delay_ms(200);
}

extern void watchDog_init(void)
{

	_WDR();
	WDTCR = 0X1F;
	WDTCR = 0X0F;
	return;
}



//数据帧正确，返回0；
//未收到数据，返回1
//长度错误，返回2，
//校验未通过，返回3，
extern u8 checkFrame(CMD *cmd)
{
	u8 *cmdPtr = (void *)0;
	u8 cmdLen = 0;

	_WDR();
	cmdPtr = getFrameData(&cmdLen);
	if (cmdPtr == (void *)0)
	{
		return 1;
	}
	else if (cmdLen != 4)
	{
		sendCMD(ackLenError, 7);
		return 2;
	}
	else if (cmdPtr[0] + cmdPtr[1] + cmdPtr[2] != cmdPtr[3])
	{
		sendCMD(ackCheckError, 7);
		return 3;
	}
	else
	{
		*cmd = (CMD)cmdPtr[1];
		return 0;
	}
}


extern void echo(void)
{
	uartSendB(speed.BrakeReleaseDelay >> 8);
	uartSendB(speed.BrakeReleaseDelay);
	uartSendB(0xAA);
	uartSendB(speed.MaxSpeed_lift >> 8);
	uartSendB(speed.MaxSpeed_lift);
	uartSendB(0xAA);
	uartSendB(speed.MaxSpeed_walk >> 8);
	uartSendB(speed.MaxSpeed_walk);
	uartSendB(0xAA);
	uartSendB(speed.MinSpeed_lift >> 8);
	uartSendB(speed.MinSpeed_lift);
	uartSendB(0xAA);
	uartSendB(speed.MinSpeed_walk >> 8);
	uartSendB(speed.MinSpeed_walk);
	uartSendB(0xAA);
	uartSendB(speed.SpeedDownDelay_lift >> 8);
	uartSendB(speed.SpeedDownDelay_lift);
	uartSendB(0xAA);
	uartSendB(speed.SpeedDownDelay_walk >> 8);
	uartSendB(speed.SpeedDownDelay_walk);
	uartSendB(0xAA);
	uartSendB(speed.SpeedUpDelay_lift >> 8);
	uartSendB(speed.SpeedUpDelay_lift);
	uartSendB(0xAA);
	uartSendB(speed.SpeedUpDelay_walk >> 8);
	uartSendB(speed.SpeedUpDelay_walk);
	//add energy
	uartSendB(0xAA);
	uartSendB(energy.Current_energy >> 8);
	uartSendB(energy.Current_energy);
	uartSendB(0xAA);
	uartSendB(energy.Threshole_top >> 8);
	uartSendB(energy.Threshole_top);
	uartSendB(0xAA);
	uartSendB(energy.Threshole_bottom >> 8);
	uartSendB(energy.Threshole_bottom);

	return;
}


extern void saveParameterToEeprom(void)
{
	u16 addr = 0x00;
	u8 *base = (u8 *)(&energy.Current_energy);
	u8 i = 0;
	for (i = 0; i < 3 * 2; i++)
	{
		eepromWrite(addr++, *(base + i));
	}
	base = (u8 *)(&speed.MaxSpeed_walk);
	for (i = 0; i < 9 * 2; i++)
	{
		eepromWrite(addr++, *(base + i));
	}
	return;
}

extern void readParameterAtEeprom(void)
{
	u16 addr = 0x00;
	u8 *base = (u8 *)(&energy.Current_energy);
	u8 i = 0;
	for (i = 0; i < 3 * 2; i++)
	{
		*(base + i) = eepromRead(addr++);
	}
	base = (u8 *)(&speed.MaxSpeed_walk);
	for (i = 0; i < 9 * 2; i++)
	{
		*(base + i) = eepromRead(addr++);
	}
}

u8 idleHooks(void)
{
	return TRUE;
}
