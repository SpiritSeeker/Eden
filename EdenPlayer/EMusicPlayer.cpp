#include <mpg123.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <list>
#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

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

static void list_audio_devices(const ALCchar *devs, list<char*> *devices, int *number)
{
		const ALCchar *device = devs, *next = devs + 1;
		size_t len = 0;
		int num = 0;
		char* dev;

		while (device && *device != '\0' && next && *next != '\0') {
				len = strlen(device);
				dev = (char*) malloc((len+1)*sizeof(char));
				strcpy(dev,device);
				devices->push_back(dev);
				device += (len + 1);
				next += (len + 2);
				num += 1;
		}
		*number = num;
}

class EMusicPlayer
{
	private:
		mpg123_handle *mh;
		unsigned char *buffer;
		size_t buffer_size;
		size_t done;
		ALCenum error;
		ALCdevice *device;
		list<ALuint> bufferQueue;
		int channels, encoding;
		long rate;
		ALCcontext *context;
		ALuint source;
		ALuint bufferID[4];
		int err;
		ALint source_state;
		ALint availBuffers;
		ALuint buffHolder[4];
		ALuint myBuff;
		char *current_song = NULL;
		thread t_play, t_dev_check;
		list<char*> devs;
		vector <string> device_list;
		int prev_num_dev = 0;
		char *new_device = NULL;
		bool exit_status = false;
		ALCint connected;
		
		void play_task(){
			while(!stop_state){
				if (!pause_state){
					alGetSourcei(source, AL_BUFFERS_PROCESSED, &availBuffers);
					if (availBuffers > 0){
						alSourceUnqueueBuffers(source, availBuffers, buffHolder);
						for (int ii = 0; ii < availBuffers; ii++)
							bufferQueue.push_back(buffHolder[ii]);
					}

					if (!bufferQueue.empty()){
						if (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK){
							myBuff = bufferQueue.front(); 
							bufferQueue.pop_front();
							alBufferData(myBuff, to_al_format(channels, mpg123_encsize(encoding)), buffer, done, rate);
							if ((error = alGetError()) != AL_NO_ERROR){
								std::cout<<"GenBuff: "<<error;
								std::cout<<"Failed to generate buffer data.\n";
								exit(0);
							}

							alSourceQueueBuffers(source, 1, &myBuff);
							if ((error = alGetError()) != AL_NO_ERROR){
								std::cout<<"Failed to link source.\n"<<error<<"\n";
								exit(0);
							}

							alGetSourcei(source, AL_SOURCE_STATE, &source_state);
							if ((error = alGetError()) != AL_NO_ERROR){
								std::cout<<"Failed to get source state.\n";
								exit(0);
							}	
							if (source_state != AL_PLAYING){
								alSourcePlay(source);
								if ((error = alGetError()) != AL_NO_ERROR){
									std::cout<<"Failed to play source.\n";
									exit(0);
								}	
							}
						}
						else{
							stop();
							return;
						}
					}
				}
				usleep((buffer_size*1000)/rate);
			}	
		}

		void device_reset(const char * name){
			pause_state = true;
			usleep((buffer_size*1000)/rate);
			
			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
			alcCloseDevice(device);
			usleep(100);
			
			device = alcOpenDevice(name);
			if (!device){
				cout<<"Open Device: "<<alGetError()<<endl;
				exit(0);
			}

			// Flush the error stack
			alGetError();

			free(current_device);
			current_device = (char*)malloc((strlen(alcGetString(device,ALC_ALL_DEVICES_SPECIFIER))+1)*sizeof(char));
			strcpy(current_device,alcGetString(device,ALC_ALL_DEVICES_SPECIFIER));

			// Create Context
			context = alcCreateContext(device, NULL);
			if(!alcMakeContextCurrent(context)){
				std::cout<<"Failed to generate context.\n";
				exit(0);
			}
			alGetError();

			// Create a Source
			alGenSources((ALuint)1, &source);
			if ((error = alGetError()) != AL_NO_ERROR){
				std::cout<<"Failed to generate source.\n";
				exit(0);
			}

			// Create Buffers
			alGenBuffers((ALuint)4, bufferID);
			if ((error = alGetError()) != AL_NO_ERROR){
				std::cout<<error<<"\n";
				exit(0);
			}

			bufferQueue.clear();
			// Push created Buffers into queue
			for (int ii = 0; ii < 4; ii++)
				bufferQueue.push_back(bufferID[ii]);

			pause_state = false;
		}

		void dev_check(){
			while(!exit_status){
				prev_num_dev = dev_num;
				update_device_list();
				if (use_default){
					free(new_device);
					new_device = (char*)malloc((strlen(alcGetString(NULL,ALC_DEFAULT_ALL_DEVICES_SPECIFIER ))+1)*sizeof(char));
					strcpy(new_device,alcGetString(NULL,ALC_DEFAULT_ALL_DEVICES_SPECIFIER ));
					if (strcmp(new_device,current_device) != 0)
						device_reset(NULL);
				}
				else{
					bool present = false;
					for (int i = 0; i < dev_num; i++)
						if (device_list[i].compare(current_device) == 0){
							present = true;
							break;
						}
					if (!present)
						device_reset(NULL);
				}
				usleep(100*1000);
			}	
		}

