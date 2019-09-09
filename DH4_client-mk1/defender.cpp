#include "defender.h"

Base::Base(uint8_t hp1,uint8_t hp2,uint8_t hp3)
{
   baseHealth1=hp1 &0x0F;
   baseHealth2=hp2 &0x0F;
   baseHealth3=hp3 &0x0F;
}
uint8_t *Base::send_arr( void )
{
    uint8_t arr[] = {this->baseHealth1,this->baseHealth2,this->baseHealth3};
    return arr;
}

uint8_t Base::getBaseHP(uint8_t base)
{
  switch( base & 0xF0 )
  {
    case BASE_L:
      return baseHealth1 & 0x0F;
      break;
    case BASE_C:
      return baseHealth2 & 0x0F;
      break;
    case BASE_R:
      return baseHealth3 & 0x0F;
      break;
  }
}


uint8_t Base::attacc_base( uint8_t base_num ) //pass a define BASE_x
{
  uint8_t hp_num;
  switch (base_num)
  {
    case BASE_L:
      if(baseHealth1!=0)
        baseHealth1-=1;
      hp_num = this->baseHealth1;
      break;
    case BASE_C:
      if(baseHealth2!=0)
        baseHealth2-=1;
      hp_num = this->baseHealth2;
      break;
    case BASE_R:
      if(baseHealth3!=0)
        baseHealth3-=1;
      hp_num = this->baseHealth3;
      break;
  }
  return hp_num;
//  switch (hp_num)
//  {
//    case 0x00: //base already destroyed
//    case 0x01: //25% hp
//      return (base_num | 0x00); //destroyed now
//      break;
//    case 0x02: //50% hp
//      return (base_num | 0x01); //now 25% hp
//      break;
//    case 0x03: //75% hp
//      return (base_num | 0x02); //now 50% hp
//      break;
//    case 0x04: //full hp
//      return (base_num | 0x03); //now 75% hp
//      break;
//  }
}

//uint8_t Base::get_base_state( uint8_t base_num )
//{
//  uint8_t hp_num = 0x00;
//  switch (base_num)
//  {
//    case BASE_L:
//      hp_num = this->baseHealth1; 
//      break;
//    case BASE_C:
//      hp_num = this->baseHealth2;
//      break;
//    case BASE_R:
//      hp_num = this->baseHealth3;
//      break;
//  }
//  return hp_num;
//}

void Base::set_damage( uint8_t base_num, uint8_t current_hp )
{
  switch(base_num)
  {
    case BASE_L:
      this->baseHealth1 = current_hp;
      break;
    case BASE_C:
      this->baseHealth2 = current_hp;
      break;
    case BASE_R:
      this->baseHealth3 = current_hp;
      break;
  }
}
