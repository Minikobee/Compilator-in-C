#include <stdlib.h> // for malloc
#include <string.h>
#include <stdio.h>
#include "TP2.h"
#include "parser.h"
int k=0;
int labelnum=1;

FILE* fp;
char* registoAuxiliar;
Atom* atomAuxiliar;

char* newTemp(){
    char* temp = malloc(1024*sizeof(char));
    sprintf(temp,"t%d",k);
  k++;
  if(k==8)
    k=0;
  return temp;
}

char* createLabel() {
	char* i;
	char* t = malloc(1024*sizeof(char));
	sprintf(t, "L%d", labelnum);
	i = strdup(t);
	labelnum++;
	return i;
}

Atom* makeVar(char* var){
    Atom* a = malloc(sizeof(Atom));
    a->kind = CSTRING;
    a->elem.name = strdup(var);
    return a;
}

Atom* makeInt(int val){
    Atom* a = malloc(sizeof(Atom));
    a->kind = CINT;
    a->elem.val = val;
    return a;
}

Atom* makeEmpty(){
    Atom* a = malloc(sizeof(Atom));
    a->kind = CEMPTY; 
    return a;
}

void printAtom(Atom* a){
     switch (a->kind){
    case CINT:
      printf("%d", a->elem.val);
      break;
    case CSTRING:
      printf("%s", a->elem.name);
      break;
    case CEMPTY:
      printf("VAZIO");
      break;
    default:
      printf("ATOM ERROR!\n");
      break;
  }

}

Instr* makeIf(int op, Atom* a1, Atom* a2, char* label1, char* label2){
    Instr* a = malloc(sizeof(Instr));
    switch(op){
      case GT:
        a->op =CIFG;
      break;
      case LT:
        a->op =CIFL;
      break;
      case GTEQ:
        a->op = CIFGE;
		break;
      break;
      case LTEQ:
        a->op =CIFLE;
      break;
      case EQUAL:
        a->op =CIFEQ;
      break;
      case NOT_EQUAL:
      a->op =CIFNE;
      break;
    }

    a->attr.instIF.operator = op;
    a->attr.instIF.el1 = a1;
    a->attr.instIF.el2 = a2;
    a->attr.instIF.l1 = strdup(label1);
    a->attr.instIF.l2 = strdup(label2);
    return a;

}

Instr* makelabel(char* label){
    Instr* inst = malloc(sizeof(inst));
    inst->op = CLABEL;
    inst->attr.instLABEL.l1= strdup(label);
    return inst;
}

Instr* makeAtrib(Atom* a1, Atom* a2){
    Instr* inst = (Instr*) malloc(sizeof(Instr));
    inst->op = CATRIB;
    inst->attr.instATRIB.el1 = a1;
    inst->attr.instATRIB.el2 = a2;
    return inst;
}

Instr* makeGoto(char* label){
  Instr* inst = malloc(sizeof(inst));
  inst->op = CGOTO;
  inst->attr.instGOTO.l1 = strdup(label);
  return inst;
}

Instr* makeOperation(int operator, Atom* a1, Atom* a2, Atom* a3){
  Instr* inst = malloc(sizeof(Instr));
  switch (operator) {
    case PLUS:
      inst->op = CPLUS;
      break;
    case SUB:
      inst->op = CMINUS;
      break;
    case DIV:
      inst->op = CDIV;
      break;
    case MULT:
      inst->op = CMULT;
      break;
  }
  inst->attr.instOP.operator = operator;
  inst->attr.instOP.el1 = a1;
  inst->attr.instOP.el2 = a2;
  inst->attr.instOP.el3 = a3;
  return inst;
}

Instr* makePrint(char* s) {
	Instr* i = (Instr*) malloc(sizeof(Instr));
	i->op = CPRINT1;
	i->attr.instPRINT1.str = s;
	return i;
}

