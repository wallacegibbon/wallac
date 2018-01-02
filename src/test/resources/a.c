typedef struct person { char *name; short age; int gender; long blah; } Person;

int test(int argc, char **argv, ...)
{
  char c1 = 'c', c2 = '\x65';
  char *s = "abcdefg\\hijklmn", *t = "";
  int i = 123;
  //int j = 0b101;

  printf("\"i\" is %d, and \"s\" is \"%s\"\n", i, s);
  //char *s = "abcdefghijklmn";
  /*
  char *s = "abcdefghijklmn
             invalid newline";
  */
  char *s = "test escape: \x65, \a, \t, \n, \10000a\9bc\7de\11fg";
  char *t = "a string with newline\
             like this";
  i >= 3 && i <= 5 || i%2==1;

  struct person *p = struct person { "Wallace", 27, 1 };
  p->age++ && p->age--;
  /** ok **/
  /***/
  /**/
  //
  //$errorsym
  .1,0.2,12.34,0x10,10,010;
  //12345678913123412313142341234123;
  return 0;
}