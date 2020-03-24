#include "edenpch.h"
#include "Application.h"

namespace Eden {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Put assert here
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(EDEN_BIND_EVENT_FN(Application::OnEvent));
		m_Player = std::unique_ptr<Player>(Player::Create());
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EDEN_BIND_EVENT_FN(Application::OnWindowClose));

	}

	void Application::Run()
	{
		m_Player->Load("/home/sanjeet/Downloads/transfer/old/1216.mp3");
		m_Player->Play();
		while (m_Running)
		{
			m_Window->OnUpdate();
			msleep(100);
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}