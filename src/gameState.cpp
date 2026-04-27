#include "gameState.h"

#include "board.h"
#include "minion.h"
#include "selectorTool.h"
#include "spell.h"
#include "utilityclasses.h"
#include "weapon.h"
#include <algorithm>
#include <cstddef>

const char *idTypeToString(IDType type) {
  switch (type) {
  case IDType::Card:    return "Card";
  case IDType::Minion:  return "Minion";
  case IDType::Weapon:  return "Weapon";
  case IDType::Hero:    return "Hero";
  }
  return "Unknown";
}

void printCardIDLine(const Card *card) {
  ID id = card->getID();
  std::cout << card->getName() << " -> " << idTypeToString(id.type) << "#" << id.number << "\n";
}

void printCardCollectionIDs(const std::vector<Card *> &cards, const char *collectionName) {
  std::cout << collectionName << ":\n";
  if (cards.empty()) { std::cout << "empty\n"; return; }
  for (const Card *card : cards) printCardIDLine(card);
}

void printBoardSideIDs(const BoardSide *boardSide) {
  if (boardSide->size() == 0) { std::cout << "empty\n"; return; }
  for (const Minion *minion : *boardSide) {
    ID id = minion->getID();
    std::cout << minion->getName() << " -> " << idTypeToString(id.type) << "#" << id.number << "\n";
  }
}



GameState::GameState(std::array<Player *, MAX_PLAYERS> players, Board *board)
    : board(board), players(players) {
  initializeCardIDs();
}


GameState &GameState::startTurn() {
  if (stateToBool(state) || hasFinished) return *this;
  if (turnStarted) { state = IllegalStates::TurnHasNotEnded; return *this; }

  turnStarted = true;
  turn++;
  currentPlayer = players[turn % MAX_PLAYERS];
  currentPlayer->onTurnStart(turn / MAX_PLAYERS + 1);
  for (Minion *minion : *currentPlayer->boardSide) minion->onTurnStart();
  return *this;
}

IllegalStates GameState::endTurn() {
  if (!turnStarted) { state = IllegalStates::TurnHasNotStarted; return state; }
  turnStarted = false;
  return state;
}

Hero *GameState::getOppositeHero()   { return players[(turn + 1) % MAX_PLAYERS]->hero; }
Hero *GameState::getCurrentHero()    { return players[turn % MAX_PLAYERS]->hero; }
Player *GameState::getOppositePlayer() { return players[(turn + 1) % MAX_PLAYERS]; }
Player *GameState::getCurrentPlayer()  { return players[turn % MAX_PLAYERS]; }


GameState &GameState::playCard(Card *card) {
  if (stateToBool(state) || hasFinished) return *this;
  assignPlayedCardID(card);
  board->playCard(currentPlayer, card, this);
  return *this;
}

Card *GameState::takeCardFromHand(size_t position) {
  if (stateToBool(state) || hasFinished) return nullptr;
  Card *card = HandSelector::getCardByPosition(currentPlayer->hand, position);
  if (!card) { state = IllegalStates::CardWrongHandPosition; return nullptr; }
  if (!currentPlayer->manaPool.reduceMana(card->getCost())) { state = IllegalStates::NotEnoughMana; return nullptr; }
  currentPlayer->hand.removeAt(position);
  return card;
}

GameState &GameState::playCardByPosition(size_t position) {
  Card *card = takeCardFromHand(position);
  if (!card) return *this;
  return playCard(card);
}

GameState &GameState::playCardByPosition(size_t position, GameStateSelector selector) {
  Card *card = takeCardFromHand(position);
  if (!card) return *this;
  Character *target = selector(this);
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  assignPlayedCardID(card);
  board->playCard(currentPlayer, card, this, target);
  return *this;
}

GameState &GameState::playCardByPosition(size_t position, GameStateWithIDSelector selector, ID id) {
  return playCardByPosition(position, [selector, id](GameState *gs) { return selector(gs, id); });
}

GameState &GameState::playCardByPosition(size_t position, MultiSelector selector) {
  Card *card = takeCardFromHand(position);
  if (!card) return *this;
  assignPlayedCardID(card);
  board->playCard(currentPlayer, card, this, selector(this));
  checkDeaths();
  return *this;
}

GameState &GameState::playCardByPosition(size_t position, GameStateWithPositionSelector selector, size_t targetPosition) {
  return playCardByPosition(position, [selector, targetPosition](GameState *gs) { return selector(gs, targetPosition); });
}


