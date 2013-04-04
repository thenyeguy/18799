#include <stdlib.h>
#include <stdio.h>
#include "../dtw/feature.h"
#include "grammar.h"
#include "viterbi.h"

int main()
{
    grammar* g = build_grammar("grammar/simplesilence.txt");
    print_grammar(g);

    feature_vectors* test = features_from_file("analysis/onetwo3-40.out"); 
    char* result = viterbi_search(g, test, 0.1);

    printf("\n\nResult: %s\n", result);

    return 0;
}
