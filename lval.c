/* enum of possible error typess */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

/* enum of possible lval types */
enum { LVAL_NUM, LVAL_ERR };

/* dec lval struct */
typedef struct {
  int type;
  long num;
  int err;
} lval;

/* num type lval */
lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

/* error lval */
lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

/* print an lval */
void lval_print(lval v) {
  switch (v.type) {
    /* if num print. */
    case LVAL_NUM: printf("%li", v.num); break;

    /* if error... */
    case LVAL_ERR:
      /* check which error */
      if (v.err == LERR_DIV_ZERO) { printf("Error: Division By Zero!"); }
      if (v.err == LERR_BAD_OP)   { printf("Error: Invalid Operator!"); }
      if (v.err == LERR_BAD_NUM)  { printf("Error: Invalid Number!"); }
    break;
  }
}

/* print lval + newline */
void lval_println(lval v) { lval_print(v); putchar('\n'); }
