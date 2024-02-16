#include <stdio.h>
#include <stdlib.h>

int main(){
	int a = 3;
	int b = 4;
	int c = 5; 
	int *p = &a;
	int *q = &b;
	int *r = &c;
	int **s = &p;
	int **t = &q;

	*s = r; //originally point to a; it is legal to write p = r in this case;
	r = *t; //originally point to c;now r points to what t points to -> r = q; 现在要取的是value，不是地址；用词是what不是where；

	**s = 99; //通过deep referencing把b的值改成99；
        **t = 55; // c变成55；
}
