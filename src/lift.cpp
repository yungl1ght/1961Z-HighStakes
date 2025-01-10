#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/misc.h"
#include "subsystems.hpp"

void liftControl(){
    // original get ring position
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)){
        lift.move_absolute(475, 127);
    }
    // scoring position
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
        intake.move(-20);
        pros::delay(200);
        intake.move(0);
        lift.move_absolute(1875, 127);
    }
    // back to default
    if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        lift.move_absolute(0, 127);
    }
   
}

void liftLoad(){
    lift.move_absolute(475, 127);
}

void lfitDown(){
    lift.move_absolute(0, 127);
}

void liftScore(){
    intake.move(-20);
    pros::delay(200);
    intake.move(0);
    lift.move_absolute(1875, 127);
}

void intakeUp(){
    intakePiston.set_value(0);
}

void intakeDown(){
    intakePiston.set_value(1);
}