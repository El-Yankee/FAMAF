import pytest
import asyncio
import uvicorn
import json
from fastapi.testclient import TestClient
from multiprocessing import Process
from fastapi import WebSocket
from websockets import connect
from time import sleep
from app import app

client = TestClient(app)

@pytest.fixture(scope="session", autouse=True)
def start_server():
    # Inicia el servidor en un proceso separado
    process = Process(target=uvicorn.run, args=(app,), kwargs={"host": "127.0.0.1", "port": 8000})
    process.start()
    sleep(1)  # Espera un momento para que el servidor inicie
    yield
    process.terminate()
    process.join()

@pytest.mark.asyncio
async def test_websocket_connection():
    async with connect("ws://127.0.0.1:8000/ws/test_player") as websocket:
        await websocket.send(json.dumps({"message": "Hello WebSocket"}))
        response = await websocket.recv()
        response_data = json.loads(response)
        
        # Verificar que la respuesta sea una lista
        assert isinstance(response_data, list)
        
        # Verificar que cada juego tenga los campos esperados
        for game in response_data:
            assert "game_name" in game
            assert "game_id" in game
            assert "state" in game
            assert "game_size" in game
            assert "players" in game
            assert "player_details" in game
            assert isinstance(game["player_details"], list)

@pytest.mark.asyncio
async def test_game_websocket_connection():
    async with connect("ws://127.0.0.1:8000/ws/game/test_game") as websocket:
        await websocket.send(json.dumps({"message": "Hello Game WebSocket"}))
        response = await websocket.recv()
        response_data = json.loads(response)
        
        # Verificar que la respuesta contenga un error
        assert "error" in response_data
        assert response_data["error"] == "Game not found"

        # Verificar que la respuesta contenga los campos esperados en caso de éxito
        if "game_name" in response_data:
            assert "game_id" in response_data
            assert "state" in response_data
            assert "game_size" in response_data
            assert "players" in response_data
            assert "player_details" in response_data
            assert isinstance(response_data["player_details"], list)
            assert "turn" in response_data

            @pytest.mark.asyncio
            async def test_chat_websocket_broadcast():
                async with connect("ws://127.0.0.1:8000/ws/chat/test_game/test_player1") as websocket1, \
                           connect("ws://127.0.0.1:8000/ws/chat/test_game/test_player2") as websocket2:
                    await websocket1.send(json.dumps({"message": "Hello from Player 1"}))
                    response1 = await websocket1.recv()
                    response2 = await websocket2.recv()
                    response_data1 = json.loads(response1)
                    response_data2 = json.loads(response2)

                    # Verificar que ambos jugadores reciban el mensaje
                    assert response_data1["message"] == "Hello from Player 1"
                    assert response_data2["message"] == "Hello from Player 1"
                    assert response_data1["player_name"] == response_data2["player_name"]

            @pytest.mark.asyncio
            async def test_chat_websocket_disconnect():
                async with connect("ws://127.0.0.1:8000/ws/chat/test_game/test_player") as websocket:
                    await websocket.send(json.dumps({"message": "Hello Chat WebSocket"}))
                    response = await websocket.recv()
                    response_data = json.loads(response)

                    # Verificar que la respuesta contenga el nombre del jugador y el mensaje
                    assert "player_name" in response_data
                    assert "message" in response_data
                    assert response_data["message"] == "Hello Chat WebSocket"

                    # Simular desconexión
                    await websocket.close()
                    try:
                        response = await websocket.recv()
                    except Exception as e:
                        response = str(e)

                    # Verificar que se haya enviado el mensaje de desconexión
                    assert "left the chat" in response
