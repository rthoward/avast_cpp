#ifndef __STATS_HPP__
#define __STATS_HPP__

class Stats {
public:
   Stats(int str = 1, int con = 1, int dex = 1);

   int getStr() const;
   int getCon() const;
   int getDex() const;

   void setStr(int str);
   void setCon(int con);
   void setDex(int dex);

private:
   int str;
   int con;
   int dex;
};

#endif