#include "patterns/SensorEvents.h"
#include <iostream>

MotionSensor::MotionSensor(const std::string& name) : name(name) {}

void MotionSensor::addListener(ISensorListener* listener) {
    listeners.push_back(listener);
}

void MotionSensor::removeListener(ISensorListener* listener) {
    listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void MotionSensor::detectMotion() {
    std::cout << "[Hardware] MotionSensor [" << name << "] detecto movimiento!\n";
    for (auto* listener : listeners) {
        listener->onSensorTriggered("MOTION_DETECTED", name);
    }
}
