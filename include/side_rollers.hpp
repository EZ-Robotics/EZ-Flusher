/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "setup.hpp"

inline void intake_left_set(int input) { intake_right_antijam.set_motors(input); }
inline void intake_right_set(int input) { intake_left_antijam.set_motors(input); }
inline void intake_set(int input) {
  intake_left_set(input);
  intake_right_set(input);
}
