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

void PlayerAI::update(Actor *me) {

   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

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
