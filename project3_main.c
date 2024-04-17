// Starter code for Project 3. Good luck!

/* TI includes */
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* Standard includes */
#include <stdio.h>

/* HAL includes */
#include "HAL/Button.h"
#include "HAL/LED.h"
#include "HAL/Timer.h"

void initialize();
void initGraphics(Graphics_Context *g_sContext_p);
void main_loop(Graphics_Context *g_sContext_p);
void sleep();

int main(void) {
  initialize();

  /* Graphics context. Works in the same as it did in the previous projects.
     You may choose to make a HAL struct and store the Graphics context in there
     instead. You may also choose to use the GFX struct from Guess the Color. */
  Graphics_Context g_sContext;
  initGraphics(&g_sContext);

  while (1) {
    sleep();

    main_loop(&g_sContext);
  }
}

void sleep() {
  // The Launchpad Green LED is used to signify the processor is in low-power
  // mode. From the human perspective, it should seem the processor is always
  // asleep except for fractions of second here and there.

  TurnOn_LLG();
  // Enters the Low Power Mode 0 - the processor is asleep and only responds to
  // interrupts
  PCM_gotoLPM0();
  TurnOff_LLG();
}
void main_loop(Graphics_Context *g_sContext_p) {
  // The first step is update the tapping status of all buttons
  buttons_t buttons = updateButtons();

  // Do not remove this statement. This is the non-bocking code for this project
  // Tapping the Joystick push button (JSB) toggles the BoosterPack Green LED
  if (buttons.JSBtapped) Toggle_BLG();

  // TODO: You probably want to define a state variable and an enum associated
  // with it You can then control the state of your application based on the
  // hardware inputs Refer to the Guess-the-color example.
}

// Initialization part is always device dependent and therefore does not change
// much with HAL
void initialize() {
  // Stop watchdog timer
  WDT_A_hold(WDT_A_BASE);
  InitSystemTiming();

  initLEDs();
  initButtons();

  // TODO: Add other initializations here
}

void initGraphics(Graphics_Context *g_sContext_p) {
  // initializing the display
  Crystalfontz128x128_Init();
  Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

  // setting up the graphics
  Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128,
                       &g_sCrystalfontz128x128_funcs);
  Graphics_setFont(g_sContext_p, &g_sFontFixed6x8);

  // set colors
  Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
  Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);

  // apply changes by clearing background
  Graphics_clearDisplay(g_sContext_p);
}
