"""
NanoBrain Python Package Setup

Install with: pip install -e .
"""

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import os

__version__ = "1.0.0"

class get_pybind_include:
    """Helper to get pybind11 include path."""
    def __str__(self):
        import pybind11
        return pybind11.get_include()

# Source files for the extension
cpp_sources = [
    "bindings.cpp",
]

# Include directories
include_dirs = [
    get_pybind_include(),
    "../src/cpp",
]

# GGML path - can be overridden via environment variable
ggml_path = os.environ.get("GGML_PATH", "../ggml")
if os.path.exists(ggml_path):
    include_dirs.append(os.path.join(ggml_path, "include"))
    include_dirs.append(ggml_path)

# Extension definition
ext_modules = [
    Extension(
        "nanobrain._core",
        cpp_sources,
        include_dirs=include_dirs,
        language="c++",
        extra_compile_args=["-std=c++17", "-O3", "-fvisibility=hidden"],
        define_macros=[("VERSION_INFO", __version__)],
    ),
]

class BuildExt(build_ext):
    """Custom build extension for adding compiler-specific options."""
    
    c_opts = {
        "msvc": ["/EHsc", "/std:c++17", "/O2"],
        "unix": ["-std=c++17", "-O3", "-fvisibility=hidden"],
    }
    
    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        
        for ext in self.extensions:
            ext.extra_compile_args = opts
        
        build_ext.build_extensions(self)


setup(
    name="nanobrain",
    version=__version__,
    author="NanoBrain Team",
    author_email="nanobrain@example.com",
    description="Python bindings for NanoBrain cognitive architecture",
    long_description=open("README.md").read() if os.path.exists("README.md") else "",
    long_description_content_type="text/markdown",
    url="https://github.com/nanobrain/nanobrain",
    packages=["nanobrain"],
    ext_modules=ext_modules,
    cmdclass={"build_ext": BuildExt},
    install_requires=[
        "numpy>=1.19.0",
        "pybind11>=2.6.0",
    ],
    extras_require={
        "dev": [
            "pytest>=6.0",
            "pytest-cov",
        ],
    },
    python_requires=">=3.8",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: C++",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
    ],
    keywords="cognitive architecture, neural-symbolic, time crystal, ppm",
)
