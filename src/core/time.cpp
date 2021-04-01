#include "time.hpp"

namespace cruelEngine::cruelCore
{
Timer::Timer()
{}

Timer::~Timer()
{
    markers.clear();
}

void Timer::AddMarker()
{
    markers.push_back(chrono::high_resolution_clock::now());
}

chrono::time_point<chrono::high_resolution_clock> Timer::CurrentTime() const
{
    return chrono::high_resolution_clock::now();
}

} // namespace cruelEngine::cruelCore