Instr* makePrintString(char* s, char* v) {
	Instr* i = (Instr*) malloc(sizeof(Instr));
	i->op = CPRINT2;
	i->attr.instPRINT2.str = s;
	i->attr.instPRINT2.var = v;
	return i;
}

Instr* makeRead(Atom* a1, char* r) {
	Instr* i = (Instr*) malloc(sizeof(Instr));
	i->op = CREADLINE;
	i->attr.instREADLINE.el1 = a1;
	i->attr.instREADLINE.name = r;
	i = makeAtrib(makeVar(i->attr.instREADLINE.name), i->attr.instREADLINE.el1);
	return i;
}

Instr* getFirst(InstrList* i) {
	return i -> inst;
}

Instr* makeMain(){
    Instr* i = (Instr*) malloc(sizeof(Instr));
    i->op = CMAIN;
    return i;
}

Instr* makeIntBool(){
    Instr* i = (Instr*) malloc(sizeof(Instr));
    i->op = CBOOL;
    return i;
}

Instr* makeIntB(){
    Instr* i = (Instr*) malloc(sizeof(Instr));
    i->op = CINTBOOL;
    return i;
}

void printinst(Instr* i) {
	switch (i->op) {
		case CLABEL:
		printf("LABEL %s\n", i->attr.instLABEL.l1);
		break;
		case CGOTO:
		printf("GOTO L%s\n", i->attr.instGOTO.l1);
		break;
		case CATRIB:
		if(i->attr.instATRIB.el2->kind == CINT){
			printAtom(i->attr.instATRIB.el1);
			printf(" = ");
			printAtom(i->attr.instATRIB.el2);
			printf("\n");
		}
		else{
			printAtom(i->attr.instATRIB.el2);
			printf(" = ");
			printAtom(i->attr.instATRIB.el1);
			printf("\n");
		}
		break;
		case CPLUS:
		printAtom(i->attr.instOP.el1);
		printf(" = ");
		printAtom(i->attr.instOP.el2);
		printf(" + ");
		printAtom(i->attr.instOP.el3);
		printf("\n");
		break;
		case CMINUS:
		printAtom(i->attr.instOP.el1);
		printf(" = ");
		printAtom(i->attr.instOP.el2);
		printf(" - ");
		printAtom(i->attr.instOP.el3);
		printf("\n");
		break;
		case CMULT:
		printAtom(i->attr.instOP.el1);
		printf(" = ");
		printAtom(i->attr.instOP.el2);
		printf(" * ");
		printAtom(i->attr.instOP.el3);
		printf("\n");
		break;
		case CDIV:
		printAtom(i->attr.instOP.el1);
		printf(" = ");
		printAtom(i->attr.instOP.el2);
		printf(" / ");
		printAtom(i->attr.instOP.el3);
		printf("\n");
		break;
		case CMOD:
		printAtom(i->attr.instOP.el1);
		printf(" = ");
		printAtom(i->attr.instOP.el2);
		printf(" mod ");
		printAtom(i->attr.instOP.el3);
		printf("\n");
		break;
		case CIFG:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" > ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CIFL:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" < ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CIFGE:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" > ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CIFLE:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" <= ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CIFEQ:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" == ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CIFNE:
		printf("if(");
		printAtom(i->attr.instIF.el1);
		printf(" != ");
		printAtom(i->attr.instIF.el2);
		printf(") then ");
		printf("%s", i->attr.instIF.l1);
		printf("\n");
		printf("else ");
		printf("%s", i->attr.instIF.l2);
		printf("\n");
		break;
		case CREADLINE:
        printf("scan_int(");
		    printAtom(i->attr.instREADLINE.el1);
		    printf(")\n"); 
        break;
      case CPRINT1:
      printf("print_int(");
		  printAtom(makeVar(i->attr.instPRINT1.str));
		  printf(")\n");
        break;
      case CPRINT2:
      printf("print_int!(");
		  printAtom(makeVar(i->attr.instPRINT2.str));
		  printf(", ");
		  printAtom(makeVar(i->attr.instPRINT2.var));
		  printf(")\n");
        break;
      case CMAIN:
        printf("Main\n");
        break;
      case CBOOL:
        printf("BOOL\n");
        break;
      case CINTBOOL:
        printf("INT\n");
        break;
		default:
		yyerror("Unknown operator!\n");
	}
}

