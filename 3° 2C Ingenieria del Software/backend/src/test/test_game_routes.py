import pytest
from fastapi.testclient import TestClient
from app import app
from models.game_models import Base, Game, engine, session
from models.player_models import PlayerGame
from models.figure_card_models import Figure_card
from models.handMovements_models import HandMovements
from models.movementChart_models import MovementChart
from GameFactory import GameFactory

# Crea todas las tablas
Base.metadata.drop_all(engine)
Base.metadata.create_all(engine)
MovementChart.game_movement()


client = TestClient(app)

@pytest.fixture(scope='function', autouse=True)
def setup_database():
  # Limpiar la base de datos antes de cada prueba
  session.rollback()
  session.query(PlayerGame).delete()
  session.query(HandMovements).delete()
  session.query(Game).delete()
  session.commit()
  yield
  # Limpiar después de cada prueba
  session.rollback()
  session.query(PlayerGame).delete()
  session.query(HandMovements).delete()
  session.query(Game).delete()
  session.commit()
    

# Test de get games
def test_get_games():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  turn = None
  game_state = "waiting"
  current_player = 1
  
  response = client.get("/games")
  assert response.status_code == 200
  assert response.json() == [{"game_name": game_name, "game_id": game_id, "host_id": player_id, "state": game_state, "size": game_size, "current_player": current_player, "turn": turn}]
  
  
