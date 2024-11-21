// Ejercicio 1
/* Buscar las ventas realizadas en "London", "Austin" o "San Diego"; a un customer con edad mayor-igual a 18 años que tengan productos que hayan salido al menos 1000 y estén etiquetados (tags) como de tipo "school" o "kids" (pueden tener más etiquetas).
Mostrar el id de la venta con el nombre "sale", la fecha (“saleDate"), el storeLocation, y el "email del cliente. No mostrar resultados anidados. */

use("supplies");

db.sales.aggregate([
  {
    $match: {
      storeLocation: { $in: ["London", "Austin", "San Diego"] },
      "customer.age": { $gte: 18 },
      items: {
        $elemMatch: {
          price: { $gte: 1000 },
          tags: { $in: ["school", "kids"] },
        },
      },
    },
  },
  {
    $project: {
      _id: 0,
      sale: "$_id",
      date: "$saleDate",
      store: "$storeLocation",
      email: "$customer.email",
    },
  },
]);

// Ejercicio 2
/* Buscar las ventas de las tiendas localizadas en Seattle, donde el método de compra sea ‘In store’ o ‘Phone’ y se hayan realizado entre 1 de febrero de 2014 y 31 de enero de 2015 (ambas fechas inclusive). Listar el email y la satisfacción del cliente, y el monto total facturado, donde el monto de cada item se calcula como 'price * quantity'. Mostrar el resultado ordenados por satisfacción (descendente), frente a empate de satisfacción ordenar por email (alfabético). */

use("supplies");

db.sales.aggregate([
  {
    $match: {
      storeLocation: "Seattle",
      purchaseMethod: { $in: ["In store", "Phone"] },
      saleDate: {
        $gte: new Date("2014-02-01"),
        $lte: new Date("2015-01-31"),
      },
    },
  },
  {
    $unwind: "$items",
  },
  {
    $group: {
      _id: "$_id", // Agrupa por ID de la venta
      email: { $first: "$customer.email" }, // Conserva el email
      satisfaction: { $first: "$customer.satisfaction" }, // Conserva la satisfacción
      totalAmount: {
        $sum: { $multiply: ["$items.price", "$items.quantity"] }, // Calcula el total
      },
    },
  },
  {
    $project: {
      _id: 0, // Excluye el ID de la venta
      email: 1, // Incluye el email
      satisfaction: 1, // Incluye la satisfacción
      totalAmount: 1, // Incluye el monto total
    },
  },
  {
    $sort: {
      satisfaction: -1, // Ordena por satisfacción (descendente)
      email: 1, // En caso de empate, ordena por email (ascendente)
    },
  },
]);

// Ejercicio 3
/* Crear la vista salesInvoiced que calcula el monto mínimo, monto máximo, monto total y monto promedio facturado por año y mes. Mostrar el resultado en orden cronológico. No se debe mostrar campos anidados en el resultado. */

use("supplies");

db.createView("salesInvoiced", "sales", [
  {
    $unwind: "$items",
  },
  {
    $group: {
      _id: "$_id",
      date: { $first: "$saleDate" },
      total_individual_sale: {
        $sum: { $multiply: ["$items.price", "$items.quantity"] },
      },
    },
  },
  {
    $group: {
      _id: {
        year: { $year: "$date" },
        month: { $month: "$date" },
      },
      min: { $min: "$total_individual_sale" },
      max: { $max: "$total_individual_sale" },
      total: { $sum: "$total_individual_sale" },
      avg: { $avg: "$total_individual_sale" },
    },
  },

  {
    $project: {
      _id: 0,
      year: "$_id.year",
      month: "$_id.month",
      min: 1,
      max: 1,
      total: 1,
      avg: 1,
    },
  },
  {
    $sort: {
      year: 1,
      month: 1,
    },
  },
]);

use("supplies");
db.salesInvoiced.drop();

use("supplies");
db.salesInvoiced.find();

// Ejercicio 4
/* Mostrar el storeLocation, la venta promedio de ese local, el objetivo a cumplir de ventas (dentro de la colección storeObjectives) y la diferencia entre el promedio y el objetivo de todos los locales.*/
use("supplies");

db.sales.aggregate([
  {
    $unwind: "$items",
  },
  // Calcular el total por documento
  {
    $group: {
      _id: "$_id",
      storeLocation: { $first: "$storeLocation" },
      totalSale: { $sum: { $multiply: ["$items.price", "$items.quantity"] } },
    },
  },
  // Agrupar por storeLocation para calcular el promedio
  {
    $group: {
      _id: "$storeLocation",
      average_sale: { $avg: "$totalSale" },
    },
  },
  {
    $lookup: {
      from: "storeObjectives",
      localField: "_id",
      foreignField: "_id",
      as: "storeObjectives",
    },
  },
  {
    // sin esto tengo una lista, con la cual no puedo restar abajo
    $unwind: "$storeObjectives",
  },
  {
    $project: {
      _id: 0,
      storeLocation: "$_id",
      average_sale: "$average_sale",
      objective: "$storeObjectives.objective",
      difference: {
        $subtract: ["$average_sale", "$storeObjectives.objective"],
      },
    },
  },
  {
    $sort: {
      storeLocation: 1,
    },
  },
]);

// Ejercicio 5
/* Especificar reglas de validación en la colección sales utilizando JSON Schema.
  a. Las reglas se deben aplicar sobre los campos: saleDate, storeLocation, purchaseMethod, y customer ( y todos sus campos anidados ). Inferir los tipos y otras restricciones que considere adecuados para especificar las reglas a partir de los documentos de la colección.
  b. Para testear las reglas de validación crear un caso de falla en la regla de validación y un caso de éxito (Indicar si es caso de falla o éxito) */
use("supplies");

db.runCommand({
  collMod: "sales",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: ["saleDate", "storeLocation", "purchaseMethod", "customer"],
      properties: {
        saleDate: {
          bsonType: "date",
          description: "must be a date",
        },
        storeLocation: {
          bsonType: "string",
          description: "must be a city",
        },
        purchaseMethod: {
          bsonType: "string",
          enum: ["Online", "In store", "Phone"],
          description: "Debe ser uno de los métodos de compra permitidos",
        },
        customer: {
          bsonType: "object",
          required: ["age", "email", "gender", "satisfaction"],
          properties: {
            age: {
              bsonType: "int",
              minimum: 0,
              description: "Debe ser un número entero mayor o igual a 0",
            },
            email: {
              bsonType: "string",
              pattern: "^[^@\\s]+@[^@\\s]+\\.[^@\\s]+$",
              description: "Debe ser un correo electrónico válido",
            },
            gender: {
              bsonType: "string",
              enum: ["M", "F"],
            },
            satisfaction: {
              bsonType: "int",
              minimum: 1,
              maximum: 5,
              description: "Debe ser un número entero entre 1 y 5",
            },
          },
        },
      },
    },
  },
  validationLevel: "strict",
  validationAction: "error",
});

use("supplies");

db.getCollectionInfos({ name: "sales" });

// TEST INSERT
// Pass
use("supplies");
db.sales.insertOne({
  saleDate: new Date(),
  customer: {
    age: 18,
    email: "example@example.com",
    gender: "M",
    satisfaction: 3,
  },
  storeLocation: "Denver",
  purchaseMethod: "Online",
});

// Fail
use("supplies");
db.sales.insertOne({
  saleDate: new Date(),
  customer: {
    age: 18,
    email: "example.com",
    gender: "M",
    satisfaction: 3,
  },
  storeLocation: "Denver",
  purchaseMethod: "Online",
});
