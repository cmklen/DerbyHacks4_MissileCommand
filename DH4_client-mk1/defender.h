#ifndef DEFENDER_H
#define DEFENDER_H

#include "master.h"


class Base
{
       
    private:
        uint8_t baseHealth1,baseHealth2,baseHealth3;
    public:
        Base(uint8_t hp1,uint8_t hp2,uint8_t hp3);
        uint8_t* send_arr( void );
        uint8_t getBaseHP(uint8_t base);
        uint8_t attacc_base( uint8_t base_num );
        void set_damage( uint8_t base_num, uint8_t current_hp );
//        uint8_t get_base_state( uint8_t base_num );
};



#endif
