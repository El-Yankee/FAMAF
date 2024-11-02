// Ejericio 1
// Insertar 5 nuevos usuarios en la colección users. Para cada nuevo usuario creado, insertar al menos un comentario realizado por el usuario en la colección comments.
use("mflix");

db.users.insertMany([
  { name: "Santi", email: "santi@gmail.com", password: "1234" },
  { name: "Pepe", email: "pepe@gmail.com", password: "1234" },
  { name: "Juan", email: "juan@gmail.com", password: "1234" },
  { name: "Fede", email: "fede@gmail.com", password: "1234" },
  { name: "Mateo", email: "mateo@gmail.com", password: "1234" },
]);

db.comments.insertMany([
  {
    name: "Santi",
    email: "santi@gmail.com",
    movie_id: ObjectId("573a1390f29313caabcd4132"),
    text: "Muy buena película",
    date: new Date(2024 - 10 - 10),
  },
  {
    name: "Pepe",
    email: "pepe@gmail.com",
    movie_id: ObjectId("573a1390f29313caabcd4132"),
    text: "Interesante",
    date: new Date(2024 - 10 - 10),
  },
  {
    name: "Juan",
    email: "juan@gmail.com",
    movie_id: ObjectId("573a1390f29313caabcd4132"),
    text: "Me encantó",
    date: new Date(2024 - 10 - 10),
  },
  {
    name: "Fede",
    email: "fede@gmail.com",
    movie_id: ObjectId("573a1390f29313caabcd4132"),
    text: "No me gustó",
    date: new Date(2024 - 10 - 10),
  },
  {
    name: "Mateo",
    email: "mateo@gmail.com",
    movie_id: ObjectId("573a1390f29313caabcd4132"),
    text: "Muy buena",
    date: new Date(2024 - 10 - 10),
  },
]);

// Ejericio 2
// Listar el título, año, actores (cast), directores y rating de las 10 películas con mayor rating (“imdb.rating”) de la década del 90. ¿Cuál es el valor del rating de la película que tiene mayor rating? (Hint: Chequear que el valor de “imdb.rating” sea de tipo “double”).
use("mflix");

db.movies
  .find(
    // Filtro
    {
      year: { $gte: 1990, $lt: 2000 },
      "imdb.rating": { $type: "double" },
    },
    // Proyección
    {
      title: 1,
      year: 1,
      cast: 1,
      directors: 1,
      "imdb.rating": 1,
    }
  ) // Ordenamiento y límite
  .sort({ "imdb.rating": -1 })
  .limit(10)
  .pretty();

// Ejericio 3
// Listar el nombre, email, texto y fecha de los comentarios que la película con id (movie_id) ObjectId("573a1399f29313caabcee886") recibió entre los años 2014 y 2016 inclusive. Listar ordenados por fecha. Escribir una nueva consulta (modificando la anterior) para responder ¿Cuántos comentarios recibió?
use("mflix");

db.comments
  .find(
    // Filtro
    {
      movie_id: ObjectId("573a1399f29313caabcee886"),
      date: {
        $gte: new Date("2014-01-01"),
        $lt: new Date("2017-01-01"),
      },
    },
    // Proyección
    {
      name: 1,
      email: 1,
      text: 1,
      date: 1,
    }
  )
  .sort({ date: 1 })
  .pretty();

db.comments
  .find(
    // Filtro
    {
      movie_id: ObjectId("573a1399f29313caabcee886"),
      date: {
        $gte: new Date("2014-01-01"),
        $lt: new Date("2017-01-01"),
      },
    },
    // Proyección
    {
      name: 1,
      email: 1,
      text: 1,
      date: 1,
    }
  ) // Ordenamiento y conteo
  .sort({ date: 1 })
  .count();

// Ejericio 4
// Listar el nombre, id de la película, texto y fecha de los 3 comentarios más recientes realizados por el usuario con email patricia_good@fakegmail.com.
use("mflix");

db.comments
  .find(
    // Filtro
    {
      email: "patricia_good@fakegmail.com",
    },
    // Proyección
    {
      name: 1,
      movie_id: 1,
      text: 1,
      date: 1,
    }
  ) // Ordenamiento y límite
  .sort({ date: -1 })
  .limit(3)
  .pretty();

// Ejericio 5
// Listar el título, idiomas (languages), géneros, fecha de lanzamiento (released) y número de votos (“imdb.votes”) de las películas de géneros Drama y Action (la película puede tener otros géneros adicionales), que solo están disponibles en un único idioma y por último tengan un rating (“imdb.rating”) mayor a 9 o bien tengan una duración (runtime) de al menos 180 minutos. Listar ordenados por fecha de lanzamiento y número de votos.
use("mflix");

