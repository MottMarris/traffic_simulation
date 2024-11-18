#include <sim.h>

int main()
{
	srand(time(0));
	int leftTime, rightTime, leftRate, rightRate;
	const gsl_rng_type *T;
	gsl_rng *r;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
	gsl_rng_set(r,time(0));

	printf("\nYou must now enter four paramter values to configure the simulation in this order: left");
	printf(" traffic light period, right traffic light period, left traffic light arrival rate and");
	printf(" right traffic light arrival rate.\n\n");
	printf("The traffic light period controls how long that traffic light is green for. So a value");
	printf(" of 10 will mean the traffic is green for 10 time steps.\n\n");
	printf("The traffic arrival rate controls how likely it is for a car to arrive at each time step,");
	printf(" so a rate of 1 means a car will arrive at every time step, and a rate of 5 means there is");
	printf(" a 1 in 5 chance of a car arriving at every time step.");
	printf("\n\nYou must enter the paramters in the following format shown inside the quotation marks: 'X, X, X, X'");
	printf(" and then hit the enter key.\n\n");
	printf("Please enter the four parameters as positive integers: ");

	if (scanf("%d, %d, %d, %d", &leftTime, &rightTime, &leftRate, &rightRate) != 4)
	{
		fprintf(stderr, "Wrong format\n.");
		return 1;
	}

	printf("\n");
	runSimulations(leftTime,rightTime,leftRate,rightRate, r);

	return 0;
}

void runSimulations(int leftLightPeriod, int rightLightPeriod, int leftArrivalRate, int rightArrivalRate, gsl_rng *random) {

	int i;
	STATS stats;
	int leftTotalCars = 0;
	long int leftAverageTimeWaiting = 0;
	int leftMaxTimeWaiting = 0;
	int leftClearanceTime = 0;
	int rightTotalCars = 0;
	long int rightAverageTimeWaiting = 0;
	int rightMaxTimeWaiting = 0;
	int rightClearanceTime = 0;

	for (i=0; i<100; i++)
	{
		STATS stats = runOneSimulation(leftLightPeriod,rightLightPeriod,leftArrivalRate,rightArrivalRate, random);
		leftTotalCars += stats.leftTotalCars;
		leftAverageTimeWaiting += stats.leftAverageTimeWaiting;
		leftMaxTimeWaiting += stats.leftMaxTimeWaiting;
		leftClearanceTime += stats.leftClearanceTime;
		rightTotalCars += stats.rightTotalCars;
		rightAverageTimeWaiting += stats.rightAverageTimeWaiting;
		rightMaxTimeWaiting += stats.rightMaxTimeWaiting;
		rightClearanceTime += stats.rightClearanceTime;
	}
	printf("Paramter values:\n");
	printf("   from left:\n");
	printf("      traffic arrival rate: %d\n", leftArrivalRate);
	printf("      traffic light period: %d\n", leftLightPeriod);
	printf("   from right:\n");
	printf("      traffic arrival rate: %d\n", rightArrivalRate);
	printf("      traffic light period: %d\n", rightLightPeriod);
	printf("Results (averaged over 100 runs):\n");
	printf("   from left:\n");
	printf("      number of vehicles: %d\n", leftTotalCars / 100 );
	printf("      average waiting time: %ld\n", leftAverageTimeWaiting / 100);
	printf("      maximum waiting time: %d\n", leftMaxTimeWaiting / 100);
	printf("      clearance time: %d\n", leftClearanceTime / 100);
	printf("   from right:\n");
	printf("      number of vehicles: %d\n", rightTotalCars / 100 );
	printf("      average waiting time: %ld\n", rightAverageTimeWaiting / 100);
	printf("      maximum waiting time: %d\n", rightMaxTimeWaiting / 100);
	printf("      clearance time: %d\n", rightClearanceTime / 100);
}

STATS runOneSimulation(int leftLightPeriod, int rightLightPeriod, int leftArrivalRate, int rightArrivalRate, gsl_rng *random)
{
	STATS stats = {0,0,0,0,0,0,0,0};
	char leftTrafficLight = '\0';
	char rightTrafficLight = '\0';
	NODE *leftTail = NULL;
	NODE *rightTail = NULL;
	int leftTimeWaiting = 0;
	int rightTimeWaiting = 0;
	int i = 0;
	int timer = 0;
	int timeWaiting;
	int nodeTime;

	setTrafficLights(&leftTrafficLight, &rightTrafficLight);

	while (leftTail != NULL || rightTail != NULL || i <= 500)
	{
		if (leftTrafficLight == 'G' && timer == leftLightPeriod)
		{
			setTrafficLights(&leftTrafficLight, &rightTrafficLight);
			timer = 0;
		}
		else if (rightTrafficLight == 'G' && timer == rightLightPeriod)
		{
			setTrafficLights(&leftTrafficLight, &rightTrafficLight);
			timer = 0;
		}
		else
		{
			if (i <= 500)
			{
				if ((gsl_ran_flat(random,0,leftArrivalRate) <= 1))
					queue(&leftTail, i);
				if ((gsl_ran_flat(random,0,rightArrivalRate) <= 1))
					queue(&rightTail, i);
			}

		if ((gsl_ran_flat(random,0,2) <= 1))
			{
				if (leftTrafficLight == 'G'){
					nodeTime = dequeue(&leftTail);
					if (nodeTime != -1)
					{
						timeWaiting = i - nodeTime;
						if (timeWaiting > stats.leftMaxTimeWaiting)
						 	stats.leftMaxTimeWaiting = timeWaiting;
						leftTimeWaiting += timeWaiting;
						stats.leftTotalCars += 1;
						if (i > 500 && leftTail == NULL)
						 	stats.leftClearanceTime = i - 500;
					}
				}
				if (rightTrafficLight == 'G'){
					nodeTime = dequeue(&rightTail);
					if (nodeTime != -1)
					{
						timeWaiting = i - nodeTime;
						if (timeWaiting > stats.rightMaxTimeWaiting)
						 	stats.rightMaxTimeWaiting = timeWaiting;
						rightTimeWaiting += timeWaiting;
						stats.rightTotalCars += 1;
						if (i > 500 && rightTail == NULL)
						 	stats.rightClearanceTime = i - 500;
					}
				}
			}
		}
		i+=1;
		timer+=1;
	}
	stats.leftAverageTimeWaiting = leftTimeWaiting / stats.leftTotalCars;
	stats.rightAverageTimeWaiting = rightTimeWaiting / stats.rightTotalCars;
	return stats;
}

void setTrafficLights(char *left, char *right)
{
	if (*left == '\0' && *right == '\0')
	{
		int action = 1+rand()%2;
		if (action == 1)
		{
			*left = 'G';
			*right = 'R';
		}
		else
		{
			*left = 'R';
			*right = 'G';
		}
	}
	if (*left == 'G' && *right == 'R')
	{
		*left = 'R';
		*right = 'G';
	}
	else
	{
		*left = 'G';
		*right = 'R';
	}
}
