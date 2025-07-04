/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

// Limit intake current
const int limited_power = 1750;
void intake_current_limit() {
  intake_left.set_current_limit(limited_power);
  intake_right.set_current_limit(limited_power);
}

// Set intake current to full power
const int full_power = 2500;
void intake_full_power() {
  intake_left.set_current_limit(full_power);
  intake_right.set_current_limit(full_power);
}

// Set intake motors through antijam
inline void intake_left_set(int input) { intake_left_antijam.set_motors(input); }
inline void intake_right_set(int input) { intake_right_antijam.set_motors(input); }
inline void intake_set(int input) {
  intake_left_set(input);
  intake_right_set(input);
}

// Intake task
void intake_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  int output_speed = 0;

  while (true) {
    // Run normal antijam when intaking, outtaking, pooping
    // Run stronger antijam when scoring that will delay trying to unjam
    if (get_active_roller_state() != SCORE) {
      intake_left_antijam.stick_disable();
      intake_right_antijam.stick_disable();
      intake_full_power();
    } else {
      intake_left_antijam.stick_enable();
      intake_right_antijam.stick_enable();
      intake_current_limit();
    }

    // Side rollers are scoring
    if (get_active_roller_state() == SCORE) {
      output_speed = intake_speed;
    }

    // Side rollers are scoring slowly
    else if (get_active_roller_state() == SCORE_SLOWLY) {
      output_speed = intake_speed;
    }

    // Side rollers are intaking
    else if (get_active_roller_state() == INTAKE || get_active_roller_state() == INTAKE_ONLY) {
      output_speed = intake_speed;
    }

    // Side rollers are outtaking
    else if (get_active_roller_state() == OUTTAKE || get_active_roller_state() == OUTTAKE_ONLY) {
      output_speed = -intake_speed;
    }

    // Side rollers are pooping
    else if (get_active_roller_state() == POOP) {
      output_speed = intake_speed;
    }

    // Side rollers rae stopped
    else if (get_active_roller_state() == STOP) {
      output_speed = 0;
    }

    intake_set(output_speed);

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Intake_Task(intake_task);  // Create the task, this will cause the function to start running
