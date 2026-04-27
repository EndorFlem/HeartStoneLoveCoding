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
Character *TargetSelector::opponentCharacterByID(GameState *state, ID id) {
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
Character *TargetSelector::myCharacterByID(GameState *state, ID id) {
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

Character *TargetSelector::opponentMinionByPosition(GameState *state, size_t position) {
  BoardSide *side = state->board->enemyMinions;
  if (position >= side->size()) {
    return nullptr;
  }
  return side->at(position);
}

Character *TargetSelector::myMinionByPosition(GameState *state, size_t position) {
  BoardSide *side = state->board->myMinions;
  if (position >= side->size()) {
    return nullptr;
  }
  return side->at(position);
}

std::vector<Character *> TargetSelector::allOpponentMinions(GameState *state) {
  std::vector<Character *> result;
  for (Minion *m : *state->board->enemyMinions) {
    result.push_back(m);
  }
  return result;
}

std::vector<Character *> TargetSelector::allMyMinions(GameState *state) {
  std::vector<Character *> result;
  for (Minion *m : *state->board->myMinions) {
    result.push_back(m);
  }
  return result;
}

std::vector<Character *> TargetSelector::allOpponentCharacters(GameState *state) {
  std::vector<Character *> result = allOpponentMinions(state);
  result.push_back(oppositeHero(state));
  return result;
}

std::vector<Character *> TargetSelector::allCharacters(GameState *state) {
  std::vector<Character *> result = allMyMinions(state);
  result.push_back(myHero(state));
  std::vector<Character *> opponents = allOpponentCharacters(state);
  result.insert(result.end(), opponents.begin(), opponents.end());
  return result;
}