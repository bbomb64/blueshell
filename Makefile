all:
	g++ -std=c++1y *.cpp -o ./build/ndsfilebrowser `pkg-config gtkmm-3.0 --cflags --libs`

clean:
	rm ./build/ndsfilebrowser