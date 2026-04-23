#include "card.h"
#include "minion.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"

#include <array>
#include <iostream>

std::array<Card *, 100> allCards = {
    new Minion(1, "Test Minion", HSClasses::ROGUE, 1, 1),
    new Weapon(2, "Test Weapon", HSClasses::ROGUE, 1, 1, {}),
    new Spell(3, "Test Spell", HSClasses::ROGUE,
              [](Board *board, Character *target) {
                std::cout << "Test Spell has played played\n";
              }),
};
