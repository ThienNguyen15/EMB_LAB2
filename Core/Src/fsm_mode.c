/*
 * fsm_mode.c
 *
 *  Created on: Oct 21, 2023
 *      Author: thien
 */

#include "global.h"

void initializeTime()
{
	ds3231_Write(ADDRESS_YEAR, 24);
	ds3231_Write(ADDRESS_MONTH, 11);
	ds3231_Write(ADDRESS_DATE, 15);
	ds3231_Write(ADDRESS_DAY, 6);
	ds3231_Write(ADDRESS_HOUR, 15);
	ds3231_Write(ADDRESS_MIN, 15);
	ds3231_Write(ADDRESS_SEC, 15);
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

void updateAlarmTime(int new_year, int new_month, int new_date, int new_day, int new_hour, int new_min, int new_sec)
{
    year_alarm = new_year;
    month_alarm = new_month;
    date_alarm = new_date;
    day_alarm = new_day;
    hour_alarm = new_hour;
    min_alarm = new_min;
    sec_alarm = new_sec;
}
void displayTime()
{
    int blinkColor1, blinkColor2;

    if ((mode == SettingMode || mode == AlarmSettingMode) && (timer1 % 10) == 0)
    {
        blinkstate ^= 1;
        blinkColor1 = (blinkstate == 1) ? GREEN : BLACK;
        blinkColor2 = (blinkstate == 1) ? YELLOW : BLACK;

        lcd_ShowIntNum(70, 130, hour_temp, 2, (current_parameter == 2) ? blinkColor1 : GREEN, BLACK, 24);
        lcd_ShowIntNum(110, 130, min_temp, 2, (current_parameter == 1) ? blinkColor1 : GREEN, BLACK, 24);
        lcd_ShowIntNum(150, 130, sec_temp, 2, (current_parameter == 0) ? blinkColor1 : GREEN, BLACK, 24);

        lcd_ShowIntNum(20, 100, day_temp, 2, (current_parameter == 3) ? blinkColor2 : YELLOW, BLACK, 24);
        lcd_ShowIntNum(70, 100, date_temp, 2, (current_parameter == 4) ? blinkColor2 : YELLOW, BLACK, 24);
        lcd_ShowIntNum(110, 100, month_temp, 2, (current_parameter == 5) ? blinkColor2 : YELLOW, BLACK, 24);
        lcd_ShowIntNum(150, 100, year_temp, 2, (current_parameter == 6) ? blinkColor2 : YELLOW, BLACK, 24);
    }
    else
    {
    	lcd_ShowIntNum(70, 130, ds3231_hour, 2, GREEN, BLACK, 24);
    	lcd_ShowIntNum(110, 130, ds3231_min, 2, GREEN, BLACK, 24);
    	lcd_ShowIntNum(150, 130, ds3231_sec, 2, GREEN, BLACK, 24);

    	lcd_ShowIntNum(20, 100, ds3231_day, 2, YELLOW, BLACK, 24);
    	lcd_ShowIntNum(70, 100, ds3231_date, 2, YELLOW, BLACK, 24);
    	lcd_ShowIntNum(110, 100, ds3231_month, 2, YELLOW, BLACK, 24);
    	lcd_ShowIntNum(150, 100, ds3231_year, 2, YELLOW, BLACK, 24);
    }
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

int checkAlarmTime()
{
	return (ds3231_hour == hour_alarm && ds3231_min == min_alarm && ds3231_sec == sec_alarm
			&& ds3231_year == year_alarm && ds3231_month == month_alarm && ds3231_date == date_alarm
			&& ds3231_day == day_alarm) ? 1 : 0;
}

void warnAlarmTime(){lcd_ShowStr(70, 160, (unsigned char *)"ALARM!", WHITE, RED, 16, 0);}

void increment()
{
	if(alarm_on != 0)
	{
		switch (current_parameter)
		{
		case 0: ds3231_sec = (ds3231_sec + 1) % 60; 		break;
		case 1: ds3231_min = (ds3231_min + 1) % 60; 		break;
		case 2: ds3231_hour = (ds3231_hour + 1) % 24; 		break;
		case 3: ds3231_day = (ds3231_day % 7) + 1; 		break;
		case 4: ds3231_date = (ds3231_date % 31) + 1; 		break;
		case 5: ds3231_month = (ds3231_month % 12) + 1; 	break;
		case 6: ds3231_year = (ds3231_year % 100) + 1; 	break;
		}
	}
	else
	{
		switch (current_parameter)
		{
		case 0: sec_alarm = (sec_alarm + 1) % 60; 		break;
		case 1: min_alarm = (min_alarm + 1) % 60; 		break;
		case 2: hour_alarm = (hour_alarm + 1) % 24; 	break;
		case 3: day_alarm = (day_alarm % 7) + 1; 		break;
		case 4: date_alarm = (date_alarm % 31) + 1; 	break;
		case 5: month_alarm = (month_alarm % 12) + 1; 	break;
		case 6: year_alarm = (year_alarm % 100) + 1; 	break;
		}
	}
}

void incrementParameter()
{
	if (button_count[4] != 0)
	{
		if(timer2 == 0 && is_pressed == 0)
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
	ds3231_ReadTime();
	lcd_Clear(WHITE);
	if(checkAlarmTime() && alarm_on == 1)
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
                timer1 = 0;
                current_parameter = 0;
                hour_temp = ds3231_hour;
                min_temp = ds3231_min;
                sec_temp = ds3231_sec;
                day_temp = ds3231_day;
                date_temp = ds3231_date;
                month_temp = ds3231_month;
                year_temp = ds3231_year;
                mode = SettingMode;
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
                    updateTime(year_temp, month_temp, date_temp, day_temp, hour_temp, min_temp, sec_temp);
                    mode = NormalMode;
                }
                updateTime(year_temp, month_temp, date_temp, day_temp, hour_temp, min_temp, sec_temp);
            }

            if(button_count[0] != 0)
            {
            	timer1 = 0;
            	current_parameter = 0;
            	hour_temp = ds3231_hour;
            	min_temp = ds3231_min;
            	sec_temp = ds3231_sec;
            	day_temp = ds3231_day;
            	date_temp = ds3231_date;
            	month_temp = ds3231_month;
            	year_temp = ds3231_year;
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
            		updateAlarmTime(year_temp, month_temp, date_temp, day_temp, hour_temp, min_temp, sec_temp);
            		alarm_on = 1;
            		mode = NormalMode;
            	}
            	updateAlarmTime(year_temp, month_temp, date_temp, day_temp, hour_temp, min_temp, sec_temp);
            }

            if(button_count[0] != 0)
            {
            	timer1 = 0;
            	current_parameter = 0;
            	mode = NormalMode;
            }
            displayTime();
            break;

        default:
            break;
    }
}
