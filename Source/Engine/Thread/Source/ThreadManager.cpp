#include "ThreadManager.hpp"

#include "Logger.hpp"

namespace engine
{
    void ThreadManager::Worker::Start()
    {
        while (!m_shutdown)
        {
            if (m_job)
            {
                while (!m_shutdown && m_job->Work()) {}
            }
            else
            {
                m_threadManager->NotifyIdle(this);
                if (!m_job)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_conditionVar.wait(lock);
                }
            }
        }

        LOGI("Thread: %d Exit", m_index);
    }

    void ThreadManager::Execute(IJob* job)
    {
        {
            std::lock_guard<std::mutex> workersGuard(m_workersMx);
            if (m_idleWorkers.size() > 0)
            {
                auto* idleWorker = m_idleWorkers.front();
                m_idleWorkers.pop_front();
                idleWorker->SetJob(job);
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
                auto* job = m_jobQueue.front();
                m_jobQueue.pop_front();
                worker->SetJob(job);
            }
        }

        std::lock_guard<std::mutex> workersGuard(m_workersMx);
        m_idleWorkers.push_back(worker);

    }

    void ThreadManager::Shutdown()
    {
        for(auto& worker: m_workers)
        {
        }
    }

} // namespace engine
