//
//  Hum_sol.cpp
//  
//
//  Created by David Lamoot on 28/11/2023.
//

#include "Hum_sol.hpp"
#include <MKRWAN.h>

const int HumiditeAir = 754;
const int HumiditeEau = 314;
const int Diff_Air_Eau = HumiditeAir-HumiditeEau;
int humiditeSol = 0;

float hum_sol(){
    humiditeSol = analogRead(A1);
    //float hum = ((float)(Diff_Air_Eau - (humiditeSol - Diff_Air_Eau))/Diff_Air_Eau) *1000;
    float hum=((float)(humiditeSol-HumiditeEau))/((float)(HumiditeAir-HumiditeEau));
    float humidite = 1-hum;
    humidite *=1000;
    if ((humidite/10) > 100){
      humidite = 1000.0;
    }
    else if ((humidite/10) < 0){
      humidite = 0.0;
    }
    return humidite;
}
