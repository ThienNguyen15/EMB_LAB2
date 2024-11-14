/*
 * global.h
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include<math.h>

#include "software_timer.h"
#include "button.h"
#include "fsm_mode.h"

#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

#include "software_timer.h"
#include "led_7seg.h"
#include "button.h"
#include "lcd.h"
#include "picture.h"
#include "ds3231.h"
#include "uart.h"



#define ModeInit			0
#define NormalMode			1
#define SettingMode			2
#define AlarmSettingMode	3


// global variables
extern int mode;
extern int changemode;

extern int year, month, date;
extern int day;
extern int hour, min, sec;

extern int current_parameter;

extern int blinkstate;

#endif /* INC_GLOBAL_H_ */
