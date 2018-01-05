package org.wallacegibbon.cc;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class Scanner
{
  private long lineNumber = 1;
  private char ch;
  private FileReader fr;
  private String filename;

  public Scanner(String filename) throws ScannerException
  {
    this.filename = filename;
    try
    {
      fr = new FileReader(filename);
    }
    catch (FileNotFoundException e)
    {
      throw new ScannerException("File not found", 0);
    }
  }

  public List<Token> tokenize() throws ScannerException
  {
    List<Token> tks = new ArrayList<Token>();
    try
    {
      getChar();
      while (true)
      {
        Token t = getToken();
        if (t != null)
        {
          tks.add(t);
        }
      }
    }
    catch (EOFException e)
    {
      tks.add(new Token(TokenType.EOF, lineNumber));
    }

    try
    {
      fr.close();
    }
    catch (IOException e)
    {
      throw new ScannerException("Failed closing " + filename, 0);
    }

    return tks;
  }

  private Token getToken() throws ScannerException, EOFException
  {
    skipSpaces();
    if (ch == ';')
    {
      Token t = new Token(TokenType.SEMICOLON, lineNumber);
      getChar();
      return t;
    }
    if (ch == ':')
    {
      Token t = new Token(TokenType.COLON, lineNumber);
      getChar();
      return t;
    }
    if (ch == ',')
    {
      Token t = new Token(TokenType.COMMA, lineNumber);
      getChar();
      return t;
    }
    if (ch == '?')
    {
      Token t = new Token(TokenType.QUESTION, lineNumber);
      getChar();
      return t;
    }
    if (ch == '^')
    {
      Token t = new Token(TokenType.CARET, lineNumber);
      getChar();
      return t;
    }
    if (ch == '~')
    {
      Token t = new Token(TokenType.TILDE, lineNumber);
      getChar();
      return t;
    }
    if (ch == '*')
    {
      Token t = new Token(TokenType.ASTERISK, lineNumber);
      getChar();
      return t;
    }
    if (ch == '%')
    {
      Token t = new Token(TokenType.MOD, lineNumber);
      getChar();
      return t;
    }
    if (ch == '{')
    {
      Token t = new Token(TokenType.BEGIN, lineNumber);
      getChar();
      return t;
    }
    if (ch == '}')
    {
      Token t = new Token(TokenType.END, lineNumber);
      getChar();
      return t;
    }
    if (ch == '[')
    {
      Token t = new Token(TokenType.OPENBR, lineNumber);
      getChar();
      return t;
    }
    if (ch == ']')
    {
      Token t = new Token(TokenType.CLOSEBR, lineNumber);
      getChar();
      return t;
    }
    if (ch == '(')
    {
      Token t = new Token(TokenType.OPENPA, lineNumber);
      getChar();
      return t;
    }
    if (ch == ')')
    {
      Token t = new Token(TokenType.CLOSEPA, lineNumber);
      getChar();
      return t;
    }
    if (ch == '/')
    {
      return getDivideOrJumpComments();
    }
    if (ch == '!')
    {
      return getExclamationOrNeq();
    }
    if (ch == '+')
    {
      return getPlusOrDplus();
    }
    if (ch == '-')
    {
      return getMinusOrDminusOrPointsto();
    }
    if (ch == '.')
    {
      return getDotOrEllipsisOrDecimal();
    }
    if (ch == '&')
    {
      return getAndOrDand();
    }
    if (ch == '|')
    {
      return getOrOrDor();
    }
    if (ch == '=')
    {
      return getAssignOrEqual();
    }
    if (ch == '>')
    {
      return getGtGeq();
    }
    if (ch == '<')
    {
      return getLtLeq();
    }
    if (ch == '\'')
    {
      return getCharacter();
    }
    if (ch == '"')
    {
      return getString();
    }
    if (isAlphabet(ch) || ch == '_')
    {
      return getIdentifier();
    }
    if (isDecimal(ch))
    {
      return getNumber();
    }
    throw new ScannerException("Unknown character " + ch + "[" + (int) ch + "]", lineNumber);
  }

  private boolean isIdentifier(char c)
  {
    return isAlphabet(c) || isDecimal(c) || c == '_';
  }

  private boolean isAlphabet(char c)
  {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }

  private boolean isDecimal(char c)
  {
    return c >= '0' && c <= '9';
  }

  private boolean isOctal(char c)
  {
    return c >= '0' && c <= '7';
  }

  private boolean isHex(char c)
  {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
  }

  private Token getNumber() throws EOFException, ScannerException
  {
    long line = lineNumber;
    Number n;
    if (ch == '0')
    {
      getChar();
      if (ch == 'x' || ch == 'X')
      {
        StringBuilder x = new StringBuilder();
        getChar();
        while (isHex(ch))
        {
          x.append(ch);
          getChar();
        }
        n = parseInt(x.toString(), 16, line);
      }
      else if (isOctal(ch))
      {
        StringBuilder x = new StringBuilder();
        while (isOctal(ch))
        {
          x.append(ch);
          getChar();
        }
        n = parseInt(x.toString(), 8, line);
      }
      else if (ch == '.')
      {
        StringBuilder x = new StringBuilder(".");
        getChar();
        while (isDecimal(ch))
        {
          x.append(ch);
          getChar();
        }
        n = parseFloat(x.toString(), line);
      }
      else
      {
        n = 0;
      }
    }
    else
    {
      StringBuilder x = new StringBuilder();
      while (isDecimal(ch))
      {
        x.append(ch);
        getChar();
      }
      if (ch == '.')
      {
        x.append(ch);
        getChar();
        while (isDecimal(ch))
        {
          x.append(ch);
          getChar();
        }
        n = parseFloat(x.toString(), line);
      }
      else
      {
        n = parseInt(x.toString(), 10, line);
      }
    }
    return new Token(TokenType.CNUMBER, line, n);
  }

  private static long parseInt(String intStr, int radix, long line) throws ScannerException
  {
    try
    {
      return Long.parseLong(intStr, radix);
    }
    catch (NumberFormatException e)
    {
      throw new ScannerException("Invalid int literal " + intStr, line);
    }
  }

  private static Double parseFloat(String floatStr, long line) throws ScannerException
  {
    try
    {
      return Double.parseDouble(floatStr);
    }
    catch (NumberFormatException e)
    {
      throw new ScannerException("Invalid float literal " + floatStr, line);
    }
  }

  private Token getCharacter() throws EOFException, ScannerException
  {
    long line = lineNumber;
    char c = getChar();
    if (c == '\\')
    {
      c = getEscapeSequence();
    }
    else
    {
      getChar();
    }

    if (ch == '\'')
    {
      getChar();
      return new Token(TokenType.CCHAR, line, c);
    }
    else
    {
      throw new ScannerException("Invalid character literal", line);
    }
  }

  private Token getString() throws EOFException, ScannerException
  {
    long line = lineNumber;
    StringBuilder s = new StringBuilder();
    getChar();
    while (ch != '"')
    {
      if (ch == '\\')
      {
        s.append(getEscapeSequence());
      }
      else
      {
        s.append(ch);
        getChar();
      }
    }
    getChar();
    return new Token(TokenType.CSTR, line, s.toString());
  }

  private char getEscapeSequence() throws EOFException, ScannerException
  {
    char c;
    switch (getChar())
    {
    case 'x':
      c = (char) (getHexDigit() * 16 + getHexDigit());
      getChar();
      return c;
    case 'a':
      getChar();
      return 7;
    case 'b':
      getChar();
      return 8;
    case 't':
      getChar();
      return 9;
    case 'n':
      getChar();
      return 10;
    case 'v':
      getChar();
      return 11;
    case 'f':
      getChar();
      return 12;
    case 'r':
      getChar();
      return 13;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      c = (char) getOctalEsc();
      return c;
    default:
      c = ch;
      getChar();
      return c;
    }
  }

  private int getOctalEsc() throws ScannerException, EOFException
  {
    String s = "" + ch;
    getChar();
    if (isOctal(ch))
    {
      s += ch;
      getChar();
      if (isOctal(ch))
      {
        s += ch;
        getChar();
      }
    }
    return Integer.parseInt(s, 8);
  }

  private int getHexDigit() throws ScannerException, EOFException
  {
    long line = lineNumber;
    char c = getChar();
    if (c >= 'a' && c <= 'f')
    {
      return c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
      return c - 'A' + 10;
    }
    else if (c >= '0' && c <= '9')
    {
      return c - '0';
    }
    else
    {
      throw new ScannerException("Invalid hex digit", line);
    }
  }

  private Token getIdentifier() throws ScannerException, EOFException
  {
    long line = lineNumber;
    StringBuilder name = new StringBuilder();
    name.append(ch);
    while (isIdentifier(getChar()))
    {
      name.append(ch);
    }
    TokenType k = tryGetKeyword(name.toString());
    if (k == null)
    {
      return new Token(TokenType.IDENT, line, name.toString());
    }
    else
    {
      return new Token(k, line);
    }
  }

  private TokenType tryGetKeyword(String identifier)
  {
    switch (identifier)
    {
    case "if":
      return TokenType.KW_IF;
    case "else":
      return TokenType.KW_ELSE;
    case "while":
      return TokenType.KW_WHILE;
    case "switch":
      return TokenType.KW_SWITCH;
    case "case":
      return TokenType.KW_CASE;
    case "default":
      return TokenType.KW_DEFAULT;
    case "for":
      return TokenType.KW_FOR;
    case "do":
      return TokenType.KW_DO;
    case "return":
      return TokenType.KW_RETURN;
    case "goto":
      return TokenType.KW_GOTO;
    case "continue":
      return TokenType.KW_CONTINUE;
    case "break":
      return TokenType.KW_BREAK;
    case "void":
      return TokenType.KW_VIOD;
    case "char":
      return TokenType.KW_CHAR;
    case "int":
      return TokenType.KW_INT;
    case "float":
      return TokenType.KW_FLOAT;
    case "double":
      return TokenType.KW_DOUBLE;
    case "short":
      return TokenType.KW_SHORT;
    case "long":
      return TokenType.KW_LONG;
    case "signed":
      return TokenType.KW_SIGNED;
    case "unsigned":
      return TokenType.KW_UNSIGNED;
    case "struct":
      return TokenType.KW_STRUCT;
    case "union":
      return TokenType.KW_UNION;
    case "enum":
      return TokenType.KW_ENUM;
    case "typedef":
      return TokenType.KW_TYPEDEF;
    case "sizeof":
      return TokenType.KW_SIZEOF;
    case "extern":
      return TokenType.KW_EXTERN;
    case "volatile":
      return TokenType.KW_VOLATILE;
    case "const":
      return TokenType.KW_CONST;
    case "auto":
      return TokenType.KW_AUTO;
    case "static":
      return TokenType.KW_STATIC;
    case "register":
      return TokenType.KW_REGISTER;
    default:
      return null;
    }
  }

  private Token getGtGeq() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '=')
    {
      getChar();
      return new Token(TokenType.GEQ, line);
    }
    else
    {
      return new Token(TokenType.GT, line);
    }
  }

  private Token getLtLeq() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '=')
    {
      getChar();
      return new Token(TokenType.LEQ, line);
    }
    else
    {
      return new Token(TokenType.LT, line);
    }
  }

  private Token getAssignOrEqual() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '=')
    {
      getChar();
      return new Token(TokenType.EQ, line);
    }
    else
    {
      return new Token(TokenType.ASSIGN, line);
    }
  }

  private Token getAndOrDand() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '&')
    {
      getChar();
      return new Token(TokenType.DAND, line);
    }
    else
    {
      return new Token(TokenType.AND, line);
    }
  }

  private Token getOrOrDor() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '|')
    {
      getChar();
      return new Token(TokenType.DOR, line);
    }
    else
    {
      return new Token(TokenType.OR, line);
    }
  }

  private Token getDotOrEllipsisOrDecimal() throws ScannerException, EOFException
  {
    long line = lineNumber;
    char c = getChar();
    if (c == '.')
    {
      if (getChar() == '.')
      {
        getChar();
        return new Token(TokenType.ELLIPSIS, line);
      }
      else
      {
        throw new ScannerException("\"..\" is not a valid c element", line);
      }
    }
    else if (isDecimal(c))
    {
      StringBuilder x = new StringBuilder(".");
      x.append(c);
      getChar();
      while (isDecimal(ch))
      {
        x.append(ch);
        getChar();
      }
      return new Token(TokenType.CNUMBER, line, parseFloat(x.toString(), line));
    }
    else
    {
      return new Token(TokenType.DOT, line);
    }
  }

  private Token getDivideOrJumpComments() throws ScannerException, EOFException
  {
    long line = lineNumber;
    char c = getChar();
    if (c == '/')
    {
      jumpLineComment();
      return null;
    }
    else if (c == '*')
    {
      jumpMultiComment();
      return null;
    }
    else
    {
      return new Token(TokenType.DIVIDE, line);
    }
  }

  private Token getExclamationOrNeq() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '=')
    {
      getChar();
      return new Token(TokenType.NEQ, line);
    }
    else
    {
      return new Token(TokenType.EXCLAMATION, line);
    }
  }

  private Token getPlusOrDplus() throws ScannerException, EOFException
  {
    long line = lineNumber;
    if (getChar() == '+')
    {
      getChar();
      return new Token(TokenType.DPLUS, line);
    }
    else
    {
      return new Token(TokenType.PLUS, line);
    }
  }

  private Token getMinusOrDminusOrPointsto() throws ScannerException, EOFException
  {
    long line = lineNumber;
    char c = getChar();
    if (c == '-')
    {
      getChar();
      return new Token(TokenType.DMINUS, line);
    }
    else if (c == '>')
    {
      getChar();
      return new Token(TokenType.POINTSTO, line);
    }
    else
    {
      return new Token(TokenType.MINUS, line);
    }
  }

  private void jumpLineComment() throws ScannerException, EOFException
  {
    while (getChar() != '\n')
    {
    }
  }

  private void jumpMultiComment() throws ScannerException, EOFException
  {
    char c1;
    char c2 = getChar();
    do
    {
      c1 = c2;
      c2 = getChar();
    }
    while (!(c1 == '*' && c2 == '/'));
    getChar();
  }

  private void skipSpaces() throws ScannerException, EOFException
  {
    while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
    {
      getChar();
    }
  }

  private char getChar() throws ScannerException, EOFException
  {
    int i;
    try
    {
      i = fr.read();
    }
    catch (IOException e)
    {
      throw new ScannerException(e.getMessage(), lineNumber);
    }

    if (i == -1)
    {
      throw new EOFException();
    }
    if (i == 10)
    {
      lineNumber++;
    }
    ch = (char) i;
    return ch;
  }
}

