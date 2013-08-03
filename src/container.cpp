#include "libtcod.hpp"
#include "actor.hpp"
#include "container.hpp"

Container::Container(int size) : size(size) {

}

Container::~Container() {
   inventory.clearAndDelete();
}

bool Container::add(Actor *actor) {

   // make sure inventory isn't full
   if (size > 0 && inventory.size() >= size)
      return false;

   inventory.push(actor);
   return true;
}

void Container::remove(Actor *actor) {
   inventory.remove(actor);
}
