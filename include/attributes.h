#pragma once

#include <iostream>
#include <vector>

class Attribute {
public:
  virtual ~Attribute() = default;
};

class Taunt : public Attribute {};
class Elusive : public Attribute {};
class Deathratle : public Attribute {};
class Poison : public Attribute {};

// очев тут templates , но это самый простой метод
class Attributable {
public:
  bool containsTaunt() {
    for (Attribute *attr : attributes) {
      if (dynamic_cast<Taunt *>(attr)) {
        return true;
      }
    }

    return false;
  };

  bool containsElusive() {
    for (Attribute *attr : attributes) {
      if (dynamic_cast<Elusive *>(attr)) {
        return true;
      }
    }

    return false;
  }

  bool containsDeathratle() {
    for (Attribute *attr : attributes) {
      if (dynamic_cast<Deathratle *>(attr)) {
        return true;
      }
    }

    return false;
  }

  void triggerDeathratles() {
    for (Attribute *attr : attributes) {
      if (dynamic_cast<Deathratle *>(attr)) {
        std::cout << "Deathratle triggered \n";
      }
    }
  }

  bool containsPoison() {
    for (Attribute *attr : attributes) {
      if (dynamic_cast<Poison *>(attr)) {
        return true;
      }
    }

    return false;
  }
  Attributable(std::vector<Attribute *> attributes) : attributes(attributes) {}
  std::vector<Attribute *> attributes;
};
