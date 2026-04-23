#pragma once
#include "attributes.h"
#include "card.h"
#include "utilityclasses.h"
#include <vector>
class Weapon : public Card, DamageDealer, public Attributable {
public:
  int durability;
  Weapon(int cost, std::string name, HSClasses cardClass, int attack,
         int durability, std::vector<Attribute *> attrs)
      : Card(cost, name, cardClass, CardType::Weapon), DamageDealer(attack),
        Attributable(attrs), durability(durability) {}

  int getAttack() override { return attack; }

  Card *clone() override { return new Weapon(*this); }
};