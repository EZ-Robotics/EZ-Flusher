/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

// Current alliance and ball color
ball_color get_current_ball_color() { return current_ball_color; }
void set_alliance_color(ball_color input) { current_alliance_color = input; }
ball_color get_alliance_color() { return current_alliance_color; }

// Functions to set sorting state
void auto_poop_disable() { auto_poop_enable_bool = false; }
void auto_poop_enable() { auto_poop_enable_bool = true; }
bool auto_poop_enabled() { return auto_poop_enable_bool; }

bool is_color_sorting() { return is_color_sorting_bool; }

// Color detection task
void color_detection_task() {
  pros::delay(2000);  // Set EZ-Template calibrate before this function starts running

  optical.set_integration_time(3);

  int blue_timer = 0;
  int red_timer = 0;

  int wait_for_timer = 50;  // ms to wait

  int color_sort_timer = 0;
  int color_sort_for_this_long = 1000;
  while (true) {
    // Run this only when auto pooping is enabled
    if (auto_poop_enabled()) {
      optical.set_led_pwm(100);  // Turn on the LED

      // If sensor detects blue
      if (optical.get_hue() > 200 && optical.get_hue() < 250) {
        if (blue_timer >= wait_for_timer) {
          blue_timer = wait_for_timer;
          current_ball_color = BLUE;
          // printf("BLUE\n");
        }

        blue_timer += ez::util::DELAY_TIME;
        red_timer = 0;  // Reset red timer
      }

      // If sensor detects red
      else if (optical.get_hue() < 20 && optical.get_hue() > 0) {
        if (red_timer >= wait_for_timer) {
          red_timer = wait_for_timer;
          current_ball_color = RED;
          // printf("RED\n");
        }

        red_timer += ez::util::DELAY_TIME;
        blue_timer = 0;  // Reset blue timer
      }

      // When neither color is detected, reset both timers
      else {
        blue_timer = 0;
        red_timer = 0;
        current_ball_color = NONE;
      }

      // If the wrong ball is detected
      if (get_current_ball_color() != get_alliance_color() && get_alliance_color() != NONE && get_current_ball_color() != NONE) {
        is_color_sorting_bool = true;
      }

      if (is_color_sorting_bool) {
        if (color_sort_timer >= color_sort_for_this_long) {
          is_color_sorting_bool = false;
          color_sort_timer = 0;
        }
        color_sort_timer += ez::util::DELAY_TIME;
      }
    }

    // Disable color sort
    else {
      optical.set_led_pwm(0);
      blue_timer = 0;
      red_timer = 0;
      current_ball_color = NONE;
      color_sort_timer = 0;
      is_color_sorting_bool = false;
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task Color_Detection_Task(color_detection_task);  // Create the task, this will cause the function to start running

// Button to toggle color sort in opcontrol
void color_detection_opcontrol() {
  if (master.get_digital_new_press(auto_poop_enable_button)) {
    if (auto_poop_enabled())
      auto_poop_disable();
    else
      auto_poop_enable();
  }
}