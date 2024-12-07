#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "struct.h"

Heure créer_durée() {
    Heure h;
    h.heure = 0;
    float U = (float)rand() / (float)(RAND_MAX + 1.0);
    int U_scale = (int)(U * 100) + 1;
    while (U_scale > 60) {
        h.heure +=1;
        U_scale -=60;
    }
    h.minute = U_scale;
    return h; // Échelle à [1, 100]

}