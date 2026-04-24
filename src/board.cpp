#include "board.h"
#include "hero.h"
#include "minion.h"
#include "player.h"
#include "spell.h"
#include "weapon.h"
#include <cstddef>
#include <iostream>
#include <vector>

void Board::printMinion(size_t pos, size_t atack, size_t hp) {
  std::cout << "[" << pos << "]" << " " << atack << "/" << hp << "\n";
}

void Board::printHero(Hero *hero, size_t pos, bool isCurrent, int mana) {
  std::cout << "[" << pos << "]" << hero->getName()
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

void Board::printOneSide(size_t initPos, std::string boardName, Player *player,
                         bool isCurrent) {
  std::cout << boardName << "\n";
  printHero(player->hero, player->boardSide->size() + initPos, isCurrent,
            player->manaPool.getMana());

  for (std::size_t i = 0; i < player->boardSide->size(); i++) {
    printMinion(i + initPos, player->boardSide->at(i)->getAttack(),
                player->boardSide->at(i)->getHealth());
  }
}

void Board::printBoard(Player *me, Player *enemy, Player *currentPlayer) {
  bool isEnemyCurrent = enemy == currentPlayer;
  bool isMeCurrent = me == currentPlayer;

  printOneSide(0, "ENEMY BOARD", enemy, isEnemyCurrent);
  std::cout
      << "---------------------------------------------------------------\n";

  int myOffset = static_cast<int>(enemy->boardSide->size()) + 1;
  printOneSide(myOffset, "MY BOARD", me, isMeCurrent);
}

bool Board::playCard(Player *player, Card *card) {
  if (dynamic_cast<Minion *>(card)) {
    if (player->boardSide->size() == 7) {
      return false;
    }
    playMinion(player, dynamic_cast<Minion *>(card));
  }

  if (dynamic_cast<Weapon *>(card)) {
    playWeapon(player, dynamic_cast<Weapon *>(card));
  }

  if (dynamic_cast<Spell *>(card)) {
    playSpell(player, dynamic_cast<Spell *>(card));
  }

  return true;
}

void Board::playMinion(Player *player, Minion *minion) {
  player->boardSide->addMinion(minion);
}

void Board::playWeapon(Player *player, Weapon *weapon) {
  std::cout << "IMPLEMENT WEOPNS";
}
void Board::playSpell(Player *player, Spell *spell) {
  std::cout << "IMPLEMENT SPELLS";
}
