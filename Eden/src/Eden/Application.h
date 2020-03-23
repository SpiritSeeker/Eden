#pragma once

#include "edenpch.h"

#include "Eden/Player.h"
#include "Eden/Core.h"

namespace Eden {

	class EDEN_API Application
	{
	public:
		Application();
		virtual ~Application();
	
		void Run();

		inline static Application& Get() { return *s_Instance; }
	private:
		std::unique_ptr<Player> m_Player;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}