USE world;


-- Ejercicio 1
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