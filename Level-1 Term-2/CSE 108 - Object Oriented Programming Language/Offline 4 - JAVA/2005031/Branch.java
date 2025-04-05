public class Branch {

    private int id;
    private String name;
    private Account []  accounts;
    private int accountCount;
	// add your code here
	// you are allowed to add variables here
    // there can be at most 20 branches
    private static Branch stcBrnc[]=new Branch[20];
    private static int stcCnt=0;

    // you are not allowed to write any other constructor
    public Branch(int id, String name) {
        this.id = id;
        this.name = name;
        this.accounts = new Account[10];
        // add your code here
        accountCount=0;
        stcBrnc[stcCnt]=this;
        stcCnt++;
    }

	// you are not allowed to modify this method
    public void addAccount(Account a) {
        accounts[accountCount++] = a;
    }

    // add your code here
    public double getBranchBalance(){
        double total=0;
        for(int i=0;i<accountCount;i++)
            total+=accounts[i].getBalance();
        return total;
    }
    public Account getMinBalanceAccount(){
        int minpos=0;
        double min=accounts[0].getBalance();
        for(int i=0;i<accountCount;i++)
            if(min>accounts[i].getBalance()) {
                min=accounts[i].getBalance();
                minpos=i;
            }
        return accounts[minpos];
    }
    public static void transferBalance(Account a1, Account a2, double amnt){
        a1.setBalance(a1.getBalance()-amnt);
        a2.setBalance(a1.getBalance()+amnt);
    }
    public static void printAllBranchesInfo(){
        for(int i=0;i<stcCnt;i++) {
            System.out.println("Branch Id: "+stcBrnc[i].id+"Branch Name: "+stcBrnc[i].name);
            for (int j = 0; j < stcBrnc[i].accountCount; j++) {
                System.out.println("Account Number: " + stcBrnc[i].accounts[j].getNumber() +
                        ", Customer Name: " + stcBrnc[i].accounts[j].getCustomer() +
                        ", Balance: " + stcBrnc[i].accounts[j].getBalance());
            }
        }
    }
}
