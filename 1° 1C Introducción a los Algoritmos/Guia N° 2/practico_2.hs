{--
Ejercico 1

a) lenght [5,6,7] = 3

b) [5,3,57] !! 1 = 3

c) [0,11,2,5] : [] = [[0,11,2,5]]

d) take 2 [5,6,7] = [5,6] 

e) drop 2 [5,6,7] = [7]

f) head ( 0 : [1,2,3]) = 0


------------------------------------------------------
Definiciones

filter: dada una lista devuelve otra lista cuyos elementos son los elementos de la primera que cumplan una determinada condición.

map: dada una lista devuelve otra lista cuyos elementos son los que se obtienen de aplicar una función a cada elemento de la primera en el mismo orden y con las mismas repeticiones.  

fold: dada una lista devuelve un valor resultante de combinar los elementos de la lista.

zip: dadas dos listas devuelve una lista de pares donde el primer elemento de cada par se corresponde con la primera lista, y el segundo elemento de cada par se corresponde con la segunda lista.

unzip: dada una lista de tuplas devuelve una lista de alguna de las proyecciones de la tupla.

--}
------------------------------------------------------

--Ejercicio 3 (funciones filter)

--Ejercicio 3.a
soloPares :: [Int] -> [Int]
soloPares [] = []
soloPares (x:xs) | (mod x 2) == 0 = x:soloPares xs
                 | (mod x 2) /= 0 = soloPares xs

--Ejercicio 3.b
mayoresQue10 :: [Int] -> [Int]
mayoresQue10 [] = []
mayoresQue10 (x:xs) | x > 10  = x:mayoresQue10 xs
                    | x <= 10 = mayoresQue10 xs
                    
--Ejercicio 3.c
mayoresQue :: Int -> [Int] -> [Int]
mayoresQue n [] = []
mayoresQue n (x:xs) | x > n  = x:mayoresQue n xs
                    | x <= n = mayoresQue n xs
------------------------------------------------------

--Ejercicio 4 (funciones map)

--Ejercicio 4.a
sumar1 :: [Int] -> [Int]
sumar1 [] = []
sumar1 (x:xs) = (x + 1):sumar1 xs

--Ejercicio 4.b
duplica :: [Int] -> [Int]
duplica [] = []
duplica (x:xs) = (2 * x):duplica xs

--Ejercicio 4.c
multiplica :: Int -> [Int] -> [Int]
multiplica n [] = []
multiplica n (x:xs) = (n * x):multiplica n xs
------------------------------------------------------

--Ejercicio 5 (funciones fold)

--Ejercicio 5.a
todosMenores10 :: [Int] -> Bool
todosMenores10 [] = True 
todosMenores10 (x:xs) = (x < 10) && todosMenores10 xs

--Ejercicio 5.b
hay0 :: [Int] -> Bool
hay0 [] = False
hay0 (x:xs) = (x == 0) || hay0 xs

--Ejercicio 5.c
sumatoria :: [Int] -> Int
sumatoria [] = 0
sumatoria (x:xs) = x + sumatoria xs
------------------------------------------------------

--Ejercicio 6 (funciones zip)

repartir :: [String] -> [String] -> [(String,String)]
repartir [] [] = []
repartir [] (y:ys) = [("No hay mas personas" , "Si hay Cartas")]
repartir (x:xs) [] = [("Hay mas personas" , "No hay Cartas")]
repartir (x:xs) (y:ys) = (x,y):repartir xs ys
------------------------------------------------------

--Ejercicio 7 (funciones unzip)

apellidos :: [(String, String, Int)] -> [String]
apellidos [] = []
apellidos ((a,b,c):xs) = b:apellidos xs 
------------------------------------------------------

--Ejercicio 8

--Cardinal
cardinal :: [a] -> Int
cardinal [] = 0
cardinal (x:xs) = 1 + cardinal xs

--Pegar al final
pegarAlFinal :: [a] -> a -> [a]
pegarAlFinal [] a = a:[]
pegarAlFinal (x:xs) a = x:pegarAlFinal xs a

--Indice
indice :: Int -> [a] -> a
indice n [a] = a
indice n (x:xs) | n > 0 = indice (n-1) xs
                | n == 0 = x

--Concatenar
concatenar :: [a] -> [a] -> [a]
concatenar [] ys = ys
concatenar (x:xs) ys = x:(concatenar xs ys)

--Tomar
tomar :: [a] -> Int -> [a]
tomar xs 0 = []
tomar [] n = []
tomar (x:xs) n = x:(tomar xs (n-1))

--Tirar
tirar :: [a] -> Int -> [a]
tirar [] n = []
tirar xs 0 = xs
tirar (x:xs) n = tirar xs (n-1)
------------------------------------------------------

