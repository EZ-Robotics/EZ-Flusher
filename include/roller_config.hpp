/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

enum roller_states {
  STOP = 0,
  INTAKE = 1,
  INTAKE_ONLY = 2,
  OUTTAKE = 3,
  OUTTAKE_ONLY = 4,
  SCORE = 5,
  POOP = 6,
};

inline roller_states active_roller_state = STOP;
roller_states get_active_roller_state();
inline int intake_speed = 127;

void set_active_roller_state(roller_states new_state, int new_intake_speed = 127);