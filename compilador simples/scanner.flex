%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

"+" { return PLUS; }
"++" { return PLUSS; }
"--" { return SUBB; }
"-" { return SUB; }
"/" { return DIV; }
"*" { return MULT; }
"%" { return MOD; }
"=" { return IGUAL; }

">" { return GT; }
">=" { return GTEQ; }
"<" { return LT; }
"<=" { return LTEQ; }
"==" { return EQUAL; }
"!=" { return NOT_EQUAL; }
"true" { return TRUE; }
"false" { return FALSE; }
"&&" { return AND; }
"||" { return OR; }  
"return" {return RETURN; }
"!" { return SS; }

"if" { return IF_CMD; }
"else" { return ELSE_CMD; }
"int" { return INT_CMD; }
"while" { return WHILE_CMD; }
"for" { return FOR_CMD; }
"print_int" { return PRINT_INT_CMD; }
"scan_int" { return SCAN_INT_CMD; }
"print_str" { PRINT_STR_CMD; }

"bool" { return BOOL; }
"main" { return MAIN; }

"continue" { return CONTINUE_CMD;}
"break" {return BREAK_CMD;}

"{" { return CHAVETA1; }
"}" { return CHAVETA2; }
"(" { return PARENTESE1; }
")" { return PARENTESE2; }
";" { return PVIRGULA; }
"," { return VIRGULA; }
"'" { return PELICAS; }
"\"" { return ASPAS; }
"&" { return ECOM; }


\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return INT;
}

\-?[a-zA-Z]+[_0-9]*[_a-zA-Z0-9]* {
   yylval.charValue = strdup(yytext);
   return VAR;
}

\".*\" {
	yylval.charValue = strdup(yytext);
	return STRING_CMD;
}

.  { yyerror("unexpected character"); }
%%


