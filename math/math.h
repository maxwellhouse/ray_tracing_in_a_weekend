#pragma once
#include <random>
namespace math 
{
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 random_number(rd()); //Standard mersenne_twister_engine seeded with rd()

// Lifted from stackoverflow
//https://stackoverflow.com/questions/21867617/best-platform-independent-pi-constant
    template <typename T> inline T calc_pi() {
        T sum = T(0), k8 = T(0), fac = T(1);
        for (;;) {
            const T next =
                sum + fac * (T(4) / (k8 + T(1)) - T(2) / (k8 + T(4)) - T(1) / (k8 + T(5)) - T(1) / (k8 + T(6)));
            if (sum == next) return sum;
            sum = next;
            fac /= T(16);
            k8 += T(8);
        }
    }
    static const auto PI = calc_pi<double>();
}
