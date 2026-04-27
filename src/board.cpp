#include "board.h"
#include "gameState.h"
#include "hero.h"
#include "minion.h"
#include "player.h"
#include "spell.h"
#include "weapon.h"
#include <cstddef>
#include <iostream>

void Board::printMinion(Minion *minion) {
  ID id = minion->getID();
  std::cout << "[" << id.number << "] " << minion->getName()
            << " " << minion->getAttack() << "/" << minion->getHealth() << "\n";
}

void Board::printHero(Hero *hero, bool isCurrent, int mana) {
  std::cout << "[Hero] " << hero->getName()
            << " hp:" << hero->getHealth();

  if (hero->armor) {
    std::cout << " armor: " << hero->armor;
  }

  if (hero->getAttack() != 0 && isCurrent) {

    if (hero->weapon) {
      std::cout << " {" << hero->getAttack() << "/" << hero->weapon->durability
                << "}";
    }

    else {
      std::cout << " {" << hero->getAttack() << "}";
    }
  }

  std::cout << " ";
  std::cout << " mana:" << mana << "\n";
}

void Board::printOneSide(std::string boardName, Player *player, bool isCurrent) {
  std::cout << boardName << "\n";
  printHero(player->hero, isCurrent, player->manaPool.getMana());

  for (std::size_t i = 0; i < player->boardSide->size(); i++) {
    printMinion(player->boardSide->at(i));
  }
}

void Board::printBoard(Player *me, Player *enemy, Player *currentPlayer) {
  bool isEnemyCurrent = enemy == currentPlayer;
  bool isMeCurrent = me == currentPlayer;

  printOneSide("ENEMY BOARD", enemy, isEnemyCurrent);
  std::cout
      << "---------------------------------------------------------------\n";

  printOneSide("MY BOARD", me, isMeCurrent);
}

bool Board::playCard(Player *player, Card *card, GameState *state) {
  if (dynamic_cast<Minion *>(card)) {
    if (player->boardSide->size() == 7) return false;
    playMinion(player, dynamic_cast<Minion *>(card), state);
  }
  if (dynamic_cast<Weapon *>(card)) playWeapon(player, dynamic_cast<Weapon *>(card), state);
  if (dynamic_cast<Spell *>(card))  playSpell(player, dynamic_cast<Spell *>(card), state);
  return true;
}

bool Board::playCard(Player *player, Card *card, GameState *state, Character *target) {
  if (dynamic_cast<Spell *>(card)) {
    playSpell(player, dynamic_cast<Spell *>(card), state, target);
    return true;
  }
  return playCard(player, card, state);
}

bool Board::playCard(Player *player, Card *card, GameState *state, const std::vector<Character *> &targets) {
  if (dynamic_cast<Spell *>(card)) {
    playSpell(player, dynamic_cast<Spell *>(card), state, targets);
    return true;
  }
  return playCard(player, card, state);
}

void Board::playMinion(Player *player, Minion *minion, GameState *state) {
  minion->onSummoned();
  player->boardSide->addMinion(minion);
  minion->triggerOnPlay(state);
}

void Board::playWeapon(Player *player, Weapon *weapon, GameState *state) {
  player->hero->equipWeapon(weapon ,state);
  weapon->triggerOnPlay(state);
}

void Board::playSpell(Player *player, Spell *spell, GameState *state) {
  (void)player;
  if (spell->effect) spell->effect(state);
}

void Board::playSpell(Player *player, Spell *spell, GameState *state, Character *target) {
  (void)player;
  if (spell->isTargeted()) spell->targetedEffect(state, target);
  else if (spell->effect)  spell->effect(state);
}

void Board::playSpell(Player *player, Spell *spell, GameState *state, const std::vector<Character *> &targets) {
  (void)player;
  if (spell->isTargeted()) {
    for (Character *target : targets) spell->targetedEffect(state, target);
  } else if (spell->effect) {
    spell->effect(state);
  }
}

void Board::removeDeadMinions(GameState *state) {
  auto removeDead = [state](BoardSide *side) {
    for (size_t i = 0; i < side->size();) {
      Minion *minion = side->at(i);
      if (minion->isDead()) {
        minion->triggerOnDeath(state);
        side->removeMinion(i);
      } else {
        i++;
      }
    }
  };

  removeDead(myMinions);
  removeDead(enemyMinions);
}
