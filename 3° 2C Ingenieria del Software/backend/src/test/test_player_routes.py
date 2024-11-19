import pytest
from fastapi.testclient import TestClient
from app import app
from models.game_models import Base, Game, engine, session
from models.player_models import Player
from PlayerFactory import PlayerFactory


# Crea todas las tablas en la base de datos
Base.metadata.create_all(engine)

client = TestClient(app)

@pytest.fixture(scope='function', autouse=True)
def setup_database():
  # Limpiar la base de datos antes de cada prueba
  session.rollback()
  session.query(Game).delete()
  session.commit()
  yield
  # Limpiar después de cada prueba
  session.rollback()
  session.query(Game).delete()
  session.commit()
    
    
# Test de get players

def test_get_players():
  client.delete("/delete_all")
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  response = client.get("/players")
  assert response.status_code == 200
  assert response.json() == [{"player_name": player_name, "player_id": player_id}]
  
  
# Test de get player for id

def test_get_player_for_game_id():
  client.delete("/delete_all")
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  response = client.get(f"/player/{player_id}")
  assert response.status_code == 200
  assert response.json() == {"name": player_name, "playerid": player_id}

def test_get_player_for_game_id_not_found():
  client.delete("/delete_all")
  player_id = "3456735675672"

  response = client.get(f"/player/{player_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
  
# Test de get players in game

def test_get_players_in_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.get(f"/players_in_game/{game_id}")
  assert response.status_code == 200
  assert response.json() == [{"player_name": player_name, "player_id": player_id}]
  
def test_get_players_in_game_not_found():
  game_id = "12323523546"
  
  response = client.get(f"/players_in_game/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}

 
# Test de crear player

def test_create_player_factory():
    player = PlayerFactory()
    session.add(player)
    session.commit()
    
    retrives_player = session.query(Player).filter_by(playerid=player.playerid).first()
    
    assert retrives_player.name == player.name
    assert retrives_player.playerid == player.playerid
    
    
def test_create_player():
  player_name = "ValidPlayer"
  response = client.post(f"/create_player/{player_name}")

  assert response.status_code == 200
  assert response.json() == {"player_id": response.json()["player_id"]}

def test_create_player_invalid_name():
  player_name = "Santi Afonso!"  
  response = client.post(f"/create_player/{player_name}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Player name must be less than 20 character or alphanumeric"}

def test_create_player_name_too_long():
  player_name = "a" * 21
  response = client.post(f"/create_player/{player_name}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Player name must be less than 20 character or alphanumeric"}
  
  
# Test delete player

def test_delete_player():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  response = client.delete(f"/delete_player/{player_id}")
  assert response.status_code == 200
  assert response.json() == {"message": "Player deleted"}
  
def test_delete_player_not_found():
  player_id = "1234"
  
  response = client.delete(f"/delete_player/{player_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  response = client.delete("/delete_all")

