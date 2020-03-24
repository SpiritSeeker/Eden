#pragma once

#include "Eden/Core.h"
#include "Eden/Events/Event.h"
#include "Eden/Events/ApplicationEvent.h"
#include "Eden/Events/KeyEvent.h"
#include "Eden/Events/MouseEvent.h"

namespace Eden {

	class EDEN_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}
