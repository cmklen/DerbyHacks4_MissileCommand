#include "attacker.h"


Missile::Missile(uint8_t vel,uint8_t ang, uint8_t xp/*, uint8_t yp*/)
{
    v_0   = vel;
    angle = ang;
    x_pos = xp;
//    y_pos = yp;
}

uint8_t *Missile::send_arr( void )
{
  uint8_t to_ret[] = {this->x_pos, this->angle, this->v_0};
  return to_ret;
}

uint8_t Missile::getVel()
{
  return v_0;
}

 uint8_t Missile::getX( void )
 {
   return x_pos;
 }
 uint8_t Missile::getY( void )
 {
   return x_pos;
 }
 void Missile::setX( uint8_t x )
 {
    x_pos = x;
 }
void Missile::setY( uint8_t y )
{
    y_pos =y;
}
void Missile::setVel(uint8_t vel)
{
    v_0 = vel;
}
uint8_t Missile::getAng( void )
{
  return angle;
}