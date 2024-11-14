/*
 * fsm_mode.c
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#include "global.h"

void initializeTime()
{
	ds3231_Write(ADDRESS_YEAR, year);
	ds3231_Write(ADDRESS_MONTH, month);
	ds3231_Write(ADDRESS_DATE, date);
	ds3231_Write(ADDRESS_DAY, day);
	ds3231_Write(ADDRESS_HOUR, hour);
	ds3231_Write(ADDRESS_MIN, min);
	ds3231_Write(ADDRESS_SEC, sec);
}

void updateTime(int new_year, int new_month, int new_date, int new_day, int new_hour, int new_min, int new_sec)
{
	ds3231_Write(ADDRESS_YEAR, new_year);
	ds3231_Write(ADDRESS_MONTH, new_month);
	ds3231_Write(ADDRESS_DATE, new_date);
	ds3231_Write(ADDRESS_DAY, new_day);
	ds3231_Write(ADDRESS_HOUR, new_hour);
	ds3231_Write(ADDRESS_MIN, new_min);
	ds3231_Write(ADDRESS_SEC, new_sec);
}

void updateAlarmTime(int new_year_alarm, int new_month_alarm, int new_date_alarm, int new_day_alarm, int new_hour_alarm, int new_min_alarm, int new_sec_alarm)
{
    year_alarm = new_year_alarm;
    month_alarm = new_month_alarm;
    date_alarm = new_date_alarm;
    day_alarm = new_day_alarm;
    hour_alarm = new_hour_alarm;
    min_alarm = new_min_alarm;
    sec_alarm = new_sec_alarm;
}
void displayTime()
{
    int blinkColor1, blinkColor2;

    if ((mode == SettingMode || mode == AlarmSettingMode) && (timer1 % 10) == 0)
    {
        blinkstate ^= 1;
        blinkColor1 = (blinkstate == 1) ? GREEN : BLACK;
        blinkColor2 = (blinkstate == 1) ? YELLOW : BLACK;
    }

    lcd_ShowIntNum(70, 130, hour, 2, (current_parameter == 2) ? blinkColor1 : GREEN, BLACK, 24);
    lcd_ShowIntNum(110, 130, min, 2, (current_parameter == 1) ? blinkColor1 : GREEN, BLACK, 24);
    lcd_ShowIntNum(150, 130, sec, 2, (current_parameter == 0) ? blinkColor1 : GREEN, BLACK, 24);

    lcd_ShowIntNum(20, 100, day, 2, (current_parameter == 3) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(70, 100, date, 2, (current_parameter == 4) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(110, 100, month, 2, (current_parameter == 5) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(150, 100, year, 2, (current_parameter == 6) ? blinkColor2 : YELLOW, BLACK, 24);
}

void displayStatus()
{
    lcd_Fill(0, 0, 240, 20, BLUE);

    switch (mode)
    {
        case NormalMode:
            lcd_StrCenter(0, 2, (unsigned char *)"Mode: Normal", RED, BLUE, 16, 1);
            break;
        case SettingMode:
            lcd_StrCenter(0, 2, (unsigned char *)"Mode: Time Setting", RED, BLUE, 16, 1);
            break;
        case AlarmSettingMode:
            lcd_StrCenter(0, 2, (unsigned char *)"Mode: Alarm Setting", RED, BLUE, 16, 1);
            break;
        default:
            break;
    }
}

int checkAlarmTime()	{	return (hour == hour_alarm && min == min_alarm && sec == sec_alarm) ? 1 : 0;	}

void warnAlarmTime()	{	lcd_ShowString(70, 160, (unsigned char *)"ALARM!");	}

void increment()
{
	if(alarm_on != 0)
	{
		switch (current_parameter)
		{
		case 0: sec = (sec + 1) % 60; break;
		case 1: min = (min + 1) % 60; break;
		case 2: hour = (hour + 1) % 24; break;
		case 3: day = (day % 7) + 1; break;
		case 4: date = (date % 31) + 1; break;
		case 5: month = (month % 12) + 1; break;
		case 6: year = (year % 100) + 1; break;
		}
	}
	else
	{
		switch (current_parameter)
		{
		case 0: sec_alarm = (sec_alarm + 1) % 60; break;
		case 1: min_alarm = (min_alarm + 1) % 60; break;
		case 2: hour_alarm = (hour_alarm + 1) % 24; break;
		case 3: day_alarm = (day_alarm % 7) + 1; break;
		case 4: date_alarm = (date_alarm % 31) + 1; break;
		case 5: month_alarm = (month_alarm % 12) + 1; break;
		case 6: year_alarm = (year_alarm % 100) + 1; break;
		}
	}
}

void incrementParameter()
{
	if (button_count[4] != 0)
	{
		if(timer2 == 0)
			increment();
		if(is_pressed == 0)
		{
			timer2++;
			if(timer2 >= 40)
			{
				timer1 = 0;
				timer2 = 0;
				is_pressed = 1;
			}
		}
		if(is_pressed == 1)
		{
			if((timer1 % 4) == 0)
				increment();
		}
	}
}

void fsm_mode()
{

	if(checkAlarmTime())
		warnAlarmTime();

    switch (mode)
    {
        case ModeInit:
            initializeTime();
            mode = NormalMode;
            break;

        case NormalMode:
        	displayStatus();
            if(button_count[0] != 0)
            {
                mode = SettingMode;
                timer1 = 0;
                current_parameter = 0;
            }
            displayTime();
            break;

        case SettingMode:
        	displayStatus();
            if(button_count[4] != 0)
            	incrementParameter();
            else
            	timer2 = 0;

            if(button_count[12] != 0)
            {
                current_parameter++;
                if(current_parameter > 6)
                {
                	timer1 = 0;
                    current_parameter = 0;
                    updateTime(year, month, date, day, hour, min, sec);
                    mode = NormalMode;
                }
            }

            if(button_count[0] != 0)
            {
            	timer1 = 0;
            	current_parameter = 0;
            	updateTime(year, month, date, day, hour, min, sec);
            	alarm_on = 1;
            	mode = AlarmSettingMode;
            }
            displayTime();
            break;

        case AlarmSettingMode:
        	displayStatus();
            if(button_count[4] != 0)
            	incrementParameter();
            else
            	timer2 = 0;

            if(button_count[12] != 0)
            {
            	current_parameter++;
            	if(current_parameter > 6)
            	{
            		timer1 = 0;
            		current_parameter = 0;
            		updateTime(year, month, date, day, hour, min, sec);
            		mode = NormalMode;
            	}
            }

            if(button_count[0] != 0)
            {
            	timer1 = 0;
            	current_parameter = 0;
            	updateAlarmTime(year, month, date, day, hour, min, sec);
            	alarm_on = 0;
            	mode = NormalMode;
            }
            displayTime();
            break;

        default:
            break;
    }
}
