#pragma once

#include "card.h"
#include <cstddef>
#include <iostream>
#include <vector>

class Hand {
  static constexpr std::size_t MAX_HAND_SIZE = 10;

public:
  Hand() = default;
  explicit Hand(std::vector<Card *> cards) : cards(cards) {}

  bool addCard(Card *card) {
    if (isFull()) {
      return false;
    }

    cards.push_back(card);
    return true;
  }

  void printCardInHand(Card *card, size_t position) const {
    std::cout << "[" << position << "] Name: " << card->getName()
              << " | Cost: " << card->getCost() << "\n";
  }

  void print() const {
    std::cout << "HAND: \n";
    for (size_t i = 0; i < size(); i++) {
      printCardInHand(at(i), i);
    }
  }

  Card *at(std::size_t index) const { return cards.at(index); }

  Card *removeAt(std::size_t index) {
    Card *card = cards.at(index);
    cards.erase(cards.begin() + index);
    return card;
  }

  std::size_t size() const { return cards.size(); }
  bool empty() const { return cards.empty(); }
  bool isFull() const { return cards.size() >= MAX_HAND_SIZE; }

  const std::vector<Card *> &getCards() const { return cards; }

private:
  std::vector<Card *> cards;
  friend class HandSelector;
};
