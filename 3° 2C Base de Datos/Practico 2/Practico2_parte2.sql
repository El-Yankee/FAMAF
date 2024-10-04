USE world;

-- Ejercicio 1
-- Devuelva una lista de los nombres y las regiones a las que pertenece cada país ‭ordenada alfabéticamente.‬
SELECT Name, Region 
FROM country
ORDER BY Name, Region  ASC;


-- Ejercicio 2
-- ‭Liste el nombre y la población de las 10 ciudades más pobladas del mundo.
SELECT Name, Population
FROM country
ORDER BY Population DESC
LIMIT 10;


-- Ejercicio 3
-- ‭Liste el nombre, región, superficie y forma de gobierno de los 10 países con menor ‭superficie.
SELECT Name, Region, SurfaceArea, GovernmentForm
FROM country
ORDER BY SurfaceArea ASC
LIMIT 10;


-- Ejercicio 4
-- Liste todos los países que no tienen independencia (hint: ver que define la ‭independencia de un país en la BD).
SELECT * 
FROM country
WHERE IndepYear IS NULL; 


-- Ejercicio 5
-- ‭Liste el nombre y el porcentaje de hablantes que tienen todos los idiomas declarados ‭oficiales.
SELECT Language, Percentage
FROM countrylanguage
WHERE IsOfficial = 'T'
ORDER BY Language ASC;


-- Ejercicio 6
-- Actualizar el valor de porcentaje del idioma inglés en el país con código 'AIA' a 100.0
UPDATE countrylanguage
SET Percentage = 100
WHERE CountryCode = 'AIA';

-- Ejercicio 7
-- Listar las ciudades que pertenecen a Córdoba (District) dentro de Argentina.
SELECT Name
FROM city
WHERE District = 'Córdoba' AND CountryCode = 'ARG';


-- Ejercicio 8
-- Eliminar todas las ciudades que pertenezcan a Córdoba fuera de Argentina.
DELETE FROM city
WHERE District = 'Córdoba' AND NOT CountryCode = 'ARG';


-- Ejercicio 9
-- Listar los países cuyo Jefe de Estado se llame John.
SELECT Name
FROM country
WHERE HeadOfState LIKE '%John%';


-- Ejercicio 10
-- Listar los países cuya población esté entre 35 M y 45 M ordenados por población de ‭forma descendente.
SELECT Name, Population
FROM country
WHERE Population BETWEEN 35000000 AND 45000000
ORDER BY Population DESC;