db.movies
  .find(
    // Filtro
    {
      genres: { $all: ["Drama", "Action"] },
      languages: { $size: 1 },
      $or: [{ "imdb.rating": { $gt: 9 } }, { runtime: { $gte: 180 } }],
    },
    // Proyección
    {
      _id: 0,
      title: 1,
      languages: 1,
      genres: 1,
      released: 1,
      "imdb.votes": 1,
    }
  )
  .sort({ released: 1, "imdb.votes": 1 })
  .pretty();

// Ejericio 6
// Listar el id del teatro (theaterId), estado (“location.address.state”), ciudad (“location.address.city”), y coordenadas (“location.geo.coordinates”) de los teatros que se encuentran en algunos de los estados "CA", "NY", "TX" y el nombre de la ciudades comienza con una ‘F’. Listar ordenados por estado y ciudad.
use("mflix");

db.theaters
  .find(
    //Filtro
    {
      "location.address.state": { $in: ["CA", "NY", "TX"] },
      "location.address.city": /^F/,
    },
    // Proyección
    {
      theaterId: 1,
      "location.address.state": 1,
      "location.address.city": 1,
      "location.geo.coordinates": 1,
    }
  )
  .sort({ "location.address.state": 1, "location.address.city": 1 })
  .pretty();

// Ejericio 7
// Actualizar los valores de los campos texto (text) y fecha (date) del comentario cuyo id es ObjectId("5b72236520a3277c015b3b73") a "mi mejor comentario" y fecha actual respectivamente.
use("mflix");

db.comments.updateOne(
  // Filtro
  {
    _id: ObjectId("5b72236520a3277c015b3b73"),
  },
  // Actualización
  {
    $set: {
      text: "mi mejor comentario",
      date: new Date(),
    },
  }
);

db.comments.find({ _id: ObjectId("5b72236520a3277c015b3b73") }).pretty();

// Ejericio 8
// Actualizar el valor de la contraseña del usuario cuyo email es joel.macdonel@fakegmail.com a "some password". La misma consulta debe poder insertar un nuevo usuario en caso que el usuario no exista. Ejecute la consulta dos veces. ¿Qué operación se realiza en cada caso?  (Hint: usar upserts).
use("mflix");

db.users.updateOne(
  // Filtro
  {
    email: "joel.macdonel@fakegmail.com",
  },
  // Actualización
  {
    $set: {
      password: "some password",
    },
    $setOnInsert: {
      name: "Joel Macdonel",
      email: "joel.macdonel@fakegmail.com",
    },
  },
  // Indica que se inserte un nuevo documento si no se encuentra ninguno que cumpla con el filtro
  { upsert: true }
);

db.users.find({ email: "joel.macdonel@fakegmail.com" }).pretty();

// Ejericio 9
// 9. Remover todos los comentarios realizados por el usuario cuyo email es victor_patel@fakegmail.com durante el año 1980.
use("mflix");

db.comments.deleteMany({
  email: "victor_patel@fakegmail.com",
  date: {
    $gte: new Date("1980-01-01"),
    $lt: new Date("1981-01-01"),
  },
});

// Ejericio 10
// Listar el id del restaurante (restaurant_id) y las calificaciones de los restaurantes donde al menos una de sus calificaciones haya sido realizada entre 2014 y 2015 inclusive, y que tenga una puntuación (score) mayor a 70 y menor o igual a 90.
use("restaurantdb");

db.restaurants.find(
  // Filtro
  {
    grades: {
      $elemMatch: {
        score: {
          $gt: 70,
          $lte: 90,
        },
        date: {
          $gte: new Date("2014-01-01"),
          $lt: new Date("2016-01-01"),
        },
      },
    },
  },
  // Proyección
  {
    _id: 0,
    restaurant_id: 1,
    "grades.score": 1,
  }
);

// Ejericio 11
// Agregar dos nuevas calificaciones al restaurante cuyo id es "50018608". A continuación se especifican las calificaciones a agregar en una sola consulta.
use("restaurantdb");

db.restaurants.updateOne(
  // Filtro
  {
    restaurant_id: "50018608",
  },
  // Actualización
  {
    $push: {
      grades: {
        $each: [
          {
            date: ISODate("2019-10-10T00:00:00Z"),
            grade: "A",
            score: 18,
          },
          {
            date: ISODate("2020-02-25T00:00:00Z"),
            grade: "A",
            score: 21,
          },
        ],
      },
    },
  }
);

db.restaurants.find({ restaurant_id: "50018608" }).pretty();
