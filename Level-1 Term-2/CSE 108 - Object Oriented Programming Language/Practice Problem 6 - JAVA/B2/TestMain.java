public class TestMain {
    public static void main(String[] args) {
        // Test Customer class
        Customer customer = new Customer(88, "John Doe", 10);
        customer.setDiscount(8);

        System.out.println("Customer Id: " + customer.getId());
        System.out.println("Customer Name: " + customer.getName());
        System.out.println("Customer Discount: " + customer.getDiscount());

        // Test Invoice class
        Invoice invoice = new Invoice(101, customer, 888.8);
        invoice.setAmount(999.9);

        System.out.println("Invoice Id: " + invoice.getId());
        System.out.println("Invoice Amount: " + invoice.getAmount());
        System.out.println("Invoice Customer's Id: " + invoice.getCustomerId());
        System.out.println("Invoice Customer's Name: " + invoice.getCustomerName());
        System.out.println("Invoice Customer's Discount: " + invoice.getCustomerDiscount());
        System.out.printf("Invoice Amount after Discount: " + invoice.getAmountAfterDiscount());
    }
}

/* output 
Customer Id: 88
Customer Name: John Doe
Customer Discount: 8
Invoice Id: 101
Invoice Amount: 999.9
Invoice Customer's Id: 88
Invoice Customer's Name: John Doe
Invoice Customer's Discount: 8
Invoice Amount after Discount: 991.9
*/