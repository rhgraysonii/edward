#include "mpc.h"
#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs("edward> ", stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
#endif

/* make enumeration of potential lval types */
enum { LVAL_NUM, LVAL_ERR };

/* enumeration of possible error types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* lisp value struct */
typedef struct {
  int type;
  long num;
  int err;
} lval;

/* number type lval */
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

/* error type lval */
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

/* print an lval */
void lval_print(lval v) {
  switch(v.type) {
    /* if type is num print */
    case LVAL_NUM: printf("%li", v.num); break;
    
    /* if type is error */
    case LVAL_ERR: 
  
    /* check error and print it */
    if(v.err == LERR_DIV_ZERO) { printf("Error: Divided by zero."); }
    if(v.err == LERR_BAD_OP) { printf("Error: Invalid operator."); }
    if(v.err == LERR_BAD_NUM) { printf("Error: Invalid number."); }
    break;
  }
}

/* print lval w/newline */
lval_println(lval v) { lval_print(v); putchar('\n'); }
  
/* define exponentiation because it is not built into C */
/* lval expo(int a, int b){
  lval result = 1;
  while (b){
    if (b&1){
      result *= a;
    }
    b >>=1 ;
    a *= a;
  }
  return lval_num(result);
}
*/

/* Use operator string to see which operation to perform */
lval eval_op(lval x, char* operator, lval y) {

  /* if either is an error return it */
  if (x.type == LVAL_ERR) { return x; }
  if (y.type == LVAL_ERR) { return y; }

  /* otherwise maths are already in the 'straight ballin' format and we continue with operations */
  if (strcmp(operator, "+") == 0) { return lval_num(x.num + y.num);      }
  if (strcmp(operator, "-") == 0) { return lval_num(x.num - y.num);      }
  if (strcmp(operator, "*") == 0) { return lval_num(x.num * y.num);      }

  if (strcmp(operator, "/") == 0) { 
    /* if second operand is zero return error over result */  
    return  y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
  }

  if (strcmp(operator, "%") == 0) { return lval_num(x.num % y.num);      }
  /* if (strcmp(operator, "^") == 0) { 
    lval lead     = lval_num(x.num);
    lval exponent = lval_num(y.num);
    return lval_num(expo(x.num, y.num)); 
  }
  return lval_err(LERR_BAD_OP);
} */

/* evaluation setup */
lval eval(mpc_ast_t* t) {
  /* if number return it directly */ 
  if (strstr(t->tag, "number")) { 
    /* check if theres error in conversion */
    long x = strtol(t->contents, NULL, 10);
   
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }
  /* operator is second child */
  char* operator = t->children[1]->contents;
  
  /* third child in placeholder */
  lval x = eval(t->children[2]);
  
  /* iterate children combining with op */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, operator, eval(t->children[i]));
    i++;
  }
  return x;
}

int main(int argc, char** argv) {

  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Edward = mpc_new("edward");

  mpca_lang(MPC_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' | '^' | '%';       \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      edward   : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Edward);

  puts("Edward Version 0.0.0.0.3");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    char* input = readline("edward> ");
    add_history(input);
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Edward, &r)) {
      lval result = eval(r.output);
      lval_println(result);
      mpc_ast_delete(r.output);
    } else {    
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Edward);
  return 0;
}
