/*
 * fsm_mode.c
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#include "global.h"

void Blink()
{
    switch (mode)
    {
        case ModeInit:
            break;

        default:
            break;
    }
    blinkstate ^= 1;
}


void displayTime()
{
	lcd_ShowIntNum(20, 30, timer, 2, WHITE, BLACK, 16);
}

void fsm_mode()
{
	switch(mode)
	{
		case ModeInit:

			break;

		default:
			break;
	}
	displayTime();
}
