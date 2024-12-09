#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../h/alea.h"

int frequence_arrive(float lambda) {
    float U = (float)rand() / (float)(RAND_MAX+1.0);
    float X = (-1)*log(1.0-U)/lambda;
    return X;
}



