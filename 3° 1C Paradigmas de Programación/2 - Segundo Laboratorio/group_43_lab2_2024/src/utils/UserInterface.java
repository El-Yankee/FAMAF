package utils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class UserInterface {

    private HashMap<String, String> optionDict;

    private List<Option> options;

    public UserInterface() {
        options = new ArrayList<Option>();
        options.add(new Option("-h", "--help", 0));
        options.add(new Option("-f", "--feed", 1));
        options.add(new Option("-ne", "--named-entity", 1));
        options.add(new Option("-pf", "--print-feed", 0));
        options.add(new Option("-sf", "--stats-format", 1));

        optionDict = new HashMap<String, String>();
    }

    public Config handleInput(String[] args) {

        for (Integer i = 0; i < args.length; i++) {
            for (Option option : options) {
                if (option.getName().equals(args[i]) || option.getLongName().equals(args[i])) {
                    if (option.getnumValues() == 0) {
                        optionDict.put(option.getName(), null);
                    } else {
                        if (i + 1 < args.length && !args[i + 1].startsWith("-")) {
                            optionDict.put(option.getName(), args[i + 1]);
                            i++;
                        } else {
                            System.out.println("Invalid inputs");
                            System.exit(1);
                        }
                    }
                }
            }
        }

        Boolean showHelp = optionDict.containsKey("-h");
        Boolean feedOne = optionDict.containsKey("-f");
        Boolean computeNamedEntities = optionDict.containsKey("-ne");
        Boolean printFeed = optionDict.containsKey("-pf");
        Boolean printStats = optionDict.containsKey("-sf");

        String feedKey = optionDict.get("-f");
        // Value for heuristic config
        String heuristicKey = optionDict.get("-ne");
        // Value for stats format
        String statsFormat = optionDict.get("-sf");

        return new Config(printFeed, computeNamedEntities, showHelp, feedKey, heuristicKey, statsFormat, printStats,
                feedOne);
    }

    // Lo movimos de App.java a UserInterface.java
    public static void printHelp(List<FeedsData> feedsDataArray) {
        System.out.println("Usage: make run ARGS=\"[OPTION]\"");
        System.out.println("Options:");
        System.out.println("  -h, --help: Show this help message and exit");
        System.out.println("  -f, --feed <feedKey>:                Fetch and process the feed with");
        System.out.println("                                       the specified key");
        System.out.println("                                       Available feed keys are: ");
        for (FeedsData feedData : feedsDataArray) {
            System.out.println("                                       " + feedData.getLabel());
        }
        System.out.println("  -ne, --named-entity <heuristicName>: Use the specified heuristic to extract");
        System.out.println("                                       named entities");
        System.out.println("                                       Available heuristic names are: ");
        // Print the available heuristics with the following format
        System.out.println(
                "                                       Heuristic 1: Busca palabras que empiecen con mayuscula y tengan mas de 3 letras");
        System.out.println("                                       Heuristic 2: Version mejorada de la Heuristic 1");
        System.out.println(
                "                                       Heuristic 3: Compara con un diccionario de palabras comunes en español");
        System.out.println("  -pf, --print-feed:                   Print the fetched feed");
        System.out.println("  -sf, --stats-format <format>:        Print the stats in the specified format");
        System.out.println("                                       Available formats are: ");
        System.out.println("                                       cat: Category-wise stats");
        System.out.println("                                       topic: Topic-wise stats");
    }
}
