//
// Created by gil on 9.1.2021.
//

#ifndef SECURE_DURATION_H
#define SECURE_DURATION_H

#include <iostream>
#include <sstream>
#include <chrono>

using namespace std::chrono;

class Duration {
private:
    enum class TIME_UNIT {NS, MS, S, M, H};
    double _duration;
    TIME_UNIT _time_unit;

private:
    void normalize_duration(std::chrono::duration<double> duration);

public:
    explicit Duration(std::chrono::duration<double> duration);

    std::string to_string();

    friend std::ostream& operator<<(std::ostream& os, const Duration& duration);
};


#endif //SECURE_DURATION_H
