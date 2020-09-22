%{
#include "tokens.h"
#include <iostream>
using namespace std;
%}

%%
[A-Z]           return VARIABLE;
"->"            return ARROW;
[a-zA-Z+x]+       return PRODUCTIONS;
"|"             return PIPE;
"\n"            return NEW_LINE;
[ \t]         ;
.               cout << "unexpected character\n";
%%

int yywrap()
{
    return 1;
}