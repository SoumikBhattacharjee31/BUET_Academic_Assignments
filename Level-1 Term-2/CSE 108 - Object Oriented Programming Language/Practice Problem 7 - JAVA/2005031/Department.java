public class Department {

    private int id;
    private String name;
    private Employee[] employees;
    private int employeeCount;
    // add your code here
    // you are allowed to add variables here
    // there can be at most 10 departments
    static Department[] stcDpt=new Department[10];
    static int stcCnt=0;

    // you are not allowed to write any other constructor
    public Department(int id, String name) {
        this.id = id;
        this.name = name;
        this.employees = new Employee[10];
        // add your code here
        employeeCount=0;
        stcDpt[stcCnt++]=this;
    }

    // add your code here
    public void addEmployee(Employee e){
        employees[employeeCount++]=e;
    }
    public double getDepartmentSalary(){
        double total=0;
        for(int i=0;i<employeeCount;i++)
            total+=employees[i].getSalary();
        return total;
    }
    public Employee getMaxSalaryEmployee(){
        double mx=employees[0].getSalary();
        int pos=0;
        for(int i=1;i<employeeCount;i++)
            if(mx<employees[i].getSalary()) {
                mx = employees[i].getSalary();
                pos=i;
            }
        return employees[pos];
    }
    public static double getTotalSalary(){
        double total=0;
        for(int i=0;i<stcCnt;i++)
            total+=(stcDpt[i].getDepartmentSalary());
        return total;
    }
    public void giveIncrementToEmployees(double inc){
        int n=getMaxSalaryEmployee().getId();
        for(int i=0;i<employeeCount;i++) {
            if(employees[i].getId()==n)
                continue;
            employees[i].setSalary(employees[i].getSalary() + inc);
        }
    }
}