--Ejercicio 9

--Ejercicio 9.a (funcion fold)
maximo :: [Int] -> Int
maximo [a] = a 
maximo (x:xs) = max x (maximo xs)

maximoV2 :: [Int] -> Int
maximoV2 [a] = a 
maximoV2 (x:(y:xs)) | x > y = maximoV2 (x:xs)
                    | y > x = maximoV2 (y:xs)

--Ejercicio 9.b (funcion fold) 
sumaPares :: [(Int,Int)] -> Int
sumaPares [] = 0
sumaPares ((a,b):xs) = (a+b) + sumaPares xs

--Ejercicio 9.c (funcion fold)
todos0y1 :: [Int] -> Bool
todos0y1 [] = True
todos0y1 (x:xs) = ((x == 0) || (x == 1)) && todos0y1 xs

--Ejercicio 9.d (funcion filter)
quitar0s :: [Int] -> [Int]
quitar0s [] = []
quitar0s (x:xs) | x == 0 = quitar0s xs
                | otherwise = x:(quitar0s xs)

--Ejercicio 9.e 
ultimo :: [a] -> a
ultimo [a] = a
ultimo (x:xs) = ultimo xs

--Ejercicio 9.f
repetir :: Int -> Int -> [Int]
repetir 0 n = []
repetir n k = k:(repetir (n-1) k)

--Ejercicio 9.g
concaten :: [[a]] -> [a]
concaten [] = []
concaten (x:xs) = x ++ (concaten xs)

--Ejercicio 9.h
rev :: [a] -> [a]
rev [] = []
rev (x:xs) = (rev xs)++[x]
------------------------------------------------------

--Ejercicio 20

--Ejercicio 20.a
listasIguales :: [Int] -> [Int] -> Bool
listasIguales [] [] = True
listasIguales xs [] = False
listasIguales [] ys = False
listasIguales (x:xs) (y:ys) = (x == y) && listasIguales xs ys

--Ejercicio 20.b
mejorNota :: [(String , Int , Int , Int)] -> [(String , Int)]
mejorNota [] = []
mejorNota ((a,b,c,d):xs) = (a,max b (max c d)):(mejorNota xs)

--Ejercicio 20.c
incPrim :: [(Int , Int)] -> [(Int , Int)]
incPrim [] = []
incPrim ((a,b):xs) = (a+1,b):(incPrim xs)

--Ejercicio 20.d
expandir :: String -> String
expandir [] = []
expandir [a] = [a]
expandir (x:xs) = ([x]++" ")++(expandir xs)
------------------------------------------------------

--Ejercicio 21

--Ejercicio 21.a
verTodas :: [(String , Int , Int , String)] -> Int
verTodas [] = 0
verTodas ((a,b,c,d):xs) = c + verTodas xs

--Ejercicio 21.b
estrenos :: [(String , Int , Int , String)] -> [String]
estrenos [] = []
estrenos ((a,b,c,d):xs) | b == 2016 = a:(estrenos xs)
                        | otherwise = estrenos xs

--Ejercicio 21.c
filmografia :: [(String , Int , Int , String)] -> String -> [String]
filmografia [] n = []
filmografia ((a,b,c,d):xs) n | d == n  = a:(filmografia xs n)
                             | otherwise = filmografia xs n

--Ejercicio 21.d
duracion :: [(String , Int , Int , String)] -> String -> Int
duracion [] n = 0
duracion ((a,b,c,d):xs) n | a == n = c
                          | otherwise = duracion xs n
------------------------------------------------------

--Ejercicio 22

vendedores = ["Martin", "Diego", "Claudio", "José"]

ventas = [((1,2,2006), "Martin", ["Monitor GPRS 3000", "Motherboard ASUS 1500"]), ((1,2,2006), "Diego", ["Monitor ASC 543", "Motherboard Pindorcho"]), ((10,2,2006), "Martin", ["Monitor ASC 543", "Motherboard ASUS 1200"]), ((12,2,2006), "Diego", ["Monitor GPRS 3000", "Motherboard ASUS 1200"]), ((4,3,2006), "Diego", ["Monitor GPRS 3000", "Motherboard ASUS 1500"])]

precios = [("Monitor GPRS 3000", 200), ("Motherboard ASUS 1500", 120), ("Monitor ASC 543", 250), ("Motherboard ASUS 1200", 100), ("Motherboard Pindorcho", 30)]

--Ejercicio 22.a
precioComp :: [(String , Integer)] -> String -> Integer
precioComp [] x = 0
precioComp ((a,b):xs) x | x == a = b
                        | otherwise = precioComp xs x

precioMaquina :: [String] -> Integer
precioMaquina [] = 0
precioMaquina (x:xs) = precioComp precios x + precioMaquina xs



















