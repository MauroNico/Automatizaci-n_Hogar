#pragma once

#include <memory>


class HomeManager;
class MotionSensor;

namespace UI {
    
    void run(HomeManager* homeManager, std::shared_ptr<MotionSensor> motionSensor);
}