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

		m_PlayerLayer = new PlayerLayer(EDEN_OPENAL_PLAYER);
		PushLayer(m_PlayerLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EDEN_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		m_PlayerLayer->Load("/home/sanjeet/Downloads/transfer/old/1216.mp3");
		m_PlayerLayer->Play();
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
