#pragma once

#include "Eden/Core.h"

// Rework this: Each Layer should have a Player (or rather make a Player stack)
namespace Eden {

	// Empty implies song not loaded
	enum PlayerState
	{
		PlayerExit, PlayerEmpty, PlayerStop, PlayerPause, PlayerPlay
	};

	class EDEN_API Player
	{
	public:
		virtual ~Player() {}

		virtual void Load(const std::string& song_name) = 0;
		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;

		virtual PlayerState GetPlayerState() = 0;

		virtual void SetDevice(const std::string& device_name) = 0;
		virtual std::string GetCurrentDevice() = 0;
		virtual std::list<std::string> GetDeviceList() = 0;
		virtual void UpdateDeviceList() = 0;

		static Player* Create();
	};

}