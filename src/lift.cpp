#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/misc.h"
#include "subsystems.hpp"

void liftControl(){
    // original get ring position
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)){
        lift.move_absolute(400, 127);
    }
    // scoring position
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
        lift.move_absolute(1700, 127);
    }
    // back to default
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        lift.move_absolute(0, 127);
    }
    // MANAS TWEAK THIS ONE
    // ADDING POSITION MAKES IT GO HIGHER, MINUSING POSITION MAKES IT GO LOWER
    // ITS LIKE A PID HEADING, 0 IS ALL THE WAY DOWN
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        lift.move_absolute(200, 127);
    }
}

void intakeUp(){
    intakePiston.set_value(0);
}

void intakeDown(){
    intakePiston.set_value(1);
}