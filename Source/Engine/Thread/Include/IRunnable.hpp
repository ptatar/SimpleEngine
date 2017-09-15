#pragma once

namespace engine
{
	class Job
	{
	public:
		virtual ~Job() {}
		virtual Bool Work() = 0;
	};
} // namespace engine