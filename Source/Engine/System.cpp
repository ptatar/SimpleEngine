#include "Systems.cpp"

namespace engine
{
    Bool System::Initialize()
    {
        if(!m_threadManager.Initialize())
        {
            return false;
        }

        if (m_windowManager.Initialize())
        {
            return false;
        }
        return true;
    }

    void System::MainLoop()
    {
        m_threadManager->Update();
        m_windowManager->Update();
        //m_renderManager->Update();
    }

    void System::Shutdown()
    {
        m_threadManager.Shutdown();
        //m_renderManager.Shutdown();
        m_windowManager.Shutdown();
    }


} // namespace engine