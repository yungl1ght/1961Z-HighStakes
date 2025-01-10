#include "main.h"

bool state = true;

void clampControl(){
    if(master.get_digital_new_press(DIGITAL_L2)){
        state = !state;
    }
    clamp1.set_value(state);
    clamp2.set_value(state);
}

void clampMogo(){
    clamp1.set_value(0);
    clamp2.set_value(0);
}

void unclampMogo(){
    clamp1.set_value(1);
    clamp2.set_value(1);
}