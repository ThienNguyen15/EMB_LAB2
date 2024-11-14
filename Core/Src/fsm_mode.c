/*
 * fsm_mode.c
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#include "global.h"

void drawTrafficLight(int verti_Red, int verti_Green, int verti_Yellow, int hori_Red, int hori_Green, int hori_Yellow)
{
    lcd_Clear(WHITE);

    lcd_DrawCircle(120, 100, RED, 15, verti_Red);
    lcd_DrawCircle(50, 100, GREEN, 15, verti_Green);
    lcd_DrawCircle(190, 100, YELLOW, 15, verti_Yellow);

    lcd_DrawCircle(50, 210, RED, 15, hori_Red);
    lcd_DrawCircle(50, 140, GREEN, 15, hori_Green);
    lcd_DrawCircle(50, 280, YELLOW, 15, hori_Yellow);
}

void RedGreenLed()	{	drawTrafficLight(1, 0, 0, 0, 1, 0);	}

void RedYellowLed()	{	drawTrafficLight(1, 0, 0, 0, 0, 1);	}

void GreenRedLed()	{	drawTrafficLight(0, 1, 0, 1, 0, 0);	}

void YellowRedLed()	{	drawTrafficLight(0, 0, 1, 1, 0, 0);	}

void ManRedLed()	{	drawTrafficLight(1, 0, 0, 1, 0, 0);	}

void ManGreenLed()	{	drawTrafficLight(0, 1, 0, 0, 1, 0);	}

void ManYellowLed(){drawTrafficLight(0, 0, 1, 0, 0, 1);}

void increase()
{
	timer++;
	if(timer > 99)
		timer = 1;
}

void balance(int newtimeRed, int newtimeGreen, int newtimeYellow)
{
	if(newtimeRed > timeRed)
	{
		timeYellow	= round( ( (float)(timeYellow * 1.0f) * (float)( (newtimeRed * 1.0f) / (timeRed * 1.0f) ) ) );
		timeGreen	= round( ( (float)(timeGreen * 1.0f) * (float)( (newtimeRed * 1.0f) / (timeRed * 1.0f) ) ) );
		timeRed		= newtimeRed;
	}
	else if(newtimeGreen > timeGreen)
	{
		timeRed		= round( ( (float)(timeRed * 1.0f) * (float)( (newtimeGreen * 1.0f) / (timeGreen * 1.0f) ) ) );
		timeYellow	= round( ( (float)(timeYellow * 1.0f) * (float)( (newtimeGreen * 1.0f) / (timeGreen * 1.0f) ) ) );
		timeGreen	= newtimeGreen;
	}
	else if(newtimeYellow > timeYellow)
	{
		timeRed		= round( ( (float)(timeRed * 1.0f) * (float)( (newtimeYellow * 1.0f) / (timeYellow * 1.0f) ) ) ) ;
		timeGreen	= round( ( (float)(timeGreen * 1.0f) * (float)( (newtimeYellow * 1.0f) / (timeYellow * 1.0f) ) ) );
		timeYellow	= newtimeYellow;
	}
	else if(newtimeRed < timeRed)
	{
		timeYellow	= round( (float)(newtimeRed * 1.0f) / (5 * 1.0f) ) * (float)(2 * 1.0f);
		timeGreen	= round( (float)(newtimeRed * 1.0f) / (5 * 1.0f) ) * (float)(3 * 1.0f);
		timeRed		= newtimeRed;
	}
	else if(newtimeGreen < timeGreen)
	{
		timeRed		= round( (float)(newtimeGreen * 1.0f) / (3 * 1.0f) ) * (float)(5 * 1.0f);
		timeYellow	= round( (float)(newtimeGreen * 1.0f) / (3 * 1.0f) ) * (float)(2 * 1.0f);
		timeGreen	= newtimeGreen;
	}
	else if(newtimeYellow < timeYellow)
	{
		timeRed		= round( (float)(newtimeYellow * 1.0f) / (2 * 1.0f) ) * (float)(5 * 1.0f);
		timeGreen	= round( (float)(newtimeYellow * 1.0f) / (2 * 1.0f) ) * (float)(3 * 1.0f);
		timeYellow	= newtimeYellow;
	}
}

void Blink()
{
    switch (mode)
    {
        case ManRed:
            drawTrafficLight(blinkstate, 0, 0, blinkstate, 0, 0);
            break;

        case ManGreen:
            drawTrafficLight(0, blinkstate, 0, 0, blinkstate, 0);
            break;

        case ManYellow:
            drawTrafficLight(0, 0, blinkstate, 0, 0, blinkstate);
            break;

        default:
            break;
    }
    blinkstate ^= 1;
}

void switchManualMode(int LEDmode, void (*LEDdisplayfunc)())
{
	setTimer1(500);
	mode = LEDmode;
	blinkstate = 0;
	LEDdisplayfunc();
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
			setTimer1(500);
			timer = timeRed;
			mode = RedGreen;
			RedGreenLed();
			break;
		case RedGreen:
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				timer--;
				if(timer <= timeYellow)
				{
					mode = RedYellow;
					RedYellowLed();
				}
			}
			if(button_count[0] != 0)
				switchManualMode(ManRed, ManRedLed);
			break;
		case RedYellow:
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				timer--;
				if(timer <= 0)
				{
					mode = GreenRed;
					GreenRedLed();
				}
			}
			if(button_count[0] != 0)
				switchManualMode(ManRed, ManRedLed);
			break;
		case GreenRed:
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				timer--;
				if(timer <= timeYellow)
				{
					mode = YellowRed;
					YellowRedLed();
				}
			}
			if(button_count[0] != 0)
				switchManualMode(ManRed, ManRedLed);
			break;
		case YellowRed:
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				timer--;
				if(timer <= 0)
				{
					mode = RedGreen;
					timer = timeRed;
					RedGreenLed();
				}
			}
			if(button_count[0] != 0)
				switchManualMode(ManRed, ManRedLed);
			break;
		case ManRed:
			timer = timeRed;
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				Blink();
			}
			if(button_count[0] != 0)
				switchManualMode(ManGreen, ManGreenLed);
			if(button_count[1] != 0)
				increase();
			if(button_count[2] != 0)
				balance(timer, timeGreen, timeYellow);
			break;
		case ManGreen:
			timer = timeGreen;
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				Blink();
			}
			if(button_count[0] != 0)
				switchManualMode(ManYellow, ManYellowLed);
			if(button_count[1] != 0)
				increase();
			if(button_count[2] != 0)
				balance(timeRed, timer, timeYellow);
			break;
		case ManYellow:
			timer = timeYellow;
			if(flag_timer1 == 1)
			{
				setTimer1(500);
				Blink();
			}
			if(button_count[0] != 0)
			{
				setTimer1(500);
				mode = RedGreen;
				timer = timeRed;
				RedGreenLed();
			}
			if(button_count[1] != 0)
				increase();
			if(button_count[2] != 0)
				balance(timeRed, timeGreen, timer);
			break;

		default:
			break;
	}
	displayTime();
}
