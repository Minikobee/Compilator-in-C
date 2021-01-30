%token
  INT
  BOOL
  VAR
  STRING_CMD
  PLUS
  PLUSS
  SUB
  SUBB
  DIV
  MULT
  MOD
  IGUAL

  GT
  GTEQ
  LT
  LTEQ
  EQUAL
  NOT_EQUAL
  TRUE
  FALSE
  AND
  OR
  SS
  RETURN

  
  IF_CMD
  ELSE_CMD
  INT_CMD
  WHILE_CMD
  FOR_CMD
  PRINT_INT_CMD
  SCAN_INT_CMD
  PRINT_STR_CMD

  CONTINUE_CMD 
  BREAK_CMD

  MAIN

  CHAVETA1
  CHAVETA2
  PARENTESE1
  PARENTESE2
  PVIRGULA
  VIRGULA
  ASPAS
  PELICAS
  ECOM


// Operator associativity and precedence
%left FALSE TRUE
%left PLUS SUB
%left MULT DIV
%left MOD
%left AND OR 


// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  ExprArit* exprValue;
  char* charValue;
  ExprBool* boolValue;
  Comand* cmdValue;
  Cmd_List* cmdListValue;
}

%type <intValue> INT
%type <exprValue> expr
%type <charValue> VAR
%type <charValue> STRING_CMD
%type <boolValue> boolean
%type <cmdValue> comand
%type <cmdListValue> cmdList

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
 %code requires {
  #include <stdio.h>
  #include <stdlib.h>
  #include "TP1.h"
  #include "TP2.h"

  extern int yylex();
  extern int yyline;
  extern char* yytext;
  extern FILE* yyin;
  extern void yyerror(const char* msg);
  Cmd_List* root;
}


%%
program: cmdList {root = $1;}
  


expr:
  INT {
    $$ = expArit_Integer($1);
  }
  |
  VAR {
    $$ = expArit_Variable($1);
  }
  |
  expr PLUS expr {
    $$ = expArit_Operation(PLUS, $1, $3);
  }
  |
  expr SUB expr {
    $$ = expArit_Operation(SUB, $1, $3);
  }
  |
  expr DIV expr {
    $$ = expArit_Operation(DIV, $1, $3);
  }
  |
  expr MULT expr {
    $$ = expArit_Operation(MULT, $1, $3);
  }
  |
  expr MOD expr {
    $$ = expArit_Operation(MOD, $1, $3);
  }
  |
  PARENTESE1 expr PARENTESE2 {
    $$ = expArit_Parentesis($2);
  }
  ;

boolean:
  expr GT expr {
    $$ = expBool_Operation(GT,$1,$3);
  }
  |
  expr GTEQ expr {
    $$ = expBool_Operation(GTEQ,$1,$3);
  }
  |
  expr LT expr {
    $$ = expBool_Operation(LT,$1,$3);
  }
  |
  expr LTEQ expr {
    $$ = expBool_Operation(LTEQ,$1,$3);
  }
  |
  expr EQUAL expr {
    $$ = expBool_Operation(EQUAL,$1,$3);
  }
  |
  expr NOT_EQUAL expr {
    $$ = expBool_Operation(NOT_EQUAL,$1,$3);
  }
  |
  TRUE {
    $$ = make_true();
  }
  |
  FALSE {
    $$ = make_false();
  }
  | 
  boolean AND boolean{
     $$ = expBool_Logic(AND,$1,$3);
  }
  | 
  boolean OR boolean{
     $$ = expBool_Logic(OR,$1,$3);
  } 
  |
  SS boolean{
      $$ = expBool_Logic(SS,NULL,$2);
  }
  ;

