#include "trellis_utils.h"
#include <strings.h>

//#define DICTIONARY_LENGTH 235886
#define DICTIONARY_LENGTH 6618



typedef struct{
	char * word;
	int score;
}word_and_score;


char ** get_dictionary(char * dictionary_filepath);

word_and_score * get_best_n_words(char * word_one,char ** dictionary, int n);

char * add_null_prefix(char * word);

int get_levenshtein_distance(char * word_one,char * word_two);

void print_n_best_words(word_and_score * n_best_words, int n);

void free_dictionary(char ** dictionary,int dictionary_size);

