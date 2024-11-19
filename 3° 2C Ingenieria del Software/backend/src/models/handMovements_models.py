from sqlalchemy import Column, String, ForeignKey
from models.game_models import engine, Base, session
import uuid
import random

class HandMovements(Base):
    __tablename__ = 'hand_movements'
    
    handid = Column(String, primary_key=True, default=lambda: str(uuid.uuid4()))
    movementid = Column(String, ForeignKey('movement_chart.movementid'), primary_key=True, nullable=False)
    playerid = Column(String, ForeignKey('players.playerid'), primary_key=True, nullable=False)
    gameid = Column(String, ForeignKey('games.gameid'), primary_key=True, nullable=False)
    
    def __init__(self, movementid: str, playerid: str, gameid: str):
        self.movementid = movementid
        self.playerid = playerid
        self.gameid = gameid
        
    @staticmethod
    def create_hand_movement(movementid: str, playerid: str, gameid: str):
        new_hand_movement = HandMovements(movementid=movementid, playerid=playerid, gameid=gameid)
        session.add(new_hand_movement)
        session.commit()
        return new_hand_movement
        
    @staticmethod
    def count_movements_by_movementid(movementid: str, gameid: str):
        return session.query(HandMovements).filter_by(movementid=movementid, gameid=gameid).count()
    
    @staticmethod
    def player_have_not_movement(playerid: str, gameid: str, movementid: str):
        return session.query(HandMovements).filter_by(playerid=playerid, gameid=gameid, movementid=movementid).count() <= 0
    
    @staticmethod
    def count_movements_charts_by_gameid_and_playerid(gameid: str, playerid: str):
        if session.query(HandMovements).filter_by(gameid=gameid, playerid=playerid).count() > 3:
            raise Exception("The player has many movements")
        return session.query(HandMovements).filter_by(gameid=gameid, playerid=playerid).count()
    
    @staticmethod
    def get_movements_charts_by_player_id(playerid: str, gameid: str):
        return [movement.movementid for movement in session.query(HandMovements).filter_by(playerid=playerid, gameid=gameid).all()]
     
    # Reparte movimientos al jugador de la partida
    @staticmethod
    def deals_moves(playerid: str, gameid: str, quantity: int):
        for _ in range(quantity):
            movement_ids = []
            for chart in range(1, 8):
                movement = HandMovements.count_movements_by_movementid(chart, gameid)
                if 0 <= movement < 7:
                    movement_ids.append(chart)
                
            if movement_ids:
                movementid = random.choice(movement_ids)
                session.add(HandMovements(movementid=movementid, playerid=playerid, gameid=gameid))
                session.commit()
                
    @staticmethod
    def delete_hand_movements(playerid: str, gameid: str, movementid: str):
        hand_movement = session.query(HandMovements).filter_by(playerid=playerid, gameid=gameid, movementid=movementid).first()
        if hand_movement:
            session.delete(hand_movement)
            session.commit()
    


# Create the table
Base.metadata.create_all(bind=engine)