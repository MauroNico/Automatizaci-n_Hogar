#pragma once
#include "devices/Devices.h"
#include "devices/ZigbeeAPI.h"

// Concepto real: Integración o Driver de dispositivo Zigbee (Implementa Adapter)
class ZigbeeDeviceHandler : public IDevice {
private:
    std::string deviceName;
    ZigbeeNode* hardwareNode;
    uint8_t zigbeeEndpoint;
public:
    ZigbeeDeviceHandler(const std::string& name, ZigbeeNode* hardware, uint8_t endpoint);
    ~ZigbeeDeviceHandler();
    
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};
