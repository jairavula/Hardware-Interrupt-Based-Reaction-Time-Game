/*
 * TimerA.c
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */



#include <HAL/TimerA.h>

volatile static int TimerARollovers;


void TA0_N_IRQHandler() {
  if (Timer_A_getInterruptStatus(TIMER_A0_BASE))
  {
    TimerARollovers++;
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
  }

}
void initTimerA() {

    Timer_A_ContinuousModeConfig myTimerAConfig;

    myTimerAConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    myTimerAConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    myTimerAConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    myTimerAConfig.timerClear = TIMER_A_DO_CLEAR;
    //As part of interrupt init, we clean the old leftover interrupts
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    Timer_A_configureContinuousMode(TIMER_A0_BASE,
                                    &myTimerAConfig);

    Interrupt_enableInterrupt(INT_TA0_N);

    TimerARollovers = 0;
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

}



