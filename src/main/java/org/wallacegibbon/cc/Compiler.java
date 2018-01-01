package org.wallacegibbon.cc;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;

public class Compiler
{
  public static void main(String[] args)
  {
    String filename = "src/test/resources/a.c";

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
    catch (FileNotFoundException e)
    {
      System.err.println("Failed reading " + filename + ": " + e.getMessage());
      return;
    }

    for (Token tk : tks)
    {
      System.out.println(tk);
    }

    new Parser(tks);
  }

  private static void checkArgs(String[] args)
  {
    if (args.length != 3)
    {
      System.err.println("Usage: java wallac myfile.c");
      System.exit(1);
    }
  }
}
