import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;

import feed.Article;
import utils.Config;
import utils.FeedsData;
import utils.DictionaryData;
import utils.JSONParser;
import utils.UserInterface;

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
            config.runHelp(feedsDataArray);
        }

        // Funcionamiento de -f
        allArticles = config.runFeed(feedsDataArray, allArticles);

        // Funcionamiento de -pf
        if (config.getPrintFeed() || !config.getComputeNamedEntities()) {
            config.runPrintFeed(allArticles);
        }

        // Funcionamiento de -ne
        if (config.getComputeNamedEntities()) {

            config.runNamedEntities(dictionaryDataArray, allArticles);
        }
    }
}