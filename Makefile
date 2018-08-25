all: build
	cd build && ninja

build: googletest
	mkdir -p build && cd build && cmake -G Ninja ..


watch:
	ag -l | entr bash -c "cd build && ninja && ./tests"

googletest:
	git clone https://github.com/google/googletest

clean:
	rm -rf build googletest
