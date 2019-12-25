#include "FramebufferVk.hpp"

#include "DeviceVk.hpp"

namespace engine
{
    void FramebufferDesc::AddRenderTarget(const ObjectRef<ImageVk>& renderTarget)
    {
        m_renderTargets.push_back(renderTarget);
        Uint32 height = renderTarget->GetImageDesc().height;
        Uint32 width = renderTarget->GetImageDesc().width;

        if (m_height == 0 || height < m_height)
        {
            m_height = height;
        }

        if (m_width == 0 || width < m_width)
        {
            m_width = width;
        }
    }

    void FramebufferDesc::AddDepthTarget(const ObjectRef<ImageVk>& depthTarget)
    {
        m_depthTarget = depthTarget;
        Uint32 height = m_depthTarget->GetImageDesc().height;
        Uint32 width = m_depthTarget->GetImageDesc().width;
        if (m_height == 0 || height < m_height)
        {
            m_height = height;
        }

        if (m_width == 0 || width < m_width)
        {
            m_width = width;
        }
    }

    FramebufferVk::FramebufferVk(std::vector<ObjectRef<ImageVk>>& renderTargets)
    {
        for(auto& rt: renderTargets)
        {
            m_imageRef.push_back(rt);
            m_renderTargetImages.push_back(rt->GetImage());
            m_renderTargetViews.push_back(rt->GetView());
        }
    }

    FramebufferVk::FramebufferVk(ObjectRef<ImageVk>& renderTarget)
    {
        m_imageRef.push_back(renderTarget);
        m_renderTargetImages.push_back(renderTarget->GetImage());
        m_renderTargetViews.push_back(renderTarget->GetView());
    }

    FramebufferVk::~FramebufferVk()
    {
        m_device->DestroyFramebuffer(m_framebuffer);
    }
}