InstrList* mkInstrList(Instr* i, InstrList* l){
    InstrList* list = malloc(sizeof(struct list_inst));
    list->inst = i;
    list->next = l;
    return list;
}

InstrList* appendinstList(InstrList* l1, InstrList* l2){
    InstrList* lf = l1;
    if(l1 == NULL) return l2;
    else{
        while(l1->next != NULL){
        l1=l1->next;
        }
        l1->next=l2;
    }
    return lf;
}

/*InstrList* getFirstinstList(InstrList* l){
  if(l == NULL){
    printf("ERROR");
  }
  else{
    return l->inst;
  }
}*/

InstrList* nextinst(InstrList* l){
    if(l == NULL){
        printf("ERROR");
    }
    else if(l->next == NULL)
        printf("ERROR");
    else{
        return l->next;
    }
}


InstrList* append(InstrList* first, InstrList* second) {
	if(first == NULL) return second;
	else if(second == NULL) return first;
	else if(first == NULL && second == NULL) return NULL;
	InstrList* list = first;
	while((first -> next) != NULL) {
		first = first -> next;
	}
	first -> next = second;
	return list;
}


int length(InstrList* l){
  int i=0;
  while(l != NULL){
    l=l->next;
    i++;
  }
  return i;
}

InstrList* compileExprArit(ExprArit* e, char* r) {
	Atom* a1 = (Atom*) malloc(sizeof(Atom));
	Atom* a2 = (Atom*) malloc(sizeof(Atom));
	char* reg1 = malloc(1024*(sizeof(char)));
	char* reg2 = malloc(1024*(sizeof(char)));
	InstrList* code1 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code2 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code3 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code4 = (InstrList*) malloc(sizeof(InstrList));

	switch (e->kind) {
		case E_INTEGER:
		a1 = makeVar(r);
		a2 = makeInt(e->attr.value);
		Instr* instINT = makeAtrib(a1, a2);
		code1 = mkInstrList(instINT, NULL);
		return code1;
		case (E_OPERATION):
		reg1 = newTemp();
		reg2 = newTemp();
		a1 = makeVar(reg1);
		a2 = makeVar(reg2);
		code1 = compileExprArit(e->attr.op.left, reg1);
		code2 = compileExprArit(e->attr.op.right, reg2);
		code3 = append(code1, code2);
		int op = e -> attr.op.operator;

		code4 = append(code3,mkInstrList(makeOperation(op,makeVar(r),a1,a2), NULL));
		return code4;
		case E_VARIABLE:
		a1 = makeVar(r);
		a2 = makeVar(e->attr.var);
		Instr* instVAR = makeAtrib(a1, a2);
		code1 = mkInstrList(instVAR, NULL);
		return code1;
		default:
		yyerror("Unknown operator3333!\n");
	}
	return NULL;
}

InstrList* compileExprBool(ExprBool* b, char* lab1, char* lab2) {
	Atom* a1 = (Atom*) malloc(sizeof(Atom));
	Atom* a2 = (Atom*) malloc(sizeof(Atom));
	char* reg1 = malloc(1024*(sizeof(char)));
	char* reg2 = malloc(1024*(sizeof(char)));
	InstrList* code1 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code2 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code3 = (InstrList*) malloc(sizeof(InstrList));
	InstrList* code4 = (InstrList*) malloc(sizeof(InstrList));

	switch (b->kind) {
		case E_BOOL:
			reg1 = newTemp();
			a1 = makeVar(reg1);
			a2 = makeInt(b->attr.value);
			Instr* inst = makeAtrib(a1, a2);
			code1 = mkInstrList(inst, NULL);
			return code1;
		case E_OP:
			reg1 = newTemp();
			reg2 = newTemp();
			code1 = compileExprArit(b->attr.opBool.left, reg1);
			code2 = compileExprArit(b->attr.opBool.right, reg2);
			code3 = append(code1, code2);
			int op = b -> attr.opBool.operator;
			code4 = append(code3, mkInstrList(makeIf(op,makeVar(reg1), makeVar(reg2), lab1, lab2), NULL));
			return code4;
		default:
			yyerror("Unknown operator222222!\n");
			break;
	}
	return NULL;
}

