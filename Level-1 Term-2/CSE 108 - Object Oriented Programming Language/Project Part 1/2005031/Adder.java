import java.util.Scanner;

public class Adder {
    public static void add(){
        Scanner input=new Scanner(System.in);
        String title,year,time,company,budget,revenue;
        String[] genre=new String[3];

        System.out.print("Type the title and press Enter: ");
        title=input.nextLine();
        for(Movie movie:Main_Class.movieList)
            if(title.equalsIgnoreCase(movie.getTitle())) {
                System.out.print("Title already exists. Try another one: ");
                return;
            }

        System.out.print("Type the Release Year and press Enter: ");
        int yr=Others.intInputError();
        year=String.valueOf(yr);

        System.out.print("The number of genres you will like to add (0-3): ");
        int gnr,i=0;
        do{
            gnr=Others.intInputError();
            if(gnr>=0&&gnr<=3)
                break;
            System.out.print("Please Type a number between 0 and 3: ");
        }while(true);
        for(;i<gnr;i++){
            System.out.print("Type the Genre("+i+") and press Enter (Press Enter if unavailable): ");
            genre[i]=input.nextLine();
        }
        for(;i<3;i++)
            genre[i]="";

        System.out.print("Type the Duration in minutes and press Enter: ");
        int tm=Others.intInputError();
        time=String.valueOf(tm);

        System.out.print("Enter the Production Company and press Enter: ");
        company=input.nextLine();

        System.out.print("Enter the Budget and press Enter: ");
        int bg=Others.intInputError();
        budget=String.valueOf(bg);

        System.out.print("Enter the Revenue and press Enter: ");
        int rvn=Others.intInputError();
        revenue=String.valueOf(rvn);

        Movie movie=new Movie(title+","+year+","+genre[0]+","+genre[1]+","+genre[2]+","+time+","+company
                +","+budget+","+revenue);
        Main_Class.movieList.add(movie);

        System.out.println("Movie Successfully Added!");
        Others.prompt();
    }
}
