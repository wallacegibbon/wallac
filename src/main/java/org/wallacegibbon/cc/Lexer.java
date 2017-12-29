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

    public Lexer(String filename) throws FileNotFoundException
    {
        fr = new FileReader(filename);
    }

    public List<Token> tokenize() throws IOException, LexerException
    {
        List<Token> tks = new ArrayList<Token>();
        Token tk = null;
        try
        {
            getChar();
            while (true)
            {
                tk = getToken();
                if (tk != null)
                {
                    tks.add(tk);
                }
            }
        }
        catch (EOFException e)
        {
            tks.add(new Token(TokenType.EOF, lineNumber));
        }
        return tks;
    }

    Token getToken() throws IOException, LexerException, EOFException
    {
        Token tk = null;
        skipSpaces();
        if (ch == ';')
        {
            tk = new Token(TokenType.SEMICOLON, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ':')
        {
            tk = new Token(TokenType.COLON, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ',')
        {
            tk = new Token(TokenType.COMMA, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '?')
        {
            tk = new Token(TokenType.QUESTION, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '^')
        {
            tk = new Token(TokenType.CARET, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '~')
        {
            tk = new Token(TokenType.TILDE, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '*')
        {
            tk = new Token(TokenType.ASTERISK, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '%')
        {
            tk = new Token(TokenType.MOD, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '{')
        {
            tk = new Token(TokenType.BEGIN, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '}')
        {
            tk = new Token(TokenType.END, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '[')
        {
            tk = new Token(TokenType.OPENBR, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ']')
        {
            tk = new Token(TokenType.CLOSEBR, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '(')
        {
            tk = new Token(TokenType.OPENPA, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ')')
        {
            tk = new Token(TokenType.CLOSEPA, lineNumber);
            getChar();
            return tk;
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

    boolean isIdentifier(char c)
    {
        return isAlphabet(c) || isDecimal(c) || c == '_';
    }

    boolean isAlphabet(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    boolean isDecimal(char c)
    {
        return c >= '0' && c <= '9';
    }

    boolean isOctal(char c)
    {
        return c >= '0' && c <= '7';
    }

    boolean isHex(char c)
    {
        return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    Token getNumber() throws IOException, EOFException
    {
        long line = lineNumber;
        Number n;
        if (ch == '0')
        {
            getChar();
            if (ch == 'x' || ch == 'X')
            {
                getChar();
                n = getHexadecimalNumber();
            }
            else if (isOctal(ch))
            {
                n = getOctalNumber();
            }
            else if (ch == '.')
            {
                String x = ".";
                getChar();
                while (isDecimal(ch))
                {
                    x += ch;
                    getChar();
                }
                n = Float.parseFloat(x);
            }
            else
            {
                n = 0;
            }
        }
        else
        {
            n = getDecimalNumber();
        }

        return new Token(TokenType.CNUMBER, line, n);
    }

    Number getHexadecimalNumber() throws IOException, EOFException
    {
        String n = "";
        while (isHex(ch))
        {
            n += ch;
            getChar();
        }
        return Integer.parseInt(n, 16);
    }

    Number getOctalNumber() throws IOException, EOFException
    {
        String n = "";
        while (isOctal(ch))
        {
            n += ch;
            getChar();
        }
        return Integer.parseInt(n, 8);
    }

    Number getDecimalNumber() throws IOException, EOFException
    {
        String n = "";
        while (isDecimal(ch))
        {
            n += ch;
            getChar();
        }
        if (ch == '.')
        {
            n += ch;
            getChar();
            while (isDecimal(ch))
            {
                n += ch;
                getChar();
            }
            return Float.parseFloat(n);
        }
        else
        {
            return Integer.parseInt(n);
        }

    }

    Token getCharacter() throws IOException, EOFException, LexerException
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

    Token getString() throws IOException, EOFException, LexerException
    {
        long line = lineNumber;
        String s = "";
        getChar();
        while (ch != '"')
        {
            if (ch == '\\')
            {
                s += getEscapeSequence();
            }
            else
            {
                s += ch;
                getChar();
            }
        }
        getChar();
        return new Token(TokenType.CSTR, line, s);
    }

    char getEscapeSequence() throws IOException, EOFException, LexerException
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

    int getOctalEsc() throws IOException, EOFException
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

    int getHexDigit() throws LexerException, IOException, EOFException
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


    Token getIdentifier() throws IOException, EOFException
    {
        long line = lineNumber;
        String name = "" + ch;
        while (isIdentifier(getChar()))
        {
            name += ch;
        }
        TokenType k = tryGetKeyword(name);
        if (k == null)
        {
            return new Token(TokenType.IDENT, line, name);
        }
        else
        {
            return new Token(k, line);
        }
    }

    TokenType tryGetKeyword(String identifier)
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

    Token getGtGeq() throws IOException, EOFException
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

    Token getLtLeq() throws IOException, EOFException
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

    Token getAssignOrEqual() throws IOException, EOFException
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

    Token getAndOrDand() throws IOException, EOFException
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

    Token getOrOrDor() throws IOException, EOFException
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

    Token getDotOrEllipsisOrDecimal() throws IOException, LexerException, EOFException
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
            String n = "." + c;
            getChar();
            while (isDecimal(ch))
            {
                n += ch;
                getChar();
            }
            return new Token(TokenType.CNUMBER, line, Float.parseFloat(n));
        }
        else
        {
            return new Token(TokenType.DOT, line);
        }
    }

    Token getDivideOrJumpComments() throws IOException, EOFException
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

    Token getExclamationOrNeq() throws IOException, EOFException
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

    Token getPlusOrDplus() throws IOException, EOFException
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

    Token getMinusOrDminusOrPointsto() throws IOException, EOFException
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

    void jumpLineComment() throws IOException, EOFException
    {
        while (getChar() != '\n')
        {
        }
    }

    void jumpMultiComment() throws IOException, EOFException
    {
        getChar();
        jumpMultiCommentRecur();
        getChar();
    }

    void jumpMultiCommentRecur() throws IOException, EOFException
    {
        while (ch != '*')
        {
            getChar();
        }

        if (getChar() != '/')
        {
            jumpMultiCommentRecur();
        }
    }

    void skipSpaces() throws IOException, EOFException
    {
        while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
        {
            getChar();
        }
    }

    char getChar() throws IOException, EOFException
    {
        int i = fr.read();
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

