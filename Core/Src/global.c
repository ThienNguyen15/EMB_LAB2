/*
 * global.c
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#include "global.h"

int mode = ModeInit;

int timer1 = 0;
int timer2 = 0;
int is_pressed = 0;

int year_temp = 0;
int month_temp = 0;
int date_temp = 0;
int day_temp = 0;
int hour_temp = 0;
int min_temp = 0;
int sec_temp = 0;

int year_alarm = 24;
int month_alarm = 11;
int date_alarm = 15;
int day_alarm = 6;
int hour_alarm = 15;
int min_alarm = 15;
int sec_alarm = 35;

int alarm_on = 1;

int current_parameter = 0;

int blinkstate = 0;
