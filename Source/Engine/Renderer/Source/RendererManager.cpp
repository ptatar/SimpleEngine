#include "RendererManager.hpp"

#include "RendererVk.hpp"
#include "Utility.hpp"
namespace engine
{

    ObjectRef<RendererVk> RendererManager::GetRenderer(IWindowSurface* surface)
    {
        if(surface->GetWindowType() == WindowType::WindowX)
        {
            IWindowSurfaceX* surfaceX = reinterpret_cast<IWindowSurfaceX*>(surface);
            ObjectRef<RendererVk> renderer = ObjectRef<RendererVk>(new RendererVk());
            if(!renderer->Initialize())
            {
                return nullptr;
            }

            if(!renderer->CreateSurface(surfaceX))
            {
                return nullptr;
            }
            auto job = ObjectRefCast<IJob, RendererVk>(renderer);
            m_threadManager->Execute(job);
            auto listener = ObjectRefCast<IJob, RendererVk>(renderer);
            //return ObjectRefCast<IRenderer, RendererVk>(renderer);
            return renderer;
        }

        ASSERT(true);
        return nullptr;
    }
} // namespace engine
