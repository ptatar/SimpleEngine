#include "StateCacheVk.hpp"

namespace engine
{

    StateVk::StateVk()
    {
        Default();
    }

    void StateVk::Default()
    {
        m_vertexInputInfo =
        {
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            nullptr, // const void*                                 pNext;
            0,       // VkPipelineVertexInputStateCreateFlags       flags;
            0,       // uint32_t                                    vertexBindingDescriptionCount;
            nullptr, // const VkVertexInputBindingDescription*      pVertexBindingDescriptions;
            0,        // uint32_t                                    vertexAttributeDescriptionCount;
            nullptr // const VkVertexInputAttributeDescription*    pVertexAttributeDescriptions;
        };

        m_inputAssemblyInfo =
        {
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            nullptr,                             // const void*                            pNext;
            0,                                   // VkPipelineInputAssemblyStateCreateFlags flags;
            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, // VkPrimitiveTopology                    topology;
            VK_FALSE,                            // VkBool32                               primitiveRestartEnable;
        };

        m_viewport =
        {
            0.0f, // float    x;
            0.0f, // float    y;
            1.0f, // float    width;
            1.0f, // float    height;
            0.0f, // float    minDepth;
            1.0f, // float    maxDepth;
        };

        m_scissor =
        {
            {   //VkOffset2D
                0, // int32_t    x;
                0, // int32_t    y;
            },
            {   // VkExtent2D
                1,// uint32_t    width;
                1,// uint32_t    height;
            }
        };

        m_viewportInfo =
        {
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, // VkStructureType              sType;
            nullptr,      // const void*                        pNext;
            0,            // VkPipelineViewportStateCreateFlags flags;
            1,            // uint32_t                           viewportCount;
            &m_viewport,  // const VkViewport*                  pViewports;
            1,            // uint32_t                           scissorCount;
            &m_scissor,   // const VkRect2D*                    pScissors;
        };


        m_blendConstants = {0.0f, 0.0f, 0.0f, 0.0f};

        m_colorBlendAttachments =
        {
            {
                VK_FALSE,                  // VkBool32                 blendEnable;
                VK_BLEND_FACTOR_ONE,       // VkBlendFactor            srcColorBlendFactor;
                VK_BLEND_FACTOR_ZERO,      // VkBlendFactor            dstColorBlendFactor;
                VK_BLEND_OP_ADD,           // VkBlendOp                colorBlendOp;
                VK_BLEND_FACTOR_ONE,       // VkBlendFactor            srcAlphaBlendFactor;
                VK_BLEND_FACTOR_ZERO,      // VkBlendFactor            dstAlphaBlendFactor;
                VK_BLEND_OP_ADD,           // VkBlendOp                alphaBlendOp;
                VK_COLOR_COMPONENT_R_BIT |
                VK_COLOR_COMPONENT_G_BIT |
                VK_COLOR_COMPONENT_B_BIT |
                VK_COLOR_COMPONENT_A_BIT   // VkColorComponentFlags    colorWriteMask;
            }
        };

        m_colorBlendStateInfo =
        {
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            nullptr,                  // const void*                                   pNext;
            0,                        // VkPipelineColorBlendStateCreateFlags          flags;
            VK_FALSE,                 // VkBool32                                      logicOpEnable;
            VK_LOGIC_OP_COPY,         // VkLogicOp                                     logicOp;
            0,                        // uint32_t                                      attachmentCount;
            nullptr,                  // const VkPipelineColorBlendAttachmentState*    pAttachments;
            {0.0f, 0.0f, 0.0f, 0.0f}, // float                                         blendConstants[4];
        };
    }

    void SetAttachments(ObjectRef<SwapchainVk>& swapchain)
    {

    }

    StateCacheVk::StateCacheVk()
    {
    };


} // namespace engine
