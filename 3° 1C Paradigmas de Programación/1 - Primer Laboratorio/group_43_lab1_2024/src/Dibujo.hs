module Dibujo (
    Dibujo,
    comp,
    figura,
    encimar,
    apilar,
    juntar,
    rot45,
    rotar,
    espejar,
    (^^^),
    (.-.),
    (///),
    r90,
    r180,
    r270,
    encimar4,
    cuarteto,
    ciclar,
    mapDib,
    change,
    foldDib,
    figuras
  )
where

-- nuestro lenguaje 
data Dibujo a = Figura a
    | Rotar (Dibujo a) 
    | Espejar (Dibujo a) 
    | Rot45 (Dibujo a)
    | Apilar (Float) (Float) (Dibujo a) (Dibujo a) 
    | Juntar (Float) (Float) (Dibujo a) (Dibujo a) 
    | Encimar (Dibujo a) (Dibujo a)
    deriving (Eq, Show)


-- combinadores
infixr 6 ^^^

infixr 7 .-.

infixr 8 ///

-- Composición n-veces de una función con sí misma. Componer 0 veces
-- es la funcion identidad, componer 1 vez es aplicar la función 1 vez, etc.
-- Componer negativamente es un error!
comp :: Int -> (a -> a) -> a -> a
comp 0 f a = a
comp n f a = comp (n-1) f (f a) 

-- Funciones constructoras
figura :: a -> Dibujo a
figura = Figura

encimar :: Dibujo a -> Dibujo a -> Dibujo a
encimar = Encimar

apilar :: Float -> Float -> Dibujo a -> Dibujo a -> Dibujo a
apilar = Apilar

juntar  :: Float -> Float -> Dibujo a -> Dibujo a -> Dibujo a
juntar = Juntar

rot45 :: Dibujo a -> Dibujo a
rot45 = Rot45

rotar :: Dibujo a -> Dibujo a
rotar = Rotar

espejar :: Dibujo a -> Dibujo a
espejar = Espejar

-- Superpone un dibujo con otro.
(^^^) :: Dibujo a -> Dibujo a -> Dibujo a
(^^^) = encimar

-- Pone el primer dibujo arriba del segundo, ambos ocupan el mismo espacio.
(.-.) :: Dibujo a -> Dibujo a -> Dibujo a
(.-.) = apilar 1.0 1.0

-- Pone un dibujo al lado del otro, ambos ocupan el mismo espacio.
(///) :: Dibujo a -> Dibujo a -> Dibujo a
(///) = juntar 1.0 1.0

-- rotaciones
r90 :: Dibujo a -> Dibujo a
r90 = rotar

r180 :: Dibujo a -> Dibujo a
r180 = comp 2 rotar

r270 :: Dibujo a -> Dibujo a
r270 = comp 3 rotar

-- una figura repetida con las cuatro rotaciones, superimpuestas.
encimar4 :: Dibujo a -> Dibujo a
encimar4 fig = (^^^) ((^^^) fig (r90 fig)) ((^^^) (r180 fig) (r270 fig)) 

-- cuatro figuras en un cuadrante.
cuarteto :: Dibujo a -> Dibujo a -> Dibujo a -> Dibujo a -> Dibujo a
cuarteto fig1 fig2 fig3 fig4 = (.-.) ((///) fig1 fig2) ((///) fig3 fig4) 

-- un cuarteto donde se repite la imagen, rotada (¡No confundir con encimar4!)
ciclar :: Dibujo a -> Dibujo a
ciclar fig = cuarteto fig (r90 fig) (r180 fig) (r270 fig)

-- map para nuestro lenguaje
mapDib :: (a -> Dibujo b) -> Dibujo a -> Dibujo b
mapDib f (Figura a) = f a
mapDib f (Rotar a) = Rotar (mapDib f a)
mapDib f (Espejar a) = Espejar (mapDib f a)
mapDib f (Rot45 a) = Rot45 (mapDib f a)
mapDib f (Apilar p1 p2 a b) = Apilar p1 p2 (mapDib f a) (mapDib f b)
mapDib f (Juntar p1 p2 a b) = Juntar p1 p2 (mapDib f a) (mapDib f b)
mapDib f (Encimar a b) = Encimar (mapDib f a) (mapDib f b)
-- verificar que las operaciones satisfagan
-- 1. map figura = id
-- 2. map (g . f) = mapDib g . mapDib f

-- Cambiar todas las básicas de acuerdo a la función.
change :: (a -> Dibujo b) -> Dibujo a -> Dibujo b
change f (Figura a) = f a
change f (Rotar a) = Rotar(change f a)
change f (Espejar a) = Espejar(change f a)
change f (Rot45 a) = Rot45(change f a)
change f (Apilar p1 p2 a b) = Apilar p1 p2 (change f a) (change f b)
change f (Juntar p1 p2 a b) = Juntar p1 p2 (change f a) (change f b)
change f (Encimar a b) = Encimar (change f a) (change f b)

-- Principio de recursión para Dibujos.
-- Estructura general para la semántica (a no asustarse). Ayuda: 
-- pensar en foldr y las definiciones de Intro a la lógica
-- foldDib aplicado a cada constructor de Dibujo deberia devolver el mismo dibujo
foldDib ::
  (a -> b) ->
  (b -> b) ->
  (b -> b) ->
  (b -> b) ->
  (Float -> Float -> b -> b -> b) ->
  (Float -> Float -> b -> b -> b) ->
  (b -> b -> b) ->
  Dibujo a ->
  b
foldDib figura rotar espejar rot45 apilar juntar encimar (Figura dibu) = figura dibu 

foldDib figura rotar espejar rot45 apilar juntar encimar (Rotar dibu) = rotar (foldDib figura rotar espejar rot45 apilar juntar encimar dibu)  

foldDib figura rotar espejar rot45 apilar juntar encimar (Espejar dibu) = espejar (foldDib figura rotar espejar rot45 apilar juntar encimar dibu)

foldDib figura rotar espejar rot45 apilar juntar encimar (Rot45 dibu) = rot45 (foldDib figura rotar espejar rot45 apilar juntar encimar dibu)

foldDib figura rotar espejar rot45 apilar juntar encimar (Apilar num1 num2 dib1 dib2) = apilar num1 num2 (foldDib figura rotar espejar rot45 apilar juntar encimar dib1) (foldDib figura rotar espejar rot45 apilar juntar encimar dib2) 

foldDib figura rotar espejar rot45 apilar juntar encimar (Juntar num1 num2 dib1 dib2) = juntar num1 num2 (foldDib figura rotar espejar rot45 apilar juntar encimar dib1) (foldDib figura rotar espejar rot45 apilar juntar encimar dib2)

foldDib figura rotar espejar rot45 apilar juntar encimar (Encimar dib1 dib2) = encimar (foldDib figura rotar espejar rot45 apilar juntar encimar dib1)  (foldDib figura rotar espejar rot45 apilar juntar encimar dib2)

-- Extrae todas las figuras básicas de un dibujo.
figuras :: Dibujo a -> [a]
figuras (Figura a) = [a]
figuras (Rotar a) = figuras a
figuras (Espejar a) = figuras a
figuras (Rot45 a) = figuras a
figuras (Apilar p1 p2 a b) = figuras a ++ figuras b
figuras (Juntar p1 p2 a b) = figuras a ++ figuras b
figuras (Encimar a b) = figuras a ++ figuras b