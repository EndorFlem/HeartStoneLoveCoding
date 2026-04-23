#include "card.h"
#include "hero.h"
#include "minion.h"
#include <cstddef>
#include <iostream>
#include <vector>
class Player {
public:
  Player(Hero *hero, std::vector<Minion *> *side)
      : hero(hero), side(side), hand(std::vector<Card *>()) {}

  Player(Hero *hero, std::vector<Minion *> *side, std::vector<Card *> hand)
      : hero(hero), side(side), hand(hand) {}
  Hero *hero;
  std::vector<Minion *> *side;

  bool addCardToHand(Card *card) {
    if (hand.size() == 10) {
      return false;
    }
    hand.push_back(card);
    return true;
  }
  void printHand() {
    for (size_t i = 0; i < hand.size(); i++) {
      std::cout << "[" << i << "] " << hand[i]->getName() << " "
                << hand[i]->getCost() << "\n";
    }
  };

  std::vector<Card *> hand;
};