title: Laboratorio de Funcional
author: Anegli Mateo, Di Forte Federico y Usaj Santiago

---

La consigna del laboratorio está en https://tinyurl.com/funcional-2024-famaf

# 1. Tareas

Pueden usar esta checklist para indicar el avance.

## Verificación de que pueden hacer las cosas.

- [x] Haskell instalado y testeos provistos funcionando. (En Install.md están las instrucciones para instalar.)

## 1.1. Lenguaje

- [x] Módulo `Dibujo.hs` con el tipo `Dibujo` y combinadores. Puntos 1 a 3 de la consigna.
- [x] Definición de funciones (esquemas) para la manipulación de dibujos.
- [x] Módulo `Pred.hs`. Punto extra si definen predicados para transformaciones innecesarias (por ejemplo, espejar dos veces es la identidad).

## 1.2. Interpretación geométrica

- [x] Módulo `Interp.hs`.

## 1.3. Expresión artística (Utilizar el lenguaje)

- [x] El dibujo de `Dibujos/Feo.hs` se ve lindo.
- [x] Módulo `Dibujos/Grilla.hs`.
- [x] Módulo `Dibujos/Escher.hs`.
- [x] Listado de dibujos en `Main.hs`.

## 1.4 Tests

- [x] Tests para `Dibujo.hs`.
- [x] Tests para `Pred.hs`.

# 2. Experiencia

- Nos costo bastante arrancar a trabajar ya que teniamos muy poco recuerdo sobre el uso de Haskell y nos encontrabamos bastante perdidos. A medida que ibamos recordando como usar Haskell el trabajo se empezo a volver mas fluido. <br>
  A la hora de trabajar decidimos no dividir las tareas tan especificamente y en su lugar hicimos pair programming utilizando live share (debido a esto figuran tantos commits de un solo integrante del grupo). Esta tecnica es la que venimos utilizando en todos los labs de la carrera y ya estamos bastante acostumbrados.

# 3. Preguntas

1. ¿Por qué están separadas las funcionalidades en los módulos indicados? Explicar detalladamente la responsabilidad de cada módulo.

   Las funcionalidades estan separadas de esta forma para poder dividir esta implementación en la sitaxis y la semantica. Esto facilita la comprension, la organizacion y el testeo del codigo.

   Las responsabilidades de cada módulo son:

   - `Dibujo.hs`: definimos la estrucutura de Dibujo y sus funciones constructoras. Se realiza la declaración de la sintaxis de nuestro lenguaje.
   - `Escher.hs`: define como se va a dibujar la figura Escher.
   - `Floatingpic.hs`: define algunas funciones de utilidad para el manejo de vectores.
   - `Grilla.hs`: define una grilla para nuestro lenguaje.
   - `Interp.hs`: definimos la semantica de nuestro lenguaje, dando la interpretación geométrica de como van a ser nuestras figuras.
   - `Main.hs`: parsea los argumentos de entrada y llama a las funciones necesarias para generar las imagenes.
   - `Pred.hs`: define las funciones que permiten evaluar las propiedades de las figuras.
   - `TestDibujo.hs`: define los tests que se van a correr para verificar que el módulo de **Dibujo.hs** funcione correctamente.
   - `TestPred.hs`: define los tests que se van a correr para verificar que el módulo de **Pred.hs** funcione correctamente.

2. ¿Por qué las figuras básicas no están incluidas en la definición del lenguaje, y en vez de eso, es un parámetro del tipo?

   Las figuras básicas no están incuidas en la definción del lenguaje ya que esto nos da una mayor abstracción y nos permite crear las figuras que queramos e ingresarlo como parametro a las funciones sin tener que hacer pattern-matching. Ademas, esto permite que en cada uso de este programa se pueda definir la(s) figura(s) basica(s) que se quiere(n) utilizar.

3. ¿Qué ventaja tiene utilizar una función de `fold` sobre hacer pattern-matching directo?

   Las ventajas de usar fold es que este nos permite aplicar nuevas funciones a todas las basicas de un dibujo de una forma mas abstracta, sin tener que estar pensando (tanto) en las distintas formas que podria tener nuestro dibujo.

4. ¿Cuál es la diferencia entre los predicados definidos en Pred.hs y los tests?

   La diferencia entre los predicados definidos en los test y en Pred.hs es que en este ultimo se puede ingresar ya sea un dibujo completo o partes de estos para ver si cumplen o no con los predicados dados. En los test le ingresamos un dibujo hecho a partir de funcionalidades y corroboramos que el dibujo realizado sea igual a un dibujo hecho por los constructores.
