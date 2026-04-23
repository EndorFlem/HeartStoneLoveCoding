#include "game.h"
#include "board.h"
#include "minion.h"
#include "utilityclasses.h"
#include <cstddef>
#include <iostream>
#include <unordered_set>

int Game::oneTurn(int i, Player *player, Deck *deck) {
  this->board->startTurn(i, player, deck);
  while (true) {
    if (player->hero->getHealth() <= 0) {
      return 1;
    }
    int opt = chooseToPlay(player);
    if (opt == 3) {
      break;
    }

    if (opt == -1) {
      continue;
    }
  }
  // да, это смешно, но временная атака сбрасывается в ноль у героев в конце
  // хода
  player->hero->attack = 0;
  return 0;
}

int Game::start() {
  size_t turn = 1;

  while (isRunning) {
    if (oneTurn(turn, me, myDeck)) {
      std::cout << me->hero->getName() << " has lost";
      break;
    };

    if (oneTurn(turn, enemy, enemyDeck)) {
      std::cout << me->hero->getName() << " has lost";
      break;
    }

    turn++;
  }

  return 0;
}

int Game::chooseToPlay(Player *player) {
  this->board->printBoard();
  std::cout << "\n";
  player->printHand();
  std::cout << "Options to play: \n";
  std::cout << "[0] Play card\n";
  std::cout << "[1] Attack with your character\n";
  std::cout << "[2] Use heropower\n";
  std::cout << "[3] End turn\n";
  int option = 0;
  std::cin >> option;

  switch (option) {
  case 0: {
    std::unordered_set<int> vc = validCards(player->hero->mana, player->hand);
    int pos = askPostion(vc);
    if (pos == -1) {
      return -1;
    }
    if (!board->playCard(player, pos)) {
      std::cout << "failed to play";
      return -1;
    }
  } break;
  case 1: {
    Character *attacker;
    int pos = askPostion(board->getCharacterPositionsOnHeroSide(player->hero));
    if (pos == -1) {
      return -1;
    }
    if (!board->playCard(player, pos)) {
      std::cout << "failed to play";
      return -1;
    }

    attacker = board->getCharacterByPosition(pos);

    pos = askPostion(board->validTargets(TargetFor::Character,
                                         board->getOpositeSide(player->hero)));

    bool isHeroAttacker = dynamic_cast<Hero *>(attacker);
    bool isPoison = isHeroAttacker
                        ? false
                        : dynamic_cast<Minion *>(attacker)->containsPoison();

    board->dealDamage(board->getCharacterByPosition(pos),
                      {attacker->getAttack(), isPoison});

  } break;
  case 2: {
    Character *target = nullptr;

    switch (player->hero->heroClass) {
    case HSClasses::WARRIOR:
    case HSClasses::SHAMAN:
    case HSClasses::DRUID:
    case HSClasses::PALADIN:
    case HSClasses::ROGUE:
    case HSClasses::WARLOCK:

      target = player->hero;
      break;

    case HSClasses::MAGE:
    case HSClasses::PRIEST: {
      int pos = askPostion(
          board->validTargets(TargetFor::SpellOrHeroPower, TargetSide::Both));

      target = board->getCharacterByPosition(pos);
    } break;
    case HSClasses::HUNTER:
      target = board->getOpositeHero(player->hero);
      break;
    }
    player->hero->heroPower.activate(board, target);
    player->hero->mana -= 2;
  } break;
  case 3:
    break;
  }

  return option;
}
