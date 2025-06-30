from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class CppTutConan(ConanFile):
    name = "cpp_tut"
    version = "1.0"
    package_type = "application"

    # Optional metadata
    license = "MIT"
    author = "Your Name <your.email@example.com>"
    url = "https://github.com/yourusername/cpp_tut"
    description = "A C++ tutorial project"
    topics = ("c++", "tutorial", "cmake")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*", "tests/*"

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        # Add common C++ dependencies here
        # Examples:
        # self.requires("fmt/9.1.0")
        # self.requires("spdlog/1.12.0")
        # self.requires("catch2/3.4.0")
        pass

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.23]")
        pass

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()