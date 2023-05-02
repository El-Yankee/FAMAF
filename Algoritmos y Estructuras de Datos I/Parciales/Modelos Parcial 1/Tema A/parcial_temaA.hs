------- Ejercicio 1 ---------
data Forma = Piedra | Papel | Tijera

-- a)

le_gana :: Forma -> Forma -> Bool
le_gana Piedra Tijera = True
le_gana Piedra Piedra = False
le_gana Piedra Papel = False
le_gana Papel Piedra = True
le_gana Papel Papel = False
le_gana Papel Tijera = False
le_gana Tijera Papel = True
le_gana Tijera Tijera = False
le_gana Tijera Piedra = False

-- b)

type Nombre = String
data Jugador = Mano Nombre Forma

ganador :: Jugador -> Jugador -> Maybe Nombre
ganador (Mano nombre_J1 forma_J1) (Mano nombre_J2 forma_J2) | le_gana forma_J1 forma_J2 = Just(nombre_J1)
                                                            | le_gana forma_J2 forma_J1 = Just(nombre_J2)
                                                            | otherwise = Nothing

------- Ejercicio 2 ---------
mismaForma :: Forma -> Forma -> Bool
mismaForma Piedra Piedra = True
mismaForma Papel Papel = True
mismaForma Tijera Tijera = True
mismaForma _ _ = False

quien_jugo :: Forma -> [Jugador] -> [Nombre]
quien_jugo f [] = []
quien_jugo f ((Mano nombre forma):xs) | mismaForma f forma = nombre : quien_jugo f (xs)
                                      | otherwise = quien_jugo f (xs)

------- Ejercicio 3 ---------
data NotaMusical = Do | Re | Mi | Fa | Sol | La | Si 
data Figura = Negra | Corchea
data Melodia = Entonar NotaMusical Figura Melodia | Vacia

-- variable para probar la funcion, tiene que dar 7
pink = Entonar Re Negra (Entonar Mi Corchea (Entonar Fa Negra (Entonar Mi Negra Vacia)))

mismaFigura :: Figura -> Figura -> Bool
mismaFigura Negra Negra = True
mismaFigura Corchea Corchea = True
mismaFigura _ _ = False

contar_tiempos :: Melodia -> Int
contar_tiempos Vacia = 0
contar_tiempos (Entonar nota fig restoMelodia)  | mismaFigura fig Negra = 2 + contar_tiempos (restoMelodia)
                                                | otherwise = 1 + contar_tiempos(restoMelodia)

------- Ejercicio 4* ---------
data Arbol a = Hoja | Rama ( Arbol a ) a ( Arbol a ) deriving (Show)

arbol_sum :: Arbol Int -> Arbol Int -> Arbol Int
arbol_sum Hoja Hoja = Hoja
arbol_sum Hoja bs = bs
arbol_sum as Hoja = as
arbol_sum (Rama b a c) (Rama e d f) = Rama (arbol_sum b e) (a+d) (arbol_sum c f)