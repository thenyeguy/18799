#include "trellis_utils.h"
#include <strings.h>


//Lengths of the two dictionary files
#define SMALLDICT_LENGTH 6618
#define DICTIONARY_LENGTH 235886


typedef struct{
	char* word;
	int score;
} word_and_score;


/* get_dictionary - reads a dictionary file into an array of words
 */
char** get_dictionary(char* dictionary_filepath, int dict_length);


/* get_best_n_words - given a word, dictionary, and the number of words to
 *                    return, computes the levenshtein distance between the
 *                    given word and every word in the dictionary and returns
 *                    the lowest n results
 */
word_and_score* get_best_n_words(char* word_one, char** dictionary, 
                                 int dictionary_length, int n);


/* get_levenshtein_distance - given two words, computes their levenshtein
 *                             distance using a search trellis
 */
int get_levenshtein_distance(char* word_one,char* word_two);


/* add_null_prefix - helper function for levensthein - takes a word and adds a
 *                   dummy character '*' add the beginning of the word
 */
char* add_null_prefix(char* word);


/* print_n_best_words - takes in a list of best words and its length and prints
 *                      out each element on its own line
 */
void print_n_best_words(word_and_score* n_best_words, int n);


/* free_dictionary - given a dictionary and its size, frees the allocated memory
 */
void free_dictionary(char** dictionary, int dictionary_size);
