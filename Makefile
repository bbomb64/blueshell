all:
	g++ -m64 -std=c++1y *.cpp ./nsmb/*.cpp -o ./build/ndsfilebrowser ./ui/*.cpp `pkg-config gtkmm-3.0 --cflags --libs` `libpng-config --cflags --ldflags` -D _DEBUG

clean:
	rm ./build/ndsfilebrowser
