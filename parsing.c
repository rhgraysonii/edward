#include <stdio.h>
#include <stdlib.h>

/* If we are compiling on Windows compile these functions */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
  fputs("lispy> ", stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}
/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#endif



int main(int argc, char** argv) {
     
  /* version/exit info */
  puts("Edward Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");
         
  /* endless loop for main REPL */
  while (1) {
    /* output prompt and read line */
    char* input = readline("edward> ");
                  
    /* put input in history  */
    add_history(input);
                      
    /* Echo input back */    
    printf("No you're a %s\n", input);

    /* free input */
    free(input);
  }
  return 0;
}
