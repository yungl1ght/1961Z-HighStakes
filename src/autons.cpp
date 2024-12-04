#include "autons.hpp"
#include "EZ-Template/util.hpp"
#include "main.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 90;

///
// Constants
///
void default_constants() {
  chassis.pid_heading_constants_set(11, 0, 20);
  chassis.pid_drive_constants_set(20, 0, 100);
  chassis.pid_turn_constants_set(3, 0.05, 20, 15);
  chassis.pid_swing_constants_set(6, 0, 65);
  chassis.drive_imu_scaler_set(1.0115);

  chassis.pid_turn_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(80_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(80_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);

  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  chassis.slew_drive_constants_set(7_in, 80);
}

///
// Drive Example
///
void drive_example() {
  // The first parameter is target inches
  // The second parameter is max speed the robot will drive at
  // The third parameter is a boolean (true or false) for enabling/disabling a slew at the start of drive motions
  // for slew, only enable it when the drive distance is greater than the slew distance + a few inches

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // The first parameter is the target in degrees
  // The second parameter is max speed the robot will drive at

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until will wait until the robot gets to a desired position

  // When the robot gets to 6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(24_in, 30, true);
  chassis.pid_wait_until(6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // When the robot gets to -6 inches slowly, the robot will travel the remaining distance at full speed
  chassis.pid_drive_set(-24_in, 30, true);
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);  // After driving 6 inches at 30 speed, the robot will go the remaining distance at DRIVE_SPEED
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // The first parameter is ez::LEFT_SWING or ez::RIGHT_SWING
  // The second parameter is the target in degrees
  // The third parameter is the speed of the moving side of the drive
  // The fourth parameter is the speed of the still side of the drive, this allows for wider arcs

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining is where motions all try to blend together instead of individual movements.
  // This works by exiting while the robot is still moving a little bit.
  // To use this, replace pid_wait with pid_wait_quick_chain.
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  // Your final motion should still be a normal pid_wait
  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    // Attempt to drive backward
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    // If failsafed...
    if (chassis.interfered) {
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    }
    // If the robot successfully drove back, return
    else {
      return;
    }
  }
}

// If there is no interference, the robot will drive forward and turn 90 degrees.
// If interfered, the robot will drive forward and then attempt to drive backward.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);
    return;
  }

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

// . . .
// Make your own autonomous functions here!
// . . .

void blueLeft(){

  // sets clamp in up position and puts intake down
  // drives forward, turns, drives forward
  // slows down right before mogo and then gets mogo clamped

  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-14.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-14_in, 40);
  pros::delay(1100);
  clampMogo();
  pros::delay(300);

  // after clamping mogo, turns to single stack to the left
  // turns intake on to score preload and drives to single stack

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  pros::delay(400);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(26_in, DRIVE_SPEED);
  chassis.pid_wait();
  
  pros::delay(1000);

  // after single stack moves back to original mogo position
  // turns to the upside down stack in front of the alliance stake
  // puts intake up and drives towards reverse stack and gets top ring

  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  pros::delay(400);
  intakeUp();
  pros::delay(200);
  chassis.pid_drive_set(35_in, DRIVE_SPEED);
  chassis.pid_wait();

  pros::delay(500);

  // drives back to original mogo position
  // turns toward ladder
  // puts ladybrown mech up and drives into ladder with clamp side

  chassis.pid_drive_set(-35_in, DRIVE_SPEED);
  pros::delay(400);
  intakeDown();
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  pros::delay(1200);
  lift.move_absolute(700, 127);
  pros::delay(250);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED);
  chassis.pid_wait();

}

void blueRight(){
  
  // sets clamp up and intake down
  // drives forward, turns, and drives forward again
  // slows down right before mogo and then gets clamp on mogo


  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-9_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-17_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-8_in, 40);
  pros::delay(400);
  clampMogo();
  chassis.pid_wait();
  pros::delay(500);

  // turns after clamping mogo to go for the single stack first
  // after single stack it goes for the 2 stacks next to each other
  

  chassis.pid_turn_set(-95_deg, 70);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(19_in, DRIVE_SPEED);
  pros::delay(1250);

  // left stack first

  chassis.pid_turn_set(-181_deg, TURN_SPEED);
  pros::delay(600);
  chassis.pid_drive_set(16_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-6_in, DRIVE_SPEED);
  pros::delay(550);
  
  // right stack second

  chassis.pid_turn_relative_set(24.5_deg, TURN_SPEED);
  pros::delay(400);
  chassis.pid_drive_set(9.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(1000);

  // moves back and swings to face ladder
  // puts ladybrown mech up and drives into ladder with clamp side

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  pros::delay(550);
  chassis.pid_swing_set(ez::RIGHT_SWING, -65_deg, SWING_SPEED, -30);
  chassis.pid_wait();
  lift.move_absolute(700, 127);
  pros::delay(300);
  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();

}

void redLeft(){
  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-9_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-17_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-8_in, 40);
  pros::delay(400);
  clampMogo();
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_turn_set(95_deg, 70);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(19_in, DRIVE_SPEED);
  pros::delay(1250);
  chassis.pid_turn_set(179_deg, TURN_SPEED);
  pros::delay(600);
  chassis.pid_drive_set(16_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-6_in, DRIVE_SPEED);
  pros::delay(550);
  
  
  chassis.pid_turn_relative_set(-24.5_deg, TURN_SPEED);
  pros::delay(400);
  chassis.pid_drive_set(9.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(1000);
  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  pros::delay(550);
  chassis.pid_swing_set(ez::LEFT_SWING, 65_deg, SWING_SPEED, -30);
  chassis.pid_wait();
  lift.move_absolute(700, 127);
  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
}

