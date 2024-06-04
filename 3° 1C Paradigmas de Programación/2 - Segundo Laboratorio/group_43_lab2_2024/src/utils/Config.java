package utils;

public class Config {
    // boolean variables
    private boolean printFeed = false;
    private boolean feedOne = false;
    private boolean computeNamedEntities = false;
    private boolean showHelp = false;
    private boolean printStats = false;

    // String variables
    private String feedKey;
    private String heuristicKey;
    private String statsFormat;

    public Config(boolean printFeed, boolean computeNamedEntities, boolean showHelp, String feedKey,
            String heuristicKey, String statsFormat, boolean printStats, boolean feedOne) {
        this.printFeed = printFeed;
        this.computeNamedEntities = computeNamedEntities;
        this.showHelp = showHelp;
        this.feedKey = feedKey;
        this.heuristicKey = heuristicKey;
        this.statsFormat = statsFormat;
        this.printStats = printStats;
        this.feedOne = feedOne;
    }

    // getters for boolean variables
    public boolean getPrintFeed() {
        return printFeed;
    }

    public boolean getComputeNamedEntities() {
        return computeNamedEntities;
    }

    public boolean getShowHelp() {
        return showHelp;
    }

    public boolean getPrintStats() {
        return printStats;
    }

    public boolean getFeedOne() {
        return feedOne;
    }

    // getters for String variables
    public String getFeedKey() {
        return feedKey;
    }

    public String getHeuristicKey() {
        return heuristicKey;
    }

    public String getStatsFormat() {
        return statsFormat;
    }
}
