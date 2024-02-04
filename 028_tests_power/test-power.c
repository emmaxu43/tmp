#include <stdio.h>
#include <stdlib.h>


//prototype
unsigned power(unsigned x, unsigned y);

/*
//call function
unsigned power(unsigned x, unsigned y) {
    unsigned result = 1;
    for (unsigned i = 0; i < y; i++) {
        result *= x;
    }
    return result;
}
*/


void run_check(unsigned x, unsigned y, unsigned expected_ans){
	if (power(x, y) != expected_ans) {
    		exit(EXIT_FAILURE);
  }	
}

int main(){
	run_check(6,0,1);
	run_check(0,10,0);
	run_check(1,6,1);
	run_check(9,0,1);
	run_check(4, 4, 256);
	run_check(50, 3, 125000);
	run_check(4294967294, 1, 4294967294);
	run_check(0,0,1);
	return EXIT_SUCCESS;



}
