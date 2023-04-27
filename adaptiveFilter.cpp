/**
* @file adaptiveFilter.cpp
* @author Melih Altun @2022
**/


#include "adaptiveFilter.h"
#include "statistics.h"


static float updateNoiseVar (float tolr, float *buffer)
{
	int k;
	float newNoiseVar = -1.0f, avg;
	bool allWithinLimit = true;

	mean(&avg, buffer, FILTER_TAPS);

	for (k = 0; k < FILTER_TAPS; k++)
		if (fabs(buffer[k] - avg) >= tolr) {
			allWithinLimit = false;
			break;
		}

	if (allWithinLimit)
		variance(&newNoiseVar, buffer, FILTER_TAPS);

	return newNoiseVar;
}


int adaptiveFilterInit(adaptiveFilterInstance *adaptiveFilterInst)
{
	if (adaptiveFilterInst == NULL)
		return -1;
	adaptiveFilterInst->clk = 0;
	adaptiveFilterInst->varHistCycle = 0;
	adaptiveFilterInst->noiseVar = 12.3f;
	adaptiveFilterInst->steadySignalTolr = 7.1f;
	memset(adaptiveFilterInst->filterBuffer, 0, FILTER_TAPS * sizeof(float));
	memset(adaptiveFilterInst->varHist, 0, 3 * sizeof(float));
	return 0;
}


int adaptiveFilterRun(adaptiveFilterInput *inp, adaptiveFilterInstance *adaptiveFilterInst, adaptiveFilterOutput *out)
{
	float newNoiseVar, localVar, avg, var;
	
	// check paramters
	if (inp == NULL || out == NULL || adaptiveFilterInst == NULL)
		return -1;
	
	//check input - use prev input if invalid
	if (isinf(inp->inputValue) || isnan(inp->inputValue)) {
		if (adaptiveFilterInst->clk == 0)
			inp->inputValue = 0.0f;
		else
			inp->inputValue = adaptiveFilterInst->filterBuffer[(adaptiveFilterInst->clk - 1) % FILTER_TAPS];
	}
	// pass input to buffer
	adaptiveFilterInst->filterBuffer[adaptiveFilterInst->clk % FILTER_TAPS] = inp->inputValue;

	// update noise variance if signal is at baseline
	if (adaptiveFilterInst->clk % FILTER_TAPS == FILTER_TAPS - 1) {
		newNoiseVar = updateNoiseVar(adaptiveFilterInst->steadySignalTolr, adaptiveFilterInst->filterBuffer);
		if (newNoiseVar > 0) {
			adaptiveFilterInst->varHist[adaptiveFilterInst->varHistCycle % 3] = newNoiseVar;
			if (adaptiveFilterInst->varHistCycle % 3 == 2) {
				mean(&newNoiseVar, adaptiveFilterInst->varHist, 3);
				adaptiveFilterInst->noiseVar = 3.0f * newNoiseVar;
			}
			adaptiveFilterInst->varHistCycle++;
		}
	}

	// apply averaging until filter buffer fills up
	if (adaptiveFilterInst->clk < FILTER_TAPS) {
		mean(&out->outputValue, adaptiveFilterInst->filterBuffer, adaptiveFilterInst->clk+1);
		out->filterActive = false;
		adaptiveFilterInst->clk++;
		return 0;
	}

	//Implementing : J.S.Lim, "Two-Dimensional Signal and Image Processing".Eqns. 9.44 to 9.46b
	mean(&avg, adaptiveFilterInst->filterBuffer, FILTER_TAPS);
	variance(&localVar, adaptiveFilterInst->filterBuffer, FILTER_TAPS);

	var = localVar - adaptiveFilterInst->noiseVar;
	if (var < 0)
		var = 0;

	out->outputValue = (inp->inputValue - avg) * var / (var + adaptiveFilterInst->noiseVar) + avg;  // adaptive wiener filter: signal power / (signal power + noise power)
	out->filterActive = true;

	adaptiveFilterInst->clk++;
	return 0;
}


int adaptiveFilterEnd(adaptiveFilterInstance *adaptiveFilterInst)
{
	// no dynamic memory;
	if (adaptiveFilterInst == NULL)
		return -1;

	adaptiveFilterInst->clk = 0;
	adaptiveFilterInst->varHistCycle = 0;

	return 0;
}

