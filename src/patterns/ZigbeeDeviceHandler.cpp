#include "patterns/ZigbeeDeviceHandler.h"
#include <iostream>

ZigbeeDeviceHandler::ZigbeeDeviceHandler(const std::string& name, ZigbeeNode* hardware, uint8_t endpoint)
    : deviceName(name), hardwareNode(hardware), zigbeeEndpoint(endpoint) {}

ZigbeeDeviceHandler::~ZigbeeDeviceHandler() {
    delete hardwareNode;
}

void ZigbeeDeviceHandler::turnOn() {
    std::cout << "ZigbeeHandler [" << deviceName << "]: Transmitiendo paquete de encendido a la red mesh.\n";
    hardwareNode->sendNetworkPayload(zigbeeEndpoint, "ON_PAYLOAD_0xFF");
}

void ZigbeeDeviceHandler::turnOff() {
    std::cout << "ZigbeeHandler [" << deviceName << "]: Transmitiendo paquete de apagado a la red mesh.\n";
    hardwareNode->sendNetworkPayload(zigbeeEndpoint, "OFF_PAYLOAD_0x00");
}

std::string ZigbeeDeviceHandler::getName() const {
    return deviceName;
}
