import pytest
import httpx
from fastapi.testclient import TestClient
from app import app
from models.game_models import Base, engine, session, Game
from models.board_models import Table, Tile, TableGame, Figures
from models.player_models import Player, PlayerGame
from models.movementChart_models import MovementChart

from httpx import WSGITransport

# Crea todas las tablas en la base de datos
Base.metadata.create_all(engine)

# Ejemplo de uso actualizado
transport = WSGITransport(app=app)
client = httpx.Client(transport=transport)

@pytest.fixture(scope='function', autouse=True)
def setup_database():
    # Limpiar la base de datos antes de cada prueba
    session.rollback()
    session.query(MovementChart).delete()
    session.query(Game).delete()
    session.query(Player).delete()
    session.query(PlayerGame).delete()
    session.query(TableGame).delete()
    session.query(Table).delete()
    session.query(Tile).delete() 
    session.commit()
    yield
    # Limpiar después de cada prueba
    session.rollback()
    session.query(MovementChart).delete()
    session.query(Game).delete()
    session.query(Player).delete()
    session.query(PlayerGame).delete()
    session.query(TableGame).delete()
    session.query(Table).delete()
    session.query(Tile).delete()
    session.commit()
    
# Test para crear una carta de movimiento
def test_create_movement_chart():
    movementid = "1"
    rot0 = "0,0"
    rot90 = "1,1"
    rot180 = "2,2"
    rot270 = "3,3"
    movement = MovementChart.create_movement_chart(movementid=movementid, rot0=rot0, rot90=rot90, rot180=rot180, rot270=rot270)
    assert movement.movementid == movementid
    assert movement.rot0 == rot0
    assert movement.rot90 == rot90
    assert movement.rot180 == rot180
    assert movement.rot270 == rot270
    
# Test para obtener una carta de movimiento por id
def test_get_movement_chart_by_id():
    movementid = "1"
    rot0 = "0,0"
    rot90 = "1,1"
    rot180 = "2,2"
    rot270 = "3,3"
    movement = MovementChart.create_movement_chart(movementid=movementid, rot0=rot0, rot90=rot90, rot180=rot180, rot270=rot270)
    assert MovementChart.get_movement_chart_by_id(movementid=movementid) == movement
    
# Test para verificar si la tabla está vacía
def test_is_table_empty():
    assert MovementChart.is_table_empty() == True
    movementid = "1"
    rot0 = "0,0"
    rot90 = "1,1"
    rot180 = "2,2"
    rot270 = "3,3"
    movement = MovementChart.create_movement_chart(movementid=movementid, rot0=rot0, rot90=rot90, rot180=rot180, rot270=rot270)
    assert MovementChart.is_table_empty() == False
    
# Test para crear movimientos de juego
def test_game_movement():
    MovementChart.game_movement()
    assert MovementChart.get_movement_chart_by_id(movementid="1").movementid == "1"
    assert MovementChart.get_movement_chart_by_id(movementid="2").movementid == "2"
    assert MovementChart.get_movement_chart_by_id(movementid="3").movementid == "3"
    assert MovementChart.get_movement_chart_by_id(movementid="4").movementid == "4"
    assert MovementChart.get_movement_chart_by_id(movementid="5").movementid == "5"
    assert MovementChart.get_movement_chart_by_id(movementid="6").movementid == "6"
    assert MovementChart.get_movement_chart_by_id(movementid="7").movementid == "7" 
    