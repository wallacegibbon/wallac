import org.wallacegibbon.cc.Lexer;
import org.wallacegibbon.cc.LexerException;

import java.io.FileNotFoundException;
import java.io.IOException;

public class TestLexer {
    public static void main(String[] args) throws FileNotFoundException, IOException {
        String filename = "src/test/Cfiles/a.c";
        Lexer l = new Lexer(filename);
        try {
            System.out.println(l.tokenize());
        } catch (LexerException e) {
            System.err.print(filename + ":" + e.getLine() + ":" + "[LEXER]" + e.getMessage());
        }
    }

}
