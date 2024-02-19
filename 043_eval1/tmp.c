#include "sunspots.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

ss_monthly_t parseLine(char * line) {
  // WRITE ME
  printf("Input line: %s\n", line);
  ss_monthly_t output;
  output.year = 0;
  output.month = 0;
  output.num = 0.0;

  char *element;
  element = strtok(line, ","); //return to the first token , (similar to split in python)
  if (element == NULL) {
    fprintf(stderr, "Error: Invalid input format.\n");
    exit(EXIT_FAILURE);
  }
  int tmp = strlen(element);
  printf("TIME: %s  %d \n", element, tmp);

  // Element should be 4 digit year & 2 digit month connected with a hypen
  if (strlen(element) != 7 || element[4] != '-') {
    fprintf(stderr, "Error: Invalid date format.\n");
    exit(EXIT_FAILURE);
    }

  // Retrieve time.
  if (sscanf(element, "%4u-%2u", &output.year, &output.month) != 2) {
    fprintf(stderr, "Error: Invalid date format.\n");
    exit(EXIT_FAILURE);
  }

  // Retrieve year and month
  if (output.year > 9999) {
    fprintf(stderr, "Error: Invalid year format.\n");
    exit(EXIT_FAILURE);
    }
  if (output.month < 1 || output.month > 12) {
    fprintf(stderr, "Error: Invalid month format.\n");
    exit(EXIT_FAILURE);
    }

  // Retrieve sunspots number.
  element = strtok(NULL, ",");
  printf("ssNUM: %s\n", element);
  if (element == NULL) {
    fprintf(stderr, "Error: Missing sunspots value.\n");
    exit(EXIT_FAILURE);
  }
  
  // A valid sunspot number is non-negative float.
  if (sscanf(element, "%lf", &output.num) != 1) {
    fprintf(stderr, "Error: Invalid sunspots format.\n");
    exit(EXIT_FAILURE);
  }
  if (output.num < 0) {
    fprintf(stderr, "Error: Invalid sunspots value.\n");
    exit(EXIT_FAILURE);
  }

  // Seperate the decimal point.
  char* decimalPart = strchr(element, '.');
  if (decimalPart == NULL) {
    fprintf(stderr, "Error: Invalid sunspots format.\n");
    exit(EXIT_FAILURE);
  }

  
  return output;
}