# Test de get game for id
def test_get_game_for_id():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  game_state = "waiting"
  current_player = 1
  
  response = client.get(f"/game/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"game_name": game_name, "game_id": game_id, "state": game_state, "game_size": game_size, "players": current_player ,"player_details": [{"player_id": player_id, "player_name": player_name}], "host_id": player_id, "turn": None}
  
def test_get_game_for_id_not_found():
  game_id = "1234"
  
  response = client.get(f"/game/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}


# Test de crear game
def test_create_game_factory():
  # Usar la fábrica para crear una instancia de Game
  game = GameFactory()

  # Guardar el juego en la base de datos
  session.add(game)
  session.commit()

  # Recuperar el juego para verificar que se ha creado correctamente
  retrieved_game = session.query(Game).filter_by(gameid=game.gameid).first()

  assert retrieved_game is not None
  assert retrieved_game.name.startswith("Game")
  assert retrieved_game.state == "waiting"
  assert retrieved_game.size == 3
  assert retrieved_game.host is not None
    
def test_create_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json().get("player_id")
  assert player_id is not None, "Player ID not found in the response"

  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response.json()["game_id"]

  assert response.status_code == 200
  assert response.json() == {"game_id": game_id}

def test_create_game_invalid_name():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "In val!d G@me"
  game_size = 3
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Game name must be less than 20 characters or alphanumeric"}

def test_create_game_name_too_long():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "a" * 21
  game_size = 3
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Game name must be less than 20 characters or alphanumeric"}
  
def test_create_game_invalid_size():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 1
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Game size must be between 2 and 4"}
  
def test_create_game_size_too_big():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 10
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  
  assert response.status_code == 400
  assert response.json() == {"detail": "Game size must be between 2 and 4"}
  
def test_create_game_player_not_found():
  player_id = "1234"
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")

  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
def test_create_game_password_too_long():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 3
  game_password = "a" * 21
  response = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")

  assert response.status_code == 400
  assert response.json() == {"detail": "Game password must be between 4 and 20 characters and alphanumeric"}

# Test join game
def test_join_game():
    player_name = "ValidPlayer"
    response_player = client.post(f"/create_player/{player_name}")
    player_id = response_player.json()["player_id"]
    
    player_name2 = "ValidPlayer2"
    response_player2 = client.post(f"/create_player/{player_name2}")
    player_id2 = response_player2.json()["player_id"]
    
    game_name = "ValidGame"
    game_size = 3
    game_password = "1234"
    response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
    game_id = response_game.json()["game_id"]
    
    response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
    assert response.status_code == 200
    assert response.json() == {"message": player_name2 + " joined the game " + game_name}

def test_join_game_not_found():
    player_name = "ValidPlayer"
    response_player = client.post(f"/create_player/{player_name}")
    player_id = response_player.json()["player_id"]
    game_id = "1234"
    game_password = "1234"
    
    response = client.put(f"/join_game/{player_id}/{game_id}/{game_password}")
    assert response.status_code == 404
    assert response.json() == {"detail": "Game not found"}

def test_join_game_player_not_found():
    player_name = "ValidPlayer"
    player_id2 = "1234"
    response_player = client.post(f"/create_player/{player_name}")
    player_id = response_player.json()["player_id"]
    game_name = "ValidGame"
    game_size = 3
    game_password = "1234"
    response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
    game_id = response_game.json()["game_id"]
    
    response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
    assert response.status_code == 404
    assert response.json() == {"detail": "Player not found"}

def test_join_game_game_is_already_playing():
    player_name = "ValidPlayer"
    response_player = client.post(f"/create_player/{player_name}")
    player_id = response_player.json()["player_id"]
    
    player_name2 = "ValidPlayer2"
    response_player2 = client.post(f"/create_player/{player_name2}")
    player_id2 = response_player2.json()["player_id"]
    
    player_name3 = "ValidPlayer3"
    response_player3 = client.post(f"/create_player/{player_name3}")
    player_id3 = response_player3.json()["player_id"]
    
    game_name = "ValidGame"
    game_size = 2
    game_password = "1234"
    response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
    game_id = response_game.json()["game_id"]
    
    response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
    assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
    
    response_start = client.put(f"/start_game/{player_id}/{game_id}")
    assert response_start.json() == {"message": "Game started"}
    
    response_leave = client.put(f"/leave_game/{player_id2}/{game_id}")
    assert response_leave.json() == {"message": player_name2 + " left the game " + game_name}
    
    response = client.put(f"/join_game/{player_id3}/{game_id}/{game_password}")
    assert response.status_code == 409
    assert response.json() == {"detail": "Game is already playing"}
  
def test_join_game_player_is_in_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  
  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Player is already in the game"}

def test_join_game_is_full():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  
  response = client.put(f"/join_game/{player_id3}/{game_id}/{game_password}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Game is full"}
  
def test_join_game_password_not_match():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/join_game/{player_id2}/{game_id}/12345")
  assert response.status_code == 409
  assert response.json() == {"detail": "Incorrect password"}

# Test de shuffle
def test_shuffle():
  player_name = "ValidPlayer1"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  response_join2 = client.put(f"/join_game/{player_id3}/{game_id}/{game_password}")
  assert response_join2.json() == {"message": player_name3 + " joined the game " + game_name}


  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"message": "Game started"}
  assert session.query(PlayerGame).filter_by(gameid=game_id).count() == 3
  assert session.query(Figure_card).filter_by(playerid=player_id).count() == 16 
  assert session.query(Figure_card).filter_by(playerid=player_id3).count() == 16
  assert session.query(Figure_card).filter_by(playerid=player_id2).count() == 16
  assert session.query(Figure_card).filter_by(gameid=game_id).count() == 48
  assert session.query(Figure_card).filter_by(gameid=game_id, in_hand=True).count() == 9


# Test leave game
def test_leave_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
  
  response = client.put(f"/leave_game/{player_id2}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"message": player_name2 + " left the game " + game_name}

def test_leave_game_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_id = "1234"
  
  response = client.put(f"/leave_game/{player_id}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}
  
def test_leave_game_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_id2 = "1234"
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/leave_game/{player_id2}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
def test_leave_game_host_cancelled_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/leave_game/{player_id}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {'message': 'Cancelled game'}

def test_leave_game_player_is_not_in_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/leave_game/{player_id2}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Player is not in the game"}
  
  
# Test start game
def test_start_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
  
  response_join2 = client.put(f"/join_game/{player_id3}/{game_id}/{game_password}")
  assert response_join2.json() == {"message": player_name3 + " joined the game " + game_name}
  
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"message": "Game started"}
  
def test_start_game_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_id = "1234"
  
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}
  
def test_start_game_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  player_id3 = "123456asdf"
  
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/start_game/{player_id3}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
def test_start_game_player_not_in_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
  
  response = client.put(f"/start_game/{player_id3}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Player is not in the game"}
  
def test_start_game_already_playing():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response_join = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name2 + " joined the game " + game_name}
  
  response_join2 = client.put(f"/join_game/{player_id3}/{game_id}/{game_password}")
  assert response_join2.json() == {"message": player_name3 + " joined the game " + game_name}
  
  response_start = client.put(f"/start_game/{player_id}/{game_id}")
  assert response_start.status_code == 200
  assert response_start.json() == {"message": "Game started"}
  
  response_start_again = client.put(f"/start_game/{player_id}/{game_id}")
  assert response_start_again.status_code == 409
  assert response_start_again.json() == {"detail": "Game is already playing"}
    
