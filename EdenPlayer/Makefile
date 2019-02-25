all:
	g++ -fPIC -c EMusicPlayer.cpp -o EMusicPlayer.o -std=c++11 -lmpg123 -lopenal -pthread
	swig3.0 -c++ -python eplayer.i 
	g++ -fPIC -I/usr/include/python3.5/ -c eplayer_wrap.cxx -o eplayer_wrap.o -std=c++11 -lmpg123 -lopenal -pthread
	g++ -shared EMusicPlayer.o eplayer_wrap.o -o _EMusicPlayer.so -lmpg123 -lopenal -std=c++11 -pthread