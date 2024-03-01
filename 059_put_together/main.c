#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"
#include "counts.h"
#include "outname.h"

counts_t * countFile(const char * filename, kvarray_t * kvPairs) {
  //WRITE ME
  //return NULL;
  FILE * f = fopen(filename, "r");
  if (f == NULL) {
    return NULL;
  }
  counts_t * c = createCounts();
  char * line = NULL;
  size_t sz;
  while (getline(&line, &sz, f) >= 0) {
    char * newline = strchr(line, '\n');
    *newline = '\0';
    addCount(c, lookupValue(kvPairs, line));
    free(line);
    line = NULL;
  }
  free(line);
  fclose(f);
  return c;
}

int main(int argc, char ** argv) {
  //WRITE ME (plus add appropriate error checking!)
  // Error check
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <k/v file> <input file>...\n", argv[0]);
    return EXIT_FAILURE;
  }
 
  //read the key/value pairs from the file named by argv[1] (call the result kv)
  kvarray_t * kv = readKVs(argv[1]);

  //count from 2 to argc (call the number you count i)
  for (int i = 2; i < argc; ++i) {
    //count the values that appear in the file named by argv[i], using kv as the key/value pair
    //   (call this result c)
    counts_t * c = countFile(argv[i], kv);
    if (c == NULL) {
      fprintf(stderr, "Failed to count file: %s\n", argv[i]);
      return EXIT_FAILURE;
    }

    //compute the output file name from argv[i] (call this outName)
    char * outName = computeOutputFileName(argv[i]);

    //open the file named by outName (call that f)
    FILE * f = fopen(outName, "w");
    if (f == NULL) {
      fprintf(stderr, "Failed to open output file: %s\n", outName);
      return EXIT_FAILURE;
    }

    //print the counts from c into the FILE f
    printCounts(c, f);

    //close f
    fclose(f);

    //free the memory for outName and c
    free(outName);
    freeCounts(c);
  }

  //free the memory for kv
  freeKVs(kv);

  return EXIT_SUCCESS;
}
