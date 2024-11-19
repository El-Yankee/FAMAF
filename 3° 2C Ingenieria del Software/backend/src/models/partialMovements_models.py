from sqlalchemy import Column, String, Integer, ForeignKey
from models.game_models import engine, Base, session
import uuid

class PartialMovements(Base):
    __tablename__ = 'partial_movements'
    
    partialid = Column(String, primary_key=True, default=lambda: str(uuid.uuid4()))
    orden = Column(Integer, nullable=True)
    playerid = Column(String, ForeignKey('players.playerid'), primary_key=True, nullable=False)
    gameid = Column(String, ForeignKey('games.gameid'), primary_key=True, nullable=False)
    movementid = Column(String, ForeignKey('movement_chart.movementid'), primary_key=True, nullable=False)
    tileid1 = Column(String, ForeignKey('tiles.id'), nullable=False)
    tileid2 = Column(String, ForeignKey('tiles.id'), nullable=False)
    
    def __init__(self, playerid: str, gameid: str, movementid: str, tileid1: str, tileid2: str):
        self.orden = PartialMovements.count_partial_movements_by_gameid(gameid) + 1
        self.playerid = playerid
        self.gameid = gameid
        self.movementid = movementid
        self.tileid1 = tileid1
        self.tileid2 = tileid2
        
    @staticmethod
    def create_partial_movement(playerid: str, gameid: str, movementid: str, tileid1: str, tileid2: str):
        new_partial_movement = PartialMovements(playerid=playerid, gameid=gameid, movementid=movementid, tileid1=tileid1, tileid2=tileid2)
        session.add(new_partial_movement)
        session.commit()
        return new_partial_movement
    
    @staticmethod
    def count_partial_movements_by_gameid(gameid: str):
        return session.query(PartialMovements).filter_by(gameid=gameid).count()

    @staticmethod
    def get_all_partial_movements_by_gameid(gameid: str):
        return [partial_movement for partial_movement in session.query(PartialMovements).filter_by(gameid=gameid).all()]
    
    @staticmethod
    def get_last_partial_movement(gameid: str):
        return session.query(PartialMovements).filter_by(gameid=gameid).order_by(PartialMovements.orden.desc()).first()
    
    @staticmethod
    def delete_partial_movement(partialid: str):
        partial_movement = session.query(PartialMovements).filter_by(partialid=partialid).first()
        if partial_movement:
            session.delete(partial_movement)
            session.commit()
    
# Create the table
Base.metadata.create_all(bind=engine)