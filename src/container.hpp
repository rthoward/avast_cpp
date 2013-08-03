class Container {

public:
   Container(int size);
   ~Container();

   bool add(Actor *actor);
   void remove(Actor *actor);

private:
   int size;
   TCODList<Actor *> inventory;
};
