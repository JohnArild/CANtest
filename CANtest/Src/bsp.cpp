#include "bsp.h"

int BSP_BluePill::initilize (){
    BSP_BluePill::MCUspeed_48MHz();
    BSP_BluePill::setup_GPIO ();
    BSP_BluePill::setup_CAN ();
    BSP_BluePill::setup_SPI ();
    return 0;
}

int BSP_BluePill::MCUspeed_48MHz (){
    return 0;
}

int BSP_BluePill::setup_GPIO (){
    return 0;    
}

int BSP_BluePill::setup_CAN (){
    return 0;    
}

int BSP_BluePill::setup_SPI (){
    return 0;    
}