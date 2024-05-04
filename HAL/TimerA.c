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
volatile static int CaptureRollovers=0; // Used in time_elapsed calculation
volatile static uint_fast16_t CaptureCycles=0; // Used in time_elapsed calculation
volatile bool buttonPressedViaIRQ = false; // Used to see is BB1 is pressed through the TimerAIRQ handler

double time_elapsed(){
    uint64_t cycles = CaptureCycles;
    uint64_t rollovers = CaptureRollovers;
    uint64_t total_cycles = cycles + rollovers*0xFFFF; // calculate the total cycles elapsed
    uint64_t frequency = 48000000;
    double total_seconds = total_cycles/(frequency/1.0); // find time frame with cycles/ frequency
    return total_seconds;
}

bool BB1PressedIRQ(){ // checks if button is pressed via TA)_N_IRQHandler(), the resets the flag to false
    bool BB1pressed = buttonPressedViaIRQ;
    buttonPressedViaIRQ = false;
    return BB1pressed;
}

void TA0_N_IRQHandler() {


    uint32_t status = Timer_A_getCaptureCompareInterruptStatus(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG);
    if (status&TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG) { // If there is a true interrupt
        CaptureCycles = Timer_A_getCaptureCompareCount(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2); // log count
        CaptureRollovers=TimerARollovers; // used in time calculation
        buttonPressedViaIRQ = true; // used in button pressed checking

    }
    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2); // clear the interrupt
    if (Timer_A_getInterruptStatus(TIMER_A0_BASE)) { // if there is an interrupt due to rollover
        TimerARollovers++; // increment number of rollovers
    }
    Timer_A_clearInterruptFlag(TIMER_A0_BASE); // clear the flag
}



void initTimerA() {

    //Boilerplate intialization code
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2,GPIO_PIN5,GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsInputPin(GPIO_PORT_P5,GPIO_PIN1);
 // Set timer A to continous mode
    Timer_A_ContinuousModeConfig myTimerAConfig;

    myTimerAConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    myTimerAConfig.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    myTimerAConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_ENABLE;
    myTimerAConfig.timerClear = TIMER_A_DO_CLEAR;

 // Set timer A to capture mode
    Timer_A_CaptureModeConfig captureConfig;

    captureConfig.captureRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2;
    captureConfig.captureMode = TIMER_A_CAPTUREMODE_FALLING_EDGE;
    captureConfig.captureInputSelect = TIMER_A_CAPTURE_INPUTSELECT_CCIxA;
    captureConfig.synchronizeCaptureSource = TIMER_A_CAPTURE_SYNCHRONOUS;
    captureConfig.captureInterruptEnable = TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    captureConfig.captureOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;

    Timer_A_initCapture(TIMER_A0_BASE, &captureConfig); // initialize the capture


    Timer_A_configureContinuousMode(TIMER_A0_BASE,
                                    &myTimerAConfig); // configure continuous mode
    Timer_A_registerInterrupt(TIMER_A0_BASE,
            TIMER_A_CCRX_AND_OVERFLOW_INTERRUPT, TA0_N_IRQHandler); // initialize which register to interrupt from

    Interrupt_enableInterrupt(INT_TA0_N); // Enable the behavior


    TimerARollovers = 0; // set the rollovers to 0
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE); // start the counter




}






