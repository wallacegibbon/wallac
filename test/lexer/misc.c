void
test(char *fmt, ...)
{
  char c1, *s, *t;
  c1 = 'c', '\x65', 123;
  s = "escape,\what? hex:\x654, octal:\7a\11b\14567\nOther esc\t\n\
       new line";
  printf("\"i\" is %d, and \"s\" is \"%s\"\n", i, s);
  p->age++ && --p->age || **p->x%2==1;
  /***************
   * this is a long comment
   * line 2
   */
  /** ok **/
  /***/
  /**/
  //
  //$errorsym
  //.2;
  //0.2;
}
