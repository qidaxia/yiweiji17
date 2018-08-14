

#include "soft.h"
#include "designed.h"




extern void speedToMax(DIRECT dir)
{
	SPI_MasterTransmit((dir == WALK) ? speed.MaxSpeed_walk : speed.MaxSpeed_lift);
}

extern void speedToMin(DIRECT dir)
{
	SPI_MasterTransmit((dir == WALK) ? speed.MinSpeed_walk : speed.MinSpeed_lift);
}

extern void soft(SpeedType speedType, DIRECT dir, u8(*LimitA)(void),u8(*requestFun)(void))
{
	u16 i;
	u16 max, min, timeDelay;
	CMD cmd = STOP;
	_WDR();

	/*max = (dir == WALK) ? speed.MaxSpeed_walk : speed.MinSpeed_lift;
	min = (dir == WALK) ? speed.MinSpeed_walk : speed.MinSpeed_lift;
	timeDelay = (dir == WALK) ? speed.SpeedUpDelay_walk : speed.SpeedUpDelay_lift;*/
	if (dir == WALK)
	{
		max = speed.MaxSpeed_walk;
		min = speed.MinSpeed_walk;
		timeDelay = speed.SpeedUpDelay_walk;
	}
	else
	{
		max = speed.MaxSpeed_lift;
		min = speed.MinSpeed_lift;
		timeDelay = speed.SpeedUpDelay_lift;
	}

	if (speedType == SPEED_DOWN)
	{
		goto SOFTDOWN;
	}
SOFTUP:
	for (i = min; i < max; i++)
	{
		SPI_MasterTransmit(i);
		delay_ms(timeDelay);
		if (LimitA()|| !checkFrame(&cmd)||!requestFun())
		{
			goto END;
		}
	}
	return;
SOFTDOWN:
	for (i = max; i < min; i--)
	{
		SPI_MasterTransmit(i);
		delay_ms(timeDelay);
		if (LimitA())
		{
			goto END;
		}
	}
	return;
END:
	toStop();
	return;
}

