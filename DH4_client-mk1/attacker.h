#ifndef ATTACKER_H
#define ATTACKER_H

#include "master.h"

class Missile
{
    private:	
		  uint8_t angle,
              x_pos,
              y_pos =63,
              v_0;
    public:
        Missile(uint8_t vel,uint8_t ang, uint8_t xp/*, uint8_t yp*/);
	      
	      uint8_t *send_arr( void );
          uint8_t getVel( void );
          uint8_t getX( void );
          uint8_t getY( void );
          uint8_t getAng( void );
          void setX( uint8_t x);
          void setY( uint8_t y );
          void setVel(uint8_t vel);
};

#endif
