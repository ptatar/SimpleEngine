#pragma once

#include "VulkanUtility.hpp"
#include "Handle.hpp"

namespace engine
{

    class ResourceVk
    {
        public:
            ResourceVk(SemaphoreH&& readySemaphore, SemaphoreH&& finishSemaphore)
                : m_readySemaphore(std::move(readySemaphore))
                , m_finishSemaphore(std::move(finishSemaphore)) {}
            SemaphoreH& GetReadySemaphore() { return m_readySemaphore; }
            SemaphoreH& GetFinishSemaphore() { return m_finishSemaphore; }
        private:
            SemaphoreH m_readySemaphore;
            SemaphoreH m_finishSemaphore;
    };

} // namespace engine
