#pragma once
#include "devices/Devices.h"
#include "devices/ZigbeeAPI.h"
#include <memory>

// Concepto real: Integración o Driver de dispositivo Zigbee (Implementa Adapter)
class ZigbeeDeviceHandler : public IDevice {
private:
    std::string deviceName;
    std::unique_ptr<ZigbeeNode> hardwareNode;
    uint8_t zigbeeEndpoint;
public:
    ZigbeeDeviceHandler(const std::string& name, std::unique_ptr<ZigbeeNode> hardware, uint8_t endpoint);
    ~ZigbeeDeviceHandler() = default;
    
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};
