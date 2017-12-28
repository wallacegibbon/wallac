package org.wallacegibbon.cc;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class Lexer {
    private long lineNumber = 1;
    private char ch;
    private FileReader fr;

    public Lexer(String filename) throws FileNotFoundException {
        fr = new FileReader(filename);
    }

    /**
     * This method will call getToken until it reaches the end of the input file.
     * @return The collected tokens in a list
     */
    public List<Token> tokenize() throws IOException, LexerException {
        List<Token> tks = new ArrayList<Token>();
        Token tk = null;
        try {
            getChar();
            while (true) {
                tk = getToken();
                if (tk != null)
                    tks.add(tk);
            }
        } catch (EOFException e) {
            tks.add(new Token(TokenType.EOF, lineNumber));
        }
        return tks;
    }

    /**
     *
     * @return
     */
    Token getToken() throws IOException, LexerException, EOFException {
        Token tk = null;
        skipSpaces();
        if (ch == ';') {
            tk = new Token(TokenType.SEMICOLON, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ':') {
            tk = new Token(TokenType.COLON, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ',') {
            tk = new Token(TokenType.COMMA, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '?') {
            tk = new Token(TokenType.QUESTION, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '^') {
            tk = new Token(TokenType.CARET, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '~') {
            tk = new Token(TokenType.TILDE, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '*') {
            tk = new Token(TokenType.ASTERISK, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '%') {
            tk = new Token(TokenType.MOD, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '{') {
            tk = new Token(TokenType.BEGIN, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '}') {
            tk = new Token(TokenType.END, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '[') {
            tk = new Token(TokenType.OPENBR, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ']') {
            tk = new Token(TokenType.CLOSEBR, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '(') {
            tk = new Token(TokenType.OPENPA, lineNumber);
            getChar();
            return tk;
        }
        if (ch == ')') {
            tk = new Token(TokenType.CLOSEPA, lineNumber);
            getChar();
            return tk;
        }
        if (ch == '/') {
            return getDivideOrJumpComments();
        }
        if (ch == '!') {
            return getExclamationOrNeq();
        }
        if (ch == '+') {
            return getPlusOrDplus();
        }
        if (ch == '-') {
            return getMinusOrDminusOrPointsto();
        }
        if (ch == '.') {
            return getDotOrEllipsis();
        }
        if (ch == '&') {
            return getAndOrDand();
        }
        if (ch == '|') {
            return getOrOrDor();
        }
        if (ch == '=') {
            return getAssignOrEqual();
        }
        if (ch == '>') {
            return getGtGeq();
        }
        if (ch == '<') {
            return getLtLeq();
        }
        if (ch == '\'') {
            return getCharacter();
        }
        if (ch == '"') {
            return getString();
        }
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            return getIdentifier();
        }
        if ((ch >= '0' && ch <= '9')) {
            return getNumber();
        }

        throw new LexerException("Unknown character " + ch + "[" + (int) ch + "]", lineNumber);
    }

    Token getNumber() throws IOException, EOFException {
        getChar();
        return new Token(TokenType.CNUMBER, lineNumber, 0);
    }

    Token getCharacter() throws IOException, EOFException {
        getChar();
        return new Token(TokenType.CCHAR, lineNumber, ' ');
    }

    Token getString() throws IOException, EOFException {
        getChar();
        return new Token(TokenType.CSTR, lineNumber, "");
    }

    Token getIdentifier() throws IOException, EOFException {
        getChar();
        return new Token(TokenType.IDENT, lineNumber, "");
    }

    Token getGtGeq() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '=') {
            getChar();
            return new Token(TokenType.GEQ, line);
        } else {
            return new Token(TokenType.GT, line);
        }
    }

    Token getLtLeq() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '=') {
            getChar();
            return new Token(TokenType.LEQ, line);
        } else {
            return new Token(TokenType.LT, line);
        }
    }

    Token getAssignOrEqual() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '=') {
            getChar();
            return new Token(TokenType.EQ, line);
        } else {
            return new Token(TokenType.ASSIGN, line);
        }
    }

    Token getAndOrDand() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '&') {
            getChar();
            return new Token(TokenType.DAND, line);
        } else {
            return new Token(TokenType.AND, line);
        }
    }

    Token getOrOrDor() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '|') {
            getChar();
            return new Token(TokenType.DOR, line);
        } else {
            return new Token(TokenType.OR, line);
        }
    }

    Token getDotOrEllipsis() throws IOException, LexerException, EOFException {
        long line = lineNumber;
        if (getChar() == '.') {
            if (getChar() == '.') {
                getChar();
                return new Token(TokenType.ELLIPSIS, line);
            } else {
                throw new LexerException("\"..\" is not a valid c element", line);
            }
        } else {
            return new Token(TokenType.DOT, line);
        }
    }

    Token getDivideOrJumpComments() throws IOException, EOFException {
        long line = lineNumber;
        char c = getChar();
        if (c == '/') {
            jumpLineComment();
            return null;
        } else if (c == '*') {
            jumpMultiComment();
            return null;
        } else {
            return new Token(TokenType.DIVIDE, line);
        }
    }

    Token getExclamationOrNeq() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '=') {
            getChar();
            return new Token(TokenType.NEQ, line);
        } else {
            return new Token(TokenType.EXCLAMATION, line);
        }
    }

    Token getPlusOrDplus() throws IOException, EOFException {
        long line = lineNumber;
        if (getChar() == '+') {
            getChar();
            return new Token(TokenType.DPLUS, line);
        } else {
            return new Token(TokenType.PLUS, line);
        }
    }

    Token getMinusOrDminusOrPointsto() throws IOException, EOFException {
        long line = lineNumber;
        char c = getChar();
        if (c == '-') {
            getChar();
            return new Token(TokenType.DMINUS, line);
        } else if (c == '>') {
            getChar();
            return new Token(TokenType.POINTSTO, line);
        } else {
            return new Token(TokenType.MINUS, line);
        }
    }

    void jumpLineComment() throws IOException, EOFException {
        while (getChar() != '\n')
        {}
    }

    void jumpMultiComment() throws IOException, EOFException {
        getChar();
        jumpMultiCommentRecur();
        getChar();
    }

    void jumpMultiCommentRecur() throws IOException, EOFException {
        while (ch != '*') {
            getChar();
        }

        if (getChar() != '/') {
            jumpMultiCommentRecur();
        }
    }

    void skipSpaces() throws IOException, EOFException {
        while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            getChar();
        }
    }

    char getChar() throws IOException, EOFException {
        int i = fr.read();
        if (i == -1) {
            throw new EOFException();
        }
        if (i == 10) {
            lineNumber++;
        }
        ch = (char) i;
        System.out.println("***" + ch);
        return ch;
    }
}

