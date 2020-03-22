#pragma once

extern Eden::Application* Eden::CreateApplication();

int main(int argc, char const *argv[])
{
	Eden::Log::Init();
	EDEN_WARN("Initialized Eden!");
	EDEN_INFO("Hello!");

	Eden::Application* app = Eden::CreateApplication();
	app->Run();
	delete app;
	return 0;
}