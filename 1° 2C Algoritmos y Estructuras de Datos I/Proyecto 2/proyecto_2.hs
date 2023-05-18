--------------- Proyecto 2 - Usaj Santiago ---------------------------------------

------- Ejercicio 1 ---------
-- a)
data Carrera = Matematica | Fisica | Computacion | Astronomia deriving (Eq)

-- b)
titulo :: Carrera -> String
titulo Matematica = "Licenciatura en Matematica"
titulo Fisica = "Licenciatura en Fisica"
titulo Computacion = "Licenciatura en Cs de la Computacion"
titulo Astronomia = "Licenciatura en Astronomia"

-- c)
data NotaBasica = Do | Re | Mi | Fa | Sol | La | Si deriving (Eq , Ord, Show, Bounded)

-- d)
cifradoAmericano :: NotaBasica -> Char
cifradoAmericano Do = 'C'
cifradoAmericano Re = 'D'
cifradoAmericano Mi = 'E'
cifradoAmericano Fa = 'F'
cifradoAmericano Sol = 'G'
cifradoAmericano La = 'A'
cifradoAmericano Si = 'B'

------- Ejercicio 2 ---------

-- Completado en el 1.c

------- Ejercicio 3 ---------
-- a)
minimoElemento :: Ord a => [a] -> a
minimoElemento [a] = a
minimoElemento (x:xs) = x `min` minimoElemento xs

-- b)
minimoElemento' :: (Bounded a , Ord a) => [a] -> a
minimoElemento' [] = maxBound
minimoElemento' (x:xs) = x `min` minimoElemento' xs

-- c)
-- Completado en el 1.c

------- Ejercicio 4 ---------
-- a)
type Ingreso = Int
data Cargo = Titular | Asociado | Adjunto | Asistente | Auxiliar deriving (Eq)
data Area = Administrativa | Ensenanza | Economica | Postgrado deriving (Eq)
data Persona = Decano | Docente Cargo | NoDocente Area | Estudiante Carrera Ingreso deriving (Eq)

-- b)
-- Docente es del tipo constructor con parametro

-- c)
cuantos_doc :: [Persona] -> Cargo -> Int
cuantos_doc [] c = 0
cuantos_doc (x:xs) c    | x == Docente c = 1 + cuantos_doc xs c
                        | otherwise = cuantos_doc xs c
                        
-- d)
cuantos_doc' :: [Persona] -> Cargo -> Int
cuantos_doc' (x:xs) c = length (filter (Docente c == ) (x:xs)) 

------- Ejercicio 5 ---------
-- a)
data Alteracion = Bemol | Sostenido | Natural deriving (Eq)
data NotaMusical = Nota NotaBasica Alteracion

sonido :: NotaBasica -> Int
sonido Do = 1
sonido Re = 3
sonido Mi = 5
sonido Fa = 6
sonido Sol = 8
sonido La = 10
sonido Si = 12  

-- b)
sonidoCromatico :: NotaMusical -> Int
sonidoCromatico (Nota a b) | b == Bemol = sonido a - 1
                           | b == Sostenido = sonido a + 1
                           | b == Natural = sonido a
                           
 -- c)
instance Eq NotaMusical
    where 
        n1 == n2 = sonidoCromatico n1 == sonidoCromatico n2

-- d)
instance Ord NotaMusical
    where 
        n1 <= n2 = sonidoCromatico n1 <= sonidoCromatico n2
 
------- Ejercicio 6 ---------
primerElemento :: [a] -> Maybe a
primerElemento [] = Nothing
primerElemento (x:xs) = Just x

------- Ejercicio 7 ---------
-- a)
data Cola = VaciaC | Encolada Persona Cola

atender :: Cola -> Maybe Cola
atender VaciaC = Nothing
atender (Encolada a b) = Just b

encolar :: Persona -> Cola -> Cola
encolar p VaciaC = (Encolada p VaciaC)
encolar p (Encolada a b) = Encolada a (encolar p b)

