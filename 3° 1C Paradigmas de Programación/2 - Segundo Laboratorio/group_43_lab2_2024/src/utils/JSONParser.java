package utils;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;

public class JSONParser {

    static public List<FeedsData> parseJsonFeedsData(String jsonFilePath) throws IOException {
        String jsonData = new String(Files.readAllBytes(Paths.get(jsonFilePath)));
        List<FeedsData> feedsList = new ArrayList<>();

        JSONArray jsonArray = new JSONArray(jsonData);
        for (int i = 0; i < jsonArray.length(); i++) {
            JSONObject jsonObject = jsonArray.getJSONObject(i);
            String label = jsonObject.getString("label");
            String url = jsonObject.getString("url");
            String type = jsonObject.getString("type");
            feedsList.add(new FeedsData(label, url, type));
        }
        return feedsList;
    }

    static public List<DictionaryData> parseJsonDictionaryData(String jsonFilePath) throws IOException {
        String jsonData = new String(Files.readAllBytes(Paths.get(jsonFilePath)));
        List<DictionaryData> dictionaryList = new ArrayList<>();

        JSONArray jsonArray = new JSONArray(jsonData);
        for (int i = 0; i < jsonArray.length(); i++) {
            JSONObject jsonObject = jsonArray.getJSONObject(i);
            String label = jsonObject.getString("label");
            String Category = jsonObject.getString("Category");

            List<String> Topics = new ArrayList<>();
            JSONArray topicsJsonArray = jsonObject.getJSONArray("Topics");
            for (int j = 0; j < topicsJsonArray.length(); j++) {
                Topics.add(topicsJsonArray.getString(j));
            }

            List<String> keywords = new ArrayList<>();
            JSONArray keywordsJsonArray = jsonObject.getJSONArray("keywords");
            for (int j = 0; j < keywordsJsonArray.length(); j++) {
                keywords.add(keywordsJsonArray.getString(j));
            }

            dictionaryList.add(new DictionaryData(label, Category, Topics, keywords));
        }
        return dictionaryList;
    }

    static public GramaticaData parseJsonGramaticaData(String jsonFilePath) throws IOException {
        String jsonData = new String(Files.readAllBytes(Paths.get(jsonFilePath)));

        JSONObject jsonObject = new JSONObject(jsonData);

        List<String> Adjetivos = JSONObjectToList(jsonObject, "adjetivos");
        List<String> Verbos = JSONObjectToList(jsonObject, "verbos");
        List<String> Determinantes = JSONObjectToList(jsonObject, "determinantes");
        List<String> Pronombres = JSONObjectToList(jsonObject, "pronombres");
        List<String> Conjunciones = JSONObjectToList(jsonObject, "conjunciones");
        List<String> Preposiciones = JSONObjectToList(jsonObject, "preposiciones");
        List<String> Adverbios = JSONObjectToList(jsonObject, "adverbios");
        List<String> Sustantivos = JSONObjectToList(jsonObject, "sustantivos");

        GramaticaData gramatica = new GramaticaData(Adjetivos, Verbos, Determinantes, Pronombres, Conjunciones,
                Preposiciones, Adverbios, Sustantivos);
        return gramatica;
    }

    static public List<String> JSONObjectToList(JSONObject jsonObject, String key) {
        JSONArray jsonArray = jsonObject.getJSONArray(key);
        List<String> list = new ArrayList<>();
        for (int i = 0; i < jsonArray.length(); i++) {
            list.add(jsonArray.getString(i));
        }
        return list;
    }
}
