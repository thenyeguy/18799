#ifndef LEXTREE_SPELLCHECK_H
#define LEXTREE_SPELLCHECK_H

#include <stdbool.h>
#include "lextree.h"


/* This spellcheck uses a relative beam pruning. This constant defines the
 * pruning threshold used.
 */
#define LEXTREE_CLOSEST_PRUNING_THRESHOLD 3
 
#define LT_STRING_LENGTH 4096
#define MININT (1<<31)
#define MAXINT (~MININT)

/* lextree_scored_word is a pair that contains a word and its edit distance.
 *                     CURRENTLY ASSUMES WORDS AT MOST 64 CHARS LONG.
 */
typedef struct {
    char string[LT_STRING_LENGTH];
    int score;
} lextree_scored_string;


/* lextree_closest_n_words - given a filled lextree, a test word, and the number
 *                        of matches to return, returns the n closest wors to
 *                        our input word and their edit distance. If segment is
 *                        true, then we search for multiple words and insert
 *                        spaces
 */
lextree_scored_string** lextree_closest_n(lextree* lex, char* string, int n,
                                          bool segment);


/* lextree_add_to_result - helper function for lextree_closest_n_words.
 *                         Attempts to add a scored word to our list of best
 *                         scored words. Does nothing if the list is full and
 *                         every word so far is better. 
 */
void lextree_add_to_result(lextree_scored_string** strings, int n,
                           char* string, int score);


/* already_in_nbest - helper function for lextree_add_to_result.  looks in 
 *					  the list "words" for "word". returns the word's index 
 *					  in the list if it is there, and -1 if it is not.
 */

 int already_in_nbest(lextree_scored_string** strings, int n, char* string);


/* lextree_print_n_best - given a list of lextree_scored_words and its length,
 *                        prints out a list of the results
 */
void lextree_print_n_best(lextree_scored_string** strings, int n);


/* lextree_free_n_best - given a list of lextree_scored_words and its length,
 *                       frees the resulting arrays
 */
void lextree_free_n_best(lextree_scored_string** strings, int n);

#endif
