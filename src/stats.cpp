#include "stats.hpp"

Stats::Stats(int str, int con, int dex) {
   this->str = str;
   this->con = con;
   this->dex = dex;
}

int Stats::getStr() const              { return this->str; }
int Stats::getCon() const              { return this->con; }
int Stats::getDex() const              { return this->dex; }

void Stats::setStr(int str)            { this->str = str; }
void Stats::setCon(int con)            { this->con = con; }
void Stats::setDex(int dex)            { this->dex = dex; }