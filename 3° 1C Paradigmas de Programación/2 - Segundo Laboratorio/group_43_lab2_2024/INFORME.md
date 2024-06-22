---
title: Laboratorio de Programación Orientada a Objetos
author: Anegli Mateo, Di Forte Federico y Usaj Santiago
---

El enunciado del laboratorio se encuentra en [este link](https://docs.google.com/document/d/1wLhuEOjhdLwgZ4rlW0AftgKD4QIPPx37Dzs--P1gIU4/edit#heading=h.xe9t6iq9fo58).

# 1. Tareas

Pueden usar esta checklist para indicar el avance.

## Verificación de que pueden hacer las cosas.

- [x] Java 17 instalado. Deben poder compilar con `make` y correr con `make run` para obtener el mensaje de ayuda del programa.

## 1.1. Interfaz de usuario

- [x] Estructurar opciones
- [x] Construir el objeto de clase `Config`
- [x] Casos especiales (los indicados debajos de la tabla)

## 1.2. FeedParser

- [x] `class Article`
  - [x] Atributos
  - [x] Constructor
  - [x] Método `print`
  - [x] _Accessors_
- [x] `parseXML`

## 1.3. Entidades nombradas

- [x] Pensar estructura y validarla con el docente
- [x] Implementarla
- [x] Extracción
  - [x] Implementación de heurísticas
- [x] Clasificación
  - [x] Por tópicos
  - [x] Por categorías
- Estadísticas
  - [x] Por tópicos
  - [x] Por categorías
  - [x] Impresión de estadísticas

## 1.4 Limpieza de código

- [x] Pasar un formateador de código
- [x] Revisar TODOs

# 2. Experiencia

Este laboratorio nos pareció mucho más divertido y entretenido que el anterior. Gran parte se debe a que no somos grandes fanáticos del lenguaje funcional.
Pudimos repartirnos mejor las tareas y mantuvimos un ritmo fluido de trabajo. No encontramos grandes dificultades a la hora de trabajar con POO, pero si hemos usado Github Copilot. Al haber hecho uso de esta herramienta en el lab anterior, aprovechamos el tenerlo activado y experimentar con él, ya que ninguno de los 3 integrantes del grupo había utilizado alguna IA para programar anteriormente. Quedamos asombrados con la utilidad y eficiencia de la IA para realizar las tareas solicitadas. Nos permitió agilizar muchísimo el trabajo y realizar cosas que no sabíamos como eran en Java.

# 3. Preguntas

1. Explicar brevemente la estructura de datos elegida para las entidades nombradas.

- En la estructura de las entidades nombradas decidimos poner el nombre, la categoría y una lista de tópicos. En los tópicos se utiliza una lista ya que una entidad puede tener varios tópicos, mientras que la categoría es única.

2. Explicar brevemente cómo se implementaron las heurísticas de extracción.

- La heurística 1: Es la que nos dieron junto al kickstart
- La heurística 2: La idea de esta heurística es continuar con la forma más intuitiva de encontrar entidades entre palabras, que inicien con mayúsculas, pero aplicar más filtros y requisitos para que sean consideradas como tales. Esto en teoría debería dar mejores resultados porque se estaría evitando la detección de “falsos positivos”. Los requisitos extras que se implementaron fueron que las palabras tengan más de 3 letras para evitar palabras como IVA, ONG, CPU, CD, etc. y que no se tomen las palabras en mayúsculas que sean el inicio de una oración. Si bien esta heurística no es perfecta, mejoró bastante los resultados en los textos utilizados en este laboratorio. Podría haber ejemplos en los cuales empeoren los resultados y obviamente se puede seguir perfeccionando, pero nos pareció una mejora buena para la mayoría de casos reales.
- La heurística 3: Compara cada palabra del texto con una lista de palabras extraídas de un diccionario convencional, donde todas son sustantivos comunes, adjetivos, pronombres, etc. Por ende las que no pertenecen a la lista son posibles candidatos a ser nombres propios y por lo tanto entidades.

# 4. Correcciones para la Reentrega

- [x] Corregir FeedParser
- [x] Crear subclases de las categorias
- [x] Modularizar run
