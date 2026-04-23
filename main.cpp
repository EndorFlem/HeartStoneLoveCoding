#include "allCards.h"
#include "board.h"
#include "deck.h"
#include "game.h"
#include "minion.h"
#include "utilityclasses.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
  Hero *enemyHero = new Hero(HSClasses::WARRIOR, 30, "Garrosh-Харрош");
  Hero *myHero = new Hero(HSClasses::ROGUE, 30, "Валира-Валера");

  std::vector<Minion *> myMinions{};
  std::vector<Minion *> enemyMinions{};
  Player *me = new Player(myHero, &myMinions);
  Player *enemny = new Player(enemyHero, &enemyMinions);
  std::vector<Card *> myCards = {allCards.at(0), allCards.at(1),
                                 allCards.at(2)};
  Deck *myDeck = new Deck(myCards);
  Deck *enemyDeck = new Deck();
  Board *board = new Board(myHero, enemyHero, myDeck, enemyDeck, &myMinions,
                           &enemyMinions);
  Game game{me, myDeck, enemny, enemyDeck, board};
  game.start();

  return 0;
}