comand:
  IF_CMD PARENTESE1 boolean PARENTESE2  cmdList  {
    $$ = comand_IF(IF_CMD, $3, $5);
  }
  |
  IF_CMD PARENTESE1 boolean PARENTESE2  cmdList  ELSE_CMD  cmdList  {
    $$ = comand_IF_THEN_ELSE(ELSE_CMD, $3, $5, $7);
  }
  |
  INT_CMD VAR IGUAL expr PVIRGULA{
    $$ = comand_INT2(INT_CMD, $2, $4);
  }
  |
  VAR IGUAL expr PVIRGULA{
    $$ = comand_ATRIB($1, $3);
  }
  |
  INT_CMD VAR PVIRGULA{
    $$ = comand_ATRIB3($2);
  }
  |
  WHILE_CMD PARENTESE1 boolean PARENTESE2 CHAVETA1 cmdList CHAVETA2 {
    $$ = comand_WHILE(WHILE_CMD, $3, $6);
  }
  |
  PRINT_INT_CMD PARENTESE1 VAR PARENTESE2 PVIRGULA{
    $$ = comand_PRINT_INT1(PRINT_INT_CMD, $3);
  }
  |
  PRINT_INT_CMD PARENTESE1 VAR VIRGULA VAR PARENTESE2 PVIRGULA{
    $$ = comand_PRINT_INT2(PRINT_INT_CMD, $3, $5);
  }
  |
  VAR IGUAL SCAN_INT_CMD PARENTESE1 PARENTESE2 PVIRGULA{
    $$ = comand_SCAN_INT(SCAN_INT_CMD, $1);
  }
  |
  INT_CMD MAIN PARENTESE1 PARENTESE2 CHAVETA1 cmdList CHAVETA2{  
    $$ = comand_make_main($6);
  }
  |
  BOOL VAR PARENTESE1 INT_CMD VAR PARENTESE2 CHAVETA1 cmdList CHAVETA2 { 
    $$ = comand_make_bool($2, $5, $8);
  }
  |
  INT_CMD VAR PARENTESE1 INT_CMD VAR PARENTESE2 CHAVETA1  cmdList CHAVETA2{
    $$ = comand_make_int($2, $5, $8);
  }
   |
  RETURN TRUE PVIRGULA{
    $$ = comand_return_true();
  }
  |
  RETURN FALSE PVIRGULA{
    $$ = comand_return_false();
  }
  |
  RETURN expr PVIRGULA{
    $$ = comand_return_int($2);
  }
  |
  PRINT_STR_CMD PARENTESE1 STRING_CMD PARENTESE2 PVIRGULA{
    $$ = comand_print_str($3);
  }
  |
  CONTINUE_CMD PVIRGULA{
     $$ = comand_continue();
  }
  |
  BREAK_CMD PVIRGULA{
    $$ = comand_break();
  }
  |
  FOR_CMD PARENTESE1 INT_CMD VAR IGUAL expr PVIRGULA boolean PVIRGULA VAR PLUSS PARENTESE2 CHAVETA1 cmdList CHAVETA2{
      $$ = comand_for($4, $6, $8, $10, PLUS, $14);
  }
  |
  FOR_CMD PARENTESE1 INT_CMD VAR IGUAL expr PVIRGULA boolean PVIRGULA VAR SUBB PARENTESE2 CHAVETA1 cmdList CHAVETA2{
      $$ = comand_for($4, $6, $8, $10, SUB, $14);
  }
  |
  FOR_CMD PARENTESE1  VAR IGUAL expr PVIRGULA boolean PVIRGULA VAR PLUSS PARENTESE2 CHAVETA1 cmdList CHAVETA2{
      $$ = comand_for1($3, $5, $7, $9, PLUS, $13);
  }
  |
  FOR_CMD PARENTESE1  VAR IGUAL expr PVIRGULA boolean PVIRGULA VAR SUBB PARENTESE2 CHAVETA1 cmdList CHAVETA2{
      $$ = comand_for1($3, $5, $7, $9, SUB, $13);
  }
  ;
  

cmdList:
  comand {
    $$ = mkListC($1, NULL);
  }
  |
  comand cmdList {
    $$ = mkListC($1,$2);
  }
  ;

  %%


void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
