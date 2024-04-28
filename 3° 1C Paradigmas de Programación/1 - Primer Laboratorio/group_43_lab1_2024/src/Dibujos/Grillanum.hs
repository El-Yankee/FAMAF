module Dibujos.Grillanum where

import Dibujo (Dibujo, figura)
import FloatingPic(Conf(..), Output)
import Graphics.Gloss (text, scale, translate)
import Dibujos.Grilla(grilla)

type Coord = (Int, Int)

getCoords :: [[Dibujo Coord]]
getCoords = [getRow x | x <- [0..7]] 
    where
    getRow x = [getValue x y | y <- [0..7]]
    getValue x y = figura (x, y)

interpBas :: Output Coord
interpBas coordenadas (x, y) (sw, _) (_, sh) =  translate (x + (sw / 3)) (y + (sh / 3)) $ scale 0.15 0.15 $ text $ show coordenadas

testAll :: Dibujo Coord
testAll = grilla getCoords

grillanumConf :: Conf
grillanumConf = Conf {
    name = "Grillanum"
    , pic = testAll
    , bas = interpBas
}