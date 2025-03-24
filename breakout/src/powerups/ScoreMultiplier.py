import time
from typing import TypeVar
from src.powerups.PowerUp import PowerUp

class ScoreMultiplier(PowerUp):
    def __init__(self, x: int, y: int) -> None:
        super().__init__(x, y, 4)  # Ajusta el frame si es necesario
        self.duration = 10  # Duración del efecto en segundos

    def take(self, play_state: TypeVar("PlayState")) -> None:
        play_state.score_multiplier = 10  # Duplica los puntos obtenidos
        play_state.multiplier_end_time = time.time() + self.duration  # Establece el tiempo de expiración
        self.active = False
