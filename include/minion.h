#pragma once

#include "attributes.h"
#include "card.h"
#include "utilityclasses.h"
#include <string>
#include <vector>

enum class MinionType {
  Beast,
  Demon,
  Dragon,
  Murloc,
  Pirate,
  Totem,
  Elemental
};

class Minion : public Card,
               public Character,
               public DamageDealer,
               public Attributable {
public:
  Minion(int cost, std::string name, HSClasses cardClass, int health,
         int attack)
      : Card(cost, name, cardClass, CardType::Minion), Character(health),
        DamageDealer(attack), Attributable(std::vector<Attribute *>{}) {}

  Minion(int cost, std::string name, HSClasses cardClass, int health,
         int attack, std::vector<Attribute *> attributes)
      : Card(cost, name, cardClass, CardType::Minion), Character(health),
        DamageDealer(attack), Attributable(attributes) {}

  Card *clone() override { return new Minion(*this); }
  int getAttack() override { return attack; }

private:
  int attack;
  MinionType type;
};