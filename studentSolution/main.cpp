#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Character {
  string name;
  int hp;
  int attack;

  Character(string name, int hp, int attack) : name(name), hp(hp), attack(attack) {}

  bool isAlive() const { return hp > 0; }

  void takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) {
      hp = 0;
    }
  }
};

struct Card {
  string name;
  int cost;

  Card(string name, int cost) : name(name), cost(cost) {}
  virtual ~Card() = default;
  virtual void play(class Player *owner, class Player *enemy) = 0;
};

struct Minion : public Card {
  int hp;
  int attack;

  Minion(string name, int cost, int hp, int attack)
      : Card(name, cost), hp(hp), attack(attack) {}

  void play(class Player *owner, class Player *enemy) override;
};

struct Spell : public Card {
  int damage;

  Spell(string name, int cost, int damage)
      : Card(name, cost), damage(damage) {}

  void play(class Player *owner, class Player *enemy) override;
};

struct Player {
  Character *hero;
  vector<Card *> deck;
  vector<Card *> hand;
  vector<Character *> board;
  int mana = 0;
  int maxMana = 0;

  Player(Character *hero) : hero(hero) {}

  void drawCard() {
    if (deck.empty()) {
      cout << hero->name << " has no cards to draw\n";
      return;
    }

    hand.push_back(deck.back());
    deck.pop_back();
  }
};

struct GameState {
  Player *players[2];
  int currentPlayerIndex = 0;
  int turnNumber = 0;

  GameState(Player *first, Player *second) {
    players[0] = first;
    players[1] = second;
  }

  Player *currentPlayer() { return players[currentPlayerIndex]; }
  Player *enemyPlayer() { return players[1 - currentPlayerIndex]; }

  void printState() {
    cout << "\n====================================\n";
    cout << "Current player: " << currentPlayer()->hero->name << "\n";
    cout << enemyPlayer()->hero->name << " hp: " << enemyPlayer()->hero->hp
         << "\n";
    cout << currentPlayer()->hero->name << " hp: " << currentPlayer()->hero->hp
         << " mana: " << currentPlayer()->mana << "/" << currentPlayer()->maxMana
         << "\n";

    cout << "Enemy board:\n";
    for (size_t i = 0; i < enemyPlayer()->board.size(); i++) {
      cout << "[" << i << "] " << enemyPlayer()->board[i]->name << " "
           << enemyPlayer()->board[i]->attack << "/"
           << enemyPlayer()->board[i]->hp << "\n";
    }

    cout << "My board:\n";
    for (size_t i = 0; i < currentPlayer()->board.size(); i++) {
      cout << "[" << i << "] " << currentPlayer()->board[i]->name << " "
           << currentPlayer()->board[i]->attack << "/"
           << currentPlayer()->board[i]->hp << "\n";
    }

    cout << "My hand:\n";
    for (size_t i = 0; i < currentPlayer()->hand.size(); i++) {
      cout << "[" << i << "] " << currentPlayer()->hand[i]->name
           << " cost:" << currentPlayer()->hand[i]->cost << "\n";
    }
    cout << "====================================\n";
  }

  void startTurn() {
    turnNumber++;
    Player *player = currentPlayer();
    player->maxMana = min(10, player->maxMana + 1);
    player->mana = player->maxMana;
    player->drawCard();
    cout << "\nStart turn for " << player->hero->name << "\n";
  }

  bool playCard(Card *card) {
    Player *player = currentPlayer();
    Player *enemy = enemyPlayer();

    auto it = find(player->hand.begin(), player->hand.end(), card);
    if (it == player->hand.end()) {
      cout << "Card is not in hand\n";
      return false;
    }

    if (card->cost > player->mana) {
      cout << "Not enough mana\n";
      return false;
    }

    player->mana -= card->cost;
    player->hand.erase(it);
    card->play(player, enemy);
    cout << player->hero->name << " played " << card->name << "\n";
    return true;
  }

  bool attack(Character *attacker, Character *target) {
    if (attacker == nullptr || target == nullptr) {
      cout << "Bad attack target\n";
      return false;
    }

    if (!attacker->isAlive() || !target->isAlive()) {
      cout << "Dead character can not attack or be attacked\n";
      return false;
    }

    target->takeDamage(attacker->attack);
    cout << attacker->name << " attacks " << target->name << " for "
         << attacker->attack << "\n";

    if (target->isAlive()) {
      attacker->takeDamage(target->attack);
      cout << target->name << " hits back for " << target->attack << "\n";
    }

    removeDeadCharacters(currentPlayer());
    removeDeadCharacters(enemyPlayer());
    return true;
  }

  void endTurn() {
    cout << "End turn for " << currentPlayer()->hero->name << "\n";
    currentPlayerIndex = 1 - currentPlayerIndex;
  }

  void removeDeadCharacters(Player *player) {
    player->board.erase(
        remove_if(player->board.begin(), player->board.end(),
                  [](Character *character) { return !character->isAlive(); }),
        player->board.end());
  }
};

void Minion::play(Player *owner, Player *enemy) {
  (void)enemy;
  owner->board.push_back(new Character(name, hp, attack));
}

void Spell::play(Player *owner, Player *enemy) {
  (void)owner;
  enemy->hero->takeDamage(damage);
}

int main() {
  Character *hero1 = new Character("Valeera", 30, 0);
  Character *hero2 = new Character("Garrosh", 30, 0);

  Player *first = new Player(hero1);
  Player *second = new Player(hero2);

  first->deck.push_back(new Spell("Eviscerate", 2, 4));
  first->deck.push_back(new Minion("Boar", 1, 1, 1));
  first->deck.push_back(new Minion("Footman", 1, 2, 1));

  second->deck.push_back(new Spell("Fireball", 4, 6));
  second->deck.push_back(new Minion("Wolf", 1, 1, 1));
  second->deck.push_back(new Minion("Ogre", 4, 4, 4));

  GameState game(first, second);

  game.startTurn();
  game.printState();
  if (!game.currentPlayer()->hand.empty()) {
    game.playCard(game.currentPlayer()->hand[0]);
  }
  game.printState();
  game.endTurn();

  game.startTurn();
  game.printState();
  if (!game.currentPlayer()->hand.empty()) {
    game.playCard(game.currentPlayer()->hand[0]);
  }
  game.printState();
  game.endTurn();

  game.startTurn();
  if (!game.currentPlayer()->hand.empty()) {
    game.playCard(game.currentPlayer()->hand[0]);
  }
  if (!game.currentPlayer()->board.empty() && !game.enemyPlayer()->board.empty()) {
    game.attack(game.currentPlayer()->board[0], game.enemyPlayer()->board[0]);
  }
  game.printState();
  game.endTurn();

  return 0;
}
