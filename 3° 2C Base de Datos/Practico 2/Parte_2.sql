USE world;

-- Ejercicio 1
SELECT Name, Region 
FROM country
ORDER BY Name, Region  ASC;


-- Ejercicio 2
SELECT Name, Population
FROM country
ORDER BY Population DESC
LIMIT 10;


-- Ejercicio 3
SELECT Name, Region, SurfaceArea, GovernmentForm
FROM country
ORDER BY SurfaceArea ASC
LIMIT 10;


-- Ejercicio 4
SELECT * 
FROM country
WHERE IndepYear IS NULL; 


-- Ejercicio 5
SELECT Language, Percentage
FROM countrylanguage
WHERE IsOfficial = 'T'
ORDER BY Language ASC;


-- Ejercicio 6
UPDATE countrylanguage
SET Percentage = 100
WHERE CountryCode = 'AIA';

-- Ejercicio 7
SELECT Name
FROM city
WHERE District = 'Córdoba' AND CountryCode = 'ARG';


-- Ejercicio 8
DELETE FROM city
WHERE District = 'Córdoba' AND NOT CountryCode = 'ARG';


-- Ejercicio 9
SELECT Name
FROM country
WHERE HeadOfState LIKE '%John%';


-- Ejercicio 10
SELECT Name, Population
FROM country
WHERE Population BETWEEN 35000000 AND 45000000
ORDER BY Population DESC;

