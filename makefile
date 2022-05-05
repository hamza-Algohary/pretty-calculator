build:
	g++ main.cpp graph.cpp text_editor.cpp `pkg-config --libs --cflags gtkmm-3.0` -o pretty-calculator
    
