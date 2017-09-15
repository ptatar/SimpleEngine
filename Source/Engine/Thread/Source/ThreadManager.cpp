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
					m_conditionVar.wait(std::unique_lock<std::mutex>(m_mutex));
				}
			}
		}

		LOGI("Thread: %d Exit", m_index);
	}


	void ThreadManager::Execute(Job* job)
	{
		std::unique_lock<std::mutex> queueLock(m_Mutex);
		if (m_idleWorkers.size() > 0)
		{
		}
	}

	void ThreadManager::NotifyIdle(Worker* worker)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_conditionVar.wait(lock, [this] { return m_jobQueue.size() < m_maxQueueSize; });
		if (m_jobQueue.empty())
		{
			m_idleFlags[worker->GetId()] = true;
		}
		else
		{
			std::unique_lock<std::mutex>(m_queueMutex);
			m_jobQueue.front()
		}

	}

} // namespace engine