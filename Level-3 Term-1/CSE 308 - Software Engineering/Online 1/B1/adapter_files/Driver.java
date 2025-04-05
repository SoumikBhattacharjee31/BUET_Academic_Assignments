import adapter.parser.IntegerParser;
import adapter.parser.ParserFactory;
import adapter.parser.Parser;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.File;

public class Driver {
    public static void main(String[] args) {
       
        ParserFactory parserFactory = new ParserFactory();
        /* 
        // check if the parser is working
        Parser parser = parserFactory.getParser("Integer");
        File _f = new File("./input_integers.txt");
        System.out.println(parser.calculateSum(_f) + "\n");
        */

        // define a adapter object
        try{
            File fr = new File("./input.txt"); 
            // following line should work  when the adapter is working
            // System.out.println(adapter.calculateSum(fr));
        }
        catch( Exception e){
            System.out.println(e);
        }
    }
}