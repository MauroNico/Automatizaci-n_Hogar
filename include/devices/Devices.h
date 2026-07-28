#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>

class IDevice {
public:
    virtual ~IDevice() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual std::string getName() const = 0;
    
    // Implementación por defecto para nodos hoja (Leaf)
    virtual void add(std::shared_ptr<IDevice> device) {}
    virtual void remove(std::shared_ptr<IDevice> device) {}
};

class SmartLight : public IDevice {
private:
    std::string name;
public:
    explicit SmartLight(const std::string& name);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};

class SmartLock : public IDevice {
private:
    std::string name;
public:
    explicit SmartLock(const std::string& name);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
};

// Composite
class DeviceGroup : public IDevice {
private:
    std::string name;
    std::vector<std::shared_ptr<IDevice>> devices;
public:
    explicit DeviceGroup(const std::string& name);
    void turnOn() override;
    void turnOff() override;
    std::string getName() const override;
    void add(std::shared_ptr<IDevice> device) override;
    void remove(std::shared_ptr<IDevice> device) override;
};
