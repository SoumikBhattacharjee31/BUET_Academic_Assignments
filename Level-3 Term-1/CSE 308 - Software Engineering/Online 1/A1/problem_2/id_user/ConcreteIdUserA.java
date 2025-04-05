package id_user;

import id_generator.IdGenerator;

public class ConcreteIdUserA implements IdUser {
    private int id;
    private IdGenerator idGenerator;

    public ConcreteIdUserA(IdGenerator idGenerator) {
        this.idGenerator = idGenerator;
    }

    public ConcreteIdUserA() {
        // You have to fill this
    }

    @Override
    public void doWork() {
        try {
            // it will pull 5 ids from the generator and print them
            for (int i = 0; i < 5; i++) {
                id = idGenerator.generate();
                System.out.println("A : " + id);
            }
        } catch (NullPointerException e) {
            System.out.println("Reporting from B: where is the generator?\n\n");
            e.printStackTrace();
        }
    }
}
