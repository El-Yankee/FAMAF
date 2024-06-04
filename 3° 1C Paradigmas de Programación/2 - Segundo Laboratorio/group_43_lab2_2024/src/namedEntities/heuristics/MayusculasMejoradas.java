package namedEntities.heuristics;

import java.text.Normalizer;
import java.util.List;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class MayusculasMejoradas {

    public static List<String> extractCandidates(String text) {
        List<String> candidates = new ArrayList<>();

        text = text.replaceAll("[-+^:,\"]", "");
        text = Normalizer.normalize(text, Normalizer.Form.NFD);
        text = text.replaceAll("\\p{M}", "");

        Pattern pattern = Pattern.compile("[A-Z][a-z]+(?:\\s[A-Z][a-z]+)*");
        Matcher matcher = pattern.matcher(text);

        // el patron detecta palabras precedidas por un punto (o signo)
        Pattern sentenceEndPattern = Pattern.compile("[.!?]\\s+");
        Matcher sentenceEndMatcher = sentenceEndPattern.matcher(text);

        // guardamos las posiciones donde termina una oracion
        List<Integer> sentenceEndPositions = new ArrayList<>();
        while (sentenceEndMatcher.find()) {
            sentenceEndPositions.add(sentenceEndMatcher.end());
        }

        while (matcher.find()) {
            String word = matcher.group();
            int wordStart = matcher.start();

            // si el inicio de una palabra coincide con el fin de una oracion el booleano es
            // falso
            boolean isStartOfSentence = false;
            if (wordStart == 0 || text.charAt(wordStart - 1) == ' ') {
                for (int pos : sentenceEndPositions) {
                    if (pos == wordStart) {
                        isStartOfSentence = true;
                        break;
                    }
                }
            }

            // agrego la palabra si no es el inicio de una oracion y tiene mas de 3 letras
            if (!isStartOfSentence && word.length() > 3) {
                candidates.add(word);
            }
        }

        return candidates;
    }
}
