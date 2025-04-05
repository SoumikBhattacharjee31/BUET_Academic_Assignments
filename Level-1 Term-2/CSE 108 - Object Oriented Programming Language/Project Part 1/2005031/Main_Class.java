import java.util.*;
import java.io.*;

public class Main_Class {

    private static final String INPUT_FILE_NAME = "movies.txt";
    private static final String OUTPUT_FILE_NAME = "movies.txt";
    public static List<Movie> movieList = new ArrayList<>();

    private static void input() throws Exception{
        BufferedReader br = new BufferedReader(new FileReader(INPUT_FILE_NAME));
        while(true){
            String line = br.readLine();
            if (line == null) break;
            Movie movie=new Movie(line);
            movieList.add(movie);
        }
        br.close();
    }


    private static void mainMenu(){
        int readMain;
        do {
            System.out.println("\nIMDB RIPOFF");
            System.out.println("Main Menu");
            System.out.println("1) Search Movies");
            System.out.println("2) Search Production Companies");
            System.out.println("3) Add Movie");
            System.out.println("4) Exit System");

            readMain=Others.intInputError();
            mainOptions(readMain);
        }while(readMain!=4);
    }


    private static void mainOptions(int readMain){
        switch(readMain){
            case 1: Search.searchMovie(); break;
            case 2: SearchProd.searchProductionCompany(); break;
            case 3: Adder.add(); break;
            case 4: break;
            default:
                System.out.println("Error: The number should be between 1 and 4");
                Others.prompt();
        }
    }


    private static void exit()throws Exception{
        BufferedWriter bw = new BufferedWriter(new FileWriter(OUTPUT_FILE_NAME));
        for(Movie movie:movieList){
            String text=movie.getTitle()+","+movie.getYear()+","+movie.getGenre()[0]+","
                    +movie.getGenre()[1]+","+movie.getGenre()[2]+","+movie.getTime()+","
                    +movie.getCompany()+","+movie.getBudget()+","+movie.getRevenue();
            bw.write(text);
            bw.write(System.lineSeparator());
        }
        bw.close();
    }


    public static void main(String[] args) throws Exception{
        input();
        mainMenu();
        exit();
    }
}
