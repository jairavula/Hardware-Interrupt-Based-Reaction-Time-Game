/*
 * TimerA.c
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */



#include <HAL/TimerA.h>
#include <HAL/HAL.h>
#include <stdlib.h>

volatile static int TimerARollovers=0;
volatile static int CaptureRollovers=0;
volatile static uint_fast16_t CaptureCycles=0;
volatile bool buttonPressedViaIRQ = false;

double time_elapsed(){
    uint64_t cycles = CaptureCycles;
    uint64_t rollovers = CaptureRollovers;
    uint64_t total_cycles = cycles + rollovers*0xFFFF;
    uint64_t frequency = 48000000;
    double total_seconds = total_cycles/(frequency/1.0);
    return total_seconds;
}

bool BB1PressedIRQ(){
    bool BB1pressed = buttonPressedViaIRQ;
    buttonPressedViaIRQ = false;
    return BB1pressed;
}

void TA0_N_IRQHandler() {


    uint32_t status = Timer_A_getCaptureCompareInterruptStatus(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG);
    if (status&TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG) {
        Toggle_BLB();

        CaptureCycles = Timer_A_getCaptureCompareCount(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
        CaptureRollovers=TimerARollovers;
        buttonPressedViaIRQ = true;

    }
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2);
    if (Timer_A_getInterruptStatus(TIMER_A0_BASE)) {
        TimerARollovers++;
    }
    Timer_A_clearInterruptFlag(TIMER_A0_BASE);
}



void initTimerA() {

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsInputPin(GPIO_PORT_P5,GPIO_PIN1);

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
    //Timer_A_clearInterruptFlag(TIMER_A0_BASE);
    Timer_A_configureContinuousMode(TIMER_A0_BASE,
                                    &myTimerAConfig);
    Timer_A_registerInterrupt(TIMER_A0_BASE,
            TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, TA0_N_IRQHandler);
 //   Timer_A_enableCaptureCompareInterrupt(TIMER_A0_BASE,
        //    TIMER_A_CAPTURECOMPARE_REGISTER_2);

    Interrupt_enableInterrupt(INT_TA0_N);
    //Timer_A_enableInterrupt(TIMER_A0_BASE);

    TimerARollovers = 0;
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);




}










//
//
//void TA1_N_IRQHandler() {
//  if (Timer_A_getInterruptStatus(TIMER_A1_BASE))
//  {
//    TimerA1Rollovers++;
//    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
//  }
//
//}
//
//void initTimerA1(){
//
//    Timer_A_ContinuousModeConfig myTimerAConfig;
//
//      myTimerAConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
//      myTimerAConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
//      myTimerAConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
//      myTimerAConfig.timerClear = TIMER_A_DO_CLEAR;
//      //As part of interrupt init, we clean the old leftover interrupts
//      Timer_A_clearInterruptFlag(TIMER_A1_BASE);
//      Timer_A_configureContinuousMode(TIMER_A1_BASE,
//                                      &myTimerAConfig);
//
//      Interrupt_enableInterrupt(INT_TA1_N);
//
//      TimerARollovers = 0;
//      Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);
//}





