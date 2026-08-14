#include "core/HomeManager.h"
#include "devices/Devices.h"
#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "patterns/SensorEvents.h"
#include "patterns/SmartRoutine.h"
#include "patterns/SecuritySystem.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <stdexcept>

// Helper for safe integer input
int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Entrada invalida. Por favor ingrese un numero.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string getStringInput(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) return value;
        std::cout << "El valor no puede estar vacio.\n";
    }
}

void deviceMenu(HomeManager* hub) {
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "1. GESTION DE DISPOSITIVOS\n";
        std::cout << "==================================================\n";
        std::cout << "1. Crear Smart Light\n";
        std::cout << "2. Crear Smart Lock\n";
        std::cout << "3. Crear High Security Lock\n";
        std::cout << "4. Crear Zigbee Relay\n";
        std::cout << "5. Encender dispositivo\n";
        std::cout << "6. Apagar dispositivo\n";
        std::cout << "7. Volver\n";
        
        int opt = getIntInput("Opcion: ");
        if (opt == 7) break;
        
        if (opt >= 1 && opt <= 4) {
            std::string id = getStringInput("ID: ");
            std::string name = getStringInput("Nombre: ");
            
            std::shared_ptr<IDevice> newDevice;
            try {
                if (opt == 1) {
                    WiFiLightProvisioner prov;
                    newDevice = prov.provisionDevice(name);
                } else if (opt == 2) {
                    WiFiLockProvisioner prov;
                    newDevice = prov.provisionDevice(name);
                } else if (opt == 3) {
                    HighSecurityLockProvisioner prov;
                    newDevice = prov.provisionDevice(name);
                } else if (opt == 4) {
                    ZigbeeRelayProvisioner prov;
                    newDevice = prov.provisionDevice(name);
                }
                
                if (newDevice) {
                    hub->addDevice(id, newDevice);
                    std::cout << "Dispositivo creado y agregado con exito.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error al agregar dispositivo: " << e.what() << "\n";
            }
        } else if (opt == 5 || opt == 6) {
            std::string id = getStringInput("ID del dispositivo: ");
            try {
                auto dev = hub->getDevice(id);
                if (dev) {
                    if (opt == 5) dev->turnOn();
                    else dev->turnOff();
                } else {
                    std::cout << "No existe un dispositivo con ese ID.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
}

void securityMenu(HomeManager* hub, std::shared_ptr<MotionSensor> sensor) {
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "--- SEGURIDAD ---\n";
        std::cout << "Estado actual: [No disponible a traves de la API]\n";
        std::cout << "1. Activar Armed Mode\n";
        std::cout << "2. Activar Night Mode\n";
        std::cout << "3. Activar Disarmed Mode\n";
        std::cout << "4. Simular evento de sensor\n";
        std::cout << "5. Volver\n";
        
        int opt = getIntInput("Opcion: ");
        if (opt == 5) break;
        
        auto secSys = hub->getSecuritySystem();
        if (!secSys) {
            std::cout << "El sistema de seguridad no esta disponible.\n";
            continue;
        }
        
        try {
            if (opt == 1) {
                secSys->setMode(std::make_unique<ArmedMode>());
                std::cout << "Modo Armed activado.\n";
            } else if (opt == 2) {
                secSys->setMode(std::make_unique<NightMode>());
                std::cout << "Modo Night activado.\n";
            } else if (opt == 3) {
                secSys->setMode(std::make_unique<DisarmedMode>());
                std::cout << "Modo Disarmed activado.\n";
            } else if (opt == 4) {
                if (sensor) sensor->detectMotion();
            } else {
                std::cout << "Opcion invalida.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void routineMenu(HomeManager* hub) {
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "--- RUTINAS ---\n";
        std::cout << "1. Ejecutar Morning Routine\n";
        std::cout << "2. Ejecutar Night Routine\n";
        std::cout << "3. Volver\n";
        
        int opt = getIntInput("Opcion: ");
        if (opt == 3) break;
        
        try {
            if (opt == 1) {
                MorningRoutine routine(hub);
                routine.run();
            } else if (opt == 2) {
                NightRoutine routine(hub);
                routine.run();
            } else {
                std::cout << "Opcion invalida.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void sensorMenu(std::shared_ptr<MotionSensor> sensor) {
    while (true) {
        std::cout << "\n==================================================\n";
        std::cout << "--- SENSORES ---\n";
        std::cout << "1. Simular movimiento\n";
        std::cout << "2. Volver\n";
        
        int opt = getIntInput("Opcion: ");
        if (opt == 2) break;
        
        if (opt == 1) {
            try {
                if (sensor) sensor->detectMotion();
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
}

void showSystemStatus(HomeManager* hub) {
    std::cout << "\n==================================================\n";
    std::cout << "--- ESTADO DEL SISTEMA ---\n";
    std::cout << "Cantidad de dispositivos registrados: [No disponible a traves de la API]\n";
    std::cout << "Estado del SecuritySystem: [No disponible a traves de la API]\n";
    std::cout << "Informacion general: La API actual no expone metodos publicos para consultar el estado interno.\n";
    std::cout << "==================================================\n";
}

int main() {
    HomeManager* hub = HomeManager::getInstance();
    
    // Configurar sensor base para simulaciones
    auto hallwayMotionSensor = std::make_shared<MotionSensor>("Sensor Movimiento Principal");
    hallwayMotionSensor->addListener(hub);

    while (true) {
        std::cout << "\n========================================\n";
        std::cout << "SMART HOME OS\n";
        std::cout << "=============\n";
        std::cout << "1. Gestionar dispositivos\n";
        std::cout << "2. Gestionar seguridad\n";
        std::cout << "3. Gestionar rutinas\n";
        std::cout << "4. Simular sensores\n";
        std::cout << "5. Estado del sistema\n";
        std::cout << "6. Salir\n";
        
        int opt = getIntInput("Opcion: ");
        
        if (opt == 6 || opt == 0) {
            std::cout << "Saliendo del sistema...\n";
            break;
        }
        
        switch (opt) {
            case 1:
                deviceMenu(hub);
                break;
            case 2:
                securityMenu(hub, hallwayMotionSensor);
                break;
            case 3:
                routineMenu(hub);
                break;
            case 4:
                sensorMenu(hallwayMotionSensor);
                break;
            case 5:
                showSystemStatus(hub);
                break;
            default:
                std::cout << "Opcion invalida.\n";
        }
    }
    
    return 0;
}
