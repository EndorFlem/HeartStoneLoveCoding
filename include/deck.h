#pragma once
#include "card.h"
#include <vector>

struct DrawResult {
  union {
    int damage;
    Card *card;
  } result;
  bool isEmpty;
};

class Deck {

public:
  Deck(std::vector<Card *> cards) : cards(cards) {};
  Deck() : cards(std::vector<Card *>()) {};
  DrawResult draw();

private:
  int damage = 0;
  std::vector<Card *> cards;
};