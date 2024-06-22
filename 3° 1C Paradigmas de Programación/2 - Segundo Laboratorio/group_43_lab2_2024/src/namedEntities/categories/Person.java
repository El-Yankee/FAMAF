package namedEntities.categories;

import java.util.List;
import namedEntities.NamedEntity;

public class Person extends NamedEntity {

    private int id;

    public Person(String name, String category, List<String> topics, int id) {
        super(name, category, topics);
        this.id = id;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}
