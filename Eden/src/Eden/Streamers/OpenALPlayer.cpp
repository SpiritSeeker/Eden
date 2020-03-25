#include "edenpch.h"
#include "OpenALPlayer.h"

static inline ALenum to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 2:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 1:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}

static std::list<std::string> list_audio_devices(const ALCchar *devs)
{
	const ALCchar *device = devs, *next = devs + 1;
	unsigned int len = 0;
	std::list<std::string> device_list;

	while (device && *device != '\0' && next && *next != '\0') {
		len = strlen(device);
		std::string dev(device, len);
		device_list.push_back(dev);
		device += (len + 1);
		next += (len + 2);
	}

	return device_list;
}

namespace Eden {

	OpenALPlayer::OpenALPlayer()
	{
		Init();
	}

	OpenALPlayer::~OpenALPlayer()
	{
		Shutdown();
	}

	void OpenALPlayer::Init()
	{
		EDEN_INFO("Initializing OpenAL Player!");

		InitDevice(m_PreferredDevice);

		// Initialize mpg123
		mpg123_init();
		m_mh = mpg123_new(NULL, &m_Err);
		m_BufferSize = 4096;
		m_Buffer = new unsigned char[m_BufferSize];

		m_AvailBuffers = 0;
		m_State = PlayerEmpty;

		m_UseDefaultOutput = true;
		t_DevCheck = std::thread(&OpenALPlayer::DeviceCheck, this);
		EDEN_INFO("OpenAL Player Initialized!");
	}

	char* OpenALPlayer::GetDefaultDevice()
	{
		if (m_PreferredDevice.empty())
			return NULL;
		else
		{
			if (std::find(m_DeviceList.begin(), m_DeviceList.end(), m_PreferredDevice) == m_DeviceList.end())
				return NULL;
			else
				return (char*)m_PreferredDevice.c_str();
		}
	}

	void OpenALPlayer::InitDevice(const std::string& device_name)
	{
		UpdateDeviceList();

		char* device;

		if (device_name.empty())
			device = GetDefaultDevice();
		else
		{
			if (std::find(m_DeviceList.begin(), m_DeviceList.end(), device_name) == m_DeviceList.end())
			{
				EDEN_WARN("Couldn't find {0}. Falling back to default output device!", device_name);
				device = GetDefaultDevice();
			}
			else
				device = (char*)device_name.c_str();
		}

		// Initialize and set device
		EDEN_INFO("Initializing New Device!");
		m_Device = alcOpenDevice(device);
		EDEN_ASSERT(m_Device, "Could not initialize device: {0}", device_name);

		// Flush error stack
		alGetError();

		// Get current device name
		m_CurrentDeviceName.assign(alcGetString(m_Device, ALC_ALL_DEVICES_SPECIFIER));

		// Create context
		m_Context = alcCreateContext(m_Device, NULL);
		EDEN_ASSERT(alcMakeContextCurrent(m_Context), "Failed to generate context!");
		alGetError();

		// Create a source
		alGenSources((ALuint)1, &m_Source);
		EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to generate source!");

		// Set source gain
		alSourcef(m_Source, AL_GAIN, (float)m_Volume / 100.0f);

		// Create buffers
		alGenBuffers((ALuint)4, m_BufferID);
		EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to generate buffers! {0}", m_Error);

		m_BufferQueue.clear();
		// Push buffers into queue
		for (int ii = 0; ii < 4; ii++)
			m_BufferQueue.push_back(m_BufferID[ii]);
		EDEN_INFO("New Device {0} Initialized!", m_CurrentDeviceName);
	}

	void OpenALPlayer::Shutdown()
	{
		Stop();
		m_State = PlayerExit;
		// Cleanup
		if (t_Play.joinable())
			t_Play.join();
		if (t_DevCheck.joinable())
			t_DevCheck.join();
		alDeleteSources(1, &m_Source);
		alDeleteBuffers(4, m_BufferID);
		m_Device = alcGetContextsDevice(m_Context);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_Context);
		alcCloseDevice(m_Device);

