#include <stdlib.h>
#include <stdio.h>
#include "../dtw/feature.h"
#include "grammar.h"
#include "viterbi.h"

int main()
{
    grammar* g = build_grammar("grammar/simple.txt");
    print_grammar(g);

    feature_vectors* test = features_from_file("analysis/onetwo-40.out"); 
    char** results = viterbi_search(g, test, true, .008, 4);

    print_viterbi_results(results,4);    

    return 0;
}
