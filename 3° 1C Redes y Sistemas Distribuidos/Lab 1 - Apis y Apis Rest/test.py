import requests

# Obtener todas las películas
response = requests.get('http://localhost:5000/peliculas')
peliculas = response.json()
print("Películas existentes:")
for pelicula in peliculas:
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
print()

# Agregar una nueva película
nueva_pelicula = {
    'titulo': 'Pelicula de prueba',
    'genero': 'Acción'
}
response = requests.post('http://localhost:5000/peliculas', json=nueva_pelicula)
if response.status_code == 201:
    pelicula_agregada = response.json()
    print("Película agregada:")
    print(f"ID: {pelicula_agregada['id']}, Título: {pelicula_agregada['titulo']}, Género: {pelicula_agregada['genero']}")
else:
    print("Error al agregar la película.")
print()

# Obtener detalles de una película específica
id_pelicula = 1  # ID de la película a obtener
response = requests.get(f"http://localhost:5000/peliculas/{id_pelicula}")
if response.status_code == 200:
    pelicula = response.json()
    print("Detalles de la película:")
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error al obtener los detalles de la película.")
print()

# Actualizar los detalles de una película
id_pelicula = 1  # ID de la película a actualizar
datos_actualizados = {
    'titulo': 'Nuevo título',
    'genero': 'Comedia'
}
response = requests.put(f"http://localhost:5000/peliculas/{id_pelicula}", json=datos_actualizados)
if response.status_code == 200:
    pelicula_actualizada = response.json()
    print("Película actualizada:")
    print(f"ID: {pelicula_actualizada['id']}, Título: {pelicula_actualizada['titulo']}, Género: {pelicula_actualizada['genero']}")
else:
    print("Error al actualizar la película.")
print()

# Eliminar una película
id_pelicula = 1  # ID de la película a eliminar
response = requests.delete(f"http://localhost:5000/peliculas/{id_pelicula}")
if response.status_code == 200:
    print("Película eliminada correctamente.")
else:
    print("Error al eliminar la película.")
print()
  
# Obtener peliculas de un mismo genero
genero = 'Acción'
response = requests.get(f"http://localhost:5000/peliculas/cartelera/{genero}")
if response.status_code == 200:
    peliculas = response.json()
    print("Peliculas del mismo genero:")
    for pelicula in peliculas:
        print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error, no hay peliculas del genero buscado.")
print()
                
# Peliculas con palabras compartida
palabra = 'The'
response = requests.get(f"http://localhost:5000/peliculas/{palabra}")
if response.status_code == 200:
    list_peliculas = response.json()
    print("Peliculas con palabras compartidas:")
    for pelicula in list_peliculas:
        print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error, no hay peliculas que contengan esa palabra.")
print()

# Obtener pelicula random
response = requests.get(f"http://localhost:5000/peliculas/random")
if response.status_code == 200:
    pelicula = response.json()
    print("Pelicula random:")
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error al obtener la pelicula random.")
print()

# Obtener pelicula random por genero
genero = 'Acción'
response = requests.get(f"http://localhost:5000/peliculas/random/{genero}")
if response.status_code == 200:
    pelicula = response.json()
    print("Pelicula random segun el genero:")
    print(f"ID: {pelicula['id']}, Título: {pelicula['titulo']}, Género: {pelicula['genero']}")
else:
    print("Error, no hay peliculas del genero buscado.")
print()

# Sugerir pelicula para el proximo feriado
genero = 'Ciencia ficción'
response = requests.get(f"http://localhost:5000/peliculas/feriado/{genero}")
if response.status_code == 200:
    pelicula = response.json()
    print(f"La pelicula recomendada del género {pelicula['genero']} para el feriado {pelicula['motivo']} el {pelicula['dia']} del mes {pelicula['mes']} es {pelicula['pelicula']} ")
else:
    print("Error, no hay peliculas del genero buscado.")
print()
