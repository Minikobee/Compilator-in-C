#ifndef __TP2_h__
#define __TP2_h__
#include "TP1.h"
#include "parser.h"


/* **** ESTRUTURAS **** */
typedef enum {CMAIN,CBOOL, CINTBOOL, CLABEL, CGOTO, CATRIB, CPLUS, CMINUS, CMULT, CDIV, CMOD, CIFG, CIFL, CIFGE, CIFLE, CIFEQ, CIFNE, CPRINT1, CPRINT2, CREADLINE} OpKind;

typedef enum {CSTRING, CINT, A_REGISTER, CEMPTY} AtomKind;

typedef struct {
  AtomKind kind;
  union {
    int val;
    char* name;
  } elem;
} Atom;

typedef struct {
  OpKind op;
  union {
    struct { 
      char* l1;
    } instLABEL;
    struct {
      char* l1;
    } instGOTO;
    struct { 
      char* name;
      Atom* el1;
      Atom* el2;
    } instATRIB;
    struct { 
      int operator;
      Atom* el1;
      Atom* el2;
      Atom* el3;
    } instOP;
    struct { 
      int operator;
      Atom* el1;
      Atom* el2;
      char* l1;
      char* l2;
    } instIF;
    struct{
      char* str;
    } instPRINT1;
    struct{
      char* str;
      char* var;
    } instPRINT2;
    struct { 
      Atom* el1;
      char* name;
    } instREADLINE;
  } attr;
} Instr;

typedef struct list_inst{
  Instr* inst;
  struct list_inst *next;
} InstrList;

char* newTemp();
char* createLabel();

Atom* makeVar(char* var);
Atom* makeInt(int val);
Atom* makeEmpty();
void printAtom(Atom* a);



Instr* makeIf(int op, Atom* a1, Atom* a2, char* label1, char* label2);
Instr* makelabel(char* label);
Instr* makeAtrib(Atom* a1, Atom* a2);
Instr* makeGoto(char* label);
Instr* makeOperation(int operator, Atom* a1, Atom* a2, Atom* a3);
Instr* makePrint(char* s);
Instr* makePrintString(char* s, char* v);
Instr* makeRead(Atom* a1, char* r);
Instr* getFirst(InstrList* i);
void printInstr(Instr* i);


InstrList* mkInstrList(Instr* i, InstrList* l);
InstrList* appendInstrList(InstrList* l1, InstrList* l2);
//InstrList* getFirstInstrList(InstrList* l);
InstrList* nextInstr(InstrList* l);
int length(InstrList* l);


InstrList* compileExprArit(ExprArit* e, char* r);
InstrList* compileExprBool(ExprBool* b, char* lab1, char* lab2);
InstrList* compileCmd(Comand* c);
InstrList* append(InstrList* first, InstrList* second);
InstrList* compileCmdList(Cmd_List* l);

Instr* makeMain();
Instr* makeIntBool();
Instr* makeIntB();
void printInstrList(InstrList* listExpr); 

void printMipsList(InstrList* list);


#endif

