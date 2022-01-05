.PHONY: all clean run wasm 

all: run

wasm:
	emcc -std=c++1z -O2 --bind -o bindings.js iq_twist.cpp bindings.cc

run: wasm
	python3 -m http.server

clean:
	rm -f *.out bindings.js *.wasm example

example:
	g++ --std=c++2a -O2 iq_twist.cpp example.cc -o example
	./example
