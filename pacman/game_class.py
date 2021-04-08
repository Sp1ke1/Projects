from enemy_class import *
from player_class import *

pygame.init()


class Game:
    def __init__(self):
        self.screen = pygame.display.set_mode((WIDTH, HEIGHT))
        self.clock = pygame.time.Clock()
        self.running = True
        self.state = "start"
        self.enemies = []
        self.e_pos = []
        self.p_pos = None
        self.highscore = 0
        self.load()
        self.player = Player(self, self.p_pos)
        self.make_enemies()

    def run(self):
        while self.running:
            if self.state == "start":
                self.start_events()
                self.start_update()
                self.start_draw()
            elif self.state == "playing":
                self.playing_events()
                self.playing_update()
                self.playing_draw()
            elif self.state == "game over":
                self.game_over_events()
                self.game_over_update()
                self.game_over_draw()
            elif self.state == 'win':
                self.game_over_win_events()
                self.game_over_win_update()
                self.game_over_win_draw()
            self.clock.tick(FPS)
        pygame.quit()

    # HELPER FUNCTIONS
    def draw_text(self, text, screen, pos,
                  size, colour, font_name, centered=False):
        font = pygame.font.SysFont(font_name, size)
        text = font.render(text, False, colour)
        text_size = text.get_size()
        if centered:
            pos[0] = pos[0] - text_size[0] // 2
            pos[1] = pos[1] - text_size[1] // 2
        screen.blit(text, pos)

    def load(self):
        self.background = pygame.image.load("background.png")
        self.background = pygame.transform.scale(self.background, (MAZE_WIDTH, MAZE_HEIGHT))
        self.walls = []
        self.coins = []
        self.set_high_score()
        with open("walls.txt", mode='r') as file:
            for yidx, line in enumerate(file):
                for xidx, char in enumerate(line):
                    if char == "1":
                        self.walls.append(np.array([xidx, yidx]))
                    elif char == "P":
                        self.p_pos = [xidx, yidx]
                        self.player_reset_pos = [xidx, yidx]
                    elif char == "C":
                        self.coins.append([xidx, yidx])
                    elif char in ["2", "3", "4", "5"]:
                        self.e_pos.append(np.array([xidx, yidx]))
                    elif char == "B":
                        pygame.draw.rect(self.background, BLACK, (xidx * CELL_WIDTH, yidx * CELL_HEIGHT,
                                                                  CELL_WIDTH, CELL_HEIGHT))
        self.walls = np.asarray(self.walls)

    def draw_grid(self):
        for x in range(WIDTH // CELL_WIDTH):
            pygame.draw.line(self.background, GREY, (x * CELL_WIDTH, 0), (x * CELL_WIDTH, HEIGHT))
        for x in range(HEIGHT // CELL_HEIGHT):
            pygame.draw.line(self.background, GREY, (0, x * CELL_HEIGHT), (WIDTH, x * CELL_HEIGHT))

    def make_enemies(self):
        for idx, pos in enumerate(self.e_pos):
            self.enemies.append(Enemy(self, pos, idx))

    def reset(self):
        self.player.lives = 3
        self.player.grid_pos[0] = self.player_reset_pos[0]
        self.player.grid_pos[1] = self.player_reset_pos[1]
        self.player.pix_pos = self.player.get_pix_pos()
        self.player.direction *= 0
        for enemy in self.enemies:
            enemy.grid_pos[0] = enemy.reset_position[0]
            enemy.grid_pos[1] = enemy.reset_position[1]
            enemy.pix_pos = enemy.get_pix_pos()
            enemy.direction *= 0

        with open("walls.txt", mode='r') as file:
            self.coins = []
            for yidx, line in enumerate(file):
                for xidx, char in enumerate(line):
                    if char == 'C':
                        self.coins.append([xidx, yidx])
        self.player.current_score = 0
        self.state = 'playing'

    def set_high_score(self):
        with open("highscore.txt", mode='r') as highscore:
            for line in highscore:
                if int(line) >= self.highscore:
                    self.highscore = int(line)

    # INTRO FUNCTIONS

    def start_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                self.state = 'playing'

    def start_update(self):
        pass

    def start_draw(self):
        self.screen.fill(BLACK)
        self.draw_text('PUSH SPACE BAR', self.screen, np.array([WIDTH // 2, HEIGHT // 2]), START_TEXT_SIZE, ORANGE,
                       START_FONT, centered=True)
        self.draw_text('1 PLAYER ONLY', self.screen, np.array([WIDTH // 2, HEIGHT // 2 + 50]), START_TEXT_SIZE, CYAN,
                       START_FONT, centered=True)
        self.draw_text('HIGH SCORE {}'.format(self.highscore), self.screen, np.array([100, 10]), START_TEXT_SIZE, WHITE,
                       START_FONT, START_FONT)
        pygame.display.update()

    # PLAYING FUNCTIONS

    def playing_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.state = 'start'
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    self.player.move(np.array([-1, 0]))
                if event.key == pygame.K_RIGHT:
                    self.player.move(np.array([1, 0]))
                if event.key == pygame.K_UP:
                    self.player.move(np.array([0, -1]))
                if event.key == pygame.K_DOWN:
                    self.player.move(np.array([0, 1]))

    def playing_update(self):
        self.player.update()
        for enemy in self.enemies:
            enemy.update()

        for enemy in self.enemies:
            if np.array_equal(enemy.grid_pos, self.player.grid_pos):
                self.remove_live()
        if self.coins == []:
            self.state = "win"

    def playing_draw(self):
        self.screen.fill(BLACK)
        self.screen.blit(self.background, (TOP_BOTTOM_BUFFER // 2, TOP_BOTTOM_BUFFER // 2))
        self.draw_coins()
        # self.draw_grid()
        self.draw_text('CURRENT SCORE: {}'.format(self.player.current_score), self.screen, np.array([125, 10]),
                       START_TEXT_SIZE, WHITE, START_FONT, START_FONT)
        self.draw_text('HIGH SCORE: {}'.format(self.highscore), self.screen, np.array([WIDTH // 2 + 120, 10]),
                       START_TEXT_SIZE, WHITE, START_FONT, START_FONT)
        self.player.draw()
        for enemy in self.enemies:
            enemy.draw()
        pygame.display.update()

    def remove_live(self):
        self.player.lives -= 1
        if self.player.lives == 0:
            self.state = 'game over'
            with open("highscore.txt", mode='a', encoding='utf8') as file:
                file.write('{}\n'.format(self.player.current_score))
            self.set_high_score()

        else:
            self.player.grid_pos[0] = self.player_reset_pos[0]
            self.player.grid_pos[1] = self.player_reset_pos[1]
            self.player.pix_pos = self.player.get_pix_pos()
            self.player.direction *= 0
            for enemy in self.enemies:
                enemy.grid_pos[0] = enemy.reset_position[0]
                enemy.grid_pos[1] = enemy.reset_position[1]
                enemy.pix_pos = enemy.get_pix_pos()
                enemy.direction *= 0

    def draw_coins(self):
        for coin in self.coins:
            pygame.draw.circle(self.screen, (124, 123, 7),
                               ((coin[0] * CELL_WIDTH) + TOP_BOTTOM_BUFFER // 2 + CELL_WIDTH // 2,
                                (coin[1] * CELL_HEIGHT) + TOP_BOTTOM_BUFFER // 2 + CELL_HEIGHT // 2), 5)

    # GAME OVER FUNCTIONS

    def game_over_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                self.reset()

    def game_over_update(self):
        pass

    def game_over_draw(self):
        self.screen.fill(BLACK)
        quit_text = "Press the escape button to QUIT"
        again_text = "Press SPACE bar to PLAY AGAIN"
        current_score_text = 'Your score is: {}'.format(self.player.current_score)
        self.draw_text("GAME OVER! YOU LOST!", self.screen, [WIDTH // 2, 100], 40, RED, "arial", centered=True)
        self.draw_text(again_text, self.screen,
                       [WIDTH // 2, HEIGHT // 2], 30, GREY, "arial", centered=True)
        self.draw_text(quit_text, self.screen,
                       [WIDTH // 2, HEIGHT // 1.5], 30, GREY, "arial", centered=True)
        self.draw_text(current_score_text, self.screen, [WIDTH // 2, HEIGHT // 1.25], 30, GREY, "arial",
                       centered=True)
        pygame.display.update()

    def game_over_win_events(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                self.running = False
            if event.type == pygame.KEYDOWN and event.key == pygame.K_SPACE:
                self.reset()

    def game_over_win_update(self):
        pass

    def game_over_win_draw(self):
        self.screen.fill(BLACK)
        quit_text = "Press the escape button to QUIT"
        again_text = "Press SPACE bar to PLAY AGAIN"
        current_score_text = 'Your score is: {}'.format(self.player.current_score)
        with open("highscore.txt", mode='a', encoding='utf8') as file:
            file.write('{}\n'.format(self.player.current_score))
        self.draw_text("COGRADULATIONS! YOU WON!", self.screen, [WIDTH // 2, 100], 40, RED, "arial", centered=True)
        self.draw_text(again_text, self.screen,
                       [WIDTH // 2, HEIGHT // 2], 30, GREY, "arial", centered=True)
        self.draw_text(quit_text, self.screen,
                       [WIDTH // 2, HEIGHT // 1.5], 30, GREY, "arial", centered=True)
        self.draw_text(current_score_text, self.screen, [WIDTH // 2, HEIGHT // 1.25], 30, GREY, "arial",
                       centered=True)

        pygame.display.update()
