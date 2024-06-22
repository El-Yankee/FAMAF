package utils;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;
import feed.Article;
import namedEntities.NamedEntity;

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

    public List<String> heuristicKeySelection(String heuristicKey, List<Article> allArticles) throws IOException {

        List<String> candidatos = new ArrayList<>();

        switch (heuristicKey) {
            case "1":
                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.CapitalizedWordHeuristic
                                    .extractCandidates(art.getDescription()));
                }
                break;

            case "2":
                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.MayusculasMejoradas.extractCandidates(art.getDescription()));
                }
                break;

            case "3":
                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.NoSustantivosPropios.extractCandidates(art.getDescription()));
                }
                break;

            default:
                System.out.println("Invalid heuristic key. You must use 1, 2 or 3");
                break;
        }
        return candidatos;
    }

    public void runHelp(List<FeedsData> feedsDataArray) {
        UserInterface.printHelp(feedsDataArray);
        System.exit(0);
    }

    public List<Article> runFeed(List<FeedsData> feedsDataArray, List<Article> allArticles)
            throws MalformedURLException, IOException, Exception {

        if (getFeedOne()) {
            // Caso que se especifica -f
            for (FeedsData feedData : feedsDataArray) {

                // Hacemos el feedParser
                if (feedData.getLabel().equals(getFeedKey())) {

                    String xmldata = feed.FeedParser.fetchFeed(feedData.getUrl());

                    // Populate allArticles with articles from corresponding feeds
                    allArticles = feed.FeedParser.parseXML(xmldata);

                }
            }
        } else {
            // Caso que no se especifica -f
            for (FeedsData feedData : feedsDataArray) {

                String xmldata = feed.FeedParser.fetchFeed(feedData.getUrl());

                // Populate allArticles with articles from corresponding feeds
                allArticles.addAll(feed.FeedParser.parseXML(xmldata));
            }
        }

        return allArticles;
    }

    public void runPrintFeed(List<Article> allArticles) {
        System.out.println("Printing feed(s) ");

        // Print the fetched feed
        for (Article a : allArticles) {

            a.printPretty();
        }
    }

    public void runNamedEntities(List<DictionaryData> dictionaryDataArray, List<Article> allArticles)
            throws IOException {

        List<NamedEntity> entitiesList = new ArrayList<>();
        List<String> candidatos = new ArrayList<>();

        // Complete the message with the selected heuristic name
        System.out.println("Computing named entities using heuristic " + getHeuristicKey());

        candidatos = heuristicKeySelection(getHeuristicKey(), allArticles);

        // Revisamos que candidatos son entidades y las categorizamos
        for (String c : candidatos) {

            boolean found = false;

            // Tomamos el diccionario y comparamos con los candidatos
            for (DictionaryData dict : dictionaryDataArray) {

                // Vemos si el candidato es igual algun keyword
                if (dict.isKeywordinDictionary(c)) {

                    NamedEntity newEnt = new NamedEntity(dict.getLabel(), dict.getCategory(), dict.getTopics());

                    newEnt = feed.Article.asignCategory(newEnt);

                    entitiesList.add(newEnt);

                    found = true;
                    break;
                }

                // Si ya encontramos la entidad, salimos del loop
                if (found) {
                    break;
                }
            }

            // Si el candidato no se corresponde a una entidad, la agregamos como "Other"
            if (!found) {
                String other = "OTHER";
                List<String> topic = new ArrayList<>();
                topic.add(other);
                NamedEntity newEnt = new NamedEntity(c, other, topic);
                entitiesList.add(newEnt);
            }
        }

        // Funcionamiento de -sf
        if (!getPrintStats() || getStatsFormat().equals("cat")) {

            Stats stats = new Stats(entitiesList, "category");

            stats.printStats(stats.getStats());

        } else if (getStatsFormat().equals("topic")) {

            Stats stats = new Stats(entitiesList, "topic");

            stats.printStats(stats.getStats());

        } else {

            System.out.println("Invalid stats format. You must use cat or topic");
        }
    }
}
