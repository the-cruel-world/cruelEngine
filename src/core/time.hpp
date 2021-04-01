#pragma once
#include "../common.h"

namespace cruelEngine::cruelCore
{
using namespace std;
class Timer
{
public:
    Timer();

    ~Timer();

    void AddMarker();

    chrono::time_point<chrono::high_resolution_clock> CurrentTime() const;

protected:
    vector<chrono::time_point<chrono::high_resolution_clock>> markers;
};
} // namespace cruelEngine::cruelCore