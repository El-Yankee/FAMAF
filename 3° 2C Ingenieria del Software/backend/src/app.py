from fastapi import FastAPI, WebSocket, WebSocketDisconnect, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from models.manager_models import ConnectionManager
from routes.player_routes import router as player_router
from routes.game_routes import router as game_router
from routes.movementChart_routes import router as movementChart_router
import asyncio
from models.game_models import Game, session
from models.player_models import PlayerGame, Player
from models.handMovements_models import HandMovements
from models.board_models import  Table, Tile, Figures, find_connected_components, match_figures, TableGame
from models.partialMovements_models import PartialMovements
from models.figure_card_models import Figure_card
from globals import game_managers, manager  # Import from globals

app = FastAPI()

# Configuración de CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],  # Permitir solicitudes desde esta URL
    allow_credentials=True,
    allow_methods=["*"],  # Permitir todos los métodos (GET, POST, PUT, DELETE, etc.)
    allow_headers=["*"],  # Permitir todos los encabezados
)

Figures.create_figures()

@app.get("/figures/{game_id}")
async def get_figures(game_id: str):
    tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
    connected_components = find_connected_components(tiles)
    match_figures(connected_components, session.query(Figures).all())
    session.commit()
    return match_figures(connected_components, session.query(Figures).all())
    
app.include_router(player_router, tags=["player"])
app.include_router(game_router, tags=["game"])
app.include_router(movementChart_router, tags=["movementChart"])

@app.delete("/delete_all")
async def delete_all():
    session.query(PlayerGame).delete()
    session.query(Game).delete()
    session.query(Player).delete()
    session.query(Tile).delete()  # Eliminar todas las fichas
    session.query(Table).delete()  # Eliminar todas las tablas
    session.query(TableGame).delete()  # Eliminar todas las relaciones entre tablas y juegos
    session.query(HandMovements).delete()  # Eliminar todos los movimientos de las manos
    session.query(PartialMovements).delete()  # Eliminar todos los movimientos parciales
    session.query(Figure_card).delete()  # Eliminar todas las cartas de figuras
    session.commit()
    return {"message": "All players, games, tables, and tiles deleted"}

@app.websocket("/ws/{player_id}")
async def websocket_endpoint(websocket: WebSocket, player_id: str):
    await manager.connect(websocket)
    try:
        while True:
            games = session.query(Game).all()
            gamelist = []
            for game in games:
                players_in_game = session.query(PlayerGame).filter_by(gameid=game.gameid).all()
                player_details = [{"player_id": pg.playerid, "player_name": session.query(Player).filter_by(playerid=pg.playerid).first().name} for pg in players_in_game]
                gamelist.append({
                    "game_name": game.name,
                    "game_id": game.gameid,
                    "state": game.state,
                    "type": "Public" if game.password == "CAB" else "Private",
                    "game_size": game.size,
                    "players": PlayerGame.get_count_of_players_in_game(session, game.gameid),
                    "player_details": player_details
                })
            await websocket.send_json(gamelist)
            await asyncio.sleep(1)  # Delay to avoid flooding the client with messages  

    except WebSocketDisconnect:
        manager.disconnect(websocket)
        await manager.broadcast(f"Client #{player_id} left the chat")

@app.websocket("/ws/game/{game_id}")
async def game_websocket_endpoint(websocket: WebSocket, game_id: str):
    await manager.connect(websocket)
    try:
        while True:
            game = session.query(Game).filter_by(gameid=game_id).first()
            if game is None:
                await websocket.send_json({"error": "Game not found"})
                break

            players_in_game = session.query(PlayerGame).filter_by(gameid=game_id).all()
            table = session.query(Table).filter_by(gameid=game_id).first()
            player_details = [
                {
                    "player_id": pg.playerid,
                    "player_name": session.query(Player).filter_by(playerid=pg.playerid).first().name,
                    "number_of_figure_card": session.query(Figure_card).filter_by(playerid=pg.playerid, gameid=game_id).count(),  
                    "number_of_movement_charts": session.query(HandMovements).filter_by(playerid=pg.playerid, gameid=game_id).count(),
                    "figure_cards": [{"card_id": fc.id, "figure": fc.figure, "state": fc.state} for fc in session.query(Figure_card).filter_by(playerid=pg.playerid, gameid=game_id, in_hand=True).all()]
                }
                for pg in players_in_game
            ]
            turnos = game.turn
            round = turnos.split(",") if turnos else []
            if turnos is not None:
                turnos = turnos.split(",")
                turnos = turnos[0]

            # Obtener el tablero y las fichas asociadas a la partida
            table = session.query(Table).filter_by(gameid=game_id).first()
            if table:
                tiles = session.query(Tile).filter_by(table_id=table.id).all()
                board = [{"id": tile.number, "x": tile.x, "y": tile.y, "color": tile.color, "highlight": tile.highlight} for tile in tiles]
            else:
                board = []

            game_details = {
                "game_name": game.name,
                "game_id": game.gameid,
                "state": game.state,
                "game_size": game.size,
                "players": PlayerGame.get_count_of_players_in_game(session, game.gameid),
                "player_details": player_details,
                "turn": turnos,
                "board": board,
                "round": round,
                "prohibited_color": table.get_prohibited_color() if table else None,
                "timestamp": game.timestamp
            }
            await websocket.send_json(game_details)
            await asyncio.sleep(1)  # Delay to avoid flooding the client with messages

    except WebSocketDisconnect:
        await game_managers[game_id].disconnect(websocket)
        await game_managers[game_id].broadcast(f"Client #{game_id} left the chat")

@app.websocket("/ws/chat/{game_id}/{player_id}")
async def chat_websocket_endpoint(websocket: WebSocket, game_id: str, player_id: str):
    try:
        if game_id not in game_managers:
            game_managers[game_id] = ConnectionManager()
        await game_managers[game_id].connect(websocket)
        player = session.query(Player).filter_by(playerid=player_id).first()
        player_name = player.name if player else "Unknown"
        while True:
            data = await websocket.receive_text()
            message = {
                "player_name": player_name,
                "message": data
            }
            await game_managers[game_id].broadcast(message)
    except WebSocketDisconnect:
        if game_id in game_managers:
            await game_managers[game_id].disconnect(websocket)
            await game_managers[game_id].broadcast(f"{player_name} left the chat")