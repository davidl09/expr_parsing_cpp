from conans import CMake, ConanFile

class ExpressionParser(ConanFile):
    name = "expression-parser"
    author = "David Laeer"
    url = "https://github.com/davidl09/expr_parsing_cpp"
    description = "A math expression parser"
    settings = "os", "compiler", "build_type", "arch"
    requires = ()
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {
        "shared": False,
        "fPIC": True,
        "gtest:shared": False,
    }
    scm = {"type": "git", "url": "auto", "revision": "auto", "submodule": "recursive"}
    generators = "cmake_find_package"

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=self.source_folder, build_folder=self.build_folder)
        return cmake

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build_requirements(self):
        self.build_requires("gtest/1.12.1")

    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
