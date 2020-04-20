all:
	g++ -m64 -std=c++1y *.cpp ./nsmb/*/*.cpp ./util/*.cpp ./filesystem/*.cpp -o ./build/ndsfilebrowser `pkg-config gtkmm-3.0 --cflags --libs` -D _DEBUG

clean:
	rm ./build/ndsfilebrowser
