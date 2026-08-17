#pragma once

#include "core/HomeManager.h"
#include "patterns/SensorEvents.h"

#include <memory>


namespace UI {

    void run(HomeManager* hub, std::shared_ptr<MotionSensor> sensor);

}