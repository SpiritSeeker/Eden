#include "Application.h"

namespace Eden {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Put assert here
		s_Instance = this;
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running);
	}
	
}