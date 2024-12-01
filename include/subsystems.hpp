#pragma once

#include "api.h"
#include "pros/adi.hpp"

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');



inline pros::MotorGroup intake({1,-2});
inline pros::Motor lift(-9);

inline pros::adi::DigitalOut clamp1(5);
inline pros::adi::DigitalOut clamp2(4);
inline pros::adi::DigitalOut doinker(2);
inline pros::adi::DigitalOut intakePiston(7);
