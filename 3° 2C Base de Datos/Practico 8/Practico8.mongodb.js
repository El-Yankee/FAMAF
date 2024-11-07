// Ejericio 1
// Cantidad de cines (theaters) por estado.
use("mflix");

db.theaters.aggregate([
  // Agrupamos por estado
  {
    $group: {
      _id: "$location.address.state",
      count: { $sum: 1 },
    },
  },
  // Ordenamos por cantidad
  {
    $sort: {
      count: -1,
    },
  },
]);

// Ejericio 2
// Cantidad de estados con al menos dos cines (theaters) registrados.
use("mflix");

db.theaters.aggregate([
  // Agrupamos por estado
  {
    $group: {
      _id: "$location.address.state",
      count: { $sum: 1 },
    },
  },
  // Filtramos por cantidad mayor o igual a 2
  {
    $match: {
      count: { $gte: 2 },
    },
  },
  // Contamos los estados
  {
    $count: "totalStates",
  },
]);

// Ejericio 3
// Cantidad de películas dirigidas por "Louis Lumière". Se puede responder sin pipeline de agregación, realizar ambas queries.
use("mflix");

db.movies
  .find(
    // Filtro
    {
      directors: "Louis Lumière",
    }
  )
  .count();

db.movies.aggregate([
  // Filtramos por director
  {
    $match: {
      directors: "Louis Lumière",
    },
  },
  // Contamos las películas
  {
    $count: "totalMovies",
  },
]);

// Ejericio 4
// Cantidad de películas estrenadas en los años 50 (desde 1950 hasta 1959). Se puede responder sin pipeline de agregación, realizar ambas queries.
use("mflix");

db.movies
  .find(
    // Filtro
    {
      year: { $gte: 1950, $lte: 1959 },
    }
  )
  .count();

db.movies.aggregate([
  // Filtramos por año
  {
    $match: {
      year: { $gte: 1950, $lte: 1959 },
    },
  },
  // Contamos las películas
  {
    $count: "totalMovies",
  },
]);

// Ejericio 5
// Listar los 10 géneros con mayor cantidad de películas (tener en cuenta que las películas pueden tener más de un género). Devolver el género y la cantidad de películas. Hint: unwind puede ser de utilidad
use("mflix");

db.movies.aggregate([
  // Separamos los generos
  {
    $unwind: "$genres",
  },
  // Agrupamos por genero
  {
    $group: {
      _id: "$genres",
      count: { $sum: 1 },
    },
  },
  // Ordenamos por cantidad
  {
    $sort: {
      count: -1,
    },
  },
  // Limitamos a 10
  {
    $limit: 10,
  },
]);

// Ejericio 6
// Top 10 de usuarios con mayor cantidad de comentarios, mostrando Nombre, Email y Cantidad de Comentarios.
use("mflix");

db.comments.aggregate([
  // Agrupamos por nombre y email
  {
    $group: {
      _id: { name: "$name", email: "$email" },
      commentCount: { $sum: 1 },
    },
  },
  // Ordenamos por cantidad
  {
    $sort: { commentCount: -1 },
  },
  // Limitamos a 10
  {
    $limit: 10,
  },
  // Proyectamos los campos
  {
    $project: {
      _id: 0,
      name: "$_id.name",
      email: "$_id.email",
      commentCount: 1,
    },
  },
]);

// Ejericio 7
// Ratings de IMDB promedio, mínimo y máximo por año de las películas estrenadas en los años 80 (desde 1980 hasta 1989), ordenados de mayor a menor por promedio del año.
use("mflix");

db.movies.aggregate([
  // Filtramos por año
  {
    $match: {
      year: { $gte: 1980, $lte: 1989 },
      "imdb.rating": { $type: "double" },
    },
  },
  // Agrupamos por año
  {
    $group: {
      _id: "$year",
      avgRating: { $avg: "$imdb.rating" },
      minRating: { $min: "$imdb.rating" },
      maxRating: { $max: "$imdb.rating" },
    },
  },
  // Ordenamos por promedio
  {
    $sort: {
      avgRating: -1,
    },
  },
]);

