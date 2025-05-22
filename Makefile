install_deps:
	make install_venv
	make install_requirements
	make build_conan

install_venv:
	python3 -m venv venv

install_requirements:
	venv/bin/pip install -r ./requirements.txt

build_conan:
	venv/bin/conan profile detect --force
	venv/bin/conan install . --build missing

build_bot:
	cmake . \
		-DCMAKE_BUILD_TYPE=Release \
		-B ./build
	cmake \
		--build ./build \
		--target KEBER_BOT
	chmod 777 ./build/KEBER_BOT

run_bot:
	./build/KEBER_BOT