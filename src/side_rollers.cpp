/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

void intake_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true) {
    if (get_active_roller_state() == SCORE) {
      intake_set(intake_speed);
    } else if (get_active_roller_state() == INTAKE) {
      intake_set(intake_speed);
    } else if (get_active_roller_state() == OUTTAKE) {
      intake_set(-intake_speed);
    } else if (get_active_roller_state() == STOP) {
      intake_set(0);
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Intake_Task(intake_task);  // Create the task, this will cause the function to start running