InstrList* compileCmd(Comand* c) {
	InstrList* code = (InstrList*) malloc(sizeof(InstrList));
	char* reg1 = malloc(1024*(sizeof(char)));
	char* reg2 = malloc(1024*(sizeof(char)));
	char* l1 = malloc(1024*(sizeof(char)));
	char* l2 = malloc(1024*(sizeof(char)));
	char* l3 = malloc(1024*(sizeof(char)));
	Atom* a1 = (Atom*) malloc(sizeof(Atom));
	Atom* a2 = (Atom*) malloc(sizeof(Atom));
	switch (c->kind) {
		case IF:
		l1 = createLabel();
		l2 = createLabel();
		code = compileExprBool(c->attr.cmdIF.condicao_IF, l1, l2);
		code = append(code, mkInstrList(makelabel(l1), NULL));
		code = append(code, compileCmdList(c->attr.cmdIF.listC_IF));
		code = append(code, mkInstrList(makelabel(l2), NULL));
		return code;
		case IF_THEN_ELSE:
		l1 = createLabel();
		l2 = createLabel();
		l3 = createLabel();
		code = compileExprBool(c->attr.cmdIF_THEN_ELSE.condicao_IF_THEN, l1, l2);
		code = append(code, mkInstrList(makelabel(l1), NULL));
		code = append(code, compileCmdList(c->attr.cmdIF_THEN_ELSE.listC_IF_THEN));
		code = append(code, mkInstrList(makeGoto(l3),NULL));
		code = append(code, mkInstrList(makelabel(l2), NULL));
		code = append(code, mkInstrList(makelabel(l3), NULL));
		return code;
        case ATRIB:
		reg1 = newTemp();
		a1 = makeVar(reg1);
		a2 = makeVar(c->attr.cmdATRIB.var);
		code = mkInstrList(makeAtrib(a1, a2),NULL);
		code = append(code,compileExprArit(c -> attr.cmdATRIB.var_data, reg1));        
        return code;
        case ATRIB3: 
        reg1 = newTemp();
		a1 = makeVar(reg1);
		a2 = makeVar(c->attr.cmdATRIB3.var);
		code = mkInstrList(makeAtrib(a1, a2),NULL);
        return code;
		case INT2:
		reg1 = newTemp();
		a1 = makeVar(reg1);
		a2 = makeVar(c->attr.cmdINT2.var);
		code = mkInstrList(makeAtrib(a1, a2),NULL);
		code = append(code,compileExprArit(c -> attr.cmdINT2.var_data, reg1));
		return code;
		case WHILE:
		l1 = createLabel();
		l2 = createLabel();
		l3 = createLabel();
		code = mkInstrList(makelabel(l1), NULL);
		code = append(code, compileExprBool(c -> attr.cmdWHILE.condicao_WHILE, l2, l3));
		code = append(code, mkInstrList(makelabel(l2), NULL));
		code = append(code, compileCmdList(c -> attr.cmdWHILE.listC_WHILE));
		code = append(code, mkInstrList(makeGoto(l1), NULL));
		code = append(code, mkInstrList(makelabel(l3), NULL));
		return code;
		case PRINT_INT1:
		reg1 = newTemp();
		code = mkInstrList(makePrint(c->attr.cmdPRINT_INT1.str), NULL);
		return code;
		case PRINT_INT2:
		reg1 = newTemp();
		reg2 = newTemp();
		code = mkInstrList(makePrintString(reg1, c->attr.cmdPRINT_INT2.str), NULL);
		code = append(code, mkInstrList(makePrintString(reg2, c -> attr.cmdPRINT_INT2.var), NULL));
		return code;
		case SCAN_INT:
		reg1 = newTemp();
		code = mkInstrList(makeRead(makeVar(c->attr.cmdSCAN_INT.str), reg1), NULL);
		return code;
        case FUNC_MAIN:
        code = mkInstrList(makeMain(), compileCmdList(c->attr.cmdMain.list));
		return code;
        case FUNC_BOOL:
        code = mkInstrList(makeIntBool(), compileCmdList(c->attr.cmdBool.list));
		return code;
        case FUNC_INT:
        code = mkInstrList(makeIntB(), compileCmdList(c->attr.cmdInt .list));
		return code;
        default:

			yyerror("Unknown operator1111!\n");
			break;
	}
	return NULL;
}

