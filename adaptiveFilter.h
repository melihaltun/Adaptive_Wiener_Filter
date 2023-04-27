/**
* @file adaptiveFilter.h
* @author Melih Altun @2022
**/


#include <stdbool.h>
#include <math.h>
#include <string.h>

#define FILTER_TAPS 9

typedef struct adaptiveFilterInstance_ {
	float filterBuffer[FILTER_TAPS]; // input history
	float varHist[3];		// history of noise variances
	float noiseVar;			// noise variance: default 0.0333;
	float steadySignalTolr;		// +/- boundry to determine if the signal is steady
	unsigned int clk;		// filter cycle
	unsigned int varHistCycle;	// variance history cycle
} adaptiveFilterInstance;


typedef struct adaptiveFilterInput_ {
	float inputValue;
} adaptiveFilterInput;

typedef struct adaptiveFilterOutput_ {
	float outputValue;
	bool filterActive;
} adaptiveFilterOutput;

int adaptiveFilterInit(adaptiveFilterInstance *adaptiveFilterInst);

int adaptiveFilterRun(adaptiveFilterInput *inp, adaptiveFilterInstance *adaptiveFilterInst, adaptiveFilterOutput *out);

int adaptiveFilterEnd(adaptiveFilterInstance *adaptiveFilterInst);

