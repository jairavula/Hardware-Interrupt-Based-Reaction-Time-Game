// Starter code for Project 3. Good luck!

/* TI includes */
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <Application.h>

/* HAL includes */
#include <HAL/HAL.h>
#include"HAL/TimerA.h"
#include "HAL/Button.h"
#include "HAL/LED.h"
#include "HAL/Timer.h"

#define TITLE_SCREEN_WAIT 2000 // wait 2 seconds to switch from title screen to home screen

void initialize();
void initGraphics(Graphics_Context *g_sContext_p);
void main_loop(HAL *hal_p, Application *app);
Application Application_construct();
void Title_screen(HAL *hal_p, Application *app_p);
void Settings_screen(HAL *hal_p, Application *app);
void Screen_manager(Application *app_p, HAL *hal_p, buttons_t *buttons);
void handle_Settings_Screen(HAL *hal_p, Application *app, buttons_t *buttons);
void Game_screen(HAL *hal_p, Application *app);
void handle_Game_Screen(HAL *hal_p, Application *app, buttons_t *buttons);
void StartRandomTimer();
void NextTrialTimer();
void Results_screen(HAL *hal_p, Application * app);
void handle_Results_screen(HAL *hal_p, Application *app, buttons_t *buttons);
void sleep();

int main(void)
{
    initialize();

    /* Graphics context. Works in the same as it did in the previous projects.
     You may choose to make a HAL struct and store the Graphics context in there
     instead. You may also choose to use the GFX struct from Guess the Color. */

    Application app = Application_construct();
    HAL hal_p = HAL_construct();
    // Graphics_Context g_sContext;
    // initGraphics(&g_sContext);'
    Title_screen(&hal_p, &app);

    while (1)
    {
        sleep();

        main_loop(&hal_p, &app);
    }
}

void sleep()
{
    // The Launchpad Green LED is used to signify the processor is in low-power
    // mode. From the human perspective, it should seem the processor is always
    // asleep except for fractions of second here and there.

    TurnOn_LLG();
    // Enters the Low Power Mode 0 - the processor is asleep and only responds to
    // interrupts
    PCM_gotoLPM0();
    TurnOff_LLG();
}
void main_loop(HAL *hal_p, Application *app)
{
    // The first step is update the tapping status of all buttons
    buttons_t buttons = updateButtons();

    // Do not remove this statement. This is the non-bocking code for this project
    // Tapping the Joystick push button (JSB) toggles the BoosterPack Green LED
    if (buttons.JSBtapped)
        Toggle_BLG();
    Screen_manager(app, hal_p, &buttons);

    // TODO: You probably want to define a state variable and an enum associated
    // with it You can then control the state of your application based on the
    // hardware inputs Refer to the Guess-the-color example.
}

// Initialization part is always device dependent and therefore does not change
// much with HAL
void initialize()
{
    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    InitSystemTiming();

    initLEDs();
    initButtons();
    InitTimer32Instance1();

    // TODO: Add other initializations here
}

void initGraphics(Graphics_Context *g_sContext_p)
{
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

Application Application_construct()
{
    Application app;

    app.screenState = titleScreen;


//    app.titleScreenTimer = SWTimer_construct(TITLE_SCREEN_WAIT);
//    SWTimer_start(&app.titleScreenTimer);
    return app;
}

void Screen_manager(Application *app_p, HAL *hal_p, buttons_t *buttons)
{
    switch (app_p->screenState)
    {
    case titleScreen:
        if (HWTimerExpired())
        {
            app_p->screenState = configScreen;
            Graphics_clearDisplay(&hal_p->g_sContext);
            Settings_screen(hal_p, app_p);
            handle_Settings_Screen(hal_p, app_p, buttons);
        }
        break;
    case configScreen:
        handle_Settings_Screen(hal_p, app_p, buttons);
        break;

    case gameScreen:
        handle_Game_Screen(hal_p, app_p, buttons);
        break;
    case resultsScreen:
        handle_Results_screen(hal_p, app_p, buttons);
        break;
    }

}

void Title_screen(HAL *hal_p, Application *app_p)
{
    // only graphics on this page
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Spring 2024 Project", -1,
                        5, 5, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Reflex Measurement", -1,
                        5, 13, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Jai Ravula", -1, 5, 21,
    true);
}

void Settings_screen(HAL *hal_p, Application *app)
{

    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Trial Config", -1, 5, 5,
    true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "# of Trials:  ", -1, 5,
                        20, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Trial 1:       sec ", -1,
                        5, 40, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Trial 2:       sec ", -1,
                        5, 48, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Trial 3:       sec ", -1,
                        5, 56, true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Tap BB2 to begin. ", -1,
                        5, 115, true);
}

