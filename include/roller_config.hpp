/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

enum roller_states {
  STOP = 0,
  INTAKE = 1,
  OUTTAKE = 2,
  SCORE = 3,
  POOP = 4,
};

inline roller_states active_roller_state = STOP;
inline roller_states get_active_roller_state() { return active_roller_state; }
inline int intake_speed = 127;

inline void set_active_roller_state(roller_states new_state, int new_intake_speed = 127) {
  active_roller_state = new_state;
  intake_speed = std::abs(intake_speed);
}