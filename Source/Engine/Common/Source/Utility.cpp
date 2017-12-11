#include "Utility.hpp"

#include <chrono>
#include <thread>

namespace engine {

    void Yield()
    {
        std::this_thread::yield();
    }

    TimeUnits TimeUnits::MakeSeconds(Uint32 seconds)
    {
        return TimeUnits(seconds* 1000 * 1000);
    }

    TimeUnits TimeUnits::MakeMiliseconds(Uint32 miliseconds)
    {
        return TimeUnits(miliseconds * 1000);
    }

    TimeUnits TimeUnits::MakeMicroseconds(Uint32 microseconds)
    {
        return TimeUnits(microseconds);
    }

    void Sleep(TimeUnits timeUnits)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(timeUnits.GetUnits()));
    }
}
