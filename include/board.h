#pragma once

#include "deck.h"
#include "hero.h"
#include "minion.h"
#include "player.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <cstddef>
#include <numeric>
#include <unordered_set>
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
  bool playCard(Player *player, Card *card);
  void activateHeroPower();

  void printHero(Hero *hero, size_t pos, bool isCurrent, int mana);
  void printMinion(size_t pos, size_t atack, size_t hp);
  void printOneSide(size_t initPos, std::string boardName, Player *player,
                    bool isCurrent);

  // std::unordered_set<int> validTargets(TargetFor targetFor, TargetSide side)
  // {
  //   std::unordered_set<int> result;
  //   switch (targetFor) {
  //   case TargetFor::SpellOrHeroPower:
  //     if (side == TargetSide::Both) {
  //       for (int i = 0; i < enemyMinions->size(); i++) {
  //         if (!enemyMinions[i][0]->containsElusive()) {
  //           result.insert(i);
  //         }
  //       }

  //       // для героя
  //       // надо как-нибудь сделать нормальную индексацию
  //       result.insert(enemyMinions->size());

  //       for (int i = 0; i < myMinions->size(); i++) {
  //         if (!enemyMinions[i][0]->containsElusive()) {
  //           result.insert(i);
  //         }
  //       }

  //       // вообще у героя может быть свойство Elusive , но ладно , пусть
  //       будет
  //       // так
  //       result.insert(myMinions->size());
  //       break;
  //     }

  //   case TargetFor::Character:
  //     if (side == TargetSide::Both) {
  //       for (int i = 0; i < enemyMinions->size(); i++) {
  //         if (enemyMinions[i][0]->containsTaunt()) {
  //           result.insert(i);
  //         }
  //       }

  //       if (!result.empty()) {
  //         return result;
  //       }
  //       for (int i = 0; i < enemyMinions->size(); i++) {
  //         result.insert(i);
  //       }

  //       // для героя
  //       // надо как-нибудь сделать нормальную индексацию
  //       result.insert(enemyMinions->size());
  //     }

  //     break;
  //   }
  //   return result;
  // }

  // // ужасный метод
  // Character *getCharacterByPosition(int postion) {
  //   if (postion <= enemyMinions->size()) {
  //     if (postion == enemyMinions->size())
  //       return enemyHero;

  //     return enemyMinions[postion][0];
  //   }

  //   int offset = enemyMinions->size();
  //   if (postion == myMinions->size() + offset)
  //     return myHero;

  //   return myMinions[postion - offset][0];
  // }

  // Character *getOpositeHero(Hero *hero) {
  //   return myHero == hero ? enemyHero : myHero;
  // }

  // TargetSide getOpositeSide(Hero *hero) {
  //   return hero == myHero ? TargetSide::Enemies : TargetSide::Mine;
  // }

  // std::unordered_set<int> getCharacterPositionsOnHeroSide(Hero *hero) {
  //   int offset = hero == myHero ? enemyMinions->size() : 0;
  //   int number = hero == myHero ? myMinions->size() : enemyMinions->size();
  //   std::unordered_set<int> result;
  //   for (int i = 0; i < number; i++) {
  //     result.insert(i + offset);
  //   }

  //   return result;
  // }

private:
  Hero *myHero;
  Hero *enemyHero;

  BoardSide *myMinions;
  BoardSide *enemyMinions;

  friend class TargetSelector;

  void playSpell(Player *player, Spell *spell);
  void playWeapon(Player *player, Weapon *weapon);
  void playMinion(Player *player, Minion *minion);
};
