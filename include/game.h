#pragma once

#include "board.h"
#include "deck.h"
#include "hero.h"
#include "utilityclasses.h"
#include <type_traits>
#include <unordered_set>
#include <vector>

class Game {
public:
  int start();
  Game(Player *me, Deck *myDeck, Player *enemy, Deck *enemyDeck, Board *board)
      : me(me), enemy(enemy), myDeck(myDeck), enemyDeck(enemyDeck),
        board(board) {}

private:
  Player *me;
  Player *enemy;
  Deck *myDeck;
  Deck *enemyDeck;
  Board *board;

  bool isRunning = true;

  int chooseToPlay(Player *player);
  int oneTurn(int i, Player *player, Deck *deck);

  int askPostion(std::unordered_set<int> valids) {
    int position = 0;
    while (true) {
      std::cout << "Choose position: ";
      std::cin >> position;
      if (valids.contains(position) || position == -1) {
        return position;
      }
      std::cout << "invalid position\n";
    }
  }

  std::unordered_set<int> validCards(int mana, std::vector<Card *> hand) {
    std::unordered_set<int> result{};
    for (int i = 0; i < hand.size(); i++) {
      if (hand[i]->getCost() > mana) {
        continue;
      }
      result.insert(i);
    }

    return result;
  }
};