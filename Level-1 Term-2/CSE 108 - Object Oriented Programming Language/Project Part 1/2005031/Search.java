import java.util.*;

class Search {
    public static int count =0;

    public static void searchMovie(){
        int readSearch;
        do {
            System.out.println("Movie Searching Options:");
            System.out.println("1) By Movie Title");
            System.out.println("2) By Release Year");
            System.out.println("3) By Genre");
            System.out.println("4) By Production Company");
            System.out.println("5) By Running Time");
            System.out.println("6) Top 10 Movies");
            System.out.println("7) Back to Main Menu");

            readSearch=Others.intInputError();
            searchOptions(readSearch);
        }while(readSearch!=7);
    }


    private static void searchOptions(int readSearch){
        List<Movie> movies = null;
        count=0;
        switch(readSearch){
            case 1: movies=searchTitle(); break;
            case 2: movies=searchYear(); break;
            case 3: movies=searchGenre(); break;
            case 4: movies=searchProduction(); break;
            case 5: movies=searchTime(); break;
            case 6: movies=searchTop10(); break;
            case 7: break;
            default: System.out.println("Error: The number should be between 1 and 7");
        }

        if(movies!=null)
            for(Movie movie: movies)
                Others.printMovieInfo(movie);
        Others.prompt();
    }


    private static List<Movie> searchTitle(){
        Scanner input=new Scanner(System.in);
        List<Movie> movies = new ArrayList<>();
        System.out.print("Please type the name of the movie: ");
        String title=input.nextLine();

        for(Movie movie:Main_Class.movieList)
            if(movie.getTitle().equalsIgnoreCase(title)){
                movies.add(movie);
                break;
            }

        if(movies.size()==0)
            System.out.println("There is no movie with this title");
        return movies;
    }


    private static List<Movie> searchYear(){
        Scanner input=new Scanner(System.in);
        List<Movie> movies = new ArrayList<>();
        System.out.print("Please type the year when the movie was released: ");
        String year=input.nextLine();

        for(Movie movie:Main_Class.movieList)
            if(movie.getYear().equals(year))
                movies.add(movie);

        if(movies.size()==0)
            System.out.println("There is no movie with this year");
        return movies;
    }


    private static List<Movie> searchGenre(){
        Scanner input=new Scanner(System.in);
        List<Movie> movies = new ArrayList<>();
        System.out.print("Please type the genre of the movie: ");
        String genre=input.nextLine();

        for(Movie movie: Main_Class.movieList)
            if(movie.getGenre()[0].equalsIgnoreCase(genre)||
                    movie.getGenre()[1].equalsIgnoreCase(genre)||
                    movie.getGenre()[2].equalsIgnoreCase(genre))
                movies.add(movie);

        if(movies.size()==0)
            System.out.println("There is no movie with this genre");
        return movies;
    }


    public static List<Movie> searchProduction(){
        Scanner input=new Scanner(System.in);
        List<Movie> movies = new ArrayList<>();
        System.out.print("Please type the name of the production company of the movie: ");
        String company=input.nextLine();

        for(Movie movie: Main_Class.movieList)
            if(movie.getCompany().equalsIgnoreCase(company))
                movies.add(movie);

        if(movies.size()==0)
            System.out.println("There is no movie with this production company");
        return movies;
    }


    private static List<Movie> searchTime(){
        List<Movie> movies = new ArrayList<>();
        System.out.print("Please type the duration of the movie:- From: ");
        int from=Others.intInputError();
        System.out.print("To: ");
        int to=Others.intInputError();

        for(Movie movie: Main_Class.movieList){
            int dur=Integer.parseInt(movie.getTime(),10);
            if(dur>=from && dur<=to)
                movies.add(movie);
        }
        if(movies.size()==0)

            System.out.println("There is no movie with duration in this range");
        return movies;
    }

    private static List<Movie> searchTop10(){
        Movie []movies=new Movie[Main_Class.movieList.size()];
        List<Movie> moviesRet=new ArrayList<>();
        Movie temp;
        int i=0,j;

        for(Movie movie: Main_Class.movieList)
            movies[i++]=movie;

        for(i=0; i<Main_Class.movieList.size(); i++)
            for(j=i+1;j<Main_Class.movieList.size();j++){
                int difI=Integer.parseInt(movies[i].getRevenue())-Integer.parseInt(movies[i].getBudget());
                int difJ=Integer.parseInt(movies[j].getRevenue())-Integer.parseInt(movies[j].getBudget());
                if (difI<difJ) {
                    temp = movies[i];
                    movies[i] = movies[j];
                    movies[j] = temp;
                }
            }

        for(i=0;i<10&&i<Main_Class.movieList.size();i++)
            moviesRet.add(movies[i]);
        return moviesRet;
    }
}
