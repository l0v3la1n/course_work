#include "ConnectToBase.h"
#include "Error.h"
#include <fstream>
#include <sstream>
#include <openssl/sha.h>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <random>
#include <cctype>

bool ConnectToBase::authenticateUser(const std::string& login, const std::string& salt, const std::string& clientHash, const std::string& dbFileName) {
    std::ifstream dbFile(dbFileName);
    if (!dbFile.is_open()) {
        Error::logError("Cannot open database file.", true);
        return false;
    }

    std::string dbLogin, dbPassword;
    while (dbFile >> dbLogin >> dbPassword) {
        if (dbLogin == login) {
            std::string serverHash = hashPassword(dbPassword, salt);
            return compareHashes(serverHash, clientHash);
        }
    }
    return false;
}

std::string ConnectToBase::generateSalt() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    uint64_t salt = dis(gen);
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << salt;
    return ss.str();
}

std::string ConnectToBase::hashPassword(const std::string& password, const std::string& salt) {
    std::string data = salt + password;
    unsigned char result[SHA224_DIGEST_LENGTH];
    SHA224(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result);

    std::stringstream hashStream;
    for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i) {
        hashStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    return hashStream.str();
}

bool ConnectToBase::compareHashes(const std::string& serverHash, const std::string& clientHash) {
    std::string serverUpper = serverHash;
    std::string clientUpper = clientHash;
    std::transform(serverUpper.begin(), serverUpper.end(), serverUpper.begin(), ::toupper);
    std::transform(clientUpper.begin(), clientUpper.end(), clientUpper.begin(), ::toupper);
    
    return serverUpper == clientUpper;
}

bool ConnectToBase::userExists(const std::string& login, const std::string& dbFileName) {
    std::ifstream dbFile(dbFileName);
    if (!dbFile.is_open()) {
        return false;
    }

    std::string dbLogin, dbPassword;
    while (dbFile >> dbLogin >> dbPassword) {
        if (dbLogin == login) {
            return true;
        }
    }
    return false;
}