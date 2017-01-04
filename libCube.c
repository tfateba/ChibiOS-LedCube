/**
 *
 * @file    cubeLib.c
 *
 * @brief   Led cube functions and demos.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    03 January 2017
 *
 * @update  05 January 2017
 */

#include "libCube.h"

static uint8_t demoIndex = 0;

/*===========================================================================*/
/* Les fonctions.                                                            */
/*===========================================================================*/

/**
 * @fn      cubeInit
 * @brief   Initialize the pins used for the cube.
 */
void cubeInit(void) {
  int8_t i;

  for (i = 5; i >= 0; i--) {
    palSetPadMode(IOPORT2, i, PAL_MODE_OUTPUT_PUSHPULL);
  }
  
  for (i = 7; i >= 2; i--) {
    palSetPadMode(IOPORT4, i, PAL_MODE_OUTPUT_PUSHPULL);
  }
}

/**
 * @fn      padCtrl
 * @brief   Set/Clear a gpio pin.
 *
 * @param[in] port    the gpio pin port to control
 * @param[in] pin     the gpio pin to control
 * @param[in] value   the gpio pin value to set
 */
static void padCtrl(volatile avr_gpio_registers_t *port, uint8_t pin,
                    uint8_t value) {
  if (value == 0)
    palClearPad(port, pin);
  else if (value == 1)
    palSetPad(port, pin);
  else
    return;
}

/**
 * @fn      cubeOn
 * @brief   Turn on all the cube.
 *
 * @param[in] tempo   the time to turn on the cube
 */
