#include "hero.h"
#include "board.h"
#include "gameState.h"
#include "utilityclasses.h"
#include "weapon.h"

#include <cstddef>

const std::array<HeroPower::HeroPowerFunction, 9> HeroPower::heroPowers = {
    &HeroPower::warriorPower, &HeroPower::magePower,
    &HeroPower::roguePower,   &HeroPower::priestPower,
    &HeroPower::hunterPower,  &HeroPower::shamanPower,
    &HeroPower::druidPower,   &HeroPower::warlockPower,
    &HeroPower::paladinPower};

HeroPower::HeroPower(HSClasses heroClass)
    : cost(2), heroPower(heroPowers[static_cast<std::size_t>(heroClass)]) {}

void HeroPower::activate(Board *board, Character *target, GameState *state) {
  if (heroPower != nullptr) {
    active = false;
    heroPower(board, target, state);
  }
}

void HeroPower::warriorPower(Board *board, Character *target, GameState *state) {
  Hero *hero = dynamic_cast<Hero *>(target);
  hero->armor += 2;
}

void HeroPower::magePower(Board *board, Character *target, GameState *state) {}

void HeroPower::roguePower(Board *board, Character *target, GameState *state) {
  Weapon *weapon = new Weapon(1, "Default blades", HSClasses::ROGUE, 1, 2, {});

  Hero *hero = dynamic_cast<Hero *>(target);
  hero->equipWeapon(weapon, state);
}

void HeroPower::priestPower(Board *board, Character *target, GameState *state) {}

void HeroPower::hunterPower(Board *board, Character *target, GameState *state) {
  target->takeDamage({2, false});
}

void HeroPower::shamanPower(Board *board, Character *target, GameState *state) {}

void HeroPower::druidPower(Board *board, Character *target, GameState *state) {
  Hero *hero = dynamic_cast<Hero *>(target);
  hero->armor += 1;
}

void HeroPower::warlockPower(Board *board, Character *target, GameState *state) {}

void HeroPower::paladinPower(Board *board, Character *target, GameState *state) {}
