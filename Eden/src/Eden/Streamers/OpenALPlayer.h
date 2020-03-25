#pragma once

#include "Eden/Player.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <mpg123.h>

// Add preferred output later.
namespace Eden {

	class OpenALPlayer : public Player
	{
	public:
		OpenALPlayer();
		virtual ~OpenALPlayer();

		void Load(const std::string& song_name) override;
		void Play() override;
		void Pause() override;
		void Stop() override;

		int GetVolume() override { return m_Volume; }
		void SetVolume(int volume) override;
		void Mute() override;
		void UnMute() override;
		bool IsMuted() override { return m_Mute; }

		PlayerState GetPlayerState() override { return m_State; }
		double GetCurrentPosition() override { return m_CurrentPosition; }
	private:
		virtual void Init();
		virtual void Shutdown();

		// Command string functions
		void SetDevice(const std::string& device_name);
		std::string GetCurrentDevice() { return m_CurrentDeviceName; }
		std::list<std::string> GetDeviceList() { return m_DeviceList; }
		void UpdateDeviceList();

		void DeviceReset(const char* device_name);
		void InitDevice(const char* device_name);
		void AsyncPlay();
		void DeviceCheck();
	private:
		mpg123_handle*            m_mh;
		long unsigned int         m_BufferSize;
		unsigned char*            m_Buffer;
		long unsigned int         m_Done;
		int                       m_Err;
		long                      m_Rate;
		int                       m_Channels;
		int                       m_Encoding;
		double                    m_CurrentPosition;

		ALCdevice*                m_Device;
		ALCcontext*               m_Context;
		ALCenum                   m_Error;
		std::list<ALuint>         m_BufferQueue;
		ALuint                    m_Source;
		ALuint                    m_BufferID[4];
		ALint                     m_SourceState;
		ALint                     m_AvailBuffers;
		ALuint                    m_BuffHolder[4];
		ALuint                    m_MyBuff;
		ALCint                    m_connected;
		int                       m_Volume = 100;
		bool                      m_Mute = false;

		std::string               m_CurrentDeviceName;
		std::list<std::string>    m_DeviceList;
		std::string               m_CurrentSong;
		double                    m_Duration;            // Duration in seconds

		std::thread               t_Play;
		std::thread               t_DevCheck;
		bool                      m_PlayLoopWaiting;
		bool                      m_UseDefaultOutput;

		PlayerState               m_State;
	};


}
