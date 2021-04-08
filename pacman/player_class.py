import pygame
from settings import *


class Player:
    def __init__(self, game, pos):
        self.game = game
        self.grid_pos = pos
        self.pix_pos = self.get_pix_pos()
        self.direction = np.array([1, 0])
        self.stored_direction = np.array([0, 0])
        self.able_to_move = True
        self.current_score = 0
        self.speed = 2
        self.lives = 3

    def update(self):
        if self.able_to_move:
            self.pix_pos += self.direction * self.speed
        if self.time_to_move():
            if np.any(self.stored_direction):
                self.direction = self.stored_direction
            self.able_to_move = self.can_move()
        self.grid_pos[0] = (self.pix_pos[0] - TOP_BOTTOM_BUFFER) // CELL_WIDTH + 1
        self.grid_pos[1] = (self.pix_pos[1] - TOP_BOTTOM_BUFFER) // CELL_HEIGHT + 1

        if self.on_coin():
            self.eat_coin()

    def draw(self):
        pygame.draw.circle(self.game.screen, PLAYER_COLOUR, self.pix_pos, CELL_WIDTH - 12)
        # drawing player live
        for x in range(self.lives):
            pygame.draw.circle(self.game.screen, PLAYER_COLOUR, (30 + 10 * 2 * x, HEIGHT - 15), 7)

    def on_coin(self):
        if self.grid_pos in self.game.coins:
            if (self.pix_pos[0] + TOP_BOTTOM_BUFFER // 2) % CELL_WIDTH == 0:
                if np.array_equal(self.direction, np.array([1, 0])) or np.array_equal(self.direction,
                                                                                      np.array([-1, 0])):
                    return True
            if (self.pix_pos[1] + TOP_BOTTOM_BUFFER // 2) % CELL_HEIGHT == 0:
                if np.array_equal(self.direction, np.array([0, 1])) or np.array_equal(self.direction,
                                                                                      np.array([0, -1])):
                    return True
        else:
            return False

    def eat_coin(self):
        self.game.coins.remove(self.grid_pos)
        self.current_score += 1

    def move(self, direction):
        self.stored_direction = direction

    def get_pix_pos(self):
        return np.array([(self.grid_pos[0] * CELL_WIDTH) + TOP_BOTTOM_BUFFER // 2 + CELL_WIDTH // 2,
                         (self.grid_pos[1] * CELL_HEIGHT) + TOP_BOTTOM_BUFFER // 2 + CELL_HEIGHT // 2])

    def time_to_move(self):
        if (self.pix_pos[0] + TOP_BOTTOM_BUFFER // 2) % CELL_WIDTH == 0:
            if np.array_equal(self.direction, np.array([1, 0])) or np.array_equal(self.direction,
                                                                                  np.array([-1, 0])) or np.array_equal(
                    self.direction, np.array([0, 0])):
                return True
        if (self.pix_pos[1] + TOP_BOTTOM_BUFFER // 2) % CELL_HEIGHT == 0:
            if np.array_equal(self.direction, np.array([0, 1])) or np.array_equal(self.direction,
                                                                                  np.array([0, -1])) or np.array_equal(
                    self.direction, np.array([0, 0])):
                return True
        else:
            return False

    def can_move(self):
        for wall in self.game.walls:
            if np.array_equal(self.grid_pos + self.direction, wall):
                return False
        return True
