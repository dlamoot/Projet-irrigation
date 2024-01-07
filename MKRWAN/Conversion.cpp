//
//  Conversion.cpp
//  
//
//  Created by David Lamoot on 17/12/2023.
//

#include "Conversion.hpp"

int minutesToMilliseconds(int minutes){
    return minutes*60*1000;
}

int hoursToMilliseconds(int hours){
    return hours*60*60*1000;
}
