#include "patterns/SecuritySystem.h"

SecuritySystem::SecuritySystem(ISecurityActionHandler* handler) : actionHandler(handler) {
    currentMode = std::make_unique<DisarmedMode>();
}

void SecuritySystem::setMode(std::unique_ptr<ISecurityMode> mode) {
    std::cout << "[SecurityPanel] Cambio de modo activo a: " << mode->getModeName() << ".\n";
    currentMode = std::move(mode);
}

ISecurityActionHandler* SecuritySystem::getActionHandler() {
    return actionHandler;
}

void SecuritySystem::triggerSensor() { currentMode->onSensorTripped(this); }
void SecuritySystem::setArmedMode() { currentMode->switchToArmed(this); }
void SecuritySystem::setDisarmedMode() { currentMode->switchToDisarmed(this); }
void SecuritySystem::setNightMode() { currentMode->switchToNightMode(this); }

std::string SecuritySystem::getCurrentModeName() const {
    return currentMode->getModeName();
}

// ArmedMode
void ArmedMode::onSensorTripped(SecuritySystem* system) {
    std::cout << "[ArmedMode] BRECHA DE SEGURIDAD! Iniciando protocolos de panico...\n";
    system->getActionHandler()->activateSirens();
    system->getActionHandler()->notifyPolice();
}
void ArmedMode::switchToArmed(SecuritySystem* system) { /* Ya armado */ }
void ArmedMode::switchToDisarmed(SecuritySystem* system) {
    system->setMode(std::make_unique<DisarmedMode>());
}
void ArmedMode::switchToNightMode(SecuritySystem* system) {
    system->setMode(std::make_unique<NightMode>());
}
std::string ArmedMode::getModeName() const { return "Armed"; }

// DisarmedMode
void DisarmedMode::onSensorTripped(SecuritySystem* system) {
    std::cout << "[DisarmedMode] Movimiento ignorado. Sistema inactivo.\n";
}
void DisarmedMode::switchToArmed(SecuritySystem* system) {
    system->setMode(std::make_unique<ArmedMode>());
}
void DisarmedMode::switchToDisarmed(SecuritySystem* system) { /* Nada */ }
void DisarmedMode::switchToNightMode(SecuritySystem* system) {
    system->setMode(std::make_unique<NightMode>());
}
std::string DisarmedMode::getModeName() const { return "Disarmed"; }

// NightMode
void NightMode::onSensorTripped(SecuritySystem* system) {
    std::cout << "[NightMode] Actividad nocturna inusual. Encendiendo perimetro...\n";
    system->getActionHandler()->turnOnEmergencyLights();
}
void NightMode::switchToArmed(SecuritySystem* system) {
    system->setMode(std::make_unique<ArmedMode>());
}
void NightMode::switchToDisarmed(SecuritySystem* system) {
    system->setMode(std::make_unique<DisarmedMode>());
}
void NightMode::switchToNightMode(SecuritySystem* system) { /* Nada */ }
std::string NightMode::getModeName() const { return "NightMode"; }
