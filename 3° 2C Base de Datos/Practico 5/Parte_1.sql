USE sakila;

-- Ejercicio 1
DROP TABLE IF EXISTS directors;
CREATE TABLE IF NOT EXISTS directors (
        name VARCHAR(200),
        surname VARCHAR(200),
        numberOfMovies INT
    );
    
   
-- Ejercicio 2 
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
ALTER TABLE customer 
ADD COLUMN premium_customer enum('T', 'F') NOT NULL DEFAULT 'F';


-- Ejercicio 4
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
SELECT MIN(payment.payment_date), MAX(payment.payment_date)
FROM payment;
   

-- Ejercicio 7
SELECT 
	MONTHNAME(payment_date) AS month_name,
	AVG(payment.amount) AS amount_avg
FROM payment
GROUP BY month_name;


-- Ejercicio 8
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
ALTER TABLE inventory
ADD COLUMN stock INT DEFAULT 5;


-- Ejercicio 10
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
DROP TABLE IF EXISTS fines;
CREATE TABLE fines (
    rental_id int,
    amount DECIMAL(65, 2),
    FOREIGN KEY (rental_id) REFERENCES rental (rental_id)
);

   
-- Ejercicio 12
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
CREATE ROLE employee;

GRANT INSERT, DELETE, UPDATE 
ON rental 
TO employee;


-- Ejercicio 14
REVOKE DELETE
ON rental
FROM employee;

CREATE ROLE administrator;
GRANT ALL PRIVILEGES ON sakila.* TO administrator;


-- Ejercicio 15
CREATE ROLE employee_A;
CREATE ROLE employee_B;

GRANT employee TO employee_A;
GRANT administrator TO employee_B;

