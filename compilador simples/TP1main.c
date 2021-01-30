#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Declaração das funções
void aritmeticPrint(ExprArit* aritm, int tabs);
void booleanPrint(ExprBool* boolean, int tabs);
void comandPrint(Comand* cmd, int tabs);
void listaCmdPrint(Cmd_List* cmdlist, int tabs);

// Imprimir Espaços
void printSpaces(int n) {
  for(int i = 0; i < n; i++)
    printf(" ");
}

// Imprimir as Expressões Aritméticas
void aritmeticPrint(ExprArit* aritm, int tabs){
  //print das expressoes aritmeticas
  if(aritm != 0) {
    switch (aritm -> kind) {
      case E_INTEGER:
      printSpaces(tabs);
      printf("%d\n", aritm -> attr.value);
      break;
      case E_VARIABLE:
      printSpaces(tabs);
      printf("%s\n", strdup(aritm -> attr.var));
      break;
      case E_OPERATION:
      switch (aritm -> attr.op.operator) {
        case PLUS:
        printSpaces(tabs);
        printf("+\n");
        break;
        case SUB:
        printSpaces(tabs);
        printf("-\n");
        break;
        case MULT:
        printSpaces(tabs);
        printf("*\n");
        break;
        case DIV:
        printSpaces(tabs);
        printf("/\n");
        break;
        case MOD:
        printSpaces(tabs);
        printf("%%\n");
        break;
        default:
        yyerror("Unknown operator!\n");
      }
      aritmeticPrint(aritm -> attr.op.left, tabs+2);
      aritmeticPrint(aritm -> attr.op.right, tabs+2);
      break;
      case E_PARENTESIS:
      printSpaces(tabs);
      printf("(\n");
      aritmeticPrint(aritm -> attr.insideP.insideParentesis, tabs+2);
      printSpaces(tabs);
      printf(")\n");
      break;
      default:
      break;
    }
  }
}

// Imprimir as Expressões Booleanas
void booleanPrint(ExprBool* boolean, int tabs){
  //print das expressoes booleanas
  if(boolean != 0) {
    switch (boolean -> kind) {
    case E_BOOL:
    printSpaces(tabs);
    printf("%d\n", boolean -> attr.value);
    break;
    case E_OP:
    switch (boolean -> attr.opBool.operator) {
      case GT:
      printSpaces(tabs);
      printf(">\n");
      break;
      case LT:
      printSpaces(tabs);
      printf("<\n");
      break;
      case GTEQ:
      printSpaces(tabs);
      printf(">=\n");
      break;
      case LTEQ:
      printSpaces(tabs);
      printf("<=\n");
      break;
      case EQUAL:
      printSpaces(tabs);
      printf("==\n");
      break;
      case NOT_EQUAL:
      printSpaces(tabs);
      printf("!=\n");
      break;
      default:
      yyerror("Unknown operator!\n");
    }
    aritmeticPrint(boolean -> attr.opBool.left, tabs+2);
    aritmeticPrint(boolean -> attr.opBool.right, tabs+2);
    break;
    case E_LOGICA:
    switch (boolean -> attr.logBool.p_logica) {
      case AND:
      printSpaces(tabs);
      printf("&&\n");
      break;
      case OR:
      printSpaces(tabs);
      printf("||\n");
      break;
      case SS:
      printSpaces(tabs);
      printf("!\n");
      break;
      default:
      yyerror("Unknown operator!\n");
    }
    booleanPrint(boolean -> attr.logBool.leftLOG, tabs+2);
    booleanPrint(boolean -> attr.logBool.rightLOG, tabs+2);
    break;
    case M_true:
      printSpaces(tabs);
      printf("true\n");
      break;
    case M_false:
      printSpaces(tabs);
      printf("false\n");
    break;
    default:
    break;
  }
  }
}

