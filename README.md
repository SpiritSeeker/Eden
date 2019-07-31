# Eden
Music Player/Visualization

## Dependecies
#### libmpg123
1. Download the tar.gz file from [here](http://mpg123.org/snapshot)
2. Extract the zip file, and cd to the folder.
3. Then open a terminal
`./configure`
`make`
`make install`
Note: The last command might give an error of Permission Denied. Modify it to `sudo make install` for installing the libmpg123 files.
#### openal

`sudo apt-get install libopenal1`
`sudo apt-get install libalut-dev`
#### fftw3
1. Download tar.gz file from [here](http://www.fftw.org/fftw-3.3.8.tar.gz)
2. Extract and cd to the folder. 
3. In a terminal, type
`./configure`
`make`
`sudo make install`
#### swig3.0
`sudo apt-get install swig3.0`
