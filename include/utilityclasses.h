#pragma once

struct Damage {
  int amount;
  bool isPoison;
};

class Character;

class DamageDealer {
public:
  DamageDealer(int attack) : attack(attack) {}
  virtual ~DamageDealer() = default;
  virtual void dealDamage(Damage damage, Character *target);

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

  int getAttack() override { return attack; }

private:
  int health;
};

inline void DamageDealer::dealDamage(Damage damage, Character *target) {
  target->takeDamage(damage);
}

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
