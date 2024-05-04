/*
 * TimerA.h
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */

#ifndef HAL_TIMERA_H_
#define HAL_TIMERA_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
void initTimerA();

double time_elapsed();
bool BB1PressedIRQ();
extern volatile bool buttonPressedViaIRQ;


#endif /* HAL_TIMERA_H_ */

