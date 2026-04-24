#pragma once
#include "board.h"
#include "player.h"
#include "selectorTool.h"
#include "utilityclasses.h"
#include <array>
#include <cstddef>

enum class IllegalStates {
  Legal,
  CardWrongHandPosition,
  NotEnoughMana,
  HeroPowerHasAlreadyBeenUsed,
  NoTarget,
  CharacterHasNoAttack,
  TurnHasNotEnded,
  TurnHasNotStarted,
};

static constexpr std::array<const char *, 8> illegalStateStrings = {
    "Legal",           "CardWrongHandPosition",
    "NotEnoughMana",   "HeroPowerHasAlreadyBeenUsed",
    "NoTarget",        "CharacterHasNoAttack",
    "TurnHasNotEnded", "TurnHasNotStarted"};

static const char *illegalStateToString(IllegalStates state) {
  return illegalStateStrings[static_cast<std::size_t>(state)];
}

static bool stateToBool(IllegalStates &state) {
  return state != IllegalStates::Legal;
}

class GameState {
  static constexpr size_t MAX_PLAYERS = 2;

public:
  GameState(std::array<Player *, MAX_PLAYERS> players, Board *board)
      : board(board), players(players) {
    initializeCardIDs();
  }

  GameState &startTurn() {
    if (stateToBool(state) || hasFinished)
      return *this;

    if (turnStarted) {
      state = IllegalStates::TurnHasNotEnded;
      return *this;
    }

    turnStarted = true;
    turn++;
    currentPlayer = players[turn % MAX_PLAYERS];
    currentPlayer->onTurnStart(turn / MAX_PLAYERS + 1);

    return *this;
  }

  Hero *getOppositeHero() { return players[(turn + 1) % MAX_PLAYERS]->hero; }
  Hero *getCurrentHero() { return players[(turn) % MAX_PLAYERS]->hero; }
  Player *getOppositePlayer() { return players[(turn + 1) % MAX_PLAYERS]; }
  Player *getCurrentPlayer() { return players[(turn) % MAX_PLAYERS]; }

  IllegalStates endTurn() {
    if (!turnStarted) {
      state = IllegalStates::TurnHasNotStarted;
      return state;
    }
    turnStarted = false;
    return state;
  }

  GameState &playCard(int) = delete;

  // сыграть карту не из руки
  GameState &playCard(Card *card) {
    if (stateToBool(state) || hasFinished)
      return *this;

    assignPlayedCardID(card);
    board->playCard(currentPlayer, card);

    return *this;
  }
  GameState &playCard(std::nullptr_t) = delete;

  // сыграть карту из руки
  GameState &playCardByPosition(size_t position) {

    if (stateToBool(state) || hasFinished)
      return *this;

    Card *card = HandSelector::getCardByPosition(currentPlayer->hand, position);
    if (!card) {
      state = IllegalStates::CardWrongHandPosition;
      return *this;
    }

    if (!currentPlayer->manaPool.reduceMana(card->getCost())) {
      state = IllegalStates::NotEnoughMana;
      return *this;
    }

    currentPlayer->hand.removeAt(position);
    return playCard(card);
  }

  GameState &useHeroPower() {
    Character *target = TargetSelector::forHeroPower(
        currentPlayer->hero->heroClass, currentPlayer->hero,
        getOppositePlayer()->hero);

    return useHeroPower(target);
  }

  GameState &useHeroPower(Character *target) {
    if (stateToBool(state) || hasFinished)
      return *this;

    if (!target) {
      state = IllegalStates::NoTarget;
      return *this;
    }

    if (!currentPlayer->manaPool.reduceMana(
            currentPlayer->hero->heroPower.getCost())) {
      state = IllegalStates::NotEnoughMana;
      return *this;
    }
    if (!currentPlayer->hero->heroPower.isActive()) {
      state = IllegalStates::HeroPowerHasAlreadyBeenUsed;
      return *this;
    }

    currentPlayer->hero->heroPower.activate(board, target);

    return *this;
  }

  GameState &attack(Character *attacker, Character *target) {
    if (stateToBool(state) || hasFinished)
      return *this;

    if (!attacker || !target) {
      state = IllegalStates::NoTarget;
      return *this;
    }

    attacker->dealDamage(target);
    return *this;
  }

  GameState &attackWithHero(GameStateWithIDSelector selector, ID id) {
    return attackWithHero(selector(this, id));
  }

  GameState &attackWithHero(GameStateSelector selector) {
    return attackWithHero(selector(this));
  };

  bool isFinished() { return hasFinished; }

  bool isIllegal() { return stateToBool(state); }

  void printBoard() {
    board->printBoard(currentPlayer, getOppositePlayer(), currentPlayer);
    currentPlayer->printHand();
  }

  void printState() { std::cout << illegalStateToString(state); }

private:
  IllegalStates state = IllegalStates::Legal;
  bool hasFinished = false;
  int turn = -1;
  Board *board;
  Player *currentPlayer;
  std::array<Player *, MAX_PLAYERS> players;
  bool turnStarted = false;
  std::size_t nextCardIDNumber = 0;

  GameState &attackWithHero(Character *target) {
    if (stateToBool(state) || hasFinished)
      return *this;

    if (!currentPlayer->hero->getAttack()) {
      state = IllegalStates::CharacterHasNoAttack;
      return *this;
    }

    if (!target) {
      state = IllegalStates::NoTarget;
      return *this;
    }

    currentPlayer->hero->dealDamage(target);
    return *this;
  }

  friend class TargetSelector;

  void initializeCardIDs() {
    for (Player *player : players) {
      assignCollectionIDType(player->hand.getCards(), IDType::Card);
      assignCollectionIDType(player->deck.getCards(), IDType::Card);
    }
  }

  void assignCollectionIDType(const std::vector<Card *> &cards, IDType type) {
    for (Card *card : cards) {
      assignNewID(card, type);
    }
  }

  void assignNewID(Card *card, IDType type) {
    if (card == nullptr) {
      return;
    }

    card->setID({type, nextCardIDNumber++});
  }

  void assignPlayedCardID(Card *card) {
    if (dynamic_cast<Minion *>(card) != nullptr) {
      assignNewID(card, IDType::Minion);
      return;
    }

    if (dynamic_cast<Weapon *>(card) != nullptr) {
      assignNewID(card, IDType::Weapon);
      return;
    }

    assignNewID(card, IDType::Card);
  }

  void checkForFinish() {
    for (auto player : players) {
      if (player->hero->isDead()) {
        hasFinished = true;
      }
    }
  }
};
