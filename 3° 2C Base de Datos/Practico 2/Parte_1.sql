-- Ejercicio 1
DROP DATABASE IF EXISTS world;
CREATE DATABASE IF NOT EXISTS world;
USE world;


-- Ejercicio 2
DROP TABLE IF EXISTS country;
CREATE TABLE IF NOT EXISTS country (
	Code CHAR(3) NOT NULL DEFAULT '',
	Name CHAR(52) NOT NULL DEFAULT '',
	Continent CHAR(35) NOT NULL DEFAULT '',
	Region CHAR(26) NOT NULL DEFAULT '',
	SurfaceArea DECIMAL(10,2) NOT NULL DEFAULT '0.00',
	IndepYear SMALLINT DEFAULT NULL,
	Population INT NOT NULL DEFAULT '0',
	LifeExpectancy DECIMAL(3,1) DEFAULT NULL,
	GNP DECIMAL(10,2) DEFAULT NULL,
	GNPOld DECIMAL(10,2) DEFAULT NULL,
	LocalName CHAR(45) NOT NULL DEFAULT '',
	GovernmentForm CHAR(45) NOT NULL DEFAULT '',
	HeadOfState CHAR(60) DEFAULT NULL,
	Capital INT DEFAULT NULL,
	Code2 CHAR(2) NOT NULL DEFAULT '',
	PRIMARY KEY (Code)
);

DROP TABLE IF EXISTS city;
CREATE TABLE IF NOT EXISTS city(
	ID INT NOT NULL AUTO_INCREMENT,
	Name CHAR(35) NOT NULL DEFAULT '',
	CountryCode CHAR(3) NOT NULL DEFAULT '',
	District CHAR(20) NOT NULL DEFAULT '',
	Population INT NOT NULL DEFAULT '0',
	PRIMARY KEY (ID),
	-- creo un indice para optimizacion
	KEY CountryCode (CountryCode),
	-- le pongo nombre a la restriccion para identificarlo
  	CONSTRAINT `city_ibfk_1` FOREIGN KEY (CountryCode) REFERENCES country (Code)
);

DROP TABLE IF EXISTS countrylanguage;
CREATE TABLE IF NOT EXISTS countrylanguage(
	CountryCode char(3) NOT NULL DEFAULT '',
  	Language char(30) NOT NULL DEFAULT '',
	IsOfficial ENUM('T','F') NOT NULL DEFAULT 'F',
	Percentage DECIMAL(4,1) NOT NULL DEFAULT '0.0',
	PRIMARY KEY (CountryCode,Language),
	-- creo un indice para optimizacion
	KEY CountryCode (CountryCode),
	-- le pongo nombre a la restriccion para identificarlo
  	CONSTRAINT `countryLanguage_ibfk_1` FOREIGN KEY (CountryCode) REFERENCES country (Code)	  	
);


-- Ejercicio 3
-- Insertar los datos
-- mysql -u root -h localhost < world-data.sql


-- Ejercicio 4
DROP TABLE IF EXISTS Continent;
CREATE TABLE IF NOT EXISTS Continent(
	Name CHAR(35) NOT NULL DEFAULT '',
	Area INT NOT NULL DEFAULT '0', 
	PercentMass FLOAT NOT NULL DEFAULT '0.0',
	MostPopuCity CHAR(35) NOT NULL DEFAULT '' UNIQUE,
	PRIMARY KEY (Name)	
);


-- Ejercicio 5
INSERT INTO Continent VALUES ("Africa", 30370000, 20.4, "Cairo, Egypt");
INSERT INTO Continent VALUES ("‭Antarctica‬", 14000000, 9.2, "Villa Las Estrellas");
INSERT INTO Continent VALUES ("Asia", 44579000, 29.5, "Mumbai, India");
INSERT INTO Continent VALUES ("Europe", 10180000, 6.8, "Instanbul, Turquia");
INSERT INTO Continent VALUES ("North America", 24709000, 16.5, "Ciudad de México, Mexico");
INSERT INTO Continent VALUES ("Oceania", 8600000, 5.9, "Sydney, Australia");
INSERT INTO Continent VALUES ("South America", 17840000, 12.0, "São Paulo, Brazil");


-- Ejercicio 6
ALTER TABLE country 
ADD CONSTRAINT fk_country_continent FOREIGN KEY (Continent) REFERENCES Continent(Name);

