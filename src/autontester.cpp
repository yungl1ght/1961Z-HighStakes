#include "main.h"
#include "pros/misc.h"

void autonTestor(){
    if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        autonomous();
    }
}