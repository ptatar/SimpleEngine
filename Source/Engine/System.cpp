#include "System.hpp"
/*
namespace engine
{
    System::System()
        : m_renderManager(this)
        , m_windowManager(this, &m_renderManager)
        , m_threadManager(this)
    {}

    System::~System() {}

    Bool System::Initialize()
    {
        if(!m_threadManager.Initialize(16, 16 * 4))
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
        //m_threadManager.Update();
        m_windowManager.Update();
        //m_renderManager->Update();
    }

    void System::Shutdown()
    {
        m_threadManager.Shutdown();
        //m_renderManager.Shutdown();
        m_windowManager.Shutdown();
    }


} // namespace engine
*/
