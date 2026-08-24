#pragma once
#include "devices/Devices.h"
#include "patterns/SensorEvents.h"
#include "patterns/SecuritySystem.h"
#include <memory>
#include <mutex>
#include <map>
#include <string>


class HomeManager : public ISensorListener {
private:
    static HomeManager* instance;
    static std::mutex mutex_;
    
    std::map<std::string, std::shared_ptr<IDevice>> registeredDevices;
    std::unique_ptr<SecuritySystem> securitySystem;
    std::string doorsGroupId = "GrupoPuertas";
    std::string outdoorLightsGroupId = "GrupoExterior";
    std::string kitchenLightId = "LuzCocina";

    HomeManager();
    ~HomeManager() = default;

public:
    HomeManager(HomeManager& other) = delete;
    void operator=(const HomeManager&) = delete;

    static HomeManager* getInstance();
    static void destroyInstance();
    

    void addDevice(const std::string& id, std::shared_ptr<IDevice> device);
    std::shared_ptr<IDevice> getDevice(const std::string& id);
    size_t getDeviceCount() const;
    
    void turnOffAll();
    void lockAllDoors();
    void unlockAllDoors();
    void setDoorsGroup(const std::string& groupId);
    void setOutdoorLightsGroup(const std::string& groupId);
    void setKitchenLight(const std::string& deviceId);
    void turnOffOutdoorLights();
    void turnOnKitchenLight();
    
    SecuritySystem* getSecuritySystem();

    // ISensorListener
    void onSensorTriggered(const std::string& eventType, const std::string& source) override;

    // Acciones fisicas
    void activateSirens();
    void notifyPolice();
    void turnOnEmergencyLights();
    void listDevices() const;
};
