#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "feature_utils.h"


int main()
{
    double* ft = calloc(32,sizeof(double));
    double* fw = calloc(32,sizeof(double));
    for(int i = 0; i<32; i++)
    {
        if(i%4<2) ft[i] = 1;
        else ft[i] = -1;
    }

    initialize_feature_engine(32);
    dtft(ft, fw);
    close_feature_engine();

    for(int i=0; i<32; i++)
    {
        printf("%1.4f    ",fw[i]);
    }
    printf("\n");

    return 0;
}
