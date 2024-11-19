import pytest
import httpx
from fastapi.testclient import TestClient
from app import app
from models.game_models import Base, engine, session, Game
from models.board_models import Table, Tile, TableGame, Figures
from models.player_models import Player, PlayerGame
from models.partialMovements_models import PartialMovements

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
    session.query(PartialMovements).delete()
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
    session.query(PartialMovements).delete()
    session.query(Game).delete()
    session.query(Player).delete()
    session.query(PlayerGame).delete()
    session.query(TableGame).delete()
    session.query(Table).delete()
    session.query(Tile).delete()
    session.commit()
    
# Test para crear un movimiento parcial
def test_create_partial_movement():
    playerid = "1"
    gameid = "1"
    movementid = "1"
    tileid1 = "1"
    tileid2 = "2"
    partial_movement = PartialMovements.create_partial_movement(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
    assert partial_movement.playerid == playerid
    assert partial_movement.gameid == gameid
    assert partial_movement.movementid == movementid
    assert partial_movement.tileid1 == tileid1
    assert partial_movement.tileid2 == tileid2
    
# Test que cuenta la cantidad de movimientos parciales por gameid
def test_count_partial_movements_by_gameid():
    gameid = "1"
    playerid = "1"
    movementid = "1"
    tileid1 = "1"
    tileid2 = "2"
    partial_movement = PartialMovements.create_partial_movement(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
    assert PartialMovements.count_partial_movements_by_gameid(gameid) == 1
    
# Test que obtiene todos los movimientos parciales por gameid
def test_get_all_partial_movements_by_gameid():
    gameid = "1"
    playerid = "1"
    movementid = "1"
    tileid1 = "1"
    tileid2 = "2"
    partial_movement = PartialMovements.create_partial_movement(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
    assert PartialMovements.get_all_partial_movements_by_gameid(gameid) == [partial_movement]
    
# Test que obtiene el último movimiento parcial por gameid
def test_get_last_partial_movement():
    gameid = "1"
    playerid = "1"
    movementid = "1"
    tileid1 = "1"
    tileid2 = "2"
    partial_movement = PartialMovements.create_partial_movement(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
    assert PartialMovements.get_last_partial_movement(gameid) == partial_movement
    
# Test que elimina un movimiento parcial por partialid
def test_delete_partial_movement():
    gameid = "1"
    playerid = "1"
    movementid = "1"
    tileid1 = "1"
    tileid2 = "2"
    partial_movement = PartialMovements.create_partial_movement(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
    assert PartialMovements.get_last_partial_movement(gameid) == partial_movement
    PartialMovements.delete_partial_movement(partial_movement.partialid)
    assert PartialMovements.get_last_partial_movement(gameid) == None
