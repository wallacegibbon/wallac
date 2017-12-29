package org.wallacegibbon.cc;

import java.io.IOException;
import java.util.List;

public class Compiler
{
    public static void main(String[] args) throws IOException, LexerException
    {
//        checkArgs(args);
//        String filename = args[3];
        String filename = "src/test/Cfiles/a.c";

        List<Token> tks = null;
        try
        {
            tks = new Lexer(filename).tokenize();
        }
        catch (LexerException e)
        {
            System.err.println(filename + ":" + e.getLine() + ":" + "[LEXER]" + e.getMessage());
            System.exit(1);
        }

        for (Token tk : tks)
        {
            System.out.println(tk);
        }

        new Parser(tks);
    }

    static void checkArgs(String[] args)
    {
        if (args.length != 3)
        {
            System.err.println("Usage: java wallac myfile.c");
            System.exit(1);
        }
    }
}
