import pygame
import numpy as np
from settings import *
import random


class Enemy():
    def __init__(self, game, pos, number):
        self.number = number
        self.game = game
        self.grid_pos = pos
        self.reset_position = np.array([self.grid_pos[0], self.grid_pos[1]])
        self.pix_pos = self.get_pix_pos()
        self.radius = CELL_WIDTH // 2.5
        self.colour = self.set_colour()
        self.direction = np.array([0, 0])
        self.personality = self.set_personality()
        self.target = None
        self.speed = self.set_speed()

    def update(self):
        self.target = self.set_target()
        if not np.array_equal(self.target, self.grid_pos):
            self.pix_pos[0] += self.direction[0] * self.speed
            self.pix_pos[1] += self.direction[1] * self.speed

            if self.time_to_move():
                self.move()

        self.grid_pos[0] = (self.pix_pos[0] - TOP_BOTTOM_BUFFER) // CELL_WIDTH + 1
        self.grid_pos[1] = (self.pix_pos[1] - TOP_BOTTOM_BUFFER) // CELL_HEIGHT + 1

    def draw(self):
        pygame.draw.circle(self.game.screen, self.colour, self.pix_pos, self.radius)

    def set_target(self):
        if self.personality == 'speedy' or self.personality == 'slow':
            return self.game.player.grid_pos
        else:
            if self.game.player.grid_pos[0] > COLS // 2 and self.game.player.grid_pos[1] > ROWS // 2:
                return np.array([1, 1])
            elif self.game.player.grid_pos[0] > COLS // 2 and self.game.player.grid_pos[1] < ROWS // 2:
                return np.array([1, ROWS - 2])
            elif self.game.player.grid_pos[0] < COLS // 2 and self.game.player.grid_pos[1] > ROWS // 2:
                return np.array([COLS - 2, 1])
            else:
                return np.array([COLS - 2, ROWS - 2])

    def set_speed(self):
        if self.personality in ["speedy", "scared"]:
            speed = 2
        else:
            speed = 1
        return speed

    def time_to_move(self):
        if (self.pix_pos[0] + TOP_BOTTOM_BUFFER // 2) % CELL_WIDTH == 0:
            if np.array_equal(self.direction, np.array([1, 0])) or np.array_equal(self.direction,
                              np.array([-1, 0])) or np.array_equal(self.direction, np.array([0, 0])):
                return True
        if (self.pix_pos[1] + TOP_BOTTOM_BUFFER // 2) % CELL_HEIGHT == 0:
            if np.array_equal(self.direction, np.array([0, 1])) or np.array_equal(self.direction,
                              np.array([0, -1])) or np.array_equal(self.direction, np.array([0, 0])):
                return True
        else:
            return False

    def move(self):
        if self.personality == "random":
            self.direction = self.get_random_direction()
        if self.personality == "slow":
            self.direction = self.get_path_direction(self.target)
        if self.personality == "speedy":
            self.direction = self.get_path_direction(self.target)
        if self.personality == "scared":
            self.direction = self.get_path_direction(self.target)

    def get_path_direction(self, target):
        next_cell = self.find_next_cell_in_path(target)
        xdir = next_cell[0] - self.grid_pos[0]
        ydir = next_cell[1] - self.grid_pos[1]
        return np.array([xdir, ydir])

    def find_next_cell_in_path(self, target):
        path = self.bfs([self.grid_pos[0], self.grid_pos[1]], [target[0], target[1]])
        return path[1]

    def bfs(self, start, target):
        grid = [[0 for x in range(28)] for x in range(30)]
        for cell in self.game.walls:
            if cell[0] < 28 and cell[1] < 30:
                grid[cell[1]][cell[0]] = 1
        # print(grid)
        queue = [start]
        path = []
        visited = []
        while queue:
            current = queue[0]
            queue.remove(queue[0])
            visited.append(current)
            if current == target:
                break
            else:
                neighbours = [[0, -1], [1, 0], [0, 1], [-1, 0]]
                for neighbour in neighbours:
                    if neighbour[0] + current[0] >= 0 and neighbour[0] + current[0] < len(grid[0]):
                        if neighbour[1] + current[1] >= 0 and neighbour[1] + current[1] < len(grid):
                            next_cell = [neighbour[0] + current[0], neighbour[1] + current[1]]
                            if next_cell not in visited:
                                if grid[next_cell[1]][next_cell[0]] != 1:
                                    queue.append(next_cell)
                                    path.append({"Current": current, "Next": next_cell})

        shortest = [target]
        while target != start:
            for step in path:
                if step["Next"] == target:
                    target = step["Current"]
                    shortest.insert(0, step["Current"])
        return shortest

    def get_pix_pos(self):
        return np.array([(self.grid_pos[0] * CELL_WIDTH) + TOP_BOTTOM_BUFFER // 2 + CELL_WIDTH // 2,
                         (self.grid_pos[1] * CELL_HEIGHT) + TOP_BOTTOM_BUFFER // 2 + CELL_HEIGHT // 2])

    def set_colour(self):
        if self.number == 0:
            return BLUE
        elif self.number == 1:
            return DARK_YELLOW
        elif self.number == 2:
            return RED
        elif self.number == 3:
            return ORANGE

    def set_personality(self):
        if self.number == 0:
            return "speedy"
        if self.number == 1:
            return "slow"
        if self.number == 2:
            return "random"
        else:
            return "scared"

    def get_random_direction(self):

        while True:
            number = random.randint(0, 3)
            if number == 0:
                x_dir, y_dir = 1, 0
            elif number == 1:
                x_dir, y_dir = 0, 1
            elif number == 2:
                x_dir, y_dir = -1, 0
            else:
                x_dir, y_dir = 0, -1
            x_y_dir = np.array([x_dir, y_dir])
            next_pos = np.array([self.grid_pos[0] + x_dir, self.grid_pos[1] + y_dir])
            FOUND = 0
            for wall in self.game.walls:
                if np.array_equal(next_pos, wall):
                    FOUND = 1
                    break
            if FOUND == 0:
                break
        return x_y_dir
