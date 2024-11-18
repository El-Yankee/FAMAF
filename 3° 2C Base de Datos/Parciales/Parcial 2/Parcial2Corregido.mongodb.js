// Ejercicio 1
/* Buscar los documentos donde el alumno tiene: 
    (i) un puntaje mayor o igual a 80  en "exam" o bien un puntaje mayor o igual a 90 en "quiz" y 
    (ii) un puntaje mayor o igual a 60 en todos los "homework" (en otras palabras no tiene un puntaje menor a 60 en algún "homework") 
Las dos condiciones se tienen que cumplir juntas (es un AND)
Se debe mostrar todos los campos excepto el _id, ordenados por el id de la clase y id del alumno en orden descendente y ascendente respectivamente.
 */
use("university");

db.grades
  .find(
    {
      $and: [
        // 1
        {
          $or: [
            {
              scores: {
                $elemMatch: {
                  type: "exam",
                  score: { $gte: 80 },
                },
              },
            },
            {
              scores: {
                $elemMatch: {
                  type: "quiz",
                  score: { $gte: 90 },
                },
              },
            },
          ],
        },
        // 2
        {
          // Negacion de que haya un elemento simultaneamente homework y <60
          scores: {
            $not: {
              $elemMatch: {
                type: "homework",
                score: { $lt: 60 },
              },
            },
          },
        },
      ],
    },
    {
      _id: 0,
      student_id: 1,
      scores: 1,
      class_id: 1,
    }
  )
  .sort({
    class_id: -1,
    student_id: 1,
  });

// Ejercicio 2
/* Calcular el puntaje mínimo, promedio, y máximo que obtuvo el alumno en las clases 20, 220, 420. El resultado debe mostrar además el id de la clase y el id del alumno, ordenados por alumno y clase en orden ascendentes.  */
use("university");

db.grades.aggregate([
  {
    $match: {
      class_id: { $in: [20, 220, 420] },
    },
  },
  {
    $unwind: "$scores",
  },
  {
    $group: {
      _id: {
        student_id: "$student_id",
        class_id: "$class_id",
      },
      min: { $min: "$scores.score" },
      avg: { $avg: "$scores.score" },
      max: { $max: "$scores.score" },
    },
  },
  {
    $sort: {
      "_id.student_id": 1,
      "_id.class_id": 1,
    },
  },
  {
    $project: {
      _id: 0,
      student_id: "$_id.student_id",
      class_id: "$_id.class_id",
      min: 1,
      avg: 1,
      max: 1,
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
      maxExam: {
        $max: {
          $cond: [{ $eq: ["$scores.type", "exam"] }, "$scores.score", 0],
        },
      },
      maxQuiz: {
        $max: {
          $cond: [{ $eq: ["$scores.type", "quiz"] }, "$scores.score", 0],
        },
      },
    },
  },
  {
    $sort: {
      _id: 1,
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
      avg: { $avg: "$scores.score" },
    },
  },
  {
    $sort: {
      avg: -1,
    },
  },
  {
    $limit: 10,
  },
  {
    $project: {
      _id: 0,
      student_id: "$_id",
      avg: 1,
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
        score_avg: {
          $avg: "$scores.score",
        },
      },
    },
    {
      $set: {
        letter: {
          $switch: {
            branches: [
              {
                case: { $gte: ["$score_avg", 80] },
                then: "P",
              },
              {
                case: { $gte: ["$score_avg", 60] },
                then: "A",
              },
            ],
            default: "NA",
          },
        },
      },
    },
  ]
);

use("university");
db.grades.find({ class_id: 339 });

// Ejericio 6
/* (a) Especificar reglas de validación en la colección grades para todos sus campos y subdocumentos anidados. Inferir los tipos y otras restricciones que considere adecuados para especificar las reglas a partir de los documentos de la colección. 
(b) Testear la regla de validación generando dos casos de fallas en la regla de validación y un caso de éxito en la regla de validación. Aclarar en la entrega cuales son los casos y por qué fallan y cuales cumplen la regla de validación. Los casos no deben ser triviales, es decir los ejemplos deben contener todos los campos..
 */
use("university");

db.runCommand({
  collMod: "grades",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["student_id", "class_id", "scores"],
      properties: {
        student_id: {
          bsonType: "int",
          minimum: 0,
        },
        class_id: {
          bsonType: "int",
          minimum: 0,
        },
        scores: {
          bsonType: "array",
          items: {
            bsonType: "object",
            required: ["type", "score"],
            properties: {
              type: {
                enum: ["exam", "quiz", "homework"],
              },
              score: {
                bsonType: "int",
                minimum: 0,
                maximum: 100,
              },
            },
          },
        },
      },
    },
  },
  validationAction: "error",
  validationLevel: "strict",
});

use("university");
db.getCollectionInfos({ name: "grades" });

// Casos que fallan
// Caso 1
use("university");
db.grades.insertOne({
  student_id: 1,
  class_id: 1,
  scores: [
    {
      type: "exam",
      score: 101,
    },
  ],
});

// Caso 2
use("university");
db.grades.insertOne({
  student_id: 1,
  class_id: 1,
  scores: [
    {
      type: "exam",
      score: 100,
    },
    {
      type: "quiz",
      score: 101,
    },
  ],
});

// Caso que cumple
use("university");
db.grades.insertOne({
  student_id: 1,
  class_id: 1,
  scores: [
    {
      type: "exam",
      score: 100,
    },
    {
      type: "quiz",
      score: 90,
    },
  ],
});
