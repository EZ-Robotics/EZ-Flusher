/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

// Get the current roller state
roller_states get_active_roller_state() { return active_roller_state; }

// Set a new roller state
void set_active_roller_state(roller_states new_state, int new_intake_speed) {
  active_roller_state = new_state;
  intake_speed = std::abs(intake_speed);
}