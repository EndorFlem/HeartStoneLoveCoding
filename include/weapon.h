#pragma once
#include "attributes.h"
#include "card.h"
#include "utilityclasses.h"
#include <vector>
class Weapon : public Card, public Attributable {
public:
  int durability;
  Weapon(int cost, std::string name, HSClasses cardClass, int attack,
         int durability, std::vector<Attribute *> attrs)
      : Card(cost, name, cardClass, CardType::Weapon), Attributable(attrs),
        durability(durability), attack(attack) {}

  int getAttack() { return attack; }

  bool reduceDurability() {
    durability -= 1;
    if (!durability) {
      if (containsDeathratle()) {
        triggerDeathratles();
      }

      return false;
    }

    return true;
  }

  Card *clone() override { return new Weapon(*this); }

private:
  int attack;
};