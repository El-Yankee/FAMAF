module Interp
  ( interp,
    initial,
  )
where

import Dibujo
import FloatingPic
import Graphics.Gloss (Display (InWindow), color, display, makeColorI, pictures, translate, white, Picture)
import qualified Graphics.Gloss.Data.Point.Arithmetic as V

-- Dada una computación que construye una configuración, mostramos por
-- pantalla la figura de la misma de acuerdo a la interpretación para
-- las figuras básicas. Permitimos una computación para poder leer
-- archivos, tomar argumentos, etc.
initial :: Conf -> Float -> IO ()
initial (Conf n dib intBas) size = display win white $ withGrid fig size
  where
    win = InWindow n (ceiling size, ceiling size) (0, 0)
    fig = interp intBas dib (0, 0) (size, 0) (0, size)
    desp = -(size / 2)
    withGrid p x = translate desp desp $ pictures [p, color grey $ grid (ceiling $ size / 10) (0, 0) x 10]
    grey = makeColorI 100 100 100 100

-- Interpretación de (^^^)
ov :: Picture -> Picture -> Picture
ov p q = pictures [p, q]

rot :: FloatingPic -> FloatingPic
rot f d w h = f (d V.+ w) h (V.negate w)

r45 :: FloatingPic -> FloatingPic
r45 f d w h = f (d V.+ half (w V.+ h)) (half (w V.+ h)) (half (h V.- w))

esp :: FloatingPic -> FloatingPic
esp f d w h = f (d V.+ w) (V.negate w) h

enc :: FloatingPic -> FloatingPic -> FloatingPic
enc f g d w h = ov (f d w h) (g d w h)

jun :: Float -> Float -> FloatingPic -> FloatingPic -> FloatingPic
jun m n f g d w h = let r' = n / (m+n)
                        r = m / (m+n)
                        w' = r V.* w
                    in ov (f d w' h) (g (d V.+ w') (r' V.*w) h)

api :: Float -> Float -> FloatingPic -> FloatingPic -> FloatingPic
api m n f g d w h = let r' = n / (m + n)
                        r = m / (m + n)
                        h' = r' V.* h
                    in ov (f (d V.+ h') w (r V.* h)) (g d w h') 

interp :: Output a -> Output (Dibujo a)
interp output = foldDib output rot esp r45 api jun enc