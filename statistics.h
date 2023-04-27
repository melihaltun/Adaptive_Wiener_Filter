/**
* @file statistics.h
* @author Melih Altun @2015
**/

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/* mean value of buffer x with size N */
int mean(float *output, float x[], int N);

/* variance of buffer x with size N */
int variance(float *output, float x[], int N);

/* standart deviation of buffer x with size N */
int stDev(float *output, float x[], int N);

/* a % b modulo operation that supports negative inputs */
int mod(int *output, int a, int b);

/* peak finder operation: (outputs) peak values, peak locations, (inputs) input series, min peak value, size of output arrays, size of input series */
int findPeaks(float pks[], int locs[], int *pkCount, float x[], float minVal, int maxNumPeaks, int sizeX);

/* in place insertion sort as a utility function: (output/input) series to be sorted, (input) series length, ascending / descending option */
int iSort(float inOut[], int length, bool descend);

/* median of a series with given length */
int median(float* output, float in[], int length);

