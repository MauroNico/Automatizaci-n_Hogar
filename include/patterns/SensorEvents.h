#pragma once
#include <string>
#include <vector>
#include <algorithm>

// escuchador de Eventos de la Casa (Observer)
class ISensorListener {
public:
    virtual ~ISensorListener() = default;
    virtual void onSensorTriggered(const std::string& eventType, const std::string& sensorName) = 0;
};

//sensor físico de movimiento (Sujeto)
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
