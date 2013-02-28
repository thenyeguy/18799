#include <feature.h>


/* Struct to store single gaussian parameters. Contains the mean model vector,
 * and the diagonals of the covariance matrix of a multivariate gaussian
 * distribution.
 */
typedef struct {
    feature means;
    feature covariances;
} single_gaussian_params;


/* compute_single_gaussian_params -
 *     Given a set of feature vectors fs, computes the mean and the square of
 *     the covariances of a gaussian distribution that best fits that data */
single_gaussian_params* compute_single_gaussian_params(feature_vectors* fs);


/* single_gaussian_pdf - given the parameters to a single gaussian distribution
 *                       and a test point, returns the log probability of a 
 *                       single gaussian evaluated at the test point */
double single_gaussian_log_pdf(single_gaussian_params* ps, feature* test);


/* print_single_gaussian_params - prints params in human readable form
 */
void print_single_gaussian_params(single_gaussian_params* ps);


/* free_single_gaussian_params - frees the single_gaussian_params struct :)
 */
void free_single_gaussian_params(single_gaussian_params* ps);
