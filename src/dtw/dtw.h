#ifndef DTW_H 

#define DTW_H

#include "dtw_trellis.h"
#include <strings.h>
#include <math.h>

//Lengths of the two dictionary files
#define SMALLDICT_LENGTH 6618
#define DICTIONARY_LENGTH 235886
#define X(x) (x)
#define Y(j,len) (((len)-(j))-1)
#define I(i) (i)
#define J(y,len) (((len)-y)-1)


void print_string_array(char ** story);
char** get_null_prefix_dictionary(char* dictionary_filepath, int dict_length);
int get_string_array_length(char ** string_array);
void show_best_path(int ** trellis,int one_length,int two_length);


typedef struct{
	feature[] recorded_features;
	int score;
    search_trellis* trellis;
} features_and_score;


/* get_dictionary - reads a dictionary file into an array of features
 */
feature* get_dictionary(char* dictionary_filepath, int dict_length);


/* get_best_n_words - given a word, dictionary, and the number of words to
 *                    return, computes the levenshtein distance between the
 *                    given word and every word in the dictionary and returns
 *                    the lowest n results
 */
word_and_score* get_best_n_words(char* word_one, char** dictionary, 
                                 int dictionary_length, int n);


/* get_dtw_distance - given two words, computes their levenshtein
 *                             distance using a search trellis
 */
dtw_t* get_levenshtein_distance(feature_vectors word_one, feature_vectors 
	word_two);


/* add_null_prefix - helper function dtw - takes a feature vector sequence and
	adds a null vector to the beginning of the sequence
 */
feature_vectors add_null_prefix(feature_vectors data);


/* print_n_best_words - takes in a list of best words and its length and prints
 *                      out each element on its own line
 */
void print_n_best_words(word_and_score* n_best_words, int n);


/* free_dictionary - given a dictionary and its size, frees the allocated memory
 */
void free_dictionary(feature* dictionary, int dictionary_size);


/* 	feature_distance - compute the distance between two feature vectors
*/
double feature_distance(feature v1, feature v2);

#endif
