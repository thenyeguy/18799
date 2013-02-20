

#include <stdlib.h>
#include <stdio.h>
#include <levenshtein_utils.h>


int main(int argc, char** argv)
{

    char** story_one = get_null_prefix_dictionary("text/prepped-story_prepped.txt", DICTIONARY_LENGTH);    
    char** story_two = get_null_prefix_dictionary("text/prepped-story_prepped2.txt", DICTIONARY_LENGTH);
/*
char** story_one = get_null_prefix_dictionary("text/dictionary.txt", DICTIONARY_LENGTH);    
    char** story_two = get_null_prefix_dictionary("text/smalldict.txt", DICTIONARY_LENGTH);
*/
    int distance = compute_levenshtein(story_one,story_two);	

    printf("Distance: %d\n",distance);

    free_dictionary(story_one,DICTIONARY_LENGTH);
    free_dictionary(story_two,DICTIONARY_LENGTH);
    return 0;
}


