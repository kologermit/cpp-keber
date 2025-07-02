from conan import ConanFile
from conan.tools.cmake import cmake_layout


class ExampleRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("sqlpp11/0.64")
        self.requires("crowcpp-crow/1.2.0")
        self.requires("fmt/11.2.0")
        self.requires("hiredis/1.2.0")
        self.requires("spdlog/1.15.3")
        self.requires("libcurl/8.12.1")

    def layout(self):
        cmake_layout(self)