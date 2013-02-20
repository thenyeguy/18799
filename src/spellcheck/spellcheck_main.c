#include <stdlib.h>
#include <stdio.h>
#include <levenshtein_utils.h>


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage: ./spellcheck [word to check]\n");
        exit(1);
    }

    char* word_one = argv[1]; 
    char** dictionary = get_dictionary("text/dictionary.txt", DICTIONARY_LENGTH);
    int n = 10;
    word_and_score* best_n_words = get_best_n_words(word_one,dictionary,
                                       DICTIONARY_LENGTH,n);
    print_n_best_words(best_n_words,n);
    free_dictionary(dictionary,DICTIONARY_LENGTH);
    return 0;
}
