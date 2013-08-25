#ifndef __PLAYER_STAT_HPP_
#define __PLAYER_STAT_HPP__

class PlayerStat {

public:
   PlayerStat();

   void update();

   void setTelepathic(int duration);
   void setBlind(int duration);

   bool isTelepathic() const;
   bool isBlind() const;
private:
   int telepathy;
   int blind;

};

#endif