package namedEntities.categories;

import java.util.List;
import namedEntities.NamedEntity;

public class Organization extends NamedEntity {

    private String headquarters;

    public Organization(String name, String category, List<String> topics, String headquarters) {
        super(name, category, topics);
        this.headquarters = headquarters;
    }

    public String getHeadquarters() {
        return headquarters;
    }

    public void setHeadquarters(String headquarters) {
        this.headquarters = headquarters;
    }

}
