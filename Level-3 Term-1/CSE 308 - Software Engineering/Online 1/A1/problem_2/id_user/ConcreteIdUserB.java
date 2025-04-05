package id_user;

import id_generator.IdGenerator;

public class ConcreteIdUserB implements IdUser {
    private int id;
    private IdGenerator idGenerator;

    public ConcreteIdUserB(IdGenerator idGenerator) {
        this.idGenerator = idGenerator;
    }

    public ConcreteIdUserB() {
        // You have to fill this
    }

    @Override
    public void doWork() {
        // it will pull 7 ids from the generator and print them
        try {

            for (int i = 0; i < 7; i++) {
                id = idGenerator.generate();
                System.out.println("B : " + id);
            }
        } catch (NullPointerException e) {
            System.out.println("Reporting from B: where is the generator?\n\n");
            e.printStackTrace();
        }
    }

}
