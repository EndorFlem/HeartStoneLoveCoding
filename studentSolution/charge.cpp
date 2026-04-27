#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Character {
  string name;
  int hp;
  int maxHp;
  int attack;
  bool hasCharge = false; 
  bool hasRush = false; 
  bool exhausted = false;
  bool summoningSickness = true;

  Character(string name, int hp, int attack)
      : name(name), hp(hp), maxHp(hp), attack(attack) {}

  bool isAlive() { return hp > 0; }
  bool isFullHp() { return hp == maxHp; }

  void takeDamage(int dmg) {
    hp -= dmg;
    if (hp < 0) hp = 0;
  }


  bool canAttack() {
    if (exhausted) return false;
    if (summoningSickness && !hasCharge && !hasRush) return false;
    return true;
  }


  bool canAttackTarget(bool isTargetMinion) {
    if (!canAttack()) return false;

    if (summoningSickness && hasRush && !isTargetMinion) return false;
    return true;
  }
};



struct Player;

struct Card {
  string name;
  int cost;
  Card(string name, int cost) : name(name), cost(cost) {}
  virtual ~Card() = default;
  virtual void play(Player *owner, Player *enemy) = 0;
};

struct MinionCard : public Card {
  int hp;
  int attack;
  bool hasCharge;
  bool hasRush;

  MinionCard(string name, int cost, int hp, int attack,
             bool hasCharge = false, bool hasRush = false)
      : Card(name, cost), hp(hp), attack(attack),
        hasCharge(hasCharge), hasRush(hasRush) {}

  void play(Player *owner, Player *enemy) override;
};

struct SpellCard : public Card {

  function<void(Player *owner, Player *enemy)> effect;

  SpellCard(string name, int cost,
            function<void(Player *owner, Player *enemy)> effect)
      : Card(name, cost), effect(effect) {}

  void play(Player *owner, Player *enemy) override {
    effect(owner, enemy);
  }
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
      cout << hero->name << " fatigue!\n";
      return;
    }
    hand.push_back(deck.back());
    deck.pop_back();
  }
};

void MinionCard::play(Player *owner, Player *enemy) {
  (void)enemy;
  Character *minion = new Character(name, hp, attack);
  minion->hasCharge = hasCharge;
  minion->hasRush = hasRush;

  owner->board.push_back(minion);
  cout << "  -> " << name << " призван на поле\n";
}


struct GameState {
  Player *players[2];
  int cur = 0;

  GameState(Player *p1, Player *p2) { players[0] = p1; players[1] = p2; }

  Player *me() { return players[cur]; }
  Player *enemy() { return players[1 - cur]; }

  void startTurn() {
    Player *p = me();
    p->maxMana = min(10, p->maxMana + 1);
    p->mana = p->maxMana;
    p->drawCard();

    p->hero->exhausted = false;
    for (auto *m : p->board) {
      m->exhausted = false;
      m->summoningSickness = false;
    }

    cout << "\n=== Ход " << p->hero->name
         << " | мана: " << p->mana << " ===\n";
  }

  void endTurn() {
    cout << "--- Конец хода " << me()->hero->name << " ---\n";
    cur = 1 - cur;
  }

  bool playCard(int handIndex) {
    Player *p = me();
    if (handIndex < 0 || handIndex >= (int)p->hand.size()) {
      cout << "Нет карты на позиции " << handIndex << "\n";
      return false;
    }
    Card *card = p->hand[handIndex];
    if (card->cost > p->mana) {
      cout << "Не хватает маны (" << card->cost << " нужно, " << p->mana << " есть)\n";
      return false;
    }
    p->mana -= card->cost;
    p->hand.erase(p->hand.begin() + handIndex);
    cout << p->hero->name << " разыгрывает " << card->name << "\n";
    card->play(p, enemy());
    removeDeadMinions();
    return true;
  }


