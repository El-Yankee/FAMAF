------- Ejercicio 1 ---------
data Palo = Espada | Basto | Oro | Copa

-- a)
mismo_palo :: Palo -> Palo -> Bool
mismo_palo Espada Espada = True
mismo_palo Basto Basto = True
mismo_palo Oro Oro = True
mismo_palo Copa Copa = True
mismo_palo _ _ = False

-- b)
data Figura = Uno | Dos | Tres | Cuatro | Cinco | Seis | Siete | Sota | Caballo | Rey deriving (Show)

valor_figura :: Figura -> Int
valor_figura Uno = 1
valor_figura Dos = 2
valor_figura Tres = 3
valor_figura Cuatro = 4
valor_figura Cinco = 5
valor_figura Seis = 6
valor_figura Siete = 7
valor_figura Sota = 8
valor_figura Caballo = 9
valor_figura Rey = 10

data Carta = Naipe Figura Palo

suma_cartas :: Carta -> Carta -> Maybe Int
suma_cartas (Naipe figura1 palo1) (Naipe figura2 palo2) | mismo_palo palo1 palo2 = Just(valor_figura(figura1) + valor_figura(figura2))
                                                        | otherwise = Nothing

------- Ejercicio 2 ---------
compatibles :: Carta -> [Carta] -> [Figura]
compatibles c [] = []
compatibles (Naipe f p) ((Naipe figura palo):cs)  | (mismo_palo p palo) && ((valor_figura f + valor_figura figura) < 15)  = figura : compatibles (Naipe f p) (cs)
                                                  | otherwise = compatibles (Naipe f p) (cs)

------- Ejercicio 3 ---------
type Duracion = Int
type Nombre = String
data Cancion = Tema Nombre Duracion
data Estado = Escuchado | NoEscuchado
data Playlist = EnLista Cancion Estado Playlist | Vacia

-- variable para probar la funcion, tiene que dar 25
cancion1 = Tema "hola" 15
cancion2 = Tema "chau" 10
cancion3 = Tema "adios" 5
playlist1 = EnLista cancion1 Escuchado (EnLista cancion2 Escuchado(EnLista cancion3 NoEscuchado(Vacia)))

mismosEstados :: Estado -> Estado -> Bool
mismosEstados Escuchado Escuchado = True
mismosEstados NoEscuchado NoEscuchado = True
mismosEstados _ _ = False

tiempo_reproduccion :: Playlist -> Int
tiempo_reproduccion Vacia = 0
tiempo_reproduccion (EnLista (Tema nombre duracion) estado restoplaylist) | mismosEstados estado Escuchado = duracion + tiempo_reproduccion restoplaylist
                                                                          | otherwise = tiempo_reproduccion restoplaylist

------- Ejercicio 4* ---------
data Arbol a = Hoja | Rama ( Arbol a ) a ( Arbol a ) deriving (Show)

a_podar :: Arbol a -> Arbol a 
a_podar (Rama Hoja a Hoja) = Hoja
a_podar (Rama b a Hoja) = Rama (a_podar b) a Hoja
a_podar (Rama Hoja a c) = Rama Hoja a (a_podar c)
a_podar (Rama b a c) = Rama (a_podar b) a (a_podar c)