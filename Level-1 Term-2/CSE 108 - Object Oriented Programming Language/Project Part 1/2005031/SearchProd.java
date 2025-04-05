import java.util.*;

class SearchProd {
    public static void searchProductionCompany(){
        int readProd;
        do {
            System.out.println("Production Company Searching Options:");
            System.out.println("1) Most Recent Movies");
            System.out.println("2) Movies with the Maximum Revenue");
            System.out.println("3) Total Profit");
            System.out.println("4) List of Production Companies and the Count of their Produced Movies");
            System.out.println("5) Back to Main Menu");

            readProd=Others.intInputError();
            prodSearchOptions(readProd);
        }while(readProd!=5);
    }


    private static void prodSearchOptions(int readProd){
        List<Movie> movies = null;
        switch(readProd){
            case 1: movies=searchRecent(); break;
            case 2: movies=searchMaxRev(); break;

            case 3:
                long total=searchTotalProfit();
                if(total!=~0)
                    System.out.println("Total "+(total<0?"Loss: "+(-total):"Profit: "+total));
                break;

            case 4:
                Map<String,Integer>map=searchProdCount();
                System.out.println("Production Company : Number of Movies");
                for(Map.Entry<String,Integer> entry: map.entrySet())
                    System.out.println(entry.getKey()+" : "+entry.getValue());
                break;

            case 5: break;
            default: System.out.println("Error: The number should be between 1 and 7");
        }

        if(readProd==1||readProd==2)
            for(Movie movie:movies)
                Others.printMovieInfo(movie);
        Others.prompt();
    }


    private static List<Movie> searchRecent(){
        List<Movie> movies = new ArrayList<>(Search.searchProduction()), retMovie=new ArrayList<>();
        int year=Integer.parseInt(movies.get(0).getYear());

        for (Movie movie:movies)
            year=Math.max(year,Integer.parseInt(movie.getYear()));

        for(Movie movie:movies)
            if(Integer.parseInt(movie.getYear())==year)
                retMovie.add(movie);
        return retMovie;
    }


    private static List<Movie> searchMaxRev() {
        List<Movie> movies = new ArrayList<>(Search.searchProduction()), retMovie=new ArrayList<>();
        long rev = Long.parseLong(movies.get(0).getRevenue());

        for (Movie movie:movies)
            rev = Math.max(rev, Long.parseLong(movie.getYear()));

        for (Movie movie:movies)
            if (Long.parseLong(movie.getRevenue()) == rev)
                retMovie.add(movie);
        return retMovie;
    }

    private static long searchTotalProfit(){
        List<Movie>movies=new ArrayList<>(Search.searchProduction());
        if(movies.size()==0)
            return ~0;
        long total=0;
        for(Movie movie:movies)
            total += Long.parseLong(movie.getRevenue()) - Long.parseLong(movie.getBudget());
        return total;
    }

    private static Map<String,Integer> searchProdCount(){
        Map<String,Integer> map=new HashMap<>();
        for(Movie movie: Main_Class.movieList)
            if(map.containsKey(movie.getCompany()))
                map.put(movie.getCompany(),map.get(movie.getCompany())+1);
            else
                map.put(movie.getCompany(),1);
        return map;
    }
}