// Imprimir os Comandos
void comandPrint(Comand* cmd, int tabs){
  //print dos comandos
  if(cmd != 0) {
    switch (cmd -> kind) {
    case IF:
    printSpaces(tabs);
    printf("if\n");
    booleanPrint(cmd -> attr.cmdIF.condicao_IF, tabs+2);
    listaCmdPrint(cmd -> attr.cmdIF.listC_IF, tabs+2);
    break;
    case IF_THEN_ELSE:
    printSpaces(tabs);
    printf("if\n");
    booleanPrint(cmd -> attr. cmdIF_THEN_ELSE.condicao_IF_THEN, tabs+2);
    listaCmdPrint(cmd -> attr. cmdIF_THEN_ELSE.listC_IF_THEN, tabs+2);
    printSpaces(tabs);
    printf("else\n");
    listaCmdPrint(cmd -> attr. cmdIF_THEN_ELSE.listC_ELSE, tabs+2);
    break;
    case INT2:
    printSpaces(tabs);
    printf("int\n");
    printSpaces(tabs+2);
    printf("=\n");
    printSpaces(tabs+4);
    printf("%s\n", cmd -> attr.cmdINT2.var);
    aritmeticPrint(cmd -> attr.cmdINT2.var_data, tabs+4);
    break;
    case WHILE:
    printSpaces(tabs);
    printf("while \n");
    booleanPrint(cmd -> attr.cmdWHILE.condicao_WHILE, tabs+2);
    printSpaces(tabs+2);
    printf("block{\n");
    listaCmdPrint(cmd -> attr.cmdWHILE.listC_WHILE, tabs+2);
    printSpaces(tabs+2);
    printf("} block\n");
    break;
    case ATRIB: 
    printSpaces(tabs);
    printf("=\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdATRIB.var);
    aritmeticPrint(cmd -> attr.cmdATRIB.var_data, tabs+4);
    break;
    case ATRIB2: 
    printSpaces(tabs);
    printf("int\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdATRIB2.var);
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdATRIB2.var2);
    break;
    case ATRIB3: 
    printSpaces(tabs-2);
    printf("int\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdATRIB3.var);
    break;
    case PRINT_INT1:
    printSpaces(tabs);
    printf("print_int()\n");
    printSpaces(tabs+2);
    printf("%s\n", strdup(cmd -> attr.cmdPRINT_INT1.str));
    break;
    case PRINT_INT2:
    printSpaces(tabs);
    printf("print_int()\n");
    printSpaces(tabs+2);
    printf("%s\n", strdup(cmd -> attr.cmdPRINT_INT2.str));
    printSpaces(tabs+2);
    printf("%s\n", strdup(cmd -> attr.cmdPRINT_INT2.var));
    break;
    case SCAN_INT:
    printSpaces(tabs);
    printf("scan_int()\n");
    printSpaces(tabs+2);
    printf("%s\n", strdup(cmd -> attr.cmdSCAN_INT.str));
    break;
    case FUNC_MAIN:
    printSpaces(tabs-4);
    printf("int main() block{\n");
    listaCmdPrint(cmd->attr.cmdMain.list, tabs);
    printf("\n");
    printSpaces(tabs-2);
    printf("} block\n");
    break;
    case FUNC_BOOL:
    printSpaces(tabs-4);
    printf("bool %s(\n", cmd->attr.cmdBool.var);
    printSpaces(tabs);
    printf("int\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdBool.var2);
    printSpaces(tabs-2);
    printf(") block{\n");
    listaCmdPrint(cmd->attr.cmdBool.list, tabs);
    printf("\n");
    printSpaces(tabs-2);
    printf("} block\n");
    break;
    case FUNC_INT:
    printSpaces(tabs-4);
    printf("int %s(\n", cmd->attr.cmdInt.var);
    printSpaces(tabs);
    printf("int\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdInt.var2);
    printSpaces(tabs-2);
    printf(")block {\n");
    listaCmdPrint(cmd->attr.cmdInt.list, tabs);
    printf("\n");
    printSpaces(tabs-2);
    printf("} block\n");
    break;
    case RETURN_TRUE:
    printSpaces(tabs);
   printf("return \n");
    printSpaces(tabs+2);
    printf("true\n");
    break;
    case RETURN_FALSE:
    printSpaces(tabs);
    printf("return \n");
    printSpaces(tabs+2);
    printf("false\n");
    break;
    case RETURN_INT:
    printSpaces(tabs);
    printf("return \n");
    aritmeticPrint(cmd -> attr.cmdReturnInt.var_data, tabs+4);
    break;
    case PRINT_STR:
    printSpaces(tabs);
    printf("print_str \n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdPrintStr.var);
    break;
    case CONTINUE:
    printSpaces(tabs);
    printf("continue \n");
    break;
    case BREAK:
    printSpaces(tabs);
    printf("break \n");
    break;
    case FOR:
    printSpaces(tabs);
    printf("for\n");
    printSpaces(tabs+2);
    printf("int \n");
    printSpaces(tabs+4);
    printf("%s\n", cmd -> attr.cmdFor.var);
    aritmeticPrint(cmd -> attr.cmdFor.expr, tabs+4);
    booleanPrint(cmd -> attr. cmdFor.left, tabs+4);
    printSpaces(tabs+4);
    printf("%s\n", cmd -> attr.cmdFor.var2);
    switch (cmd -> attr.cmdFor.operator) {
    case PLUS:
    printSpaces(tabs+4);
    printf("++ \n");
    break;
    case SUB:
    printSpaces(tabs+4);
    printf("-- \n");
    break;
    default:
    yyerror("Unknown operator!\n");
    }
    printSpaces(tabs+2);
    printf("block{\n");
    listaCmdPrint(cmd->attr.cmdFor.cmdList, tabs+4);
    printSpaces(tabs+2);
    printf("}block\n");
    printSpaces(tabs-4);
    break;
    case FOR1:
    printSpaces(tabs);
    printf("for\n");
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdFor.var);
    aritmeticPrint(cmd -> attr.cmdFor.expr, tabs+2);
    booleanPrint(cmd -> attr. cmdFor.left, tabs+2);
    printSpaces(tabs+2);
    printf("%s\n", cmd -> attr.cmdFor.var2);
    switch (cmd -> attr.cmdFor.operator) {
    case PLUS:
    printSpaces(tabs+2);
    printf("++ \n");
    break;
    case SUB:
    printSpaces(tabs+2);
    printf("-- \n");
    break;
    default:
    yyerror("Unknown operator!\n");
    }
    printSpaces(tabs+2);
    printf("block{\n");
    listaCmdPrint(cmd->attr.cmdFor.cmdList, tabs+4);
    printSpaces(tabs+2);
    printf("}block\n");
    printSpaces(tabs-4);
    break;

    default:
    yyerror("Unknown operator!\n");
  }
  }
}

// Imprimir a Lista de Comandos
void listaCmdPrint(Cmd_List* cmdlist, int tabs){
  //percorrer lista e chamar funcao print comando
  if(cmdlist != NULL) {
    comandPrint(cmdlist -> cmd, tabs);
    listaCmdPrint(cmdlist -> next, tabs);
  }
}

// Função Main
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
    int tabs = 2;
    printSpaces(tabs);
   // printf("int main()\n");
    listaCmdPrint(root, tabs+2);
  }
  return 0;
}
