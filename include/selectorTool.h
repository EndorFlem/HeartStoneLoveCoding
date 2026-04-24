#pragma once
#include "hand.h"
#include "hero.h"
#include "minion.h"
#include "utilityclasses.h"
#include <cstddef>
#include <functional>
#include <unordered_set>

class Selector {};

class GameState;

using GameStateSelector = std::function<Character *(GameState *)>;
using GameStateWithIDSelector = std::function<Character *(GameState *, ID)>;

class IDSelector {
  static Character *getCharacter(ID id);
  static Minion *getMinion(ID id);
};

class HandSelector {
public:
  static Card *getCardByPosition(Hand &hand, std::size_t position) {
    if (position > hand.MAX_HAND_SIZE || position >= hand.size()) {
      return nullptr;
    }
    return hand.cards.at(position);
  }

  static std::size_t getFirstPositionByCardID(Hand &hand, ID id) {
    for (size_t i = 0; i < hand.cards.size(); i++) {
      if (hand.cards[i]->getID() == id)
        return i;
    }
    return hand.MAX_HAND_SIZE + 1;
  }

  static std::unordered_set<std::size_t> getPositionsByCardID(Hand &hand,
                                                              ID id) {
    std::unordered_set<std::size_t> result{};
    for (size_t i = 0; i < hand.cards.size(); i++) {
      if (hand.cards[i]->getID() == id)
        result.insert(i);
    }
    return result;
  }
};
class GameState;
class TargetSelector {
public:
  static Character *myHero(GameState *state);
  static Character *oppositeHero(GameState *state);
  static Character *opponentCharacterByID(ID id, GameState *state);
  static Character *myCharacterByID(ID id, GameState *state);

private:
  static Character *forHeroPower(HSClasses heroClass, Hero *currentHero,
                                 Hero *enemyHero) {
    switch (heroClass) {
    case HSClasses::WARRIOR:
    case HSClasses::SHAMAN:
    case HSClasses::DRUID:
    case HSClasses::PALADIN:
    case HSClasses::ROGUE:
    case HSClasses::WARLOCK:
      return currentHero;

    case HSClasses::HUNTER:
      return enemyHero;

    default:
      return nullptr;
    }

    return nullptr;
  }

  friend class GameState;
};