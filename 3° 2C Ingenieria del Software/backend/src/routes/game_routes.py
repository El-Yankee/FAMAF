from fastapi import APIRouter, HTTPException
from models.game_models import Game, session
from models.player_models import Player, PlayerGame
from models.board_models import  Table, Tile, Figures, find_connected_components, match_figures, TableGame
from models.handMovements_models import HandMovements
from models.movementChart_models import MovementChart
from models.partialMovements_models import PartialMovements
from models.board_models import Table, Tile, Figures, find_connected_components, match_figures, TableGame
from models.figure_card_models import Figure_card, shuffle, take_cards, has_blocked_card, has_marked_card
import random
import time
from globals import game_managers, manager  # Import from globals

router = APIRouter()

@router.get("/games")
async def get_games():
    games = session.query(Game).all()
    return [{"game_name": game.name, "game_id": game.gameid, "host_id": game.host,
            "state": game.state, "size": game.size, "current_player": PlayerGame.get_count_of_players_in_game(session, game.gameid), 
            "turn": game.turn} for game in games]

@router.get("/game/{game_id}")
async def get_game(game_id: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")

    players_in_game = session.query(PlayerGame).filter_by(gameid=game.gameid).all()
    player_details = [{"player_id": pg.playerid, "player_name": session.query(Player).filter_by(playerid=pg.playerid).first().name} for pg in players_in_game]

    return {
        "game_name": game.name,
        "game_id": game.gameid,
        "state": game.state,
        "game_size": game.size,
        "players": PlayerGame.get_count_of_players_in_game(session, game.gameid),
        "player_details": player_details,
        "host_id": game.host,
        "turn": game.turn
    }

@router.post("/create_game/{player_id}/{game_name}/{game_size}/{game_password}")
async def create_game(player_id: str, game_name: str, game_size: int, game_password: str = "CAB"):
    try:
        if len(game_name) > 20 or not game_name.isalnum():
            raise ValueError("Game name must be less than 20 characters or alphanumeric")
        elif game_size < 2 or game_size > 4:
            raise ValueError("Game size must be between 2 and 4")
        elif not game_password == "CAB" and game_password and (len(game_password) > 20 or len(game_password) < 4 or not game_password.isalnum()):
            raise ValueError("Game password must be between 4 and 20 characters and alphanumeric")
        elif (session.query(Player).filter_by(playerid=player_id).first()) is None:
            raise HTTPException(status_code=404, detail="Player not found")
        else:
            player = session.query(Player).filter_by(playerid=player_id).first()
            game = Game(game_name, game_size, player.playerid)
            playergame = PlayerGame(player.playerid, game.gameid)
            game.set_game_password(game_password)
            session.add(game)
            session.add(playergame)
            session.commit()
            global update
            update = True
            return {"game_id": game.gameid}
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))

@router.put("/join_game/{player_id}/{game_id}/{game_password}")
async def join_game(player_id: str, game_id: str, game_password: str = "CAB"):
    game = session.query(Game).filter_by(gameid=game_id).first()
    player = session.query(Player).filter_by(playerid=player_id).first()
    if game_password == " ":
        game_password = None
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")
    elif player is None:
        raise HTTPException(status_code=404, detail="Player not found")
    elif game.state == "playing":
        raise HTTPException(status_code=409, detail="Game is already playing")
    elif session.query(PlayerGame).filter_by(gameid=game_id, playerid=player_id).count() > 0:
        raise HTTPException(status_code=409, detail="Player is already in the game")
    elif session.query(PlayerGame).filter_by(gameid=game_id).count() == game.size:
        raise HTTPException(status_code=409, detail="Game is full")
    elif game.password != "CAB" and game_password != game.get_game_password():
        raise HTTPException(status_code=409, detail="Incorrect password")
    else:
        playergame = PlayerGame(player_id, game_id)
        session.add(playergame)
        session.commit()
        update = True
        return {"message": player.name + " joined the game " + game.name}
     

