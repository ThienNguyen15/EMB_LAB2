/*
 * fsm_mode.h
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#ifndef INC_FSM_MODE_H_
#define INC_FSM_MODE_H_

#include "main.h"
#include "global.h"

void initializeTime();
void updateTime(int new_year, int new_month, int new_date, int new_day, int new_hour, int new_min, int new_sec);
void displayTime();
void displayStatus();

void fsm_mode();

#endif /* INC_FSM_MODE_H_ */
