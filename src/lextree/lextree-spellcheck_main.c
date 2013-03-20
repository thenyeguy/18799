#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"
#include "lextree_spellcheck.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./lextree-spellcheck [word]\n");
        printf("    Returns up to 10 close matches to a word\n\n");
        exit(1);
    }

    //Read dictionary
	// TODO: make this a macro
    printf("Building lextree...\n");
	int dict_size = 6250;
	lextree* lex = build_lextree_from_file("text/dict-assn4.txt", dict_size);
    printf("Lextree built.\n\n");

    printf("Spellchecking \"%s\"...\n", argv[1]);
    int n = 10;
    lextree_scored_word** results =
        lextree_closest_n_words(lex, argv[1], n, false);
    lextree_print_n_best(results, 10);

    return 0;
}


