/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

int slow_speed = 50;

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
    } else if (get_active_roller_state() == SCORE_SLOWLY) {
      conveyor_bottom_set(slow_speed);
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
  bool is_ball = false;
  bool last_is_ball = is_ball;
  int is_ball_timer = 0;
  int amount_of_balls = 0;
  conveyor_top_antijam.disable();
  while (true) {
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

    printf("is ball %i   last ball: %i      amount: %i \n", (int)is_ball, (int)last_is_ball, amount_of_balls);

    if (get_active_roller_state() == INTAKE) {
      conveyor_top_antijam.stick_enable();

      if (amount_of_balls == 0) {
        conveyor_top_set(30);
      } else {
        conveyor_top_set(-1);
      }

    } else if (get_active_roller_state() == OUTTAKE) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;
      conveyor_top_set(-127);
    } else if (get_active_roller_state() == SCORE) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;
      if (is_color_sorting())
        conveyor_top_set(-127);
      else
        conveyor_top_set(127);

    } else if (get_active_roller_state() == SCORE_SLOWLY) {
      conveyor_top_set(slow_speed);
    } else if (get_active_roller_state() == POOP) {
      conveyor_top_antijam.stick_disable();
      amount_of_balls = 0;
      conveyor_top_set(-127);
    } else {
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
