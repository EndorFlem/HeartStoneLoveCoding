#pragma once
#include "card.h"
#include "utilityclasses.h"
#include <functional>

class Board;

class Spell : public Card {
public:
  std::function<void(Board *board, Character *character)> effect;
  Spell(int cost, std::string name, HSClasses cardClass,
        std::function<void(Board *board, Character *target)> effect)
      : Card(cost, name, cardClass, CardType::Spell), effect(effect) {}
  Card *clone() override { return new Spell(*this); }
};