#include <stdlib.h>
#include <stdio.h>
#include "lextree.h"
#include "lextree_spellcheck.h"


int main()
{
    int dict_size = 6250;

    printf("Building lextree...\n");
    lextree* lex = build_lextree_from_file("text/dict-assn4.txt", dict_size);
    printf("Lextree built!\n\n");

    /*
    print_lextree_contents(lex);
    printf("Word yuck in dictionary: %d\n", is_word_in_lextree(lex,"yuck"));
    printf("Word zzz in dictionary: %d\n", is_word_in_lextree(lex,"zzz"));
    printf("Word zel in dictionary: %d\n", is_word_in_lextree(lex,"zel"));
    printf("Word zele in dictionary: %d\n", is_word_in_lextree(lex,"zele"));
    printf("Word murder in dictionary: %d\n", is_word_in_lextree(lex,"murder"));
    */

    //Try spellcheck
    int n = 10;
    lextree_scored_word** res = lextree_closest_n_words(lex, "monkeys", n, false);
    lextree_print_n_best(res, n);

    return 0;
}
