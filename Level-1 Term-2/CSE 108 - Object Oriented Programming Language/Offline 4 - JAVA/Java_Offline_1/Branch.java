public class Branch {

    private int id;
    private String name;
    private Account []  accounts;
    private int accountCount;
	// add your code here
	// you are allowed to add variables here
    // there can be at most 20 branches  

    // you are not allowed to write any other constructor
    public Branch(int id, String name) {
        this.id = id;
        this.name = name;
        this.accounts = new Account[10];
        // add your code here
    }

	// you are not allowed to modify this method
    public void addAccount(Account a) {
        accounts[accountCount++] = a;
    }

    // add your code here

}
