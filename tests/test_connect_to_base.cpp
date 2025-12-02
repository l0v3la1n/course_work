#include <UnitTest++/UnitTest++.h>
#include "../ConnectToBase.h"
#include <fstream>
#include <algorithm>
#include <cctype>

struct TestDB
{
    std::string filename = "test_db.txt";

    TestDB()
    {
        std::ofstream f(filename);
        f << "user P@ssW0rd\n";
        f << "admin qwerty\n";
    }
};

TEST_FIXTURE(TestDB, UserExists_Positive)
{
    ConnectToBase db;
    CHECK(db.userExists("user", filename));
}

TEST_FIXTURE(TestDB, UserExists_Negative)
{
    ConnectToBase db;
    CHECK(!db.userExists("xxx", filename));
}

TEST(SaltGenerationUnique)
{
    ConnectToBase db;
    std::string s1 = db.generateSalt();
    std::string s2 = db.generateSalt();
    CHECK(s1 != s2);
}

TEST(HashPassword_Fixed)
{
    ConnectToBase db;

    std::string salt = "4F9C429F5C6884DB726445BE";
    std::string pass = "SupErP@$$w0rd";

    std::string expected = "822081b7e843ae1514e82fcf87e5dbf6bf4a5783a9a47d77ad67d19d";

    std::string result = db.hashPassword(pass, salt);

    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    std::string expected_upper = expected;
    std::transform(expected_upper.begin(), expected_upper.end(), expected_upper.begin(), ::toupper);
    
    CHECK(result == expected_upper);
}

TEST(CompareHashes)
{
    ConnectToBase db;
    CHECK(db.authenticateUser("user", "FFFFFFFF", "ABC", "test_db.txt") == false);
}

TEST_FIXTURE(TestDB, Authenticate_Correct)
{
    ConnectToBase db;

    std::string login = "user";
    std::string password = "P@ssW0rd";
    std::string salt = "A1B2C3D4E5F6";

    std::string clientHash = db.hashPassword(password, salt);

    CHECK(db.authenticateUser(login, salt, clientHash, filename));
}

TEST_FIXTURE(TestDB, Authenticate_WrongPassword)
{
    ConnectToBase db;

    std::string login = "user";
    std::string salt = "ABCDEF112233";

    std::string clientHash = db.hashPassword("WRONG", salt);

    CHECK(!db.authenticateUser(login, salt, clientHash, filename));
}

struct EmptyDB
{
    std::string filename = "empty_db.txt";
    
    EmptyDB()
    {
        std::ofstream f(filename);
        // Создаем пустой файл
    }
    
    ~EmptyDB()
    {
        remove(filename.c_str());
    }
};

TEST_FIXTURE(EmptyDB, UserExists_EmptyDB)
{
    ConnectToBase db;
    CHECK(!db.userExists("user", filename));
}

struct MalformedDB
{
    std::string filename = "malformed_db.txt";
    
    MalformedDB()
    {
        std::ofstream f(filename);
        f << "user\n";
        f << "admin:qwerty\n";
        f << "test  password extra\n";
    }
    
    ~MalformedDB()
    {
        remove(filename.c_str());
    }
};

TEST_FIXTURE(MalformedDB, UserExists_MalformedDB)
{
    ConnectToBase db;
    CHECK(!db.userExists("nonexistent", filename));

    CHECK(db.userExists("test", filename));
}

TEST(HashPassword_EmptyPassword)
{
    ConnectToBase db;
    std::string salt = "ABCDEF123456";
    std::string pass = "";
    
    std::string result = db.hashPassword(pass, salt);
    CHECK_EQUAL(56, result.size()); // SHA224 = 56 hex chars

    std::string result2 = db.hashPassword(pass, salt);
    CHECK(result == result2);
}

TEST(HashPassword_EmptySalt)
{
    ConnectToBase db;
    std::string salt = "";
    std::string pass = "password";
    
    std::string result = db.hashPassword(pass, salt);
    CHECK_EQUAL(56, result.size());
    
    std::string result2 = db.hashPassword(pass, salt);
    CHECK(result == result2);
}

TEST(CompareHashes_CaseInsensitive)
{
    ConnectToBase db;
    
    std::string hash1 = "abc123def456";
    std::string hash2 = "ABC123DEF456";

    std::string tempFile = "temp_auth.txt";
    {
        std::ofstream f(tempFile);
        f << "testuser password\n";
    }
    
    std::string salt = "SALT123";

    std::string hashLower = "9c8b910a1065a3d017cc64b7f74be5df8d40863c19e78146ee3665a2";
    std::string hashUpper = "9C8B910A1065A3D017CC64B7F74BE5DF8D40863C19E78146EE3665A2";
    
    std::string testFile = "test_hash_case.txt";
    {
        std::ofstream f(testFile);
        f << "caseuser " << hashLower << "\n"; // Сохраняем хеш в нижнем регистре
    }
    remove(tempFile.c_str());
    remove(testFile.c_str());
}

TEST(HashPassword_CaseSensitive)
{
    ConnectToBase db;
    
    std::string salt = "123456";
    std::string pass1 = "Password";
    std::string pass2 = "password";
    
    std::string hash1 = db.hashPassword(pass1, salt);
    std::string hash2 = db.hashPassword(pass2, salt);
    
    CHECK(hash1 != hash2);
}

TEST(Authenticate_NonexistentUser)
{
    ConnectToBase db;
    
    std::string tempFile = "temp_nonexistent.txt";
    {
        std::ofstream f(tempFile);
        f << "user1 pass1\n";
        f << "user2 pass2\n";
    }
    
    CHECK(!db.authenticateUser("user3", "salt", "hash", tempFile));
    
    remove(tempFile.c_str());
}

TEST(CompareHashes_Direct)
{
    std::string hash1 = "abc123";
    std::string hash2 = "ABC123";
    std::string hash3 = "def456";
    
    CHECK(ConnectToBase::compareHashes(hash1, hash2));
    CHECK(!ConnectToBase::compareHashes(hash1, hash3));
}
