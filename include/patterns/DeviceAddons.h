#pragma once
#include "devices/Devices.h"
#include <memory>
#include <string>

// Concepto real: Una cerradura con auditoría de seguridad (Implementa Decorator)
class AuditedDevice : public IDevice {
private:
    std::shared_ptr<IDevice> baseDevice;
public:
    explicit AuditedDevice(std::shared_ptr<IDevice> device);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};

// Concepto real: Una cerradura o puerta que se bloquea sola (Implementa Decorator)
class AutoLockingDevice : public IDevice {
private:
    std::shared_ptr<IDevice> baseDevice;
public:
    explicit AutoLockingDevice(std::shared_ptr<IDevice> device);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};
