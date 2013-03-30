#include "grammar.h"

int main()
{
    grammar* g = build_grammar("text/bnf.txt");
    print_grammar(g);

    return 0;
}
