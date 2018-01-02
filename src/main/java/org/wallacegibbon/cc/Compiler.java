package org.wallacegibbon.cc;

import java.util.List;

public class Compiler
{
  public static void main(String[] args)
  {
    if (args.length != 1)
    {
      System.err.println("** The target source file is not given");
      System.exit(1);
    }
    String filename = args[0];

    List<Token> tks;
    try
    {
      tks = new Lexer(filename).tokenize();
    }
    catch (LexerException e)
    {
      System.err.println(filename + ":" + e.getLine() + ":" + "[LEXER]" + e.getMessage());
      return;
    }

    for (Token tk : tks)
    {
      System.out.println(tk);
    }

    new Parser(tks);
  }
}
