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



#define ModeInit	0
#define RedGreen	1
#define RedYellow	2
#define GreenRed	3
#define YellowRed	4

#define ManRed		5
#define ManYellow	6
#define ManGreen	7

// global variables
extern int mode;

extern int timeRed;
extern int timeYellow;
extern int timeGreen;

extern int timer;

extern int blinkstate;

#endif /* INC_GLOBAL_H_ */
