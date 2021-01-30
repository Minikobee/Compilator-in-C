#include <stdlib.h>
#include <string.h>
#include "TP1.h"

/* **** EXPRESSÕES ARITMÉTICAS (INTEIROS) **** */

ExprArit *expArit_Integer(int v)
{
  ExprArit *expA = (ExprArit *)malloc(sizeof(ExprArit));
  expA->kind = E_INTEGER;
  expA->attr.value = v;
  return expA;
}

ExprArit *expArit_Operation(int operator, ExprArit * left, ExprArit * right)
{
  ExprArit *expA = (ExprArit *)malloc(sizeof(ExprArit));
  expA->kind = E_OPERATION;
  expA->attr.op.operator= operator;
  expA->attr.op.left = left;
  expA->attr.op.right = right;
  return expA;
}

ExprArit *expArit_Variable(char *var)
{
  ExprArit *expA = (ExprArit *)malloc(sizeof(ExprArit));
  expA->kind = E_VARIABLE;
  expA->attr.var = strdup(var);
  return expA;
}

ExprArit *expArit_Parentesis(ExprArit *expr)
{
  ExprArit *expA = (ExprArit *)malloc(sizeof(ExprArit));
  expA->kind = E_PARENTESIS;
  expA->attr.insideP.insideParentesis = expr;
  return expA;
}

/* **** EXPRESSÕES BOOLEANAS (TRUE/FALSE) **** */

ExprBool *expBool_Value(int v)
{
  ExprBool *expB = (ExprBool *)malloc(sizeof(ExprBool));
  expB->kind = E_BOOL;
  expB->attr.value = v;
  return expB;
}

ExprBool *expBool_Operation(int operator, ExprArit * left, ExprArit * right)
{
  ExprBool *expB = (ExprBool *)malloc(sizeof(ExprBool));
  expB->kind = E_OP;
  expB->attr.opBool.operator= operator;
  expB->attr.opBool.left = left;
  expB->attr.opBool.right = right;
  return expB;
}

ExprBool *expBool_Logic(int operator, ExprBool * left, ExprBool * right)
{
  ExprBool *expB = (ExprBool *)malloc(sizeof(ExprBool));
  expB->kind = E_LOGICA;
  expB->attr.logBool.p_logica = operator;
  expB->attr.logBool.leftLOG = left;
  expB->attr.logBool.rightLOG = right;
  return expB;
}

ExprBool *make_true()
{
  ExprBool *expB = (ExprBool *)malloc(sizeof(ExprBool));
  expB->kind = M_true;
  return expB;
}

ExprBool *make_false()
{
  ExprBool *expB = (ExprBool *)malloc(sizeof(ExprBool));
  expB->kind = M_false;
  return expB;
}

/* **** COMANDOS (IF, IF THEN ELSE, INT, WHILE, PPRINT_INT, SCAN_INT) **** */

Comand *comand_IF(int operator, ExprBool * c, Cmd_List * lc)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = IF;
  cmd->attr.cmdIF.operator= operator;
  cmd->attr.cmdIF.condicao_IF = c;
  cmd->attr.cmdIF.listC_IF = lc;
  return cmd;
}

Comand *comand_IF_THEN_ELSE(int operator, ExprBool * c, Cmd_List * lc_if, Cmd_List * lc_else)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = IF_THEN_ELSE;
  cmd->attr.cmdIF_THEN_ELSE.operator= operator;
  cmd->attr.cmdIF_THEN_ELSE.condicao_IF_THEN = c;
  cmd->attr.cmdIF_THEN_ELSE.listC_IF_THEN = lc_if;
  cmd->attr.cmdIF_THEN_ELSE.listC_ELSE = lc_else;
  return cmd;
}

Comand *comand_INT2(int operator, char * v, ExprArit * expA)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = INT2;
  cmd->attr.cmdINT2.operator= operator;
  cmd->attr.cmdINT2.var = strdup(v);
  cmd->attr.cmdINT2.var_data = expA;
  return cmd;
}

Comand *comand_ATRIB(char *v, ExprArit *expA)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = ATRIB;
  cmd->attr.cmdATRIB.var = strdup(v);
  cmd->attr.cmdATRIB.var_data = expA;
  return cmd;
}

