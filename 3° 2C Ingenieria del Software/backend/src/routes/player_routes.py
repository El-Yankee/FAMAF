from fastapi import APIRouter, HTTPException
from models.player_models import Player, PlayerGame
from models.game_models import Game, session
from models.movementChart_models import MovementChart

router = APIRouter()

@router.get("/players")
async def get_players():
    
    
    # QUITAR
    if MovementChart.is_table_empty():
        MovementChart.game_movement()
    players = session.query(Player).all()
    return [{"player_name": player.name, "player_id": player.playerid} for player in players]

@router.get("/player/{player_id}")
async def get_player(player_id: str):
    player = session.query(Player).filter_by(playerid=player_id).first()
    if player is None:
        raise HTTPException(status_code=404, detail="Player not found")
    return player

@router.get("/players_in_game/{game_id}")
async def get_players_in_game(game_id: str):
    game = session.query(Game).filter_by(gameid=game_id).first()
    if game is None:
        raise HTTPException(status_code=404, detail="Game not found")
    else:
        player_games = session.query(PlayerGame).filter_by(gameid=game_id).all()
        players_in_game = [session.query(Player).filter_by(playerid=pg.playerid).first() for pg in player_games]
        return [{"player_name": player.name, "player_id": player.playerid} for player in players_in_game]

@router.post("/create_player/{player_name}")
async def create_player(player_name: str):
    try:
        if len(player_name) > 20 or not player_name.isalnum():
            raise ValueError("Player name must be less than 20 character or alphanumeric")
        player = Player(player_name)
        session.add(player)
        session.commit()
        return {"player_id": player.playerid}
    except ValueError as e:
        raise HTTPException(status_code=400, detail=str(e))
        
@router.delete("/delete_player/{player_id}")
async def delete_player(player_id: str):
    player = session.query(Player).filter_by(playerid=player_id).first()
    if player is None:
        raise HTTPException(status_code=404, detail="Player not found")
    else:
        session.query(PlayerGame).filter_by(playerid=player_id).delete()
        session.query(Player).filter_by(playerid=player_id).delete()
        session.commit()
        return {"message": "Player deleted"}
