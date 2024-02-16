#include <stdio.h>
#include <stdlib.h>

void rt_triangle(int N) {
	//count from 1 to N inclusively
	for (int i=1; i<=N; ++i) {
		// count from 1 to i
		for (int j=1; j<=i; ++j){
			printf('*')
		// create a new line
		printf('\n')
		}

}
// ++i is faster then i++: pre-increment ++i, and post_increment i++.
// single quotes are used to denote a single character, whereas double quotes are used to denote a string of characters.

int main(int argc, char** argv){
	if (argc!=2){
		fprintnf(stderr,"Usage: ./main N\n");
		return EXIT_FAILURE;
	}
	print_rt_triangle(strtod(argv[1],NULL));
	return EXIT_SUCCESS;
}


// compling cmd: gcc -std=gnu99 -Wall -Werror -pedantic main.c -o main
// -Wall WARNING ALL
// -Werror turn all the warnings into errors
// -pedantic flag things which the authors of gcc thought quality compilers should suppor
// -o output -> excutable to main
// execute cmd: ./main -parameter
