from sqlalchemy import Column, Integer, String, ForeignKey, Boolean
from sqlalchemy.orm import relationship
from models.game_models import  Base, session
import random



class Table(Base):
    __tablename__ = 'table'

    id = Column(Integer, primary_key=True)
    gameid = Column(String, ForeignKey('games.gameid'), nullable=False)
    tiles = relationship("Tile", backref="table")
    prohibited_color = Column(String, default="white")
    

    def __init__(self, gameid: str):
        self.gameid = gameid
        
    def get_table_id(self):
        return self.id
        
    def get_prohibited_color(self):
        return self.prohibited_color
    
    def set_prohibited_color(self, color):
        self.prohibited_color = color
        session.commit()

    @staticmethod
    def get_table_by_game_id(game_id: str):
        return session.query(Table).filter_by(gameid=game_id).first()
    
    
class Tile(Base):
    __tablename__ = 'tiles'
    id = Column(Integer, primary_key=True)
    table_id = Column(Integer, ForeignKey('table.id'), nullable=False)
    x = Column(Integer, nullable=False)
    y = Column(Integer, nullable=False)
    color = Column(String, default="white")
    highlight = Column(Boolean, default=False)
    number = Column(Integer, default=0)

    def __init__(self, table_id: int, x: int, y: int, color: str, highlight: bool,number: int):
        self.table_id = table_id
        self.x = x
        self.y = y
        self.color = color
        self.highlight = False
        self.number = number

    @staticmethod
    def create_tiles_for_table(table_id: int):
        colors = ['red', 'green', 'yellow', 'blue']
        tiles = []
        color_distribution = colors * 9  # 36 tiles, 9 of each color
        random.shuffle(color_distribution)
        aux=1
        for i in range(6):
            for j in range(6):
                color = color_distribution.pop()
                tiles.append(Tile(table_id=table_id, x=i, y=j, color=color, highlight=False,number=aux))
                aux+=1
        session.add_all(tiles)
        session.commit()

    @staticmethod
    def swap_tiles_color(tile_id1: int, tile_id2: int):
        tile1 = session.query(Tile).filter_by(id=tile_id1).first()
        tile2 = session.query(Tile).filter_by(id=tile_id2).first()
        if tile1 and tile2:
            tile1.color, tile2.color = tile2.color, tile1.color
            session.commit()
            
    @staticmethod
    def get_tiles_by_table_id(table_id: int):
        return session.query(Tile).filter_by(table_id=table_id).all()

class TableGame(Base):
    __tablename__ = 'tablegames'

    tableid = Column(Integer, ForeignKey('table.id'), primary_key=True)
    gameid = Column(String, ForeignKey('games.gameid'), primary_key=True)

    def __init__(self, tableid: int, gameid: str):
        self.tableid = tableid
        self.gameid = gameid

    @staticmethod
    def create_table_for_game(gameid: str):
        table = Table(gameid)
        session.add(table)
        session.commit()
        Tile.create_tiles_for_table(table.id)

