//
// Created by gil on 9.1.2021.
//

#include "Duration.h"

Duration::Duration(std::chrono::duration<double> duration) {
    normalize_duration(duration);
}

void Duration::normalize_duration(std::chrono::duration<double> duration) {
    long NANOSECONDS_IN_MILLISECONDS = 1000000;
    long MILISECONDS_IN_SECOND = 1000;
    long SECONDS_IN_MINUTE = 60;
    long MINUTES_IN_HOUR = 60;

    // get duration in seconds
    double time_duration_in_nanoseconds = duration_cast<nanoseconds>(duration).count();
    if (time_duration_in_nanoseconds > (NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND * SECONDS_IN_MINUTE * MINUTES_IN_HOUR)) {
        _duration = duration_cast<hours>(duration).count();
        _time_unit = TIME_UNIT::H;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND * SECONDS_IN_MINUTE) {
        _duration = duration_cast<minutes>(duration).count();
        _time_unit = TIME_UNIT::M;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS * MILISECONDS_IN_SECOND) {
        _duration = duration_cast<seconds>(duration).count();
        _time_unit = TIME_UNIT::S;
    }
    else if (time_duration_in_nanoseconds > NANOSECONDS_IN_MILLISECONDS) {
        _duration = duration_cast<milliseconds>(duration).count();
        _time_unit = TIME_UNIT::MS;
    }
    else {
        _duration = time_duration_in_nanoseconds;
        _time_unit = TIME_UNIT::NS;
    }
}

std::string Duration::to_string() {
    std::ostringstream ss;
    ss << *this;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Duration& duration) {
    std::string unit_string;
    switch (duration._time_unit) {
        case Duration::TIME_UNIT::NS:
            unit_string = "ns";
            break;

        case Duration::TIME_UNIT::MS:
            unit_string = "ms";
            break;

        case Duration::TIME_UNIT::S:
            unit_string = "s";
            break;

        case Duration::TIME_UNIT::M:
            unit_string = "m";
            break;

        case Duration::TIME_UNIT::H:
            unit_string = "h";
            break;
    }
    os << duration._duration << " " << unit_string;
    return os;
}
