#pragma once
#include "devices/Devices.h"
#include <memory>
#include <string>

// Concepto real: Provisionador de Hardware de IoT (Implementa Factory Method)
class DeviceProvisioner {
public:
    static std::shared_ptr<IDevice> provisionDevice(const std::string& hardwareProfile, const std::string& name);
};