@router.put("/leave_game/{player_id}/{game_id}")
async def leave_game(player_id: str, game_id: str):
    if session.query(Game).filter_by(gameid=game_id).count() == 0:
        raise HTTPException(status_code=404, detail="Game not found")
    elif not session.query(Player).filter_by(playerid=player_id).first():
        raise HTTPException(status_code=404, detail="Player not found")
    else:
        game = session.query(Game).filter_by(gameid=game_id).first()
        # If the game is in waiting state and the player is the host, cancel the game
        if game.host == player_id and game.state == "waiting":
            tables = session.query(Table).filter_by(gameid=game_id).all()
            session.query(Table).filter_by(gameid=game_id).delete()
            session.query(PlayerGame).filter_by(gameid=game_id).delete()
            session.query(Game).filter_by(gameid=game_id).delete()
            session.query(TableGame).filter_by(gameid=game_id).delete()
            session.query(Figure_card).filter_by(gameid=game_id).delete()
            session.commit()
            update = True
            return {"message": "Cancelled game"}
        elif session.query(PlayerGame).filter_by(playerid=player_id, gameid=game_id).count() == 0:
            raise HTTPException(status_code=409, detail="Player is not in the game")
        else:
            session.query(PlayerGame).filter_by(playerid=player_id, gameid=game_id).delete()
            # Remove the player from the turn order if they are in it and the game is in playing state
            if game.state == "playing":
                turn_order = game.turn.split(",")
                turn_order = [pid for pid in turn_order if pid != player_id]
                game.turn = ",".join(turn_order)
                session.query(Figure_card).filter_by(playerid=player_id, gameid=game_id).delete()
            
            session.commit()
            player = session.query(Player).filter_by(playerid=player_id).first()
            update = True
            return {"message": player.name + " left the game " + game.name}

@router.put("/start_game/{player_id}/{game_id}")
async def start_game(player_id: str, game_id: str):
    if session.query(Game).filter_by(gameid=game_id).count() == 0:
        raise HTTPException(status_code=404, detail="Game not found")
    elif session.query(Player).filter_by(playerid=player_id).count() == 0:
        raise HTTPException(status_code=404, detail="Player not found")
    elif session.query(PlayerGame).filter_by(gameid=game_id, playerid=player_id).count() == 0:
        raise HTTPException(status_code=409, detail="Player is not in the game")
    elif session.query(Game).filter_by(gameid=game_id).first().state == "playing":
        raise HTTPException(status_code=409, detail="Game is already playing")
    else:
        game = session.query(Game).filter_by(gameid=game_id).first()
        if player_id != game.host:
            raise HTTPException(status_code=409, detail="Only the host can start the game")
        elif PlayerGame.get_count_of_players_in_game(session, game_id) < game.get_game_size():
            raise HTTPException(status_code=409, detail="The game is not full")
        else:
            update = True
            game.start_game()
            player_ids = [str(player.playerid) for player in session.query(PlayerGame).filter_by(gameid=game_id).all()]
            random.shuffle(player_ids)
            game.turn = ",".join(player_ids)
            session.commit()
            shuffle(game_id)
            # Repartir movimientos a los jugadores
            for player in player_ids:
                HandMovements.deals_moves(player, game.gameid, 3)
                take_cards(game_id, player)

            # Crear una tabla para el juego y las fichas asociadas
            TableGame.create_table_for_game(game_id)
            table = session.query(Table).filter_by(gameid=game_id).first()
            tablegame = TableGame(table.id,game_id)
            session.add(tablegame)
            session.commit()

            tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
            connected_components = find_connected_components(tiles)
            match_figures(connected_components, session.query(Figures).all(), table)
            session.commit()

            game.timestamp = int(time.time())
            session.commit()
            return {"message": "Game started"}