class Figures(Base):
    __tablename__ = 'figures'

    id = Column(Integer, primary_key=True)
    points = Column(String)
    rot90=Column(String)
    rot180=Column(String)
    rot270=Column(String)

    @staticmethod
    def detect_figures(table_id: int):
        table = session.query(Table).filter_by(id=table_id).first()
        tiles = session.query(Tile).filter_by(table_id=table_id).all()
        points = 0
        for tile in tiles:
            points += 1
            tile.highlight = True
            session.commit()
        return points
    
    @staticmethod
    def create_figures():
        if session.query(Figures).count() == 0:
            figures_data = [
                ("01,02,10,11", "00,01,11,12", "00,10,11,12", "10,20,01,11"),                       # 1
                ("00,10,01,11", "00,10,01,11", "00,10,01,11", "00,10,01,11"),                       # 2
                ("00,10,11,21", "00,10,11,21", "10,01,11,02", "10,01,11,02"),                       # 3
                ("00,10,20,11", "00,01,02,11", "10,01,11,12", "10,11,01,21"),                       # 4
                ("11,12,13,03", "00,10,20,21", "00,01,02,10", "00,01,11,21"),                       # 5
                ("10,20,30,40", "00,01,02,03", "10,20,30,40", "00,01,02,03"),                       # 6
                ("00,01,02,12", "01,11,21,20", "00,10,11,12", "00,10,20,01"),                       # 7
                ("20,12,11,10,00", "21,11,02,01,00", "22,12,02,11,10", "22,21,11,01,20"),           # 8
                ("31,21,11,10,00", "30,20,10,11,01", "03,12,02,11,10", "31,21,20,10,00"),           # 9
                ("24,23,13,12,11", "30,20,21,11,01", "13,12,11,01,00", "30,20,10,11,01"),           # 10
                ("20,10,11,02,01", "22,12,11,01,00", "20,21,11,12,02", "22,21,11,10,00"),           # 11
                ("40,30,20,10,00", "04,03,02,01,00", "40,30,20,10,00", "04,03,02,01,00"),           # 12
                ("20,10,02,01,00", "22,12,02,01,00", "02,12,22,21,20", "22,21,20,10,00"),           # 13
                ("10,11,12,13,03", "31,30,20,10,00", "10,03,02,01,00", "31,21,11,01,00"),           # 14
                ("13,03,02,01,00", "30,31,21,11,01", "13,12,11,10,00", "00,10,20,30,01"),           # 15
                ("10,11,01,12,22", "12,21,11,10,00", "21,11,10,02,01", "21,11,10,02,01"),           # 16
                ("10,11,01,12,22", "20,21,11,02,01", "22,12,11,10,00", "20,21,11,02,01"),           # 17
                ("20,12,11,10,01", "21,12,11,01,00", "21,10,11,12,02", "22,21,10,11,01"),           # 18
                ("20,10,11,12,02", "00,11,01,21,22", "20,10,11,12,02", "00,11,01,21,22"),           # 19
                ("02,13,12,11,10", "21,30,20,10,00", "11,03,02,01,00", "10,31,21,11,01"),           # 20
                ("12,03,02,01,00", "20,31,21,11,01", "13,12,10,11,01", "11,30,20,10,00"),           # 21
                ("12,11,02,01,00", "10,20,21,11,01", "01,12,11,10,00", "01,11,20,10,00"),           # 22
                ("12,02,01,10,00", "20,21,11,01,00", "02,12,11,10,00", "21,20,10,01,00"),           # 23
                ("21,12,10,11,01", "21,12,10,11,01", "21,12,10,11,01", "21,12,10,11,01"),           # 24
                ("10,11,12,02,01", "11,21,20,10,00", "02,01,11,10,00", "01,21,11,10,00")            # 25
            ]

            for figure in figures_data:
                new_figure = Figures(
                    points=figure[0],
                    rot90=figure[1],
                    rot180=figure[2],
                    rot270=figure[3]
                )
                session.add(new_figure)
            session.commit()
 
def find_connected_components(tiles):
    def dfs(tile, visited, component):
        stack = [tile]
        while stack:
            current_tile = stack.pop()
            if current_tile not in visited:
                visited.add(current_tile)
                component.append(current_tile)
                neighbors = [
                    t for t in tiles if (
                        t.color == current_tile.color and (
                            (t.x == current_tile.x and abs(t.y - current_tile.y) == 1) or
                            (t.y == current_tile.y and abs(t.x - current_tile.x) == 1)
                        )
                    )
                ]
                stack.extend(neighbors)
    visited = set()
    components = []
    for tile in tiles:
        if tile not in visited:
            component = []
            dfs(tile, visited, component)
            components.append(component)
    return components

def get_connected_component_for_tile_by_number(tile: Tile):
    tile_number = tile.number
    tile = session.query(Tile).filter_by(number=tile_number, table_id=tile.table_id).first()
    if not tile:
        return []

    tiles = session.query(Tile).filter_by(table_id=tile.table_id).all()
    connected_components = find_connected_components(tiles)
    
    for component in connected_components:
        if tile in component:
            return component
    return []
    
def normalize_points(points):
    # Convert points to a list of tuples (x, y)
    points = [tuple(map(int, point)) for point in points]
    # Find the point with the smallest x and y values
    min_x = min(point[0] for point in points)
    min_y = min(point[1] for point in points)
    # Normalize points so that the smallest point is (0, 0)
    normalized_points = {(x - min_x, y - min_y) for x, y in points}
    return normalized_points

def match_figures(connected_components, figures, table: Table):
    matching_tiles = {}
    for component in connected_components:
        component_points = {f"{tile.x}{tile.y}" for tile in component}
        normalized_component_points = normalize_points(component_points)
        matched = False
        for figure in figures:
            figure_points_variants = [
                set(figure.points.split(",")),
                set(figure.rot90.split(",")),
                set(figure.rot180.split(",")),
                set(figure.rot270.split(","))
            ]
            for figure_points in figure_points_variants:
                normalized_figure_points = normalize_points(figure_points)
                if normalized_component_points == normalized_figure_points and table.prohibited_color != component[0].color:
                    for tile in component:
                        tile.highlight = True
                        matching_tiles[f"{tile.x}{tile.y}"] = figure.id
                    matched = True
                    break
            if matched:
                break
        if not matched:
            for tile in component:
                tile.highlight = False
    session.commit()  # Commit the changes to the database
    return matching_tiles