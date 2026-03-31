/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#pragma once

#include <chrono>
#include <sstream>

std::ostream& format_time(std::ostream &out, std::chrono::nanoseconds ns);

std::string format_time(std::chrono::nanoseconds ns);
