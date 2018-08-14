#ifndef _SYS_H__
#define _SYS_H__
#include <iom16v.h>
#include <macros.h>

typedef unsigned char u8;

typedef unsigned int u16;

typedef  unsigned long  u32;

typedef enum { OK = 0, ERROR = !(OK) }Status;
typedef enum { NO = 0, YES = !(NO) }Whether;

//Enumeration definition of control instructions.
typedef enum
{
	STOP = 'S',		///< 停止（default）
	UP = 'U',		///< 上升
	DOWN = 'D',		///< 下降
	FORWARD = 'F',	///< 前进
	BACK = 'B',		///< 后退
	POWER = 'P',	///< 电源


	//SOFT
	CFG_SOFT_ADD_WALK = '1',		///< 增加 行走软启动时间
	CFG_SOFT_ADD_LIFT = '2',		///< 增加 提升软启动时间

	CFG_TOPSPEED_SUB_WALK = '3',	///< 减小 行走软启动速度上限
	CFG_TOPSPEED_SUB_LIFT = '4',	///< 减小 提升软启动速度上限

	CFG_LOWSPEED_ADD_WALK = '5',	///< 增加 行走软启动速度下限
	CFG_LOWSPEED_ADD_LIFT = '6',	///< 增加 提升软启动速度下限

	ADD_BRAKE_RELEASE_DELAY = '7',	///< 增加 松闸间隙（默认为0）

	CFG_SOFT_ADD_WALK_DOWN = '8',	///< 增加 行走软停止时间
	CFG_SOFT_ADD_LIFT_DOWN = '9',	///< 增加 提升软停止时间

	CFG_TOMINSPEED_WALK = 'X',		///< 最小速度行走
	CFG_TOMINSPEED_LIFT = 'Y',		///< 最小速度提升
	CFG_READ = 'R',					///< 读取配置

	CFG_SET_THRESHOLD_B = 'C',		///< 设置当前电压为电压下限
	CFG_SET_THRESHOLD_T = 'T',		///< 设置当前电压为电压上限
	CFG_READ_CURRENT_ENERGY='N',	///< 读取当前电量
	CFG_RESTART = 'E',				///< 软件复位
	CFG_HAND = 'H',					///< 握手

	CFG_LOAD_PARAMS='L',			///< 从eeprom中加载参数
	CFG_UPLOAD_PARAMS='M',			///< 将参数保存至eeprom中





}CMD;

typedef enum
{
	PTOP = '+',			///< 电动式
	LOCK = '#'			///< 自锁式
}REMOTEMODE;




typedef struct 
{
	u16 Current_energy;		///< 当前电压
	u16 Threshole_top;		///< 门限电压
	u16 Threshole_bottom;	///< 门限电压

}ENERGY;


typedef struct  
{
	u16 MaxSpeed_walk;			///< 最大行走速度
	u16 MaxSpeed_lift;			///< 最大提升速度
	u16 MinSpeed_walk;			///< 最小行走速度
	u16 MinSpeed_lift;			///< 最小提升速度
	u16 SpeedUpDelay_walk;		///< 加速时间（行走）
	u16 SpeedDownDelay_walk;	///< 减速时间（行走）
	u16 SpeedUpDelay_lift;		///< 加速时间（提升）
	u16 SpeedDownDelay_lift;	///< 减速时间（提升）
	u16 BrakeReleaseDelay;		///< 松闸时间
}SpeedLimit;



typedef enum
{
	SPEED_UP = 0,		///< 加速
	SPEED_DOWN			///< 减速
}SpeedType;

typedef enum
{
	LIFT = 0,			///< 提升
	WALK				///< 行走
}DIRECT;


//如果使用内部1M晶振E1 99 FF
#define  mclk   8000000

#define	TRUE		1
#define FALSE		!TRUE



void delay_ms(u16 ms);
void delay_us(u16 us);


#endif