package adapter.parser;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Scanner;

public class IntegerParser implements Parser {
    private FileReader fr;
    private BufferedReader br;
    private Scanner sc;

    public IntegerParser() {
        br = null;
        sc = null;
    }

    @Override
    public int calculateSum(File file) {
        if (file == null) {
            System.out.println("Please provide a valid file");
            return -1;
        }
        int sum = 0;
        try {
            this.fr = new FileReader(file);

            this.br = new BufferedReader(fr);
            this.sc = new Scanner(br);

            while (this.sc.hasNextInt()) {
                int temp = sc.nextInt();
                sum += temp;
            }
            //System.out.println("sum is " + sum);
        } catch (Exception e) {

        } finally {

            try {
                sc.close();
                br.close();
                fr.close();
            } catch (Exception e) {
                System.out.print(e);
            }
        }
        return sum;
    }
}
