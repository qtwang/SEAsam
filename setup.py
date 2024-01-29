'''
SEAsam -- Shape-based sampling for large time series collections.

This code is open-sourced at https://github.com/qtwang/SEAsam.
Supplemental materials can be found at https://qtwang.github.io/kdd21-seanet.

If you find this code useful in your work, please consider citing:
Qitong Wang, Themis Palpanas: Deep Learning Embeddings for Data Series
Similarity Search. KDD 2021: 1708-1716.
BibTeX record can be found at
https://dblp.org/rec/conf/kdd/WangP21.html?view=bibtex.


Copyright (C) 2021 - Qitong Wang <qitong.wang@etu.u-paris.fr> and Themis
Palpanas <themis@mi.parisdescartes.fr>, Université Paris Cité, France.

This program is free software; you can redistribute it and/or modify
it under the terms of the Mozilla Public License Version 2.0 as published by
the Mozilla Foundation. If a copy of the MPL was not distributed with this
file, you can obtain one at https://mozilla.org/MPL/2.0/.
'''

from setuptools import setup, Extension, Command
import os

extension_mod = Extension('seasam_c_ext',
                          sources=['src/seasam.c',
                                   'src/seasam_wrapper.c'],
                          include_dirs=['include'],
                          define_macros=[('PYTHON_MODULE', None)])


class BuildSharedLib(Command):
    description = "build SEAsam into a shared library"
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        os.system(
            'gcc -shared -o libseasam.so -fPIC src/seasam.c -I include')


class BuildPythonModule(Command):
    description = "build SEAsam into a Python extension module"
    user_options = []

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        os.system('python setup.py build_ext --inplace')


setup(
    name='SEAsam',
    packages=['seasam'],
    version='0.1',
    description='Shape-based sampling for large time series collections',
    url='https://github.com/qtwang/SEAsam',
    author='Qitong Wang, Themis Palpanas',
    author_email='qitong.wang@etu.u-paris.fr',
    ext_modules=[extension_mod],
    cmdclass={
        'build_lib': BuildSharedLib,
        'build_py_mod': BuildPythonModule,
    }
)
