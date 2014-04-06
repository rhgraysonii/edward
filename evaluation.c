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

/* define exponentiation because it is not built into C */
int expo(int a, int b){
  int result = 1;
  while (b){
    if (b&1){
      result *= a;
    }
    b >>=1 ;
    a *= a;
  }
  return result;
}

/* Use operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if (strcmp(op, "+") == 0) { return x + y;      }
  if (strcmp(op, "-") == 0) { return x - y;      }
  if (strcmp(op, "*") == 0) { return x * y;      }
  if (strcmp(op, "/") == 0) { return x / y;      }
  if (strcmp(op, "%") == 0) { return x % y;      }
  if (strcmp(op, "^") == 0) { return expo(x, y); }
  return 0;
}

/* lisp value struct */
typedef struct {
  int type;
  long num;
  int err;
} lval;

/* make enumeration of potential lval types */
enum { LVAL_NUM, LVAL_ERR };

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

/* enumeration of possible error types */
enum {LERR_DIV-ZERO, LERR_BAD_OP, LERR_BAD_NUM

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

/* evaluation setup */
long eval(mpc_ast_t* t) {
  
  /* if number return it directly */ 
  if (strstr(t->tag, "number")) { return atoi(t->contents); }
  
  /* operator is second child */
  char* operator = t->children[1]->contents;
  
  /* third child in placeholder */
  long x = eval(t->children[2]);
  
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

      long result = eval(r.output);
      printf("%li\n", result);
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
