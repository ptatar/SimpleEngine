#pragma once

namespace engine
{
	// TODO Spinlock/mutex mutex should be fine for now
	class ThreadManager
	{
		class Worker
		{
		public:
			Worker(ThreadManager* threadManager, Uint32 index):m_threadManager(threadManager), m_index(index), m_job(nullptr), m_shutdown(false)  {}
			~Worker() {};
			void Start();
		private:
			ThreadManager* m_threadManager;
			Uint32 m_index;
			Job* m_job;
			std::mutex m_mutex;
			std::condition_variable m_conditionVar;
			Bool m_shutdown;
		};

	public:
		ThreadManager() {}
		~ThreadManager() {}
		void Execute(Job* job);
		void NotifyIdle(Worker* worker);

	private:
		Uint32 m_maxQueueSize;
		std::list<Job*> m_jobQueue;
		std::list<Worker*> m_idleWorkers;
		std::mutex mutex;
		std::condition_variable m_conditionVar;
		std::vector<std::atomic<bool>> m_idleFlags;
		std::vector<Worker*> m_workers;
	};

} // namespace engine