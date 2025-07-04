/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

const int slow_speed = 50;  // Speed for scoring into the middle goal

// Set conveyor motors through antijam
void conveyor_bottom_set(int input) { conveyor_bottom_antijam.set_motors(input); }
void conveyor_top_set(int input) { conveyor_top_antijam.set_motors(input); }

// Bottom conveyor task
void conveyor_bottom_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  while (true) {
    // Bottom conveyor is intaking
    if (get_active_roller_state() == INTAKE) {
      conveyor_bottom_set(127);
    }

    // Bottom conveyor is outtaking
    else if (get_active_roller_state() == OUTTAKE) {
      conveyor_bottom_set(-127);
    }

    // Bottom conveyor is scoring
    else if (get_active_roller_state() == SCORE) {
      conveyor_bottom_set(127);
    }

    // Bottom conveyor is scoring slowly
    else if (get_active_roller_state() == SCORE_SLOWLY) {
      conveyor_bottom_set(slow_speed);
    }

    // Bottom conveyor is pooping
    else if (get_active_roller_state() == POOP) {
      conveyor_bottom_set(127);
    }

    // Bottom conveyor is doing nothing
    else {
      conveyor_bottom_set(0);
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Conveyor_Bottom_Task(conveyor_bottom_task);  // Create the task, this will cause the function to start running

// Top conveyor task
void conveyor_top_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running
  bool is_ball = false;
  bool last_is_ball = is_ball;
  int is_ball_timer = 0;
  int amount_of_balls = 0;
  conveyor_top_antijam.disable();
  while (true) {
    // Check if there is currently a ball past the proximity of the optical sensor
    last_is_ball = is_ball;
    if (!is_ball) {
      if (optical.get_proximity() >= 255) {
        if (is_ball_timer > 50) {
          is_ball = true;
        }
        is_ball_timer += ez::util::DELAY_TIME;
      } else {
        is_ball_timer = 0;
      }
    } else {
      if (optical.get_proximity() < 255) {
        if (is_ball_timer > 50) {
          is_ball = false;
        }
        is_ball_timer += ez::util::DELAY_TIME;
      } else {
        is_ball_timer = 0;
      }
    }

    if (last_is_ball == true && is_ball == false) {
      amount_of_balls++;
    }

    // Top conveyor is intaking
    if (get_active_roller_state() == INTAKE) {
      conveyor_top_antijam.stick_enable();

      // Change the speed of the motor depending on if there's a ball there or not
      if (amount_of_balls == 0) {
        conveyor_top_set(30);
      } else {
        conveyor_top_set(-1);
      }

    }

    // Top conveyor is outtaking
    else if (get_active_roller_state() == OUTTAKE) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;
      conveyor_top_set(-127);
    }

    // Top conveyor is scoring
    else if (get_active_roller_state() == SCORE) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;

      // Automatic color sorting when scoring
      if (is_color_sorting())
        conveyor_top_set(-127);
      else
        conveyor_top_set(127);

    }

    // Top conveyor is scoring on the middle goal
    else if (get_active_roller_state() == SCORE_SLOWLY) {
      conveyor_top_set(slow_speed);
    }

    // Top conveyor is pooping
    else if (get_active_roller_state() == POOP) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;
      conveyor_top_set(-127);
    }

    // Top conveyor is not moving
    else {
      conveyor_top_set(0);
    }

    pros::delay(ez::util::DELAY_TIME);

    // Run normal antijam when scoring, outtaking, pooping
    // Run stronger antijam when intaking that will delay trying to unjam
    // if (get_active_roller_state() != INTAKE) {
    //   conveyor_top_antijam.stick_disable();
    // } else {
    //   conveyor_top_antijam.stick_enable();
    // }
  }
}
pros::Task Conveyor_Top_Task(conveyor_top_task);  // Create the task, this will cause the function to start running
