#include "game.h"
#include "board.h"
#include "minion.h"
#include "utilityclasses.h"
#include <cstddef>
#include <iostream>

GameState &Game::start() { return gameState; }