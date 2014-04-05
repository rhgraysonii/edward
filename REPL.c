#include <stdio.h>

/* static buffer to limit input size */
static char input[2048];

int main(int argc, char** argv) {
  /* print version info and tell how to exit */
  puts("Edward Version 0.0.1\n");
  puts("Press Ctrl+C to exit\n");

  /* Endless loop to run main REPL */
  while (1) {
    /* prompt output */
    fputs("edward", stdout);

    /* check user input size to compare with buffer */
    fgets(input, 2048, stdin);

    /* echo input */
    printf("No you're a %s", input);
  }

  return 0;
}

