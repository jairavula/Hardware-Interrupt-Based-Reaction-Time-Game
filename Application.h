/*
 * Application.h
 *
 *  Created on: Apr 25, 2024
 *      Author: jaikr
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <HAL/HAL.h>

#define MAX_NUM_TRIALS 3

typedef struct _Application Application;
typedef enum {titleScreen, configScreen} _screenState;

struct _Application {
    _screenState screenState;
    SWTimer titleScreenTimer;
    int numTrials;
    float trialTime[MAX_NUM_TRIALS];
};



#endif /* APPLICATION_H_ */
