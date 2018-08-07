#pragma once

#include <VulkanUtility.hpp>

#include <list>

namespace engine
{

    class StateVk
    {
        public:
            StateVk();

            ~StateVk() {}

            void Default();

        private:
            VkPipelineVertexInputStateCreateInfo m_vertexInputInfo;

            VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyInfo;

            VkViewport m_viewport;

            VkRect2D m_scissor;

            VkPipelineViewportStateCreateInfo m_viewportInfo;

            VkPipelineRasterizationStateCreateInfo m_rasterizationInfo;

            VkPipelineMultisampleStateCreateInfo m_multisampleInfo;

            VkPipelineColorBlendAttachmentState m_colorBlendAttachmentInfo;

            VkPipelineColorBlendStateCreateInfo m_colorBlendStateCreateInfo;
    };


    class StateCacheVk
    {
        public:
            StateCacheVk();
            ~StateCacheVk() {}

        private:
            std::list<StateVk> m_states;
    };

}; // namespace engine
