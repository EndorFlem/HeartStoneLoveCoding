#pragma once
#include "card.h"
#include "selectorTool.h"
#include "utilityclasses.h"
#include <functional>

class GameState;

class Spell : public Card {
public:

  std::function<void(GameState *state)> effect;
 
  std::function<void(GameState *state, Character *target)> targetedEffect;


  Spell(int cost, std::string name, HSClasses cardClass,
        std::function<void(GameState *state)> effect)
      : Card(cost, name, cardClass, CardType::Spell), effect(effect) {}


  Spell(int cost, std::string name, HSClasses cardClass,
        std::function<void(GameState *state, Character *target)> targetedEffect)
      : Card(cost, name, cardClass, CardType::Spell),
        effect(nullptr), targetedEffect(targetedEffect) {}

  bool isTargeted() const { return targetedEffect != nullptr; }

  Card *clone() override { return new Spell(*this); }
};