#include "core/HomeManager.h"
#include <iostream>

HomeManager* HomeManager::instance = nullptr;
std::mutex HomeManager::mutex_;

HomeManager::HomeManager() {
    securitySystem = std::make_unique<SecuritySystem>();
    
}

HomeManager* HomeManager::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (instance == nullptr) {
        instance = new HomeManager();
    }
    return instance;
}

void HomeManager::destroyInstance() {
    std::lock_guard<std::mutex> lock(mutex_); 
    if (instance != nullptr) {
        delete instance;      
        instance = nullptr;   
    }
}

void HomeManager::addDevice(const std::string& id, std::shared_ptr<IDevice> device) {
    registeredDevices[id] = device;
}

std::shared_ptr<IDevice> HomeManager::getDevice(const std::string& id) {
    if (registeredDevices.find(id) != registeredDevices.end()) {
        return registeredDevices[id];
    }
    return nullptr;
}

size_t HomeManager::getDeviceCount() const {
    return registeredDevices.size();
}

void HomeManager::turnOffAll() {
    for (auto const& pair : registeredDevices) {
        pair.second->turnOff();
    }
}

void HomeManager::setDoorsGroup(const std::string& groupId) {
    doorsGroupId = groupId;
}

void HomeManager::lockAllDoors() {
    auto group = getDevice(doorsGroupId);
    if(group) group->turnOn();
}

void HomeManager::unlockAllDoors() {
    auto group = getDevice(doorsGroupId);
    if(group) group->turnOff();
}

SecuritySystem* HomeManager::getSecuritySystem() {
    return securitySystem.get();
}

void HomeManager::onSensorTriggered(const std::string& eventType, const std::string& source) {
    if (eventType == "MOTION_DETECTED") {
        std::cout << "[Hub] Procesando evento de sensor: " << source << "\n";
        securitySystem->triggerSensor();
    }
}

void HomeManager::activateSirens() {
    std::cout << "[Hub Action] HACIENDO SONAR LAS SIRENAS FISICAS!\n";
}
void HomeManager::notifyPolice() {
    std::cout << "[Hub Action] LLAMANDO AL 911 VÍA API EXTERNA!\n";
}
void HomeManager::turnOnEmergencyLights() {
    std::cout << "[Hub Action] Encendiendo iluminación de emergencia.\n";
    auto outdoorGroup = getDevice("GrupoExterior");
    if(outdoorGroup) outdoorGroup->turnOn();
}

void HomeManager::listDevices() const {
    std::cout << "--- Dispositivos Registrados ---\n";
    if (registeredDevices.empty()) {
        std::cout << "No hay dispositivos registrados.\n";
        return;
    }
    for (const auto& pair : registeredDevices) {
        std::cout << "- ID: " << pair.first << " | Nombre: " << pair.second->getName() << "\n";
    }
}
