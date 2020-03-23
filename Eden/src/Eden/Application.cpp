#include "edenpch.h"
#include "Application.h"

namespace Eden {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Put assert here
		s_Instance = this;

		m_Player = std::unique_ptr<Player>(Player::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		m_Player->Load("/home/sanjeet/Downloads/transfer/old/1216.mp3");
		m_Player->Play();
		while (m_Running)
			msleep(100);
	}

}