/*
 * button.c
 *
 *  Created on: Apr 6, 2021
 *      Author: leyla
 */

#include "HAL/Button.h"

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "HAL/LED.h"
#include "HAL/Timer.h"
#include "HAL/TimerA.h"

// A boolean variable that is true when a high-to-low transition is sensed on
// JSB For a global variable, the keyword static limits the scope of the
// variable to this function only. This means functions in other files of the
// project cannot access this variable
volatile static bool JSBmodified;
volatile static bool LB1modified;
volatile static bool LB2modified;
volatile static bool BB1modified;
volatile static bool BB2modified;

// 300 ms debouncing wait
#define DEBOUNCE_WAIT 300

// An internal function that initializes a button and enables the high-to-low
// transition
void initButton(uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    // This sets up an input with pull-up resistor. If the input does not need the
    // pull-up resistor, the extra added resistor is harmless.
    GPIO_setAsInputPinWithPullUpResistor(selectedPort, selectedPins);

    // clear interrupt on port selectedPort, pin selectedPins
    // If we fail to this, we "might" get an interrupt as soon we enable interrupt
    GPIO_clearInterruptFlag(selectedPort, selectedPins);

    // enable interrupt on port selectedPort, pin selectedPins
    GPIO_enableInterrupt(selectedPort, selectedPins);

    // the interrupt is triggered on high to low transition (tapping)
    GPIO_interruptEdgeSelect(selectedPort, selectedPins,
    GPIO_HIGH_TO_LOW_TRANSITION);
}

void initButtons()
{
    initButton(GPIO_PORT_P4, GPIO_PIN1);  // JSB

    // enable the port 4 interrupt related to JSB
    Interrupt_enableInterrupt(INT_PORT4);

    // This allows us to start from a clean slate
    JSBmodified = false;

    initButton(GPIO_PORT_P1, GPIO_PIN1);  // LB1Modified

    // enable the port 4 interrupt related to LB1Modified
    Interrupt_enableInterrupt(INT_PORT1);

    // This allows us to start from a clean slate
    LB1modified = false;

    initButton(GPIO_PORT_P1, GPIO_PIN4);  // LB2Modified

    // enable the port 4 interrupt related to LB2Modified
    Interrupt_enableInterrupt(INT_PORT1);

    // This allows us to start from a clean slate
    LB2modified = false;

    initButton(GPIO_PORT_P5, GPIO_PIN1);  // BB1modified

    // enable the port 4 interrupt related to BB1modified
    Interrupt_enableInterrupt(INT_PORT5);

    // This allows us to start from a clean slate
    BB1modified = false;

    initButton(GPIO_PORT_P3, GPIO_PIN5);  // BB2modified

    // enable the port 4 interrupt related to BB2modified
    Interrupt_enableInterrupt(INT_PORT3);

    // This allows us to start from a clean slate
    BB2modified = false;
}

void PORT4_IRQHandler()
{
    // We check to see if the port4 interrupt came from JSB
    if (GPIO_getInterruptStatus(GPIO_PORT_P4, GPIO_PIN1))
    {
        JSBmodified = true;

        // A very critical step: If we don't clear the interrupt, the ISR will be
        // called again and again.
        GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
    }
}

void PORT1_IRQHandler()
{
    if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN1))
    {
        LB1modified = true;

        // A very critical step: If we don't clear the interrupt, the ISR will be
        // called again and again.
        GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    }
    if (GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN4))
       {
           LB2modified = true;

           // A very critical step: If we don't clear the interrupt, the ISR will be
           // called again and again.
           GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
       }
}




void PORT5_IRQHandler()
{
    if (GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN1))
    {
        BB1modified = true;

        // A very critical step: If we don't clear the interrupt, the ISR will be
        // called again and again.
        GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1);
    }
}

void PORT3_IRQHandler()
{
    // We check to see if the port4 interrupt came from JSB
    if (GPIO_getInterruptStatus(GPIO_PORT_P3, GPIO_PIN5))
    {
        BB2modified = true;
        // A very critical step: If we don't clear the interrupt, the ISR will be
        // called again and again.
        GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    }
}



















bool JSBtapped()
{
    // This variable is true if we are in debouncing state (we are ignoring the
    // extra transitions) This variable has to be static so that it "remembers"
    static bool debouncing = false;

    // The SW timer used for debouncing. It has to be static, otherwise it won't
    // work.
    static SWTimer debounceTimer;

    // the single output of the FMS
    bool tapped = false;

    // If we are in debouncing state and the debouncing timer is expired, in other
    // words, if wait time is over, we should leave the debouncing state.
    if (debouncing && SWTimer_expired(&debounceTimer))
        debouncing = false;

    // if we are not in the debouncing state and a transition is detected
    if (!debouncing && JSBmodified)
    {
        // We are not in debouncing and the first transition is detected
        tapped = true;

        // Let's enter debouncing state
        debouncing = true;

        // We should setup a timer for how much to wait
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
    }

    // This is a very critical step similar to clearing interrupt flag.
    // If we don't refresh this variable, next time we enter this function, we
    // think a new transition has happened.
    JSBmodified = false;

    return tapped;
}



