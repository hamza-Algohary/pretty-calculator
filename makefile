CXXFLAGS := $(shell pkg-config --libs --cflags gtkmm-3.0) -O2 $(CXXFLAGS)

sources = $(wildcard src/*.cpp)
headers = $(wildcard src/*.h)

all: pretty-calculator

pretty-calculator: $(sources) $(headers)
	$(CXX) -o $@ $(sources) $(CXXFLAGS)

clean:
	rm -f pretty-calculator

install: pretty-calculator
	install -vDm755 pretty-calcualtor $(PREFIX)/bin/pretty-calculator

.PHONY: all clean install
    
