from main import *
import pytest
from numpy.testing import assert_equal


@pytest.fixture()
def prepare_game():
    game = Game()
    return game


def test_game_is_running(prepare_game):
    expected_running = True
    result = prepare_game
    assert expected_running == result.running


def test_loading_enemies_func(prepare_game):
    result = prepare_game
    expected_first = np.array([11, 13])
    expected_second = np.array([16, 13])
    expected_third = np.array([11, 15])
    expected_fouth = np.array([16, 15])

    assert_equal(result.enemies[0].grid_pos, expected_first)
    assert_equal(result.enemies[1].grid_pos, expected_second)
    assert_equal(result.enemies[2].grid_pos, expected_third)
    assert_equal(result.enemies[3].grid_pos, expected_fouth)


def test_loading_player_func(prepare_game):
    result = prepare_game
    expected = [1, 1]
    assert result.player.grid_pos == expected


def test_current_score(prepare_game):
    result = prepare_game
    result.run()
    expected = 0
    assert result.player.current_score == expected


def test_highscore(prepare_game):
    result = prepare_game
    expected = 0
    with open("highscore.txt", mode='r') as file:
        for line in file:
            if int(line) >= expected:
                expected = int(line)

    assert expected == result.highscore


def test_enemies_setup(prepare_game):
    result = prepare_game
    expected = ["speedy", "slow", "random", "scared"]
    result_list = []
    for enemy in result.enemies:
        result_list.append(enemy.personality)

    assert result_list == expected
