#include "core/HomeManager.h"
#include "devices/Devices.h"
#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "patterns/SensorEvents.h"
#include "patterns/SmartRoutine.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=================================================\n";
    std::cout << "    SMART HOME OS CORE - INICIANDO SISTEMA\n";
    std::cout << "=================================================\n\n";

    HomeManager* hub = HomeManager::getInstance();

    // Provisionar dispositivos en el hub
    std::unique_ptr<DeviceProvisioner> highSecLockFactory = std::make_unique<HighSecurityLockProvisioner>();
    auto secureDoor = highSecLockFactory->provisionDevice("Puerta Principal");
    
    auto autoLockingSecureDoor = std::make_shared<AutoLockingDevice>(secureDoor);
    hub->addDevice("PuertaPrincipal", autoLockingSecureDoor);

    std::unique_ptr<DeviceProvisioner> zigbeeFactory = std::make_unique<ZigbeeRelayProvisioner>();
    auto zigbeeLight = zigbeeFactory->provisionDevice("Luz Porche (Zigbee)");
    
    std::unique_ptr<DeviceProvisioner> wifiLightFactory = std::make_unique<WiFiLightProvisioner>();
    auto cocinaLight = wifiLightFactory->provisionDevice("Luz Cocina");
    
    auto outdoorGroup = std::make_shared<DeviceGroup>("Grupo Exterior");
    outdoorGroup->add(zigbeeLight);
    
    hub->addDevice("GrupoExterior", outdoorGroup);
    hub->addDevice("LuzCocina", cocinaLight);

    std::cout << ">>> PRUEBA 1: Addons de Seguridad <<<\n";
    autoLockingSecureDoor->turnOff(); // Deja auditoría, desbloquea y se vuelve a bloquear

    std::cout << "\n>>> PRUEBA 2: Macro de Noche <<<\n";
    NightRoutine nightMacro(hub);
    nightMacro.run(); // Apaga todo, asegura puerta, pone modo noche en panel de alarma

    std::cout << "\n>>> PRUEBA 3: Eventos Sensoriales y Panel de Alarma <<<\n";
    auto hallwayMotionSensor = std::make_shared<MotionSensor>("Sensor Movimiento Patio");
    hallwayMotionSensor->addListener(hub);

    std::cout << "[Simulando intrusion en Night Mode]\n";
    hallwayMotionSensor->detectMotion();

    std::cout << "\n[Cambiando a modo 'Armed' e intentando de nuevo]\n";
    hub->getSecuritySystem()->setArmedMode();
    hallwayMotionSensor->detectMotion();

    std::cout << "\n=================================================\n";
    std::cout << "    SMART HOME OS CORE - FINALIZADO\n";
    std::cout << "=================================================\n";
    return 0;
}
