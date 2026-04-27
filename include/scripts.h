#pragma once

#include "gameState.h"
#include "minion.h"
#include "selectorTool.h"
#include "utilityclasses.h"
#include <cstddef>
inline void firstTest(GameState &gameState) {
  gameState.startTurn().endTurn(); // первый ход 1 игрока
  // gameState.startTurn().playCardByPosition(0).endTurn(); // альтернативный
  // ход

  gameState.startTurn().playCardByPosition(0).endTurn(); // первый ход 2
  // игрока

  gameState.startTurn()
      .useHeroPower()
      .attackWithHero(TargetSelector::opponentCharacterByID,
                      {IDType::Minion, 6})
      .endTurn();

  // gameState.startTurn().useHeroPower().endTurn(); // второй ход 2 игрока

  // gameState.startTurn()
  //     .attackWithHero(TargetSelector::oppositeHero)
  //     .useHeroPower()
  //     .endTurn(); // тетрий ход 1 игрока

  // gameState.startTurn().useHeroPower().endTurn(); // третий ход 2 игрока

  gameState.printBoard();
  gameState.printState();
}
inline void secondTest(GameState &gameState) {
  gameState.startTurn().printIDs().endTurn();

  gameState.startTurn().playCardByPosition(0).endTurn();

  gameState.startTurn()
      .useHeroPower()
      .attackWithHero(TargetSelector::opponentCharacterByID,
                      {IDType::Minion, 6})
      .endTurn();

  gameState.printBoard();
}

inline void dealDamageTest(GameState &gameState) {
  gameState.startTurn().dealDamage(TargetSelector::myHero, 26).endTurn();

  gameState.startTurn().playCardByPosition(0).endTurn();
  gameState.startTurn()
      .useHeroPower()
      .attackWithHero(TargetSelector::opponentCharacterByID,
                      {IDType::Minion, 6})
      .endTurn();
  gameState.printBoard().printState();
  gameState.startTurn().endTurn();
  gameState.printState();
}