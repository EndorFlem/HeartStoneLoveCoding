#pragma once

#include <cstddef>
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

  DamageDealer(int attack) : attack(attack), maxAttack(attack) {}

  virtual int getAttack() = 0;
  int attack;
  int maxAttack;
};

class GameState;

class Character : public DamageDealer {
public:
  virtual ~Character() = default;
  Character(int health, int attack) : DamageDealer(attack), health(health), maxHealth(health) {}
  Character(int health) : DamageDealer(0), health(health), maxHealth(health) {}
  virtual void takeDamage(Damage damage) { health -= damage.amount; }
  int getHealth() { return health; }
  int getMaxHealth() { return maxHealth; }
  bool isDead() { return health <= 0; }
  bool isFullHealth() { return health >= maxHealth; }

  bool canAttack() { return !exhausted && !summoningSickness; }
  void setExhausted() { exhausted = true; }
  bool isExhausted() { return exhausted; }
  bool hasSummoningSickness() { return summoningSickness; }
  void onTurnStart() {
    exhausted = false;
    summoningSickness = false;
  }

  void setSummoningSickess(bool newState) {
    summoningSickness = newState;
  }
  void onSummoned() { summoningSickness = true; }

  void dealDamage(Character *target) override {
    target->takeDamage({getAttack(), false});
  }

  int getAttack() override { return attack; }

protected:
  int health;
  int maxHealth;
  bool exhausted = false;
  bool summoningSickness = false;

  friend class GameState;
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