InstrList* compileCmdList(Cmd_List* l) {
	if(l == NULL) return NULL;
	InstrList* l1 = compileCmd(l -> cmd);
	InstrList* l2 = compileCmdList(l -> next);
	return (append(l1,l2));
}

void printInstrList(InstrList* listExpr){
    while(listExpr!=NULL){
		printinst(listExpr->inst);
		listExpr = listExpr->next;
	}
}


void fprintAtom(Atom* a) {
  switch (a->kind) {
		case CSTRING:
			fprintf(fp, "%s ", strdup(a->elem.name));
			break;
		case CINT:
			fprintf(fp, "%d ", a->elem.val);
			break;
		default:
			break;
	}
}

void printMipsList(InstrList* list) {
  Atom* a1;
  Atom* a2;

  char fileName[50] = "MipsCode.txt";
  fp = fopen(fileName, "w");
  if(fp == NULL) {
    yyerror("Impossível criar o ficheiro Mips!\n");
    return;
  }
  registoAuxiliar = newTemp();
  atomAuxiliar = makeVar(registoAuxiliar);
  fprintf(fp, " .text\n\n");
  fprintf(fp, "main:\n");
  while(list != NULL) {
    switch(list->inst->op) {
      case CLABEL:
      fprintAtom(makeVar(list->inst->attr.instLABEL.l1));
      fprintf(fp, ":");
      break;
      case CGOTO:
      fprintf(fp, "\tj ");
      fprintAtom(makeVar(list->inst->attr.instGOTO.l1));
      break;
      case CATRIB:
      if (list->inst->attr.instATRIB.el2->kind == CINT){
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instATRIB.el1);
        fprintAtom(list->inst->attr.instATRIB.el2);
      }
      else if(list->inst->attr.instATRIB.el2->kind == CSTRING) {
        fprintf(fp, "\tsw ");
        fprintAtom(list->inst->attr.instATRIB.el2);
        fprintAtom(list->inst->attr.instATRIB.el1);
      }
      break;
      case CPLUS:
      if (list->inst->attr.instOP.el2->kind == CINT && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\taddi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if (list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\taddi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if (list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CSTRING) {
        fprintf(fp, "\tadd ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tadd ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      break;
      case CMINUS:
      if(list->inst->attr.instOP.el2->kind == CINT && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tsubi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tsubi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CSTRING) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tlw ");
        fprintAtom(atomAuxiliar);
        fprintAtom(list->inst->attr.instOP.el3);
        fprintf(fp, "\n");
        fprintf(fp, "\tsub ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(atomAuxiliar);
      }
      else {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tsub ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      break;
      case CMULT:
      if(list->inst->attr.instOP.el2->kind == CINT && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tmulti ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tmulti ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CSTRING) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tlw ");
        fprintAtom(atomAuxiliar);
        fprintAtom(list->inst->attr.instOP.el3);
        fprintf(fp, "\n");
        fprintf(fp, "\tmult ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(atomAuxiliar);
      }
      else {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tmult ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      break;
      case CDIV:
      if(list->inst->attr.instOP.el2->kind == CINT && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdivi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdivi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CSTRING) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tlw ");
        fprintAtom(atomAuxiliar);
        fprintAtom(list->inst->attr.instOP.el3);
        fprintf(fp, "\n");
        fprintf(fp, "\tdiv ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(atomAuxiliar);
      }
      else {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdiv ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      fprintf(fp, "\n\tMFLO ");
      fprintAtom(list->inst->attr.instOP.el1);
      break;
      case CMOD:
      if(list->inst->attr.instOP.el2->kind == CINT && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdivi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CINT) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdivi ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      else if(list->inst->attr.instOP.el2->kind == CSTRING && list->inst->attr.instOP.el3->kind == CSTRING ) {
        fprintf(fp, "\tlw ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tlw ");
        fprintAtom(atomAuxiliar);
        fprintAtom(list->inst->attr.instOP.el3);
        fprintf(fp, "\n");
        fprintf(fp, "\tdiv ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(atomAuxiliar);
      }
      else {
        fprintf(fp, "\tli ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el2);
        fprintf(fp, "\n");
        fprintf(fp, "\tdiv ");
        fprintAtom(list->inst->attr.instOP.el1);
        fprintAtom(list->inst->attr.instOP.el3);
      }
      fprintf(fp, "\n\tMFHO ");
      fprintAtom(list->inst->attr.instOP.el1);
      break;
      case CIFG:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }
      fprintf(fp, "\tbgt ");
      fprintAtom(a1);
      fprintAtom(a2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CIFL:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }
      fprintf(fp, "\tblt ");
      fprintAtom(list->inst->attr.instIF.el1);
      fprintAtom(list->inst->attr.instIF.el2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CIFGE:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }
      fprintf(fp, "\tbge ");
      fprintAtom(a1);
      fprintAtom(a2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CIFLE:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }
     
      fprintf(fp, "\tble ");
      fprintAtom(a1);
      fprintAtom(a2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CIFEQ:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }

      fprintf(fp, "\tbeq ");
      fprintAtom(a1);
      fprintAtom(a2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CIFNE:
      if(list->inst->attr.instIF.el1->kind == CINT) {
        a1 = makeVar(newTemp());
        fprintf(fp, "\tli ");
        fprintAtom(a1);
        fprintAtom(list->inst->attr.instIF.el1);
        fprintf(fp, "\n");
      }
      fprintf(fp, "\tbne ");
      fprintAtom(a1);
      fprintAtom(a2);
      fprintAtom(makeVar(list->inst->attr.instIF.l1));
      break;
      case CPRINT1:
      fprintf(fp, "\tli $v0, 4\n");
      fprintf(fp, "\tla $a0, %s", list->inst->attr.instPRINT1.str);
      fprintf(fp, "\n\tsyscall");
      break;
      case CPRINT2:
      fprintf(fp, "\tli $v0, 4\n");
      fprintf(fp, "\tla $a0, %s", list->inst->attr.instPRINT2.str);
      fprintf(fp, "\n\tsyscall");
      fprintf(fp, "\tli $v0, 4\n");
      fprintf(fp, "\tla $a0, %s", list->inst->attr.instPRINT2.var);
      fprintf(fp, "\n\tsyscall");
      break;
      case CREADLINE:
      fprintf(fp, "\tli $v0, 5\n");
      fprintf(fp, "\tsyscall\n");
      fprintf(fp, "\tsw %s", list->inst->attr.instREADLINE.name);
      fprintf(fp, "$v0");
      break;
      case CMAIN:
      break;
      case CBOOL:
      break;
      case CINTBOOL:
      break;
      default:
      yyerror("Unknown operator222!\n");
    }
    fprintf(fp, "\n");
    list = list->next;
  }
  fprintf(fp,"\tli $v0, 10\n  syscall");
  return;
}
