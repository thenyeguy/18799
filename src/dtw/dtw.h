#ifndef DTW_H 

#define DTW_H

#include "dtw_trellis.h"
#include "feature.h"
#include <strings.h>
#include <math.h>

//Lengths of the two dictionary files
#define SMALLDICT_LENGTH 6618
#define DICTIONARY_LENGTH 235886
#define FILENAME_BUF_LENGTH 256
#define LINE_BUF_LENGTH 512
#define MAX_FEATURE_VECTORS 1024
#define X(x) (x)
#define Y(j,len) (((len)-(j))-1)
#define I(i) (i)
#define J(y,len) (((len)-y)-1)
#define MIN(a,b,c) ((c<((a<b)?a:b))?c:((a<b)?a:b))
#define DEFAULT_PRUNING_THRESHOLD MAXINT


/* get_null_prefix_dictionary_dtw: read a list of feature_vectors from a source
	directory, add null prefixes for processing */
feature_vectors** get_null_prefix_dictionary_dtw();


/* show_best_dtw_path - TODO: do we need this?  
	Insertion / deletion / substitution count may no longer be relevant for 
	dtw
*/
void show_best_dtw_path(dtw_t trellis,int one_length,int two_length);


typedef struct {
	feature_vectors* features;
	double score;
    dtw_t* trellis;
} features_and_score;


/* get_features_from_file - reads a cepstrum analysis file into an array of 
	features
 */
feature_vectors* get_features_from_file(char* recording_name, int num_filters);


/* get_best_n_words - given an input vector, dictionary, and the number of
	words to return, scores the input vector against every vector in the 
	dictionary and returns the n words in the dictionary that match best
 */
dtw_t** get_best_n_words_dtw(feature_vectors* input, 
								feature_vectors** dictionary, int dict_length,
                                int n);


/* get_dtw_distance - scores a dtw trellis, returns a pointer to that trellis
	(which contains the score)
 	FIXME: unused for now, because it messes up pruning to score trellises one
			at a time.
 */
dtw_t* get_dtw_distance(feature_vectors* data1, feature_vectors* 
	data2);


/* add_null_prefix - helper function dtw - takes a feature vector sequence and
	adds a null vector to the beginning of the sequence
 
 	FIXME: unused for now, relying on other factors not to need a null prefix
 */
feature_vectors* add_null_prefix_dtw(feature_vectors* data);


/* print_n_best_words - takes in a list of best words and its length and prints
 *                      out each element on its own line. Assumes list is 
 						ordered by score.
 */
void print_n_best_words_dtw(dtw_t** n_best_words, int n);


/* get_trellis_list - given a dictionary of feature vectors and an input
	feature set, build the trellis 
*/
dtw_t** get_trellis_list (feature_vectors* input_data, 
						feature_vectors** dictionary, int dict_length);

/* get_best_n_from_trellis - given the complete trellis structure built
	from an input feature set, score it and return the best n matches
*/
dtw_t** get_best_n_from_trellis(dtw_t** trellis, int trellis_length, int n);


/* insert - helper function for get_best_n, inserts the input into list in 
	order if it is greater than one of length entries already in list.  length
	refers to the maximum length of the list, not necessarily the current
	number of non-null entries in list.
*/
bool insert(dtw_t* input, dtw_t** list, int length);


/* free_dictionary - given a dictionary and its size, frees the allocated 
	memory
 */
void free_dictionary_dtw(feature_vectors* dictionary, int dictionary_size);


/* score - compute the score between two vectors at the given indices
*/
double score(void* test, void* template, int test_index,
	int template_index, dtw_trellis_dir direction);

/* dtw_usage - usage printout for dtw_main
*/
void dtw_usage();


#endif
