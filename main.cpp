#include "allCards.h"
#include "board.h"
#include "deck.h"
#include "game.h"
#include "gameState.h"
#include "hand.h"
#include "manaPool.h"
#include "minion.h"
#include "scripts.h"
#include "utilityclasses.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
  Hero *enemyHero = new Hero(HSClasses::WARRIOR, 30, "Garrosh-Харрош");
  Hero *myHero = new Hero(HSClasses::ROGUE, 30, "Валира-Валера");

  BoardSide *myMinions = new BoardSide{{}};
  BoardSide *enemyMinions = new BoardSide{{}};

  std::vector<Card *> myCards = getCardCollection({0, 1, 2});
  std::vector<Card *> enemyCards = getCardCollection({0, 1, 2});
  Deck myDeck{};
  Deck enemyDeck{enemyCards};

  Player *me =
      new Player(myHero, ManaPool{10}, Hand{myCards}, myDeck, myMinions);
  Player *enemy =
      new Player(enemyHero, ManaPool{10}, Hand{}, enemyDeck, enemyMinions);
  Board *board = new Board(myHero, enemyHero, myMinions, enemyMinions);
  Game game{me, enemy, board};
  GameState &state = game.start();

  firstTest(state);

  return 0;
}
