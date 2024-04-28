import Test.HUnit
import Dibujo
import System.Exit (exitFailure)

-- Para ejecutar este test se debe comentar las líneas 4 a 23 de Dibujo.sh (lo que oculta la implementación del módulo Dibujo)

-- Definimos una figura de ejemplo para utilizar en las pruebas
figuraEjemplo = figura "Ejemplo"

-- test de dibujo.h
testFigura = TestCase (assertEqual "figura" (Figura "Ejemplo") figuraEjemplo)

testRotar = TestCase (assertEqual "rotar" (Rotar (Figura "Ejemplo")) (rotar figuraEjemplo))

testEspejar = TestCase (assertEqual "espejar" (Espejar (Figura "Ejemplo")) (espejar figuraEjemplo))

testRot45 = TestCase (assertEqual "rot45" (Rot45 (Figura "Ejemplo")) (rot45 figuraEjemplo))

testApilar = TestCase (assertEqual "apilar" (Apilar 1 1 (Figura "Ejemplo") (Figura "Ejemplo")) (apilar 1 1 figuraEjemplo figuraEjemplo))

testJuntar = TestCase (assertEqual "juntar" (Juntar 1 1 (Figura "Ejemplo") (Figura "Ejemplo")) (juntar 1 1 figuraEjemplo figuraEjemplo))

testEncimar = TestCase (assertEqual "encimar" (Encimar (Figura "Ejemplo") (Figura "Ejemplo")) (encimar figuraEjemplo figuraEjemplo))

testR180 = TestCase (assertEqual "r180" (Rotar (Rotar (Figura "Ejemplo"))) (r180 figuraEjemplo))

testR270 = TestCase (assertEqual "r270" (Rotar (Rotar (Rotar (Figura "Ejemplo")))) (r270 figuraEjemplo))

-- tests de foldib
testFoldDib1 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (figura 1)" 1 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Figura 1)))
testFoldDib2 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Rotar (Figura 1))" 1 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Rotar (Figura 1))))
testFoldDib3 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Espejar (Figura 1))" 1 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Espejar (Figura 1))))
testFoldDib4 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Rot45 (Figura 1))" 1 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Rot45 (Figura 1))))
testFoldDib5 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Juntar 1 1 (Figura 1) (Figura 2))" 3 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Juntar 1 1 (Figura 1) (Figura 2))))
testFoldDib6 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Apilar 1 1 (Figura 1) (Figura 2))" 3 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Apilar 1 1 (Figura 1) (Figura 2))))
testFoldDib7 = TestCase (assertEqual "foldDib id id id id (_ _ a b -> a+b) (_ _ a b -> a+b) (+) (Encimar (Figura 1) (Figura 2))" 3 (foldDib id id id id (\_ _ a b -> a+b) (\_ _ a b -> a+b) (+) (Encimar (Figura 1) (Figura 2))))

-- tests de mapDib
testMapDib1 = TestCase (assertEqual "mapDib figura (figura 1)" (Figura 1) (mapDib figura (Figura 1)))
testMapDib2 = TestCase (assertEqual "mapDib figura (Rotar (figura 1))" (Rotar (Figura 1)) (mapDib figura (Rotar (Figura 1))))
testMapDib3 = TestCase (assertEqual "mapDib figura (Espejar (figura 1))" (Espejar (Figura 1)) (mapDib figura (Espejar (Figura 1))))
testMapDib4 = TestCase (assertEqual "mapDib figura (Rot45 (figura 1))" (Rot45 (Figura 1)) (mapDib figura (Rot45 (Figura 1))))
testMapDib5 = TestCase (assertEqual "mapDib figura (Juntar 1 1 (figura 1) (figura 2))" (Juntar 1 1 (Figura 1) (Figura 2)) (mapDib figura (Juntar 1 1 (Figura 1) (Figura 2))))
testMapDib6 = TestCase (assertEqual "mapDib figura (Apilar 1 1 (figura 1) (figura 2))" (Apilar 1 1 (Figura 1) (Figura 2)) (mapDib figura (Apilar 1 1 (Figura 1) (Figura 2))))
testMapDib7 = TestCase (assertEqual "mapDib figura (Encimar (figura 1) (figura 2))" (Encimar (Figura 1) (Figura 2)) (mapDib figura (Encimar (Figura 1) (Figura 2))))


-- tests de la funcion figuras
testFiguras1 = TestCase (assertEqual "figuras (figura 1)" [1] (figuras (Figura 1)))
testFiguras2 = TestCase (assertEqual "figuras (Rotar (figura 1))" [1] (figuras (Rotar (Figura 1))))
testFiguras3 = TestCase (assertEqual "figuras (Espejar (figura 1))" [1] (figuras (Espejar (Figura 1))))
testFiguras4 = TestCase (assertEqual "figuras (Rot45 (figura 1))" [1] (figuras (Rot45 (Figura 1))))
testFiguras5 = TestCase (assertEqual "figuras (Juntar 1 1 (figura 1) (figura 2))" [1,2] (figuras (Juntar 1 1 (Figura 1) (Figura 2))))
testFiguras6 = TestCase (assertEqual "figuras (Apilar 1 1 (figura 1) (figura 2))" [1,2] (figuras (Apilar 1 1 (Figura 1) (Figura 2))))
testFiguras7 = TestCase (assertEqual "figuras (Encimar (figura 1) (figura 2))" [1,2] (figuras (Encimar (Figura 1) (Figura 2))))


conjuntoPruebas = TestList [
    TestLabel "testFigura" testFigura,
    TestLabel "testRotar" testRotar,
    TestLabel "testEspejar" testEspejar,
    TestLabel "testRot45" testRot45,
    TestLabel "testApilar" testApilar,
    TestLabel "testJuntar" testJuntar,
    TestLabel "testEncimar" testEncimar,
    TestLabel "testR180" testR180,
    TestLabel "testR270" testR270,
    TestLabel "testFolDib1" testFoldDib1,
    TestLabel "testFolDib2" testFoldDib2,
    TestLabel "testFolDib3" testFoldDib3,
    TestLabel "testFolDib4" testFoldDib4,
    TestLabel "testFolDib5" testFoldDib5,
    TestLabel "testFolDib6" testFoldDib6,
    TestLabel "testFolDib7" testFoldDib7,
    TestLabel "testMapDib1" testMapDib1,
    TestLabel "testMapDib2" testMapDib2,
    TestLabel "testMapDib3" testMapDib3,
    TestLabel "testMapDib4" testMapDib4,
    TestLabel "testMapDib5" testMapDib5,
    TestLabel "testMapDib6" testMapDib6,
    TestLabel "testMapDib7" testMapDib7,
    TestLabel "testFiguras1" testFiguras1,
    TestLabel "testFiguras2" testFiguras2,
    TestLabel "testFiguras3" testFiguras3,
    TestLabel "testFiguras4" testFiguras4,
    TestLabel "testFiguras5" testFiguras5,
    TestLabel "testFiguras6" testFiguras6,
    TestLabel "testFiguras7" testFiguras7
    ]

-- Ejecutamos el conjunto de pruebas
main :: IO ()
main = do
  counts <- runTestTT conjuntoPruebas
  if errors counts + failures counts == 0
    then putStrLn "All tests passed"
    else exitFailure