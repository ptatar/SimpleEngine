#pragma once

#include "Types.hpp"
#include "IJob.hpp"
#include <list>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <thread>

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
                , m_shutdown(false) {}
            ~Worker() {};
            void Start();
            void Loop();
            void SetJob(ObjectRef<IJob>& job);
            void Shutdown();
            void Join() { if(m_thread.joinable()) m_thread.join(); }
            void WakeUp() { m_conditionVar.notify_one(); }

        private:
            ThreadManager* m_threadManager;
            Uint32 m_index;
            ObjectRef<IJob> m_job;
            std::mutex m_mutex;
            std::condition_variable m_conditionVar;
            volatile Bool m_shutdown;
            std::thread m_thread;
        };

    public:
        ThreadManager()
            : m_maxQueueSize(0)
            , m_shutdown(false) {}
        ~ThreadManager() { Shutdown(); }
        Bool Initialize(Uint32 workerCount, Uint32 maxQueueSize);
        void Execute(ObjectRef<IJob>& job);
        void NotifyIdle(Worker* worker);
        void Shutdown();
        void Finish();
        void Sleep();

    private:
        Uint32 m_maxQueueSize;
        std::list<ObjectRef<IJob>> m_jobQueue;
        std::list<Worker*> m_idleWorkers;
        std::mutex m_workersMx;
        std::mutex m_jobsMx;
        std::condition_variable m_jobCv;
        std::vector<std::unique_ptr<Worker>> m_workers;
        Bool m_shutdown;
    };

/*
    class ThreadManagerUnit: public IUnit, public ThreadManager
    {
        public:
            ThreadManagerUnit(ISystem* system)
                : IUnit(system) {}
    }
*/
} // namespace engine
