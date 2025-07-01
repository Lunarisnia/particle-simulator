run:
	cmake . -DCMAKE_BUILD_TYPE=debug -B ./build
	cmake --build ./build/
	./build/debug/ParticleSimulator
