#pragma once

#include "Types.hpp"

#include <atomic>

namespace engine
{
    class ThreadManager;

    class IJob
    {
    public:
        virtual ~IJob() {}

        virtual Bool Work() = 0;

        Bool IsRunning() { return m_running; }

    private:
        void SetRunning(Bool running) { m_running = running; }
    private:
        std::atomic<Bool> m_running;

        friend ThreadManager;
    };

} // namespace engine