		void update_device_list(){
			list_audio_devices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER), &devs, &dev_num);
			alGetError();
			device_list.clear();
			for (int i = 0; i < dev_num; i++){
				device_list.push_back(devs.front());
				devs.pop_front();
			}
		}
		
	public:
		bool pause_state, stop_state;
		int dev_num;
		char* current_device = NULL;
		bool use_default = true;
		EMusicPlayer(){

			// Open Default Audio Device
			device = alcOpenDevice(NULL);
			if (!device){
				cout<<"Open Device: "<<alGetError()<<endl;
				exit(0);
			}

			// Flush the error stack
			alGetError();

			free(current_device);
			current_device = (char*)malloc((strlen(alcGetString(device,ALC_ALL_DEVICES_SPECIFIER))+1)*sizeof(char));
			strcpy(current_device,alcGetString(device,ALC_ALL_DEVICES_SPECIFIER));
			update_device_list();

			// Create Context
			context = alcCreateContext(device, NULL);
			if(!alcMakeContextCurrent(context)){
				std::cout<<"Failed to generate context.\n";
				exit(0);
			}
			alGetError();

			// Create a Source
			alGenSources((ALuint)1, &source);
			if ((error = alGetError()) != AL_NO_ERROR){
				std::cout<<"Failed to generate source.\n";
				exit(0);
			}

			// Create Buffers
			alGenBuffers((ALuint)4, bufferID);
			if ((error = alGetError()) != AL_NO_ERROR){
				std::cout<<error<<"\n";
				exit(0);
			}

			// Push created Buffers into queue
			for (int ii = 0; ii < 4; ii++)
				bufferQueue.push_back(bufferID[ii]);
			
			// Initialize mpg123
			mpg123_init();
			mh = mpg123_new(NULL, &err);
			buffer_size = 4096;
			buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
		
			availBuffers = 0;
			pause_state = false;
			stop_state = true;

			t_dev_check = thread(&EMusicPlayer::dev_check, this);
			// dev_check();
		}
		~EMusicPlayer(){
			if (t_play.joinable())
				t_play.join();
			exit_status = true;
			if (t_dev_check.joinable())
				t_dev_check.join();
			alDeleteSources(1, &source);
			alDeleteBuffers(4, bufferID);
			device = alcGetContextsDevice(context);
			alcMakeContextCurrent(NULL);
			alcDestroyContext(context);
			alcCloseDevice(device);
			free(buffer);
			mpg123_close(mh);
			mpg123_delete(mh);
			mpg123_exit();
		}

		string get_device_name(int index){
			return device_list[index]; 
		}

		void set_device(string new_device_name){
			cout<<new_device_name<<endl;
			usleep(100*1000);
			use_default = false;
			device_reset((const char*)new_device_name.c_str());
		}
		
		void load(const char* path){
			stop();
			free(current_song);
			current_song = (char*)malloc((strlen(path)+1)*sizeof(char));
			strcpy(current_song, path);
			mpg123_close(mh);
			mpg123_open(mh, (const char*)current_song);
			mpg123_getformat(mh, &rate, &channels, &encoding);
		}

		void set_volume(float newVolume){
			alSourcef(source, AL_GAIN, newVolume);
		}

		void play(){
			if (t_play.joinable())
				t_play.join();
			stop_state = false;
			pause_state = false;
			t_play = thread(&EMusicPlayer::play_task, this);
		}

		void pause(){
			pause_state = true;
			alSourcePause(source);
			alGetError();
		}

		void unpause() {
			pause_state = false;
			alSourcePlay(source);
			alGetError();
		}

		void stop() {
			stop_state = true;
			alGetSourcei(source, AL_SOURCE_STATE, &source_state);
			alGetError();
			if (source_state == AL_PLAYING){
				alSourceStop(source);
				alGetError();
			}
			alGetSourcei(source, AL_BUFFERS_PROCESSED, &availBuffers);
			if (availBuffers > 0){
				alSourceUnqueueBuffers(source, availBuffers, buffHolder);
				for (int ii = 0; ii < availBuffers; ii++)
					bufferQueue.push_back(buffHolder[ii]);
			}
			mpg123_close(mh);
			mpg123_open(mh, (const char *)current_song);
		}
};

// int main(int argc, char const *argv[])
// {
// 	EMusicPlayer eden;
// 	float volume = 1;
// 	// eden.load("/home/sanjeet/Desktop/Eden/EdenPlayer/test.mp3");
// 	eden.load("/home/sanjeet/Downloads/transfer/old/1216.mp3");
// 	eden.play();
// 	for (int i = 0; i < 5; i++)
// 	{
// 		usleep(1000 * 1000);
// 		cout<<i<<endl;
// 	}
// 	eden.set_device((const char*)eden.get_device_name(1).c_str());		
// 		// eden.pause();
// 		// cout<<"Playing Stopped"<<endl;
// 	usleep(4000*1000);
// 	eden.set_device((const char*)eden.get_device_name(2).c_str());
// 		// eden.unpause();
// 		// cout<<"Resume"<<endl;
// 	usleep(4000*1000);
// 	eden.set_device((const char*)eden.get_device_name(0).c_str());		
// 		// eden.stop();
// 		// usleep(500*1000);
// 		// cout<<"Done Sleepin"<<endl;
// 		// eden.play();
// 	usleep(2000*1000);
// 	exit(0);
// }