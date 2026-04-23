#pragma once

#include "deck.h"
#include "hero.h"
#include "minion.h"
#include "player.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <numeric>
#include <unordered_set>
#include <vector>

class Spell;

enum class TargetFor { SpellOrHeroPower, Character };
enum class TargetSide { Mine, Enemies, Both };

class Board {
public:
  Board(Hero *myHero, Hero *enemyHero, Deck *myDeck, Deck *enemyDeck,
        std::vector<Minion *> *myMinions, std::vector<Minion *> *enemyMinions)
      : myHero(myHero), enemyHero(enemyHero), myMinions(myMinions),
        enemyMinions(enemyMinions), myDeck(myDeck), enemyDeck(enemyDeck) {}
  void startTurn(int turn, Player *player, Deck *deck);
  void printBoard();

  void dealDamage(Character *target, Damage damage);
  bool playCard(Player *player, int i);
  void activateHeroPower();

  std::unordered_set<int> validTargets(TargetFor targetFor, TargetSide side) {
    std::unordered_set<int> result;
    switch (targetFor) {
    case TargetFor::SpellOrHeroPower:
      if (side == TargetSide::Both) {
        for (int i = 0; i < enemyMinions->size(); i++) {
          if (!enemyMinions[i][0]->containsElusive()) {
            result.insert(i);
          }
        }

        // для героя
        // надо как-нибудь сделать нормальную индексацию
        result.insert(enemyMinions->size());

        for (int i = 0; i < myMinions->size(); i++) {
          if (!enemyMinions[i][0]->containsElusive()) {
            result.insert(i);
          }
        }

        // вообще у героя может быть свойство Elusive , но ладно , пусть будет
        // так
        result.insert(myMinions->size());
        break;
      }

    case TargetFor::Character:
      if (side == TargetSide::Both) {
        for (int i = 0; i < enemyMinions->size(); i++) {
          if (enemyMinions[i][0]->containsTaunt()) {
            result.insert(i);
          }
        }

        if (!result.empty()) {
          return result;
        }
        for (int i = 0; i < enemyMinions->size(); i++) {
          result.insert(i);
        }

        // для героя
        // надо как-нибудь сделать нормальную индексацию
        result.insert(enemyMinions->size());
      }

      break;
    }
    return result;
  }

  // ужасный метод
  Character *getCharacterByPosition(int postion) {
    if (postion <= enemyMinions->size()) {
      if (postion == enemyMinions->size())
        return enemyHero;

      return enemyMinions[postion][0];
    }

    int offset = enemyMinions->size();
    if (postion == myMinions->size() + offset)
      return myHero;

    return myMinions[postion - offset][0];
  }

  Character *getOpositeHero(Hero *hero) {
    return myHero == hero ? enemyHero : myHero;
  }

  TargetSide getOpositeSide(Hero *hero) {
    return hero == myHero ? TargetSide::Enemies : TargetSide::Mine;
  }

  std::unordered_set<int> getCharacterPositionsOnHeroSide(Hero *hero) {
    int offset = hero == myHero ? enemyMinions->size() : 0;
    int number = hero == myHero ? myMinions->size() : enemyMinions->size();
    std::unordered_set<int> result;
    for (int i = 0; i < number; i++) {
      result.insert(i + offset);
    }

    return result;
  }

private:
  Hero *myHero;
  Hero *enemyHero;
  std::vector<Minion *> *myMinions;
  std::vector<Minion *> *enemyMinions;
  Deck *myDeck;
  Deck *enemyDeck;

  void playSpell(Player *player, Spell *spell);
  void playWeapon(Player *player, Weapon *weapon);
  void playMinion(Player *player, Minion *minion);
};