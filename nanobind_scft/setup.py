# from setuptools import setup, Extension
# from Cython.Build import cythonize
# import numpy as np

# extensions = [
#     Extension(
#         "rho_q",                # Name of the resulting .so file
#         ["rho_q.pyx"],          # Our Cython source file
#         include_dirs=[np.get_include()],  # Ensure numpy headers are included
#     )
# ]

# setup(
#     ext_modules=cythonize(extensions, language_level="3"),
# )

# setup.py
from setuptools import setup, Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "rho_q_wrapper",
        sources=["rho_q_wrapper.pyx", "rho_q.cpp"],  # Cython file and C++ source file
        language="c++",  # Specifies that the source code is C++
    )
]

setup(
    name="rho_q_wrapper",
    ext_modules=cythonize(ext_modules),
)