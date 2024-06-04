package namedEntities.heuristics;

import java.io.IOException;
import java.text.Normalizer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import utils.GramaticaData;
import utils.JSONParser;

public class NoSustantivosPropios {

    public static List<String> extractCandidates1(String text) throws IOException {
        List<String> candidates = new ArrayList<>();

        GramaticaData gramatica = JSONParser.parseJsonGramaticaData("src/data/gramatica.json");

        text = text.replaceAll("[-+.^:,\"]", "");
        text = Normalizer.normalize(text, Normalizer.Form.NFD);
        text = text.replaceAll("\\p{M}", "");

        // Dividir el texto en palabras
        String[] wordsArray = text.split("\\s+");
        // Convertir el array en una lista
        List<String> wordsList = Arrays.asList(wordsArray);

        for (String word : wordsList) {
            if (!gramatica.estaLaPalabre(word)) {
                candidates.add(word);
            }
        }

        return candidates;
    }

}
