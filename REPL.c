#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <editeline/history.h>

/* static buffer to limit input size */
static char input[2048];

int main(int argc, char** argv) {
  /* print version info and tell how to exit */
  puts("Edward Version 0.0.1\n");
  
  puts("Press Ctrl+C to exit\n");

  /* Endless loop to run main REPL */
  while (1) {
    /* prompt output */
    char* input = readline("Edward> ");
    
    /* add history */
    add_history(input);

    /* echo input */
    printf("No you're a %s", input);

    /* free retrieved input */
    free(input)
  }
  return 0;
}

