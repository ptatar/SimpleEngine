#include "Utility.hpp"

#include <chrono>
#include <thread>

namespace engine {

    void Yield()
    {
        std::this_thread::yield();
    }

    TimeUnits TimeUnits::MakeSeconds(Uint64 seconds)
    {
        return TimeUnits(seconds* 1000 * 1000 * 1000);
    }

    TimeUnits TimeUnits::MakeMiliseconds(Uint64 miliseconds)
    {
        return TimeUnits(miliseconds * 1000 * 1000);
    }

    TimeUnits TimeUnits::MakeMicroseconds(Uint64 microseconds)
    {
        return TimeUnits(microseconds * 1000);
    }

     TimeUnits TimeUnits::MakeNanoseconds(Uint64 nanoseconds)
    {
        return TimeUnits(nanoseconds);
    }

    void Sleep(TimeUnits timeUnits)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(timeUnits.GetUnits()));
    }
}
