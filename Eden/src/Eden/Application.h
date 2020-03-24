#pragma once

#include "edenpch.h"

#include "Eden/Core.h"
#include "Eden/Window.h"
#include "Eden/LayerStack.h"
#include "Eden/PlayerLayer.h"
#include "Eden/Events/Event.h"
#include "Eden/Events/ApplicationEvent.h"

namespace Eden {

	class EDEN_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		PlayerLayer* m_PlayerLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();

}
