/**
 *
 * @file    main.c
 *
 * @brief   Main file of the application.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    03 January 2017
 *
 * @update  05 January 2017
 *
 */

#include "ch.h"
#include "hal.h"
#include "libCube.h"

static THD_WORKING_AREA(waThread1, 64);
static THD_FUNCTION(Thread1, arg) {
  uint8_t demoIndex = 0;
  (void)arg;

  chRegSetThreadName("demo");

  while (true) {
    demoCube();
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Initialization of the cube.
   */
  cubeInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  sdStart(&SD1, NULL);

  /*
   * Starts the LED blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 2, Thread1, NULL);

  while(TRUE) {
    chThdSleepMilliseconds(1000);
  }
}
