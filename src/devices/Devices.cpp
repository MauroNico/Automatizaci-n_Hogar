#include "Devices.h"

SmartLight::SmartLight(const std::string& name) : name(name) {}

void SmartLight::turnOn() { 
    std::cout << "SmartLight [" << name << "] encendida.\n"; 
}

void SmartLight::turnOff() { 
    std::cout << "SmartLight [" << name << "] apagada.\n"; 
}

std::string SmartLight::getName() const { 
    return name; 
}

SmartLock::SmartLock(const std::string& name) : name(name) {}

void SmartLock::turnOn() { 
    std::cout << "SmartLock [" << name << "] bloqueada (On).\n"; 
}

void SmartLock::turnOff() { 
    std::cout << "SmartLock [" << name << "] desbloqueada (Off).\n"; 
}

std::string SmartLock::getName() const { 
    return name; 
}

DeviceGroup::DeviceGroup(const std::string& name) : name(name) {}

void DeviceGroup::turnOn() {
    std::cout << "DeviceGroup [" << name << "] encendiendo todos los dispositivos...\n";
    for (auto& device : devices) { 
        device->turnOn(); 
    }
}

void DeviceGroup::turnOff() {
    std::cout << "DeviceGroup [" << name << "] apagando todos los dispositivos...\n";
    for (auto& device : devices) { 
        device->turnOff(); 
    }
}

std::string DeviceGroup::getName() const { 
    return name; 
}

void DeviceGroup::add(std::shared_ptr<IDevice> device) { 
    devices.push_back(device); 
}

void DeviceGroup::remove(std::shared_ptr<IDevice> device) {
    for (auto it = devices.begin(); it != devices.end(); ++it) {
        if (*it == device) { 
            devices.erase(it); 
            break; 
        }
    }
}
