#pragma once
#include <algorithm>
struct ManaPool {
public:
  ManaPool(int maxMana) : MAX_MANA(maxMana) {}
  void refreshMana(int turn) { mana = std::min(10, turn); }
  std::size_t getMana() const { return mana; }

  bool reduceMana(size_t amount) {
    if (amount > mana)
      return false;

    mana -= amount;
    return true;
  }

private:
  friend class Player;
  friend void printHero();
  const int MAX_MANA;
  std::size_t mana = 0;
};
