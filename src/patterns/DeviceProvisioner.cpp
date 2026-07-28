#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "devices/ZigbeeAPI.h"
#include <stdexcept>

std::shared_ptr<IDevice> DeviceProvisioner::provisionDevice(const std::string& hardwareProfile, const std::string& name) {
    if (hardwareProfile == "WiFiLight") {
        return std::make_shared<SmartLight>(name);
    } 
    else if (hardwareProfile == "WiFiLock") {
        return std::make_shared<SmartLock>(name);
    }
    else if (hardwareProfile == "HighSecurityLock") {
        // Fabrica un hardware y le inyecta seguridad adicional de software
        auto baseLock = std::make_shared<SmartLock>(name);
        return std::make_shared<AuditedDevice>(baseLock);
    }
    else if (hardwareProfile == "ZigbeeRelay") {
        return std::make_shared<ZigbeeDeviceHandler>(name, new ZigbeeNode(), 0x1A);
    }
    
    throw std::invalid_argument("El Provisioner no soporta el hardware profile: " + hardwareProfile);
}
