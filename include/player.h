#pragma once

#include "deck.h"
#include "hand.h"
#include "hero.h"
#include "manaPool.h"
#include <cstddef>

class BoardSide;
class Board;
class Player {
public:
  Player(Hero *hero, ManaPool manaPool, Hand hand, Deck deck,
         BoardSide *boardSide)
      : boardSide(boardSide), manaPool(manaPool), hand(hand), deck(deck),
        hero(hero) {}

  BoardSide *boardSide;

  bool addCardToHand(Card *card) { return hand.addCard(card); }
  bool drawCard() {
    DrawResult drawed = deck.draw();

    if (drawed.isEmpty) {
      hero->takeDamage({drawed.result.damage, false});
      return false;
    }
    return addCardToHand(drawed.result.card);
  }
  void printHand() const { hand.print(); }

  void onTurnStart(size_t turn) {
    manaPool.refreshMana(turn);
    drawCard();
    hero->heroPower.active = true;
    hero->onTurnStart();
  }

private:
  friend class GameState;
  friend class Board;
  ManaPool manaPool;
  Hand hand;
  Deck deck;
  Hero *hero;
};
