#pragma once
#include "utilityclasses.h"
#include <string>

enum class CardType { Minion, Spell, Weapon };

class Card {
public:
  virtual ~Card() = default;
  virtual Card *clone() = 0;
  Card(int cost, std::string name, HSClasses cardClass, CardType type)
      : cost(cost), name(name), cardClass(cardClass), type(type) {}

  void setID(ID id) { this->id = id; }
  ID getID() { return id; }
  std::string getName() { return name; }
  int getCost() { return cost; }

private:
  int cost;
  std::string name;
  HSClasses cardClass;
  CardType type;

  ID id;
};