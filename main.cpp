#include "allCards.h"
#include "board.h"
#include "deck.h"
#include "game.h"
#include "gameState.h"
#include "hand.h"
#include "manaPool.h"
#include "selectorTool.h"
#include "utilityclasses.h"
#include <vector>
using namespace std;

void script(GameState &state) {
  state.startTurn();
  state.playCardByPosition(0).playCardByPosition(
      0, TargetSelector::myMinionByPosition, 0);
  state.playCardByPosition(0, TargetSelector::myMinionByPosition, 0)
      .playCardByPosition(0, TargetSelector::myMinionByPosition, 0);
  state.attack(TargetSelector::myMinionByPosition, 0,
               TargetSelector::oppositeHero);
  state.printBoard();
  state.printState();

}

// реализация варианта "Кабанчики"
int main() {
  Hero *enemyHero = new Hero(HSClasses::WARRIOR, 30, "Garrosh-Харрош");
  Hero *myHero = new Hero(HSClasses::ROGUE, 30, "Валира-Валера");

  BoardSide *myMinions = new BoardSide{{}};
  BoardSide *enemyMinions = new BoardSide{{}};

  std::vector<Card *> myCards = getCardCollection({0, 2, 3, 4});
  std::vector<Card *> enemyCards = getCardCollection({0});

  Player *me =
      new Player(myHero, ManaPool{10}, Hand{myCards}, Deck{}, myMinions);
  Player *enemy = new Player(enemyHero, ManaPool{10}, Hand{enemyCards}, Deck{},
                             enemyMinions);
  Board *board = new Board(myHero, enemyHero, myMinions, enemyMinions);
  Game game{me, enemy, board};
  GameState &state = game.start();
  script(state);

  return 0;
}
