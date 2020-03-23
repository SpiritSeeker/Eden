#pragma once

#include "Eden/Core.h"

// Rework this: Each Layer should have a Player (or rather make a Player stack)
namespace Eden {

	// Empty implies song not loaded
	enum PlayerState
	{
		PlayerEmpty, PlayerStop, PlayerPause, PlayerPlay
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

		static Player* Create();
	};

}