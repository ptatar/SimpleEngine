#include "ThreadManager.hpp"

#include "Logger.hpp"

#include <thread>

namespace engine
{
    void ThreadManager::Worker::Start()
    {
        LOGI("Thread: %d Starts");
        m_shutdown = false;
        m_thread = std::thread([this](){ Loop(); } );
    }

    void ThreadManager::Worker::Loop()
    {
        while (!m_shutdown)
        {
            if (m_job.Valid())
            {
                m_job->SetRunning(true);
                while (!m_shutdown && m_job->Work()) {}
                m_job->SetRunning(false);
                m_job = nullptr;
            }

            m_threadManager->NotifyIdle(this);
            if (!m_job.Valid() && !m_shutdown)
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_conditionVar.wait(lock);
            }
        }

        LOGI("Thread: %d Exit", m_index);
    }

    void ThreadManager::Worker::SetJob(ObjectRef<IJob>& job)
    {
        m_job = job;
    }

    void ThreadManager::Worker::Shutdown()
    {
        m_shutdown = true;
        m_conditionVar.notify_one();
    }

    Bool ThreadManager::Initialize(Uint32 workerCount, Uint32 maxQueueSize)
    {
        std::lock_guard<std::mutex> workersGuard(m_workersMx);
        m_workers.resize(workerCount);
        for (Uint32 i = 0; i < workerCount; i++)
        {
            m_workers[i] = std::make_unique<Worker>(this, i);
            m_workers[i]->Start();
        }
        m_shutdown = false;
        m_maxQueueSize = maxQueueSize;
        return true;
    }

    void ThreadManager::Execute(ObjectRef<IJob>& job)
    {
        {
            std::lock_guard<std::mutex> workersGuard(m_workersMx);
            if (m_idleWorkers.size() > 0)
            {
                auto* idleWorker = m_idleWorkers.front();
                m_idleWorkers.pop_front();
                idleWorker->SetJob(job);
                idleWorker->WakeUp();
                return;
            }
        }

        std::unique_lock<std::mutex> jobsLock(m_jobsMx);
        m_jobCv.wait(jobsLock, [this](){ return m_jobQueue.size()>=m_maxQueueSize; });
        m_jobQueue.push_back(job);
    }

    void ThreadManager::NotifyIdle(Worker* worker)
    {
        {
            std::lock_guard<std::mutex> jobsGuard(m_jobsMx);
            if(m_jobQueue.size() > 0)
            {
                auto& job = m_jobQueue.front();
                m_jobQueue.pop_front();
                worker->SetJob(job);
                return;
            }
        }

        std::lock_guard<std::mutex> workersGuard(m_workersMx);
        m_idleWorkers.push_back(worker);

    }

    void ThreadManager::Shutdown()
    {
        if(m_shutdown)
        {
            return;
        }
        else
        {
            m_shutdown = true;
        }

        LOGI("ThreadManager Shut down starts");
        for(auto& worker: m_workers)
        {
            worker->Shutdown();
        }

        for (auto& worker: m_workers)
        {
            worker->Join();
        }
        LOGI("ThreadManager Shut down ends");
    }

    void ThreadManager::Finish()
    {
        Bool working = false;

        while(m_idleWorkers.size() != m_workers.size())
        {
            Sleep();
        }
    }

    Bool ThreadManager::IsFinished() const
    {
        return m_idleWorkers.size() == m_workers.size();
    }

    void ThreadManager::Sleep()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

} // namespace engine
