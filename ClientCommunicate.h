#ifndef CLIENTCOMMUNICATE_H
#define CLIENTCOMMUNICATE_H

#include <string>
#include <vector>
#include <sys/socket.h>

class ClientCommunicate {
public:
    void communicate(int socket, const std::string& userDbFileName, const std::string& logFileName);

private:
    std::string readLogin(int socket);
};

#endif