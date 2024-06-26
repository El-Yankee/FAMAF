package grupo43.namedEntities.categories;

import java.util.List;
import grupo43.namedEntities.NamedEntity;

public class Location extends NamedEntity {

    private String country;

    public Location(String name, String category, List<String> topics, String country) {
        super(name, category, topics);
        this.country = country;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

}
