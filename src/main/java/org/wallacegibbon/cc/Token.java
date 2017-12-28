package org.wallacegibbon.cc;

public class Token
{
    private TokenType type;
    private long lineNumber;
    private String s = "";
    private char c = ' ';
    private Number n = 0;

    Token(TokenType type, long lineNumber, String s)
    {
        this.type = type;
        this.lineNumber = lineNumber;
        this.s = s;
    }

    Token(TokenType type, long lineNumber, Number n)
    {
        this.type = type;
        this.lineNumber = lineNumber;
        this.n = n;
    }

    Token(TokenType type, long lineNumber, char c)
    {
        this.type = type;
        this.lineNumber = lineNumber;
        this.c = c;
    }

    Token(TokenType type, long lineNumber)
    {
        this.type = type;
        this.lineNumber = lineNumber;
    }

    @Override
    public String toString()
    {
        return "(" + this.getLineNumber() + ")" +
                this.getType().toString() + "," +
                "<" + this.getS() + ">," +
                this.getC() + "," +
                this.getN();
    }

    public TokenType getType()
    {
        return type;
    }

    public void setType(TokenType type)
    {
        this.type = type;
    }

    public long getLineNumber()
    {
        return lineNumber;
    }

    public void setLineNumber(long lineNumber)
    {
        this.lineNumber = lineNumber;
    }

    public String getS()
    {
        return s;
    }

    public void setS(String s)
    {
        this.s = s;
    }

    public char getC()
    {
        return c;
    }

    public void setC(char c)
    {
        this.c = c;
    }

    public Number getN()
    {
        return n;
    }

    public void setN(Number n)
    {
        this.n = n;
    }
}
