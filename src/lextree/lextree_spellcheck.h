#ifndef LEXTREE_SPELLCHECK_H
#define LEXTREE_SPELLCHECK_H

#include "lextree.h"


/* This spellcheck uses a relative beam pruning. This constant defines the
 * pruning threshold used.
 */
#define LEXTREE_CLOSEST_PRUNING_THRESHOLD 2
#define PRUNE 1
#define ABSOLUTE_THRESHOLD 2
#define ABSOLUTE 1
#define LT_WORD_LENGTH 64
#define MININT (1<<31)
#define MAXINT (~MININT)

/* lextree_scored_word is a pair that contains a word and its edit distance.
 *                     CURRENTLY ASSUMES WORDS AT MOST 64 CHARS LONG.
 */
typedef struct {
    char word[LT_WORD_LENGTH];
    int score;
} lextree_scored_word;


/* pq_node is a container struct used by the spellcheck search. It contains
 *         the nessecary state information to determine our location in the
 *         word we are checking, and in the lextree.
 *
 *         Also stores the number of kind of edits seen in that word up to this
 *         point, and the word we have built so far.
 */
typedef struct {
    int cur_word_i;
    int cur_depth;

    int insertions;
    int deletions;
    int substitutions;
    int score;

    lextree_node* cur_node;
    char word_so_far[LT_WORD_LENGTH];
} pq_node;
    

/* lextree_closest_n_words - given a filled lextree, a test word, and the number
 *                        of matches to return, returns the n closest wors to
 *                        our input word and their edit distance
 */
lextree_scored_word** lextree_closest_n_words(lextree* lex, char* word, int n);


/* lextree_add_to_result - helper function for lextree_closest_n_words.
 *                         Attempts to add a scored word to our list of best
 *                         scored words. Does nothing if the list is full and
 *                         every word so far is better. 
 */
void lextree_add_to_result(lextree_scored_word** words, int n,
                           char* word, int score);


/* already_in_nbest - helper function for lextree_add_to_result.  looks in 
 *					  the list "words" for "word". returns the word's index 
 *					  in the list if it is there, and -1 if it is not.
 */

 int already_in_nbest(lextree_scored_word** words, int n, char* word);


/* lextree_print_n_best - given a list of lextree_scored_words and its length,
 *                        prints out a list of the results
 */
void lextree_print_n_best(lextree_scored_word** words, int n);


/* lextree_free_n_best - given a list of lextree_scored_words and its length,
 *                       frees the resulting arrays
 */
void lextree_free_n_best(lextree_scored_word** words, int n);

#endif