  bool attackMinionVsMinion(int myMinionIdx, int enemyMinionIdx) {
    Player *p = me();
    Player *e = enemy();

    if (myMinionIdx >= (int)p->board.size() ||
        enemyMinionIdx >= (int)e->board.size()) {
      cout << "Неверный индекс\n";
      return false;
    }

    Character *attacker = p->board[myMinionIdx];
    Character *target = e->board[enemyMinionIdx];

    if (!attacker->canAttackTarget(true)) {
      if (attacker->exhausted)
        cout << attacker->name << " уже атаковал в этот ход\n";
      else
        cout << attacker->name << " не может атаковать (болезнь призыва)\n";
      return false;
    }

    doAttack(attacker, target);
    removeDeadMinions();
    return true;
  }

  bool attackMinionVsHero(int myMinionIdx) {
    Player *p = me();
    Character *attacker = p->board[myMinionIdx];
    Character *target = enemy()->hero;

    if (!attacker->canAttackTarget(false)) {
      if (attacker->exhausted)
        cout << attacker->name << " уже атаковал в этот ход\n";
      else if (attacker->summoningSickness && attacker->hasRush)
        cout << attacker->name << " (натиск) не может атаковать героя в ход призыва\n";
      else
        cout << attacker->name << " не может атаковать (болезнь призыва)\n";
      return false;
    }

    doAttack(attacker, target);
    removeDeadMinions();
    return true;
  }

  void doAttack(Character *attacker, Character *target) {
    cout << attacker->name << " атакует " << target->name << "\n";
    target->takeDamage(attacker->attack);
    attacker->takeDamage(target->attack);
    attacker->exhausted = true;

    cout << "  " << target->name << " hp: " << target->hp << "\n";
    cout << "  " << attacker->name << " hp: " << attacker->hp << "\n";
  }

  void removeDeadMinions() {
    for (Player *p : players) {
      p->board.erase(
          remove_if(p->board.begin(), p->board.end(),
                    [](Character *m) { return !m->isAlive(); }),
          p->board.end());
    }
    if (!enemy()->hero->isAlive()) {
      cout << "\n*** " << enemy()->hero->name << " погиб! Победа "
           << me()->hero->name << "! ***\n";
    }
  }

  void printState() {
    cout << "\n--- Доска ---\n";
    cout << "Противник: " << enemy()->hero->name
         << " hp:" << enemy()->hero->hp << "\n";
    for (int i = 0; i < (int)enemy()->board.size(); i++) {
      auto *m = enemy()->board[i];
      cout << "  [" << i << "] " << m->name << " "
           << m->attack << "/" << m->hp;
      if (m->hasCharge) cout << " [Рывок]";
      if (m->hasRush) cout << " [Натиск]";
      cout << "\n";
    }
    cout << "Я: " << me()->hero->name
         << " hp:" << me()->hero->hp
         << " мана:" << me()->mana << "/" << me()->maxMana << "\n";
    for (int i = 0; i < (int)me()->board.size(); i++) {
      auto *m = me()->board[i];
      cout << "  [" << i << "] " << m->name << " "
           << m->attack << "/" << m->hp;
      if (m->hasCharge) cout << " [Рывок]";
      if (m->hasRush) cout << " [Натиск]";
      if (m->exhausted) cout << " [устал]";
      if (m->summoningSickness && !m->hasCharge && !m->hasRush)
        cout << " [болезнь призыва]";
      cout << "\n";
    }
    cout << "Рука:\n";
    for (int i = 0; i < (int)me()->hand.size(); i++) {
      cout << "  [" << i << "] " << me()->hand[i]->name
           << " (" << me()->hand[i]->cost << " маны)\n";
    }
    cout << "-------------\n";
  }
};


struct GromCard : public Card {
  GromCard() : Card("Громмаш Адский Крик", 8) {}

