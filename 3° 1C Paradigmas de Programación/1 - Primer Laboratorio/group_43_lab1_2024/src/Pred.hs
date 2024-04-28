module Pred (
  Pred,
  cambiar,
  anyDib,
  allDib,
  orP,
  andP,
  falla
) where

import Dibujo
type Pred a = a -> Bool

-- Dado un predicado sobre básicas, cambiar todas las que satisfacen
-- el predicado por la figura básica indicada por el segundo argumento.
cambiar :: Pred a -> (a -> Dibujo a) -> Dibujo a -> Dibujo a
cambiar predicado f fig = mapDib (\x -> if predicado x then f x else figura x) fig  -- fig podria no estar en esta linea y funciona igual (?¿¡!!)

-- Alguna básica satisface el predicado.
anyDib :: Pred a -> Dibujo a -> Bool
anyDib pred fig = foldDib pred id id id (\ _ _ x y -> x || y) (\ _ _ x y -> x || y) (\x y -> x || y) fig  -- fig podria no estar en esta linea y funciona igual (?¿¡!!)

-- Todas las básicas satisfacen el predicado.
allDib :: Pred a -> Dibujo a -> Bool
allDib pred fig = foldDib pred id id id (\ _ _ x y -> x && y) (\ _ _ x y -> x && y) (\x y -> x && y) fig  -- fig podria no estar en esta linea y funciona igual (?¿¡!!)

-- Los dos predicados se cumplen para el elemento recibido.
andP :: Pred a -> Pred a -> Pred a --Pred a = (a -> Bool)
andP pred1 pred2 fig = pred1 fig && pred2 fig

-- Algún predicado se cumple para el elemento recibido.
orP :: Pred a -> Pred a -> Pred a --Pred a = (a -> Bool)
orP pred1 pred2 fig = pred1 fig || pred2 fig

falla = True
-- ^^^ venia en el archivo no se que es