USE world;

-- Opción 1: Usando agrupaciones

SELECT country.Name AS CountryName, city.Name AS MostPopulatedCity, city.Population AS LargestCityPopulation
FROM city
INNER JOIN country ON city.CountryCode = country.Code
WHERE city.Population = (
    SELECT MAX(city2.Population)
    FROM city AS city2
    WHERE city2.CountryCode = country.Code
);

-- Opción 2: Usando consultas escalares

SELECT country.Name AS CountryName,
       (SELECT city.Name 
        FROM city 
        WHERE city.CountryCode = country.Code 
        ORDER BY city.Population DESC 
        LIMIT 1) AS MostPopulatedCity,
       (SELECT MAX(city.Population)
        FROM city
        WHERE city.CountryCode = country.Code) AS LargestCityPopulation
FROM country;

-- Conclusión:
/*
Con consultas escalares, puedes obtener el nombre de la ciudad más poblada y su población directamente con subconsultas independientes.

Con agrupaciones, necesitas usar una combinación de una subconsulta correlacionada en la cláusula WHERE para obtener tanto la ciudad como su población.
Ambas soluciones cumplen con el objetivo de listar el nombre del país, el nombre de la ciudad más poblada y la cantidad de habitantes de esa ciudad.
*/

