/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "setup.hpp"

inline void conveyor_bottom_set(int input) { conveyor_bottom_antijam.set_motors(input); }
inline void conveyor_top_set(int input) { conveyor_top_antijam.set_motors(input); }