void redRight(){

  // sets clamp in up position and puts intake down
  // drives forward, turns, drives forward
  // slows down right before mogo and then gets mogo clamped

  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-14.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-14_in, 40);
  pros::delay(1100);
  clampMogo();
  pros::delay(300);

  // after clamping mogo, turns to single stack to the left
  // turns intake on to score preload and drives to single stack

  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  pros::delay(400);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(26_in, DRIVE_SPEED);
  chassis.pid_wait();
  
  pros::delay(1000);

  // after single stack moves back to original mogo position
  // turns to the upside down stack in front of the alliance stake
  // puts intake up and drives towards reverse stack and gets top ring

  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  pros::delay(400);
  intakeUp();
  pros::delay(200);
  chassis.pid_drive_set(35_in, DRIVE_SPEED);
  chassis.pid_wait();

  pros::delay(500);

  // drives back to original mogo position
  // turns toward ladder
  // puts ladybrown mech up and drives into ladder with clamp side

  chassis.pid_drive_set(-35_in, DRIVE_SPEED);
  pros::delay(400);
  intakeDown();
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  pros::delay(1200);
  lift.move_absolute(700, 127);
  pros::delay(250);
  chassis.pid_drive_set(-25_in, DRIVE_SPEED);
  chassis.pid_wait();

}

void redElimPositive(){

  // sets clamp in up position and puts intake down
  // drives forward, turns, drives forward
  // slows down right before mogo and then gets mogo clamped

  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-18_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-9_in, 60);
  pros::delay(700);
  clampMogo();
  pros::delay(300);

  

  chassis.pid_turn_set(-90_deg, TURN_SPEED);
  pros::delay(400);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(26_in, DRIVE_SPEED);
  chassis.pid_wait();
  
  pros::delay(1000);

  

  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  pros::delay(400);
  intakeUp();
  pros::delay(200);
  chassis.pid_drive_set(35_in, DRIVE_SPEED);
  chassis.pid_wait();

  pros::delay(500);

  chassis.pid_drive_set(-10_in, DRIVE_SPEED);
  pros::delay(400);
  intakeDown();
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 0);
  chassis.pid_wait();

  chassis.pid_drive_set(58_in, DRIVE_SPEED);
  chassis.pid_wait();

  doinker.set_value(1);
  pros::delay(300);

  chassis.pid_turn_set(-225_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-15_in, DRIVE_SPEED);
  pros::delay(600);
  unclampMogo();
  chassis.pid_wait();
}

void redElimNegative(){

}

void blueElimPositive(){

  // sets clamp in up position and puts intake down
  // drives forward, turns, drives forward
  // slows down right before mogo and then gets mogo clamped

  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-14.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-10_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-14_in, 40);
  pros::delay(1100);
  clampMogo();
  pros::delay(300);

  // turns towards single stack
  // turns intake on and drives into single stack

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  pros::delay(400);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(26_in, DRIVE_SPEED);
  chassis.pid_wait();
  
  pros::delay(1000);

  // moves back to original mogo position
  // turns towards reverse stack infront of alliance stake
  // puts intake up and grabs top ring from reverse stake

  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  pros::delay(400);
  intakeUp();
  pros::delay(200);
  chassis.pid_drive_set(35_in, DRIVE_SPEED);
  chassis.pid_wait();

  pros::delay(500);

  // moves back to original mogo position
  // turns towards positive corner
  // drives to positive corner

  chassis.pid_drive_set(-35_in, DRIVE_SPEED);
  pros::delay(400);
  intakeDown();
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  pros::delay(500);
  chassis.pid_drive_set(70_in, DRIVE_SPEED);
  chassis.pid_wait();

  // puts doinker down
  // spins to remove rings
  // faces clamp side tangent to positive corner
  // drops mogo and moves forward to get out the way

  doinker.set_value(1);
  pros::delay(200);
  chassis.pid_turn_set(225_deg, TURN_SPEED);
  pros::delay(800);
  unclampMogo();
  pros::delay(200);
  chassis.pid_drive_set(10_in, DRIVE_SPEED);
  chassis.pid_wait();

}

void blueElimNegative(){
  unclampMogo();
  intakeDown();
  chassis.pid_drive_set(-9_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_turn_set(-30_deg, TURN_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-17_in, DRIVE_SPEED);
  chassis.pid_wait();
  chassis.pid_drive_set(-8_in, 40);
  pros::delay(400);
  clampMogo();
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_turn_set(-95_deg, 70);
  intakeOn();
  pros::delay(200);
  chassis.pid_drive_set(19_in, DRIVE_SPEED);
  pros::delay(1250);
  chassis.pid_turn_set(-181_deg, TURN_SPEED);
  pros::delay(600);
  chassis.pid_drive_set(16_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_drive_set(-6_in, DRIVE_SPEED);
  pros::delay(550);
  
  
  chassis.pid_turn_relative_set(24.5_deg, TURN_SPEED);
  pros::delay(400);
  chassis.pid_drive_set(9.5_in, DRIVE_SPEED);
  chassis.pid_wait();
  pros::delay(1000);
  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  pros::delay(550);
  chassis.pid_swing_set(ez::RIGHT_SWING, -65_deg, SWING_SPEED, -30);
  chassis.pid_wait();
  lift.move_absolute(700, 127);
  pros::delay(300);
  chassis.pid_drive_set(-26_in, DRIVE_SPEED);
  chassis.pid_wait();
}