/**
 * @file hooks.c
 * @date 2018/03/09 18:38
 *
 * @author QiDaXia
 * Contact: 1176201157@qq.com
 *
 * @brief: 一些钩子函数，判断输入信号有无
 *
 * description:
 *
 * note:
*/

#include "hooks.h"
#include "designed.h"


extern u8 IsBackRequest(void)
{
	if (PINC&BIT(IO_BACK))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsForwardRequest(void)
{
	if (PINC&BIT(IO_FORWARD))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsUpRequest(void)
{
	if (PINC&BIT(IO_UP))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsDownRequest(void)
{
	if (PINC&BIT(IO_DOWN))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsSettingRequest(void)
{
	if (PINC&BIT(IO_SETTING))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsPowerRequest(void)
{
	if (PINC&BIT(IO_POWER))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsContact(void)
{
	if (PINC&BIT(IO_CONTACT))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsLimitUp(void)
{
	if (PIND&BIT(IO_LIMIT_UP))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsLimitDown(void)
{
	if (PIND&BIT(IO_LIMIT_DOWN))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsCharge(void)
{
	if (PIND&BIT(IO_CHARGE))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsInPlace(void)
{
	if (PIND&BIT(IO_INPLACE))
	{
		return FALSE;
	}
	return TRUE;
}

extern u8 IsKeyPress(void)
{
	if ((PINC & 0x3F) == 0x3F)
	{
		return FALSE;
	}
	return TRUE;
}

CMD keyScan(void)
{
	CMD cd = STOP;
	switch (PINC & 0x3F)
	{
	case 0x3E:
		cd = BACK;
		break;
	case 0x3D:
		cd = FORWARD;
		break;
	case 0x3B:
		break;
	case 0x37:
		cd = POWER;
		break;
	case 0x2F:
		cd = UP;
		break;
	case 0x1F:
		cd = DOWN;
		break;
	default:
		break;
	}
	return cd;
}



