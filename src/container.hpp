#ifndef __CONTAINER_HPP__
#define __CONTAINER_HPP__

class Container {

public:
   Container(int size);
   ~Container();

   bool add(Actor *actor);
   void remove(Actor *actor);

   TCODList<Actor *> getInventory();

private:
   int size;
   TCODList<Actor *> inventory;
};

#endif
