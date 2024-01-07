//
//  Electrovanne.cpp
//  
//
//  Created by David Lamoot on 28/11/2023.
//

#include "Electrovanne.hpp"
#include "Arduino.h"

int enablePin = 0;
int phasePin = 1;

void setup_electrovanne(){
    pinMode(enablePin, INPUT);
    pinMode(phasePin, OUTPUT);
    digitalWrite(enablePin, HIGH);
    digitalWrite(phasePin, HIGH);
}

void ouverture_fermeture(int duree_ouverture){
    /*digitalWrite(enablePin, HIGH);
    digitalWrite(phasePin, HIGH);
    delay(100);*/
    digitalWrite(enablePin, HIGH);
    delay(2000);
    digitalWrite(phasePin, LOW);
    digitalWrite(enablePin, LOW);
    delay(2000);
    digitalWrite(enablePin, HIGH);
    delay((long)duree_ouverture*1000); //5 secondes
    digitalWrite(phasePin, HIGH);
    delay(2000);
    digitalWrite(enablePin, LOW);
    digitalWrite(phasePin, HIGH);
}
