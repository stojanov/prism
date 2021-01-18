#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <math.h>

#include "prism/Math/Smoothing.h"
#include "prism/System/ScopeTimer.h"

template<float(*f)(float)>
void RunFunc()
{
    static const int RANGE = 10;
    for (int i = 0; i < RANGE; i++)
    {
        f(i * 1.f / RANGE);
    }
}

void MeasureFunctions()
{
    using ScopedTimer = Prism::System::Time::Scope<Prism::System::Time::Nanoseconds>;
    {
        ScopedTimer m("CosSmooth");
        RunFunc<Prism::Math::CosSmooth>();
    }
    {
        ScopedTimer m("CubicSmooth");
        RunFunc<Prism::Math::CubicSmooth>();
    }
    {
        ScopedTimer m("CubicSmooth_2");
        RunFunc<Prism::Math::CubicSmooth_2>();
    }
    {
        ScopedTimer m("CubicSmooth_3");
        RunFunc<Prism::Math::CubicSmooth_3>();
    }
    {
        ScopedTimer m("ACosSmooth");
        RunFunc<Prism::Math::ACosSmooth>();
    }
    {
        ScopedTimer m("FastACosSmooth");
        RunFunc<Prism::Math::CubicSmooth_3>();
    }
}