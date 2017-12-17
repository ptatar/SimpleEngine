#pragma once

#include "Types.hpp"
#include "Assert.hpp"

namespace engine {

    void Yield();

    class TimeUnits
    {
        public:
            static TimeUnits MakeSeconds(Uint64 seconds);

            static TimeUnits MakeMiliseconds(Uint64 miliseconds);

            static TimeUnits MakeMicroseconds(Uint64 microseconds);

            static TimeUnits MakeNanoseconds(Uint64 nanoseconds);

            Uint64 GetUnits() const { return m_units; };

            Uint64 GetSeconds() const { return m_units / 1000 / 1000 / 1000; }

            Uint64 GetMiliseconds() const { return m_units / 1000 / 1000; }

            Uint64 GetMicroseconds() const { return m_units / 1000; }

            Uint64 GetNanoseconds() const { return m_units; }
        private:
            TimeUnits(Uint64 units): m_units(units) {}

        private:
            Uint64 m_units;
    };

    void Sleep(TimeUnits timeUnits);

}
