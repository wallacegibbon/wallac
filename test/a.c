struct person { char *name; int age; char gender } wallace;

int test(int argc, char **argv, ...)
{
  float f1 = 3.1415926, f2 = .15;
  char c1 = 'c', c2 = '\x65';
  char *s = "abcdefg\\hijklmn";
  printf("\"f\" is %f, and \"i\" is %d\n", f, i);
  //char *s = "abcdefghijklmn";
  /*
  char *s = "abcdefghijklmn
             invalid newline";
  */
  char *s = "test escape: \x65, \a, \n";
  char *t = "a string with newline\
             like this";
  i >= 3 && i <= 5 || i%2==1;

  struct person *p = struct person { "Wallace", 27, 1 };
  p->age++ && p->age--;

  //$errorsym
  int i = 10, j = 0x10, k = 010, l = 0x1000, m = 01000;

  return 0;
}