GameState &GameState::useHeroPower() {
  Character *target = TargetSelector::forHeroPower(
      currentPlayer->hero->heroClass, currentPlayer->hero, getOppositePlayer()->hero);
  return useHeroPower(target);
}

GameState &GameState::useHeroPower(Character *target) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  if (!currentPlayer->manaPool.reduceMana(currentPlayer->hero->heroPower.getCost())) {
    state = IllegalStates::NotEnoughMana; return *this;
  }
  if (!currentPlayer->hero->heroPower.isActive()) {
    state = IllegalStates::HeroPowerHasAlreadyBeenUsed; return *this;
  }
  currentPlayer->hero->heroPower.activate(board, target, this);
  return *this;
}


GameState &GameState::attack(GameStateWithPositionSelector attackerSelector, size_t attackerPosition, GameStateWithPositionSelector targetSelector, size_t targetPosition) {
Character* attacker = attackerSelector(this, attackerPosition);
Character* target = targetSelector(this, targetPosition);

  if (stateToBool(state) || hasFinished) return *this;
  if (!attacker || !target) { state = IllegalStates::NoTarget; return *this; }
  if (attacker->hasSummoningSickness()) { state = IllegalStates::CharacterHasSummoningSickness; return *this; }
  if (attacker->isExhausted()) { state = IllegalStates::CharacterIsExhausted; return *this; }
  attacker->setExhausted();
  attacker->dealDamage(target);
  target->dealDamage(attacker);
  checkDeaths();
  return *this;
}

GameState &GameState::attack(GameStateWithPositionSelector attackerSelector, size_t attackerPosition, GameStateSelector targetSelector) {
  Character* attacker = attackerSelector(this, attackerPosition);
  Character* target = targetSelector(this);
  if (stateToBool(state) || hasFinished) return *this;
  if (!attacker || !target) { state = IllegalStates::NoTarget; return *this; }
  if (attacker->hasSummoningSickness()) { state = IllegalStates::CharacterHasSummoningSickness; return *this; }
  if (attacker->isExhausted()) { state = IllegalStates::CharacterIsExhausted; return *this; }
  attacker->setExhausted();
  attacker->dealDamage(target);
  target->dealDamage(attacker);
  checkDeaths();
  return *this;
}

GameState &GameState::attackWithHero(Character *target) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!currentPlayer->hero->getAttack()) { state = IllegalStates::CharacterHasNoAttack; return *this; }
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  if (currentPlayer->hero->isExhausted()) { state = IllegalStates::CharacterIsExhausted; return *this; }
  currentPlayer->hero->setExhausted();
  currentPlayer->hero->dealDamage(target);
  target->dealDamage(currentPlayer->hero);
  checkDeaths();
  return *this;
}


GameState &GameState::attackWithHero(GameStateSelector selector)                          { return attackWithHero(selector(this)); }
GameState &GameState::attackWithHero(GameStateWithIDSelector selector, ID id)              { return attackWithHero(selector(this, id)); }
GameState &GameState::attackWithHero(GameStateWithPositionSelector selector, size_t pos)   { return attackWithHero(selector(this, pos)); }


GameState &GameState::dealDamage(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->takeDamage({amount, false});
  checkDeaths();
  return *this;
}

GameState &GameState::dealDamage(GameStateSelector selector, int amount)                              { return dealDamage(selector(this), amount); }
GameState &GameState::dealDamage(GameStateWithIDSelector selector, ID id, int amount)                 { return dealDamage(selector(this, id), amount); }
GameState &GameState::dealDamage(GameStateWithPositionSelector selector, size_t pos, int amount)      { return dealDamage(selector(this, pos), amount); }

GameState &GameState::dealDamage(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) t->takeDamage({amount, false});
  checkDeaths();
  return *this;
}


GameState &GameState::heal(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->health = std::min(target->health + amount, target->maxHealth);
  return *this;
}

GameState &GameState::heal(GameStateSelector selector, int amount)                              { return heal(selector(this), amount); }
GameState &GameState::heal(GameStateWithIDSelector selector, ID id, int amount)                 { return heal(selector(this, id), amount); }
GameState &GameState::heal(GameStateWithPositionSelector selector, size_t pos, int amount)      { return heal(selector(this, pos), amount); }

GameState &GameState::heal(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) t->health = std::min(t->health + amount, t->maxHealth);
  return *this;
}

GameState &GameState::buffMaxHealth(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->maxHealth += amount;
  target->health += amount;
  return *this;
}

GameState &GameState::buffMaxHealth(GameStateSelector selector, int amount) { return buffMaxHealth(selector(this), amount); }

GameState &GameState::buffMaxHealth(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) { t->maxHealth += amount; t->health += amount; }
  return *this;
}

