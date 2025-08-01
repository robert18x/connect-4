from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMakeDeps, cmake_layout, CMake

required_conan_version = ">2"

class Connect4Conan(ConanFile):
    name = "connect-4"
    version = "0.1"
    description = ""
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "fmt/11.2.0",
        "magic_enum/0.9.7",
        "inplace_vector/0.1",
    ]
    options = {
        "build_gui": [True, False],
        "build_tests": [True, False],
    }
    default_options = {
        "build_gui": True,
        "build_tests": True,
    }
    generators = "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def configure(self):
        if self.options.build_gui:
            self._configure_qt()

    def requirements(self):
        if self.options.build_gui:
            self.requires("qt/6.7.3")

        if self.options.build_tests:
            self.requires("snitch/1.2.3")

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CMAKE_BUILD_TYPE"] = self.settings.build_type
        tc.variables["BUILD_GUI"] = self.options.build_gui
        tc.variables["BUILD_TESTING"] = self.options.build_tests

        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def _configure_qt(self):
        self.options["qt"].shared = True
        self.options["qt"].gui = True
        self.options["qt"].widgets = True
        self.options["qt"].qttranslations = True
        self.options["qt"].qttools = True
        self.options["qt"].with_pcre2 = True
        self.options["qt"].with_doubleconversion = True


        # self.options["qt"].opengl = 'no'
        # self.options["qt"].with_vulkan = False
        # self.options["qt"].openssl = False
        # self.options["qt"].with_glib = False
        # self.options["qt"].with_libjpeg = False
        # self.options["qt"].with_icu = False
        # self.options["qt"].with_odbc = False
        # self.options["qt"].with_harfbuzz = False
        # self.options["qt"].with_libpng = False
        # self.options["qt"].with_sqlite3 = False
        # self.options["qt"].with_mysql = False
        # self.options["qt"].with_pq = False
        # self.options["qt"].with_odbc = False
        # self.options["qt"].with_zstd = False
        # self.options["qt"].with_brotli = False
        # self.options["qt"].with_dbus = False
        # self.options["qt"].with_libalsa = False
        # self.options["qt"].with_openal = False
        # self.options["qt"].with_gstreamer = False
        # self.options["qt"].with_pulseaudio = False
        # self.options["qt"].with_gssapi = False
        # self.options["qt"].with_md4c = False
        # self.options["qt"].with_x11 = False
        # self.options["qt"].with_egl = False
        # self.options["qt"].multiconfiguration = False
        # self.options["qt"].qtsvg = False
        # self.options["qt"].qtwayland = False
        # self.options["qt"].qtquickcontrols2 = False
        # self.options["qt"].qtquicktimeline = False
        # self.options["qt"].qtquick3d = False
        # self.options["qt"].qtshadertools = False
        # self.options["qt"].qt5compat = False
        # self.options["qt"].qtactiveqt = False
        # self.options["qt"].qtcharts = False
        # self.options["qt"].qtdatavis3d = False
        # self.options["qt"].qtlottie = False
        # self.options["qt"].qtscxml = False
        # self.options["qt"].qtvirtualkeyboard = False
        # self.options["qt"].qt3d = False
        # self.options["qt"].qtimageformats = False
        # self.options["qt"].qtnetworkauth = False
        # self.options["qt"].qtcoap = False
        # self.options["qt"].qtmqtt = False
        # self.options["qt"].qtopcua = False
        # self.options["qt"].qtmultimedia = False
        # self.options["qt"].qtlocation = False
        # self.options["qt"].qtsensors = False
        # self.options["qt"].qtconnectivity = False
        # self.options["qt"].qtserialbus = False
        # self.options["qt"].qtserialport = False
        # self.options["qt"].qtwebsockets = False
        # self.options["qt"].qtwebchannel = False
        # self.options["qt"].qtwebengine = False
        # self.options["qt"].qtwebview = False
        # self.options["qt"].qtremoteobjects = False
        # self.options["qt"].qtpositioning = False
        # self.options["qt"].qtlanguageserver = False
        # self.options["qt"].qtspeech = False
        # self.options["qt"].qthttpserver = False
        # self.options["qt"].qtquick3dphysics = False
        # self.options["qt"].qtgrpc = False
        # self.options["qt"].qtquickeffectmaker = False
        # self.options["qt"].qtgraphs = False
        # self.options["qt"].essential_modules = False
        # self.options["qt"].addon_modules = False
        # self.options["qt"].deprecated_modules = False
        # self.options["qt"].preview_modules = False
