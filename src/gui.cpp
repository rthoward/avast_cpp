#include "libtcod.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "destructible.hpp"
#include "engine.hpp"
#include <stdarg.h>

static const int PANEL_HEIGHT = 7;
static const int BAR_WIDTH = 20;
static const int MSG_X = BAR_WIDTH + 2;
static const int MSG_HEIGHT = PANEL_HEIGHT - 1;

GUI::GUI() {
   con = new TCODConsole(engine.getScreenWidth(), PANEL_HEIGHT);
}

GUI::~GUI() {
   delete con;
   log.clearAndDelete();
}

void GUI::render() {
   con->setDefaultBackground(TCODColor::black);
   con->clear();
   
   // draw health bar
   renderBar(1, 1, BAR_WIDTH, "HP", engine.getPlayer()->getDestructible()->getHP(),
         engine.getPlayer()->getDestructible()->getHPMax(), TCODColor::lightRed,
         TCODColor::darkerRed);

   int msg_num = 1;
   float colorCoef = 0.4f;

   // draw messages
   for (Message **iter = log.begin(); iter != log.end(); iter++) {
      Message *message = *iter;
      con->setDefaultForeground(message->col * colorCoef);
      con->print(MSG_X, msg_num, message->text.c_str());
      msg_num++;

      if (colorCoef < 1.0f)      colorCoef += 0.3f;
   }

   TCODConsole::blit(con, 0, 0, engine.getScreenWidth(), PANEL_HEIGHT,
         TCODConsole::root, 0, engine.getScreenHeight() - PANEL_HEIGHT);

}

void GUI::renderBar(int x, int y, int width, string name, float value,
      float maxValue, const TCODColor &barCol, const TCODColor &backCol) {

   // bar background color
   con->setDefaultBackground(backCol);
   con->rect(x, y, width, 1, false, TCOD_BKGND_SET);

   // determine how much of bar should be filled with color
   int barWidth = (int)(value / maxValue * width);
   if (barWidth > 0) {
      // draw bar
      con->setDefaultBackground(barCol);
      con->rect(x, y, barWidth, 1, false, TCOD_BKGND_SET);
   }

   con->setDefaultForeground(TCODColor::white);
   con->printEx(x + width / 2, y, TCOD_BKGND_NONE, TCOD_CENTER,
         "%s : %g / %g", name.c_str(), value, maxValue);
}

void GUI::message(const TCODColor &col, const char *text, ...) {

}

GUI::Message::Message(string text, const TCODColor &col) :
   text(text), col(col) {

}

GUI::Message::~Message() {}
