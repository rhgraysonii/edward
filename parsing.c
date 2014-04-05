#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs("lispy> ", stdout);
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

int main(int argc, char** argv) {
  
  /* initial parser creation */
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Edward   = mpc_new("lispy");
  
  /* parser language defintions */
  mpca_lang(MPC_LANG_DEFAULT,
    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      edward   : /^/ <operator> <expr>+ /$/ ;             \
    ",
    Number, Operator, Expr, Edward);
  
  puts("Edward Version 0.0.0.0.2");
  puts("Press Ctrl+c to Exit\n");
  
  while (1) {
  
    char* input = readline("lispy> ");
    add_history(input);
    
    /* parse user input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Edward, &r)) {
      /* when successful print and delete the AST */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* else, print and delete error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    free(input);
  }
  
  /* undefine/delete parsers */
  mpc_cleanup(4, Number, Operator, Expr, Edward);
  
  return 0;
}