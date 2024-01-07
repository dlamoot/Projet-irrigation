//
//  Air_Hum_temp.cpp
//  
//
//  Created by David Lamoot on 28/11/2023.
//

#include "Air_Hum_temp.hpp"
#include "DHT.h"

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht (DHTPIN, DHTTYPE);

void setup_dht(){
    dht.begin();
}

float humidity_dht(){
    float hum_air = dht.readHumidity()*100;
    return hum_air;
}

float temp_dht(){
    float temp_air = dht.readTemperature()*100;
    return temp_air;
}
