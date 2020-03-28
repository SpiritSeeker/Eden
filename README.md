# Eden
Music Visualization GUI

This project is undergoing heavy rework and hopefully will turn out to be better than it used to be.

## Dependecies
#### libmpg123
1. Download the tar.gz file from [here](http://mpg123.org/snapshot)
2. Extract the zip file, and cd to the folder and run
```shell
./configure
make
sudo make install
```

#### OpenAL
```shell
sudo apt-get install libopenal1 libalut-dev
```

## Building
Build tools are currently made for Linux based systems only. Multiplatform support coming soon. :)

Clone the repo with submodules using
```shell
git clone --recurse-submodules https://github.com/SpiritSeeker/Eden
```

From inside the root Eden directory, run
```shell
vendor/bin/premake/premake5 gmake
make
./bin/Debug-linux-x86_64/Sandbox/Sandbox
```

### Main Goals:
  * <b>The OpenAL Player</b>: A lot of cool new features to be implemented, (think automatic 3D audio)
  * <b>The GUI</b>: Building a basic GUI which can then be upgraded using an in-house renderer (Coming soon) ;)
  * <b>The Visualizer</b>: Spectrum visualizer with FFT followed by some DSP for some FancyEffects&trade;.

### Other Potential Goals:
  * <b>Integration with other Players</b>: Integration with Players/ Streaming services such as Spotify, YTMusic.
  * <b>Intelligent Playlists</b>: AI based automation of playlists for a given mood.
  * <b>Intelligent Visualization</b>: Automation of above mentioned FancyEffects&trade; for every song.
