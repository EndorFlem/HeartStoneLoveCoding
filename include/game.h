#pragma once

#include "board.h"
#include "gameState.h"
#include "hero.h"
#include "utilityclasses.h"
class Game {
public:
  Game(Player *me, Player *enemy, Board *board)
      : me(me), enemy(enemy), board(board),
        gameState(std::array<Player *, 2>{me, enemy}, board) {}

  GameState &start();

private:
  Player *me;
  Player *enemy;
  Board *board;
  GameState gameState;

  bool isRunning = true;

  Character *selectHeroPowerTarget(Player *player) const;
};
