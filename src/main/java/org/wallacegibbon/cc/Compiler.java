package org.wallacegibbon.cc;

import java.io.IOException;
import java.util.List;

public class Compiler
{
    public static void main(String[] args)
    {
//        checkArgs(args);
//        String filename = args[3];
        String filename = "src/test/Cfiles/a.c";

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
        catch (IOException e)
        {
            System.err.print("Failed reading file " + filename + ": " + e.getMessage());
            return;
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
