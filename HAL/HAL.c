/*
 * HAL.c
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */


#include <HAL/HAL.h>

/**
 * Constructs a new API object. The API constructor should simply call the
 * constructors of each of its sub-members with the proper inputs.
 *
 * @return a properly constructed API object.
 */

HAL HAL_construct() {
  // The API object which will be returned at the end of construction
  HAL hal;
/*
  // Initialize all LEDs by calling their constructors with correctly-defined
  // arguments.
  hal.launchpadLED1 = LED_construct(LAUNCHPAD_LED1_PORT, LAUNCHPAD_LED1_PIN);

  hal.launchpadLED2Red =
      LED_construct(LAUNCHPAD_LED2_RED_PORT, LAUNCHPAD_LED2_RED_PIN);
  hal.launchpadLED2Green =
      LED_construct(LAUNCHPAD_LED2_GREEN_PORT, LAUNCHPAD_LED2_GREEN_PIN);
  hal.launchpadLED2Blue =
      LED_construct(LAUNCHPAD_LED2_BLUE_PORT, LAUNCHPAD_LED2_BLUE_PIN);

  hal.boosterpackRed =
      LED_construct(BOOSTERPACK_LED_RED_PORT, BOOSTERPACK_LED_RED_PIN);
  hal.boosterpackGreen =
      LED_construct(BOOSTERPACK_LED_GREEN_PORT, BOOSTERPACK_LED_GREEN_PIN);
  hal.boosterpackBlue =
      LED_construct(BOOSTERPACK_LED_BLUE_PORT, BOOSTERPACK_LED_BLUE_PIN);

  // Initialize all Buttons by calling their constructors with correctly-defined
  // arguments.
  hal.launchpadS1 =
      Button_construct(LAUNCHPAD_S1_PORT, LAUNCHPAD_S1_PIN);  // Launchpad S1
  hal.launchpadS2 =
      Button_construct(LAUNCHPAD_S2_PORT, LAUNCHPAD_S2_PIN);  // Launchpad S2

  hal.boosterpackS1 = Button_construct(BOOSTERPACK_S1_PORT,
                                       BOOSTERPACK_S1_PIN);  // Boosterpack S1
  hal.boosterpackS2 = Button_construct(BOOSTERPACK_S2_PORT,
                                       BOOSTERPACK_S2_PIN);  // Boosterpack S2
  hal.boosterpackJS = Button_construct(BOOSTERPACK_JS_PORT,
                                       BOOSTERPACK_JS_PIN);  // Joystick Button
  hal.joystick = Joystick_construct();

  // Construct the UART module inside of this HAL struct
  hal.uart = UART_construct(USB_UART_INSTANCE, USB_UART_PORT, USB_UART_PINS);

  InitGraphics(&hal.g_sContext);
  */

  // Enable the UART at 9600 BPS
  // TODO: Call UART_SetBaud_Enable to achieve the above goal

  // Once we have finished building the API, return the completed struct.
  initGraphics(&hal.g_sContext);
  return hal;
}

/**
 * Upon every new cycle of the main super-loop, we MUST UPDATE the status of
 * all inputs. In this program, this function is called only once in the
 * Application_loop() function. Since the Application_loop() function is called
 * once per loop of the while (true) loop in main, we are effectively polling
 * all inputs once per loop.
 *
 * @param hal:  The API whose input modules we wish to refresh
 */
void HAL_refresh(HAL* hal) {

}
