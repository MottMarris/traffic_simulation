#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

struct trafficLightStatistics
{
	int leftTotalCars;
	int leftAverageTimeWaiting;
	int leftMaxTimeWaiting;
	int leftClearanceTime;
	int rightTotalCars;
	int rightAverageTimeWaiting;
	int rightMaxTimeWaiting;
	int rightClearanceTime;
};
typedef struct trafficLightStatistics STATS;

void setTrafficLights(char *left, char *right);
void runSimulations(int leftLightPeriod, int rightLightPeriod, int leftArrivalRate, int rightArrivalRate, gsl_rng *random);
STATS runOneSimulation(int leftLightPeriod, int rightLightPeriod, int leftArrivalRate, int rightArrivalRate, gsl_rng *random);
