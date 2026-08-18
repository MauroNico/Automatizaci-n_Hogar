#include "ui/ConsoleUI.h"

#include "core/HomeManager.h"
#include "devices/Devices.h"
#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "patterns/SensorEvents.h"
#include "patterns/SmartRoutine.h"
#include "patterns/SecuritySystem.h"

#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>

namespace {
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
void printHeader(const std::string& title) {
    std::cout << "\n==================================================\n";
    std::cout << "--- " << title << " ---\n";
}
void handleDeviceCreation(HomeManager* homeManager, int deviceType) {
    std::string deviceId = getStringInput("ID: ");
    std::string deviceName = getStringInput("Nombre: ");
    
    std::shared_ptr<IDevice> newDevice;
    try {
        if (deviceType == 1) {
            WiFiLightProvisioner provisioner;
            newDevice = provisioner.provisionDevice(deviceName);
        } else if (deviceType == 2) {
            WiFiLockProvisioner provisioner;
            newDevice = provisioner.provisionDevice(deviceName);
        } else if (deviceType == 3) {
            HighSecurityLockProvisioner provisioner;
            newDevice = provisioner.provisionDevice(deviceName);
        } else if (deviceType == 4) {
            ZigbeeRelayProvisioner provisioner;
            newDevice = provisioner.provisionDevice(deviceName);
        }
        
        if (newDevice) {
            std::string autoLock = getStringInput("Desea activar Auto-Cierre para este dispositivo? (s/n): ");
            if (autoLock == "s" || autoLock == "S") {
                newDevice = std::make_shared<AutoLockingDevice>(newDevice);
                std::cout << "[Info] Dispositivo creado con funcionalidad de Auto-Cierre (Decorator).\n";
            }
            homeManager->addDevice(deviceId, newDevice);
            std::cout << "Dispositivo creado y agregado con exito.\n";
        }
    } catch (const std::exception& error) {
        std::cout << "Error al agregar dispositivo: " << error.what() << "\n";
    }
}

void handleDeviceToggle(HomeManager* homeManager, bool turnOn) {
    std::string deviceId = getStringInput("ID del dispositivo: ");
    try {
        auto targetDevice = homeManager->getDevice(deviceId);
        if (targetDevice) {
            if (turnOn) targetDevice->turnOn();
            else targetDevice->turnOff();
        } else {
            std::cout << "No existe un dispositivo con ese ID.\n";
        }
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

void handleGroupCreation(HomeManager* homeManager) {
    std::string groupId = getStringInput("ID del nuevo grupo: ");
    std::string groupName = getStringInput("Nombre del grupo: ");
    try {
        auto deviceGroup = std::make_shared<DeviceGroup>(groupName);
        homeManager->addDevice(groupId, deviceGroup);
        std::cout << "Grupo de dispositivos creado con exito.\n";
    } catch (const std::exception& error) {
        std::cout << "Error al crear el grupo: " << error.what() << "\n";
    }
}

void handleGroupAddition(HomeManager* homeManager) {
    std::string groupId = getStringInput("ID del grupo: ");
    std::string deviceId = getStringInput("ID del dispositivo a agregar: ");
    try {
        auto group = homeManager->getDevice(groupId);
        auto device = homeManager->getDevice(deviceId);
        
        if (!group) {
            std::cout << "No existe el grupo con ese ID.\n";
        } else if (!device) {
            std::cout << "No existe el dispositivo con ese ID.\n";
        } else {
            group->add(device);
            std::cout << "Dispositivo agregado al grupo exitosamente.\n";
        }
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}


void deviceMenu(HomeManager* homeManager) {
    while (true) {
        printHeader("GESTION DE DISPOSITIVOS");
        std::cout << "1. Crear Smart Light\n";
        std::cout << "2. Crear Smart Lock\n";
        std::cout << "3. Crear High Security Lock\n";
        std::cout << "4. Crear Zigbee Relay\n";
        std::cout << "5. Encender dispositivo\n";
        std::cout << "6. Apagar dispositivo\n";
        std::cout << "7. Crear grupo de dispositivos\n";
        std::cout << "8. Agregar dispositivo a grupo\n";
        std::cout << "9. Volver\n";
        
        int selectedOption = getIntInput("Opcion: ");
        if (selectedOption == 9) break;
        
        if (selectedOption >= 1 && selectedOption <= 4) {
            handleDeviceCreation(homeManager, selectedOption);
        } else if (selectedOption == 5 || selectedOption == 6) {
            handleDeviceToggle(homeManager, selectedOption == 5);
        } else if (selectedOption == 7) {
            handleGroupCreation(homeManager);
        } else if (selectedOption == 8) {
            handleGroupAddition(homeManager);
        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
}

void securityMenu(HomeManager* homeManager, std::shared_ptr<MotionSensor> motionSensor) {
    while (true) {
        auto securitySystem = homeManager->getSecuritySystem();
        std::string modeName = securitySystem ? securitySystem->getCurrentModeName() : "[No disponible]";

        printHeader("SEGURIDAD");
        std::cout << "Estado actual: " << modeName << "\n";
        std::cout << "1. Activar Armed Mode\n";
        std::cout << "2. Activar Night Mode\n";
        std::cout << "3. Activar Disarmed Mode\n";
        std::cout << "4. Simular evento de sensor\n";
        std::cout << "5. Volver\n";
        
        int selectedOption = getIntInput("Opcion: ");
        if (selectedOption == 5) break;
        
        if (!securitySystem) {
            std::cout << "El sistema de seguridad no esta disponible.\n";
            continue;
        }
        
        try {
            if (selectedOption == 1) {
                securitySystem->setMode(std::make_unique<ArmedMode>());
                std::cout << "Modo Armed activado.\n";
            } else if (selectedOption == 2) {
                securitySystem->setMode(std::make_unique<NightMode>());
                std::cout << "Modo Night activado.\n";
            } else if (selectedOption == 3) {
                securitySystem->setMode(std::make_unique<DisarmedMode>());
                std::cout << "Modo Disarmed activado.\n";
            } else if (selectedOption == 4) {
                if (motionSensor) motionSensor->detectMotion();
            } else {
                std::cout << "Opcion invalida.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void routineMenu(HomeManager* homeManager) {
    while (true) {
        printHeader("RUTINAS");
        std::cout << "1. Ejecutar Morning Routine\n";
        std::cout << "2. Ejecutar Night Routine\n";
        std::cout << "3. Volver\n";
        
        int selectedOption = getIntInput("Opcion: ");
        if (selectedOption == 3) break;
        
        try {
            if (selectedOption == 1) {
                MorningRoutine routine(homeManager);
                routine.run();
            } else if (selectedOption == 2) {
                NightRoutine routine(homeManager);
                routine.run();
            } else {
                std::cout << "Opcion invalida.\n";
            }
        } catch (const std::exception& error) {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void sensorMenu(std::shared_ptr<MotionSensor> motionSensor) {
    while (true) {
        printHeader("SENSORES");
        std::cout << "1. Simular movimiento\n";
        std::cout << "2. Volver\n";
        
        int selectedOption = getIntInput("Opcion: ");
        if (selectedOption == 2) break;
        
        if (selectedOption == 1) {
            try {
                if (motionSensor) motionSensor->detectMotion();
            } catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << "\n";
            }
        } else {
            std::cout << "Opcion invalida.\n";
        }
    }
}

void showSystemStatus(HomeManager* homeManager) {
    auto securitySystem = homeManager->getSecuritySystem();
    std::string modeName = securitySystem ? securitySystem->getCurrentModeName() : "[No disponible]";

    printHeader("ESTADO DEL SISTEMA");
    std::cout << "Cantidad de dispositivos registrados: " << homeManager->getDeviceCount() << "\n";
    std::cout << "Estado del SecuritySystem: " << modeName << "\n";
    std::cout << "==================================================\n";
    std::cout <<"\n";

    homeManager->listDevices();

    std::cout << "==================================================\n";
}

} 

namespace UI {

void run(HomeManager* homeManager, std::shared_ptr<MotionSensor> motionSensor) {
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
        
        int selectedOption = getIntInput("Opcion: ");
        
        if (selectedOption == 6 || selectedOption == 0) {
            std::cout << "Saliendo del sistema...\n";
            break;
    
        }
        
        switch (selectedOption) {
            case 1: deviceMenu(homeManager); break;
            case 2: securityMenu(homeManager, motionSensor); break;
            case 3: routineMenu(homeManager); break;
            case 4: sensorMenu(motionSensor); break;
            case 5: showSystemStatus(homeManager); break;
            default: std::cout << "Opcion invalida.\n";
        }
    }
}

} 