busca :: Cola -> Cargo -> Maybe Persona
busca VaciaC c = Nothing
busca (Encolada a b) c | a == Docente c = Just a
                                       | otherwise = busca b c

-- b)
-- Recursion sobre una lista


------- Ejercicio 8 ---------
data ListaAsoc a b = Vacia | Nodo a b ( ListaAsoc a b ) deriving (Eq, Show)
type Diccionario = ListaAsoc String String
type Padron = ListaAsoc Int String
  
-- a) 
type Guia = ListaAsoc Int String

-- b)
--- 1)
la_long :: ListaAsoc a b -> Int
la_long Vacia = 0
la_long (Nodo a b c) = 1 + la_long(c)

--- 2)
la_concat :: ListaAsoc a b -> ListaAsoc a b -> ListaAsoc a b
la_concat Vacia Vacia = Vacia
la_concat Vacia c = c
la_concat c Vacia = c
la_concat (Nodo a b c) (Nodo d e f) = (Nodo a b (la_concat c (Nodo d e f)))

--- 3)
la_agregar :: ListaAsoc a b -> a -> b -> ListaAsoc a b
la_agregar Vacia a b = (Nodo a b Vacia)
la_agregar (Nodo f g c) a b = (Nodo f g (la_agregar (c) a b))

--- 4)
la_pares :: ListaAsoc a b -> [(a, b)]
la_pares Vacia = []
la_pares (Nodo a b c) = [(a,b)] ++ la_pares c

--- 5)
la_busca :: Eq a => ListaAsoc a b -> a -> Maybe b
la_busca Vacia d = Nothing
la_busca (Nodo a b c)  d    | d == a = Just b
                                            | otherwise = la_busca c d   

--- 6)
la_borrar :: Eq a => a -> ListaAsoc a b -> ListaAsoc a b
la_borrar x Vacia = Vacia
la_borrar x (Nodo a b c)      | x == a = (la_borrar x c) 
                                            | otherwise = Nodo a b (la_borrar x c)
                                            
------- Ejercicio 9 ---------
data Arbol a = Hoja | Rama ( Arbol a ) a ( Arbol a ) deriving (Show)

-- a)
a_long :: Arbol a -> Int 
a_long Hoja = 0
a_long (Rama b a c) = 1 + (a_long b) + (a_long c)

-- b)
a_hojas :: Arbol a -> Int
a_hojas Hoja = 1
a_hojas (Rama b a c) = a_hojas b + a_hojas c

-- c)
a_inc :: Num a => Arbol a -> Arbol a
a_inc Hoja = Hoja
a_inc (Rama a b c) = (Rama (a_inc a) (b+1) (a_inc c))

-- d)
a_map :: (a -> b) -> Arbol a -> Arbol b
a_map f Hoja = Hoja
a_map f (Rama b a c) = Rama (a_map f b) (f(a)) (a_map f c)

------- Ejercicios Bonus ---------

-- Como comparar sin usar deriving Eq
data CargosAux = Presidente | VicePresidente | Senador

mismoCargo :: CargosAux -> CargosAux -> Bool
mismoCargo Presidente Presidente = True
mismoCargo VicePresidente VicePresidente = True
mismoCargo Senador Senador = True
mismoCargo _ _ = False

-- Como utilizar el case
data Dia = Lunes | Martes | Miercoles | Jueves | Viernes | Sabado | Domingo
data Tarea = Trabajar | TrabajarPoco | Joda | Descansar

tareaDiaria :: Dia -> Tarea
tareaDiaria Viernes = TrabajarPoco
tareaDiaria Sabado = Joda
tareaDiaria Domingo = Descansar
tareaDiaria _ = Trabajar

horasTrabajo :: Dia -> Int
horasTrabajo d = case tareaDiaria d of
                    Trabajar -> 8
                    TrabajarPoco -> 4
                    _ -> 0