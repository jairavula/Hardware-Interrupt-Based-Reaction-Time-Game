/*
 * TimerA.c
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */



#include <HAL/TimerA.h>
#include <HAL/HAL.h>

volatile static int TimerARollovers;
volatile static int TimerA1Rollovers;



void TA0_N_IRQHandler() {
  uint32_t status = Timer_A_getCaptureCompareInterruptStatus(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG);

  if (status & TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG){
      Toggle_BLB();
  }
  Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);

  if (Timer_A_getInterruptStatus(TIMER_A0_BASE))
  {
    TimerARollovers++;
  }

  Timer_A_clearInterruptFlag(TIMER_A0_BASE);


}
void initTimerA() {

    GPIO_setAsInputPin(GPIO_PORT_P2,GPIO_PIN5);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_ContinuousModeConfig myTimerAConfig;

    myTimerAConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    myTimerAConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    myTimerAConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    myTimerAConfig.timerClear = TIMER_A_DO_CLEAR;


    Timer_A_CaptureModeConfig captureConfig;

    captureConfig.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    captureConfig.captureMode = TIMER_A_CAPTUREMODE_FALLING_EDGE;
    captureConfig.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    captureConfig.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    captureConfig.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    captureConfig.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;

    Timer_A_initCapture(TIMER_A0_BASE, &captureConfig);

    //As part of interrupt init, we clean the old leftover interrupts
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    Timer_A_configureContinuousMode(TIMER_A0_BASE,
                                    &myTimerAConfig);

    Interrupt_enableInterrupt(INT_TA0_N);

    TimerARollovers = 0;
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);

}

void TA1_N_IRQHandler() {
  if (Timer_A_getInterruptStatus(TIMER_A1_BASE))
  {
    TimerA1Rollovers++;
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
  }

}

void initTimerA1(){

    Timer_A_ContinuousModeConfig myTimerAConfig;

      myTimerAConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
      myTimerAConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
      myTimerAConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
      myTimerAConfig.timerClear = TIMER_A_DO_CLEAR;
      //As part of interrupt init, we clean the old leftover interrupts
      Timer_A_clearInterruptFlag(TIMER_A1_BASE);
      Timer_A_configureContinuousMode(TIMER_A1_BASE,
                                      &myTimerAConfig);

      Interrupt_enableInterrupt(INT_TA1_N);

      TimerARollovers = 0;
      Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
}



