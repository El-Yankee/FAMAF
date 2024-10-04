USE sakila;

-- Ejercicio 1
-- Cree una tabla de `directors` con las columnas: Nombre, Apellido, Número de ‭Películas.
DROP TABLE IF EXISTS directors;
CREATE TABLE IF NOT EXISTS directors (
        name VARCHAR(200),
        surname VARCHAR(200),
        numberOfMovies INT
    );
    
   
-- Ejercicio 2
-- El top 5 de actrices y actores de la tabla `actors` que tienen la mayor experiencia 
-- (i.e.‭el mayor número de películas filmadas) son también directores de las películas en ‭las que participaron. 
-- Basados en esta información, inserten, utilizando una subquery ‭los valores correspondientes en la tabla `directors`. 
INSERT INTO 
	directors (name, surname, numberOfMovies)
SELECT
	actor.first_name,
	actor.last_name,
	COUNT(film_actor.film_id) AS totalmovies
FROM
	actor
INNER JOIN film_actor ON
	actor.actor_id = film_actor.actor_id 
GROUP BY film_actor.actor_id 
ORDER BY totalmovies DESC
LIMIT 5;


-- Ejercicio 3
-- ‭Agregue una columna `premium_customer` que tendrá un valor 'T' o 'F' de acuerdo a ‭si el cliente es "premium" o no. Por defecto ningún cliente será premium.
ALTER TABLE customer 
ADD COLUMN premium_customer enum('T', 'F') NOT NULL DEFAULT 'F';


-- Ejercicio 4
-- Modifique la tabla customer. Marque con 'T' en la columna `premium_customer` de ‭los 10 clientes con mayor dinero gastado en la plataforma.
UPDATE customer c
SET c.premium_customer = 'T'
WHERE c.customer_id IN (
    SELECT top10.customer_id
    FROM (
        SELECT
            p.customer_id,
            sum(p.amount) AS `total`
        FROM payment p
        GROUP BY p.customer_id
        ORDER BY `total` DESC
        LIMIT 10
    ) top10
);

UPDATE customer c
INNER JOIN (
    SELECT p.customer_id, SUM(amount) as total_spent
    FROM payment p
    GROUP BY
        p.customer_id
    ORDER BY total_spent DESC
    LIMIT 10
) AS top_10 ON top_10.customer_id = c.customer_id
SET c.premium_customer = 'T';


-- Ejercicio 5
-- ‭Listar, ordenados por cantidad de películas (de mayor a menor), los distintos ratings ‭de las películas existentes 
-- (Hint: rating se refiere en este caso a la clasificación ‭según edad: G, PG, R, etc).‬
SELECT 
    rating, 
    COUNT(*) AS total_movies
FROM 
    film
GROUP BY 
    rating
ORDER BY 
    total_movies DESC;

   
-- Ejercicio 6
-- ¿Cuáles fueron la primera y última fecha donde hubo pagos?
SELECT MIN(payment.payment_date), MAX(payment.payment_date)
FROM payment;
   

-- Ejercicio 7
-- ‭Calcule, por cada mes, el promedio de pagos (Hint: vea la manera de extraer el ‭nombre del mes de una fecha).
SELECT 
	MONTHNAME(payment_date) AS month_name,
	AVG(payment.amount) AS amount_avg
FROM payment
GROUP BY month_name;


-- Ejercicio 8
-- Listar los 10 distritos que tuvieron mayor cantidad de alquileres (con la cantidad total ‭de alquileres).‬
SELECT
    a.district,
    count(r.rental_id) AS total_alquileres
FROM rental r
    INNER JOIN customer c ON r.customer_id = c.customer_id
    INNER JOIN address a ON a.address_id = c.address_id
GROUP BY a.district
ORDER BY total_alquileres DESC
LIMIT 10;


-- Ejercicio 9
-- Modifique la table `inventory_id` agregando una columna `stock` que sea un número ‭entero y representa 
-- la cantidad de copias de una misma película que tiene ‭determinada tienda. El número por defecto debería ser 5 copias.
ALTER TABLE inventory
ADD COLUMN stock INT DEFAULT 5;


-- Ejercicio 10
-- Cree un trigger `update_stock` que, cada vez que se agregue un nuevo registro a la‭tabla rental,
-- haga un update en la tabla `inventory` restando una copia al stock de la ‭película rentada 
-- (Hint: revisar que el rental no tiene información directa sobre la ‭tienda, sino sobre el cliente, que está asociado a una tienda en particular).‬
DROP TRIGGER IF EXISTS update_stock;
DELIMITER //
CREATE TRIGGER update_stock 
AFTER INSERT ON rental 
FOR EACH ROW
BEGIN
    UPDATE inventory AS i
    SET i.stock = i.stock - 1
    WHERE i.inventory_id = NEW.inventory_id AND i.stock > 0;
END//
DELIMITER ;


-- Ejercicio 11
-- Cree una tabla `fines` que tenga dos campos: `rental_id` y `amount`. 
-- El primero es ‭una clave foránea a la tabla rental y el segundo es un valor numérico con dos ‭decimales.‬
DROP TABLE IF EXISTS fines;
CREATE TABLE fines (
    rental_id int,
    amount DECIMAL(65, 2),
    FOREIGN KEY (rental_id) REFERENCES rental (rental_id)
);

   
-- Ejercicio 12
-- Cree un procedimiento `check_date_and_fine` que revise la tabla `rental` y cree un‭registro en 
-- la tabla `fines` por cada `rental` cuya devolución (return_date) haya ‭tardado más de 3 días 
-- (comparación con rental_date). El valor de la multa será el ‭número de días de retraso multiplicado por 1.5.
DROP PROCEDURE IF EXISTS check_date_and_fine;
DELIMITER //
CREATE PROCEDURE check_date_and_fine() 
BEGIN	
	
	INSERT INTO fines (rental_id, amount)
	SELECT
	    r.rental_id,
	    DATEDIFF(r.return_date, r.rental_date) * 1.5 AS amount 
	FROM rental AS r
	WHERE
    	DATEDIFF(r.return_date, r.rental_date) > 3
    AND NOT EXISTS (
    	SELECT 1
    	FROM fines f
    	WHERE r.rental_id = f.rental_id);
END //
DELIMITER ;

-- Test
START TRANSACTION;
SELECT *
FROM fines;
CALL check_date_and_fine();

SELECT COUNT(*)
FROM fines;
ROLLBACK;


-- Ejercicio 13
-- Crear un rol `employee` que tenga acceso de inserción, eliminación y actualización a ‭la tabla `rental`.
CREATE ROLE employee;

GRANT INSERT, DELETE, UPDATE 
ON rental 
TO employee;


-- Ejercicio 14
-- Revocar el acceso de eliminación a `employee` y crear un rol `administrator` que ‭tenga todos los privilegios sobre la BD `sakila`.
REVOKE DELETE
ON rental
FROM employee;

CREATE ROLE administrator;
GRANT ALL PRIVILEGES ON sakila.* TO administrator;


-- Ejercicio 15
-- Crear dos roles de empleado. A uno asignarle los permisos de `employee` y al otro ‭de `administrator`.
CREATE ROLE employee_A;
CREATE ROLE employee_B;

GRANT employee TO employee_A;
GRANT administrator TO employee_B;

