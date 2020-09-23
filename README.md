# Group 23 Compiler Design Assignment

Feed in any CFG and check whether it is LL1 or not.

Epsilon Symbol ~

Dollar Symbol =

```
lex tokens.l; g++ lex.yy.c code.cpp -std=c++11 -o ll1_parser;
./ll1_parser < input1.txt
./ll1_parser < input2.txt
./ll1_parser < input3.txt
```
