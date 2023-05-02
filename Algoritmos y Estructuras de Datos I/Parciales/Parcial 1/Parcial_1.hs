-------- Ejercicio 1

-- a)
type Titulo = String
type Artista = String
type Duracion = Int
data Genero = Rock | Blues | Pop | Jazz
data Cancion = Tema Titulo Artista Genero Duracion | Publicidad Duracion

-- b)
mismo_genero :: Genero -> Genero -> Bool
mismo_genero Rock Rock = True
mismo_genero Blues Blues = True
mismo_genero Pop Pop = True
mismo_genero Jazz Jazz = True
mismo_genero _ _ = False

-- c)
duracion_de :: Cancion -> Duracion
duracion_de (Tema titulo artista genero duracion) = duracion
duracion_de (Publicidad duracion) = duracion

-- d)
instance Eq Cancion
  where
  c1 == c2 = duracion_de c1 == duracion_de c2

instance Ord Cancion
  where
  c1 <= c2 = duracion_de c1 <= duracion_de c2

-------- Ejercicio 2
-- variable con la que probe
listaCancion = [(Tema "hola" "katy" Rock 15),(Tema "chau" "katy" Pop 30),(Publicidad 10),(Tema "adios" "katy" Pop 15) ]

solo_genero :: [Cancion] -> Genero -> [Titulo]
solo_genero [] gi = []
solo_genero ((Publicidad durac):cs) gi = solo_genero cs gi
solo_genero ((Tema tit art gen dura):cs) gi | mismo_genero gen gi = tit : solo_genero cs gi
                                            | otherwise = solo_genero cs gi

-------- Ejercicio 3
data ListaAsoc a b = Vacia | Nodo a b ( ListaAsoc a b ) deriving (Show)

type A = String
type B = Integer

-- variable que probe
listaprueba = Nodo "juan" 5 (Nodo "perdo" 8 (Nodo "miguel" 10 Vacia))

la_suma_mayores :: ListaAsoc A B -> B -> B
la_suma_mayores Vacia x = 0
la_suma_mayores (Nodo dato1 dato2 la) x | dato2 > x = dato2 + la_suma_mayores la x
                                        | otherwise = la_suma_mayores la x

-------- Ejercicio 4
data Arbol a = Hoja | Rama ( Arbol a ) a ( Arbol a ) deriving (Show)

-- a)
a_listar :: Arbol a -> [a]
a_listar Hoja = []
a_listar (Rama b a c) = a :( (a_listar b) ++ (a_listar c) )

-- b)
arbolprueba = Rama (Rama (Rama Hoja 1 Hoja) 7 (Rama Hoja 2 Hoja)) 5 (Rama Hoja 8 Hoja)

-- c)
-- resutlado = [5,7,1,2,8]


