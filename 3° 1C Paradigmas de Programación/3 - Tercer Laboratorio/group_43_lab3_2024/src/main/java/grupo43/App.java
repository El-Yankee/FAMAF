package grupo43;

import java.io.IOException;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.List;

import grupo43.utils.Config;
import grupo43.utils.DictionaryData;
import grupo43.utils.JSONParser;
import grupo43.utils.UserInterface;

// Spark imports
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.JavaRDD;

public final class App {

    public static void main(String[] args) throws MalformedURLException, IOException, Exception {

        // Configure Spark
        SparkConf sparkConf = new SparkConf().setAppName("My Spark App").setMaster("spark://localhost:7077");

        // Create a Java Spark Context
        JavaSparkContext sparkContext = new JavaSparkContext(sparkConf);

        JavaRDD<String> lines = sparkContext.textFile(args[0]);

        List<DictionaryData> dictionaryDataArray = new ArrayList<>();
        try {
            dictionaryDataArray = JSONParser.parseJsonDictionaryData(args[1]);

        } catch (IOException e) {
            e.printStackTrace();
            System.exit(1);
        }

        UserInterface ui = new UserInterface();
        Config config = ui.handleInput(args);

        run(config, lines, dictionaryDataArray, sparkContext);

        // Don't forget to stop the SparkContext when you're done
        sparkContext.stop();
    }

    // Change the signature of this function if needed
    private static void run(Config config, JavaRDD<String> lines, List<DictionaryData> dictionaryDataArray,
            JavaSparkContext sc)
            throws MalformedURLException, IOException, Exception {

        if (dictionaryDataArray == null || dictionaryDataArray.size() == 0) {
            System.out.println("No dictionary data found");
            return;
        }

        // Funcionamiento de -h
        if (config.getShowHelp()) {
            config.runHelp();
        }

        // Funcionamiento de -ne
        if (config.getComputeNamedEntities()) {

            config.runNamedEntities(dictionaryDataArray, lines, sc);
        }
    }
}