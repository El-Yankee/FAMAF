// Ejercicio 1
// Buscar los documentos donde el alumno tiene:
/* (i) un puntaje mayor o igual a 80  en "exam" o bien un puntaje mayor o igual a 90 en "quiz" y 
(ii) un puntaje mayor o igual a 60 en todos los "homework" (en otras palabras no tiene un puntaje menor a 60 en algún "homework") 
Se debe mostrar todos los campos excepto el _id, ordenados por el id de la clase y id del alumno en orden descendente y ascendente respectivamente.. */

use("university");

db.grades
  .find(
    {
      $and: [
        {
          $or: [
            {
              scores: {
                $elemMatch: {
                  score: {
                    $gte: 80,
                  },
                  type: "exam",
                },
              },
            },
            {
              scores: {
                $elemMatch: {
                  score: {
                    $gte: 90,
                  },
                  type: "quiz",
                },
              },
            },
          ],
        },
        {
          scores: {
            $elemMatch: {
              score: {
                $gte: 60,
              },
              type: "homework",
            },
          },
        },
      ],
    },
    // Proyección
    {
      _id: 0,
    }
  )
  .sort({ class_id: -1, student_id: 1 });

//Ejercicio 2
/* Calcular el puntaje mínimo, promedio, y máximo que obtuvo el alumno en las clases 20, 220, 420. El resultado debe mostrar además el id de la clase y el id del alumno, ordenados por alumno y clase en orden ascendentes.  */
use("university");

db.grades.aggregate([
  {
    $match: {
      $or: [
        {
          class_id: 20,
        },
        {
          class_id: 220,
        },
        {
          class_id: 420,
        },
      ],
    },
  },
  {
    $sort: { class_id: 1, student_id: 1 },
  },
  {
    $project: {
      _id: 0,
      class_id: 1,
      student_id: 1,
      minRating: { $min: "$scores.score" },
      maxRating: { $max: "$scores.score" },
      avgRating: { $avg: "$scores.score" },
    },
  },
]);

// Ejercicio 3
/* Para cada clase listar el puntaje máximo de las evaluaciones de tipo "exam" y el puntaje máximo de las evaluaciones de tipo "quiz". Listar en orden ascendente por el id de la clase. HINT: El operador $filter puede ser de utilidad. */
use("university");

db.grades.aggregate([
  {
    $unwind: "$scores",
  },
  {
    $group: {
      _id: "$class_id",
      minRating: { $min: "$scores.score" },
      maxRating: { $max: "$scores.score" },
      avgRating: { $avg: "$scores.score" },
    },
  },
  {
    $sort: {
      _id: 1,
    },
  },

  {
    $project: {
      _id: 1,
      minRating: 1,
      maxRating: 1,
      avgRating: 1,
    },
  },
]);

// Ejercicio 4
/* Crear una vista "top10students" que liste los 10 estudiantes con los mejores promedios. */
use("university");

db.createView("top10students", "grades", [
  {
    $unwind: "$scores",
  },
  {
    $group: {
      _id: "$student_id",
      avgRating: { $avg: "$scores.score" },
    },
  },
  {
    $sort: { avgRating: -1 },
  },
  {
    $limit: 10,
  },

  {
    $project: {
      _id: 0,
      student_id: "$_id",
      avgRating: 1,
    },
  },
]);

use("university");
db.top10students.drop();
use("university");
db.top10students.find();

// Ejercicio 5
/* Actualizar los documentos de la clase 339, agregando dos nuevos campos: el campo "score_avg" que almacena el puntaje promedio y el campo "letter" que tiene el valor "NA" si el puntaje promedio está entre [0, 60), el valor "A" si el puntaje promedio está entre [60, 80) y el valor "P" si el puntaje promedio está entre [80, 100]. 
HINTS: (i) para actualizar se puede usar pipeline de agregación. (ii) El operador $cond o $switch pueden ser de utilidad.
 */
use("university");

db.grades.updateMany(
  {
    class_id: 339,
  },
  [
    {
      $set: {
        score_avg: { $avg: "$scores.score" },
        letter: {
          $switch: {
            branches: [
              {
                case: {
                  $and: [
                    { $gte: ["score_avg", 0] },
                    { $lt: ["score_avg", 60] },
                  ],
                },
                then: "NA",
              },
              {
                case: {
                  $and: [
                    { $gte: ["score_avg", 60] },
                    { $lt: ["score_avg", 80] },
                  ],
                },
                then: "A",
              },
              {
                case: {
                  $and: [
                    { $gte: ["score_avg", 80] },
                    { $lte: ["score_avg", 100] },
                  ],
                },
                then: "P",
              },
            ],
            default: "N/A", // Valor por defecto si no cumple ninguna condición
          },
        },
      },
    },
  ]
);

use("university");

db.grades.find({ class_id: 339 });

// Ejercicio 6
/* (a) Especificar reglas de validación en la colección grades para todos sus campos y subdocumentos anidados. Inferir los tipos y otras restricciones que considere adecuados para especificar las reglas a partir de los documentos de la colección. 
(b) Testear la regla de validación generando dos casos de fallas en la regla de validación y un caso de éxito en la regla de validación. Aclarar en la entrega cuales son los casos y por qué fallan y cuales cumplen la regla de validación. Los casos no deben ser triviales, es decir los ejemplos deben contener todos los campos..
 */
use("university");

db.runCommand({
  collMod: "grades",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["class_id", "scores", "student_id"],
      properties: {
        class_id: {
          bsonType: "int",
          minimum: 0,
          description: "debe ser un numero",
        },
        scores: {
          bsonType: "array",
          required: ["score", "type"],
          properties: {
            score: {
              bsonType: "int",
              minimum: 0,
              maximum: 100,
              description: "debe ser un numero",
            },
            type: {
              enum: ["quiz", "homework", "exam"],
            },
          },
        },
        student_id: {
          bsonType: "int",
          minimum: 0,
          description: "debe ser un numero",
        },
      },
    },
  },
  validationLevel: "strict",
  validationAction: "error",
});

use("university");
db.getCollectionInfos({ name: "grades" });

// Casos que falla
use("university");
db.grades.insertOne({
  class_id: 999,
  scores: [
    {
      score: 60,
      type: "quiz",
    },
    {
      score: 60,
      type: "exam",
    },
  ],
  student_id: "Sant",
});

use("university");
db.grades.insertOne({
  class_id: "ciencias",
  scores: [
    {
      score: 60,
      type: "quiz",
    },
    {
      score: 60,
      type: "exam",
    },
  ],
  student_id: 11,
});

// Caso que funciona
use("university");
db.grades.insertOne({
  class_id: 50,
  scores: [
    {
      score: 60,
      type: "quiz",
    },
    {
      score: 60,
      type: "exam",
    },
  ],
  student_id: 11,
});
