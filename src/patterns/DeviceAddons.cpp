#include "patterns/DeviceAddons.h"
#include <iostream>

AuditedDevice::AuditedDevice(std::shared_ptr<IDevice> device) : DeviceDecorator(device) {}

// AuditedDevice

void AuditedDevice::turnOn() {
    std::cout << "[DB_AUDIT] REGISTRO INMUTABLE: Accion de Bloqueo en '" << getName() << "'\n";
    DeviceDecorator::turnOn();
}

void AuditedDevice::turnOff() {
    std::cout << "[DB_AUDIT] REGISTRO INMUTABLE: Accion de Apertura en '" << getName() << "'\n";
    DeviceDecorator::turnOff();
}

// AutoLockingDevice
AutoLockingDevice::AutoLockingDevice(std::shared_ptr<IDevice> device) : DeviceDecorator(device) {}

void AutoLockingDevice::turnOff() {
    DeviceDecorator::turnOff();
    std::cout << "[FIRMWARE] " << getName() << " desbloqueado. Temporizador de Auto-Cierre iniciado...\n";
    std::cout << "[FIRMWARE] Tiempo agotado. Asegurando " << getName() << " de nuevo.\n";
    DeviceDecorator::turnOn();
}
