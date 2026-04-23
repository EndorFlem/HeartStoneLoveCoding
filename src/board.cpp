#include "board.h"
#include "deck.h"
#include "minion.h"
#include "spell.h"
#include "weapon.h"
#include <cstddef>
#include <iostream>
#include <vector>

void printMinion(int pos, int atack, int hp) {
  std::cout << "[" << pos << "]" << " " << atack << "/" << hp << "\n";
}
void printHero(Hero *hero, int pos) {
  std::cout << "[" << pos << "]" << hero->getName()
            << " hp:" << hero->getHealth();

  if (hero->getAttack() != 0) {
    if (hero->weapon) {
      std::cout << " {" << hero->getAttack() << "/" << hero->weapon->durability
                << "}";
    }

    else {
      std::cout << " {" << hero->getAttack() << "}";
    }
  }

  std::cout << " mana:" << hero->mana << "\n";
}
void printOneSide(int initPos, std::string boardName, Hero *hero,
                  std::vector<Minion *> *minions) {
  std::cout << boardName << "\n";
  printHero(hero, minions->size() + initPos);

  for (size_t i = 0; i < minions->size(); i++) {

    printMinion(i + initPos, minions[i][0]->getAttack(),
                minions[i][0]->getHealth());
  }
}

void Board::printBoard() {
  printOneSide(0, "ENEMY BOARD", enemyHero, enemyMinions);
  std::cout
      << "---------------------------------------------------------------\n";
  printOneSide(enemyMinions->size() + 1, "MY BOARD", myHero, myMinions);
}

int refreshMana(int turn) { return std::min(turn, 10); }

void Board::dealDamage(Character *target, Damage damage) {
  target->takeDamage(damage);
}

void Board::startTurn(int turn, Player *player, Deck *deck) {
  player->hero->mana = refreshMana(turn);
  DrawResult drawed = deck->draw();
  if (drawed.isEmpty) {
    dealDamage(player->hero, {drawed.result.damage, false});
    return;
  }

  player->addCardToHand(drawed.result.card);
}

bool Board::playCard(Player *player, int i) {
  Card *card = player->hand[i];
  player->hand.erase(player->hand.begin() + i);

  if (dynamic_cast<Minion *>(card)) {
    if (player->side->size() == 7) {
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

  player->hero->mana -= card->getCost();
  return true;
}

void Board::playMinion(Player *player, Minion *minion) {
  player->side->push_back(minion);
}

void Board::playWeapon(Player *player, Weapon *weapon) {
  std::cout << "IMPLEMENT WEOPNS";
}
void Board::playSpell(Player *player, Spell *spell) {
  std::cout << "IMPLEMENT SPELLS";
}