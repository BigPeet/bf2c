#!/usr/bin/env python3

from setuptools import setup, Extension
import os

bf2c_include_dir = os.environ.get("BF2C_INCLUDE_DIR", "")
bf2c_lib_dir = os.environ.get("BF2C_LIB_DIR", "")
core_include_dir = os.environ.get("CORE_INCLUDE_DIR", "")
core_lib_dir = os.environ.get("CORE_LIB_DIR", "")
bf2c_version = os.environ.get("BF2C_VERSION", "")

ext_modules = [
    Extension(
        name="bf2c.py_bf2c",
        sources=["src/bf2c/py_bf2c.c"],
        include_dirs=[bf2c_include_dir, core_include_dir],
        library_dirs=[bf2c_lib_dir, core_lib_dir],
        libraries=["bf2c_lib", "core"],
        extra_compile_args=["-O3", "-std=c99"],
    )
]

setup(
    name="bf2c",
    version=bf2c_version,
    description="A Brainfuck to C transpiler with Python bindings",
    package_dir={"": "src"},
    packages=["bf2c"],
    ext_modules=ext_modules,
)