void handle_Settings_Screen(HAL *hal_p, Application *app, buttons_t *buttons)
{
    static int trials = 1;
    char trialsStr[4];
    sprintf(trialsStr, "%d", trials);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) trialsStr, -1, 90, 20,
    true);

    if (buttons->LB1tapped && trials > 1)
    {
        trials--;
        sprintf(trialsStr, "%d", trials);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "  ", -1, 90, 20,
        true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) trialsStr, -1, 90, 20,
        true);
        app->numTrials--;
    }
    if (buttons->LB2tapped && trials < 3)
    {
        trials++;
        sprintf(trialsStr, "%d", trials);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "  ", -1, 90, 20,
        true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) trialsStr, -1, 90, 20,
        true);
    }
    if (buttons->BB2tapped)
    {
        Game_screen(hal_p, app);
        app->numTrials = trials;
        app->screenState = gameScreen;
      //StartRandomTimer();
    }
}

void Game_screen(HAL *hal_p, Application *app)
{
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "             ", -1, 5, 5,
                        true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Testing", -1, 5, 5,
    true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "React to LED w/ BB1. ",
                        -1, 5, 115,
                        true);

    app->reactionTimes[0] = 0.0;
      app->reactionTimes[1] = 0.0;
      app->reactionTimes[2] = 0.0;
}

void handle_Game_Screen(HAL *hal_p, Application *app, buttons_t *buttons)
{
    static bool timerStarted = false;
    static bool timerAStarted = true;
    static int trialTimeYCoordinate = 40;
    static int i;


    if (app->numTrials > 0)
    {
        if (!timerStarted)
        {
            HWTimerExpired();
            TurnOff_LL1();  // Ensure the LED is off when starting the timer
            StartRandomTimer();  // Start the timer with a random delay
            timerStarted = true;
            timerAStarted = false;
        }

        // Check if the timer has expired
        if (!timerAStarted && timerStarted && HWTimerExpired())
        {
            TurnOn_LL1();  // Turn on the LED only if the timer has expired
            initTimerA();
            timerAStarted = true;
            //        timerStarted = false;  // Reset the flag to allow re-starting the timer next time
        }
        if (timerStarted && timerAStarted && buttons->BB1tapped
                && !buttons->BB2tapped)
        {
            double timeElapsed = time_elapsed();
            app->reactionTimes[i] = timeElapsed;

            char valueStr[16]; // Increased buffer size to handle larger floating point numbers
            sprintf(valueStr, "%.3f", timeElapsed); // Formatting the double to a string with 3 decimal places

            Graphics_drawString(&hal_p->g_sContext, (int8_t*) valueStr, -1, 55,
                                trialTimeYCoordinate, true);
            timerStarted = false;
            timerAStarted = true;
            app->numTrials--;
            i++;
            trialTimeYCoordinate += 8;
            NextTrialTimer();
        }
    }
    else
    {
        trialTimeYCoordinate = 40;
                          timerStarted = false;
                          timerAStarted = true;

        TurnOff_LL1();
        double mean = (app->reactionTimes[0] + app->reactionTimes[1] + app->reactionTimes[2]) / (i);
        char meanStr[16];
        i = 0;
        sprintf(meanStr, "Mean: %.3f", mean); // Formatting the double to a string with 3 decimal places
                   Graphics_drawString(&hal_p->g_sContext, (int8_t*) meanStr, -1, 35,
                                       80, true);
                   app->screenState = resultsScreen;
                   Results_screen(hal_p, app);
                   NextTrialTimer();
                   HWTimerExpired();


    }

}

void Results_screen(HAL *hal_p, Application * app){
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "             ", -1, 5, 5,
                            true);
    Graphics_drawString(&hal_p->g_sContext, (int8_t*) "                   ",
                                    -1, 5, 115,
                                    true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Results", -1, 5, 5,
        true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Return to Settings",
                            -1, 5, 105,
                            true);
        Graphics_drawString(&hal_p->g_sContext, (int8_t*) "Press BB2",
                                   -1, 5, 115,
                                   true);
}

void handle_Results_screen(HAL *hal_p, Application *app, buttons_t *buttons){

    if (buttons->BB2tapped){
        app->screenState = configScreen;
        Graphics_clearDisplay(&hal_p->g_sContext);
        Settings_screen(hal_p, app);
        NextTrialTimer();
    }
}


void StartRandomTimer()
{
    uint32_t minTime = 3000; // Minimum time in milliseconds (3 seconds)
    uint32_t maxTime = 5000; // Maximum time in milliseconds (5 seconds)

    // Generate a random time between 3 and 5 seconds
    uint32_t randomTime_ms = minTime + (rand() % (maxTime - minTime + 1));

    // Convert milliseconds to cycles
    uint32_t cycles = (SYSTEM_CLOCK / 1000) * randomTime_ms;

    // Initialize and start the timer
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                       TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, cycles);
    Timer32_clearInterruptFlag(TIMER32_1_BASE);
    Interrupt_enableInterrupt(INT_T32_INT2);
    Timer32_startTimer(TIMER32_1_BASE, true);
}

void NextTrialTimer()
{

    uint32_t cycles = 2000000;
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
                       TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, cycles);
    Timer32_clearInterruptFlag(TIMER32_1_BASE);
    Interrupt_enableInterrupt(INT_T32_INT2);
    Timer32_startTimer(TIMER32_1_BASE, true);
}

