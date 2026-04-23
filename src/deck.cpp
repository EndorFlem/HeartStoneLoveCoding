#include "deck.h"
#include "card.h"

DrawResult Deck::draw() {
  if (cards.empty()) {
    damage++;
    return {{damage}, true};
  }

  Card *card = cards.back();
  cards.pop_back();

  return {{.card = card}, false};
}