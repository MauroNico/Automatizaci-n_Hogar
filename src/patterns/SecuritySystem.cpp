#include "patterns/SecuritySystem.h"
#include "core/HomeManager.h"

SecuritySystem::SecuritySystem() {
    currentMode = std::make_unique<DisarmedMode>();
}

void SecuritySystem::setMode(std::unique_ptr<ISecurityMode> mode) {
    std::cout << "[SecurityPanel] Cambio de modo activo a: " << mode->getModeName() << ".\n";
    currentMode = std::move(mode);
}

void SecuritySystem::triggerSensor() { currentMode->onSensorTripped(); }

void SecuritySystem::setArmedMode() {
    setMode(std::make_unique<ArmedMode>());
}

void SecuritySystem::setDisarmedMode() {
    setMode(std::make_unique<DisarmedMode>());
}

void SecuritySystem::setNightMode() {
    setMode(std::make_unique<NightMode>());
}

std::string SecuritySystem::getCurrentModeName() const {
    return currentMode->getModeName();
}

// ArmedMode
void ArmedMode::onSensorTripped() {
    std::cout << "[ArmedMode] BRECHA DE SEGURIDAD! Iniciando protocolos de panico...\n";
    HomeManager::getInstance()->activateSirens();
    HomeManager::getInstance()->notifyPolice();
}

std::string ArmedMode::getModeName() const { return "Armed"; }

// DisarmedMode
void DisarmedMode::onSensorTripped() {
    std::cout << "[DisarmedMode] Movimiento ignorado. Sistema inactivo.\n";
}

std::string DisarmedMode::getModeName() const { return "Disarmed"; }

// NightMode
void NightMode::onSensorTripped() {
    std::cout << "[NightMode] Actividad nocturna inusual. Encendiendo perimetro...\n";
    HomeManager::getInstance()->turnOnEmergencyLights();
}

std::string NightMode::getModeName() const { return "NightMode"; }
