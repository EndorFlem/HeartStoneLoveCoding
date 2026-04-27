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
  CharacterIsExhausted,
  CharacterHasSummoningSickness,
};

static constexpr std::array<const char *, 10> illegalStateStrings = {
    "OK",
    "Карта не найдена в руке на указанной позиции",
    "Недостаточно маны",
    "Сила героя уже использована в этот ход",
    "Цель не выбрана",
    "Персонаж не может атаковать: нет атаки",
    "Ход ещё не завершён",
    "Ход ещё не начат",
    "Персонаж уже атаковал в этот ход",
    "Существо не может атаковать в ход своего призыва"};

static const char *illegalStateToString(IllegalStates state) {
  return illegalStateStrings[static_cast<std::size_t>(state)];
}

static bool stateToBool(IllegalStates &state) {
  return state != IllegalStates::Legal;
}

class GameState {
  static constexpr size_t MAX_PLAYERS = 2;

public:
  GameState(std::array<Player *, MAX_PLAYERS> players, Board *board);

  GameState &startTurn();

  Hero *getOppositeHero();
  Hero *getCurrentHero();
  Player *getOppositePlayer();
  Player *getCurrentPlayer();

  IllegalStates endTurn();

  GameState &playCard(int) = delete;


  GameState &playCard(Card *card);
  GameState &playCard(std::nullptr_t) = delete;


  GameState &playCardByPosition(size_t position);
  GameState &playCardByPosition(size_t position, GameStateSelector selector);
  GameState &playCardByPosition(size_t position, GameStateWithIDSelector selector, ID id);
  GameState &playCardByPosition(size_t position, MultiSelector selector);
  GameState &playCardByPosition(size_t cardPosition, GameStateWithPositionSelector selector, size_t targetPostion);

  GameState &useHeroPower();
  GameState &useHeroPower(Character *target);

  GameState &attack(Character *attacker, Character *target);
  GameState &attack(GameStateWithPositionSelector attackerSelector, size_t attackerPosition, GameStateWithPositionSelector targetSelector, size_t targetPosition);
  GameState &attack(GameStateWithPositionSelector attackerSelector, size_t attackerPosition, GameStateSelector targetSelector);

  GameState &attackWithHero(GameStateWithIDSelector selector, ID id);
  GameState &attackWithHero(GameStateSelector selector);
  GameState &attackWithHero(GameStateWithPositionSelector selector, size_t position);

  GameState &dealDamage(Character *target, int amount);
  GameState &dealDamage(GameStateSelector selector, int amount);
  GameState &dealDamage(GameStateWithIDSelector selector, ID id, int amount);
  GameState &dealDamage(GameStateWithPositionSelector selector, size_t position, int amount);
  GameState &dealDamage(MultiSelector selector, int amount);

  GameState &heal(Character *target, int amount);
  GameState &heal(GameStateSelector selector, int amount);
  GameState &heal(GameStateWithIDSelector selector, ID id, int amount);
  GameState &heal(GameStateWithPositionSelector selector, size_t position, int amount);
  GameState &heal(MultiSelector selector, int amount);


  GameState &buffMaxHealth(Character *target, int amount);
  GameState &buffMaxHealth(GameStateSelector selector, int amount);
  GameState &buffMaxHealth(MultiSelector selector, int amount);

  GameState &buffMaxAttack(Character *target, int amount);
  GameState &buffMaxAttack(GameStateSelector selector, int amount);
  GameState &buffMaxAttack(MultiSelector selector, int amount);

  GameState &buffHealth(Character *target, int amount);
  GameState &buffHealth(GameStateSelector selector, int amount);
  GameState &buffHealth(MultiSelector selector, int amount);


  GameState &buffAttack(Character *target, int amount);
  GameState &buffAttack(GameStateSelector selector, int amount);
  GameState &buffAttack(MultiSelector selector, int amount);

  GameState &setAttack(Character *target, int value);
  GameState &setAttack(GameStateSelector selector, int value);
  GameState &setAttack(MultiSelector selector, int value);

  GameState &setHealth(Character *target, int value);
  GameState &setHealth(GameStateSelector selector, int value);
  GameState &setHealth(MultiSelector selector, int value);

  bool isFinished();

  bool isIllegal();

  GameState &printBoard();

  GameState &printState();

  GameState &printIDs();

private:
  IllegalStates state = IllegalStates::Legal;
  bool hasFinished = false;
  int turn = -1;
  Board *board;
  Player *currentPlayer;
  std::array<Player *, MAX_PLAYERS> players;
  bool turnStarted = false;
  std::size_t nextCardIDNumber = 0;

  GameState &attackWithHero(Character *target);

  friend class TargetSelector;

  void initializeCardIDs();

  void assignCollectionIDType(const std::vector<Card *> &cards, IDType type);

  void assignNewID(Card *card, IDType type);

  void assignPlayedCardID(Card *card);

  void checkDeaths();
  void checkForFinish();

  Card *takeCardFromHand(size_t position);
};
