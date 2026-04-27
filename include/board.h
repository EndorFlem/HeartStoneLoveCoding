#pragma once

#include "hero.h"
#include "minion.h"
#include "player.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <cstddef>
#include <vector>

class Spell;

enum class TargetFor { SpellOrHeroPower, Character };
enum class TargetSide { Mine, Enemies, Both };

class BoardSide {
  static constexpr size_t MAX_MINIONS_NUMBER = 7;

public:
  BoardSide(std::vector<Minion *> side) : side(side) {}

  auto begin() { return side.begin(); }
  auto end() { return side.end(); }
  auto begin() const { return side.begin(); }
  auto end() const { return side.end(); }

  Minion *at(size_t i) { return side.at(i); }

  bool addMinion(Minion *minion) {
    if (side.size() == MAX_MINIONS_NUMBER) {
      return false;
    }

    side.push_back(minion);
    return true;
  }

  void removeMinion(size_t index) { side.erase(side.begin() + index); }

  size_t size() const { return side.size(); }

private:
  std::vector<Minion *> side;
};

class Board {
public:
  Board(Hero *myHero, Hero *enemyHero, BoardSide *myMinions,
        BoardSide *enemyMinions)
      : myHero(myHero), enemyHero(enemyHero), myMinions(myMinions),
        enemyMinions(enemyMinions) {}
  void printBoard(Player *me, Player *enemy, Player *currentPlayer);

  void dealDamage(Character *target, Damage damage);
  bool playCard(Player *player, Card *card, GameState *state);
  bool playCard(Player *player, Card *card, GameState *state, Character *target);
  bool playCard(Player *player, Card *card, GameState *state, const std::vector<Character *> &targets);
  void activateHeroPower();
  void removeDeadMinions(GameState *state);

  void printHero(Hero *hero, bool isCurrent, int mana);
  void printMinion(Minion *minion);
  void printOneSide(std::string boardName, Player *player, bool isCurrent);

private:
  Hero *myHero;
  Hero *enemyHero;

  BoardSide *myMinions;
  BoardSide *enemyMinions;

  friend class TargetSelector;

  void playSpell(Player *player, Spell *spell, GameState *state);
  void playSpell(Player *player, Spell *spell, GameState *state, Character *target);
  void playSpell(Player *player, Spell *spell, GameState *state, const std::vector<Character *> &targets);
  void playWeapon(Player *player, Weapon *weapon, GameState *state);
  void playMinion(Player *player, Minion *minion, GameState *state);
};
