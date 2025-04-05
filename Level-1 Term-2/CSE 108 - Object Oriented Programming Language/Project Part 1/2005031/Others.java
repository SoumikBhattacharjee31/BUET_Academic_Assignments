import java.util.*;

class Others {
    public static void printMovieInfo(Movie movie){
        System.out.println("Title: "+movie.getTitle());
        System.out.println("Year of Release: "+movie.getYear());
        System.out.print("Genre: ");

        int []mask=new int[2];
        if(!movie.getGenre()[0].equals("")) {
            System.out.print(movie.getGenre()[0]);
            mask[0]=1;
        }
        if(!movie.getGenre()[1].equals("")) {
            if(mask[0]==1)
                System.out.print(", ");
            System.out.print(movie.getGenre()[1]);
            mask[1]=1;
        }
        if(!movie.getGenre()[2].equals("")) {
            if(mask[1]==1||mask[0]==1)
                System.out.print(", ");
            System.out.print(movie.getGenre()[2]);
        }

        System.out.println("\nRunning Time: "+movie.getTime()+" minutes");
        System.out.println("Production Company: "+movie.getCompany());
        System.out.println("Budget: "+movie.getBudget());
        System.out.println("Revenue: "+movie.getRevenue()+"\n");
    }

    public static int intInputError(){
        int num;
        while(true){
            try{
                Scanner input=new Scanner(System.in);
                num=input.nextInt();
                break;
            }catch(InputMismatchException e){
                System.out.println("INVALID INPUT. Please type a number and press Enter: ");
            }
        }
        return num;
    }

    public static void prompt(){
        Scanner input=new Scanner(System.in);
        System.out.println("Press Enter to continue");
        input.nextLine();
    }
}
