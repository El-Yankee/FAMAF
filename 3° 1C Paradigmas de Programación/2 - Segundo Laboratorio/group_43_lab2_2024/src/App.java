import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;

import feed.Article;
import namedEntities.NamedEntity;
import utils.Config;
import utils.FeedsData;
import utils.DictionaryData;
import utils.JSONParser;
import utils.UserInterface;
import java.util.HashMap;

public class App {

    public static void main(String[] args) throws MalformedURLException, IOException, Exception {

        List<FeedsData> feedsDataArray = new ArrayList<>();
        List<DictionaryData> dictionaryDataArray = new ArrayList<>();
        try {
            feedsDataArray = JSONParser.parseJsonFeedsData("src/data/feeds.json");
            dictionaryDataArray = JSONParser.parseJsonDictionaryData("src/data/dictionary.json");

        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        UserInterface ui = new UserInterface();
        Config config = ui.handleInput(args);

        run(config, feedsDataArray, dictionaryDataArray);
    }

    // Change the signature of this function if needed
    private static void run(Config config, List<FeedsData> feedsDataArray, List<DictionaryData> dictionaryDataArray)
            throws MalformedURLException, IOException, Exception {

        if (feedsDataArray == null || feedsDataArray.size() == 0) {
            System.out.println("No feeds data found");
            return;
        }

        if (dictionaryDataArray == null || dictionaryDataArray.size() == 0) {
            System.out.println("No dictionary data found");
            return;
        }

        List<Article> allArticles = new ArrayList<>();

        // Funcionamiento de -h
        if (config.getShowHelp()) {
            UserInterface.printHelp(feedsDataArray);
            System.exit(0);
        }

        // Funcionamiento de -f
        if (config.getFeedOne()) {

            // Caso que se especifica -f
            for (FeedsData feedData : feedsDataArray) {

                // Hacemos el feedParser
                if (feedData.getLabel().equals(config.getFeedKey())) {

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

        // Funcionamiento de -pf
        if (config.getPrintFeed() || !config.getComputeNamedEntities()) {

            System.out.println("Printing feed(s) ");

            // Print the fetched feed
            for (Article a : allArticles) {

                a.printPretty();
            }
        }

        // Funcionamiento de -ne
        if (config.getComputeNamedEntities()) {

            List<NamedEntity> entitiesList = new ArrayList<>();
            List<String> candidatos = new ArrayList<>();

            // Complete the message with the selected heuristic name
            System.out.println("Computing named entities using heuristic " + config.getHeuristicKey());

            // Tomamos los candidatos segun la heuristica seleccionada
            if (config.getHeuristicKey().equals("1")) {

                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.CapitalizedWordHeuristic.extractCandidates(art.getDescription()));

                }

            } else if (config.getHeuristicKey().equals("2")) {

                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.MayusculasMejoradas.extractCandidates(art.getDescription()));
                }

            } else if (config.getHeuristicKey().equals("3")) {

                // Recorremos los articulos y extraemos los candidatos
                for (Article art : allArticles) {

                    candidatos.addAll(
                            namedEntities.heuristics.NoSustantivosPropios.extractCandidates1(art.getDescription()));
                }

            } else {

                System.out.println("Invalid heuristic key. You must use 1, 2 or 3");
            }

            // Revisamos que candidatos son entidades y las categorizamos
            for (String c : candidatos) {

                boolean found = false;

                // Tomamos el diccionario y comparamos con los candidatos
                for (DictionaryData dict : dictionaryDataArray) {

                    List<String> keywords = dict.getKeywords();

                    // Vemos si el candidato es igual algun keyword
                    for (String keyw : keywords) {

                        if (c.equals(keyw)) {

                            NamedEntity newEnt = new NamedEntity(dict.getLabel(), dict.getCategory(), dict.getTopics());

                            entitiesList.add(newEnt);

                            found = true;
                            break;
                        }
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
            if (!config.getPrintStats() || config.getStatsFormat().equals("cat")) {

                // Count named entities by category
                HashMap<String, HashMap<String, Integer>> categoryCount = new HashMap<String, HashMap<String, Integer>>();

                for (NamedEntity entity : entitiesList) {

                    String category = entity.getCategory();
                    categoryCount.putIfAbsent(category, new HashMap<String, Integer>());

                    HashMap<String, Integer> prueba = categoryCount.get(category);

                    String name = entity.getName();
                    prueba.put(name, prueba.getOrDefault(name, 0) + 1);

                }

                System.out.println("Stats by category: ");
                System.out.println("-".repeat(60));
                // Print named entity counts by category
                for (HashMap.Entry<String, HashMap<String, Integer>> entry : categoryCount.entrySet()) {
                    String category = entry.getKey();
                    HashMap<String, Integer> entityCount = entry.getValue();
                    System.out.println("Category: " + category);
                    for (HashMap.Entry<String, Integer> entry2 : entityCount.entrySet()) {
                        String entity = entry2.getKey();
                        Integer count = entry2.getValue();
                        System.out.println("        " + entity + "(" + count + ")");
                    }
                }

            } else if (config.getStatsFormat().equals("topic")) {

                // Count named entities by topic
                HashMap<String, HashMap<String, Integer>> topicCount = new HashMap<String, HashMap<String, Integer>>();

                for (NamedEntity entity : entitiesList) {

                    List<String> topics = entity.getTopics();

                    for (String topic : topics) {

                        topicCount.putIfAbsent(topic, new HashMap<String, Integer>());

                        HashMap<String, Integer> topicEntities = topicCount.get(topic);

                        String name = entity.getName();
                        topicEntities.put(name, topicEntities.getOrDefault(name, 0) + 1);
                    }
                }

                System.out.println("Stats by topic: ");
                System.out.println("-".repeat(60));
                // Print named entity counts by topic
                for (HashMap.Entry<String, HashMap<String, Integer>> entry : topicCount.entrySet()) {
                    String topic = entry.getKey();
                    HashMap<String, Integer> entityCount = entry.getValue();
                    System.out.println("Topic: " + topic);
                    for (HashMap.Entry<String, Integer> entry2 : entityCount.entrySet()) {
                        String entity = entry2.getKey();
                        Integer count = entry2.getValue();
                        System.out.println("        " + entity + "(" + count + ")");
                    }
                }

            } else {

                System.out.println("Invalid stats format. You must use cat or topic");
            }
        }
    }
}