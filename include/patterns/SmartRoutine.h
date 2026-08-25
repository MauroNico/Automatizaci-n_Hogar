#pragma once
#include "core/HomeManager.h"

// automatización (Template)
class SmartRoutine {
protected:
    HomeManager* hub;
    
    // pasos que todas las rutinas deben implementar
    virtual void configureLighting() = 0;
    virtual void configureSecurity() = 0;

public:
    explicit SmartRoutine(HomeManager* manager) : hub(manager) {}
    virtual ~SmartRoutine() = default;
    
    void run() {
        std::cout << "\n--- Iniciando Macro de Domotica ---\n";
        configureLighting();
        configureSecurity();
        std::cout << "--- Macro Finalizada ---\n\n";
    }
};

class MorningRoutine : public SmartRoutine {
private:
    std::string outdoorLightsId;
    std::string indoorLightsId;
    std::string doorsGroupId;
public:
    explicit MorningRoutine(HomeManager* manager, 
                            const std::string& outdoorId, 
                            const std::string& indoorId, 
                            const std::string& doorsId) 
        : SmartRoutine(manager), outdoorLightsId(outdoorId), indoorLightsId(indoorId), doorsGroupId(doorsId) {}
protected:
    void configureLighting() override;
    void configureSecurity() override;
};

class NightRoutine : public SmartRoutine {
private:
    std::string doorsGroupId;
public:
    explicit NightRoutine(HomeManager* manager, const std::string& doorsId) 
        : SmartRoutine(manager), doorsGroupId(doorsId) {}
protected:
    void configureLighting() override;
    void configureSecurity() override;
};
