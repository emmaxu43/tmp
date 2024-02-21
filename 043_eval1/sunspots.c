#include "sunspots.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

ss_monthly_t parseLine(char * line) {
  // WRITE ME
  ss_monthly_t output;
  output.year = 0;
  output.month = 0;
  output.num = 0.0;

  // Read the input file line by line. Check the line is empty or not.
  if (line == NULL) {
    fprintf(stderr, "Empty input.\n");
    exit(EXIT_FAILURE);
  }

  // Parse the input string from csv file.
  char *element;
  element = strtok(line, ","); //return to the first token , (similar to split[0] in python)
  if (element == NULL) {
    fprintf(stderr, "Error: Invalid input format.\n");
    exit(EXIT_FAILURE);
  }

  /* The input 'line' did not contain headers, also string headers did not include within the struct. No need to consider the incorrect format of header. 
  if (strcmp(element, "date,number of sunspots") == 1)) {
    fprintf(stderr, "Error: Invalid input format.\n");
    exit(EXIT_FAILURE);
  }
  */

  // Element should be 4 digit year & 2 digit month connected with a hypen
  if (strlen(element) != 7 || element[4] != '-') {
    fprintf(stderr, "Error: Invalid date format.\n");
    exit(EXIT_FAILURE);
    }

  // Retrieve year and month
  char year_str[5];
  char month_str[3];
  strncpy(year_str, element, 4);
  year_str[4] = '\0';
  strncpy(month_str, element+5, 2);
  month_str[2] = '\0';
  output.year = atoi(year_str);
  output.month = atoi(month_str);

  // Check the temporal information are valid or not.
  if ((output.year > 9999) || (output.year < 0)) {
    fprintf(stderr, "Error: Invalid year format.\n"); // year between 0000 and 9999
    exit(EXIT_FAILURE);
    }
  if ((output.month < 1) || (output.month > 12)) {
    fprintf(stderr, "Error: Invalid month format.\n"); // month between 01 and 12
    exit(EXIT_FAILURE);
    }

  // Retrieve sunspots number.
  element = strtok(NULL, ",");
  if (element == NULL) {
    fprintf(stderr, "Error: Missing sunspots value.\n");
    exit(EXIT_FAILURE);
  }

  // A valid sunspot number is non-negative float. 
  double sunspots = atof(element);
  output.num = sunspots;
  if (output.num < 0) {
    fprintf(stderr, "Error: Invalid sunspots value.\n");
    exit(EXIT_FAILURE);
  }

  // Seperate the decimal point to check it is a int or float.
  char* decimalP = strchr(element, '.');
  if (decimalP == NULL) {
    fprintf(stderr, "Error: Invalid sunspots format.\n");
    exit(EXIT_FAILURE);
  }

  return output;
}


void meanFilter(ss_monthly_t * data, size_t n, ss_monthly_t * mean, unsigned w) {
  // WRITE ME
  if (data == NULL) {
    fprintf(stderr, "Error: Empty array.\n");
    exit(EXIT_FAILURE);
  }
  
  // Check w is an odd positive number or not.
  if ((w <= 0) || (w % 2 !=1)) {
    fprintf(stderr, "Error: Invalid window size.\n");
    exit(EXIT_FAILURE);
  }

  // Check whether window size > neighboring points.
  if (w > n) {
    fprintf(stderr, "Error: Window size is larger than the data length.\n");
    exit(EXIT_FAILURE);
  }

  unsigned half_w = w / 2; // window size w is an odd number (casting occurs here & round floating points part).
  
  // Read through the neighboring points.
  for (size_t i = 0 ; i < n ; ++i) {
    double sum = 0.0;
    unsigned sunspots_count=0;

    // Compute the sum of neighboring points.
    unsigned start_idx;
    if (i >= half_w) {
      start_idx = i - half_w;
    } else {
      start_idx = 0; //Handle boundaries where the window is out of bounds.
    }
    
    unsigned end_idx;
    if (i + half_w < n) {
      end_idx = i + half_w;
    } else {
      end_idx = n-1; // handle boundaries where the window is out of bounds.
    }

    for (unsigned idx = start_idx; idx <= end_idx; ++idx){
      sum += data[idx].num;
      ++sunspots_count;
    }
    
    // Compute the mean sunspots (year, month, and ss_number should be valid afterh applying the formatting check in the step 1).
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

  // Only headers present.
  if (n == 0 ){
    fprintf(stderr,"Error: Empty data.\n");
    exit(EXIT_FAILURE);
  }

  // Initialize variables.
  int max_idx = -1;
  double localMax_sunspots= -1.0;
  for (size_t i=0; i<n; ++i){
    if (data[i].num > localMax_sunspots){ // > operand make sure we only save the first max element.
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
  if (n <= 1) {
    fprintf(stderr,"Error: Need more sample data to compute.\n");
    return EXIT_FAILURE;
  }
  
  // Compute the averaged cycle of sunspots.
  double temporalDiff = 0.0;
  double totalDiff = 0.0;
  for (size_t i = 1; i < n; ++i) {
    temporalDiff = fabs(timeStamps[i] - timeStamps[i-1]); //absolute difference of timeStamps to handle both ascending and descending order
    totalDiff += temporalDiff;
  } 
  double avrg_cycle = totalDiff / (n-1);
  return avrg_cycle;

  return 0;
}


