#include "patterns/SmartRoutine.h"
#include <iostream>

void MorningRoutine::configureLighting() {
    std::cout << "MorningRoutine: Optimizando luz natural y apagando luces exteriores...\n";
    auto outdoorGroup = hub->getDevice("GrupoExterior");
    if (outdoorGroup) outdoorGroup->turnOff();
    
    auto indoorGroup = hub->getDevice("LuzCocina");
    if (indoorGroup) indoorGroup->turnOn();
}

void MorningRoutine::configureSecurity() {
    std::cout << "MorningRoutine: Abriendo la casa para el dia...\n";
    hub->getSecuritySystem()->setDisarmedMode();
    hub->unlockAllDoors();
}

void NightRoutine::configureLighting() {
    std::cout << "NightRoutine: Apagando iluminacion de la casa...\n";
    hub->turnOffAll();
}

void NightRoutine::configureSecurity() {
    std::cout << "NightRoutine: Activando proteccion perimetral...\n";
    hub->lockAllDoors();
    hub->getSecuritySystem()->setNightMode();
}
