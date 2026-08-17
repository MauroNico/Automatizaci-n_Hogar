#include "patterns/DeviceProvisioner.h"
#include "patterns/DeviceAddons.h"
#include "patterns/ZigbeeDeviceHandler.h"
#include "devices/ZigbeeAPI.h"
#include <stdexcept>

std::shared_ptr<IDevice> WiFiLightProvisioner::provisionDevice(const std::string& name) const {
    return std::make_shared<SmartLight>(name);
}

std::shared_ptr<IDevice> WiFiLockProvisioner::provisionDevice(const std::string& name) const {
    return std::make_shared<SmartLock>(name);
}

std::shared_ptr<IDevice> HighSecurityLockProvisioner::provisionDevice(const std::string& name) const {
    // Fabrica un hardware y le inyecta seguridad adicional de software
    auto baseLock = std::make_shared<SmartLock>(name);
    return std::make_shared<AuditedDevice>(baseLock);
}

std::shared_ptr<IDevice> ZigbeeRelayProvisioner::provisionDevice(const std::string& name) const {
    return std::make_shared<ZigbeeDeviceHandler>(name, std::make_unique<ZigbeeNode>(), 0x1A);
}
