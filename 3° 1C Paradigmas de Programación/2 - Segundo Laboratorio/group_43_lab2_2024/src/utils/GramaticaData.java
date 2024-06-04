package utils;

import java.util.List;

public class GramaticaData {
    private List<String> adjetivos;
    private List<String> verbos;
    private List<String> determinantes;
    private List<String> pronombres;
    private List<String> conjunciones;
    private List<String> preposiciones;
    private List<String> adverbios;
    private List<String> sustantivos;

    public GramaticaData(List<String> adjetivos, List<String> verbos, List<String> determinantes,
            List<String> pronombres, List<String> conjunciones, List<String> preposiciones, List<String> adverbios,
            List<String> sustantivos) {
        this.adjetivos = adjetivos;
        this.verbos = verbos;
        this.determinantes = determinantes;
        this.pronombres = pronombres;
        this.conjunciones = conjunciones;
        this.preposiciones = preposiciones;
        this.adverbios = adverbios;
        this.sustantivos = sustantivos;
    }

    public List<String> getAdjetivos() {
        return adjetivos;
    }

    public List<String> getVerbos() {
        return verbos;
    }

    public List<String> getDeterminantes() {
        return determinantes;
    }

    public List<String> getPronombres() {
        return pronombres;
    }

    public List<String> getConjunciones() {
        return conjunciones;
    }

    public List<String> getPreposiciones() {
        return preposiciones;
    }

    public List<String> getAdverbios() {
        return adverbios;
    }

    public List<String> getSustantivos() {
        return sustantivos;
    }

    public boolean estaLaPalabre(String test) {
        boolean result = adjetivos.contains(test) ||
                verbos.contains(test) ||
                determinantes.contains(test) ||
                pronombres.contains(test) ||
                conjunciones.contains(test) ||
                preposiciones.contains(test) ||
                adverbios.contains(test) ||
                sustantivos.contains(test);

        return result;
    }
}