#include "Calculator.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <limits>

float Calculator::calculateAverage(const std::vector<float>& vec) {
    if (vec.empty()) {
        return 0.0f;
    }
    
    float sum = 0.0f;
    for (const auto& value : vec) {
        // Проверка переполнения
        if (value > 0 && sum > std::numeric_limits<float>::max() - value) {
            return std::numeric_limits<float>::infinity(); // +inf
        }
        if (value < 0 && sum < -std::numeric_limits<float>::max() - value) {
            return -std::numeric_limits<float>::infinity(); // -inf
        }
        sum += value;
    }
    
    return sum / vec.size();
}

bool readExactly(int socket, void* buffer, size_t length) {
    char* ptr = static_cast<char*>(buffer);
    size_t totalRead = 0;
    
    while (totalRead < length) {
        ssize_t bytesRead = recv(socket, ptr + totalRead, length - totalRead, 0);
        if (bytesRead <= 0) {
            if (bytesRead == 0) {
                std::cerr << "Connection closed by client" << std::endl;
            } else {
                std::cerr << "recv error: " << strerror(errno) << std::endl;
            }
            return false;
        }
        totalRead += bytesRead;
    }
    return true;
}

uint16_t Calculator::processVectors(int socket) {
    uint32_t numberOfVectors;

    // Читаем количество векторов (little-endian)
    if (!readExactly(socket, &numberOfVectors, sizeof(uint32_t))) {
        std::cerr << "Failed to read number of vectors" << std::endl;
        return -1;
    }
    std::cerr << "Number of vectors: " << numberOfVectors << std::endl;

    for (uint32_t i = 0; i < numberOfVectors; ++i) {
        uint32_t vectorSize;

        // Читаем размер вектора (little-endian)
        if (!readExactly(socket, &vectorSize, sizeof(uint32_t))) {
            std::cerr << "Failed to read vector size for vector " << i << std::endl;
            return -1;
        }
        
        std::cerr << "Vector " << i << " size: " << vectorSize << std::endl;

        if (vectorSize > 1000000) {
            std::cerr << "Vector size too large: " << vectorSize << std::endl;
            return -1;
        }

        // Читаем данные вектора (float в little-endian)
        std::vector<float> vectorData(vectorSize);
        if (!readExactly(socket, vectorData.data(), vectorSize * sizeof(float))) {
            std::cerr << "Failed to read vector data for vector " << i << std::endl;
            return -1;
        }

        float result = calculateAverage(vectorData);
        std::cerr << "Calculated average: " << result << std::endl;

        if (send(socket, &result, sizeof(float), 0) <= 0) {
            std::cerr << "Failed to send result for vector " << i << std::endl;
            return -1;
        }
    }

    return 0;
}