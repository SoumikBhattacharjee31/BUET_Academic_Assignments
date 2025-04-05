package adapter.parser;

public class ParserFactory {
    public ParserFactory(){

    }

    public Parser getParser(String name){
        Parser ret = null;
        if(name == "Integer"){
            ret = new IntegerParser();
        }else{
            System.out.println("Unknown type of Parser");
        }
        return ret;
    }
}
