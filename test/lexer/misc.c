struct person { char *name; short age; int gender; long blah; };
struct person x;

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
  char *s = "test escape, hex:\x654, octal:\7a\11b\14567\nOther esc\t\n";
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
  //.2;
  //0.2;

  return 0;
}

typedef int a;
