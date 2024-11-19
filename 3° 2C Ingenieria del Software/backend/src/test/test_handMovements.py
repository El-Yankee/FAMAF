import pytest
import httpx
from fastapi.testclient import TestClient
from app import app
from models.game_models import Base, engine, session, Game
from models.board_models import Table, Tile, TableGame, Figures
from models.player_models import Player, PlayerGame
from models.handMovements_models import HandMovements
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
    session.query(HandMovements).delete()
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
    session.query(HandMovements).delete()
    session.query(Game).delete()
    session.query(Player).delete()
    session.query(PlayerGame).delete()
    session.query(TableGame).delete()
    session.query(Table).delete()
    session.query(Tile).delete()
    session.commit()
    
# Test para crear un movimiento de mano
def test_create_hand_movement():
    gameid = "1111"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    assert movement.movementid == "1"
    assert movement.playerid == playerid
    assert movement.gameid == gameid

# Test para contar los movimientos por movimiento y juego
def test_count_movements_by_movementid():
    gameid = "0001"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    assert HandMovements.count_movements_by_movementid(movementid="1", gameid=gameid) == 1
    
# Test para verificar si el jugador no tiene un movimiento
def test_player_have_not_movement():
    gameid = "0001"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    assert HandMovements.player_have_not_movement(playerid=playerid, gameid=gameid, movementid="2") == True
    assert HandMovements.player_have_not_movement(playerid=playerid, gameid=gameid, movementid="1") == False

# Test para contar los movimientos por juego y jugador
def test_count_movements_charts_by_gameid_and_playerid():
    gameid = "0001"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    assert HandMovements.count_movements_charts_by_gameid_and_playerid(gameid=gameid, playerid=playerid) == 1
    
# Test para obtener los movimientos por jugador y juego
def test_get_movements_charts_by_player_id():
    gameid = "0001"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    assert HandMovements.get_movements_charts_by_player_id(playerid=playerid, gameid=gameid) == ["1"]
    
# Test para repartir movimientos al jugador de la partida
def test_deals_moves():
    gameid = "0001"
    playerid = "1234"
    HandMovements.deals_moves(playerid=playerid, gameid=gameid, quantity=1)
    assert HandMovements.count_movements_charts_by_gameid_and_playerid(gameid=gameid, playerid=playerid) == 1
    HandMovements.deals_moves(playerid=playerid, gameid=gameid, quantity=1)
    assert HandMovements.count_movements_charts_by_gameid_and_playerid(gameid=gameid, playerid=playerid) == 2
    
# Test para eliminar un movimiento de mano
def test_delete_hand_movements():
    gameid = "0001"
    playerid = "1234"
    movement = HandMovements.create_hand_movement(movementid="1", playerid=playerid, gameid=gameid)
    HandMovements.delete_hand_movements(playerid=playerid, gameid=gameid, movementid="1")
    assert HandMovements.count_movements_by_movementid(movementid="1", gameid=gameid) == 0
