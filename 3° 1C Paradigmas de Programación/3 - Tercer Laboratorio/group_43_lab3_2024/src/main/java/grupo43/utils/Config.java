package grupo43.utils;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.broadcast.Broadcast;

import grupo43.namedEntities.NamedEntity;

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

    public JavaRDD<String> heuristicKeySelection(String heuristicKey, JavaRDD<String> lines)
            throws IOException {

        JavaRDD<String> candidatos = lines.flatMap(line -> {

            List<String> candidates = new ArrayList<>();

            switch (heuristicKey) {
                case "1":
                    candidates.addAll(
                            grupo43.namedEntities.heuristics.CapitalizedWordHeuristic
                                    .extractCandidates(line));
                    break;

                case "2":
                    candidates.addAll(
                            grupo43.namedEntities.heuristics.MayusculasMejoradas
                                    .extractCandidates(line));
                    break;

                case "3":
                    candidates.addAll(
                            grupo43.namedEntities.heuristics.NoSustantivosPropios
                                    .extractCandidates(line));
                    break;

                default:
                    System.out.println("Invalid heuristic key. You must use 1, 2 or 3");
                    break;
            }

            return candidates.iterator();

        });

        return candidatos;
    }

    public void runHelp() {
        UserInterface.printHelp();
        System.exit(0);
    }

    public void runNamedEntities(List<DictionaryData> dictionaryDataArray, JavaRDD<String> lines, JavaSparkContext sc)
            throws IOException {

        // Complete the message with the selected heuristic name
        System.out.println("Computing named entities using heuristic " + getHeuristicKey());

        JavaRDD<String> candidatos = heuristicKeySelection(getHeuristicKey(), lines);

        // Assuming sc is your JavaSparkContext and dictionaryDataArray is a
        // collection of DictionaryData
        Broadcast<List<DictionaryData>> broadcastedDictionaryData = sc.broadcast(dictionaryDataArray);

        // Revisamos que candidatos son entidades y las categorizamos
        JavaRDD<NamedEntity> entitiesList = candidatos.map(c -> {

            // boolean found = false;

            List<DictionaryData> localDictionaryData = broadcastedDictionaryData.value();
            // Use localDictionaryData instead of dictionaryDataArray in your existing logic

            // Tomamos el diccionario y comparamos con los candidatos
            for (DictionaryData dict : localDictionaryData) {
                // Vemos si el candidato es igual algun keyword
                if (dict.isKeywordinDictionary(c)) {
                    NamedEntity newEnt = new NamedEntity(dict.getLabel(), dict.getCategory(), dict.getTopics());
                    newEnt = grupo43.namedEntities.NamedEntity.asignCategory(newEnt);
                    // found = true;

                    return newEnt;
                }
            }

            // No consideramos la categoria OTHER debido a que para una bigdata de 1.5GB se
            // demora mucho

            /*
             * // Si el candidato no se corresponde a una entidad, la agregamos como "Other"
             * if (!found) {
             * String other = "OTHER";
             * List<String> topic = new ArrayList<>();
             * topic.add(other);
             * NamedEntity newEnt = new NamedEntity(c, other, topic);
             * return newEnt;
             * }
             */

            return null;
        }).filter(ent -> ent != null);

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
