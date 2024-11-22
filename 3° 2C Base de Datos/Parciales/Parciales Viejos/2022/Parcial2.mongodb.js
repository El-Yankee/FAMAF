// Ejercicio 1
/* Buscar los clientes que no tengan el campo active y que o bien posean más de 4 cuentas o bien nacieron entre Abril de 1995 y Marzo de 1997 inclusives. Listar el nombre, email, fecha de nacimiento y cantidad de cuentas. Limitar el resultado a los 50 primeros clientes de acuerdo al orden alfabético. */
use("analytics");

db.customers.aggregate([
  {
    $match: {
      active: { $exists: false },
      $or: [
        { "accounts.4": { $exists: true } }, // Más de 4 cuentas
        {
          birthDate: {
            $gte: ISODate("1995-04-01T00:00:00Z"),
            $lte: ISODate("1997-03-31T23:59:59Z"),
          },
        },
      ],
    },
  },
  {
    $project: {
      _id: 0,
      name: 1,
      email: 1,
      birthDate: 1,
      accountsCount: { $size: "$accounts" }, // Cantidad de cuentas
    },
  },
  {
    $sort: { name: 1 }, // Ordenar alfabéticamente por nombre
  },
  {
    $limit: 50, // Limitar a los primeros 50
  },
]);

// Ejercicio 2
/* Actualizar las cuentas que tengan un límite entre 8000 y 9000 inclusives, agregando un nuevo campo "class" con el valor "A" si la cuenta tiene hasta dos productos y con el valor "B" si tiene 3 o más productos. */
use("analytics");

db.accounts.updateMany(
  {
    limit: { $gte: 8000, $lte: 9000 }, // Límite entre 8000 y 9000 inclusive
  },
  [
    {
      $set: {
        class: {
          $cond: [
            { $lte: [{ $size: "$products" }, 2] }, // Si tiene hasta 2 productos
            "A", // Clase A
            "B", // Clase B
          ],
        },
      },
    },
  ]
);

use("analytics");
db.accounts.find({
  limit: { $gte: 8000, $lte: 9000 },
});

// Ejercicio 3
/* Buscar las transacciones donde la cantidad de transacciones sea mayor a 94. Listar id de transacción, id de la cuenta, y solo aquellas transacciones que tengan el código de transacción igual a "buy" y con "total" mayor a 500000. Mostrar el resultado ordenados por el id de la cuenta en orden decreciente. HINTS: (i) El operador $filter puede ser de utilidad. (ii) Notar que el valor del campo total está en string y requiere conversión. */

use("analytics");

db.transactions.aggregate([
  {
    $match: {
      "transactions.94": { $exists: true }, // Más de 94 transacciones
    },
  },
  {
    $project: {
      _id: 1, // ID de la transacción
      account_id: 1, // ID de la cuenta
      filtered_transactions: {
        $filter: {
          input: "$transactions",
          as: "transaction",
          cond: {
            $and: [
              { $eq: ["$$transaction.transaction_code", "buy"] }, // Código "buy"
              { $gt: [{ $toDouble: "$$transaction.total" }, 500000] }, // Total > 500000
            ],
          },
        },
      },
    },
  },
  {
    $match: {
      "filtered_transactions.0": { $exists: true }, // Solo transacciones con resultados
    },
  },
  {
    $sort: { account_id: -1 }, // Ordenar por account_id en orden decreciente
  },
]);

// Ejercicio 4
/* Crear la vista "transactionCountByCode" que lista el id de transacción, id de la cuenta, cantidad de transacciones, cantidad de transacciones de compra (transacciones con transaction_code igual a buy) y cantidad de transacciones de venta (transacciones con transaction_code igual a sell). Listar el resultado ordenados por cantidad de transacciones (orden decreciente). */
use("analytics");

