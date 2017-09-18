#pragma once

#include "Types.hpp"
#include "IJob.hpp"
#include <list>
#include <mutex>
#include <condition_variable>
#include <vector>

namespace engine
{
    // TODO Spinlock/mutex mutex should be fine for now
    class ThreadManager
    {
        class Worker
        {
        public:
            Worker(ThreadManager* threadManager, Uint32 index)
                : m_threadManager(threadManager)
                , m_index(index)
                , m_job(nullptr)
                , m_shutdown(false)  {}
            ~Worker() {};
            void Start();
            void SetJob(IJob* job) { m_job = job; }

        private:
            ThreadManager* m_threadManager;
            Uint32 m_index;
            IJob* m_job;
            std::mutex m_mutex;
            std::condition_variable m_conditionVar;
            Bool m_shutdown;
        };

    public:
        ThreadManager() {}
        ~ThreadManager() { Shutdown(); }
        void Execute(IJob* job);
        void NotifyIdle(Worker* worker);
        void Shutdown();
    private:
        Uint32 m_maxQueueSize;
        std::list<IJob*> m_jobQueue;
        std::list<Worker*> m_idleWorkers;
        std::mutex m_workersMx;
        std::mutex m_jobsMx;
        std::condition_variable m_jobCv;
        std::vector<Worker*> m_workers;
    };

} // namespace engine
