#pragma once
#include <string>
#include <vector>
#include <algorithm>

// Concepto real: Escuchador de Eventos de la Casa (Implementa Observer)
class ISensorListener {
public:
    virtual ~ISensorListener() = default;
    virtual void onSensorTriggered(const std::string& eventType, const std::string& sensorName) = 0;
};

// Concepto real: Un sensor físico de movimiento (Actúa como Subject)
class MotionSensor {
private:
    std::string name;
    std::vector<ISensorListener*> listeners;
public:
    explicit MotionSensor(const std::string& name);
    void addListener(ISensorListener* listener);
    void removeListener(ISensorListener* listener);
    
    // Simula hardware detectando movimiento
    void detectMotion(); 
};
