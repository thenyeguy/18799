#include <stdlib.h>
#include <stdio.h>
#include "../dtw/feature.h"
#include "grammar.h"
#include "viterbi.h"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("Usage: ./viterbi [grammar.txt] [recording.out]\n");
        printf("    Given a grammar file and a cepstrum analysis file,\n");
        printf("    Performs a viterbi continuous recognition of the");
        printf("test utterance\n");
        printf("    using the specified grammar as its structure.\n\n");
        exit(1);
    }

    //Parse args
    char* grammar_file = argv[1]; 
    char* test_file = argv[2];

    //Read in the files
    printf("Building grammar from %s...\n",grammar_file);
    grammar* g = build_grammar(grammar_file);
    feature_vectors* test = features_from_file(test_file); 

    //Run the test and display result
    printf("Testing against %s...\n",test_file);
    char* result = viterbi_search(g, test, 0.1);
    printf("\nResult: %s\n", result);

    return 0;
}
