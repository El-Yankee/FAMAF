import factory
import uuid
import factory.random
from models.game_models import session
from models.player_models import Player

class PlayerFactory(factory.alchemy.SQLAlchemyModelFactory):
    class Meta:
        model = Player
        sqlalchemy_session = session

    name = factory.Sequence(lambda n: f'Player {n}')
    
    @factory.post_generation
    def assign_playerid(self, create, extracted, **kwargs):
        if not self.playerid:
            self.playerid = str(uuid.uuid4())
    