// Ejericio 8
// Título, año y cantidad de comentarios de las 10 películas con más comentarios.
use("mflix");

db.movies.aggregate([
  {
    $lookup: {
      from: "comments", // Nombre de la colección de comentarios
      localField: "_id", // Campo en la colección movies que coincide con el campo de comentarios
      foreignField: "movie_id", // Campo en la colección comments que hace referencia a la película
      as: "movie_comments", // Alias para los comentarios de cada película
    },
  },
  {
    $addFields: {
      commentCount: { $size: "$movie_comments" }, // Agregar campo con la cantidad de comentarios
    },
  },
  {
    $sort: { commentCount: -1 }, // Ordenar por cantidad de comentarios en orden descendente
  },
  {
    $limit: 10, // Limitar a las 10 películas con más comentarios
  },
  {
    $project: {
      _id: 0, // Excluir el campo _id
      title: 1, // Incluir el título de la película
      year: 1, // Incluir el año de la película
      commentCount: 1, // Incluir la cantidad de comentarios
    },
  },
]);

// Ejericio 9
// Crear una vista con los 5 géneros con mayor cantidad de comentarios, junto con la cantidad de comentarios.
use("mflix");

db.createView(
  "Top5GenresByComments", // Nombre de la vista
  "movies", // Colección base
  [
    {
      $lookup: {
        from: "comments", // Colección de comentarios
        localField: "_id", // Campo _id de la colección movies
        foreignField: "movie_id", // Campo de referencia en comments
        as: "movie_comments", // Alias para comentarios de cada película
      },
    },
    {
      $unwind: "$genres", // Descomponer el array de géneros para trabajar con cada género individualmente
    },
    {
      $group: {
        _id: "$genres", // Agrupar por cada género
        commentCount: { $sum: { $size: "$movie_comments" } }, // Contar la cantidad de comentarios por género
      },
    },
    {
      $sort: { commentCount: -1 }, // Ordenar por cantidad de comentarios en orden descendente
    },
    {
      $limit: 5, // Limitar a los 5 géneros con mayor cantidad de comentarios
    },
    {
      $project: {
        _id: 0, // Excluir el campo _id
        genre: "$_id", // Incluir el género con un nombre descriptivo
        commentCount: 1, // Incluir la cantidad de comentarios
      },
    },
  ]
);

db.Top5GenresByComments.find();

// Ejericio 10
// Listar los actores (cast) que trabajaron en 2 o más películas dirigidas por "Jules Bass". Devolver el nombre de estos actores junto con la lista de películas (solo título y año) dirigidas por “Jules Bass” en las que trabajaron.
//     a. Hint1: addToSet
//     b. Hint2: {'name.2': {$exists: true}} permite filtrar arrays con al menos 2 elementos, entender por qué.
//     c. Hint3: Puede que tu solución no use Hint1 ni Hint2 e igualmente sea correcta
use("mflix");

db.movies.aggregate([
  {
    $match: {
      directors: "Jules Bass", // Filtrar por películas dirigidas por Jules Bass
    },
  },
  {
    $unwind: "$cast", // Descomponer el array de actores
  },
  {
    $group: {
      _id: "$cast", // Agrupar por cada actor
      movies: { $addToSet: { title: "$title", year: "$year" } }, // Agregar las películas en las que trabajó
    },
  },
  {
    $match: {
      "movies.1": { $exists: true }, // Filtrar actores que trabajaron en al menos 2 películas
    },
  },
  {
    $project: {
      _id: 0, // Excluir el campo _id
      actor: "$_id", // Incluir el nombre del actor
      movies: 1, // Incluir la lista de películas
    },
  },
]);

// Ejericio 11
// Listar los usuarios que realizaron comentarios durante el mismo mes de lanzamiento de la película comentada, mostrando Nombre, Email, fecha del comentario, título de la película, fecha de lanzamiento.
// HINT: usar $lookup con multiple condiciones
use("mflix");

