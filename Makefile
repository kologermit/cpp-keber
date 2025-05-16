install_deps:
	make install_venv
	make install_requirements
	make build_conan

install_venv:
	python3 -m venv ./.venv

install_requirements:
	./.venv/bin/pip install -r ./requirements.txt

build_conan:
	./.venv/bin/conan install . --build missing -s compiler.cppstd=gnu20 --output-folder=./.conan

build_bot:
	cmake ./bot -DCMAKE_TOOLCHAIN_FILE=$$PWD/.conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -B ./.build/bot/
	cmake --build ./.build/bot