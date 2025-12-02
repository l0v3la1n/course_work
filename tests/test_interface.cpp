#include <UnitTest++/UnitTest++.h>
#include "../Interface.h"
#include <sstream>

TEST(Help_Short)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-h", nullptr};
    int argc = 2;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(Help_Long)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "--help", nullptr};
    int argc = 2;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(Help_WithUser)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-h", "-b", "base.txt", nullptr};
    int argc = 4;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(CorrectParams)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log.txt", "-b", "userdb.txt", "-p", "44444", nullptr};
    int argc = 7;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 0);
}

TEST(NoParams)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", nullptr};
    int argc = 1;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(InvalidPort)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log", "-b", "db", "-p", "9999999", nullptr};
    int argc = 7;

    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == -1);
}

TEST(HelpWithLogFile)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log.txt", "-h", nullptr};
    int argc = 4;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(HelpWithUserDb)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-b", "userdb.txt", "--help", nullptr};
    int argc = 4;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(PortOutOfRange)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log.txt", "-b", "db.txt", "-p", "0", nullptr};
    int argc = 7;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == -1);
    CHECK(buffer.str().find("Port value") != std::string::npos);
}

TEST(PortNegative)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log.txt", "-b", "db.txt", "-p", "-1", nullptr};
    int argc = 7;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == -1);
    // Ищем либо "Port value", либо "Invalid port"
    std::string bufferStr = buffer.str();
    CHECK((bufferStr.find("Port value") != std::string::npos || 
           bufferStr.find("Invalid port") != std::string::npos));
}

TEST(RequiredParamsMissing)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-l", "log.txt", nullptr};
    int argc = 3;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}

TEST(RequiredParamsMissing2)
{
    std::stringstream buffer;
    const char* argv[] = {"prog", "-b", "db.txt", nullptr};
    int argc = 3;
    
    int result = Interface::getParseResult(argc, const_cast<char**>(argv), buffer);
    CHECK(result == 1);
    CHECK(buffer.str().find("Usage") != std::string::npos);
}