  void play(Player *owner, Player *enemy) override {
    (void)enemy;
    Character *grom = new Character("Громмаш", 9, 4);
    grom->hasCharge = true;
    grom->summoningSickness = false;
    owner->board.push_back(grom);
    cout << "  -> Громмаш призван (4/9, Рывок)\n";
    cout << "  -> Если получит урон — атака станет 10\n";
  }
};

void applyGromBuff(Character *grom) {
  if (!grom->isFullHp() && grom->attack == 4) {
    grom->attack += 6;
    cout << "  -> Громмаш в ярости! Атака: " << grom->attack << "\n";
  }
}

struct BoarCard : public MinionCard {
  BoarCard() : MinionCard("Вепрь-камнеклык", 1, 1, 1, true, false) {}
};

struct WaspCard : public Card {
  WaspCard() : Card("Краснобрюхая оса", 2) {}

  void play(Player *owner, Player *enemy) override {
    (void)enemy;
    Character *wasp = new Character("Краснобрюхая оса", 3, 1);
    wasp->hasRush = true;
    owner->board.push_back(wasp);
    cout << "  -> Оса призвана (1/3, Натиск)\n";
    cout << "  -> Если получит урон — атака станет 4\n";
  }
};

void applyWaspBuff(Character *wasp) {
  if (!wasp->isFullHp() && wasp->attack == 1) {
    wasp->attack += 3;
    cout << "  -> Оса разъярена! Атака: " << wasp->attack << "\n";
  }
}


struct UnleashCard : public Card {
  UnleashCard() : Card("Спустить собак", 3) {}

  void play(Player *owner, Player *enemy) override {
    int count = enemy->board.size();
    cout << "  -> Спускаем " << count << " гончих!\n";
    for (int i = 0; i < count; i++) {
      Character *hound = new Character("Гончая", 1, 1);
      hound->hasCharge = true;
      hound->summoningSickness = false;
      owner->board.push_back(hound);
      cout << "  -> Гончая призвана (1/1, Рывок)\n";
    }
  }
};



int main() {
  Character *hero1 = new Character("Валира", 30, 0);
  Character *hero2 = new Character("Гаррош", 30, 0);

  Player *p1 = new Player(hero1);
  Player *p2 = new Player(hero2);

  p1->deck.push_back(new BoarCard());
  p1->deck.push_back(new WaspCard());
  p1->deck.push_back(new UnleashCard());
  p1->deck.push_back(new GromCard());

  p2->deck.push_back(new MinionCard("Пехотинец", 1, 2, 1));
  p2->deck.push_back(new MinionCard("Огр", 4, 4, 4));

  GameState game(p1, p2);


  game.startTurn();
  game.playCard(0);
  game.printState();
  game.endTurn();


  game.startTurn();
  game.playCard(0);
  game.printState();
  game.endTurn();


  game.startTurn();
  game.printState();
  game.playCard(0); 
  cout << "\nВепрь атакует героя сразу (Рывок):\n";
  game.attackMinionVsHero(0);
  game.printState();
  game.endTurn();

  
  game.startTurn();
  game.playCard(0);
  game.printState();
  game.endTurn();

  game.startTurn();
  game.printState();
  game.playCard(0); 
  cout << "\nОса пробует атаковать героя (Натиск — нельзя):\n";
  game.attackMinionVsHero((int)game.me()->board.size() - 1);
  cout << "\nОса атакует существо противника (Натиск — можно):\n";
  if (!game.enemy()->board.empty()) {
    game.attackMinionVsMinion((int)game.me()->board.size() - 1, 0);
   
    if (!game.me()->board.empty()) {
      applyWaspBuff(game.me()->board.back());
    }
  }
  game.printState();
  game.endTurn();


  game.startTurn();
  game.endTurn();


  game.startTurn();
  game.printState();
  game.playCard(0);
  game.printState();

  cout << "\nГончие атакуют существ противника:\n";
  while (!game.me()->board.empty() && !game.enemy()->board.empty()) {
    game.attackMinionVsMinion(0, 0);
  }

  game.printState();
  game.endTurn();

  return 0;
}