db.comments.aggregate([
  {
    $lookup: {
      from: "movies",
      let: { movieId: "$movie_id", commentDate: "$date" },
      pipeline: [
        {
          $match: {
            $expr: {
              $and: [
                { $eq: ["$_id", "$$movieId"] }, // Coincidir la película comentada con el comentario
                { $eq: [{ $year: "$released" }, { $year: "$$commentDate" }] }, // Coincidir el año
                { $eq: [{ $month: "$released" }, { $month: "$$commentDate" }] }, // Coincidir el mes
              ],
            },
          },
        },
        { $project: { title: 1, released: 1 } }, // Incluir solo título y fecha de lanzamiento de la película
      ],
      as: "movieInfo",
    },
  },
  { $unwind: "$movieInfo" }, // Descomponer el array movieInfo, para tener un documento por coincidencia
  {
    $project: {
      _id: 0,
      Nombre: "$name",
      Email: "$email",
      "Fecha del Comentario": "$date",
      "Título de la Película": "$movieInfo.title",
      "Fecha de Lanzamiento": "$movieInfo.released",
    },
  },
]);

// Ejericio 12
// Listar el id y nombre de los restaurantes junto con su puntuación máxima, mínima y la suma total. Se puede asumir que el restaurant_id es único.
//     a. Resolver con $group y accumulators.
//     b. Resolver con expresiones sobre arreglos (por ejemplo, $sum) pero sin $group.
//     c. Resolver como en el punto b) pero usar $reduce para calcular la puntuación total.
//     d. Resolver con find.
use("restaurantdb");

db.restaurants.aggregate([
  // Descomponer el array grades para procesar cada puntuación
  {
    $unwind: "$grades",
  },
  // Agrupar por restaurant_id y name
  {
    $group: {
      _id: { id: "$restaurant_id", name: "$name" },
      maxScore: { $max: "$grades.score" },
      minScore: { $min: "$grades.score" },
      totalScore: { $sum: "$grades.score" },
    },
  },
  // Proyectar los campos solicitados
  {
    $project: {
      _id: 0,
      restaurant_id: "$_id.id",
      name: "$_id.name",
      maxScore: 1,
      minScore: 1,
      totalScore: 1,
    },
  },
]);

use("restaurantdb");

db.restaurants.aggregate([
  {
    $project: {
      restaurant_id: 1,
      name: 1,
      maxScore: { $max: "$grades.score" },
      minScore: { $min: "$grades.score" },
      totalScore: { $sum: "$grades.score" },
    },
  },
]);

use("restaurantdb");

db.restaurants.aggregate([
  {
    $project: {
      restaurant_id: 1,
      name: 1,
      maxScore: { $max: "$grades.score" },
      minScore: { $min: "$grades.score" },
      totalScore: {
        $reduce: {
          input: "$grades",
          initialValue: 0,
          in: { $add: ["$$value", "$$this.score"] },
        },
      },
    },
  },
]);

// Ejericio 13
// Actualizar los datos de los restaurantes añadiendo dos campos nuevos.
//     a. "average_score": con la puntuación promedio
//     b. "grade": con "A" si "average_score" está entre 0 y 13,
//                 con "B" si "average_score" está entre 14 y 27
//                 con "C" si "average_score" es mayor o igual a 28
// Se debe actualizar con una sola query.
//     a. HINT1. Se puede usar pipeline de agregación con la operación update
//     b. HINT2. El operador $switch o $cond pueden ser de ayuda.
use("restaurantdb");

db.restaurants.updateMany({}, [
  {
    $set: {
      average_score: { $avg: "$grades.score" }, // Calcula el promedio de "score"
      grade: {
        $switch: {
          branches: [
            {
              case: {
                $and: [
                  { $gte: ["$average_score", 0] },
                  { $lte: ["$average_score", 13] },
                ],
              },
              then: "A",
            },
            {
              case: {
                $and: [
                  { $gte: ["$average_score", 14] },
                  { $lte: ["$average_score", 27] },
                ],
              },
              then: "B",
            },
            { case: { $gte: ["$average_score", 28] }, then: "C" },
          ],
          default: "N/A", // Valor por defecto si no cumple ninguna condición
        },
      },
    },
  },
]);

db.restaurants.find();
