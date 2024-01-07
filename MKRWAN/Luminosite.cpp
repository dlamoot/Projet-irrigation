//
//  Luminosite.cpp
//  
//
//  Created by David Lamoot on 28/11/2023.
//

#include "Luminosite.hpp"
#include "MKRWAN.h"
#include <DFRobot_B_LUX_V30B.h>
DFRobot_B_LUX_V30B    myLux(13, 6, 7);//The sensor chip is set to 13 pins, SCL and SDA adopt personal configuration

void setup_lum(){
    myLux.begin();
}

float lum(){
  return myLux.lightStrengthLux();
}
