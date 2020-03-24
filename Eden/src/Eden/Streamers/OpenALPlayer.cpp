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

		InitDevice(NULL);

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

	void OpenALPlayer::InitDevice(const char* device_name)
	{
		// Initialize and set device
		EDEN_INFO("Initializing New Device!");
		m_Device = alcOpenDevice(device_name);
		if (!m_Device)
		{
			EDEN_ERROR("Open Device: {0}", alGetError());
			exit(0);
		}

		// Flush error stack
		alGetError();

		// Get current device name and update device list
		m_CurrentDeviceName.assign(alcGetString(m_Device, ALC_ALL_DEVICES_SPECIFIER));
		UpdateDeviceList();

		// Create context
		m_Context = alcCreateContext(m_Device, NULL);
		if (!alcMakeContextCurrent(m_Context))
		{
			EDEN_ERROR("Failed to generate context!");
			exit(0);
		}
		alGetError();

		// Create a source
		alGenSources((ALuint)1, &m_Source);
		if ((m_Error = alGetError()) != AL_NO_ERROR)
		{
			EDEN_ERROR("Failed to generate source!");
			exit(0);
		}

		// Create buffers
		alGenBuffers((ALuint)4, m_BufferID);
		if ((m_Error = alGetError()) != AL_NO_ERROR)
		{
			EDEN_ERROR("Failed to generate buffers! {0}", m_Error);
			exit(0);
		}

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

	void OpenALPlayer::DeviceReset(const char* device_name)
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
		DeviceReset((const char*)device_name.c_str());
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
						if ((m_Error = alGetError()) != AL_NO_ERROR)
						{
							EDEN_ERROR("GenBuff: {0}", m_Error);
							EDEN_ERROR("Failed to generate buffer data!");
							exit(0);
						}

						alSourceQueueBuffers(m_Source, 1, &m_MyBuff);
						if ((m_Error = alGetError()) != AL_NO_ERROR)
						{
							EDEN_ERROR("Failed to link source: {0}", m_Error);
							exit(0);
						}

						alGetSourcei(m_Source, AL_SOURCE_STATE, &m_SourceState);
						if ((m_Error = alGetError()) != AL_NO_ERROR)
						{
							EDEN_ERROR("Failed to get source state!");
							exit(0);
						}

						if (m_SourceState != AL_PLAYING)
						{
							alSourcePlay(m_Source);
							if ((m_Error = alGetError()) != AL_NO_ERROR)
							{
								EDEN_ERROR("Failed to play source!");
								exit(0);
							}
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
				if (m_CurrentDeviceName.compare(currentDevice) != 0)
					DeviceReset(NULL);
			}

			else
			{
				if (std::find(temp.begin(), temp.end(), m_CurrentDeviceName) == temp.end())
				{
					m_UseDefaultOutput = true;
					DeviceReset(NULL);
				}
			}

			m_DeviceList = temp;
			msleep(100);
		}
	}

}
