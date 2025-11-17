#ifndef CONNECTTOBASE_H
#define CONNECTTOBASE_H

#include <string>

class ConnectToBase {
public:
    bool authenticateUser(const std::string& login, const std::string& salt, const std::string& clientHash, const std::string& dbFileName);
    std::string generateSalt();
    std::string hashPassword(const std::string& password, const std::string& salt);
    bool userExists(const std::string& login, const std::string& dbFileName);

private:
    bool compareHashes(const std::string& serverHash, const std::string& clientHash);
};

#endif