def test_start_game_not_host():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id2}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response_join = client.put(f"/join_game/{player_id}/{game_id}/{game_password}")
  assert response_join.json() == {"message": player_name + " joined the game " + game_name}
  
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Only the host can start the game"}
  
def test_start_game_is_not_full():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 3
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "The game is not full"}
  
  
# Test para next turn 
def test_next_turn_game_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_id = "1234"
  
  response = client.put(f"/next_turn/{player_id}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}

def test_next_turn_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  player_id2 = "1234"
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/next_turn/{player_id2}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}

def test_next_turn_not_your_turn():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  
  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]
  
  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]
  
  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  client.put(f"/start_game/{player_id}/{game_id}")
  
  response = client.put(f"/next_turn/{player_id3}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "It's not your turn"}
  
# Test para swap tiles
def test_swap_tiles():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]
  
  movement_id = "3"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  tile_id1 = 1
  tile_id2 = 2

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 200
  assert response.json() == {"message": "Tiles swapped"}
  
def test_swap_tiles_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]

  player_id3 = "1234"
  movement_id = "10"
  tile_id1 = 1
  tile_id2 = 2

  response = client.put(f"/swap_tiles/{player_id3}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
def test_swap_tiles_player_is_not_in_game():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]

  player_name3 = "ValidPlayer3"
  response_player3 = client.post(f"/create_player/{player_name3}")
  player_id3 = response_player3.json()["player_id"]

  movement_id = "10"
  tile_id1 = 1
  tile_id2 = 2

  response = client.put(f"/swap_tiles/{player_id3}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Player is not in the game"}
 
def test_swap_tiles_tile_1_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]

  movement_id = "10"
  tile_id1 = 100
  tile_id2 = 2

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Tile 1 not found"}
  
def test_swap_tiles_tile_2_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]

  movement_id = "10"
  tile_id2 = 100
  tile_id1 = 2

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Tile 2 not found"}
  
def test_swap_tiles_movement_has_not_this_movement():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]

  tile_id = 15
  tile_id2 = 16
  movement_id = "10"
  movement_id2 = "3"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id2}/{tile_id}/{tile_id2}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Player has not this movement"}

def test_swap_tiles_not_your_turn():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[1]
  
  movement_id = "1"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  tile_id1 = 1
  tile_id2 = 2

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 409
  assert response.json() == {"detail": "It's not your turn"}
  
def test_swap_tiles_invalid_movement():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]
  
  movement_id = "1"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  tile_id1 = 1
  tile_id2 = 36

  response = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Invalid movement"}
  
  
# Test para undo a movement

