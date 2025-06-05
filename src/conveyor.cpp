/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

void conveyor_bottom_set(int input) { conveyor_bottom_antijam.set_motors(input); }
void conveyor_top_set(int input) { conveyor_top_antijam.set_motors(input); }

void conveyor_bottom_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true) {
    if (get_active_roller_state() == INTAKE) {
      conveyor_bottom_set(127);
    } else if (get_active_roller_state() == OUTTAKE) {
      conveyor_bottom_set(-127);
    } else if (get_active_roller_state() == SCORE) {
      conveyor_bottom_set(127);
    } else if (get_active_roller_state() == POOP) {
      conveyor_bottom_set(127);
    } else {
      conveyor_bottom_set(0);
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Conveyor_Bottom_Task(conveyor_bottom_task);  // Create the task, this will cause the function to start running

void conveyor_top_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true) {
    // Run normal antijam when scoring, outtaking, pooping
    // Run stronger antijam when intaking that will delay trying to unjam
    if (get_active_roller_state() != INTAKE) {
      conveyor_top_antijam.stick_disable();
    } else {
      conveyor_top_antijam.stick_enable();
    }

    if (get_active_roller_state() == INTAKE) {
      conveyor_top_set(20);
    } else if (get_active_roller_state() == OUTTAKE) {
      conveyor_top_set(-127);
    } else if (get_active_roller_state() == SCORE) {
      conveyor_top_set(127);
    } else if (get_active_roller_state() == POOP) {
      conveyor_top_set(-127);
    } else {
      conveyor_top_set(0);
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Conveyor_Top_Task(conveyor_top_task);  // Create the task, this will cause the function to start running