db.createView("transactionCountByCode", "transactions", [
  {
    $project: {
      _id: 1, // ID de transacción
      account_id: 1, // ID de la cuenta
      total_transactions: { $size: "$transactions" }, // Cantidad total de transacciones
      buy_count: {
        $size: {
          $filter: {
            input: "$transactions",
            as: "transaction",
            cond: { $eq: ["$$transaction.transaction_code", "buy"] },
          },
        },
      }, // Cantidad de transacciones tipo "buy"
      sell_count: {
        $size: {
          $filter: {
            input: "$transactions",
            as: "transaction",
            cond: { $eq: ["$$transaction.transaction_code", "sell"] },
          },
        },
      }, // Cantidad de transacciones tipo "sell"
    },
  },
  {
    $sort: { total_transactions: -1 }, // Ordenar por cantidad total de transacciones, descendente
  },
]);

use("analytics");
db.transactionCountByCode.drop();
use("analytics");
db.transactionCountByCode.find();

// Ejercicio 5
/* Calcular la suma total, suma total de ventas y suma total de compras de las transacciones realizadas por año y mes. Mostrar el resultado en orden cronológico. No se debe mostrar resultados anidados en el resultado. HINT: El operador $cond o $switch puede ser de utilidad. */
use("analytics");

db.transactions.aggregate([
  {
    $unwind: "$transactions", // Descomponer las transacciones individuales
  },
  {
    $project: {
      year: { $year: "$transactions.date" }, // Extraer el año de la transacción
      month: { $month: "$transactions.date" }, // Extraer el mes de la transacción
      transaction_type: "$transactions.transaction_code", // Tipo de transacción
      total: { $toDouble: "$transactions.total" }, // Convertir total a número
    },
  },
  {
    $group: {
      _id: { year: "$year", month: "$month" }, // Agrupar por año y mes
      total_sum: { $sum: "$total" }, // Suma total de todas las transacciones
      total_sales: {
        $sum: {
          $cond: [{ $eq: ["$transaction_type", "sell"] }, "$total", 0],
        },
      }, // Suma total de ventas (sell)
      total_purchases: {
        $sum: {
          $cond: [{ $eq: ["$transaction_type", "buy"] }, "$total", 0],
        },
      }, // Suma total de compras (buy)
    },
  },
  {
    $project: {
      _id: 0,
      year: "$_id.year",
      month: "$_id.month",
      total_sum: 1,
      total_sales: 1,
      total_purchases: 1,
    },
  },
  {
    $sort: {
      year: 1,
      month: 1, // Ordenar en orden cronológico
    },
  },
]);

// Ejercicio 6
/* Especificar reglas de validación en la colección transactions (a) usando JSON Schema a los campos: account_id, transaction_count, bucket_start_date, bucket_end_date y transactions ( y todos sus campos anidados ). Inferir los tipos y otras restricciones que considere adecuados para especificar las reglas a partir de los documentos de la colección. (b) Luego añadir una regla de validación tal que bucket_start_date debe ser menor o igual a bucket_end_date. (c) Testear la regla de validación generando dos casos de falla en la regla de validación y dos casos donde cumple la regla de validación. Aclarar en la entrega cuales son los casos que fallan y cuales cumplen la regla de validación. Los casos no deben ser triviales. */
use("analytics");

db.runCommand({
  collMod: "transactions",
  validator: {
    $jsonSchema: {
      bsonType: "object",
      required: [
        "account_id",
        "transaction_count",
        "bucket_start_date",
        "bucket_end_date",
        "transactions",
      ],
      properties: {
        account_id: {
          bsonType: "int",
          minimum: 0,
        },
        transaction_count: {
          bsonType: "int",
          minimum: 0,
        },
        bucket_start_date: {
          bsonType: "date",
        },
        bucket_end_date: {
          bsonType: "date",
        },
        transactions: {
          bsonType: "array",
          items: {
            bsonType: "object",
            required: [
              "amount",
              "date",
              "price",
              "symbol",
              "total",
              "transaction_code",
            ],
            properties: {
              amount: {
                bsonType: "int",
                minimum: 0,
              },
              date: {
                bsonType: "date",
              },
              price: {
                bsonType: "string",
              },
              symbol: {
                bsonType: "string",
              },
              total: {
                bsonType: "string",
              },
              transaction_code: {
                bsonType: "string",
              },
            },
          },
        },
      },
    },
    $expr: {
      $lte: ["$bucket_start_date", "$bucket_end_date"],
    },
  },
  validationAction: "error",
  validationLevel: "strict",
});

use("analytics");
db.getCollectionInfos({ name: "transactions" });