		delete[] m_Buffer;
		mpg123_close(m_mh);
		mpg123_delete(m_mh);
		mpg123_exit();
		EDEN_INFO("Shutting down OpenAL Player!");
	}

	void OpenALPlayer::UpdateDeviceList()
	{
		m_DeviceList = list_audio_devices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
		alGetError();
	}

	void OpenALPlayer::DeviceReset(const std::string& device_name)
	{
		bool play = false;
		if (m_State == PlayerPlay)
		{
			play = true;
			Pause();
			while (!m_PlayLoopWaiting);
		}

		alcMakeContextCurrent(NULL);
		alcDestroyContext(m_Context);
		alcCloseDevice(m_Device);
		msleep(0.1);

		InitDevice(device_name);

		if (play)
			Play();
	}

	void OpenALPlayer::SetDevice(const std::string& device_name)
	{
		EDEN_INFO("Setting output device: {0}", device_name);
		msleep(100);

		m_UseDefaultOutput = false;
		DeviceReset(device_name);
		EDEN_INFO("Output device set!");
	}

	void OpenALPlayer::Load(const std::string& song_name)
	{
		EDEN_INFO("Loading song: {0}", song_name);
		Stop();
		m_CurrentSong = song_name;

		mpg123_close(m_mh);
		mpg123_open(m_mh, (const char*)song_name.c_str());
		mpg123_getformat(m_mh, &m_Rate, &m_Channels, &m_Encoding);
		m_Duration = mpg123_length(m_mh) * mpg123_tpf(m_mh) / ((double)mpg123_spf(m_mh));
		EDEN_INFO("Song loaded!");
	}

	void OpenALPlayer::Play()
	{
		EDEN_TRACE("Playing song!");
		if (m_State == PlayerStop)
		{
			m_State = PlayerPlay;
			if (t_Play.joinable())
				t_Play.join();
			t_Play = std::thread(&OpenALPlayer::AsyncPlay, this);
		}

		else if (m_State == PlayerPause)
		{
			m_State = PlayerPlay;
			alSourcePlay(m_Source);
			alGetError();
		}
	}

	void OpenALPlayer::Pause()
	{
		EDEN_TRACE("Pausing!");
		m_State = PlayerPause;
		while (!m_PlayLoopWaiting);
	}

	void OpenALPlayer::Stop()
	{
		EDEN_TRACE("Stopping!");
		m_State = PlayerStop;
		m_CurrentPosition = 0;
		while (!m_PlayLoopWaiting);

		alGetSourcei(m_Source, AL_SOURCE_STATE, &m_SourceState);
		alGetError();
		if (m_SourceState == AL_PLAYING)
		{
			alSourceStop(m_Source);
			alGetError();
		}

		alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &m_AvailBuffers);
		if (m_AvailBuffers > 0)
		{
			alSourceUnqueueBuffers(m_Source, m_AvailBuffers, m_BuffHolder);
			for (int ii = 0; ii < m_AvailBuffers; ii++)
				m_BufferQueue.push_back(m_BuffHolder[ii]);
		}

		mpg123_close(m_mh);
		if (m_CurrentSong.empty())
			mpg123_open(m_mh, NULL);
		else
			mpg123_open(m_mh, (const char*)m_CurrentSong.c_str());
	}

	void OpenALPlayer::SetVolume(int volume)
	{
		if (m_Volume != volume)
		{
			EDEN_TRACE("Setting volume: {0}%", volume);
			if (!m_Mute)
				alSourcef(m_Source, AL_GAIN, (float)volume / 100.0f);
			m_Volume = volume;
		}
	}

	void OpenALPlayer::Mute()
	{
		m_Mute = true;
		EDEN_TRACE("Muting player!");
		alSourcef(m_Source, AL_GAIN, 0);
	}

	void OpenALPlayer::UnMute()
	{
		m_Mute = false;
		EDEN_TRACE("UnMuting player!");
		alSourcef(m_Source, AL_GAIN, (float)m_Volume / 100.0f);
	}

	void OpenALPlayer::AsyncPlay()
	{
		while (m_State > PlayerStop)
		{
			if (m_State == PlayerPlay)
			{
				m_PlayLoopWaiting = false;
				alGetSourcei(m_Source, AL_BUFFERS_PROCESSED, &m_AvailBuffers);
				if (m_AvailBuffers > 0)
				{
					alSourceUnqueueBuffers(m_Source, m_AvailBuffers, m_BuffHolder);
					for (int ii = 0; ii < m_AvailBuffers; ii++)
						m_BufferQueue.push_back(m_BuffHolder[ii]);
				}

				if (!m_BufferQueue.empty())
				{
					if (mpg123_read(m_mh, m_Buffer, m_BufferSize, &m_Done) == MPG123_OK)
					{
						m_CurrentPosition = mpg123_tellframe(m_mh) * mpg123_tpf(m_mh);

						m_MyBuff = m_BufferQueue.front();
						m_BufferQueue.pop_front();
						alBufferData(m_MyBuff, to_al_format(m_Channels, mpg123_encsize(m_Encoding)), m_Buffer, m_Done, m_Rate);
						EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to generate buffer data! GenBuff: {0}", m_Error);

						alSourceQueueBuffers(m_Source, 1, &m_MyBuff);
						EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to link source: {0}", m_Error);

						alGetSourcei(m_Source, AL_SOURCE_STATE, &m_SourceState);
						EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to get source state!");

						if (m_SourceState != AL_PLAYING)
						{
							alSourcePlay(m_Source);
							EDEN_ASSERT((m_Error = alGetError()) == AL_NO_ERROR, "Failed to play source!");
						}
					}

					else
					{
						Stop();
						return;
					}
				}
			}

			m_PlayLoopWaiting = true;
			msleep((float)m_BufferSize / m_Rate);
		}
	}

	void OpenALPlayer::DeviceCheck()
	{
		while (m_State > PlayerExit)
		{
			std::list<std::string> temp = list_audio_devices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
			alGetError();

			if (m_UseDefaultOutput)
			{
				std::string currentDevice(alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER));
				alGetError();

				if (m_PreferredDevice.empty())
				{
					if (m_CurrentDeviceName.compare(currentDevice) != 0)
						DeviceReset(m_PreferredDevice);
				}
				else
					if (m_PreferredDevice.compare(m_CurrentDeviceName) != 0)
						if (std::find(temp.begin(), temp.end(), m_PreferredDevice) != temp.end())
						{
							EDEN_TRACE("{0}: {1}", m_PreferredDevice, currentDevice);
							EDEN_INFO("Found {0}. Switching!", m_PreferredDevice);
							DeviceReset(m_PreferredDevice);
						}
			}

			else
			{
				if (std::find(temp.begin(), temp.end(), m_CurrentDeviceName) == temp.end())
				{
					m_UseDefaultOutput = true;
					DeviceReset(m_PreferredDevice);
				}
			}

			m_DeviceList = temp;
			msleep(100);
		}
	}

}
