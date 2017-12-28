import org.wallacegibbon.cc.Lexer;
import org.wallacegibbon.cc.LexerException;
import org.wallacegibbon.cc.Token;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.List;

public class TestLexer
{
    public static void main(String[] args) throws FileNotFoundException, IOException
    {
        String filename = "src/test/Cfiles/a.c";
//        String filename = "src/test/Cfiles/kw.c";
        try
        {
            List<Token> l = new Lexer(filename).tokenize();
            for (int i = 0; i < l.size(); i++)
            {
                System.out.println(l.get(i));
            }
        }
        catch (LexerException e)
        {
            System.err.print(filename + ":" + e.getLine() + ":" + "[LEXER]" + e.getMessage());
        }
    }

}
