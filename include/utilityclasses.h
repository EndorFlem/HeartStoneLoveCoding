#pragma once

#include <cstddef>
#include <iostream>
enum class IDType { Card, Minion, Weapon, Hero };

struct ID {
  IDType type;
  std::size_t number;

  bool operator==(const ID &other) {
    return other.type == type && other.number == number;
  }

  bool operator!=(const ID &other) { return !operator==(other); }
};

struct Damage {
  int amount;
  bool isPoison;
};

class Character;

class DamageDealer {
public:
  virtual ~DamageDealer() = default;
  virtual void dealDamage(Character *target) = 0;

  DamageDealer(int attack) : attack(attack) {}

  virtual int getAttack() = 0;
  int attack;
};

class Character : public DamageDealer {
public:
  virtual ~Character() = default;
  Character(int health, int attack) : DamageDealer(attack), health(health) {}
  Character(int health) : DamageDealer(0), health(health) {}
  virtual void takeDamage(Damage damage) { health -= damage.amount; }
  int getHealth() { return health; }

  void dealDamage(Character *target) override {

    target->takeDamage({getAttack(), false});
  }

  int getAttack() override { return attack; }

protected:
  int health;
};

enum class HSClasses {
  WARRIOR,
  MAGE,
  ROGUE,
  PRIEST,
  HUNTER,
  SHAMAN,
  DRUID,
  WARLOCK,
  PALADIN
};
