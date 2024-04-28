module Dibujos.Escher where

import Dibujo 
import FloatingPic(Conf(..), Output, half, zero)
import Graphics.Gloss (text, scale, translate, blank, line, polygon, pictures)
import qualified Graphics.Gloss.Data.Point.Arithmetic as V
import Dibujos.Grilla(grilla)    

-- Supongamos que eligen.
type Escher = Bool

figEscher :: Output Escher
figEscher True x y w = pictures [line $ triangulo x y w, cara x y w]
    where
        triangulo x y w = map (x V.+) [zero, y, w, zero]
        cara x y w = polygon $ triangulo (x V.+ half w) (half y) (half w)

blankEsch :: Dibujo Escher
blankEsch = figura False

-- El dibujo u.
dibujoU :: Dibujo Escher -> Dibujo Escher
dibujoU e = encimar4 (espejar (rot45 e))

-- El dibujo t.
dibujoT :: Dibujo Escher -> Dibujo Escher
dibujoT e = encimar e (encimar (espejar (rot45 e)) (r270 (espejar (rot45 e))))

-- Esquina con nivel de detalle en base a la figura p.
esquina :: Int -> Dibujo Escher -> Dibujo Escher                         
esquina 1 e = cuarteto blankEsch blankEsch blankEsch (dibujoU e)
esquina n e = cuarteto (esquina (n-1) e)        (lado (n-1) e) 
                       (rotar $ lado (n-1) e)   (dibujoU e)

-- Lado con nivel de detalle.
lado :: Int -> Dibujo Escher -> Dibujo Escher
lado 1 e = cuarteto blankEsch blankEsch (rotar $ dibujoT e) (dibujoT e)
lado n e = cuarteto (lado (n-1) e)       (lado (n-1) e) 
                    (rotar $ dibujoT e)  (dibujoT e)

-- Por suerte no tenemos que poner el tipo!
noneto p q r s t u v w x = grilla [[p, q, r], [s, t, u], [v, w, x]]

-- El dibujo de Escher:
escher :: Int -> Escher -> Dibujo Escher
escher n e = noneto 
             (esquina n (figura e))         (lado n (figura e))        (r270 $ esquina n (figura e)) 
             (rotar $ lado n (figura e))    (dibujoU $ figura e)       (r270 $ lado n (figura e)) 
             (rotar $ esquina n (figura e)) (r180 $ lado n (figura e)) (r180 $ esquina n (figura e)) 

interpBas :: Output Escher
interpBas False _ _ _ = blank
interpBas True x y w = figEscher True x y w        -- EN ESA FUNCION ESTA EL DIBUJO BASICO

testAll :: Dibujo Escher
testAll = grilla [[escher 8 True]]                 -- ACA SE REGULA LA COMPLEJIDAD DEL DIBUJO

escherConf :: Conf
escherConf = Conf {
    name = "Escher"
    , pic = testAll
    , bas = interpBas
}