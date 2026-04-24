#pragma once

#include "gameState.h"
#include "selectorTool.h"
#include <cstddef>
inline void firstTest(GameState &gameState) {
  gameState.startTurn().endTurn();                       // первый ход 1 игрока
  gameState.startTurn().playCardByPosition(0).endTurn(); // альтернативный ход

  // gameState.startTurn().endTurn(); // первый ход 2 игрока

  // gameState.startTurn()
  //     .useHeroPower()
  //     .attackWithHero(TargetSelector::oppositeHero)
  //     // .attack(TargetSelector::myCharacterByID,
  //     TargetSelector::oppositeHero) .endTurn(); // втьрой ход 1 игрока

  // gameState.startTurn().useHeroPower().endTurn(); // второй ход 2 игрока

  // gameState.startTurn()
  //     .attackWithHero(TargetSelector::oppositeHero)
  //     .useHeroPower()
  //     .endTurn(); // тетрий ход 1 игрока

  // gameState.startTurn().useHeroPower().endTurn(); // третий ход 2 игрока

  gameState.printBoard();
  gameState.printState();
}
void secondTest();