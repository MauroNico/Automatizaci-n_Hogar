#pragma once
#include <iostream>
#include <memory>
#include <string>

class SecuritySystem;

// Concepto real: Enlace para accionar hardware de emergencia
class ISecurityActionHandler {
public:
    virtual ~ISecurityActionHandler() = default;
    virtual void activateSirens() = 0;
    virtual void notifyPolice() = 0;
    virtual void turnOnEmergencyLights() = 0;
};

// Concepto real: Modo de Operación del Panel de Alarma (Implementa State)
class ISecurityMode {
public:
    virtual ~ISecurityMode() = default;
    virtual void onSensorTripped(SecuritySystem* system) = 0;
    virtual void switchToArmed(SecuritySystem* system) = 0;
    virtual void switchToDisarmed(SecuritySystem* system) = 0;
    virtual void switchToNightMode(SecuritySystem* system) = 0;
    virtual std::string getModeName() const = 0;
};

// Panel de Alarma
class SecuritySystem {
private:
    std::unique_ptr<ISecurityMode> currentMode;
    ISecurityActionHandler* actionHandler;
public:
    explicit SecuritySystem(ISecurityActionHandler* handler);
    void setMode(std::unique_ptr<ISecurityMode> mode);
    ISecurityActionHandler* getActionHandler();
    
    void triggerSensor();
    void setArmedMode();
    void setDisarmedMode();
    void setNightMode();
};

class ArmedMode : public ISecurityMode {
public:
    void onSensorTripped(SecuritySystem* system) override;
    void switchToArmed(SecuritySystem* system) override;
    void switchToDisarmed(SecuritySystem* system) override;
    void switchToNightMode(SecuritySystem* system) override;
    std::string getModeName() const override;
};

class DisarmedMode : public ISecurityMode {
public:
    void onSensorTripped(SecuritySystem* system) override;
    void switchToArmed(SecuritySystem* system) override;
    void switchToDisarmed(SecuritySystem* system) override;
    void switchToNightMode(SecuritySystem* system) override;
    std::string getModeName() const override;
};

class NightMode : public ISecurityMode {
public:
    void onSensorTripped(SecuritySystem* system) override;
    void switchToArmed(SecuritySystem* system) override;
    void switchToDisarmed(SecuritySystem* system) override;
    void switchToNightMode(SecuritySystem* system) override;
    std::string getModeName() const override;
};
