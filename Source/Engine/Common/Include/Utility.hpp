#pragma once

#include "Types.hpp"
#include "Assert.hpp"

namespace engine {

    void Yield();

    class TimeUnits
    {
        public:
            static TimeUnits MakeSeconds(Uint32 seconds);

            static TimeUnits MakeMiliseconds(Uint32 miliseconds);

            static TimeUnits MakeMicroseconds(Uint32 microseconds);

            Uint64 GetUnits() const { return m_units; };

        private:
            TimeUnits(Uint64 units): m_units(units) {}

        private:
            Uint64 m_units;
    };

    void Sleep(TimeUnits timeUnits);

}
