#pragma once
#include "devices/Devices.h"
#include "patterns/SensorEvents.h"
#include "patterns/SecuritySystem.h"
#include <memory>
#include <mutex>
#include <map>
#include <string>

// Hub Principal (Singleton)
// El mutex protege la inicialización de la instancia única frente a accesos concurrentes.
// Las operaciones sobre el estado interno del HomeManager no están sincronizadas de forma general.
class HomeManager : public ISensorListener, public ISecurityActionHandler {
private:
    static HomeManager* instance;
    static std::mutex mutex_;
    
    std::map<std::string, std::shared_ptr<IDevice>> registeredDevices;
    std::unique_ptr<SecuritySystem> securitySystem;

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
    
    SecuritySystem* getSecuritySystem();

    // ISensorListener
    void onSensorTriggered(const std::string& eventType, const std::string& source) override;

    // ISecurityActionHandler
    void activateSirens() override;
    void notifyPolice() override;
    void turnOnEmergencyLights() override;
};
