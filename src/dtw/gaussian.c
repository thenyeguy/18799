#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gaussian.h"


single_gaussian_params* compute_single_gaussian_params(feature_vectors* fs)
{
    //Allocate the gaussian params
    single_gaussian_params* ps = malloc(sizeof(single_gaussian_params));

    //Zero the gaussian params
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++){
        ps->means.values[j] =0; 
        ps->means.deltas[j] =0; 
        ps->means.doubles[j] =0; 
        ps->covariances.values[j] =0;
        ps->covariances.deltas[j] =0;
        ps->covariances.doubles[j] =0;
    } 

    //Calculate the model vector
    for(int i = 0; i < fs->num_vectors; i++)
    {
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
        {	
            ps->means.values[j] += (fs->features[i].values[j] / fs->num_vectors);
            ps->means.deltas[j] += (fs->features[i].deltas[j] / fs->num_vectors);
            ps->means.doubles[j] += (fs->features[i].doubles[j] / fs->num_vectors);
        }
    }


    //Calculate the diagonal covariances vector
    for(int i = 0; i < fs->num_vectors; i++)
    {
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
        {
            ps->covariances.values[j] +=
                pow(fs->features[i].values[j]-ps->means.values[j],2);
            ps->covariances.deltas[j] +=
                pow(fs->features[i].deltas[j]-ps->means.deltas[j],2);
            ps->covariances.doubles[j] +=
                pow(fs->features[i].doubles[j]-ps->means.doubles[j],2);
        }
    }
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        ps->covariances.values[j] /= fs->num_vectors;
        ps->covariances.deltas[j] /= fs->num_vectors;
        ps->covariances.doubles[j] /= fs->num_vectors;
    }

    return ps;
}


double single_gaussian_log_pdf(single_gaussian_params* ps, feature* test)
{
    double result = 0;
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
    {
        result += 0.5 * log(2*M_PI * ps->covariances.values[i]+ DELTA);
        result += 0.5 * log(2*M_PI * ps->covariances.deltas[i] + DELTA);
        result += 0.5 * log(2*M_PI * ps->covariances.doubles[i] + DELTA);

        result += 0.5 * pow(ps->means.values[i] - test->values[i], 2)
            / (ps->covariances.values[i] + DELTA);
        result += 0.5 * pow(ps->means.deltas[i] - test->deltas[i], 2)
            / (ps->covariances.deltas[i] + DELTA);
        result += 0.5 * pow(ps->means.doubles[i] - test->doubles[i], 2)
            / (ps->covariances.doubles[i] + DELTA);
    }

    return -1.0*result;
}


void print_single_gaussian_params(single_gaussian_params* ps)
{
    printf("Gaussian mean:\n");
    print_feature(&ps->means);
    printf("Gaussian variances:\n");
    print_feature(&ps->covariances);
}


void free_single_gaussian_params(single_gaussian_params* ps)
{
    free(ps);
}
