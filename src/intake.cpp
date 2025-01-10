#include "main.h"
#include "EZ-Template/util.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

void intakeControl(){
    if (master.get_digital(DIGITAL_R1)){
        intake.move(127);
    } else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        intake.move(-127);
    } else {
        intake.move(0);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
        intake.move(-40);
        pros::delay(50);
        intake.move(0);
    }

    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
        for (int i = 0; i < 7; i++) {
            intake.move(127);
            pros::delay(65);
            intake.move(0);
            pros::delay(10);
        }

        intake.move(-40);
        pros::delay(50);
        intake.move(0);
    }
    
}

void intakeOn(){
    intake.move(127);
}

void intakeOff(){
    intake.move(0);
}
