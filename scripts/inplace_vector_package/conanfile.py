from conan import ConanFile
from conan.tools.files import copy, get
from conan.tools.layout import basic_layout
import os

required_conan_version = ">=2.12"

class InplaceVectorConan(ConanFile):
    name = "inplace_vector"
    description = "A dynamically-resizable vector with fixed capacity and embedded storage (P0843)."
    license = "Apache License v2.0"
    exports_sources = "include/*"
    package_type = "header-library"
    no_copy_source = True

    def source(self):
        get(self, **self.conan_data["sources"][self.version], destination=self.source_folder, strip_root=True)

    def package(self):
        copy(self, "*.hpp", self.source_folder, self.package_folder)
        
    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []

    def layout(self):
        basic_layout(self, src_folder=".")