GameState &GameState::buffMaxAttack(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->maxAttack += amount;
  target->attack += amount;
  return *this;
}

GameState &GameState::buffMaxAttack(GameStateSelector selector, int amount) { return buffMaxAttack(selector(this), amount); }

GameState &GameState::buffMaxAttack(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) { t->maxAttack += amount; t->attack += amount; }
  return *this;
}


GameState &GameState::buffHealth(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->health = std::min(target->health + amount, target->maxHealth);
  checkDeaths();
  return *this;
}

GameState &GameState::buffHealth(GameStateSelector selector, int amount) { return buffHealth(selector(this), amount); }

GameState &GameState::buffHealth(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) t->health = std::min(t->health + amount, t->maxHealth);
  checkDeaths();
  return *this;
}

GameState &GameState::buffAttack(Character *target, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->attack = std::min(target->attack + amount, target->maxAttack);
  return *this;
}

GameState &GameState::buffAttack(GameStateSelector selector, int amount) { return buffAttack(selector(this), amount); }

GameState &GameState::buffAttack(MultiSelector selector, int amount) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) t->attack = std::min(t->attack + amount, t->maxAttack);
  return *this;
}


GameState &GameState::setAttack(Character *target, int value) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->attack = value;
  if (value > target->maxAttack) target->maxAttack = value;
  return *this;
}

GameState &GameState::setAttack(GameStateSelector selector, int value) { return setAttack(selector(this), value); }

GameState &GameState::setAttack(MultiSelector selector, int value) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) { t->attack = value; if (value > t->maxAttack) t->maxAttack = value; }
  return *this;
}

GameState &GameState::setHealth(Character *target, int value) {
  if (stateToBool(state) || hasFinished) return *this;
  if (!target) { state = IllegalStates::NoTarget; return *this; }
  target->health = value;
  if (value > target->maxHealth) target->maxHealth = value;
  checkDeaths();
  return *this;
}

GameState &GameState::setHealth(GameStateSelector selector, int value) { return setHealth(selector(this), value); }

GameState &GameState::setHealth(MultiSelector selector, int value) {
  if (stateToBool(state) || hasFinished) return *this;
  for (Character *t : selector(this)) { t->health = value; if (value > t->maxHealth) t->maxHealth = value; }
  checkDeaths();
  return *this;
}


bool GameState::isFinished() { return hasFinished; }
bool GameState::isIllegal()  { return stateToBool(state); }

GameState &GameState::printBoard() {
  board->printBoard(currentPlayer, getOppositePlayer(), currentPlayer);
  currentPlayer->printHand();
  return *this;
}

GameState &GameState::printState() {
  if (hasFinished) {
    std::cout << "Game is finished with winner "
              << (players[0]->hero->getHealth() == 0 ? players[1]->hero->getName() : players[0]->hero->getName());
    return *this;
  }
  std::cout << illegalStateToString(state);
  return *this;
}

GameState &GameState::printIDs() {
  for (std::size_t i = 0; i < players.size(); i++) {
    Player *player = players[i];
    std::cout << "PLAYER " << i << " " << player->hero->getName() << "\n";
    printCardCollectionIDs(player->hand.getCards(), "HAND IDS");
    printCardCollectionIDs(player->deck.getCards(), "DECK IDS");
    std::cout << "BOARD IDS:\n";
    printBoardSideIDs(player->boardSide);
    std::cout << "\n";
  }
  return *this;
}


void GameState::initializeCardIDs() {
  for (Player *player : players) {
    assignCollectionIDType(player->hand.getCards(), IDType::Card);
    assignCollectionIDType(player->deck.getCards(), IDType::Card);
  }
}

void GameState::assignCollectionIDType(const std::vector<Card *> &cards, IDType type) {
  for (Card *card : cards) assignNewID(card, type);
}

void GameState::assignNewID(Card *card, IDType type) {
  if (card == nullptr) return;
  card->setID({type, nextCardIDNumber++});
}

void GameState::assignPlayedCardID(Card *card) {
  if (dynamic_cast<Minion *>(card) != nullptr)  { assignNewID(card, IDType::Minion);  return; }
  if (dynamic_cast<Weapon *>(card) != nullptr)  { assignNewID(card, IDType::Weapon);  return; }
  assignNewID(card, IDType::Card);
}

void GameState::checkDeaths() {
  board->removeDeadMinions(this);
  checkForFinish();
}

void GameState::checkForFinish() {
  for (Player *player : players) {
    if (player->hero->isDead()) hasFinished = true;
  }
}
