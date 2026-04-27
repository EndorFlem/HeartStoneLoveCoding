#include "card.h"
#include "gameState.h"
#include "minion.h"
#include "selectorTool.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <array>
#include <cstddef>
#include <vector>


//пример наследования
class Berserk : public Minion {
  public: Berserk(int cost, std::string name, HSClasses cardClass, int health, int attack) : Minion(cost, name, cardClass, health, attack) {

  }

  int getAttack() override {
    if (health == maxHealth) {
      return attack;
    }
    return attack + 3;
  }
};

std::array<Card *, 100> allCards = {
    // Charge — рывок через атрибут
    new Minion(1, "Test Minion", HSClasses::ROGUE, 1, 1, {
        new BattleCry([](GameState *state, Attributable *self) {
            (void)state;
            dynamic_cast<Character *>(self)->onTurnStart(); // снимает summoningSickness
        })
    }),
    new Weapon(2, "Test Weapon", HSClasses::ROGUE, 1, 1, {}),
    new Spell(0, "Слово силы: Щит", HSClasses::PRIEST,
              [](GameState *state, Character *target) {
                state->buffMaxHealth(static_cast<Minion *>(target), 2);
              }),
    new Spell(0, "Божественный дух", HSClasses::PRIEST,
              [](GameState *state, Character *target) {
                state->buffMaxHealth(static_cast<Minion *>(target), target->getMaxHealth());
              }),

      new Spell(0, "Переворот", HSClasses::PRIEST,
              [](GameState *state, Character *target) {
                size_t health = target->getHealth();
                size_t attack = target->getAttack();
                state->setAttack(target, health);
                state->setHealth(target, attack);
              }),
      new Berserk(0, "Berserk", HSClasses::WARRIOR, 4, 2),
};

inline std::vector<Card *>
getCardCollection(std::initializer_list<std::size_t> cardNumbers) {
  std::vector<Card *> result;
  result.reserve(cardNumbers.size());

  for (std::size_t cardNumber : cardNumbers) {
    result.push_back(allCards.at(cardNumber)->clone());
  }

  return result;
}
