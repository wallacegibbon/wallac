package org.wallacegibbon.cc;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class Lexer
{
  private long lineNumber = 1;
  private char ch;
  private FileReader fr;
  private String filename;

  public Lexer(String filename) throws LexerException
  {
    this.filename = filename;
    try
    {
      fr = new FileReader(filename);
    }
    catch (FileNotFoundException e)
    {
      throw new LexerException("File not found", 0);
    }
  }

  public List<Token> tokenize() throws LexerException
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
      throw new LexerException("Failed closing " + filename, 0);
    }

    return tks;
  }

  private Token getToken() throws LexerException, EOFException
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
    throw new LexerException("Unknown character " + ch + "[" + (int) ch + "]", lineNumber);
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

  private Token getNumber() throws EOFException, LexerException
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

  private static long parseInt(String intStr, int radix, long line) throws LexerException
  {
    try
    {
      return Long.parseLong(intStr, radix);
    }
    catch (NumberFormatException e)
    {
      throw new LexerException("Invalid int literal " + intStr, line);
    }
  }

  private static Double parseFloat(String floatStr, long line) throws LexerException
  {
    try
    {
      return Double.parseDouble(floatStr);
    }
    catch (NumberFormatException e)
    {
      throw new LexerException("Invalid float literal " + floatStr, line);
    }
  }

  private Token getCharacter() throws EOFException, LexerException
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
      throw new LexerException("Invalid character literal", line);
    }
  }

  private Token getString() throws EOFException, LexerException
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

  private char getEscapeSequence() throws EOFException, LexerException
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

  private int getOctalEsc() throws LexerException, EOFException
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

  private int getHexDigit() throws LexerException, EOFException
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
      throw new LexerException("Invalid hex digit", line);
    }
  }


  private Token getIdentifier() throws LexerException, EOFException
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
    if (identifier.equals("if"))
    {
      return TokenType.KW_IF;
    }
    if (identifier.equals("else"))
    {
      return TokenType.KW_ELSE;
    }
    if (identifier.equals("while"))
    {
      return TokenType.KW_WHILE;
    }
    if (identifier.equals("switch"))
    {
      return TokenType.KW_SWITCH;
    }
    if (identifier.equals("case"))
    {
      return TokenType.KW_CASE;
    }
    if (identifier.equals("default"))
    {
      return TokenType.KW_DEFAULT;
    }
    if (identifier.equals("for"))
    {
      return TokenType.KW_FOR;
    }
    if (identifier.equals("do"))
    {
      return TokenType.KW_DO;
    }
    if (identifier.equals("return"))
    {
      return TokenType.KW_RETURN;
    }
    if (identifier.equals("goto"))
    {
      return TokenType.KW_GOTO;
    }
    if (identifier.equals("continue"))
    {
      return TokenType.KW_CONTINUE;
    }
    if (identifier.equals("break"))
    {
      return TokenType.KW_BREAK;
    }
    if (identifier.equals("void"))
    {
      return TokenType.KW_VIOD;
    }
    if (identifier.equals("char"))
    {
      return TokenType.KW_CHAR;
    }
    if (identifier.equals("int"))
    {
      return TokenType.KW_INT;
    }
    if (identifier.equals("float"))
    {
      return TokenType.KW_FLOAT;
    }
    if (identifier.equals("double"))
    {
      return TokenType.KW_DOUBLE;
    }
    if (identifier.equals("short"))
    {
      return TokenType.KW_SHORT;
    }
    if (identifier.equals("long"))
    {
      return TokenType.KW_LONG;
    }
    if (identifier.equals("signed"))
    {
      return TokenType.KW_SIGNED;
    }
    if (identifier.equals("unsigned"))
    {
      return TokenType.KW_UNSIGNED;
    }
    if (identifier.equals("struct"))
    {
      return TokenType.KW_STRUCT;
    }
    if (identifier.equals("union"))
    {
      return TokenType.KW_UNION;
    }
    if (identifier.equals("enum"))
    {
      return TokenType.KW_ENUM;
    }
    if (identifier.equals("typedef"))
    {
      return TokenType.KW_TYPEDEF;
    }
    if (identifier.equals("sizeof"))
    {
      return TokenType.KW_SIZEOF;
    }
    if (identifier.equals("extern"))
    {
      return TokenType.KW_EXTERN;
    }
    if (identifier.equals("volatile"))
    {
      return TokenType.KW_VOLATILE;
    }
    if (identifier.equals("const"))
    {
      return TokenType.KW_CONST;
    }
    if (identifier.equals("auto"))
    {
      return TokenType.KW_AUTO;
    }
    if (identifier.equals("static"))
    {
      return TokenType.KW_STATIC;
    }
    if (identifier.equals("register"))
    {
      return TokenType.KW_REGISTER;
    }
    return null;
  }

  private Token getGtGeq() throws LexerException, EOFException
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

  private Token getLtLeq() throws LexerException, EOFException
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

  private Token getAssignOrEqual() throws LexerException, EOFException
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

  private Token getAndOrDand() throws LexerException, EOFException
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

  private Token getOrOrDor() throws LexerException, EOFException
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

  private Token getDotOrEllipsisOrDecimal() throws LexerException, EOFException
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
        throw new LexerException("\"..\" is not a valid c element", line);
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

  private Token getDivideOrJumpComments() throws LexerException, EOFException
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

  private Token getExclamationOrNeq() throws LexerException, EOFException
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

  private Token getPlusOrDplus() throws LexerException, EOFException
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

  private Token getMinusOrDminusOrPointsto() throws LexerException, EOFException
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

  private void jumpLineComment() throws LexerException, EOFException
  {
    while (getChar() != '\n')
    {
    }
  }

  private void jumpMultiComment() throws LexerException, EOFException
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

  private void skipSpaces() throws LexerException, EOFException
  {
    while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
    {
      getChar();
    }
  }

  private char getChar() throws LexerException, EOFException
  {
    int i;
    try
    {
      i = fr.read();
    }
    catch (IOException e)
    {
      throw new LexerException(e.getMessage(), lineNumber);
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

