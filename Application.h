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
    _screenState screenState; // tracks the screen state
    SWTimer titleScreenTimer; // SW timer used for title screen
    int numTrials; // counts the numiber of trials chosen
    double reactionTimes[MAX_NUM_TRIALS]; // tracks reaction time of a trial
    float trialTime[MAX_NUM_TRIALS]; // tracks the mean of 1 set of trials
    double meanTimes[MAX_MEAN_ENTRIES]; // mean array for high scores
};



#endif /* APPLICATION_H_ */
