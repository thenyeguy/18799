#ifndef LEXTREE_H
#define LEXTREE_H

#include <stdbool.h>


/* lextree_node represents one node in a lextree. It contains the char at
 *              that node, a bool indicating whether the characeters we have
 *              seen thus far represent a full word, and a list of its children
 *              for each letter that could follow it. Currently designed to
 *              accept only lowercase letters for words.
 */
typedef struct _lextree_node {
    char c;
    bool is_full_word;

    struct _lextree_node* children[26];
} lextree_node;


/* lextree is mostly a wrapper for the entire lextree_node structure. It
 *         contains a count of the dictionary size used to seed the lextree,
 *         the number of nodes deep our lextree goes before terminating, and
 *         a pointer to the dummy node '*' that is the head of our lextree.
 *
 *         The depth does NOT count the dummy node and any null terminator, so
 *         if the longest word in our tree is "feisty" then the depth is 6.
 */
typedef struct {
    int size;
    int depth;
    lextree_node* head;
} lextree;


/* build_lextree_from_file - reads a dictionary file with one word per line,
 *                           strips any non-lowercase letters, and returns a
 *                           lextree from this.
 */
lextree* build_lextree_from_file(char* dictionary_path, int dict_size);


/* copy_word_only_lowercase - helper for build_lextree_from_file. Copies a word
 *                            into a destination array, with only the lowercase
 *                            letters.
 */
void copy_word_only_lowercase(char* dest, char* source);


/* build_lextree - given an array of words in our dictionary and the size of
 *                 that dictionary, adds the dictionary into a new lextree.
 *                 Assumes the dictionary contains only lowercase letters.
 */
lextree* build_lextree(char** dictionary, int dict_size);


/* add_word_to_lextree - helper for build_lextree. Given a word and a partial
 *                       lextree, adds the word into that lextree. Allocates
 *                       any nodes required along the way.
 */
void add_word_to_lextree(lextree_node* head, char* word);


/* new_lextree_node - helper for build_lextree/add_word_to_lextree. Allocates
 *                    a new node and populates the children with null
 */
lextree_node* new_lextree_node(char c);


/* is_word_in_lextree - traverses the given lextree and returns true if the word
 *                      is stored in the lextree
 */
bool is_word_in_lextree(lextree* lex, char* word);


/* print_lextree_node - prints out a human-readable representation of a single
 *                      node of the lextree
 */
void print_lextree_node(lextree_node* node);


/* print_lextree_contents - traverses a lextree and prints out a list of every
 *                          word in our lextree. The helper function handles the
 *                          recursion. Traverses in pre-order.
 */
void print_lextree_contents(lextree* lex);
void print_lextree_contents_helper(lextree_node* node, char* buffer, int i);


/* free_lextree_node - mostly a helper for free_lextree. Given a node,
 *                     RECURSIVELY frees that node and all its children.
 */
void free_lextree_node(lextree_node* node);


/* free_lextree - frees a lextree and all the nodes it contains
 */
void free_lextree(lextree* lex);

#endif
