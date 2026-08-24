#pragma once
#include <iostream>
#include <memory>
#include <string>

class SecuritySystem;

//Operaciones del Panel de Alarma (State)
class ISecurityMode {
public:
    virtual ~ISecurityMode() = default;
    virtual void onSensorTripped() = 0;
    virtual std::string getModeName() const = 0;
};

// Panel de Alarma
class SecuritySystem {
private:
    std::unique_ptr<ISecurityMode> currentMode;
public:
    SecuritySystem();
    void setMode(std::unique_ptr<ISecurityMode> mode);
    
    void triggerSensor();
    void setArmedMode();
    void setDisarmedMode();
    void setNightMode();
    std::string getCurrentModeName() const;
};

class ArmedMode : public ISecurityMode {
public:
    void onSensorTripped() override;
    std::string getModeName() const override;
};

class DisarmedMode : public ISecurityMode {
public:
    void onSensorTripped() override;
    std::string getModeName() const override;
};

class NightMode : public ISecurityMode {
public:
    void onSensorTripped() override;
    std::string getModeName() const override;
};
