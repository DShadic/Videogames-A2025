import random
from typing import TypeVar

import settings
from src.Ball import Ball
from src.powerups.PowerUp import PowerUp

class ScoreMultiplier(PowerUp):
    def __init__(self, x: int, y: int) -> None:
        super().__init__(x, y, 1)  
        

    def take(self, play_state: TypeVar("PlayState")) -> None:
        play_state.score_multiplier = 4  
        play_state.score_timer = 0
        self.active = False