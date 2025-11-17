#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <cstdint>
#include <sys/socket.h>

class Calculator {
public:
    uint16_t processVectors(int socket);
    static float calculateAverage(const std::vector<float>& vec);
};

#endif