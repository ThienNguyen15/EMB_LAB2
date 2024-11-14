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

void displayTime()
{
    int blinkColor1, blinkColor2;

    if (mode == SettingMode || mode == AlarmSettingMode)
    {
        blinkstate ^= 1;
        blinkColor1 = (blinkstate == 1) ? GREEN : BLACK;
        blinkColor2 = (blinkstate == 1) ? YELLOW : BLACK;
    }

    lcd_ShowIntNum(70, 100, hour, 2, (current_parameter == 2) ? blinkColor1 : GREEN, BLACK, 24);
    lcd_ShowIntNum(110, 100, min, 2, (current_parameter == 1) ? blinkColor1 : GREEN, BLACK, 24);
    lcd_ShowIntNum(150, 100, sec, 2, (current_parameter == 0) ? blinkColor1 : GREEN, BLACK, 24);

    lcd_ShowIntNum(20, 130, day, 2, (current_parameter == 3) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(70, 130, date, 2, (current_parameter == 4) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(110, 130, month, 2, (current_parameter == 5) ? blinkColor2 : YELLOW, BLACK, 24);
    lcd_ShowIntNum(150, 130, year, 2, (current_parameter == 6) ? blinkColor2 : YELLOW, BLACK, 24);
}

void displayStatus()
{
    lcd_Fill(0, 0, 240, 20, BLUE);

    switch (mode)
    {
        case NormalMode:
            lcd_StrCenter(0, 2, "Mode: Normal", RED, BLUE, 16, 1);
            break;
        case SettingMode:
            lcd_StrCenter(0, 2, "Mode: Time Setting", RED, BLUE, 16, 1);
            break;
        case AlarmSettingMode:
            lcd_StrCenter(0, 2, "Mode: Alarm Setting", RED, BLUE, 16, 1);
            break;
        default:
            break;
    }
}

void incrementParameter()
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

void adjustParameterWithHold()
{
    static int hold_time = 0;
    if (button_count[4] != 0)
    {
        hold_time++;
        if (hold_time >= 10)
        {
            incrementParameter();
            delay_ms(200);
        }
    }
    else
    {
        hold_time = 0;
        incrementParameter();
    }
}

void fsm_mode()
{
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
                current_parameter = 0;
            }
            displayTime();
            break;

        case SettingMode:
        	displayStatus();
            if(button_count[4] != 0)
            {
                switch(current_parameter)
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

            if(button_count[12] != 0)
            {
                current_parameter++;
                if(current_parameter > 6)
                {
                    current_parameter = 0;
                    updateTime(year, month, date, day, hour, min, sec);
                    mode = NormalMode;
                }
            }

            if(button_count[0] != 0)
            {
            	current_parameter = 0;
            	updateTime(year, month, date, day, hour, min, sec);
            	mode = AlarmSettingMode;
            }
            displayTime();
            break;

        case AlarmSettingMode:
        	displayStatus();
            if(button_count[4] != 0)
            {
                switch(current_parameter)
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

            if(button_count[12] != 0)
            {
            	current_parameter++;
            	if(current_parameter > 6)
            	{
            		current_parameter = 0;
            		updateTime(year, month, date, day, hour, min, sec);
            		mode = NormalMode;
            	}
            }

            if(button_count[0] != 0)
            {
            	current_parameter = 0;
            	updateTime(year, month, date, day, hour, min, sec);
            	mode = NormalMode;
            }
            displayTime();
            break;

        default:
            break;
    }
}
