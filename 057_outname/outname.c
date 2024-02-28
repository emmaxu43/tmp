#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
  //WRITE ME
  size_t length = strlen(inputName) + strlen(".counts") + 1; // plus one for '\0'
  
  char *outputName = malloc(length * (sizeof(*outputName)));
  if (outputName == NULL) {
    fprintf(stderr, "Failed to allocate momery.\n");
    exit(EXIT_FAILURE);
  }

  // Generate new fileName
  strcpy(outputName, inputName);
  strcat(outputName, ".counts"); //连接两个字符串

  return outputName;

}
