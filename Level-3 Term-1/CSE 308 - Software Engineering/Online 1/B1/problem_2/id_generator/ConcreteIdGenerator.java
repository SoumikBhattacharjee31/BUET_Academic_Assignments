package id_generator;

public class ConcreteIdGenerator implements IdGenerator {
    private int id = 0;

    @Override
    public int generate() {
        return ++id;
    }
}
