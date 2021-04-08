import numpy as np

# screen settings

WIDTH = 610
HEIGHT = 670
FPS = 60
TOP_BOTTOM_BUFFER = 50
MAZE_WIDTH = WIDTH - TOP_BOTTOM_BUFFER
MAZE_HEIGHT = HEIGHT - TOP_BOTTOM_BUFFER
COLS = 28
ROWS = 30
CELL_WIDTH = MAZE_WIDTH // COLS
CELL_HEIGHT = MAZE_HEIGHT // ROWS

# colour settings
BLACK = np.array([0, 0, 0])
WHITE = np.array([255, 255, 255])
GREY = np.array([107, 107, 107])
ORANGE = np.array([170, 132, 58])
CYAN = np.array([30, 137, 156])
RED = np.array([255, 0, 0])
GREY = np.array([190, 190, 190])
BLUE = np.array([43, 78, 203])
DARK_YELLOW = np.array([197, 200, 27])

# font settings

START_TEXT_SIZE = 20
START_FONT = 'arial black'

# player settings
PLAYER_COLOUR = np.array([255, 251, 13])

