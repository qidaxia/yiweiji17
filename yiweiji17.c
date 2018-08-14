/**
 * @file operating.c
 * @date 2018/07/13 18:24
 *
 * @author QiDaXia
 * @Contact: 1176201157@qq.com
 *
 * @brief:

 * @description:
 *
 * @note:

   熔丝位：FE D9 FF
*/

#include "designed.h"
#include "hooks.h"
#include "soft.h"
#include "powerManager.h"
#include "usart.h"
#include "TLC5615.h"
#include "ad.h"

int main(void)
{
	CMD cmd = STOP;
	u8 i = 0;
	u16 adcVal = 0;
	delay_ms(500);//Ensure power supply
	ioInit();
	usartInit(19200);
	SPI_MasterInit();
	watchDog_init();

	speed.MaxSpeed_lift = 1022;
	speed.MinSpeed_lift = 100;
	speed.SpeedDownDelay_lift = 0;
	speed.SpeedUpDelay_lift = 1;
	speed.BrakeReleaseDelay = 0;
	onceBeep();

	while (TRUE)
	{
		/*!
		* @brief:  按键控制
		*/
		if (IsKeyPress())
		{
			_WDR();
			keyDown();
			switch (keyScan())
			{
			case UP:
				handlerLiftForKey(toUp, IsLimitUp, IsUpRequest);
				break;
			case DOWN:
				handlerLiftForKey(toDown, IsLimitDown, IsDownRequest);
				break;
			default:
				break;
			}
			keyOff();
		}
		/*!
		* @brief:  指令控制
		*/
		if (checkFrame(&cmd) == 0)
		{
			_WDR();
			keyDown();
			sendCMD(ack, 3);
			switch (cmd)
			{
			case UP:
				handlerLiftForCmd(UP, IsLimitUp);
				break;
			case DOWN:
				handlerLiftForCmd(DOWN, IsLimitDown);
				break;

			case CFG_SOFT_ADD_LIFT:
				speed.SpeedUpDelay_lift++;
				echo();
				break;
			case CFG_SOFT_ADD_LIFT_DOWN:
				speed.SpeedDownDelay_lift++;
				echo();
				break;
			case CFG_TOPSPEED_SUB_LIFT:
				speed.MaxSpeed_lift -= 5;
				echo();
				break;
			case CFG_LOWSPEED_ADD_LIFT:
				speed.MinSpeed_lift += 5;
				echo();
				break;
			case ADD_BRAKE_RELEASE_DELAY:
				speed.BrakeReleaseDelay++;
				echo();
				break;
			case CFG_READ:
				echo();
				break;

				//复位，握手
			case CFG_RESTART:
				while (1);//wait for wdt restart
				break;
			case CFG_HAND:
				sendCMD(ackOk, 3);
				break;
				//eeprom操作
			case CFG_LOAD_PARAMS:
				readParameterAtEeprom();
				break;
			case CFG_UPLOAD_PARAMS:
				saveParameterToEeprom();
				break;

			default:
				toStop();
				break;
			}
			//sendCMD(ackIdle, 4);
			keyOff();
		}
	FINISHED:
		_WDR();
		toStop();
	}
	return 0;
}
