package grupo43.utils;

import java.util.HashMap;
import java.util.List;

import grupo43.namedEntities.NamedEntity;

import java.io.Serializable;

import org.apache.spark.api.java.JavaRDD;

public class Stats implements Serializable {

    private JavaRDD<NamedEntity> entitiesList;
    private String format;

    public Stats(JavaRDD<NamedEntity> entitiesList, String format) {

        this.entitiesList = entitiesList;
        this.format = format;

    }

    public HashMap<String, HashMap<String, Integer>> getStats() {

        // Count named entities by category
        HashMap<String, HashMap<String, Integer>> ListCount = new HashMap<String, HashMap<String, Integer>>();

        List<NamedEntity> entitiesCollected = entitiesList.collect();

        for (NamedEntity entity : entitiesCollected) {

            switch (format) {

                case "category":

                    String category = entity.getCategory();
                    ListCount.putIfAbsent(category, new HashMap<String, Integer>());

                    HashMap<String, Integer> prueba = ListCount.get(category);

                    String name = entity.getName();
                    prueba.put(name, prueba.getOrDefault(name, 0) + 1);

                    break;

                case "topic":

                    List<String> topics = entity.getTopics();

                    for (String topic : topics) {

                        ListCount.putIfAbsent(topic, new HashMap<String, Integer>());

                        HashMap<String, Integer> topicEntities = ListCount.get(topic);

                        String name1 = entity.getName();
                        topicEntities.put(name1, topicEntities.getOrDefault(name1, 0) + 1);
                    }

                    break;

                default:
                    break;
            }

        }
        ;

        return ListCount;
    }

    public void printStats(HashMap<String, HashMap<String, Integer>> ListCount) {

        System.out.println("Stats by " + format);
        System.out.println("-".repeat(60));

        for (HashMap.Entry<String, HashMap<String, Integer>> entry : ListCount.entrySet()) {
            String topic = entry.getKey();
            HashMap<String, Integer> entityCount = entry.getValue();
            System.out.println(format + ": " + topic);
            for (HashMap.Entry<String, Integer> entry2 : entityCount.entrySet()) {
                String entity = entry2.getKey();
                Integer count = entry2.getValue();
                System.out.println("        " + entity + "(" + count + ")");
            }
        }
    }

}
