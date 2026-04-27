#pragma once

#include "attributes.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <string>

class Board;
class HeroPower {
private:
  using HeroPowerFunction =
      std::function<void(Board *board, Character *target, GameState *state)>;
  static const std::array<HeroPowerFunction, 9> heroPowers;

  size_t cost;
  bool active = true;
  HeroPowerFunction heroPower;

public:
  HeroPower(HSClasses heroClass);
  void activate(Board *board, Character *target, GameState *state);
  size_t getCost() { return cost; }
  bool isActive() { return active; }

private:
  static void warriorPower(Board *board, Character *target,GameState *state);
  static void magePower(Board *board, Character *target, GameState *state);
  static void roguePower(Board *board, Character *target, GameState *state);
  static void priestPower(Board *board, Character *target, GameState *state);
  static void hunterPower(Board *board, Character *target, GameState *state);
  static void shamanPower(Board *board, Character *target, GameState *state);
  static void druidPower(Board *board, Character *target, GameState *state);
  static void warlockPower(Board *board, Character *target, GameState *state);
  static void paladinPower(Board *board, Character *target, GameState *state);

  friend class Player;
};

class Hero : public Character {
public:
  int armor;
  int attack = 0;
  HSClasses heroClass;

  Hero(HSClasses heroClass, int health, std::string name)
      : Character(health), heroClass(heroClass), heroPower(heroClass),
        name(name) {}

  std::string getName() const { return name; }

  void equipWeapon(Weapon *newWeapon, GameState *state) {

    if (weapon && weapon->containsAttribute<Deathrattle>()) {
      weapon->triggerOnDeath(state);
    }

    weapon = newWeapon;
  }

  int getAttack() override {
    return attack + (weapon == nullptr ? 0 : weapon->getAttack());
  }

  void takeDamage(Damage damage) override {
    int damagedConsumed = damage.amount;
    damagedConsumed = reduceArmor(damagedConsumed);
    health -= damagedConsumed;
  }

  bool isDead() { return getHealth() <= 0; }

  void dealDamage(Character *target) override {
    int totalAttack = 0;
    if (weapon) {
      totalAttack = weapon->getAttack();
      if (!weapon->reduceDurability()) {
        weapon = nullptr;
      }
    }

    totalAttack += attack;

    target->takeDamage({totalAttack, false});
  }
  HeroPower heroPower;
  Weapon *weapon = nullptr;

private:
  int reduceArmor(int amount) {
    if (!armor) {
      return amount;
    }

    armor = std::max(0, armor - amount);
    amount = std::abs(armor - amount);

    return amount;
  }
  std::string name;
};
