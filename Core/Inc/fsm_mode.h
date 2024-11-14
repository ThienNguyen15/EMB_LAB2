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

void drawTrafficLight(int verti_Red, int verti_Green, int verti_Yellow, int hori_Red, int hori_Green, int hori_Yellow);

void RedGreenLed();
void RedYellowLed();
void GreenRedLed();
void YellowRedLed();

void ManRedLed();
void ManYellowLed();
void ManGreenLed();

void increase();
void balance(int newtimRed, int newtimGreen, int newtimYellow);

void Blink();

void switchManualMode(int LEDmode, void (*LEDdisplayfunc)());

void displayTime();

void fsm_mode();

#endif /* INC_FSM_MODE_H_ */
