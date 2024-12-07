#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int frequence_arrive(float lambda) {
    float U = (float)rand() / (float)(RAND_MAX+1.0);
    printf("%lf\n", U);
    float X = (-1)*log(1.0-U)/lambda;
    printf("%lf", X);
    return X;
}



