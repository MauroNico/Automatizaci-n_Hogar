#pragma once
#include "devices/Devices.h"
#include <memory>
#include <string>

// Concepto real: Provisionador de Hardware de IoT (Implementa Factory Method)
class DeviceProvisioner {
public:
    virtual ~DeviceProvisioner() = default;
    virtual std::shared_ptr<IDevice> provisionDevice(const std::string& name) const = 0;
};

class WiFiLightProvisioner : public DeviceProvisioner {
public:
    std::shared_ptr<IDevice> provisionDevice(const std::string& name) const override;
};

class WiFiLockProvisioner : public DeviceProvisioner {
public:
    std::shared_ptr<IDevice> provisionDevice(const std::string& name) const override;
};

class HighSecurityLockProvisioner : public DeviceProvisioner {
public:
    std::shared_ptr<IDevice> provisionDevice(const std::string& name) const override;
};

class ZigbeeRelayProvisioner : public DeviceProvisioner {
public:
    std::shared_ptr<IDevice> provisionDevice(const std::string& name) const override;
};
