#include "RendererManager.hpp"

#include "RendererVk.hpp"
#include "Utility.hpp"
namespace engine
{

    ObjectRef<IRenderer> RendererManager::GetRenderer(IWindowSurface* surface)
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
            auto job = static_cast<ObjectRef<IJob>>(renderer);
            m_threadManager->Execute(job);
            return ObjectRefCast<IRenderer, RendererVk>(renderer);
        }

        ASSERT(true);
        return nullptr;
    }
} // namespace engine
