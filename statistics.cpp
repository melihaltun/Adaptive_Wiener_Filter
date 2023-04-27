/**
* @file statistics.cpp
* @author Melih Altun @2015
**/

#include "statistics.h"


void swap(float* a, float* b)
{
	float temp = *a;
	*a = *b;
	*b = temp;
}

/* finds average of a series */
int mean(float *output, float x[], int N)
{
	float sum = 0;
	int i;

	if (x == NULL || output == NULL)
		return -1;
	if (N < 1)
		return -2;

	for (i = 0; i < N; i++)
		sum += x[i];

	*output =  sum / (float)N;

	return 0;
}

/* finds variance of a series using sigma = sum_k (x_k - mean(x)) / (N-1) formula
   note that there is also another variance formula where sigma = sum_k (x_k - mean(x)) / N */
int  variance(float *output, float x[], int N)
{
	float avg, sum = 0;
	int i;

	if (x == NULL || output == NULL)
		return -1;
	if (N < 1)
		return-2;

	mean(&avg, x, N);

	for (i = 0; i < N; i++)
		sum += powf((x[i] - avg), 2);
	*output = sum / (float)(N - 1);

	return 0;
}

/* returns standard deviation of a series */
int stDev(float* output, float x[], int N)
{
	float var;

	if (x == NULL || output == NULL)
		return -1;
	if (N < 1)
		return -2;

	variance(&var, x, N);
	*output = sqrtf(var);

	return 0;
}


/* modulo function for circular buffer indices */
int mod(int *output, int a, int b)
{
	int c;
	if (output == NULL)
		return -1;
	if (b == 0)
		return -2;

	if (b < 0) {
		mod(output, -a, -b);
		return 0;
	}
	c = a % b;
	*output = (c < 0) ? c + b : c;

	return 0;
}

/* finds peak values above a minimum value in a series. Peaks and Locations arrays can hold a limited about of elements */
int findPeaks(float pks[], int locs[], int* pkCount, float x[], float minVal, int maxNumPeaks, int sizeX)
{
	int lastIncrease = 0, i;

	if (pkCount == NULL || pks == NULL || locs == NULL || x == NULL)
		return -1;
	if (sizeX < 1)
		return -2;

	*pkCount = 0;

	//make sure peaks and locs arrays are allocated!
	memset(pks, 0, maxNumPeaks*sizeof(float));
	memset(locs, 0, maxNumPeaks*sizeof(int));

	for (i = 1; i < sizeX; i++) {
		if (x[i]>x[i - 1])
			lastIncrease = i;
		else if (x[i] < x[i - 1]) {
			if (x[i-1]>minVal && lastIncrease != 0 && *pkCount < maxNumPeaks) {
				locs[*pkCount] = lastIncrease;
				pks[*pkCount] = x[i - 1];
				(*pkCount)++;
			}
			lastIncrease = 0;
		}
	}
	return 0;
}

//insertion sort in place
int iSort(float inOut[], int length, bool descend)
{
	int i, j;

	if (inOut == NULL)
		return -1;
	if (length < 1)
		return -2;

	if (!descend) { //sort ascending
		for (i = 1, j = i; i < length; i++, j = i) {
			while (j > 0 && inOut[j - 1] > inOut[j]) {
				swap(&inOut[j], &inOut[j - 1]);
				j--;
			}
		}
	}
	else {
		for (i = 1, j = i; i < length; i++, j = i) {
			while (j > 0 && inOut[j - 1] < inOut[j]) {
				swap(&inOut[j], &inOut[j - 1]);
				j--;
			}
		}
	}
	return 0;
}

//median with insertion sort in place
int median(float *output, float in[], int length)
{
	if (in == NULL || output == NULL)
		return -1;
	if (length < 1)
		return -2;

	iSort(in, length, false);  //comb sort
	if (length % 2 == 0)
		*output = (in[length / 2] + in[(length / 2) - 1]) / 2;
	else
		*output = in[(length - 1) / 2];
	return 0;
}
