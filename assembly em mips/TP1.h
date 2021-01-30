#ifndef __TP1_h__
#define __TP1_h__

typedef struct _ExprArit ExprArit;
typedef struct _ExprBool ExprBool;
typedef struct _Comand Comand;
typedef struct _Cmd_List Cmd_List;

// Estruturas

/* **** EXPRESSÕES ARITMÉTICAS (INTEIROS) **** */

struct _ExprArit
{
  enum
  {
    E_INTEGER,
    E_OPERATION,
    E_VARIABLE,
    E_PARENTESIS
  } kind;
  union
  {
    int value;
    char *var;
    struct
    {
      int operator;
      struct _ExprArit *left;
      struct _ExprArit *right;
    } op;
    struct
    {
      struct _ExprArit *insideParentesis;
    } insideP;
  } attr;
};

/* **** EXPRESSÕES BOOLEANAS (TRUE/FALSE) **** */

struct _ExprBool
{
  enum
  {
    E_BOOL,
    E_OP,
    E_LOGICA,
    M_true,
    M_false
  } kind;
  union
  {
    int value;
    struct
    {
      int operator;
      struct _ExprArit *left;
      struct _ExprArit *right;
    } opBool;
    struct
    {
      int p_logica;
      struct _ExprBool *leftLOG;
      struct _ExprBool *rightLOG;
    } logBool;
  } attr;
};

/* **** COMANDOS (IF, IF THEN ELSE, WHILE, PRINT_INT, SCAN_INT) **** */

struct _Comand
{
  enum
  {
    IF,
    IF_THEN_ELSE,
    INT2,
    ATRIB,
    ATRIB2,
    ATRIB3,
    WHILE,
    FOR,
    FOR1,
    PRINT_INT1,
    PRINT_INT2,
    SCAN_INT,
    FUNC_MAIN,
    FUNC_INT,
    FUNC_BOOL,
    RETURN_TRUE,
    RETURN_FALSE,
    PRINT_STR,
    CONTINUE,
    BREAK,
    RETURN_INT
  } kind;
  union
  {
    struct
    { // 2 campos
      int operator;
      ExprBool *condicao_IF;
      Cmd_List *listC_IF;
    } cmdIF;

    struct
    { // 3 campos
      int operator;
      ExprBool *condicao_IF_THEN;
      Cmd_List *listC_IF_THEN;
      Cmd_List *listC_ELSE;
    } cmdIF_THEN_ELSE;

    struct
    { // 2 campos
      int operator;
      char *var;
      ExprArit *var_data;
    } cmdINT2;

    struct
    { // 2 campos
      char *var;
      ExprArit *var_data;
    } cmdATRIB;

    struct
    { // 2 campos
      char *var;
      char *var2;
    } cmdATRIB2;

    struct
    { // 2 campos
      char *var;
    } cmdATRIB3;

    struct
    { // 2 campos
      int operator;
      ExprBool *condicao_WHILE;
      Cmd_List *listC_WHILE;
    } cmdWHILE;

    struct
    { // 1 campo
      int operator;
      char *str;
    } cmdPRINT_INT1;

    struct
    { // 1 campo
      int operator;
      char *str;
      char *var;
    } cmdPRINT_INT2;

    struct
    { // 1 campo
      int operator;
      char *str;
    } cmdSCAN_INT;

    struct
    {
      Cmd_List *list;
    } cmdMain;

    struct
    {
      char *var;
      char *var2;
      Cmd_List *list;
    } cmdBool;

    struct
    {
      char *var;
      char *var2;
      Cmd_List *list;
    } cmdInt;

    struct
    {
      ExprArit *var_data;
    } cmdReturnInt;

    struct
    {
      char *var;
    } cmdPrintStr;

    struct
    {
      char *var;
      ExprArit* expr;
      ExprBool *left;
      char *var2;
      int operator;
      Cmd_List *cmdList;
    } cmdFor;

    struct
    {
      char *var;
      ExprArit* expr;
      ExprBool *left;
      char *var2;
      int operator;
      Cmd_List *cmdList;
    } cmdFor1;
    

  } attr;
};

/* **** LISTA DE COMANDOS **** */

struct _Cmd_List
{
  Comand *cmd;
  struct _Cmd_List *next;
};

// Construtores

/* **** EXPRESSÕES ARITMÉTICAS (INTEIROS) **** */
ExprArit *expArit_Integer(int v);
ExprArit *expArit_Operation(int operator, ExprArit * left, ExprArit * right);
ExprArit *expArit_Variable(char *var);
ExprArit *expArit_Parentesis(ExprArit *expr);

/* **** EXPRESSÕES BOOLEANAS (TRUE/FALSE) **** */
ExprBool *expBool_Value(int v);
ExprBool *expBool_Operation(int operator, ExprArit * left, ExprArit * right);
ExprBool *expBool_Logic(int operator, ExprBool * left, ExprBool * right);
ExprBool *make_true();
ExprBool *make_false();

/* **** COMANDOS **** */
Comand *comand_IF(int operator, ExprBool * c, Cmd_List * lc);
Comand *comand_IF_THEN_ELSE(int operator, ExprBool * c, Cmd_List * lc_if, Cmd_List * lc_else);
Comand *comand_INT2(int operator, char * v, ExprArit * expA);
Comand *comand_ATRIB(char *v, ExprArit *expA);
Comand *comand_ATRIB2(char *v, char *d);
Comand *comand_ATRIB3(char *v);
Comand *comand_WHILE(int operator, ExprBool * c, Cmd_List * lc);
Comand *comand_PRINT_INT1(int operator, char * str);
Comand *comand_PRINT_INT2(int operator, char * str, char * var);
Comand *comand_SCAN_INT(int operator, char * str);
Comand *comand_make_main(Cmd_List *cmdList);
Comand *comand_make_bool(char *var, char *var2, Cmd_List *cmdList);
Comand *comand_make_int(char *var, char *var2, Cmd_List *cmdList);
Comand *comand_return_true();
Comand *comand_return_false();
Comand *comand_return_int(ExprArit *expr);
Comand *comand_print_str(char *var);
Comand *comand_continue();
Comand *comand_break();
Comand* comand_for(char* var, ExprArit* expr, ExprBool* left, char* var2, int operator ,Cmd_List* cmdList);
Comand* comand_for1(char* var, ExprArit* expr, ExprBool* left, char* var2, int operator ,Cmd_List* cmdList);

/* **** LISTA DE COMANDOS **** */
Cmd_List *mkListC(Comand *cmd, Cmd_List *cmdList);

#endif
