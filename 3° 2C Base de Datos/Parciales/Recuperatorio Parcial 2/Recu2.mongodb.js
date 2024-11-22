// Ejercicio 1
use("university");
db.grades.aggregate([
  {
    $unwind: "$scores",
  },
  {
    $group: {
      _id: "$class_id",
      average_score: { $avg: "$scores.score" },
    },
  },
  {
    $sort: {
      _id: 1,
      average_score: -1,
    },
  },
  {
    $lookup: {
      from: "grades",
      pipeline: [
        {
          $unwind: "$scores",
        },
        {
          $group: {
            _id: null,
            average_score: { $avg: "$scores.score" },
          },
        },
        {
          $project: {
            _id: 0,
            average_score: 1,
          },
        },
      ],
      as: "overall_average",
    },
  },
  {
    $project: {
      _id: 0,
      class_id: "$_id",
      average_score: 1,
      overall_average: "$overall_average.average_score",
      comparison_to_overall_average: {
        $switch: {
          branches: [
            {
              case: {
                $gt: [
                  "$average_score",
                  { $first: "$overall_average.average_score" },
                ],
              },
              then: "above",
            },
            {
              case: {
                $lt: [
                  "$average_score",
                  { $first: "$overall_average.average_score" },
                ],
              },
              then: "below",
            },
          ],
          default: "equal",
        },
      },
    },
  },
]);

// Ejercicio 2
use("university");
db.grades.updateMany({}, [
  {
    $set: {
      scores: {
        $map: {
          input: "$scores",
          as: "grade",
          in: {
            type: "$$grade.type",
            score: { $multiply: [{ $divide: ["$$grade.score", 100] }, 7] },
          },
        },
      },
    },
  },
]);

use("university");
db.grades.find();

// Ejercicio 3
use("university");
db.createView("top10students_homework", "grades", [
  {
    $unwind: "$scores",
  },
  {
    $group: {
      _id: "$student_id",
      average_homework_score: {
        $avg: {
          $cond: [{ $eq: ["$scores.type", "homework"] }, "$scores.score", 0],
        },
      },
    },
  },
  {
    $sort: {
      average_homework_score: -1,
    },
  },
  {
    $limit: 10,
  },
  {
    $project: {
      _id: 0,
      student_id: "$_id",
      average_homework_score: 1,
    },
  },
]);

use("university");
db.top10students_homework.drop();
use("university");
db.top10students_homework.find();

// Ejercicio 4
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
