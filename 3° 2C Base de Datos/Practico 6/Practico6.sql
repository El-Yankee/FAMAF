USE classicmodels;

-- Ejercicio 1
-- Devuelva la oficina con mayor número de empleados.
SELECT 
	o.officeCode AS "Office",
	COUNT(e.employeeNumber) AS "TotalEmployees"
FROM offices o 
INNER JOIN employees e ON
	e.officeCode = o.officeCode
GROUP BY o.officeCode 
ORDER BY "TotalEmployees" DESC
LIMIT 1;


-- Ejercicio 2
-- ¿Cuál es el promedio de órdenes hechas por oficina?, ¿Qué oficina vendió la mayor ‭cantidad de productos?‬
SELECT
	o.officeCode,
	COUNT(ord.orderNumber) AS avg_orders_per_office	
FROM offices o 
INNER JOIN employees e ON
	e.officeCode = o.officeCode 
INNER JOIN customers c ON
	c.salesRepEmployeeNumber = e.employeeNumber 
INNER JOIN orders ord ON
	ord.customerNumber = c.customerNumber 
GROUP BY o.officeCode;

SELECT
	o.officeCode,
	COUNT(ord.orderNumber) AS avg_orders_per_office	
FROM offices o 
INNER JOIN employees e ON
	e.officeCode = o.officeCode 
INNER JOIN customers c ON
	c.salesRepEmployeeNumber = e.employeeNumber 
INNER JOIN orders ord ON
	ord.customerNumber = c.customerNumber 
GROUP BY o.officeCode
ORDER BY avg_orders_per_office DESC 
LIMIT 1;


-- Ejercicio 3
-- ‭Devolver el valor promedio, máximo y mínimo de pagos que se hacen por mes.
SELECT
	MONTHNAME(p.paymentDate) AS month,
	AVG(p.amount) AS avg_pay,
	MAX(p.amount) AS max_pay,
	MIN(p.amount) AS min_pay
FROM payments p
GROUP BY MONTHNAME(p.paymentDate);


-- Ejercicio 4
-- Crear un procedimiento "Update Credit" en donde se modifique el límite de crédito de ‭un cliente con un valor pasado por parámetro.
DROP PROCEDURE IF EXISTS update_Credit;
DELIMITER //
CREATE PROCEDURE update_Credit(IN customerNumber INT, IN new_credit_limit INT)
BEGIN
	
	UPDATE customers AS c
	SET c.creditLimit = new_credit_limit
	WHERE c.customerNumber = customerNumber;	
END //
DELIMITER ;

-- Test
START TRANSACTION;
-- Llamada al procedimiento para probarlo
CALL update_Credit(103, 50000);
-- Comprobar si los cambios fueron realizados correctamente
SELECT creditLimit FROM customers WHERE customerNumber = 103;
-- Deshacer los cambios para no modificar la base de datos
ROLLBACK;


-- Ejercicio 5
-- Cree una vista "Premium Customers" que devuelva el top 10 de clientes que más ‭dinero han gastado en la plataforma. 
-- La vista deberá devolver el nombre del cliente, ‭la ciudad y el total gastado por ese cliente en la plataforma. 
CREATE VIEW premium_customers AS
SELECT 
	c.customerName AS "Nombre",
	c.city AS "Ciudad",
	SUM(p.amount) AS "compras"
FROM customers c
INNER JOIN payments p ON
	p.customerNumber = c.customerNumber 
GROUP BY c.customerNumber 
ORDER BY compras DESC 
LIMIT 10; 

-- Test
SELECT * FROM premium_customers;


-- Ejercicio 6
-- Cree una función "employee of the month" que tome un mes y un año y devuelve el ‭empleado (nombre y apellido) 
-- cuyos clientes hayan efectuado la mayor cantidad de ‭órdenes en ese mes.
DROP FUNCTION IF EXISTS employee_of_the_month;
DELIMITER //
CREATE FUNCTION employee_of_the_month(input_month INT, input_year INT)
RETURNS VARCHAR(255)
DETERMINISTIC
BEGIN
		DECLARE employee_name VARCHAR(255);
	
		SELECT CONCAT(e.firstName, ' ', e.lastName) 
		INTO employee_name
		FROM employees e	
		INNER JOIN customers c ON
			e.employeeNumber = c.salesRepEmployeeNumber
		INNER JOIN orders o ON
			o.customerNumber = c.customerNumber
		WHERE 
			MONTH(o.orderDate) = input_month AND 
			YEAR(o.orderDate) = input_year
		GROUP BY e.employeeNumber
		ORDER BY COUNT(o.orderNumber) DESC 
		LIMIT 1;
	RETURN employee_name;	
END //
DELIMITER ;

-- Test
SELECT * FROM employees e 
WHERE CONCAT(e.firstName ," ", e.lastName) = employee_of_the_month(2, 2003);


-- Ejercicio 7
-- Crear una nueva tabla "Product Refillment".
-- Deberá tener una relación varios a uno ‭con "products" y los campos: `refillmentID`, `productCode`, `orderDate`, `quantity`.
DROP TABLE IF EXISTS product_refillment;
CREATE TABLE product_refillment (
	refillmentID INT AUTO_INCREMENT PRIMARY KEY,
    productCode VARCHAR(255),
    orderDate DATE,
    quantity INT,
    FOREIGN KEY (productCode) REFERENCES products(productCode)
);


-- Ejercicio 8
-- Definir un trigger "Restock Product" que esté pendiente de los cambios efectuados ‭en `orderdetails`
-- y cada vez que se agregue una nueva orden revise la cantidad de ‭productos pedidos (`quantityOrdered`)
-- y compare con la cantidad en stock ‭(`quantityInStock`) y si es menor a 10 genere un pedido en la tabla "Product‭Refillment" por 10 nuevos productos.‬
DROP TRIGGER IF EXISTS restock_product;
DELIMITER //
CREATE TRIGGER restock_product 
AFTER INSERT ON orderdetails 
FOR EACH ROW
BEGIN
	DECLARE currentStock INT;
	-- Obtener la cantidad en stock del producto
	SELECT p.quantityInStock INTO currentStock
	FROM products p 
	WHERE productCode = NEW.productCode;
	-- Si el stock actual es menor a 10, generar un pedido de reabastecimiento
	IF currentStock < 10 THEN
		INSERT INTO product_refillment (productCode, orderDate, quantity)
        VALUES (NEW.productCode, CURDATE(), 10);
		-- Actualizar el stock de productos
       	UPDATE products
        SET quantityInStock = quantityInStock + 10
        WHERE productCode = NEW.productCode;
    END IF;    
END;//
DELIMITER ;


-- Ejercicio 9
-- Crear un rol "Empleado" en la BD que establezca accesos de lectura a todas las ‭tablas y accesos de creación de vistas.
CREATE ROLE empleado;
GRANT SELECT ON classicmodels.* TO empleado;
GRANT CREATE VIEW ON classicmodels.* TO empleado;






















