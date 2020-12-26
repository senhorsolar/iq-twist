.PHONY: all clean run wasm 

all: run

wasm:
	emcc -std=c++1z --bind -o bindings.js bindings.cc

run: wasm
	python3 -m http.server

clean:
	rm -f *.out bindings.js *.wasm example

example:
	g++ --std=c++17 example.cc -o example
	./example
