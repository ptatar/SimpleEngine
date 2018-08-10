#pragma once

#include "Types.hpp"
#include "Array.hpp"
#include "VulkanUtility.hpp"

#include <list>
#include <vector>
namespace engine
{

    class SwapchainVk;

    class StateVk
    {
        public:
            StateVk();

            ~StateVk() {}

            void Default();

            void SetAttachments(ObjectRef<SwapchainVk>& swapchain);

        private:
            VkPipelineVertexInputStateCreateInfo m_vertexInputInfo;

            VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyInfo;

            VkViewport m_viewport;

            VkRect2D m_scissor;

            VkPipelineViewportStateCreateInfo m_viewportInfo;

            VkPipelineRasterizationStateCreateInfo m_rasterizationInfo;

            VkPipelineMultisampleStateCreateInfo m_multisampleInfo;

            std::vector<VkPipelineColorBlendAttachmentState> m_colorBlendAttachments;

            Array<Float> m_blendConstants;

            VkPipelineColorBlendStateCreateInfo m_colorBlendStateInfo;
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
