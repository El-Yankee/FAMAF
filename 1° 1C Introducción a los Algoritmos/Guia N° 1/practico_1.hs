
--Ejercicio 13

--Ejercicio 13.a
por5 :: Int -> Int
por5 x = 5*x

--Ejercicio 13.b
duplica :: Int -> Int 
duplica a = a+a

--Ejercicio 13.c
por2 :: Int -> Int
por2 y = 2*y

--Ejercicio 13.d
multiplicar :: Int -> Int -> Int
multiplicar a b = a*b
------------------------------------------------------

--Ejercicio 14

promedio :: Float -> Float -> Float
promedio x y = (x+y)/2
------------------------------------------------------

--Ejemplos

--Ejemplos 1
mayor :: Int -> Bool
mayor x = x>0

--Ejemplos 2
sgntexto :: Int -> String
sgntexto x  | x>0 = "Positivo"
			| x==0 = "Cero"
			| x<0 = "Negativo" 

--Ejemplos 3
unirtexto :: String -> String -> String
unirtexto a b = a ++ b

--Ejemplos 4
sgn :: Int -> Int
sgn x 	| x>0 = 1
		| x==0 = 0
		| x<0 = -1

--Ejemplos 5
dentro :: Int -> Bool
dentro x = (x>0) && (x<10)

--Ejemplos 6
afuera :: Int -> Bool
afuera x = (x<0) || (x>10)

--Ejemplos 7
negacion :: Bool -> Bool
negacion x = not(x)
------------------------------------------------------

--Ejercicio 18

--Ejercicio 18.a
entre0y9 :: Int -> Bool
entre0y9 n | n < 0 = False
           | (n >= 0) && (n <= 9) = True
           | n > 9 = False

--Ejercicio 18.b
rangoprecio :: Int -> String
rangoprecio x | (0 < x) && (x < 2000) = "muy barato"
              | x > 5000 = "muy caro"
              | (2000 <= x) && (x <= 5000) = "hay que verlo bien"
			  | x < 0 = "esto no puede ser"

--Ejercico 18.c
absoluto :: Int -> Int
absoluto n = abs(n)

--Ejercicio 18.d
esMultiplo2 :: Int -> Bool
esMultiplo2 n = (mod n 2) == 0 

------------------------------------------------------

--Ejercicio 19

esmultiplode :: Int -> Int -> Bool
esmultiplode x y = (mod y x) == 0
------------------------------------------------------

--Ejercicio 20

esbisiesto :: Int -> Bool
esbisiesto x = ((mod x 400) == 0) || ((mod x 4) == 0) && ((mod x 100) /= 0) 
------------------------------------------------------

--Ejercicio 21

dispersion :: Int -> Int -> Int -> Int
dispersion x y z = (max x (max y z)) - (min x (min y z))
------------------------------------------------------

--Ejercicio 22

celsiustofar :: Float -> Float
celsiustofar x = (x * 1.8) + 32
------------------------------------------------------

--Ejercicio 23

fartocel :: Float -> Float
fartocel x = (x - 32) / 1.8
------------------------------------------------------

--Ejercicio 24

hacefriof :: Float -> Bool
hacefriof x = (fartocel x) < 8
------------------------------------------------------

--Ejemplo Tuplas

tuplas :: (Int , Int) -> (Int , Int) -> (Int , Int)
tuplas (a,b) (c,d) = (a+c , b+d)
------------------------------------------------------

--Ejercicio 25

--Ejercicio 25.a
segundo3 :: (Int , Int , Int) -> Int
segundo3 (a , b , c) = b

--Ejercicio 25.b
ordena :: (Int , Int) -> (Int, Int)
ordena (a , b) | a <= b = (a , b)
			   | b < a = (b , a)

--Ejercicio 25.c
rangopreciopara :: Int -> (Int , Int) -> String
rangopreciopara x (a,b) | (0 < x) && (x < a) = "muy barato"
						| x > b = "demasiado caro"
						| (a < x) && (x < b) = "hay que verlo bien"
						| x < 0 = "esto no puede ser"

--Ejercicio 25.d
mayor3 :: (Int , Int , Int) -> (Bool , Bool , Bool)
mayor3 (a,b,c) = (a > 3 , b > 3 , c > 3)
 
--Ejercicio 25.e
todosiguales :: (Int , Int , Int) -> Bool
todosiguales (a,b,c) = (a == b) && (b == c) && (c == a)



