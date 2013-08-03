class Pickable {

public:
   bool pick(Actor *me, Actor *wearer);
   virtual bool use(Actor *me, Actor *wearer);
};
