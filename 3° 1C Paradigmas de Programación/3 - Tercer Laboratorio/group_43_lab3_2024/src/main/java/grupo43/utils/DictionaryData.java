package grupo43.utils;

import java.util.List;

import java.io.Serializable;
public class DictionaryData implements Serializable {

    private String label;
    private String Category;
    private List<String> Topics;
    private List<String> keywords;

    public DictionaryData(String label, String Category, List<String> Topics, List<String> keywords) {
        this.label = label;
        this.Category = Category;
        this.Topics = Topics;
        this.keywords = keywords;
    }

    public String getLabel() {
        return label;
    }

    public String getCategory() {
        return Category;
    }

    public List<String> getTopics() {
        return Topics;
    }

    public List<String> getKeywords() {
        return keywords;
    }

    public boolean isKeywordinDictionary(String key) {
        return getKeywords().contains(key);
    }

    public void print() {
        System.out.println("Label: " + label);
        System.out.println("Category: " + Category);
        System.out.println("Topics: " + Topics);
        System.out.println("Keywords: " + keywords);
    }

}
