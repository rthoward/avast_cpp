#include "libtcod.hpp"
#include "actor.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "pickable.hpp"
#include "container.hpp"
#include "engine.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "ai.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

static const int MONS_TRACKING_TURNS = 3;

AI::~AI() {};

PlayerAI::PlayerAI() {
   aiState = NORMAL;
}

void PlayerAI::update(Actor *me) {

   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   if (aiState != NORMAL) {
      switch (aiState) {
         case AUTORUN_LEFT    : handleActionKey(me, 'h'); break;
         case AUTORUN_DOWN    : handleActionKey(me, 'j'); break;
         case AUTORUN_UP      : handleActionKey(me, 'k'); break;
         case AUTORUN_RIGHT   : handleActionKey(me, 'l'); break;
         default              : break;
      }
   }

   int dx = 0, dy = 0;

   switch(engine.getLastKey().vk) {
      case TCODK_UP     : dy -= 1; break;
      case TCODK_DOWN   : dy += 1; break;
      case TCODK_LEFT   : dx -= 1; break;
      case TCODK_RIGHT  : dx += 1; break;
      case TCODK_CHAR   : handleActionKey(me, engine.getLastKey().c); break;
      default           : break;
   }

   if ( (dx + dy) != 0) {
      engine.setStatus(Engine::NEW_TURN);
      if (moveOrAttack(me, me->getX() + dx, me->getY() + dy)) {
         engine.getMap()->computeFov();
      }
   }
   
   if (aiState != NORMAL && shouldStop(me)) {
      aiState = NORMAL;
      string msg = "You stop moving.";
      engine.getGUI()->message(msg);
   }
}

void PlayerAI::handleActionKey(Actor *me, int ascii) {

   int dx = 0, dy = 0;
   string invalid  = "Not a recognized action.";
   Actor *item;

   switch (ascii) {
      case 'h'    : dx -= 1; break;
      case 'j'    : dy += 1; break;
      case 'k'    : dy -= 1; break;
      case 'l'    : dx += 1; break;
      case 'y'    : dx -= 1; dy -= 1; break;
      case 'u'    : dx += 1; dy -= 1; break;
      case 'b'    : dx -= 1; dy += 1; break;
      case 'n'    : dx += 1; dy += 1; break;
      case 'q'    : engine.setStatus(Engine::QUIT); break;
      case 'g'    : aiState = chooseAutorun(); break;
      case '<'    : tryStaircase(me, true); break;
      case '>'    : tryStaircase(me, false); break;
      case 'H'    : me->getDestructible()->heal(10); break;
      case 'i' :
         item = chooseFromInventory(me);
         if (item) {
            item->getPickable()->use(item, me);
            engine.setStatus(Engine::NEW_TURN);
         }
         break;
      case ',' :
         me->tryPickUp(me, engine.getMap()->getItemAt(me->getX(), me->getY()));
         break;
      default  :
         engine.getGUI()->message(invalid, TCODColor::lightRed);
         break;
   }

   if ( dx != 0 || dy != 0) {
      engine.setStatus(Engine::NEW_TURN);
      if (moveOrAttack(me, me->getX() + dx, me->getY() + dy)) {
         engine.getMap()->computeFov();
      }
   }
}

bool PlayerAI::moveOrAttack(Actor *me, int targetx, int targety) {

   // stop player from running in to wall
   if (engine.getMap()->isWall(targetx, targety))
      return false;

   Actor *actor = engine.getMap()->getActorAt(targetx, targety);
   if (actor == NULL) {
      me->moveTo(targetx, targety);
      return true;
   }
   else if (actor->getType() == Actor::MONSTER) {
      me->getAttacker()->attack(me, actor);
      return false;
   } else {
      checkTile(actor);
      me->moveTo(targetx, targety);
      return true;
   }

   return false;
}

void PlayerAI::checkTile(Actor *actor) {

   string msg = "";

   switch (actor->getType()) {
      case Actor::ITEM:
         msg = "You see here a "; msg += actor->getName() + ".";
         engine.getGUI()->message(msg);
         break;
      case Actor::CORPSE:
         msg = "There is a "; msg += actor->getName() + " here.";
         engine.getGUI()->message(msg);
         break;
      default: break;
   }
}

