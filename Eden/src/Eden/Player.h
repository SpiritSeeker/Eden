#pragma once

#include "Eden/Core.h"

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

		// Volume is dealt as a percentage
		virtual int GetVolume() = 0;
		virtual void SetVolume(int volume) = 0;
		virtual void Mute() = 0;
		virtual void UnMute() = 0;
		virtual bool IsMuted() = 0;

		virtual PlayerState GetPlayerState() = 0;
		virtual void SetCurrentPosition(float position) = 0;
		virtual double GetCurrentPosition() = 0;
		virtual double GetSongDuration() = 0;
	};

}
