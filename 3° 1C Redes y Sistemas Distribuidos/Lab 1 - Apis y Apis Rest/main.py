from flask import Flask, jsonify, request
import random
from proximo_feriado import NextHoliday

app = Flask(__name__)
peliculas = [
    {"id": 1, "titulo": "Indiana Jones", "genero": "Acción"},
    {"id": 2, "titulo": "Star Wars", "genero": "Acción"},
    {"id": 3, "titulo": "Interstellar", "genero": "Ciencia ficción"},
    {"id": 4, "titulo": "Jurassic Park", "genero": "Aventura"},
    {"id": 5, "titulo": "The Avengers", "genero": "Acción"},
    {"id": 6, "titulo": "Back to the Future", "genero": "Ciencia ficción"},
    {"id": 7, "titulo": "The Lord of the Rings", "genero": "Fantasía"},
    {"id": 8, "titulo": "The Dark Knight", "genero": "Acción"},
    {"id": 9, "titulo": "Inception", "genero": "Ciencia ficción"},
    {"id": 10, "titulo": "The Shawshank Redemption", "genero": "Drama"},
    {"id": 11, "titulo": "Pulp Fiction", "genero": "Crimen"},
    {"id": 12, "titulo": "Fight Club", "genero": "Drama"},
]


def obtener_peliculas():
    return jsonify(peliculas)


def obtener_pelicula(id):
    pelicula_encontrada = {404: "Movie not founded"}
    for pelicula in peliculas:
        if pelicula["id"] == id:
            pelicula_encontrada = pelicula
            break
    return jsonify(pelicula_encontrada)


def agregar_pelicula():
    nueva_pelicula = {
        "id": obtener_nuevo_id(),
        "titulo": request.json["titulo"],
        "genero": request.json["genero"],
    }
    peliculas.append(nueva_pelicula)
    print(peliculas)
    return jsonify(nueva_pelicula), 201


def actualizar_pelicula(id):
    pelicula = {404: "Movie not founded"}
    for pelicula in peliculas:
        if pelicula["id"] == id:
            pelicula["titulo"] = request.json["titulo"]
            pelicula["genero"] = request.json["genero"]
            break

    return jsonify(pelicula)


def eliminar_pelicula(id):
    pelicula_encontrada = {404: "Movie not founded"}
    for pelicula in peliculas:
        if pelicula["id"] == id:
            pelicula_encontrada = pelicula
            break

    if pelicula_encontrada == {404: "Movie not founded"}:
        return jsonify(pelicula_encontrada)
    else:
        peliculas.remove(pelicula_encontrada)
        return jsonify({"mensaje": "Película eliminada correctamente"})


def obtener_nuevo_id():
    if len(peliculas) > 0:
        ultimo_id = peliculas[-1]["id"]
        return ultimo_id + 1
    else:
        return 1


def cartelera_de_genero(gen):
    lista_gen = []

    for pelicula in peliculas:
        if pelicula["genero"] == gen:
            lista_gen.append(pelicula)

    if len(lista_gen) == 0:
        return jsonify({"mensaje": "No hay peliculas del genero buscado"})
    else:
        return jsonify(lista_gen)


def palabra_compartida(palabra):
    lista_pel = []

    for pelicula in peliculas:
        if palabra in pelicula["titulo"]:
            lista_pel.append(pelicula)

    if len(lista_pel) == 0:
        return jsonify({"mensaje": "No hay peliculas que contengan esa palabra"})
    else:
        return jsonify(lista_pel)


def pelicula_random():
    peli_random = random.choice(peliculas)
    return jsonify(peli_random)


def pelicula_random_genero(gen):
    lista_gen = []

    for pelicula in peliculas:
        if pelicula["genero"] == gen:
            lista_gen.append(pelicula)

    if len(lista_gen) == 0:
        return jsonify({"mensaje": "No hay peliculas del genero buscado"})
    else:
        peli_random = random.choice(lista_gen)
        return jsonify(peli_random)


def pelicula_feriado(gen, tipo_feriado=""):
    feriado = NextHoliday()

    feriado.fetch_holidays(tipo_feriado)

    lista_gen = []
    for pelicula in peliculas:
        if pelicula["genero"] == gen:
            lista_gen.append(pelicula)

    if len(lista_gen) == 0:
        return jsonify({"mensaje": "No hay peliculas del genero buscado"})
    else:
        peli_random = random.choice(lista_gen)

        peli_feriado = {
            "dia": feriado.holiday["dia"],
            "mes": feriado.holiday["mes"],
            "motivo": feriado.holiday["motivo"],
            "genero": gen,
            "pelicula": peli_random["titulo"],
        }
        return jsonify(peli_feriado)


app.add_url_rule("/peliculas", "obtener_peliculas", obtener_peliculas, methods=["GET"])
app.add_url_rule("/peliculas/<int:id>", "obtener_pelicula", obtener_pelicula, methods=["GET"])
app.add_url_rule("/peliculas", "agregar_pelicula", agregar_pelicula, methods=["POST"])
app.add_url_rule("/peliculas/<int:id>", "actualizar_pelicula", actualizar_pelicula, methods=["PUT"])
app.add_url_rule("/peliculas/<int:id>", "eliminar_pelicula", eliminar_pelicula, methods=["DELETE"])
app.add_url_rule("/peliculas/cartelera/<string:gen>", "obtener_cartelera_de_genero", cartelera_de_genero, methods=["GET"])
app.add_url_rule("/peliculas/<string:palabra>", "obtener_palabra_compartida", palabra_compartida, methods=["GET"])
app.add_url_rule("/peliculas/random", "obtener_pelicula_random", pelicula_random, methods=["GET"])
app.add_url_rule("/peliculas/random/<string:gen>", "obtener_pelicula_random_genero", pelicula_random_genero, methods=["GET"])
app.add_url_rule("/peliculas/feriado/<string:gen>", "recomendar_peli_para_el_feriado", pelicula_feriado, methods=["GET"],)
app.add_url_rule("/peliculas/feriado/<string:gen>/<string:tipo_feriado>", "recomendar_peli_para_el_feriado", pelicula_feriado, methods=["GET"])

if __name__ == "__main__":
    app.run()