@router.put("/next_turn/{player_id}/{game_id}")
async def next_turn(player_id: str, game_id: str):
    from globals import manager  # Ensure manager is imported
    if session.query(Game).filter_by(gameid=game_id).count() == 0:
        raise HTTPException(status_code=404, detail="Game not found")
    elif session.query(Player).filter_by(playerid=player_id).count() == 0:
        raise HTTPException(status_code=404, detail="Player not found")
    else:
        game = session.query(Game).filter_by(gameid=game_id).first()
        if player_id != game.turn.split(",")[0]:
            raise HTTPException(status_code=409, detail="It's not your turn")
        else:
            game.turn = ",".join(game.turn.split(",")[1:] + game.turn.split(",")[:1])
            # Tomar cartas si no una tengo carta bloqueada or "marcada"
            if (has_blocked_card(game_id, player_id) or has_marked_card(game_id, player_id)) == False:
                take_cards(game_id, player_id)

            # Devuelvo las cartas si tengo movimientos parciales y no descarte niguna figura
            if len(PartialMovements.get_all_partial_movements_by_gameid(game_id)) > 0 and HandMovements.count_movements_charts_by_gameid_and_playerid(game.gameid, player_id) < 3:
                partial_movements = PartialMovements.get_all_partial_movements_by_gameid(game_id)
                partial_movements = sorted(partial_movements, key=lambda x: x.orden, reverse=True)
                for partial_movement in partial_movements:
                    Tile.swap_tiles_color(partial_movement.tileid1, partial_movement.tileid2)
                    HandMovements.create_hand_movement(partial_movement.movementid, partial_movement.playerid, game_id)
                    PartialMovements.delete_partial_movement(partial_movement.partialid)
                    
            # Si descarte una figura y no tengo movimientos parciales reparto cartas nuevas
            if len(PartialMovements.get_all_partial_movements_by_gameid(game_id)) == 0 and HandMovements.count_movements_charts_by_gameid_and_playerid(game.gameid, player_id) < 3:
                HandMovements.deals_moves(player_id, game.gameid, 3 - HandMovements.count_movements_charts_by_gameid_and_playerid(game.gameid, player_id))
                
            
            table = session.query(Table).filter_by(gameid=game_id).first()
            tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
            connected_components = find_connected_components(tiles)
            match_figures(connected_components, session.query(Figures).all(), table)
            session.commit()
            update = True

            game.timestamp = int(time.time())
            session.commit()

            # Enviar mensaje por WebSocket
            player = session.query(Player).filter_by(playerid=player_id).first()
            message = {"message":"finalizo el turno de"+" "+player.name, "player_name": "sistema"}
            if game_id in game_managers:
                await game_managers[game_id].broadcast(message)

            return {"message": "Next turn"}

