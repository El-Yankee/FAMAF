USE world;


-- Ejercicio 1
-- ‭Lista el nombre de la ciudad, nombre del país, región y forma de gobierno de las 10 ‭ciudades más pobladas del mundo.
SELECT 
	city.Name AS "City Name",
	country.Name AS "Country Name",
	country.Region,
	country.GovernmentForm AS "Government Form"
FROM
	city
INNER JOIN country ON 
	city.CountryCode = country.Code 
ORDER BY city.Population DESC 
LIMIT 10;


-- Ejercicio 2
-- Listar los 10 países con menor población del mundo, junto a sus ciudades capitales‬
‭-- (Hint: puede que uno de estos países no tenga ciudad capital asignada, en este caso ‭deberá mostrar "NULL").‬
SELECT 
	country.Name AS "Country Name",
	city.Name AS "Country Capital",
	country.Population AS "Population"
FROM
	country 
LEFT JOIN city ON
	city.ID = country.Capital	
ORDER BY country.Population ASC 
LIMIT 10;


-- Ejercicio 3
-- Listar el nombre, continente y todos los lenguajes oficiales de cada país. 
-- (Hint: habrá ‭más de una fila por país si tiene varios idiomas oficiales).
SELECT 
	country.Name AS "Country Name",
	country.Continent AS "Continent",
	countrylanguage.`Language` AS "Language"
FROM
	country
INNER JOIN countrylanguage ON
	countrylanguage.CountryCode = country.Code
WHERE 
	countrylanguage.IsOfficial = "T"
ORDER BY country.Name ASC;


-- Ejercicio 4
-- Listar el nombre del país y nombre de capital, de los 20 países con mayor superficie ‭del mundo.
SELECT 
	country.Name AS "Country Name",
	city.Name AS "Country Capital",
	country.SurfaceArea AS "Surface"
FROM
	country
LEFT JOIN city ON
	city.ID = country.Capital	
ORDER BY country.SurfaceArea DESC 
LIMIT 20;


-- Ejercicio 5
-- Listar las ciudades junto a sus idiomas oficiales 
-- (ordenado por la población de la ‭ciudad) y el porcentaje de hablantes del idioma.
SELECT 
	city.Name AS "City Name",
	countrylanguage.`Language` AS "Language"
FROM 	
	city
INNER JOIN countrylanguage ON
	city.CountryCode = countrylanguage.CountryCode 
WHERE 
	countrylanguage.IsOfficial = "T"
ORDER BY city.Population DESC;


-- Ejercicio 6
-- Listar los 10 países con mayor población y los 10 países con menor población 
-- (que ‭tengan al menos 100 habitantes) en la misma consulta.
(
SELECT country.Name, country.Population 
FROM country
WHERE country.Population >= 100
ORDER BY country.Population DESC
LIMIT 10
)
UNION
(
SELECT country.Name, country.Population 
FROM country
WHERE country.Population >= 100
ORDER BY country.Population ASC 
LIMIT 10
);


-- Ejercicio 7
-- Listar aquellos países cuyos lenguajes oficiales son el Inglés y el Francés 
-- (hint: no ‭debería haber filas duplicadas).
(
SELECT country.Name
FROM country
INNER JOIN countrylanguage ON
	countrylanguage.CountryCode = country.Code
WHERE countrylanguage.IsOfficial = 'T'
ORDER BY country.Name ASC 
)
INTERSECT 
(
SELECT country.Name 
FROM country
INNER JOIN countrylanguage ON
	countrylanguage.CountryCode = country.Code
WHERE countrylanguage.Language IN ('English', 'French')
ORDER BY country.Name ASC 
);


-- Ejercicio 8
-- Listar aquellos países que tengan hablantes del Inglés pero no del Español en su ‭población.‬
(
SELECT country.Name
FROM country
INNER JOIN countrylanguage ON
	countrylanguage.CountryCode = country.Code
WHERE countrylanguage.Language IN ('English')
ORDER BY country.Name ASC 
)
EXCEPT 
(
SELECT country.Name 
FROM country
INNER JOIN countrylanguage ON
	countrylanguage.CountryCode = country.Code
WHERE countrylanguage.Language IN ('Spanish')
ORDER BY country.Name ASC 
);