#include <Eden.h>

class Sandbox : public Eden::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
	
};

Eden::Application* Eden::CreateApplication()
{
	return new Sandbox();
}