Actor *PlayerAI::chooseFromInventory(Actor *me) {
   static const int INV_WIDTH = 50;
   static const int INV_HEIGHT = 28;
   static TCODConsole con(INV_WIDTH, INV_HEIGHT);

   con.setDefaultForeground(TCODColor(200, 180, 50));
   con.printFrame(0, 0, INV_WIDTH, INV_HEIGHT, true, TCOD_BKGND_DEFAULT,
         "inventory");
   con.setDefaultForeground(TCODColor::white);
   int shortcut = 'a';
   int y = 1;

   TCODList<Actor *> inventory = me->getContainer()->getInventory();
   Actor *item;

   for (Actor **iter = inventory.begin(); iter != inventory.end(); iter++) {
      item = *iter;
      con.print(2, y, "(%c) - %s", shortcut, item->getName().c_str());
      y++;
      shortcut++;
   }

   TCODConsole::blit(&con, 0, 0, INV_WIDTH, INV_HEIGHT, TCODConsole::root,
         engine.getScreenWidth() / 2 - INV_WIDTH / 2,
         engine.getScreenHeight() / 2 - INV_HEIGHT / 2);
   TCODConsole::flush();

   TCOD_key_t key;
   TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

   if (key.vk == TCODK_CHAR) {
      int itemIndex = key.c - 'a';
      if (itemIndex >= 0 && itemIndex < inventory.size())
         return inventory.get(itemIndex);
   }

   return NULL;
}

PlayerAI::AIState PlayerAI::chooseAutorun() {
   string msg = "Which direction do you want to run to?"; 
   engine.getGUI()->message(msg);

   TCOD_key_t key;
   TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL, true);

   if (key.vk == TCODK_CHAR) {
      int dir = key.c;
      switch (dir) {
         case 'h' : return AUTORUN_LEFT; break;
         case 'j' : return AUTORUN_DOWN; break;
         case 'k' : return AUTORUN_UP; break;
         case 'l' : return AUTORUN_RIGHT; break;
         default  : 
            string msg = "Not a valid direction";
            engine.getGUI()->message(msg);
            return NORMAL;
            break;
      }
      
   } else {
      string msg = "Not a valid direction";
      return NORMAL;
   }
}

bool PlayerAI::shouldStop(Actor *me) {
   TCODList<Actor *> actors = engine.getActorList();

   // check if any potential attackers entered FOV
   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (actor == NULL)                     continue;
      if (actor == me)                       continue;
      if ( !(actor->getAttacker()) )         continue;
      if (actor->isDead())                   continue;
      if (engine.getMap()->isInFov(actor->getX(), actor->getY()))
         return true;
   }

   int x = me->getX(), y = me->getY();
   Map *map = engine.getMap();

   // check if next tile is obstructed
   switch (aiState) {
      case AUTORUN_LEFT    : return !(map->canWalk(x - 1, y)); break;
      case AUTORUN_RIGHT   : return !(map->canWalk(x + 1, y)); break;
      case AUTORUN_UP      : return !(map->canWalk(x, y - 1)); break;
      case AUTORUN_DOWN    : return !(map->canWalk(x, y + 1)); break;
      default              : return false;
   }
}

void PlayerAI::tryStaircase(Actor *me, bool up) {
   Actor *standing = engine.getMap()->getActorAt(me->getX(), me->getY());

   if (up && standing->getChar() == '<') {
      engine.upLevel();
      engine.getMap()->computeFov();
   }
   else if (!up && standing->getChar() == '>') {
      engine.downLevel();
      engine.getMap()->computeFov();
   }
}

// monster AI -----------------------------------------------------------------

MonsterAI::MonsterAI() : moveCount(3) {};

void MonsterAI::update(Actor *me) {
   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   // if player is in fov, move toward him / her
   if (engine.getMap()->isInFov(me->getX(), me->getY())) {
      moveCount = MONS_TRACKING_TURNS;
   } else {
      moveCount--;
   }

   if (moveCount > 0) {
      Actor *player = engine.getPlayer();
      moveOrAttack(me, player->getX(), player->getY());
   }
}

void MonsterAI::moveOrAttack(Actor *me, int targetx, int targety) {
   int dx = targetx - me->getX();
   int dy = targety - me->getY();
   int stepdx = (dx > 0 ? 1 : -1);
   int stepdy = (dy > 0 ? 1 : -1);
   float distance = sqrtf(dx * dx + dy * dy);

   // if out of melee range, move toward player
   if (distance >= 2) {
      // normalize vector
      dx = (int)(round(dx / distance));
      dy = (int)(round(dy / distance));

      if (engine.getMap()->canWalk(me->getX() + dx, me->getY() + dy)) {
         me->moveTo(me->getX() + dx, me->getY() + dy);
      } else if (engine.getMap()->canWalk(me->getX() + stepdx, me->getY())) {
         me->moveTo(me->getX() + stepdx, me->getY());
      } else if (engine.getMap()->canWalk(me->getX(), me->getY() + stepdy)) {
         me->moveTo(me->getX(), me->getY() + stepdy);
      }
   } else if (me->getAttacker()) {
      me->getAttacker()->attack(me, engine.getPlayer());
   }

}
