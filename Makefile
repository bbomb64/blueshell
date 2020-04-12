all:
	g++ *.cpp -o ./build/ndsfilebrowser `pkg-config gtkmm-3.0 --cflags --libs`

clean:
	rm ./build/ndsfilebrowser