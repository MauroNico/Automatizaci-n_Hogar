#include "patterns/SmartRoutine.h"
#include <iostream>

void MorningRoutine::configureLighting() {
    std::cout << "MorningRoutine: Optimizando luz natural y apagando luces exteriores...\n";
    auto outdoorGroup = hub->getDevice(outdoorLightsId);
    if (outdoorGroup) outdoorGroup->turnOff();
    
    auto indoorGroup = hub->getDevice(indoorLightsId);
    if (indoorGroup) indoorGroup->turnOn();
}

void MorningRoutine::configureSecurity() {
    std::cout << "MorningRoutine: Abriendo la casa para el dia...\n";
    hub->getSecuritySystem()->setDisarmedMode();
    auto doors = hub->getDevice(doorsGroupId);
    if (doors) doors->turnOff();
}

void NightRoutine::configureLighting() {
    std::cout << "NightRoutine: Apagando iluminacion de la casa...\n";
    hub->turnOffAll();
}

void NightRoutine::configureSecurity() {
    std::cout << "NightRoutine: Activando proteccion perimetral...\n";
    auto doors = hub->getDevice(doorsGroupId);
    if (doors) doors->turnOn();
    hub->getSecuritySystem()->setNightMode();
}
