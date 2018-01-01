package org.wallacegibbon.cc;

public class LexerException extends Exception
{
  private long line;

  LexerException(String message, long line)
  {
    super(message);
    this.line = line;
  }

  public long getLine()
  {
    return line;
  }
}
