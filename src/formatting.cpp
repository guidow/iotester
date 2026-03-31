/*
 * Copyright 2026 Guido Winkelmann
 *
 * Licensed under the terms of the GNU General Public License version 3
 */

#include "formatting.h"

#include <iomanip>

std::ostream& format_time(std::ostream &out, std::chrono::nanoseconds ns) {
    typedef std::chrono::duration<int, std::ratio<86400>> days_t;

    char fill = out.fill();
    out.fill('0');

    auto days = std::chrono::duration_cast<days_t>(ns);
    ns -= days;
    auto hours = std::chrono::duration_cast<std::chrono::hours>(ns);
    ns -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(ns);
    ns -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(ns);
    ns -= seconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(ns);
    ns -= microseconds;

    if(days.count() > 0) {
        out << days.count() << "d ";
    }
    if(days.count() > 0
       || hours.count() > 0) {
        out << std::setw(2) << hours.count() << ":";
    }
    if(days.count() > 0
       || hours.count() > 0
       || minutes.count() > 0) {
        out << std::setw(2) << minutes.count() << ":";
    }
    if(days.count() > 0
       || hours.count() > 0
       || minutes.count() > 0
       || seconds.count() > 0) {
        out << std::setw(2) << seconds.count() << ".";
    }
    out << microseconds.count();

    out.fill(fill);
    return out;
}

std::string format_time(std::chrono::nanoseconds ns) {
    std::ostringstream out;

    format_time(out, ns);

    return out.str();
}
