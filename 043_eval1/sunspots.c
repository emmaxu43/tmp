#include "sunspots.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

ss_monthly_t parseLine(char * line) {
  // WRITE ME
  ss_monthly_t ans;
  ans.year = 0;
  ans.month = 0;
  ans.num = 0;

  // Check whether the input file is empty or not.
  if (line == NULL) {
    fprintf(stderr, "Empty input.\n");
    exit(EXIT_FAILURE);
  }

  // Parse the input string from csv file.
  char *element;
  element = strtok(line, ","); //return to the first token , (similar to split in python)
  if (element == NULL) {
      fprintf(stderr, "Error: Invalid input format.\n");
      exit(EXIT_FAILURE);
  }

  // Retrieve time
  if (sscanf(element, "%4u-%2u", &ans.year, &ans.month) != 2) {
      fprintf(stderr, "Error: Invalid date format.\n");
      exit(EXIT_FAILURE);
  }
  
  // Retrieve sunspots number
  element = strtok(NULL, ",");
  if (element == NULL) {
      fprintf(stderr, "Error: Missing sunspots value.\n");
      exit(EXIT_FAILURE);
  }
  if (sscanf(element, "%lf", &ans.num) != 1) {
      fprintf(stderr, "Error: Invalid sunspots value.\n");
      exit(EXIT_FAILURE);
  }

  return ans;
}

void meanFilter(ss_monthly_t * data, size_t n, ss_monthly_t * mean, unsigned w) {
  // WRITE ME
  if (data == NULL) {
    fprintf(stderr, "Empty array.\n");
  }
  
  unsigned half_w = w/2; // window size w is an odd number. Truncation occurs here.
  
  // Read through the neighboring points.
  for (size_t i=0 ; i<n ; ++i){
    double sum = 0.0;
    unsigned sunspots_count=0;

    // Compute the sum of neighboring points.
    unsigned start_idx;
    if (i >= half_w){
      start_idx = i - half_w;
    } else {
      start_idx = 0;
    }
    
    unsigned end_idx;
    if (i + half_w < n){
      end_idx = i + half_w;
    } else {
      end_idx = n-1;
    }
    for (unsigned idx = start_idx; idx < end_idx+1; ++idx){
      sum += data[idx].num;
      ++sunspots_count;
    }
    
    // Compute the mean sunspots.
    mean[i].year = data[i].year;
    mean[i].month = data[i].month;
    mean[i].num = sum / sunspots_count;
  } 

}

double findLocalMax(ss_monthly_t * data, size_t n) {
  // WRITE ME
  if (data == NULL){
    fprintf(stderr,"Error: Empty input.\n");
    exit(EXIT_FAILURE);
  }
  
  if (n == 0 ){
    fprintf(stderr,"Error: Empty data.\n");
    exit(EXIT_FAILURE);
  }

  // Initialize variables.
  int max_idx = -1;
  double localMax_sunspots= -1.0;
  for (size_t i=0; i<n; ++i){
    if (data[i].num > localMax_sunspots){ // > operand make sure only save the first max element.
      localMax_sunspots = data[i].num;
      max_idx = i;
    }
  }
  
  // Check whether there is a localMAX or not.
  if (max_idx < 0){
    fprintf(stderr,"Error: No local maximum.\n");
    exit(EXIT_FAILURE);
  }

  // Compute the timestamp of localMAX.
  double timestamp;
  timestamp = data[max_idx].year + data[max_idx].month / 12.0;
  return timestamp; //return a double of the corresponding time stamp by calculating
  
  return 0;
}

double calcSsPeriod(double * timeStamps, size_t n) {
  //WRITE ME
  if (n <= 1){
    fprintf(stderr,"Error: Need more sample data to compute.\n");
    return EXIT_FAILURE;
  }
  
  // Compute the averaged cycle of sunspots.
  double temporalDiff = 0.0;
  double totalDiff = 0.0;
  for (size_t i=1; i<n; ++i){
    temporalDiff = fabs(timeStamps[i] - timeStamps[i-1]); //absolute difference of timeStamps to handle both ascending and descending order
    totalDiff += temporalDiff;
  } 
  double avrg_cycle = totalDiff / (n-1);
  return avrg_cycle;

  return 0;
}
