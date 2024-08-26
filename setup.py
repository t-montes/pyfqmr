from setuptools import setup
from setuptools.extension import Extension

import numpy as np

# setup file based on:  
# https://github.com/AshleySetter/HowToPackageCythonAndCppFuncs


# load version
with open("VERSION", 'r') as f_vers:
    version = f_vers.read()

# load readme
with open('README.rst') as f_read:
    readme = f_read.read()

extensions = [
    Extension(
    name         = "pyfqmr.Simplify",        # name/path of generated .so file
    sources      = ["pyfqmr/Simplify.pyx"],  # cython generated cpp file
    include_dirs = [ np.get_include() ],    # ensure numpy can find headers
    language     = "c++"),                  # tells python that the language of the extension is c++
    ]

setup(
    name        = "pyfqmr_withuvs",
    version     = version,
    description = "cython wrapper around C++ library for fast triangular mesh reduction",
    author      = "kramer84",
    url         = "https://github.com/t-montes/pyfqmr",
    license     = 'MIT',
    include_package_data = True,
    packages = 
        [
        'pyfqmr'
        ],
    ext_modules      = extensions,
    long_description = readme,
    install_requires = [],
    classifiers = 
        [
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Science/Research",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Topic :: Scientific/Engineering",
        "License :: OSI Approved :: MIT License",
        "Natural Language :: English"
        ],
    )



