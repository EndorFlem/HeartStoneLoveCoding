#pragma once
#include "utilityclasses.h"
#include "weapon.h"
#include <array>
#include <cstddef>
#include <functional>
#include <string>

class Board;
class HeroPower {
private:
  using HeroPowerFunction =
      std::function<void(Board *board, Character *target)>;
  static const std::array<HeroPowerFunction, 9> heroPowers;

  int cost;
  HeroPowerFunction heroPower;

public:
  HeroPower(HSClasses heroClass);
  void activate(Board *board, Character *target);

private:
  static void warriorPower(Board *board, Character *target);
  static void magePower(Board *board, Character *target);
  static void roguePower(Board *board, Character *target);
  static void priestPower(Board *board, Character *target);
  static void hunterPower(Board *board, Character *target);
  static void shamanPower(Board *board, Character *target);
  static void druidPower(Board *board, Character *target);
  static void warlockPower(Board *board, Character *target);
  static void paladinPower(Board *board, Character *target);
};

class Hero : public Character {
public:
  int mana;
  int armor;
  int attack = 0;
  HSClasses heroClass;

  Hero(HSClasses heroClass, int health, std::string name)
      : Character(health), mana(0), heroClass(heroClass), heroPower(heroClass),
        name(name) {}

  std::string getName() { return name; }

  void equipWeapon(Weapon *newWeapon) {

    if (weapon && weapon->containsDeathratle()) {
      weapon->triggerDeathratles();
    }

    weapon = newWeapon;
  }

  int getAttack() { return weapon == nullptr ? 0 : weapon->getAttack(); }
  HeroPower heroPower;
  Weapon *weapon = nullptr;

private:
  std::string name;
};
