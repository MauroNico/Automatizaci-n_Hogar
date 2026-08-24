#include "core/HomeManager.h"
#include "patterns/SensorEvents.h"
#include "ui/ConsoleUI.h"

#include <iostream>
#include <memory>
#include <exception>

int main() {
    try {
        
        HomeManager* homeManager = HomeManager::getInstance();
        
        auto hallwayMotionSensor = std::make_shared<MotionSensor>("Sensor Movimiento Principal");
        hallwayMotionSensor->addListener(homeManager);

        
        UI::run(homeManager, hallwayMotionSensor);

        HomeManager::destroyInstance();

    } catch (const std::exception& error) {
        
        std::cerr << "\n[Error Critico] Excepcion no controlada: " << error.what() << "\n";
        HomeManager::destroyInstance();
        return 1;
    } catch (...) {
        std::cerr << "\n[Error Critico] Ocurrio un error desconocido en el sistema.\n";
        HomeManager::destroyInstance();
        return 1;
    }
    
    return 0;
}