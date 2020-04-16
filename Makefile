all:
	g++ -std=c++1y *.cpp ./nsmb/*.cpp ./ui/*.cpp -o ./build/ndsfilebrowser `pkg-config gtkmm-3.0 --cflags --libs` -D _DEBUG

clean:
	rm ./build/ndsfilebrowser
