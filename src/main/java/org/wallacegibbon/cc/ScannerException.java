package org.wallacegibbon.cc;

public class ScannerException extends Exception
{
  private long line;

  ScannerException(String message, long line)
  {
    super(message);
    this.line = line;
  }

  public long getLine()
  {
    return line;
  }
}
