/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "antijam.hpp"

antijam::antijam(pros::Motor *input_motor, int input_wait_time, int input_outtake_time, int input_min_speed)
    : antijam_task([this] { this->antijam_function(); }) {
  motor = input_motor;

  wait_time_set(input_wait_time);
  outtake_time_set(input_outtake_time);
  min_activate_speed_set(input_min_speed);

  antijam_task.resume();
}

antijam::antijam(pros::MotorGroup *input_motors, int input_wait_time, int input_outtake_time, int input_min_speed)
    : antijam_task([this] { this->antijam_function(); }) {
  motors = input_motors;

  wait_time_set(input_wait_time);
  outtake_time_set(input_outtake_time);
  min_activate_speed_set(input_min_speed);

  antijam_task.resume();
}

void antijam::set_motors_raw(int input) {
  if (pros::millis() < 1500) return;

  if (motor != NULL)
    motor->move(input);
  else
    motors->move(input);
}

void antijam::set_motors(int input) {
  actual_speed = input;
  if (input != last_input_speed) set_motors_raw(input);
  last_input_speed = input;
}

double antijam::get_velocity() {
  if (motor != NULL)
    return motor->get_actual_velocity();
  else
    return motors->get_actual_velocity();
}

void antijam::enable() { is_enabled = true; }
bool antijam::enabled() { return is_enabled; }
void antijam::disable() {
  is_enabled = false;
  set_motors(real_speed());
}

void antijam::wait_time_set(int input) { wait_time = input; }
void antijam::outtake_time_set(int input) { outtake_time = input; }
void antijam::min_activate_speed_set(int input) { min_speed = input; }

int antijam::wait_time_get() { return wait_time; }
int antijam::outtake_time_get() { return outtake_time; }
int antijam::min_activate_speed_get() { return min_speed; }

int antijam::real_speed() { return actual_speed; }

void antijam::antijam_function() {
  pros::delay(1500);

  int jam_counter = 0;
  while (true) {
    // Only run antijam when enabled
    if (enabled()) {
      // Run intake full power in opposite direction for hook_outtake_time ms when jammed, then
      // set intake back to normal
      if (is_jammed) {
        set_motors_raw(-127 * ez::util::sgn(real_speed()));
        jam_counter += ez::util::DELAY_TIME;
        if (jam_counter > outtake_time_get()) {
          is_jammed = false;
          jam_counter = 0;
          set_motors_raw(real_speed());
        }
      }

      // Detect a jam if velocity is 0 for hook_wait_time ms
      else if (abs(real_speed()) >= min_activate_speed_get() && get_velocity() == 0.0) {
        jam_counter += ez::util::DELAY_TIME;
        if (jam_counter > wait_time_get()) {
          jam_counter = 0;
          is_jammed = true;
        }
      }

      // Reset jam_counter when button is released
      if (abs(real_speed()) <= min_activate_speed_get()) {
        jam_counter = 0;
      }
    } else {
      // Reset jam counter when antijam is disabled
      jam_counter = 0;
    }

    // printf("is enabled: %i      is jammed: %i   real speed: %i   min speed: %i   vel: %f      counter: %i\n", (int)enabled(), (int)is_jammed, real_speed(), min_activate_speed_get(), get_velocity(), jam_counter);

    pros::delay(ez::util::DELAY_TIME);
  }
}