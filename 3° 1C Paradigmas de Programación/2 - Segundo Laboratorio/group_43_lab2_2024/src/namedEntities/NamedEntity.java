package namedEntities;

import java.util.List;

public class NamedEntity {
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
}