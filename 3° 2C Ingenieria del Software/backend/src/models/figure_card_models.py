from sqlalchemy import Column, Integer, String, Boolean, ForeignKey, create_engine
from sqlalchemy.ext.declarative import declarative_base
from models.game_models import Game, engine, Base, session
from models.player_models import Player, PlayerGame
import random
import uuid
from sqlalchemy.exc import IntegrityError

class Figure_card(Base):
    __tablename__ = 'Figure_cards'
    id = Column(String, primary_key=True)
    gameid = Column(String, ForeignKey('games.gameid'), nullable=False)
    playerid = Column(String, ForeignKey('players.playerid'), nullable=False)
    figure = Column(Integer, primary_key=True, nullable=False)
    in_hand = Column(Boolean, default=False)
    state = Column(String, default="active")

    def __init__(self,game_id,player_id,figure):
        self.id = str(uuid.uuid4())
        self.gameid = game_id
        self.playerid = player_id
        self.figure = figure

    def take_card(self):
        self.in_hand = True

    def return_card(self):
        session.delete(self)

    def block_card(self):
        self.state = "blocked"
    
    def get_state(self):
        return self.state

    def unblock_card(self):
        self.state = "marked"


def has_blocked_card(game, player):
    return session.query(Figure_card).filter_by(gameid=game, playerid=player, state="blocked").count() > 0

def has_marked_card(game, player):
    return session.query(Figure_card).filter_by(gameid=game, playerid=player, state="marked").count() > 0

def shuffle(game):
    easy_figures = list(range(1, 8))*2
    hard_figures = list(range(8, 26))*2
    random.shuffle(easy_figures)  # Mezclar las figuras
    random.shuffle(hard_figures) 
 
    players = session.query(PlayerGame).filter_by(gameid=game).all()
    num_players = len(players)
    easy_fig_per_player = len(easy_figures) // num_players
    hard_fig_per_player = len(hard_figures) // num_players

    for i in range(num_players):  # Repartir
        player = players[i]
        for j in range(easy_fig_per_player):
            figure_card = Figure_card(game, player.playerid, easy_figures.pop())
            session.add(figure_card)

        
        for j in range(hard_fig_per_player):
            figure_card = Figure_card(game, player.playerid, hard_figures.pop())
            session.add(figure_card)

    try:
        session.commit()
    except IntegrityError as e:
        session.rollback()
        print(f"Error al insertar Figure_card: {e}")

def take_cards(game, player):
    # Determinar cuántas cartas necesita el jugador para tener 3 en mano
    num_cards_in_hand = session.query(Figure_card).filter_by(gameid=game, playerid=player, in_hand=True).count()
    cards_needed = max(0, 3 - num_cards_in_hand)  # Ensure cards_needed is not negative

    if cards_needed > 0:
        # Obtener todas las cartas disponibles que no están en mano
        available_cards = session.query(Figure_card).filter_by(gameid=game, playerid=player, in_hand=False).all()
        
        # Seleccionar aleatoriamente las cartas necesarias
        cards = random.sample(available_cards, min(cards_needed, len(available_cards)))

        # Marcar las cartas como tomadas (en mano) y guardarlas en la base de datos
        for card in cards:
            card.take_card()  # Este método debe marcar la carta como "en mano"

        # Guardar los cambios en la base de datos
        try:
            session.commit()
        except Exception as e:  # Capturar una excepción más específica
            session.rollback()
            raise Exception(f"Error al tomar las cartas: {e}")


# Create the table
Base.metadata.create_all(bind=engine)