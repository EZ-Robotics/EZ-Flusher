/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

inline void intake_left_set(int input) { intake_left_antijam.set_motors(input); }
inline void intake_right_set(int input) { intake_right_antijam.set_motors(input); }
inline void intake_set(int input) {
  intake_left_set(input);
  intake_right_set(input);
}

void intake_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  int output_speed = 0;
  int jam_timer = 0;
  bool is_jammed_while_scoring = false;

  while (true) {
    // While scoring, disable antijam and enable the stronger custom antijam for the side rollers
    // When not scoring, reset the intake jam flag
    if (get_active_roller_state() != SCORE) {
      jam_timer = 0;
      is_jammed_while_scoring = false;
      intake_left_antijam.enable();
      intake_right_antijam.enable();
    } else {
      intake_left_antijam.disable();
      intake_right_antijam.disable();
    }

    // Side rollers are scoring
    if (get_active_roller_state() == SCORE) {
      // Check if the intakes are stalled or drawing too many amps
      if ((fabs(intake_left.get_actual_velocity()) == 0.0 || intake_left.is_over_current()) ||
          (fabs(intake_right.get_actual_velocity()) == 0.0 || intake_right.is_over_current())) {
        // If the intakes have been jammed while scoring for ~50ms, set this flag to true
        if (jam_timer > intake_wait_time)
          is_jammed_while_scoring = true;

        jam_timer += ez::util::DELAY_TIME;
      } else {
        jam_timer = 0;
      }

      // If jammed, don't run the intake.  If not jammed, run the intake like normal
      output_speed = is_jammed_while_scoring ? 0 : intake_speed;
    }

    // Side rollers are intaking
    else if (get_active_roller_state() == INTAKE) {
      output_speed = intake_speed;
    }

    // Side rollers are outtaking
    else if (get_active_roller_state() == OUTTAKE) {
      output_speed = -intake_speed;
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
