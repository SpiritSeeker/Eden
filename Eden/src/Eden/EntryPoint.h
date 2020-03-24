#pragma once

extern Eden::Application* Eden::CreateApplication();

int main(int argc, char const *argv[])
{
	Eden::Log::Init();
	EDEN_WARN("Initialized Eden!");

	Eden::Application* app = Eden::CreateApplication();
	app->Run();
	delete app;
	EDEN_WARN("Exiting Eden!");
	return 0;
}
