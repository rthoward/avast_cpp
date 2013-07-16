#include "libtcod.hpp"
#include "actor.hpp"

Actor::Actor(int x, int y, int ch, const TCODColor &col) :
   x(x), y(y), ch(ch), color(col) {}

void Actor::render() const {
   TCODConsole::root->setChar(x, y, ch);
   TCODConsole::root->setCharForeground(x, y, color);
}

void Actor::move(int x, int y) {
   this->x += x;
   this->y += y;
}

void Actor::moveTo(int x, int y) {
   this->x = x;
   this->y = y;
}

int Actor::getX()   { return x; }
int Actor::getY()   { return y; }
