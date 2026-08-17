#pragma once
#include "devices/Devices.h"
#include <memory>
#include <string>

// Una cerradura con auditoría de seguridad (Decorator)
class AuditedDevice : public IDevice {
private:
    std::shared_ptr<IDevice> baseDevice;
public:
    explicit AuditedDevice(std::shared_ptr<IDevice> device);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};

// cerradura o puerta que se bloquea sola (Decorator)
class AutoLockingDevice : public IDevice {
private:
    std::shared_ptr<IDevice> baseDevice;
public:
    explicit AutoLockingDevice(std::shared_ptr<IDevice> device);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};
