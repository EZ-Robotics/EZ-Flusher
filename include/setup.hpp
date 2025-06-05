/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "EZ-Template/api.hpp"
#include "antijam.hpp"
#include "api.h"
#include "pros/misc.h"

/////
// Drive Config
/////

// Drive ports.  Negative port will reverse the motor
inline const std::vector<int> drive_ports_left = {-16, -17, -18};  // Left ports
inline const std::vector<int> drive_ports_right = {2, 3, 4};       // Right ports

inline const int imu_port = 19;         // IMU Port (this is required)
inline const double wheel_dia = 3.25;   // Wheel diameter
inline const double wheel_rpm = 400.0;  // Wheel RPM

inline pros::motor_brake_mode_e_t opcontrol_brake = pros::E_MOTOR_BRAKE_BRAKE;  // Chassis brake most for opcontrol

inline bool curve_buttons_toggle = false;                             // Enables modifying the controller curve with buttons on the joysticks
inline double active_brake_constant = 2.0;                            // Sets the active brake kP. We recommend ~2.  0 will disable
inline std::pair<double, double> drive_curve_constants = {0.0, 0.0};  // Defaults for curve. If using tank, only the first parameter is used

extern Drive chassis;

/////
// Intake Config
/////

inline pros::Motor intake_left(1);    // Motor
inline pros::Motor intake_right(-6);  // Motor

// Hook antijam variables
inline const int intake_wait_time = 30;           // The time in ms that the intake has to have 0 velocity before deciding a jam is happening
inline const int intake_outtake_time = 100;       // The time to outtake for to unjam
inline const int intake_stuck_retry_time = 1000;  // When scoring, wait before trying to unjam the intake
inline const int intake_min_speed = 20;           // Minimum speed that allows antijam to trigger (out of 127)

inline antijam intake_left_antijam(&intake_left, intake_wait_time, intake_outtake_time, intake_min_speed, intake_stuck_retry_time);    // Create the antijam
inline antijam intake_right_antijam(&intake_right, intake_wait_time, intake_outtake_time, intake_min_speed, intake_stuck_retry_time);  // Create the antijam

/////
// Bottom conveyor config
/////

inline pros::Motor conveyor_bottom(8);  // Motor

// Hook antijam variables
inline const int conveyor_bottom_wait_time = 40;  // The time in ms that the intake has to have 0 velocity before deciding a jam is happening
inline const int conveyor_bottom_time = 150;      // The time to outtake for to unjam
inline const int conveyor_bottom_speed = 20;      // Minimum speed that allows antijam to trigger (out of 127)

inline antijam conveyor_bottom_antijam(&conveyor_bottom, conveyor_bottom_wait_time, conveyor_bottom_time, conveyor_bottom_speed);  // Create the antijam

/////
// Top conveyor config
/////

inline pros::Motor conveyor_top(-7);  // Motor

// Hook antijam variables
inline const int conveyor_top_wait_time = 40;      // The time in ms that the intake has to have 0 velocity before deciding a jam is happening
inline const int conveyor_top_outtake_time = 150;  // The time to outtake for to unjam
inline const int conveyor_top_min_speed = 15;      // Minimum speed that allows antijam to trigger (out of 127)

inline antijam conveyor_top_antijam(&conveyor_top, conveyor_top_wait_time, conveyor_top_outtake_time, conveyor_top_min_speed, 180000);  // Create the antijam

/////
// Opcontrol config
/////

inline pros::controller_digital_e_t intake_button = pros::E_CONTROLLER_DIGITAL_L2;
inline pros::controller_digital_e_t outtake_button = pros::E_CONTROLLER_DIGITAL_L1;
inline pros::controller_digital_e_t score_button = pros::E_CONTROLLER_DIGITAL_R2;
inline pros::controller_digital_e_t poop_button = pros::E_CONTROLLER_DIGITAL_R1;

inline void joystick_opcontrol() {
  chassis.opcontrol_tank();  // Tank control
  // chassis.opcontrol_arcade_standard(ez::SPLIT);  // Standard split arcade
  // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
  // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
  // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade
}