/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

inline bool is_color_sorting_bool = false;
inline bool auto_poop_enable_bool = true;
void auto_poop_disable();
void auto_poop_enable();
bool auto_poop_enabled();

enum ball_color {
  NONE = 0,
  RED = 1,
  BLUE = 2
};

inline ball_color current_ball_color = NONE;
ball_color get_current_ball_color();

void set_alliance_color(ball_color input);
ball_color get_alliance_color();

inline ball_color current_alliance_color = NONE;

void color_detection_opcontrol();

bool is_color_sorting();