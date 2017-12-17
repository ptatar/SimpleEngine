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

        virtual void Finish() {}

        void Shutdown()
        {
            m_shutdown = true;
        }

        Bool IsRunning() const { return m_running; }
        Bool IsShutdown() const { return m_shutdown; }

    private:
        void SetRunning(Bool running) { m_running = running; }

    private:
        std::atomic<Bool> m_running;
        std::atomic<Bool> m_shutdown;

        friend ThreadManager;
    };

} // namespace engine
