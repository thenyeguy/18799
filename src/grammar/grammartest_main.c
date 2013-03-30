#include <stdlib.h>
#include <stdio.h>
#include "../dtw/feature.h"
#include "grammar.h"
#include "viterbi.h"

int main()
{
    grammar* g = build_grammar("grammar/simple.txt");
    print_grammar(g);

    feature_vectors* test = features_from_file("analysis/two1-40.out"); 
    char** results = viterbi_search(g, test, true, .1, 2);

    print_viterbi_results(results,2);    

    return 0;
}
