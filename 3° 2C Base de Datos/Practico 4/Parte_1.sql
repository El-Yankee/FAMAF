USE world;


-- Ejercicio 1
SELECT 
	city.Name AS "City Name",
	country.Name AS "Country Name",
	country.Population AS "Population"
FROM
	city
INNER JOIN country ON 
	city.CountryCode = country.Code 
WHERE 
	country.Population < 10000;
	

-- Ejercicio 2
SELECT 
	city.Name AS "City Name"
FROM
	city
WHERE city.Population > (
	SELECT AVG(city.Population)
	FROM city	
	);


-- Ejercicio 3
SELECT
	city.Name AS "City Name",
	city.Population AS "City Population"
From 
	city
INNER JOIN country ON
	city.CountryCode = country.Code 	
WHERE country.Continent NOT LIKE 'ASIA' AND
	city.Population >= (
	SELECT MIN(country.Population)
	From country 
	WHERE		
		country.Continent LIKE 'ASIA'
	);


-- Ejercicio 4
SELECT 
	country.Name AS "Country Name",
	countrylanguage.Language AS "Language"
FROM 
	country
INNER JOIN countrylanguage ON
	country.Code = countrylanguage.CountryCode
WHERE countrylanguage.IsOfficial LIKE 'F' AND 
	countrylanguage.Percentage > ALL(	
	SELECT countrylanguage.Percentage
	FROM countrylanguage			
	WHERE country.Code = countrylanguage.CountryCode AND 
		countrylanguage.IsOfficial LIKE 'T'  
	);


-- Ejercicio 5
SELECT DISTINCT 
	country.Region AS "Regions",
	country.Name AS "Country Name"	
FROM 
	country
WHERE country.SurfaceArea < 1000 AND 
	EXISTS (
	SELECT city.Name
	FROM city
	WHERE city.CountryCode = country.Code AND 
		city.Population > 100000	
	);


SELECT DISTINCT 
	country.Region AS "Regions",
	country.Name AS "Country Name"
FROM 
	country
INNER JOIN city ON
	city.CountryCode = country.Code
WHERE
	country.SurfaceArea < 1000 AND
	city.Population > 100000;


-- Ejercicio 6
SELECT
	country.Name AS "Country Name",
	MAX(city.Population) AS "MAX Population"
FROM country
INNER JOIN city ON
	city.CountryCode = country.Code
GROUP BY country.Code;


SELECT 
	country.Name AS "CountryName", 
	(SELECT MAX(city.Population)
	FROM city
	WHERE city.CountryCode = country.Code) AS "MAX Population"
FROM country;

-- Ejercicio 7
SELECT country.Name AS CountryName, 
       countrylanguage.Language AS NonOfficialLanguage, 
       countrylanguage.Percentage AS NonOfficialPercentage
FROM country
INNER JOIN countrylanguage ON country.Code = countrylanguage.CountryCode
WHERE countrylanguage.IsOfficial = 'F'  -- Seleccionar solo los lenguajes no oficiales
  AND countrylanguage.Percentage > (
      SELECT AVG(officialLang.Percentage)
      FROM countrylanguage AS officialLang
      WHERE officialLang.CountryCode = country.Code
        AND officialLang.IsOfficial = 'T'  -- Filtrar solo los lenguajes oficiales
  );
 
-- Ejercicio 8
SELECT country.Continent AS Continent,
 		SUM(country.Population) AS Population
FROM country
GROUP BY country.Continent 
ORDER BY Population DESC;	
 		
-- Ejercicio 9
SELECT country.Continent, 
		AVG(country.LifeExpectancy) AS AvgLifeExpectancy
FROM country
WHERE country.LifeExpectancy BETWEEN 40 AND 70
GROUP BY country.Continent;

-- Ejercicio 10
SELECT country.Continent, 
		MAX(country.Population) AS MaxPopulation,
		MIN(country.Population) AS MinPopulation,
		AVG(country.Population) AS AvgPopulation,
		SUM(country.Population) AS SumPopulation 
FROM country
GROUP BY country.Continent; 		
 

