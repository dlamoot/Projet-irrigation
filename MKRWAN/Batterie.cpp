//
//  Batterie.cpp
//  
//
//  Created by David Lamoot on 28/11/2023.
//

#include "Batterie.hpp"
#include "Arduino.h"

const int BATTERYPIN = A0; //pin de la batterie
const float TensionMin = 2.35; //tension min
const float TensionMax = 3.3; //tension max

int battery (){
  float b = analogRead(BATTERYPIN); //valeur analogique
  float tension = (b/1023.0)*3.3 ;
  
  // int minValue = (TensionMin/1023)*3.3; //Arduino
  //int maxValue = (TensionMax/1023)*3.3; //Arduino

  int minValue = (1023 * TensionMin) / 3.3; //Arduino
  int maxValue = (1023 * TensionMax) / 3.3; //Arduino

  int pourcentage = ((b - minValue) / (maxValue - minValue)) * 100;

  if (pourcentage > 100) //max is 100%
    pourcentage = 100;

  else if (pourcentage < 0) //min is 0%
    pourcentage = 0;

  return pourcentage*100;
}