#include <stdlib.h>
#include <stdio.h>
#include "../dtw/feature.h"
#include "grammar.h"
#include "viterbi.h"

int main(int argc, char* argv[])
{
	if (argc!=2) {
		printf("usage: grammartest [grammar file] [analysis file]\n")
	}
    //grammar* g = build_grammar("grammar/simple.txt");
    grammar* g = build_grammar(argv[1]);
    //grammar* g = build_grammar("grammar/double loop 1-9.txt");
    //grammar* g = build_grammar("grammar/triple loop 1-9.txt");
    //grammar* g = build_grammar("grammar/quad loop 1-9.txt");
    print_grammar(g);

    //feature_vectors* test = features_from_file("analysis/369-40.out");    
    feature_vectors* test = features_from_file(argv[2]); 
/*
    char* result = viterbi_search(g, test, .1);

    printf("\n\nResult: %s\n", result);
*/
/*
    char* result2 = viterbi_search2(g, test, .1);
    printf("\n\nResult2: %s\n",result2);
*/
    char* result3 = viterbi_search3(g, test, .1);
    printf("\n\nResult3: %s\n",result3);


    return 0;
}
