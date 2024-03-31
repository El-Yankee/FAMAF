import requests
import pytest
import requests_mock

@pytest.fixture
def mock_response():
    with requests_mock.Mocker() as m:
        # Simulamos la respuesta para obtener todas las películas
        m.get('http://localhost:5000/peliculas', json=[
            {'id': 1, 'titulo': 'Indiana Jones', 'genero': 'Acción'},
            {'id': 2, 'titulo': 'Star Wars', 'genero': 'Acción'}
        ])

        # Simulamos la respuesta para agregar una nueva película
        m.post('http://localhost:5000/peliculas', status_code=201, json={'id': 3, 'titulo': 'Pelicula de prueba', 'genero': 'Acción'})

        # Simulamos la respuesta para obtener detalles de una película específica
        m.get('http://localhost:5000/peliculas/1', json={'id': 1, 'titulo': 'Indiana Jones', 'genero': 'Acción'})

        # Simulamos la respuesta para actualizar los detalles de una película
        m.put('http://localhost:5000/peliculas/1', status_code=200, json={'id': 1, 'titulo': 'Nuevo título', 'genero': 'Comedia'})

        # Simulamos la respuesta para eliminar una película
        m.delete('http://localhost:5000/peliculas/1', status_code=200)

        # Simulamos la respuesta para obtener peliculas del mismo genero
        m.get('http://localhost:5000/peliculas/cartelera/Acción', json=[
            {"genero": "Acción","id": 1, "titulo": "Indiana Jones"  },
            {"genero": "Acción","id": 2,"titulo": "Star Wars"},           
        ])

        # Simulamos la respuesta para obtener peliculas con palabra compartida
        m.get('http://localhost:5000/peliculas/The', json=[
            {"genero": "Acción","id": 5, "titulo": "The Avengers"  },
            {"genero": "Fantasía","id": 7,"titulo": "The Lord of the Rings"},           
        ])

        # Simulamos la respuesta para obtener peliculas random
        m.get('http://localhost:5000/peliculas/random', json=
            { "genero": "Crimen", "id": 11,"titulo": "Pulp Fiction"},           
        )

        # Simulamos la respuesta para obtener peliculas random por genero
        m.get('http://localhost:5000/peliculas/random/Acción', json=
            {"genero": "Acción","id": 5,"titulo": "The Avengers"},           
        )

        # Simulamos la respuesta para obtener peli para el feriado
        m.get('http://localhost:5000/peliculas/feriado/Acción', json=
            {"dia": 24,"genero": "Acción","mes": 3,"motivo": "Día Nacional de la Memoria por la Verdad y la Justicia","pelicula": "The Avengers"},           
        )
  
        yield m

def test_obtener_peliculas(mock_response):
    response = requests.get('http://localhost:5000/peliculas')
    assert response.status_code == 200
    assert len(response.json()) == 2

def test_agregar_pelicula(mock_response):
    nueva_pelicula = {'titulo': 'Pelicula de prueba', 'genero': 'Acción'}
    response = requests.post('http://localhost:5000/peliculas', json=nueva_pelicula)
    assert response.status_code == 201
    assert response.json()['id'] == 3

def test_obtener_detalle_pelicula(mock_response):
    response = requests.get('http://localhost:5000/peliculas/1')
    assert response.status_code == 200
    assert response.json()['titulo'] == 'Indiana Jones'

def test_actualizar_detalle_pelicula(mock_response):
    datos_actualizados = {'titulo': 'Nuevo título', 'genero': 'Comedia'}
    response = requests.put('http://localhost:5000/peliculas/1', json=datos_actualizados)
    assert response.status_code == 200
    assert response.json()['titulo'] == 'Nuevo título'

def test_eliminar_pelicula(mock_response):
    response = requests.delete('http://localhost:5000/peliculas/1')
    assert response.status_code == 200

def test_peliculas_del_mismo_genero(mock_response):
    response = requests.get('http://localhost:5000/peliculas/cartelera/Acción')
    assert response.status_code == 200
    assert len(response.json()) == 2

def test_peliculas_con_palabras_compartidas(mock_response):
    response = requests.get('http://localhost:5000/peliculas/The')
    assert response.status_code == 200
    assert len(response.json()) == 2

def test_pelicula_random(mock_response):
    response = requests.get('http://localhost:5000/peliculas/random')
    assert response.status_code == 200

def test_pelicula_random_genero(mock_response):
    response = requests.get('http://localhost:5000/peliculas/random/Acción')
    assert response.status_code == 200
    assert response.json()['genero'] == 'Acción'

def test_pelicula_feriado(mock_response):
    response = requests.get('http://localhost:5000/peliculas/feriado/Acción')
    assert response.status_code == 200
    assert response.json()['genero'] == 'Acción'