def test_undo_a_movement():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]
  
  movement_id = "3"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  tile_id1 = 1
  tile_id2 = 2

  response_swap = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response_swap.json() == {"message": "Tiles swapped"}
  
  response = client.put(f"/undo_a_movement/{first_turn}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"message": "Movement undone"}

def test_undo_a_movement_game_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]
  game_id = "1234"
  
  response = client.put(f"/undo_a_movement/{player_id}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}
  
def test_undo_a_movement_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  movement_id = "1"
  player_id2 = "1234"

  response = client.put(f"/undo_a_movement/{player_id2}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}
  
def test_undo_a_movement_game_is_not_playing():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  movement_id = "1"

  response = client.put(f"/undo_a_movement/{player_id}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Game is not playing"}
  
def test_undo_a_movement_not_your_turn():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[1]

  movement_id = "1"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  response = client.put(f"/undo_a_movement/{first_turn}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "It's not your turn"}
  
def test_undo_a_movement_no_movements_to_undo():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}
  
  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]
  
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()

  response = client.put(f"/undo_a_movement/{first_turn}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "No movements to undo"}

# Test para undo all movements
def test_undo_all_movements():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[0]
  
  movement_id = "3"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  tile_id1 = 1
  tile_id2 = 2

  response_swap = client.put(f"/swap_tiles/{first_turn}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
  assert response_swap.json() == {"message": "Tiles swapped"}
  
  response = client.put(f"/undo_all_movements/{first_turn}/{game_id}")
  assert response.status_code == 200
  assert response.json() == {"message": "All movements undone"}
  
def test_undo_all_movement_player_not_found():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  movement_id = "1"
  player_id2 = "1234"

  response = client.put(f"/undo_all_movements/{player_id2}/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Player not found"}

def test_undo_all_movement_game_is_not_playing():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]
  
  response = client.put(f"/undo_all_movements/{player_id}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "Game is not playing"}
  
def test_undo_all_movement_not_your_turn():
  player_name = "ValidPlayer"
  response_player = client.post(f"/create_player/{player_name}")
  player_id = response_player.json()["player_id"]

  game_name = "ValidGame"
  game_size = 2
  game_password = "1234"
  response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
  game_id = response_game.json()["game_id"]

  player_name2 = "ValidPlayer2"
  response_player2 = client.post(f"/create_player/{player_name2}")
  player_id2 = response_player2.json()["player_id"]

  response = client.put(f"/join_game/{player_id2}/{game_id}/{game_password}")
  assert response.json() == {"message": player_name2 + " joined the game " + game_name}
  response = client.put(f"/start_game/{player_id}/{game_id}")
  assert response.json() == {"message": "Game started"}

  game = session.query(Game).filter_by(gameid=game_id).first()
  assert game is not None, "Game not found"
  turn = game.turn
  assert turn is not None, "Turn not found"
  first_turn = turn.split(",")[1]

  movement_id = "1"
  session.query(HandMovements).filter_by(playerid=first_turn, gameid=game_id).delete()
  session.commit()
  session.add(HandMovements(movementid=movement_id, playerid=first_turn, gameid=game_id))
  session.commit()

  response = client.put(f"/undo_all_movements/{first_turn}/{game_id}")
  assert response.status_code == 409
  assert response.json() == {"detail": "It's not your turn"}

# Test delete game
def test_delete_game():
    player_name = "ValidPlayer"
    response_player = client.post(f"/create_player/{player_name}")
    player_id = response_player.json()["player_id"]

    game_name = "ValidGame"
    game_size = 3
    game_password = "1234"
    response_game = client.post(f"/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
    game_id = response_game.json()["game_id"]

    response = client.delete(f"/delete_game/{game_id}")
    assert response.status_code == 200
    assert response.json() == {"message": "Game and all associated data deleted"}
  
def test_delete_game_not_found():
  game_id = "1234"
  
  response = client.delete(f"/delete_game/{game_id}")
  assert response.status_code == 404
  assert response.json() == {"detail": "Game not found"}
  
  
# Test delete all
def test_delete_all():
  response = client.delete("/delete_all")
  assert response.status_code == 200
  assert response.json() == {"message": "All players, games, tables, and tiles deleted"}