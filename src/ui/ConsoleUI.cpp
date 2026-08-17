#include "ui/ConsoleUI.h"

#include "devices/Devices.h"
#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "patterns/SmartRoutine.h"
#include "patterns/SecuritySystem.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

// Todo lo de este archivo tiene enlace interno (namespace anonimo): son
// detalles de implementacion de la UI de consola, no forman parte de la
// interfaz publica del modulo (solo UI::run se expone en el .h).
namespace {

// ---------- Entrada de datos ----------

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Entrada invalida. Por favor ingrese un numero.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

// ---------- Presentacion ----------
// Antes cada menu repetia las mismas 2 lineas de separador; se extrae
// para no duplicar el mismo literal en 5 lugares distintos (DRY).
void printSectionHeader(const std::string& title) {
    std::cout << "\n==================================================\n";
    std::cout << "--- " << title << " ---\n";
}

// ---------- Menus ----------

void deviceMenu(HomeManager* hub) {
    while (true) {
        printSectionHeader("GESTION DE DISPOSITIVOS");
        std::cout << "1. Crear Smart Light\n";
        std::cout << "2. Crear Smart Lock\n";
        std::cout << "3. Crear High Security Lock\n";
        std::cout << "4. Crear Zigbee Relay\n";
        std::cout << "5. Encender dispositivo\n";
        std::cout << "6. Apagar dispositivo\n";
        std::cout << "7. Envolver dispositivo en Auto-Locking\n";
        std::cout << "8. Crear grupo de dispositivos\n";
        std::cout << "9. Agregar dispositivo a grupo\n";
        std::cout << "10. Volver\n";

        int opt = getIntInput("Opcion: ");
        if (opt == 10) break;

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
        } else if (opt == 7) {
            std::string id = getStringInput("ID del dispositivo a envolver: ");
            try {
                auto dev = hub->getDevice(id);
                if (dev) {
                    auto wrappedDev = std::make_shared<AutoLockingDevice>(dev);
                    hub->addDevice(id, wrappedDev);
                    std::cout << "Dispositivo [" << id << "] envuelto en Auto-Locking exitosamente.\n";
                } else {
                    std::cout << "No existe un dispositivo con ese ID.\n";
                }
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        } else if (opt == 8) {
            std::string id = getStringInput("ID del nuevo grupo: ");
            std::string name = getStringInput("Nombre del grupo: ");
            try {
                auto group = std::make_shared<DeviceGroup>(name);
                hub->addDevice(id, group);
                std::cout << "Grupo de dispositivos creado con exito.\n";
            } catch (const std::exception& e) {
                std::cout << "Error al crear el grupo: " << e.what() << "\n";
            }
        } else if (opt == 9) {
            std::string groupId = getStringInput("ID del grupo: ");
            std::string devId = getStringInput("ID del dispositivo a agregar: ");
            try {
                auto group = hub->getDevice(groupId);
                auto dev = hub->getDevice(devId);

                if (!group) {
                    std::cout << "No existe el grupo con ese ID.\n";
                } else if (!dev) {
                    std::cout << "No existe el dispositivo con ese ID.\n";
                } else {
                    group->add(dev);
                    std::cout << "Dispositivo agregado al grupo exitosamente.\n";
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
        auto secSys = hub->getSecuritySystem();
        std::string modeName = secSys ? secSys->getCurrentModeName() : "[No disponible]";

        printSectionHeader("SEGURIDAD");
        std::cout << "Estado actual: " << modeName << "\n";
        std::cout << "1. Activar Armed Mode\n";
        std::cout << "2. Activar Night Mode\n";
        std::cout << "3. Activar Disarmed Mode\n";
        std::cout << "4. Simular evento de sensor\n";
        std::cout << "5. Volver\n";

        int opt = getIntInput("Opcion: ");
        if (opt == 5) break;

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
        printSectionHeader("RUTINAS");
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
        printSectionHeader("SENSORES");
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
    auto secSys = hub->getSecuritySystem();
    std::string modeName = secSys ? secSys->getCurrentModeName() : "[No disponible]";

    printSectionHeader("ESTADO DEL SISTEMA");
    std::cout << "Cantidad de dispositivos registrados: " << hub->getDeviceCount() << "\n";
    std::cout << "Estado del SecuritySystem: " << modeName << "\n";
    std::cout << "==================================================\n";
}

}  // namespace

namespace UI {

void run(HomeManager* hub, std::shared_ptr<MotionSensor> sensor) {
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
            case 1: deviceMenu(hub); break;
            case 2: securityMenu(hub, sensor); break;
            case 3: routineMenu(hub); break;
            case 4: sensorMenu(sensor); break;
            case 5: showSystemStatus(hub); break;
            default: std::cout << "Opcion invalida.\n";
        }
    }
}

}  // namespace UI