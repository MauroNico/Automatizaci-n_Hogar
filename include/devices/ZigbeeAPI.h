#pragma once
#include <iostream>
#include <string>
#include <cstdint>

// Simulamos una API/SDK de bajo nivel de un fabricante de chips Zigbee.
// Este código NO usa nuestras interfaces (IDevice) porque es externo.
class ZigbeeNode {
public:
    void sendNetworkPayload(uint8_t endpoint, const std::string& action) {
        std::cout << "[ZIGBEE RADIO] Tx -> Endpoint: " << (int)endpoint 
                  << " | Comando crudo: " << action << "\n";
    }
};
