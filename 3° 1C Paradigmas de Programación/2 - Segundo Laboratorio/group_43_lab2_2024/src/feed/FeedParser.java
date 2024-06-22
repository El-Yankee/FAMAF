package feed;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.StringReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class FeedParser {

    // Voy a suponer que es todo el xml en un string
    public static List<Article> parseXML(String xmlData) {

        List<Article> Articles = new ArrayList<>();

        try {
            // Toma el string y lo transformar en un tipo de dato para procesar XML.
            InputSource inputSource = new InputSource(new StringReader(xmlData));

            // Crea un objeto DocumentBuilder que crea objetos que analizan documentos XML.
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

            // Objeto que analiza documentos (crea una fabrica)
            DocumentBuilder builder = factory.newDocumentBuilder();

            // El tipo Document representa todo el árbol de un documento XML.
            // Parsea el InputSource, que es el xmlData transformado en un flujo de lectura
            // de caracteres y crea un arbol de nodos Document.
            Document doc = builder.parse(inputSource);

            // Organiza los datos
            doc.getDocumentElement().normalize();

            // Obtiene todos los elementos 'item'
            NodeList itemNodes = doc.getElementsByTagName("item");

            // Itera sobre cada elemento 'item'
            for (int i = 0; i < itemNodes.getLength(); i++) {
                Node itemNode = itemNodes.item(i);

                if (itemNode.getNodeType() == Node.ELEMENT_NODE) {
                    Element itemElement = (Element) itemNode;

                    // Obtiene los datos de cada etiqueta
                    String title = itemElement.getElementsByTagName("title").item(0).getTextContent();

                    // Obtengo el contenido de la etiqueta description
                    NodeList descriptionNodes = itemElement.getElementsByTagName("description");
                    String description = " ";

                    // Si la etiqueta description tiene contenido, lo guardo
                    if (descriptionNodes.getLength() > 0) {
                        description = descriptionNodes.item(0).getTextContent();
                    }

                    String pubDate = itemElement.getElementsByTagName("pubDate").item(0).getTextContent();
                    String link = itemElement.getElementsByTagName("link").item(0).getTextContent();

                    // Crea un nuevo artículo y lo añade a la lista
                    Articles.add(new Article(title, description, pubDate, link));
                }
            }

        } catch (Exception e) {

            e.printStackTrace();
        }

        return Articles;
    }

    public static String fetchFeed(String feedURL) throws MalformedURLException, IOException, Exception {

        URL url = new URL(feedURL);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();

        connection.setRequestMethod("GET");
        connection.setRequestProperty("Content-Type", "application/json");

        // Cambiar el user-agent al nombre de su grupo.
        // Si todos los grupos usan el mismo user-agent, el servidor puede bloquear las
        // solicitudes.
        // No cambiamos User-agent para no romper la funcionalidad, ya que los links de
        // pagina 12 no funcionan si realizamos el cambio.
        connection.setRequestProperty("User-agent", "lab_paradigmas");
        connection.setConnectTimeout(5000);
        connection.setReadTimeout(5000);

        int status = connection.getResponseCode();
        if (status != 200) {
            throw new Exception("HTTP error code: " + status);
        } else {
            BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()));
            String inputLine;
            StringBuffer content = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                content.append(inputLine);
            }
            in.close();
            connection.disconnect();
            return content.toString();
        }
    }
}
