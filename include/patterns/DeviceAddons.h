#pragma once
#include "devices/Devices.h"
#include <memory>
#include <string>

// Base Decorator class
class DeviceDecorator : public IDevice {
protected:
    std::shared_ptr<IDevice> baseDevice;
public:
    explicit DeviceDecorator(std::shared_ptr<IDevice> device) : baseDevice(device) {}
    
    void turnOn() override { baseDevice->turnOn(); }
    void turnOff() override { baseDevice->turnOff(); }
    std::string getName() const override { return baseDevice->getName(); }
    void add(std::shared_ptr<IDevice> device) override { baseDevice->add(device); }
    void remove(std::shared_ptr<IDevice> device) override { baseDevice->remove(device); }
};

// Una cerradura con auditoría de seguridad (Decorator)
class AuditedDevice : public DeviceDecorator {
public:
    explicit AuditedDevice(std::shared_ptr<IDevice> device);
    void turnOn() override;
    void turnOff() override;
};

// cerradura o puerta que se bloquea sola (Decorator)
class AutoLockingDevice : public DeviceDecorator {
public:
    explicit AutoLockingDevice(std::shared_ptr<IDevice> device);
    void turnOff() override;
};
