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

    lcd_ShowPicture(80, 200, 90, 90, gImage_l_flag);
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
	timer_temp++;
	if(timer_temp > 99)
		timer_temp = 1;
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
	counter = 0;
	mode = LEDmode;
	blinkstate = 0;
	LEDdisplayfunc();
}

void displayTime()
{
	if(mode == ManRed || mode == ManGreen || mode == ManYellow)
		lcd_ShowIntNum(20, 30, timer_temp, 2, WHITE, BLACK, 16);
	else
		lcd_ShowIntNum(20, 30, timer, 2, WHITE, BLACK, 16);
}

void fsm_mode()
{
	switch(mode)
	{
		case ModeInit:
			counter = 0;
			timer = timeRed;
			mode = RedGreen;
			RedGreenLed();
			break;
		case RedGreen:
			if(counter >= 10)
			{
				counter = 0;
				if(check_timer == 0)
					check_timer++;
				else if(check_timer == 1)
				{
					check_timer = 0;
					timer--;
				}
				if(timer <= timeYellow)
				{
					mode = RedYellow;
					RedYellowLed();
				}
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManRed, ManRedLed);
				timer_temp = timeRed;
			}
			break;
		case RedYellow:
			if(counter >= 10)
			{
				counter = 0;
				if(check_timer == 0)
					check_timer++;
				else if(check_timer == 1)
				{
					check_timer = 0;
					timer--;
				}
				if(timer <= 0)
				{
					mode = GreenRed;
					timer = timeGreen;
					GreenRedLed();
				}
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManRed, ManRedLed);
				timer_temp = timeRed;
			}
			break;
		case GreenRed:
			if(counter >= 10)
			{
				counter = 0;
				if(check_timer == 0)
					check_timer++;
				else if(check_timer == 1)
				{
					check_timer = 0;
					timer--;
				}
				if(timer <= 0)
				{
					mode = YellowRed;
					timer = timeYellow;
					YellowRedLed();
				}
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManRed, ManRedLed);
				timer_temp = timeRed;
			}
			break;
		case YellowRed:
			if(counter >= 10)
			{
				counter = 0;
				if(check_timer == 0)
					check_timer++;
				else if(check_timer == 1)
				{
					check_timer = 0;
					timer--;
				}
				if(timer <= 0)
				{
					mode = RedGreen;
					timer = timeRed;
					RedGreenLed();
				}
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManRed, ManRedLed);
				timer_temp = timeRed;
			}
			break;
		case ManRed:
			if(counter >= 10)
			{
				counter = 0;
				Blink();
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManGreen, ManGreenLed);
				timer_temp = timeGreen;
			}
			else if(button_count[1] >= 3)
				increase();
			else if(button_count[2] >= 3)
				balance(timer_temp, timeGreen, timeYellow);
			break;
		case ManGreen:
			if(counter >= 10)
			{
				counter = 0;
				Blink();
			}
			else if(button_count[0] >= 3)
			{
				switchManualMode(ManYellow, ManYellowLed);
				timer_temp = timeYellow;
			}
			else if(button_count[1] >= 3)
				increase();
			else if(button_count[2] >= 3)
				balance(timeRed, timer_temp, timeYellow);
			break;
		case ManYellow:
			if(counter >= 10)
			{
				counter = 0;
				Blink();
			}
			else if(button_count[0] >= 3)
			{
				counter = 0;
				mode = RedGreen;
				timer = timeRed;
				RedGreenLed();
			}
			else if(button_count[1] >= 3)
				increase();
			else if(button_count[2] >= 3)
				balance(timeRed, timeGreen, timer_temp);
			break;

		default:
			break;
	}
	displayTime();
}
