#include <stdlib.h>
#include <stdio.h>
#include <word_levenshtein_utils.h>


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage: ./spellcheck [word to check]\n");
        exit(1);
    }

    char* word_one = argv[1]; 
    char** dictionary = word_get_dictionary("text/dictionary.txt", DICTIONARY_LENGTH);
    int n = 10;
    word_and_score* best_n_words = word_get_best_n_words(word_one,dictionary,
                                       DICTIONARY_LENGTH,n);
    word_print_n_best_words(best_n_words,n);
    word_free_dictionary(dictionary,DICTIONARY_LENGTH);
    return 0;
}
