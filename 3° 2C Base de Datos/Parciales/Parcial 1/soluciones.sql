USE airbnb_like_db;

-- Ejercicio 1
-- Listar las 7 propiedades con la mayor cantidad de reviews en el año 2024.
SELECT 
	p.name AS propiedad,
	COUNT(r.id) AS total_reviews
FROM properties p 
INNER JOIN reviews r ON
	r.property_id = p.id 
GROUP BY propiedad
ORDER BY total_reviews DESC
LIMIT 7;

-- Ejercicio 2
-- Obtener los ingresos por reservas de cada propiedad. 
-- Esta consulta debe calcular los ingresos totales generados por cada propiedad. 
-- Ayuda: hay un campo `price_per_night` en la tabla de `properties` donde los ingresos 
-- totales se computan  sumando la cantidad de noches reservadas para cada reserva multiplicado por el precio por noche.
SELECT 
	p.name AS propiedad,	
	SUM(DATEDIFF(b.check_out, b.check_in) * p.price_per_night) AS ingresos_totales	
FROM properties p
INNER JOIN bookings b ON
	b.property_id = p.id
GROUP BY p.name 
ORDER BY propiedad ASC;


-- Ejercicio 3
-- Listar los principales usuarios según los pagos totales. 
-- Esta consulta calcula los pagos totales realizados por cada usuario y enumera los principales 10 usuarios según la suma de sus pagos.
SELECT 	
	u.name AS nombre_usuario,
	SUM(p.amount) AS total_gastado
FROM users u 
INNER JOIN payments p ON
	p.user_id = u.id
GROUP BY u.id
ORDER BY total_gastado DESC ;


-- Ejercicio 4
-- Crear un trigger notify_host_after_booking que notifica al anfitrión sobre una nueva reserva. 
-- Es decir, cuando se realiza una reserva, notifique al anfitrión de la propiedad mediante un mensaje.
DROP TRIGGER IF EXISTS notify_host_after_booking;
DELIMITER //
CREATE TRIGGER notify_host_after_booking 
AFTER INSERT ON bookings 
FOR EACH ROW
BEGIN
	DECLARE prop_owner INT;
	
	SELECT p.owner_id INTO prop_owner
	FROM properties p
	WHERE 
		p.id = NEW.property_id;
	
	INSERT INTO messages (sender_id, receiver_id,property_id,content,sent_at)
	VALUES (NEW.user_id, prop_owner, NEW.property_id, "Hice una reserva" , CURDATE());
END;
//
DELIMITER ;

-- Test
SELECT * from messages m ;
START TRANSACTION;
INSERT INTO bookings (property_id,user_id,check_in,check_out,total_price)
VALUES (1603,1737,CURDATE(),CURDATE(), 152);
-- Comprobar si los cambios fueron realizados correctamente
SELECT * from messages m;
-- Deshacer los cambios para no modificar la base de datos
ROLLBACK;


-- Ejercicio 5
-- Crear un procedimiento add_new_booking  para agregar una nueva reserva.
-- Este procedimiento agrega una nueva reserva para un usuario, según el ID de la propiedad, el ID del usuario y las fechas de entrada y salida.
-- Verifica si la propiedad está disponible durante las fechas especificadas antes de insertar la reserva.
DROP PROCEDURE IF EXISTS add_new_booking;
DELIMITER //
CREATE PROCEDURE add_new_booking(IN prop_id INT, IN user_id INT, IN check_in DATE, IN check_out DATE)
BEGIN	
	
	DECLARE price INT;	
	SELECT DATEDIFF(check_out, check_in) * p.price_per_night INTO price
	FROM properties p
	WHERE 
		p.id = prop_id;
	
	IF TRUE THEN
	INSERT INTO bookings (property_id, user_id, check_in, check_out, total_price)
	VALUES (prop_id, user_id, check_in, check_out, price);				
	END IF;		
END;
//
DELIMITER ;

-- Test
SELECT * from bookings b ;
START TRANSACTION;
-- Llamada al procedimiento para probarlo
CALL add_new_booking(1603,1737 ,CURDATE(),CURDATE());
-- Comprobar si los cambios fueron realizados correctamente
SELECT * from bookings b ;
-- Deshacer los cambios para no modificar la base de datos
ROLLBACK;

-- Ejercicio 6
-- Crear el rol `admin` y asignarle permisos de creación sobre la tabla `properties` y permiso de actualización 
-- sobre la columna `status`  de la tabla `property_availability`
CREATE ROLE admin;
GRANT INSERT ON properties TO admin;
GRANT UPDATE (status) ON property_availability TO admin;


-- Ejercicio 7
START TRANSACTION;
UPDATE reviews
SET comment = 'bad'
WHERE rating = 1;
COMMIT

-- Porque se inicializa una transaccion al ejecutar START TRANSACTION y se completa al utilizar COMMIT.



