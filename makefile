build:
	g++ src/main.cpp src/graph.cpp src/text_editor.cpp `pkg-config --libs --cflags gtkmm-3.0` -o pretty-calculator
    
