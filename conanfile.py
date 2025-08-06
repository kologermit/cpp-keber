from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CPPKeberRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("libuuid/1.0.3")
        self.requires("cpp-httplib/0.20.1")
        self.requires("libpqxx/7.10.1")
        self.requires("nlohmann_json/3.12.0")
        self.requires("fmt/11.2.0")
        self.requires("spdlog/1.15.3")
        self.requires("cpp-datetime-library/1.0.2")
        self.requires("openssl/3.5.2")

    def layout(self):
        cmake_layout(self)