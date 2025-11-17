#include "ClientCommunicate.h"
#include "ConnectToBase.h"
#include "Calculator.h"
#include "Interface.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <algorithm>

std::string ClientCommunicate::readLogin(int socket) {
    char buffer[256] = {0};
    
    ssize_t bytesRead = recv(socket, buffer, 255, 0);
    if (bytesRead <= 0) {
        return "";
    }
    
    std::string login(buffer, bytesRead);
    size_t endPos = login.find_last_not_of(" \n\r\t");
    if (endPos != std::string::npos) {
        login = login.substr(0, endPos + 1);
    }
    
    return login;
}

void ClientCommunicate::communicate(int socket, const std::string& userDbFileName, const std::string& logFileName) {
    std::string login = readLogin(socket);
    if (login.empty()) {
        Interface::logError(logFileName, "Failed to read login from client", false);
        const char* err_msg = "ERR";
        send(socket, err_msg, 3, 0);
        close(socket);
        return;
    }

    Interface::logMessage(logFileName, "Received login: " + login);

    ConnectToBase dbConnection;

    if (!dbConnection.userExists(login, userDbFileName)) {
        Interface::logError(logFileName, "User not found: " + login, false);
        const char* err_msg = "ERR";
        send(socket, err_msg, 3, 0);
        close(socket);
        return;
    }

    std::string salt = dbConnection.generateSalt();
    Interface::logMessage(logFileName, "Generated salt: " + salt);
    
    if (send(socket, salt.c_str(), 16, 0) <= 0) {
        Interface::logError(logFileName, "Failed to send salt to client", false);
        close(socket);
        return;
    }

    char hashBuffer[57] = {0};
    ssize_t hashBytes = recv(socket, hashBuffer, 56, 0);
    if (hashBytes <= 0) {
        Interface::logError(logFileName, "Failed to receive hash from client", false);
        close(socket);
        return;
    }
    
    std::string clientHash(hashBuffer, hashBytes);
    Interface::logMessage(logFileName, "Received hash: " + clientHash);

    if (dbConnection.authenticateUser(login, salt, clientHash, userDbFileName)) {
        const char* ok_msg = "OK";
        send(socket, ok_msg, 2, 0);
        Interface::logMessage(logFileName, "User " + login + " authenticated successfully.");
        
        Calculator calc;
        if (calc.processVectors(socket) != 0) {
            Interface::logError(logFileName, "Error processing vectors.", false);
        }
    } else {
        Interface::logError(logFileName, "Authentication failed for user: " + login, false);
        const char* err_msg = "ERR";
        send(socket, err_msg, 3, 0);
    }
    
    close(socket);
}