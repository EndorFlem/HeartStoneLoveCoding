#include "selectorTool.h"
#include "gameState.h"

Character *TargetSelector::myHero(GameState *state) {
  return const_cast<Character *>(
      static_cast<const Character *>(state->getCurrentHero()));
}
Character *TargetSelector::oppositeHero(GameState *state) {
  return const_cast<Character *>(
      static_cast<const Character *>(state->getOppositeHero()));
}
Character *TargetSelector::opponentCharacterByID(ID id, GameState *state) {
  if (id.type == IDType::Hero) {
    return oppositeHero(state);
  }
  for (auto minion : *state->board->enemyMinions) {
    if (minion->getID() == id) {
      return minion;
    }
  }
  return nullptr;
}
Character *TargetSelector::myCharacterByID(ID id, GameState *state) {
  if (id.type == IDType::Hero) {
    return myHero(state);
  }
  for (auto minion : *state->board->myMinions) {
    if (minion->getID() == id) {
      return minion;
    }
  }
  return nullptr;
}