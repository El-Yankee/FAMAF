import factory
import uuid
import factory.random
from models.game_models import Game, session

class GameFactory(factory.alchemy.SQLAlchemyModelFactory):
    class Meta:
        model = Game
        sqlalchemy_session = session

    name = factory.Sequence(lambda n: f'Game {n}')
    size = 3
    host = factory.LazyFunction(lambda: str(uuid.uuid4()))
    
    @factory.post_generation
    def assign_gameid(self, create, extracted, **kwargs):
        if not self.gameid:
            self.gameid = str(uuid.uuid4())
        if not self.state:
            self.state = 'waiting'
        if not self.turn:
            self.turn = None
            
            
    