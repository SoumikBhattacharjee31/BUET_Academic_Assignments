import id_generator.ConcreteIdGenerator;
import id_generator.IdGenerator;
import id_user.ConcreteIdUserA;
import id_user.ConcreteIdUserB;

public class Driver {

    public static void previous_main() {
        IdGenerator idGenerator = new ConcreteIdGenerator();
        ConcreteIdUserA idUserA = new ConcreteIdUserA(idGenerator);
        ConcreteIdUserB idUserB = new ConcreteIdUserB(idGenerator);
        idUserA.doWork();
        idUserB.doWork();
        idUserA.doWork();
        idUserB.doWork();

    }

    public static void new_main() {
        ConcreteIdUserA idUserA = new ConcreteIdUserA();
        ConcreteIdUserB idUserB = new ConcreteIdUserB();
        idUserA.doWork();
        idUserB.doWork();
        idUserA.doWork();
        idUserB.doWork();
    }

    public static void main(String[] args) {
        previous_main(); // this line should be commented
        // new_main(); // this line should be uncommented
        System.out.println("Simulation over!");
    }
}