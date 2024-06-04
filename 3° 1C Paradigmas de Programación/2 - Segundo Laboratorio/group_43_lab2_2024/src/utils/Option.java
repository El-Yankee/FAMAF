package utils;

public class Option {
    private String name;
    private String longName;
    private Integer numValues;

    public Option(String name, String longName, Integer numValues) {
        this.name = name;
        this.longName = longName;
        this.numValues = numValues;
    }

    public String getName() {
        return name;
    }

    public String getLongName() {
        return longName;
    }

    public Integer getnumValues() {
        return numValues;
    }

}
