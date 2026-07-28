#pragma once
#include "core/HomeManager.h"

// Concepto real: Una Macro de automatización (Implementa Template Method)
class SmartRoutine {
protected:
    HomeManager* hub;
    
    // Pasos fijos que todas las rutinas deben pensar cómo implementar
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
public:
    explicit MorningRoutine(HomeManager* manager) : SmartRoutine(manager) {}
protected:
    void configureLighting() override;
    void configureSecurity() override;
};

class NightRoutine : public SmartRoutine {
public:
    explicit NightRoutine(HomeManager* manager) : SmartRoutine(manager) {}
protected:
    void configureLighting() override;
    void configureSecurity() override;
};
