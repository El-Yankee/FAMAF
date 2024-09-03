USE world;

SELECT 
	city.Name ,
	country.Name 
FROM
	city
INNER JOIN country ON
	city.CountryCode = country.Code AND 
	country.Name = 'Argentina';

SELECT 
	city.Name ,
	country.Name 
FROM
	city
INNER JOIN country ON
	city.CountryCode = country.Code 
WHERE country.Name = 'Argentina';

-- ¿Devuelven los mismos valores las siguientes consultas? ¿Por qué?

/* 
 * Ubicación de la Condición:
 * 
	Consulta 1: La condición country.Name = 'Argentina' se coloca dentro de la cláusula ON,
 	lo que significa que afecta la forma en que se realiza la unión (JOIN) entre las tablas.
 
	Consulta 2: La condición country.Name = 'Argentina' se coloca en la cláusula WHERE,
 	lo que significa que la condición se aplica después de que se realiza la unión.
 * 
 * 
 * Impacto en los Resultados:

	Consulta 1 (condición en ON):
	Esta condición actúa como parte del criterio de unión. Solo se consideran para la unión aquellas
	filas donde city.CountryCode = country.Code y country.Name = 'Argentina'.
	Es decir, solo se unirán filas si el país es "Argentina", y luego se seleccionan las ciudades correspondientes.

	Consulta 2 (condición en WHERE):
	La unión se realiza primero sin tener en cuenta el nombre del país, es decir, todas las ciudades
 	se unirán con sus respectivos países según city.CountryCode = country.Code.
	Luego, se filtran las filas resultantes donde country.Name = 'Argentina'.
 * 
*/


-- ¿Y si en vez de INNER JOIN fuera un LEFT JOIN?

/* 
 * Comportamiento de la Unión:

	En consultas con INNER JOIN, ambas consultas devolverán el mismo resultado porque INNER JOIN solo retiene las filas
	que cumplen con la condición de unión (city.CountryCode = country.Code) y la condición adicional (country.Name = 'Argentina'),
 	independientemente de si se aplica en ON o WHERE.
	La diferencia es más relevante si estuvieras usando otros tipos de uniones como LEFT JOIN o RIGHT JOIN, donde la ubicación 
	de la condición (ON vs WHERE) puede afectar los resultados, ya que las filas no coincidentes
	(que no cumplen la condición de unión) se manejan de manera diferente.* 
 * 
 * Conclusión:
	Para un INNER JOIN, ambas consultas devolverán el mismo conjunto de resultados. 
	Sin embargo, la práctica más común y generalmente recomendada es colocar las condiciones de filtrado de los 
	datos en la cláusula WHERE, y las condiciones que definen cómo deben unirse las tablas en la cláusula ON.
	Si en algún momento cambias a un LEFT JOIN o RIGHT JOIN, la colocación de las condiciones en ON o WHERE puede
	producir diferencias significativas en los resultados.
*/


