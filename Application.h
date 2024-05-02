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
#define MAX_MEAN_ENTRIES 50

typedef struct _Application Application;
typedef enum {titleScreen, configScreen, gameScreen, resultsScreen} _screenState;

struct _Application {
    _screenState screenState;
    SWTimer titleScreenTimer;
    int numTrials;
    double reactionTimes[MAX_NUM_TRIALS];
    float trialTime[MAX_NUM_TRIALS];
    double meanTimes[MAX_MEAN_ENTRIES];
};



#endif /* APPLICATION_H_ */
