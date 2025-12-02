#include <limits>
#include <UnitTest++/UnitTest++.h>
#include "../Calculator.h"
#include <cmath>

TEST(Average_Normal)
{
    std::vector<float> v = {1.f, 2.f, 3.f};
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(2.0f, result, 1e-6);
}

TEST(Average_Empty)
{
    std::vector<float> v;
    float result = Calculator::calculateAverage(v);
    CHECK_EQUAL(0.0f, result);
}

TEST(Average_Overflow_Positive)
{
    std::vector<float> v = {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
    float res = Calculator::calculateAverage(v);

    CHECK(std::isinf(res));
    CHECK(res > 0);
}

TEST(Average_Overflow_Negative)
{
    std::vector<float> v = {
        -std::numeric_limits<float>::max(),
        -std::numeric_limits<float>::max()
    };
    float res = Calculator::calculateAverage(v);
    CHECK(std::isinf(res));
    CHECK(res < 0);
}

TEST(Average_Mixed)
{
    std::vector<float> v = {-2.f, 1.f, 3.f};
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(0.666666f, result, 1e-5);
}

TEST(Average_SingleElement)
{
    std::vector<float> v = {42.5f};
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(42.5f, result, 1e-6);
}

TEST(Average_AllZeros)
{
    std::vector<float> v = {0.0f, 0.0f, 0.0f, 0.0f};
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(0.0f, result, 1e-6);
}

TEST(Average_LargeVector)
{
    std::vector<float> v(1000, 1.5f); // 1000 элементов со значением 1.5
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(1.5f, result, 1e-6);
}

TEST(Average_WithNaN)
{
    // Проверяем, что NaN не ломает вычисления
    std::vector<float> v = {1.0f, 2.0f, std::numeric_limits<float>::quiet_NaN(), 4.0f};
    float result = Calculator::calculateAverage(v);
    // Результат может быть NaN, это нормально
    CHECK(std::isnan(result));
}

TEST(Average_Precision)
{
    std::vector<float> v = {1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f};
    float result = Calculator::calculateAverage(v);
    CHECK_CLOSE(1.0f/3.0f, result, 1e-6);
}
