#pragma once

#include "edenpch.h"

#include "Eden/Player.h"
#include "Eden/Core.h"
#include "Eden/Events/Event.h"
#include "Eden/Events/ApplicationEvent.h"
#include "Eden/Window.h"

namespace Eden {

	class EDEN_API Application
	{
	public:
		Application();
		virtual ~Application();
	
		void Run();

		void OnEvent(Event& e);

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<Player> m_Player;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}