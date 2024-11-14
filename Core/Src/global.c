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

int year = 24;
int month = 11;
int date = 15;
int day = 6;
int hour = 15;
int min = 15;
int sec = 15;

int year_alarm = 0;
int month_alarm = 0;
int date_alarm = 0;
int day_alarm = 0;
int hour_alarm = 0;
int min_alarm = 0;
int sec_alarm = 0;

int alarm_on = 0;

int current_parameter = 0;

int blinkstate = 0;
