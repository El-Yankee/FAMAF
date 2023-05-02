------- Ejercicio 1 ---------
data Numeracion = Cero | Uno | Dos | Tres | Cuatro deriving (Show)

-- a)
misma_numeracion :: Numeracion -> Numeracion -> Bool
misma_numeracion Cero Cero = True
misma_numeracion Uno Uno = True
misma_numeracion Dos Dos = True
misma_numeracion Tres Tres = True
misma_numeracion Cuatro Cuatro = True
misma_numeracion _ _ = False

-- b)
data Domino = Ficha Numeracion Numeracion

encajar :: Domino -> Domino -> Maybe Numeracion
encajar (Ficha numarriba1 numabajo1) (Ficha numarriba2 numabajo2) | misma_numeracion numabajo1 numarriba2 = Just(numabajo2)
                                                                  | otherwise = Nothing

------- Ejercicio 2 ---------
compatibles :: [Domino] -> Numeracion -> [Numeracion]
compatibles [] n = []
compatibles ((Ficha numarriba numabajo):ds) n | misma_numeracion numarriba n = numabajo : compatibles ds n
                                              | otherwise = compatibles ds n

------- Ejercicio 3 ---------
type Evento = String
data Categoria = Cumple | Reunion | Otro
data Calendario = Agendar Evento Categoria Calendario | SinEventos

-- variable para probar la funcion, tiene que dar ["Tesis","Parcial"]
calendario = Agendar "Tesis" Reunion (Agendar "Cumple" Cumple (Agendar "Parcial" Reunion SinEventos))

misma_categoria :: Categoria -> Categoria -> Bool
misma_categoria Cumple Cumple = True
misma_categoria Reunion Reunion = True
misma_categoria Otro Otro = True
misma_categoria _ _ = False

listar_reuniones :: Calendario -> [Evento]
listar_reuniones SinEventos = []
listar_reuniones (Agendar evento categoria restoCalend) | misma_categoria categoria Reunion = evento : listar_reuniones restoCalend
                                                        | otherwise = listar_reuniones restoCalend