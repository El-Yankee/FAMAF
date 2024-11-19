from sqlalchemy import Column, String
from models.game_models import engine, Base, session
from models.board_models import Tile


class MovementChart(Base):
    __tablename__ = 'movement_chart'
    
    movementid = Column(String, primary_key=True)
    rot0 = Column(String, nullable=False)
    rot90 = Column(String, nullable=False)
    rot180 = Column(String, nullable=False)
    rot270 = Column(String, nullable=False)
    
    def __init__(self, movementid:str, rot0: str, rot90: str, rot180: str, rot270: str):
        self.movementid = movementid
        self.rot0 = rot0
        self.rot90 = rot90
        self.rot180 = rot180
        self.rot270 = rot270
        
    @staticmethod
    def create_movement_chart(movementid:str, rot0: str, rot90: str, rot180: str, rot270: str):
        new_movement = MovementChart(movementid=movementid ,rot0=rot0, rot90=rot90, rot180=rot180, rot270=rot270)
        session.add(new_movement)
        session.commit()
        return new_movement
    
    @staticmethod
    def get_movement_chart_by_id(movementid: str):
        return session.query(MovementChart).filter_by(movementid=movementid).first()
    
    @staticmethod
    def is_table_empty():
        return session.query(MovementChart).count() == 0
    
    @staticmethod
    def get_tile_for_rotation(rotation: str, cordenada:Tile):
            rot = rotation.split(",")
            new_x = int(rot[0]) + cordenada.x
            new_y = int(rot[1]) + cordenada.y
            if new_x < 0 or new_x > 5 or new_y < 0 or new_y > 5:
                return None
            else:
                tile = session.query(Tile).filter_by(x=new_x, y=new_y).first()
                return tile.number if tile else None
    
    @staticmethod
    def game_movement():
        MovementChart.create_movement_chart("1", "2,2", "-2,2", "-2,-2", "2,-2")
        MovementChart.create_movement_chart("2", "0,2", "-2,0", "0,-2", "2,0")
        MovementChart.create_movement_chart("3", "0,1", "-1,0", "0,-1", "1,0")
        MovementChart.create_movement_chart("4", "1,1", "-1,1", "-1,-1", "1,-1")
        MovementChart.create_movement_chart("5", "2,1", "-1,2", "-2,-1", "1,-2")
        MovementChart.create_movement_chart("6", "1,2", "-2,1", "-1,-2", "2,-1")
        MovementChart.create_movement_chart("7", "0,4", "-4,0", "0,-4", "4,0")
        
# Create the table
Base.metadata.create_all(bind=engine)

# Ejecutar game_movement solo si la tabla está vacía
if MovementChart.is_table_empty():
    MovementChart.game_movement()
    