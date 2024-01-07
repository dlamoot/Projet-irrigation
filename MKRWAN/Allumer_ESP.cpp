//
//  Allumer_ESP.cpp
//  
//
//  Created by David Lamoot on 17/12/2023.
//

#include "Allumer_ESP.hpp"
#include "Arduino.h"

#define enableESP 10

void allumer_ESP(){
    digitalWrite(enableESP,HIGH); //Régulateur alimente l'ESP
    delay(60000);
    digitalWrite(enableESP,LOW); //Régulateur n'alimente plus l'ESP
}
