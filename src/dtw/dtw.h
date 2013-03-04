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

/*
void print_string_array(char ** story);
*/
char** get_null_prefix_dictionary_dtw(char* dictionary_filepath, 
	int dict_length);
//int get_string_array_length(char ** string_array);

void show_best_dtw_path(dtw_t trellis,int one_length,int two_length);


typedef struct{
	feature* recorded_features;
	int score;
    dtw_t* trellis;
} features_and_score;


/* get_features_from_file - reads a cepstrum analysis file into an array of 
	features
 */
feature_vectors* get_features_from_file(char* recording_name, int num_filters);


/* get_best_n_words - given a word, dictionary, and the number of words to
 *                    return, computes the levenshtein distance between the
 *                    given word and every word in the dictionary and returns
 *                    the lowest n results
 */
features_and_score* get_best_n_words_dtw(char* word_one, char** dictionary, 
                                 int dictionary_length, int n);


/* get_dtw_distance - given two words, computes their levenshtein
 *                             distance using a search trellis
 */
dtw_t* get_dtw_distance(feature_vectors word_one, feature_vectors 
	word_two);


/* add_null_prefix - helper function dtw - takes a feature vector sequence and
	adds a null vector to the beginning of the sequence
 */
feature_vectors* add_null_prefix_dtw(feature_vectors data);


/* print_n_best_words - takes in a list of best words and its length and prints
 *                      out each element on its own line
 */
void print_n_best_words_dtw(features_and_score* n_best_words, int n);


/* free_dictionary - given a dictionary and its size, frees the allocated 
	memory
 */
void free_dictionary_dtw(feature* dictionary, int dictionary_size);


/* score - compute the score between two vectors
*/
double score(feature* test, feature* template, int test_index,
	int template_index, dtw_trellis_dir direction);

#endif