static void cubeOn(uint16_t tempo) {
  int8_t i;

  for (i = 5; i >= 0; i--) {
    padCtrl(IOPORT2, i, 1);
  }

  for (i = 7; i >= 2; i--) {
    padCtrl(IOPORT4, i, 1);
  }

  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeOff
 * @brief   Turn off the cube
 *
 * @param[in] tempo   the time to turn on the cube
 */
static void cubeOff(uint16_t tempo) {
  int8_t i;

  for (i = 5; i >= 0; i--) {
    padCtrl(IOPORT2, i, 0);
  }

  for (i = 7; i >= 2; i--) {
    padCtrl(IOPORT4, i, 0);
  }

  chThdSleepMilliseconds(tempo);
}

static void facto(uint16_t tempo) {
  uint8_t i;

  for (i = 2; i <= 7; i++) {
    palSetPad(IOPORT4, i);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 0; i <= 2; i++) {
    palSetPad(IOPORT2, i);
    chThdSleepMilliseconds(tempo);
  }
}

static void demo1(uint8_t tempo) {
  cubeOff(0);
  padCtrl(IOPORT2, 3, 1);
  padCtrl(IOPORT2, 4, 0);
  padCtrl(IOPORT2, 5, 0);
  facto(tempo);

  cubeOff(0);
  padCtrl(IOPORT2, 3, 0);
  padCtrl(IOPORT2, 4, 1);
  padCtrl(IOPORT2, 5, 0);
  facto(tempo);

  cubeOff(0);
  padCtrl(IOPORT2, 3, 0);
  padCtrl(IOPORT2, 4, 0);
  padCtrl(IOPORT2, 5, 1);
  facto(tempo);
}

/**
 * @fn      lineWrite
 * @brief   piloter les Leds des plans du cube qui sont actifs.
 *
 * @param[in] lineState   buffer of line state
 * @param[in] lineSize    Size of the line state buffer
 */
static void lineWrite( uint8_t *lineState, uint8_t lineSize) {
  uint8_t line = 0, pin;

  for (pin = 2; pin <= 7; pin++) {
    padCtrl(IOPORT4, pin, lineState[line++]);
    if (line > lineSize)
      return;
  }

  for (pin = 0; pin <= 2; pin++) {
    padCtrl(IOPORT2, pin, lineState[line++]);
    if (line > lineSize)
      return;
  }
}


/**
 * @fn      layerWrite
 * @brief   Selection d'un des trois plans du cube.
 * 
 * @param[in] layerState  buffer of layer state
 * @param[in] layerSize   Size of the layer state buffer
 */
static void layerWrite(uint8_t *layerState, uint8_t layerSize) {
  uint8_t layer = 2, pin;

  for (pin = 5; pin >= 3; pin--) {
    padCtrl(IOPORT2, pin, layerState[layer--]);
    if (layer > layerSize)
      return;
  }
}

/**
 * @fn      cubeTopOff
 * @brief   Eteindre le plan 1 du cube.
 *
 * @param[in] tempo temps d'extinction du dessus du cube
 */
static void cubeTopOff(uint16_t tempo) {
  palClearPad(IOPORT2, 3);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeTopOn
 * @brief   Allumer le plan 1 du cube.
 * 
 * @param[in] tempo temps d'allumage du dessus du cube.
 */
static void cubeTopOn(uint16_t tempo){
  uint8_t plan[3] = {1,0,0}; // top , midle, Bottom
  uint8_t lineState[9] = {1, 1, 1, 1,1, 1, 1, 1, 1};

	layerWrite(plan, 3);
	lineWrite(lineState, 9);
	chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeMidleOff
 * @brief   Eteindre le plan 2 du cube.
 *
 * @param[in] tempo   temps d'extinction du plan 2 du cube
 */
static void cubeMidleOff(uint16_t tempo) {
  palClearPad(IOPORT2, 4);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeMidleOn
 * @brief   Allumer le Plan 2 du cube.
 *
 * @param[in] tempo   temps d'allumage du plan 2 du cube.
 */
static void cubeMidleOn(uint16_t tempo) {
  uint8_t plan[3] = {0,1,0}; // top , midle, Bottom
  uint8_t lineState[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeBottomOff
 * @brief   Eteindre le plan 3 du cube.
 *
 * @param[in] tempo   temps d'extinction du plan 3 du cube
 */
static void cubeBottomOff(uint16_t tempo) {
  palClearPad(IOPORT2, 5);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeBottomOn
 * @brief   Allumer le plan 3 du cube.
 *
 * @param[in]	tempo   temps d'allumage du plan 3 du cube
 */
static void cubeBottomOn(uint16_t tempo) {
  uint8_t plan[3] = {0,0,1}; // top , midle, Bottom
  uint8_t lineState[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; 

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeBlink
 * @brief   Faire clignoter le cube de leds.
 *
 * @param[in] tempo   le temps de clignotement en milli seconde
 * @param[in] nbr     le nombre de clignotement souhaite
 */
static void cubeBlink(uint16_t tempo, uint8_t nbr) {
  int8_t i;

  for (i = nbr; i >= 0; i--) {
    cubeOn(tempo);
    cubeOff(tempo);
  }
}

/**
 * @fn      cubeCircularDemo
 * @brief   Faire une demonstareation d'alumage circulaire.
 *
 * @param[in] tempo   temps de l'effet de rotation
 */
static void cubeCircularDemo(uint16_t tempo) {
  int8_t i, j;
  uint8_t lineState[8][9] = { {1, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 1, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 1, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {0, 0, 0, 0, 0, 0, 0, 1, 0},
                              {0, 0, 0, 0, 0, 0, 1, 0, 0},
                              {0, 0, 0, 1, 0, 0, 0, 0, 0}};

  for (i = 2; i >= 0; i--) {
    if (i == 0) {
      uint8_t plan[3] = {1,0,0};
      layerWrite(plan, 3);
    }

    if (i == 1) {
      uint8_t plan[3] = {0,1,0};
      layerWrite(plan, 3);
    }

    if (i == 2) {
      uint8_t plan[3] = {0,0,1};
      layerWrite(plan, 3);
    }

    for (j = 0; j <= 7; j++) {
      lineWrite(&(lineState[j][0]), 9);
      chThdSleepMilliseconds(tempo);
    }
  }
}

/**
 * @fn      cubeFace1On
 * @brief   Allumer la face numero 1 du cube.
 *
 * @param[in] tempo   temps d'allumage de la face 1 du cube
 */
static void cubeFace1On(uint16_t tempo) {
  uint8_t plan[3] = {1,1,1}; // Bottom, midle, top
  uint8_t lineState[9] = {1, 1, 1, 0, 0, 0, 0, 0, 0};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeFace2On
 * @brief   Allumer la face numero 2 du cube.
 *
 * @param[in] tempo   temps d'allumage de la face 2 du cube
 */
static void cubeFace2On(uint16_t tempo) {
  uint8_t plan[3] = {1,1,1}; // Bottom, midle, top
  uint8_t lineState[9] = {1, 0, 0, 1, 0, 0, 1, 0, 0};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeFace3On
 * @brief   Allumer la face numero 3 du cube.
 *
 * @param[in] tempo   temps d'allumage de la face 3 du cube.
 */
static void cubeFace3On(uint16_t tempo) {
  uint8_t plan[3] = {1,1,1}; // Bottom, midle, top
  uint8_t lineState[9] = {0, 0, 0, 0, 0, 0, 1, 1, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeFace4On
 * @brief   Allumer la face numero 4 du cube.
 *
 * @param[in] tempo   Operation delay
 */
static void cubeFace4On(uint16_t tempo) {
  uint8_t plan[3] = {1,1,1}; // Bottom, midle, top
  uint8_t lineState[9] = {0, 0, 1, 0, 0, 1, 0, 0, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @fn      cubeAllFaceOn
 * @brief   Allumer toutes les faces du cube.
 *
 * @param[in] tempo Operation delay
 */
static void cubeAllFaceOn(uint16_t tempo) {
  cubeFace1On(tempo);
  cubeFace2On(tempo);
  cubeFace3On(tempo);
  cubeFace4On(tempo);
}

/**
 * @fn      rotation
 * @brief   Faire une demonstareation d'alumage circulaire.
 *
 * @param[in] tempo temps d'allumage entre les LEDs des plans du cube
 */
static void rotation(uint16_t tempo) {
  uint8_t i;
  uint8_t plan[3] = {1,1,1}; // Bottom, midle, top
  uint8_t lineState[4][9] = { {1, 0, 0, 0, 1, 0, 0, 0, 1},
                              {0, 0, 0, 1, 1, 1, 0, 0, 0},
                              {0, 0, 1, 0, 1, 0, 1, 0, 0},
                              {0, 1, 0, 0, 1, 0, 0, 1, 0}};

  layerWrite(plan, 3);

  for (i = 0; i <= 3; i++) {
    lineWrite(&(lineState[i][0]), 9);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @fn      effect
 * @brief   Effet d'allumage du bas vers le haut et vis vers ca.
 *
 * @param[in] tempo   temps  entre les transition de niveaux
 * @param[in] nbr     nobre de fois que l'on veut voir l'effet
 */
static void effect(uint16_t tempo, uint8_t nbr) {
  uint8_t j;

  for (j = 0; j <= nbr; j++) {
    cubeTopOn(tempo);
    cubeMidleOn(tempo);
    cubeBottomOn(tempo);
    cubeMidleOn(tempo);
  }
}

/**
 * @fn      shadowOn
 * @brief   Allumage aleatoire.
 *
 * @param[in] tempo temps entre l'allumage de deux LEDs, en milli-seconde
 */
static void shadowOn(uint16_t tempo) {
  uint8_t i;

  for (i = 3; i <= 5; i++) {
    palSetPad(IOPORT2, i);
  }

  for (i = 0; i <= 2; i++) {
    padCtrl(IOPORT2, i, 1);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 2; i <= 7; i++) {
    padCtrl(IOPORT4, i, 1);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @fn      shadowOff
 * @brief   turn off the cube aleatoire.
 *
 * @param[in] tempo temps entre l'allumage de deux LEDs, en milli-seconde
 */
static void shadowOff(uint16_t tempo) {
  uint8_t i;

  for (i = 3; i <= 5; i++) {
    palSetPad(IOPORT2, i);
  }

  for (i = 0; i <= 2; i++) {
    padCtrl(IOPORT2, i, 0);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 2; i <= 7; i++) {
    padCtrl(IOPORT4, i, 0);
    chThdSleepMilliseconds(tempo);
  }
}

void demoCube(void) {
  register uint8_t i, nbr = 10;

  cubeOff(0);

  //demoIndex = 12;

  if (demoIndex <= 12) {
    for (i = 0; i < nbr; i++) {
      switch (demoIndex) {
        case 0:
          cubeOff(50);
          cubeOn(50);
        break;

        case 1:
          cubeTopOff(50);
          cubeTopOn(50);
        break;

        case 2:
          cubeMidleOn(50);
          cubeMidleOff(50);
        break;

        case 3:
          cubeBottomOn(50);
          cubeBottomOff(50);
        break;

        case 4:
          cubeFace1On(50);
          cubeOff(50);
        break;

        case 5:
          cubeFace2On(50);
          cubeOff(50);
        break;

        case 6:
          cubeFace3On(50);
          cubeOff(50);
        break;

        case 7:
          cubeFace4On(50);
          cubeOff(50);
        break;

        case 8:
          cubeAllFaceOn(100);
        break;

        case 9:
          cubeCircularDemo(50);
        break;

        case 10:
          rotation(100);
        break;

        case 11:
          demo1(50);
        break;

        case 12:
          shadowOn(80);
          shadowOff(80);
        break;
      }
    }
    demoIndex++;
  }
  else if (demoIndex == 13) {
    effect(50, 10);
    demoIndex++;
  }
  else if (demoIndex == 14) {
    cubeBlink(50, 5);
    demoIndex++;
  }
  else
    demoIndex = 0;
}
