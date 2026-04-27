#pragma once
#include "attributes.h"
#include "card.h"
#include "utilityclasses.h"
#include <vector>

class GameState;

class Weapon : public Card, public Attributable {
public:
  int durability;

  Weapon(int cost, std::string name, HSClasses cardClass, int attack,
         int durability, std::vector<Attribute *> attrs)
      : Card(cost, name, cardClass, CardType::Weapon), Attributable(attrs),
        durability(durability), attack(attack) {}

  int getAttack() { return attack; }

  bool reduceDurability(GameState *state) {
    durability -= 1;
    if (!durability) {
      triggerOnDeath(state);
      return false;
    }
    return true;
  }


  bool reduceDurability() {
    durability -= 1;
    return durability > 0;
  }

  Card *clone() override { return new Weapon(*this); }

private:
  int attack;
};