bool LB1tapped()
{
    // This variable is true if we are in debouncing state (we are ignoring the
    // extra transitions) This variable has to be static so that it "remembers"
    static bool debouncing = false;

    // The SW timer used for debouncing. It has to be static, otherwise it won't
    // work.
    static SWTimer debounceTimer;

    // the single output of the FMS
    bool tapped = false;

    // If we are in debouncing state and the debouncing timer is expired, in other
    // words, if wait time is over, we should leave the debouncing state.
    if (debouncing && SWTimer_expired(&debounceTimer))
        debouncing = false;

    // if we are not in the debouncing state and a transition is detected
    if (!debouncing && LB1modified)
    {
        // We are not in debouncing and the first transition is detected
        tapped = true;

        // Let's enter debouncing state
        debouncing = true;

        // We should setup a timer for how much to wait
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
    }

    // This is a very critical step similar to clearing interrupt flag.
    // If we don't refresh this variable, next time we enter this function, we
    // think a new transition has happened.
    LB1modified = false;

    return tapped;
}

bool LB2tapped()
{
    // This variable is true if we are in debouncing state (we are ignoring the
    // extra transitions) This variable has to be static so that it "remembers"
    static bool debouncing = false;

    // The SW timer used for debouncing. It has to be static, otherwise it won't
    // work.
    static SWTimer debounceTimer;

    // the single output of the FMS
    bool tapped = false;

    // If we are in debouncing state and the debouncing timer is expired, in other
    // words, if wait time is over, we should leave the debouncing state.
    if (debouncing && SWTimer_expired(&debounceTimer))
        debouncing = false;

    // if we are not in the debouncing state and a transition is detected
    if (!debouncing && LB2modified)
    {
        // We are not in debouncing and the first transition is detected
        tapped = true;

        // Let's enter debouncing state
        debouncing = true;

        // We should setup a timer for how much to wait
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
    }

    // This is a very critical step similar to clearing interrupt flag.
    // If we don't refresh this variable, next time we enter this function, we
    // think a new transition has happened.
    LB2modified = false;

    return tapped;
}

bool BB1tapped()
{
    // This variable is true if we are in debouncing state (we are ignoring the
    // extra transitions) This variable has to be static so that it "remembers"
    static bool debouncing = false;

    // The SW timer used for debouncing. It has to be static, otherwise it won't
    // work.
    static SWTimer debounceTimer;

    // the single output of the FMS
    bool tapped = false;

    // If we are in debouncing state and the debouncing timer is expired, in other
    // words, if wait time is over, we should leave the debouncing state.
    if (debouncing && SWTimer_expired(&debounceTimer))
        debouncing = false;

    // if we are not in the debouncing state and a transition is detected
    if (!debouncing && BB1modified)
    {
        // We are not in debouncing and the first transition is detected
        tapped = true;

        // Let's enter debouncing state
        debouncing = true;

        // We should setup a timer for how much to wait
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
    }

    // This is a very critical step similar to clearing interrupt flag.
    // If we don't refresh this variable, next time we enter this function, we
    // think a new transition has happened.
    BB1modified = false;

    return tapped;
}

bool BB1tappedIRQ() {
    static bool debouncing = false;
    static SWTimer debounceTimer;
    bool tapped = false;

    if (debouncing && SWTimer_expired(&debounceTimer)) {
        debouncing = false;
    }

    if (!debouncing && buttonPressedViaIRQ && BB1modified) { // Check the global flag
        tapped = true;
        debouncing = true;
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
        buttonPressedViaIRQ = false; // Reset the flag
    }

    BB1modified = false; // Assuming you still need to clear this flag as well

    return tapped;
}




bool BB2tapped()
{
    // This variable is true if we are in debouncing state (we are ignoring the
    // extra transitions) This variable has to be static so that it "remembers"
    static bool debouncing = false;

    // The SW timer used for debouncing. It has to be static, otherwise it won't
    // work.
    static SWTimer debounceTimer;

    // the single output of the FMS
    bool tapped = false;

    // If we are in debouncing state and the debouncing timer is expired, in other
    // words, if wait time is over, we should leave the debouncing state.
    if (debouncing && SWTimer_expired(&debounceTimer))
        debouncing = false;

    // if we are not in the debouncing state and a transition is detected
    if (!debouncing && BB2modified)
    {
        // We are not in debouncing and the first transition is detected
        tapped = true;

        // Let's enter debouncing state
        debouncing = true;

        // We should setup a timer for how much to wait
        debounceTimer = SWTimer_construct(DEBOUNCE_WAIT);
        SWTimer_start(&debounceTimer);
    }

    // This is a very critical step similar to clearing interrupt flag.
    // If we don't refresh this variable, next time we enter this function, we
    // think a new transition has happened.
    BB2modified = false;

    return tapped;
}


// This function calls all the functions that check button status and stores
// them in one structure This will allow the user to reliably get the latest
// button status. If we choose not to use this method, the user has to be
// careful to call JSBtapped() or any similar function only once in the main
// loop.
buttons_t updateButtons()
{
    buttons_t buttons;

    buttons.JSBtapped = JSBtapped();
    buttons.LB1tapped = LB1tapped();
    buttons.LB2tapped = LB2tapped();
    buttons.BB1tapped = BB1tapped();
    buttons.BB2tapped = BB2tapped();
    buttons.BB1tappedIRQ = BB1tappedIRQ();

    return (buttons);
}
