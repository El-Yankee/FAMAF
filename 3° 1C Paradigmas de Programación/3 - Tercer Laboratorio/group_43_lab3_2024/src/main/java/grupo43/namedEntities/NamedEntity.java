package grupo43.namedEntities;

import java.util.List;

import grupo43.namedEntities.categories.Location;
import grupo43.namedEntities.categories.Organization;
import grupo43.namedEntities.categories.Person;

import java.io.Serializable;

public class NamedEntity implements Serializable {
    private String name;
    private String category;
    private List<String> topics;

    public NamedEntity(String name, String category, List<String> topics) {
        super();
        this.name = name;
        this.category = category;
        this.topics = topics;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCategory() {
        return category;
    }

    public void setCategory(String category) {
        this.category = category;
    }

    public List<String> getTopics() {
        return topics;
    }

    public void setTopics(String topic) {
        this.topics.add(topic);
    }

    public void printPretty() {
        System.out.println("NamedEntity [name=" + name + ", category=" + category + ", topics=" + topics + "]");
    }

    public static NamedEntity asignCategory(NamedEntity ne) {

		String category = ne.getCategory();

		if (category != null) {

			switch (category) {
				case "Person":
					Person p = new Person(ne.getName(), ne.getCategory(), ne.getTopics(), 11);
					return p;
				case "Organization":
					Organization org = new Organization(ne.getName(), ne.getCategory(), ne.getTopics(), null);
					return org;
				case "Location":
					Location loc = new Location(ne.getName(), ne.getCategory(), ne.getTopics(), null);
					return loc;
				default:
					return ne;
			}
		}
		return null;
	}
}