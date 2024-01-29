#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
	int months;
  	double contribution;
  	double rate_of_return;

};

typedef struct _retire_info retire_info;


double monthly_calculator(retire_info name,double balance){

	double monthly_rate = name.rate_of_return/12;
	double returns = balance * monthly_rate;
	balance += (returns + name.contribution);

	return balance;
}


void retirement (int startAge,   //in months
                       double initial, //initial savings in dollars
                       retire_info working, //info about working
                       retire_info retired) //info about being retired
{
	while (working.months>0)	
		{
		int yr = startAge/12;
		int month = startAge - 12*yr;
		printf("Age %3d month %2d you have $%.2f\n", yr,month,initial);
		initial = monthly_calculator(working,initial);
		working.months -= 1;
		startAge += 1;
		}

	while (retired.months>0)
		{int yr = startAge/12;
                int month = startAge - 12*yr;
                printf("Age %3d month %2d you have $%.2f\n", yr,month,initial);
		initial = monthly_calculator(retired,initial);
		retired.months -= 1;
                startAge += 1;
		}

}

int main() {

	retire_info working;
	working.months = 489;
	working.contribution = 1000;
	working.rate_of_return = 0.045;
  
	retire_info retired;
	retired.months = 384;
	retired.contribution = -4000;
	retired.rate_of_return = 0.01;
	retirement(327, 21345, working, retired);
  
	int startAge = 327;
	double initial = 21345.0;
	retirement(startAge,initial,working,retired);

	return EXIT_SUCCESS;

}