@router.put("/swap_tiles/{player_id}/{game_id}/{movement_id}/{tile_id1}/{tile_id2}")
async def swap_tiles(player_id: str, game_id: str, movement_id: str, tile_id1: str, tile_id2: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    player = session.query(Player).filter_by(playerid=player_id).first()
    movement = session.query(MovementChart).filter_by(movementid=movement_id).first()
    table = session.query(Table).filter_by(gameid=game_id).first()
    tile1 = session.query(Tile).filter_by(table_id=table.id, number=tile_id1).first()
    tile2 = session.query(Tile).filter_by(table_id=table.id, number=tile_id2).first()
    if session.query(Game).filter_by(gameid=game_id).count() == 0:
        raise HTTPException(status_code=404, detail="Game not found")
    elif session.query(Player).filter_by(playerid=player_id).count() == 0:
        raise HTTPException(status_code=404, detail="Player not found")
    elif session.query(PlayerGame).filter_by(gameid=game_id, playerid=player_id).count() == 0:
        raise HTTPException(status_code=409, detail="Player is not in the game")
    elif session.query(Tile).filter_by(table_id=table.id, number=tile_id1).count() == 0:
        raise HTTPException(status_code=404, detail="Tile 1 not found")
    elif session.query(Tile).filter_by(table_id=table.id, number=tile_id2).count() == 0:
        raise HTTPException(status_code=404, detail="Tile 2 not found")
    elif HandMovements.player_have_not_movement(player_id, game_id, movement_id):
        raise HTTPException(status_code=409, detail="Player has not this movement")
    elif player_id != game.turn.split(",")[0]:
            raise HTTPException(status_code=409, detail="It's not your turn")
    else:
        
        movementchart = MovementChart.get_movement_chart_by_id(movement_id)
        rot0 = MovementChart.get_tile_for_rotation(movementchart.rot0, tile1)
        rot90 = MovementChart.get_tile_for_rotation(movementchart.rot90, tile1)
        rot180 = MovementChart.get_tile_for_rotation(movementchart.rot180, tile1)
        rot270 = MovementChart.get_tile_for_rotation(movementchart.rot270, tile1)
        
        if rot0 == tile2.number or rot90 == tile2.number or rot180 == tile2.number or rot270 == tile2.number:
            Tile.swap_tiles_color(tile1.id, tile2.id)
            table = session.query(Table).filter_by(gameid=game_id).first()
            tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
            connected_components = find_connected_components(tiles)
            match_figures(connected_components, session.query(Figures).all(), table)
            HandMovements.delete_hand_movements(player_id, game_id, movement_id)
            PartialMovements.create_partial_movement(player_id, game_id, movement_id, tile1.id, tile2.id)
            session.commit()
            # Enviar mensaje por WebSocket
            player = session.query(Player).filter_by(playerid=player_id).first()
            message = {"message":player.name+" "+"realizó un movimiento", "player_name": "sistema"}
            if game_id in game_managers:
                await game_managers[game_id].broadcast(message)
            return {"message": "Tiles swapped"}
            # Enviar mensaje por WebSocket



        else:
            raise HTTPException(status_code=409, detail="Invalid movement")
        
@router.put("/undo_a_movement/{player_id}/{game_id}")
async def undo_a_movement(player_id: str, game_id: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    player = session.query(Player).filter_by(playerid=player_id).first()
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")
    elif player is None:
        raise HTTPException(status_code=404, detail="Player not found")
    elif game.state == "waiting":
        raise HTTPException(status_code=409, detail="Game is not playing")
    elif player_id != game.turn.split(",")[0]:
        raise HTTPException(status_code=409, detail="It's not your turn")
    else:
        partial_movement = PartialMovements.get_last_partial_movement(game_id)
        if partial_movement is None:
            raise HTTPException(status_code=404, detail="No movements to undo")
        else:
            Tile.swap_tiles_color(partial_movement.tileid1, partial_movement.tileid2)
            table = session.query(Table).filter_by(gameid=game_id).first()
            tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
            connected_components = find_connected_components(tiles)
            match_figures(connected_components, session.query(Figures).all(), table)
            session.commit()
            HandMovements.create_hand_movement(partial_movement.movementid, partial_movement.playerid, game_id)
            PartialMovements.delete_partial_movement(partial_movement.partialid)
            player = session.query(Player).filter_by(playerid=player_id).first()
            # Enviar mensaje por WebSocket
            player = session.query(Player).filter_by(playerid=player_id).first()
            message = {"message":player.name+" "+"deshizo un movimiento", "player_name": "sistema"}
            if game_id in game_managers:
                await game_managers[game_id].broadcast(message)
            return {"message": "Movement undone"}
        
@router.put("/undo_all_movements/{player_id}/{game_id}")
async def undo_all_movements(player_id: str, game_id: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    player = session.query(Player).filter_by(playerid=player_id).first()
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")
    elif player is None:
        raise HTTPException(status_code=404, detail="Player not found")
    elif game.state == "waiting":
        raise HTTPException(status_code=409, detail="Game is not playing")
    elif player_id != game.turn.split(",")[0]:
            raise HTTPException(status_code=409, detail="It's not your turn")
    else:
        partial_movements = PartialMovements.get_all_partial_movements_by_gameid(game_id)
        if partial_movements is None:
            raise HTTPException(status_code=404, detail="No movements to undo")
        else:
            partial_movements = sorted(partial_movements, key=lambda x: x.orden, reverse=True)
            for partial_movement in partial_movements:
                Tile.swap_tiles_color(partial_movement.tileid1, partial_movement.tileid2)
                table = session.query(Table).filter_by(gameid=game_id).first()
                tiles = session.query(Tile).join(Table).filter(Table.gameid == game_id).all()
                connected_components = find_connected_components(tiles)
                match_figures(connected_components, session.query(Figures).all(), table)
                session.commit()
                HandMovements.create_hand_movement(partial_movement.movementid, partial_movement.playerid, game_id)
                PartialMovements.delete_partial_movement(partial_movement.partialid)
            if len(partial_movements) >= 1:
                # Enviar mensaje por WebSocket
                player = session.query(Player).filter_by(playerid=player_id).first()
                message = {"message": player.name + " " + "deshizo todos sus movimientos", "player_name": "sistema"}
                if game_id in game_managers:
                    await game_managers[game_id].broadcast(message)
            return {"message": "All movements undone"}

@router.delete("/delete_game/{game_id}")
async def delete_game(game_id: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")
    
    # Eliminar todas las fichas asociadas a las tablas del juego
    tables = session.query(Table).filter_by(gameid=game_id).all()
    for table in tables:
        session.query(Tile).filter_by(table_id=table.id).delete()
    
    # Eliminar todas las tablas asociadas al juego
    session.query(Table).filter_by(gameid=game_id).delete()
    
    # Eliminar todas las relaciones de jugadores con el juego
    session.query(PlayerGame).filter_by(gameid=game_id).delete()
    
    # Eliminar el juego
    session.query(Game).filter_by(gameid=game_id).delete()

    # Eliminar todas las relaciones de tablas con el juego
    session.query(TableGame).filter_by(gameid=game_id).delete()

    session.query(Figure_card).filter_by(gameid=game_id).delete()
    
    session.commit()
    return {"message": "Game and all associated data deleted"}

