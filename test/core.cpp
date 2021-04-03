#include "../src/core/time.hpp"

using namespace cruelEngine::cruelCore;

int main(int argc, char const *argv[])
{
    Timer timer;

    chrono::system_clock::to_time_t(timer.CurrentDate());

    return 0;
}