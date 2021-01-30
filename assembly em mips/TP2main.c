#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TP2.h"
#include "parser.h"

int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  }
  if (yyparse() == 0) {

    InstrList* list = compileCmdList(root);
    // codigo intermedio funciona direito
    printInstrList(list);  
    // codigo só funciona em casos simples
    printMipsList(list);
  }

  return 0;
}
