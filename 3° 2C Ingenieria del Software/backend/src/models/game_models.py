from sqlalchemy import Column, Integer, String, ForeignKey, create_engine, Boolean
from sqlalchemy.orm import declarative_base
from sqlalchemy.orm import sessionmaker, relationship
import random
import uuid
import os
# Crear la carpeta "base de datos" si no existe
if not os.path.exists('database'):
    os.makedirs('database')

# Configurar el motor de la base de datos para usar un archivo SQLite en la carpeta "base de datos"
engine = create_engine('sqlite:///database/games.db', connect_args={'check_same_thread': False})
Base = declarative_base()

Session = sessionmaker(bind=engine)
session = Session()

class Game(Base):
    __tablename__ = 'games'

    gameid = Column(String, primary_key=True)
    name = Column(String, nullable=False)
    state = Column(String, default="waiting")
    size = Column(Integer, nullable=False)
    host = Column(String, nullable=True)
    turn = Column(String, nullable=True)
    timestamp = Column(Integer, nullable=True)
    password = Column(String, nullable=True)

    def __init__(self, name: str, size: int, host: str):
        self.gameid = str(uuid.uuid4())
        self.name = name
        self.size = size
        self.host = host

    def start_game(self):
        self.state = "playing"

    def get_host(self):
        return self.host
    
    def get_game_size(self):
        return self.size
    
    def set_game_password(self, password):
        self.password = password
    
    def get_game_password(self):
        return self.password