Comand *comand_ATRIB2(char *v, char *d)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = ATRIB2;
  cmd->attr.cmdATRIB2.var = strdup(v);
  cmd->attr.cmdATRIB2.var2 = strdup(d);
  return cmd;
}

Comand *comand_ATRIB3(char *v)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = ATRIB3;
  cmd->attr.cmdATRIB3.var = strdup(v);
  return cmd;
}

Comand *comand_WHILE(int operator, ExprBool * c, Cmd_List * lc)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = WHILE;
  cmd->attr.cmdWHILE.operator= operator;
  cmd->attr.cmdWHILE.condicao_WHILE = c;
  cmd->attr.cmdWHILE.listC_WHILE = lc;
  return cmd;
}

Comand *comand_PRINT_INT1(int operator, char * str)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = PRINT_INT1;
  cmd->attr.cmdPRINT_INT1.operator= operator;
  cmd->attr.cmdPRINT_INT1.str = strdup(str);
  return cmd;
}

Comand *comand_PRINT_INT2(int operator, char * str, char * var)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = PRINT_INT2;
  cmd->attr.cmdPRINT_INT2.operator= operator;
  cmd->attr.cmdPRINT_INT2.str = strdup(str);
  cmd->attr.cmdPRINT_INT2.var = strdup(var);
  return cmd;
}

Comand *comand_SCAN_INT(int operator, char * str)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = SCAN_INT;
  cmd->attr.cmdSCAN_INT.operator= operator;
  cmd->attr.cmdSCAN_INT.str = strdup(str);
  return cmd;
}

Comand *comand_make_main(Cmd_List *cmdList)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = FUNC_MAIN;
  cmd->attr.cmdMain.list = cmdList;
  return cmd;
}

Comand *comand_make_bool(char *var, char *var2, Cmd_List *cmdList)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = FUNC_BOOL;
  cmd->attr.cmdBool.var = strdup(var);
  cmd->attr.cmdBool.var2 = strdup(var2);
  cmd->attr.cmdBool.list = cmdList;
  return cmd;
}

Comand *comand_make_int(char *var, char *var2, Cmd_List *cmdList)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = FUNC_INT;
  cmd->attr.cmdInt.var = strdup(var);
  cmd->attr.cmdInt.var2 = strdup(var2);
  cmd->attr.cmdInt.list = cmdList;
  return cmd;
}

Comand *comand_return_true()
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = RETURN_TRUE;
  return cmd;
}

Comand *comand_return_false()
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = RETURN_FALSE;
  return cmd;
}
Comand *comand_return_int(ExprArit *expr)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = RETURN_INT;
  cmd->attr.cmdReturnInt.var_data = expr;
  return cmd;
}

Comand *comand_print_str(char *var)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = PRINT_STR;
  cmd->attr.cmdPrintStr.var = strdup(var);
  return cmd;
}

Comand *comand_continue()
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = CONTINUE;
  return cmd;
}

Comand *comand_break()
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = BREAK;
  return cmd;
}

Comand *comand_for(char *var, ExprArit *expr, ExprBool *left, char *var2, int operator, Cmd_List * cmdList)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = FOR;
  cmd->attr.cmdFor.var = strdup(var);
  cmd->attr.cmdFor.var2 = strdup(var2);
  cmd->attr.cmdFor.cmdList = cmdList;
  cmd->attr.cmdFor.operator= operator;
  cmd->attr.cmdFor.expr = expr;
  cmd->attr.cmdFor.left = left;

  return cmd;
}

Comand *comand_for1(char *var, ExprArit *expr, ExprBool *left, char *var2, int operator, Cmd_List * cmdList)
{
  Comand *cmd = (Comand *)malloc(sizeof(Comand));
  cmd->kind = FOR1;
  cmd->attr.cmdFor1.var = strdup(var);
  cmd->attr.cmdFor1.var2 = strdup(var2);
  cmd->attr.cmdFor1.cmdList = cmdList;
  cmd->attr.cmdFor1.operator= operator;
  cmd->attr.cmdFor1.expr = expr;
  cmd->attr.cmdFor1.left = left;

  return cmd;
}

/* **** LISTA DE COMANDOS **** */

Cmd_List *mkListC(Comand *cmd, Cmd_List *cmdList)
{
  Cmd_List *cl = (Cmd_List *)malloc(sizeof(Cmd_List));
  cl->cmd = cmd;
  cl->next = cmdList;
  return cl;
}
