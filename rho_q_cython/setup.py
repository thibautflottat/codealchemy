# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy as np

extensions = [
    Extension(
        "rho_q",
        sources=["rho_q.pyx", "rho_q_cpp.cpp"],  # Cython and your own C++ code
        depends=["rho_q_cpp.hpp"],  # Any additional dependencies
        language="c++",
        extra_compile_args=["-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION"],
        include_dirs=[np.get_include()],
    )
]

setup(
    ext_modules=cythonize(extensions),
)