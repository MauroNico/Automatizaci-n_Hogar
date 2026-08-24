#include "patterns/DeviceAddons.h"
#include <iostream>

AuditedDevice::AuditedDevice(std::shared_ptr<IDevice> device) : baseDevice(device) {}

// AuditedDevice

void AuditedDevice::turnOn() {
    std::cout << "[DB_AUDIT] REGISTRO INMUTABLE: Accion de Bloqueo en '" << getName() << "'\n";
    baseDevice->turnOn();
}

void AuditedDevice::turnOff() {
    std::cout << "[DB_AUDIT] REGISTRO INMUTABLE: Accion de Apertura en '" << getName() << "'\n";
    baseDevice->turnOff();
}

std::string AuditedDevice::getName() const { return baseDevice->getName(); }

void AuditedDevice::add(std::shared_ptr<IDevice> device) {
    baseDevice->add(device);
}

void AuditedDevice::remove(std::shared_ptr<IDevice> device) {
    baseDevice->remove(device);
}

// AutoLockingDevice
AutoLockingDevice::AutoLockingDevice(std::shared_ptr<IDevice> device) : baseDevice(device) {}

void AutoLockingDevice::turnOn() { 
    baseDevice->turnOn(); 
}

void AutoLockingDevice::turnOff() {
    baseDevice->turnOff();
    std::cout << "[FIRMWARE] " << getName() << " desbloqueado. Temporizador de Auto-Cierre iniciado...\n";
    std::cout << "[FIRMWARE] Tiempo agotado. Asegurando " << getName() << " de nuevo.\n";
    baseDevice->turnOn();
}

std::string AutoLockingDevice::getName() const { return baseDevice->getName(); }

void AutoLockingDevice::add(std::shared_ptr<IDevice> device) {
    baseDevice->add(device);
}

void AutoLockingDevice::remove(std::shared_ptr<IDevice> device) {
    baseDevice->remove(device);
}
