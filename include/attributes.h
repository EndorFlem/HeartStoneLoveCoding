#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

class Attribute {
public:
  virtual ~Attribute() = default;
};

class Taunt    : public Attribute {};
class Elusive  : public Attribute {};
class Poison   : public Attribute {};

class SpellDamage : public Attribute {
public:
  SpellDamage(size_t bonus) : bonus(bonus) {}
  size_t bonus;
};

class GameState;
class Attributable;

class BattleCry : public Attribute {
public:
  using Fn = std::function<void(GameState *state, Attributable *self)>;
  BattleCry(Fn fn) : fn(fn) {}
  void trigger(GameState *state, Attributable *self) { if (fn) fn(state, self); }
private:
  Fn fn;
};

class Deathrattle : public Attribute {
public:
  using Fn = std::function<void(GameState *state, Attributable *self)>;
  Deathrattle(Fn fn) : fn(fn) {}
  void trigger(GameState *state, Attributable *self) { if (fn) fn(state, self); }
private:
  Fn fn;
};

template<typename T>
concept AttributeType = std::derived_from<T, Attribute>;

class Attributable {
public:
  virtual ~Attributable() = default;
  Attributable(std::vector<Attribute *> attributes) : attributes(attributes) {}

  template<AttributeType Attr>
  bool containsAttribute() {
    for (Attribute *attr : attributes)
      if (dynamic_cast<Attr *>(attr)) return true;
    return false;
  }

  template<AttributeType Attr>
  Attr *getAttribute() {
    for (Attribute *attr : attributes)
      if (auto *a = dynamic_cast<Attr *>(attr)) return a;
    return nullptr;
  }

  void triggerOnPlay(GameState *state) {
    for (Attribute *attr : attributes)
      if (auto *a = dynamic_cast<BattleCry *>(attr)) a->trigger(state, this);
  }

  void triggerOnDeath(GameState *state) {
    for (Attribute *attr : attributes)
      if (auto *a = dynamic_cast<Deathrattle *>(attr)) a->trigger(state, this);
  }

  bool containsTaunt()      { return containsAttribute<Taunt>(); }
  bool containsElusive()    { return containsAttribute<Elusive>(); }
  bool containsDeathratle() { return containsAttribute<Deathrattle>(); }
  bool containsPoison()     { return containsAttribute<Poison>(); }

  std::vector<Attribute *> attributes;
};
