#pragma once
#include <random>

namespace math
{
static constexpr float PI = 3.14159265358979323846f;
static constexpr float TOLERANCE = 1e-06f;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

std::random_device rd;            //Will be used to obtain a seed for the random number engine
std::mt19937 random_number(rd()); //Standard mersenne_twister_engine seeded with rd()

float rand()
{
    return distribution(random_number);
}

bool floatEquals(const float num1, const float num2, const float tolerance = TOLERANCE)
{
    return (std::fabs(num2 - num1) < tolerance);
}

const float FloatCosTable[] = {
    1,          //  0  Degrees
    0.999847f,  //  1  Degrees
    0.99939f,   //  2  Degrees
    0.998627f,  //  3  Degrees
    0.997559f,  //  4  Degrees
    0.996201f,  //  5  Degrees
    0.994522f,  //  6  Degrees
    0.992554f,  //  7  Degrees
    0.990265f,  //  8  Degrees
    0.987686f,  //  9  Degrees
    0.984802f,  //  10  Degrees
    0.981628f,  //  11  Degrees
    0.978149f,  //  12  Degrees
    0.974365f,  //  13  Degrees
    0.970291f,  //  14  Degrees
    0.965927f,  //  15  Degrees
    0.961258f,  //  16  Degrees
    0.956299f,  //  17  Degrees
    0.95105f,   //  18  Degrees
    0.945526f,  //  19  Degrees
    0.939697f,  //  20  Degrees
    0.933578f,  //  21  Degrees
    0.927185f,  //  22  Degrees
    0.920502f,  //  23  Degrees
    0.913544f,  //  24  Degrees
    0.906311f,  //  25  Degrees
    0.898788f,  //  26  Degrees
    0.891006f,  //  27  Degrees
    0.88295f,   //  28  Degrees
    0.874619f,  //  29  Degrees
    0.866028f,  //  30  Degrees
    0.857162f,  //  31  Degrees
    0.848053f,  //  32  Degrees
    0.838669f,  //  33  Degrees
    0.829041f,  //  34  Degrees
    0.819153f,  //  35  Degrees
    0.809021f,  //  36  Degrees
    0.79863f,   //  37  Degrees
    0.78801f,   //  38  Degrees
    0.777145f,  //  39  Degrees
    0.766037f,  //  40  Degrees
    0.754715f,  //  41  Degrees
    0.743149f,  //  42  Degrees
    0.731354f,  //  43  Degrees
    0.719345f,  //  44  Degrees
    0.707108f,  //  45  Degrees
    0.694656f,  //  46  Degrees
    0.681992f,  //  47  Degrees
    0.669128f,  //  48  Degrees
    0.656052f,  //  49  Degrees
    0.642792f,  //  50  Degrees
    0.629318f,  //  51  Degrees
    0.615662f,  //  52  Degrees
    0.601822f,  //  53  Degrees
    0.587784f,  //  54  Degrees
    0.573578f,  //  55  Degrees
    0.559189f,  //  56  Degrees
    0.544632f,  //  57  Degrees
    0.529922f,  //  58  Degrees
    0.515045f,  //  59  Degrees
    0.5f,       //  60  Degrees
    0.484802f,  //  61  Degrees
    0.469467f,  //  62  Degrees
    0.453995f,  //  63  Degrees
    0.43837f,   //  64  Degrees
    0.422623f,  //  65  Degrees
    0.406738f,  //  66  Degrees
    0.390732f,  //  67  Degrees
    0.374603f,  //  68  Degrees
    0.358368f,  //  69  Degrees
    0.342026f,  //  70  Degrees
    0.325562f,  //  71  Degrees
    0.309021f,  //  72  Degrees
    0.292374f,  //  73  Degrees
    0.275635f,  //  74  Degrees
    0.25882f,   //  75  Degrees
    0.241928f,  //  76  Degrees
    0.224945f,  //  77  Degrees
    0.207916f,  //  78  Degrees
    0.190811f,  //  79  Degrees
    0.173645f,  //  80  Degrees
    0.156433f,  //  81  Degrees
    0.139175f,  //  82  Degrees
    0.121872f,  //  83  Degrees
    0.104523f,  //  84  Degrees
    0.0871582f, //  85  Degrees
    0.0697632f, //  86  Degrees
    0.0523376f, //  87  Degrees
    0.0348969f, //  88  Degrees
    0.0174561f, //  89  Degrees
    0,          //  90  Degrees
};

constexpr float fastFloatCos(int Angle)
{
    assert(Angle < 360 * 3 && Angle > -360 * 3);

    while (Angle >= 360)
    {
        Angle -= 360;
    }

    while (Angle < 0)
    {
        Angle += 360;
    }

    if (Angle <= 90)
    {
        return (FloatCosTable[Angle]);
    }
    else if (Angle <= 180)
    {
        return (-FloatCosTable[180 - Angle]);
    }
    else if (Angle <= 270)
    {
        return (-FloatCosTable[Angle - 180]);
    }
    else
    {
        return (FloatCosTable[360 - Angle]);
    }
}

constexpr float fastFloatSin(int Angle)
{
    return (fastFloatCos(Angle - 90));
}

float fastSquareRoot(float x)
{
    unsigned int i = *(unsigned int*)&x;

    // adjust bias
    i += 127 << 23;
    // approximation of square root
    i >>= 1;

    return *(float*)&i;
}

template <class T>
T clamp(const T& value, const T& lowerLimit, const T& upperLimit)
{
    return std::min(upperLimit, std::max(lowerLimit, value));
}
} // namespace math
