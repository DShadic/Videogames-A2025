import random
from typing import TypeVar

import settings
from src.Ball import Ball
from src.powerups.PowerUp import PowerUp


class StickyPaddle(PowerUp):
    """
    Power-up to add two more ball to the game.
    """

class StickyPaddle(PowerUp):
    def __init__(self, x: int, y: int) -> None:
        super().__init__(x, y, 3)  # 3 = frame/textura del power-up
   
    def take(self, play_state: TypeVar("PlayState")) -> None:
        play_state.paddle.sticky = True  # Activa el modo "pegajoso"

        self.active = False