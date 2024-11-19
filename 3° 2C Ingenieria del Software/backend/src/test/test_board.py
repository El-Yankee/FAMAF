import sys
import os
import random

# Añadir el directorio raíz del proyecto al path de Python
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import pytest
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from models.game_models import Base, session
from models.board_models import Table, Tile, Figures
from app import find_connected_components, match_figures 

# Configura la base de datos para las pruebas
TEST_DATABASE_URL = "sqlite:///test/database/test.db"  # Base de datos en archivo para pruebas
engine = create_engine(TEST_DATABASE_URL)
SessionLocal = sessionmaker(autocommit=False, autoflush=False, bind=engine)

# Crea todas las tablas
Base.metadata.create_all(bind=engine)

@pytest.fixture(scope="function")
def db_session():
    session = SessionLocal()
    yield session
    session.rollback()  # Revertir cualquier cambio hecho durante la prueba
    session.close()
    Base.metadata.drop_all(bind=engine)  # Eliminar todas las tablas
    Base.metadata.create_all(bind=engine)  # Crear todas las tablas de nuevo

def test_create_table(db_session):
    # Crear una nueva tabla
    new_table = Table(gameid="game_1")
    db_session.add(new_table)
    db_session.commit()

    # Verificar que la tabla se ha creado correctamente
    table = db_session.query(Table).filter_by(gameid="game_1").first()
    assert table is not None
    assert table.gameid == "game_1"

def test_create_tile(db_session):
    # Crear una nueva tabla para asociar el tile
    new_table = Table(gameid="game_2")
    db_session.add(new_table)
    db_session.commit()

    # Crear un nuevo tile
    new_tile = Tile(x=0, y=0, color="red", table_id=new_table.id, highlight=False, number=1)
    db_session.add(new_tile)
    db_session.commit()

    # Verificar que el tile se ha creado correctamente
    tile = db_session.query(Tile).filter_by(x=0, y=0, color="red").first()
    assert tile is not None
    assert tile.x == 0
    assert tile.y == 0
    assert tile.color == "red"
    assert tile.table_id == new_table.id
    assert tile.number == 1

def test_create_figure(db_session):
    # Crear una nueva figura
    new_figure = Figures(points="00,01,02,03")
    db_session.add(new_figure)
    db_session.commit()

    # Verificar que la figura se ha creado correctamente
    figure = db_session.query(Figures).filter_by(points="00,01,02,03").first()
    assert figure is not None
    assert figure.points == "00,01,02,03"

def test_find_connected_components(db_session):
    # Crear una nueva tabla para asociar los tiles
    new_table = Table(gameid="game_3")
    db_session.add(new_table)
    db_session.commit()

    # Crear tiles conectados
    tiles = [
        Tile(x=0, y=0, color="red", table_id=new_table.id, highlight=False, number=1),
        Tile(x=0, y=1, color="red", table_id=new_table.id, highlight=False, number=2),
        Tile(x=1, y=0, color="blue", table_id=new_table.id, highlight=False, number=3),
        Tile(x=1, y=1, color="red", table_id=new_table.id, highlight=False, number=4)
    ]
    db_session.add_all(tiles)
    db_session.commit()

    # Obtener los tiles de la base de datos
    tiles_from_db = db_session.query(Tile).filter_by(table_id=new_table.id).all()
    assert len(tiles_from_db) == 4

# este test modifica la base de datos original, por ende se puede ejecutar, y da correcto el resultado 
# def test_create_tiles_for_table(db_session):
#     # Crear una nueva tabla
#     new_table = Table(gameid="game_5")
#     session.add(new_table)
#     session.commit()

#     # Verificar que la tabla se ha creado correctamente
#     assert new_table.id is not None, "La tabla no se ha creado correctamente"

#     # Llamar al método create_tiles_for_table
#     Tile.create_tiles_for_table(new_table.id)

#     # Verificar que se han creado 36 tiles
#     tiles = session.query(Tile).filter_by(table_id=new_table.id).all()
#     assert len(tiles) == 36, f"Se esperaban 36 tiles, pero se encontraron {len(tiles)}"

#     # Verificar que cada tile tiene un color válido
#     valid_colors = ['red', 'green', 'yellow', 'blue']
#     for tile in tiles:
#         assert tile.color in valid_colors, f"Tile con color inválido: {tile.color}"

    

# def test_swap_tiles_color(db_session):
#     # Borrar todos los tiles existentes
#     db_session.query(Tile).delete()
#     db_session.commit()

#     # Crear una nueva tabla
#     new_table = Table(gameid="game_6")
#     db_session.add(new_table)
#     db_session.commit()

#     # Crear dos tiles con colores diferentes
#     tile1 = Tile(x=0, y=0, color="red", table_id=new_table.id, highlight=False, number=1)
#     tile2 = Tile(x=0, y=1, color="blue", table_id=new_table.id, highlight=False, number=2)
#     db_session.add_all([tile1, tile2])
#     db_session.commit()

#     # Llamar al método swap_tiles_color
#     tile1.swap_tiles_color(tile2.id)

#     # Verificar que los colores de los tiles se han intercambiado
#     tile1 = db_session.query(Tile).filter_by(number=1, table_id=new_table.id).first()
#     tile2 = db_session.query(Tile).filter_by(number=2, table_id=new_table.id).first()
#     assert tile1.color == "blue"
#     assert tile2.color == "red"

# def test_create_figures(db_session):
#     # Llamar al método create_figures
#     Figures.create_figures()

#     # Verificar que se han creado 25 figuras
#     figures = db_session.query(Figures).all()
#     assert len(figures) == 25

#     # Verificar algunas figuras específicas
#     figure = db_session.query(Figures).filter_by(points="01,02,01,11").first()
#     assert figure is not None
#     assert figure.points == "01,02,01,11"
