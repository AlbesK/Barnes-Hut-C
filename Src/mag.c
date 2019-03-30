#include<stdio.h>
#include<math.h>
#include"bhHeaders.h"

/*
    Get the magnitude of the 2D vector
*/
void mag(double* m, double* d){
    *m = sqrt(d[0]*d[0]+d[1]*d[1]);
}
